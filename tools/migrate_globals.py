#!/usr/bin/env python3
"""Fold splat globals into the aggregate that holds them, and keep them folded (T-5100).

splat names every address the original code touches, so the fields of one struct of the original
show up as many `D_` globals. Once the struct is declared (for example `GameState D_800E6280` in
include/main_api.h, wiki/game-state.md), every C use of such a global must become a field access
on the struct: separate symbols let IDO move loads above stores that the original kept in order,
and the old names hide the layout. This tool does that rewrite and keeps it done.

Configuration, config/migrate_globals.txt:
  aggregate <base symbol> <type> <header>     every D_XXXXXXXX inside [base, base + sizeof(type))
                                              is a field of <base>; <type> is parsed from <header>
  keep <src file> <symbol> <reason ...>       a use that matched only on the old view: the file keeps
                                              <symbol> (declared in the header with its old type)

The layout comes from the header itself: `typedef struct|union Name { ... } Name;` blocks with
fields of the fixed-width types, pointers, arrays, bit-fields (packed into u32 units) and other
typedefs of the same header. Field offsets follow the MIPS ABI alignment; a `/* 0xNN */` comment
in front of a field must equal the computed offset (checked).

A use of `D_X` becomes the path of the subobject at offset X - base whose type equals the old view
of `D_X` (the declaration the file saw: its own override, the overlay header's, else the main
header's). A scalar maps to the scalar field (`D_800E6280.unk_03E`, `D_800E6280.unk_1BC[2].unk_06`,
`D_800E6280.unk_0F4.b[1]` for a byte of a union); an array to the array field that starts there,
or, when the old array starts inside a longer array, to an index into that array
(`D_X[i]` -> `ARR[k + i]`). Without a declaration (new code), the deepest scalar at that offset is
used, or the array that starts there when the use is indexed. The base symbol itself must be used
as a struct (`D_800E6280.f`, `&D_800E6280`); an old view of it (`D_800E6280[i]`, a scalar
override) is reported for a hand rewrite unless a scalar field of that type sits at offset 0.

Modes (run from the repo root, inside Docker like every tool):
  migrate_globals.py --check     report every C use of an absorbed symbol with the field to use, every
                                 old-view use of a base symbol, and layout comment errors; exit 1 if any
                                 (ninja runs this: build/globals.ok)
  migrate_globals.py --apply     rewrite src/**/*.c, delete the absorbed declarations and their
                                 MAIN_API_OVERRIDE_ guards from include/ and src/ (kept symbols stay);
                                 safe to re-run: a migrated tree is left unchanged
  migrate_globals.py --dry-run   what --apply would change, per file
  migrate_globals.py --layout    the computed layout of each aggregate (offset, size, field)
  migrate_globals.py --at ADDR   the field paths at an address (0x800E62BE or D_800E62BE)
"""
import argparse
import os
import re
import sys

CONFIG = "config/migrate_globals.txt"
BASE_SIZES = {"s8": 1, "u8": 1, "s16": 2, "u16": 2, "s32": 4, "u32": 4, "char": 1, "short": 2,
              "int": 4, "long": 4, "void": 1}
SYM_RE = re.compile(r"^D_([0-9A-F]{8})$")
# comments, string and character literals, identifiers; everything else passes through
TOKEN_RE = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|[A-Za-z_]\w*', re.S)
DECL_RE = re.compile(r"^[ \t]*extern[ \t]+(?:volatile[ \t]+)?([A-Za-z_]\w*)[ \t]+(\**)[ \t]*"
                     r"(D_[0-9A-F]{8})[ \t]*((?:\[[^\]]*\])*)[ \t]*;[ \t]*(?:/\*.*?\*/)?[ \t]*\n?", re.M)
OVR_DEF_RE = r"^[ \t]*#[ \t]*define[ \t]+MAIN_API_OVERRIDE_%s\b[^\n]*\n?"
OVR_GUARD_RE = r"^[ \t]*#[ \t]*ifndef[ \t]+MAIN_API_OVERRIDE_%s[ \t]*\n((?:(?!#[ \t]*endif).*\n)*?)[ \t]*#[ \t]*endif[^\n]*\n?"


