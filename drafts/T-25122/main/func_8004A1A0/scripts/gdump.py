#!/usr/bin/env python3
"""Scratch (T-25122): compile a body, dump ugen binasm (pre-as1 order) of FUNC."""
import sys, tempfile
from pathlib import Path
sys.path.insert(0, "tools")
import funcloc, funcdiff_verify as proof, cc, frame_pass as fp
name = sys.argv[1]
c = funcloc.locate(name, "main")
text = open(c.src).read()
if len(sys.argv) > 2:
    body = open(sys.argv[2]).read()
    i = text.index("#ifdef NON_MATCHING\nvoid " + name); j = text.index("#else", i)
    text = text[:i] + "#ifdef NON_MATCHING\n" + body + text[j:]
d = Path(tempfile.mkdtemp()); src = d / Path(c.src).name; src.write_text(text)
u = d / "u.c"; u.write_text(proof.preprocess_draft(src, True))
orig = cc.ido_frame_env
cc.ido_frame_env = lambda v, tmp, extra=None: orig(v, tmp, {"as1": "/scratch/as1cap.py"})
cc.compile_ido(str(u), str(d / "u.o"), "5.3")
recs = fp.parse(open("/scratch/cap.G", "rb").read())
names = {v: k[1:] for k, v in fp.Z.items()}
R = "zero at v0 v1 a0 a1 a2 a3 t0 t1 t2 t3 t4 t5 t6 t7 s0 s1 s2 s3 s4 s5 s6 s7 t8 t9 k0 k1 gp sp fp ra".split()
def rn(r): return R[r] if r < 32 else ("r%d" % r if r != 72 else "-")
on = False
for rec, _ in recs:
    if rec.instr == fp.I_IENT:
        on = True if not on else on
    if not on: continue
    if rec.instr == fp.I_IOCODE:
        print("%-7s %s %s %s %d" % (names.get(rec.op, "op%d" % rec.op), rn(rec.reg1), rn(rec.reg2), rn(rec.reg3), rec.imm))
    elif rec.instr in (fp.I_ILABEL, fp.I_ILAB):
        print("LABEL", rec.w[0])
    elif rec.instr == fp.I_IEND:
        print("END"); on = False
    elif rec.instr in (48, 49):
        print("%s %s %s" % ("noalias" if rec.instr == 48 else "alias", rn(rec.reg1), rn(rec.reg2)), ["%08x" % w for w in rec.w])
    elif rec.instr == 28:
        print("loc", rec.w[2] & 0xFFFF, rec.w[3])
    else:
        print("rec", rec.instr)
