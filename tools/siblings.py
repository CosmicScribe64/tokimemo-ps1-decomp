#!/usr/bin/env python3
"""Matched-sibling report: the closest matched functions for every unmatched one (T-16520).

tools/dupes.py and tools/neardupes.py only see functions whose instructions are equal up to
relocations and ALU constants. Most of the remaining functions are not twins of anything but still
have a matched relative: the same loop over a record table, the same menu handler with another
field, the same state machine with one more case. Starting from the relative's C saves the first
hours of a match. This tool finds them.

Similarity: every function becomes its sequence of opcodes with the registers, immediates, shift
amounts, branch offsets and jump targets erased (`op_token`). The score is difflib's ratio of two
such sequences, 2*M/T over contiguous matching blocks; the tool always compares the matched
function (a) with the target (b), the value is not symmetric to the last digit. Not reused:
tools/regdiff.py key_shape keeps branch offsets, jump targets and shift amounts (it aligns a draft
with its own target, too fine for relatives), and the fingerprints of dupes.py, neardupes.py and
queue.compute_groups are hashes that find equal shapes, not similar ones. Reused: the function list
and matched state (dupes.asm_sources), the asm line parser, C extraction and C file rule of dupes.py.
Speed: functions are indexed by opcode 4-grams; a target probes the postings of its rarest grams,
the functions sharing most of them are candidates; the length bound and the exact LCS ratio (bit
parallel, an upper bound of difflib's ratio) drop most candidates, difflib scores the rest.
`--check-recall N` compares this with a brute-force search over all matched functions on N targets.

Functions: every split asm file of the tree (dupes.asm_sources: stale splat files dropped, matched
state from the C source). Targets are the unmatched ones (INCLUDE_ASM, guarded NON_MATCHING drafts
included); siblings are the matched ones in any unit (TT included, it can be read).

Report:
  - text summary by tier of the target's best sibling: >= 0.90, 0.80-0.90, 0.70-0.80 (functions
    and bytes), how many of those are clones (same opcodes and registers, ratio 1.0, differing
    only in constants and addresses: tools/neardupes.py territory), the largest per tier;
  - JSON (--json): per target the top 3 siblings (ratio, bytes, C file, function) and the
    unmatched-unmatched clusters at ratio >= 0.90 (single link: members chained by pairs at or
    above the ratio; `min_ratio` is the lowest ratio of any member pair found), with the best
    matched sibling of the cluster. Matching the cheapest member of a cluster without a matched sibling opens the rest.
  - --package LIST: for each listed target a note file (default build/siblings/<unit>__<name>.md)
    with the sibling C and the two asm listings side by side, for the work package of an agent.
    The notes contain game asm: build/ is gitignored, never commit them.

Usage (inside Docker, from the repo root after `ninja` has generated asm/):
  tools/docker.sh python3 tools/siblings.py                       # summary + build/siblings.json
  tools/docker.sh python3 tools/siblings.py --skip-unit TT        # leave claimed units out of the targets
  tools/docker.sh python3 tools/siblings.py --unit TACO -v        # one unit, every target listed
  tools/docker.sh python3 tools/siblings.py --package TACO/func_80135638,BUNKAKEN/func_80140F60
  tools/docker.sh python3 tools/siblings.py --package list.txt    # one entry per line
  tools/docker.sh python3 tools/siblings.py --check-recall 40
Package entries: `unit/name`, a bare `name` (every unmatched function of that name: overlays reuse
addresses), `file:<C path or stem>` (every unmatched function of a C file: file:src/ovl/TACO/80134930.c,
file:TACO/80134930, file:80134930 or, for the main exe, file:80041000), or the JSON of
`tools/queue.py --plan N --json` (when it has one). `#` starts a comment in a list file. INDEX.md in
the notes folder lists the last run only; notes of earlier runs stay there.
First run reads the asm of the tree (about a minute); build/siblings-cache.json keeps the tokens
per file (keyed by size and mtime), later runs skip the reading.
Tests: tools/test_siblings.py.
"""
import argparse
import difflib
import hashlib
import heapq
import json
import os
import re
import struct
import sys
import time
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dupes  # noqa: E402

