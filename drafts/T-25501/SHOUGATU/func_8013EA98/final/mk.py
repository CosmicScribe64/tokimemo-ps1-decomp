#!/usr/bin/env python3
"""mk.py BASE OUT [OLD=>NEW ...]: copy build/ea98/BASE.c to build/ea98/OUT.c with literal replacements (each must apply)."""
import sys
s = open('build/ea98/%s.c' % sys.argv[1]).read()
for a in sys.argv[3:]:
    old, new = a.split('=>', 1)
    assert old in s, (sys.argv[2], old)
    s = s.replace(old, new, 1)
open('build/ea98/%s.c' % sys.argv[2], 'w').write(s)
