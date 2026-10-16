#!/usr/bin/env python3
"""Replace the prototype of one or more functions for one compile (T-16530).

    --proto 'void func_80012345(s32 a0);'        (repeatable; used by tools/regdiff.py and tools/funcdiff.py)

Matching often turns on how the file declares a callee: `void`, K&R `s32 f();` (the implicit-int view of
T-16210), `s32 f(s32 a0);`. Trying one means a scratch copy of the C file and of the header, or a
`MAIN_API_OVERRIDE_<symbol>` define plus a declaration (CODING_STANDARDS.md section 8a). This module does the same
to the preprocessed translation unit, in memory, so that nothing under src/ or include/ is touched:

  - every top-level declaration of the function in the preprocessed unit (the header's and the file's own) is
    removed, and the given declaration is written where the first mention of the function was (the removed
    declaration's place, or in front of the first function that calls it);
  - a function that the unit defines is refused (its definition fixes the type; use a scratch copy and --src);
  - a name that does not occur in the unit is refused (a typo would otherwise pass silently);
  - only function declarations (`ret name(args);`, `name();` for implicit int) are accepted, not data.

The result of the override is what the compiler sees; it is not a match until the same view is in the tree
(`commit_hint` names the `*_OVERRIDE_<symbol>` guard to define). No game data is needed for the unit tests
(tools/test_protoverride.py).
"""
import re
from collections import namedtuple
from pathlib import Path

IDENT = r"[A-Za-z_]\w*"
HEAD_OK = re.compile(r"^[\w\s*]*$")
KR_HEAD = re.compile(r"^[^(){};=]*?\b%s\s*\(\s*%s(?:\s*,\s*%s)*\s*\)\s*[A-Za-z_]" % (IDENT, IDENT, IDENT))

Proto = namedtuple("Proto", "name decl")


class ProtoError(ValueError):
    """A --proto argument that cannot be parsed, or cannot be applied to the translation unit."""


# ---------------------------------------------------------------------------------------------
# declarations

def _declarator(text):
    """(name, name_start, after) for the first `ident(` of a statement, `after` being the index after
    its matching ')'; None when the statement has no such pair (a variable, a function pointer)."""
    p = text.find("(")
    if p < 0:
        return None
    m = re.search(r"(%s)\s*$" % IDENT, text[:p])
    if not m:
        return None
    depth = 0
    for k in range(p, len(text)):
        if text[k] == "(":
            depth += 1
        elif text[k] == ")":
            depth -= 1
            if depth == 0:
                return m.group(1), m.start(1), k + 1
    return None


def _split_statements(spec):
    """The `;`-terminated statements of one --proto argument (several may share an argument)."""
    out, depth, cur = [], 0, []
    for ch in spec:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == ";" and depth == 0:
            out.append("".join(cur))
            cur = []
        else:
            cur.append(ch)
    tail = "".join(cur)
    if tail.strip():
        out.append(tail)
    return out


def parse(specs):
    """[Proto] from the --proto arguments. Raises ProtoError for anything that is not a plain function
    declaration, or for two declarations of one function."""
    protos, seen = [], set()
    for spec in specs or ():
        count = len(protos)
        for stmt in _split_statements(spec):
            text = " ".join(stmt.split())
            if not text:
                continue
            if any(c in text for c in "{}=") or re.match(r"typedef\b", text):
                raise ProtoError("--proto %r: expected a function declaration like 'void func_80012345(s32 a0);'" % spec)
            d = _declarator(text)
            if d is None or text[d[2]:].strip():
                raise ProtoError("--proto %r: expected a function declaration like 'void func_80012345(s32 a0);'" % spec)
            name, start, _after = d
            if not HEAD_OK.match(text[:start]):
                raise ProtoError("--proto %r: the part before the name must be a plain type (no function pointers or "
                                 "several declarators)" % spec)
            if name in seen:
                raise ProtoError("--proto: %s is declared twice" % name)
            seen.add(name)
            protos.append(Proto(name, text + ";"))
        if len(protos) == count:
            raise ProtoError("--proto %r: empty declaration" % spec)
    return protos


def describe(protos):
    return "; ".join(p.decl for p in protos)


def add_argument(ap):
    """The shared --proto option of regdiff.py and funcdiff.py."""
    ap.add_argument("--proto", action="append", default=[], metavar="DECL",
                    help="replace the declaration of a function for this compile only, e.g. --proto "
                         "'void func_80012345(s32 a0);' (repeatable; no file is edited; the function must not be "
                         "defined in the file; T-16530, wiki/decompile-workflow.md)")


# ---------------------------------------------------------------------------------------------
# the preprocessed unit

def _skip_literal(text, i):
    """Index after the string or character literal that starts at text[i]."""
    quote = text[i]
    i += 1
    while i < len(text) and text[i] != quote and text[i] != "\n":
        i += 2 if text[i] == "\\" else 1
    return i + 1


def _line_end(text, i):
    j = text.find("\n", i)
    return len(text) if j < 0 else j


