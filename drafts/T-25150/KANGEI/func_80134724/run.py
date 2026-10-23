"""run.py NAME SRC.c [NAME SRC.c ...] : full-TU compile of each variant with ugen/as1 capture
and the probe as1 (AS1TRACE); prints words/score/rows for func_80134724 and the
bb17 scan/move lines. Captures go to build/t25150/cap/NAME (ignored)."""
import os, sys, subprocess, json, re
from pathlib import Path
sys.path.insert(0, '/work/tools'); os.chdir('/work')
import cc, funcdiff as fd, funcdiff_verify as fv
F = 'func_80134724'
c = fd.locate(F, 'KANGEI'); e, _ = fd.expected_object(c)
want = fd.resolved_functions(e, [F], c)[F]
args = sys.argv[1:]
for name, src in zip(args[::2], args[1::2]):
    p = Path('build/t25150/cap') / name; p.mkdir(parents=True, exist_ok=True)
    env = dict(os.environ, AS1CAP=str(p), AS1TRACE='/work/build/t25150/as1-probe')
    (p / 'unit.c').write_text(fv.preprocess_draft(src, True))
    r = subprocess.run(['python3', '-c', 'import sys;sys.path.insert(0,"/work/tools");import cc;'
                        'cc.SHIMS["as1"]="/work/build/as1tr/shim.py";cc.SHIMS["ugen"]="/work/build/t25037/ugen_cap.py";'
                        'cc.main(["cc.py",sys.argv[1],sys.argv[2],"ido","5.3"])', str(p / 'unit.c'), str(p / 'unit.o')],
                       env=env, capture_output=True, text=True)
    if r.returncode:
        print('==', name, 'COMPILE FAIL', r.stderr[-600:]); continue
    got = fd.resolved_functions(str(p / 'unit.o'), [F], c)[F]
    rows = fd.diff_lines(want, got)
    print('== %s: words %d/%d score %d rows %d' % (name, len(got), len(want), fd.diff_score(want, got), len(rows)))
    for l in rows[:int(os.environ.get('ROWS', '12'))]: print('   ', l)
    if os.environ.get('TR'):
        out = subprocess.run(['python3', 'build/t25150/fn.py', str(p / 'unit.c'), str(p / 'xtr.log'), F],
                             capture_output=True, text=True).stdout
        (p / 'fn.txt').write_text(out)
        for l in out.splitlines():
            if re.search(os.environ.get('TR'), l): print('   |', l[:160])
