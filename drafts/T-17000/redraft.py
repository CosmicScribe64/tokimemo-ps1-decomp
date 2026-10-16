#!/usr/bin/env python3
"""T-17000: redraft every unmatched main-exe function after the types-first pass; score it like T-14500.

Run inside the project Docker image from the repo root:
    python3 drafts/T-17000/redraft.py [--jobs 2] [--timeout 180] [--only func_X ...] [--finish-only]

Adapted from drafts/T-16550/redraft.py (the asm fold) and drafts/T-16560/redraft.py (two variants,
typed rewrites, draft_fixes, finish). Two drafts per function, each through the census path of
tools/exhaustive_drafts.py (m2c with the default context game.h + main_api.h, the bulk_m2c cleanup,
full-translation-unit compile with the cheap declaration fixes, order_search's resolved diff as the
score):
  plain  the m2c draft as the census made it; only the headers differ;
  typed  the asm handed to m2c is folded first, then the type-driven rewrites below run.
The better of the two then goes through tools/draft_fixes.py (all rewrites, each alone, greedy
build-up). The best body, its full unit, its declarations and a result.json with every score and the
census score land in drafts/T-17000/main/<func>/.

fold (from T-16550): a splat label inside a typed main aggregate (GameState D_800E6280, Rec34
  D_800B0A04, Rec24 D_801217D0, Work80125D10 D_80125D10, the typed tables of main_api.h, the
  D_8011ECD0 display records) becomes `base + offset` in the asm (`%lo(D_800E71DF)` ->
  `%lo(D_800E6280+0xF5F)`; the %hi keeps the bare base), so m2c prints fields instead of splat
  names. The m2c context sees D_8011ECD0 as DisplayActor44[]; the draft writes
  ((DisplayActor44 *)D_8011ECD0), byte offsets on it stay bytes, typed pointer steps become element
  steps, and stack copies of a one-array-struct table become one struct assignment.
Type-driven rewrites (from T-16560; each is applied, the compiler decides through the two variants):
  actor-offset    `*(T *)((unsigned char *)SYM + (N))` on a typed global table: the element and field.
  struct-walk     `GameState *p; p += 4;`: m2c types a byte walk over a struct with the struct pointer;
                  the local becomes `u8 *` and its member reads byte offsets (`*(u8 *)(p + 0x66F)`).
  pointer-step    `p += 0x24` on a local `Rec24 *p` (or `s16 *`, `T **`): the element step.
  matrix-copy     eight word copies from D_80129C40 or D_80125C60 or a stack matrix: one MATRIX copy.
  field-symbols   splat names inside a declared aggregate: the field access (tools/migrate_globals.py).
Nothing under src/ or include/ is edited.
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

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tools"))
import bulk_m2c as bulk  # noqa: E402
import draft_fixes  # noqa: E402
import exhaustive_drafts as ex  # noqa: E402
import m2c  # noqa: E402
import migrate_globals as mg  # noqa: E402

OUT = Path(os.environ.get("T17000_OUT", "drafts/T-17000"))
UNIT = "main"
CENSUS = Path("drafts/T-14500/best-quality-catalog.jsonl")
T16510 = Path("drafts/T-16510/results.jsonl")
BIG = 10 ** 6
HEADERS = ("include/libgte.h", "include/psx_types.h", "include/main_api.h", "include/game.h")
API = "include/main_api.h"
DISPLAY = ("D_8011ECD0", 0x8011ECD0, 160)          # DisplayActor44 records (game.h view)


def digest(text):
    return hashlib.sha256(text.encode()).hexdigest()


# ------------------------------------------------------------------------------------------ types

def header_types():
    """name -> mg.Type for the typedefs of the main headers."""
    text = "".join(mg.read(h) + "\n" for h in HEADERS)
    text = re.sub(r"\bstruct\s+(\w+)\s*\*", r"\1 *", mg.strip_comments_keep_offsets(text))
    return {k: v for k, v in mg.parse_types(text).items() if not isinstance(v, mg.LayoutError)}


TYPES = header_types()
DECL_RE = re.compile(r"^\s*extern\s+(\w+)\s*(\*?)\s*(D_[0-9A-F]{8})\s*(\[(\w*)\])?\s*;(?:\s*/\*\s*(\d+) records)?", re.M)
# element counts of the typed main tables declared without one (evidence in the T-17000 ticket)
COUNTS = {"D_800B0A04": 12, "D_801217D0": 12, "D_80125130": 48}


def typed_globals():
    """SYM -> (struct Type, 'ptr' | 'array' | 'obj', count) for main_api.h's struct globals."""
    out = {}
    for m in DECL_RE.finditer(mg.read(API)):
        tname, star, sym, arr, n, note = m.groups()
        t = TYPES.get(tname)
        if t is None or t.kind != "struct":
            continue
        count = int(n, 0) if n else int(note) if note else COUNTS.get(sym)
        out[sym] = (t, "ptr" if star else "array" if arr is not None else "obj", count)
    out[DISPLAY[0]] = (TYPES["DisplayActor44"], "array", DISPLAY[2])
    return out


