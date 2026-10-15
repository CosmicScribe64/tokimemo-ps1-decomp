#!/usr/bin/env python3
"""T-14500: bounded serial declaration/type fallback after the complete default census.

Run in Docker after exhaustive_drafts.py finishes. Initial artifacts are immutable.
Reserved DATE/type-worker scopes are excluded. Failed targets first retry the latest
mechanical repairs, then failures try no-context and no-unknown-inference. Failed retained guards
are regenerated separately, preserving the original guarded source in the initial archive.
Every attempt has its own complete context and outcome under repairs/automatic/.
No production source is changed and no zero-score candidate is promoted.
"""
import argparse
from collections import Counter
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

BASE = Path('drafts/T-14500')
FAIL = {'failed', 'timeout', 'worker-error'}


def read_result(base, row):
    path = base / row['unit'] / row['func'] / 'result.json'
    return json.loads(path.read_text()) if path.exists() else None


def prepare(base, rows):
    base.mkdir(parents=True, exist_ok=True)
    manifest = base / 'inventory.json'
    if not manifest.exists():
        manifest.write_text(json.dumps(rows, indent=2) + '\n')
    shutil.copyfile(BASE / 'environment.json', base / 'environment.json')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--timeout', type=int, default=120)
    args = parser.parse_args()
    rows = json.loads((BASE / 'inventory.json').read_text())
    initial = {r['scope']: read_result(BASE, r) for r in rows}
    absent = [scope for scope, result in initial.items() if result is None]
    if absent:
        raise SystemExit('default census incomplete: %d absent outcomes; do not run concurrently' % len(absent))
    reserved = set()
    for path in BASE.glob('*-repair-reservation.json'):
        reserved.update(r['scope'] for r in json.loads(path.read_text()))
    selected = [r for r in rows if initial[r['scope']]['status'] in FAIL and r['scope'] not in reserved]
    attempts = {r['scope']: [] for r in rows}
    for label, variant in [('mechanical', 'default'), ('no-context', 'no-context'),
                           ('no-unk-inference', 'no-unk-inference')]:
        base = BASE / 'repairs' / 'automatic' / label
        if not selected:
            continue
        prepare(base, selected)
        env = dict(os.environ, TOKIMEMO_DRAFT_BASE=str(base), TOKIMEMO_DRAFT_VARIANT=variant,
                   TOKIMEMO_REGENERATE_GUARDS='1')
        print('REPAIR PASS', label, len(selected), 'targets', flush=True)
        subprocess.run([sys.executable, 'tools/exhaustive_drafts.py', '--timeout', str(args.timeout)],
                       env=env, check=True)
        remaining = []
        for row in selected:
            got = read_result(base, row)
            if got is None:
                raise SystemExit('repair outcome missing for '+row['scope'])
            attempts[row['scope']].append(dict(path=str(base / row['unit'] / row['func']), result=got))
            if got['status'] in FAIL:
                remaining.append(row)
        selected = remaining
    # Independently owned source/type repairs are separate, context-qualified alternatives.
    for catalog in (BASE / 'repairs').rglob('catalog.jsonl'):
        if 'automatic' in catalog.parts:
            continue
        for line in catalog.read_text().splitlines():
            alternative = json.loads(line)
            scope = alternative.get('scope')
            path = alternative.get('repair_path')
            if scope in attempts and path:
                attempts[scope].append(dict(path=path, result=alternative))
    best = []
    for row in rows:
        original = initial[row['scope']]
        options = [dict(path=str(BASE / row['unit'] / row['func']), result=original)] + attempts[row['scope']]
        successful = [o for o in options if o['result'].get('score') is not None]
        winner = min(successful, key=lambda o: o['result']['score']) if successful else options[-1]
        best.append(dict(scope=row['scope'], src=row['src'], size=row['size'], guarded=row['guarded'],
                         status=winner['result']['status'], score=winner['result'].get('score'),
                         selected_path=winner['path'], reserved=row['scope'] in reserved,
                         attempts=[dict(path=o['path'], status=o['result']['status'],
                                        score=o['result'].get('score'),reason=o['result'].get('reason')) for o in options]))
    (BASE / 'best-catalog.jsonl').write_text(''.join(json.dumps(r, sort_keys=True)+'\n' for r in best))
    counts = dict(Counter(r['status'] for r in best))
    summary = dict(total=len(best), statuses=counts, reserved=len(reserved),
                   repaired=sum(initial[r['scope']]['status'] in FAIL and r['score'] is not None for r in best),
                   absent=0)
    (BASE / 'coverage.json').write_text(json.dumps(summary,indent=2)+'\n')
    print('REPAIR COVERAGE', summary, flush=True)


if __name__ == '__main__':
    main()