RADIX = 1024                       # op_token values are below this
TIERS = ((0.90, '>=0.90'), (0.80, '0.80-0.90'), (0.70, '0.70-0.80'))
BELOW = '<0.70'
CACHE_VERSION = 1
CACHE = 'build/siblings-cache.json'
JSON_OUT = 'build/siblings.json'
NOTES_DIR = 'build/siblings'


# ---------------------------------------------------------------- tokens

def op_token(word):
    """The operation of an instruction word with registers, immediates, shift amounts, branch
    offsets and jump targets erased. A nop (all zero) is its own token: delay-slot filling is part
    of the structure. R-type: funct; REGIMM: 0x80 + rt code; GTE command: 0x300 + command;
    coprocessor moves: 0x200 + 32*cop + rs sub-op; the rest: 0x100 + opcode."""
    op = word >> 26
    if op == 0:
        return 64 if word == 0 else word & 63
    if op == 1:
        return 0x80 + ((word >> 16) & 31)
    if 0x10 <= op <= 0x13:
        if op == 0x12 and word & (1 << 25):
            return 0x300 + (word & 63)
        return 0x200 + 32 * (op & 3) + ((word >> 21) & 31)
    return 0x100 + op


def reg_token(word):
    """The word with only the constants erased (registers kept): two functions with equal reg_token
    sequences are clones that differ in immediates, offsets and call targets."""
    op = word >> 26
    if op == 0:
        return word & ~0x7C0
    if op in (2, 3):
        return word & 0xFC000000
    if 0x10 <= op <= 0x13:
        return word
    return word & 0xFFFF0000


def read_words(text):
    """[(word, mnemonic, operands)] of the instructions of one split function file (the rodata that
    rodata_pieces.py appended after RODATA_MARK is not code)."""
    out = []
    for line in text.partition(dupes.RODATA_MARK)[0].splitlines():
        m = dupes.INSN_RE.match(line)
        if m and not m.group(4).startswith('.'):
            out.append((int.from_bytes(bytes.fromhex(m.group(3)), 'little'), m.group(4), m.group(5)))
    return out


def clone_key(words):
    """Digest of the reg_token sequence."""
    toks = [reg_token(w) & 0xFFFFFFFF for w in words]
    return hashlib.sha1(struct.pack('<%dI' % len(toks), *toks)).hexdigest()[:16]


class Fn:
    __slots__ = ('unit', 'name', 'folder', 'rel', 'matched', 'guarded', 'tokens', 'rkey')

    def __init__(self, src, tokens, rkey):
        self.unit, self.name, self.folder, self.rel = src.unit, src.name, src.folder, src.rel
        self.matched, self.guarded = src.matched, src.guarded
        self.tokens, self.rkey = tokens, rkey

    @property
    def key(self):
        return '%s/%s' % (self.unit, self.name)

    @property
    def n(self):
        return len(self.tokens)

    @property
    def bytes(self):
        return 4 * len(self.tokens)

    @property
    def cfile(self):
        return dupes.src_for_dir(self.unit, self.folder)


class TokenCache:
    """build/siblings-cache.json: {rel: [size, mtime_ns, tokens, clone key]} per split asm file. A
    changed size or mtime misses. Disabled without a build/ directory or with enabled=False."""

    def __init__(self, root, enabled=True):
        self.path = os.path.join(root, CACHE)
        self.enabled = enabled and os.path.isdir(os.path.dirname(self.path))
        self.files = {}
        self.dirty = False
        if self.enabled:
            try:
                with open(self.path) as fh:
                    data = json.load(fh)
                if isinstance(data, dict) and data.get('v') == CACHE_VERSION:
                    self.files = data['files']
            except (OSError, ValueError, KeyError):
                pass

    def get(self, rel, st):
        row = self.files.get(rel)
        if row and row[0] == st.st_size and row[1] == st.st_mtime_ns:
            return row[2], row[3]
        return None

    def put(self, rel, st, tokens, rkey):
        if self.enabled:
            self.files[rel] = [st.st_size, st.st_mtime_ns, tokens, rkey]
            self.dirty = True

    def save(self, keep):
        if not (self.enabled and self.dirty):
            return
        data = {'v': CACHE_VERSION, 'files': {k: v for k, v in self.files.items() if k in keep}}
        try:
            tmp = '%s.tmp%d' % (self.path, os.getpid())
            with open(tmp, 'w') as fh:
                json.dump(data, fh, separators=(',', ':'))
            os.replace(tmp, self.path)
        except OSError:
            pass


