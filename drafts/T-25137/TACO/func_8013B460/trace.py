"""T-25137: as1 per-block scheduler trace of TACO func_8013B460 (run from the repository root in Docker).

    tools/docker.sh python3 drafts/T-25137/TACO/func_8013B460/trace.py NAME [BODYFILE]

Preprocesses src/ovl/TACO/8013B170.c (NON_MATCHING view, as funcdiff does), swaps in BODYFILE as the
body of func_8013B460 if given, compiles the unit with the normal IDO 5.3 pipeline through the
T-24900 as1 capture shim, and prints the word count, aligned cost and the DAG nodes of the first quad's
y3 / UV / AddPrim block from stock `as1 -R -xbbdbg 8`. Output goes to build/t25137/cap-NAME/.
Needs drafts/T-24900-as1/{cc_cap.py,shim.py} copied to build/as1tr/ first.
Note: preprocess_draft runs `gcc -E -P`, which drops the `#line` markers of the committed source, so
the default body traces as the 4-row baseline. A BODYFILE is inserted after preprocessing and keeps
its `#line` lines (IDO's own cpp reads them), so pass the committed body as BODYFILE to trace it.
"""
import os
import pathlib
import re
import subprocess
import sys

sys.path.insert(0, "/work/tools")
import funcdiff as fd
import funcdiff_verify as fv

SRC = "src/ovl/TACO/8013B170.c"
OUT = pathlib.Path("build/t25137")


def split():
    base = fv.preprocess_draft(SRC)
    m = re.search(r"s32 func_8013B460\([^;]*?\)\s*\{", base)
    depth = 0
    for j in range(base.index("{", m.start()), len(base)):
        depth += {"{": 1, "}": -1}.get(base[j], 0)
        if base[j] == "}" and depth == 0:
            return base[:m.start()], base[m.start():j + 1], base[j + 1:]
    raise SystemExit("body not found")


def main():
    name = sys.argv[1]
    pre, body, post = split()
    if len(sys.argv) > 2:
        body = pathlib.Path(sys.argv[2]).read_text()
    cap = OUT / ("cap-" + name)
    cap.mkdir(parents=True, exist_ok=True)
    unit = cap / "unit.c"
    unit.write_text(pre + body + post)
    env = dict(os.environ, AS1CAP=str(cap.resolve()))
    subprocess.check_call(["python3", "/work/build/as1tr/cc_cap.py", str(unit), str(cap / "unit.o"), "ido", "5.3"], env=env)
    want = fd.resolved_functions("expected/build/ovl/TACO/src/ovl/TACO/8013B170.o", ["func_8013B460"])["func_8013B460"]
    got = fd.resolved_functions(str(cap / "unit.o"), ["func_8013B460"])["func_8013B460"]
    print(name, "words", len(got), "cost", fd.diff_score(want, got))
    # first quad: sh y3 (a60b0022), sb u3 (a2..0024), sb u0 (a2..000c), lui/lw of the AddPrim argument
    pat = re.compile(r"inst (a60b0022|a20.0024|a20.000c|3c0c0000|8d8c0000|254bff88),")
    log = (cap / "bbdbg.log").read_text(errors="replace").split("\n")
    first = next(i for i, l in enumerate(log) if "inst a60b0022" in l)
    for l in log[max(0, first - 40):first + 60]:
        if pat.search(l):
            print(l)


if __name__ == "__main__":
    main()