GLOBALS = typed_globals()


def aggregate_ranges():
    """(start, end, base) of every typed main aggregate whose extent is known."""
    out = []
    for sym, (t, kind, count) in GLOBALS.items():
        if kind == "ptr" or (kind == "array" and not count):
            continue
        size = t.size * (count if kind == "array" else 1)
        out.append((int(sym[2:], 16), int(sym[2:], 16) + size, sym))
    # sized scalar arrays (`extern u8 D_800B4198[7];`): a label inside one is an element
    for m in re.finditer(r"^\s*extern\s+(?:volatile\s+)?([us](?:8|16|32))\s+(D_[0-9A-F]{8})\s*((?:\[(?:0x[0-9A-Fa-f]+|\d+)\])+)\s*;",
                         mg.read(API), re.M):
        size = int(m.group(1)[1:]) // 8
        for dim in re.findall(r"\[(\w+)\]", m.group(3)):
            size *= int(dim, 0)
        start = int(m.group(2)[2:], 16)
        if not any(a < start + size and start < b for a, b, _ in out):
            out.append((start, start + size, m.group(2)))
    return sorted(out)


RANGES = aggregate_ranges()
SYM_RE = re.compile(r"%(hi|lo)\(\s*(D_([0-9A-F]{8}))\s*(?:([+-])\s*(0x[0-9A-Fa-f]+|\d+)\s*)?\)")


LUI_LINE = re.compile(r"\blui\s+(\$\w+),\s*%hi\(")
INS_RE = re.compile(r"^\s*/\*[^*]*\*/\s+(\w+)\s+(.*?)\s*$")


def indexed_his(lines):
    """Indexes of `lui r, %hi(sym)` lines whose register gets an index added (`addu r, r, x`) before
    its %lo use: m2c needs the whole address in such a %hi, or it drops the field offset."""
    out = set()
    for i, line in enumerate(lines):
        m = LUI_LINE.search(line)
        if not m:
            continue
        reg = m.group(1)
        for line2 in lines[i + 1:i + 24]:
            if re.match(r"\s*\.L\w+:", line2):
                break
            ins = INS_RE.match(line2)
            if not ins:
                continue
            ops = [o.strip() for o in ins.group(2).split(",")]
            if ins.group(1) == "addu" and ops[0] == reg and reg in ops[1:]:
                out.add(i)
                break
            if "%%lo(" in line2 and "(%s)" % reg in line2:
                break
            if ops and ops[0] == reg:
                break
    return out


def fold(text):
    """T-16550 fold: a label inside a typed aggregate becomes base + offset in the asm given to m2c."""
    lines = text.split("\n")
    keep = indexed_his(lines)
    return "\n".join(fold_line(l, i in keep) for i, l in enumerate(lines))


def fold_line(text, indexed=False):
    def rep(m):
        addr = int(m.group(3), 16)
        if m.group(4):
            addr += int(m.group(5), 0) * (-1 if m.group(4) == "-" else 1)
        for start, end, base in RANGES:
            if start <= addr < end and m.group(2) != base:
                off = addr - start
                # m2c reads a lui %hi(sym+k) as the whole address sym+k for every %lo paired with it;
                # give the hi the bare base when its high half is the same, so each %lo keeps its own k
                if m.group(1) == "hi" and not indexed and (start + 0x8000) >> 16 == (addr + 0x8000) >> 16:
                    off = 0
                return "%%%s(%s+0x%X)" % (m.group(1), base, off) if off else "%%%s(%s)" % (m.group(1), base)
        return m.group(0)
    return SYM_RE.sub(rep, text)


