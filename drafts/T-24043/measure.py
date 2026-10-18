"""Measure the T-24043 source-only draft in its full TU; run in Docker from repo root."""
from pathlib import Path
import re
import sys
sys.path.insert(0, "tools")
import funcdiff as fd
import funcdiff_verify as proof

name = "func_80139CC8"
c = fd.locate(name, "MASTER")
directory = Path("build/master24043/archive")
directory.mkdir(parents=True, exist_ok=True)
body = Path("drafts/T-24043/MASTER/func_80139CC8/body.c").read_text()
source = directory / "source.c"
text = Path(c.src).read_text()
changed, count = re.subn(r'INCLUDE_ASM\([^;]*\b' + name + r'\);', lambda _: body, text)
if count != 1:
    raise SystemExit("expected one production assembly fallback")
source.write_text(changed)
obj, _ = proof.compile_unit(c, directory, source=source)
expected, error = fd.expected_object(c, False)
if error:
    raise SystemExit(error)
want = fd.resolved_functions(expected, [name], c)[name]
got = fd.resolved_functions(obj, [name])[name]
diff = fd.diff_lines(want, got)
(directory / "diff.txt").write_text("\n".join(diff) + "\n")
print("MASTER:%s cost=%d rows=%d; source-only diagnostic, NOT a match" %
      (name, fd.diff_score(want, got), len(diff)), flush=True)
if Path(c.src).read_text() != text:
    raise SystemExit("production source changed during measurement")
print("Production source unchanged; all prior matching bodies preserved.")
