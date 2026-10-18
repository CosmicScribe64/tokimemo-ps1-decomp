"""Measure the four T-24031 source-only drafts in full TUs; run in Docker from repo root."""
from pathlib import Path
import re
import sys
sys.path.insert(0, "tools")
import funcdiff as fd
import funcdiff_verify as proof

ROOT = Path("drafts/T-24031")
TARGETS = [("GYOZI", "func_80144018"), ("GYOZI", "func_80144B70"),
           ("SHOUGATU", "func_80141D20"), ("SHOUGATU", "func_8014288C")]
for unit, name in TARGETS:
    c = fd.locate(name, unit)
    directory = Path("build/pairs24031/archive") / unit / name
    directory.mkdir(parents=True, exist_ok=True)
    body = (ROOT / unit / name / "body.c").read_text()
    source = directory / "source.c"
    text = Path(c.src).read_text()
    pattern = re.compile(r'INCLUDE_ASM\([^;]*\b' + name + r'\);')
    changed, count = pattern.subn(body, text)
    if count != 1:
        raise SystemExit("expected one production assembly fallback for " + unit + ":" + name)
    source.write_text(changed)
    edit = None
    if unit == "GYOZI":
        # Archived s32 fall-through view only; the production void header is unchanged.
        edit = lambda pre: pre.replace("void func_80144018(", "s32 func_80144018(")
    obj, _ = proof.compile_unit(c, directory, source=source, edit=edit)
    expected, error = fd.expected_object(c, False)
    if error:
        raise SystemExit(error)
    want = fd.resolved_functions(expected, [name], c)[name]
    got = fd.resolved_functions(obj, [name])[name]
    diff = fd.diff_lines(want, got)
    (directory / "diff.txt").write_text("\n".join(diff) + "\n")
    print("%s:%s cost=%d rows=%d; source-only diagnostic, NOT a match" %
          (unit, name, fd.diff_score(want, got), len(diff)), flush=True)
