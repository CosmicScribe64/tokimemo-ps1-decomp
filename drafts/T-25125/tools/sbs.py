"""sbs.py [OUT] : compile the NON_MATCHING TU, write aligned side-by-side of func_8007132C vs target to OUT."""
import sys, re, subprocess, tempfile, difflib
from pathlib import Path
sys.path.insert(0, 'tools')
import funcdiff as fd
import funcdiff_verify as fv
name = 'func_8007132C'
out = sys.argv[1] if len(sys.argv) > 1 else 'build/t25125/sbs.txt'
c = fd.locate(name, 'main')
d = Path(tempfile.mkdtemp(dir='build/t25125'))
obj = fv.compile_guarded(c, d)
dis = subprocess.run(['mips-linux-gnu-objdump', '-drz', str(obj)], stdout=subprocess.PIPE, text=True).stdout
funcs = fd.split_functions(dis)
lines = funcs[name]
mine = []
for l in lines:
    m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l)
    if m:
        mine.append([m.group(3).split('<')[0].strip().replace('\t', ' '), ''])
    else:
        m = re.match(r'\s*[0-9a-f]+: (R_MIPS_\w+)\s+(\S+)', l)
        if m and mine:
            mine[-1][1] = m.group(2)
orig = []
for l in open('asm/nonmatchings/main/8006CB30/%s.s' % name):
    m = re.match(r'\s*/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/\s+(.*)', l)
    if m:
        orig.append(m.group(2).strip())
def norm(s):
    s = re.sub(r'\s+', ' ', s.strip())
    s = s.replace('$', '')
    return s
def op(s):
    return s.split()[0] if s.split() else ''
def regs(s):
    parts = s.split(None, 1)
    if len(parts) < 2:
        return ()
    return tuple(re.findall(r'\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|fp|ra|sp|gp|k[01])\b', parts[1]))
mo = [norm(x[0]) for x in mine]
oo = [norm(x) for x in orig]
def canon(x):
    x = x.replace(', ', ',').replace(',', ', ')
    m = re.match(r'^move (\S+), (\S+)$', x)
    if m: return 'or %s, %s, zero' % m.groups()
    m = re.match(r'^li (\S+), (-?\d+)$', x)
    if m:
        v = int(m.group(2))
        return ('addiu %s, zero, %d' if -32768 <= v < 32768 else 'ori %s, zero, %d') % (m.group(1), v)
    m = re.match(r'^negu (\S+), (\S+)$', x)
    if m: return 'subu %s, zero, %s' % m.groups()
    return x
mo = [canon(x) for x in mo]
sm = difflib.SequenceMatcher(None, [op(x) for x in oo], [op(x) for x in mo], autojunk=False)
rows = []
nstruct = nreg = 0
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    n = max(i2 - i1, j2 - j1)
    for k in range(n):
        a = oo[i1 + k] if i1 + k < i2 else ''
        b = mo[j1 + k] if j1 + k < j2 else ''
        bi = j1 + k if j1 + k < j2 else -1
        rel = mine[bi][1] if bi >= 0 else ''
        if tag != 'equal':
            mark = '#'
            nstruct += 1
        elif regs(a) != regs(b):
            mark = 'r'
            nreg += 1
        else:
            mark = ' '
        rows.append('%4s %4s %s %-48s | %s %s' % (i1 + k if i1 + k < i2 else '', bi if bi >= 0 else '', mark, a[:48], b[:44], rel))
Path(out).write_text('\n'.join(rows) + '\n')
print('orig %d mine %d struct %d reg %d' % (len(oo), len(mo), nstruct, nreg))
