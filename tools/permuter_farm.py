#!/usr/bin/env python3
"""CPU-only, resumable near-miss searches (T-9810); run inside Docker.

collect scans wave scratch directories, NON_MATCHING definitions and T-0018 rows.
run uses private build roots, never the live src/ tree. tools/permuter_farm.sh
starts one persistent Docker container per job. See wiki/decompile-workflow.md.
"""
import argparse
from collections import Counter
from contextlib import contextmanager
import fcntl
import hashlib
import json
import multiprocessing
import os
from pathlib import Path
import re
import shutil
import signal
import socket
import subprocess
import sys
import time

import permute
import srcscan

DEFAULT_SCRATCH = Path('/private/tmp/claude-501/-Users-lila-Documents-Projects-tokimemo-decomp/43264b98-bb59-4c1b-ad99-a2084d55fcf0/scratchpad')
IDENT = re.compile(r'^[A-Za-z_]\w*$')
_scorer_ready = False


def atomic_json(path, data):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_name(path.name + '.%s.tmp' % os.getpid())
    with tmp.open('w') as out:
        json.dump(data, out, indent=2, sort_keys=True)
        out.write('\n')
        out.flush()
        os.fsync(out.fileno())
    os.replace(tmp, path)


def load_json(path, default):
    return json.loads(Path(path).read_text()) if Path(path).exists() else default


def bodies(text):
    for name in sorted(permute.signature_names(text)):
        span = permute.function_range(text, name)
        if span:
            yield name, text[span[0]:span[1]]


def score_of(row):
    """Rank by measured instruction differences; never mistake row index n for a score."""
    status = str(row.get('status') or row.get('state') or '').lower()
    if any(word in status for word in ('error', 'failed', 'repair')):
        return None
    for field in ('last_best_score', 'diff_count', 'diffs', 'score'):
        value = row.get(field)
        if isinstance(value, (int, float)) and value >= 0:
            return int(value)
    diff = row.get('diff', '')
    if isinstance(diff, str):
        if 'ERROR' in diff:
            return None
        if re.search(r': MATCH\b', diff):
            return 0
        if 'DIFF' in diff:
            return sum(bool(re.match(r'^\s+[+-][0-9a-fA-F]{8}\b', line)) for line in diff.splitlines()) or None
    for field in ('edits', 'lines'):
        value = row.get(field)
        if isinstance(value, (int, float)) and value >= 0:
            return int(value)
    if row.get('state') == 'diff' and isinstance(row.get('n'), int):
        return row['n']
    return None


def path_unit(value):
    value = str(value or '').replace('\\', '/')
    if 'src/ovl/' in value:
        return value.split('src/ovl/', 1)[1].split('/')[0].removesuffix('.c')
    if 'src/main/' in value or re.fullmatch(r'[0-9A-Fa-f]{8}', value):
        return 'main'
    if '/' in value and IDENT.fullmatch(value.split('/')[0]):
        return value.split('/')[0]
    return None


def resolve_draft(value, wave, scratch, root):
    """Accept host paths, /scratch aliases and wave-relative draft paths."""
    value = str(value)
    if value.startswith('/scratch/'):
        value = str(scratch / value[len('/scratch/'):])
    elif '/scratchpad/' in value:
        value = str(scratch / value.split('/scratchpad/', 1)[1])
    choices = [Path(value), wave / value, scratch / value, root / value]
    return next((p.resolve() for p in choices if p.is_file()), None)


