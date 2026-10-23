"""usage (host): python3 var.py BASE.c OUTDIR 'name::old=>new;;old2=>new2' ... ; writes OUTDIR/name.c variants (T-25317 helper)."""
import sys
from pathlib import Path
base = Path(sys.argv[1]).read_text()
out = Path(sys.argv[2]); out.mkdir(exist_ok=True)
for spec in sys.argv[3:]:
    name, edits = spec.split("::", 1)
    t = base
    for e in edits.split(";;"):
        if not e: continue
        old, new = e.split("=>", 1)
        old = old.replace("\\n", "\n"); new = new.replace("\\n", "\n")
        if old not in t:
            sys.exit("not found in %s: %r" % (name, old))
        t = t.replace(old, new, 1)
    (out / (name + ".c")).write_text(t)
    print(out / (name + ".c"))
