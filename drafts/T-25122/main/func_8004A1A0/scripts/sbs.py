#!/usr/bin/env python3
"""Side-by-side of expected vs NON_MATCHING-built func (scratch helper, T-25122).
Usage (docker, repo root): python3 sbs.py FUNC [src.c]"""
import sys, subprocess, tempfile, re, difflib
from pathlib import Path
sys.path.insert(0, "tools")
import funcloc, funcdiff_verify as proof

name = sys.argv[1]
src = None
c = funcloc.locate(name, "main")
if len(sys.argv) > 2:
    body = open(sys.argv[2]).read()
    text = open(c.src).read()
    i = text.index("#ifdef NON_MATCHING\nvoid " + name)
    j = text.index("#else", i)
    text = text[:i] + "#ifdef NON_MATCHING\n" + body + text[j:]
    src = Path(tempfile.mkdtemp()) / Path(c.src).name
    src.write_text(text)
quiet = "-q" in sys.argv
d = Path(tempfile.mkdtemp())
obj, _ = proof.compile_unit(c, d, source=src)

def dis(o):
    out = subprocess.run(["mips-linux-gnu-objdump", "-d", "--no-show-raw-insn", str(o)],
                         capture_output=True, text=True).stdout
    lines, on = [], False
    for l in out.splitlines():
        if re.match(r"^[0-9a-f]+ <%s>:" % name, l):
            on = True; continue
        if on and re.match(r"^[0-9a-f]+ <", l):
            break
        if on:
            m = re.match(r"^\s*[0-9a-f]+:\s+(.*)$", l)
            if m and m.group(1).startswith("R_MIPS"):
                pass
            elif m:
                lines.append(re.sub(r"\s+", " ", m.group(1)).strip())
            elif "R_MIPS" in l and lines:
                lines[-1] += "  [" + l.split()[-1] + "]"
    return lines

e = dis("expected/build/src/main/80049FF0.o")
b = dis(obj)
n = max(len(e), len(b))
for i in range(n if not quiet else 0):
    x = e[i] if i < len(e) else ""
    y = b[i] if i < len(b) else ""
    mark = "  " if x == y else "!!"
    print("%3d %s %-48s | %s" % (i, mark, x[:48], y[:60]))
print("len", len(e), len(b), "equal-pos", sum(1 for i in range(min(len(e), len(b))) if e[i] == b[i]))
