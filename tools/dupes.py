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
  target's existing declaration when it differs), -v (list every skipped function with its reason).

T-7030: besides the symbols of the relocation sequence the copy takes along what the body needs:
fields of the game-state aggregate (D_800E6280.unk_110D, matched to the asm's D_800E738D through the
address, migrate_globals), MAIN_API_OVERRIDE_ views of the source file, file-local struct typedefs
and their declarations, `*(s16 *)0x801F0D14` address literals (the relocation's address of the
target), and the target's prototype (a missing one is added, a `void` that the twin contradicts is
corrected, a callee the target .c defines further down gets one). Declarations of main-exe symbols
are written to include/main_api.h and sync_protos.py --fix then makes the other headers agree; the
whole include/ tree is restored when a copy is rejected. The batch driver apply_all is shared with
neardupes.py.
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


def find_decl(texts, sym, toplevel=False):
    """First one-line declaration or prototype of sym in the given header texts. With `toplevel` (a
    .c file) only lines that start in column 0 with a type count: an indented `f();` is a call."""
    var = re.compile(r'^\s*(?:extern\s+)?[^;(){}=#]*?\b%s\b\s*(?:\[[^\]]*\])*\s*[;,]' % re.escape(sym))
    proto = re.compile(r'^[^;{}=#]*\b%s\s*\([^;{}]*\)\s*;' % re.escape(sym))
    for t in texts:
        for line in t.splitlines():
            code = strip_comments(line)
            if toplevel and (not code[:1].isalpha() or re.match(r'\s*%s\b' % re.escape(sym), code)):
                continue
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


OVERRIDE_PREFIX = 'MAIN_API_OVERRIDE_'


def norm_decl(s):
    """norm() with `f(void)` and `f()` equal: they generate the same code for a call without arguments."""
    return re.sub(r'\(\s*void\s*\)', '()', norm(s))


def override_reason(texts, sym):
    """The reason of `#define MAIN_API_OVERRIDE_<sym> /* reason */` in one of the texts, '' when it has
    none, None when no text defines it."""
    for t in texts:
        m = re.search(r'^[ \t]*#[ \t]*define[ \t]+%s%s\b[ \t]*(?:/\*(.*?)\*/)?' % (OVERRIDE_PREFIX, re.escape(sym)), t, re.M)
        if m:
            return (m.group(1) or '').strip()
    return None


def cdecl_key(decl):
    """A declaration without `extern`, comments and the spaces around `*`, `(void)` as `()`."""
    d = re.sub(r'^\s*extern\s+', '', norm_decl(decl))
    return re.sub(r'\s*\*\s*', '*', d).rstrip(';').strip()


def find_definition(text, sym):
    """`ret name(params);` of a function the C text defines (a definition declares it), or None."""
    m = re.search(r'^([A-Za-z_][^;{}()\n]*?)\b%s\s*\(([^;{}]*)\)\s*\{' % re.escape(sym), text, re.M)
    return '%s%s(%s);' % (m.group(1), sym, m.group(2)) if m else None


def params_of(decl):
    """The parameter list of a function declaration or definition line, normalised; None for data."""
    m = re.search(r'\(([^()]*(?:\([^()]*\)[^()]*)*)\)\s*;?\s*$', norm_decl(decl))
    return m.group(1).strip() if m else None


def only_return_type_differs(a, b):
    """True for two function declarations that take the same parameters: the return type does not change
    how a call whose result is unused is compiled, the build decides."""
    pa, pb = params_of(a), params_of(b)
    return pa is not None and pa == pb


def value_used(code, ident):
    """True when the body uses the result of a call to `ident` (anything but a call statement)."""
    for m in re.finditer(r'\b%s\s*\(' % re.escape(ident), code):
        line_start = code.rfind('\n', 0, m.start()) + 1
        if code[line_start:m.start()].strip():
            return True
    return False


def soft_return_difference(have, want, used_value):
    """True when two declarations of a function differ only in the return type and that cannot matter
    for the copy: a call whose result is not used compiles alike for `void` and `int`. If the body
    uses the result and the target's type is `void`, the copy would not compile."""
    if not only_return_type_differs(have, want):
        return False
    return not (used_value and (return_type(have, re.search(r'(\w+)\s*\(', have).group(1)) or '') == 'void')


