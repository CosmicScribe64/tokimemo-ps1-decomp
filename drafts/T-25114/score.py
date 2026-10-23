#!/usr/bin/env python3
"""Fast row diff of the guarded C build of one RPG_BAT/80154430.c function against its .s (T-25114).

Usage (Docker): python3 drafts/T-25114/score.py [--src alt.c] [--func NAME] [-v]
Prints the number of differing rows (-/+ lines, like funcdiff) and with -v the rows.
Relocated immediates are masked on both sides; the relocation symbol is compared instead.
"""
import argparse
import difflib
import re
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, "tools")
import funcdiff_verify as proof  # noqa: E402
import srcscan  # noqa: E402
import protoverride  # noqa: E402

ap = argparse.ArgumentParser()
ap.add_argument("--src")
ap.add_argument("--func", default="func_80154DD8")
ap.add_argument("-v", action="store_true")
ap.add_argument("--proto", action="append", default=[])
ap.add_argument("--dump")
ap.add_argument("--list", help="A-B: print expected and built side by side for this index range")
a = ap.parse_args()

c = next(f for f in srcscan.unit_c_files("RPG_BAT") if f.src.name == "80154430.c")
out = Path("build/T-25114")
out.mkdir(parents=True, exist_ok=True)
with tempfile.TemporaryDirectory(dir=out) as tmp:
    try:
        obj = proof.compile_unit(c, Path(tmp), protos=protoverride.parse(a.proto) if a.proto else (), source=Path(a.src) if a.src else None)[0]
    except Exception as e:  # noqa: BLE001
        print("COMPILE FAIL", e)
        sys.exit(1)
    text = subprocess.run(["mips-linux-gnu-objdump", "-drz", str(obj)], capture_output=True, text=True,
                          check=True).stdout
start = text.index("<%s>:" % a.func)
end = text.find("\n\n", start + 1)
body = text[start:end if end > 0 else None]
if a.dump:
    Path(a.dump).write_text(body)

ours = []
for line in body.splitlines()[1:]:
    m = re.match(r"\s+[0-9a-f]+:\s+([0-9a-f]{8})\s+(.*)", line)
    if m:
        ours.append([int(m.group(1), 16), None, m.group(2).replace("\t", " ")])
        continue
    m = re.match(r"\s+[0-9a-f]+: (R_MIPS_\w+)\s+(\S+)", line)
    if m and ours:
        ours[-1][1] = (m.group(1), m.group(2))

exp = []
s = Path("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430/%s.s" % a.func).read_text()
for line in s.splitlines():
    m = re.match(r"\s*/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/\s+(.*)", line)
    if not m:
        continue
    b = bytes.fromhex(m.group(1))
    w = int.from_bytes(b, "little")
    ins = m.group(2)
    sym = None
    r = re.search(r"%(hi|lo)\((\w+)\)", ins)
    if r:
        sym = ("R_MIPS_HI16" if r.group(1) == "hi" else "R_MIPS_LO16", r.group(2))
    elif re.match(r"jal\s", ins):
        sym = ("R_MIPS_26", ins.split()[1])
    exp.append([w, sym, re.sub(r"\s+", " ", ins)])


def symaddr(name):
    m = re.match(r"(?:D|func)_([0-9A-F]{8})$", name)
    return int(m.group(1), 16) if m else None


def resolve(w, sym):
    """Final word of one of our relocated instructions, when the symbol name gives its address."""
    a_ = symaddr(sym[1])
    if a_ is None:
        return None
    if sym[0] == "R_MIPS_26":
        return (w & 0xFC000000) | ((a_ >> 2) & 0x3FFFFFF)
    lo = w & 0xFFFF
    lo = lo - 0x10000 if lo & 0x8000 else lo
    if sym[0] == "R_MIPS_LO16":
        return (w & 0xFFFF0000) | ((a_ + lo) & 0xFFFF)
    if sym[0] == "R_MIPS_HI16":
        return (w & 0xFFFF0000) | (((a_ + 0x8000) >> 16) & 0xFFFF)
    return None


def key(w, sym, final=False):
    if sym is None or final:
        return "%08x" % w
    if sym[0] == "R_MIPS_26":
        return "%08x %s" % (w & 0xFC000000, sym[1])
    return "%08x %s" % (w & 0xFFFF0000, sym[1])


ka = []
for w, s_, _ in exp:
    ka.append(key(w, s_, final=s_ is not None and symaddr(s_[1]) is not None))
kb = []
for w, s_, _ in ours:
    r_ = resolve(w, s_) if s_ else None
    kb.append(key(r_, s_, final=True) if r_ is not None else key(w, s_))
rows = 0
lines = []
for tag, i1, i2, j1, j2 in difflib.SequenceMatcher(None, ka, kb, autojunk=False).get_opcodes():
    if tag == "equal":
        continue
    for i in range(i1, i2):
        rows += 1
        lines.append("-%3d %s" % (i, exp[i][2]))
    for j in range(j1, j2):
        rows += 1
        lines.append("+%3d %s" % (j, ours[j][2]))
print("rows", rows, "words", len(exp), len(ours))
if a.v:
    print("\n".join(lines))

if a.list:
    lo, hi = (int(x) for x in a.list.split("-"))
    for i in range(lo, hi + 1):
        e_ = exp[i][2] if i < len(exp) else ""
        o_ = ours[i][2] if i < len(ours) else ""
        if i < len(ours) and ours[i][1]:
            o_ += " <" + ours[i][1][1] + ">"
        mark = " " if i < len(exp) and i < len(ours) and ka[i] == kb[i] else "*"
        print("%3d%s %-44s %s" % (i, mark, e_[:44], o_))
