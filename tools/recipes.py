#!/usr/bin/env python3
"""Edit kinds for four recipes that closed wave-3 functions by hand (T-20830).

autotweak.py (T-11170) tries single edits of the cheat sheet. Four recipes were found by hand in
wave 3 and are not edits there; each covers several notes. They are pure text transforms on
autotweak's Func/Variant interface (scoring, exact rescoring and verification stay autotweak's),
registered by `register` at the end of autotweak.py like tools/typefix.py:

    gstate        `GameState *g = &D_800E6280;` among the locals, `D_800E6280.F` -> `g->F` (T-16930
                  idiom, wiki/register-cheatsheet.md row "struct or table field compared constant-first").
                  Variants: every use (declared first, last or between the locals), the uses of the first
                  N statement lines, the uses of one switch arm. No FAKE comment. Nothing when a name `g`
                  exists.
    group         k = 3..8 adjacent simple statements at one indent on one source line (as1 schedules
                  by line), joined with `;` or `,` (the PsyQ macro form), a long run cut into chunks of
                  2..4, and the initial assignments right before a for/while/do put on the loop-head
                  line (T-20620); the reverse splits. Only runs of independent statements
                  (autotweak.independent) are touched.
    chain         consecutive stores of one side-effect-free value, `a = v; b = v; c = v;` ->
                  `a = b = c = v;` (target orders: source, reversed, and all of them up to three), and
                  the reverse. Nothing when two target widths are known to differ or an earlier store
                  may change the value.
    inline_multi  a local assigned once from a global or field read and used 2 or more times is
                  replaced by its source at every use, or at one use with the local kept for the others
                  (nothing in between may store to the source or call); the reverse introduces a local
                  for a read that appears twice or more.

Types come from the locals, the `extern` objects of include/**/*.h and the field names of the structs
there (a field name that two structs declare differently is unknown; an unknown width does not block a
chain, a known difference does). Usage: library only (python3 tools/test_recipes.py for the tests).
"""
import bisect
import glob
import itertools
import os
import re
import sys

sys.path.append(os.path.dirname(os.path.abspath(__file__)))

TICKET = "T-20830"
KINDS = ("gstate", "group", "chain", "inline_multi")   # the first word of every label
STATE = "D_800E6280"            # the GameState aggregate
STATE_PTR = "g"
STATE_DECL = "GameState *g = &D_800E6280;"
GROUP_MIN, GROUP_MAX = 3, 8     # statements joined onto one line
GROUP_WINDOWS = 60              # windows joined per function (run ends first, then by size)
HEAD_MAX = 8                    # initial assignments joined onto a loop head
CHUNKS = (2, 3, 4)
CHAIN_MAX = 6
CHAIN_PERMS = 3                 # all target orders up to this many targets
SUBSETS = (1, 2, 3)             # gstate: first N statement lines
MAX_ARMS = 8
MAX_INTRO = 4                   # inline_multi: read expressions turned into a local
MAX_VARIANTS = {"gstate": 20, "group": 128, "chain": 24, "inline_multi": 12}   # per function
STRICT = False                  # tests: let a bug in a transform raise instead of returning no variant

_AT = None


def at():
    """tools/autotweak.py (imported on first use; `register` sets it when autotweak loads us)."""
    global _AT
    if _AT is None:
        import autotweak
        _AT = autotweak
    return _AT


def variant(text, label):
    return at().Variant(text, label)


# ---------------------------------------------------------------------------------------------
# Types: locals, extern objects and struct fields of include/**/*.h
# ---------------------------------------------------------------------------------------------

WIDTH = {"s8": 1, "u8": 1, "s16": 2, "u16": 2, "s32": 4, "u32": 4}
STRUCT_RE = re.compile(r"\b(?:struct|union)\b[^;{}()]*\{")
FIELD_RE = re.compile(r"^\s*((?:(?:const|volatile|unsigned|signed|struct|union|enum)\s+)*\w+)\s+(\**)\s*(\w+)\s*"
                      r"((?:\[[^\]]*\])*)\s*;\s*$")
NOT_TYPE = {"typedef", "extern", "static", "return", "goto"}


def _body_lines(masked, open_brace):
    """The top-level lines of the struct body whose `{` is at open_brace (nested braces dropped)."""
    depth, out, cur = 0, [], []
    for ch in masked[open_brace:]:
        if ch == "{":
            depth += 1
            if depth > 1:
                cur.append(" ")
            continue
        if ch == "}":
            depth -= 1
            if depth == 0:
                break
            cur.append(" ")
            continue
        if depth == 1:
            cur.append(ch)
            if ch == ";":
                out.append("".join(cur))
                cur = []
    return out


def field_types(root="."):
    """{field: (base, stars, dims)} of the struct and union members declared in include/**/*.h;
    a name that is declared with two different types is left out."""
    import typefix
    seen, bad = {}, set()
    for path in glob.glob(os.path.join(root, "include", "**", "*.h"), recursive=True):
        try:
            with open(path, errors="replace") as fh:
                masked = at().mask(fh.read())
        except OSError:
            continue
        for m in STRUCT_RE.finditer(masked):
            for line in _body_lines(masked, m.end() - 1):
                fm = FIELD_RE.match(" ".join(line.split()))
                if not fm or fm.group(1).split()[0] in NOT_TYPE:
                    continue
                words = [w for w in fm.group(1).split() if w not in typefix.QUAL_WORDS]
                got = (typefix.norm_base(words), len(fm.group(2)), tuple(re.findall(r"\[[^\]]*\]", fm.group(4))))
                if seen.setdefault(fm.group(3), got) != got:
                    bad.add(fm.group(3))
    return {k: v for k, v in seen.items() if k not in bad}


