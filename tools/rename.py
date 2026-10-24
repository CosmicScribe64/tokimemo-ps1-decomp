#!/usr/bin/env python3
"""Rename functions, globals, fields, types, locals, parameters, files and constants (T-27002).

Every phase 2 rename goes through this tool; nothing is renamed by search-and-replace. A rename is
a row of a table. The tool resolves each row to (unit, address) or (unit, type), checks it for
collisions, edits sources and configs, records it in a ledger under config/renames/, and can replay
the ledger on any tree (a branch, a contributor PR, the integration tree after a merge).
Usage for agents: wiki/rename-tool.md. Design: drafts/T-25900/audit.md, section 3.

Usage (inside Docker, from the repo root):
  rename.py plan TABLE [--ledger NAME] [--allow-alias]   dry run: resolve, check, report; exit 1 on any problem
  rename.py apply TABLE [--ledger NAME] [--allow-alias] [--no-verify] [--build]
                                                    edit, prove (see Proof), append the rows to the ledger
  rename.py check                                   no ledger name has come back (ninja: build/renames.ok)
  rename.py replay [LEDGER...]                      apply the whole ledger again; a renamed tree is a no-op
  rename.py lookup NAME [--unit U]                  current name and history of a name
  rename.py export                                  old-to-current mapping, tab separated (tools/wiki.py)
  rename.py const --header H (--enum NAME | --defines) --values FILE --site SITE... [--apply]
                                                    add an enum or #defines and use them at the named sites

Table rows (whitespace separated; a line that starts with `#` is a comment; the rest of a row is its
note, a `#` inside it included):
  # kind scope            old            new               note
  func   main             func_8004284C  pad_trigger_any   T-26010 returns the edge bits of the pad word
  data   DATE             D_8015DFCC     date_scene_timer  T-26100
  field  GameState        unk_F5F        girl              HYPOTHESIS: index of the current girl
  type   main             Rec38          GirlRecord        T-26001
  local  DATE:func_8013E7C0  var_s0      index
  param  main:func_80046500  arg0        sector
  file   TAIIKU           80133C80       runner_race       T-26300 the runner race event
  dir    TAIIKU           TAIIKU         sports_festival   folder src/ovl/TAIIKU -> src/ovl/sports_festival
  file   include          ovl/TAIIKU.h   ovl/sports_festival.h
scope: the link unit (`main` or an overlay name) for func, data, type, file and dir; the type name for
field; `UNIT:function` for local and param; `include` for a header (paths relative to include/).
A note that starts with HYPOTHESIS marks a claim of game meaning that the code does not prove: a
function or global then goes into a HYPOTHESES symbol file, a field or type gets a
`/* HYPOTHESIS: ... */` comment at its definition. Tables with file or dir rows hold nothing else,
so the moves get a commit of their own (git keeps the history of the moved files).

Resolution (func, data): the address comes from the placeholder (func_/D_ + 8 hex digits) or from
the symbol files the unit's yaml loads (tools/symtab.py). Rows are keyed by (unit, address), never by
the old name alone: placeholders repeat across overlays, and EVENT's range overlaps the main bss. An
overlay row may only rename the overlay's own symbols. A main symbol is renamed in every unit
except the overlays whose own range holds the address (EVENT for 0x800F6000-0x80126000).
  symbol files  an existing line at the address is renamed in place (attributes kept) or moved to
                the file of the row's role (fact or hypothesis); otherwise a line is added where
                symtab.placement() says, and a new file is listed in the yamls that need it
  code          identifier tokens in the scope's C files and headers, the suffix of guard macros
                (MAIN_API_OVERRIDE_<old>, <OVL>_OVERRIDE_<old>, W9_TT_<old>), INCLUDE_ASM and
                INCLUDE_RODATA name arguments; never inside string literals
  comments      mentions in the scope's files; `UNIT:old` mentions in any file of src/, include/
                and config/ when UNIT sees the address; other mentions are listed, not changed
  config keys   config/proto_known.txt (`kind UNIT:name`), config/migrate_globals.txt (symbols,
                `proof=<func>`, file paths, types, headers), config/toolchains.txt (file names)
field: the type must have one definition and the field one declaration in it. Use sites are found
by the compiler: gcc -fsyntax-only (native gcc in the image) over every C file that includes the
type's header, before and after renaming the declaration; each new "has no member named" error
gives the position to patch (a macro body when the use is inside a macro). Repeated until no new
error; the error set must then equal the one before. Files with NON_MATCHING run again with
-DNON_MATCHING. Uses in disabled code are listed.
type: the typedef name and the struct tag, in every file of the header closure of the C files
that see the definition. local/param: tokens inside the one function body (and, for a parameter,
its prototypes in the headers).
file/dir: the yaml `c` subsegment name and its `.rodata`/`.data` islands, `git mv` of the C file,
asm path strings in the C files, config keys and path mentions; a renamed C file gets a first
comment with its original address range (config/objects/<UNIT>.txt). header: `git mv`, every
`#include`, the include guard, config/migrate_globals.txt.

Collision checks (all in plan): a valid identifier in the project's case convention (snake_case;
PascalCase for types), not a C keyword, not reserved (_X, __x), not a placeholder pattern, no
address inside the name; unique in its link scope against every symbol file name; not an
identifier anywhere in the header closure of the scope (so no shadowing local, macro, typedef or
SDK name); unique without regard to case per unit (splat's .s files on macOS); never used before in
the ledger for another symbol. A name that already names the same address elsewhere (the O.BIN
alias debt, G2 of the audit) needs --allow-alias; the build then decides (tools/sync_protos.py --fix).

Proof (apply, unless --no-verify): (1) the inverse rows applied to the changed sources give the old
text byte for byte (identifier kinds); (2) the gcc -fsyntax-only error set of the affected C files
is the same before and after, with names mapped; (3) tools/sync_protos.py views: no C file sees
another type for any symbol, names mapped; (4) tools/check_symbols.py and `rename.py check`.
The proof that counts is the clean build: `rm -rf asm build; configure; ninja` with 27 of 27 sha1
OK and unchanged progress (`--build` runs it under nice -n 19 with 2 jobs).

Ledger: config/renames/<NAME>.txt, one file per work unit (default: the first row's unit), rows
`kind scope old new key note` (key: the address for func, data and file, else `-`). Rows of one key
form a chain (a later row may rename a name again); replay maps every earlier name of a chain to its
last name, in any order of the ledger files. The lint refuses branching chains and cycles. In a
table and in a ledger only a line that starts with # is a comment (a note keeps its #).
Tests: tools/test_rename.py (synthetic trees, no game data).
"""
import argparse
import glob
import os
import re
import shutil
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import symtab  # noqa: E402

ENC = "latin-1"           # byte-transparent: files are read and written byte for byte
LEDGER_DIR = "config/renames"
KINDS = ("func", "data", "field", "type", "local", "param", "file", "dir", "const")
IDENT_KINDS = ("func", "data", "type", "local", "param")
EXE_TEXT_OFF = 0x80041000 - 0x800
SDK_RANGE = (0x80086810, 0x800AF340)

C_KEYWORDS = set("""auto break case char const continue default do double else enum extern float for goto
if int long register return short signed sizeof static struct switch typedef union unsigned void volatile
while asm inline fortran entry""".split())
# libc names the game and the PsyQ headers use; a symbol must not take one
LIBC = set("""abs atoi atol bcmp bcopy bsearch bzero calloc exit free getc getchar gets index isalnum isalpha
isdigit islower isspace isupper labs longjmp main malloc memchr memcmp memcpy memmove memset printf putchar
puts qsort rand realloc rindex setjmp sprintf srand strcat strchr strcmp strcpy strcspn strlen strncat
strncmp strncpy strpbrk strrchr strspn strstr strtok strtol strtoul tolower toupper""".split())
# m2c's own shapes: sp + upper-case hex offset, var_/temp_/phi_ + register (+ _n), argN
PLACEHOLDER_NAME_RE = re.compile(
    r"^(?:(?:func|D|jtbl|L)_?[0-9A-Fa-f]{8}|unk_?[0-9A-Fa-f]*|pad\d*|sp[0-9A-F]+|arg\d+"
    r"|(?:var|temp|phi)_(?:[vatskf]\d+|at|ra|fp|gp|sp|lo|hi|zero)(?:_\d+)?)$")
SNAKE_RE = re.compile(r"^[a-z][a-z0-9_]*$")
PASCAL_RE = re.compile(r"^[A-Z][A-Za-z0-9]*$")
UPPER_RE = re.compile(r"^[A-Z][A-Z0-9_]*$")
IDENT_RE = re.compile(r"^[A-Za-z_]\w*$")
TICKET_RE = re.compile(r"^T-\d+[:,.]?\s*")

TOKEN_RE = re.compile(r"""
    (?P<c>/\*.*?\*/|//[^\n]*)
  | (?P<s>"(?:\\.|[^"\\\n])*")
  | (?P<ch>'(?:\\.|[^'\\\n])*')
  | (?P<n>\.?[0-9](?:[eEpP][+-]|[\w.])*)
  | (?P<id>[A-Za-z_]\w*)
""", re.S | re.X)
INCLUDE_RE = re.compile(r'^[ \t]*#[ \t]*include[ \t]*"([^"]+)"', re.M)
GUARD_PREFIX_RE = re.compile(r"^([A-Z][A-Z0-9_]*_)")
SUB_RE = re.compile(r'^(\s*- (?:\[\s*0x[0-9A-Fa-f]+\s*,\s*([.\w]+)\s*,\s*|'
                    r'\{\s*start:\s*0x[0-9A-Fa-f]+\s*,\s*type:\s*([.\w]+)\s*,\s*name:\s*))([\w/]+)')
SUB_START_RE = re.compile(r'^\s*- (?:\[\s*(0x[0-9A-Fa-f]+)\s*,\s*([.\w]+)\s*,\s*([\w/]+)|'
                          r'\{\s*start:\s*(0x[0-9A-Fa-f]+)\s*,\s*type:\s*([.\w]+)\s*,\s*name:\s*([\w/]+))')
VRAM_RE = re.compile(r"^\s*vram:\s*(0x[0-9A-Fa-f]+)", re.M)


class RenameError(Exception):
    pass


# ======================================================================================= rows

class Row:
    def __init__(self, kind, scope, old, new, note="", where="", key="-"):
        self.kind, self.scope, self.old, self.new = kind, scope, old, new
        self.note, self.where, self.key = note, where, key
        self.uses_only = False      # replay of a member chain: the declaration holds another name of the chain

    @property
    def hypothesis(self):
        return self.note.upper().startswith("HYPOTHESIS")

    @property
    def unit(self):
        """The link unit of the row (field and header rows: filled in when resolved)."""
        if self.kind in ("local", "param"):
            return self.scope.split(":", 1)[0]
        return self.scope

    def ledger_line(self):
        return " ".join([self.kind, self.scope, self.old, self.new, self.key] + ([self.note] if self.note else []))

    def __repr__(self):
        return "%s %s %s -> %s" % (self.kind, self.scope, self.old, self.new)


def strip_comment(line):
    return re.split(r"(?:^|\s)#", line, 1)[0]


def strip_line_comment(line):
    """Tables and ledgers: only a line that starts with # is a comment (a note may hold ' #')."""
    return "" if line.lstrip().startswith("#") else line


def parse_rows(text, where, ledger=False):
    """Rows of a table (kind scope old new note) or a ledger (kind scope old new key note)."""
    rows = []
    for n, line in enumerate(text.splitlines(), 1):
        parts = strip_line_comment(line).split()
        if not parts:
            continue
        need = 5 if ledger else 4
        if len(parts) < need or parts[0] not in KINDS:
            raise RenameError("%s:%d: expected `kind scope old new%s note`, got %r"
                              % (where, n, " key" if ledger else "", line.strip()))
        note = " ".join(parts[need:])
        row = Row(parts[0], parts[1], parts[2], parts[3], note, "%s:%d" % (where, n),
                  parts[4] if ledger else "-")
        rows.append(row)
    return rows


def read_ledger(root):
    """All ledger rows, file by file in name order."""
    rows = []
    for p in sorted(glob.glob(os.path.join(root, LEDGER_DIR, "*.txt"))):
        with open(p, encoding=ENC) as f:
            rows += parse_rows(f.read(), os.path.relpath(p, root), ledger=True)
    return rows


# ======================================================================================= tree

