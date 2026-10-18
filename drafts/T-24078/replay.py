"""Replay T-24078 source-only full-TU controls in Docker; production unchanged."""
from pathlib import Path
import argparse
import subprocess
import sys
sys.path.insert(0, 'drafts/T-24011')
from diagnostic import report

variants = {'shared-temps': 'shared-temps.c', 'baseline': 'baseline.c',
    'sdk-uv': 'sdk-uv.c', 'uv-pair': 'uv-pair.c', 'case-height': 'case-height.c',
    'inline-height': 'inline-height.c', 'tile-width': 'tile-width.c', 'indexed-page': 'indexed-page.c', 'word-coordinates': 'body.c', 'word-uv-pair': 'word-uv-pair.c',
    'case-lifetimes': 'case-lifetimes.c', 'case0-compound': 'case0-compound.c', 'x1-narrow': 'x1-narrow.c', 'origin-first': 'origin-first.c', 'origin-x1': 'origin-x1.c', 'origin-void': 'origin-void.c', 'origin-uv-pair': 'origin-uv-pair.c', 'counter-first': 'counter-first.c', 'shared-x0': 'shared-x0.c'}
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--variant', choices=variants, default='word-coordinates')
args = parser.parse_args()
p = Path('src/ovl/ETC/8013BEF0.c')
before = p.read_text()
fallback = 'INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013D570);'
if before.count(fallback) == 1:
    matched = fallback
else:
    matched = (Path('drafts/T-24078') / 'body.c').read_text().split('\n', 1)[1].rstrip()
    if before.count(matched) != 1:
        raise SystemExit('pinned production body changed')
body = (Path('drafts/T-24078') / variants[args.variant]).read_text()
assert body.startswith(('/* NON_MATCHING:', '/* T-24078:'))
body = body.split('\n', 1)[1]
root = Path('build/etc24078/replay') / args.variant
root.mkdir(parents=True, exist_ok=True)
source, obj = root / 'source.c', root / 'source.o'
private = before.replace(matched, body.rstrip())
if args.variant == 'origin-void':
    private = private.replace('#include "ovl/ETC.h"',
        '#include "ovl/ETC.h"\nvoid func_8013D570(s32 arg0, s32 arg1);', 1)
source.write_text(private)
subprocess.run([sys.executable, 'tools/cc.py', str(source), str(obj), 'ido', '5.3'], check=True)
result = subprocess.run([sys.executable, 'tools/funcdiff.py', '--built', str(obj),
    '--no-build', '--resolve', '--version', 'default', '--unit', 'ETC',
    'func_8013D570'], capture_output=True, text=True)
report(result, 'func_8013D570', root / 'diagnostic.txt')
sys.path.insert(0, 'tools')
import funcdiff as fd
candidate = fd.locate('func_8013D570', 'ETC')
expected, error = fd.expected_object(candidate, False)
if error:
    raise SystemExit(error)
want = fd.resolved_functions(expected, ['func_8013D570'], candidate)['func_8013D570']
got = fd.resolved_functions(obj, ['func_8013D570'])['func_8013D570']
print('Aligned cost=%d; words=%d (original %d); first=%s' %
      (fd.diff_score(want, got), len(got), len(want), got[0][1]))
if p.read_text() != before:
    raise SystemExit('production changed')
print('Private source-only diagnostic, no actual default C match claimed.')