class Types:
    """Types of names and members, as (base, depth) with depth = stars + array dimensions."""

    def __init__(self, names=None, fields=None):
        self.names = {k: (b, s + len(d)) for k, (b, s, d) in (names or {}).items()}
        self.fields = {k: (b, s + len(d)) for k, (b, s, d) in (fields or {}).items()}

    @classmethod
    def load(cls, root="."):
        import typefix
        return cls(typefix.global_types(root), field_types(root))

    def name_type(self, f, name):
        import typefix
        got = f.locals.get(name) or f.params.get(name)
        if got is not None:
            return typefix.norm_base([got[0]]), (1 if got[1] else 0) + (1 if got[2] else 0)
        return self.names.get(name)

    def type_of(self, f, toks):
        """(base, depth) of the expression in `toks` (names, casts, `*`, `&`, `[]`, `.`, `->`), or None."""
        n, i, ops = len(toks), 0, []
        while i < n and toks[i].text in ("*", "&"):
            ops.append(toks[i].text)
            i += 1
        if i >= n:
            return None
        ty = None
        if toks[i].text == "(":
            j = at().match_close(toks, i)
            if j is None:
                return None
            inner = toks[i + 1:j - 1]
            cast = self.cast_type(inner)
            if cast is not None and j < n:
                return self._apply(cast, ops)     # the operand after a cast does not change the type
            ty, i = self.type_of(f, inner), j
        elif toks[i].kind == "id":
            ty, i = self.name_type(f, toks[i].text), i + 1
        else:
            return None
        while i < n:
            x = toks[i].text
            if x == "[":
                j = at().match_close(toks, i)
                ty, i = (ty[0], ty[1] - 1) if ty is not None and j is not None and ty[1] > 0 else None, j or n
            elif x in (".", "->") and i + 1 < n:
                ty, i = self.fields.get(toks[i + 1].text), i + 2     # the member name alone gives the type
            else:
                return None
        return self._apply(ty, ops)

    @staticmethod
    def cast_type(inner):
        """(base, depth) when the parenthesised tokens are a type name, else None."""
        import typefix
        if not inner or not all(t.kind == "id" or t.text == "*" for t in inner):
            return None
        words = [t.text for t in inner if t.kind == "id"]
        stars = sum(t.text == "*" for t in inner)
        base = typefix.norm_base(words)
        if stars or base in typefix.INT_BASES or words[0] in typefix.TYPE_WORDS:
            return base, stars
        return None

    @staticmethod
    def _apply(ty, ops):
        for op in reversed(ops):
            if ty is None:
                return None
            if op == "*":
                if ty[1] <= 0:
                    return None
                ty = (ty[0], ty[1] - 1)
            else:
                ty = (ty[0], ty[1] + 1)
        return ty

    def width(self, f, toks):
        """Byte width of the integer or pointer lvalue in `toks`, or None when unknown."""
        ty = self.type_of(f, toks)
        if ty is None:
            return None
        return 4 if ty[1] > 0 else WIDTH.get(ty[0])

    def decl(self, f, toks):
        """Declaration text for a local that holds the value of `toks`: the exact type when it is a
        pointer or a known integer, else None."""
        ty = self.type_of(f, toks)
        if ty is None or ty[1] > 1:
            return None
        if ty[1] == 1:
            return "%s *" % ty[0]
        return ty[0] if ty[0] in WIDTH else None


TYPES = None


def types(root="."):
    global TYPES
    if TYPES is None:
        TYPES = Types.load(root)
    return TYPES


def set_types(value):
    """Tests: replace the type tables (None reloads them from include/)."""
    global TYPES
    TYPES = value


# ---------------------------------------------------------------------------------------------
# Function layout: braces, loops, and what each line stores and calls
# ---------------------------------------------------------------------------------------------

STOP = {";", "{", "}", ",", "?", ":", "&&", "||", "==", "!=", "<", ">", "<=", ">=", "(", "[", "return",
        "case", "else", "do", "+", "-", "/", "%", "|", "^", "<<", ">>", "!", "~"}
INCDEC = ("++", "--")


def back_match(toks, j):
    """Index of the opening bracket matching the closing toks[j] (`)` or `]`), or None."""
    closing = toks[j].text
    opening = "(" if closing == ")" else "["
    depth = 0
    for m in range(j, -1, -1):
        if toks[m].text == closing:
            depth += 1
        elif toks[m].text == opening:
            depth -= 1
            if depth == 0:
                return m
    return None


def lvalue_before(toks, k):
    """The tokens of the lvalue that ends right before toks[k] (an assignment operator)."""
    j = k - 1
    while j >= 0:
        x = toks[j].text
        if x in (")", "]"):
            m = back_match(toks, j)
            if m is None:
                return None
            j = m - 1
            continue
        if x in STOP or x in at().ASSIGN_OPS:
            break
        j -= 1
    return toks[j + 1:k]


class LV:
    """An lvalue or read expression: its text, root name and member path, whether it is a bare name,
    whether it is a direct path (no pointer dereference) and every name it mentions."""

    __slots__ = ("text", "root", "path", "bare", "direct", "names")

    def __init__(self, text, root, path, bare, direct, names):
        self.text, self.root, self.path, self.bare, self.direct, self.names = text, root, path, bare, direct, names


