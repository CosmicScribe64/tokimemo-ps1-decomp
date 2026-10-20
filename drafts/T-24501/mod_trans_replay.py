"""T-24501 typed font upload source-only full-TU diagnostic; run in Docker."""
import argparse
import queue
import sys
from pathlib import Path
sys.path.insert(0, "tools")
import funcdiff as f
import funcdiff_verify as v
ap = argparse.ArgumentParser()
ap.add_argument("--source", default="drafts/T-24501/main/mod_trans_vram.c")
ap.add_argument("--scratch", default="build/scratch/t24501-mod-trans")
args = ap.parse_args()
name = "mod_trans_vram"
c = f.locate(name, "main")
d = Path(args.scratch)
d.mkdir(parents=True, exist_ok=True)
base = Path(c.src).read_text()
marker = 'INCLUDE_ASM("asm/nonmatchings/main/8004C3F0", ' + name + ');'
assert base.count(marker) == 1
src = d / "variant.c"
src.write_text(base.replace(marker, Path(args.source).read_text()))
obj, _ = v.compile_unit(c, d, source=src)
e, _ = f.expected_object(c)
want = f.resolved_functions(e, [name])[name]
got = f.resolved_functions(obj, [name])[name]
print("resolved_cost", f.diff_score(want, got), "target_words", len(want), "candidate_words", len(got))
(d / "resolved.diff").write_text("\n".join(f.diff_lines(want, got)) + "\n")
