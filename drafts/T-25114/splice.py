#!/usr/bin/env python3
"""Put a body file into the NON_MATCHING guard of func_80154DD8 (T-25114 helper).
Usage: python3 drafts/T-25114/splice.py body.c      (extract: --get out.c)"""
import sys
p = 'src/ovl/RPG_BAT/80154430.c'
s = open(p).read()
tail = '#else\nINCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80154DD8);'
e = s.index(tail)
b = s.rindex('#ifdef NON_MATCHING\n', 0, e) + len('#ifdef NON_MATCHING\n')
if sys.argv[1] == '--get':
    open(sys.argv[2], 'w').write(s[b:e])
else:
    body = open(sys.argv[1]).read()
    if not body.endswith('\n'):
        body += '\n'
    open(p, 'w').write(s[:b] + body + s[e:])