def collect(root, scratch, farm):
    """Build one scoped queue entry per unmatched function; retain the best available draft."""
    root, scratch, farm = Path(root).resolve(), Path(scratch).resolve(), Path(farm).resolve()
    farm.mkdir(parents=True, exist_ok=True)
    targets, units = {}, {}
    for src in sorted((root / 'src').rglob('*.c')):
        text = src.read_text(errors='replace')
        unit = path_unit(str(src))
        if not unit:
            continue
        for _, name in srcscan.INCLUDE_ASM_RE.findall(text):
            targets[(unit, name)] = src
            units.setdefault(name, set()).add(unit)
    known_units = {key[0] for key in targets}
    ranked, drafts, warnings = [], [], []
    for wave in sorted(scratch.glob('w*-*')):
        if not wave.is_dir():
            continue
        for path in sorted(wave.rglob('*.jsonl')):
            if any(part in ('work', 'workers', 'build') for part in path.relative_to(wave).parts):
                continue
            for n, line in enumerate(path.read_text(errors='replace').splitlines(), 1):
                try:
                    row = json.loads(line)
                except ValueError:
                    warnings.append('%s:%d: invalid JSON' % (path, n))
                    continue
                if not isinstance(row, dict):
                    continue
                name = row.get('function') or row.get('func') or row.get('name')
                if not isinstance(name, str):
                    continue
                unit = row.get('unit')
                if not isinstance(unit, str) or unit not in known_units:
                    unit = path_unit(unit)
                    if unit not in known_units:
                        unit = None
                unit = unit or path_unit(row.get('src') or row.get('file') or row.get('stem') or row.get('label'))
                if ':' in name:
                    unit, name = name.split(':', 1)
                if unit and '/' in unit:
                    unit = path_unit(unit)
                if not unit and len(units.get(name, ())) == 1:
                    unit = next(iter(units[name]))
                if (unit, name) not in targets:
                    continue
                draft, body = None, None
                for field in ('draft_path', 'best_path', 'source', 'mapped', 'body', 'draft', 'c'):
                    value = row.get(field)
                    if not isinstance(value, str):
                        continue
                    if '\n' in value or '{' in value:
                        span = permute.function_range(value, name)
                        if span:
                            body = value[span[0]:span[1]]
                            break
                    else:
                        draft = resolve_draft(value, wave, scratch, root)
                        if draft:
                            text = draft.read_text(errors='replace')
                            span = permute.function_range(text, name)
                            if span:
                                body = text[span[0]:span[1]]
                                break
                ranked.append((unit, name, body, draft, score_of(row), str(path)))
        for path in sorted(wave.rglob('*.c')):
            # Ignore generated compiler inputs and outputs; collect durable drafts only.
            if any(part in ('build', 'work', 'workers') or part.startswith('output-')
                   for part in path.relative_to(wave).parts) or path.name.endswith('.sjis.c'):
                continue
            text = path.read_text(errors='replace')
            hint = None
            header = re.search(r'#\s*include\s+"ovl/(\w+)\.h"', text)
            if header:
                hint = header.group(1)
            for name, body in bodies(text):
                unit = hint
                if not unit and len(units.get(name, ())) == 1:
                    unit = next(iter(units[name]))
                if (unit, name) in targets:
                    drafts.append((unit, name, body, path, None, str(path)))
    # T-0018 rows scope ambiguous drafts that lack overlay headers.
    cases = root / 'wiki/data/t0018-cases.md'
    if cases.exists():
        for line in cases.read_text().splitlines():
            cols = [c.strip() for c in line.split('|')]
            if len(cols) < 5 or not cols[1] or cols[1] == 'file':
                continue
            name = cols[2].strip('` ')
            unit = path_unit(cols[1]) or cols[1]
            if (unit, name) not in targets or 'draft' not in line.lower():
                continue
            for m in re.finditer(r'(?:scratchpad[/ ]+)?(w\d+-[^ /`;)]+)[/ ]+([^ `;,)]+\.c)', line):
                path = scratch / m.group(1) / m.group(2)
                if path.is_file():
                    span = permute.function_range(path.read_text(), name)
                    if span:
                        body = path.read_text()[span[0]:span[1]]
                        drafts.append((unit, name, body, path, None, str(cases)))
    for (unit, name), src in targets.items():
        text = src.read_text()
        if 'NON_MATCHING' in text:
            span = permute.function_range(text, name)
            if span:
                drafts.append((unit, name, text[span[0]:span[1]], src, None, 'NON_MATCHING'))
    # A rank record can supply a score without a body; associate only the same draft path
    # when supplied, otherwise the same wave and function. Never cross overlay units.
    for i, (unit, name, body, path, score, origin) in enumerate(drafts):
        scores = [s for u, f, b, p, s, o in ranked if (u, f) == (unit, name) and s is not None
                  and ((p and p == path) or (not b and Path(o).parent == path.parent))]
        if scores:
            drafts[i] = (unit, name, body, path, min(scores), origin)
    candidates = {}
    for unit, name, body, path, score, origin in ranked + drafts:
        if not body or not IDENT.fullmatch(name) or not IDENT.fullmatch(unit):
            continue
        key = (unit, name)
        item = (score if score is not None else float('inf'), -(path.stat().st_mtime if path else Path(origin).stat().st_mtime))
        if key not in candidates or item < candidates[key][0]:
            candidates[key] = (item, body, score, origin)
    queue = []
    for (unit, name), (_, body, score, origin) in candidates.items():
        digest = hashlib.sha256(body.encode()).hexdigest()
        path = farm / 'drafts' / unit / (name + '-' + digest[:12] + '.c')
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(body + '\n')
        queue.append(dict(function=name, unit=unit, draft_path=str(path), last_best_score=score,
                          initial_diff_count=score, score_kind='instruction-diff',
                          fingerprint=digest, source=str(targets[(unit, name)].relative_to(root)), origin=origin))
    queue.sort(key=lambda c: (c['last_best_score'] if c['last_best_score'] is not None else float('inf'), c['unit'], c['function']))
    with state_lock(farm):
        atomic_json(farm / 'queue.json', queue)
        atomic_json(farm / 'collection.json', dict(queue_size=len(queue), warnings=warnings))
    return queue


