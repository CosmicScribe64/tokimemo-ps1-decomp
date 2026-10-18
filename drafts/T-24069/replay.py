"""Replay T-24069 source-only full-TU controls in Docker; production unchanged."""
from pathlib import Path
import argparse
import subprocess
import sys
sys.path.insert(0, 'drafts/T-24011')
from diagnostic import report

variants = {'zero': 'body.c', 'baseline': 'baseline-body.c',
    'duplicated-clear': 'duplicated-clear.c', 'void': 'void-body.c',
    'state-pointer': 'state-pointer.c', 'step-pointer': 'step-pointer.c',
    'shared-value': 'shared-value.c'}
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--variant', choices=variants, default='zero')
args = parser.parse_args()
archive = Path('drafts/T-24069')
p = Path('src/ovl/ETC/8013BEF0.c')
before = p.read_text()
matched = (archive / 'body.c').read_text()
if before.count(matched) != 1:
    raise SystemExit('pinned production body changed')
body = (archive / variants[args.variant]).read_text()
if body.startswith('/* NON_MATCHING:'):
    body = body.split('\n', 1)[1]
candidate = before.replace(matched, body)
if body.count('void func_8013BFFC(void)') == 1:
    marker = 'void func_8013BF34(void)'
    assert candidate.count(marker) == 1
    candidate = candidate.replace(marker, 'void func_8013BFFC(void);\n\n' + marker)
root = Path('build/etc24069/replay') / args.variant
root.mkdir(parents=True, exist_ok=True)
source, obj = root / 'source.c', root / 'source.o'
source.write_text(candidate)
subprocess.run([sys.executable, 'tools/cc.py', str(source), str(obj), 'ido', '5.3'], check=True)
result = subprocess.run([sys.executable, 'tools/funcdiff.py', '--built', str(obj),
    '--no-build', '--resolve', '--version', 'default', '--unit', 'ETC',
    'func_8013BFFC'], capture_output=True, text=True)
report(result, 'func_8013BFFC', root / 'diagnostic.txt')
if p.read_text() != before:
    raise SystemExit('production changed')
print('Private explicit-object replay; normal default proof is recorded separately.')
