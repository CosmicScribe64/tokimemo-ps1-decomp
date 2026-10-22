"""Test named late-zero producers in the full MAIN translation unit, in Docker.

Generated sources, objects and game-derived comparisons stay in ignored build/.
No source or compiler files are changed. Run with named variants as arguments.
"""
import json
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, "tools")
import funcdiff as fd
import funcdiff_verify as fv

name = "func_8006D7EC"
c = fd.locate(name, "main")
base = Path(c.src).read_text()
start = base.index("void " + name + "(void)")
next_function = base.index("\ns32 func_8006EA40", start)
guard_end = base.find("\n#else", start, next_function)
end = guard_end if guard_end != -1 else next_function
body = base[start:end]
body = body.replace("    /* FAKE: T-25036, unsigned < 0 is zero, but folds after uopt so idx keeps a register dependency. */\n", "")
body = body.replace("    idx = (u32)D_800E6280.unk_F5F < 0;", "    idx = 0;", 1)
body = body.replace("        /* FAKE: T-25036, this unreachable identity clears as1's zero substitution; it emits no code. */\n", "")
body = body.replace("        if (idx) { idx = idx + ((u32)day < 0); }\n", "", 1)
variants = {
    "baseline": "0",
    "global-byte": "(u32)D_800E6280.unk_F5F < 0",
    "live-day": "(u32)day < 0",
    "counter-address": "(u32)&idx < 0",
    "global-address": "(u32)&D_800E6280 < 0",
    "narrow-shift": "(u8)((u32)day << 8)",
    "narrow-mask": "(u8)((u32)day & 0xff00)",
    "widen-shift": "((u32)(u8)day) >> 8",
    "mixed-sign-xor": "(u32)D_800B6740 ^ *(s32 *)&D_800B6740",
    "mixed-sign-sub": "(u32)D_800B6740 - *(s32 *)&D_800B6740",
    "late-product": "((u32)D_800E6280.unk_F5F < 0) * (u32)day",
    "late-shift-count": "((u32)D_800E6280.unk_F5F < 0) << (day & 31)",
}
producer = "(u32)D_800E6280.unk_F5F < 0"
bodies = {
    "late-dead-def": body.replace("    idx = 0;", "    idx = " + producer + ";\n    if ((u32)D_800B6740 < 0) { idx = day; }", 1),
    "late-dead-inner": body.replace("    idx = 0;", "    idx = " + producer + ";", 1).replace("        idx++;", "        if ((u32)D_800B6740 < 0) { idx = day; }\n        idx++;", 1),
    "late-dead-back": body.replace("    idx = 0;", "    idx = " + producer + ";\ncounter_entry:", 1).replace("        idx++;", "        if ((u32)D_800B6740 < 0) goto counter_entry;\n        idx++;", 1),
    "late-one-loop": body.replace("    idx = 0;", "    idx = " + producer + ";", 1).replace("        idx++;", "        do { idx++; } while ((u32)D_800B6740 < 0);", 1),
    "carrier-dead-inner": body.replace("    idx = 0;", "    idx = " + producer + ";", 1).replace("        idx++;", "        if (idx) { idx = day; }\n        idx++;", 1),
    "carrier-dead-back": body.replace("    idx = 0;", "    idx = " + producer + ";\ncounter_entry:", 1).replace("        idx++;", "        if (idx) goto counter_entry;\n        idx++;", 1),
    "carrier-dead-forward": body.replace("    idx = 0;", "    idx = " + producer + ";\n    if (idx) goto counter_alt;\n    goto counter_test;\ncounter_alt:\n    idx = day;\ncounter_test:", 1),
    "carrier-one-loop": body.replace("    idx = 0;", "    idx = " + producer + ";", 1).replace("        idx++;", "        do { idx++; } while (idx < 1);", 1),
}
for label, expression in {
    "late-add": "idx + ((u32)day < 0)",
    "late-or": "idx | ((u32)day < 0)",
    "late-xor": "idx ^ ((u32)day < 0)",
    "late-zero": "(u32)day < 0",
}.items():
    bodies["carrier-forward-" + label] = bodies["carrier-dead-forward"].replace("    idx = day;", "    idx = " + expression + ";")
    bodies["carrier-inner-" + label] = bodies["carrier-dead-inner"].replace("{ idx = day; }", "{ idx = " + expression + "; }")
expected, _ = fd.expected_object(c)
want = fd.resolved_functions(expected, [name], c)[name]
for label in sys.argv[1:] or list(variants) + list(bodies):
    directory = Path("build/t25036") / label
    directory.mkdir(parents=True, exist_ok=True)
    source = directory / "unit.c"
    candidate = bodies[label] if label in bodies else body.replace("    idx = 0;", "    idx = " + variants[label] + ";", 1)
    source.write_text(base[:start] + candidate + base[end:])
    obj, _ = fv.compile_unit(c, directory, source=source)
    got = fd.resolved_functions(obj, [name], c)[name]
    rows = fd.diff_lines(want, got)
    (directory / "diff.txt").write_text("\n".join(rows) + "\n")
    dump = subprocess.check_output([
        "mips-linux-gnu-objdump", "-drz", "--disassemble=" + name, str(obj)
    ], text=True)
    (directory / "disassembly.txt").write_text(dump)
    result = dict(target_words=len(want), candidate_words=len(got),
                  printed_lines=len(rows), aligned_cost=fd.diff_score(want, got))
    (directory / "measurement.json").write_text(json.dumps(result, indent=2) + "\n")
    print(label, result, flush=True)
assert Path(c.src).read_text() == base
