#!/usr/bin/env python3
"""Scratch helper: compile the *guarded* ETC/8013BEF0.c TU and dump func_8013CCE4.

Usage (docker): python3 drafts/T-24921/ccdump.py [outdir]
"""
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, "tools")
import funcdiff_verify
import funcloc

name = "func_8013CCE4"
out = Path(sys.argv[1] if len(sys.argv) > 1 else "drafts/T-24921/asm")
out.mkdir(parents=True, exist_ok=True)
c = funcloc.locate(name, "ETC")
obj = funcdiff_verify.compile_guarded(c, out)
dump = subprocess.run(["mips-linux-gnu-objdump", "-dr", "--disassemble=" + name, obj],
                      capture_output=True, text=True)
if dump.returncode:
    sys.exit(dump.stderr)
(out / "func.s").write_text(dump.stdout)
print(out / "func.s")