def effective_decl(sym, ctext, cheads, overridden):
    """(declaration line, where) of `sym` as the C file `ctext` with the header texts `cheads` sees it:
    where is 'file' (declared in the .c) or 'header'. An overridden symbol (MAIN_API_OVERRIDE_ define)
    hides the declaration in main_api.h: the file's own, or another header's, counts."""
    if overridden:
        d = find_decl([ctext], sym, True)
        if d:
            return d, 'file'
        d = find_decl([h for h in cheads if 'MAIN_API_H' not in h], sym)
        return (d, 'header') if d else (None, None)
    d = find_decl(cheads, sym)
    if d:
        return d, 'header'
    d = find_decl([ctext], sym, True) or find_definition(ctext, sym)
    return (d, 'file') if d else (None, None)


TYPEDEF_RE = re.compile(r'^typedef\s+(?:struct|union)\b[^{;]*\{.*?\n\}\s*(\w+)\s*;[^\n]*', re.M | re.S)


def local_typedef(stext, name):
    """Text of a struct/union typedef named `name` that the .c file defines itself, or None."""
    for m in TYPEDEF_RE.finditer(stext):
        if m.group(1) == name:
            return m.group(0)
    return None


def load_aggregates(root, cache):
    """The aggregates of config/migrate_globals.txt (GameState at D_800E6280) as migrate_globals sees them."""
    if 'aggs' not in cache:
        try:
            import migrate_globals as mg
            cache['aggs'] = mg.load_config(root)[0]
        except (ImportError, SystemExit, OSError, ValueError):
            cache['aggs'] = []
    return cache['aggs']


def resolve_path(agg, path):
    """(byte offset, Type, dynamic) of a field path such as `.unk_1BC[2].unk_06` of the aggregate, or None.
    A non-constant index counts as 0 (dynamic is then True): the relocation of an indexed access names
    the address of element 0."""
    import migrate_globals as mg
    t, off, dyn = agg.type, 0, False
    for m in re.finditer(r'\.\s*([A-Za-z_]\w*)|\[([^\]]*)\]', path):
        if m.group(1):
            if t.kind not in ('struct', 'union'):
                return None
            hit = next((f for f in t.fields if f[0] == m.group(1)), None)
            if hit is None or (len(hit) > 3 and hit[3]):
                return None
            off += hit[1]
            t = hit[2]
        else:
            if t.kind != 'array':
                return None
            idx = m.group(2).strip()
            if re.match(r'^(0[xX][0-9A-Fa-f]+|\d+)$', idx):
                k = int(idx, 0)
            else:
                k, dyn = 0, True
            off += k * t.elem.size
            t = t.elem
    return off, t, dyn


def game_state_edits(root, body, fwd, tname, cache):
    """Rewrite the aggregate field accesses of a source body (`D_800E6280.unk_110D`) for the target.

    The relocations of the asm name the old per-field symbols (`D_800E738D`), the C names the field of
    the aggregate (T-5100). Every path is resolved to its address; that address must be in the
    relocation sequence; when the target's symbol is another one the path of the target's field of the
    same type is written (migrate_globals.find_mapping), when it is the same the text stays.
    A field whose counterpart in the target is another global (outside the aggregate) is written with
    that global's name; it is returned in `pseudo` {name: type spelling} for the declaration check.
    Returns (new body, spans blanked for the symbol check, pseudo, reason or None)."""
    aggs = load_aggregates(root, cache)
    if not aggs:
        return body, [], {}, None
    import migrate_globals as mg
    edits, spans, pseudo = [], [], {}
    for agg in aggs:
        for m in re.finditer(r'\b%s((?:\s*\.\s*[A-Za-z_]\w*|\s*\[[^\]]*\])+)' % re.escape(agg.base), body):
            res = resolve_path(agg, m.group(1))
            if res is None:
                return body, [], {}, '%s%s is not a field path of %s' % (agg.base, m.group(1).strip(), agg.tname)
            off, typ, dyn = res
            addr = agg.addr + off
            k = ('a', addr)
            if k not in fwd:
                return body, [], {}, 'body names %s%s (0x%08X) which is not in the relocation sequence' % (
                    agg.base, m.group(1).strip(), addr)
            spans.append((m.start(), m.end()))
            tk = fwd[k]
            if tk == k:
                continue
            if dyn:
                return body, [], {}, '%s%s is indexed and the target field is another one' % (agg.base, m.group(1).strip())
            if tk[0] != 'a' or not agg.holds(tk[1]):
                if '[' in m.group(1):
                    return body, [], {}, ('%s%s is an element of a struct array and the target reads a plain global: IDO '
                                          'orders the loads of the two forms differently (T-5100)' % (agg.base, m.group(1).strip()))
                tsym = tname[tk]
                edits.append((m.start(), m.end(), tsym))
                pseudo[tsym] = typ.spell()
                continue
            mp, why = mg.find_mapping(agg, 'D_%08X' % tk[1], typ.spell(), False)
            if mp is None or mp.inner is not None:
                return body, [], {}, 'target field 0x%08X: %s' % (tk[1], why or 'inside a longer array')
            edits.append((m.start(), m.end(), mp.expr))
    if len(pseudo) >= 2:
        return body, [], {}, ('%d fields of the aggregate map to plain globals (%s): IDO orders the loads of separate '
                              'symbols differently from fields of one struct (T-5100)' % (len(pseudo), ', '.join(sorted(pseudo))))
    for a, b, new in sorted(edits, reverse=True):
        body = body[:a] + new + body[b:]
    return body, spans, pseudo, None


