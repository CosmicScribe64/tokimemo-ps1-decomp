"""dump.py OUTDIR SRC [--asm] : compile the guarded TU of SRC and dump func_8013B460.

Writes OUTDIR/guarded.o and OUTDIR/dis.txt (mips-linux-gnu-objdump -drz) for the
requested function, so a source-order experiment can be compared with the target asm.
"""
import os
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, "/work/tools")
import funcdiff_verify as fv
import funcloc

out = Path(sys.argv[1]).resolve()
src = sys.argv[2] if len(sys.argv) > 2 else "src/ovl/TACO/8013B170.c"
nm = "--asm" not in sys.argv
out.mkdir(parents=True, exist_ok=True)
c = funcloc.locate("func_8013B460", "TACO")
obj, _ = fv.compile_unit(c, out, non_matching=nm, source=src)
dis = subprocess.run(["mips-linux-gnu-objdump", "-drz", "--no-show-raw-insn", obj],
                     capture_output=True, text=True).stdout
Path(out / "dis.all.txt").write_text(dis)
body, keep = [], False
for line in dis.splitlines():
    if "<func_8013B460>:" in line:
        keep = True
    elif keep and line.endswith(">:"):
        break
    if keep:
        body.append(line)
Path(out / "dis.txt").write_text("\n".join(body) + "\n")
print("\n".join(body))
