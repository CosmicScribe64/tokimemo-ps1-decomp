"""Replay T-24075 source-only full-TU controls in Docker; production unchanged."""
from pathlib import Path
import argparse
import subprocess
import sys
sys.path.insert(0, 'drafts/T-24011')
from diagnostic import report

variants = {'direct-pair': 'body.c', 'global-loop': 'global-loop.c', 'baseline': 'baseline.c',
    'join-remainder': 'join-remainder.c', 'word-pair': 'word-pair.c',
    'pair-order': 'pair-order.c'}
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--variant', choices=variants, default='direct-pair')
args = parser.parse_args()
p = Path('src/ovl/ETC/8013BEF0.c')
before = p.read_text()
fallback = 'INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013C8EC);'
if before.count(fallback) != 1:
    raise SystemExit('assigned fallback changed')
body = (Path('drafts/T-24075') / variants[args.variant]).read_text()
assert body.startswith('/* NON_MATCHING:')
body = body.split('\n', 1)[1]
root = Path('build/etc24075/replay') / args.variant
root.mkdir(parents=True, exist_ok=True)
source, obj = root / 'source.c', root / 'source.o'
source.write_text(before.replace(fallback, body))
subprocess.run([sys.executable, 'tools/cc.py', str(source), str(obj), 'ido', '5.3'], check=True)
result = subprocess.run([sys.executable, 'tools/funcdiff.py', '--built', str(obj),
    '--no-build', '--resolve', '--version', 'default', '--unit', 'ETC',
    'func_8013C8EC'], capture_output=True, text=True)
report(result, 'func_8013C8EC', root / 'diagnostic.txt')
if p.read_text() != before:
    raise SystemExit('production changed')
print('Private source-only diagnostic, no actual default C match claimed.')
