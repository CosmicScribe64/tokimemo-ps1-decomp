#!/usr/bin/env python3
"""Move the C sources and the splat config of a unit to one C file per original object (T-0500).

usage: split_objects.py [--dry-run] UNIT [UNIT...]      UNIT = an overlay name, `main/<address>`
                                                         (one src/main file) or `main` (all of them)
       split_objects.py [--dry-run] --all               every overlay and every src/main file
(run in Docker from the repo root after a full `ninja`, so asm/ matches the current yaml files;
then `python3 configure.py && ninja`)

The objects come from config/objects/<UNIT>.txt (tools/object_boundaries.py --write). For every
selected unit the script
  1. reads all current C files of the unit (the `c` subsegments of its yaml, whatever layout they
     have) and cuts them into top-level pieces: INCLUDE_ASM lines, C function definitions,
     `#if` groups, declarations, preprocessor lines; comments go with the piece after them;
  2. gives every function piece to the object that holds its address (func_XXXXXXXX, or the
     address of a named function from its splat .s file); declarations between functions stay in
     front of the next function; the declarations at the top of an old file are copied into each
     new file that uses the declared name (definitions with storage only into one file, and the
     script stops if two objects use one);
  3. writes src/ovl/<NAME>/<address>.c (overlays) or src/main/<address>.c (main exe) per object,
     with the INCLUDE_ASM folders rewritten to the object's splat folder, and deletes the old files;
  4. regenerates INCLUDE_RODATA lines for island objects (island=yes in the objects file): every
     early rodata symbol of the chunk that no function of the object mentions, or that a C
     function names as an extern, gets a line in front of the function owning the next symbol
     (tools/rodata_pieces.py hands the other symbols to the INCLUDE_ASM functions);
  5. adds a prototype to the overlay header (main: include/main_only.h) for every function
     defined in C that C code of another object calls and that no header declares;
  6. rewrites the yaml: one `c` subsegment per object, a `.rodata` island per island object,
     `rodata` asm pieces for the gaps, and (overlays) a `data` subsegment `<NAME>_data` from the
     end of the rodata to the end of the file.
Running it again on its own output changes nothing; on a changed tree it re-cuts from the
current files, so it can be repeated after new C has been written. --dry-run prints the plan.
Tests: tools/test_objects.py.
"""
import argparse
import os
import re
import sys

import check_headers
import object_boundaries as ob
import srcscan

INCLUDE_ASM_RE = re.compile(r'INCLUDE_ASM\(\s*"([^"]*)"\s*,\s*(\w+)\s*\)')
INCLUDE_RODATA_RE = re.compile(r'INCLUDE_RODATA\(\s*"([^"]*)"\s*,\s*(\w+)\s*\)')
FUNC_ADDR_RE = re.compile(r'^func_([0-9A-Fa-f]{8})$')
IDENT_RE = re.compile(r'[A-Za-z_]\w*')
KEYWORDS = set("""auto break case char const continue default do double else enum extern float for
goto if int long register return short signed sizeof static struct switch typedef union unsigned
void volatile while s8 u8 s16 u16 s32 u32 s64 u64""".split())
EXE_TEXT_OFF = 0x80041000 - 0x800       # main exe: vram - file offset


class SplitError(Exception):
    pass


def read_text(path):
    with open(path) as f:
        return f.read()


# ------------------------------------------------------------------ C pieces

class Piece:
    """One top-level piece of a C file. kind: include, pp, cond, asm, rodata, func, decl, tail."""

    def __init__(self, kind, text, code):
        self.kind = kind
        self.text = text          # full text, with the comments and blank lines in front
        self.code = code          # the piece without the leading comments
        self.func = None          # function name (asm, func, cond)
        self.names = set()        # declared names (decl)
        self.definition = False   # decl that defines storage

    def __repr__(self):
        return "Piece(%s, %s)" % (self.kind, self.func or sorted(self.names))


def _skip_string(s, i):
    q = s[i]
    i += 1
    while i < len(s) and s[i] != q:
        i += 2 if s[i] == "\\" else 1
    return i + 1


def _skip_comment(s, i):
    if s.startswith("/*", i):
        j = s.find("*/", i + 2)
        if j < 0:
            raise SplitError("unterminated comment")
        return j + 2
    j = s.find("\n", i)
    return len(s) if j < 0 else j


