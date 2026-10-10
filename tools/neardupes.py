#!/usr/bin/env python3
"""Reuse matched C for near-duplicate functions: same code shape, different constants (T-3320).

tools/dupes.py only copies C between byte-identical functions (relocations masked). The overlays
are scenes built from shared templates, so many functions have the same instructions with other
immediates: event ids, text ids, coordinates, counts. This tool fingerprints every function with
the 16-bit immediate of ALU-immediate ops masked as well (addiu/addi/slti/sltiu/andi/ori/xori/lui,
not on $sp: frame size and local addresses stay in the shape; load/store offsets, shift amounts and
branch offsets stay too), so members of one group have the same opcodes, registers and relocation
positions but may differ in constants. A `lui` followed by an `ori`/`addiu` on the same register is
one 32-bit constant.

For a group with a function already in C (the source) and unmatched members (INCLUDE_ASM), the
source C is copied with dupes.plan_copy (rename, symbol mapping, header declarations) and then every
differing constant is substituted by value: a source constant s that becomes t in the target is
replaced in the C text, but only when it is unambiguous:
  - every source instruction carrying s maps to the same t,
  - s occurs in the C as a literal (a leading or binary minus counts as a sign) at least once, and
    not more often than there are instructions carrying it (so no literal is used for something
    that does not produce an immediate, such as an array index).
Anything else is skipped and listed with a reason. The build is the final judge (`--check`).

Usage (run inside Docker, from the repo root, after a build so asm/ exists):
  tools/docker.sh python3 tools/neardupes.py                    # --report is the default
  tools/docker.sh python3 tools/neardupes.py --report [-v]      # groups, fillable members, reasons
  tools/docker.sh python3 tools/neardupes.py --apply --check    # write C, build each object, revert failures
  options: --unit NAME (main or overlay name, repeatable), --func NAME, --lenient (as dupes.py),
  -v (list every skipped function with its reason).
Run dupes.py first; this tool skips members that have a byte-identical matched twin.
Tests: tools/test_neardupes.py.
"""
import argparse
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dupes  # noqa: E402

OP_ADDI, OP_ADDIU, OP_SLTI, OP_SLTIU, OP_ANDI, OP_ORI, OP_XORI, OP_LUI = 8, 9, 10, 11, 12, 13, 14, 15
SIGNED_OPS = {OP_ADDI, OP_ADDIU, OP_SLTI, OP_SLTIU}
PAIR_WINDOW = 4          # a lui and the ori/addiu completing it are at most this many ops apart

LIT_RE = re.compile(r'(?<![\w.])(0[xX][0-9A-Fa-f]+|\d+)([uUlL]*)(?![\w.])')
COMMENT_RE = re.compile(r'/\*.*?\*/|//[^\n]*', re.S)


class Const:
    """One constant of a function: the immediate of one op, or a lui+ori/addiu pair."""

    def __init__(self, op, value, bits, idxs):
        self.op = op
        self.value = value       # unsigned, modulo 2**bits
        self.bits = bits
        self.idxs = idxs         # instruction indices carrying it


def consts_of(imm_words):
    """Constants of a function from its masked (index, word) list."""
    out = []
    used = set()
    for n, (i, w) in enumerate(imm_words):
        if n in used:
            continue
        op, rt, imm = w >> 26, (w >> 16) & 31, w & 0xFFFF
        if op != OP_LUI:
            out.append(Const(op, imm, 16, [i]))
            continue
        pair = None
        for m in range(n + 1, len(imm_words)):
            j, w2 = imm_words[m]
            if j - i > PAIR_WINDOW:
                break
            if m not in used and w2 >> 26 in (OP_ORI, OP_ADDIU) and (w2 >> 21) & 31 == rt:
                pair = (m, j, w2)
                break
        if pair is None:
            out.append(Const(OP_LUI, imm << 16, 32, [i]))
            continue
        m, j, w2 = pair
        used.add(m)
        lo = w2 & 0xFFFF
        if w2 >> 26 == OP_ADDIU and lo & 0x8000:
            lo -= 0x10000
        out.append(Const(OP_LUI, ((imm << 16) + lo) & 0xFFFFFFFF, 32, [i, j]))
    return out


# ---------------------------------------------------------------- C literals

