"""Capture optimized ucode for NAME_ENT controls inside Docker."""
from pathlib import Path
import sys
sys.path.insert(0, 'tools')
import regdiff
import cvt_pass as cv
regdiff.WRAPPER = regdiff.WRAPPER.replace('import os, shutil, sys', 'import os, shutil, sys, subprocess').replace('os.execv(real, [real] + argv)', 'rc = subprocess.call([real] + argv)\nshutil.copy(files[1], os.environ["REGDIFF_UCODE"] + ".after")\nsys.exit(rc)')
name = 'func_8013AF78'
for label in sys.argv[1:]:
    directory = Path('build/T-25062') / name / label / 'capture'
    directory.mkdir(exist_ok=True)
    result = regdiff.compile_draft(str(Path.cwd()), name, str(directory.parent/'variant.c'), '/scratch/variants/trace', str(directory), unit='NAME_ENT')
    assert result.returncode == 0, result.log[-3000:]
    records = cv.parse((directory/'draft.B.after').read_bytes())
    for a, b in cv._procedures(records):
        if any(name.encode() in r.extra for r in records[a:b+1]):
            rows = ['%03d %-5s %s %r' % (n, cv.OPS[r.opc], ' '.join('%08x' % w for w in r.words), r.extra) for n,r in enumerate(records[a:b+1])]
            (directory/'ucode.txt').write_text('\n'.join(rows)+'\n')
    print(label, 'captured', flush=True)