class LayoutError(Exception):
    pass


# ------------------------------------------------------------------------------------- types

class Type:
    """kind: 'scalar' (name), 'ptr', 'array' (elem, count), 'struct'/'union' (name, fields)."""

    def __init__(self, kind, name=None, size=0, align=1, elem=None, count=None, fields=None):
        self.kind, self.name, self.size, self.align = kind, name, size, align
        self.elem, self.count, self.fields = elem, count, fields or []

    def spell(self):
        if self.kind == "array":
            return self.elem.spell() + "[]"
        if self.kind == "ptr":
            return "%s *" % (self.name or "void")
        return self.name

    def same(self, other):
        if self.kind == "array" and other.kind == "array":
            return self.elem.same(other.elem)
        return self.kind == other.kind and self.name == other.name


def strip_comments(text):
    return re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), text, flags=re.S)


def parse_types(text):
    """name -> Type for every `typedef struct|union X { ... } X;` in text (in order). A block this
    parser cannot read is recorded as an error and only fails when an aggregate needs it."""
    types = {}
    for m in re.finditer(r"typedef\s+(struct|union)\s+(\w+)?\s*\{(.*?)\}\s*(\w+)\s*;", text, flags=re.S):
        kind, body, name = m.group(1), m.group(3), m.group(4)
        try:
            types[name] = layout(kind, name, body, types)
        except LayoutError as e:
            types[name] = e
    return types


def base_type(name, types):
    if name in BASE_SIZES:
        sz = BASE_SIZES[name]
        return Type("scalar", name, sz, sz)
    t = types.get(name)
    if isinstance(t, LayoutError):
        raise t
    if t is None:
        raise LayoutError("unknown type %s" % name)
    return t


FIELD_RE = re.compile(r"^(?:/\*\s*(0x[0-9A-Fa-f]+)\s*\*/)?\s*(?:volatile\s+|unsigned\s+|signed\s+)*"
                      r"(\w+)\s*(\**)\s*(\w+)\s*((?:\[[^\]]+\])*)\s*(?::\s*(\d+))?\s*$")


def layout(kind, name, body, types):
    fields, off, align, bits = [], 0, 1, None   # bits: (unit offset, used bits) of an open u32 unit
    size = 0
    for raw in body.split(";"):
        line = " ".join(raw.split())
        if not line:
            continue
        m = FIELD_RE.match(line)
        if not m:
            raise LayoutError("%s: cannot parse field '%s'" % (name, line))
        comment, tname, stars, fname, dims, width = m.groups()
        if width is not None:                  # bit-field: packed into 32-bit units
            w = int(width)
            if kind == "union":
                fo = 0
            else:
                if bits is None or bits[1] + w > 32:
                    off = (off + 3) & ~3
                    bits = (off, 0)
                    off += 4
                fo = bits[0]
                bits = (fo, bits[1] + w)
            align = max(align, 4)
            fields.append((fname, fo, Type("scalar", "u32", 4, 4), True))
            size = max(size, fo + 4)
            continue
        bits = None
        t = Type("ptr", tname, 4, 4) if stars else base_type(tname, types)
        for d in reversed(re.findall(r"\[([^\]]+)\]", dims)):
            n = int(d, 0)
            t = Type("array", size=t.size * n, align=t.align, elem=t, count=n)
        fo = 0 if kind == "union" else (off + t.align - 1) // t.align * t.align
        if comment is not None and int(comment, 16) != fo:
            raise LayoutError("%s.%s: comment says %s, computed offset 0x%X" % (name, fname, comment, fo))
        fields.append((fname, fo, t, False))
        align = max(align, t.align)
        if kind == "union":
            size = max(size, t.size)
        else:
            off = fo + t.size
            size = off
    size = (size + align - 1) // align * align
    return Type(kind, name, size, align, fields=fields)


