#!/usr/bin/env python3
"""T-25151: run try.py over template variants. Usage: sweep.py <template.c> <alts.txt>
alts.txt: blocks separated by lines '===' ; each block is 'KEY=>replacement' lines (KEY appears as @KEY@ in template)."""
import subprocess, sys
from pathlib import Path
D = Path(__file__).parent
tpl = Path(sys.argv[1]).read_text()
blocks = Path(sys.argv[2]).read_text().split("\n===\n")
for n, b in enumerate(blocks):
    s = tpl
    for line in b.strip("\n").splitlines():
        k, v = line.split("=>", 1)
        s = s.replace("@" + k.strip() + "@", v.replace("\\n", "\n"))
    f = D / f"_sw{n}.c"
    f.write_text(s)
    out = subprocess.run([sys.executable, str(D / "try.py"), str(f)], capture_output=True, text=True).stdout
    r = [l for l in out.splitlines() if l.startswith("rows:")]
    m = "MATCH" if ("MATCH" in out and "DIFF" not in out) else ""
    print(n, r[0] if r else out[-300:], m, "|", b.strip().replace("\n", " ; ")[:150], flush=True)
