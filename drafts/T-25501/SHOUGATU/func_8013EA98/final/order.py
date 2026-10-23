#!/usr/bin/env python3
"""order.py LABEL: colouring order of func_8013EA98's ranges from the uopt trace
(bit, register name, priority, units). Register numbers: 1..13 = v0 v1 a0 a1 a2 a3 t0..t5 ra."""
import sys, re
L = sys.argv[1]
log = open('build/ea98/%s/trace/draft.log' % L).read()
s = log.index('R9PROC func_8013EA98'); e = log.find('R9PROC', s + 10)
blk = log[s:e if e > 0 else None].splitlines()
names = 'v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 ra s0 s1 s2 s3 s4 s5 s6 s7 s8'.split()
pri = {}; units = {}; cur = None
for l in blk:
    if l.startswith('R9SAVE'):
        m = re.search(r'bit=(\d+) lr=(\d+) blocks=(\d+) priority=(\S+)', l)
        cur = (m.group(1), m.group(2)); pri[cur] = (float(m.group(4)), int(m.group(3))); units[cur] = []
    elif l.startswith('R9UNIT') and cur:
        m = re.search(r'bb=(\d+) freq=(\d+) loads=(\d+) stores=(\d+) load=(\d)', l)
        units[cur].append(m.group(1) + ('L' + m.group(3) if m.group(3) != '0' else '') + ('S' + m.group(4) if m.group(4) != '0' else '') + ('r' if m.group(5) == '1' else ''))
for l in blk:
    if l.startswith('R9COST'):
        m = re.search(r'bit=(\d+) lr=(\d+) cost=(\S+) reg=(\d+)', l)
        k = (m.group(1), m.group(2)); p = pri.get(k, ('?', '?'))
        print('%4s/%-4s %-3s cost=%-6s pri=%s D=%s %s' % (k[0], k[1], names[int(m.group(4)) - 1], m.group(3), p[0], p[1], ' '.join(units.get(k, [])[:6])))
# summary line: loop-range colouring order with labels from the preheader of the built code
import os
dis = open('build/ea98/%s/dis.txt' % L).read().splitlines()
ins = [l.split('\t')[-2:] for l in dis if re.match(r'^\s+[0-9a-f]+:\t[0-9a-f]{8} \t', l)]
lab = {}
for k, (op, args) in enumerate(ins[:70]):
    a = args.split(',')
    if op == 'li' and k > 30: lab[a[0]] = a[1]
    if op == 'addiu' and k < 30 and a[0] == a[1] and a[2] == '0' and a[0] not in ('sp',): lab.setdefault(a[0], 'base')
    if op == 'lbu' and 40 < k < 52: lab[a[0]] = '54'
seq = []
for l in blk:
    if l.startswith('R9COST'):
        m = re.search(r'bit=(\d+) lr=(\d+) cost=(\S+) reg=(\d+)', l)
        r = names[int(m.group(4)) - 1]; k = (m.group(1), m.group(2)); p = pri.get(k, ('?', '?'))
        nm = 'W' if m.group(1) == '24' else lab.get(r, '')
        if nm: seq.append('%s:%s(%.3g/%s)' % (nm, r, p[0], p[1]))
print('ORDER', ' '.join(seq))
