"""Capture normal and optimized ucode for the flag dispatcher, Docker only."""
import sys
from pathlib import Path
sys.path.insert(0, "tools")
import regdiff
import cvt_pass as cv

regdiff.WRAPPER = regdiff.WRAPPER.replace(
    "import os, shutil, sys", "import os, shutil, sys, subprocess"
).replace("os.execv(real, [real] + argv)",
          'rc = subprocess.call([real] + argv)\nshutil.copy(files[1], os.environ["REGDIFF_UCODE"] + ".after")\nsys.exit(rc)')
label = sys.argv[1]
directory = Path("build/t25040") / label / "trace"
directory.mkdir(exist_ok=True)
result = regdiff.compile_draft(str(Path.cwd()), "func_8006D138",
    str(directory.parent / "unit.c"), "/tracer/trace", str(directory), unit="main")
if result.returncode:
    print(result.log[-3000:])
    sys.exit(result.returncode)
for suffix in ("", ".after"):
    records = cv.parse((directory / ("draft.B" + suffix)).read_bytes())
    for a, b in cv._procedures(records):
        if any(b"func_8006D138" in r.extra for r in records[a:b+1]):
            rows = ["%03d %-5s %s %r" % (n, cv.OPS[r.opc],
                " ".join("%08x" % w for w in r.words), r.extra)
                for n, r in enumerate(records[a:b+1])]
            (directory / ("ucode" + suffix + ".txt")).write_text("\n".join(rows) + "\n")
print(label, "captured")
