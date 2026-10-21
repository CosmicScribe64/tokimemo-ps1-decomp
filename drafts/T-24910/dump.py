#!/usr/bin/env python3
"""Dump the guarded C build of RPG_BAT/80154430.c (T-24910, local helper).

Usage (Docker): python3 drafts/T-24910/dump.py [func]
Writes build/T-24910/dd8.s (objdump of the guarded object).
"""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, "tools")
import funcdiff_verify as proof  # noqa: E402
import srcscan  # noqa: E402

func = sys.argv[1] if len(sys.argv) > 1 else "func_80154DD8"
c = next(f for f in srcscan.unit_c_files("RPG_BAT") if f.src.name == "80154430.c")
out = Path("build/T-24910")
out.mkdir(parents=True, exist_ok=True)
with tempfile.TemporaryDirectory(dir=out) as tmp:
    obj = proof.compile_guarded(c, Path(tmp))
    text = subprocess.run(
        ["mips-linux-gnu-objdump", "-dr", str(obj)], capture_output=True, text=True, check=True
    ).stdout
start = text.index("<%s>:" % func)
end = text.find("\n0000", start + 1)
Path("build/T-24910/dd8.s").write_text(text[start:end if end > 0 else None])
print("wrote build/T-24910/dd8.s")