def subobjects(t, rel, path=""):
    """(path, Type) of every subobject of type t that starts at byte rel, outermost first."""
    out = []
    if rel == 0:
        out.append((path, t))
    if t.kind in ("struct", "union"):
        for fname, fo, ft, _bit in t.fields:
            if fo <= rel < fo + ft.size:
                out += subobjects(ft, rel - fo, path + "." + fname)
    elif t.kind == "array":
        k, r = divmod(rel, t.elem.size)
        if k < t.count:
            out += subobjects(t.elem, r, "%s[%d]" % (path, k))
    return [o for o in out if o[0]]


def walk_layout(t, off=0, path=""):
    """(offset, size, path, type) of every leaf (scalar or array of scalars), in order."""
    if t.kind in ("struct", "union"):
        for fname, fo, ft, _bit in t.fields:
            yield from walk_layout(ft, off + fo, path + "." + fname)
    else:
        yield off, t.size, path, t


def parse_view(spell, types):
    """Type for an old declaration spelled 'u8', 'u8[]', 'Rec38[]', 's32 *'."""
    spell = spell.strip()
    arr = spell.endswith("]")
    name = re.sub(r"\[.*$", "", spell).strip()
    if name.endswith("*"):
        t = Type("ptr", name.rstrip("* ").strip(), 4, 4)
    else:
        try:
            t = base_type(name, types)
        except LayoutError:
            return None
    return Type("array", elem=t, count=0) if arr else t


# ------------------------------------------------------------------------------------- config

class Aggregate:
    def __init__(self, base, tname, header, root):
        self.base, self.tname, self.header = base, tname, header
        m = SYM_RE.match(base)
        if not m:
            raise SystemExit("migrate_globals: base %s must be a D_XXXXXXXX name" % base)
        self.addr = int(m.group(1), 16)
        text = read(os.path.join(root, header))
        self.types = parse_types(strip_comments_keep_offsets(text))
        if tname not in self.types:
            raise SystemExit("migrate_globals: %s does not define %s" % (header, tname))
        self.type = base_type(tname, self.types)
        self.end = self.addr + self.type.size

    def holds(self, addr):
        return self.addr < addr < self.end


def strip_comments_keep_offsets(text):
    """Drop comments except `/* 0xNN */` offset markers (kept for the layout check)."""
    return re.sub(r"/\*(?!\s*0x[0-9A-Fa-f]+\s*\*/).*?\*/", " ", text, flags=re.S)


def read(path):
    with open(path, encoding="utf-8", errors="surrogateescape") as f:
        return f.read()


def write(path, text):
    with open(path, "w", encoding="utf-8", errors="surrogateescape") as f:
        f.write(text)