@contextmanager
def state_lock(farm):
    Path(farm).mkdir(parents=True, exist_ok=True)
    with (Path(farm) / 'state.lock').open('a') as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        yield


def owner_alive(farm, owner):
    with (Path(farm) / ('owner-' + owner + '.lock')).open('a') as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            return True
        return False


def claim(farm, owner, budget, max_budget, patience):
    with state_lock(farm):
        state = load_json(Path(farm) / 'state.json', {})
        queue = load_json(Path(farm) / 'queue.json', [])
        # Preserve the collector's instruction-diff ordering. Stored upstream scores
        # drive retry budgets; they are a different metric from instruction counts.
        for cand in queue:
            key = cand['unit'] + ':' + cand['function']
            rec = state.get(key, {})
            if rec.get('fingerprint') != cand['fingerprint']:
                if rec.get('status') == 'running' and owner_alive(farm, rec['owner']):
                    continue
                rec = dict(fingerprint=cand['fingerprint'], attempts=0, stagnant=0, status='pending', history=[])
            if rec.get('status') == 'running' and not owner_alive(farm, rec['owner']):
                rec['status'] = 'pending'
            state[key] = rec
        eligible = [(c, state[c['unit'] + ':' + c['function']]) for c in queue
                    if state[c['unit'] + ':' + c['function']]['status'] not in ('running', 'won', 'hopeless')]
        if not eligible:
            atomic_json(Path(farm) / 'state.json', state)
            return None
        cand, rec = eligible[0]
        seconds = min(max_budget, rec.get('next_budget', budget))
        rec.update(status='running', owner=owner, started=time.time(), budget=seconds)
        atomic_json(Path(farm) / 'state.json', state)
        return cand, seconds


def finish(farm, cand, result, budget, max_budget, patience):
    key = cand['unit'] + ':' + cand['function']
    with state_lock(farm):
        state = load_json(Path(farm) / 'state.json', {})
        rec = state[key]
        previous = rec.get('permuter_score')
        best = result.get('score')
        baseline = result.get('baseline_score')
        improved = best is not None and best < (previous if previous is not None else baseline if baseline is not None else best)
        rec['attempts'] += 1
        rec['history'].append(dict(result, budget=rec['budget'], improved=improved))
        if best is not None:
            rec['permuter_score'] = min(previous, best) if previous is not None else best
            queue = load_json(Path(farm) / 'queue.json', [])
            for item in queue:
                if (item['unit'], item['function'], item['fingerprint']) == (cand['unit'], cand['function'], cand['fingerprint']):
                    item.update(last_best_score=rec['permuter_score'], score_kind='permuter')
            atomic_json(Path(farm) / 'queue.json', queue)
        rec['stagnant'] = 0 if improved else rec['stagnant'] + 1
        rec['next_budget'] = min(max_budget, rec['budget'] * 2) if improved else rec['budget']
        rec['status'] = 'won' if result.get('win') else 'hopeless' if rec['stagnant'] >= patience else 'pending'
        if result.get('error'):
            rec['status'] = 'hopeless'
        atomic_json(Path(farm) / 'state.json', state)


def command(cmd, cwd, log, timeout=900):
    with Path(log).open('a') as out:
        bounded_run(cmd, cwd=cwd, stdout=out, stderr=subprocess.STDOUT, check=True, timeout=timeout)


