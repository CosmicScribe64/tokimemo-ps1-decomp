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


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--file", default="")
    ap.add_argument("--skip", default="80080000-80086810")
    ap.add_argument("--limit", type=int, default=0)
    a = ap.parse_args()
    lo, hi = (int(x, 16) for x in a.skip.split("-"))
    rows = []
    for c in sorted(Path("src/main").glob("*.c")):
        if a.file not in c.stem:
            continue
        for folder, n in re.findall(r'^\s*INCLUDE_ASM\(\s*"([^"]+)"\s*,\s*(\w+)\s*\)', c.read_text(), re.M):
            p = Path(folder) / (n + ".s")
            if not p.exists():
                continue
            t = p.read_text()
            m = re.search(r'^nonmatching\s+\w+,\s*(0x[0-9A-Fa-f]+|\d+)', t, re.M)
            size = int(m.group(1), 0) if m else 0
            am = re.match(r'func_([0-9A-Fa-f]{8})$', n)
            if am and lo <= int(am.group(1), 16) < hi:
                continue
            if re.search(r'\bjalr?\b', t):
                continue
            rows.append((size, n, c.stem))
    rows.sort()
    for s, n, f in rows[:a.limit or None]:
        print("%d %s %s" % (s, n, f))
    print("total leaves: %d" % len(rows), file=sys.stderr)


if __name__ == "__main__":
    sys.exit(main())
