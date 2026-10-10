#!/usr/bin/env python3
"""Find byte-identical functions at different addresses and reuse matched C (T-1300).

Overlays and the main exe hold many functions whose code is identical except for
relocated operands. This tool fingerprints every function of the split asm
(asm/{nonmatchings,matchings}/main/*, asm/ovl/*/{nonmatchings,matchings}/*): the
instruction words with every relocated field masked (%hi/%lo/%gp_rel low 16 bits,
jal/j target, branch-to-symbol offset) and a marker for which words are relocated.
Register and opcode bits stay. Functions with equal fingerprints form a group.

For each group with a function already in C (the source) and unmatched members
(INCLUDE_ASM), the tool plans a copy of the source C: rename the function, map the
referenced symbols by position in the relocation sequence (source symbol -> the
target's symbol at the same position; identical symbols stay), and declare the
target's symbols in the right header (CODING_STANDARDS 8a). Anything ambiguous is
skipped and listed with a reason.

Usage (run inside Docker, from the repo root, after a build so asm/ exists):
  tools/docker.sh python3 tools/dupes.py                 # report groups and plans
  tools/docker.sh python3 tools/dupes.py --apply         # write the copies
  tools/docker.sh python3 tools/dupes.py --apply --check # + build each object, revert failures
  options: --unit NAME (main or overlay name, repeatable), --lenient (reuse the
  target's existing declaration when it differs), -v (list skipped functions).
Tests: tools/test_dupes.py.
"""
import argparse
import glob
import hashlib
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sync_protos import is_main_symbol  # noqa: E402

# asm/[ovl/<NAME>/](non)matchings/[main/]<c subsegment name>/<func>.s; the name is <addr>, <NAME> or
# <NAME>/<addr> (per-object overlay files, T-0500)
ASM_RE = re.compile(r'^asm/(?:ovl/(\w+)/)?(non)?matchings/(?:main/)?([\w/]+)/(\w+)\.s$')
INSN_RE = re.compile(r'^\s*/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*(\S+)\s*(.*?)\s*$')
RELOC_RE = re.compile(r'%(hi|lo|gp_rel|got\w*)\(([^)]*)\)')
SYM_RE = re.compile(r'^([A-Za-z_.$][\w.$]*)\s*(?:([+-])\s*(0x[0-9A-Fa-f]+|\d+))?$')
SKIP_LINE_RE = re.compile(r'^\s*($|glabel\b|endlabel\b|nonmatching\b|\.L\w+:|jlabel\b|dlabel\b|enddlabel\b|\.set\b|\.size\b|\.type\b|\.align\b|/\*(?!.*\*/\s*\S).*$)')
BASIC_WORDS = set("""extern const volatile unsigned signed char short int long void struct union enum
typedef static register s8 u8 s16 u16 s32 u32 size_t""".split())
C_KEYWORDS = set("""if else while for do switch case default return break continue goto sizeof""".split())


class Func:
    def __init__(self, name, unit, matched, path, key, relocs, bad):
        self.name = name
        self.unit = unit          # 'main' or overlay name
        self.matched = matched    # asm lives under matchings/
        self.path = path
        self.key = key
        self.relocs = relocs      # [(kind, symbol, addend)]
        self.bad = bad            # reason this function cannot take part, or None
        self.exact = key          # exact fingerprint (key is replaced by the near one in near mode)
        self.wide = None          # fingerprint with load/store offsets and shifts masked too (near mode)
        self.imm_words = []       # [(item index, word)] of the masked ALU-immediate ops (near mode)
        self.n = 0                # instruction count


ALU_IMM_OPS = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}   # addi addiu slti sltiu andi ori xori lui
MEM_OPS = set(range(0x20, 0x2F)) | {0x31, 0x32, 0x39, 0x3A}          # loads, stores, lwc/swc