class Tree:
    """The working tree with pending edits held in memory until flush()."""

    def __init__(self, root):
        self.root = root
        self.texts = {}     # path -> text (None: deleted)
        self.orig = {}      # path -> text on disk (None: missing)
        self.moves = []     # (old path, new path)
        self._index = None
        self.units = symtab.read_units(root)
        self.unit_names = [u.name for u in self.units]
        self._idents = {}   # path -> (text, identifiers): the collision checks read every file per row

    # ------------------------------------------------------------- files
    def read(self, path):
        if path in self.texts:
            return self.texts[path]
        if path not in self.orig:
            full = os.path.join(self.root, path)
            if os.path.isfile(full):
                with open(full, encoding=ENC, newline="") as f:
                    self.orig[path] = f.read()
            else:
                self.orig[path] = None
        return self.orig[path]

    def exists(self, path):
        return self.read(path) is not None

    def write(self, path, text):
        self.read(path)
        self.texts[path] = text

    def idents(self, path):
        """Identifier tokens of a file (member names excluded), cached per text."""
        text = self.read(path) or ""
        got = self._idents.get(path)
        if got is None or got[0] is not text:
            got = (text, ident_tokens(text))
            self._idents[path] = got
        return got[1]

    def move(self, old, new):
        text = self.read(old)
        if text is None:
            raise RenameError("%s does not exist" % old)
        if self.exists(new):
            raise RenameError("%s exists already" % new)
        self.write(new, text)
        self.texts[old] = None
        self.moves.append((old, new))
        self._index = None

    def changed(self):
        return sorted(p for p, t in self.texts.items() if t != self.orig.get(p))

    def listdir(self, pattern):
        """Paths matching a glob pattern (relative), with pending creations and deletions."""
        out = {os.path.relpath(p, self.root) for p in glob.glob(os.path.join(self.root, pattern), recursive=True)
               if os.path.isfile(p)}
        rx = re.compile(fnmatch_to_re(pattern))
        for p, t in self.texts.items():
            if t is None:
                out.discard(p)
            elif rx.match(p):
                out.add(p)
        return sorted(out)

    def flush(self, git=True):
        """Write every pending edit; moves go through `git mv` when the file is tracked."""
        for old, new in self.moves:
            src, dst = os.path.join(self.root, old), os.path.join(self.root, new)
            if not os.path.exists(src):
                continue
            os.makedirs(os.path.dirname(dst), exist_ok=True)
            done = False
            if git and is_tracked(self.root, old):
                done = subprocess.run(["git", "mv", old, new], cwd=self.root,
                                      stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode == 0
            if not done:
                os.rename(src, dst)
            try:
                os.removedirs(os.path.dirname(src))
            except OSError:
                pass
        for p, t in sorted(self.texts.items()):
            full = os.path.join(self.root, p)
            if t is None:
                if os.path.exists(full):
                    os.remove(full)
                continue
            if t == self.orig.get(p) and os.path.exists(full):
                continue
            os.makedirs(os.path.dirname(full) or ".", exist_ok=True)
            with open(full, "w", encoding=ENC, newline="") as f:
                f.write(t)

    def snapshot(self):
        return dict(self.texts), list(self.moves)

    def restore(self, snap):
        self.texts, self.moves = dict(snap[0]), list(snap[1])
        self._index = None

    # ------------------------------------------------------------- units and files
    def yaml_path(self, unit):
        return symtab.EXE_YAML if unit == "main" else "config/overlays/%s.yaml" % unit

    def unit_range(self, unit):
        return next((u.base, u.end) for u in self.units if u.name == unit)

    def own_unit(self, unit, addr):
        """`unit` when addr lies in its range, else main (symtab's scope rule)."""
        if unit != "main":
            lo, hi = self.unit_range(unit)
            if lo <= addr < hi:
                return unit
        return "main"

    def subsegments(self, unit):
        """[(vram, type, name)] of a unit's yaml (from the pending text)."""
        text = self.read(self.yaml_path(unit)) or ""
        m = VRAM_RE.search(text)
        delta = int(m.group(1), 16) if (m and unit != "main") else EXE_TEXT_OFF
        out = []
        for line in text.splitlines():
            m = SUB_START_RE.match(line)
            if m:
                off, typ, name = (m.group(1), m.group(2), m.group(3)) if m.group(1) else \
                    (m.group(4), m.group(5), m.group(6))
                out.append((int(off, 16) + delta, typ, name))
        return out

    def c_names(self, unit):
        return [(v, n) for v, t, n in self.subsegments(unit) if t == "c"]

    def source_path(self, unit, name):
        return "src/%s.c" % name if unit == "main" else "src/ovl/%s.c" % name

    def index(self):
        """{'sources': {c path: unit}, 'headers': [h], 'closure': {c path: set(h)},
        'users': {h: set(units)}} for the pending tree."""
        if self._index is None:
            sources = {}
            for u in self.unit_names:
                for _v, n in self.c_names(u):
                    p = self.source_path(u, n)
                    if self.exists(p):
                        sources[p] = u
            headers = self.listdir("include/**/*.h")
            closure = {p: self.closure(p) for p in sources}
            users = {}
            for p, hs in closure.items():
                for h in hs:
                    users.setdefault(h, set()).add(sources[p])
            self._index = {"sources": sources, "headers": headers, "closure": closure, "users": users}
        return self._index

    def includes(self, path):
        text = self.read(path) or ""
        out = []
        for inc in INCLUDE_RE.findall(text):
            for cand in (os.path.normpath(os.path.join(os.path.dirname(path), inc)), "include/" + inc):
                if cand.startswith("include/") and self.exists(cand):
                    out.append(cand)
                    break
        return out

    def closure(self, path, seen=None):
        seen = set() if seen is None else seen
        for h in self.includes(path):
            if h not in seen:
                seen.add(h)
                self.closure(h, seen)
        return seen

    def private_headers(self, unit):
        idx = self.index()
        return sorted(h for h, us in idx["users"].items() if us == {unit})

    def unit_sources(self, unit):
        return sorted(p for p, u in self.index()["sources"].items() if u == unit)

    def header_owner(self, h):
        """The one unit whose C files include h, else main."""
        us = self.index()["users"].get(h, set())
        return next(iter(us)) if len(us) == 1 else "main"


def fnmatch_to_re(pattern):
    out = ""
    i = 0
    while i < len(pattern):
        if pattern.startswith("**/", i):
            out += "(?:.*/)?"
            i += 3
        elif pattern[i] == "*":
            out += "[^/]*"
            i += 1
        else:
            out += re.escape(pattern[i])
            i += 1
    return out + "$"


def is_tracked(root, path):
    try:
        return subprocess.run(["git", "ls-files", "--error-unmatch", path], cwd=root, stdout=subprocess.DEVNULL,
                              stderr=subprocess.DEVNULL).returncode == 0
    except OSError:
        return False


# ======================================================================================= lexer

def tokens(text):
    """[(kind, start, end)]: kind c (comment), s (string), ch (char), n (number), id (identifier)."""
    return [(m.lastgroup, m.start(), m.end()) for m in TOKEN_RE.finditer(text)]


def after_member(text, i):
    """True when the token at i follows `.` or `->` (a member name)."""
    j = i - 1
    while j >= 0 and text[j] in " \t\r\n":
        j -= 1
    if j < 0:
        return False
    if text[j] == "." and not (j > 0 and text[j - 1] == "."):
        return True
    return text[j] == ">" and j > 0 and text[j - 1] == "-"


def ident_tokens(text, members=False):
    """{identifier} of a text (member names after . and -> only with members=True)."""
    out = set()
    for k, s, e in tokens(text):
        if k == "id" and (members or not after_member(text, s)):
            out.add(text[s:e])
    return out


def guard_suffix(tok, name):
    """The prefix when tok is a guard macro <PREFIX>_<name> (MAIN_API_OVERRIDE_x, W9_TT_x), else None."""
    if tok.endswith("_" + name) and len(tok) > len(name) + 1:
        pre = tok[:-len(name)]
        if GUARD_PREFIX_RE.match(pre) and pre == GUARD_PREFIX_RE.match(pre).group(1):
            return pre
    return None


def comment_regex(name):
    return re.compile(r"(?<![\w])(?:([A-Za-z_]\w*):)?((?:[A-Z][A-Z0-9_]*_)?)" + re.escape(name) + r"(?![\w])")


def rewrite_text(text, old, new, idents=True, comments=True, accept=None, members=False, spans=None,
                 suffixes=True):
    """(text, idents changed, comment mentions changed) with identifier tokens `old` -> `new`.

    accept(qualifier) decides a comment mention `Q:old` (qualifier None for a plain mention);
    default: every mention. members: also
    tokens after . and ->. spans: only inside these (start, end) ranges. suffixes: guard macros."""
    out, pos, n_id, n_c = [], 0, 0, 0
    crx = comment_regex(old) if comments else None
    for k, s, e in tokens(text):
        if spans is not None and not any(a <= s and e <= b for a, b in spans):
            continue
        if k == "id" and idents:
            tok = text[s:e]
            if tok == old and (members or not after_member(text, s)):
                out += [text[pos:s], new]
                pos, n_id = e, n_id + 1
            elif suffixes and guard_suffix(tok, old) is not None:
                out += [text[pos:s], guard_suffix(tok, old) + new]
                pos, n_id = e, n_id + 1
        elif k == "c" and comments and old in text[s:e]:
            body, cnt = text[s:e], [0]

            def sub(m):
                q = m.group(1)
                if accept is not None and not accept(q):
                    return m.group(0)
                cnt[0] += 1
                return (q + ":" if q else "") + m.group(2) + new
            nb = crx.sub(sub, body)
            if nb != body:
                out += [text[pos:s], nb]
                pos, n_c = e, n_c + cnt[0]
    out.append(text[pos:])
    return "".join(out), n_id, n_c


def string_mentions(text, name):
    """Line numbers of string literals that mention name."""
    rx = re.compile(r"(?<![\w])" + re.escape(name) + r"(?![\w])")
    return [text.count("\n", 0, s) + 1 for k, s, e in tokens(text) if k == "s" and rx.search(text[s:e])]


def comment_mentions(text, name):
    rx = re.compile(r"(?<![\w])" + re.escape(name) + r"(?![\w])")
    return [text.count("\n", 0, s) + 1 for k, s, e in tokens(text) if k == "c" and rx.search(text[s:e])]


# ======================================================================================= names

def name_problem(row, new=None):
    """Why `new` (default row.new) is not an acceptable name for the row's kind, else None."""
    new = row.new if new is None else new
    if not IDENT_RE.match(new):
        return "%s is not a C identifier" % new
    if new in C_KEYWORDS:
        return "%s is a C keyword" % new
    if new.startswith("__") or re.match(r"^_[A-Z]", new) or (new.startswith("_") and not row.old.startswith("_")):
        return "%s is a reserved name (leading underscore)" % new
    if PLACEHOLDER_NAME_RE.match(new) or symtab.placeholder_addr(new) is not None or re.match(r"^(?:unk(?:_|\d|$)|pad(?:\d*|_[0-9A-Fa-f]+)$)", new):
        return "%s looks like a placeholder; a rename needs a real name" % new
    m = re.search(r"[0-9A-Fa-f]{8}", new)
    if m and re.search(r"\d", m.group(0)):
        return "%s holds an address; the address lives in the symbol file" % new
    if new in LIBC:
        return "%s is a C library name" % new
    if row.kind in ("func", "data", "field", "local", "param") and not SNAKE_RE.match(new):
        return "%s is not snake_case (lower case, digits and _)" % new
    if row.kind == "type" and not PASCAL_RE.match(new):
        return "%s is not PascalCase" % new
    if row.new == row.old:
        return "old and new are the same"
    return None


def stem_problem(stem, header=False):
    parts = stem.split("/")
    last = parts[-1]
    if header:
        if not last.endswith(".h"):
            return "%s: a header name ends in .h" % stem
        last = last[:-2]
    for p in parts[:-1] + [last]:
        if not SNAKE_RE.match(p) and not (header and p == "ovl"):
            return "%s: file and folder names are snake_case" % stem
    if re.search(r"[0-9A-Fa-f]{8}", last) and re.search(r"\d", last):
        return "%s holds an address; the address lives in the first comment and config/objects" % stem
    return None


# ======================================================================================= ledger chains

def row_key(row):
    """Chain key of a ledger row: (kind group, unit, address or type or function)."""
    if row.kind in ("func", "data"):
        return ("sym", row.scope, row.key)
    if row.kind == "file":
        return ("header" if row.scope == "include" else "file", row.scope, row.key)
    if row.kind in ("local", "param"):
        return ("local", row.scope, row.kind)
    return (row.kind, row.scope, "-")


def chains(rows):
    """{key + (n,): (names in order, rows)} from ledger rows: the rows of one key (one address, or one
    type's members, one unit's types, ...) split into chains of connected names. Raises RenameError
    on a branching chain (one name renamed to two names) or a cycle."""
    by_key = {}
    for r in rows:
        if r.kind != "const":
            by_key.setdefault(row_key(r), []).append(r)
    out, problems = {}, []
    for key, rs in by_key.items():
        parent = {}

        def find(x):
            while parent.setdefault(x, x) != x:
                parent[x] = parent[parent[x]]
                x = parent[x]
            return x
        for r in rs:
            parent[find(r.old)] = find(r.new)
        groups = {}
        for r in rs:
            groups.setdefault(find(r.old), []).append(r)
        for n, g in enumerate(groups.values()):
            out[key + (n,)] = chain_of(g, problems)
    if problems:
        raise RenameError("ledger: " + "; ".join(problems))
    return out


def chain_of(rows, problems):
    nxt = {}
    for r in rows:
        if r.old in nxt and nxt[r.old][0] != r.new:
            problems.append("%s renames %s to %s, %s to %s" % (r.where, r.old, r.new, nxt[r.old][1], nxt[r.old][0]))
        nxt[r.old] = (r.new, r.where)
    starts = [r.old for r in rows if r.old not in {x.new for x in rows}]
    if not starts:
        problems.append("%s: the renames of %s form a cycle" % (rows[0].where, rows[0].old))
        return [rows[0].old, rows[0].new], rows
    names = [starts[0]]
    while names[-1] in nxt and len(names) <= len(rows):
        names.append(nxt[names[-1]][0])
    if len(set(names)) != len(names):
        problems.append("%s: the renames of %s form a cycle" % (rows[0].where, starts[0]))
    return names, rows


# ======================================================================================= context

class Context:
    """State shared by the rows of one run."""

    def __init__(self, root, ledger_rows=None, allow_alias=False, merge=False, log=None):
        self.root = root
        self.tree = Tree(root)
        self.ledger = read_ledger(root) if ledger_rows is None else ledger_rows
        self.allow_alias = allow_alias
        self.merge = merge          # replay: the new name may exist already for the same key
        self.problems = []
        self.notes = []             # mentions listed, not changed
        self.report = []            # per-row summary lines
        self.done = []              # resolved rows (key filled in)
        self.log = log or (lambda s: None)
        self.field_files = set()    # C files the field oracle located uses in

    def table(self):
        texts = {p: t for p, t in self.tree.texts.items() if t is not None and p.startswith("config/")}
        return symtab.SymbolTable(self.root, texts=texts)

    def chains(self):
        """Ledger chains, with this run's rows added (plan, apply). Replay (merge) uses the ledger only: its
        rows are the ledger's own renames written as old -> last name, which would branch the chains."""
        if self.merge:
            return chains(self.ledger)
        return chains(self.ledger + [r for r in self.done if r.kind != "const"])

    def problem(self, row, msg):
        self.problems.append("%s: %s: %s" % (row.where, row, msg))

    def note(self, row, msg):
        self.notes.append("%s: %s" % (row.where, msg))


# ======================================================================================= symbols

def ledger_names(ctx):
    """{name: key} of every name the ledger has given (for the 'never reused' rule)."""
    out = {}
    for r in ctx.ledger + ctx.done:
        if r.kind in ("func", "data"):
            out.setdefault(r.new, ("sym", r.scope, r.key))
    return out


def scope_units_for(tree, owner, addr):
    """Units whose sources see (owner, addr): the owner overlay, or every unit that sees addr as main."""
    if owner != "main":
        return [owner]
    return [u for u in tree.unit_names if tree.own_unit(u, addr) == "main"]


def scope_files(tree, owner, addr):
    """C files and headers whose tokens name (owner, addr) (memoized per index and unit set)."""
    units = scope_units_for(tree, owner, addr)
    idx = tree.index()
    memo = idx.setdefault("scope_memo", {})
    key = (owner, tuple(units))
    if key not in memo:
        memo[key] = _scope_files(tree, owner, units, idx)
    return memo[key]


def _scope_files(tree, owner, units, idx):
    files = [p for p, u in idx["sources"].items() if u in units]
    if owner != "main":
        files += tree.private_headers(owner)
    else:
        other = [u for u in tree.unit_names if u not in units]
        files += [h for h in idx["headers"] if not (idx["users"].get(h) and idx["users"][h] <= set(other))]
    return sorted(set(files))


def closure_files(tree, files):
    out = set(files)
    for f in files:
        if f.endswith(".c"):
            out |= tree.index()["closure"].get(f, tree.closure(f))
    return sorted(out)


def unit_qualifier_ok(tree, owner, addr):
    """accept() for comment mentions `Q:name` of (owner, addr)."""
    seeing = set(scope_units_for(tree, owner, addr))

    def accept(q):
        if q is None or q not in tree.unit_names:
            return True
        return q in seeing
    return accept


def plan_symbol(ctx, row):
    tree = ctx.tree
    if row.scope not in tree.unit_names:
        return ctx.problem(row, "unknown unit %s" % row.scope)
    table = ctx.table()
    if ctx.merge and row.key.startswith("0x"):
        res = (row.scope, int(row.key, 16))       # replay: the ledger knows the address
    else:
        res = table.resolve(row.scope, row.old)
    if res is None:
        return ctx.problem(row, "%s is neither a placeholder nor a name of %s's symbol files" % (row.old, row.scope))
    owner, addr = res
    if owner != row.scope:
        return ctx.problem(row, "%s is a main-exe symbol (0x%08X); overlay rows rename only the overlay's own "
                                "symbols (ask the main owner: config/renames/requests.txt)" % (row.old, addr))
    if owner == "main" and not symtab.MAIN_LO <= addr < symtab.MAIN_HI:
        return ctx.problem(row, "0x%08X is not a main-exe address" % addr)
    lines = table.lines_at(owner, addr)
    is_func = row.old.startswith("func_") or any(l.is_func for l in lines if l.name == row.old)
    if row.kind == "data" and (row.old.startswith("func_") or any(l.is_func for l in lines if l.name == row.old)):
        return ctx.problem(row, "%s is a function; use kind func" % row.old)
    if row.kind == "func" and (row.old.startswith("D_") or row.old.startswith("jtbl_")):
        return ctx.problem(row, "%s is data; use kind data" % row.old)
    names = table.names_at(owner, addr)
    real = [n for n in names if symtab.placeholder_addr(n) is None and n != row.new
            and not any(l.alias for l in lines if l.name == n)]
    if symtab.placeholder_addr(row.old) is not None and real and not ctx.merge:
        return ctx.problem(row, "0x%08X is named %s already; use that as the old name" % (addr, real[0]))
    if symtab.placeholder_addr(row.old) is None and row.old not in names and not ctx.merge:
        return ctx.problem(row, "%s does not name 0x%08X in the symbol files" % (row.old, addr))
    row.key = "0x%08X" % addr
    files = scope_files(tree, owner, addr)
    row.files = set(files)
    alias = row.new in names
    row.alias = alias
    # ---- collisions
    if not ctx.merge:
        why = name_problem(row)
        if why:
            return ctx.problem(row, why)
    units = scope_units_for(tree, owner, addr)
    check_units = set(units) | ({"main"} if owner != "main" else set())
    for u in sorted(check_units):
        for name, a in table.all_names(u).items():
            if name == row.new and (tree.own_unit(u, a) != owner or a != addr):
                return ctx.problem(row, "%s already names 0x%08X in %s" % (row.new, a, u))
            if name != row.new and name.lower() == row.new.lower():
                return ctx.problem(row, "%s differs from %s (0x%08X in %s) only in case" % (row.new, name, a, u))
    prev = ledger_names(ctx).get(row.new)
    if prev and prev != ("sym", row.scope, row.key):
        return ctx.problem(row, "%s was given to another symbol (%s %s) in the ledger" % (row.new, prev[1], prev[2]))
    if alias and not (ctx.allow_alias or ctx.merge):
        return ctx.problem(row, "%s already names 0x%08X (alias debt, audit G2); the two declarations may differ "
                                "in type: rerun with --allow-alias, then tools/sync_protos.py --fix and build" %
                           (row.new, addr))
    if not alias and not ctx.merge:
        users = [f for f in closure_files(tree, files) if row.new in tree.idents(f)]
        if users:
            return ctx.problem(row, "%s is already an identifier in %s (it would shadow or clash)" %
                               (row.new, ", ".join(users[:3]) + (" ..." if len(users) > 3 else "")))
    # ---- symbol files
    edit_symbol_files(ctx, row, owner, addr, is_func, table)
    # ---- code and comments
    accept = unit_qualifier_ok(tree, owner, addr)
    n_files = n_tok = n_com = 0
    for f in files:
        text = tree.read(f)
        if text is None or (row.old not in text):
            continue
        new, a, b = rewrite_text(text, row.old, row.new, accept=accept)
        if new != text:
            tree.write(f, new)
            n_files, n_tok, n_com = n_files + 1, n_tok + a, n_com + b
        for line in string_mentions(new, row.old):
            ctx.note(row, "%s:%d: string mentions %s (not changed)" % (f, line, row.old))
    # qualified mentions elsewhere
    others = [p for p in tree.listdir("src/**/*.c") + tree.listdir("include/**/*.h") if p not in files]
    for f in others:
        text = tree.read(f)
        if not text or row.old not in text:
            continue
        new, _a, b = rewrite_text(text, row.old, row.new, idents=False,
                                  accept=lambda q: q is not None and q in tree.unit_names and accept(q))
        if new != text:
            tree.write(f, new)
            n_com += b
        if comment_mentions(new, row.old):
            ctx.note(row, "%s: %d comment mention(s) of %s outside the scope (not changed)"
                     % (f, len(comment_mentions(new, row.old)), row.old))
    n_cfg = edit_config_keys(ctx, row, owner, addr, accept)
    ctx.report.append("%s: 0x%08X (%s), %d tokens and %d comment mentions in %d files, %d config keys%s" % (
        row, addr, owner, n_tok, n_com, n_files, n_cfg, ", alias" if alias else ""))
    ctx.done.append(row)


def symbol_line(name, addr, attrs):
    return "%s = 0x%08X;%s\n" % (name, addr, (" // " + " ".join(attrs)) if attrs else "")


def splat_attrs(comment):
    """The splat key:value attributes of a symbol line comment."""
    return [w for w in comment.split() if re.match(r"^[a-z_]+:[^:\s]+$", w)]


FILE_HEADERS = {
    "fact": "// Names of {what} (tools/rename.py, T-27002): facts, with the evidence in the ledger "
            "(config/renames/).\n",
    "hyp": "// HYPOTHESES, not facts: names of {what} that claim a game meaning the code does not prove\n"
           "// (tools/rename.py, T-27002; the evidence is in the ledger, config/renames/).\n",
}


def ensure_symbol_file(ctx, path, owner):
    """Create a placement file with its header and list it in the yamls that need it."""
    tree = ctx.tree
    if not tree.exists(path):
        what = "the overlay %s" % owner if owner != "main" else (
            "main-exe symbols inside EVENT's range (main yaml only)" if "main" in os.path.basename(path)
            else "main-exe symbols (every yaml loads this file)")
        tree.write(path, FILE_HEADERS["hyp" if symtab.is_hypotheses_file(path) else "fact"].format(what=what))
    table = ctx.table()
    for y in table.yamls_for(path, owner):
        text = tree.read(y)
        if text is not None and path not in symtab.yaml_symbol_files(text):
            tree.write(y, symtab.add_yaml_symbol_file(text, path))


def insert_sorted(text, line, addr):
    """text with line inserted before the first symbol line of a higher address (else at the end)."""
    lines = text.splitlines(True)
    for i, l in enumerate(lines):
        m = symtab.LINE_RE.match(l)
        if m and int(m.group(2), 0) > addr:
            return "".join(lines[:i] + [line] + lines[i:])
    if lines and not lines[-1].endswith("\n"):
        lines[-1] += "\n"
    return "".join(lines + [line])


def edit_symbol_files(ctx, row, owner, addr, is_func, table):
    """Rename, move or add the symbol-file lines of (owner, addr) (see the module docstring)."""
    tree = ctx.tree
    target = table.placement(owner, addr, row.hypothesis)
    attrs, kept_visible = [], False
    loaded = set(table.files[owner]) - {symtab.TYPES_FILE}     # what the owner's splat sees
    by_path = {}
    for l in table.lines_at(owner, addr):
        if l.name == row.old:
            by_path.setdefault(l.path, []).append(l)
    for path, ls in sorted(by_path.items()):
        lines = tree.read(path).splitlines(True)
        for l in sorted(ls, key=lambda x: -x.lineno):        # bottom up: removals keep line numbers
            in_place = (l.path == symtab.TYPES_FILE or l.path == target or
                        (not row.hypothesis and not symtab.is_hypotheses_file(l.path)))
            if in_place:
                lines[l.lineno - 1] = re.sub(r"^(\s*)" + re.escape(row.old) + r"\b", lambda m: m.group(1) + row.new,
                                             lines[l.lineno - 1], count=1)
                if l.path in loaded:
                    kept_visible = True
            else:
                attrs += [a for a in splat_attrs(l.comment) if a not in attrs]
                del lines[l.lineno - 1]
        tree.write(path, "".join(lines))
    if row.new in [l.name for l in ctx.table().lines_at(owner, addr) if l.path in loaded]:
        kept_visible = True
    if not kept_visible:
        if is_func and "type:func" not in attrs:
            attrs.insert(0, "type:func")
        ensure_symbol_file(ctx, target, owner)
        tree.write(target, insert_sorted(tree.read(target), symbol_line(row.new, addr, attrs), addr))
    # one line per name and address in each file: drop repeats (replay of a merged tree)
    for path in set(ctx.table().loaders):
        text = tree.read(path)
        if text is None or row.new not in text:
            continue
        out = dedupe_symbol_lines(ctx, row, text, path)
        if out != text:
            tree.write(path, out)


def dedupe_symbol_lines(ctx, row, text, path):
    """`text` with a repeated `row.new = ADDR;` line dropped: the first stays, and takes the splat attributes
    (type:func size:0x10) of the dropped lines that it lacks. A different value of an attribute that
    both have stays as the first line has it, and is listed."""
    lines = text.splitlines(True)
    first = {}              # (name, address) -> index of the kept line in `out`
    out = []
    for l in lines:
        m = symtab.LINE_RE.match(l)
        if not (m and m.group(1) == row.new):
            out.append(l)
            continue
        key = (m.group(1), int(m.group(2), 0))
        if key not in first:
            first[key] = len(out)
            out.append(l)
            continue
        k = first[key]
        kept = symtab.LINE_RE.match(out[k])
        have = {a.split(":", 1)[0]: a for a in splat_attrs(kept.group(3))}
        extra = []
        for a in splat_attrs(m.group(3)):
            name = a.split(":", 1)[0]
            if name not in have:
                have[name] = a
                extra.append(a)
            elif have[name] != a:
                ctx.note(row, "%s: %s has %s, the dropped duplicate line has %s (kept the first)" % (
                    path, row.new, have[name], a))
        if extra:
            body = out[k].rstrip("\r\n")
            eol = out[k][len(body):]
            out[k] = body + ("" if "//" in kept.group(3) else " //") + " " + " ".join(extra) + eol
    return "".join(out)


def edit_config_keys(ctx, row, owner, addr, accept):
    """proto_known `kind UNIT:name` keys and migrate_globals symbols/proofs. Returns the count."""
    tree, n = ctx.tree, 0
    p = "config/proto_known.txt"
    text = tree.read(p)
    if text and row.old in text:
        def sub(m):
            return m.group(1) + ":" + row.new if accept(m.group(1)) and (m.group(1) == owner or owner == "main") \
                else m.group(0)
        new = re.sub(r"\b(\w+):" + re.escape(row.old) + r"\b", sub, text)
        n += new != text
        tree.write(p, new)
    p = "config/migrate_globals.txt"
    text = tree.read(p)
    if text and row.old in text:
        out = []
        for line in text.splitlines(True):
            body, _, com = line.partition("#")
            parts = body.split()
            if parts and parts[0] in ("aggregate", "keep") and row.old in body:
                fu = srcscan_unit(tree, parts[1]) if parts[0] == "keep" else "main"
                if owner == "main" or fu == owner:
                    body2 = re.sub(r"(?<![\w])" + re.escape(row.old) + r"(?![\w])", row.new, body)
                    if body2 != body:
                        n += 1
                    body = body2
            if com and row.old in com:
                com = re.sub(r"(?<![\w])(\w+):" + re.escape(row.old) + r"(?![\w])",
                             lambda m: m.group(1) + ":" + row.new if accept(m.group(1)) and m.group(1) in
                             tree.unit_names else m.group(0), com)
            out.append(body + ("#" + com if _ else ""))
        tree.write(p, "".join(out))
    return n


def srcscan_unit(tree, path):
    return tree.index()["sources"].get(path, "main")


# ======================================================================================= gcc oracle

GCC = ["gcc", "-fsyntax-only", "-std=gnu89", "-w", "-fdiagnostics-column-unit=byte",
       "-fno-diagnostics-show-caret", "-fmax-errors=0", "-Iinclude"]
DIAG_RE = re.compile(r"^(.+?):(\d+):(\d+): (error|note): (.*)$")
NO_MEMBER_RE = re.compile(r"^'(?:struct |union )?(\w+)'(?: \{aka '(?:struct |union )?(\w+)'\})? has no member named "
                          r"'(\w+)'(?:; did you mean '\w+'\?)?$")


class Scratch:
    """A copy of src/ and include/ with the pending texts, where gcc runs."""

    def __init__(self, tree, config=False):
        self.tree = tree
        self.dir = tempfile.mkdtemp(prefix="rename-")
        self.dirs = ("src/", "include/") + (("config/",) if config else ())
        for d in self.dirs:
            if os.path.isdir(os.path.join(tree.root, d)):
                shutil.copytree(os.path.join(tree.root, d), os.path.join(self.dir, d))
        self.synced = {}

    def sync(self):
        for p, t in self.tree.texts.items():
            if not p.startswith(self.dirs) or self.synced.get(p, 0) == t:
                continue
            full = os.path.join(self.dir, p)
            if t is None:
                if os.path.exists(full):
                    os.remove(full)
            else:
                os.makedirs(os.path.dirname(full), exist_ok=True)
                with open(full, "w", encoding=ENC, newline="") as f:
                    f.write(t)
            self.synced[p] = t

    def errors(self, files, jobs=2):
        """[(file, line, col, message, [(file, line, col) of macro expansion notes])] of gcc over files."""
        self.sync()
        out = []
        groups = []
        plain = [f for f in files if "NON_MATCHING" not in (self.tree.read(f) or "")]
        guarded = [f for f in files if f not in plain]
        for fs, extra in ((plain, []), (guarded, []), (guarded, ["-DNON_MATCHING"])):
            if fs:
                k = max(1, (len(fs) + jobs - 1) // jobs)
                groups += [(fs[i:i + k], extra) for i in range(0, len(fs), k)]
        env = dict(os.environ, LC_ALL="C", LANG="C")     # ASCII quotes in the messages
        procs = [subprocess.Popen(GCC + extra + fs, cwd=self.dir, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE,
                                  env=env) for fs, extra in groups]
        for (fs, extra), p in zip(groups, procs):
            _o, err = p.communicate()
            for line in err.decode(ENC).splitlines():
                m = DIAG_RE.match(line)
                if not m:
                    continue
                f, ln, col, kind, msg = m.group(1), int(m.group(2)), int(m.group(3)), m.group(4), m.group(5)
                if kind == "error":
                    out.append((f, ln, col, msg, [], tuple(extra)))
                elif out and msg.startswith("in expansion of macro"):
                    out[-1][4].append((f, ln, col))
        return out

    def close(self):
        shutil.rmtree(self.dir, ignore_errors=True)


def have_gcc():
    return shutil.which("gcc") is not None


def blank_comments(text):
    """text with comments and strings replaced by spaces (positions kept)."""
    out = list(text)
    for k, s, e in tokens(text):
        if k in ("c", "s", "ch"):
            for i in range(s, e):
                if out[i] != "\n":
                    out[i] = " "
    return "".join(out)


def match_brace(text, i):
    """Index after the brace that closes the `{` at i."""
    depth = 0
    for j in range(i, len(text)):
        if text[j] == "{":
            depth += 1
        elif text[j] == "}":
            depth -= 1
            if depth == 0:
                return j + 1
    raise RenameError("unbalanced braces")


TYPEDEF_FWD_RE = re.compile(r"\btypedef\s+(struct|union)\s+(\w+)\s+(\w+)\s*;")
AGG_RE = re.compile(r"\b(struct|union)\s+(\w+\s*)?\{")


def type_defs(text):
    """[(tag, typedef name, body start, body end, end of the definition)] of struct/union bodies."""
    clean = blank_comments(text)
    out = []
    for m in AGG_RE.finditer(clean):
        b = m.end() - 1
        e = match_brace(clean, b)
        tail = re.match(r"\s*(\w+)?", clean[e:])
        before = clean[:m.start()].rstrip()
        is_td = before.endswith("typedef")
        tdname = tail.group(1) if (is_td and tail.group(1)) else None
        tag = (m.group(2) or "").strip() or None
        semi = clean.find(";", e)
        out.append((tag, tdname, b + 1, e - 1, semi + 1 if semi >= 0 else e))
    fwd = {td: tag for _k, tag, td in TYPEDEF_FWD_RE.findall(clean)}
    return out, fwd


def find_type(tree, spec):
    """[(path, tag, typedef, body start, body end, def end)] for a type name, or for UNIT:Type."""
    unit, name = spec.split(":", 1) if ":" in spec else (None, spec)
    found = []
    files = tree.index()["headers"] + sorted(tree.index()["sources"])
    for p in files:
        text = tree.read(p) or ""
        if name not in text:
            continue
        defs, fwd = type_defs(text)
        for tag, td, b, e, end in defs:
            if td == name or tag == name or (name in fwd and tag == fwd[name]):
                if unit is None or file_unit(tree, p) == unit:
                    found.append((p, tag, td or (name if name in fwd else None), b, e, end))
    return found


def file_unit(tree, path):
    if path.endswith(".c"):
        return tree.index()["sources"].get(path, "main")
    return tree.header_owner(path)


def users_of(tree, path):
    """C files that see the definitions in `path`."""
    if path.endswith(".c"):
        return [path]
    return sorted(p for p, hs in tree.index()["closure"].items() if path in hs)


def plan_fields(ctx, rows, use_gcc=True):
    """Field rows of one batch (several types at once): the gcc oracle locates every use."""
    tree = ctx.tree
    batch = []      # (row, path, type names, member pos)
    for row in rows:
        if not ctx.merge:
            why = name_problem(row)
            if why:
                ctx.problem(row, why)
                continue
        defs = find_type(tree, row.scope)
        if len(defs) != 1:
            ctx.problem(row, "%s has %d definitions%s" % (row.scope, len(defs),
                        " (write UNIT:Type)" if len(defs) > 1 else ""))
            continue
        path, tag, td, b, e, end = defs[0]
        text = tree.read(path)
        clean = blank_comments(text)
        body = clean[b:e]
        hits = [b + s for k, s, x in tokens(body) if k == "id" and body[s:x] == row.old]
        news = [b + s for k, s, x in tokens(body) if k == "id" and body[s:x] == row.new]
        if news and not ctx.merge:
            ctx.problem(row, "%s already has a member %s" % (row.scope, row.new))
            continue
        if len(hits) != 1:
            if ctx.merge and not hits and (news or row.uses_only):
                batch.append((row, path, {tag, td} - {None}, None))
                continue
            ctx.problem(row, "%s has %d members named %s" % (row.scope, len(hits), row.old))
            continue
        batch.append((row, path, {tag, td} - {None}, hits[0]))
    if not batch:
        return
    users = sorted({u for _r, path, _t, _h in batch for u in users_of(tree, path)})
    scratch = Scratch(tree) if use_gcc else None
    try:
        base = scratch.errors(users) if scratch else []
        # the declarations, last first in each file (the offsets were taken before any edit)
        for row, path, _types, pos in sorted((x for x in batch if x[3] is not None), key=lambda x: (x[1], -x[3])):
            text = tree.read(path)
            assert text[pos:pos + len(row.old)] == row.old
            tree.write(path, text[:pos] + row.new + text[pos + len(row.old):])
        by_name = {}
        for row, _p, types, _h in batch:
            for t in types:
                by_name[(t, row.old)] = row
        located = {id(r): 0 for r, _p, _t, _h in batch}
        for _round in range(40):
            if not scratch:
                break
            spots = {}          # (file, offset) -> row, from this round's texts (patched after the round)
            for f, ln, col, msg, notes, _extra in scratch.errors(users):
                m = NO_MEMBER_RE.match(msg)
                if not m:
                    continue
                row = by_name.get((m.group(1), m.group(3))) or by_name.get((m.group(2), m.group(3)))
                if row is None:
                    continue
                for (pf, pl, pc) in [(f, ln, col)] + notes:
                    at = locate_member(tree.read(pf), pl, pc, row.old)
                    if at is not None:
                        spots[(pf, at)] = row
                        break
                else:
                    ctx.problem(row, "%s:%d:%d: gcc reports a use of %s that is not the token %s (token pasting?); "
                                     "fix it by hand" % (f, ln, col, row.old, row.old))
            patched = 0
            for pf in sorted({f for f, _a in spots}):
                text = tree.read(pf)
                for (_f, at), row in sorted(((k, r) for k, r in spots.items() if k[0] == pf), key=lambda kv: -kv[0][1]):
                    text = text[:at] + row.new + text[at + len(row.old):]
                    patched += 1
                    located[id(row)] += 1
                tree.write(pf, text)
                ctx.field_files.add(pf)
            if not patched:
                break
        if scratch:
            names = {r.old: r.new for r, _p, _t, _h in batch}
            after = scratch.errors(users)
            want = {norm_diag(d, names) for d in base}
            got = {norm_diag(d, {}) for d in after}
            for d in sorted(got - want)[:10]:
                ctx.problems.append("field oracle: new gcc error %s:%d: %s" % d)
        for row, path, types, _h in batch:
            n_c = rewrite_field_comments(ctx, row, path, types, users)
            if row.hypothesis and _h is not None:
                at = member_offset(tree, row.scope, row.new)
                if at is None:
                    ctx.note(row, "HYPOTHESIS mark not placed: %s has no single member %s" % (row.scope, row.new))
                else:
                    mark_hypothesis(tree, path, row.new, row.note, at=at)
            row.files = set(closure_files(tree, users_of(tree, path))) | {path}
            left = 0
            for u in users:
                text = tree.read(u)
                left += sum(1 for k, s, e in tokens(text) if k == "id" and text[s:e] == row.old and after_member(text, s))
            if left:
                ctx.note(row, "%d member uses named %s remain in the users of %s (other types, or code gcc does not "
                              "compile: #if 0, other #ifdef branches)" % (left, row.old, row.scope))
            ctx.report.append("%s: %d uses located by gcc in %d C files, %d comment mentions" % (
                row, located[id(row)], len(users), n_c))
            ctx.done.append(row)
    finally:
        if scratch:
            scratch.close()


def norm_diag(d, names):
    f, ln, _col, msg = d[0], d[1], d[2], d[3]
    for o, n in names.items():
        msg = msg.replace("'%s'" % o, "'%s'" % n)
    return (f, ln, msg)


def locate_member(text, line, col, old):
    """Offset of the member name after the `.`/`->` gcc points at (1-based line, byte column), or of
    the name itself; None when the token there is not `old`."""
    if text is None:
        return None
    starts = [0] + [m.end() for m in re.finditer(r"\n", text)]
    if line - 1 >= len(starts):
        return None
    i = starts[line - 1] + col - 1
    for rx in (r"(?:\.|->)\s*(\w+)", r"(\w+)"):
        m = re.compile(rx).match(text, i)
        if m and m.group(1) == old:
            return m.start(1)
    return None


def rewrite_field_comments(ctx, row, path, types, users):
    """Comment mentions `Type.old`, `Type->old` and `<aggregate base>.old`; returns the count."""
    tree = ctx.tree
    bases = set(types)
    agg = tree.read("config/migrate_globals.txt") or ""
    for line in agg.splitlines():
        parts = strip_comment(line).split()
        if len(parts) == 4 and parts[0] == "aggregate" and parts[2] in types:
            bases.add(parts[1])
    rx = re.compile(r"(?<![\w])((?:%s)(?:\.|->))%s(?![\w])" % ("|".join(map(re.escape, sorted(bases))), re.escape(row.old)))
    n = 0
    for f in sorted(set(users) | {path} | set(tree.closure(path) if path else ())):
        text = tree.read(f)
        if not text or row.old not in text:
            continue
        out, pos = [], 0
        for k, s, e in tokens(text):
            if k == "c" and row.old in text[s:e]:
                body, c = rx.subn(lambda m: m.group(1) + row.new, text[s:e])
                if c:
                    out += [text[pos:s], body]
                    pos, n = e, n + c
        out.append(text[pos:])
        new = "".join(out)
        if new != text:
            tree.write(f, new)
    return n


def member_offset(tree, spec, name):
    """Offset of the member `name` in the body of type `spec` (now), or None unless exactly one."""
    defs = find_type(tree, spec)
    if len(defs) != 1:
        return None
    path, _tag, _td, b, e, _end = defs[0]
    body = blank_comments(tree.read(path))[b:e]
    hits = [b + s for k, s, x in tokens(body) if k == "id" and body[s:x] == name]
    return hits[0] if len(hits) == 1 else None


def mark_hypothesis(tree, path, name, note, at=None):
    """Append `/* HYPOTHESIS: ... */` to the line at position `at`, or to the first line that names `name`."""
    text = tree.read(path)
    clean = blank_comments(text)
    if at is None:
        m = re.search(r"(?<![\w])%s(?![\w])" % re.escape(name), clean)
        if not m:
            return
        at = m.start()
    eol = text.find("\n", at)
    eol = len(text) if eol < 0 else eol
    body = re.sub(r"^HYPOTHESIS:?\s*", "", note, flags=re.I).strip() or "name inferred, not proven"
    if "HYPOTHESIS" in text[at:eol]:
        return
    tree.write(path, text[:eol] + " /* HYPOTHESIS: %s */" % body.replace("*/", "* /") + text[eol:])


# ======================================================================================= types

def plan_type(ctx, row):
    tree = ctx.tree
    if row.scope not in tree.unit_names:
        return ctx.problem(row, "unknown unit %s" % row.scope)
    defs = [d for d in find_type(tree, row.old) if file_unit(tree, d[0]) in (row.scope, "main")]
    if row.scope != "main":
        own = [d for d in defs if file_unit(tree, d[0]) == row.scope]
        defs = own or defs
    if len(defs) != 1:
        if ctx.merge and not defs and find_type(tree, row.new):
            defs = []
        else:
            return ctx.problem(row, "%s has %d definitions visible to %s" % (row.old, len(defs), row.scope))
    if not ctx.merge:
        why = name_problem(row)
        if why:
            return ctx.problem(row, why)
    path = defs[0][0] if defs else None
    unit = file_unit(tree, path) if path else row.scope
    if path and unit != row.scope:
        return ctx.problem(row, "%s is defined in %s (unit %s); use scope %s" % (row.old, path, unit, unit))
    users = users_of(tree, path) if path else tree.unit_sources(row.scope)
    files = closure_files(tree, users) + ([path] if path else [])
    files = sorted(set(files))
    row.files = set(files)      # the type's scope: name_map maps the old name only there (proofs 2 and 3)
    if not ctx.merge:
        clash = [f for f in files if row.new in ident_tokens(tree.read(f) or "", members=True)]
        if clash:
            return ctx.problem(row, "%s is already an identifier in %s" % (row.new, ", ".join(clash[:3])))
        for u in tree.unit_names if unit == "main" else [unit, "main"]:
            if row.new in ctx.table().all_names(u):
                return ctx.problem(row, "%s is a symbol name in %s" % (row.new, u))
    if unit != "main":
        shared = [f for f in files if not f.endswith(".c") and tree.header_owner(f) != unit
                  and row.old in ident_tokens(tree.read(f) or "")]
        if shared:
            return ctx.problem(row, "%s is private to %s but a shared header names it: %s" % (
                row.old, unit, ", ".join(shared)))
    n_tok = n_com = 0
    for f in files:
        text = tree.read(f)
        if text and row.old in text:
            new, a, b = rewrite_text(text, row.old, row.new, suffixes=False)
            tree.write(f, new)
            n_tok, n_com = n_tok + a, n_com + b
    p = "config/migrate_globals.txt"
    text = tree.read(p)
    if text and row.old in text:
        out = []
        for line in text.splitlines(True):
            parts = strip_comment(line).split()
            if len(parts) == 4 and parts[0] == "aggregate" and parts[2] == row.old:
                line = line.replace(" %s " % row.old, " %s " % row.new, 1)
            out.append(line)
        tree.write(p, "".join(out))
    if row.hypothesis and path:
        defs2 = [d for d in find_type(tree, row.new) if d[0] == path]
        if defs2:
            mark_hypothesis(tree, defs2[0][0], row.new, row.note, at=defs2[0][4] - 1)
    row.key = "-"
    ctx.report.append("%s: defined in %s, %d tokens and %d comment mentions in %d files" % (
        row, path, n_tok, n_com, len(files)))
    ctx.done.append(row)


# ======================================================================================= locals, params

DEF_HEAD_RE = r"^[A-Za-z_][\w \t*]*[ \t*]%s[ \t]*\("


def function_spans(text, name):
    """[(start, end, params end)] of the definitions of function `name` (ANSI or K&R) in a C text."""
    clean = blank_comments(text)
    out = []
    for m in re.finditer(DEF_HEAD_RE % re.escape(name), clean, re.M):
        i = m.end() - 1
        depth, j = 0, i
        while j < len(clean):
            if clean[j] == "(":
                depth += 1
            elif clean[j] == ")":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        k = clean.find("{", j + 1)
        between = clean[j + 1:k] if k >= 0 else ";"
        # nothing (ANSI) or K&R parameter declarations between `)` and `{`; a prototype ends in `;`
        if k < 0 or not re.match(r"^(?:\s*[A-Za-z_][^;{}()]*;)*\s*$", between) or re.match(r"^\s*;", between):
            continue
        out.append((m.start(), match_brace(clean, k), k))
    return out


def file_scope_names(tree, path):
    """Identifiers declared at file scope in a C file and its header closure: macros, typedefs, enum
    constants, globals and functions (tokens at brace depth 0 outside parentheses), tags."""
    out = set()
    for f in [path] + sorted(tree.closure(path)):
        text = blank_comments(tree.read(f) or "")
        out.update(re.findall(r"^[ \t]*#[ \t]*define[ \t]+(\w+)", text, re.M))
        for body in re.findall(r"\benum\s*\w*\s*\{([^}]*)\}", text):
            out.update(re.findall(r"(\w+)\s*(?:=[^,]*)?(?:,|$)", body.strip()))
        depth = paren = 0
        for m in re.finditer(r"^[ \t]*#[^\n]*|[A-Za-z_]\w*|[{}()]", text, re.M):
            t = m.group(0)
            if t == "{":
                depth += 1
            elif t == "}":
                depth = max(0, depth - 1)
            elif t == "(":
                paren += 1
            elif t == ")":
                paren = max(0, paren - 1)
            elif t.lstrip().startswith("#"):
                continue
            elif depth == 0 and paren == 0:
                out.add(t)
    return out - C_KEYWORDS


def plan_local(ctx, row):
    tree = ctx.tree
    if ":" not in row.scope:
        return ctx.problem(row, "scope must be UNIT:function")
    unit, func = row.scope.split(":", 1)
    if unit not in tree.unit_names:
        return ctx.problem(row, "unknown unit %s" % unit)
    func = current_function_name(ctx, unit, func)
    hits = []
    for p in tree.unit_sources(unit):
        text = tree.read(p)
        if func in text:
            hits += [(p, s) for s in function_spans(text, func)]
    if not hits:
        return ctx.problem(row, "no definition of %s in %s's C files" % (func, unit))
    if len({p for p, _s in hits}) != 1:
        return ctx.problem(row, "%s is defined in several files" % func)
    path = hits[0][0]
    text = tree.read(path)
    spans = [(a, b) for _p, (a, b, _k) in hits]
    in_body = set()
    for a, b in spans:
        in_body |= ident_tokens(text[a:b], members=False)
    if row.old not in in_body:
        if ctx.merge:
            return
        return ctx.problem(row, "%s does not occur in %s" % (row.old, func))
    params = set()
    for a, _b, k in [s for _p, s in hits]:
        head = text[a:k]
        params |= ident_tokens(head[head.find("("):], members=False)
    if row.kind == "param" and row.old not in params:
        return ctx.problem(row, "%s is not a parameter of %s" % (row.old, func))
    if row.kind == "local" and row.old in params:
        return ctx.problem(row, "%s is a parameter of %s; use kind param" % (row.old, func))
    if not ctx.merge:
        why = name_problem(row)
        if why:
            return ctx.problem(row, why)
        if row.new in in_body:
            return ctx.problem(row, "%s already occurs in %s" % (row.new, func))
        if row.new in file_scope_names(tree, path):
            return ctx.problem(row, "%s is a file-scope name (macro, type, global or function) for %s" % (row.new, path))
        for u in (unit, "main"):
            if row.new in ctx.table().all_names(u):
                return ctx.problem(row, "%s is a symbol name in %s" % (row.new, u))
    new, n_tok, n_com = rewrite_text(text, row.old, row.new, spans=spans, suffixes=False)
    tree.write(path, new)
    n_proto = 0
    if row.kind == "param":
        for h in sorted(tree.closure(path)):
            ht = tree.read(h)
            if func not in ht:
                continue
            clean = blank_comments(ht)
            pspans = []
            for m in re.finditer(r"(?<![\w])%s\s*\(" % re.escape(func), clean):
                end = clean.find(";", m.end())
                close = clean.rfind(")", m.end(), end)
                if end > 0 and close > 0 and "{" not in clean[m.end():end]:
                    pspans.append((m.end(), close))
            if pspans:
                nh, a, _b = rewrite_text(ht, row.old, row.new, spans=pspans, comments=False, suffixes=False)
                if nh != ht:
                    tree.write(h, nh)
                    n_proto += a
    row.key = "-"
    row.scope = "%s:%s" % (unit, func)
    ctx.report.append("%s: %d tokens, %d comment mentions in %s%s" % (
        row, n_tok, n_com, path, (", %d prototype names" % n_proto) if n_proto else ""))
    ctx.done.append(row)


def current_function_name(ctx, unit, name):
    """The name a function has now, through the ledger chains (local rows keep their old scope)."""
    for key, (names, _rows) in ctx.chains().items():
        if key[0] == "sym" and key[1] in (unit, "main") and name in names[:-1]:
            return names[-1]
    return name


# ======================================================================================= files

def object_range(tree, unit, start):
    """(start, end) of the original objects a C file starting at `start` holds (config/objects)."""
    path = "config/objects/%s.txt" % unit
    text = tree.read(path) or ""
    objs = [(int(m.group(1), 16), int(m.group(2), 16))
            for m in re.finditer(r"^object\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})", text, re.M)]
    starts = sorted(v for v, _n in tree.c_names(unit))
    nxt = next((v for v in starts if v > start), None)
    mine = [o for o in objs if o[0] >= start and (nxt is None or o[0] < nxt)]
    return (start, max(e for _s, e in mine)) if mine else (start, None)


def path_rewrite(tree, files, rx, repl, strings=True, comments=True, prefix_ok=None):
    """Rewrite regex matches inside string literals and/or comments of files; returns the count."""
    n = 0
    for f in files:
        text = tree.read(f)
        if not text:
            continue
        out, pos = [], 0
        for k, s, e in tokens(text):
            if (k == "s" and strings and (prefix_ok is None or prefix_ok(text[s:e]))) or (k == "c" and comments):
                body, c = rx.subn(repl, text[s:e])
                if c:
                    out += [text[pos:s], body]
                    pos, n = e, n + c
        out.append(text[pos:])
        new = "".join(out)
        if new != text:
            tree.write(f, new)
    return n


def config_line_rewrite(tree, path, rx, repl):
    text = tree.read(path)
    if not text:
        return 0
    new, c = rx.subn(repl, text)
    if c:
        tree.write(path, new)
    return c


def all_code_files(tree):
    return tree.listdir("src/**/*.c") + tree.listdir("include/**/*.h")


def config_text_files(tree):
    """config files with keys or comments that name files (not the yaml, not symbol files)."""
    return [p for p in ("config/toolchains.txt", "config/migrate_globals.txt", "config/proto_known.txt")
            if tree.exists(p)]


ISLAND_TYPES = ("c", ".rodata", ".data", ".bss")


def stem_of(name):
    return name.rsplit("/", 1)[-1]


def plan_file(ctx, row):
    """Rename the object `row.old` (an address stem or a name) of a unit to `row.new`. Replay (ctx.merge) also
    settles what a stale branch left under the old name when the yaml c line has the new name already or
    never had the old one: island lines, the C file at the old path, path strings."""
    tree = ctx.tree
    if row.scope == "include":
        return plan_header(ctx, row)
    if row.scope not in tree.unit_names:
        return ctx.problem(row, "unknown unit %s" % row.scope)
    unit = row.scope
    subs = tree.c_names(unit)
    mine = [(v, n) for v, n in subs if stem_of(n) == row.old]
    if len(mine) == 1:
        start, oname = mine[0]
        nname = "%s/%s" % (oname.rsplit("/", 1)[0], row.new)
    elif ctx.merge and not mine:
        # the object's c line has the new name, or another name of the chain; stale parts keep the old one
        here = [(v, n) for v, n in subs if "0x%08X" % v == row.key]
        if len(here) != 1:
            return
        start, cur = here[0]
        oname = "%s/%s" % (cur.rsplit("/", 1)[0], row.old)
        nname = "%s/%s" % (cur.rsplit("/", 1)[0], row.new)
    else:
        return ctx.problem(row, "%s has %d c subsegments named */%s" % (unit, len(mine), row.old))
    why = stem_problem(row.new)
    if why and not ctx.merge:
        return ctx.problem(row, why)
    if mine and any(n.lower() == nname.lower() for _v, n in subs):
        return ctx.problem(row, "%s exists already in %s (names are compared without case)" % (nname, unit))
    osrc, nsrc = tree.source_path(unit, oname), tree.source_path(unit, nname)
    have_old, have_new = tree.exists(osrc), tree.exists(nsrc)
    if ctx.merge:
        if have_old and have_new:
            return ctx.problem(row, "%s and %s both exist (a branch kept the old path); merge the code into %s by "
                                    "hand, delete %s, and replay again" % (osrc, nsrc, nsrc, osrc))
        if mine and not have_old and not have_new:
            return ctx.problem(row, "neither %s nor %s exists" % (osrc, nsrc))
        move = have_old
    else:
        if not have_old:
            return ctx.problem(row, "%s does not exist" % osrc)
        if have_new:
            return ctx.problem(row, "%s exists already" % nsrc)
        move = True
    # yaml: the c subsegment and its .rodata/.data islands (same name)
    y = tree.yaml_path(unit)
    out, n_yaml = [], 0
    for line in tree.read(y).splitlines(True):
        m = SUB_RE.match(line)
        if m and m.group(4) == oname and (m.group(2) or m.group(3)) in ISLAND_TYPES:
            line = line[:m.start(4)] + nname + line[m.end(4):]
            n_yaml += 1
        out.append(line)
    if n_yaml:
        tree.write(y, "".join(out))
    if move:
        tree.move(osrc, nsrc)
    asm_rx = re.compile(r"(?<=/)%s(?=[./\"])" % re.escape(oname))
    n_str = path_rewrite(tree, all_code_files(tree), asm_rx, nname, comments=False,
                         prefix_ok=lambda s: s.startswith('"asm/'))
    word = re.compile(r"(?<![\w/])((?:src/(?:ovl/)?)?)%s(?![\w])" % re.escape(oname))
    n_com = path_rewrite(tree, all_code_files(tree), word, lambda m: m.group(1) + nname, strings=False)
    n_cfg = 0
    for p in config_text_files(tree):
        n_cfg += config_line_rewrite(tree, p, word, lambda m: m.group(1) + nname)
    if ctx.merge and not (n_yaml or move or n_str or n_com or n_cfg):
        return                                  # renamed already
    lo, hi = object_range(tree, unit, start)
    desc = TICKET_RE.sub("", row.note).strip().rstrip(".")
    rng = "0x%08X-0x%08X" % (lo, hi) if hi else "0x%08X" % lo
    if move:
        first = "/* %s.c: %s%s (config/objects/%s.txt). */\n" % (
            row.new, (desc + ". Original object ") if desc else "original object ", rng, unit)
        text = tree.read(nsrc)
        if re.match(r"^/\* \w+\.c: .*\(config/objects/\w+\.txt\)\. \*/\n", text):
            text = text[text.index("\n") + 1:]
        tree.write(nsrc, first + text)
    row.key = "0x%08X" % start
    ctx.report.append("%s: %s -> %s (%s), %d yaml lines, %d asm path strings, %d comment mentions, %d config keys" % (
        row, osrc, nsrc, rng, n_yaml, n_str, n_com, n_cfg))
    ctx.done.append(row)
    tree._index = None


def plan_dir(ctx, row):
    """Move an overlay's folder src/ovl/<old> to src/ovl/<new>. Replay (ctx.merge) moves only what still uses the
    old folder: objects, island lines and C files a stale branch added or edited there."""
    tree = ctx.tree
    unit = row.scope
    if unit not in tree.unit_names or unit == "main":
        return ctx.problem(row, "dir rows rename an overlay's folder (src/ovl/<folder>)")
    subs = tree.c_names(unit)
    y = tree.yaml_path(unit)
    lines = tree.read(y).splitlines(True)
    prefix = row.old + "/"
    if ctx.merge:
        stale = [(v, n) for v, n in subs if n.startswith(prefix)]
        named = [m.group(4) for m in map(SUB_RE.match, lines) if m and m.group(4).startswith(prefix)]
        old_files = tree.listdir("src/ovl/%s/*.c" % row.old)
        if not (stale or named or old_files):
            return                              # renamed already (or never in this tree)
        stems = {n[len(prefix):] for n in named} | {os.path.basename(f)[:-2] for f in old_files}
        sources = {tree.source_path(unit, n) for _v, n in stale} | set(old_files)
    else:
        if not subs or any(n.split("/", 1)[0] != row.old for _v, n in subs if "/" in n):
            return ctx.problem(row, "the c subsegments of %s are not all in %s/" % (unit, row.old))
        stems = {n.split("/", 1)[1] for _v, n in subs if "/" in n}
        sources = {tree.source_path(unit, n) for _v, n in subs}
    why = stem_problem(row.new)
    if why and not ctx.merge:
        return ctx.problem(row, why)
    others = [u for u in tree.unit_names if u != unit and any(n.split("/", 1)[0].lower() == row.new.lower()
                                                               for _v, n in tree.c_names(u))]
    if others or (not ctx.merge and os.path.exists(os.path.join(tree.root, "src/ovl", row.new))):
        return ctx.problem(row, "src/ovl/%s is taken" % row.new)
    moves = []
    for src in sorted(sources):
        dst = "src/ovl/%s%s" % (row.new, src[len("src/ovl/") + len(row.old):])
        if ctx.merge and not tree.exists(src):
            if not tree.exists(dst):
                return ctx.problem(row, "neither %s nor %s exists" % (src, dst))
            continue
        if ctx.merge and tree.exists(dst):
            return ctx.problem(row, "%s and %s both exist (a branch kept the old path); merge the code into %s by "
                                    "hand, delete %s, and replay again" % (src, dst, dst, src))
        moves.append((src, dst))
    out, n_yaml = [], 0
    for line in lines:
        m = SUB_RE.match(line)
        if m and m.group(4).startswith(prefix):
            line = line[:m.start(4)] + row.new + m.group(4)[len(row.old):] + line[m.end(4):]
            n_yaml += 1
        out.append(line)
    tree.write(y, "".join(out))
    for src, dst in moves:
        tree.move(src, dst)
    asm_rx = re.compile(r"(?<=/%s/)(?:non)?matchings/%s(?=/)|(?<=/%s/)data/%s(?=/)" % (
        re.escape(unit), re.escape(row.old), re.escape(unit), re.escape(row.old)))
    n_str = path_rewrite(tree, all_code_files(tree), asm_rx, lambda m: m.group(0)[:-len(row.old)] + row.new,
                         comments=False, prefix_ok=lambda s: s.startswith('"asm/'))
    n_com = n_cfg = 0
    if stems:
        word = re.compile(r"(?<![\w/])((?:src/ovl/)?)%s/(%s)(?![\w])" % (
            re.escape(row.old), "|".join(map(re.escape, sorted(stems, key=len, reverse=True)))))
        n_com = path_rewrite(tree, all_code_files(tree), word, lambda m: m.group(1) + row.new + "/" + m.group(2),
                             strings=False)
        for p in config_text_files(tree):
            n_cfg += config_line_rewrite(tree, p, word, lambda m: m.group(1) + row.new + "/" + m.group(2))
    row.key = "-"
    ctx.report.append("%s: %d files moved to src/ovl/%s, %d yaml lines, %d asm path strings, %d comment mentions, "
                      "%d config keys" % (row, len(moves), row.new, n_yaml, n_str, n_com, n_cfg))
    ctx.done.append(row)
    tree._index = None


# headers that build tools find by name (tools/sync_protos.py, tools/check_headers.py); renaming
# them needs those tools changed first
TOOL_HEADERS = ("main_api.h", "game.h", "main_only.h", "common.h", "include_asm.h", "psx_types.h")


def guard_of(path):
    return re.sub(r"[^A-Za-z0-9]", "_", path).upper()


def plan_header(ctx, row):
    tree = ctx.tree
    old, new = row.old, row.new
    move = tree.exists("include/" + old)
    if not move and not ctx.merge:
        return ctx.problem(row, "include/%s does not exist" % old)
    # replay without the old header: it is renamed already (or has another name of the chain); a stale
    # branch may still include it by the old name
    if move:
        if os.path.basename(old) in TOOL_HEADERS:
            return ctx.problem(row, "tools find include/%s by name (tools/sync_protos.py, tools/check_headers.py); "
                                    "change them first" % old)
        why = stem_problem(new, header=True)
        if why and not ctx.merge:
            return ctx.problem(row, why)
        if old.startswith("ovl/") != new.startswith("ovl/"):
            return ctx.problem(row, "an overlay's own header stays in include/ovl/ (tools find it there)")
        if tree.exists("include/" + new) or any(h.lower() == ("include/" + new).lower()
                                                for h in tree.index()["headers"]):
            return ctx.problem(row, "include/%s exists already" % new)
        tree.move("include/" + old, "include/" + new)
    n_inc = 0
    for f in all_code_files(tree):
        text = tree.read(f)
        if old not in text:
            continue
        rel_old = os.path.relpath("include/" + old, os.path.dirname(f)) if f.startswith("include/") else None
        rel_new = os.path.relpath("include/" + new, os.path.dirname(f)) if f.startswith("include/") else None

        def sub(m):
            inc = m.group(2)
            if inc == old:
                return m.group(1) + new + '"'
            if rel_old and inc == rel_old:
                return m.group(1) + rel_new + '"'
            return m.group(0)
        nt, c = re.subn(r'^([ \t]*#[ \t]*include[ \t]*")([^"]+)"', sub, text, flags=re.M)
        if nt != text:
            tree.write(f, nt)
            n_inc += sum(1 for _ in re.finditer(r'^[ \t]*#[ \t]*include[ \t]*"', nt, re.M)) and c
    og, ng = guard_of(old), guard_of(new)
    text = tree.read("include/" + new) if move else None
    if text and re.search(r"^#ifndef %s\b" % og, text, re.M):
        text = re.sub(r"(?<![\w])%s(?![\w])" % og, ng, text)
        tree.write("include/" + new, text)
    word = re.compile(r"(?<![\w/])((?:include/)?)%s(?![\w])" % re.escape(old))
    n_com = path_rewrite(tree, all_code_files(tree), word, lambda m: m.group(1) + new, strings=False)
    n_cfg = sum(config_line_rewrite(tree, p, word, lambda m: m.group(1) + new) for p in config_text_files(tree))
    tools = []
    for p in sorted(glob.glob(os.path.join(tree.root, "tools", "*.py")) + glob.glob(os.path.join(tree.root, "configure.py"))):
        with open(p, encoding=ENC) as f:
            t = f.read()
        if ("include/" + old) in t or ('"%s"' % old) in t:
            tools.append(os.path.relpath(p, tree.root))
    if ctx.merge and not (move or n_inc or n_com or n_cfg):
        return                                  # renamed already
    for t in tools:
        ctx.note(row, "%s names include/%s (check that it still works)" % (t, old))
    row.key = "-"
    ctx.report.append("%s: %d #include lines, %d comment mentions, %d config keys" % (row, n_inc, n_com, n_cfg))
    ctx.done.append(row)
    tree._index = None


# ======================================================================================= constants

CMP_OPS = ("==", "!=", "<", ">", "<=", ">=")
BIT_OPS = ("&", "|", "^")


def parse_values(text, where):
    """[(name, value text, comment)] of a values file: `NAME VALUE [comment]` per line."""
    out = []
    for n, line in enumerate(text.splitlines(), 1):
        parts = strip_comment(line).split(None, 2)
        if not parts:
            continue
        if len(parts) < 2 or not UPPER_RE.match(parts[0]):
            raise RenameError("%s:%d: expected `NAME VALUE [comment]` with an UPPER_SNAKE name" % (where, n))
        int(parts[1], 0)
        out.append((parts[0], parts[1], parts[2] if len(parts) > 2 else ""))
    return out


def parse_site(spec):
    m = re.match(r"^(\w+):(\w+)\(arg(\d+)\)$", spec)
    if m:
        return ("arg", m.group(1), m.group(2), int(m.group(3)))
    m = re.match(r"^(\w+)\.(\w+)$", spec)
    if m:
        return ("member", m.group(1), m.group(2))
    if IDENT_RE.match(spec):
        return ("var", spec)
    raise RenameError("site %r: expected Type.field, a variable name or UNIT:function(argN)" % spec)


def literal_value(node):
    """int value of a pycparser integer Constant (or -Constant), else None."""
    from pycparser import c_ast
    neg = False
    if isinstance(node, c_ast.UnaryOp) and node.op == "-":
        neg, node = True, node.expr
    if isinstance(node, c_ast.Constant) and node.type in ("int", "unsigned int", "long int", "unsigned long int"):
        v = int(node.value.rstrip("uUlL"), 0)
        return (-v if neg else v), node
    return None


def site_matches(node, sites):
    from pycparser import c_ast
    while isinstance(node, c_ast.Cast):
        node = node.expr
    for s in sites:
        if s[0] == "member" and isinstance(node, c_ast.StructRef) and node.field.name == s[2]:
            return True
        if s[0] == "var" and isinstance(node, c_ast.ID) and node.name == s[1]:
            return True
    return False


def const_hits(ast, sites, ops):
    """[(Constant node, value)] tied to a site: comparisons, assignments, switch cases, call arguments."""
    from pycparser import c_ast
    hits = []

    class V(c_ast.NodeVisitor):
        def visit_BinaryOp(self, n):
            if n.op in ops:
                for a, b in ((n.left, n.right), (n.right, n.left)):
                    lv = literal_value(b)
                    if lv and site_matches(a, sites):
                        hits.append((lv[1], lv[0]))
            self.generic_visit(n)

        def visit_Assignment(self, n):
            if n.op == "=" or (n.op[:-1] in ops and n.op.endswith("=")):
                lv = literal_value(n.rvalue)
                if lv and site_matches(n.lvalue, sites):
                    hits.append((lv[1], lv[0]))
            self.generic_visit(n)

        def visit_Switch(self, n):
            if site_matches(n.cond, sites):
                def cases(x):
                    for c in x:
                        if isinstance(c, c_ast.Switch):
                            continue
                        if isinstance(c, c_ast.Case):
                            lv = literal_value(c.expr)
                            if lv:
                                hits.append((lv[1], lv[0]))
                        cases(c)
                cases(n.stmt)
            self.generic_visit(n)

        def visit_FuncCall(self, n):
            name = n.name.name if isinstance(n.name, c_ast.ID) else None
            for s in sites:
                if s[0] == "arg" and name == s[2] and n.args and s[3] < len(n.args.exprs):
                    lv = literal_value(n.args.exprs[s[3]])
                    if lv:
                        hits.append((lv[1], lv[0]))
            self.generic_visit(n)
    V().visit(ast)
    return hits


def const_plan(root, header, enum, defines, values, sites, note="", ledger=None, log=print):
    """(tree, report lines, problems) for one const run (nothing written)."""
    from pycparser import c_parser
    tree = Tree(root)
    problems, report = [], []
    hpath = header if header.startswith("include/") else "include/" + header
    if not tree.exists(hpath):
        return tree, report, ["%s does not exist" % hpath]
    if enum and not PASCAL_RE.match(enum):
        problems.append("%s: enum names are PascalCase" % enum)
    users = users_of(tree, hpath)
    files = closure_files(tree, users)
    taken = set()
    for f in files:
        taken |= ident_tokens(tree.read(f) or "", members=True)
    for name, _v, _c in values:
        if name in taken:
            problems.append("%s is already an identifier in the closure of %s" % (name, hpath))
    if enum and enum in taken:
        problems.append("%s is already an identifier in the closure of %s" % (enum, hpath))
    by_value = {}
    for name, v, _c in values:
        by_value.setdefault(int(v, 0), name)
    sites = [parse_site(s) for s in sites]
    for s in sites:
        if s[0] == "member":
            owners = [d for d in find_type(tree, s[1])]
            if len(owners) != 1:
                problems.append("site %s.%s: %s has %d definitions" % (s[1], s[2], s[1], len(owners)))
            n_types = 0
            for h in files:
                defs, _fwd = type_defs(tree.read(h) or "")
                text = blank_comments(tree.read(h) or "")
                for _t, _td, b, e, _end in defs:
                    if s[2] in ident_tokens(text[b:e]):
                        n_types += 1
            if n_types != 1:
                problems.append("site %s.%s: %d types in the closure have a member %s; the site would be ambiguous"
                                % (s[1], s[2], n_types, s[2]))
    if problems:
        return tree, report, problems
    words = {s[2] if s[0] in ("member", "arg") else s[1] for s in sites}
    cands = [u for u in users if any(w in (tree.read(u) or "") for w in words)]
    ops = CMP_OPS + (BIT_OPS if defines else ())
    edits = {}      # path -> [(start, end, name)]
    unresolved = []
    scratch = Scratch(tree)
    try:
        scratch.sync()
        for u in cands:
            pp = subprocess.run(["gcc", "-E", "-std=gnu89", "-DM2CTX", "-Iinclude", u], cwd=scratch.dir,
                                stdout=subprocess.PIPE, stderr=subprocess.DEVNULL).stdout.decode(ENC)
            try:
                ast = c_parser.CParser().parse(pp, u)
            except Exception as e:     # pycparser raises its own ParseError subclasses
                problems.append("%s: pycparser: %s" % (u, str(e)[:120]))
                continue
            for node, val in const_hits(ast, sites, ops):
                c = node.coord
                where = "%s:%s" % (c.file, c.line)
                if val not in by_value:
                    unresolved.append("%s: %s has no name in the values file" % (where, node.value))
                    continue
                text = tree.read(c.file)
                if text is None:
                    unresolved.append("%s: not a project file" % where)
                    continue
                starts = [0] + [m.end() for m in re.finditer(r"\n", text)]
                ls = starts[c.line - 1]
                le = text.find("\n", ls)
                le = len(text) if le < 0 else le
                line = text[ls:le]
                pos = None
                col = (c.column or 0) - 1
                if 0 <= col < len(line) and line.startswith(node.value, col):
                    pos = ls + col
                else:
                    found = [ls + s for k, s, e in tokens(line) if k == "n" and line[s:e] == node.value]
                    if len(found) == 1:
                        pos = found[0]
                if pos is None:
                    unresolved.append("%s: %s not found on its line (macro?)" % (where, node.value))
                    continue
                edits.setdefault(c.file, set()).add((pos, pos + len(node.value), by_value[val]))
    finally:
        scratch.close()
    n = 0
    for path, es in edits.items():
        text = tree.read(path)
        for s, e, name in sorted(es, reverse=True):
            text = text[:s] + name + text[e:]
            n += 1
        tree.write(path, text)
    block = const_block(enum, defines, values, note)
    htext = tree.read(hpath)
    k = htext.rfind("#endif")
    tree.write(hpath, (htext[:k] + block + "\n" + htext[k:]) if k >= 0 else (htext.rstrip("\n") + "\n\n" + block))
    for s in sites:
        if s[0] == "member" and enum:
            d = find_type(tree, s[1])[0]
            if file_unit(tree, d[0]) != file_unit(tree, hpath):
                report.append("%s.%s: no /* %s */ comment, the type is shared and the enum is %s's" % (
                    s[1], s[2], enum, file_unit(tree, hpath)))
                continue
            text = tree.read(d[0])
            body = blank_comments(text)[d[3]:d[4]]
            m = re.search(r"(?<![\w])%s(?![\w])" % re.escape(s[2]), body)
            if m:
                at = d[3] + m.start()
                eol = text.find("\n", at)
                if ("/* %s */" % enum) not in text[at:eol]:
                    tree.write(d[0], text[:eol] + " /* %s */" % enum + text[eol:])
    report.append("const %s: %d values, %d literals named in %d files, %d unresolved" % (
        enum or "defines", len(values), n, len(edits), len(unresolved)))
    report += unresolved[:50]
    return tree, report, problems


def const_block(enum, defines, values, note):
    head = ""
    if note:
        body = re.sub(r"^HYPOTHESIS:?\s*", "", note, flags=re.I) if note.upper().startswith("HYPOTHESIS") else note
        head = "/* %s%s */\n" % ("HYPOTHESIS: " if note.upper().startswith("HYPOTHESIS") else "", body)
    if defines:
        return head + "".join("#define %s %s%s\n" % (n, v, (" /* %s */" % c) if c else "") for n, v, c in values)
    last = len(values) - 1    # C89: no comma after the last enumerator
    return head + "enum %s {\n%s};\n" % (enum, "".join(
        "    %s = %s%s%s\n" % (n, v, "," if k < last else "", (" /* %s */" % c) if c else "")
        for k, (n, v, c) in enumerate(values)))


# ======================================================================================= driver

def run_rows(ctx, rows, use_gcc=True):
    """Plan every row in order into ctx.tree (nothing is written)."""
    kinds = {r.kind for r in rows}
    if not ctx.merge and kinds & {"file", "dir"} and kinds - {"file", "dir"}:
        ctx.problems.append("file and dir rows go in a table of their own (their moves get their own commit)")
        return
    if "const" in kinds:
        ctx.problems.append("const rows are written by `rename.py const`, not by a table")
        return
    i = 0
    while i < len(rows):
        row = rows[i]
        if row.kind == "field":
            j = i
            while j < len(rows) and rows[j].kind == "field":
                j += 1
            batch = rows[i:j]
            for r in batch:
                r.scope = current_type_name(ctx, r.scope)
            plan_fields(ctx, batch, use_gcc=use_gcc)
            i = j
            continue
        before = dict(ctx.tree.texts) if row.kind in IDENT_KINDS else None
        n_done = len(ctx.done)
        why = None if ctx.merge else ledger_reuse(ctx, row)
        if why:
            ctx.problem(row, why)
            i += 1
            continue
        if row.kind in ("func", "data"):
            plan_symbol(ctx, row)
        elif row.kind == "type":
            plan_type(ctx, row)
        elif row.kind in ("local", "param"):
            plan_local(ctx, row)
        elif row.kind == "file":
            plan_file(ctx, row)
        elif row.kind == "dir":
            plan_dir(ctx, row)
        if before is not None and len(ctx.done) > n_done and not ctx.merge:
            round_trip(ctx, row, before)
        i += 1


def ledger_reuse(ctx, row):
    """Why row.new may not be used: an earlier ledger chain of the same kind and scope had it."""
    if row.kind in ("func", "data"):
        return None             # plan_symbol checks names across all units
    kg = row_key(row)[:2]
    for key, (names, rs) in ctx.chains().items():
        if key[:2] == kg and row.new in names and names[-1] != row.old:
            return "%s was used before in the ledger (%s, %s)" % (row.new, " -> ".join(names), rs[0].where)
    return None


def current_type_name(ctx, spec):
    unit, name = spec.split(":", 1) if ":" in spec else (None, spec)
    for key, (names, _rows) in ctx.chains().items():
        if key[0] == "type" and (unit is None or key[1] == unit) and name in names[:-1]:
            name = names[-1]
    return "%s:%s" % (unit, name) if unit else name


def round_trip(ctx, row, before):
    """Proof 1: the inverse substitution gives the old text back, and no old token is left in scope."""
    tree = ctx.tree
    for p, text in tree.texts.items():
        if text is None or p.startswith("config/") or before.get(p, tree.orig.get(p)) == text:
            continue
        old = before.get(p, tree.orig.get(p)) or ""
        if re.search(r"(?<![A-Za-z0-9])%s(?![\w])" % re.escape(row.new), old):
            ctx.note(row, "%s: mentions %s already; round trip not checked" % (p, row.new))
            continue
        inv, _a, _b = rewrite_text(text, row.new, row.old, members=True)
        if inv != old:
            ctx.problem(row, "%s: the inverse rename does not give the old text back (not a pure substitution)" % p)


def affected_sources(ctx):
    """C files (after the moves) whose text or header closure changed."""
    tree = ctx.tree
    changed = set(tree.changed())
    out = set()
    for p, hs in tree.index()["closure"].items():
        if p in changed or hs & changed:
            out.add(p)
    return sorted(out)


def name_map(ctx, path=None):
    """{old: new} of the identifier rows that apply to `path` (symbol rows only inside their scope)."""
    out = {}
    for r in ctx.done:
        if r.kind in ("func", "data", "type", "field"):
            files = getattr(r, "files", None)
            if path is None or files is None or path in files:
                out[r.old] = r.new
    return out


def map_names(text, names):
    if not names:
        return text
    rx = re.compile(r"(?<![\w])(%s)(?![\w])" % "|".join(map(re.escape, sorted(names, key=len, reverse=True))))
    return rx.sub(lambda m: names[m.group(1)], text)


def gcc_proof(ctx):
    """Proof 2: the gcc error set of the affected C files is the same, names and paths mapped."""
    if not have_gcc():
        ctx.notes.append("gcc not found: proof 2 skipped (run inside Docker)")
        return
    tree = ctx.tree
    after_files = affected_sources(ctx)
    if not after_files:
        return
    back = {new: old for old, new in tree.moves}
    before_files = []
    for p in after_files:
        q = p
        while q in back:
            q = back[q]
        before_files.append(q)
    orig = Tree(ctx.root)
    s1, s2 = Scratch(orig), Scratch(tree)
    try:
        e1 = s1.errors(before_files)
        e2 = s2.errors(after_files)
    finally:
        s1.close()
        s2.close()
    fwd = dict(tree.moves)
    shift = {r_new: 1 for r_new in [p for _o, p in tree.moves if p.endswith(".c")]}

    def norm(d, mapped):
        f, ln, _c, msg = d[0], d[1], d[2], d[3]
        if mapped:
            msg = map_names(msg, name_map(ctx, f))
            while f in fwd:
                f = fwd[f]
            ln += shift.get(f, 0)
        return (f, ln, msg)
    want = {norm(d, True) for d in e1}
    got = {norm(d, False) for d in e2}
    for d in sorted(got - want)[:10]:
        ctx.problems.append("proof 2: new gcc error %s:%d: %s" % d)
    for d in sorted(want - got)[:10]:
        ctx.problems.append("proof 2: gcc error gone %s:%d: %s" % d)


def views_proof(ctx):
    """Proof 3: tools/sync_protos.py views; no C file sees another type for a symbol (names mapped)."""
    tree = ctx.tree
    if not os.path.exists(os.path.join(ctx.root, "include/main_api.h")):
        return
    import sync_protos as sp
    s2 = Scratch(tree, config=True)
    try:
        s2.sync()
        before = sp.snapshot(sp.Model(os.path.join(ctx.root, "include"), os.path.join(ctx.root, "src")))
        after = sp.snapshot(sp.Model(os.path.join(s2.dir, "include"), os.path.join(s2.dir, "src")))
    finally:
        s2.close()
    fwd = dict(tree.moves)
    mapped = {}
    for path, views in before.items():
        names = name_map(ctx, path)
        p = path
        while p in fwd:
            p = fwd[p]
        mapped[p] = {map_names(n, names): map_names(t, names) for n, t in views.items()}
    aliases = {r.new for r in ctx.done if getattr(r, "alias", False)}
    changes = [c for c in sp.view_changes(mapped, after)]
    for unit, name, kind, ta, tb, cls in changes:
        msg = "%s sees %s differently: %s (%s -> %s, %s)" % (unit, name, kind, ta, tb, cls)
        if name in aliases:
            ctx.notes.append("proof 3 (alias merge, the build decides): " + msg)
        elif len([p for p in ctx.problems if p.startswith("proof 3")]) < 10:
            ctx.problems.append("proof 3: " + msg)


def ledger_path(root, name):
    return os.path.join(root, LEDGER_DIR, name + ".txt")


LEDGER_HEAD = ("# Rename ledger (tools/rename.py, T-27002): kind scope old new key note, appended by `rename.py apply`.\n"
               "# Do not edit by hand; `rename.py replay` applies it to any tree, `rename.py check` keeps old names out.\n")


def append_ledger(root, name, rows):
    path = ledger_path(root, name)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    new = not os.path.exists(path)
    with open(path, "a", encoding=ENC) as f:
        if new:
            f.write(LEDGER_HEAD)
        for r in rows:
            f.write(r.ledger_line() + "\n")
    return os.path.relpath(path, root)


def default_ledger(ctx, rows):
    for r in ctx.done or rows:
        if r.kind == "field":
            defs = find_type(ctx.tree, r.scope)
            return file_unit(ctx.tree, defs[0][0]) if defs else "main"
        if r.kind == "file" and r.scope == "include":
            return "main"
        return r.unit
    return "main"


def plan_cmd(root, table_path, ledger=None, allow_alias=False, apply=False, verify=True, build=False, out=sys.stdout):
    with open(table_path, encoding=ENC) if table_path != "-" else sys.stdin as f:
        rows = parse_rows(f.read(), table_path)
    ctx = Context(root, allow_alias=allow_alias)
    try:
        chains(ctx.ledger)
    except RenameError as e:
        ctx.problems.append(str(e))
    run_rows(ctx, rows)
    if not ctx.problems and verify:
        gcc_proof(ctx)
        if apply:
            views_proof(ctx)
    table = ctx.table()
    ctx.problems += ["symbols: " + p for p in table.problems()]
    for line in ctx.report:
        out.write(line + "\n")
    if ctx.tree.moves:
        for o, n in ctx.tree.moves:
            out.write("move %s -> %s\n" % (o, n))
    changed = ctx.tree.changed()
    out.write("%d rows, %d files to change%s\n" % (len(ctx.done), len(changed), "" if apply else " (dry run)"))
    for n in ctx.notes[:40]:
        out.write("  note: %s\n" % n)
    if len(ctx.notes) > 40:
        out.write("  note: ... %d more\n" % (len(ctx.notes) - 40))
    for p in ctx.problems:
        out.write("PROBLEM: %s\n" % p)
    if ctx.problems:
        return 1
    if apply:
        name = ledger or default_ledger(ctx, rows)
        ctx.tree.flush()
        path = append_ledger(root, name, ctx.done)
        out.write("applied; ledger %s\n" % path)
        bad = check(root, out=out)
        if bad:
            out.write("apply: the edits and the ledger rows above are written although check failed; fix the "
                      "problems, or undo the edits (git checkout) and remove the new ledger lines\n")
            return 1
        if build:
            return run_build(root, out)
    return 0


def run_build(root, out=sys.stdout):
    """Clean build under nice -n 19 with 2 jobs; 0 when every sha1 check is OK."""
    n_units = len(symtab.read_units(root))
    cmd = "rm -rf asm build && python3 configure.py >/dev/null && nice -n 19 ninja -j2"
    p = subprocess.run(cmd, shell=True, cwd=root, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    text = p.stdout.decode(ENC, "replace")
    oks = len(re.findall(r": OK$", text, re.M))
    out.write("build: rc %d, %d of %d sha1 OK\n" % (p.returncode, oks, n_units))
    if p.returncode or oks != n_units:
        out.write("\n".join(text.splitlines()[-15:]) + "\n")
        return 1
    return 0


# ======================================================================================= check

def guard_suffixes(tok):
    out = []
    for i in range(1, len(tok)):
        if tok[i - 1] == "_" and GUARD_PREFIX_RE.match(tok[:i]) and GUARD_PREFIX_RE.match(tok[:i]).group(1) == tok[:i]:
            out.append(tok[i:])
    return out


def name_index(tree, files):
    """{identifier or guard-macro suffix: set(files)} over files (member names excluded)."""
    idx = {}
    for f in files:
        text = tree.read(f) or ""
        for k, s, e in tokens(text):
            if k != "id" or after_member(text, s):
                continue
            tok = text[s:e]
            idx.setdefault(tok, set()).add(f)
            if "_" in tok and tok[0].isupper():
                for suf in guard_suffixes(tok):
                    idx.setdefault(suf, set()).add(f)
    return idx


def check(root, out=sys.stdout):
    """Problems: old ledger names back in src/, include/, config/; symbol file consistency."""
    tree = Tree(root)
    rows = read_ledger(root)
    problems = []
    try:
        ch = chains(rows)
    except RenameError as e:
        problems.append(str(e))
        ch = {}
    table = symtab.SymbolTable(root)
    problems += ["symbols: " + p for p in table.problems()]
    if ch:
        files = all_code_files(tree)
        idx = name_index(tree, files)
        for key, (names, rs) in sorted(ch.items(), key=lambda kv: str(kv[0])):
            final, olds = names[-1], names[:-1]
            where = rs[-1].where
            for o in olds:
                hits = old_name_hits(tree, ch, key, o, files, idx, table)
                for h in hits[:3]:
                    problems.append("%s: %s was renamed to %s (%s); run tools/rename.py replay" % (h, o, final, where))
    for p in problems:
        out.write(p + "\n")
    if not problems:
        out.write("renames OK (%d ledger rows)\n" % len(rows))
    return len(problems)


def old_name_hits(tree, ch, key, o, files, idx, table):
    """Where the old name `o` of ledger chain `key` still is, inside the chain's scope ([] when nowhere).
    `files`: every code file; `idx`: name_index of them. Used by check, and by replay to pick its rows."""
    hits = []
    kind = key[0]
    if kind == "sym":
        unit, addr = key[1], int(key[2], 16)
        scope = set(scope_files(tree, unit, addr))
        hits = sorted(idx.get(o, set()) & scope)
        if o in table.names_at(unit, addr):
            hits.append("the symbol files")
        hits += config_key_hits(tree, o, set(scope_units_for(tree, unit, addr)), unit)
    elif kind == "type":
        unit = key[1]
        scope = set(files) if unit == "main" else set(tree.unit_sources(unit) + tree.private_headers(unit))
        hits = sorted(idx.get(o, set()) & scope)
    elif kind == "field":
        tname = current_type_name_rows(ch, key[1])
        for d in find_type(tree, tname):
            body = blank_comments(tree.read(d[0]))[d[3]:d[4]]
            if o in ident_tokens(body):
                hits.append("%s (definition of %s)" % (d[0], tname))
    elif kind == "file":
        # the object's c line or an island of it named `o`, or its C file at the old path
        unit = key[1]
        subs = tree.subsegments(unit)
        for folder in sorted({n.rsplit("/", 1)[0] for v, t, n in subs if t == "c" and "0x%08X" % v == key[2]}):
            oname = "%s/%s" % (folder, o)
            if any(n == oname and t in ISLAND_TYPES for _v, t, n in subs):
                hits.append(tree.yaml_path(unit))
            if tree.exists(tree.source_path(unit, oname)):
                hits.append(tree.source_path(unit, oname))
    elif kind == "dir":
        # a yaml line (c or island) in the old folder, or a C file there
        unit = key[1]
        if any(n.startswith(o + "/") for _v, _t, n in tree.subsegments(unit)):
            hits.append(tree.yaml_path(unit))
        hits += tree.listdir("src/ovl/%s/*.c" % o)
    elif kind == "header":
        if tree.exists("include/" + o):
            hits.append("include/" + o)
        hits += sorted(f for f in files if re.search(r'^[ \t]*#[ \t]*include[ \t]*"%s"' % re.escape(o),
                                                     tree.read(f) or "", re.M))
    elif kind == "local":
        unit, func = key[1].split(":", 1)
        for fn in func_chain_names(ch, unit, func):     # replay looks before the function is renamed
            for p in tree.unit_sources(unit):
                text = tree.read(p)
                if fn in text:
                    for a, b, _k in function_spans(text, fn):
                        if o in ident_tokens(text[a:b]):
                            hits.append("%s (%s)" % (p, fn))
    return hits


def func_chain_names(ch, unit, name):
    """Every name of the function chain that holds `name`, the current name first ([name] without one)."""
    for key, (names, _r) in ch.items():
        if key[0] == "sym" and key[1] in (unit, "main") and name in names:
            return names[::-1]
    return [name]


def config_key_hits(tree, name, units, owner):
    """config files whose keys name `name` as seen from `units` (proto_known `UNIT:name`,
    migrate_globals symbols and proofs of files of those units)."""
    out = []
    text = tree.read("config/proto_known.txt") or ""
    if any(m.group(1) in units for m in re.finditer(r"\b(\w+):%s\b" % re.escape(name), strip_hash_comments(text))):
        out.append("config/proto_known.txt")
    for line in strip_hash_comments(tree.read("config/migrate_globals.txt") or "").splitlines():
        parts = line.split()
        if not parts or not re.search(r"(?<![\w])%s(?![\w])" % re.escape(name), line):
            continue
        unit = "main" if parts[0] == "aggregate" else srcscan_unit(tree, parts[1]) if len(parts) > 1 else None
        if (owner == "main" and parts[0] == "aggregate") or unit in units:
            out.append("config/migrate_globals.txt")
            break
    return out


def strip_hash_comments(text):
    return "\n".join(strip_comment(l) for l in text.splitlines())


def current_type_name_rows(ch, spec):
    unit, name = spec.split(":", 1) if ":" in spec else (None, spec)
    for key, (names, _r) in ch.items():
        if key[0] == "type" and (unit is None or key[1] == unit) and name in names[:-1]:
            name = names[-1]
    return "%s:%s" % (unit, name) if unit else name


# ======================================================================================= replay

def replay(root, files=None, out=sys.stdout, use_gcc=True):
    """Apply every ledger chain again: each earlier name that is still present becomes the last name."""
    rows = read_ledger(root)
    if files:
        keep = {os.path.relpath(os.path.abspath(f), os.path.abspath(root)) for f in files}
        sel = [r for r in rows if r.where.split(":")[0] in keep]
    else:
        sel = rows
    ch = chains(rows)
    ctx = Context(root, ledger_rows=rows, merge=True)
    tree = ctx.tree
    files = all_code_files(tree)
    idx = name_index(tree, files)
    table = ctx.table()
    sel_where = {r.where for r in sel}
    todo = []
    for key, (names, rs) in ch.items():
        if not any(r.where in sel_where for r in rs):
            continue
        final = names[-1]
        # moves of files, folders and headers: the newest earlier name first, so a stale path of the oldest name
        # meets the file of the current name and not a half-renamed one
        olds = names[:-1][::-1] if key[0] in ("file", "dir", "header") else names[:-1]
        for o in olds:
            r0 = next(r for r in rs if r.old == o)
            row = Row(r0.kind, r0.scope, o, final, r0.note, r0.where, r0.key)
            # symbols, types, locals: the old name still in the chain's scope (as check sees it); fields are
            # picked below, file, folder and header rows always run (they no-op on a renamed tree)
            if key[0] in ("sym", "type", "local"):
                present = bool(old_name_hits(tree, ch, key, o, files, idx, table))
            else:
                present = True
            if present:
                todo.append(row)
    fields = [r for r in todo if r.kind == "field"]
    # symbols before the locals and params of their functions (a local row found through a later function
    # rename needs the function renamed first), types before fields, file and folder moves last
    order = {"func": 0, "data": 0, "type": 1, "local": 2, "param": 2}
    others = sorted((r for r in todo if r.kind != "field"), key=lambda r: order.get(r.kind, 3))
    run_rows(ctx, others, use_gcc=use_gcc)
    if fields:
        need = []
        members = set()
        for u in list(tree.index()["sources"]) + tree.index()["headers"]:
            members |= ident_tokens(tree.read(u) or "", members=True)
        groups = {}
        for r in fields:
            r.scope = current_type_name(ctx, r.scope)
            if r.old in members:
                groups.setdefault((r.scope, r.new), []).append(r)
        for (scope, final), rs in groups.items():
            need += settle_field_chain(ctx, scope, final, rs)
        if need:
            plan_fields(ctx, need, use_gcc=use_gcc)
    for line in ctx.report:
        out.write(line + "\n")
    for p in ctx.problems:
        out.write("PROBLEM: %s\n" % p)
    if ctx.problems:
        return 1
    ctx.tree.flush()
    out.write("replay: %d rows run (old name still in scope; %d chains), %d files changed\n" % (
        len(ctx.done), len(ch), len(ctx.tree.changed())))
    return 0


def settle_field_chain(ctx, scope, final, rows):
    """The rows of one member chain for replay (one row per earlier name still in use, all to `final`).
    The type's definition holds one name of the chain (or `final` already): the row of the name it holds
    renames the declaration, the others only follow the uses gcc reports (`uses_only`), so the chain is
    walked from whatever name the tree has. Returns the rows to plan."""
    tree = ctx.tree
    defs = find_type(tree, scope)
    if len(defs) != 1:
        return rows                         # plan_fields reports it
    path, _tag, _td, b, e, _end = defs[0]
    body = blank_comments(tree.read(path))[b:e]
    held = [r for r in rows if any(k == "id" and body[s:x] == r.old for k, s, x in tokens(body))]
    has_final = any(k == "id" and body[s:x] == final for k, s, x in tokens(body))
    if len(held) > 1 or (held and has_final):
        names = ", ".join(sorted({r.old for r in held} | ({final} if has_final else set())))
        ctx.problem(held[0], "%s declares %s: replay cannot tell which one the ledger chain renamed" % (scope, names))
        return []
    if held or has_final:
        for r in rows:
            r.uses_only = r not in held
    return rows


# ======================================================================================= lookup, export

def lookup(root, name, unit=None, out=sys.stdout):
    found = 0
    for key, (names, rs) in sorted(chains(read_ledger(root)).items(), key=lambda kv: str(kv[0])):
        if name in names and (unit is None or key[1].split(":")[0] == unit):
            found += 1
            out.write("%s %s %s: %s (current: %s)\n" % (key[0], key[1], key[2], " -> ".join(names), names[-1]))
            for r in rs:
                out.write("  %s %s -> %s %s\n" % (r.where, r.old, r.new, r.note))
    if not found:
        out.write("%s: not in the ledger\n" % name)
    return 0 if found else 1


def export(root, out=sys.stdout):
    """Tab separated: kind, scope, key, old name, current name (one line per earlier name)."""
    for key, (names, rs) in sorted(chains(read_ledger(root)).items(), key=lambda kv: str(kv[0])):
        for o in names[:-1]:
            r = next(x for x in rs if x.old == o)
            out.write("\t".join([r.kind, r.scope, r.key, o, names[-1]]) + "\n")
    return 0


# ======================================================================================= main

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--root", default=".")
    sub = ap.add_subparsers(dest="cmd", required=True)
    for name in ("plan", "apply"):
        p = sub.add_parser(name)
        p.add_argument("table")
        p.add_argument("--ledger", help="ledger file name under config/renames/ (default: the first row's unit)")
        p.add_argument("--allow-alias", action="store_true", help="the new name already names the address")
        if name == "apply":
            p.add_argument("--no-verify", action="store_true", help="skip proofs 2 and 3 (gcc, views)")
            p.add_argument("--build", action="store_true", help="then the clean build (nice -n 19, 2 jobs)")
        else:
            p.add_argument("--no-verify", action="store_true", help="skip proof 2 (gcc)")
    sub.add_parser("check")
    p = sub.add_parser("replay")
    p.add_argument("ledgers", nargs="*")
    p = sub.add_parser("lookup")
    p.add_argument("name")
    p.add_argument("--unit")
    sub.add_parser("export")
    p = sub.add_parser("const")
    p.add_argument("--header", required=True)
    g = p.add_mutually_exclusive_group(required=True)
    g.add_argument("--enum")
    g.add_argument("--defines", action="store_true")
    p.add_argument("--values", required=True)
    p.add_argument("--site", action="append", required=True)
    p.add_argument("--note", default="")
    p.add_argument("--ledger")
    p.add_argument("--apply", action="store_true")
    p.add_argument("--stamp", help=argparse.SUPPRESS)
    a = ap.parse_args(argv)
    try:
        if a.cmd in ("plan", "apply"):
            return plan_cmd(a.root, a.table, a.ledger, a.allow_alias, apply=a.cmd == "apply",
                            verify=not a.no_verify, build=getattr(a, "build", False))
        if a.cmd == "check":
            return int(bool(check(a.root)))
        if a.cmd == "replay":
            return replay(a.root, a.ledgers)
        if a.cmd == "lookup":
            return lookup(a.root, a.name, a.unit)
        if a.cmd == "export":
            return export(a.root)
        if a.cmd == "const":
            with open(a.values, encoding=ENC) as f:
                values = parse_values(f.read(), a.values)
            tree, report, problems = const_plan(a.root, a.header, a.enum, a.defines, values, a.site, a.note)
            for line in report:
                print(line)
            for p in problems:
                print("PROBLEM: %s" % p)
            if problems:
                return 1
            if a.apply:
                tree.flush()
                h = a.header if a.header.startswith("include/") else "include/" + a.header
                row = Row("const", tree.header_owner(h), a.enum or "defines", h, a.note, key="-")
                print("applied; ledger %s" % append_ledger(a.root, a.ledger or row.scope, [row]))
            return 0
    except RenameError as e:
        print("rename.py: %s" % e, file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
