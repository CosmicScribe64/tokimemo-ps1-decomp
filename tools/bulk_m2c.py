#!/usr/bin/env python3
"""Unattended m2c + auto-solve pass over the small unmatched functions (T-11190).

Usage (in Docker, repo root as cwd; --out is a scratch directory outside the checkout):
    TOKIMEMO_SCRATCH=/path/to/scratch tools/docker.sh python3 tools/bulk_m2c.py --out /scratch/bulk_m2c \\
        [--max-size 600] [--unit NAME] [--exclude TT] [--limit N] [--jobs N] [--keep-score 30] [--time 60]
        [--variants N] [--retry near,fail [--max-score S]]
    python3 tools/bulk_m2c.py --out DIR --summary      # summary of the state file only, no work

Targets: every `INCLUDE_ASM` function under --max-size bytes (splat `nonmatching` header of its .s)
that is not already a guarded draft, smallest first. For each one:

1. tools/m2c.py's run_m2c gives the draft with the usual context (headers, rodata, callee argument
   counts, post-increment order). With --variants N (T-11230) the first N option sets of VARIANTS
   (m2c and wrapper options) each give a draft; each goes through steps 2 and 3, and the one with
   the lowest score is kept (a tie keeps the earlier variant, a score of 0 stops the sweep). If no
   variant scores, the first failure is recorded. The winner is in state.jsonl and near.txt.
2. Mechanical cleanup only: m2c's unknown type `?` becomes `s32` (declarations, casts, parameters),
   `//` comments become `/* */`, and a draft with an `M2C_*` placeholder is refused. Declarations
   m2c printed for symbols the headers lack are kept for the symbols the body uses (`static` and
   initialisers dropped, functions as K&R `T f();`). A second variant inlines `temp_*` locals that
   are assigned once from a side-effect-free expression and read once on the next line; both are
   scored and the better one is kept.
3. The draft goes into the C file as `#ifdef NON_MATCHING <draft> #else INCLUDE_ASM #endif`, the
   extra declarations in a marked block above it, and only that function is compiled (autotweak's
   BuildScorer: permute.py setup, tools/cc.py). On a compile error the cheap fixes are tried:
   declare an undefined symbol (m2c's declaration, else `extern s32 X;`), drop a declaration the
   headers already have with another type. Otherwise the reason is recorded.
4. Score 0 is verified like a permuter output (ninja object, funcdiff --resolve, unit sha1). A score
   under --keep-score goes to autotweak's hill climb for --time seconds, including its
   statement-order stage (T-11200; --no-order turns it off) and, with the traced uopt, its
   regdiff-guided edit order (T-11220; --no-regdiff turns it off). A verified match replaces
   the guard with plain C; its extra declarations move to include/main_api.h (main-exe addresses)
   or include/ovl/<UNIT>.h (overlay addresses), then check_headers.py and the unit target must pass,
   else everything is reverted. The climb compiles --score-jobs candidates at once (T-11250,
   tools/fastscore.py; --exact scores one at a time, as before). Every other draft is reverted to INCLUDE_ASM and saved as
   <out>/drafts/<UNIT>/<func>.c with its score.

The state file <out>/state.jsonl makes runs resumable: a target already in it is skipped, except
with --retry STATUSES (near, fail), which processes those entries again and replaces their lines;
--max-score S limits the near ones to scores of at most S. Jobs
work on disjoint C files; ninja, header writes and the final checks hold one lock (<out>/build.lock),
so one build runs at a time. A draft left in src/ by a killed run is restored from <out>/inflight/
at the next start. Output: one line per target, then MATCH count/bytes, failures by reason, wins per
variant, and <out>/near.txt (near misses by score).
"""
import argparse
import collections
import contextlib
import fcntl
import io
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import autotweak  # noqa: E402
import typefix  # noqa: E402

TICKET = "T-11190"
DECL_START = "/* bulk_m2c declarations (%s) */" % TICKET
DECL_END = "/* end bulk_m2c declarations */"
NOTE = "/* NON_MATCHING: %s bulk_m2c draft */" % TICKET
MAIN_RANGE = (0x80041000, 0x8012B537)
OVERLAY_BASE = 0x80132000
INCLUDE_ASM_TMPL = r'^[ \t]*INCLUDE_ASM\(\s*"[^"]*"\s*,\s*%s\s*\);[^\n]*\n?'
ERROR_RE = re.compile(r"(?i)\berror\b[^\n]*")


# ---------------------------------------------------------------------------------------------
# Cleanup of the m2c output
# ---------------------------------------------------------------------------------------------

def fix_comments(text):
    """`// x` comments outside strings and block comments become `/* x */`."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] not in (c, "\n"):
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append("/* %s */" % text[i + 2:j].strip().replace("*/", "* /"))
            i = j
            continue
        else:
            j = i + 1
        out.append(text[i:j])
        i = j
    return "".join(out)


def fix_unknown_types(text):
    """m2c's `?` (unknown type) as s32: at the start of a declaration, in a cast, in a parameter."""
    text = re.sub(r"^([ \t]*(?:extern[ \t]+|static[ \t]+)?)\?(?=[ \t*])", r"\1s32", text, flags=re.M)
    text = re.sub(r"\(\?([ \t]*\**)\)", r"(s32\1)", text)
    text = re.sub(r"\(\?([ \t]*\()", r"(s32\1", text)
    text = re.sub(r"([(,][ \t]*)\?(?=[ \t]*[*\w),])", r"\1s32", text)
    return text


def decl_name(line):
    m = re.search(r"(\w+)\s*(?:\(|\[|;)", line)
    return m.group(1) if m else None


def clean_decls(decls):
    """{name: declaration line} from m2c's declarations of symbols missing from the headers."""
    out, depth = {}, 0
    for raw in decls.splitlines():
        line = re.sub(r"/\*.*?\*/", "", raw).strip()
        opened = line.count("{") - line.count("}")
        if depth or opened:
            depth += opened
            continue                       # struct definitions and initialiser blocks
        if not line.endswith(";") or "=" in line or line.startswith("#"):
            continue
        line = fix_unknown_types(re.sub(r"^static\s+", "", line))
        name = decl_name(line)
        if not name:
            continue
        if "(" in line:
            line = line[:line.index("(")] + "();"
        elif not line.startswith("extern "):
            line = "extern " + line
        out[name] = line
    return out


SIDE_EFFECT_RE = re.compile(r"\+\+|--|(?<![=!<>])=(?!=)|\b(?!sizeof\b)[A-Za-z_]\w*\s*\(")
SIMPLE_RE = re.compile(r"^[\w.]+(?:->\w+)*$")