def blank_spans(text, spans):
    for a, b in sorted(spans, reverse=True):
        text = text[:a] + ' ' * (b - a) + text[b:]
    return text


def addr_literal_edits(code, relocs_pairs):
    """{old literal text: new} for hex literals in `code` that are the address a relocation of the source
    names (`*(s16 *)0x801F0D14` for `%lo(D_801F0D14)`) and another address in the target."""
    want = {}
    for (s1, a1), (s2, a2) in relocs_pairs:
        m1, m2 = ADDR_RE.match(s1), ADDR_RE.match(s2)
        if not (m1 and m2):
            continue
        v1, v2 = int(m1.group(1), 16) + a1, int(m2.group(1), 16) + a2
        if v1 != v2 and v1 >= 0x80000000:
            if want.setdefault(v1, v2) != v2:
                return None
    return want


def return_type(decl, name):
    """Return type of the function `name` in a declaration or definition text, or None."""
    m = re.match(r'^\s*(?:extern\s+)?([A-Za-z_][\w \t*]*?)[ \t*]*%s\s*\(' % re.escape(name), decl)
    return re.sub(r'\s+', ' ', m.group(1)).strip() if m else None


def prototype_of(definition, name):
    """`ret name(params);` of a definition text."""
    m = re.match(r'^\s*([A-Za-z_][^;{}()\n]*?)\b%s\s*\(([^;{}]*)\)\s*\{' % re.escape(name), definition)
    return '%s%s(%s);' % (m.group(1), name, m.group(2)) if m else None


def retype_definition(text, hdr_decl, name):
    """The definition `text` with the return type of the target's prototype `hdr_decl` (a header says
    `s32 f(void);` where the matched source twin is `void f(void) {...}`): same code, no header clash."""
    m = re.match(r'^(\s*)([A-Za-z_][\w \t*]*?)([ \t*])%s\s*\(' % re.escape(name), text)
    d = re.match(r'^\s*((?:extern\s+)?[A-Za-z_][\w \t*]*?)[ \t*]*%s\s*\(' % re.escape(name), hdr_decl or '')
    if not m or not d:
        return text
    ret = re.sub(r'^extern\s+', '', d.group(1)).strip()
    return text[:m.start(2)] + ret + text[m.end(2):] if norm(ret) != norm(m.group(2)) else text


