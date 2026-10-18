"""Replay only T-24068 private full-TU controls in Docker; production stays assembly."""
from pathlib import Path
import argparse
import subprocess
import sys
sys.path.insert(0, 'drafts/T-24011')
from diagnostic import report

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--variant', choices=['baseline', 'calls-comma', 'neighbor-selector', 'target-return', 'discard-final'], default='baseline')
args = parser.parse_args()
p = Path('src/ovl/ETC/8013FF80.c')
before = p.read_text()
fallback = 'INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_80140AE4);'
if before.count(fallback) != 1:
    raise SystemExit('assigned fallback changed')
body = Path('drafts/T-24068/body.c').read_text()
assert body.startswith('/* NON_MATCHING:')
body = body.split('\n', 1)[1]
if args.variant == 'calls-comma':
    old = '    func_80140BC4();\n    func_80140DC8();\n    func_80140E80();'
    assert body.count(old) == 1
    body = body.replace(old, '    func_80140BC4(), func_80140DC8(), func_80140E80();')
elif args.variant == 'neighbor-selector':
    body = '/* FAKE DIAGNOSTIC: DC through existing D8+4; aggregate unknown. T-24068. */\n' + body.replace('D_801500DC', '((s32 *)&D_801500D8)[1]')
elif args.variant == 'target-return':
    body = '/* FAKE DIAGNOSTIC: s32 fallthrough return view; original type unproven. T-24068. */\n' + body.replace('void func_80140AE4(void)', 's32 func_80140AE4(void)')
elif args.variant == 'discard-final':
    body = body.replace('    func_80140E80();', '    (void)func_80140E80();')
root = Path('build/etc24068') / args.variant
root.mkdir(parents=True, exist_ok=True)
source, obj = root / 'source.c', root / 'source.o'
source.write_text(before.replace(fallback, body))
subprocess.run([sys.executable, 'tools/cc.py', str(source), str(obj), 'ido', '5.3'], check=True)
result = subprocess.run([sys.executable, 'tools/funcdiff.py', '--built', str(obj),
    '--no-build', '--resolve', '--version', 'default', '--unit', 'ETC', 'func_80140AE4'], capture_output=True, text=True)
report(result, 'func_80140AE4', root / 'diagnostic.txt')
if p.read_text() != before:
    raise SystemExit('production changed')
print('Source-only diagnostic; production unchanged. No C match claimed.')