def _line_end(s, i):
    """Index after the rest of the line at i (spaces/comments only) and its newline."""
    while i < len(s) and s[i] in " \t":
        i += 1
    if s.startswith("/*", i):
        j = s.find("*/", i)
        if j >= 0 and "\n" not in s[i:j]:
            i = j + 2
            while i < len(s) and s[i] in " \t":
                i += 1
    if i < len(s) and s[i] == "\n":
        i += 1
    return i


def _directive(line):
    m = re.match(r'\s*#\s*(\w+)', line)
    return m.group(1) if m else ""


def parse_c(text):
    """[Piece] of a C file; the concatenated texts equal the input."""
    pieces = []
    i = start = 0
    n = len(text)
    while True:
        # leading whitespace and comments belong to the next piece
        while i < n:
            if text[i] in " \t\r\n":
                i += 1
            elif text.startswith("/*", i) or text.startswith("//", i):
                i = _skip_comment(text, i)
            else:
                break
        if i >= n:
            if start < n:
                pieces.append(Piece("tail", text[start:], ""))
            return pieces
        code_start = i
        if text[i] == "#":
            word = _directive(text[i:i + 40])
            depth = 0
            while True:
                j = text.find("\n", i)
                j = n if j < 0 else j + 1
                while text[i:j].rstrip("\n").endswith("\\") and j < n:
                    k = text.find("\n", j)
                    j = n if k < 0 else k + 1
                d = _directive(text[i:j])
                if d in ("if", "ifdef", "ifndef"):
                    depth += 1
                elif d == "endif":
                    depth -= 1
                i = j
                if depth <= 0 or i >= n:
                    break
                # skip to the next line start inside the group
            kind = "cond" if word in ("if", "ifdef", "ifndef") else ("include" if word == "include" else "pp")
            p = Piece(kind, text[start:i], text[code_start:i])
            if kind == "cond":
                m = INCLUDE_ASM_RE.search(p.code)
                if m:
                    p.func = m.group(2)
                else:
                    defs = function_defs(p.code)
                    p.func = defs[0] if defs else None
            pieces.append(p)
            start = i
            continue
        # C code up to `;` at depth 0, or the closing brace of a function body
        depth_p = depth_b = 0
        func_body = None
        while i < n:
            c = text[i]
            if c in "\"'":
                i = _skip_string(text, i)
                continue
            if text.startswith("/*", i) or text.startswith("//", i):
                i = _skip_comment(text, i)
                continue
            if c == "(":
                depth_p += 1
            elif c == ")":
                depth_p -= 1
            elif c == "{":
                if depth_b == 0 and func_body is None:
                    head = text[code_start:i]
                    func_body = "(" in head and "=" not in head and not re.match(
                        r'\s*(typedef|struct|union|enum)\b', head)
                depth_b += 1
            elif c == "}":
                depth_b -= 1
                if depth_b == 0 and func_body:
                    i = _line_end(text, i + 1)
                    break
            elif c == ";" and depth_p == 0 and depth_b == 0:
                i = _line_end(text, i + 1)
                break
            i += 1
        else:
            raise SplitError("unterminated top-level statement at offset %d" % code_start)
        code = text[code_start:i]
        if func_body:
            p = Piece("func", text[start:i], code)
            p.func = function_name(code)
        elif INCLUDE_ASM_RE.match(code.strip()):
            p = Piece("asm", text[start:i], code)
            p.func = INCLUDE_ASM_RE.match(code.strip()).group(2)
        elif INCLUDE_RODATA_RE.match(code.strip()):
            p = Piece("rodata", text[start:i], code)
            p.func = INCLUDE_RODATA_RE.match(code.strip()).group(2)
        else:
            p = Piece("decl", text[start:i], code)
            p.names, p.definition = declared_names(code)
        pieces.append(p)
        start = i


def strip_c(code):
    code = re.sub(r'/\*.*?\*/', ' ', code, flags=re.S)
    return re.sub(r'"(?:\\.|[^"\\])*"', '""', code)


def function_name(code):
    head = strip_c(code).split("{", 1)[0]
    m = re.search(r'([A-Za-z_]\w*)\s*\([^()]*(?:\([^()]*\)[^()]*)*\)\s*$', head.strip())
    if not m:
        raise SplitError("cannot find the function name in: %s" % head.strip()[:80])
    return m.group(1)


def function_defs(code):
    return [m.group(1) for m in re.finditer(r'^[A-Za-z_][\w \t*]*[ \t*](\w+)\([^;{}]*\)\s*\{', code, re.M)]