def load(root, units=None, cache=None):
    """Fn list of every current split function file; (list, number of cache hits)."""
    cache = cache or TokenCache(root, False)
    fns, hits = [], 0
    for src in dupes.asm_sources(root, units):
        path = os.path.join(root, src.rel)
        st = os.stat(path)
        got = cache.get(src.rel, st)
        if got:
            hits += 1
            tokens, rkey = got
        else:
            with open(path) as fh:
                words = [w for w, _m, _o in read_words(fh.read())]
            tokens, rkey = [op_token(w) for w in words], clone_key(words)
            cache.put(src.rel, st, tokens, rkey)
        if tokens:
            fns.append(Fn(src, tokens, rkey))
    cache.save({s.rel for s in fns})
    return fns, hits


# ---------------------------------------------------------------- index and similarity

def grams_of(tokens, n):
    """Counter of the opcode n-grams (each an int, base RADIX); a sequence shorter than n is one gram."""
    out = Counter()
    for tup in zip(*[tokens[i:] for i in range(n)]) if len(tokens) >= n else [tuple(tokens)]:
        g = 0
        for t in tup:
            g = g * RADIX + t
        out[g] += 1
    return out


class Pool:
    """Functions indexed by opcode n-grams: gram -> [(function index, count)]."""

    def __init__(self, fns, n=4):
        self.fns, self.n = fns, n
        self.counts = [grams_of(f.tokens, n) for f in fns]
        self.post = defaultdict(list)
        for i, c in enumerate(self.counts):
            for g, k in c.items():
                self.post[g].append((i, k))

    def candidates(self, counter, k, probe, exclude=None):
        """Indices of the k functions sharing most of the `probe` rarest grams of counter."""
        rare = sorted((g for g in counter if g in self.post), key=lambda g: (len(self.post[g]), g))[:probe]
        score = defaultdict(int)
        for g in rare:
            c = counter[g]
            for i, cc in self.post[g]:
                score[i] += min(c, cc)
        score.pop(exclude, None)
        return heapq.nlargest(k, score, key=lambda i: (score[i], -i))


def lcs_masks(tokens):
    """(position bitmask per token, mask of all positions) of a sequence, for lcs_length."""
    masks = {}
    for j, t in enumerate(tokens):
        masks[t] = masks.get(t, 0) | (1 << j)
    return masks, (1 << len(tokens)) - 1


def lcs_length(tokens, masks, full):
    """Length of the longest common subsequence of `tokens` and the sequence described by lcs_masks
    (bit-parallel, Crochemore et al.; about twenty times faster than difflib on these functions).
    difflib's matching blocks are a common subsequence, so this is an upper bound of its ratio."""
    v = full
    get = masks.get
    for t in tokens:
        u = v & get(t, 0)
        v = ((v + u) | (v - u)) & full
    return full.bit_length() - bin(v).count('1')


def ratio_to(target_tokens, pool, indices, lo):
    """[(ratio, index)] with difflib ratio >= lo of the pool functions `indices` against the target.
    Cheap bounds first: the length bound, then the LCS ratio (an upper bound that leaves few
    candidates for difflib). The LCS ratio alone is no similarity measure: unrelated generic code
    reaches 0.7 by scattered matches, difflib's contiguous blocks do not."""
    lb = len(target_tokens)
    masks, full = lcs_masks(target_tokens)
    sm = None
    out = []
    for i in indices:
        a = pool.fns[i].tokens
        total = len(a) + lb
        if 2 * min(len(a), lb) < lo * total or 2 * lcs_length(a, masks, full) < lo * total:
            continue
        if sm is None:
            sm = difflib.SequenceMatcher(None, autojunk=False)
            sm.set_seq2(target_tokens)
        sm.set_seq1(a)
        ratio = sm.ratio()
        if ratio >= lo:
            out.append((ratio, i))
    return out


def rank_key(target):
    """Sort key for (ratio, Fn) pairs: best ratio, then the target's own unit (shared headers and
    types), then closest size, then name."""
    return lambda rf: (-rf[0], rf[1].unit != target.unit, abs(rf[1].n - target.n), rf[1].unit, rf[1].name)


