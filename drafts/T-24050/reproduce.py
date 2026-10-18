"""Docker-only full-file source checkpoint replay; never writes production source."""
from pathlib import Path
import subprocess
import sys
root = Path(__file__).resolve().parents[2]
source = root / "src/ovl/GEKO/80134040.c"
marker = 'INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134040", func_80134808);'
text = source.read_text()
if text.count(marker) != 1:
    raise SystemExit("Expected one unchanged target assembly fallback; refusing ambiguous replay")
work = root / "build/T24050"
work.mkdir(parents=True, exist_ok=True)
scratch = work / "replay.c"
scratch.write_text(text.replace(marker, (Path(__file__).parent / "body.c").read_text()))
subprocess.run([sys.executable, "tools/viewsweep.py", "GEKO:func_80134808", "--src", str(scratch), "--budget", "1"], cwd=root, check=True)
