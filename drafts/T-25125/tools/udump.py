"""udump.py [OUT]: dump input ucode of func_8007132C from the last regdiff trace (draft.B)."""
import sys
from pathlib import Path
sys.path.insert(0, "tools")
import cvt_pass as cv
records = cv.parse(Path("build/scratch/regdiff/func_8007132C/draft.B").read_bytes())
out = []
for a, b in cv._procedures(records):
    if any(b"func_8007132C" in r.extra for r in records[a:b+1]):
        for n, r in enumerate(records[a:b+1]):
            out.append("%04d %-5s %s %r" % (n, cv.OPS[r.opc], " ".join("%08x" % w for w in r.words), r.extra))
Path(sys.argv[1] if len(sys.argv) > 1 else "build/t25125/ucode.txt").write_text("\n".join(out) + "\n")
print(len(out))