def find_siblings(target, counter, pool, args, exclude=None):
    """Top siblings [(ratio, Fn)] of target in pool."""
    idx = pool.candidates(counter, args.candidates, args.probe, exclude)
    found = [(r, pool.fns[i]) for r, i in ratio_to(target.tokens, pool, idx, args.min_ratio)]
    found.sort(key=rank_key(target))
    return found[:args.top]


def brute_siblings(target, pool, args):
    """The same by comparing with every function of the pool (for --check-recall)."""
    found = [(r, pool.fns[i]) for r, i in ratio_to(target.tokens, pool, range(len(pool.fns)), args.min_ratio)]
    found.sort(key=rank_key(target))
    return found[:args.top]


def tier_of(ratio):
    for lo, name in TIERS:
        if ratio is not None and ratio >= lo:
            return name
    return BELOW


# ---------------------------------------------------------------- the report

def sib_json(rf, target):
    r, f = rf
    return {'unit': f.unit, 'name': f.name, 'ratio': round(r, 3), 'bytes': f.bytes, 'file': f.cfile,
            'clone': f.rkey == target.rkey}


def target_json(t, sibs):
    return {'unit': t.unit, 'name': t.name, 'bytes': t.bytes, 'file': t.cfile, 'guarded': t.guarded,
            'tier': tier_of(sibs[0][0] if sibs else None), 'siblings': [sib_json(rf, t) for rf in sibs]}


def cluster_targets(targets, pool, args):
    """Single-link clusters of the targets at ratio >= args.cluster_ratio: [(member indices, weakest link)].
    `pool` is the Pool of the targets, in the same order."""
    parent = list(range(len(targets)))

    def find(x):
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    edges = []
    for i, t in enumerate(targets):
        for r, j in ratio_to(t.tokens, pool, pool.candidates(pool.counts[i], args.candidates, args.probe, i),
                             args.cluster_ratio):
            edges.append((i, r))
            a, b = find(i), find(j)
            if a != b:
                parent[b] = a
    groups = defaultdict(list)
    for i in range(len(targets)):
        groups[find(i)].append(i)
    weakest = {}
    for i, r in edges:
        root = find(i)
        weakest[root] = min(weakest.get(root, 1.0), r)
    return [(members, weakest.get(root, 1.0)) for root, members in groups.items() if len(members) > 1]


def build_report(targets, matched, args, with_clusters=True, log=lambda m: None):
    """(JSON dict, per-target sibling lists). Targets and matched are Fn lists."""
    log('indexing %d matched functions' % len(matched))
    mpool = Pool(matched, args.ngram)
    log('matching %d targets' % len(targets))
    tpool = Pool(targets, args.ngram) if with_clusters else None
    sibs, rows = [], {}
    for i, t in enumerate(targets):
        counter = tpool.counts[i] if tpool else grams_of(t.tokens, args.ngram)
        s = find_siblings(t, counter, mpool, args)
        sibs.append(s)
        rows[t.key] = target_json(t, s)
    clusters = []
    if with_clusters:
        log('clustering the targets')
        for members, weakest in cluster_targets(targets, tpool, args):
            fs = [targets[i] for i in members]
            best = max(((sibs[i][0], targets[i]) for i in members if sibs[i]), key=lambda x: x[0][0], default=None)
            clusters.append({
                'members': [{'unit': f.unit, 'name': f.name, 'bytes': f.bytes, 'file': f.cfile} for f in fs],
                'functions': len(fs), 'bytes': sum(f.bytes for f in fs),
                'min_ratio': round(weakest, 3),
                'best_matched': sib_json(*best) if best else None})
        clusters.sort(key=lambda c: (-c['bytes'], c['members'][0]['unit'], c['members'][0]['name']))
    tiers = {name: {'functions': 0, 'bytes': 0, 'clones': 0} for _lo, name in TIERS}
    tiers[BELOW] = {'functions': 0, 'bytes': 0, 'clones': 0}
    for t, s in zip(targets, sibs):
        e = tiers[tier_of(s[0][0] if s else None)]
        e['functions'] += 1
        e['bytes'] += t.bytes
        e['clones'] += bool(s and s[0][1].rkey == t.rkey)
    report = {
        'tool': 'tools/siblings.py', 'task': 'T-16520',
        'params': {'min_ratio': args.min_ratio, 'cluster_ratio': args.cluster_ratio, 'top': args.top,
                   'ngram': args.ngram, 'candidates': args.candidates, 'probe': args.probe},
        'counts': {'matched': len(matched), 'targets': len(targets), 'target_bytes': sum(t.bytes for t in targets),
                   'guarded': sum(t.guarded for t in targets)},
        'tiers': tiers, 'targets': rows, 'clusters': clusters}
    return report, sibs


