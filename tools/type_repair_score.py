"""T-14700 serial full-TU compile/scoring of archived type repairs (Docker only).

Run after private splat generation. Optional positional function names restrict the
set. Never modifies production C/headers; original-* snapshots remain immutable.
Resolved scores are diagnostic only and do not establish literal-byte matching.
"""
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import autotweak
import bulk_m2c as bulk
import funcdiff
import funcdiff_verify as proof
import funcloc
import order_search

BASE = Path('drafts/T-14500/repairs/types')
MESSAGES = []


def full_command(args):
    """Retain compiler diagnostics without the proof wrapper's twelve-line truncation."""
    result = subprocess.run(args, capture_output=True, text=True)
    if result.stderr:
        MESSAGES.append(result.stderr)
    if args[0] != 'gcc' and result.stdout:
        MESSAGES.append(result.stdout)
    if result.returncode:
        raise ValueError(result.stdout + result.stderr)
    return result.stdout


proof.command = full_command
rows = json.loads((BASE / 'inventory.json').read_text())
for row in rows:
    if len(sys.argv) > 1 and row['func'] not in sys.argv[1:]:
        continue
    MESSAGES.clear()
    folder = BASE / row['unit'] / row['func']
    body = (folder / 'body.c').read_text()
    declarations = (folder / 'declarations.h').read_text().splitlines()
    original = Path(row['src']).read_text()
    # Preserve the first source/header snapshots on repeat scoring runs.
    for name, content in [('baseline-source.c', original),
                          ('baseline-headers.h', bulk.header_text('.', row['unit']))]:
        if not (folder / name).exists():
            (folder / name).write_text(content)
    text = autotweak.rewrite_guard(
        bulk.insert_draft(original, row['func'], body, declarations), row['func'], body)
    (folder / 'context.c').write_text(text)
    unit = funcloc.locate(row['func'], row['src'])
    result = dict(scope=row['scope'], unit=row['unit'], func=row['func'], src=row['src'],
                  size=row['size'], status='failed', score=None,
                  base_commit='4836200960c04659c0705aa566dc4da3b5749e7a', ticket='T-14700',
                  production_changed=False, literal_verified=False,
                  affected_sha1_verified=False, compile_verified=False,
                  scoring='default full-TU resolved comparison; diagnostic only')
    try:
        obj = order_search.compile_variant(unit, text, folder / 'work', False)
        result['compile_verified'] = True
        expected, problem = funcdiff.expected_object(unit)
        if problem:
            raise RuntimeError(problem)
        want = funcdiff.resolved_functions(expected, [row['func']], unit)[row['func']]
        got = funcdiff.resolved_functions(obj, [row['func']])[row['func']]
        score = (0 if funcdiff.keys(want) == funcdiff.keys(got)
                 else len(funcdiff.diff_lines(want, got)))
        result.update(status='candidate-zero' if score == 0 else 'scored', score=score)
        result['remaining_blockers'] = [
            'Literal-byte proof and affected SHA1 required before promotion.' if score == 0
            else 'Nonzero code generation differences; literal-byte proof and affected SHA1 required before promotion.']
        MESSAGES.append('Full-TU compile successful. Default resolved score: %d\n' % score)
    except (ValueError, OSError, RuntimeError, SystemExit) as error:
        result['reason'] = str(error)
        result['remaining_blockers'] = [str(error)]
        MESSAGES.append(str(error))
    for name in ('body.c', 'declarations.h', 'context.c'):
        result[name + '_sha256'] = hashlib.sha256((folder / name).read_bytes()).hexdigest()
    (folder / 'diagnostics.txt').write_text('\n'.join(MESSAGES))
    (folder / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
    shutil.rmtree(folder / 'work', ignore_errors=True)
    print(row['scope'], result['status'], result['score'], flush=True)

catalog = []
for row in rows:
    folder = BASE / row['unit'] / row['func']
    result = json.loads((folder / 'result.json').read_text())
    catalog.append(dict(scope=row['scope'], src=row['src'], path=str(folder),
                        status=result['status'], score=result['score'],
                        compile_verified=result['compile_verified'], literal_verified=False))
(BASE / 'catalog.json').write_text(json.dumps(catalog, indent=2) + '\n')

selected = set(sys.argv[1:])
unknown = selected - {row['func'] for row in rows}
failures = [row for row in catalog if (not selected or row['scope'].split(':')[1] in selected)
            and row['status'] not in ('scored', 'candidate-zero')]
if failures or unknown:
    raise SystemExit('type repairs failed: %d unsuccessful, %d unknown targets' %
                     (len(failures), len(unknown)))
