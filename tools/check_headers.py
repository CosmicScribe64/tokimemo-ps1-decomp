#!/usr/bin/env python3
"""Fail when one identifier is declared with different types in the same header closure.

IDO's cfe stops with "redeclaration of X" when a translation unit sees two
declarations of one name with different types. Every .c file includes one root header
(include/game.h, or include/ovl/<NAME>.h which includes game.h), so the unit that matters
is the include closure of each header under include/. Overlays are separate programs: two
overlay headers may type the same overlay-local address differently, because they are never
in one translation unit. Inside one closure there must be exactly one type per symbol
(CODING_STANDARDS.md, "Shared externs").

With a source directory, the declarations and function definitions at the top level of each
.c file are also compared with the headers it includes (conflicts only).

Main-exe symbols have one home, include/main_api.h (T-3340): when that file exists, the checks of
tools/sync_protos.py run too (a main-exe symbol declared in another header, an overlay view that
differs from main_api.h without an explicit MAIN_API_OVERRIDE_<symbol>, ...). Each message names
the fix.

Headers are read with a small preprocessor (#include, #define, #ifdef/#ifndef/#else/#endif in
file order), so a MAIN_API_OVERRIDE_<symbol> define in an overlay header hides the declaration
of main_api.h in that overlay's translation units.

Checked: `extern` objects and function prototypes (return type and parameter types; an
unprototyped `()` list is compatible with any list). Exact repeats are reported too:
they are harmless to the compiler but are clutter, so they fail the check as well.

Usage: python3 tools/check_headers.py [include_dir [src_dir]]
Exit status 0 when clean, 1 when conflicts or duplicates are found.
"""
import os
import re
import sys


KR_PARAMS_RE = re.compile(r"(\)[ \t]*\n)(?:[ \t]*[A-Za-z_][^;{}()\n]*;[ \t]*\n)+(?=[ \t]*\{)")


def strip(text):
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    text = re.sub(r"^[ \t]*#.*$", " ", text, flags=re.M)
    return text


def split_top(s, sep):
    out, depth, cur = [], 0, ""
    for ch in s:
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        if ch == sep and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += ch
    out.append(cur)
    return out


def norm(s):
    s = re.sub(r"\s+", " ", s).strip()
    s = re.sub(r"\s*([*\[\](),])\s*", r"\1", s)
    return s


def parse_declarator(d, base):
    """Return (name, normalized type) for one declarator, or None."""
    d = d.strip()
    m = re.match(r"^([\s*]*)([A-Za-z_]\w*)\s*(.*)$", d, flags=re.S)
    if not m:
        return None
    stars, name, rest = m.groups()
    rest = norm(rest)
    stars = stars.replace(" ", "")
    if rest.startswith("("):
        # function: normalise parameters, (void) and () differ on purpose
        params = norm(rest[1:rest.rindex(")")])
        parts = [p for p in split_top(params, ",")]
        types = []
        for p in parts:
            p = p.strip()
            pm = re.match(r"^(.*?[\s*])([A-Za-z_]\w*)((?:\[\])*)$", p)
            if pm and p not in ("", "void", "..."):
                p = pm.group(1) + pm.group(3)
            types.append(norm(p))
        sig = "(" + ",".join(types) + ")"
        return name, norm(base) + stars + "F" + sig
    return name, norm(base) + stars + rest


