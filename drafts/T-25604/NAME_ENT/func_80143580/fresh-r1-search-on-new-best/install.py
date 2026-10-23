"""T-25604 scratch: write BODY.c into the NON_MATCHING guard of src/ovl/NAME_ENT/80142A80.c (override defines to the top).
    python3 install.py BODY.c"""
import sys, os
from pathlib import Path
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, 'tools'); sys.path.insert(0, HERE)
import funcloc, score2
c = funcloc.locate(score2.NAME, "NAME_ENT")
text = score2.unit_text(c, Path(sys.argv[1]).read_text())
lines = text.splitlines(True)
seen = set(); out = []
for l in lines:  # drop a duplicate override define (already at the top from an earlier install)
    if l.startswith("#define MAIN_API_OVERRIDE_"):
        k = l.split()[1]
        if k in seen: continue
        seen.add(k)
    out.append(l)
Path(c.src).write_text("".join(out))
print("installed", sys.argv[1])
