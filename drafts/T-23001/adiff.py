#!/usr/bin/env python3
"""aligned diff of guarded C vs original for one function.  usage: adiff.py UNIT:func [--ctx N] [--max H] [--regs]"""
import sys, re, difflib, tempfile, os
sys.path.insert(0, 'tools')
import funcdiff as fd, funcloc
import funcdiff_verify as proof
from pathlib import Path
args = sys.argv[1:]
if '--win' in args:
    i = args.index('--win'); extra = args[i:]; args = args[:i]
ctx = 2; maxh = 400; showregs = False
if '--ctx' in args: i = args.index('--ctx'); ctx = int(args[i+1]); del args[i:i+2]
if '--max' in args: i = args.index('--max'); maxh = int(args[i+1]); del args[i:i+2]
if '--regs' in args: showregs = True; args.remove('--regs')
unit, name = args[0].split(':')
c = fd.locate(name, unit)
job = Path(tempfile.mkdtemp(dir='/tmp'))
b = proof.compile_guarded(c, job)
e, problem = fd.expected_object(c, False, stale=[])
want = fd.resolved_functions(str(e), [name], c)[name]
got = fd.resolved_functions(b, [name], c)[name]
def disp(r): return r[1] if isinstance(r, tuple) else r
def key(r): return r[0] if isinstance(r, tuple) else r
def mk(k):
    w = int(k[:8], 16); rest = k[8:]
    op = w >> 26
    if op == 0:
        w &= ~(0x1f << 21 | 0x1f << 16 | 0x1f << 11)
    elif op == 1:
        w &= ~(0x1f << 21 | 0xffff)
    elif op in (4, 5, 6, 7):
        w &= ~(0x1f << 21 | 0x1f << 16 | 0xffff)
    elif op in (2, 3):
        pass
    else:
        w &= ~(0x1f << 21 | 0x1f << 16)
    return '%08x%s' % (w, rest)
W = [mk(key(r)) for r in want]
G = [mk(key(r)) for r in got]
def d(r):
    s = disp(r)
    return re.sub(r'^[0-9a-f]{8}( R_MIPS\S+ \S+)?\s*;?\s*', '', s) if ';' in s else s
Wd = [d(r) for r in want]; Gd = [d(r) for r in got]
W2 = W; G2 = G
if '--win' in sys.argv:
    i = sys.argv.index('--win'); a0 = int(sys.argv[i+1]); a1 = int(sys.argv[i+2])
    print('ORIG'); [print(k, Wd[k]) for k in range(a0, a1)]
    b0 = int(sys.argv[i+3]) if len(sys.argv) > i+3 else a0
    print('BUILT'); [print(k, Gd[k]) for k in range(b0, b0 + (a1-a0) + 4)]
    sys.exit()
sm = difflib.SequenceMatcher(None, W2, G2, autojunk=False)
ops = [o for o in sm.get_opcodes() if o[0] != 'equal']
print('want %d got %d insns; %d differing hunks structural)' % (len(W), len(G), len(ops)))
n = 0
for op, i1, i2, j1, j2 in sm.get_opcodes():
    if op == 'equal': continue
    n += 1
    if n > maxh: break
    print('--- hunk %d  orig@%d..%d  built@%d..%d  (%s)' % (n, i1, i2, j1, j2, op))
    for k in range(max(0, i1-ctx), i1): print('   ', Wd[k])
    for k in range(i1, i2): print(' -', Wd[k])
    for k in range(j1, j2): print(' +', Gd[k])
    for k in range(i2, min(len(W), i2+ctx)): print('   ', Wd[k])