def imm_mask(word, mode):
    """Extra mask of a non-relocated word for the near-duplicate fingerprint (T-3320).

    mode 'alu': the 16-bit immediate of ALU-immediate ops (not on $sp: frame size and local
    addresses stay in the shape). mode 'wide' additionally masks load/store offsets (not $sp
    based) and shift amounts.
    """
    op, rs = word >> 26, (word >> 21) & 31
    if op in ALU_IMM_OPS and rs != 29 and not (op == 0x0F and (word >> 16) & 31 == 29):
        return 0xFFFF
    if mode == 'wide':
        if op in MEM_OPS and rs != 29:
            return 0xFFFF
        if op == 0 and word & 0x3F in (0, 2, 3):
            return 0x7C0
    return 0


def parse_asm(text, imm=None, imm_out=None, info=None):
    """Return (key, relocs, bad) for the text of one split function file.

    imm=None is the exact fingerprint. imm='alu'/'wide' also masks immediates (see imm_mask);
    imm_out, when a list, receives (item index, word) of every ALU-immediate op that was masked.
    info, when a dict, receives 'n' (instruction count).
    """
    items = []
    relocs = []
    bad = None
    nglabel = 0
    for line in text.splitlines():
        if re.match(r'^glabel\b', line):
            nglabel += 1
        m = INSN_RE.match(line)
        if not m:
            if line.strip() and not SKIP_LINE_RE.match(line):
                bad = bad or 'data in function'
            continue
        mnem, ops = m.group(4), m.group(5)
        word = int.from_bytes(bytes.fromhex(m.group(3)), 'little')
        mask = 0
        rel = None
        r = RELOC_RE.search(ops)
        if r:
            mask = 0xFFFF
            rel = (r.group(1), r.group(2))
        elif mnem in ('jal', 'j') and not ops.startswith('.L'):
            mask = 0x03FFFFFF
            rel = (mnem, ops)
        elif mnem[0] == 'b' and mnem != 'break' and ',' in ops + ',' and ops.split(',')[-1].strip() \
                and not ops.split(',')[-1].strip().startswith('.L') and SYM_RE.match(ops.split(',')[-1].strip()):
            mask = 0xFFFF
            rel = ('br', ops.split(',')[-1].strip())
        if rel is not None:
            sm = SYM_RE.match(rel[1].strip())
            if not sm:
                bad = bad or 'unparsed reloc operand'
                continue
            if sm.group(1).startswith('jtbl') or sm.group(1).startswith('.L'):
                bad = bad or 'jump table / local label reloc'
            add = 0
            if sm.group(2):
                add = int(sm.group(3), 0) * (1 if sm.group(2) == '+' else -1)
            relocs.append((rel[0], sm.group(1), add))
            items.append((word & ~mask & 0xFFFFFFFF, rel[0]))
        else:
            m2 = imm_mask(word, imm) if imm else 0
            if m2 and imm_out is not None and (word >> 26) in ALU_IMM_OPS:
                imm_out.append((len(items), word))
            items.append((word & ~m2 & 0xFFFFFFFF, 'i' if m2 else ''))
    if nglabel != 1:
        bad = bad or 'not exactly one glabel'
    if not items:
        bad = bad or 'no instructions'
    key = hashlib.sha1(repr(items).encode()).hexdigest()
    if info is not None:
        info['n'] = len(items)
    return key, relocs, bad


def load_funcs(root, units=None, near=False):
    funcs = []
    for p in sorted(glob.glob(os.path.join(root, 'asm/**/*.s'), recursive=True)):
        rel = os.path.relpath(p, root).replace(os.sep, '/')
        m = ASM_RE.match(rel)
        if not m:
            continue
        unit = m.group(1) or 'main'
        if units and unit not in units:
            continue
        if not os.path.exists(os.path.join(root, src_for_dir(unit, m.group(3)))):
            continue          # a folder of an old source layout (asm/ is not cleaned by splat)
        with open(p) as f:
            text = f.read()
        info = {}
        key, relocs, bad = parse_asm(text, info=info)
        fn = Func(m.group(4), unit, m.group(2) is None, rel, key, relocs, bad)
        fn.n = info.get('n', 0)
        if near:      # T-3320: key becomes the immediate-masked fingerprint, exact keeps the old one
            fn.key = parse_asm(text, 'alu', fn.imm_words)[0]
            fn.wide = parse_asm(text, 'wide')[0]
        funcs.append(fn)
    return funcs