def declared_names(code):
    """(set of declared names, defines storage) of a declaration."""
    s = strip_c(code)
    flat, depth = "", 0
    for ch in s:              # drop brace bodies (struct members, initialisers)
        if ch == "{":
            depth += 1
        elif ch == "}":
            depth -= 1
        elif depth == 0:
            flat += ch
    names, pdepth = set(), 0
    toks = re.findall(r'[A-Za-z_]\w*|\S', flat)
    for k, t in enumerate(toks):
        if t == "(":
            pdepth += 1
        elif t == ")":
            pdepth -= 1
        nxt = toks[k + 1] if k + 1 < len(toks) else ""
        prv = toks[k - 1] if k else ""
        if IDENT_RE.fullmatch(t) and t not in KEYWORDS:
            if pdepth == 0 and nxt in ("[", "(", ";", ",", "=", ")"):
                names.add(t)
            elif pdepth == 1 and prv == "*" and nxt == ")":     # (*fp)(...)
                names.add(t)
    words = set(re.findall(r'\b\w+\b', flat))
    is_proto = re.search(r'\w\s*\(', flat) is not None and "=" not in flat
    definition = not ({"extern", "typedef"} & words) and not is_proto
    return names, definition


# ------------------------------------------------------------------ addresses

def symbol_addresses(root, unit):
    """{name: vram} from the splat symbol files a unit's config reads."""
    files = ["config/symbol_addrs.txt", "config/symbol_addrs_sdk.txt", "config/symbol_addrs_obin.txt",
             "config/symbol_addrs_main.txt"]
    if unit != "main":
        files.append("config/overlays/%s_symbols.txt" % unit)
    out = {}
    for f in files:
        p = os.path.join(root, f)
        if not os.path.exists(p):
            continue
        for line in read_text(p).splitlines():
            m = re.match(r'\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;', line)
            if m:
                out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def address_of(name, funcs_by_name, syms):
    m = FUNC_ADDR_RE.match(name or "")
    if m:
        return int(m.group(1), 16)
    if name in funcs_by_name:
        return funcs_by_name[name].addr
    return syms.get(name)


# ------------------------------------------------------------------ planning

class Target:
    """One new C file."""

    def __init__(self, unit, obj):
        self.unit = unit
        self.obj = obj
        self.addr = obj.text_start
        if unit == "main":
            self.name = "main/%08X" % self.addr
            self.src = "src/main/%08X.c" % self.addr
            self.asm_dir = "asm/nonmatchings/" + self.name
            self.rodata_dir = "asm/data/%s.rodata" % self.name
        else:
            self.name = "%s/%08X" % (unit, self.addr)
            self.src = "src/ovl/%s.c" % self.name
            self.asm_dir = "asm/ovl/%s/nonmatchings/%s" % (unit, self.name)
            self.rodata_dir = "asm/ovl/%s/data/%s.rodata" % (unit, self.name)
        self.preamble = []        # include/pp/decl pieces in front
        self.body = []            # [(piece, owner old file)] in order
        self.rodata = {}          # function name or None (end): [symbol]
        self.island = obj.island == "yes"

    def render(self):
        def norm(t):
            return t.strip("\n") + "\n"
        out = []
        for p in self.preamble:
            out.append(norm(p.text))
        text = "".join(out)
        body = []
        for p in self.body:
            if p.kind == "rodata":
                continue
            t = INCLUDE_ASM_RE.sub(lambda m: 'INCLUDE_ASM("%s", %s)' % (self.asm_dir, m.group(2)), p.text)
            lines = self.rodata.get(p.func) if p.kind in ("asm", "func", "cond") else None
            if lines:
                body.append("\n".join('INCLUDE_RODATA("%s", %s);\n' % (self.rodata_dir, s) for s in lines))
            body.append(norm(t))
        if self.rodata.get(None):
            body.append("\n".join('INCLUDE_RODATA("%s", %s);\n' % (self.rodata_dir, s)
                                  for s in self.rodata[None]))
        return text + "\n" + "\n".join(b.strip("\n") + "\n" for b in body)


def uses(text, name):
    return re.search(r'\b%s\b' % re.escape(name), strip_c(text)) is not None


