"""Recreate private T-24518 controls exactly; run in project Docker."""
import argparse
import hashlib
import json
from pathlib import Path

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('names', nargs='*')
parser.add_argument('--all', action='store_true')
args = parser.parse_args()
root = Path(__file__).parent
data = json.loads((root / 'controls.json').read_text())
baseline = (root / 'baseline.c').read_text()


def sha(text):
    return hashlib.sha256(text.encode()).hexdigest()


assert sha(baseline) == data['baseline_sha256']
out = Path('build/T-24518/replay')
out.mkdir(parents=True, exist_ok=True)
names = list(data['controls']) if args.all else args.names
if not names:
    parser.error('specify a control name or --all')
for name in names:
    control = data['controls'][name]
    source_base = Path(control['base']).read_text() if 'base' in control else baseline
    expected_base = control.get('base_sha256', data['baseline_sha256'])
    assert sha(source_base) == expected_base, name
    lines = source_base.splitlines(keepends=True)
    for start, stop, replacement in reversed(control['edits']):
        lines[start:stop] = [replacement]
    source = ''.join(lines)
    assert sha(source) == control['sha256'], name
    path = out / (name + '.c')
    path.write_text(source)
    print(path)