def summary(report, out=print, verbose=False):
    c = report['counts']
    out('siblings: %d targets (%d bytes, %d guarded drafts) against %d matched functions'
        % (c['targets'], c['target_bytes'], c['guarded'], c['matched']))
    out('best matched sibling by tier (register-erased opcode ratio):')
    out('  %-10s %9s %9s %7s' % ('tier', 'functions', 'bytes', 'clones'))
    for _lo, name in TIERS + ((0, BELOW),):
        e = report['tiers'][name]
        out('  %-10s %9d %9d %7d' % (name if name != BELOW else BELOW + '/none', e['functions'], e['bytes'], e['clones']))
    out('  (clones: same opcodes and registers as the sibling, only constants differ: neardupes.py territory)')
    by = defaultdict(lambda: [0, 0])
    rows = report['targets']
    for r in rows.values():
        if r['tier'] in ('>=0.90', '0.80-0.90'):
            by[r['unit']][0] += 1
            by[r['unit']][1] += r['bytes']
    if by:
        out('tiers >= 0.80 by unit: ' + ', '.join('%s %d/%dB' % (u, n, b) for u, (n, b) in
                                                  sorted(by.items(), key=lambda kv: -kv[1][1])[:10]))
    for _lo, name in TIERS:
        big = sorted((r for r in rows.values() if r['tier'] == name), key=lambda r: -r['bytes'])[:3]
        for r in big:
            s = r['siblings'][0]
            out('  largest %-9s %s/%s %dB <- %s/%s %.3f %dB' % (name, r['unit'], r['name'], r['bytes'],
                                                              s['unit'], s['name'], s['ratio'], s['bytes']))
    cl = report['clusters']
    if cl:
        free = [x for x in cl if not x['best_matched']]
        opens = sum(x['functions'] - 1 for x in free)
        opens_b = sum(x['bytes'] - min(m['bytes'] for m in x['members']) for x in free)
        out('unmatched clusters at ratio >= %.2f: %d (%d functions, %d bytes); %d of them have no matched sibling '
            '>= %.2f: matching one member each opens %d functions, %d bytes'
            % (report['params']['cluster_ratio'], len(cl), sum(x['functions'] for x in cl), sum(x['bytes'] for x in cl),
               len(free), report['params']['min_ratio'], opens, opens_b))
        for x in cl[:5]:
            out('  cluster %d fn %dB weakest link %.3f: %s%s' % (
                x['functions'], x['bytes'], x['min_ratio'],
                ' '.join('%s/%s' % (m['unit'], m['name']) for m in x['members'][:4]),
                ' ...' if x['functions'] > 4 else ''))
    if verbose:
        for key in sorted(rows):
            r = rows[key]
            s = r['siblings'][0] if r['siblings'] else None
            out('  %-28s %6dB %-9s %s' % (key, r['bytes'], r['tier'],
                                         '%s/%s %.3f' % (s['unit'], s['name'], s['ratio']) if s else '-'))


# ---------------------------------------------------------------- package notes

def asm_rows(root, f):
    """[(byte offset, 'mnemonic operands')] of the instructions of Fn f (from its split asm file)."""
    with open(os.path.join(root, f.rel)) as fh:
        rows = read_words(fh.read())
    return [(4 * i, ('%-10s %s' % (m, o)).rstrip()) for i, (_w, m, o) in enumerate(rows)]


SYMBOL_RE = re.compile(r'%(?:hi|lo|gp_rel)\([^)]*\)|\.L[0-9A-Fa-f]+|\b(?:func|D|jtbl)_[0-9A-Fa-f]{8}\b|\b[A-Za-z_]\w*_[0-9A-Fa-f]{8}\b')
NUMBER_RE = re.compile(r'(?<![\w$])-?(?:0[xX][0-9A-Fa-f]+|\d+)\b')


def erase_constants(text):
    """An instruction line with symbols, labels and numbers replaced: what is left is the operation and the registers."""
    return NUMBER_RE.sub('N', SYMBOL_RE.sub('S', text))