def group_funcs(funcs):
    groups = {}
    for f in funcs:
        if f.bad:
            continue
        groups.setdefault(f.key, []).append(f)
    return {k: v for k, v in groups.items() if len(v) > 1}


# ---------------------------------------------------------------- source side

def src_for_dir(unit, name):
    """The src .c file of a splat folder name (the c subsegment name without `main/`)."""
    return 'src/main/%s.c' % name if unit == 'main' else 'src/ovl/%s.c' % name


def src_for(root, f):
    """The src .c file holding function f (the splat folder of its asm, srcscan's path rule)."""
    m = ASM_RE.match(f.path)
    return src_for_dir(f.unit, m.group(3))


def read(root, rel):
    with open(os.path.join(root, rel)) as fh:
        return fh.read()


def header_closure(root, srcrel, cache):
    """Paths (relative to root) of the headers a src file includes, recursively."""
    out = []
    todo = re.findall(r'^\s*#\s*include\s+"([^"]+)"', read(root, srcrel), re.M)
    while todo:
        h = todo.pop(0)
        for cand in ('include/' + h,):
            if cand in out or not os.path.exists(os.path.join(root, cand)):
                continue
            out.append(cand)
            todo += re.findall(r'^\s*#\s*include\s+"([^"]+)"', read(root, cand), re.M)
    return out


def target_headers(root, tfile, tgt, cache):
    """Headers the target file sees after the copy (an overlay without a header gets one)."""
    hp = header_closure(root, tfile, cache)
    if tgt.unit != 'main' and not os.path.exists(os.path.join(root, 'include/ovl/%s.h' % tgt.unit)):
        hp = ['include/game.h'] + [h for h in header_closure(root, 'include/game.h', cache) if h != 'include/game.h']
    return hp


def strip_comments(s):
    return re.sub(r'/\*.*?\*/', '', s, flags=re.S)


def norm(s):
    return re.sub(r'\s+', ' ', strip_comments(s)).strip()


def find_decl(texts, sym):
    """First one-line declaration or prototype of sym in the given header texts."""
    var = re.compile(r'^\s*(?:extern\s+)?[^;(){}=#]*?\b%s\b\s*(?:\[[^\]]*\])*\s*[;,]' % re.escape(sym))
    proto = re.compile(r'^[^;{}=#]*\b%s\s*\([^;{}]*\)\s*;' % re.escape(sym))
    for t in texts:
        for line in t.splitlines():
            code = strip_comments(line)
            if var.match(code) or proto.match(code):
                return single_decl(re.sub(r'\s*/\*.*?\*/', '', line).strip(), sym)
    return None


def single_decl(line, sym):
    """Reduce a multi-declarator line ("extern s32 A, B;") to the declaration of sym alone."""
    code = strip_comments(line).strip()
    if ',' not in code or '(' in code:
        return line
    head, _, rest = code.rstrip(';').partition(' ')
    m = re.match(r'^((?:extern\s+)?(?:(?:unsigned|signed|const|volatile|struct|union)\s+)*\w+)\s+(.*)$', code.rstrip(';'))
    if not m:
        return None
    base = m.group(1)
    for part in m.group(2).split(','):
        if re.search(r'\b%s\b' % re.escape(sym), part):
            return '%s %s;' % (base, part.strip())
    return None


def extract_c(text, name):
    """(start, end) character range of the definition of name, or None."""
    m = re.search(r'^[A-Za-z_][^;{}()\n]*\b%s\s*\([^;{}]*\)\s*\{' % re.escape(name), text, re.M)
    if not m:
        return None
    e = text.find('\n}', m.end() - 1)
    if e < 0:
        return None
    return m.start(), e + 2


def in_conditional(text, pos):
    depth = 0
    for line in text[:pos].splitlines():
        s = line.strip()
        if re.match(r'#\s*if', s):
            depth += 1
        elif re.match(r'#\s*endif', s):
            depth -= 1
    return depth > 0


GLOBAL_LIMIT = 0x800F6000  # lowest overlay load address: below it a symbol is a main-exe symbol
ADDR_RE = re.compile(r'^(?:D|func|jtbl)_([0-9A-Fa-f]{8})$')


