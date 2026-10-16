#!/usr/bin/env python3
"""Whole-tree twin sweep: copy matched C onto every unmatched twin in one pass (T-16500).

tools/dupes.py (byte-identical functions, T-1300) and tools/neardupes.py (same shape, other
constants, T-3320) plan their copies over the whole tree, but they were run once per work package
with `--files`, so a function matched in one package never reached its twin in another one. This
tool runs both planners over every unit at once and lists each copy as `target <- donor` with its
mode and size. Nothing is written unless you ask for it.

Modes (a target gets the first that plans it):
  exact    dupes.py: equal fingerprint, relocated operands masked.
  near     neardupes.py: equal shape, every differing constant substituted by value.
  lenient  the same two planners with --lenient: where the target's header declares a symbol
           differently from the donor's file, the target's declaration is kept. Only the build
           can say whether such a copy is right, so it is only worth applying with the sha1 check
           (which --apply always does).

Usage (inside Docker, from the repo root, after a build so asm/ exists; a run takes some minutes):
  tools/docker.sh python3 tools/twin_sweep.py                 # dry run: plan, by mode, and summary
  tools/docker.sh python3 tools/twin_sweep.py -v              # + every target that stays unmatched, with its reason
  tools/docker.sh python3 tools/twin_sweep.py --list-files    # only the target files of the plan, comma separated
  tools/docker.sh python3 tools/twin_sweep.py --apply --files GEKO,main/80061710
  tools/docker.sh python3 tools/twin_sweep.py --apply --files ALL --exclude <files agents own>
  options: --unit NAME and --func NAME (target filters, repeatable), --no-lenient, --keep-drafts,
  --jobs N (ninja -j for the checks, default 2), --exclude FILES.
FILES is a comma or space separated list (or `@list.txt`, one per line) of exact C files as in
`dupes.py --files` (`GEKO` is src/ovl/GEKO.c, `GEKO/8013C000` is src/ovl/GEKO/8013C000.c, `main/80061710`
or a bare address is a main file) or src paths (`src/ovl/GEKO/8013C000.c`). `--files` and `--exclude`
also limit the dry-run plan.

NON_MATCHING drafts: a target that is the `#else INCLUDE_ASM` arm of an `#ifdef NON_MATCHING` draft is
planned like any other (it is unmatched), but writing the copy into the `#else` arm leaves the stale
draft behind, and a draft whose signature differs from the copy's prototype fails the header check. So
by default the copy replaces the whole `#ifdef ... #endif` block (the plan note says so); a rejected
copy brings the block back with the rest. `--keep-drafts` writes into the `#else` arm as dupes.py does.

--apply needs --files (`ALL` means every file the plan reaches, minus --exclude) so a run never
touches a C file by accident. Every copy is verified with the unit sha1: the targets of one unit
and mode are written together and the unit is built (`build/<unit>.ok`); if that fails the batch
is reverted and retried copy by copy, and a copy that does not match is taken back, headers
included (dupes.apply_all). A unit whose build is already failing before the sweep is skipped
and reported. Copies into a jump-table or own-rodata function are applied only because the check
is always on; those whose table or data is not in the function's own file are listed as HELD and
never applied (dupes.split_held).

Limits: the planners' own (a body with strings or statics, ambiguous constants or symbols,
unrelated type views, targets without a matched twin; `-v` names the reason of each skip). A
donor is a function that is matched when the sweep starts; a copy made now does not become a
donor for the same run, so after applying, run the sweep again. The sweep does not know which
files other agents are editing: pass them as --exclude.
Tests: tools/test_twin_sweep.py.
"""
import argparse
import copy
import os
import re
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dupes  # noqa: E402
import neardupes  # noqa: E402

MODES = ('exact', 'near', 'lenient')
ALL = 'ALL'
FILE_RE = re.compile(r'(?:^|/)src/(main/[^/]+|ovl/.+?)\.c$')


def file_spec(path):
    """The dupes.wanted_file name of a src path ('src/ovl/GEKO/8013C000.c' -> 'GEKO/8013C000',
    'src/main/80061710.c' -> 'main/80061710'); anything else loses a trailing '.c' only."""
    path = path.replace(os.sep, '/')
    m = FILE_RE.search(path)
    if m:
        return m.group(1).removeprefix('ovl/')
    return path[:-2] if path.endswith('.c') else path


