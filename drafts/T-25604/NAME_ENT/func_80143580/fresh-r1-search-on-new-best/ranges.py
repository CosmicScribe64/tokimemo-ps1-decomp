"""T-25604 scratch: list ranges of one procedure in a traced-uopt log with colour, priority, blocks.
    python3 ranges.py draft.log [COLOUR ...]     (COLOUR as v0..ra; none = all coloured ranges)"""
import sys
from collections import defaultdict
COL = {1:'v0',2:'v1',3:'a0',4:'a1',5:'a2',6:'a3',7:'t0',8:'t1',9:'t2',10:'t3',11:'t4',12:'t5',13:'ra'}
inproc = False; cur = None; units = defaultdict(list); prio = {}; colour = {}; order = []
for line in open(sys.argv[1]):
    if line.startswith('R9PROC'):
        inproc = line.split()[1] == 'func_80143580'; continue
    if not inproc: continue
    f = dict(kv.split('=', 1) for kv in line.split()[1:] if '=' in kv)
    if line.startswith('R9SAVE'):
        cur = (int(f['bit']), int(f['lr'])); prio[cur] = float(f['priority'])
    elif line.startswith('R9UNIT') and cur:
        units[cur].append((int(f['bb']), int(f['loads']), int(f['stores'])))
    elif line.startswith('R9COST'):
        k = (int(f['bit']), int(f['lr'])); colour[k] = COL.get(int(f['reg']), f['reg']); order.append(k)
blk = int(sys.argv[sys.argv.index('--block') + 1]) if '--block' in sys.argv else None
want = set(a for a in sys.argv[2:] if not a[0].isdigit() and a != '--block')
for n, k in enumerate(order):
    if want and colour[k] not in want: continue
    if blk is not None and blk not in [b for b, _, _ in units[k]]: continue
    print('#%-3d %-3s bit=%d lr=%d prio=%g units=%s' % (n + 1, colour[k], k[0], k[1], prio[k],
          ' '.join('%d%s' % (b, '' if (l, s) == (0, 0) else '(%d/%d)' % (l, s)) for b, l, s in units[k])))
# usage 2: python3 ranges.py draft.log --block BB   -> every range with a unit in block BB