def _spans(text):
    """Top-level statements of preprocessed C as (start, end, body): `body` is True for a function
    definition, whose span ends at its closing brace (a K&R definition includes its parameter
    declarations). Anything else ends at its `;`. Leftover `#` lines (pragmas) belong to no statement."""
    spans = []
    n = len(text)
    i = depth = 0
    start = head_end = None
    while i < n:
        ch = text[i]
        if start is None:
            if ch.isspace():
                i += 1
                continue
            if ch == "#":
                i = _line_end(text, i)
                continue
            start, head_end = i, None
        if ch in "\"'":
            i = _skip_literal(text, i)
            continue
        if ch == "{":
            if depth == 0 and head_end is None:
                head_end = i
            depth += 1
        elif ch == "}" and depth > 0:
            depth -= 1
            if depth == 0:
                head = text[start:head_end].strip()
                if not head or head.endswith(")"):
                    spans.append([start, i + 1, True if head else "kr"])
                    start = None
        elif ch == ";" and depth == 0:
            spans.append([start, i + 1, False])
            start = None
        i += 1
    if start is not None:
        spans.append([start, n, False])
    return _merge_kr(text, spans)


def _merge_kr(text, spans):
    """Join a K&R definition `f(a, b) s32 a; s32 b; { ... }`, scanned as several statements and a
    headless body, back into one definition span."""
    out = []
    for sp in spans:
        if sp[2] == "kr":
            j = len(out) - 1
            while j >= 0 and "(" not in text[out[j][0]:out[j][1]]:
                j -= 1
            if j >= 0 and not out[j][2] and KR_HEAD.match(text[out[j][0]:out[j][1]]):
                sp = [out[j][0], sp[1], True]
                del out[j:]
            else:
                sp[2] = True
        out.append(sp)
    return out


def _find_declarations(text, spans, name):
    """(declaration spans of `name`, whether the unit defines it). A statement that declares `name`
    together with other declarators is refused: removing it would drop them too."""
    decls, defined = [], False
    for start, end, body in spans:
        stmt = text[start:end]
        if re.match(r"\s*typedef\b", stmt):
            continue
        head = stmt[:stmt.find("{")] if body else stmt
        d = _declarator(head)
        if d is None or d[0] != name:
            continue
        if body:
            defined = True
            continue
        if "," in stmt[:d[1]] or stmt[d[2]:].strip() != ";":
            raise ProtoError("the declaration of %s shares a statement with other declarators (%s); "
                             "split it in a scratch copy" % (name, " ".join(stmt.split())[:80]))
        decls.append((start, end))
    return decls, defined


def apply(text, protos):
    """(new text, notes) with each prototype in place of the unit's declarations of that function."""
    notes = []
    for proto in protos:
        name = proto.name
        first = re.search(r"\b%s\b" % re.escape(name), text)
        if first is None:
            raise ProtoError("%s does not occur in the preprocessed translation unit; check the name (a prototype "
                             "override changes how this file declares and calls a function)" % name)
        spans = _spans(text)
        decls, defined = _find_declarations(text, spans, name)
        if defined:
            raise ProtoError("%s is defined in this file; --proto replaces the declaration of a function the file "
                             "only calls. Change the definition in a scratch copy and compare that (tools/regdiff.py --src COPY)." % name)
        at = next((s for s, e, _b in spans if s <= first.start() < e), None)
        if at is None:
            at = text.rfind("\n", 0, first.start()) + 1
        old = [" ".join(text[s:e].split()) for s, e in decls]
        edits = [(s, e, "") for s, e in decls]
        if any(s == at for s, _e in decls):
            edits = [(s, e, proto.decl if s == at else "") for s, e, _ in edits]
        else:
            edits.append((at, at, proto.decl + "\n"))
        for s, e, repl in sorted(edits, reverse=True):
            text = text[:s] + repl + text[e:]
        line = text.count("\n", 0, at) + 1
        notes.append("proto override %s: %s%s" % (
            name, proto.decl,
            "  (replaces %s)" % " ".join(o if len(o) <= 90 else o[:87] + "..." for o in old) if old else
            "  (no declaration in the unit; implicit before; placed before the first use, line %d)" % line))
    return text, notes


# ---------------------------------------------------------------------------------------------
# keeping an override

def override_macros(name, include_dir="include"):
    """The `*_OVERRIDE_<name>` guards that the headers under include_dir already wrap `name` in."""
    pat = re.compile(r"^[ \t]*#[ \t]*ifndef[ \t]+(\w*_OVERRIDE_%s)[ \t]*$" % re.escape(name), re.M)
    found = set()
    for path in Path(include_dir).rglob("*.h"):
        try:
            found.update(pat.findall(path.read_text(errors="replace")))
        except OSError:
            pass
    return sorted(found)


def commit_hint(protos, include_dir="include"):
    """Lines that say how to keep the overridden views in the tree (CODING_STANDARDS.md section 8a)."""
    lines = []
    for proto in protos:
        macros = override_macros(proto.name, include_dir)
        if macros:
            lines.append("to keep %s: put '#define %s /* reason */' before the first include of the .c file and "
                         "'%s' next to its other declarations (CODING_STANDARDS.md 8a)"
                         % (proto.name, macros[0], proto.decl))
        else:
            lines.append("to keep %s: no *_OVERRIDE_%s guard in include/ yet; wrap its declaration in the header "
                         "(CODING_STANDARDS.md 8a), then define the guard and declare '%s' in the .c file"
                         % (proto.name, proto.name, proto.decl))
    return lines