def plan_copy(root, src, tgt, cache):
    """Plan the copy of matched src to unmatched tgt.

    Returns (plan, reason). plan = dict(text, tgtfile, name, decls=[(header, line)], local=[(kind, text)],
    retype=[(header, function, old return type, new)], src). local: 'define' lines go before the first
    #include of the target file (MAIN_API_OVERRIDE_ of a view the source was matched with), 'block'
    texts (file-local typedefs and declarations the source file carries) after its last #include,
    'late' texts right above the function (a declaration that needs a typedef the target .c defines
    further down). decls whose header is main_api.h are brought in line by sync_protos --fix when
    the plans are applied (apply_all). retype: a `void` prototype whose twin returns a value.
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
    if '"' in strip_comments(body) or re.search(r'\bstatic\b|#', strip_comments(body)):
        return None, 'source has string literal, static or preprocessor use'
    sheads = [read(root, h) for h in header_closure(root, sfile, cache)]
    thp = target_headers(root, tfile, tgt, cache)
    theads = [read(root, h) for h in thp]
    tall = '\n'.join(theads) + '\n' + ttext
    # fields of the game-state aggregate (T-5100): resolved through their addresses
    body, gs_spans, pseudo, why = game_state_edits(root, body, fwd, tname, cache)
    if why:
        return None, why
    # symbols of the body: the text before the field rewrite, with the aggregate paths blanked
    orig_body = stext[rng[0]:rng[1]]
    code = strip_comments(blank_spans(orig_body, gs_spans))
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
    # file-local types the body needs (struct typedefs of the source .c) move with it
    local, local_text, local_types = [], '', set()
    for w in sorted(set(re.findall(r'\b[A-Z][A-Za-z0-9_]*\b', code))):
        if w in used or ADDR_RE.match(w) or re.search(r'\b%s\b' % re.escape(w), tall):
            continue
        td = local_typedef(stext, w)
        if td is not None:
            local.append(('block', td))
            local_types.add(w)
            local_text += '\n' + td
    tall += local_text
    # type and field names used by the body must exist in the target's closure
    for w in set(re.findall(r'\b[A-Z][A-Za-z0-9_]*\b', code)) | set(re.findall(r'(?:->|\.)\s*([A-Za-z_]\w*)', code)):
        if w in used or ADDR_RE.match(w):
            continue
        if not re.search(r'\b%s\b' % re.escape(w), tall):
            return None, 'body uses %s which the target closure lacks' % w
    mapping = {src.name: tgt.name}
    decls = []
    tmain = tgt.unit == 'main'
    aggs = load_aggregates(root, cache)

    def declare(t, want, sover=None, swhere='header', ident=None, used_value=False):
        """Make the target see `want` for its symbol `t`: nothing to do, a header line, or a file-local
        addition. Returns a reason when the target already holds another view. want=None: the source
        never declared it (implicit declaration); only a function the target .c defines below needs one."""
        tover = override_reason(theads + [ttext], t)
        have, _w = effective_decl(t, ttext, theads, tover is not None)
        ismain = is_main_symbol(root, t, tgt.unit) and os.path.exists(os.path.join(root, 'include/main_api.h'))
        tdef = find_definition(ttext, t)
        if tdef and not find_decl(theads, t) and not find_decl([ttext], t, True):
            # defined in the target .c and declared nowhere: the new function calls it above its
            # definition (implicit `int f()`, then a clash), so it needs a prototype in a header
            if want is not None and cdecl_key(tdef) != cdecl_key(want) and not (
                    cache.get('lenient') or soft_return_difference(tdef, want, used_value)):
                return 'target declares %s differently: "%s" vs "%s"' % (t, norm(tdef), norm(want))
            decls.append(('include/main_api.h' if ismain else ('include/game.h' if tmain else 'include/ovl/%s.h' % tgt.unit), tdef))
            return None
        if want is None:
            return None
        if have is not None and cdecl_key(have) == cdecl_key(want):
            return None
        if sover is not None and ismain:
            # matched against its own view of a main-exe symbol: the target needs the same override
            if tover is not None and have is not None and not cache.get('lenient'):
                return 'target declares %s differently: "%s" vs "%s"' % (t, norm(have), norm(want))
            local.append(('define', '#define %s%s /* %s */' % (OVERRIDE_PREFIX, t, sover or 'matched like %s (T-7030)' % src.name)))
            local.append(('block', want))
            return None
        if have is not None:
            if cache.get('lenient') or soft_return_difference(have, want, used_value):
                return None
            return 'target declares %s differently: "%s" vs "%s"' % (t, norm(have), norm(want))
        words = set(re.findall(r'\b[A-Za-z_]\w*\b', strip_comments(want))) - BASIC_WORDS
        c_only = {w for w in words if re.match(r'[A-Z]', w) and not re.search(r'\b%s\b' % re.escape(w), '\n'.join(theads))}
        if swhere == 'file' and (local_types | c_only) & words:
            if ismain:
                return 'declaration of %s only in %s' % (ident or t, sfile)
            # a typedef only the .c files have: the copy declares it in its .c as well; right above the
            # function when the typedef is the target file's own (it may stand anywhere above)
            local.append(('block' if local_types & words else 'late', want))
            return None
        if ismain:
            dest = 'include/main_api.h'      # T-3340: one home for main-exe symbols
        elif tmain:
            dest = 'include/game.h'
        else:
            dest = 'include/ovl/%s.h' % tgt.unit
        decls.append((dest, want))
        return None

    for ident, k in sorted(used.items()):
        t = tname[fwd[k]]
        names = [t] + [n for n in al[1].get(fwd[k][1], [])] if fwd[k][0] == 'a' else [t]
        declared = [n for n in names if find_decl(theads, n) or find_decl([ttext], n, True)]
        if declared:
            t = declared[0]
        elif fwd[k] == k:
            t = ident
        sover = override_reason(sheads + [stext], ident)
        sd, swhere = effective_decl(ident, stext, sheads, sover is not None)
        agg = next((g for g in aggs if fwd[k][0] == 'a' and g.holds(fwd[k][1])), None)
        if agg is not None and not any(g.holds(k[1]) for g in aggs if k[0] == 'a'):
            # the target reads a field of the game-state aggregate, the source a plain global (T-5100)
            import migrate_globals as mg
            view = mg.declarations(sd + '\n').get(ident) if sd else None
            indexed = bool(re.search(r'\b%s\s*\[' % re.escape(ident), code))
            mp, why = mg.find_mapping(agg, 'D_%08X' % fwd[k][1], view, indexed)
            if mp is None or mp.inner is not None:
                return None, 'target field of %s: %s' % (ident, why or 'inside a longer array')
            mapping[ident] = mp.expr
            continue
        mapping[ident] = t
        if sd is None:       # implicit declaration in the source: a target function defined below still needs one
            why = declare(t, None)
            if why:
                return None, why
            continue
        want = re.sub(r'\b%s\b' % re.escape(ident), t, sd)
        for w in set(re.findall(r'\b[A-Za-z_]\w*\b', strip_comments(want))):
            if w in BASIC_WORDS or w == t or re.match(r'((D|func)_[0-9A-F]{8}|arg\d+)$', w):
                continue
            if not re.search(r'\b%s\b' % re.escape(w), tall):
                return None, 'declaration of %s uses %s which the target closure lacks' % (ident, w)
        why = declare(t, want, sover, swhere, ident, value_used(code, ident))
        if why:
            return None, why
    for t, spell in sorted(pseudo.items()):      # globals written for fields of the aggregate
        arr = spell.endswith('[]')
        want = 'extern %s %s%s;' % (spell[:-2] if arr else spell, t, '[]' if arr else '')
        why = declare(t, want)
        if why:
            return None, why
    # the address idiom: *(s16 *)0x801F0D14 stands for %lo(D_801F0D14); the target's address replaces it
    lit = addr_literal_edits(code, [((s1, a1), (s2, a2)) for (_k1, s1, a1), (_k2, s2, a2) in zip(src.relocs, tgt.relocs)])
    if lit is None:
        return None, 'one address literal maps to two target addresses'
    text = body
    if lit:
        text = re.sub(r'\b0[xX]([0-9A-Fa-f]{8})\b(?![\w.])',
                      lambda m: ('0x%08X' % lit[int(m.group(1), 16)]) if int(m.group(1), 16) in lit else m.group(0), text)
    text = re.sub(r'\b(%s)\b' % '|'.join(map(re.escape, sorted(mapping, key=len, reverse=True))),
                  lambda m: mapping[m.group(1)], text)
    # the target's own prototype: the definition must agree with it, and a call earlier in the file
    # would otherwise make the function `int f()` (implicit declaration) before the definition
    retype = []
    hdr = find_decl(theads, tgt.name) or find_decl([ttext], tgt.name, True)
    if hdr:
        sret = return_type(text, tgt.name)
        hret = return_type(hdr, tgt.name)
        if sret and hret and norm(sret) != norm(hret):
            if hret == 'void' and sret != 'void':
                # the callers' evidence said void, the matched twin returns a value: the header learns it
                for hp in thp:
                    if find_decl([read(root, hp)], tgt.name):
                        retype.append((hp, tgt.name, hret, sret))
                        break
            else:
                text = retype_definition(text, hdr, tgt.name)
    else:
        proto = prototype_of(text, tgt.name)
        if proto:
            if is_main_symbol(root, tgt.name, tgt.unit) and os.path.exists(os.path.join(root, 'include/main_api.h')):
                dest = 'include/main_api.h'
            elif tmain:
                dest = 'include/game.h'
            else:
                dest = 'include/ovl/%s.h' % tgt.unit
            decls.append((dest, proto))
    return dict(text=text, tgtfile=tfile, name=tgt.name, decls=decls, local=local, retype=retype,
                src=src.name), None


def add_local(text, local):
    """`text` of a C file with the plan's file-local additions: the override defines before its first
    #include, the typedefs and declarations after its last one. Anything already there is kept as is."""
    defs = [d for kind, d in local if kind == 'define' and d not in text]
    blocks = []
    for kind, b in local:
        if kind == 'block' and b not in text and b not in blocks:
            blocks.append(b)
    if defs:
        m = re.search(r'^[ \t]*#[ \t]*include\b', text, re.M)
        at = m.start() if m else 0
        text = text[:at] + '\n'.join(defs) + '\n' + text[at:]
    if blocks:
        last = None
        for m in re.finditer(r'^[ \t]*#[ \t]*include\b[^\n]*\n', text, re.M):
            last = m
        at = last.end() if last else 0
        text = text[:at] + '\n' + '\n'.join(blocks) + '\n' + text[at:]
    return text


