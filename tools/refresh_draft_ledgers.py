"""Rebuild census ledgers from archived outcomes; never launch new compiler work."""
import json
from collections import Counter
from pathlib import Path
from audit_drafts import alternatives, audit

BASE = Path('drafts/T-14500')
EXTERNAL = ['T-15700', 'T15800', 'T-16000']


def main():
    registry = BASE / 'repairs' / 'final-compilation'
    registry.mkdir(parents=True, exist_ok=True)
    external = []
    for ticket in EXTERNAL:
        catalog = Path('drafts') / ticket / 'catalog.jsonl'
        if not catalog.exists():
            raise SystemExit('Missing completed catalog: ' + str(catalog))
        for line in catalog.read_text().splitlines():
            row = json.loads(line)
            path = row.get('repair_path') or row.get('path') or row.get('archive_path')
            if not path or not row.get('scope'):
                raise SystemExit('Incomplete archive reference: ' + str(row))
            external.append({**row, 'repair_path': path})
    (registry / 'catalog.jsonl').write_text(''.join(json.dumps(r, sort_keys=True) + '\n' for r in external))
    rows = json.loads((BASE / 'inventory.json').read_text())
    options = alternatives(BASE, rows)
    best = []
    reserved = set()
    for path in BASE.glob('*-repair-reservation.json'):
        reserved.update(r['scope'] for r in json.loads(path.read_text()))
    repaired = 0
    for row in rows:
        choices = options[row['scope']]
        if not choices:
            raise SystemExit('Missing scope: ' + row['scope'])
        successes = [c for c in choices if c['result'].get('score') is not None and c['result']['status'] in ('scored', 'candidate-zero')]
        winner = min(successes, key=lambda c: c['result']['score']) if successes else choices[-1]
        initial = next(c for c in choices if c['origin'] == 'initial')
        repaired += bool(successes and initial['result'].get('score') is None)
        best.append(dict(scope=row['scope'], src=row['src'], size=row['size'], guarded=row['guarded'], status=winner['result']['status'], score=winner['result'].get('score'), selected_path=winner['path'], reserved=row['scope'] in reserved, attempts=[dict(path=c['path'], status=c['result']['status'], score=c['result'].get('score'), reason=c['result'].get('reason')) for c in choices]))
    (BASE / 'best-catalog.jsonl').write_text(''.join(json.dumps(r, sort_keys=True) + '\n' for r in best))
    summary = dict(total=len(best), statuses=dict(Counter(r['status'] for r in best)), reserved=len(reserved), repaired=repaired, absent=0)
    (BASE / 'coverage.json').write_text(json.dumps(summary, indent=2) + '\n')
    print(json.dumps(summary))
    print(json.dumps(audit(BASE)))


if __name__ == '__main__':
    main()
