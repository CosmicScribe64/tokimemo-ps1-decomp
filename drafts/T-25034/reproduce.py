"""Replay an archived research candidate using the whole-TU Docker pipeline."""
import sys
from pathlib import Path
sys.path.insert(0, "tools")
import funcdiff as f
import funcdiff_verify as v

name = "func_80046AC8"
context = f.locate(name, "main")
expected, problem = f.expected_object(context)
assert problem is None, problem
want = f.resolved_functions(expected, [name], context)[name]
variant = sys.argv[1] if len(sys.argv) > 1 else "body.c"
assert variant in ("body.c", "forward-join.c")
body = (Path("drafts/T-25034/main/func_80046AC8") / variant).read_text()
marker = 'INCLUDE_ASM("asm/nonmatchings/main/80046500", func_80046AC8);'
base = Path(context.src).read_text()
assert base.count(marker) == 1
output = Path("build/T-25034-replay") / variant.removesuffix(".c")
output.mkdir(parents=True, exist_ok=True)
source = output / "full.c"
source.write_text(base.replace(marker, body))
obj, _ = v.compile_unit(context, output, source=source)
got = f.resolved_functions(obj, [name], context)[name]
print("target_words", len(want), "candidate_words", len(got),
      "aligned_cost", f.diff_score(want, got),
      "printed_rows", len(f.diff_lines(want, got)))