def side_by_side(t_rows, s_rows, t_tokens, s_tokens, width=40):
    """(lines, stats): the target on the left, the sibling on the right, paired by the opcode
    alignment. Marker `=` same text, `~` same operation and registers (constants, symbols or labels
    differ), `!` same operation with other registers, blank for an instruction without a partner."""
    sm = difflib.SequenceMatcher(None, s_tokens, t_tokens, autojunk=False)
    lines = []
    st = {'same': 0, 'constants': 0, 'registers': 0, 'target_only': 0, 'sibling_only': 0}

    def row(ti, si, mark):
        left = '%04X %-*s' % (t_rows[ti][0], width, t_rows[ti][1][:width]) if ti is not None else ' ' * (5 + width)
        right = '%04X %s' % (s_rows[si][0], s_rows[si][1]) if si is not None else ''
        lines.append(('%s %s %s' % (left, mark, right)).rstrip())

    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op == 'equal':
            for k in range(i2 - i1):
                ti, si = j1 + k, i1 + k
                a, b = t_rows[ti][1], s_rows[si][1]
                if a == b:
                    mark, key = '=', 'same'
                elif erase_constants(a) == erase_constants(b):
                    mark, key = '~', 'constants'
                else:
                    mark, key = '!', 'registers'
                st[key] += 1
                row(ti, si, mark)
            continue
        both = min(i2 - i1, j2 - j1)
        for k in range(both):
            row(j1 + k, i1 + k, ' ')
        for k in range(both, j2 - j1):
            row(j1 + k, None, ' ')
        for k in range(both, i2 - i1):
            row(None, i1 + k, ' ')
        st['target_only'] += j2 - j1
        st['sibling_only'] += i2 - i1
    return lines, st


def c_excerpt(root, f, limit):
    """(path, first line, text) of the definition of Fn f in its C file, truncated to `limit` lines."""
    path = f.cfile
    try:
        with open(os.path.join(root, path)) as fh:
            text = fh.read()
    except OSError:
        return path, 0, '(C file not found)'
    span = dupes.extract_c(text, f.name)
    if not span:
        return path, 0, '(definition of %s not found in %s)' % (f.name, path)
    body = text[span[0]:span[1]].rstrip('\n').split('\n')
    first = text.count('\n', 0, span[0]) + 1
    if len(body) > limit:
        body = body[:limit] + ['/* ... %d more lines in %s */' % (len(body) - limit, path)]
    return path, first, '\n'.join(body)


def note_text(root, target, sibs, args):
    """Markdown note of one target: header, then for the first args.notes_top siblings the C and the listing."""
    head = '# Siblings of %s (%d bytes, %s%s)\n\n' % (target.key, target.bytes, target.cfile,
                                                     ', guarded NON_MATCHING draft' if target.guarded else '')
    head += 'Target asm: `%s`. Ratio = register-erased opcode similarity of the matched function with the target ' \
            '(1.0 = same operations; registers, constants, offsets and call targets may differ).\n' % target.rel
    if not sibs:
        return head + '\nNo matched function reaches ratio %.2f.\n' % args.min_ratio
    t_rows = asm_rows(root, target)
    out = [head]
    out.append('| # | sibling | ratio | bytes | C file |\n|---|---|---|---|---|')
    for k, (r, f) in enumerate(sibs, 1):
        out.append('| %d | `%s` | %.3f | %d | `%s` |' % (k, f.key, r, f.bytes, f.cfile))
    for k, (r, f) in enumerate(sibs[:args.notes_top], 1):
        cpath, first, ctext = c_excerpt(root, f, args.max_lines)
        out.append('\n## Sibling %d: %s (ratio %.3f%s)\n' % (k, f.key, r, ', clone: same registers' if f.rkey == target.rkey else ''))
        out.append('C (`%s` line %d):\n\n```c\n%s\n```\n' % (cpath, first, ctext))
        lines, st = side_by_side(t_rows, asm_rows(root, f), target.tokens, f.tokens)
        out.append('Asm, target (left, `%s`) against sibling (right, `%s`): %d identical (`=`), %d differ in constants or '
                   'symbols only (`~`), %d differ in registers (`!`), %d not aligned in the target, %d not aligned in the sibling.\n'
                   % (target.name, f.name, st['same'], st['constants'], st['registers'], st['target_only'],
                      st['sibling_only']))
        if len(lines) > args.max_lines:
            lines = lines[:args.max_lines] + ['... %d more rows (full asm: %s, %s)' % (len(lines) - args.max_lines, target.rel, f.rel)]
        out.append('```\n%s\n```' % '\n'.join(lines))
    return '\n'.join(out) + '\n'


