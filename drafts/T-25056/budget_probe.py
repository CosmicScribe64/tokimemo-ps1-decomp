#!/usr/bin/env python3
"""Docker-only scratch AS1 budget diagnostic; never installs a compiler or build rule.

Pass the read-only external T-24047 reconstruction directory as the first argument.
The default diagnostic is print-only; T25056_BUDGET controls a counterfactual run.
"""
from pathlib import Path
import hashlib
import subprocess
import sys

external = Path(sys.argv[1])
out = Path('build/t25056/budget')
out.mkdir(parents=True, exist_ok=True)
source = (external/'as1-reference.c').read_text()
expected = '905927751a79aba82d332bdce254b2b30a4312ae14a4108897e11b48487d17a1'
assert hashlib.sha256(source.encode()).hexdigest() == expected
anchor = 'a0 = (int)t4 < (int)t5;\nif (a0 != 0) {\nMEM_U32(sp + 104) = t6;\ngoto L42b23c;}'
assert source.count(anchor) == 1
replacement = '''fprintf(stderr, "T25056_BUDGET dst=%u src=%u old_count=%u limit=%u\\n",
    MEM_U32(s4+64), MEM_U32(s3+64), t5, t4);
if (getenv("T25056_BUDGET")) t4 = atoi(getenv("T25056_BUDGET"));
''' + anchor
source = source.replace(anchor, replacement)
(out/'as1-budget.c').write_text(source)
subprocess.run(['gcc','-std=c11','-O1','-fno-strict-aliasing',
    '-I'+str(external/'recomp'), str(out/'as1-budget.c'),
    str(external/'recomp/build/5.3/libc_impl_53.o'),
    str(external/'recomp/build/5.3/version_info.o'), '-lm',
    '-o',str(out/'as1-budget')], check=True)
print('built scratch diagnostic', flush=True)
