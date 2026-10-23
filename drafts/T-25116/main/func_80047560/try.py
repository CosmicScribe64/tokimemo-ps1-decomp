#!/usr/bin/env python3
"""usage (in docker, repo root): python3 drafts/T-25116/main/func_80047560/try.py OUTDIR BODY.c [BODY.c ...]
Each BODY.c replaces the guarded block (or INCLUDE_ASM line) of func_80047560 in src/main/80047550.c in
memory and is compiled as a whole unit (funcdiff's way), two at a time. Writes OUTDIR/<name>.got/.diff and
prints the number of differing words per body. OUTDIR should be outside the repo (the .want text is
disassembly of the original and must not be committed)."""
import sys, tempfile
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, "tools")
import funcdiff as fd
import funcdiff_verify as proof

NAME = "func_80047560"
outdir = Path(sys.argv[1]); outdir.mkdir(parents=True, exist_ok=True)
bodies = sys.argv[2:]
c = fd.locate(NAME, "main")
base = Path(c.src).read_text()
inc = 'INCLUDE_ASM("asm/nonmatchings/main/80047550", %s);\n' % NAME
idx = base.find(inc)
start = base.rfind("#ifdef NON_MATCHING\n", 0, idx)
if start >= 0 and "#endif" not in base[start:idx]:
    ls = base.rfind("\n", 0, start - 1) + 1
    if base[ls:start].startswith("/* NON_MATCHING"):
        start = ls
    end = idx + len(inc) + len("#endif\n")
else:
    start, end = idx, idx + len(inc)
e, problem = fd.expected_object(c)
if problem:
    sys.exit("expected: " + problem)
want = fd.resolved_functions(e, [NAME], c)[NAME]
(outdir / "want.txt").write_text("\n".join(fd.functions(e, [NAME])[NAME]) + "\n")

def run(bf):
    body = Path(bf).read_text().rstrip("\n")
    text = base[:start] + "#ifdef NON_MATCHING\n" + body + "\n#else\n" + inc + "#endif\n" + base[end:]
    d = Path(tempfile.mkdtemp(dir="/tmp"))
    (d / "cand.c").write_text(text)
    try:
        b = proof.compile_unit(c, d, source=str(d / "cand.c"))[0]
        got = fd.resolved_functions(b, [NAME])[NAME]
    except Exception as ex:
        return "%-28s ERROR %s" % (Path(bf).stem, str(ex).splitlines()[-1][:200] if str(ex) else type(ex).__name__)
    stem = Path(bf).stem
    (outdir / (stem + ".got")).write_text("\n".join(fd.functions(b, [NAME])[NAME]) + "\n")
    dl = fd.diff_lines(want, got)
    (outdir / (stem + ".diff")).write_text("\n".join(dl) + "\n")
    nw = sum(1 for l in dl if l.lstrip().startswith("-"))
    return "%-28s want=%d got=%d minus=%d difflines=%d %s" % (stem, len(want), len(got), nw, len(dl),
        "MATCH" if fd.keys(want) == fd.keys(got) else "")

with ThreadPoolExecutor(2) as ex:
    for r in ex.map(run, bodies):
        print(r, flush=True)
