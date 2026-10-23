"""T-25604 scratch helper: compare a compiled func_80143580 with an asm copy (rev1 or the target).

    mips-linux-gnu-objdump -d --disassemble=func_80143580 OBJ > X.dis   (in Docker)
    python3 rev1cmp.py X.dis versions/asm/rev1/NAME_ENT/func_80143580.s [x]

Aligns the opcode sequences (registers, immediates and relocations ignored), then counts the
register substitutions draft -> copy in the aligned part. `x` also lists the structural hunks.
Reads the asm at run time; holds no game data."""
import re, sys, difflib, collections
def canon(op, args):
    a = [x.strip().lstrip('$') for x in args.split(',')] if args.strip() else []
    if op in ('or', 'addu') and len(a) == 3 and a[2] == 'zero': op, a = 'move', a[:2]
    if op == 'addiu' and len(a) == 3 and a[1] == 'zero': op, a = 'li', [a[0]]
    if op == 'ori' and len(a) == 3 and a[1] == 'zero': op, a = 'li', [a[0]]
    if op == 'beq' and len(a) == 3 and a[1] == 'zero': op, a = 'beqz', a[:1]
    if op == 'bne' and len(a) == 3 and a[1] == 'zero': op, a = 'bnez', a[:1]
    if op in ('b', 'j', 'jal'): a = []
    regs = [re.sub(r'.*\((\w+)\)', r'\1', x) for x in a]
    regs = [r for r in regs if re.fullmatch(r'(v[01]|a[0-3]|t\d|s\d|ra|sp|at|zero|fp|gp|k[01])', r)]
    return op, regs
def objdump(path):
    out = []
    for line in open(path):
        m = re.match(r"\s+[0-9a-f]+:\s+[0-9a-f]{8}\s+(\S+)\s*([^<]*)", line)
        if m: out.append(canon(m.group(1), m.group(2)))
    return out
def splat(path):
    out = []
    for line in open(path):
        m = re.match(r"\s*/\* [0-9A-F]+ [0-9A-F]+ [0-9A-F]{8} \*/\s+(\S+)\s*(.*?)\s*(#.*)?$", line)
        if m and not m.group(1).startswith('.'):
            args = re.sub(r'%(hi|lo)\([^)]*\)', '0', m.group(2))
            out.append(canon(m.group(1), args))
    return out
D = objdump(sys.argv[1]); R = splat(sys.argv[2])
sm = difflib.SequenceMatcher(None, [o for o, _ in D], [o for o, _ in R], autojunk=False)
pairs = collections.Counter(); hunks = []
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag != 'equal': hunks.append((tag, i1, i2, j1, j2)); continue
    for k in range(i2 - i1):
        for x, y in zip(D[i1+k][1], R[j1+k][1]):
            if x != y: pairs[(x, y)] += 1
print("draft", len(D), "rev1", len(R), "struct hunks", len(hunks), "reg diffs", sum(pairs.values()))
print("  ", ", ".join(f"{x}->{y} x{n}" for (x, y), n in pairs.most_common(14)))
if len(sys.argv) > 3:
    for h in hunks[:30]: print("  ", h, [f"{D[i][0]} {','.join(D[i][1])}" for i in range(h[1], min(h[2], h[1]+3))], [f"{R[j][0]} {','.join(R[j][1])}" for j in range(h[3], min(h[4], h[3]+3))])