def parse_files(arg):
    """Comma separated dupes.wanted_file spec from a --files/--exclude argument: names, src paths and
    `@listfile` entries (one per line, or comma separated). '' for no argument; ALL stays ALL."""
    items = []
    for tok in re.split(r'[,\s]+', arg or ''):
        if tok.startswith('@'):
            with open(tok[1:]) as fh:
                items += re.split(r'[,\s]+', fh.read())
        else:
            items.append(tok)
    names = [file_spec(i) for i in items if i]
    if any(n.upper() == ALL for n in names):
        return ALL
    return ','.join(names)


IF_RE = re.compile(r'^[ \t]*#[ \t]*(ifdef|ifndef|if)\b(.*)$')
ELSE_RE = re.compile(r'^[ \t]*#[ \t]*(else|elif)\b')
ENDIF_RE = re.compile(r'^[ \t]*#[ \t]*endif\b')


def draft_block(text, name):
    """The text of the `#ifdef NON_MATCHING <draft> #else INCLUDE_ASM(..., name); #endif` block that
    holds the INCLUDE_ASM line of `name` (from `#ifdef` to `#endif`, no final newline), or None when
    the line is not the whole `#else` arm of such a block. The draft is a near miss that is still in
    the file; a matched twin makes it obsolete, and it would clash with the copy's prototype."""
    lines = text.splitlines(keepends=True)
    inc = re.compile(r'^INCLUDE_ASM\("[^"]*",\s*%s\);[ \t]*$' % re.escape(name))
    stack = []                       # open conditionals: [index of #if, condition, index of #else or None]
    for i, line in enumerate(lines):
        s = line.rstrip('\r\n')
        m = IF_RE.match(s)
        if m:
            stack.append([i, ' '.join((m.group(1) + ' ' + m.group(2)).split()), None])
            continue
        m = ELSE_RE.match(s)
        if m and stack:
            stack[-1][2] = i if m.group(1) == 'else' else -1          # #elif: not the form we handle
            continue
        if ENDIF_RE.match(s) and stack:
            stack.pop()
            continue
        if not inc.match(s):
            continue
        if not stack or stack[-1][1] != 'ifdef NON_MATCHING' or stack[-1][2] in (None, -1):
            return None
        start, _cond, els = stack[-1]
        if any(l.strip() for l in lines[els + 1:i]):
            return None                                # something else shares the #else arm
        end = i + 1
        while end < len(lines) and not lines[end].strip():
            end += 1
        if end >= len(lines) or not ENDIF_RE.match(lines[end]):
            return None
        return ''.join(lines[start:end + 1]).rstrip('\r\n')
    return None


def drop_drafts(root, rows):
    """Make each row's copy replace the whole NON_MATCHING block around its INCLUDE_ASM line, not just
    the line (dupes.apply_plan pattern); rolled back with the rest when the build rejects the copy."""
    texts = {}
    for r in rows:
        text = texts.setdefault(r['file'], dupes.read(root, r['file']))
        block = draft_block(text, r['name'])
        if block:
            r['plan']['pattern'] = re.compile(re.escape(block))
            r['draft'] = block.count('\n') + 1


def exact_view(funcs):
    """The functions of a near-mode load as the exact planner wants them: key = exact fingerprint."""
    out = []
    for f in funcs:
        g = copy.copy(f)
        g.key = f.exact
        out.append(g)
    return out


def tid(unit, name):
    return '%s/%s' % (unit, name)


def make_row(mode, via, plan):
    p = plan
    why = None
    for k in ('jtbl', 'data'):
        if p.get(k) not in (None, 'island'):
            why = '%s %s' % (k, p[k])
    plan['mode'] = mode
    return dict(mode=mode, via=via, unit=p['unit'], name=p['name'], donor_unit=p.get('src_unit', '?'),
                donor=p['src'], bytes=p['bytes'], ndiff=p.get('ndiff', 0), file=p['tgtfile'],
                check=[k for k in ('jtbl', 'data') if p.get(k) == 'island'], held=why, draft=None, plan=p)


