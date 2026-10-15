"""Rebuild all source-draft census ledgers; fail closed before claiming full compilation.

Run in Docker after all four final repair catalogs are integrated. No compiler work
is launched. Original outcomes remain immutable; the registry normalizes external
compiler-proof schemas for the existing audit reader.
"""
import hashlib
import json
from collections import Counter
from pathlib import Path
from audit_drafts import alternatives, audit, manual_catalogs

BASE = Path('drafts/T-14500')
EXTERNAL = {'T-15700': 28, 'T-15800': 23, 'T-16000': 13}
SUCCESS = {'compiled', 'scored', 'candidate-zero'}


def checked(row):
    path = row.get('repair_path') or row.get('path') or row.get('archive_path')
    if not path or not row.get('scope'):
        raise ValueError('Incomplete archive reference: ' + str(row))
    folder = Path(path)
    result_file = folder / 'result.json'
    complete = json.loads(result_file.read_text()) if result_file.exists() else {}
    row = {**complete, **row, 'repair_path': path}
    if row.get('status') not in SUCCESS or row.get('score') is None:
        raise ValueError('Missing compiled/scored alternative: ' + row['scope'])
    for name in ('body.c', 'context.c'):
        if not (folder / name).is_file():
            raise ValueError('Missing source context: ' + str(folder / name))
    hashes = dict(row.get('hashes', {}))
    for filename in ('body.c', 'declarations.h', 'context.c', 'full-context.c'):
        if row.get(filename + '_sha256'):
            hashes[filename] = row[filename + '_sha256']
    for field, filename in [('body_hash', 'body.c'), ('draft_hash', 'body.c'),
                            ('context_hash', 'context.c'), ('body_c_sha256', 'body.c'),
                            ('context_c_sha256', 'context.c'),
                            ('declarations_hash', 'declarations.h'),
                            ('declarations_h_sha256', 'declarations.h'),
                            ('diagnostics_hash', 'diagnostics.txt'),
                            ('full_context_hash', 'full-context.c'),
                            ('preprocessed_context_hash', 'full-context.c')]:
        if row.get(field):
            hashes[filename] = row[field]
    for name, expected in hashes.items():
        file = folder / name
        if not file.is_file() or hashlib.sha256(file.read_bytes()).hexdigest() != expected:
            raise ValueError('Archive hash mismatch: ' + str(file))
    for name, expected in row.get('header_hashes', {}).items():
        snapshot = BASE / 'baseline' / name
        if not snapshot.is_file() or hashlib.sha256(snapshot.read_bytes()).hexdigest() != expected:
            raise ValueError('Frozen header hash mismatch: ' + str(snapshot))
    row['archive_status'] = row['status']
    row['status'] = 'candidate-zero' if row['score'] == 0 else 'scored'
    return row


def main():
    rows = json.loads((BASE / 'inventory.json').read_text())
    inventory = {r['scope'] for r in rows}
    assert len(rows) == len(inventory) == 1066, 'Frozen1066-scope inventory changed'
    pending = {r['scope'] for r in json.loads((BASE / 'remaining77-manifest.json').read_text())}
    assert len(pending) == 77 and pending <= inventory
    external = []
    for ticket, count in EXTERNAL.items():
        catalog = Path('drafts') / ticket / 'catalog.jsonl'
        entries = [checked(json.loads(line)) for line in catalog.read_text().splitlines() if line.strip()]
        assert len(entries) == len({r['scope'] for r in entries}) == count, ticket
        external.extend(entries)
    internal = [checked(json.loads(line)) for line in
                (BASE / 'repairs/t15900/catalog.jsonl').read_text().splitlines() if line.strip()]
    assert len(internal) == len({r['scope'] for r in internal}) == 13
    final = external + internal
    assert len(final) == len({r['scope'] for r in final}) == 77
    assert {r['scope'] for r in final} == pending, 'Final repair scopes differ from reserved77'
    registry = BASE / 'repairs/final-compilation'
    registry.mkdir(parents=True, exist_ok=True)
    (registry / 'catalog.jsonl').write_text(''.join(json.dumps(r, sort_keys=True) + '\n' for r in external))
    options = alternatives(BASE, rows)
    best = []
    reserved = set()
    for path in BASE.glob('*-repair-reservation.json'):
        reserved.update(r['scope'] for r in json.loads(path.read_text()))
    repaired = 0
    for row in rows:
        choices = options[row['scope']]
        successes = [c for c in choices if c['result'].get('score') is not None and
                     c['result']['status'] in ('scored', 'candidate-zero')]
        assert successes, 'No compiling alternative: ' + row['scope']
        winner = min(successes, key=lambda c: c['result']['score'])
        assert (Path(winner['path']) / 'body.c').is_file()
        assert (Path(winner['path']) / 'context.c').is_file()
        initial = next(c for c in choices if c['origin'] == 'initial')
        repaired += initial['result'].get('score') is None
        best.append(dict(scope=row['scope'], src=row['src'], size=row['size'], guarded=row['guarded'],
                         status=winner['result']['status'], score=winner['result']['score'],
                         selected_path=winner['path'], reserved=row['scope'] in reserved,
                         attempts=[dict(path=c['path'], status=c['result']['status'],
                                        score=c['result'].get('score'), reason=c['result'].get('reason')) for c in choices]))
    (BASE / 'best-catalog.jsonl').write_text(''.join(json.dumps(r, sort_keys=True) + '\n' for r in best))
    summary = dict(total=len(best), statuses=dict(Counter(r['status'] for r in best)),
                   reserved=len(reserved), repaired=repaired, absent=0)
    (BASE / 'coverage.json').write_text(json.dumps(summary, indent=2) + '\n')
    quality = audit(BASE)
    assert sum(quality['statuses'].values()) == 1066
    assert not quality['statuses'].get('uncompilable') and not quality['statuses'].get('unattempted')
    compact = []
    for line in (BASE / 'quality-audit.jsonl').read_text().splitlines():
        item = json.loads(line)
        choice = next(c for c in item['alternatives'] if c['path'] == item['selected_path'])
        compact.append({k: item[k] for k in ('scope', 'src', 'size', 'status', 'selected_path', 'flags')} |
                       {'score': choice['score'], 'origin': choice['origin']})
    (BASE / 'best-quality-catalog.jsonl').write_text(''.join(json.dumps(r, sort_keys=True) + '\n' for r in compact))
    previous = json.loads((BASE / 'stopping-point.json').read_text())
    previous.update(compiler_coverage=summary, quality=quality, stopped=True,
                    final_compilation_repairs=77, repaired_initial_failures=repaired,
                    manual_repairs=len({(r['scope'], r['repair_path']) for r in manual_catalogs(BASE)}),
                    remaining='Every frozen scope has a compiling alternative; %d selected drafts retain reconstruction-review flags. Audit-clear does not prove semantics.' % quality['statuses'].get('compiling-review-needed', 0),
                    matching_claim='No production matching claim follows from draft counts or resolved scores.')
    (BASE / 'stopping-point.json').write_text(json.dumps(previous, indent=2) + '\n')
    print(json.dumps(summary))
    print(json.dumps(quality))


if __name__ == '__main__':
    main()
