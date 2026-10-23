#!/usr/bin/env python3
"""usage: python3 mkfull.py BODY.c OUT.c -- writes src/main/80047550.c with BODY.c as the guarded draft
of func_80047560 (for regdiff --src)."""
import sys
from pathlib import Path
NAME = "func_80047560"
base = Path("src/main/80047550.c").read_text()
inc = 'INCLUDE_ASM("asm/nonmatchings/main/80047550", %s);\n' % NAME
idx = base.find(inc)
start = base.rfind("#ifdef NON_MATCHING\n", 0, idx)
if start >= 0 and "#endif" not in base[start:idx]:
    ls = base.rfind("\n", 0, start - 1) + 1
    if base[ls:start].startswith("/* NON_MATCHING"):
        start = ls
    end = idx + len(inc) + len("#endif\n")
else:
    start, end = idx, idx + len(inc)
body = Path(sys.argv[1]).read_text().rstrip("\n")
Path(sys.argv[2]).write_text(base[:start] + "#ifdef NON_MATCHING\n" + body + "\n#else\n" + inc + "#endif\n" + base[end:])