def table_types():
    """sym -> (type, array field) for main globals whose type is a one-array-field struct (stack tables)."""
    text = mg.read(API)
    field = {}
    for m in re.finditer(r"typedef\s+struct\s+\w*\s*\{\s*(?:/\*[^*]*\*/\s*)?\w+\s+(\w+)\[[^\]]+\];\s*\}\s*(\w+)\s*;", text):
        field[m.group(2)] = m.group(1)
    out = {}
    for m in DECL_RE.finditer(text):
        tname, star, sym, arr = m.group(1), m.group(2), m.group(3), m.group(4)
        if tname in field and not star and arr is None:
            out[sym] = (tname, field[tname])
    return out


TABLES = table_types()
COPY_HEAD = re.compile(r"^(\s+)(?:\*\(M2cBlock\d+ \*\)\(&)?(sp\w+)\)?(?:\.unk0)? = (?:\*\(M2cBlock\d+ \*\)\(&)?(?:\(\w+\) )?(D_[0-9A-F]{8})\)?(?:\.unk0)?;$", re.M)


def struct_copies(body, tables=TABLES):
    """T-16550: m2c prints a struct copy of a constant table to the stack as one assignment per word;
    write it as `spXX = D_YYY;` with spXX of the table's type, and its element uses as spXX.v[i]."""
    for m in list(COPY_HEAD.finditer(body)):
        indent, sp, sym = m.groups()
        if sym not in tables:
            continue
        tname, fld = tables[sym]
        word = r"\*\(\w+ \*\)\(\(unsigned char \*\)\(&%s\) \+ \(\d+\)\) = (?:\(\w+\) )?\*\(\w+ \*\)\(\(unsigned char \*\)\(&%s\) \+ \(\d+\)\);" % (sp, sym)
        out, done = [], False
        for line in body.split("\n"):
            if not done and line == m.group(0).strip("\n"):
                out.append("%s%s = %s;" % (indent, sp, sym))
                done = True
            elif re.fullmatch(r"\s+" + word, line):
                continue
            else:
                out.append(line)
        if not done:
            continue
        body = "\n".join(out)
        body = re.sub(r"^(\s+)\w+ %s(?:\[\d+\])?;$" % sp, r"\g<1>%s %s;" % (tname, sp), body, count=1, flags=re.M)
        body = re.sub(r"\(&%s\)\[" % sp, "%s.%s[" % (sp, fld), body)
        body = re.sub(r"&%s\b(?!\.)" % sp, "%s.%s" % (sp, fld), body)
        body = re.sub(r"(?<![.\w])%s\[" % sp, "%s.%s[" % (sp, fld), body)
        body = re.sub(r"(?<![.\w&])%s\b(?![.\w\[])(?! = %s;)" % (sp, sym), "%s.%s[0]" % (sp, fld), body)
        body = body.replace("%s %s.%s[0];" % (tname, sp, fld), "%s %s;" % (tname, sp))
    return body


def display_view(body):
    """T-16550 post-fix: the draft sees D_8011ECD0 as u8[] in the real unit; m2c saw DisplayActor44[]."""
    def disp(m):
        n = int(m.group(1), 0)
        return "@@REC%d@@" % (n // 0x44) if n % 0x44 == 0 else "@@BYTE%d@@" % n
    body = re.sub(r"\bD_8011ECD0 \+ (0x[0-9A-Fa-f]+|\d+)\b", disp, body)
    body = re.sub(r"\bD_8011ECD0\b", "((DisplayActor44 *)D_8011ECD0)", body)
    body = re.sub(r"@@REC(\d+)@@", r"&((DisplayActor44 *)D_8011ECD0)[\1]", body)
    return re.sub(r"@@BYTE(\d+)@@", lambda m: "(DisplayActor44 *)(D_8011ECD0 + 0x%X)" % int(m.group(1)), body)


def install_fold():
    """Patch m2c so the typed variant folds the asm and sees the display records as structs."""
    orig_hi = m2c.fix_shared_hi
    m2c.fix_shared_hi = lambda t: orig_hi(fold(t))
    orig_ctx = m2c.build_context

    def build_context(root, heads, out_path, *a, **k):
        orig_ctx(root, heads, out_path, *a, **k)
        s = open(out_path).read()
        s = re.sub(r"\bextern\s+u8\s+D_8011ECD0\s*\[\s*\]\s*;", "", s) + "\nextern DisplayActor44 D_8011ECD0[];\n"
        open(out_path, "w").write(s)
    m2c.build_context = build_context
    orig_split = m2c.split_draft

    def split_draft(text, name):
        decls, body = orig_split(text, name)
        body = struct_copies(body)
        return decls, body
    m2c.split_draft = split_draft


def leaf_at(t, rel, want_size):
    """C path (".a.b[2]") of the scalar of `want_size` bytes that starts at `rel` inside `t`."""
    for off, size, path, lt in mg.walk_layout(t):
        if lt.kind == "array":
            elem = lt.elem
            while elem.kind == "array":
                elem = elem.elem
            if off <= rel < off + size and (rel - off) % elem.size == 0 and elem.size == want_size:
                idx, rest = rel - off, []
                dims, e = [], lt
                while e.kind == "array":
                    dims.append(e.count)
                    e = e.elem
                stride = [elem.size] * len(dims)
                for i in range(len(dims) - 2, -1, -1):
                    stride[i] = stride[i + 1] * dims[i + 1]
                for s in stride:
                    rest.append(idx // s)
                    idx %= s
                return path + "".join("[%d]" % i for i in rest), elem
        elif off == rel and size == want_size and lt.kind in ("scalar", "ptr"):
            return path, lt
    return None, None


SIZES = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4}
OFF_RE = re.compile(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)\(?(&?)(D_[0-9A-F]{8})(?:\[([^\[\]]+)\])?\)? \+ \((\d+)\)\)")


