#!/usr/bin/env python3
"""grid.py PREFIX: write variants of build/ea98/H1.c (W1a loop, b1 hole, alias forced store, bb8 free ref)."""
import itertools, sys
base = open('build/ea98/H1.c').read()
FR = '*(u8 *)&D_800E6280.unk_F5F * 0'
FE = '(*(u8 *)&D_80145FE4)'
def rep(s, a, b):
    assert a in s, a
    return s.replace(a, b, 1)
names = []
for k2, p24, taut in itertools.product([0, 1], ['nn', 'an', 'na'], ['', 'b', 'bb', 'f']):
    s = base
    if k2:
        s = rep(s, '(D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) != 9)', '((D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) + %s) != 9)' % FR)
    if p24[0] == 'a':
        s = rep(s, 'D_80146058 = D_800E6280.unk_F5F;', 'D_80146058 = D_800E71DF;')
    if p24[1] == 'a':
        s = rep(s, 'func_80051A68(*(u8 *)&D_800E6280.unk_F5F)', 'func_80051A68(D_800E71DF)')
    cond = '(%s == 7 || %s == 9) && (*(u8 *)&D_80146054) != %s' % (FE, FE, FE)
    if taut == 'b':
        s = rep(s, cond, '(%s == 7 || %s == 9) && %s >= 0 && (*(u8 *)&D_80146054) != %s' % (FE, FE, FE, FE))
    elif taut == 'bb':
        s = rep(s, cond, '(%s == 7 || %s == 9) && %s >= 0 && D_80145FE4 >= 0 && (*(u8 *)&D_80146054) != %s' % (FE, FE, FE, FE))
    elif taut == 'f':
        s = rep(s, '        D_800E71DF = (*(u8 *)&D_80145FE4);', '        ((u8 *)&D_800E6280.unk_F5F)[status * 0] = (*(u8 *)&D_80145FE4);')
    n = '%s%d%s%s' % (sys.argv[1], k2, p24, taut)
    open('build/ea98/%s.c' % n, 'w').write(s)
    names.append(n)
print(' '.join(names))