def load_aliases(root):
    """name -> address and address -> [names] from the config/symbol_addrs*.txt files."""
    by_name, by_addr = {}, {}
    for p in sorted(glob.glob(os.path.join(root, 'config/symbol_addrs*.txt'))):
        with open(p) as fh:
            for line in fh:
                m = re.match(r'^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;', line)
                if m and int(m.group(2), 16) < GLOBAL_LIMIT:
                    by_name[m.group(1)] = int(m.group(2), 16)
                    by_addr.setdefault(int(m.group(2), 16), []).append(m.group(1))
    return by_name, by_addr


def sym_key(name, aliases):
    """Identity of a symbol: its address when it is a main-exe symbol, else its name."""
    a = aliases[0].get(name)
    if a is None:
        m = ADDR_RE.match(name)
        if m and int(m.group(1), 16) < GLOBAL_LIMIT:
            a = int(m.group(1), 16)
    return ('a', a) if a is not None else ('n', name)


def plan_copy(root, src, tgt, cache):
    """Plan the copy of matched src to unmatched tgt.

    Returns (plan, reason). plan = dict(text, tgtfile, name, decls=[(header, line)], src).
    """
    al = cache['aliases']
    if len(src.relocs) != len(tgt.relocs):
        return None, 'reloc count differs'
    fwd, back, tname = {}, {}, {}
    for (k1, s1, a1), (k2, s2, a2) in zip(src.relocs, tgt.relocs):
        if k1 != k2 or a1 != a2:
            return None, 'reloc kind/addend differs (%s+%d vs %s+%d)' % (s1, a1, s2, a2)
        k1, k2 = sym_key(s1, al), sym_key(s2, al)
        if fwd.setdefault(k1, k2) != k2 or back.setdefault(k2, k1) != k1:
            return None, 'symbol mapping not one-to-one (%s/%s)' % (s1, s2)
        tname.setdefault(k2, s2)
    sfile, tfile = src_for(root, src), src_for(root, tgt)
    stext, ttext = read(root, sfile), read(root, tfile)
    inc = re.compile(r'^INCLUDE_ASM\("[^"]*",\s*%s\);\s*$' % re.escape(tgt.name), re.M)
    if not inc.search(ttext):
        return None, 'target INCLUDE_ASM line not found in %s' % tfile
    if re.search(r'^INCLUDE_ASM\("[^"]*",\s*%s\);' % re.escape(src.name), stext, re.M):
        return None, 'source is not in C'
    rng = extract_c(stext, src.name)
    if not rng:
        return None, 'source C definition not found'
    if in_conditional(stext, rng[0]):
        return None, 'source inside #if'
    body = stext[rng[0]:rng[1]]
    code = strip_comments(body)
    if '"' in code or re.search(r'\bstatic\b|#', code):
        return None, 'source has string literal, static or preprocessor use'
    sheads = [read(root, h) for h in header_closure(root, sfile, cache)]
    thp = target_headers(root, tfile, tgt, cache)
    theads = [read(root, h) for h in thp]
    tall = '\n'.join(theads) + '\n' + ttext
    # every game symbol and every called name in the body must come from the relocations
    used = {}
    for ident in set(re.findall(r'\b[A-Za-z_]\w*\b', code)):
        isgame = ADDR_RE.match(ident) or ident in al[0] or re.match(r'L[0-9A-F]{8}$', ident)
        called = re.search(r'\b%s\s*\(' % re.escape(ident), code) and ident not in C_KEYWORDS \
            and ident not in BASIC_WORDS
        if ident == src.name or not (isgame or called):
            continue
        k = sym_key(ident, al)
        if k not in fwd:
            return None, 'body names %s which is not in the relocation sequence' % ident
        used[ident] = k
    # type and field names used by the body must exist in the target's closure
    for w in set(re.findall(r'\b[A-Z][A-Za-z0-9_]*\b', code)) | set(re.findall(r'(?:->|\.)\s*([A-Za-z_]\w*)', code)):
        if w in used or ADDR_RE.match(w):
            continue
        if not re.search(r'\b%s\b' % re.escape(w), tall):
            return None, 'body uses %s which the target closure lacks' % w
    mapping = {src.name: tgt.name}
    decls = []
    tmain = tgt.unit == 'main'
    for ident, k in sorted(used.items()):
        t = tname[fwd[k]]
        names = [t] + [n for n in al[1].get(fwd[k][1], [])] if fwd[k][0] == 'a' else [t]
        declared = [n for n in names if find_decl(theads, n)]
        if declared:
            t = declared[0]
        elif fwd[k] == k:
            t = ident
        mapping[ident] = t
        sd = find_decl(sheads, ident)
        if sd is None:
            if find_decl([stext], ident):
                return None, 'declaration of %s only in %s' % (ident, sfile)
            continue  # implicit declaration in the source: nothing to add
        want = re.sub(r'\b%s\b' % re.escape(ident), t, sd)
        for w in set(re.findall(r'\b[A-Za-z_]\w*\b', strip_comments(want))):
            if w in BASIC_WORDS or w == t or re.match(r'((D|func)_[0-9A-F]{8}|arg\d+)$', w):
                continue
            if not re.search(r'\b%s\b' % re.escape(w), tall):
                return None, 'declaration of %s uses %s which the target closure lacks' % (ident, w)
        have = find_decl(theads, t)
        if have is not None:
            if norm(have) != norm(want) and not cache.get('lenient'):
                return None, 'target declares %s differently: "%s" vs "%s"' % (t, norm(have), norm(want))
            continue
        if find_decl([ttext], t):
            return None, 'target declares %s inside its .c' % t
        if os.path.exists(os.path.join(root, 'include/main_api.h')) and is_main_symbol(root, t, tgt.unit):
            dest = 'include/main_api.h'      # T-3340: one home for main-exe symbols
        elif tmain:
            dest = 'include/game.h'
        else:
            dest = 'include/ovl/%s.h' % tgt.unit
        decls.append((dest, want))
    text = re.sub(r'\b(%s)\b' % '|'.join(map(re.escape, sorted(mapping, key=len, reverse=True))),
                  lambda m: mapping[m.group(1)], body)
    return dict(text=text, tgtfile=tfile, name=tgt.name, decls=decls, src=src.name), None


