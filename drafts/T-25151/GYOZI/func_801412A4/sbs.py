import sys, os, tempfile, difflib, re
sys.path.insert(0, '/work/tools'); os.chdir('/work')
import funcdiff, funcdiff_verify as proof
from pathlib import Path
name = sys.argv[1]; unit = sys.argv[2]
c = funcdiff.locate(name, unit)
job = Path(tempfile.mkdtemp())
b = proof.compile_guarded(c, job)
e, prob = funcdiff.expected_object(c, False, stale=[])
got = funcdiff.resolved_functions(b, [name])[name]
want = funcdiff.resolved_functions(e, [name], c)[name]
def d(r): return r[1] if isinstance(r, tuple) else r
def k(r): return r[0] if isinstance(r, tuple) else r
def strip(s):
    s = d(s); s = s.split(';',1)[-1].strip() if ';' in s else s
    return s
sm = difflib.SequenceMatcher(None, [k(r) for r in want], [k(r) for r in got], autojunk=False)
n=0
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=='equal':
        if '-a' in sys.argv:
            for i in range(i1,i2): print("%3d %3d   %-36s | %s"%(i,j1+i-i1,strip(want[i]),strip(got[j1+i-i1])))
        else:
            print("     ... %d equal"%(i2-i1))
        continue
    L=max(i2-i1,j2-j1)
    for t in range(L):
        a = strip(want[i1+t]) if i1+t<i2 else ''
        bb = strip(got[j1+t]) if j1+t<j2 else ''
        print("%3s %3s * %-36s | %s"%(i1+t if i1+t<i2 else '', j1+t if j1+t<j2 else '', a, bb))
print("rows(funcdiff)=%d  len want=%d got=%d"%(len(funcdiff.diff_lines(want,got)),len(want),len(got)))
if '-g' in sys.argv:
    lo,hi = int(sys.argv[sys.argv.index('-g')+1]), int(sys.argv[sys.argv.index('-g')+2])
    for i in range(lo, min(hi,len(got))): print("%3d %s"%(i, strip(got[i])))