def make_lv(toks):
    a = at()
    text = "".join(t.text for t in toks)
    names = {t.text for k, t in enumerate(toks)
             if t.kind == "id" and t.text not in a.KEYWORDS and t.text not in a.SCALAR_TYPES
             and not (k and toks[k - 1].text in (".", "->"))}
    unknown = LV(text, None, (), False, False, names)
    i, deref = 0, False
    while i < len(toks) and toks[i].text in ("*", "&"):
        deref |= toks[i].text == "*"
        i += 1
    if i >= len(toks) or toks[i].kind != "id" or toks[i].text in a.KEYWORDS:
        return unknown
    root, path, direct, j = toks[i].text, [], not deref, i + 1
    while j < len(toks):
        x = toks[j].text
        if x in (".", "->") and j + 1 < len(toks):
            path.append((".", toks[j + 1].text))
            direct = direct and x == "."
            j += 2
        elif x == "[":
            e = a.match_close(toks, j)
            if e is None:
                return unknown
            path.append(("[]",))
            j = e
        else:
            return unknown
    return LV(text, root, tuple(path), not path and i == 0, direct, names)


def may_alias(s, r, plain):
    """True when a store to `s` may change the value of the read `r` (both LV). Name based, as
    autotweak.independent is: a store to a plain name changes the reads that mention it; a store through
    a pointer, member or index changes the memory of the names of its left side (the same object:
    the first member where the paths differ decides, two different fields do not overlap)."""
    if s.bare:
        return s.root in r.names
    if s.root is None or r.root is None:
        return bool((s.names & r.names) - plain)
    if s.root != r.root:
        return r.root in s.names and r.root not in plain
    for x, y in zip(s.path, r.path):
        if x == y:
            continue
        return not (x[0] in (".", "->") and y[0] in (".", "->"))
    return True


def plain_locals(f, taken):
    return {n for n, (_t, _p, arr) in list(f.locals.items()) + list(f.params.items()) if not arr} - taken


class Layout:
    """Per-function tables shared by the transforms: token lines, brace depth, loops, and the
    stores and calls of each line."""

    def __init__(self, f):
        a = at()
        self.f = f
        t = f.toks
        starts, off = [], 0
        for l in f.lines:
            starts.append(off)
            off += len(l) + 1
        self.tok_line = [bisect.bisect_right(starts, tok.start) - 1 for tok in t]
        self.first_tok = {}
        for k, l in enumerate(self.tok_line):
            self.first_tok.setdefault(l, k)
        self.depth, self.match, stack, d = [], {}, [], 0
        for k, tok in enumerate(t):
            self.depth.append(d)
            if tok.text == "{":
                d += 1
                stack.append(k)
            elif tok.text == "}":
                d -= 1
                if stack:
                    self.match[stack.pop()] = k
        lo, hi = f.body_tokens()
        self.has_goto = any(t[k].text == "goto" for k in range(lo, hi))
        self.taken = a.address_taken(f)
        self.plain = plain_locals(f, self.taken)
        self.loops = self._loops(lo, hi)
        self._ltoks, self._effects = {}, {}

    def _loops(self, lo, hi):
        a, t, out = at(), self.f.toks, []
        for k in range(lo, hi):
            word = t[k].text
            if word not in ("for", "while", "do"):
                continue
            if word == "do":
                e = k + 1
            else:
                if k + 1 >= hi or t[k + 1].text != "(":
                    continue
                e = a.match_close(t, k + 1)
                if e is None:
                    continue
                if word == "while" and (e < len(t) and t[e].text == ";"):
                    continue                    # the tail of a do-while, or an empty loop
            end = None
            if e < len(t) and t[e].text == "{" and e in self.match:
                end = self.match[e]
                if word == "do" and end + 1 < len(t) and t[end + 1].text == "while":
                    tail = a.match_close(t, end + 2) if end + 2 < len(t) else None
                    end = tail if tail is not None and tail < len(t) else end
            else:
                semi = next((j for j in range(e, hi) if t[j].text == ";" and self.depth[j] == self.depth[k]), None)
                end = semi
            if end is not None:
                out.append((self.tok_line[k], self.tok_line[min(end, len(t) - 1)]))
        return out

    def line_tokens(self, i):
        if i not in self._ltoks:
            self._ltoks[i] = at().tokenize(self.f.mlines[i])
        return self._ltoks[i]

    def dominated(self, a_tok, u_tok):
        """True when the code at token u_tok runs only after the code at a_tok (it stays inside
        the block that holds a_tok: the brace depth never drops below the depth at a_tok)."""
        if u_tok < a_tok:
            return False
        return min(self.depth[a_tok:u_tok + 1]) >= self.depth[a_tok]

    def effects(self, i):
        """(stores [LV], has call) of line i."""
        if i not in self._effects:
            a = at()
            toks = self.line_tokens(i)
            stores = []
            for k, tok in enumerate(toks):
                if tok.text in a.ASSIGN_OPS:
                    lv = lvalue_before(toks, k)
                elif tok.text in INCDEC:
                    postfix = k > 0 and (toks[k - 1].kind in ("id", "num") or toks[k - 1].text in (")", "]"))
                    if postfix:
                        lv = lvalue_before(toks, k)
                    else:
                        e = a.operand_end(toks, k + 1)
                        lv = toks[k + 1:e] if e is not None else None
                else:
                    continue
                stores.append(make_lv(lv) if lv else LV("", None, (), False, False, set()))
            self._effects[i] = (stores, a._is_call(toks))
        return self._effects[i]

    def blocked(self, reads, lo, hi):
        """True when a line of [lo, hi) calls something or stores to something that may change one
        of the reads (LV)."""
        for i in range(max(lo, self.f.stmt_line), min(hi, self.f.end_line)):
            stores, call = self.effects(i)
            if call or any(may_alias(s, r, self.plain) for s in stores for r in reads):
                return True
        return False

    def extend_over_loops(self, first, last):
        """`last`, moved to the end of any loop that starts after line `first` and holds `last`."""
        for h, e in self.loops:
            if first < h <= last <= e:
                last = max(last, e)
        return last