def inline_temps(body):
    """`temp_x = E; <next line using temp_x once>` -> the next line with (E), when temp_x has one
    declaration, one assignment and one use and E has no call, assignment or ++/--."""
    lines = body.split("\n")
    changed = True
    while changed:
        changed = False
        masked = autotweak.mask("\n".join(lines)).split("\n")
        for i, line in enumerate(masked):
            m = re.match(r"^\s*(temp_\w+)\s*=\s*(.+);\s*$", line)
            if not m:
                continue
            name, expr = m.group(1), m.group(2).strip()
            if SIDE_EFFECT_RE.search(expr) or re.search(r"\b%s\b" % name, expr):
                continue
            uses = [k for k, l in enumerate(masked) if re.search(r"\b%s\b" % name, l)]
            decl = [k for k in uses if re.match(r"^\s*[\w ]+[\s*]%s\s*;\s*$" % name, masked[k])]
            nxt = next((k for k in range(i + 1, len(lines)) if masked[k].strip()), None)
            if len(decl) != 1 or sorted(uses) != sorted(decl + [i, nxt] if nxt is not None else []):
                continue
            if len(re.findall(r"\b%s\b" % name, masked[nxt])) != 1:
                continue
            use = re.search(r"\b%s\b" % name, masked[nxt])
            before = masked[nxt][:use.start()].rstrip()
            after = masked[nxt][use.end():].lstrip()
            whole = (before.endswith(("(", ",")) or (before.endswith("=") and before[-2:-1] not in "=!<>")) \
                and after[:1] in (")", ",", ";")
            repl = expr if whole or SIMPLE_RE.match(expr) else "(%s)" % expr
            lines[nxt] = lines[nxt][:use.start()] + repl + lines[nxt][use.end():]
            for k in sorted([decl[0], i], reverse=True):
                del lines[k]
            k = decl[0]
            if 0 < k < len(lines) and not lines[k].strip() and lines[k - 1].rstrip().endswith("{"):
                del lines[k]                # the blank line after the last declaration
            changed = True
            break
    return "\n".join(lines)


def cleanup(decls, body, glob=None):
    """(body, {name: decl}, problem): the mechanical cleanup of the module docstring. `glob` maps the
    variable names the headers and the C file declare to (base, stars, dims) for the selector rewrite."""
    body = typefix.early(fix_memcpy(stack_arrays(body)))     # T-11240
    bad = re.search(r"\bM2C_\w+", body)
    if bad:
        return body, {}, "m2c-error: %s" % bad.group(0)
    # m2c's switch annotations (`/* switch 1 */`, `/* irregular */`) are not project comments
    body = re.sub(r"[ \t]*/\* (?:switch \d+(?:; irregular)?|irregular) \*/", "", body)
    body = fix_unknown_types(fix_comments(body)).strip("\n") + "\n"
    names = set(re.findall(r"[A-Za-z_]\w*", autotweak.mask(body)))
    known = {n: d for n, d in clean_decls(decls).items() if n in names}
    types = dict(glob or {})
    collect_vars("\n".join(known.values()), types)
    body = typefix.late(void_locals(fix_selectors(fix_pointer_calls(body), types)), types)     # T-11240
    return body, known, None


# ---------------------------------------------------------------------------------------------
# Draft placement in the C file
# ---------------------------------------------------------------------------------------------

def guarded_funcs(text):
    return {g.func for g in autotweak.guards(text)}


def include_asm_funcs(text):
    """Functions of plain INCLUDE_ASM lines (not the #else branch of a guarded draft)."""
    guarded = guarded_funcs(text)
    return [f for f in re.findall(r'^[ \t]*INCLUDE_ASM\(\s*"[^"]*"\s*,\s*(\w+)\s*\)', text, re.M)
            if f not in guarded]


def insert_draft(text, func, body, decl_lines):
    """`text` with the INCLUDE_ASM line of `func` wrapped in a NON_MATCHING guard holding `body`,
    and `decl_lines` in a marked block above it. None when there is no plain INCLUDE_ASM line."""
    if func in guarded_funcs(text):
        return None
    m = re.search(INCLUDE_ASM_TMPL % re.escape(func), text, re.M)
    if not m:
        return None
    inc = m.group(0).rstrip("\n") + "\n"
    block = ""
    if decl_lines:
        block = DECL_START + "\n" + "".join(d + "\n" for d in decl_lines) + DECL_END + "\n"
    new = (block + "#ifdef NON_MATCHING\n" + NOTE + "\n" + body.strip("\n") + "\n#else\n" + inc + "#endif\n")
    return text[:m.start()] + new + text[m.end():]


def drop_decl_block(text):
    return re.sub(r"^%s\n.*?^%s\n" % (re.escape(DECL_START), re.escape(DECL_END)), "", text, flags=re.M | re.S)


def header_home(name, unit):
    """include/ path where a declaration of `name` belongs, or None (no address in the name)."""
    m = re.search(r"_([0-9A-Fa-f]{8})$", name)
    if not m:
        return None
    addr = int(m.group(1), 16)
    if MAIN_RANGE[0] <= addr <= MAIN_RANGE[1]:
        return "include/main_api.h"
    if addr >= OVERLAY_BASE and unit != "main":
        return "include/ovl/%s.h" % unit
    return None


def insert_in_header(text, lines):
    """`text` with `lines` before the #endif that closes its include guard (else at the end)."""
    add = "/* %s: declarations added by tools/bulk_m2c.py. */\n" % TICKET + "".join(l + "\n" for l in lines)
    first = re.search(r"^[ \t]*#[ \t]*ifndef\b[^\n]*\n", text, re.M)
    if first:
        depth = 1
        for d in re.finditer(r"^[ \t]*#[ \t]*(if|endif)\w*[^\n]*\n?", text[first.end():], re.M):
            depth += 1 if d.group(1) == "if" else -1
            if depth == 0:
                at = first.end() + d.start()
                return text[:at] + add + "\n" + text[at:]
    return text.rstrip("\n") + "\n\n" + add


def write_atomic(path, text):
    tmp = "%s.bulk_m2c.%d.tmp" % (path, os.getpid())
    with open(tmp, "w") as fh:
        fh.write(text)
    os.replace(tmp, path)


def read(path):
    with open(path) as fh:
        return fh.read()


# ---------------------------------------------------------------------------------------------
# Typed rewrites of m2c output (T-11210)
# ---------------------------------------------------------------------------------------------

PRIMS = {"u8", "s8", "u16", "s16", "u32", "s32", "u64", "s64", "f32", "f64", "float", "double", "char",
         "short", "int", "long", "void", "unsigned", "signed"}
