"""Replay T-24070 source-only full-TU controls in Docker; production unchanged."""
from pathlib import Path
import argparse
import subprocess
import sys
sys.path.insert(0, 'drafts/T-24011')
from diagnostic import report

variants = {'direct-caps': 'body.c', 'baseline': 'baseline-body.c',
    'state-pointer': 'state-pointer.c', 'raw-index-capture': 'raw-index-capture.c'}
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--variant', choices=variants, default='direct-caps')
args = parser.parse_args()
p = Path('src/ovl/ETC/80143240.c')
before = p.read_text()
fallback = 'INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80143240", func_80144F74);'
if before.count(fallback) != 1:
    raise SystemExit('assigned fallback changed')
body = (Path('drafts/T-24070') / variants[args.variant]).read_text()
assert body.startswith('/* NON_MATCHING:')
body = body.split('\n', 1)[1]
root = Path('build/etc24070/replay') / args.variant
root.mkdir(parents=True, exist_ok=True)
source, obj = root / 'source.c', root / 'source.o'
source.write_text(before.replace(fallback, body))
subprocess.run([sys.executable, 'tools/cc.py', str(source), str(obj), 'ido', '5.3'], check=True)
result = subprocess.run([sys.executable, 'tools/funcdiff.py', '--built', str(obj),
    '--no-build', '--resolve', '--version', 'default', '--unit', 'ETC',
    'func_80144F74'], capture_output=True, text=True)
report(result, 'func_80144F74', root / 'diagnostic.txt')
if p.read_text() != before:
    raise SystemExit('production changed')
print('Private source-only diagnostic, no actual default C match claimed.')
