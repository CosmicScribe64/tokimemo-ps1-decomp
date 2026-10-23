"""T-25604 fresh-start round 1 scratch helper: measure a body of NAME_ENT func_80143580.

    tools/docker.sh python3 drafts/T-25604/NAME_ENT/func_80143580/fresh-r1/replay.py BODY.c TAG [--rev1 | --rev1=PARTS]

BODY.c holds the scratch declarations and the function (the format of ../fresh-r1-best.c). The
script puts it under a NON_MATCHING guard in a copy of src/ovl/NAME_ENT/80142A80.c (with the
s32 func_80143498 view), compiles the whole unit the way funcdiff does and prints target/draft
words, funcdiff --resolve rows and the Levenshtein score. With --rev1 it applies the rev1 source
delta first (see rev1.py) and only writes the object; compare it with rev1cmp.py. Output goes to
build/T-25604/fresh-r1/TAG (ignored). No game data is read or written by this script.
"""
from pathlib import Path
import sys
sys.path.insert(0, "tools")
sys.path.insert(0, str(Path(__file__).parent))
import funcloc, funcdiff, order_search
import rev1

name = "func_80143580"
body, tag = Path(sys.argv[1]).read_text(), sys.argv[2]
c = funcloc.locate(name, "NAME_ENT")
text = Path(c.src).read_text()
asm = 'INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80143580);\n'
if asm + "#endif" not in text:  # source still INCLUDE_ASM only: add the guard
    assert text.count(asm) == 1
    view = "void func_80143498(void) {\n"
    if view in text:
        text = text.replace(view, "s32 func_80143498(void) {\n")
    text = text.replace(asm, "#ifdef NON_MATCHING\n" + body + "#else\n" + asm + "#endif\n")
else:  # source already guarded: swap the guarded body
    s = text.rindex("#ifdef NON_MATCHING\n", 0, text.index(asm + "#endif"))
    e = text.index("#else\n" + asm)
    text = text[:s] + "#ifdef NON_MATCHING\n" + body + text[e:]
parts = [a.split("=", 1)[1] for a in sys.argv if a.startswith("--rev1=")]
if "--rev1" in sys.argv or parts:
    text = rev1.apply(text, parts[0] if parts else "034")
out = Path("build/T-25604/fresh-r1") / tag
out.mkdir(parents=True, exist_ok=True)
obj = order_search.compile_variant(c, text, out, True)
if "--rev1" in sys.argv or parts:
    print(tag, "rev1 variant object:", obj)
    sys.exit(0)
expected, problem = funcdiff.expected_object(c)
assert not problem, problem
want = funcdiff.resolved_functions(expected, [name], c)[name]
got = funcdiff.resolved_functions(obj, [name])[name]
lines = funcdiff.diff_lines(want, got)
(out / "diff.txt").write_text("\n".join(lines))
print(tag, "target", len(want), "draft", len(got), "rows", len(lines), "score", funcdiff.diff_score(want, got))