def reads_of(toks):
    """[LV]: the maximal postfix expressions in `toks` that read a named object."""
    a = at()
    out, k = [], 0
    while k < len(toks):
        tok = toks[k]
        if tok.kind == "id" and tok.text not in a.KEYWORDS and not (k and toks[k - 1].text in (".", "->")):
            if k + 1 < len(toks) and toks[k + 1].text == "(":
                k += 1
                continue
            e = a.operand_end(toks, k)
            if e is None:
                k += 1
                continue
            out.append(make_lv(toks[k:e]))
            k = e
            continue
        k += 1
    return out


def pure(toks):
    """True when the expression has no call, increment or assignment."""
    a = at()
    return not a._is_call(toks) and not any(t.text in INCDEC or t.text in a.ASSIGN_OPS for t in toks)


def capped(kind, *lists):
    """The variants of the lists taken in turn (one of each, then the next), at most MAX_VARIANTS[kind]."""
    out = []
    for k in range(max((len(l) for l in lists), default=0)):
        out += [l[k] for l in lists if k < len(l)]
    return out[:MAX_VARIANTS[kind]]


def guarded(fn):
    """A transform that never stops a long run: a bug returns no variant (tests set STRICT)."""
    def wrapper(f):
        try:
            return fn(f)
        except (ValueError, IndexError, KeyError, StopIteration, TypeError, AttributeError):
            if STRICT:
                raise
            return []
    wrapper.__name__, wrapper.__doc__ = fn.__name__, fn.__doc__
    return wrapper


# ---------------------------------------------------------------------------------------------
# gstate: GameState *g = &D_800E6280; and g->F
# ---------------------------------------------------------------------------------------------

def switch_arms(f):
    """[(start, end)] text offsets of every switch arm (a run of labels and its statements)."""
    a, t, out = at(), f.toks, []
    lo, hi = f.body_tokens()
    for k in range(lo, hi - 1):
        if t[k].text != "switch" or t[k + 1].text != "(":
            continue
        e = a.match_close(t, k + 1)
        if e is None or e >= hi or t[e].text != "{":
            continue
        depth, starts, close = 0, [], None
        for j in range(e, hi):
            x = t[j].text
            if x == "{":
                depth += 1
            elif x == "}":
                depth -= 1
                if depth == 0:
                    close = j
                    break
            elif depth == 1 and x in ("case", "default") and t[j - 1].text != ":":
                starts.append(j)
        if close is None:
            continue
        bounds = [t[j].start for j in starts] + [t[close].start]
        out += list(zip(bounds, bounds[1:]))
    return out


