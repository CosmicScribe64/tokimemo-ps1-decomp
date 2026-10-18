"""Replay the source-only ETC album-copy checkpoint in Docker, without production edits."""
from pathlib import Path
import argparse
import subprocess
import sys
sys.path.insert(0, 'drafts/T-24011')
from diagnostic import report

name = 'func_80141ABC'
production = Path('src/ovl/ETC/80140F50.c')
original = production.read_text()
fallback = 'INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141ABC);'
if original.count(fallback) != 1:
    raise SystemExit('assigned fallback changed')
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--variant', choices=['indexed', 'cursor'], default='indexed')
args = parser.parse_args()
body = 'body.c' if args.variant == 'indexed' else 'cursor-body.c'
root = Path('build/etc24067/archive') / args.variant
root.mkdir(parents=True, exist_ok=True)
source, obj = root / 'source.c', root / 'source.o'
source.write_text(original.replace(fallback, (Path('drafts/T-24067') / body).read_text()))
subprocess.run([sys.executable, 'tools/cc.py', str(source), str(obj), 'ido', '5.3'], check=True)
result = subprocess.run([sys.executable, 'tools/funcdiff.py', '--built', str(obj),
    '--no-build', '--resolve', '--version', 'default', '--unit', 'ETC', name],
    capture_output=True, text=True)
report(result, name, root / 'diagnostic.txt')
if production.read_text() != original:
    raise SystemExit('production changed')
print('Source-only diagnostic; production unchanged. No default C match claimed.')