def apply_plan(root, plan):
    """Write one plan; returns {path: previous text} for rollback."""
    saved = {}
    tp = os.path.join(root, plan['tgtfile'])
    with open(tp) as fh:
        t = fh.read()
    saved[plan['tgtfile']] = t
    pat = re.compile(r'^INCLUDE_ASM\("[^"]*",\s*%s\);[ \t]*$' % re.escape(plan['name']), re.M)
    t = pat.sub(lambda m: plan['text'].rstrip('\n'), t, count=1)
    with open(tp, 'w') as fh:
        fh.write(t)
    for dest, line in plan['decls']:
        hp = os.path.join(root, dest)
        if not os.path.exists(hp):
            guard = 'OVL_%s_H' % os.path.basename(dest)[:-2].upper()
            saved.setdefault(dest, None)
            with open(hp, 'w') as fh:
                fh.write('#ifndef %s\n#define %s\n\n#include "common.h"\n#include "game.h"\n\n'
                         '/* Overlay-local externs (T-1300). */\n\n#endif\n' % (guard, guard))
            with open(tp) as fh:
                t = fh.read()
            saved.setdefault(plan['tgtfile'], t)
            inc = '#include "ovl/%s"' % os.path.basename(dest)
            if inc not in t:
                with open(tp, 'w') as fh:
                    fh.write(t.replace('#include "common.h"\n', '#include "common.h"\n' + inc + '\n', 1))
        with open(hp) as fh:
            h = fh.read()
        saved.setdefault(dest, h)
        name = re.search(r'(\w+)\s*(?:\[[^\]]*\]\s*)?(?:\(.*\))?\s*;$', line)
        if find_decl([h], name.group(1)) if name else re.search(r'^%s$' % re.escape(line), h, re.M):
            continue
        i = h.rfind('#endif')
        h = h[:i].rstrip('\n') + '\n' + line + '\n\n' + h[i:] if i >= 0 else h.rstrip('\n') + '\n' + line + '\n'
        with open(hp, 'w') as fh:
            fh.write(h)
    return saved


