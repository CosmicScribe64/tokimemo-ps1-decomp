#!/usr/bin/env python3
"""List remaining INCLUDE_ASM leaf functions (no jal/jalr) of the game code, smallest first.

Usage: python3 tools/list_leaves.py [--file 80079B10] [--skip LO-HI] [--limit N]
Reads src/main/*.c (T-0012: one file per original object, named by start address) and the
asm/nonmatchings/main/<addr>/*.s each INCLUDE_ASM points at; prints "size name file" lines.
--file restricts to files whose name contains the text; --skip takes a hex address range
(default 80080000-80086810).
"""
import argparse
import re
import sys
from pathlib import Path

import srcscan


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--file", default="")
    ap.add_argument("--skip", default="80080000-80086810")
    ap.add_argument("--limit", type=int, default=0)
    a = ap.parse_args()
    lo, hi = (int(x, 16) for x in a.skip.split("-"))
    rows = []
    for c in srcscan.source_files():
        if a.file not in c.stem:
            continue
        for e in srcscan.include_asm_entries(c):
            p = Path(e.folder) / (e.name + ".s")
            if not p.exists():
                continue
            head = srcscan.nonmatching_size(p)
            am = re.match(r'func_([0-9A-Fa-f]{8})$', e.name)
            if am and lo <= int(am.group(1), 16) < hi:
                continue
            if re.search(r'\bjalr?\b', p.read_text()):
                continue
            rows.append((head[1] if head else 0, e.name, e.file))
    rows.sort()
    for size, name, file in rows[:a.limit or None]:
        print("%d %s %s" % (size, name, file))
    print("total leaves: %d" % len(rows), file=sys.stderr)


if __name__ == "__main__":
    sys.exit(main())