def apply_plan(root, plan):
    """Write one plan; returns {path: previous text} for rollback."""
    saved = {}
    tp = os.path.join(root, plan['tgtfile'])
    with open(tp) as fh:
        t = fh.read()
    saved[plan['tgtfile']] = t
    pat = re.compile(r'^INCLUDE_ASM\("[^"]*",\s*%s\);[ \t]*$' % re.escape(plan['name']), re.M)
    late = [b for kind, b in plan.get('local', ()) if kind == 'late' and b not in t]
    body = '\n'.join(late) + '\n\n' + plan['text'].rstrip('\n') if late else plan['text'].rstrip('\n')
    t = pat.sub(lambda m: body, t, count=1)
    t = add_local(t, plan.get('local', ()))
    with open(tp, 'w') as fh:
        fh.write(t)
    for hdr, name, old, new in plan.get('retype', ()):
        hp = os.path.join(root, hdr)
        with open(hp) as fh:
            h = fh.read()
        saved.setdefault(hdr, h)
        h2 = re.sub(r'^([ \t]*)%s(\s+%s\s*\()' % (re.escape(old), re.escape(name)), lambda m: m.group(1) + new + m.group(2), h,
                    count=1, flags=re.M)
        with open(hp, 'w') as fh:
            fh.write(h2)
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