def sweep(root, lenient=True, drafts=True, log=lambda m: None):
    """Plan every copy over the whole tree. Returns (rows, skipped, stats). rows: one dict per
    planned target in mode order (exact, near, lenient), skipped: {unit/name: dict(unit, name, bytes,
    reasons, file)} of the targets no pass could plan (reasons of the last pass that tried them)."""
    t0 = time.time()
    funcs = dupes.load_funcs(root, near=True)
    efuncs = exact_view(funcs)
    log('loaded %d functions in %.0fs' % (len(funcs), time.time() - t0))
    cache = {'lenient': False, 'aliases': dupes.load_aliases(root)}
    rows, skipped = {}, {}
    stats = {'functions': len(funcs), 'matched': sum(1 for f in funcs if f.matched)}

    def run(mode, fs, efs):
        ep, es, _ = dupes.plan_all(root, efs, cache)
        npl, ns, nst, nosrc = neardupes.plan_all(root, fs, cache)
        for via, plans in (('exact', ep), ('near', npl)):
            for p in plans:
                k = tid(p['unit'], p['name'])
                if k not in rows:
                    rows[k] = make_row(mode if mode == 'lenient' else via, via, p)
        sk = {}
        for t, rs in es + ns:
            sk[tid(t.unit, t.name)] = dict(unit=t.unit, name=t.name, bytes=t.n * 4, reasons=rs,
                                           file=dupes.src_for(root, t))
        return sk, nst

    skipped, nst = run('strict', funcs, efuncs)
    log('strict pass done in %.0fs: %d planned' % (time.time() - t0, len(rows)))
    stats['levers'] = (nst['no_src'], nst['nosrc_members'], nst['nosrc_bytes'])
    if lenient:
        cache['lenient'] = True
        rest = [f for f in funcs if f.matched or tid(f.unit, f.name) not in rows]
        skipped, _ = run('lenient', rest, exact_view(rest))
        log('lenient pass done in %.0fs: %d planned' % (time.time() - t0, len(rows)))
    skipped = {k: v for k, v in skipped.items() if k not in rows}
    order = {m: i for i, m in enumerate(MODES)}
    out = sorted(rows.values(), key=lambda r: (order[r['mode']], r['unit'], r['name']))
    stats['unmatched'] = stats['functions'] - stats['matched']
    stats['wide'] = sorted((f.unit, f.name, f.n * 4) for f in neardupes.wide_extra(funcs))
    if drafts:
        drop_drafts(root, out)
    return out, skipped, stats


def select(rows, skipped, files=None, exclude=None, units=None, names=None):
    """Rows and skipped targets limited to the given files, units and function names."""
    def keep(unit, name, path):
        if units and unit not in units:
            return False
        if names and name not in names:
            return False
        if files and files != ALL and not dupes.wanted_file(path, files):
            return False
        return not (exclude and dupes.wanted_file(path, exclude))
    return ([r for r in rows if keep(r['unit'], r['name'], r['file'])],
            {k: v for k, v in skipped.items() if keep(v['unit'], v['name'], v['file'])})


def describe(r):
    notes = []
    if r['mode'] == 'near':
        notes.append('%d consts' % r['ndiff'])
    if r['mode'] == 'lenient':
        notes.append('via %s%s' % (r['via'], ', %d consts' % r['ndiff'] if r['via'] == 'near' else ''))
    notes += ['%s island: sha1 check' % k for k in r['check']]
    if r['draft']:
        notes.append('replaces a %d-line NON_MATCHING draft' % r['draft'])
    if r['held']:
        notes.append('HELD, never applied: ' + r['held'])
    return '%-7s %s <- %s  %d B  %s%s' % (r['mode'], tid(r['unit'], r['name']), tid(r['donor_unit'], r['donor']),
                                         r['bytes'], r['file'], ' [%s]' % '; '.join(notes) if notes else '')