class Lit:
    def __init__(self, start, end, eff, text, suffix, binary_minus):
        self.start, self.end = start, end     # span replaced (includes a unary minus)
        self.eff = eff                         # effective value, sign included
        self.text, self.suffix = text, suffix
        self.binary_minus = binary_minus       # `a - L`: the instruction carries -L


def find_literals(code):
    """Numeric literals of C code outside comments, as Lit objects."""
    skip = [(m.start(), m.end()) for m in COMMENT_RE.finditer(code)]
    lits = []
    for m in LIT_RE.finditer(code):
        if any(a <= m.start() < b for a, b in skip):
            continue
        s, e = m.start(), m.end()
        v = int(m.group(1), 0)
        j = s - 1
        while j >= 0 and code[j] in ' \t':
            j -= 1
        neg = binary = False
        if j >= 0 and code[j] == '-' and not (j > 0 and code[j - 1] == '-'):
            k = j - 1
            while k >= 0 and code[k] in ' \t':
                k -= 1
            if k >= 0 and (code[k].isalnum() or code[k] in '_)]'):
                binary = True
            else:
                neg, s = True, j
        lits.append(Lit(s, e, -v if (neg or binary) else v, m.group(1), m.group(2), binary))
    return lits


def fmt_literal(lit, newval):
    """Text replacing lit when the effective value becomes newval (None if not expressible)."""
    if lit.binary_minus:
        if newval > 0:
            return None
        n = -newval
    else:
        n = abs(newval)
    if lit.text[:2] in ('0x', '0X'):
        digits = '%0*X' % (len(lit.text) - 2, n)
        body = lit.text[:2] + (digits.lower() if lit.text[2:] != lit.text[2:].upper() else digits)
    else:
        body = str(n)
    body += lit.suffix
    if lit.binary_minus:
        return body
    return ('-' + body) if newval < 0 else body


# ---------------------------------------------------------------- substitution

def eff_matches(lit, c):
    return (lit.eff - c.value) % (1 << c.bits) == 0


def new_effective(c, t, lit):
    """Effective C value for the target constant t (unsigned, modulo 2**bits) at literal lit."""
    signed_t = t - (1 << c.bits) if t >> (c.bits - 1) else t
    if lit.eff < 0 or lit.binary_minus or c.op in SIGNED_OPS:
        return signed_t
    return t


def substitute(text, scons, tcons):
    """Rewrite the literals of function text for the target's constants.

    Returns (new text, reason). scons/tcons are the const lists of source and target.
    """
    if len(scons) != len(tcons):
        return None, 'constant count differs'
    mapping = {}           # (bits, source value) -> target value
    count = {}             # (bits, source value) -> number of carrying instructions
    for s, t in zip(scons, tcons):
        if (s.op, s.bits) != (t.op, t.bits):
            return None, 'constant kinds differ'
        k = (s.bits, s.value)
        if mapping.setdefault(k, t.value) != t.value:
            return None, 'value 0x%X maps to different constants (0x%X, 0x%X)' % (s.value, mapping[k], t.value)
        count[k] = count.get(k, 0) + 1
    code = text
    lits = find_literals(code)
    edits = []
    for (bits, sv), tv in sorted(mapping.items()):
        if tv == sv:
            continue
        c = Const(OP_ADDIU if bits == 16 else OP_ORI, sv, bits, [])
        occ = [l for l in lits if eff_matches(l, c)]
        if bits == 16:           # a 16-bit instruction also matches a literal of the same low 16 bits only if small
            occ = [l for l in occ if abs(l.eff) < 0x10000]
        if not occ:
            return None, 'no C literal for constant 0x%X (folded, enum, macro or computed)' % sv
        if len(occ) > count[(bits, sv)]:
            return None, 'constant 0x%X: %d C literals for %d instructions' % (sv, len(occ), count[(bits, sv)])
        ref = next(t for s, t in zip(scons, tcons) if (s.bits, s.value) == (bits, sv))
        for l in occ:
            ne = new_effective(ref, tv, l)
            new = fmt_literal(l, ne)
            if new is None:
                return None, 'constant 0x%X: sign flip in a subtraction' % sv
            edits.append((l.start, l.end, new))
    edits.sort()
    for a, b in zip(edits, edits[1:]):
        if a[1] > b[0]:
            return None, 'overlapping literals'
    for s, e, new in reversed(edits):
        code = code[:s] + new + code[e:]
    return code, None


