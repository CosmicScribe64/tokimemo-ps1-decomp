"""Replay the T-25319 draft of NAME_ENT func_80143580 as a full translation unit (in Docker, repo root).

Installs body.c under a NON_MATCHING guard in a scratch copy of the source file (with the
prototypes of declarations.h), applies the one view the body needs outside itself (the s32
definition of func_80143498, see NOTES.md), compiles it the way funcdiff does and prints the
word counts, the funcdiff row count and the Levenshtein score. Output stays under
build/T-25319 (ignored).

    tools/docker.sh python3 drafts/T-25319/NAME_ENT/func_80143580/replay.py
"""
from pathlib import Path
import sys
sys.path.insert(0, "tools")
import funcloc
import funcdiff
import order_search

packet = Path(__file__).parent
name = "func_80143580"
c = funcloc.locate(name, "NAME_ENT")
out = Path("build/T-25319/NAME_ENT") / name
out.mkdir(parents=True, exist_ok=True)
text = Path(c.src).read_text()
asm = 'INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80143580);\n'
assert asm in text, "func_80143580 is not INCLUDE_ASM in the source file"
view = "void func_80143498(void) {\n"
assert text.count(view) == 1, "func_80143498 definition not found"
guard = ((packet / "declarations.h").read_text() + "#ifdef NON_MATCHING\n" +
         (packet / "body.c").read_text() + "#else\n" + asm + "#endif\n")
text = text.replace(view, "s32 func_80143498(void) {\n").replace(asm, guard)
obj = order_search.compile_variant(c, text, out, True)
expected, problem = funcdiff.expected_object(c)
assert not problem, problem
want = funcdiff.resolved_functions(expected, [name], c)[name]
got = funcdiff.resolved_functions(obj, [name])[name]
lines = funcdiff.diff_lines(want, got)
(out / "diff.txt").write_text("\n".join(lines))
print("target words:", len(want), "draft words:", len(got), "rows:", len(lines),
      "score:", funcdiff.diff_score(want, got))