def report(rows, skipped, stats, verbose, log=print):
    """Summary and plan lines of a sweep."""
    held = [r for r in rows if r['held']]
    log('sweep: %d functions (%d matched, %d unmatched); planned %d copies, %d bytes'
        % (stats['functions'], stats['matched'], stats['unmatched'], len(rows), sum(r['bytes'] for r in rows)))
    per = []
    for m in MODES:
        rs = [r for r in rows if r['mode'] == m]
        per.append('%s %d/%dB' % (m, len(rs), sum(r['bytes'] for r in rs)))
    log('by mode: ' + ', '.join(per))
    log('held (never applied): %d, %d bytes; no plan: %d functions, %d bytes'
        % (len(held), sum(r['bytes'] for r in held), len(skipped), sum(v['bytes'] for v in skipped.values())))
    files = sorted({r['file'] for r in rows if not r['held']})
    log('target files: %d' % len(files))
    cls = {}
    for v in skipped.values():
        c = neardupes.reason_class(v['reasons'][0]) if v['reasons'] else '?'
        e = cls.setdefault(c, [0, 0])
        e[0] += 1
        e[1] += v['bytes']
    for c, (n, b) in sorted(cls.items(), key=lambda kv: -kv[1][0])[:6]:
        log('  no plan: %3d fn %6d B  %s' % (n, b, c))
    ng, nm, nb = stats.get('levers', (0, 0, 0))
    log('groups without a matched member (matching one gets the rest): %d groups, %d functions, %d bytes' % (ng, nm, nb))
    wide = stats.get('wide', [])
    log('wider shape (load/store offsets and shifts masked too): %d more unmatched functions, %d bytes, have a '
        'matched twin (a field or scale differs; copy by hand)' % (len(wide), sum(w[2] for w in wide)))
    for r in rows:
        log('PLAN ' + describe(r))
    if verbose:
        for unit, name, size in wide:
            log('WIDE %s/%s %d B' % (unit, name, size))
        for k, v in sorted(skipped.items()):
            log('SKIP %s: %s' % (k, '; '.join(v['reasons'][:2])))


def apply_rows(root, rows, verbose=False, log=print):
    """Write the rows' copies and keep those whose unit sha1 holds. Returns (kept plans, skipped units)."""
    units = sorted({r['unit'] for r in rows if not r['held']})
    bad = []
    for u in units:
        ok, _out = dupes.build_ok(root, u)
        if not ok:
            bad.append(u)
            log('SKIP UNIT %s: its build does not match before the sweep' % u)
    kept = []
    for m in MODES:
        ps = [r['plan'] for r in rows if r['mode'] == m and r['unit'] not in bad]
        go, held = dupes.split_held(ps, True)
        if go:
            kept += dupes.apply_all(root, go, True, verbose, log=log,
                                    describe=lambda p: '%s %s/%s <- %s/%s' % (
                                        p['mode'], p['unit'], p['name'], p.get('src_unit', '?'), p['src']))
    return kept, bad


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--root', default='.')
    ap.add_argument('--files', help='limit the plan (and --apply) to these target files: units/objects as in '
                                    'dupes.py --files, src paths, @listfile; ALL with --apply = every file')
    ap.add_argument('--exclude', help='drop the plans into these files (files other agents are editing)')
    ap.add_argument('--apply', action='store_true', help='write the copies of --files, each verified by the unit sha1')
    ap.add_argument('--list-files', action='store_true', help='print only the target files of the plan')
    ap.add_argument('--unit', action='append', help='only targets in this unit (repeatable)')
    ap.add_argument('--func', action='append', help='only this target function name (repeatable)')
    ap.add_argument('--no-lenient', action='store_true', help='skip the lenient pass')
    ap.add_argument('--keep-drafts', action='store_true',
                    help='put a copy into the #else arm of a NON_MATCHING draft instead of replacing the block')
    ap.add_argument('--jobs', type=int, default=2, help='ninja -j for the sha1 checks (default 2)')
    ap.add_argument('-v', action='store_true')
    a = ap.parse_args(argv)
    files, exclude = parse_files(a.files), parse_files(a.exclude)
    if exclude == ALL:
        ap.error('--exclude ALL leaves nothing to do')
    if a.apply and not files:
        ap.error('--apply needs --files (a list of target files, or ALL, optionally with --exclude)')
    if a.apply and a.list_files:
        ap.error('--apply and --list-files are exclusive')
    if a.jobs > 0:
        dupes.NINJA_ARGS = ['-j%d' % a.jobs]
    rows, skipped, stats = sweep(a.root, not a.no_lenient, not a.keep_drafts, log=lambda m: print(m, file=sys.stderr))
    rows, skipped = select(rows, skipped, files, exclude, a.unit, a.func)
    if a.list_files:
        print(','.join(sorted({file_spec(r['file']) for r in rows if not r['held']})))
        return 0
    report(rows, skipped, stats, a.v)
    if a.apply:
        kept, bad = apply_rows(a.root, rows, a.v)
        todo = [r for r in rows if not r['held'] and r['unit'] not in bad]
        print('applied %d of %d copies (%d bytes); rejected %d, held %d, units skipped %d'
              % (len(kept), len(todo), sum(p['bytes'] for p in kept), len(todo) - len(kept),
                 sum(1 for r in rows if r['held']), len(bad)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