def declarations_text(text, defs=False):
    """Yield (name, normalized type, raw statement) for the top-level declarations of C text.

    `raw` is the statement with comments removed and white space collapsed (no `extern`, no `;`);
    tools/sync_protos.py uses it to write declarations."""
    text = strip(text)
    # K&R (old-style) definitions: the parameter declarations between `)` and `{` are not
    # top-level declarations (T-9210)
    text = KR_PARAMS_RE.sub(r"\1", text)
    # drop brace bodies (typedef struct/enum/union bodies, inline code)
    out, depth = "", 0
    for ch in text:
        if ch == "{":
            depth += 1
            if depth == 1:
                out += "{}"
            continue
        if ch == "}":
            depth -= 1
            if depth == 0:
                out += ";"
            continue
        if depth == 0:
            out += ch
    for stmt in out.split(";"):
        stmt = stmt.strip()
        if defs and re.search(r"\)\s*\{\}$", stmt):
            stmt = stmt[:-2].strip()
        if not stmt or stmt.startswith("typedef") or "{}" in stmt:
            continue
        stmt = re.sub(r"^extern\s+", "", stmt)
        decls = split_top(stmt, ",")
        first = decls[0].strip()
        # base type: all leading words but the declared name
        mm = re.match(r"^((?:\w+[\s]+)*?)([\s*]*[A-Za-z_]\w*\s*(?:\[.*|\(.*)?)$", first, flags=re.S)
        if not mm or not mm.group(1).strip():
            continue
        base, d0 = mm.group(1), mm.group(2)
        for i, d in enumerate([d0] + decls[1:]):
            r = parse_declarator(d, base)
            if r:
                yield r[0], r[1], re.sub(r"\s+", " ", (base + d if i == 0 else base + " " + d)).strip()


def declarations(path, defs=False):
    """Yield (name, normalized type) for the top-level declarations of one file."""
    with open(path) as fh:
        text = fh.read()
    for name, typ, _raw in declarations_text(text, defs):
        yield name, typ


_INCLUDES = {}   # path -> ((mtime_ns, size), [included names]): include closures re-read every file


def includes(path):
    """Names a file includes with #include "...". Cached per (mtime, size); drop_cache(path) after a write."""
    st = os.stat(path)
    key = (st.st_mtime_ns, st.st_size)
    hit = _INCLUDES.get(path)
    if hit is None or hit[0] != key:
        with open(path) as fh:
            src = fh.read()
        hit = _INCLUDES[path] = (key, re.findall(r'^\s*#\s*include\s+"([^"]+)"', src, flags=re.M))
    return iter(hit[1])


def drop_cache(path=None):
    """Forget the parse caches of one file after it was written (all files without a path)."""
    if path is None:
        _INCLUDES.clear()
        _SEGMENTS.clear()
    else:
        _INCLUDES.pop(path, None)
        _SEGMENTS.pop(path, None)


_SEGMENTS = {}


def segments(path):
    """File as a list of ('decls', [(name, type, raw)]) and ('dir', keyword, argument), in order.
    Cached per (mtime, size); drop_cache(path) after writing the file."""
    st = os.stat(path)
    key = (st.st_mtime_ns, st.st_size)
    hit = _SEGMENTS.get(path)
    if hit is not None and hit[0] == key:
        return hit[1]
    with open(path) as fh:
        text = fh.read()
    # comments out (newlines kept), directive continuation lines joined
    text = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), text, flags=re.S)
    text = re.sub(r"\\\n", " ", text)
    segs, chunk = [], []

    def flush():
        if chunk:
            segs.append(("decls", list(declarations_text("\n".join(chunk)))))
            del chunk[:]

    for line in text.split("\n"):
        m = re.match(r"^\s*#\s*(\w+)\s*(.*)$", line)
        if m:
            flush()
            segs.append(("dir", m.group(1), m.group(2).strip()))
        else:
            chunk.append(line)
    flush()
    _SEGMENTS[path] = (key, segs)
    return segs


def walk(path, inc, defines=None, seen=None, rel=None, out=None):
    """Declarations visible after reading `path` (a header or a .c file) in order, with #include,
    #define, #undef and #ifdef/#ifndef/#else/#endif evaluated (`#if` is read as true, `#if 0` as
    false). Returns [(name, type, file relative to inc, raw)]; `defines` is updated in place."""
    defines = set() if defines is None else defines
    seen = set() if seen is None else seen
    out = [] if out is None else out
    rel = rel or os.path.relpath(path, inc)
    seen.add(rel)
    stack, active = [], True   # (parent active, this branch taken before)
    for seg in segments(path):
        if seg[0] == "decls":
            if active:
                for name, typ, raw in seg[1]:
                    out.append((name, typ, rel, raw))
            continue
        _k, kw, arg = seg
        word = arg.split()[0] if arg.split() else ""
        if kw in ("ifdef", "ifndef"):
            cond = (word in defines) == (kw == "ifdef")
            stack.append((active, cond))
            active = active and cond
        elif kw == "if":
            cond = arg.strip() != "0"
            stack.append((active, cond))
            active = active and cond
        elif kw in ("elif", "else"):
            parent, taken = stack[-1]
            cond = (not taken) if kw == "else" else (not taken and arg.strip() != "0")
            stack[-1] = (parent, taken or cond)
            active = parent and cond
        elif kw == "endif":
            if stack:
                active, _t = stack.pop()
        elif not active:
            continue
        elif kw == "define":
            defines.add(re.split(r"[\s(]", arg)[0])
        elif kw == "undef":
            defines.discard(word)
        elif kw == "include":
            m = re.match(r'"([^"]+)"', arg)
            if m and m.group(1) not in seen and os.path.exists(os.path.join(inc, m.group(1))):
                walk(os.path.join(inc, m.group(1)), inc, defines, seen, m.group(1), out)
    return out