# ---------------------------------------------------------------- planning

def plan_near(root, src, tgt, cache):
    plan, why = dupes.plan_copy(root, src, tgt, cache)
    if not plan:
        return None, why
    scons, tcons = consts_of(src.imm_words), consts_of(tgt.imm_words)
    text, why = substitute(plan['text'], scons, tcons)
    if text is None:
        return None, why
    plan['text'] = text
    plan['ndiff'] = sum(1 for s, t in zip(scons, tcons) if s.value != t.value)
    return plan, None


def build_groups(funcs):
    """key -> members, for near groups (more than one member, not all with the same exact key)."""
    groups = {}
    for f in funcs:
        if f.bad:
            continue
        groups.setdefault(f.key, []).append(f)
    return {k: v for k, v in groups.items() if len({m.exact for m in v}) > 1}


def plan_all(root, funcs, cache):
    groups = build_groups(funcs)
    plans, skipped = [], []
    stats = dict(groups=len(groups), with_src=0, no_src=0, cand=0, nosrc_members=0, nosrc_bytes=0)
    nosrc = []
    for key, mem in sorted(groups.items(), key=lambda kv: kv[1][0].name):
        srcs = [f for f in mem if f.matched]
        matched_exact = {f.exact for f in srcs}
        tgts = [f for f in mem if not f.matched and f.exact not in matched_exact]
        if not tgts:
            continue
        if not srcs:
            stats['no_src'] += 1
            nosrc.append(mem)
            stats['nosrc_members'] += len(tgts)
            stats['nosrc_bytes'] += sum(t.n * 4 for t in tgts)
            continue
        stats['with_src'] += 1
        for t in tgts:
            stats['cand'] += 1
            reasons, best = [], None
            for s in srcs:
                plan, why = plan_near(root, s, t, cache)
                if plan:
                    plan['unit'] = t.unit
                    plan['bytes'] = t.n * 4
                    if best is None or plan['ndiff'] < best['ndiff']:
                        best = plan
                else:
                    reasons.append('%s: %s' % (s.name, why))
            if best:
                plans.append(best)
            else:
                skipped.append((t, reasons))
    return plans, skipped, stats, nosrc


def reason_class(r):
    r = re.sub(r'^\w+: ', '', r)
    r = re.sub(r'0x[0-9A-Fa-f]+', 'N', r)
    r = re.sub(r'\d+', 'N', r)
    r = re.sub(r'\(.*\)', '', r)
    r = re.sub(r'(body names|body uses|declaration of|declares|inside|only in) \S+', r'\1 X', r)
    return r.strip()[:70]


def wide_extra(funcs):
    """Members that a wider shape (load/store offsets and shifts masked) would pull into a group
    that has a matched twin but that the narrow shape does not cover."""
    covered = {id(f) for f in funcs if not f.matched and f.key in
               {g.key for g in funcs if g.matched}}
    groups = {}
    for f in funcs:
        if not f.bad and f.wide:
            groups.setdefault(f.wide, []).append(f)
    extra = []
    for mem in groups.values():
        if any(f.matched for f in mem):
            extra += [f for f in mem if not f.matched and id(f) not in covered]
    return extra


