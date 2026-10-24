"""T-25608: as1 per-block trace of main func_8006B0C8 (run from the worktree root in Docker).

    tools/docker.sh python3 drafts/T-25608/main/func_8006B0C8/fresh-r1-as1-line-order-plus-permuter/trace.py NAME [BODYFILE] [--raw]

Preprocesses src/main/800674B0.c (NON_MATCHING view), swaps in BODYFILE as the body if given (a BODYFILE
keeps its #line lines, which IDO's cpp reads), compiles through the T-24900 as1 capture shim
(drafts/T-24900-as1/{cc_cap.py,shim.py} copied to build/as1tr/), prints word count, diff rows and the
bbdbg nodes of the outer-loop head block. Output in build/t25608/cap-NAME/.
"""
import os, pathlib, re, subprocess, sys
sys.path.insert(0, "/work/tools")
import funcdiff as fd
import funcdiff_verify as fv

SRC = "src/main/800674B0.c"
OUT = pathlib.Path("build/t25608")


def split():
    base = fv.preprocess_draft(SRC)
    m = re.search(r"void func_8006B0C8\(void\)\s*\{", base)
    depth = 0
    for j in range(base.index("{", m.start()), len(base)):
        depth += {"{": 1, "}": -1}.get(base[j], 0)
        if base[j] == "}" and depth == 0:
            return base[:m.start()], base[m.start():j + 1], base[j + 1:]
    raise SystemExit("body not found")


def main():
    name = sys.argv[1]
    pre, body, post = split()
    if len(sys.argv) > 2 and not sys.argv[2].startswith("--"):
        body = pathlib.Path(sys.argv[2]).read_text()
        body = body[body.index("void func_8006B0C8"):] if "void func_8006B0C8" in body else body
    cap = OUT / ("cap-" + name)
    cap.mkdir(parents=True, exist_ok=True)
    unit = cap / "unit.c"
    unit.write_text(pre + body + post)
    env = dict(os.environ, AS1CAP=str(cap.resolve()))
    subprocess.check_call(["python3", "/work/build/as1tr/cc_cap.py", str(unit), str(cap / "unit.o"), "ido", "5.3"], env=env)
    want = fd.resolved_functions("expected/build/src/main/800674B0.o", ["func_8006B0C8"])["func_8006B0C8"]
    got = fd.resolved_functions(str(cap / "unit.o"), ["func_8006B0C8"])["func_8006B0C8"]
    rows = sum(1 for a, b in zip(want, got) if a[0] != b[0]) + abs(len(want) - len(got))
    print(name, "words", len(got), "rows(same-pos)", rows, "w133-136", " ".join(str(w) for w in got[132:136]))
    if "--raw" in sys.argv:
        return
    log = (cap / "bbdbg.log").read_text(errors="replace").split("\n")
    print("log lines", len(log))


if __name__ == "__main__":
    main()