def restore(root, saved):
    for rel, text in saved.items():
        if text is None:
            os.remove(os.path.join(root, rel))
            continue
        with open(os.path.join(root, rel), 'w') as fh:
            fh.write(text)


def build_ok(root, unit):
    tgt = 'build/SLPM_86.053.ok' if unit == 'main' else 'build/ovl/%s.ok' % unit
    try:
        os.remove(os.path.join(root, tgt))
    except OSError:
        pass
    r = subprocess.run(['ninja', tgt], cwd=root, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    return r.returncode == 0, r.stdout


# ---------------------------------------------------------------- driver

def plan_all(root, funcs, cache):
    groups = group_funcs(funcs)
    plans, skipped, stats = [], [], {'groups': 0, 'cand': 0}
    for key, mem in sorted(groups.items(), key=lambda kv: kv[1][0].name):
        srcs = [f for f in mem if f.matched]
        tgts = [f for f in mem if not f.matched]
        if not srcs or not tgts:
            continue
        stats['groups'] += 1
        for t in tgts:
            stats['cand'] += 1
            reasons = []
            for s in srcs:
                plan, why = plan_copy(root, s, t, cache)
                if plan:
                    plan['unit'] = t.unit
                    plans.append(plan)
                    break
                reasons.append('%s: %s' % (s.name, why))
            else:
                skipped.append((t, reasons))
    return plans, skipped, stats


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--root', default='.')
    ap.add_argument('--apply', action='store_true')
    ap.add_argument('--check', action='store_true', help='with --apply: build each object, revert failures')
    ap.add_argument('--unit', action='append')
    ap.add_argument('--func', action='append', help='only copy into this function name (repeatable)')
    ap.add_argument('--lenient', action='store_true')
    ap.add_argument('-v', action='store_true')
    a = ap.parse_args(argv)
    root = a.root
    funcs = load_funcs(root)
    cache = {'lenient': a.lenient, 'aliases': load_aliases(root)}
    plans, skipped, stats = plan_all(root, funcs, cache)
    if a.unit:
        plans = [p for p in plans if p['unit'] in a.unit]
        skipped = [s for s in skipped if s[0].unit in a.unit]
    if a.func:
        plans = [p for p in plans if p['name'] in a.func]
    nm = sum(1 for f in funcs if f.matched)
    print('functions %d (matched %d), groups with matched+unmatched %d, unmatched members %d: '
          'plans %d, skipped %d' % (len(funcs), nm, stats['groups'], stats['cand'], len(plans), len(skipped)))
    if a.v:
        for t, rs in skipped:
            print('SKIP %s/%s: %s' % (t.unit, t.name, '; '.join(rs[:2])))
    kept = []
    if a.apply:
        # repeated rounds: a copy can make a new source for another group member
        for p in plans:
            print('PLAN %s <- %s (%s)' % (p['name'], p['src'], p['unit']))
        by_unit = {}
        for p in plans:
            by_unit.setdefault(p['unit'], []).append(p)
        for unit, ps in sorted(by_unit.items()):
            saved_all = {}
            for p in ps:
                for k, v in apply_plan(root, p).items():
                    saved_all.setdefault(k, v)
            if not a.check:
                kept += ps
                continue
            ok, out = build_ok(root, unit)
            if ok:
                kept += ps
                print('OK %s: %d copies' % (unit, len(ps)))
                continue
            if a.v and len(ps) == 1:
                print('\n'.join(out.splitlines()[-12:]))
            restore(root, saved_all)
            print('FAIL %s: reverting, retrying one by one' % unit)
            for p in ps:
                saved = apply_plan(root, p)
                ok, out = build_ok(root, unit)
                if ok:
                    kept.append(p)
                    print('  keep %s' % p['name'])
                else:
                    if a.v:
                        print('\n'.join(out.splitlines()[-12:]))
                    restore(root, saved)
                    print('  reject %s' % p['name'])
        print('applied %d of %d' % (len(kept), len(plans)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
