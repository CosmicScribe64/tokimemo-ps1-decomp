"""Replay T-24073 source-only full-TU controls in Docker; production unchanged."""
from pathlib import Path
import argparse
import subprocess
import sys
sys.path.insert(0, 'drafts/T-24011')
from diagnostic import report

variants = {'zero': 'body.c', 'post-id': 'post-id.c', 'for-loop': 'for-loop.c', 'baseline': 'baseline.c',
    'word-x': 'word-x.c', 'word-values': 'word-values.c',
    'sdk-width': 'sdk-width.c', 'direct-id': 'direct-id.c',
    'early-y': 'early-y.c', 'early-id': 'early-id.c'}
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--variant', choices=variants, default='zero')
args = parser.parse_args()
p = Path('src/ovl/ETC/80143240.c')
before = p.read_text()
matched = (Path('drafts/T-24073') / 'body.c').read_text()
if before.count(matched) != 1:
    raise SystemExit('pinned production body changed')
body = (Path('drafts/T-24073') / variants[args.variant]).read_text()
if body.startswith('/* NON_MATCHING:'):
    body = body.split('\n', 1)[1]
root = Path('build/etc24073/replay') / args.variant
root.mkdir(parents=True, exist_ok=True)
source, obj = root / 'source.c', root / 'source.o'
source.write_text(before.replace(matched, body))
header = Path('include/ovl/ETC.h').read_text()
old = 'void func_8014407C(void);'
new = 'void func_8014407C();'
if old in header:
    assert header.count(old) == 1
    header = header.replace(old, new)
else:
    assert header.count(new) == 1
private_header = root / 'ovl' / 'ETC.h'
private_header.parent.mkdir(exist_ok=True)
private_header.write_text(header)
subprocess.run([sys.executable, 'tools/cc.py', str(source), str(obj), 'ido', '5.3'], check=True)
result = subprocess.run([sys.executable, 'tools/funcdiff.py', '--built', str(obj),
    '--no-build', '--resolve', '--version', 'default', '--unit', 'ETC',
    'func_8014407C'], capture_output=True, text=True)
report(result, 'func_8014407C', root / 'diagnostic.txt')
if p.read_text() != before:
    raise SystemExit('production changed')
print('Private source-only diagnostic, no actual default C match claimed.')
