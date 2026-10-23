"""Per function and symbol S: materialisations (lui R,%hi(S); addiu R,R,%lo(S)) and how each is used
(addu operand = array base, bne/beq operand = compare) until R is redefined. Report functions where S is
used both as base and in a compare: 'shared' (one materialisation feeds both) or 'separate'."""
import re, glob, collections, sys
INS = re.compile(r'^\s*/\*.*?\*/\s*(\S+)\s+(.*?)\s*$')
BR = {'bnez', 'beqz', 'blez', 'bgtz', 'bltz', 'bgez', 'b', 'j', 'jr', 'sw', 'sh', 'sb', 'bne', 'beq', 'jal'}
out = collections.defaultdict(list)
for f in glob.glob('asm/**/*.s', recursive=True):
    if '/data/' in f or 'matchings' not in f: continue
    ins = []
    for line in open(f, errors='ignore'):
        m = INS.match(line)
        if m: ins.append((m.group(1), [a.strip() for a in m.group(2).split(',')]))
    per = collections.defaultdict(list)   # S -> list of (R, uses set)
    for i, (op, a) in enumerate(ins):
        if op != 'lui' or len(a) != 2 or not a[1].startswith('%hi('): continue
        R, S = a[0], a[1][4:-1]
        j = next((k for k in range(i+1, min(i+7, len(ins))) if ins[k][0] == 'addiu' and ins[k][1][:2] == [R, R] and ins[k][1][2] == '%lo(' + S + ')'), None)
        if j is None: continue
        uses = set()
        for k in range(j+1, min(j+300, len(ins))):
            op2, b = ins[k]
            if op2 == 'addu' and R in b[1:]: uses.add('base')
            if op2 in ('bne', 'beq') and R in b[:2]: uses.add('cmp')
            if op2 == 'jal' and not R.startswith('$s'): break
            if b and b[0] == R and op2 not in BR: break
        per[S].append((R, uses))
    for S, mats in per.items():
        allu = set().union(*[u for _, u in mats])
        if {'base', 'cmp'} <= allu:
            kind = 'shared' if any({'base', 'cmp'} <= u for _, u in mats) else 'separate'
            out[kind].append((f.split('/')[-1], S, [(R, sorted(u)) for R, u in mats]))
for k, v in out.items():
    print(k, len(v))
    for x in v[:int(sys.argv[1])]: print('  ', x[0], x[1], x[2])
