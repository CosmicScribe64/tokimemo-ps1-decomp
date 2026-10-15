#!/usr/bin/env python3
"""T-14500: serial, resumable full-TU draft census; never edits production sources.

Run inside the project Docker image: python3 tools/exhaustive_drafts.py.
Every INCLUDE_ASM in configured game sources is inventoried, including guarded drafts.
A separate bounded subprocess handles each target. Source/header snapshots, raw m2c C,
cleaned body, declarations, full translation unit, diagnostics and hashes are retained.
Scores use order_search's resolved full-TU comparison; zero is an unverified candidate.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import signal
import subprocess
import sys
import time

sys.path.insert(0, str(Path(__file__).resolve().parent))
import autotweak
import bulk_m2c as bulk
import funcloc
import funcdiff
import m2c
import order_search
import srcscan

BASE = Path(os.environ.get('TOKIMEMO_DRAFT_BASE', 'drafts/T-14500'))
VARIANT = os.environ.get('TOKIMEMO_DRAFT_VARIANT', 'default')
OUT = BASE / 'catalog.jsonl'


def digest(text):
    return hashlib.sha256(text.encode()).hexdigest()


def save(path, text):
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + '.tmp-' + str(os.getpid()))
    temporary.write_text(text)
    os.replace(temporary, path)


def census():
    rows = []
    for c in srcscan.c_files('.'):
        if not c.src.exists():
            continue
        original = c.src.read_text()
        guards = {g.func for g in autotweak.guards(original)}
        for _, func in srcscan.INCLUDE_ASM_RE.findall(original):
            asm = c.nonmatchings / (func + '.s')
            got = srcscan.nonmatching_size(asm) if asm.exists() else None
            rows.append(dict(unit=c.unit, src=str(c.src), func=func,
                             size=got[1] if got else None, guarded=func in guards,
                             source_hash=digest(original), scope=c.unit+':'+func))
    assert len({r['scope'] for r in rows}) == len(rows)
    return sorted(rows, key=lambda r: (r['unit'], r['src'], r['func']))


def process(row):
    started = time.monotonic()
    folder = BASE / row['unit'] / row['func']
    folder.mkdir(parents=True, exist_ok=True)
    original = Path(row['src']).read_text()
    headers = bulk.header_text('.', row['unit'])
    known = {}
    decl_lines = []
    messages = []
    body = ''
    row = dict(row, status='failed', score=None, environment_hash=digest((BASE / 'environment.json').read_text()), harness_hash=digest(Path(__file__).read_text()), variant='guarded' if row['guarded'] else VARIANT)
    try:
        save(folder / 'phase.txt', 'generation\n')
        if row['guarded']:
            guard = next(g for g in autotweak.guards(original) if g.func == row['func'])
            body = original[guard.body:guard.else_start].strip() + '\n'
            # Keep declarations and all target-specific guard content together.
            plain = autotweak.rewrite_guard(original, row['func'], body)
        else:
            try:
                raw = m2c.run_m2c('.', row['scope'], context=VARIANT != 'no-context',
                                  extra=('--no-unk-inference',) if VARIANT == 'no-unk-inference' else (),
                                  log=lambda msg: messages.append(msg))
            except (RuntimeError, LookupError, SystemExit) as exc:
                messages.append(str(exc))
                row['variant'] = 'no-context-repair'
                raw = m2c.run_m2c('.', row['scope'], context=False, log=lambda msg: messages.append(msg))
            save(folder / 'm2c.c', raw)
            decls, body = m2c.split_draft(raw, row['func'])
            body, known, problem = bulk.cleanup(decls, body, bulk.collect_vars(headers + '\n' + original))
            if problem:
                raise RuntimeError(problem)
            decl_lines = sorted(known.values()) + bulk.forward_decls(original, row['func'], body, headers, known)
            plain = None
        c = funcloc.locate(row['func'], row['src'])
        expected, problem = funcdiff.expected_object(c)
        if problem:
            raise RuntimeError(problem)
        want = funcdiff.resolved_functions(expected, [row['func']], c)[row['func']]
        if want is None:
            raise RuntimeError('target missing from expected object')
        def scorer(text, work):
            try:
                obj = order_search.compile_variant(c, text, work, False)
                got = funcdiff.resolved_functions(obj, [row['func']])[row['func']]
                if got is None:
                    raise RuntimeError('target missing from candidate object')
                return 0 if funcdiff.keys(got) == funcdiff.keys(want) else len(funcdiff.diff_lines(want, got))
            except (SystemExit, ValueError, OSError) as exc:
                messages.append(str(exc))
                return 10**6
        for attempt in range(12):
            row['compile_attempts'] = attempt + 1
            save(folder / 'phase.txt', 'compile-' + str(attempt + 1) + '\n')
            if row['guarded']:
                plain = autotweak.rewrite_guard(original, row['func'], body)
            else:
                placed = bulk.insert_draft(original, row['func'], body, decl_lines)
                plain = autotweak.rewrite_guard(placed, row['func'], body)
            save(folder / 'body.c', body)
            save(folder / 'declarations.h', '\n'.join(decl_lines) + '\n')
            save(folder / 'context.c', plain)
            message_start = len(messages)
            try:
                score = scorer(plain, folder / 'work')
            finally:
                # Compiler diagnostics are retained independently of the million-row sentinel.
                pass
            if score < 10**6:
                row.update(status='candidate-zero' if score == 0 else 'scored', score=score,
                           draft_hash=digest(body), context_hash=digest(plain))
                break
            msg = '\n'.join(messages[message_start:])
            fixed = bulk.cheap_fix(msg, decl_lines, known)
            if fixed is not None and fixed != decl_lines:
                decl_lines = fixed
                continue
            more = bulk.source_fix(msg, body, decl_lines, row['func'], original, headers)
            # Earlier calls without a prototype imply int in the project's K&R mode.
            # A newly generated void definition conflicts with that existing TU view.
            if more is None and ("redeclaration of '%s'" % row['func']) in msg:
                inc = re.search(bulk.INCLUDE_ASM_TMPL % re.escape(row['func']), original, re.M)
                before = original[:inc.start()] if inc else ''
                if not bulk.prototype_in(headers + '\n' + before, row['func']) and re.search(
                        r'\b' + re.escape(row['func']) + r'\s*\(', before):
                    repaired = bulk.with_return_type(body, row['func'], 's32')
                    if repaired != body:
                        more = repaired, decl_lines
                        messages.append('repair: existing earlier unprototyped call implies s32 return')
            if more is None:
                row['reason'] = bulk.first_error(msg) or 'full-TU compile did not score'
                break
            body, decl_lines = more
            # CFE can report several missing stack slots in one diagnostic. Apply every
            # independent existing repair before paying for the next full-TU compile.
            for _batch in range(32):
                additional = bulk.source_fix(msg, body, decl_lines, row['func'], original, headers)
                if additional is None or additional == (body, decl_lines):
                    break
                body, decl_lines = additional
        else:
            row['reason'] = 'compile: no fix after 12 attempts'
    except BaseException as exc:
        row['reason'] = str(exc)
        messages.append(type(exc).__name__ + ': ' + str(exc))
    finally:
        save(folder / 'diagnostics.txt', '\n'.join(messages))
        if body and not (folder / 'body.c').exists():
            save(folder / 'body.c', body)
        shutil.rmtree(folder / 'work', ignore_errors=True)
    save(folder / 'phase.txt', 'finished\n')
    if body:
        row['draft_hash'] = digest(body)
    if (folder / 'context.c').exists():
        row['context_hash'] = digest((folder / 'context.c').read_text())
    row['seconds'] = round(time.monotonic() - started, 3)
    save(folder / 'result.json', json.dumps(row, sort_keys=True) + '\n')
    return row


def main():
    global BASE, OUT
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--out', type=Path, default=BASE, help='draft archive directory')
    p.add_argument('--worker', type=int)
    p.add_argument('--limit', type=int)
    p.add_argument('--stop-after-unit', action='store_true')
    p.add_argument('--timeout', type=int, default=120)
    args = p.parse_args()
    BASE, OUT = args.out, args.out / 'catalog.jsonl'
    if args.worker is not None:
        row = json.loads((BASE / 'inventory.json').read_text())[args.worker]
        process(row)
        return
    BASE.mkdir(parents=True, exist_ok=True)
    manifest = BASE / 'inventory.json'
    if not manifest.exists():
        rows = census()
        save(manifest, json.dumps(rows, indent=2) + '\n')
        # Snapshot the complete source/header context once, not 1,066 times.
        for row in rows:
            src = Path(row['src'])
            dest = BASE / 'baseline' / src
            if not dest.exists():
                save(dest, src.read_text())
        shutil.copytree('include', BASE / 'baseline/include', dirs_exist_ok=True)
    rows = json.loads(manifest.read_text())
    print('CENSUS', len(rows), 'functions', sum(r['size'] or 0 for r in rows), 'bytes',
          sum(r['guarded'] for r in rows), 'guarded', flush=True)
    n = 0
    active_unit = None
    for i, row in enumerate(rows):
        folder = BASE / row['unit'] / row['func']
        result = folder / 'result.json'
        if result.exists():
            continue
        if args.stop_after_unit and active_unit is not None and row['unit'] != active_unit:
            break
        active_unit = row['unit']
        folder.mkdir(parents=True, exist_ok=True)
        started = time.monotonic()
        with open(folder / 'worker.log', 'w') as log:
            proc = subprocess.Popen([sys.executable, __file__, '--out', str(BASE), '--worker', str(i)], stdout=log,
                                    stderr=subprocess.STDOUT, start_new_session=True)
            try:
                rc = proc.wait(timeout=args.timeout)
            except subprocess.TimeoutExpired:
                os.killpg(proc.pid, signal.SIGKILL)
                proc.wait()
                rc = -signal.SIGKILL
        if not result.exists():
            phase = (folder / 'phase.txt').read_text().strip() if (folder / 'phase.txt').exists() else 'worker-start'
            failed = dict(row, phase=phase, status='timeout' if rc == -signal.SIGKILL else 'worker-error',
                          reason='bounded target timeout' if rc == -signal.SIGKILL else 'worker exit '+str(rc),
                          score=None, seconds=round(time.monotonic()-started, 3))
            save(result, json.dumps(failed, sort_keys=True) + '\n')
            shutil.rmtree(folder / 'work', ignore_errors=True)
        got = json.loads(result.read_text())
        with OUT.open('a') as f:
            f.write(json.dumps(got, sort_keys=True) + '\n')
        print(i+1, '/', len(rows), row['scope'], got['status'], got.get('score'),
              got.get('reason', '')[:160], flush=True)
        n += 1
        if args.limit and n >= args.limit:
            break
    # Rebuild the durable catalog from per-target atomic evidence (resumable after interruption).
    results = [json.loads((BASE/r['unit']/r['func']/'result.json').read_text())
               for r in rows if (BASE/r['unit']/r['func']/'result.json').exists()]
    save(OUT, ''.join(json.dumps(r, sort_keys=True)+'\n' for r in results))
    from collections import Counter
    print('RESULTS', len(results), dict(Counter(r['status'] for r in results)), flush=True)


if __name__ == '__main__':
    main()
