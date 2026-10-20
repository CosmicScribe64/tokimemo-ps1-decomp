"""Replay the source-only T-24526 regional checkpoint in full TU, in Docker."""
from pathlib import Path
import sys
sys.path.insert(0, "tools")
import draft_fixes
import funcloc
import funcdiff
import order_search

packet = Path(__file__).parent
name = "func_80143580"
c = funcloc.locate(name, "NAME_ENT")
out = Path("build/T-24526/NAME_ENT") / name
out.mkdir(parents=True, exist_ok=True)
(out / "declarations.h").write_text((packet / "declarations.h").read_text())
source = draft_fixes.fresh_context(dict(func=name, src=str(c.src), scope="NAME_ENT:" + name), (packet / "body.c").read_text(), out)
obj = order_search.compile_variant(c, source, out, False)
expected, problem = funcdiff.expected_object(c)
assert not problem, problem
want = funcdiff.resolved_functions(expected, [name], c)[name]
got = funcdiff.resolved_functions(obj, [name])[name]
lines = funcdiff.diff_lines(want, got)
(out / "diff.txt").write_text("\n".join(lines))
print("target words:", len(want), "draft words:", len(got), "resolved rows:", len(lines))
