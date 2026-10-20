"""T-24501 inverse serializer full-TU diagnostic replay; no production edits.
Run in Docker from repository root. Resolved score is not literal proof.
"""
import argparse
import queue
import sys
from pathlib import Path
sys.path.insert(0, "tools")
import funcdiff as f
import funcdiff_verify as v

ap = argparse.ArgumentParser()
ap.add_argument("--scratch", default="build/scratch/t24501-save-inverse")
ap.add_argument("--int-return", action="store_true")
ap.add_argument("--signed-counter", action="store_true")
ap.add_argument("--signed-address", action="store_true")
args = ap.parse_args()
name = "func_80054FC0"
c = f.locate(name, "main")
d = Path(args.scratch)
d.mkdir(parents=True, exist_ok=True)
body = Path("drafts/T-24501/main/func_80054FC0.c").read_text()
ret = "s32" if args.int_return else "void"
body = body.replace("void " + name, ret + " " + name)
if args.signed_counter:
    body = body.replace("u32 offset;", "s32 offset;")
if args.signed_address:
    body = body.replace("image = arg0 * 0xF00 + D_80123120;",
                        "image = (u8 *)(arg0 * 0xF00 + (s32)D_80123120);")
base = Path(c.src).read_text()
base = base.replace('#include "game.h"', '#include "game.h"\n' + ret + " " + name + "(s32 arg0);")
marker = 'INCLUDE_ASM("asm/nonmatchings/main/80053650", ' + name + ');'
assert base.count(marker) == 1
src = d / "variant.c"
src.write_text(base.replace(marker, body))
obj, _ = v.compile_unit(c, d, source=src)
e, _ = f.expected_object(c)
want = f.resolved_functions(e, [name])[name]
got = f.resolved_functions(obj, [name])[name]
print("resolved_cost", f.diff_score(want, got), "target_words", len(want), "candidate_words", len(got))
(d / "resolved.diff").write_text("\n".join(f.diff_lines(want, got)) + "\n")