def snapshot_include(root):
    """{relative path: text} of every file under include/ (restored when a batch is rejected)."""
    out = {}
    for d, _dirs, files in os.walk(os.path.join(root, 'include')):
        for f in files:
            p = os.path.join(d, f)
            with open(p, errors='surrogateescape') as fh:
                out[os.path.relpath(p, root)] = fh.read()
    return out


def restore_include(root, snap):
    for rel, text in snap.items():
        p = os.path.join(root, rel)
        with open(p, errors='surrogateescape') as fh:
            have = fh.read()
        if have != text:
            with open(p, 'w', errors='surrogateescape') as fh:
                fh.write(text)
    for d, _dirs, files in os.walk(os.path.join(root, 'include')):
        for f in files:
            rel = os.path.relpath(os.path.join(d, f), root)
            if rel not in snap:
                os.remove(os.path.join(root, rel))


def needs_header_sync(plans):
    """True when the plans declared a main-exe symbol in main_api.h or added an override: the other
    headers and the guards of main_api.h must follow (tools/sync_protos.py --fix does that)."""
    return any(d[0] == 'include/main_api.h' for p in plans for d in p['decls']) or \
        any(r[0] == 'include/main_api.h' for p in plans for r in p.get('retype', ())) or \
        any(k == 'define' for p in plans for k, _t in p.get('local', ()))