NARROW = {"void", "u8", "s8", "u16", "s16", "char", "short", "unsigned char", "signed char",
          "unsigned short"}
QUALS = {"extern", "static", "const", "volatile", "register"}
UNK_RE = re.compile(r"(?<![\w.>])([A-Za-z_]\w*)((?:\[[^\[\]]*\])*)[ \t]*(\.|->)[ \t]*unk([0-9A-Fa-f]+)\b")
VAR_RE = re.compile(r"^[ \t]*((?:[A-Za-z_]\w*[ \t]+)+?)(\**)[ \t]*([A-Za-z_]\w*)((?:\[[^\]]*\])*)[ \t]*(?:;|=[^=])")
NOT_TYPES = {"return", "typedef", "goto", "else", "case", "default", "break", "continue", "sizeof", "do"}


def split_args(text):
    """Top-level comma-separated arguments of `text` (the inside of a call's parentheses)."""
    out, depth, cur = [], 0, ""
    for ch in text:
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    out.append(cur.strip())
    return out


def balanced(text, open_at):
    """Index just past the parenthesis group that opens at text[open_at]."""
    depth = 0
    for i in range(open_at, len(text)):
        if text[i] == "(":
            depth += 1
        elif text[i] == ")":
            depth -= 1
            if depth == 0:
                return i + 1
    return -1