def bounded_run(cmd, timeout=900, check=False, **kwargs):
    """Bound build helpers and terminate their children on timeout or interruption."""
    proc = subprocess.Popen(cmd, start_new_session=True, **kwargs)
    try:
        stdout, stderr = proc.communicate(timeout=timeout)
    except BaseException:
        permute.kill_group(proc, signal.SIGKILL)
        proc.wait()
        raise
    result = subprocess.CompletedProcess(cmd, proc.returncode, stdout, stderr)
    if check:
        result.check_returncode()
    return result


def private_root(root, farm, worker):
    """Copy only build inputs; preserve private incremental outputs between resumes."""
    dest = Path(farm) / 'workers' / worker / 'repo'
    dest.mkdir(parents=True, exist_ok=True)
    for name in ('src', 'include', 'config', 'tools'):
        shutil.copytree(Path(root) / name, dest / name, dirs_exist_ok=True,
                        ignore=shutil.ignore_patterns('__pycache__', '*.pyc'))
    shutil.copy2(Path(root) / 'configure.py', dest / 'configure.py')
    # Read-only mounts and symlinks prevent raw source modification. prepare_disc.py
    # sees the populated files directory and does not need to extract the game again.
    for name in ('disc', 'game'):
        path = dest / name
        if not path.exists():
            path.symlink_to((Path(root) / name).resolve(), target_is_directory=True)
    command([sys.executable, 'configure.py'], dest, dest.parent / 'prepare.log')
    stamps = ['build/split.stamp'] + ['build/ovl/%s.stamp' % u for u in srcscan.overlay_names(dest)]
    command(['ninja', '-j', '1'] + stamps, dest, dest.parent / 'prepare.log')
    return dest


def verify(root, private, cand, output, farm, verifier=None):
    """Verify in the isolated real build and export only its target body, preserving layout.

    All exported bodies are conservatively FAKE-labelled: automatic syntax inspection
    cannot establish whether a dummy local, cast or source line arrangement is plausible.
    A reviewer may later replace this annotation with a precise explanation.
    """
    root, private, farm = Path(root).resolve(), Path(private).resolve(), Path(farm).resolve()
    if private == root or not private.is_relative_to(farm / 'workers'):
        raise ValueError('verification root must be a private worker copy')
    if not (private / cand['source']).resolve().is_relative_to(private / 'src'):
        raise ValueError('verification source must be inside the private src tree')
    if not IDENT.fullmatch(cand['unit']) or not IDENT.fullmatch(cand['function']):
        raise ValueError('invalid function or unit in queue')
    meta = dict(func=cand['function'], unit=cand['unit'], src=cand['source'])
    if verifier is None:
        ok, report = permute.verify_candidate(str(private), meta, str(output), runner=bounded_run)
    else:
        ok, report = verifier(str(private), meta, str(output))
    if not ok:
        return None, report
    text = (Path(output) / 'source.c').read_text()
    span = permute.function_range(text, cand['function'])
    if not span:
        return None, report + ['missing target definition']
    body = text[span[0]:span[1]]
    comment = '/* Verified by funcdiff --resolve and unit sha1; T-9810. */\n'
    comment += ('/* FAKE: automated draft/permuter result preserves compiler-sensitive\n'
                ' * instruction/register ordering; may include dummy locals, casts or line-layout\n'
                ' * tricks. Preserve layout; manual plausibility review required before\n'
                ' * application. Conservative annotation, not a proven trick. T-9810. */\n')
    win = farm / 'wins' / cand['unit'] / (cand['function'] + '.c')
    win.parent.mkdir(parents=True, exist_ok=True)
    win.write_text(comment + body + '\n')
    atomic_json(win.with_suffix('.json'), dict(report=report, source=cand['source'], candidate=str(output)))
    return str(win), report