def plan_unit(root, unit, objs, old_files, funcs, items):
    """[Target] for the objects `objs` from the C files `old_files` (CFile list, link order)."""
    syms = symbol_addresses(root, unit)
    by_name = {f.name: f for f in funcs}
    targets = [Target(unit, o) for o in objs]
    starts = [t.addr for t in targets]

    def target_of(addr):
        k = max(i for i, s in enumerate(starts) if s <= addr) if addr >= starts[0] else None
        if k is None or addr >= objs[k].text_end:
            raise SplitError("%s: address %08X is outside the objects" % (unit, addr))
        return targets[k]

    shared_decls = []                 # (piece, target that holds it) for inter-function decls
    for cf in old_files:
        pieces = parse_c(read_text(cf.src))
        first = next((k for k, p in enumerate(pieces) if p.kind in ("asm", "func", "cond")), None)
        if first is None:
            raise SplitError("%s: no functions" % cf.src)
        # INCLUDE_RODATA lines are regenerated (plan_rodata), wherever they are now
        pieces = [p for p in pieces if p.kind != "rodata"]
        first = next(k for k, p in enumerate(pieces) if p.kind in ("asm", "func", "cond"))
        pre, rest = pieces[:first], pieces[first:]
        pending, groups = [], []      # groups: (function piece, [pieces in front])
        for p in rest:
            if p.kind in ("asm", "func", "cond") and p.func:
                groups.append((p, pending))
                pending = []
            else:
                pending.append(p)
        if pending:                   # tail after the last function
            groups[-1][1].append(("after", pending))
        file_targets = []
        for p, front in groups:
            addr = address_of(p.func, by_name, syms)
            if addr is None:
                raise SplitError("%s: no address for %s (static helper without splat symbol?)"
                                 % (cf.src, p.func))
            t = target_of(addr)
            after = [x for x in front if isinstance(x, tuple)]
            front = [x for x in front if not isinstance(x, tuple)]
            for x in front:
                if x.kind == "decl":
                    shared_decls.append((x, t))
            t.body += front + [p]
            for _tag, ps in after:
                t.body += ps
            if t not in file_targets:
                file_targets.append(t)
        for t in file_targets:
            t.preamble_src = getattr(t, "preamble_src", []) + [pre]
    # preambles: includes and pp lines always, declarations where used
    for t in targets:
        if not t.body:
            raise SplitError("%s: object %08X has no functions in the sources" % (unit, t.addr))
        body_text = "".join(p.text for p in t.body)
        seen = set()
        for pre in getattr(t, "preamble_src", []):
            for p in pre:
                key = p.code.strip()
                if key in seen:
                    continue
                if p.kind == "decl" and p.names and not any(uses(body_text, nm) for nm in p.names):
                    continue
                if p.kind == "decl" and p.definition:
                    users = [u for u in targets if any(uses("".join(x.text for x in u.body), nm)
                                                      for nm in p.names)]
                    if len(users) > 1:
                        raise SplitError("%s: %s defined at file scope is used by objects %s"
                                         % (unit, "/".join(sorted(p.names)),
                                            ", ".join("%08X" % u.addr for u in users)))
                seen.add(key)
                t.preamble.append(p)
        for x, holder in shared_decls:
            if holder is t or x.code.strip() in seen or not x.names:
                continue
            if any(uses(body_text, nm) for nm in x.names):
                if x.definition:
                    raise SplitError("%s: %s is defined in object %08X but used in %08X"
                                     % (unit, "/".join(sorted(x.names)), holder.addr, t.addr))
                seen.add(x.code.strip())
                t.preamble.append(x)
    plan_rodata(targets, funcs, items)
    return targets


def plan_rodata(targets, funcs, items):
    """Fill Target.rodata: the INCLUDE_RODATA symbols of island objects, keyed by the function
    they go in front of (None: end of file)."""
    for t in targets:
        if not t.island:
            continue
        o = t.obj
        fns = [f for f in funcs if o.text_start <= f.addr < o.text_end]
        c_funcs = {p.func for p in t.body if p.kind == "func"}
        c_text = "".join(p.text for p in t.body if p.kind in ("func", "cond"))
        early = [it for it in items if o.ro_start <= it.addr < o.ro_end and it.kind != "jtbl"]
        owners = []
        for it in early:
            us = [f for f in fns if it.name in f.refs]
            owners.append(min(us, key=lambda f: f.addr).name if us else None)
        need = []
        for k, it in enumerate(early):
            if owners[k] is None:
                anchor = next((x for x in owners[k + 1:] if x is not None), None)
                need.append((anchor, it.name))
            elif owners[k] in c_funcs and uses(c_text, it.name):
                need.append((owners[k], it.name))
        for anchor, sym in need:
            t.rodata.setdefault(anchor, []).append(sym)


