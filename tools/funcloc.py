"""Find the C file that holds a function: its definition or its INCLUDE_ASM, never a mention (T-5030).

Used by tools/funcdiff.py and tools/permute.py. All overlays load at 0x80132000, so a name such as
`func_8013xxxx` is a different function in several overlays, and callers mention names of other
files; picking "the first file that contains the name" gave the wrong object (false MATCH, "missing
in built"). The rule here:

  - a C file holds `name` when it has `INCLUDE_ASM(..., name)` or a C definition of it (ANSI or
    K&R, also inside `#ifdef NON_MATCHING`); calls and declarations do not count;
  - the scope is a unit (`main` or an overlay name) or one C file: given as `UNIT:name`, as a unit
    name, or as a path (src/ovl/<NAME>.c, src/ovl/<NAME>/<addr>.c, src/main/<addr>.c, asm/ovl/<NAME>/...);
  - without a scope the name must have exactly one holder in the whole project; otherwise
    AmbiguousError lists the candidates and the way to pick one.
"""
import os
import re
from pathlib import Path

import srcscan


class LocateError(LookupError):
    """No C file holds the function, or the scope names no known unit or file."""


class AmbiguousError(LocateError):
    """Several C files hold the function and nothing says which one is meant."""


def unit_of_path(path):
    """Unit (`main` or an overlay name) a source or asm path belongs to, or None."""
    p = str(path).replace(os.sep, "/")
    m = re.search(r"(?:^|/)(?:src|asm)/ovl/([^/.]+)", p)
    if m:
        return m.group(1)
    if re.search(r"(?:^|/)src/main/|(?:^|/)asm/(?:non)?matchings/main/|(?:^|/)asm/data/", p):
        return "main"
    return None


_index_cache = {}


def index(root="."):
    """{name: [(CFile, defined)]}: every function an INCLUDE_ASM or a C definition names, per C file.
    `defined` is True for a C definition. Cached per root and per source mtime signature."""
    files = [c for c in srcscan.c_files(root) if c.src.exists()]
    sig = tuple((str(c.src), c.src.stat().st_mtime_ns) for c in files)
    hit = _index_cache.get(os.path.abspath(root))
    if hit and hit[0] == sig:
        return hit[1]
    out = {}
    for c in files:
        text = c.src.read_text(errors="replace")
        for name in {n for _f, n in srcscan.INCLUDE_ASM_RE.findall(text)}:
            out.setdefault(name, []).append((c, False))
        for name in set(srcscan.DEF_RE.findall(text)) | set(srcscan.KR_DEF_RE.findall(text)):
            out.setdefault(name, []).append((c, True))
    _index_cache[os.path.abspath(root)] = (sig, out)
    return out


def split_scope(name, scope=None):
    """(bare name, scope): a `UNIT:name` prefix is a scope; it must agree with an explicit one."""
    if ":" in name and "/" not in name:
        prefix, name = name.split(":", 1)
        if scope and scope != prefix and unit_of_path(scope) != prefix:
            raise LocateError("conflicting scopes: %s: prefix and --unit %s" % (prefix, scope))
        scope = scope or prefix
    return name, scope


def candidates(name, scope=None, root=".", defined_only=False):
    """[CFile] that hold `name` within `scope` (unit name or C/asm path, None for everywhere)."""
    name, scope = split_scope(name, scope)
    hits = []
    for c, d in index(root).get(name, []):   # INCLUDE_ASM under #else and a body: one file
        if (d or not defined_only) and all(h.src != c.src for h in hits):
            hits.append(c)
    if not scope:
        return hits
    exact = None
    if "/" in scope or scope.endswith((".c", ".s")):
        path = os.path.normpath(os.path.join(root, scope) if not os.path.isabs(scope) else scope)
        for c, _d in (h for hs in index(root).values() for h in hs):
            if os.path.normpath(str(c.src)) == path or os.path.abspath(str(c.src)) == os.path.abspath(path):
                exact = c
                break
        if exact is not None:
            return [c for c in hits if c.src == exact.src]
        unit = unit_of_path(scope)
        if unit is None:
            raise LocateError("cannot tell the unit from the path %s (expected src/ovl/<NAME>.c, "
                              "src/main/<addr>.c or an asm/ovl/<NAME>/... path)" % scope)
    else:
        unit = scope
        if unit != "main" and unit not in srcscan.overlay_names(root):
            raise LocateError("unknown unit %s (main or an overlay of config/overlays.txt)" % unit)
    return [c for c in hits if c.unit == unit]


def locate(name, scope=None, root=".", defined_only=False):
    """The one CFile that holds `name` in `scope`. Raises LocateError (none) or AmbiguousError."""
    bare, scope = split_scope(name, scope)
    hits = candidates(bare, scope, root, defined_only)
    if len(hits) == 1:
        return hits[0]
    where = " in %s" % scope if scope else ""
    if not hits:
        raise LocateError("%s: no INCLUDE_ASM or C definition of it%s in the sources of the splat configs "
                          "(a mention in a caller does not count)" % (bare, where))
    cands = ", ".join("%s (%s)" % (c.unit, c.src) for c in hits)
    raise AmbiguousError("%s: held by %d C files: %s; pick one with --unit NAME, a UNIT:%s prefix, or a "
                         "source path as --unit" % (bare, len(hits), cands, bare))


def includes_of(path, root="."):
    """Set of the files a C file includes with #include "..." (include/ relative), recursively."""
    seen, todo = set(), [Path(path)]
    inc = Path(root) / "include"
    while todo:
        p = todo.pop()
        try:
            text = p.read_text(errors="replace")
        except OSError:
            continue
        for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"', text, re.M):
            q = inc / m.group(1)
            if q not in seen and q.exists():
                seen.add(q)
                todo.append(q)
    return seen