def search(root, private, farm, cand, seconds, worker):
    start = time.monotonic()
    run_dir = Path(farm) / 'searches' / cand['unit'] / cand['function'] / cand['fingerprint'][:12]
    log = run_dir / ('run-%s-%d.log' % (worker, time.time_ns()))
    run_dir.mkdir(parents=True, exist_ok=True)
    result = dict(started=time.time(), score=None, baseline_score=None)
    try:
        source = private / cand['source']
        original = source.read_text()
        body = Path(cand['draft_path']).read_text()
        context = permute.replace_function(original, cand['function'], body)
        if context is None:
            raise ValueError('target missing from private source')
        scratch = run_dir / 'context.c'
        scratch.write_text(context)
        # setup on each resume refreshes absolute compile paths; use the best prior
        # output as a seed without losing accumulated outputs/history.
        old = permute.list_outputs(str(run_dir))
        if old and old[0][0] > 0:
            text = (Path(old[0][2]) / 'source.c').read_text()
            span = permute.function_range(text, cand['function'])
            if span:
                scratch.write_text(permute.replace_function(original, cand['function'], text[span[0]:span[1]]))
        os.chdir(private)
        command([sys.executable, 'tools/permute.py', 'setup', cand['function'], '--unit', cand['unit'],
                 '--src', str(scratch), '--out', str(run_dir)], private, log)
        # Score the starting C using the same permuter scorer; output collection
        # omits unchanged base C, including a base which already scores zero.
        result['baseline_score'] = base_score(run_dir)
        if result['baseline_score'] == 0:
            base_dir = run_dir / 'base-zero'
            base_dir.mkdir(exist_ok=True)
            shutil.copy2(run_dir / 'base.c', base_dir / 'source.c')
            zero_dirs = [base_dir]
            result['score'] = 0
        else:
            with log.open('a') as out:
                outputs = permute.run(str(run_dir), seconds, 1,
                                      progress=lambda line: print(line, file=out, flush=True))
            result['score'] = outputs[0][0] if outputs else result['baseline_score']
            zero_dirs = [Path(p) for score, _, p in outputs if score == 0]
        result['verification'] = []
        for output in zero_dirs:
            win, report = verify(root, private, cand, output, farm)
            result['verification'].append(dict(output=str(output), report=report))
            if win:
                result['win'] = win
    except (Exception, SystemExit) as exc:
        result['error'] = str(exc)
    result['elapsed_seconds'] = round(time.monotonic() - start, 3)
    result['log'] = str(log)
    return result


def base_score(directory):
    """Evaluate even an unchanged score-zero seed using upstream's exact scorer."""
    global _scorer_ready
    if not _scorer_ready:
        permute.install_permuter_compat()
        _scorer_ready = True
    from src.compiler import Compiler
    from src.scorer import Scorer
    directory = Path(directory)
    compiler = Compiler(str(directory / 'compile.sh'), show_errors=False, debug_mode=False)
    obj = compiler.compile((directory / 'base.c').read_text(), show_errors=True)
    if obj is None:
        raise ValueError('base C did not compile')
    scorer = Scorer(str(directory / 'target.o'), stack_differences=True, algorithm='difflib',
                    debug_mode=False, ign_branch_targets=False, objdump_command=None)
    try:
        return scorer.score(obj)[0]
    finally:
        os.unlink(obj)