def missing_prototypes(root, unit, targets):
    """[(prototype text, header)] for C functions called from another object's C and not
    declared in any header the caller includes."""
    header = "include/main_only.h" if unit == "main" else "include/ovl/%s.h" % unit
    declared = set()
    for t in targets:
        for p in t.preamble:
            if p.kind == "include":
                m = re.search(r'#\s*include\s+"([^"]+)"', p.code)
                if m:
                    declared |= header_names(root, m.group(1))
            declared |= p.names
    defs = {}
    for t in targets:
        for p in t.body:
            if p.kind == "func":
                defs[p.func] = (t, strip_c(p.code).split("{", 1)[0].strip())
    out = []
    for name, (holder, sig) in sorted(defs.items()):
        if name in declared or sig.startswith("static"):
            continue
        for t in targets:
            if t is holder:
                continue
            if any(uses(p.code, name) for p in t.body if p.kind == "func"):
                out.append((re.sub(r'\s+', ' ', sig) + ";", header))
                break
    return out


def header_names(root, inc, seen=None):
    seen = set() if seen is None else seen
    path = os.path.join(root, "include", inc)
    if inc in seen or not os.path.exists(path):
        return set()
    seen.add(inc)
    names = {n for n, _t in check_headers.declarations(path)}
    for sub in check_headers.includes(path):
        names |= header_names(root, sub, seen)
    return names


# ------------------------------------------------------------------ yaml

def labels_path(unit):
    return "config/labels/%s.txt" % unit


LABELS_HEADER = """// Rodata labels for the INCLUDE_RODATA lines of {what} (T-0500). Written by
// tools/split_objects.py, do not edit. splat names a symbol only when something in the same
// subsegment points at it; strings reached through .data pointer tables lost their labels when
// the rodata was cut into per-object islands, so they are listed here.
"""


def labels_text(unit, syms):
    what = "the main exe" if unit == "main" else "overlay %s" % unit
    return LABELS_HEADER.format(what=what) + "".join(
        "%s = 0x%s;\n" % (s, s[2:]) for s in sorted(syms, key=lambda x: int(x[2:], 16)))


def add_symbol_file(ytext, path):
    """The yaml text with `path` in its symbol_addrs_path list (unchanged if it is there)."""
    if "    - %s\n" % path in ytext:
        return ytext
    lines = ytext.splitlines(True)
    k = next((i for i, l in enumerate(lines) if l.strip() == "symbol_addrs_path:"), None)
    if k is None:
        o = next(i for i, l in enumerate(lines) if l.strip() == "options:")
        return "".join(lines[:o + 1] + ["  symbol_addrs_path:\n", "    - %s\n" % path] + lines[o + 1:])
    e = k + 1
    while e < len(lines) and (lines[e].startswith("    - ") or lines[e].startswith("    #")):
        e += 1
    return "".join(lines[:e] + ["    - %s\n" % path] + lines[e:])


def yaml_path(unit):
    return "config/SLPM_86.053.yaml" if unit == "main" else "config/overlays/%s.yaml" % unit


def overlay_subsegments(unit, base, objs, ro_lo, ro_end):
    """Subsegment lines of an overlay's code segment."""
    lines = ["      - [0x%X, c, %s/%08X]\n" % (o.text_start - base, unit, o.text_start) for o in objs]
    lines += rodata_lines(unit, base, [o for o in objs if o.island == "yes"], ro_lo, ro_end,
                          "%s_rodata_" % unit, lambda o: "%s/%08X" % (unit, o.text_start), list_form=True)
    lines.append("      - [0x%X, data, %s_data]\n" % (ro_end - base, unit))
    return lines


