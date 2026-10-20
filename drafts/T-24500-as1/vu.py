"""vu.py UNIT CFILE VARIANT.c... : per-function scores of every function of the unit (variant vs expected)."""
import os, sys, tempfile, re
from pathlib import Path
sys.path.insert(0, "/work/tools"); os.chdir("/work")
import funcdiff as fd, funcdiff_verify as fv
unit, cfile, variants = sys.argv[1], sys.argv[2], sys.argv[3:]
names = re.findall(r"^(?:INCLUDE_ASM\([^,]*, (\w+)\)|[a-zA-Z_][\w ]*?\b(func_\w+)\([^;]*\) \{)", open(cfile).read(), re.M)
names = sorted({a or b for a, b in names})
c = fd.locate(names[0], unit)
e, _ = fd.expected_object(c)
want = fd.resolved_functions(e, names, c)
for v in variants:
    with tempfile.TemporaryDirectory() as tmp:
        obj, _ = fv.compile_unit(c, Path(tmp), source=v)
        got = fd.resolved_functions(obj, names, c)
        bad = ["%s:%d" % (n, fd.diff_score(want[n], got[n])) for n in names if want.get(n) is not None and got.get(n) is not None and fd.keys(want[n]) != fd.keys(got[n])]
        print("== %s: %d funcs, differing: %s" % (v, len(names), " ".join(bad) or "none"))
