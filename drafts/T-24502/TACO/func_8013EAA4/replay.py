"""Full-TU EAA4 replay in Docker; --control preserves the pre-reconstruction baseline."""
from pathlib import Path
import argparse
import sys
sys.path.insert(0, "tools")
import draft_fixes
import funcloc
import funcdiff
import order_search

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--control", action="store_true")
args = parser.parse_args()
packet = Path(__file__).parent
name = "func_8013EAA4"
c = funcloc.locate(name, "TACO")
label = "control" if args.control else "coordinate"
out = Path("build/T-24502-eaa-replay") / label
out.mkdir(parents=True, exist_ok=True)
(out / "declarations.h").write_text((packet / "declarations.h").read_text())
body = packet / ("body.c" if args.control else "coordinate.c")
source = draft_fixes.fresh_context(dict(func=name, src=str(c.src), scope="TACO:" + name), body.read_text(), out)
obj = order_search.compile_variant(c, source, out, False)
expected, problem = funcdiff.expected_object(c)
assert not problem, problem
want = funcdiff.resolved_functions(expected, [name], c)[name]
got = funcdiff.resolved_functions(obj, [name])[name]
lines = funcdiff.diff_lines(want, got)
(out / "diff.txt").write_text("\n".join(lines))
print(label, "target words", len(want), "draft words", len(got), "aligned cost", funcdiff.diff_score(want, got), "printed rows", len(lines))