def rodata_lines(unit, delta, islands, lo, hi, prefix, iname, list_form, extra=()):
    """`.rodata` island lines and `rodata` asm pieces covering [lo, hi). islands: Obj list;
    extra: [(start, end, name)] islands kept as they are."""
    marks = sorted([(o.ro_start, o.ro_end, iname(o)) for o in islands] + list(extra))
    for (s1, e1, n1), (s2, _e2, n2) in zip(marks, marks[1:]):
        if e1 > s2:
            raise SplitError("%s: islands %s and %s overlap" % (unit, n1, n2))
    out, pos = [], lo

    def line(off, typ, name):
        if list_form:
            return "      - [0x%X, %s, %s]\n" % (off, typ, name)
        return "      - [0x%X, %s, %s]\n" % (off, typ, name)
    for s, e, n in marks:
        if s > pos:
            out.append(line(pos - delta, "rodata", "%s%08X" % (prefix, pos)))
        out.append(line(s - delta, ".rodata", n))
        pos = e
    if pos < hi:
        out.append(line(pos - delta, "rodata", "%s%08X" % (prefix, pos)))
    return out


SUB_LINE_RE = re.compile(r'^\s*- (?:\[(0x[0-9A-Fa-f]+),\s*([.\w]+)(?:,\s*([\w/]+))?\]|'
                         r'\{\s*start:\s*(0x[0-9A-Fa-f]+),\s*type:\s*([.\w]+),\s*name:\s*([\w/]+)\s*\})')


def parse_sub_line(line):
    m = SUB_LINE_RE.match(line)
    if not m:
        return None
    if m.group(1):
        return int(m.group(1), 16), m.group(2), m.group(3)
    return int(m.group(4), 16), m.group(5), m.group(6)


def rewrite_overlay_yaml(text, unit, base, objs, ro_lo, ro_end):
    lines = text.splitlines(True)
    k = next(i for i, l in enumerate(lines) if l.strip() == "subsegments:")
    e = next(i for i in range(k + 1, len(lines)) if not lines[i].startswith("      "))
    return "".join(lines[:k + 1] + overlay_subsegments(unit, base, objs, ro_lo, ro_end) + lines[e:])


def rewrite_main_yaml(text, split_files, rodata_lo):
    """Replace the `c` lines of the files in split_files ({old name: [Obj]}) by one line per
    object and regenerate the rodata lines from rodata_lo to the next subsegment after them
    (the islands of files not in split_files are kept as they are)."""
    lines = text.splitlines(True)
    parsed = [parse_sub_line(l) for l in lines]
    ro = [i for i, q in enumerate(parsed) if q and q[1] in ("rodata", ".rodata")
          and q[0] + EXE_TEXT_OFF >= rodata_lo]
    if not ro or parsed[ro[0]][0] + EXE_TEXT_OFF != rodata_lo or ro != list(range(ro[0], ro[-1] + 1)):
        raise SplitError("main: the yaml has no contiguous rodata lines from %08X" % rodata_lo)
    hi = next(q for q in parsed[ro[-1] + 1:] if q)[0] + EXE_TEXT_OFF
    keep = []
    for i in ro:
        q = parsed[i]
        if q[1] == ".rodata" and q[2] not in split_files:
            keep.append((q[0] + EXE_TEXT_OFF, parsed[i + 1][0] + EXE_TEXT_OFF, q[2]))
    islands = [o for objs in split_files.values() for o in objs if o.island == "yes"]
    new_ro = rodata_lines("main", EXE_TEXT_OFF, islands, rodata_lo, hi, "rodata_",
                          lambda o: "main/%08X" % o.text_start, list_form=True, extra=keep)
    out = []
    for i, l in enumerate(lines):
        q = parsed[i]
        if q and q[1] == "c" and q[2] in split_files:
            out += ["      - { start: 0x%X, type: c, name: main/%08X }\n" % (o.text_start - EXE_TEXT_OFF, o.text_start)
                    for o in split_files[q[2]]]
        elif i == ro[0]:
            out += new_ro
        elif i not in ro:
            out.append(l)
    return "".join(out)


# ------------------------------------------------------------------ driver

def unit_of(arg):
    return "main" if arg == "main" or arg.startswith("main/") else arg