def load_config(root, path=None):
    aggs, keep = [], {}
    p = path or os.path.join(root, CONFIG)
    if not os.path.exists(p):
        return aggs, keep
    for n, line in enumerate(read(p).splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        parts = line.split()
        if parts[0] == "aggregate" and len(parts) == 4:
            aggs.append(Aggregate(parts[1], parts[2], parts[3], root))
        elif parts[0] == "keep" and len(parts) >= 4:
            keep.setdefault(os.path.normpath(parts[1]), set()).add(parts[2])
        else:
            raise SystemExit("%s:%d: cannot parse '%s'" % (p, n, line))
    return aggs, keep


# ------------------------------------------------------------------------------------- views

def declarations(text):
    """D_ name -> old view spelling for every top-level extern declaration in text."""
    out = {}
    for m in DECL_RE.finditer(text):
        tname, stars, sym, dims = m.group(1), m.group(2), m.group(3), m.group(4)
        out[sym] = tname + (" *" if stars else "") + ("[]" if dims else "")
    return out


def includes(text):
    return re.findall(r'^[ \t]*#[ \t]*include[ \t]+"([^"]+)"', text, flags=re.M)


class Views:
    """The declaration each file sees for a symbol (own > included header > main header)."""

    def __init__(self, root, inc="include"):
        self.root, self.inc = root, inc
        self.headers = {}
        for dirpath, _d, files in os.walk(os.path.join(root, inc)):
            for f in files:
                if f.endswith(".h"):
                    p = os.path.join(dirpath, f)
                    self.headers[os.path.relpath(p, os.path.join(root, inc))] = declarations(read(p))
        self.main = dict(self.headers.get("main_api.h", {}))

    def header_view(self, h, sym, seen=None):
        seen = seen or set()
        if h in seen or h not in self.headers:
            return None
        seen.add(h)
        if sym in self.headers[h] and h != "main_api.h":
            return self.headers[h][sym]
        p = os.path.join(self.root, self.inc, h)
        for i in includes(read(p)):
            v = self.header_view(i, sym, seen)
            if v:
                return v
        return None

    def view(self, text, sym):
        own = declarations(text)
        if sym in own:
            return own[sym]
        for i in includes(text):
            v = self.header_view(i, sym)
            if v:
                return v
        return self.main.get(sym)


# ------------------------------------------------------------------------------------- mapping

class Mapping:
    """How one use of an absorbed symbol is written: path (full C expression), and for an array that
    starts inside a longer array, the array expression and the start index."""

    def __init__(self, expr, typ, inner=None):
        self.expr, self.typ, self.inner = expr, typ, inner   # inner: (array expr, k)


def find_mapping(agg, sym, view_spell, indexed, stores_only=False):
    """Mapping for `sym` (old view `view_spell` or None), or (None, reason). With stores_only (every
    use in the file is a plain store), a scalar of the same width but other signedness also fits:
    sb/sh/sw do not depend on it."""
    addr = int(SYM_RE.match(sym).group(1), 16)
    rel = addr - agg.addr
    subs = subobjects(agg.type, rel)
    want = parse_view(view_spell, agg.types) if view_spell else None
    pre = agg.base
    if want is not None and want.kind == "array":
        for path, t in subs:
            if t.kind == "array" and t.elem.same(want.elem):
                return Mapping(pre + path, t), None
        for path, t in subs:
            m = re.match(r"^(.*)\[(\d+)\]$", path)
            if m and t.same(want.elem):
                return Mapping(pre + path, t, (pre + m.group(1), int(m.group(2)))), None
        return None, "no %s array at offset 0x%X" % (want.elem.spell(), rel)
    if want is not None:
        for path, t in reversed(subs):
            if t.same(want):
                return Mapping(pre + path, t), None
        if stores_only and want.kind == "scalar":
            for path, t in reversed(subs):
                if t.kind == "scalar" and t.size == want.size:
                    return Mapping(pre + path, t), None
        return None, "no %s field at offset 0x%X (there: %s)" % (
            want.spell(), rel, ", ".join("%s %s" % (t.spell(), p) for p, t in subs) or "nothing")
    # no declaration (new code): only an unambiguous offset, where one scalar and nothing else starts
    starts = [(p, t) for p, t in subs]
    if indexed:
        arrays = [(p, t) for p, t in starts if t.kind == "array"]
        if len(arrays) == 1:
            return Mapping(pre + arrays[0][0], arrays[0][1]), None
    elif len(starts) == 1 and starts[0][1].kind in ("scalar", "ptr"):
        return Mapping(pre + starts[0][0], starts[0][1]), None
    return None, "no declaration and offset 0x%X is ambiguous (%s): declare the old view in %s or write " \
        "the field" % (rel, ", ".join("%s %s" % (t.spell(), p) for p, t in starts) or "nothing starts there",
                       agg.header)


def stores_only(text, sym, skip=()):
    """True when every use of sym in text is the target of a plain `=` store."""
    for s, e, tok in code_tokens(text):
        if tok != sym or any(a <= s < b for a, b in skip):
            continue
        if not re.match(r"\s*=(?!=)", text[e:]):
            return False
    return True


def match_bracket(text, i):
    """Index after the `]` matching the `[` at text[i]."""
    depth = 0
    for j in range(i, len(text)):
        if text[j] == "[":
            depth += 1
        elif text[j] == "]":
            depth -= 1
            if depth == 0:
                return j + 1
    return -1


def code_tokens(text):
    """(start, end, identifier) for identifiers outside comments, strings and preprocessor lines."""
    pp = [(m.start(), m.end()) for m in re.finditer(r"^[ \t]*#[^\n]*(?:\\\n[^\n]*)*", text, flags=re.M)]
    k = 0
    for m in TOKEN_RE.finditer(text):
        tok = m.group(0)
        if tok[0] in "/\"'":
            continue
        while k < len(pp) and pp[k][1] <= m.start():
            k += 1
        if k < len(pp) and pp[k][0] <= m.start() < pp[k][1]:
            continue
        yield m.start(), m.end(), tok


def agg_for(aggs, sym):
    m = SYM_RE.match(sym)
    if not m:
        return None
    a = int(m.group(1), 16)
    for g in aggs:
        if g.holds(a):
            return g
    return None


def base_of(aggs, sym):
    for g in aggs:
        if g.base == sym:
            return g
    return None


def rewrite_text(text, aggs, keep, views, decl_view=None):
    """(new text, problems) for one .c file. decl_view: callable(sym) -> old view or None."""
    out, problems, last = [], [], 0
    decl_spans = [(m.start(), m.end()) for m in DECL_RE.finditer(text)]   # removed later, not rewritten
    for s, e, tok in code_tokens(text):
        if any(a <= s < b for a, b in decl_spans):
            continue
        g = agg_for(aggs, tok)
        b = base_of(aggs, tok) if g is None else None
        if g is None and b is None:
            continue
        if tok in keep:
            continue
        rest = text[e:]
        nxt = rest.lstrip()
        line = text.count("\n", 0, s) + 1
        if b is not None:
            if nxt.startswith(".") or nxt.startswith("->"):
                continue
            before = text[:s].rstrip()
            if before.endswith("&") and not before.endswith("&&") and not nxt.startswith("["):
                continue
            view = decl_view(tok) if decl_view else None
            want = parse_view(view, b.types) if view else None
            if want is not None and want.kind == "scalar" and not nxt.startswith("["):
                for path, t in reversed(subobjects(b.type, 0)):
                    if t.same(want):
                        out.append(text[last:s] + b.base + path)
                        last = e
                        break
                else:
                    problems.append((line, tok, "old %s view of the base: write the field access" % view))
                continue
            problems.append((line, tok, "old view of %s %s: write the field access (wiki/game-state.md)"
                             % (b.tname, b.base)))
            continue
        indexed = nxt.startswith("[")
        view = decl_view(tok) if decl_view else None
        mp, why = find_mapping(g, tok, view, indexed, stores_only(text, tok, decl_spans))
        if mp is None:
            problems.append((line, tok, why))
            continue
        if mp.inner is None:
            out.append(text[last:s] + mp.expr)
            last = e
            continue
        arr, k = mp.inner
        if indexed:
            i = e + (len(rest) - len(nxt))
            j = match_bracket(text, i)
            idx = text[i + 1:j - 1].strip()
            if re.match(r"^(0x[0-9A-Fa-f]+|\d+)$", idx):
                new = "%s[0x%X]" % (arr, k + int(idx, 0)) if idx.startswith("0x") else "%s[%d]" % (arr, k + int(idx))
            else:
                new = "%s[%s + %d]" % (arr, idx, k)
            out.append(text[last:s] + new)
            last = j
            continue
        before = text[:s].rstrip()
        if before.endswith("&") and not before.endswith("&&"):
            out.append(text[last:s] + mp.expr)
        else:
            out.append(text[last:s] + "&" + mp.expr)
        last = e
    out.append(text[last:])
    return "".join(out), problems


def suggestion(aggs, sym, view):
    g = agg_for(aggs, sym)
    if g is None:
        return None
    mp, why = find_mapping(g, sym, view, False)
    if mp:
        return "use %s (%s) (run python3 tools/migrate_globals.py --apply)" % (mp.expr, mp.typ.spell())
    return "no field fits (%s): add the field to %s in %s or write the access by hand" % (why, g.tname, g.header)


# ------------------------------------------------------------------------------------- declarations

def absorbed_names(text, aggs, keep_all):
    return [s for s in declarations(text) if agg_for(aggs, s) and s not in keep_all]


def drop_declarations(text, names, base_names=()):
    """Delete extern lines, MAIN_API_OVERRIDE_ guards and defines of names (and of overridden bases)."""
    for sym in list(names) + list(base_names):
        text = re.sub(OVR_GUARD_RE % sym, lambda m: m.group(1), text, flags=re.M)
        text = re.sub(OVR_DEF_RE % sym, "", text, flags=re.M)
    for sym in names:
        text = re.sub(r"^[ \t]*extern[^;\n]*\b%s\b[^;\n]*;[^\n]*\n?" % sym, "", text, flags=re.M)
    return text


def src_files(root, src="src"):
    out = []
    for dirpath, _d, files in os.walk(os.path.join(root, src)):
        for f in files:
            if f.endswith(".c"):
                out.append(os.path.join(dirpath, f))
    return sorted(out)


def plan(root, aggs, keep):
    """{path: new text} for --apply, and the problems [(path, line, sym, msg)]."""
    views = Views(root)
    keep_all = set().union(*keep.values()) if keep else set()
    changes, problems = {}, []
    rewritten = {}
    for path in src_files(root):
        rel = os.path.normpath(os.path.relpath(path, root))
        text = read(path)
        own_keep = keep.get(rel, set())
        rewritten[path] = rewrite_text(text, aggs, own_keep, views, lambda s, t=text: views.view(t, s))
    # a symbol with a use left for a hand rewrite keeps its declarations until that use is gone
    pending = {s for _new, probs in rewritten.values() for _ln, s, _m in probs}
    keep_all = keep_all | pending
    for path in src_files(root):
        rel = os.path.normpath(os.path.relpath(path, root))
        text = read(path)
        own_keep = keep.get(rel, set()) | pending
        new, probs = rewritten[path]
        # the file's own declarations and override defines of absorbed names (and of base views)
        names = [s for s in absorbed_names(new, aggs, own_keep)]
        bases = [g.base for g in aggs if re.search(r"MAIN_API_OVERRIDE_%s\b" % g.base, new)]
        if bases:
            new = re.sub(r"^[ \t]*extern[^;\n]*\b(%s)\b[^;\n]*;[^\n]*\n?" % "|".join(bases), "", new, flags=re.M)
        new = drop_declarations(new, names, bases)
        if new != text:
            changes[path] = new
        problems += [(rel, ln, s, m) for ln, s, m in probs]
    for h in sorted(views.headers, key=lambda x: (x == "main_api.h", x)):   # main_api.h last
        p = os.path.join(root, "include", h)
        text = read(p)
        names = absorbed_names(text, aggs, keep_all)
        bases = []
        if h != "main_api.h":
            bases = [g.base for g in aggs if re.search(r"MAIN_API_OVERRIDE_%s\b" % g.base, text)]
            if bases:
                text2 = re.sub(r"^[ \t]*extern[^;\n]*\b(%s)\b[^;\n]*;[^\n]*\n?" % "|".join(bases), "", text, flags=re.M)
            else:
                text2 = text
        else:
            text2 = text
        new = drop_declarations(text2, names, bases)
        if h == "main_api.h":   # guards of base overrides that no file defines any more
            for g in aggs:
                if not any(re.search(r"#[ \t]*define[ \t]+MAIN_API_OVERRIDE_%s\b" % g.base, read(q))
                           for q in src_files(root) + [os.path.join(root, "include", x) for x in views.headers
                                                       if x != "main_api.h"]
                           if q not in changes) and not any(
                               re.search(r"#[ \t]*define[ \t]+MAIN_API_OVERRIDE_%s\b" % g.base, t)
                               for t in changes.values()):
                    new = re.sub(OVR_GUARD_RE % g.base, lambda m: m.group(1), new, flags=re.M)
        if new != text:
            changes[p] = new
    return changes, problems


def check(root, aggs, keep):
    """Problems as printable lines (uses of absorbed symbols with the field to use)."""
    views = Views(root)
    lines = []
    for path in src_files(root):
        rel = os.path.normpath(os.path.relpath(path, root))
        text = read(path)
        own_keep = keep.get(rel, set())
        for s, e, tok in code_tokens(text):
            line = text.count("\n", 0, s) + 1
            g = agg_for(aggs, tok)
            if g is not None and tok not in own_keep:
                sug = suggestion(aggs, tok, views.view(text, tok))
                lines.append("%s:%d: %s is a field of %s %s: %s" % (rel, line, tok, g.tname, g.base, sug))
                continue
            b = base_of(aggs, tok)
            if b is not None:
                nxt = text[e:].lstrip()
                before = text[:s].rstrip()
                if nxt.startswith(".") or nxt.startswith("->"):
                    continue
                if before.endswith("&") and not before.endswith("&&") and not nxt.startswith("["):
                    continue
                lines.append("%s:%d: %s is the %s struct: access its fields (%s.unk_XXX), not an old view"
                             % (rel, line, tok, b.tname, tok))
    for h, decl in sorted(views.headers.items()):
        keep_all = set().union(*keep.values()) if keep else set()
        for sym in decl:
            if agg_for(aggs, sym) and sym not in keep_all:
                lines.append("include/%s: declares %s, a field of %s: delete the declaration "
                             "(python3 tools/migrate_globals.py --apply)" % (h, sym, agg_for(aggs, sym).base))
    for p, syms in sorted(keep.items()):
        for sym in sorted(syms):
            if sym not in views.main:
                lines.append("%s: %s: kept symbol %s is not declared in include/main_api.h" % (CONFIG, p, sym))
    return lines


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--root", default=".")
    ap.add_argument("--config")
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--check", action="store_true")
    g.add_argument("--apply", action="store_true")
    g.add_argument("--dry-run", action="store_true")
    g.add_argument("--layout", action="store_true")
    g.add_argument("--at")
    a = ap.parse_args(argv)
    try:
        aggs, keep = load_config(a.root, a.config)
    except LayoutError as e:
        print("migrate_globals: layout: %s" % e)
        return 1
    if a.layout:
        for agg in aggs:
            print("%s %s: 0x%08X..0x%08X, size 0x%X" % (agg.tname, agg.base, agg.addr, agg.end, agg.type.size))
            for off, size, path, t in walk_layout(agg.type):
                print("  0x%04X 0x%08X %5s %-8s %s" % (off, agg.addr + off, hex(size), t.spell(), path[1:]))
        return 0
    if a.at:
        addr = int(a.at[2:] if a.at.startswith("D_") else a.at, 16)
        for agg in aggs:
            if agg.addr <= addr < agg.end:
                for path, t in subobjects(agg.type, addr - agg.addr):
                    print("%s%s  %s" % (agg.base, path, t.spell()))
        return 0
    if a.check:
        lines = check(a.root, aggs, keep)
        for line in lines:
            print(line)
        if lines:
            print("%d use(s) of absorbed globals" % len(lines))
            return 1
        print("globals OK")
        return 0
    changes, problems = plan(a.root, aggs, keep)
    for path in sorted(changes):
        print("%s %s" % ("would rewrite" if a.dry_run else "rewrote", os.path.relpath(path, a.root)))
        if a.apply:
            write(path, changes[path])
    for rel, ln, sym, msg in problems:
        print("%s:%d: %s: %s (left for a hand rewrite)" % (rel, ln, sym, msg))
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
