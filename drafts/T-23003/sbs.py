#!/usr/bin/env python3
"""sbs.py MINE.dis ORIG.s A B [OFF] : side by side listing of original instructions A..B-1 against mine (index + OFF).

MINE.dis is `mips-linux-gnu-objdump -dz` of the scratch object of one function, ORIG.s the splat file
asm/nonmatchings/<unit>/<file>/<func>.s (read at run time only; nothing from it is stored). T-23003.
"""
import re
import sys

mine_path, orig_path = sys.argv[1], sys.argv[2]
a, b = int(sys.argv[3]), int(sys.argv[4])
off = int(sys.argv[5]) if len(sys.argv) > 5 else 0
orig = []
for line in open(orig_path):
    m = re.match(r'\s*/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/\s+(.*)', line)
    if m:
        orig.append(m.group(2).strip()[:46])
mine = []
for line in open(mine_path):
    m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
    if m:
        mine.append(m.group(3).strip()[:46].replace('\t', ' '))
for i in range(a, b):
    print('%4d %-46s | %s' % (i, orig[i] if i < len(orig) else '', mine[i + off] if 0 <= i + off < len(mine) else ''))