def plan(root, args, out=sys.stdout):
    """[(path, new text or None to delete)], [(prototype, header)] for the selected units."""
    by_unit = {}
    for a in args:
        by_unit.setdefault(unit_of(a), []).append(a)
    units = {u.name: u for u in ob.units(root)}
    writes, protos = [], []
    for unit, sel in by_unit.items():
        if unit not in units:
            raise SplitError("unknown unit %s" % unit)
        objs, ro_end, _orph = ob.read_objects(ob.objects_path(unit, root))
        all_c = srcscan.unit_c_files(unit, root)
        cfiles = all_c
        if unit == "main" and "main" not in sel:
            cfiles = [c for c in all_c if c.name in sel]
            if len(cfiles) != len(set(sel)):
                raise SplitError("main: no such c subsegment among %s" % ", ".join(sel))
        text_end = units[unit].text[1]
        ends = {c.name: (all_c[k + 1].start if k + 1 < len(all_c) else text_end) for k, c in enumerate(all_c)}
        groups = {}
        for c in cfiles:
            mine = [o for o in objs if c.start <= o.text_start < ends[c.name]]
            if not mine or mine[0].text_start != c.start or mine[-1].text_end != ends[c.name]:
                raise SplitError("%s: %s (%08X-%08X) is not a union of objects; re-run "
                                 "object_boundaries.py --write or fix the file boundaries"
                                 % (unit, c.name, c.start, ends[c.name]))
            groups[c.name] = mine
        funcs, items = ob.load(units[unit], root)
        targets = plan_unit(root, unit, [o for c in cfiles for o in groups[c.name]], cfiles, funcs, items)
        news = {t.src: t.render() for t in targets}
        olds = [os.path.relpath(str(c.src), root) for c in cfiles]
        ytext = read_text(os.path.join(root, yaml_path(unit)))
        if unit == "main":
            ynew = rewrite_main_yaml(ytext, groups, ob.MAIN_RODATA[0])
        else:
            u = units[unit]
            ynew = rewrite_overlay_yaml(ytext, unit, u.text[0], objs, u.rodata_lo, ro_end)
        labels = set()
        kept = [c for c in all_c if c not in cfiles]
        for t in targets:
            for v in t.rodata.values():
                labels.update(v)
        for c in kept:
            labels.update(n for _f, n in INCLUDE_RODATA_RE.findall(read_text(str(c.src))))
        labels = {x for x in labels if re.match(r'^D_[0-9A-F]{8}$', x)}
        if labels:
            news[labels_path(unit)] = labels_text(unit, labels)
            ynew = add_symbol_file(ynew, labels_path(unit))
        changed = [p for p, t in news.items() if not os.path.exists(os.path.join(root, p))
                   or read_text(os.path.join(root, p)) != t]
        gone = [o for o in olds if o not in news]
        p_unit = missing_prototypes(root, unit, targets)
        n_rod = sum(len(v) for t in targets for v in t.rodata.values())
        out.write("%s: %d files -> %d objects (%d islands, %d INCLUDE_RODATA), %d to write, %d to remove, "
                  "yaml %s, %d prototypes\n" % (
                      unit if unit != "main" else ",".join(sel), len(cfiles), len(targets),
                      sum(t.island for t in targets), n_rod, len(changed), len(gone),
                      "changed" if ynew != ytext else "unchanged", len(p_unit)))
        writes += [(p, news[p]) for p in changed] + [(o, None) for o in gone]
        if ynew != ytext:
            writes.append((yaml_path(unit), ynew))
        protos += p_unit
    return writes, protos


def apply(root, writes, protos):
    for p, text in writes:
        full = os.path.join(root, p)
        if text is None:
            os.remove(full)
            continue
        os.makedirs(os.path.dirname(full), exist_ok=True)
        with open(full, "w") as f:
            f.write(text)
    add_prototypes(root, protos)


def add_prototypes(root, protos):
    """Append prototypes before the final #endif of their header."""
    by_h = {}
    for p, h in protos:
        by_h.setdefault(h, []).append(p)
    for h, ps in by_h.items():
        path = os.path.join(root, h)
        text = read_text(path)
        block = "".join("%s\n" % p for p in ps if p not in text)
        if not block:
            continue
        k = text.rstrip().rfind("#endif")
        if k < 0:
            raise SplitError("%s: no include guard #endif" % h)
        text = text[:k] + "/* defined in C in one object, called from another (T-0500) */\n" + block + "\n" + text[k:]
        with open(path, "w") as f:
            f.write(text)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("units", nargs="*")
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--root", default=".")
    a = ap.parse_args(argv)
    sel = list(a.units)
    if a.all:
        sel = ["main"] + srcscan.overlay_names(a.root)
    if not sel:
        ap.error("name units or use --all")
    try:
        writes, protos = plan(a.root, sel)
        for p, h in protos:
            print("  prototype for %s: %s" % (h, p))
        if not a.dry_run:
            apply(a.root, writes, protos)
    except SplitError as e:
        print("split_objects.py: %s" % e, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