def report(args, funcs, plans, skipped, stats, nosrc):
    nm = sum(1 for f in funcs if f.matched)
    print('functions %d (matched %d), near groups %d' % (len(funcs), nm, stats['groups']))
    print('groups with a matched source: %d (unmatched members without a matched twin: %d)'
          % (stats['with_src'], stats['cand']))
    pb = sum(p['bytes'] for p in plans)
    print('fillable now: %d functions, %d bytes (%d differing constants)'
          % (len(plans), pb, sum(p['ndiff'] for p in plans)))
    print('skipped: %d functions, %d bytes' % (len(skipped), sum(t.n * 4 for t, _ in skipped)))
    cls = {}
    for t, rs in skipped:
        c = reason_class(rs[0]) if rs else '?'
        e = cls.setdefault(c, [0, 0])
        e[0] += 1
        e[1] += t.n * 4
    for c, (n, b) in sorted(cls.items(), key=lambda kv: -kv[1][0]):
        print('  %4d fn %6d B  %s' % (n, b, c))
    print('groups without any matched member: %d (%d unmatched functions, %d bytes; matching one '
          'member would unlock the rest)' % (stats['no_src'], stats['nosrc_members'], stats['nosrc_bytes']))
    bad = {}
    for f in funcs:
        if f.bad and not f.matched:
            e = bad.setdefault(f.bad, [0, 0])
            e[0] += 1
            e[1] += f.n * 4
    print('not fingerprinted (unmatched): ' + ', '.join('%s %d fn/%dB' % (k, n, b) for k, (n, b) in sorted(bad.items())))
    ex = wide_extra(funcs)
    print('with load/store offsets and shifts masked as well, %d more unmatched functions (%d bytes) '
          'have a matched twin (struct field or scale differs; not handled)'
          % (len(ex), sum(f.n * 4 for f in ex)))
    by_unit = {}
    for p in plans:
        e = by_unit.setdefault(p['unit'], [0, 0])
        e[0] += 1
        e[1] += p['bytes']
    print('fillable per unit: ' + ', '.join('%s %d/%dB' % (u, n, b) for u, (n, b) in sorted(by_unit.items())))
    if args.v:
        for p in sorted(plans, key=lambda p: (p['unit'], p['name'])):
            print('PLAN %s/%s <- %s (%d consts)' % (p['unit'], p['name'], p['src'], p['ndiff']))
        for t, rs in sorted(skipped, key=lambda x: (x[0].unit, x[0].name)):
            print('SKIP %s/%s: %s' % (t.unit, t.name, '; '.join(rs[:2])))
        for mem in nosrc:
            print('NOSRC %s' % ' '.join('%s/%s' % (m.unit, m.name) for m in mem[:8]))


def apply_plans(args, root, plans):
    kept = []
    for p in plans:
        print('PLAN %s <- %s (%s, %d consts)' % (p['name'], p['src'], p['unit'], p['ndiff']))
    by_unit = {}
    for p in plans:
        by_unit.setdefault(p['unit'], []).append(p)
    for unit, ps in sorted(by_unit.items()):
        saved_all = {}
        for p in ps:
            for k, v in dupes.apply_plan(root, p).items():
                saved_all.setdefault(k, v)
        if not args.check:
            kept += ps
            continue
        ok, out = dupes.build_ok(root, unit)
        if ok:
            kept += ps
            print('OK %s: %d copies' % (unit, len(ps)))
            continue
        dupes.restore(root, saved_all)
        print('FAIL %s: reverting, retrying one by one' % unit)
        for p in ps:
            saved = dupes.apply_plan(root, p)
            ok, out = dupes.build_ok(root, unit)
            if ok:
                kept.append(p)
                print('  keep %s' % p['name'])
            else:
                if args.v:
                    print('\n'.join(out.splitlines()[-12:]))
                dupes.restore(root, saved)
                print('  reject %s' % p['name'])
    print('applied %d of %d (%d bytes)' % (len(kept), len(plans), sum(p['bytes'] for p in kept)))
    return kept


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--root', default='.')
    ap.add_argument('--report', action='store_true')
    ap.add_argument('--apply', action='store_true')
    ap.add_argument('--check', action='store_true', help='with --apply: build each object, revert failures')
    ap.add_argument('--unit', action='append')
    ap.add_argument('--func', action='append', help='only fill this function name (repeatable)')
    ap.add_argument('--lenient', action='store_true')
    ap.add_argument('-v', action='store_true')
    a = ap.parse_args(argv)
    if a.apply and a.report:
        ap.error('--apply and --report are exclusive')
    funcs = dupes.load_funcs(a.root, near=True)
    cache = {'lenient': a.lenient, 'aliases': dupes.load_aliases(a.root)}
    plans, skipped, stats, nosrc = plan_all(a.root, funcs, cache)
    if a.unit:
        plans = [p for p in plans if p['unit'] in a.unit]
        skipped = [s for s in skipped if s[0].unit in a.unit]
    if a.func:
        plans = [p for p in plans if p['name'] in a.func]
    if a.apply:
        apply_plans(a, a.root, plans)
    else:
        report(a, funcs, plans, skipped, stats, nosrc)
    return 0


if __name__ == '__main__':
    sys.exit(main())
