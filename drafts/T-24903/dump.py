"""dump.py UNIT FUNC VARIANT.c [lo hi] : print expected vs built resolved instruction words
side by side (aligned by funcdiff) around rows lo..hi, plus a full listing of the first 70."""
import os, sys, tempfile
from pathlib import Path
sys.path.insert(0, "/work/tools"); os.chdir("/work")
import funcdiff as fd, funcdiff_verify as fv

unit, name, variant = sys.argv[1], sys.argv[2], sys.argv[3]
c = fd.locate(name, unit)
e, _ = fd.expected_object(c)
want = fd.resolved_functions(e, [name], c)[name]
with tempfile.TemporaryDirectory() as tmp:
    obj, _ = fv.compile_unit(c, Path(tmp), source=variant)
    got = fd.resolved_functions(obj, [name], c)[name]
print("len got %d want %d" % (len(got), len(want)))
for i, (a, b) in enumerate(zip(want, got)):
    mark = "  " if a == b else "!!"
    print("%s %3d  %-46s | %s" % (mark, i, a, b))
if len(want) != len(got):
    longer, tag = (want, "want") if len(want) > len(got) else (got, "got")
    for i in range(min(len(want), len(got)), len(longer)):
        print("   %3d  %s only: %s" % (i, tag, longer[i]))