NARROW = ("u8", "s8", "u16", "s16", "float")


def compatible(a, b):
    """True when two normalised types can be declared together without a conflict."""
    if a == b:
        return True
    if "F(" in a and "F(" in b:
        ra, pa = a.split("F(", 1)
        rb, pb = b.split("F(", 1)
        if ra != rb:
            return False
        if pa == ")" or pb == ")":
            # K&R `()` against a prototype: the parameters must survive default promotion
            other = pb if pa == ")" else pa
            return not any(p in NARROW for p in other.rstrip(")").split(","))
        return False
    return False


def header_files(inc):
    """{path relative to inc: absolute path} of every header under inc."""
    headers = {}
    for root, _d, files in os.walk(inc):
        for f in files:
            if f.endswith(".h"):
                headers[os.path.relpath(os.path.join(root, f), inc)] = os.path.join(root, f)
    return headers


def check(inc, src=None, api=True):
    drop_cache()
    headers = header_files(inc)
    problems = set()
    visible = {}   # header -> declarations visible after reading it as a root
    for h in sorted(headers):
        items = walk(headers[h], inc)
        visible[h] = items
        seen = {}
        for name, typ, f, _raw in items:
            seen.setdefault(name, []).append((typ, f))
        for name, lst in seen.items():
            for i in range(len(lst)):
                for j in range(i + 1, len(lst)):
                    (ta, fa), (tb, fb) = lst[i], lst[j]
                    if fa == fb:
                        continue
                    if ta == tb:
                        problems.add("duplicate %s: '%s' in %s and %s" % (name, ta, fa, fb))
                    elif not compatible(ta, tb):
                        problems.add("conflict %s: '%s' in %s vs '%s' in %s" % (name, ta, fa, tb, fb))
    # a duplicate inside one header (same file twice)
    for h in sorted(headers):
        c = {}
        for name, typ in declarations(headers[h]):
            c.setdefault(name, []).append(typ)
        for name, ts in c.items():
            for k in range(1, len(ts)):
                if ts[k] == ts[0]:
                    problems.add("duplicate %s: '%s' declared twice in %s" % (name, ts[0], h))
                elif not compatible(ts[0], ts[k]):
                    problems.add("conflict %s: '%s' vs '%s' in %s" % (name, ts[0], ts[k], h))
    if src:
        for root, _d, files in os.walk(src):
            for f in sorted(files):
                if not f.endswith(".c"):
                    continue
                path = os.path.join(root, f)
                seen = {}
                for name, typ, hf, _raw in walk(path, inc, rel="<src>"):
                    seen.setdefault(name, []).append((typ, hf))
                for name, typ in declarations(path, defs=True):
                    for t2, hf in seen.get(name, []):
                        if not compatible(typ, t2):
                            problems.add("conflict %s: '%s' in %s vs '%s' in %s" % (name, typ, path, t2, hf))
    if api and "main_api.h" in headers:
        import sync_protos
        problems.update(sync_protos.check_api(inc, src))
    return sorted(problems)


def main():
    inc = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(__file__), "..", "include")
    src = sys.argv[2] if len(sys.argv) > 2 else None
    problems = check(inc, src)
    for p in problems:
        print(p)
    if problems:
        print("%d header declaration problem(s)" % len(problems))
        return 1
    print("headers OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