@guarded
def t_gstate(f):
    """`GameState *g = &D_800E6280;` and `g->F` for the reads of `D_800E6280.F`."""
    t = f.toks
    lo, hi = f.body_tokens()
    if STATE_PTR in f.locals or STATE_PTR in f.params:
        return []
    if any(tok.text == STATE_PTR and tok.kind == "id" and not (k and t[k - 1].text in (".", "->"))
           for k, tok in enumerate(t)):
        return []
    uses = []
    for k in range(lo, hi - 2):
        if t[k].text == STATE and t[k + 1].text == "." and t[k + 2].kind == "id" and \
                not (k and t[k - 1].text in (".", "->")):
            line = f.line_of(t[k].start)
            if line >= f.stmt_line:
                uses.append((t[k].start, t[k + 1].end, line))
    if not uses:
        return []
    subsets, seen = [], set()

    def add(label, sub, pos=0):
        key = (tuple(sub), pos)
        if sub and key not in seen:
            seen.add(key)
            subsets.append((label, sub, pos))

    n = len(f.decl_lines)
    add("all", uses)
    if n:
        add("all last", uses, n)
    lines = sorted({u[2] for u in uses})
    for k in sorted({*SUBSETS, (len(lines) + 1) // 2}):
        if 0 < k < len(lines):
            add("first%d" % k, [u for u in uses if u[2] in lines[:k]])
    arms = 0
    for s, e in switch_arms(f):
        sub = [u for u in uses if s <= u[0] < e]
        if sub and len(sub) < len(uses) and arms < MAX_ARMS:
            arms += 1
            add("arm L%d" % f.line_of(s), sub)
    for pos in range(1, n):                     # between the locals: the declaration order is part of the recipe
        add("all at%d" % pos, uses, pos)
    out = []
    for label, sub, pos in subsets:
        text = at().splice(f.text, [(s, e, STATE_PTR + "->") for s, e, _l in sub])
        lines_ = text.split("\n")
        if n:
            lines_.insert(f.decl_lines[pos] if pos < n else f.decl_lines[-1] + 1,
                          f.indent(f.decl_lines[0]) + STATE_DECL)
        else:
            lines_, _pos = f.insert_decl(lines_, STATE_DECL)
        out.append(variant("\n".join(lines_), "gstate " + label))
    return out[:MAX_VARIANTS["gstate"]]


# ---------------------------------------------------------------------------------------------
# group: several statements on one source line
# ---------------------------------------------------------------------------------------------

def _simple_segments(f):
    """[[line]]: runs of line-adjacent simple statements at one indent."""
    a = at()
    segs, cur = [], []
    for i in range(f.stmt_line, f.end_line):
        if a.simple_statement(f, i) and (not cur or (i == cur[-1] + 1 and f.indent(i) == f.indent(cur[0]))):
            cur.append(i)
            continue
        if cur:
            segs.append(cur)
        cur = [i] if a.simple_statement(f, i) else []
    if cur:
        segs.append(cur)
    return segs


def _independent_windows(seg, acc, kmin, kmax):
    a = at()
    out = []
    for s in range(len(seg)):
        for e in range(s + 1, min(len(seg), s + kmax)):
            if not all(a.independent(acc[seg[j]], acc[seg[e]]) for j in range(s, e)):
                break
            if e - s + 1 >= kmin:
                out.append((seg[s], seg[e]))
    return out


def _independent_runs(seg, acc, cap=48):
    a = at()
    runs, cur = [], []
    for i in seg:
        if cur and len(cur) < cap and all(a.independent(acc[j], acc[i]) for j in cur):
            cur.append(i)
        else:
            if cur:
                runs.append(cur)
            cur = [i]
    if cur:
        runs.append(cur)
    return runs


def _code(f, i):
    """The statement of line i without its `;` and trailing comment."""
    return f.lines[i][:f.mlines[i].rfind(";")].strip()


def _joined(f, idx, sep):
    """The one-line text for the simple statements on lines `idx`, joined with `;` or `,`."""
    if sep == ";":
        return f.indent(idx[0]) + " ".join([f.lines[idx[0]].strip()] + [f.lines[i].strip() for i in idx[1:]])
    codes = [_code(f, i) for i in idx[:-1]] + [f.lines[idx[-1]].strip()]
    return f.indent(idx[0]) + ", ".join(codes)


def _comment_free(f, idx):
    """No `//` comment before the last line (it would swallow the joined statements)."""
    return not any("//" in f.lines[i] for i in idx[:-1])


def _replace_lines(f, first, last, new_lines):
    lines = list(f.lines)
    lines[first:last + 1] = new_lines
    return "\n".join(lines)


HEAD_WORD = re.compile(r"(for|while)\s*\(|do\b")


def _head_split(f, i):
    """[(start, end)] of the `;` statements before the loop keyword on line i (empty when the line
    starts with it), or None when the line is not a for/while/do head at a statement start."""
    ml, spans, pos = f.mlines[i], [], 0
    while True:
        rest = ml[pos:].lstrip()
        start = len(ml) - len(rest)
        m = HEAD_WORD.match(rest)
        if m:
            break
        depth, end = 0, None
        for k in range(start, len(ml)):
            c = ml[k]
            if c in "([":
                depth += 1
            elif c in ")]":
                depth -= 1
            elif c in "{}":
                return None
            elif c == ";" and depth == 0:
                end = k + 1
                break
        if end is None:
            return None
        spans.append((start, end))
        pos = end
    if m.group(1) == "while" and rest.rstrip().endswith(";"):
        return None                         # the tail of a do-while, or an empty loop
    first = spans[0][0] if spans else start
    if f.mlines[i][:first].strip():
        return None
    j = i - 1
    while j > f.brace_line and not f.mlines[j].strip():
        j -= 1
    if j != f.brace_line and not f.mlines[j].rstrip().endswith(at().STATEMENT_ENDS):
        return None
    return spans


def group_joins(f, acc):
    """([window joins], [chunked joins of a long run])."""
    out, chunked = [], []
    wins = []
    for seg in _simple_segments(f):
        wins += _independent_windows(seg, acc, GROUP_MIN, GROUP_MAX)
    anchors = set()                    # the first and last line of every maximal run of independent statements
    for seg in _simple_segments(f):
        for run in _independent_runs(seg, acc):
            anchors |= {run[0], run[-1]}
    order = {k: n for n, k in enumerate((GROUP_MAX, GROUP_MIN, 4, 6, 5, 7))}
    wins.sort(key=lambda w: (not (w[0] in anchors or w[1] in anchors), order[w[1] - w[0] + 1], w[0]))
    for lo, hi in wins[:GROUP_WINDOWS]:
        idx = list(range(lo, hi + 1))
        if not _comment_free(f, idx):
            continue
        for sep in (";", ","):
            out.append(variant(_replace_lines(f, lo, hi, [_joined(f, idx, sep)]), "group join%s L%d-%d" % (
                "" if sep == ";" else ",", lo, hi)))
    for seg in _simple_segments(f):
        for run in _independent_runs(seg, acc):
            for k in CHUNKS:
                if len(run) < 2 * k:
                    continue
                chunks = [run[j:j + k] for j in range(0, len(run), k)]
                chunks = [c for c in chunks if len(c) >= 2 and _comment_free(f, c)]
                if len(chunks) < 2:
                    continue
                for sep in (";", ","):
                    lines = list(f.lines)
                    for c in reversed(chunks):
                        lines[c[0]:c[-1] + 1] = [_joined(f, c, sep)]
                    chunked.append(variant("\n".join(lines), "group chunks%d%s L%d-%d" % (
                        k, "" if sep == ";" else ",", run[0], run[-1])))
    return out, chunked


def group_heads(f, acc):
    """The initial assignments right before a loop, put on the loop-head line."""
    a = at()
    out = []
    for h in range(f.stmt_line, f.end_line):
        if _head_split(f, h) != []:
            continue
        run = []
        i = h - 1
        while i >= f.stmt_line and len(run) < HEAD_MAX and a.simple_statement(f, i) and \
                f.indent(i) == f.indent(h) and all(a.independent(acc[j], acc[i]) for j in run):
            run.insert(0, i)
            i -= 1
        for k in [len(run)] + list(range(1, len(run))):
            if k < 1:
                continue
            idx = run[len(run) - k:]
            if not _comment_free(f, idx + [h]):
                continue
            text = f.indent(idx[0]) + " ".join([f.lines[j].strip() for j in idx] + [f.lines[h].strip()])
            out.append(variant(_replace_lines(f, idx[0], h, [text]), "group head L%d (%d)" % (h, k)))
    return out


def _depth0(ml, ch):
    """Positions of `ch` at paren depth 0 in a masked line."""
    depth, out = 0, []
    for k, c in enumerate(ml):
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        elif c == ch and depth == 0:
            out.append(k)
    return out


def group_splits(f, acc):
    """The reverse: a line of several statements back to fewer per line, a comma line to `;`, and
    the statements before a loop head off the head."""
    a = at()
    out = []
    for i in range(f.stmt_line, f.end_line):
        ml, s = f.mlines[i], f.mlines[i].strip()
        spans = _head_split(f, i)
        if spans is not None:
            if spans:
                ind = f.indent(i)
                last = spans[-1][1]
                head = f.lines[i][last:].strip()
                parts = [ind + f.lines[i][x:y].strip() for x, y in spans]
                out.append(variant(_replace_lines(f, i, i, parts + [ind + head]), "group unhead L%d" % i))
                if len(spans) > 1:
                    first = f.lines[i][spans[0][0]:spans[0][1]].strip()
                    rest = f.lines[i][spans[1][0]:].strip()
                    out.append(variant(_replace_lines(f, i, i, [ind + first, ind + rest]), "group unhead1 L%d" % i))
            continue
        if re.match(r"(for|if|while|else|switch|do|return|break|continue|goto|case|default)\b", s):
            continue
        spans = a.split_statements(ml)
        if spans and GROUP_MIN <= len(spans) <= GROUP_MAX and not any(
                re.match(r"\s*(if|for|while|else|switch|do|return|break|continue|goto)\b", ml[x:y]) for x, y in spans):
            ind = f.indent(i)
            for j in range(1, len(spans)):
                cut = spans[j - 1][1]
                first = ind + f.lines[i][spans[0][0]:cut].strip()
                rest = ind + f.lines[i][spans[j][0]:].strip()
                out.append(variant(_replace_lines(f, i, i, [first, rest]), "group split L%d @%d" % (i, j)))
        elif a.simple_statement(f, i):
            commas = _depth0(ml[:ml.rfind(";")], ",")
            if commas and "?" not in ml:
                code = f.lines[i][:ml.rfind(";")]
                tail = f.lines[i][ml.rfind(";"):]
                cuts = [0] + [c + 1 for c in commas] + [len(code)]
                parts = [code[cuts[j]:cuts[j + 1] - (1 if j < len(commas) else 0)].strip() for j in range(len(cuts) - 1)]
                ind = f.indent(i)
                out.append(variant(_replace_lines(f, i, i, [ind + "; ".join(parts) + tail]), "group semi L%d" % i))
                lines = [ind + p + ";" for p in parts]
                lines[-1] = lines[-1][:-1] + tail
                out.append(variant(_replace_lines(f, i, i, lines), "group split L%d" % i))
    return out


@guarded
def t_group(f):
    """Statement runs on one line (and back), and loop-head initial assignments."""
    a = at()
    taken = a.address_taken(f)
    acc = {i: a.access(f, i, taken) for seg in _simple_segments(f) for i in seg}
    joins, chunked = group_joins(f, acc)
    return capped("group", joins, chunked, group_heads(f, acc), group_splits(f, acc))


# ---------------------------------------------------------------------------------------------
# chain: a = b = c = v;
# ---------------------------------------------------------------------------------------------

LVALUE_BAD = {"+", "-", "/", "%", "<<", ">>", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "?", ":", ",", "|", "^", "!",
              "~", ";", "{", "}", "++", "--"}


def split_assign(toks, start=0):
    """(lvalue tokens, index of the `=`) of the assignment that starts at toks[start], or None. The
    lvalue is a name with `.`, `->`, `[]` and a prefix of `*`, `&` and casts."""
    a = at()
    depth = 0
    for k in range(start, len(toks)):
        x = toks[k].text
        if x in ("(", "["):
            depth += 1
        elif x in (")", "]"):
            depth -= 1
        elif depth == 0 and x == "=":
            return (toks[start:k], k) if k > start else None
        elif depth == 0 and (x in LVALUE_BAD or x in a.ASSIGN_OPS or x in a.KEYWORDS and x not in a.SCALAR_TYPES):
            return None
        if depth < 0:
            return None
    return None


def store_parts(f, lay, i):
    """(lvalue tokens, rhs tokens) of the simple statement on line i of the form `LV = RHS;`."""
    toks = lay.line_tokens(i)
    got = split_assign(toks)
    if got is None or toks[-1].text != ";" or got[1] >= len(toks) - 2:
        return None
    return got[0], toks[got[1] + 1:-1]


def chain_parts(f, lay, i):
    """([lvalue tokens], rhs tokens) of a line `a = b = v;` with 2 or more targets, else None."""
    toks = lay.line_tokens(i)
    if not toks or toks[-1].text != ";":
        return None
    targets, pos = [], 0
    while True:
        got = split_assign(toks, pos)
        if got is None:
            break
        targets.append(got[0])
        pos = got[1] + 1
    rhs = toks[pos:-1]
    return (targets, rhs) if len(targets) >= 2 and rhs else None


def _norm(toks):
    return "".join(t.text for t in toks)


def _slice(f, i, toks):
    return f.lines[i][toks[0].start:toks[-1].end]


def widths_ok(f, targets):
    """False when two of the target lvalues have known, different widths."""
    ws = {types().width(f, tk) for tk in targets}
    ws.discard(None)
    return len(ws) <= 1


def stores_clear(lay, lvs, rhs):
    """True when none of the stores `lvs` may change the value of `rhs`."""
    reads = reads_of(rhs)
    return not any(may_alias(s, r, lay.plain) for s in lvs for r in reads)


def _orders(n):
    ident = list(range(n))
    if n <= CHAIN_PERMS:
        return [list(p) for p in itertools.permutations(ident)]
    return [ident, ident[::-1], ident[1:] + ident[:1], ident[-1:] + ident[:-1]]


def chain_joins(f, lay):
    out = []
    lines = list(range(f.stmt_line, f.end_line))
    runs, cur = [], []
    a = at()
    for i in lines:
        parts = store_parts(f, lay, i) if a.simple_statement(f, i) else None
        ok = parts is not None and pure(parts[1]) and pure(parts[0])
        if ok and cur and i == cur[-1][0] + 1 and f.indent(i) == f.indent(cur[0][0]) and \
                _norm(parts[1]) == _norm(cur[0][2]):
            cur.append((i, parts[0], parts[1]))
            continue
        if len(cur) >= 2:
            runs.append(cur)
        cur = [(i, parts[0], parts[1])] if ok else []
    if len(cur) >= 2:
        runs.append(cur)
    for run in runs[:6]:
        n = len(run)
        wins = [(0, n)]
        if n >= 3:
            wins += [(0, n - 1), (1, n)]
        for s, e in wins:
            win = run[s:e]
            if len(win) > CHAIN_MAX or len({_norm(w[1]) for w in win}) != len(win):
                continue
            if not widths_ok(f, [w[1] for w in win]):
                continue
            lvs = [make_lv(w[1]) for w in win[:-1]]
            if not stores_clear(lay, lvs, win[0][2]):
                continue
            first, last = win[0][0], win[-1][0]
            rhs_text = _slice(f, first, win[0][2])
            targets = [_slice(f, w[0], w[1]) for w in win]
            for order in _orders(len(win)):
                new = f.indent(first) + " = ".join([targets[j] for j in order] + [rhs_text]) + ";"
                tag = "" if order == list(range(len(win))) else " %s" % "".join(str(j) for j in order)
                out.append(variant(_replace_lines(f, first, last, [new]), "chain join L%d-%d%s" % (first, last, tag)))
    return out


def chain_splits(f, lay):
    out = []
    a = at()
    for i in range(f.stmt_line, f.end_line):
        if not a.simple_statement(f, i):
            continue
        parts = chain_parts(f, lay, i)
        if parts is None:
            continue
        targets, rhs = parts
        if not pure(rhs) or not all(pure(tk) for tk in targets) or not widths_ok(f, targets):
            continue
        if not stores_clear(lay, [make_lv(tk) for tk in targets[:-1]], rhs):
            continue
        ind, rhs_text = f.indent(i), _slice(f, i, rhs)
        texts = [_slice(f, i, tk) for tk in targets]
        for order, tag in ((texts[::-1], "rtl"), (texts, "ltr")):
            lines = ["%s%s = %s;" % (ind, tg, rhs_text) for tg in order]
            out.append(variant(_replace_lines(f, i, i, lines), "chain split L%d %s" % (i, tag)))
    return out


@guarded
def t_chain(f):
    """`a = v; b = v; c = v;` <-> `a = b = c = v;`."""
    lay = Layout(f)
    return capped("chain", chain_joins(f, lay), chain_splits(f, lay))


# ---------------------------------------------------------------------------------------------
# inline_multi: a multi-use local <-> its source
# ---------------------------------------------------------------------------------------------

def _is_memory_read(f, toks):
    """True when the expression reads a global or a field or element (not just locals)."""
    a = at()
    names = f.locals.keys() | f.params.keys()
    if any(t.text in ("->", "[", "*") for t in toks):
        return True
    for k, t in enumerate(toks):
        if t.kind == "id" and t.text not in a.KEYWORDS and t.text not in a.SCALAR_TYPES and t.text not in names \
                and not (k and toks[k - 1].text in (".", "->")):
            return True
    return False


def _wrap(src_toks, text):
    """`text` ready to stand in an expression: parenthesised unless it is a name or a postfix chain."""
    a = at()
    if src_toks and src_toks[0].kind in ("id", "num") and a.operand_end(src_toks, 0) == len(src_toks):
        return text
    return "(" + text + ")"


def inline_forward(f, lay):
    a = at()
    out = []
    t = f.toks
    lo, hi = f.body_tokens()
    for d in f.decl_lines:
        m = a.DECL_RE.match(f.mlines[d])
        if not m or m.group(4) or m.group(5) or m.group(6) or m.group(3) in lay.taken:
            continue
        name = m.group(3)
        occ = [k for k in range(lo, hi) if t[k].text == name and t[k].kind == "id" and lay.tok_line[k] >= f.stmt_line
               and not (k and t[k - 1].text in (".", "->"))]
        stores = [k for k in occ if (k + 1 < len(t) and (t[k + 1].text in a.ASSIGN_OPS or t[k + 1].text in INCDEC))
                  or (k and t[k - 1].text in INCDEC)]
        if len(stores) != 1:
            continue
        s = stores[0]
        al = lay.tok_line[s]
        lt = lay.line_tokens(al)
        if not (a.simple_statement(f, al) and len(lt) > 3 and lt[0].text == name and lt[1].text == "="
                and lt[-1].text == ";" and lay.first_tok[al] == s):
            continue
        rhs = lt[2:-1]
        if not pure(rhs) or not _is_memory_read(f, rhs) or any(x.text == name for x in rhs):
            continue
        uses = [k for k in occ if k != s]
        if len(uses) < 2 or any(lay.tok_line[k] <= al for k in uses):
            continue
        reads = reads_of(rhs)
        # a use can read the source directly when the assignment dominates it and nothing between
        # the two stores to or calls what the source reads (a use inside a loop counts the whole loop)
        ok = [k for k in uses if lay.dominated(s, k)
              and not lay.blocked(reads, al + 1, lay.extend_over_loops(al, lay.tok_line[k]))]
        src = f.lines[al][rhs[0].start:rhs[-1].end]
        rep = _wrap(rhs, src)
        if len(ok) == len(uses):
            text = a.splice(f.text, [(t[k].start, t[k].end, rep) for k in uses])
            lines = text.split("\n")
            for k in sorted((d, al), reverse=True):
                del lines[k]
            out.append(variant("\n".join(lines), "inline_multi %s L%d" % (name, al)))
        for n, k in enumerate(uses, 1):          # one use read directly, the local stays for the others
            if k in ok:
                out.append(variant(a.splice(f.text, [(t[k].start, t[k].end, rep)]),
                                   "inline_multi %s L%d use%d" % (name, al, n)))
    return out


def read_chains(f, lay):
    """{text: [(first token, end token, line, LV)]}: the reads of globals and fields, in source order."""
    a = at()
    t = f.toks
    lo, hi = f.body_tokens()
    names = f.locals.keys() | f.params.keys()
    out = {}
    k = lo
    while k < hi:
        tok = t[k]
        line = lay.tok_line[k]
        if line < f.stmt_line or tok.kind != "id" or tok.text in a.KEYWORDS or tok.text in a.SCALAR_TYPES \
                or (k and t[k - 1].text in (".", "->")):
            k += 1
            continue
        if k + 1 < hi and t[k + 1].text == "(":
            k += 1
            continue
        e = a.operand_end(t, k)
        if e is None or any(t[j].text == "(" for j in range(k, e)):
            k += 1
            continue
        prev = t[k - 1].text if k else ""
        nxt = t[e].text if e < len(t) else ""
        skip = (prev == "&" and not a.binary_amp(t, k - 1)) or prev in INCDEC or prev == "sizeof" \
            or nxt in a.ASSIGN_OPS or nxt in INCDEC       # an address, a store target
        lv = make_lv(t[k:e])
        memory = bool(lv.path) or (tok.text not in names and (tok.text.startswith("D_") or tok.text in types().names))
        if not skip and memory and lv.root is not None:
            out.setdefault(lv.text, []).append((k, e, line, lv))
        k = e
    return out


def inline_intro(f, lay):
    a = at()
    out = []
    t = f.toks
    groups = []
    for text, occ in read_chains(f, lay).items():
        if len(occ) < 2:
            continue
        i = occ[0][2]
        if not f.statement_start(i):
            continue
        ft = lay.first_tok[i]
        chosen = [occ[0]]
        for o in occ[1:]:
            last = lay.extend_over_loops(i - 1, o[2])        # a loop that starts at line i counts whole too
            if not lay.dominated(ft, o[0]) or lay.blocked([o[3]], i, last):
                break
            chosen.append(o)
        if len(chosen) >= 2:
            groups.append((-len(chosen), i, text, chosen))
    for _n, i, text, chosen in sorted(groups)[:MAX_INTRO]:
        name = f.unique("temp")
        first = chosen[0]
        src = f.text[t[first[0]].start:t[first[1] - 1].end]
        decls = ["s32"]
        exact = types().decl(f, t[first[0]:first[1]])
        if exact and exact not in decls:
            decls.append(exact)
        for typ in decls:
            body = a.splice(f.text, [(t[o[0]].start, t[o[1] - 1].end, name) for o in chosen])
            lines = body.split("\n")
            lines.insert(i, "%s%s = %s;" % (f.indent(i), name, src))
            lines, _pos = f.insert_decl(lines, "%s%s;" % (typ if typ.endswith("*") else typ + " ", name), at_end=True)
            out.append(variant("\n".join(lines), "inline_multi intro %s L%d" % (text, i)))
    return out


@guarded
def t_inline_multi(f):
    """A local read twice or more <-> the global or field read it copies."""
    lay = Layout(f)
    if lay.has_goto:
        return []
    return capped("inline_multi", inline_forward(f, lay), inline_intro(f, lay))


# ---------------------------------------------------------------------------------------------
# Registration
# ---------------------------------------------------------------------------------------------

TRANSFORMS = (t_gstate, t_group, t_chain, t_inline_multi)

# Hint class -> kinds appended to autotweak's HINT_EDITS (the cheat-sheet rows name them)
HINT_ADD = {
    "tie": ("group",),
    "ugen": ("group", "chain"),
    "priority": ("gstate", "inline_multi"),
    "structure": ("gstate",),
    "argument": ("gstate",),
}


def register(module):
    """Add the edits to autotweak's list (`module` is the loaded autotweak) and to the kinds its
    regdiff hints try first."""
    global _AT
    _AT = module
    if not all(t in module.TRANSFORMS for t in TRANSFORMS):
        module.TRANSFORMS = tuple(module.TRANSFORMS) + TRANSFORMS
    for hint, add in HINT_ADD.items():
        kinds = tuple(module.HINT_EDITS.get(hint, ()))
        module.HINT_EDITS[hint] = kinds + tuple(k for k in add if k not in kinds)
