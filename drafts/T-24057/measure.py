"""Measure the T-24057 source-only pair in private full TUs; run in Docker."""
from pathlib import Path
import re
import sys
sys.path.insert(0, "tools")
import funcdiff as fd
import funcdiff_verify as proof

production = Path("src/ovl/ENDING/80133C10.c")
original = production.read_text()
for name in ("func_80134B18", "func_80134C2C"):
    directory = Path("build/ending24057/archive") / name
    directory.mkdir(parents=True, exist_ok=True)
    body = Path("drafts/T-24057/ENDING", name, "body.c").read_text()
    text = original
    if name == "func_80134B18":
        pattern = r'#ifdef NON_MATCHING\n/\* NON_MATCHING: T-10520[^#]+#else\nINCLUDE_ASM\([^;]*func_80134B18\);\n#endif'
    else:
        pattern = r'INCLUDE_ASM\([^;]*\b' + name + r'\);'
    text, count = re.subn(pattern, lambda _: body, text)
    if count != 1:
        raise SystemExit("expected exactly one assigned target replacement: " + name)
    source = directory / "source.c"
    source.write_text(text)
    c = fd.locate(name, "ENDING")
    obj, _ = proof.compile_unit(c, directory, non_matching=False, source=source)
    expected, error = fd.expected_object(c, False)
    if error:
        raise SystemExit(error)
    want = fd.resolved_functions(expected, [name], c)[name]
    got = fd.resolved_functions(obj, [name])[name]
    diff = fd.diff_lines(want, got)
    (directory / "diff.txt").write_text("\n".join(diff) + "\n")
    print("ENDING:%s cost=%d rows=%d; source-only diagnostic, NOT a match" %
          (name, fd.diff_score(want, got), len(diff)), flush=True)
    for label, instructions in (("expected", want), ("draft", got)):
        frames = [display for key, display in instructions[:10] if "addiu sp,sp," in display]
        print(label + " frame: " + "; ".join(frames), flush=True)
if production.read_text() != original:
    raise SystemExit("production changed during measurement")
print("Production unchanged; only assigned targets replaced in private source copies.")
