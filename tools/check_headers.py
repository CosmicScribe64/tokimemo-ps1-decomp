#!/usr/bin/env python3
"""Fail when one identifier is declared with different types in the same header closure.

IDO's cfe stops with "redeclaration of X" when a translation unit sees two
declarations of one name with different types. Every .c file includes one root header
(include/game.h, or include/ovl/<NAME>.h which includes game.h), so the unit that matters
is the include closure of each header under include/. Overlays are separate programs: two
overlay headers may type the same overlay-local address differently, because they are never
in one translation unit. Inside one closure there must be exactly one type per symbol
(CODING_STANDARDS.md, "Shared externs").

Checked: `extern` objects and function prototypes (return type and parameter types; an
unprototyped `()` list is compatible with any list). Exact repeats are reported too:
they are harmless to the compiler but are clutter, so they fail the check as well.

Usage: python3 tools/check_headers.py [include_dir]
Exit status 0 when clean, 1 when conflicts or duplicates are found.
"""
import os
import re
import sys


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


def declarations(path):
    with open(path) as fh:
        text = strip(fh.read())
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
            continue
        if depth == 0:
            out += ch
    for stmt in out.split(";"):
        stmt = stmt.strip()
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
                yield r


def includes(path):
    with open(path) as fh:
        src = fh.read()
    for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"', src, flags=re.M):
        yield m.group(1)


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


def check(inc):
    headers = {}
    for root, _d, files in os.walk(inc):
        for f in files:
            if f.endswith(".h"):
                headers[os.path.relpath(os.path.join(root, f), inc)] = os.path.join(root, f)

    def closure(h, seen):
        if h in seen or h not in headers:
            return
        seen.append(h)
        for i in includes(headers[h]):
            closure(i, seen)

    own = {h: list(declarations(p)) for h, p in headers.items()}
    problems = set()
    for h in sorted(headers):
        files = []
        closure(h, files)
        seen = {}
        for f in files:
            for name, typ in own[f]:
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
        for name, typ in own[h]:
            c.setdefault(name, []).append(typ)
        for name, ts in c.items():
            for k in range(1, len(ts)):
                if ts[k] == ts[0]:
                    problems.add("duplicate %s: '%s' declared twice in %s" % (name, ts[0], h))
                elif not compatible(ts[0], ts[k]):
                    problems.add("conflict %s: '%s' vs '%s' in %s" % (name, ts[0], ts[k], h))
    return sorted(problems)


def main():
    inc = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(__file__), "..", "include")
    problems = check(inc)
    for p in problems:
        print(p)
    if problems:
        print("%d header declaration problem(s)" % len(problems))
        return 1
    print("headers OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