def safe_name(key):
    return re.sub(r'[^\w.-]', '_', key.replace('/', '__')) + '.md'


def write_notes(root, targets, sibs, out_dir, args):
    """Write the note files and an INDEX.md; returns the paths written."""
    os.makedirs(out_dir, exist_ok=True)
    paths, index = [], ['# Sibling notes (T-16520)\n', '| target | bytes | best sibling | ratio |', '|---|---|---|---|']
    for t, s in zip(targets, sibs):
        name = safe_name(t.key)
        with open(os.path.join(out_dir, name), 'w') as fh:
            fh.write(note_text(root, t, s, args))
        paths.append(os.path.join(out_dir, name))
        index.append('| [%s](%s) | %d | %s | %s |' % (t.key, name, t.bytes, '`%s`' % s[0][1].key if s else '-',
                                                      '%.3f' % s[0][0] if s else '-'))
    with open(os.path.join(out_dir, 'INDEX.md'), 'w') as fh:
        fh.write('\n'.join(index) + '\n')
    return paths


# ---------------------------------------------------------------- package list

def unit_of_file(file):
    """Unit of a work-package file name (tools/queue.py): an 8-hex stem is the main exe."""
    return 'main' if re.match(r'^[0-9A-Fa-f]{8}$', file) else file.split('/')[0]


def read_entries(spec):
    """Entries of a --package argument: a file (list or queue.py JSON) or a comma separated string."""
    if os.path.isfile(spec):
        with open(spec) as fh:
            text = fh.read()
        if text.lstrip()[:1] in ('[', '{'):
            data = json.loads(text)
            items = []
            for row in data if isinstance(data, list) else [data]:
                items += row['functions'] if isinstance(row, dict) and 'functions' in row else [row]
            return ['%s/%s' % (unit_of_file(i['file']), i['name']) for i in items if isinstance(i, dict) and 'name' in i]
        return [re.sub(r'#.*', '', ln).strip() for ln in text.splitlines() if re.sub(r'#.*', '', ln).strip()]
    return [e.strip() for e in spec.split(',') if e.strip()]


def resolve_entries(entries, targets):
    """(selected Fn list in entry order without repeats, unresolved entries)."""
    by_key = {t.key: t for t in targets}
    by_name = defaultdict(list)
    by_file = defaultdict(list)
    for t in targets:
        by_name[t.name].append(t)
        stem = re.sub(r'^src/(main|ovl)/', '', t.cfile)[:-2]
        for k in (t.cfile, stem, os.path.basename(stem)):
            by_file[k].append(t)
    chosen, seen, bad = [], set(), []
    for e in entries:
        if e.startswith('file:'):
            found = by_file.get(e[5:], [])
        elif e in by_key:
            found = [by_key[e]]
        else:
            found = by_name.get(e, [])
        if not found:
            bad.append(e)
        for t in found:
            if t.key not in seen:
                seen.add(t.key)
                chosen.append(t)
    return chosen, bad


# ---------------------------------------------------------------- recall check

