#!/usr/bin/env python3
"""Scratch harness (T-25301, not committed): splice a body into 80139740.c, compile, diff func_8013AF78.

usage: python3 build/t25301/cmp.py BODY.c [-q]
"""
import difflib, re, subprocess, sys, os

SRC = "src/ovl/NAME_ENT/80139740.c"
ASM = "asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740/func_8013AF78.s"
FN = "func_8013AF78"
LINE = 'INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740", func_8013AF78);'


def target():
    rows = []
    for ln in open(ASM):
        m = re.match(r"\s*/\* [0-9A-F]+ ([0-9A-F]+) ([0-9A-F]{8}) \*/\s+(\S+)\s*(.*)", ln)
        if m:
            rows.append((int(m.group(1), 16), m.group(3), m.group(4).replace(" ", "")))
    base = rows[0][0]
    out = []
    for addr, op, args in rows:
        args = re.sub(r"\.L([0-9A-F]+)", lambda mm: "@%x" % (int(mm.group(1), 16) - base), args)
        args = re.sub(r"%hi\(D_([0-9A-F]{8})\)", lambda mm: "%%hi(0x%x)" % (((int(mm.group(1), 16) + 0x8000) >> 16) & 0xFFFF), args)
        out.append("%s %s" % (op, args))
    return out


def built(obj):
    txt = subprocess.run(["mips-linux-gnu-objdump", "-drz", "-M", "no-aliases",
                          "--disassemble=" + FN, obj], capture_output=True, text=True).stdout
    ins = []
    start = None
    for ln in txt.splitlines():
        m = re.match(r"\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(\S+)\s*(.*)", ln)
        if m:
            a = int(m.group(1), 16)
            if start is None:
                start = a
            ins.append([a - start, m.group(3), m.group(4).replace(" ", ""), None])
            continue
        m = re.match(r"\s*[0-9a-f]+:\s+(R_MIPS_\w+)\s+(\S+)", ln)
        if m and ins:
            ins[-1][3] = (m.group(1), m.group(2))
    out = []
    for off, op, args, rel in ins:
        args = re.sub(r"[0-9a-f]+<" + FN + r"(\+0x([0-9a-f]+))?>",
                      lambda mm: "@%x" % (int(mm.group(2) or "0", 16)), args)
        if rel:
            kind, sym = rel
            sym = re.sub(r"\+0x0$", "", sym)
            sv = int(sym[2:10], 16) if re.match(r"[DL]_[0-9A-Fa-f]{8}$", sym) else None
            if kind == "R_MIPS_HI16":
                if sv is None:
                    args = re.sub(r",(0x[0-9a-f]+|\d+)$", ",%%hi(%s)" % sym, args)
                else:
                    args = re.sub(r",(0x[0-9a-f]+|\d+)$", lambda mm: ",%%hi(0x%x)" % (((sv + (int(mm.group(1), 0) << 16) + 0x8000) >> 16) & 0xFFFF), args)
            elif kind == "R_MIPS_LO16":
                def lo(mm):
                    if sv is None:
                        return ",%%lo(%s)%s" % (sym, mm.group(2))
                    v = int(mm.group(1), 0)
                    if v >= 0x8000:
                        v -= 0x10000
                    return ",%%lo(D_%08X)%s" % (sv + v, mm.group(2))
                args = re.sub(r",(-?0x[0-9a-f]+|-?\d+)(\(\w+\))", lo, args)
        args = args.replace("zero", "zero")
        out.append("%s %s" % (op, args))
    while out and out[-1] == "nop ":
        out.pop()
    return out


def norm(s):
    s = re.sub(r"\$", "", s)
    s = re.sub(r"^beqz zero,", "b ", s)
    s = re.sub(r"^beq zero,zero,", "b ", s)
    s = re.sub(r"0x([0-9A-Fa-f]+)", lambda m: str(int(m.group(1), 16)), s)
    s = re.sub(r"-0x([0-9A-Fa-f]+)", lambda m: str(-int(m.group(1), 16)), s)
    return s.lower()


def main():
    body = open(sys.argv[1]).read()
    quiet = "-q" in sys.argv
    text = open(SRC).read()
    assert LINE in text
    os.makedirs("build/t25301/o", exist_ok=True)
    tag = os.path.splitext(os.path.basename(sys.argv[1]))[0]
    c = "build/t25301/o/%s.c" % tag
    o = "build/t25301/o/%s.o" % tag
    open(c, "w").write(text.replace(LINE, body))
    r = subprocess.run(["python3", "tools/cc.py", c, o, "ido", "5.3"], capture_output=True, text=True)
    if r.returncode:
        print("COMPILE FAIL", (r.stderr + r.stdout)[-1500:])
        sys.exit(1)
    t = [norm(x) for x in target()]
    b = [norm(x) for x in built(o)]
    diff = [d for d in difflib.unified_diff(t, b, "target", "built", n=0, lineterm="") if d[:1] in "+-" and not d.startswith(("---", "+++"))]
    print("%s: words target %d built %d rows %d" % (tag, len(t), len(b), len(diff)))
    if "--entry" in sys.argv:
        seq = [x for x in b[7:24]]
        print("   " + " ; ".join(seq))
        return
    if not quiet:
        sm = difflib.SequenceMatcher(None, t, b, autojunk=False)
        for i, (x, y) in enumerate(zip(t, b + [""] * max(0, len(t) - len(b)))):
            pass
        w = max(len(x) for x in t) + 2
        for tag2, i1, i2, j1, j2 in sm.get_opcodes():
            for k in range(max(i2 - i1, j2 - j1)):
                x = t[i1 + k] if i1 + k < i2 else ""
                y = b[j1 + k] if j1 + k < j2 else ""
                mark = " " if tag2 == "equal" else "|"
                print("%3d %-*s %s %s" % (i1 + k, w, x, mark, y))


main()
