#!/usr/bin/env python3
"""Scratch (T-25301): ugen listing (cc -S) of one body, with the cc.py shims."""
import os, subprocess, sys, tempfile, shutil
sys.path.insert(0, "tools")
import cc
body = open(sys.argv[1]).read()
out = os.path.abspath("build/t25301/lst")
os.makedirs(out, exist_ok=True)
tag = os.path.splitext(os.path.basename(sys.argv[1]))[0]
src = os.path.join(out, tag + ".c")
open(src, "w").write('#include "common.h"\n#include "ovl/NAME_ENT.h"\n' + body)
tmp = tempfile.mkdtemp()
env = cc.ido_frame_env("5.3", tmp)
flags = [f for f in cc.IDO_CFLAGS if f != "-c"]
flags = [("-I" + os.path.abspath(f[2:])) if f.startswith("-I") else f for f in flags]
r = subprocess.run(["/opt/ido/5.3/cc", "-S"] + flags + [src], cwd=out, env=env, capture_output=True, text=True)
shutil.rmtree(tmp)
if r.returncode:
    print(r.stderr[-2000:]); sys.exit(1)
s = open(os.path.join(out, tag + ".s")).read()
on = False
for ln in s.splitlines():
    if "func_8013AF78" in ln and ".ent" in ln: on = True
    if on and ln.strip() and not ln.strip().startswith(("#", ".loc", ".mask", ".fmask", ".frame", ".bgnb", ".endb")):
        print(ln)
    if on and ".end" in ln and "func_8013AF78" in ln: break