def actor_offsets(body):
    def element(sym, idx, k):
        if idx is None:
            return "%s[%d]" % (sym, k)
        return "%s[%s + %d]" % (sym, idx, k) if k else "%s[%s]" % (sym, idx)

    def repl(m):
        ctype, amp, sym, idx, n = m.group(1), m.group(2), m.group(3), m.group(4), int(m.group(5))
        if sym not in GLOBALS:
            return m.group(0)
        t, kind = GLOBALS[sym][:2]
        if idx is not None and not amp:
            return m.group(0)
        k, r = divmod(n, t.size)
        path, lt = leaf_at(t, r, SIZES[ctype])
        if path is None:
            return m.group(0)
        if idx is not None:                       # &SYM[i] + n
            expr = element(sym, idx, k) + path
        elif amp and kind == "obj":               # &SYM + n
            if k:
                return m.group(0)
            expr = sym + path
        elif amp and kind == "ptr":               # address of the pointer itself
            return m.group(0)
        else:                                     # SYM + n (pointer value or array)
            expr = element(sym, None, k) + path
        if kind == "ptr" and k == 0 and idx is None:
            expr = sym + "->" + path[1:]
        store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
        if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
            return expr                           # a store does not depend on the signedness
        return "*(%s *)&%s" % (ctype, expr)
    body = OFF_RE.sub(repl, body)

    def plus(m):
        sym, n = m.group(1), int(m.group(2), 0)
        if sym not in GLOBALS or GLOBALS[sym][1] == "obj":
            return m.group(0)
        t = GLOBALS[sym][0]
        if n % t.size:
            return m.group(0)
        return "&%s[%d]" % (sym, n // t.size)
    return re.sub(r"(?<![\w&])(D_[0-9A-F]{8}) \+ (0x[0-9A-Fa-f]+|\d+)\b(?!\s*[\[(])", plus, body)


LOCAL_PTR_RE = re.compile(r"^\s*(\w+) (\*+)(\w+);\s*$", re.M)
SCALAR_SIZES = {"s16": 2, "u16": 2, "s32": 4, "u32": 4}


def pointer_steps(body):
    """m2c prints pointer steps in bytes (`Rec24 *p; p += 0x24`, `s16 **q; q += 4`); C counts elements."""
    for tname, stars, name in LOCAL_PTR_RE.findall(body):
        if len(stars) > 1:
            size = 4
        elif tname in SCALAR_SIZES:
            size = SCALAR_SIZES[tname]
        else:
            t = TYPES.get(tname)
            size = t.size if t is not None and t.kind == "struct" else 0
        if size <= 1:
            continue

        def step(m, size=size):
            n = int(m.group(3), 0)
            if n % size:
                return m.group(0)
            return "%s %s %d" % (m.group(1), m.group(2), n // size)
        body = re.sub(r"\b(%s) ([-+]=) (0x[0-9A-Fa-f]+|\d+)\b" % re.escape(name), step, body)
    return body


MATRIX_GLOBALS = {"D_80129C40": "%s", "D_80125C60": "*(MATRIX *)%s"}
OBJ50_FIELDS = {"unk0": 0, "unk18": 0x18, "unk1C": 0x1C, "unk20": 0x20}
ID = r"[A-Za-z_]\w*"


def word_ref(expr):
    """(base, offset) of a word access as m2c and the bulk cleanup print it, or None.
    base is a variable name, or '*' + pointer name for an access through a pointer."""
    e = expr.strip()
    while e.startswith("(s32) "):
        e = e[6:].strip()
    pats = (
        (r"^\(\*\(s32 \*\)&(%s)\)$" % ID, lambda m: (m.group(1), 0)),
        (r"^\*\(s32 \*\)\(\(unsigned char \*\)\(&(%s)\) \+ \((\d+)\)\)$" % ID, lambda m: (m.group(1), int(m.group(2)))),
        (r"^\*\(s32 \*\)\(\(unsigned char \*\)(%s) \+ \((\d+)\)\)$" % ID,
         lambda m: (m.group(1) if m.group(1) in MATRIX_GLOBALS else "*" + m.group(1), int(m.group(2)))),
        (r"^\(\*\(s32 \*\)(%s)\)$" % ID,
         lambda m: (m.group(1) if m.group(1) in MATRIX_GLOBALS else "*" + m.group(1), 0)),
        (r"^(%s)\.t\[(\d)\]$" % ID, lambda m: (m.group(1), 0x14 + 4 * int(m.group(2)))),
        (r"^(%s)\.unk0$" % ID, lambda m: (m.group(1), 0)),
        (r"^(%s)->(unk0|unk18|unk1C|unk20)$" % ID, lambda m: ("*" + m.group(1), OBJ50_FIELDS[m.group(2)])),
        (r"^(D_80129C40)$", lambda m: (m.group(1), 0)),
    )
    for pat, fn in pats:
        m = re.match(pat, e)
        if m:
            return fn(m)
    return None


def matrix_lvalue(base, off, body):
    if not base.startswith("*"):
        if base in MATRIX_GLOBALS and off == 0:
            return MATRIX_GLOBALS[base] % base
        return base if off == 0 else None
    ptr = base[1:]
    return "*(MATRIX *)%s" % ptr if off == 0 else "*(MATRIX *)((u8 *)%s + %d)" % (ptr, off)


def matrix_copies(body):
    """Eight word copies that move a whole MATRIX (from D_80129C40, D_80125C60, a coordinate object
    or a stack matrix), in any order and interleaved with other statements: one struct copy."""
    lines = body.split("\n")
    groups = {}
    for i, l in enumerate(lines):
        m = re.match(r"^(\s*)(.+?) = (.+);\s*$", l)
        if not m:
            continue
        d, s_ = word_ref(m.group(2)), word_ref(m.group(3))
        if d is None or s_ is None or d[0] == s_[0]:
            continue
        key = (d[0], s_[0], d[1] - s_[1])
        groups.setdefault(key, []).append((i, s_[1]))
    retype = set()
    for (dbase, sbase, delta), items in groups.items():
        offs = sorted(o for _, o in items)
        if len(items) != 8 or len(set(offs)) != 8:
            continue
        start = min(offs)
        if offs != list(range(start, start + 32, 4)):
            continue
        dst = matrix_lvalue(dbase, start + delta, body)
        src = matrix_lvalue(sbase, start, body)
        if dst is None or src is None:
            continue
        first = min(i for i, _ in items)
        indent = re.match(r"^(\s*)", lines[first]).group(1)
        lines[first] = "%s%s = %s;" % (indent, dst, src)
        for i, _ in items:
            if i != first:
                lines[i] = None
        for b in (dbase, sbase):
            if not b.startswith("*") and b not in MATRIX_GLOBALS:
                retype.add(b)
    out = [l for l in lines if l is not None]
    text = "\n".join(out)
    for name in retype:
        text = re.sub(r"^(\s*)(?:s32|\?) %s(?:\[\d*\])?;" % re.escape(name), r"\1MATRIX %s;" % name, text,
                      flags=re.M)
    for g in MATRIX_GLOBALS:
        text = re.sub(r"\b%s\.unk0\b" % g, "(*(s32 *)&%s)" % g, text)
    return text


class TypedAggregate(mg.Aggregate):
    """An aggregate built from a declaration instead of config/migrate_globals.txt."""

    def __init__(self, base, typ, header):
        self.base, self.tname, self.header = base, typ.spell(), header
        self.addr = int(base[2:], 16)
        self.types, self.type = TYPES, typ
        self.end = self.addr + typ.size


def aggregates():
    aggs, _keep = mg.load_config(".")
    have = {a.base for a in aggs}
    for sym, (t, kind, count) in GLOBALS.items():
        if sym in have or kind == "ptr" or sym == DISPLAY[0]:
            continue
        if kind == "array":
            if not count:
                continue
            t = mg.Type("array", size=t.size * count, align=t.align, elem=t, count=count)
        aggs.append(TypedAggregate(sym, t, API))
    return aggs


AGGS = aggregates()


def field_symbols(body, known):
    """(body, known) with splat names inside an aggregate written as fields; their declarations dropped."""
    views = {}
    for d in known.values():
        views.update(mg.declarations(d.rstrip() + "\n"))
    hdr = mg.declarations(mg.read(API))
    new, _problems = mg.rewrite_text(body, AGGS, set(), None, lambda s: views.get(s) or hdr.get(s))
    used = set(re.findall(r"\bD_[0-9A-F]{8}\b", new))
    known = {k: v for k, v in known.items() if not (k.startswith("D_") and k not in used)}
    return new, known


def path_leaf(t, path):
    """(offset, leaf type) of a C member path (`.unk_66C[0].unk_03`) inside struct type `t`, or None."""
    off = 0
    for name, idx in re.findall(r"\.(\w+)|\[(\d+)\]", path):
        if name:
            if t.kind not in ("struct", "union"):
                return None
            hit = [f for f in t.fields if f[0] == name]
            if not hit or hit[0][3]:              # unknown member or bit-field
                return None
            off += hit[0][1]
            t = hit[0][2]
        else:
            if t.kind != "array":
                return None
            off += int(idx) * t.elem.size
            t = t.elem
    return off, t


def struct_walks(body):
    """m2c types a byte walk over a struct (`GameState *p; ... p += 4;`) with the struct's pointer, so
    the step counts whole structs. Such a local becomes `u8 *` and its member reads byte offsets."""
    for tname, name in re.findall(r"^\s*(\w+) \*(\w+);\s*$", body, re.M):
        t = TYPES.get(tname)
        if t is None or t.kind != "struct" or t.size <= 1:
            continue
        steps = [int(n, 0) for n in re.findall(r"\b%s [-+]= (0x[0-9A-Fa-f]+|\d+);" % re.escape(name), body)]
        if not steps or all(n % t.size == 0 for n in steps) or re.search(r"\b%s\[" % re.escape(name), body):
            continue
        bad = []

        def member(m, t=t):
            got = path_leaf(t, "." + m.group(1))
            if got is None or got[1].kind not in ("scalar", "ptr"):
                bad.append(m.group(0))
                return m.group(0)
            ctype = got[1].name if got[1].kind == "scalar" else "s32"
            return "*(%s *)(%s + 0x%X)" % (ctype, name, got[0])
        new = re.sub(r"\b%s->(\w+(?:\[\d+\]|\.\w+)*)" % re.escape(name), member, body)
        if bad:
            continue
        new = re.sub(r"^(\s*)%s \*%s;" % (re.escape(tname), re.escape(name)), r"\1u8 *%s;" % name, new, count=1, flags=re.M)
        new = re.sub(r"\b(%s\s*(?:=|!=|==|<|>|<=|>=)\s*)(&?D_[0-9A-F]{8}\b)" % re.escape(name), r"\1(u8 *)\2", new)
        body = new
    return body


def typed_rewrites(body, known, log=None, asm_text=None):
    steps = (("actor-offset", actor_offsets), ("struct-walk", struct_walks), ("pointer-step", pointer_steps),
             ("matrix-copy", matrix_copies))
    for name, fn in steps:
        new = fn(body)
        if new != body and log is not None:
            log.append(name)
        body = new
    new, known = field_symbols(body, known)
    if new != body and log is not None:
        log.append("field-symbols")
    return display_view(new), known


# ------------------------------------------------------------------------------------------ workers

def inventory():
    rows = [r for r in ex.census() if r["unit"] == UNIT]
    return rows


def worker(index, variant):
    rows = json.loads((OUT / "inventory.json").read_text())
    row = rows[index]
    ex.BASE = OUT / "_runs" / variant
    os.environ["TOKIMEMO_REGENERATE_GUARDS"] = "1"
    applied = []
    if variant == "typed":
        install_fold()
        plain_cleanup = bulk.cleanup

        def cleanup(decls, body, glob=None):
            body, known, problem = plain_cleanup(decls, body, glob)
            if problem:
                return body, known, problem
            asm = draft_fixes.asm_for(row["scope"], draft_fixes.asm_files("."))
            body, known = typed_rewrites(body, known, applied, asm)
            return body, known, None
        bulk.cleanup = cleanup
    got = ex.process(row)
    if variant == "typed":
        got["rewrites"] = applied
        ex.save(ex.BASE / row["unit"] / row["func"] / "result.json", json.dumps(got, sort_keys=True) + "\n")


def run_all(rows, variants, jobs, timeout, only):
    todo = [(i, v) for v in variants for i, r in enumerate(rows) if not only or r["func"] in only]
    todo = [(i, v) for i, v in todo if not (OUT / "_runs" / v / UNIT / rows[i]["func"] / "result.json").exists()]
    running = []
    while todo or running:
        while todo and len(running) < jobs:
            i, v = todo.pop(0)
            folder = OUT / "_runs" / v / UNIT / rows[i]["func"]
            folder.mkdir(parents=True, exist_ok=True)
            log = open(folder / "worker.log", "w")
            p = subprocess.Popen([sys.executable, __file__, "--worker", str(i), "--variant", v], stdout=log,
                                 stderr=subprocess.STDOUT, start_new_session=True)
            running.append((p, i, v, time.monotonic(), log))
        time.sleep(0.2)
        for item in list(running):
            p, i, v, t0, log = item
            if p.poll() is None and time.monotonic() - t0 < timeout:
                continue
            if p.poll() is None:
                os.killpg(p.pid, signal.SIGKILL)
                p.wait()
            log.close()
            running.remove(item)
            res = OUT / "_runs" / v / UNIT / rows[i]["func"] / "result.json"
            if not res.exists():
                ex.save(res, json.dumps(dict(rows[i], status="timeout" if p.returncode < 0 else "worker-error",
                                             score=None, variant=v)) + "\n")
            got = json.loads(res.read_text())
            print(v, rows[i]["func"], got.get("status"), got.get("score"), flush=True)


# ------------------------------------------------------------------------------------------ final

def census_scores():
    out = {}
    for line in CENSUS.read_text().splitlines():
        r = json.loads(line)
        if r["scope"].startswith(UNIT + ":"):
            out[r["scope"].split(":")[1]] = dict(score=r["score"], path=r["selected_path"])
    if T16510.exists():
        for line in T16510.read_text().splitlines():
            r = json.loads(line)
            if r.get("unit") == UNIT and r.get("score") is not None and r["status"] in ("improved", "zero"):
                if r["score"] < (out[r["func"]]["score"] or BIG):
                    out[r["func"]] = dict(score=r["score"], path="drafts/T-16510/%s/%s" % (UNIT, r["func"]))
    return out


def finish(rows, only):
    files = draft_fixes.asm_files(".")
    census = census_scores()
    summary = []
    for row in rows:
        func = row["func"]
        if only and func not in only:
            continue
        runs = {}
        for v in ("plain", "typed"):
            folder = OUT / "_runs" / v / UNIT / func
            res = folder / "result.json"
            if res.exists():
                runs[v] = (json.loads(res.read_text()), folder)
        scored = {v: r for v, r in runs.items() if r[0].get("score") is not None}
        dest = OUT / UNIT / func
        dest.mkdir(parents=True, exist_ok=True)
        record = dict(scope=row["scope"], func=func, src=row["src"], size=row["size"], unit=UNIT,
                      guarded=row["guarded"], census_score=census[func]["score"], census_path=census[func]["path"],
                      plain_score=runs.get("plain", ({},))[0].get("score"),
                      typed_score=runs.get("typed", ({},))[0].get("score"),
                      rewrites=runs.get("typed", ({},))[0].get("rewrites", []),
                      tool="T-17000 drafts/T-17000/redraft.py", harness_hash=digest(Path(__file__).read_text()),
                      created=time.strftime("%Y-%m-%d", time.gmtime()))
        if not scored:
            record.update(status="failed", score=None,
                          reason=" | ".join(r[0].get("reason", "") for r in runs.values())[:300])
            src = runs.get("typed") or runs.get("plain")
            if src and (src[1] / "body.c").exists():
                shutil.copy(src[1] / "body.c", dest / "body.c")
        else:
            v = min(scored, key=lambda k: (scored[k][0]["score"], k != "typed"))
            got, folder = scored[v]
            body = (folder / "body.c").read_text()
            if (folder / "context.c").exists():
                context = (folder / "context.c").read_text()
            else:                                  # pruned run folder: rebuild the unit from src
                context = draft_fixes.fresh_context(dict(row), body, folder)
            record.update(variant=v, score=got["score"])
            if got["score"] > 0:
                scorer = draft_fixes.make_scorer(dict(row))
                base, score, chosen, new_body, new_context, tried = draft_fixes.best_rewrite(
                    row, body, draft_fixes.asm_for(row["scope"], files), context, scorer, OUT / "_work" / func)
                record.update(draft_fixes_baseline=base, draft_fixes=["%s#%d" % c for c in chosen],
                              draft_fixes_tried=tried)
                if score < got["score"]:
                    body, context = new_body, new_context
                    record["score"] = score
            for name in ("declarations.h",):
                if (folder / name).exists():
                    shutil.copy(folder / name, dest / name)
            (dest / "body.c").write_text(body)
            (dest / "context.c").write_text(context)
            record["status"] = "candidate-zero" if record["score"] == 0 else "scored"
            record["draft_hash"], record["context_hash"] = digest(body), digest(context)
        c = record["census_score"]
        s = record["score"]
        record["vs_census"] = None if s is None or c is None else ("better" if s < c else "same" if s == c else "worse")
        (dest / "result.json").write_text(json.dumps(record, sort_keys=True) + "\n")
        summary.append(record)
    merged = {}
    if (OUT / "catalog.jsonl").exists():
        for line in (OUT / "catalog.jsonl").read_text().splitlines():
            r = json.loads(line)
            merged[r["func"]] = r
    for r in summary:
        merged[r["func"]] = r
    with open(OUT / "catalog.jsonl", "w") as fh:
        for row in rows:
            if row["func"] in merged:
                fh.write(json.dumps(merged[row["func"]], sort_keys=True) + "\n")
    shutil.rmtree(OUT / "_work", ignore_errors=True)
    return summary


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--worker", type=int)
    p.add_argument("--variant", choices=("plain", "typed"))
    p.add_argument("--jobs", type=int, default=2)
    p.add_argument("--timeout", type=int, default=180)
    p.add_argument("--only", nargs="*")
    p.add_argument("--finish-only", action="store_true")
    p.add_argument("--rewrite", help="print the typed rewrite of one body file and exit")
    p.add_argument("--scope", help="UNIT:func for --rewrite (asm-based rewrites)")
    a = p.parse_args()
    if a.rewrite:
        log = []
        asm = draft_fixes.asm_for(a.scope, draft_fixes.asm_files(".")) if a.scope else None
        body, _ = typed_rewrites(Path(a.rewrite).read_text(), {}, log, asm)
        sys.stdout.write(body)
        print(log, file=sys.stderr)
        return
    if a.worker is not None:
        worker(a.worker, a.variant)
        return
    OUT.mkdir(parents=True, exist_ok=True)
    if not (OUT / "inventory.json").exists():
        ex.save(OUT / "inventory.json", json.dumps(inventory(), indent=1) + "\n")
    if not (OUT / "environment.json").exists():
        env = dict(base=subprocess.run(["git", "rev-parse", "HEAD"], capture_output=True, text=True).stdout.strip(),
                   files={h: digest(mg.read(h)) for h in HEADERS})
        ex.save(OUT / "environment.json", json.dumps(env, indent=1, sort_keys=True) + "\n")
    rows = json.loads((OUT / "inventory.json").read_text())
    for v in ("plain", "typed"):
        (OUT / "_runs" / v).mkdir(parents=True, exist_ok=True)
        shutil.copy(OUT / "environment.json", OUT / "_runs" / v / "environment.json")
    if not a.finish_only:
        run_all(rows, ("plain", "typed"), a.jobs, a.timeout, a.only)
    summary = finish(rows, a.only)
    print("DONE", len(summary), sum(r["status"] == "candidate-zero" for r in summary), "zero", flush=True)


if __name__ == "__main__":
    main()
