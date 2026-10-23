#!/usr/bin/env python3
"""Scratch (T-25122): compile a body, dump uopt output ucode op names for FUNC's procedure."""
import sys, tempfile
from pathlib import Path
sys.path.insert(0, "tools")
import funcloc, funcdiff_verify as proof, cc, ugen_pass as up
name = sys.argv[1]
c = funcloc.locate(name, "main")
text = open(c.src).read()
body = open(sys.argv[2]).read()
i = text.index("#ifdef NON_MATCHING\nvoid " + name); j = text.index("#else", i)
text = text[:i] + "#ifdef NON_MATCHING\n" + body + text[j:]
d = Path(tempfile.mkdtemp()); src = d / Path(c.src).name; src.write_text(text)
u = d / "u.c"; u.write_text(proof.preprocess_draft(src, True))
orig = cc.ido_frame_env
cc.ido_frame_env = lambda v, tmp, extra=None: orig(v, tmp, {"ugen": "/scratch/ugencap.py"})
cc.compile_ido(str(u), str(d / "u.o"), "5.3")
ins = up.parse_ucode(open("/scratch/cap.O", "rb").read())
n = 0
for x in ins:
    op = up.OPS[x.opc]
    if op == "ent": n += 1
    if n == 3:
        print(op, " ".join("%x" % w for w in x.words[1:]))
    if op == "end" and n == 3: break
