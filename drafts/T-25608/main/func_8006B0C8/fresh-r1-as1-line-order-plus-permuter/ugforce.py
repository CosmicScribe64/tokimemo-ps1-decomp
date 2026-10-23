"""T-25608: compile src/main/800674B0.c (NON_MATCHING view, BODYFILE swapped in if given) with the
ugstate.py ugen shim and print rows against the original.  ugforce.py NAME [BODYFILE] [FORCE]"""
import os, pathlib, re, subprocess, sys
sys.path.insert(0, "/work/tools")
import funcdiff as fd
import funcdiff_verify as fv
import cc
HERE = os.path.dirname(os.path.abspath(__file__))
cc.SHIMS["ugen"] = os.path.join(HERE, "ugstate.py")
OUT = pathlib.Path("build/t25608")


def split():
    base = fv.preprocess_draft("src/main/800674B0.c")
    m = re.search(r"void func_8006B0C8\(void\)\s*\{", base)
    depth = 0
    for j in range(base.index("{", m.start()), len(base)):
        depth += {"{": 1, "}": -1}.get(base[j], 0)
        if base[j] == "}" and depth == 0:
            return base[:m.start()], base[m.start():j + 1], base[j + 1:]


name = sys.argv[1]
pre, body, post = split()
if len(sys.argv) > 2 and sys.argv[2] != "-":
    body = pathlib.Path(sys.argv[2]).read_text()
    body = body[body.index("void func_8006B0C8"):]
if len(sys.argv) > 3:
    os.environ["UGFORCE"] = sys.argv[3]
d = OUT / ("ug-" + name)
d.mkdir(parents=True, exist_ok=True)
(d / "unit.c").write_text(pre + body + post)
log = d / "uglog.txt"
if log.exists():
    log.unlink()
os.environ["UGLOG"] = str(log.resolve()); os.environ["UGDUMP"] = str((d / "ugdump.txt").resolve())
sys.argv = ["cc.py", str(d / "unit.c"), str(d / "unit.o"), "ido", "5.3"]
if (d / "unit.o").exists():
    (d / "unit.o").unlink()
try:
    cc.main(sys.argv)
except SystemExit as e:
    if e.code:
        raise
want = fd.resolved_functions("expected/build/src/main/800674B0.o", ["func_8006B0C8"])["func_8006B0C8"]
got = fd.resolved_functions(str(d / "unit.o"), ["func_8006B0C8"])["func_8006B0C8"]
diff = [i for i, (a, b) in enumerate(zip(want, got)) if a[0] != b[0]]
print(name, "len", len(got), "rows", len(diff) + abs(len(want) - len(got)), "first", diff[:12])
