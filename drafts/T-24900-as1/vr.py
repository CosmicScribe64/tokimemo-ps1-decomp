"""vr.py UNIT FUNC VARIANT.c... : compile each variant (full copy of the unit's C file, NON_MATCHING)
and print the aligned diff score and diff rows for FUNC. Set TRACE=1 to keep the as1 trace."""
import os, sys, tempfile
from pathlib import Path
sys.path.insert(0, "/work/tools"); os.chdir("/work")
import funcdiff as fd, funcdiff_verify as fv
unit, name, variants = sys.argv[1], sys.argv[2], sys.argv[3:]
c = fd.locate(name, unit)
e, problem = fd.expected_object(c)
want = fd.resolved_functions(e, [name], c)[name]
quiet = os.environ.get("Q")
for v in variants:
    with tempfile.TemporaryDirectory() as tmp:
        try:
            obj, _ = fv.compile_unit(c, Path(tmp), source=v)
        except BaseException as ex:
            print("== %s: COMPILE FAIL %s" % (v, ex)); continue
        got = fd.resolved_functions(obj, [name], c)[name]
        if got is None:
            print("== %s: no function" % v); continue
        rows = fd.diff_lines(want, got)
        print("== %s: score %d rows %d len %d/%d" % (v, fd.diff_score(want, got), len(rows), len(got), len(want)))
        if not quiet or os.environ.get("ROWS"):
            for r in rows[:int(os.environ.get("ROWS","30"))]: print("   ", r)
