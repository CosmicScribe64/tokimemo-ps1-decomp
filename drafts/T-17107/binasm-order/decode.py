import sys, struct
sys.path.insert(0, "/work/tools")
import frame_pass as fp
names = {v: k[1:] for k, v in fp.Z.items()}
names.update({48: "li", 0: "?"})
recs = fp.parse(open(sys.argv[1], "rb").read())
n = int(sys.argv[2]) if len(sys.argv) > 2 else 60
start = int(sys.argv[3]) if len(sys.argv) > 3 else 0
out = []; inproc = False; k = 0
for r, p in recs:
    if r.instr == fp.I_IENT: inproc = True
    if not inproc: continue
    if r.instr == 28: out.append("L%d" % r.w[3] if r.w[3] else "L?%x/%x" % (r.w[2], r.w[3]))
    elif r.instr == fp.I_ILABEL: out.append("$%d:" % r.w[0])
    elif r.instr == fp.I_IOCODE:
        out.append("%s r%d,r%d,r%d #%d s%d" % (names.get(r.op, "op%d" % r.op), r.reg1, r.reg2, r.reg3, r.imm, r.w[0]))
    elif r.instr == fp.I_IEND: break
print(" | ".join(out[start:start + n]))