def worker(args, number):
    owner = '%s-%s-%s' % (socket.gethostname(), os.getpid(), number)
    farm, root = Path(args.farm).resolve(), Path(args.root).resolve()
    farm.mkdir(parents=True, exist_ok=True)
    # Held for the whole worker lifetime: OS releases the lock on crashes, so the
    # next run can recover claims immediately without stealing work from live jobs.
    with (farm / ('owner-' + owner + '.lock')).open('a') as owner_lock:
        fcntl.flock(owner_lock, fcntl.LOCK_EX)
        slot_lock = (farm / ('worker-' + str(number) + '.lock')).open('a')
        try:
            fcntl.flock(slot_lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError:
            raise RuntimeError('worker slot %s already active' % number)
        private = private_root(root, farm, str(number))
        deadline = time.monotonic() + args.wall_time if args.wall_time else float('inf')
        while time.monotonic() < deadline:
            item = claim(farm, owner, args.time, args.max_time, args.patience)
            if item is None:
                break
            cand, seconds = item
            if deadline != float('inf'):  # no --wall-time: keep the claimed budget
                seconds = max(1, min(seconds, int(deadline - time.monotonic())))
            result = search(root, private, farm, cand, seconds, str(number))
            result['actual_budget'] = seconds
            finish(farm, cand, result, args.time, args.max_time, args.patience)
            print(json.dumps(dict(function=cand['function'], unit=cand['unit'], **result)), flush=True)


def positive(value):
    value = int(value)
    if value <= 0:
        raise argparse.ArgumentTypeError('must be positive')
    return value


def report(farm):
    """Save all candidate timings, with a short console summary."""
    farm = Path(farm)
    with state_lock(farm):
        queue = load_json(farm / 'queue.json', [])
        state = load_json(farm / 'state.json', {})
    rows = []
    for key, rec in state.items():
        for attempt, result in enumerate(rec.get('history', []), 1):
            rows.append(dict(function=key, attempt=attempt, status=rec['status'], **result))
    # A new draft resets its search state. Worker journals retain earlier timings
    # across fingerprints, crashes and recollection; merge by unique run log.
    recorded = {row.get('log') for row in rows}
    for path in sorted(farm.glob('worker-*.log')):
        for line in path.read_text(errors='replace').splitlines():
            if not line.startswith('{'):
                continue
            try:
                result = json.loads(line)
            except ValueError:
                continue
            if not isinstance(result, dict) or not {'function', 'unit', 'started', 'elapsed_seconds', 'log'} <= result.keys():
                continue
            if result['log'] in recorded:
                continue
            key = result.pop('unit') + ':' + result.pop('function')
            rows.append(dict(function=key, status=state.get(key, {}).get('status', 'archived'), **result))
            recorded.add(result['log'])
    rows.sort(key=lambda r: r['started'])
    attempts = Counter()
    for row in rows:
        attempts[row['function']] += 1
        row['attempt'] = attempts[row['function']]
    winning = {row['function'] for row in rows if row.get('win')} | {key for key, rec in state.items() if rec['status'] == 'won'}
    summary = dict(queue_size=len(queue), wins=len(winning),
                   attempts=len(rows), counts=dict(Counter(r['status'] for r in state.values())))
    atomic_json(farm / 'report.json', dict(summary=summary, candidates=rows))
    lines = ['# T-9810 farm run', '', 'Queue: %d; verified wins: %d; completed attempts: %d.' %
             (summary['queue_size'], summary['wins'], summary['attempts']), '',
             '| Function | Attempt | Seconds | Budget | Best score | Verified win |',
             '|---|---:|---:|---:|---:|---|']
    for row in rows:
        lines.append('| %s | %d | %.3f | %s | %s | %s |' %
                     (row['function'], row['attempt'], row['elapsed_seconds'], row.get('actual_budget', row.get('budget')),
                      row.get('score'), 'yes' if row.get('win') else 'no'))
    (farm / 'report.md').write_text('\n'.join(lines) + '\n')
    return summary


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('command', choices=('collect', 'run', 'status', 'report'))
    ap.add_argument('--root', default='.')
    ap.add_argument('--scratch', default=str(DEFAULT_SCRATCH))
    ap.add_argument('--farm', default=None)
    ap.add_argument('--time', type=positive, default=1200, help='initial seconds per candidate')
    ap.add_argument('--max-time', type=positive, default=4800, help='maximum extended budget')
    ap.add_argument('-j', '--jobs', type=positive, default=max(1, (os.cpu_count() or 1) // 2))
    ap.add_argument('--wall-time', type=positive, help='stop worker after this many search seconds')
    ap.add_argument('--patience', type=positive, default=2, help='non-improving attempts before skipping')
    ap.add_argument('--worker-id', default=None, help='one worker in a persistent Docker container')
    args = ap.parse_args(argv)
    if args.worker_id is not None and not re.fullmatch(r'[A-Za-z0-9_-]+', args.worker_id):
        ap.error('worker ID must contain only letters, digits, underscores or hyphens')
    if not Path('/.dockerenv').exists():
        ap.error('run inside Docker (tools/permuter_farm.sh)')
    args.farm = str(Path(args.farm or Path(args.scratch) / 'r9-farm').resolve())
    if args.command == 'collect':
        print('queue size: %d' % len(collect(args.root, args.scratch, args.farm)))
    elif args.command == 'status':
        with state_lock(args.farm):
            queue = load_json(Path(args.farm) / 'queue.json', [])
            state = load_json(Path(args.farm) / 'state.json', {})
        active = {key: round(time.time() - rec['started'], 1) for key, rec in state.items() if rec['status'] == 'running'}
        print(json.dumps(dict(queue_size=len(queue), wins=sum(r['status'] == 'won' for r in state.values()),
                              counts=dict(Counter(r['status'] for r in state.values())), active_seconds=active), indent=2))
    elif args.command == 'report':
        print(json.dumps(report(args.farm)))
    elif args.worker_id is not None:
        worker(args, args.worker_id)
    else:
        processes = [multiprocessing.Process(target=worker, args=(args, str(i))) for i in range(args.jobs)]
        for proc in processes:
            proc.start()
        for proc in processes:
            proc.join()
        return int(any(proc.exitcode != 0 for proc in processes))
    return 0


if __name__ == '__main__':
    sys.exit(main())