def check_recall(targets, matched, args, log=print):
    """Compare the index with a brute-force search on args.check_recall sample targets (evenly spread
    over the sorted targets, at most args.recall_max_insns long). Returns (tier or top-1 mismatches,
    targets that have a sibling by brute force)."""
    pool = Pool(matched, args.ngram)
    pick = [t for t in sorted(targets, key=lambda t: t.key) if t.n <= args.recall_max_insns]
    pick = pick[::max(1, len(pick) // args.check_recall)][:args.check_recall]
    have = bad = top3 = 0
    for t in pick:
        fast = [r for r, _f in find_siblings(t, grams_of(t.tokens, args.ngram), pool, args)]
        slow = [r for r, _f in brute_siblings(t, pool, args)]
        if not slow:
            continue
        have += 1
        wrong = tier_of(fast[0] if fast else None) != tier_of(slow[0]) or fast[:1] != slow[:1]
        bad += wrong
        top3 += fast != slow
        if wrong:
            log('  top sibling missed for %s: index %s, brute force %s' % (t.key, fast[:1], slow[:1]))
    log('recall check: %d targets (<= %d insns), %d with a sibling >= %.2f by brute force: best sibling and tier found '
        'for %d, the top %d ratios agree for %d' % (len(pick), args.recall_max_insns, have, args.min_ratio,
                                                      have - bad, args.top, have - top3))
    return bad, have


# ---------------------------------------------------------------- main

def parse_args(argv):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--root', default='.')
    ap.add_argument('--json', default=None, metavar='PATH',
                    help='write the report JSON here (default %s; - for stdout; with --package only when given)' % JSON_OUT)
    ap.add_argument('--unit', action='append', help='only targets of this unit (main or overlay name), repeatable; not with --package')
    ap.add_argument('--skip-unit', action='append', help='leave this unit out of the targets (still a sibling source); not with --package')
    ap.add_argument('--package', metavar='LIST', help='write sibling notes for these targets (see the docstring)')
    ap.add_argument('--out-dir', default=NOTES_DIR, help='directory of the --package notes (default %s)' % NOTES_DIR)
    ap.add_argument('--notes-top', type=int, default=2, help='siblings with C and listing per note (default 2)')
    ap.add_argument('--max-lines', type=int, default=300, help='lines of C or listing per sibling (default 300)')
    ap.add_argument('--min-ratio', type=float, default=0.70, help='weakest sibling reported (default 0.70)')
    ap.add_argument('--cluster-ratio', type=float, default=0.90, help='unmatched-unmatched cluster threshold (default 0.90)')
    ap.add_argument('--top', type=int, default=3, help='siblings kept per target (default 3)')
    ap.add_argument('--ngram', type=int, default=4, help='opcode n-gram length of the index (default 4)')
    ap.add_argument('--candidates', type=int, default=800, help='functions per target checked against the target (default 800)')
    ap.add_argument('--probe', type=int, default=256, help='rarest n-grams of a target probed in the index (default 256)')
    ap.add_argument('--check-recall', type=int, default=0, metavar='N', help='compare the index with brute force on N targets')
    ap.add_argument('--recall-max-insns', type=int, default=400, help='skip longer targets in --check-recall (default 400)')
    ap.add_argument('--no-cache', action='store_true')
    ap.add_argument('-v', action='store_true', help='list every target with its best sibling')
    return ap.parse_args(argv)


def main(argv=None):
    args = parse_args(argv)
    t0 = time.time()

    def log(msg):
        print('[%4.0fs] %s' % (time.time() - t0, msg), file=sys.stderr)

    fns, hits = load(args.root, cache=TokenCache(args.root, not args.no_cache))
    log('read %d functions (%d from the cache)' % (len(fns), hits))
    matched = [f for f in fns if f.matched]
    targets = [f for f in fns if not f.matched]
    if args.unit and not args.package:
        targets = [f for f in targets if f.unit in args.unit]
    if args.skip_unit and not args.package:
        targets = [f for f in targets if f.unit not in args.skip_unit]
    targets.sort(key=lambda f: (f.unit, f.name))
    if args.check_recall:
        bad, _have = check_recall(targets, matched, args)
        return 1 if bad else 0
    chosen = None
    if args.package:
        chosen, bad = resolve_entries(read_entries(args.package), targets)
        for e in bad:
            print('siblings: no unmatched function for %r' % e, file=sys.stderr)
        if not chosen:
            return 1
        targets = chosen
    report, sibs = build_report(targets, matched, args, with_clusters=not chosen, log=log)
    if chosen:
        paths = write_notes(args.root, targets, sibs, os.path.join(args.root, args.out_dir), args)
        print('siblings: wrote %d notes and INDEX.md to %s' % (len(paths), args.out_dir))
    if args.json == '-':
        json.dump(report, sys.stdout, indent=1, sort_keys=True)
        print()
        return 0
    path = args.json or (None if chosen else JSON_OUT)
    if path:
        full = path if os.path.isabs(path) else os.path.join(args.root, path)
        os.makedirs(os.path.dirname(full) or '.', exist_ok=True)
        with open(full, 'w') as fh:
            json.dump(report, fh, indent=1, sort_keys=True)
        print('siblings: wrote %s' % path)
    summary(report, verbose=args.v)
    log('done')
    return 0


if __name__ == '__main__':
    sys.exit(main())
