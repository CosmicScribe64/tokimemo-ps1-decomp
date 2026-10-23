"""Replay this source-only checkpoint in the complete current TACO unit (Docker)."""
from pathlib import Path
import subprocess
import sys

packet = Path(__file__).parent
unit = Path("src/ovl/TACO/80139170.c").read_text()
unit = unit.replace('#include "ovl/TACO.h"', '#define func_80139424 ignored_font_view\n#include "ovl/TACO.h"\n#undef func_80139424\ns32 func_80139424();', 1)
needle = 'INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80139170", func_80139FB8);'
assert unit.count(needle) == 1
unit = unit.replace(needle, (packet / "body.c").read_text())
source = Path("build/T24502/carousel-full-replay.c")
source.parent.mkdir(parents=True, exist_ok=True)
source.write_text(unit)
obj = source.with_suffix(".o")
subprocess.run([sys.executable, "tools/cc.py", str(source), str(obj), "ido", "5.3"], check=True)
sys.exit(subprocess.run([sys.executable, "tools/funcdiff.py", "--built", str(obj), "--resolve", "TACO:func_80139FB8"]).returncode)
