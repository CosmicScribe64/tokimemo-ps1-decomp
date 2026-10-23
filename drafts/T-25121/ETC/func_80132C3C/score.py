#!/usr/bin/env python3
"""Fast single-function compile + diff for func_80132C3C (scratch helper, T-25121). Needs build/permute/func_80132C3C from `tools/permute.py setup`; run inside Docker with the scratch dir mounted.
Usage inside docker: python3 /scratch/fc.py BODY.c [BODY2.c ...]  (prints rows per body)
BODY.c holds the complete function definition (and optional extra top-level decls before it)."""
import difflib, os, re, subprocess, sys, tempfile

BASE = "/work/build/permute/func_80132C3C/base.c"
TARGET = "/work/build/permute/func_80132C3C/target.o"
FN = "func_80132C3C"


def dis(obj):
    out = subprocess.run(["mips-linux-gnu-objdump", "-d", "-r", "--no-show-raw-insn", "-M", "no-aliases", obj],
                         capture_output=True, text=True).stdout
    rows = []
    infn = False
    for line in out.splitlines():
        if re.match(r"^[0-9a-f]+ <%s>:" % FN, line):
            infn = True
            continue
        if infn and re.match(r"^[0-9a-f]+ <", line):
            break
        if not infn:
            continue
        m = re.match(r"^\s+[0-9a-f]+:\s+(.*)$", line)
        if m:
            ins = m.group(1).strip()
            if ins.startswith("R_MIPS"):
                if rows and "  ; " not in rows[-1]:
                    rows[-1] = rows[-1] + "  ; " + ins.split()[0]
                continue
            rows.append(ins)
    # strip trailing nops
    while rows and rows[-1] == "nop":
        rows.pop()
    # normalize jal targets/relocated immediates
    norm = []
    for r in rows:
        if "; " in r:
            ins, sym = r.split("  ; ")
            ins = re.sub(r",(-?0x[0-9a-f]+|-?\d+)(\(|$)", r",REL\2", ins)
            ins = re.sub(r"^(jal\s+)\S+", r"\1", ins)
            r = ins + " ; " + sym
        r = re.sub(r"^(b\w*\s+.*,|b\s+|j\s+)\s*[0-9a-f]+ <[^>]*>", r"\1BR", r)
        norm.append(r)
    return norm


def main():
    base = open(BASE).read()
    i = base.index("s32 %s(void) {" % FN)
    # find end of function: matching brace
    depth = 0
    j = base.index("{", i)
    k = j
    while True:
        c = base[k]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                break
        k += 1
    pre, post = base[:i], base[k + 1:]
    tgt = dis(TARGET)
    verbose = os.environ.get("V")
    for body in sys.argv[1:]:
        btxt = open(body).read()
        p2 = pre
        for ln in btxt.splitlines():
            if ln.startswith("//@ "):
                o, n = eval(ln[4:])
                assert o in p2, o
                p2 = p2.replace(o, n)
        src = p2 + btxt + post
        with tempfile.TemporaryDirectory() as td:
            c = os.path.join(td, "x.c")
            o = os.path.join(td, "x.o")
            open(c, "w").write(src)
            r = subprocess.run(["python3", "/scratch/ccwrap.py" if os.environ.get("UC") else "/work/tools/cc.py", c, o, "ido", "5.3"], capture_output=True, text=True, cwd="/work")
            if r.returncode:
                print(body, "COMPILE FAIL", r.stderr[-800:])
                continue
            got = dis(o)
        sm = difflib.SequenceMatcher(None, tgt, got, autojunk=False)
        rows = 0
        first = None
        lines = []
        for op, a1, a2, b1, b2 in sm.get_opcodes():
            if op == "equal":
                continue
            rows += max(a2 - a1, b2 - b1)
            if first is None:
                first = (a1, b1)
            for x in range(a1, a2):
                lines.append("- %3d %s" % (x, tgt[x]))
            for x in range(b1, b2):
                lines.append("+ %3d %s" % (x, got[x]))
        print("%s: rows=%d len=%d/%d first=%s" % (body, rows, len(got), len(tgt), first))
        if verbose:
            n = int(verbose)
            print("\n".join(lines[:n]))
        if os.environ.get("DUMP"):
            open(body + ".s", "w").write("\n".join(got) + "\n")
            open("/scratch/target.s", "w").write("\n".join(tgt) + "\n")


main()
