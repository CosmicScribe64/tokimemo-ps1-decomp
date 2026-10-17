#!/usr/bin/env python3
"""One-off rewrite of T-20930: fold ENDING's scalar data labels into the recovered aggregates.

For every `D_XXXXXXXX` inside one of the ranges below this rewrites the C uses in src/ovl/ENDING/*.c into
the element or field of the aggregate (D_8013C2CC -> D_8013C280[1][2], D_8013C664 -> D_8013C620[1].unk_00),
and deletes the scalar `extern` lines of include/ovl/ENDING.h (the aggregate declarations are written by
hand). tools/migrate_globals.py is not used: it rewrites every src file, and overlay addresses repeat
across overlays. Run from the repo root: python3 drafts/T-20930/fold_ending.py [--dry-run]
"""
import glob
import re
import sys

SCENE = ["unk_00", "unk_04", "unk_08", "unk_0C"]


def scene(off):
    if off < 0x0C:
        return "." + SCENE[off // 4]
    if off == 0x0C:
        return ".unk_0C"
    return ".unk_10[%d]" % ((off - 0x10) // 4)


def table(rowlen):
    return lambda off: "[%d][%d]" % (off // rowlen, (off % rowlen) // 4)


def portraits(off):
    if off < 0xA8:
        return ".%s[%d]" % (("unk_00", "unk_38", "unk_70")[off // 0x38], (off % 0x38) // 4)
    return ".unk_A8[%d].unk_00" % ((off - 0xA8) // 4)


RANGES = [
    (0x8013C280, 0x8013C34C, "D_8013C280", table(0x44)),
    (0x8013C34C, 0x8013C358, "D_8013C34C", lambda off: "." + SCENE[off // 4]),
    (0x8013C4C0, 0x8013C5A0, "D_8013C4C0", portraits),
    (0x8013C5A0, 0x8013C5BC, "D_8013C5A0", lambda off: "[%d]" % (off // 4)),
    (0x8013C5F0, 0x8013C620, "D_8013C5F0", scene),
    (0x8013C620, 0x8013CA1C, "D_8013C620", lambda off: "[%d]%s" % (off // 0x44, scene(off % 0x44))),
    (0x8013CAC0, 0x8013CB74, "D_8013CAC0", table(0x3C)),
    (0x8013CBB0, 0x8013CBE0, "D_8013CBB0", scene),
]


def expr(addr):
    for lo, hi, base, f in RANGES:
        if lo <= addr < hi:
            return base + f(addr - lo)
    return None


def main():
    dry = "--dry-run" in sys.argv
    sym = re.compile(r"\bD_(8013[0-9A-F]{4})\b")
    n = 0
    for p in sorted(glob.glob("src/ovl/ENDING/*.c")):
        s = open(p).read()
        t = s.replace("D_8013CAF0 = (s32 **) 0x801835E4;", "D_8013CAF0 = 0x801835E4;")
        t = sym.sub(lambda m: expr(int(m.group(1), 16)) or m.group(0), t)
        if t != s:
            n += sum(1 for m in sym.finditer(s) if expr(int(m.group(1), 16)))
            print("%s: rewritten" % p)
            if not dry:
                open(p, "w").write(t)
    h = "include/ovl/ENDING.h"
    s = open(h).read()
    decl = re.compile(r"^extern [^;\n]*\bD_(8013[0-9A-F]{4})\b[^;\n]*;[^\n]*\n", re.M)
    t = decl.sub(lambda m: "" if expr(int(m.group(1), 16)) else m.group(0), s)
    print("%d uses rewritten, %d header lines removed" % (n, len(decl.findall(s)) - len(decl.findall(t))))
    if not dry:
        open(h, "w").write(t)


if __name__ == "__main__":
    main()
