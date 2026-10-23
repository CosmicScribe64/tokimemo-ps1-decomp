"""Capture unchanged as1 inputs and stock initial-node diagnostics in Docker."""
import os
import sys
from pathlib import Path
sys.path.insert(0, "tools")
import cc
label = sys.argv[1]
directory = Path("build/t25036") / label / "as1"
directory.mkdir(exist_ok=True)
os.environ["AS1CAP"] = str(directory.resolve())
cc.SHIMS["as1"] = str(Path("drafts/T-24900-as1/shim.py").resolve())
cc.main(["cc.py", str(directory.parent / "guarded.c"),
         str(directory / "unit.o"), "ido", "5.3"])
print(label, "as1 capture complete")
