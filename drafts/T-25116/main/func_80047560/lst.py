#!/usr/bin/env python3
"""usage (in docker, repo root): python3 drafts/T-25116/main/func_80047560/lst.py OUTDIR BODY.c [...]
Compiles each body as a snippet (common.h, game.h, func_80047550, the body) with the build's IDO
passes and writes OUTDIR/<name>.s, the ugen listing of `cc -S` (which pass placed what). Quick view
only: the whole-unit compile of try.py is the measurement."""
import os, sys, shutil, subprocess, tempfile
from pathlib import Path
sys.path.insert(0, "tools")
import cc

HEAD = '#include "common.h"\n#include "game.h"\n\nvoid func_80047550(void) {\n    D_801255DC = 4;\n}\n\n'
outdir = Path(sys.argv[1]); outdir.mkdir(parents=True, exist_ok=True)
for bf in sys.argv[2:]:
    stem = Path(bf).stem
    d = Path(tempfile.mkdtemp(dir="/tmp"))
    src = d / (stem + ".c")
    src.write_text(HEAD + Path(bf).read_text())
    lib = tempfile.mkdtemp(prefix="idolib")
    flags = [f for f in cc.IDO_CFLAGS if f != "-c"]
    flags = [("-I" + os.path.abspath("include")) if f == "-Iinclude" else f for f in flags]
    r = subprocess.run(["/opt/ido/5.3/cc", "-S"] + flags + [str(src)], cwd=d,
                       env=cc.ido_frame_env("5.3", lib), capture_output=True, text=True)
    s = d / (stem + ".s")
    if r.returncode or not s.exists():
        print(stem, "ERROR", (r.stderr or r.stdout)[-300:]); continue
    shutil.copy(s, outdir / (stem + ".s"))
    print(stem, "ok")
