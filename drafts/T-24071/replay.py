"""Replay T-24071 source-only full-TU controls in Docker; production unchanged."""
from pathlib import Path
import argparse
import subprocess
import sys
sys.path.insert(0, 'drafts/T-24011')
from diagnostic import report

variants = {'buttons': 'body.c', 'baseline': 'baseline-body.c',
    'state-pointer': 'state-pointer.c', 'selected-capture': 'selected-capture.c',
    'work-fields': 'work-fields.c', 'stored-argument': 'stored-argument.c',
    'status-first': 'status-first.c', 'status-last': 'status-last.c'}
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--variant', choices=variants, default='buttons')
args = parser.parse_args()
p = Path('src/ovl/ETC/80143240.c')
before = p.read_text()
fallback = 'INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144DF0);'
if before.count(fallback) != 1:
    raise SystemExit('assigned fallback changed')
body = (Path('drafts/T-24071') / variants[args.variant]).read_text()
assert body.startswith('/* NON_MATCHING:')
body = body.split('\n', 1)[1]
root = Path('build/etc24071/replay') / args.variant
root.mkdir(parents=True, exist_ok=True)
source, obj = root / 'source.c', root / 'source.o'
source.write_text(before.replace(fallback, body))
subprocess.run([sys.executable, 'tools/cc.py', str(source), str(obj), 'ido', '5.3'], check=True)
result = subprocess.run([sys.executable, 'tools/funcdiff.py', '--built', str(obj),
    '--no-build', '--resolve', '--version', 'default', '--unit', 'ETC',
    'func_80144DF0'], capture_output=True, text=True)
report(result, 'func_80144DF0', root / 'diagnostic.txt')
if p.read_text() != before:
    raise SystemExit('production changed')
print('Private source-only diagnostic, no actual default C match claimed.')
