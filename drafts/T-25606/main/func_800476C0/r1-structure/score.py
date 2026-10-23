#!/usr/bin/env python3
"""usage (in docker, repo root): python3 drafts/T-25116/main/func_800476C0/score.py BODY.c OUTPREFIX [...]
Single-function scorer for main:func_800476C0 (T-25116). BODY.c is the whole C definition; it replaces
the INCLUDE_ASM line (or a guarded block) of src/main/80047550.c in memory, the other bodies are stripped (permute.build_base),
the file is compiled with tools/cc.py (ido 5.3) and compared with the original after relocation
resolving. Writes OUTPREFIX.got/.want/.diff, prints rows and the diff-line count. Several BODY/OUT pairs
run one after another in one Docker start."""
import sys, os, re, subprocess, tempfile
from pathlib import Path
sys.path.insert(0, "tools")
import funcdiff as fd
import permute

NAME = "func_800476C0"
args = sys.argv[1:]
c = fd.locate(NAME, "main")
e, problem = fd.expected_object(c)
if problem:
    sys.exit("expected: " + problem)
want = fd.resolved_functions(e, [NAME], c)[NAME]
for body_file, outp in zip(args[0::2], args[1::2]):
    body = Path(body_file).read_text().rstrip("\n")
    text = Path(c.src).read_text()
    inc = 'INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);\n'
    if "#ifdef NON_MATCHING\nvoid func_800476C0" in text:      # a guarded draft in src/
        a = text.index("#ifdef NON_MATCHING\nvoid func_800476C0")
        b = text.index(inc + "#endif\n", a) + len(inc) + len("#endif\n")
    else:                                                      # plain INCLUDE_ASM
        a = text.index(inc)
        b = a + len(inc)
    # the draft calls func_8004901C before its u8 definition later in the file
    decl = ""
    text = text[:a] + decl + body + "\n" + text[b:]
    d = Path(tempfile.mkdtemp(dir="/tmp"))
    src = d / "cand.c"
    src.write_text(text)
    base = permute.build_base(".", str(src), NAME)
    (d / "base.c").write_text(base)
    obj = d / "base.o"
    r = subprocess.run(["python3", "tools/cc.py", str(d / "base.c"), str(obj), "ido", "5.3"],
                       capture_output=True, text=True)
    if r.returncode:
        print(body_file, "COMPILE ERROR", (r.stdout + r.stderr)[-1500:])
        continue
    got = fd.resolved_functions(str(obj), [NAME])[NAME]
    Path(outp + ".got").write_text("\n".join(fd.functions(str(obj), [NAME])[NAME]) + "\n")
    Path(outp + ".want").write_text("\n".join(fd.functions(e, [NAME])[NAME]) + "\n")
    dl = fd.diff_lines(want, got)
    Path(outp + ".diff").write_text("\n".join(dl) + "\n")
    print("%s want=%d got=%d score=%d difflines=%d %s" % (
        body_file, len(want), len(got), fd.diff_score(want, got), len(dl),
        "MATCH" if fd.keys(want) == fd.keys(got) else ""))
