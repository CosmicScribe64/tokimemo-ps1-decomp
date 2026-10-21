import sys, tempfile, difflib
from pathlib import Path
sys.path.insert(0, "tools")
import funcdiff, funcdiff_verify as proof, funcloc
unit, name = sys.argv[1].split(":")
src = sys.argv[2] if len(sys.argv) > 2 else None
c = funcdiff.locate(name, unit)
d = Path(tempfile.mkdtemp(dir="build/scratch/t25012"))
obj, _ = proof.compile_unit(c, d, source=src)
e, problem = funcdiff.expected_object(c, False, stale=[])
want = funcdiff.functions(e, [name])[name]
got = funcdiff.functions(obj, [name])[name]
import re
def norm(l): return re.sub(r"<[^>]*>","<>",l)
w=[norm(x) for x in want]; g=[norm(x) for x in got]
print("want %d got %d"%(len(w),len(g)))
for tag,i1,i2,j1,j2 in difflib.SequenceMatcher(None,w,g,autojunk=False).get_opcodes():
    if tag=="equal":
        print("  = (%d lines)"%(i2-i1)); continue
    for x in w[i1:i2]: print("  - "+x)
    for x in g[j1:j2]: print("  + "+x)