def views_before(root, plans):
    """sync_protos snapshot of the views every file sees, taken before the plans are written (None when no
    header sync will run): the baseline for the risky-change check."""
    if not needs_header_sync(plans):
        return None
    import sync_protos
    cwd = os.getcwd()
    try:
        os.chdir(root)
        return sync_protos.snapshot(sync_protos.Model('include', 'src'))
    finally:
        os.chdir(cwd)


def sync_headers(root, plans, log=print, before=None):
    """Run sync_protos --fix when needed. Returns a reason when it left a risky view change or failed."""
    if not needs_header_sync(plans):
        return None
    import sync_protos
    cwd = os.getcwd()
    try:
        os.chdir(root)
        risky = sync_protos.run_write('include', 'src', fix=True, log=lambda *a: None, before=before,
                                      ignore={p['tgtfile'] for p in plans})
    except Exception as e:      # noqa: BLE001 - a broken header state is a rejection, not a crash
        return 'sync_protos failed: %s' % e
    finally:
        os.chdir(cwd)
    return 'sync_protos reports %d risky view change(s)' % risky if risky else None


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


def apply_all(root, plans, check, verbose, log=print, describe=None):
    """Write the plans unit by unit; with `check` build each unit and take back what does not match
    (all of a unit first, then one by one). Headers are brought in line by sync_protos --fix when a
    plan declared a main-exe symbol or an override; the whole include/ tree is restored on rejection.
    Returns the kept plans."""
    describe = describe or (lambda p: '%s <- %s (%s)' % (p['name'], p['src'], p['unit']))
    for p in plans:
        log('PLAN ' + describe(p))
    by_unit = {}
    for p in plans:
        by_unit.setdefault(p['unit'], []).append(p)
    kept = []

    def write(ps):
        """(saved files, include snapshot, problem) after writing the plans and fixing the headers."""
        snap = snapshot_include(root)
        before = views_before(root, ps)
        saved = {}
        for p in ps:
            for k, v in apply_plan(root, p).items():
                saved.setdefault(k, v)
        return saved, snap, sync_headers(root, ps, before=before)

    def undo(saved, snap):
        restore(root, saved)
        restore_include(root, snap)

    for unit, ps in sorted(by_unit.items()):
        saved, snap, problem = write(ps)
        if not check:
            kept += ps
            continue
        ok, out = (False, problem) if problem else build_ok(root, unit)
        if ok:
            kept += ps
            log('OK %s: %d copies' % (unit, len(ps)))
            continue
        if verbose and len(ps) == 1:
            log('\n'.join(out.splitlines()[-12:]))
        undo(saved, snap)
        log('FAIL %s: reverting, retrying one by one' % unit)
        for p in ps:
            saved, snap, problem = write([p])
            ok, out = (False, problem) if problem else build_ok(root, unit)
            if ok:
                kept.append(p)
                log('  keep %s' % p['name'])
            else:
                if verbose:
                    log('\n'.join(out.splitlines()[-12:]))
                undo(saved, snap)
                log('  reject %s' % p['name'])
    return kept


def file_stem(path):
    return os.path.splitext(os.path.basename(path))[0]


def wanted_file(path, spec):
    """True when the C file `path` is named by `spec` (queue.py --files syntax: comma separated main
    address stems / overlay names, matched case-insensitively as prefixes). Everything when spec is empty."""
    if not spec:
        return True
    stem = file_stem(path).lower()
    return any(w and stem.startswith(w.strip().lower()) for w in spec.split(','))


def restrict_files(plans, skipped, spec, tgt_path):
    """Plans and skipped entries limited to the target files named by `spec` (T-9030: parallel batch
    agents each pass their own list, so --apply never edits another agent's file)."""
    return ([p for p in plans if wanted_file(p['tgtfile'], spec)],
            [s for s in skipped if wanted_file(tgt_path(s[0]), spec)])


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--root', default='.')
    ap.add_argument('--files', help='only edit these C files (queue.py --files syntax: main address stems / overlay '
                                    'names, comma separated); default is the whole tree')
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
    if a.files:
        plans, skipped = restrict_files(plans, skipped, a.files, lambda t: src_for(root, t))
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
        kept = apply_all(root, plans, a.check, a.v)
        print('applied %d of %d' % (len(kept), len(plans)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