def fix_memcpy(body):
    """`M2C_MEMCPY_ALIGNED(dst, src, n)` -> a struct copy of n bytes through a block-scope typedef
    (a whole-word struct of the right size, so IDO copies it word by word like m2c's memcpy)."""
    sizes, pos = [], 0
    while True:
        at = body.find("M2C_MEMCPY_ALIGNED(", pos)
        if at < 0:
            break
        op = at + len("M2C_MEMCPY_ALIGNED")
        end = balanced(body, op)
        args = split_args(body[op + 1:end - 1]) if end > 0 else []
        try:
            n = int(args[2], 0)
        except (IndexError, ValueError):
            pos = op
            continue
        if len(args) != 3 or n <= 0 or n % 4:
            pos = op
            continue
        typ = "M2cBlock%X" % n
        if n not in sizes:
            sizes.append(n)
        repl = "*(%s *)(%s) = *(%s *)(%s)" % (typ, args[0], typ, args[1])
        body = body[:at] + repl + body[end:]
        pos = at + len(repl)
    if sizes:
        lines = ["    typedef struct { s32 w[%d]; } M2cBlock%X; /* m2c memcpy of %d bytes */" % (n // 4, n, n)
                 for n in sizes]
        m = re.search(r"\{[ \t]*\n", body)
        if m:
            body = body[:m.end()] + "\n".join(lines) + "\n" + body[m.end():]
    return body


def stack_arrays(body):
    """m2c's `? spXX;` locals (struct temporaries it could not type) become s32 word arrays that cover
    every offset the body touches; the selector pass then turns their `.unkN` into casts."""
    for m in list(re.finditer(r"^([ \t]*)\?[ \t]+(sp[0-9A-Fa-f]+);", body, re.M)):
        name = m.group(2)
        top = 4
        for off in re.finditer(r"\(&%s\)[ \t]*\+[ \t]*\(?(-?\w+)\)?|\b%s\.unk([0-9A-Fa-f]+)" % (name, name), body):
            try:
                top = max(top, (int(off.group(1), 0) if off.group(1) else int(off.group(2), 16)) + 4)
            except ValueError:
                pass
        for cp in re.finditer(r"M2C_MEMCPY_ALIGNED\(([^;]*)\)", body):
            args = split_args(cp.group(1))
            if len(args) == 3 and any(re.fullmatch(r"&%s(\[0\])?" % name, a.replace(" ", "")) for a in args[:2]):
                try:
                    top = max(top, int(args[2], 0))
                except ValueError:
                    pass
        body = body.replace(m.group(0), "%ss32 %s[%d];" % (m.group(1), name, (top + 3) // 4), 1)
    return body


def parse_var(line):
    """(name, (base, stars, dims)) of a variable declaration line, or None."""
    if "(" in line.split(";")[0].split("=")[0]:
        return None
    m = VAR_RE.match(line)
    if not m:
        return None
    words = [w for w in m.group(1).split() if w not in QUALS]
    if not words or words[0] in NOT_TYPES:
        return None
    return m.group(3), (" ".join(words), len(m.group(2)), re.findall(r"\[[^\]]*\]", m.group(4)))


def collect_vars(text, into=None):
    """{name: (base, stars, dims)} from the declaration lines of `text`."""
    into = {} if into is None else into
    for line in text.splitlines():
        got = parse_var(line)
        if got:
            into.setdefault(got[0], got[1])
    return into


def signature_vars(body):
    """Parameters of the first function head of `body`."""
    out = {}
    m = re.search(r"\w+\(([^)]*)\)\s*\{", body)
    if m and m.group(1).strip() not in ("", "void"):
        for part in split_args(m.group(1)):
            got = parse_var(part + ";")
            if got:
                out[got[0]] = got[1]
    return out


def is_prim(base):
    return all(w in PRIMS for w in base.split())


def field_type(base, stars):
    """Access type for an object of (base, stars): narrow and void types widen to s32."""
    if stars == 0 and base in NARROW:
        return "s32"          # an unkN access has lost its width; a word is the commonest
    return base + (" " + "*" * stars if stars else "")


def fix_selectors(body, glob=None):
    """`X.unkN` / `X->unkN` where the project's type of X is not an aggregate (a scalar, an array or
    pointer of scalars, `void *`) -> `*(T *)((u8 *)&X + N)` (`X` itself for offset 0 of a scalar).
    Variables of unknown or struct type are left alone."""
    types = dict(glob or {})
    types.update(collect_vars(body))
    types.update(signature_vars(body))

    def repl(m):
        name, subs, op, hexoff = m.groups()
        t = types.get(name)
        if t is None:
            return m.group(0)
        base, stars, dims = t[0], t[1], list(t[2])
        for _ in re.findall(r"\[", subs):
            if dims:
                dims.pop(0)
            elif stars:
                stars -= 1
            else:
                return m.group(0)
        off = int(hexoff, 16)
        ref = name + subs
        if op == ".":
            if not is_prim(base) or (stars and not dims):
                return m.group(0)
            if not dims and stars == 0 and off == 0:
                return ref
            typ = field_type(base, stars)
            return "(*(%s *)((u8 *)&%s + %d))" % (typ, ref, off) if off else "(*(%s *)&%s)" % (typ, ref)
        if dims:
            if len(dims) > 1 or not is_prim(base):
                return m.group(0)
            typ = field_type(base, stars)
        elif stars:
            if not is_prim(base) and stars == 1:
                return m.group(0)
            typ = field_type(base, stars - 1)
        elif is_prim(base):
            typ = "s32"
        else:
            return m.group(0)
        return "(*(%s *)((u8 *)%s + %d))" % (typ, ref, off) if off else "(*(%s *)%s)" % (typ, ref)

    return UNK_RE.sub(repl, body)


def fix_pointer_calls(body):
    """m2c's `*(T (**)(args))(addr)(x)` (a call through a pointer read from memory) parses as a call
    of a non-function; it becomes `(*(T (**)(args))(addr))(x)`."""
    out, pos = [], 0
    for m in re.finditer(r"\*\(((?:[\w ]+) \(\*\*\)\([^()]*\))\)\(", body):
        if m.start() < pos:
            continue
        op = m.end() - 1
        end = balanced(body, op)
        if end > 0 and body[end:end + 1] == "(":
            out.append(body[pos:m.start()] + "(" + body[m.start():end] + ")")
            pos = end
    return "".join(out) + body[pos:]


def void_locals(body):
    """`void *` locals that the body does arithmetic on become `u8 *` (m2c means byte arithmetic;
    IDO refuses arithmetic on void *)."""
    for m in list(re.finditer(r"^([ \t]+)void[ \t]*\*[ \t]*(\w+);", body, re.M)):
        name = m.group(2)
        rest = body.replace(m.group(0), "")
        if re.search(r"\b%s\b[ \t]*(?:[-+]=|[-+][ \t(\w])|[-+][ \t]*\(?%s\b|\(u8 \*\)%s\b|\b%s\b[ \t]*\[" % ((name,) * 4), rest):
            body = body.replace(m.group(0), "%su8 *%s;" % (m.group(1), name), 1)
    return body


def header_text(root, unit):
    """Text of the headers a declaration for `unit` can come from (main_api.h, the overlay header)."""
    out = []
    for rel in ("include/main_api.h", "include/ovl/%s.h" % unit):
        try:
            out.append(read(os.path.join(root, rel)))
        except OSError:
            pass
    return "\n".join(out)


def signature_of(text, name, tail):
    """(return type, parameter text) of `name` followed by `tail` (`;` prototype, `{` definition)."""
    m = re.search(r"^[ \t]*(?:extern[ \t]+)?([A-Za-z_][\w \t]*?)([ \t]+|[ \t]*\*+[ \t]*)\b%s\s*\(([^)]*)\)\s*%s"
                  % (re.escape(name), tail), text, re.M)
    if not m:
        return None
    ret = re.sub(r"\s+", " ", m.group(1)).strip()
    stars = m.group(2).count("*")
    return (ret + " " + "*" * stars if stars else ret), re.sub(r"\s+", " ", m.group(3)).strip()


def prototype_in(text, name):
    return signature_of(text, name, ";")


def definition_in(text, name):
    """Like prototype_in for a definition; a K&R parameter list (names only) gives `()`."""
    got = signature_of(text, name, r"(?:[^;{}]*;\s*)*\{")
    if got and re.fullmatch(r"\w+(\s*,\s*\w+)*", got[1]) and got[1] != "void":
        return got[0], ""
    return got


# ---------------------------------------------------------------------------------------------
# Compile errors and their cheap fixes
# ---------------------------------------------------------------------------------------------

def first_error(msg):
    """The first error line of a compiler or tool message, without directory paths."""
    msg = (msg or "").strip()
    m = ERROR_RE.search(msg)
    line = m.group(0) if m else (msg.splitlines()[-1] if msg else "unknown")
    return re.sub(r"\S*/", "", line).strip()[:100]


def cheap_fix(msg, decl_lines, known):
    """New declaration list for a compile error `msg`, or None when no cheap fix applies."""
    new = list(decl_lines)
    for name in re.findall(r"redeclaration of '(\w+)'", msg):
        new = [d for d in new if decl_name(d) != name]
    for name in re.findall(r"'(\w+)' undefined", msg):
        if any(decl_name(d) == name for d in new) or re.fullmatch(r"sp[0-9A-Fa-f]+", name):
            continue                       # a stack slot is a local, see source_fix
        new.append(known.get(name) or "extern s32 %s;" % name)
    return new if new != list(decl_lines) else None


def with_return_type(body, func, ret):
    """`body` with the return type of the definition of `func` replaced by `ret`."""
    return re.sub(r"^[A-Za-z_][\w \t*]*?([ \t*])(%s\s*\()" % re.escape(func),
                  lambda m: ret + (" " if not ret.endswith("*") else "") + m.group(2), body, count=1, flags=re.M)


def forward_decls(original, func, body, headers, have=()):
    """Prototypes for functions the draft calls that the C file defines after `func`'s INCLUDE_ASM line
    and no header declares: the implicit `int f()` at the call would clash with the definition."""
    m = re.search(INCLUDE_ASM_TMPL % re.escape(func), original, re.M)
    if not m:
        return []
    out, seen = [], set(have)
    for name in dict.fromkeys(re.findall(r"\b([A-Za-z_]\w*)\s*\(", autotweak.mask(body))):
        if name in seen or name == func or prototype_in(headers, name):
            continue
        sig = definition_in(original[m.end():], name)
        if sig and not definition_in(original[:m.start()], name):
            out.append("%s %s(%s);" % (sig[0], name, sig[1]))
    return out


def source_fix(msg, body, decl_lines, func, original, headers):
    """(body, decl_lines) after a fix that needs the C file or the headers, or None.

    Redeclaration of a function the draft calls before the file defines it: declare it as the
    definition does (the implicit `int f()` clashes). Redeclaration of the draft's own function:
    take the return type of the header prototype. Undefined `spXX`: a local word array."""
    for name in re.findall(r"redeclaration of '(\w+)'", msg):
        if any(decl_name(d) == name for d in decl_lines):
            continue
        if name == func:
            proto = prototype_in(headers, name)
            if proto:
                new = with_return_type(body, func, proto[0])
                if new != body:
                    return new, decl_lines
            continue
        sig = definition_in(original, name) or prototype_in(headers, name)
        if sig:
            return body, list(decl_lines) + ["%s %s(%s);" % (sig[0], name, sig[1])]
    for name in re.findall(r"'(sp[0-9A-Fa-f]+)' undefined", msg):
        m = re.search(r"\{[ \t]*\n", body)
        if m and not re.search(r"^[ \t]+\w[\w \t*]*\b%s\b[^;(]*;" % name, body, re.M):
            decl = "    s32 %s[%d];\n" % (name, 16 if re.search(r"\b%s\[|&%s" % (name, name), body) else 1)
            if decl.endswith("[1];\n"):
                decl = "    s32 %s;\n" % name
            return fix_selectors(body[:m.end()] + decl + body[m.end():]), decl_lines
    return None


# ---------------------------------------------------------------------------------------------
# Backend: the parts that need Docker (m2c, IDO, ninja); mocked in the tests
# ---------------------------------------------------------------------------------------------

class Variant(collections.namedtuple("Variant", "name extra opts")):
    """One m2c option set: `extra` goes to m2c itself, `opts` to tools/m2c.py's run_m2c."""


# The m2c option sets that --variants N takes the first N of, in this order (T-11230). The options
# are those of m2c 708d2d2 (tools/Dockerfile) and of tools/m2c.py. Not used: --stack-structs (only
# prints template structs, the body is the same), --no-casts (drops casts IDO needs), --valid-syntax
# (M2C_* macros, which cleanup refuses), formatting options (no effect on the compiled code).
VARIANTS = (
    # tools/m2c.py's defaults: header context, rodata, callee argument counts, post-increment fix
    Variant("default", (), {}),
    # m2c --no-andor: nested ifs instead of && and ||
    Variant("no-andor", ("--no-andor",), {}),
    # m2c --reg-vars saved: one variable per callee-saved register instead of temps and phis
    Variant("reg-vars", ("--reg-vars", "saved"), {}),
    # m2c --void: the function returns void (a $v0 left by a call is not a return value)
    Variant("void", ("--void",), {}),
    # m2c --no-switches: if trees stay ifs (jump-table switches are still switches)
    Variant("no-switches", ("--no-switches",), {}),
    # tools/m2c.py --no-callee-args: m2c's own guess of the callees' argument counts
    Variant("no-callee-args", (), {"callee_args": False}),
    # tools/m2c.py --no-postinc: keep m2c's `D += 1; if (D ...)` order
    Variant("no-postinc", (), {"postinc": False}),
    # m2c --no-unk-inference: no type inference on unknown struct fields and globals
    Variant("no-unk-inference", ("--no-unk-inference",), {}),
    # tools/m2c.py --no-context: no header context, m2c guesses every type itself
    Variant("no-context", (), {"context": False}),
    # m2c --gotos-only: no control-flow structuring, labels and gotos in asm block order
    Variant("gotos-only", ("--gotos-only",), {}),
)


def variant_list(n):
    """The first n option sets of VARIANTS (at least one, at most all)."""
    return list(VARIANTS[:max(1, min(n, len(VARIANTS)))])


class Scorer(autotweak.BuildScorer):
    INSTRUCTION_TOLERANCE = 10 ** 6


class Backend:
    score_jobs, exact = 1, False       # candidate scoring threads and the legacy switch (T-11250)

    def m2c(self, root, unit, func, variant=VARIANTS[0]):
        """(declarations, body) of the m2c draft with the options of `variant`; raises
        LookupError/RuntimeError."""
        import m2c
        with contextlib.redirect_stderr(io.StringIO()):
            text = m2c.run_m2c(root, "%s:%s" % (unit, func), extra=variant.extra, log=lambda _m: None,
                               **variant.opts)
        return m2c.split_draft(text, func)

    def scorer(self, root, draft):
        """An object with score(text) -> int|None and verify(text) -> bool; raises on compile errors."""
        with contextlib.redirect_stdout(io.StringIO()):
            scorer = Scorer(root, draft)
        scorer.configure(jobs=self.score_jobs, exact=self.exact)
        return scorer

    def migrate_globals(self, root):
        """Rewrite raw GameState globals in the matched C files (tools/migrate_globals.py --apply)."""
        p = subprocess.run([sys.executable, "tools/migrate_globals.py", "--apply"], cwd=root,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        return p.returncode == 0, p.stdout

    def check_headers(self, root):
        p = subprocess.run([sys.executable, "tools/check_headers.py", "include", "src"], cwd=root,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        return p.returncode == 0, p.stdout

    def build_unit(self, root, unit):
        import permute
        p = subprocess.run(["ninja", permute.unit_target(unit)], cwd=root,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        return p.returncode == 0, p.stdout


@contextlib.contextmanager
def locked(path):
    with open(path, "a") as fh:
        fcntl.flock(fh, fcntl.LOCK_EX)
        try:
            yield
        finally:
            fcntl.flock(fh, fcntl.LOCK_UN)


# ---------------------------------------------------------------------------------------------
# One target
# ---------------------------------------------------------------------------------------------

class Target:
    def __init__(self, unit, src, func, size):
        self.unit, self.src, self.func, self.size = unit, src, func, size

    @property
    def scope(self):
        return "%s:%s" % (self.unit, self.func)

    @property
    def key(self):
        return "%s:%s:%s" % (self.unit, self.src, self.func)

    def as_tuple(self):
        return (self.unit, self.src, self.func, self.size)


class Candidate:
    """A compiled draft of one variant: the body that compiled, its declarations, its scorer and the
    best start text (the body or its inlined form) with that text's score."""

    def __init__(self, variant, body, decl_lines, scorer):
        self.variant, self.body, self.decl_lines, self.scorer = variant, body, decl_lines, scorer
        self.cache, self.best, self.text = {}, None, None

    def score(self, text):
        if text not in self.cache:
            self.cache[text] = self.scorer.score(text)
        return self.cache[text]

    def prefetch(self, texts):
        """Let the scorer compile the texts the climb will ask for in parallel (T-11250)."""
        self.scorer.prefetch([t for t in texts if t not in self.cache])


def pick_best(candidates):
    """The candidate with the lowest score; on a tie the first one (the earlier variant)."""
    best = None
    for c in candidates:
        if best is None or c.best < best.best:
            best = c
    return best


def replace_state_row(path, key, row):
    """Rewrite the state file `path` with every line of `key` dropped and `row` appended."""
    keep = []
    if os.path.exists(path):
        with open(path) as fh:
            keep = [l for l in fh if l.strip() and json.loads(l)["key"] != key]
    write_atomic(path, "".join(keep) + json.dumps(row) + "\n")


class Run:
    """Paths and options shared by the jobs."""

    def __init__(self, root, out, keep_score=30, time_budget=60.0, backend=None, climb=None, emit=print,
                 order=True, regdiff=True, variants=None, replace=(), score_jobs=1):
        self.root, self.out = root, out
        self.score_jobs = score_jobs     # scoring threads per climb (T-11250); 1 = one at a time
        self.keep_score, self.time_budget = keep_score, time_budget
        self.backend = backend or Backend()
        self.climb = climb or autotweak.climb
        self.emit = emit
        self.order = order
        self.regdiff = regdiff
        self.variants = list(variants or VARIANTS[:1])
        self.replace = set(replace)        # state keys whose old lines a new record replaces (--retry)
        self.lock = os.path.join(out, "build.lock")
        self.state = os.path.join(out, "state.jsonl")
        self.inflight = os.path.join(out, "inflight")

    def record(self, t, status, score=None, reason=None, draft=None, variant=None):
        row = {"key": t.key, "scope": t.scope, "src": t.src, "size": t.size, "status": status,
               "score": score, "reason": reason, "draft": draft, "variant": variant,
               "tried": len(self.variants)}
        with locked(self.state + ".lock"):
            if t.key in self.replace:
                replace_state_row(self.state, t.key, row)
            else:
                with open(self.state, "a") as fh:
                    fh.write(json.dumps(row) + "\n")
        detail = {"match": "", "near": " %s" % score, "fail": " %s" % reason}[status]
        if variant and len(self.variants) > 1:
            detail += " [%s]" % variant
        self.emit("%s %s 0x%X%s" % (status.upper() if status == "match" else status, t.scope, t.size, detail))
        return row

    def save_draft(self, t, text, score, note, variant=None):
        d = os.path.join(self.out, "drafts", t.unit)
        os.makedirs(d, exist_ok=True)
        path = os.path.join(d, t.func + ".c")
        if variant and len(self.variants) > 1:
            note = ("%s m2c variant %s" % (note, variant)).strip()
        what = note if score is None else ("score %d %s" % (score, note)).strip()
        with open(path, "w") as fh:
            fh.write("/* %s %s (%s): %s */\n%s\n" % (TICKET, t.scope, t.src, what, text.strip("\n")))
        return path

    def inflight_path(self, t):
        return os.path.join(self.inflight, "%s__%s.json" % (t.unit, t.func))

    # -- the pipeline -----------------------------------------------------------------------

    def process(self, t):
        path = os.path.join(self.root, t.src)
        original = read(path)
        if t.func not in include_asm_funcs(original):
            return self.record(t, "fail", reason="gone: no plain INCLUDE_ASM line")
        os.makedirs(self.inflight, exist_ok=True)
        with open(self.inflight_path(t), "w") as fh:
            json.dump({"src": t.src, "text": original}, fh)
        matched = False
        try:
            best, failure = self._best_draft(t, path, original)
            if best is None:
                reason, body, note = failure
                draft = self.save_draft(t, body, None, note) if body is not None else None
                return self.record(t, "fail", reason=reason, draft=draft)
            matched, row = self._solve(t, path, original, best)
            return row
        except Exception as exc:  # noqa: BLE001  (unattended: record and go on with the next target)
            return self.record(t, "fail", reason="error: %s" % first_error("%s: %s" % (type(exc).__name__, exc)))
        finally:
            if not matched and read(path) != original:
                write_atomic(path, original)
            os.unlink(self.inflight_path(t))

    def _best_draft(self, t, path, original):
        """(best Candidate, None), or (None, (reason, body or None, note)) with the failure of the
        first variant that failed when no variant gives a draft that scores (T-11230). A score of 0
        ends the sweep. On return the C file holds the winner's guarded draft and its scorer is
        current."""
        glob = collect_vars(header_text(self.root, t.unit) + "\n" + original)
        found, failure, last, seen = [], None, None, set()
        for v in self.variants:
            cand, fail = self._candidate(t, path, original, v, glob, seen)
            if cand is None:
                failure = failure or fail
                continue
            found.append(cand)
            last = cand
            if cand.best == 0:
                break
        best = pick_best(found)
        if best is None:
            return None, failure
        if best is not last:
            # a later variant's scorer set up the shared work directory: compile the winner again
            write_atomic(path, insert_draft(original, t.func, best.body, best.decl_lines))
            best.scorer = self.backend.scorer(self.root, autotweak.Draft(t.src, t.unit, t.func, best.body))
        return best, None

    def _candidate(self, t, path, original, variant, glob, seen):
        """(Candidate, None) for one m2c variant, (None, None) when its m2c output repeats an earlier
        variant's (`seen`), or (None, (reason, body or None, note))."""
        try:
            decls, body = self.backend.m2c(self.root, t.unit, t.func, variant)
        except (LookupError, RuntimeError, OSError, SystemExit) as exc:
            return None, ("m2c: %s" % first_error(str(exc)), None, None)
        if (decls, body) in seen:
            return None, None              # many options change nothing for a small function
        seen.add((decls, body))
        body, known, problem = cleanup(decls, body, glob)
        if problem:
            return None, (problem, body, problem)
        decl_lines = sorted(known.values()) + forward_decls(
            original, t.func, body, header_text(self.root, t.unit), known)
        scorer, decl_lines, body = self._compile(t, path, original, body, decl_lines, known)
        if scorer is None:
            return None, (decl_lines, body, decl_lines)
        cand = Candidate(variant.name, body, decl_lines, scorer)
        start = [(cand.score(v), v) for v in dict.fromkeys([body, inline_temps(body)])]
        start = sorted((s, v) for s, v in start if s is not None)
        if not start:
            return None, ("compile: draft does not score", body, "does not score")
        cand.best, cand.text = start[0]
        return cand, None

    def _compile(self, t, path, original, body, decl_lines, known):
        """(scorer, decl_lines, body) once the guarded draft compiles, or (None, reason, body)."""
        for _attempt in range(12):
            text = insert_draft(original, t.func, body, decl_lines)
            write_atomic(path, text)
            try:
                return self.backend.scorer(self.root, autotweak.Draft(t.src, t.unit, t.func, body)), decl_lines, body
            except (RuntimeError, SystemExit, OSError, ImportError) as exc:
                msg = str(exc)
                fixed = cheap_fix(msg, decl_lines, known)
                if fixed is not None:
                    decl_lines = fixed
                    continue
                more = source_fix(msg, body, decl_lines, t.func, original, header_text(self.root, t.unit))
                if more is None:
                    return None, "compile: %s" % first_error(msg), body
                body, decl_lines = more
        return None, "compile: no fix after 12 attempts", body

    def _solve(self, t, path, original, cand):
        body, decl_lines, best, text, var = cand.body, cand.decl_lines, cand.best, cand.text, cand.variant

        def verify(text):
            with locked(self.lock):
                return cand.scorer.verify(text)

        final = None
        if best == 0:
            final = text if verify(text) else None
        elif best < self.keep_score:
            # the climb runs autotweak's statement-order stage (T-11200) unless --no-order
            extra = {} if self.order else {"order": False}
            # regdiff orders the climb's edits when the traced uopt is there (T-11220)
            advise = autotweak.make_advisor(self.root, autotweak.Draft(t.src, t.unit, t.func, body)) \
                if self.regdiff else None
            if advise is not None:
                extra["advise"] = advise
            if self.score_jobs > 1:
                extra["prefetch"] = cand.prefetch
                extra["order_opts"] = {"jobs": self.score_jobs}
            res = self.climb(text, cand.score, verify, budget=self.time_budget, **extra)
            if res.matched and res.best == 0:
                final = res.text
            elif res.best is not None and res.best < best:
                best, text = res.best, res.text
        note = "score 0 but verification failed" if best == 0 and final is None else ""
        if final is not None:
            reason = self._finish(t, path, original, final, decl_lines)
            if reason is None:
                return True, self.record(t, "match", score=0, variant=var)
            return False, self.record(t, "fail", reason=reason, draft=self.save_draft(t, final, 0, reason, var),
                                      variant=var)
        return False, self.record(t, "near", score=best, reason=note or None,
                                  draft=self.save_draft(t, text, best, note, var), variant=var)

    def _finish(self, t, path, original, final, decl_lines):
        """Write the verified match (guard -> plain C, declarations -> headers); None or a reason."""
        with locked(self.lock):
            new = autotweak.rewrite_guard(read(path), t.func, final)
            if new is None:
                return "rewrite: guard block not found"
            write_atomic(path, drop_decl_block(new))
            if not decl_lines:
                return None
            saved, homes = {}, {}
            for d in decl_lines:
                home = header_home(decl_name(d), t.unit)
                if home is None:
                    write_atomic(path, original)
                    return "headers: no header home for %s" % decl_name(d)
                homes.setdefault(home, []).append(d)
            for home, lines in homes.items():
                hp = os.path.join(self.root, home)
                saved[hp] = read(hp)
                write_atomic(hp, insert_in_header(saved[hp], lines))
            ok, msg = self.backend.check_headers(self.root)
            if ok:
                ok, msg = self.backend.build_unit(self.root, t.unit)
            if ok:
                return None
            for hp, text in saved.items():
                write_atomic(hp, text)
            write_atomic(path, original)
            return "headers: %s" % first_error(msg)


# ---------------------------------------------------------------------------------------------
# Targets, state, jobs, summary
# ---------------------------------------------------------------------------------------------

def find_targets(root, max_size, units=None, exclude=()):
    import srcscan
    out = []
    for c in srcscan.c_files(root):
        if c.unit in exclude or (units and c.unit not in units) or not c.src.exists():
            continue
        rel = os.path.relpath(str(c.src), root)
        for func in include_asm_funcs(c.src.read_text(errors="replace")):
            asm = c.nonmatchings / (func + ".s")
            got = srcscan.nonmatching_size(asm) if asm.exists() else None
            if got and got[1] < max_size:
                out.append(Target(c.unit, rel, func, got[1]))
    return select(out, max_size)


def select(targets, max_size, done=(), limit=None):
    """Targets under max_size and not in `done`, smallest first, at most `limit`."""
    out = sorted((t for t in targets if t.size < max_size and t.key not in done),
                 key=lambda t: (t.size, t.unit, t.src, t.func))
    return out[:limit] if limit else out


def load_state(path):
    rows = {}
    if os.path.exists(path):
        with open(path) as fh:
            for line in fh:
                line = line.strip()
                if line:
                    row = json.loads(line)
                    rows[row["key"]] = row
    return rows


def recover(root, out, emit=print):
    """Restore C files that a killed run left with a bulk_m2c draft in them."""
    d = os.path.join(out, "inflight")
    if not os.path.isdir(d):
        return 0
    n = 0
    for name in sorted(os.listdir(d)):
        p = os.path.join(d, name)
        with open(p) as fh:
            info = json.load(fh)
        src = os.path.join(root, info["src"])
        if os.path.exists(src) and (NOTE in read(src) or DECL_START in read(src)):
            write_atomic(src, info["text"])
            emit("restored %s" % info["src"])
            n += 1
        os.unlink(p)
    return n


def group_by_file(targets):
    groups = {}
    for t in targets:
        groups.setdefault(t.src, []).append(t)
    return list(groups.values())


def retry_keys(rows, statuses, max_score=None):
    """State keys to process again (--retry): rows whose status is in `statuses`; with `max_score`,
    near rows only when their score is at most that."""
    out = set()
    for key, r in rows.items():
        if r["status"] not in statuses:
            continue
        if r["status"] == "near" and max_score is not None and (r["score"] is None or r["score"] > max_score):
            continue
        out.add(key)
    return out


def run_group(root, out, keep_score, time_budget, items, order=True, regdiff=True, variants=1, replace=(),
              score_jobs=1, exact=False):
    """Pool worker: one C file's targets, in order, with the real backend."""
    run = Run(root, out, keep_score, time_budget, emit=lambda s: print(s, flush=True), order=order,
              regdiff=regdiff, variants=variant_list(variants), replace=replace,
              score_jobs=1 if exact else score_jobs)
    run.backend.score_jobs, run.backend.exact = run.score_jobs, exact
    for item in items:
        run.process(Target(*item))
    return len(items)


def variant_wins(rows):
    """Summary lines: per m2c variant, how many matches and near misses it gave as the best of a
    sweep (rows of runs with --variants above 1 only), in VARIANTS order."""
    wins = {}
    for r in rows:
        if r["status"] in ("match", "near") and r.get("variant") and (r.get("tried") or 1) > 1:
            w = wins.setdefault(r["variant"], [0, 0])
            w[0 if r["status"] == "match" else 1] += 1
    if not wins:
        return []
    order = [v.name for v in VARIANTS]
    keys = sorted(wins, key=lambda n: (order.index(n) if n in order else len(order), n))
    return ["variant wins (match, near):"] + ["  %-16s %d %d" % (n, wins[n][0], wins[n][1]) for n in keys]


def summary(out, emit=print):
    rows = load_state(os.path.join(out, "state.jsonl")).values()
    match = [r for r in rows if r["status"] == "match"]
    near = sorted((r for r in rows if r["status"] == "near"), key=lambda r: (r["score"], r["size"], r["scope"]))
    fails = {}
    for r in rows:
        if r["status"] == "fail":
            kind = r["reason"].split(":")[0]
            fails[kind] = fails.get(kind, 0) + 1
    with open(os.path.join(out, "near.txt"), "w") as fh:
        for r in near:
            fh.write("%d\t0x%X\t%s\t%s\t%s\t%s\n" % (r["score"], r["size"], r["scope"], r["src"], r["draft"],
                                                     r.get("variant") or "-"))
    emit("MATCH %d (0x%X bytes)  near %d  fail %d  (%d processed)" % (
        len(match), sum(r["size"] for r in match), len(near), sum(fails.values()), len(rows)))
    for kind, n in sorted(fails.items(), key=lambda kv: (-kv[1], kv[0])):
        emit("  fail %-12s %d" % (kind, n))
    for line in variant_wins(rows):
        emit(line)
    emit("near misses: %s" % os.path.join(out, "near.txt"))
    return len(match)


def parse_args(argv):
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("--out", default="/scratch/bulk_m2c", help="state, drafts and near.txt (outside the repo)")
    p.add_argument("--max-size", type=lambda s: int(s, 0), default=600, help="bytes, exclusive (default 600)")
    p.add_argument("--unit", default="", help="comma-separated units to work on (default all)")
    p.add_argument("--exclude", default="TT", help="comma-separated units to skip (default TT)")
    p.add_argument("--limit", type=int, help="at most N new targets")
    p.add_argument("--jobs", type=int, default=1, help="parallel jobs, one C file each (default 1)")
    p.add_argument("--keep-score", type=int, default=30, help="climb drafts scoring below this (default 30)")
    p.add_argument("--time", type=float, default=60, help="autotweak seconds per draft (default 60)")
    p.add_argument("--no-order", action="store_true", help="no statement-order stage in the climb (T-11200)")
    p.add_argument("--no-regdiff", action="store_true", help="no regdiff-guided edit order in the climb (T-11220)")
    p.add_argument("--score-jobs", type=int, default=0,
                   help="candidates compiled in parallel per climb (default: (cores - 2) / --jobs; T-11250)")
    p.add_argument("--exact", action="store_true",
                   help="legacy scoring: one candidate at a time through permute.build_base (T-11250)")
    p.add_argument("--variants", type=int, default=1,
                   help="m2c option sets to try per target, the first N of VARIANTS (default 1, max %d)"
                   % len(VARIANTS))
    p.add_argument("--retry", default="", help="comma-separated state statuses to process again (near, fail)")
    p.add_argument("--max-score", type=int, help="with --retry: near entries only when scoring at most this")
    p.add_argument("--summary", action="store_true", help="print the summary of the state file and stop")
    p.add_argument("--root", default=".", help=argparse.SUPPRESS)
    return p.parse_args(argv)


def main(argv=None, backend=None, targets=None, climb=None, emit=print):
    args = parse_args(sys.argv[1:] if argv is None else argv)
    root, out = os.path.abspath(args.root), os.path.abspath(args.out)
    if out == root or out.startswith(root + os.sep):
        emit("bulk_m2c.py: --out must be outside the checkout (got %s)" % args.out)
        return 2
    try:
        os.makedirs(out, exist_ok=True)
    except OSError as exc:
        emit("bulk_m2c.py: cannot create --out %s: %s (in Docker set TOKIMEMO_SCRATCH)" % (args.out, exc))
        return 2
    if args.summary:
        summary(out, emit)
        return 0
    retry = {s for s in args.retry.split(",") if s}
    if retry - {"near", "fail"}:
        emit("bulk_m2c.py: --retry takes near and fail (got %s)" % args.retry)
        return 2
    recover(root, out, emit)
    done = load_state(os.path.join(out, "state.jsonl"))
    again = retry_keys(done, retry, args.max_score)
    if retry:
        emit("retrying %d state entries (%s)" % (len(again), ",".join(sorted(retry))))
    done = {k: r for k, r in done.items() if k not in again}
    units = {u for u in args.unit.split(",") if u}
    exclude = {u for u in args.exclude.split(",") if u}
    if targets is None:
        targets = find_targets(root, args.max_size, units, exclude)
    else:
        targets = [t for t in targets if t.unit not in exclude and (not units or t.unit in units)]
    todo = select(targets, args.max_size, set(done), args.limit)
    emit("%d targets (%d already processed)" % (len(todo), len([t for t in targets if t.key in done])))
    # candidates scored in parallel inside one climb (T-11250); with --jobs the cores are shared
    score_jobs = 1 if args.exact else args.score_jobs or max(1, autotweak.fastscore.default_jobs() // max(1, args.jobs))
    if args.jobs <= 1 or backend is not None:
        run = Run(root, out, args.keep_score, args.time, backend, climb, emit, order=not args.no_order,
                  regdiff=not args.no_regdiff, variants=variant_list(args.variants), replace=again,
                  score_jobs=score_jobs if backend is None else 1)
        if backend is None:
            run.backend.score_jobs, run.backend.exact = score_jobs, args.exact
        for t in todo:
            run.process(t)
    else:
        # tools/queue.py shadows the stdlib queue that concurrent.futures needs (as in tools/permute.py)
        here = os.path.dirname(os.path.abspath(__file__))
        saved = sys.path[:]
        sys.path[:] = [q for q in saved if os.path.abspath(q or '.') != here]
        shadow = sys.modules.pop('queue', None)
        try:
            from concurrent.futures import ProcessPoolExecutor
        finally:
            sys.path[:] = saved
            if shadow is not None and (getattr(shadow, '__file__', None) or '').startswith(here):
                sys.modules['queue'] = shadow
        with ProcessPoolExecutor(max_workers=args.jobs) as pool:
            futures = [pool.submit(run_group, root, out, args.keep_score, args.time,
                                   [t.as_tuple() for t in g], not args.no_order,
                                   not args.no_regdiff, args.variants,
                                   sorted(again & {t.key for t in g}), score_jobs, args.exact)
                       for g in group_by_file(todo)]
            for f in futures:
                f.result()
    summary(out, emit)
    if len(load_state(os.path.join(out, "state.jsonl"))) and any(
            r.get("status") == "match" for k, r in load_state(os.path.join(out, "state.jsonl")).items()
            if k not in done or done[k].get("status") != "match"):
        # matches keep raw GameState globals (D_800E6xxx...): migrate them so build/globals.ok holds
        be = backend or Backend()
        ok, msg = be.migrate_globals(root)
        emit("migrate_globals --apply: %s" % ("ok" if ok else "FAILED " + first_error(msg)))
        if ok:
            ok, msg = be.check_headers(root)
            emit("check_headers: %s" % ("ok" if ok else "FAILED " + first_error(msg)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
