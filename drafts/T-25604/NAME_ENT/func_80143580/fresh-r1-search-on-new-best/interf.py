"""List the live ranges that share a block with one range in a traced-uopt log (T-25604).

    python3 interf.py draft.log func_80143580 BIT [LR]

Reads the R9SAVE/R9UNIT/R9COST lines of one procedure (regdiff's draft.log) and prints each
range that has a live unit in a block of the given range, with its colour and the shared blocks.
"""
import sys
from collections import defaultdict
COL = {1:'v0',2:'v1',3:'a0',4:'a1',5:'a2',6:'a3',7:'t0',8:'t1',9:'t2',10:'t3',11:'t4',12:'t5',13:'ra'}
log, proc, bit = sys.argv[1], sys.argv[2], int(sys.argv[3])
lr = int(sys.argv[4]) if len(sys.argv) > 4 else bit
inproc = False; cur = None
units = defaultdict(set); prio = {}; colour = {}
for line in open(log):
    if line.startswith('R9PROC'):
        inproc = line.split()[1] == proc
        continue
    if not inproc:
        continue
    f = dict(kv.split('=', 1) for kv in line.split()[1:] if '=' in kv)
    if line.startswith('R9SAVE'):
        cur = (int(f['bit']), int(f['lr'])); prio[cur] = float(f['priority'])
    elif line.startswith('R9UNIT') and cur:
        units[cur].add(int(f['bb']))
    elif line.startswith('R9COST'):
        colour[(int(f['bit']), int(f['lr']))] = COL.get(int(f['reg']), f['reg'])
me = (bit, lr)
print('range', me, 'prio', prio.get(me), 'colour', colour.get(me), 'blocks', sorted(units[me]))
rows = []
for k, u in units.items():
    if k == me:
        continue
    s = u & units[me]
    if s:
        rows.append((colour.get(k, '-'), k, prio[k], sorted(s)))
for r in sorted(rows, key=lambda r: (r[0], -r[2])):
    print('  %-3s bit=%d lr=%d prio=%g shared=%s' % (r[0], r[1][0], r[1][1], r[2], r[3][:12]))
