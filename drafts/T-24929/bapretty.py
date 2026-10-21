#!/usr/bin/env python3
"""Scratch binasm printer (T-24929): ugen output records of procedure N (1-based ient count)."""
import sys
sys.path.insert(0, "tools")
import frame_pass as fp

NAMES = {v: k for k, v in fp.Z.items()}
REG = ["zero","at","v0","v1","a0","a1","a2","a3"]+["t%d"%i for i in range(8)]+["s%d"%i for i in range(8)]+["t8","t9","k0","k1","gp","sp","s8","ra"]
def r(n): return REG[n] if n < 32 else "r%d" % n
data = open(sys.argv[1], "rb").read()
want = int(sys.argv[2])
recs = fp.parse(data)
n = 0
for k, (rec, _) in enumerate(recs):
    if rec.instr == fp.I_IENT:
        n += 1
    if n == want:
        if rec.instr == fp.I_IENT or rec.instr != 0 and False:
            pass
        name = NAMES.get(rec.op, "?") if rec.instr in (getattr(fp, "I_IRI", -1), getattr(fp, "I_IRRR", -1), getattr(fp, "I_IRR", -1), getattr(fp, "I_IRRI", -1), getattr(fp, "I_IA1", -1), getattr(fp, "I_IA2", -1)) or True else ""
        print(k, "instr=%d" % rec.instr, name, r(rec.reg1), r(rec.reg2), r(rec.reg3), rec.imm, "form=%d" % rec.form)
    if n > want:
        break
