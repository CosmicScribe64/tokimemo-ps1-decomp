"""Replay T-24077 source-only full-TU controls in Docker; production unchanged."""
from pathlib import Path
import argparse
import subprocess
import sys
sys.path.insert(0, 'drafts/T-24011')
from diagnostic import report

variants = {'shared-state': 'body.c', 'baseline': 'baseline.c',
    'unsigned-state': 'unsigned-state.c', 'direct-pair': 'direct-pair.c',
    'natural-default': 'natural-default.c', 'return-view': 'return-view.c',
    'shared-value': 'shared-value.c'}
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--variant', choices=variants, default='shared-state')
args = parser.parse_args()
p = Path('src/ovl/ETC/8013BEF0.c')
before = p.read_text()
fallback = 'INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013BEF0", func_8013CF2C);'
if before.count(fallback) == 1:
    matched = fallback
else:
    matched = (Path('drafts/T-24077') / 'body.c').read_text().split('\n', 1)[1].rstrip()
    if before.count(matched) != 1:
        raise SystemExit('pinned production body changed')
body = (Path('drafts/T-24077') / variants[args.variant]).read_text()
assert body.startswith(('/* NON_MATCHING:', '/* T-24077:'))
body = body.split('\n', 1)[1]
root = Path('build/etc24077/replay') / args.variant
root.mkdir(parents=True, exist_ok=True)
source, obj = root / 'source.c', root / 'source.o'
private = before.replace(matched, body.rstrip())
if 's32 func_8013CF2C(void) {' in body:
    target_forward = 's32 func_8013CF2C(void);'
elif 'void func_8013CF2C(void) {' in body:
    target_forward = 'void func_8013CF2C(void);'
else:
    raise SystemExit('unknown target signature')
private = private.replace('#include "ovl/ETC.h"', '#include "ovl/ETC.h"\n' + target_forward + '\nvoid func_8013E1DC(s32 arg0);\nvoid func_8013F320(s32 arg0);')
source.write_text(private)
subprocess.run([sys.executable, 'tools/cc.py', str(source), str(obj), 'ido', '5.3'], check=True)
result = subprocess.run([sys.executable, 'tools/funcdiff.py', '--built', str(obj),
    '--no-build', '--resolve', '--version', 'default', '--unit', 'ETC',
    'func_8013CF2C'], capture_output=True, text=True)
report(result, 'func_8013CF2C', root / 'diagnostic.txt')
if p.read_text() != before:
    raise SystemExit('production changed')
print('Private source-only diagnostic, no actual default C match claimed.')
