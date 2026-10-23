#!/usr/bin/env python3
"""usage (docker, repo root): python3 drafts/T-25606/main/func_800476C0/r1-structure/lst.py BODY.c OUT.s
ugen's text listing (cc -S, before as1 scheduling, with the cvt/ugen passes) of main:func_800476C0 with
BODY.c in place of the INCLUDE_ASM line. T-25606 r1-structure: shows the order ugen hands out temporaries."""
import sys, os, subprocess, tempfile, shutil
from pathlib import Path
sys.path.insert(0, "tools")
import funcdiff as fd
import permute
import cc
NAME = "func_800476C0"
body_file, out = sys.argv[1:3]
c = fd.locate(NAME, "main")
text = Path(c.src).read_text()
inc = 'INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);\n'
a = text.index(inc)
text = text[:a] + "u8 func_8004901C(void);\n" + Path(body_file).read_text().rstrip("\n") + "\n" + text[a + len(inc):]
d = Path(tempfile.mkdtemp(dir="/tmp"))
(d / "cand.c").write_text(text)
base = permute.build_base(".", str(d / "cand.c"), NAME)
(d / "base.c").write_text(base)
tmp = tempfile.mkdtemp(prefix="idolib")
flags = [f for f in cc.IDO_CFLAGS if f != "-c"]
inc_flags = ["-I" + os.path.abspath("include") if f == "-Iinclude" else f for f in flags]
r = subprocess.run(["/opt/ido/5.3/cc", "-S"] + inc_flags + ["base.c"], cwd=str(d),
                   env=cc.ido_frame_env("5.3", tmp), capture_output=True, text=True)
shutil.copy(str(d / "base.s"), out)
shutil.rmtree(tmp)
print(r.returncode, (r.stdout + r.stderr)[-800:])
