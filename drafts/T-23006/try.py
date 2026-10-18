#!/usr/bin/env python3
"""usage (in docker, repo root): python3 drafts/T-23006/try.py func_X BODY.c OUTPREFIX
BODY.c replaces the guarded block or the INCLUDE_ASM line of func_X in src/main/80047550.c (in memory),
compiled in the full-unit way of funcdiff. Writes OUTPREFIX.got / .want (objdump text of the built and
the original function) and OUTPREFIX.diff, prints the row counts and the aligned score. About 25 s per run
at load 30. drafts/T-23006/shape.py OUTPREFIX shows the opcode-shape hunks (registers masked)."""
import sys, re, tempfile
from pathlib import Path
sys.path.insert(0, "tools")
import funcdiff as fd
import funcdiff_verify as proof

name, body_file, outp = sys.argv[1:4]
body = Path(body_file).read_text().rstrip("\n")
c = fd.locate(name, "main")
text = Path(c.src).read_text()
inc = 'INCLUDE_ASM("asm/nonmatchings/main/80047550", %s);\n' % name
idx = text.find(inc)
assert idx >= 0, "no INCLUDE_ASM for " + name
start = text.rfind("#ifdef NON_MATCHING\n", 0, idx)
if start >= 0 and "#endif" not in text[start:idx]:
    ls = text.rfind("\n", 0, start - 1) + 1
    if text[ls:start].startswith("/* NON_MATCHING"):
        start = ls
    end = idx + len(inc) + len("#endif\n")
else:
    start, end = idx, idx + len(inc)
new = "#ifdef NON_MATCHING\n" + body + "\n#else\n" + inc + "#endif\n"
text = text[:start] + new + text[end:]
d = Path(tempfile.mkdtemp(dir="/tmp"))
(d / "cand.c").write_text(text)
b = proof.compile_unit(c, d, source=str(d / "cand.c"))[0]
e, problem = fd.expected_object(c)
if problem:
    sys.exit("expected: " + problem)
got = fd.resolved_functions(b, [name])[name]
want = fd.resolved_functions(e, [name], c)[name]
Path(outp + ".got").write_text("\n".join(fd.functions(b, [name])[name]) + "\n")
Path(outp + ".want").write_text("\n".join(fd.functions(e, [name])[name]) + "\n")
dl = fd.diff_lines(want, got)
Path(outp + ".diff").write_text("\n".join(dl) + "\n")
print("%s rows want=%d got=%d score=%d difflines=%d %s" % (
    name, len(want), len(got), fd.diff_score(want, got), len(dl), "MATCH" if fd.keys(want) == fd.keys(got) else ""))
