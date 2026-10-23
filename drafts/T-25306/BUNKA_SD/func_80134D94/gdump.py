"""gdump.py in.G [from to]: print func_80134D94's binasm (ugen output)"""
import sys, os
sys.path.insert(0, 'tools')
import frame_pass as fp
R = ['zero','at','v0','v1','a0','a1','a2','a3','t0','t1','t2','t3','t4','t5','t6','t7','s0','s1','s2','s3','s4','s5','s6','s7','t8','t9','k0','k1','gp','sp','fp','ra']
names = {v: k[1:] for k, v in fp.Z.items()}
extra = {3:'and', 5:'bc0f', 10:'bc1t', 31:'div', 32:'divu', 41:'li', 53:'mul', 59:'mflo', 58:'mfhi', 62:'neg', 64:'nor', 65:'or', 69:'rem', 73:'seq', 80:'sle', 82:'slt', 83:'sltu', 84:'sne', 85:'sra', 88:'xor', 77:'sltu?', 81:'sleu', 78:'sgtu?', 89:'sltiu?', 226:'ori', 203:'andi', 225:'xori', 227:'slti', 228:'sltiu', 33:'divu?', 63:'not', 75:'sgt', 74:'sge', 66:'rol', 67:'ror', 68:'mult?', 90:'multu?',  57:'mflo?', 86:'subu'}
data = open(sys.argv[1], 'rb').read()
recs = fp.parse(data)
lo, hi = (int(sys.argv[2]), int(sys.argv[3])) if len(sys.argv) > 3 else (0, 10**9)
for s, e in fp.procedures(recs):
    rec0 = recs[s][0]
    k = 0
    out = []
    for rec, _ in recs[s:e+1]:
        if rec.instr == fp.I_IOCODE:
            op = rec.op
            nm = names.get(op) or extra.get(op) or ('op%d' % op)
            regs = [R[r] if r < 32 else ('-' if r == 72 else 'r%d' % r) for r in (rec.reg1, rec.reg2, rec.reg3)]
            out.append('%3d %-6s f%-2d %s,%s,%s imm=%d sym=%d' % (k, nm, rec.form, regs[0], regs[1], regs[2], rec.imm, rec.w[0]))
            k += 1
        elif rec.instr == fp.I_ILABEL:
            out.append('    L%d:' % rec.w[0])
    if len(out) > 150:
        print('\n'.join(l for l in out if not l[:3].strip().isdigit() or lo <= int(l[:3]) <= hi))
