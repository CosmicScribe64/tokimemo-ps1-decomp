#!/usr/bin/env python3
"""Try the register cheat sheet's known C edits on guarded near-miss drafts (T-11170).

Usage (in Docker, repo root as cwd):
    python3 tools/autotweak.py [UNIT:]func_X [--time 300]          # one draft
    python3 tools/autotweak.py --all [--exclude TT] [--time 120]     # every guarded draft
    python3 tools/autotweak.py --list [--all | UNIT:func_X]          # drafts and edit counts, no compile

A draft is a function under `#ifdef NON_MATCHING ... #else INCLUDE_ASM(...) #endif` in src/.
Instead of the permuter's random rewrites, the tool applies the deterministic text edits of
wiki/register-cheatsheet.md to the one function body:

    byteview   switch (D_X)            -> switch (*(u8 *)&D_X)               FAKE
    swap       a + b, a == b, a != b   -> b + a, ...
    lines      merge two simple statements onto one line, or split them
    compound   x = x + k               <-> x += k   (also -, |, &, ^)
    emptytest  if (v) {} before a statement that uses local v                 FAKE
    pad        an unused s32 local at the top or the end of the locals        FAKE
    declswap   swap two adjacent local declarations
    shift      a * 2^n                 <-> a << n
    temp       inline a single-use temporary, or introduce one (assignment
               right side, switch selector, left side of an if comparison)
    width      s16/u16/s32/u32 and u8/s8 of a local or parameter
    order      swap two adjacent independent simple statements (T-11200)

Scoring is the permuter's (tools/permute.py setup + upstream Compiler/Scorer): only the one
function is compiled, through tools/cc.py, against the original bytes of its splat .s. The
search is a greedy hill climb: every single edit of the current best text is scored; the best
improvement is kept; when no single edit improves, pairs of edits (depth 2) are tried from the
best-scoring single edits. --time bounds the search per function.

When neither improves and the score is above 0, the order stage (T-11200, --no-order to skip)
finds runs of 2..6 consecutive simple statements at one indent that are pairwise independent
(conservative token check: a store through a pointer, member, index or global writes every name
of its left side; a call only moves past statements on plain locals), marks the best one or two
runs (most stores first) as `/* ORDER */` blocks and up to --order-swaps operand pairs of `+`,
`==`, `!=` as ORDER_SWAP, and searches them with tools/order_search.py's search (diman8, T-11530)
in-process, scored by the same compile. The variant count stays within --max-exhaustive as in
order_search.py. The marked text exists only in memory; only rendered variants, which carry no
marker, reach the scorer or a file.

When the traced uopt of tools/regdiff.py is available ($REGDIFF_UOPT or its default under
/scratch, i.e. TOKIMEMO_SCRATCH set; T-11220, --no-regdiff to skip), regdiff runs in-process on
the start text and on every new best text. Its FIRST DIVERGENCE gives a hint class (tie, priority,
extra-range, missing-range, ugen, ...; HINT_EDITS maps each to cheat-sheet edits) and the named
variables and lines. The climb then scores the suggested kinds that touch those names or lines
first, the other suggested kinds next, then edits at that location, then the rest; nothing is
dropped, so --cap and --time bound the order only. The output line ends with `regdiff: <hint>`
for the best text. Without the traced uopt the search is the unguided one.

A score of 0 is checked like a permuter output (permute.verify_candidate: ninja object,
funcdiff --resolve, unit sha1). Only then the guard block is replaced by the plain function
(INCLUDE_RODATA lines and FAKE comments of the block are kept, the NON_MATCHING note is
dropped) and `MATCH UNIT:func` is printed. Edits marked FAKE above carry an inline
`/* FAKE: ... T-11170 */` comment. --dry-run reports without writing. The best non-matching
text is saved as build/autotweak/<unit>/<func>/best.c. A draft that scores 0 but fails the
check is not a match: the search goes on for a variant that passes, and the line ends with
`(score 0 failed verification)` when none does.
"""
import argparse
import contextlib
import glob
import io
import os
import re
import sys
import tempfile
import threading
import time

sys.path.append(os.path.dirname(os.path.abspath(__file__)))
import fastscore  # noqa: E402

TICKET = "T-11170"
WORK_ROOT = os.path.join("build", "autotweak")

# ---------------------------------------------------------------------------------------------
# Lexing helpers (text spans stay aligned with the source: masking keeps every offset)
# ---------------------------------------------------------------------------------------------

KEYWORDS = {
    "if", "else", "for", "while", "do", "switch", "case", "default", "return", "break",
    "continue", "goto", "sizeof", "struct", "union", "enum", "typedef", "static", "extern",
    "register", "volatile", "const", "unsigned", "signed", "int", "char", "short", "long",
    "void", "float", "double",
}
SCALAR_TYPES = {"s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "int", "char", "short",
                "long", "unsigned", "signed"}
WIDTHS = {"s16": ("u16", "s32"), "u16": ("s16", "s32"), "s32": ("s16", "u16", "u32"),
          "u32": ("s32",), "u8": ("s8",), "s8": ("u8",)}
TYPE_PREFIX = r"(?:(?:register|static|volatile|const|unsigned|signed|struct|union|enum)\s+)*"
DECL_RE = re.compile(r"^\s*" + TYPE_PREFIX + r"(\w+)(\s+\**\s*|\s*\*+\s*)(\w+)\s*(\[[^\]]*\])?\s*(=[^;]*)?(,[^;]*)?;\s*$")
NOT_DECL_HEADS = {"return", "goto", "case", "else", "do", "break", "continue"}
TOKEN_RE = re.compile(r"[A-Za-z_]\w*|0[xX][0-9A-Fa-f]+[uUlL]*|\d+[uUlL]*|->|\+\+|--|<<=|>>=|<<|>>"
                      r"|&&|\|\||[-+*/%&|^!=<>]=|\S")
ASSIGN_OPS = {"=", "+=", "-=", "*=", "/=", "%=", "|=", "&=", "^=", "<<=", ">>="}
# tokens after which an operand starts an expression whose operators all bind less than `+`
PREV_PLUS = {"(", ",", ";", "{", "}", "?", ":", "[", "return", "case", "&&", "||", "==", "!=",
             "<", ">", "<=", ">=", "<<", ">>", "&", "|", "^"} | ASSIGN_OPS
NEXT_PLUS = {")", ",", ";", "]", "}", "?", ":", "&&", "||", "==", "!=", "<", ">", "<=", ">=",
             "<<", ">>", "&", "|", "^"}
PREV_EQ = {"(", ",", ";", "{", "}", "?", ":", "return", "&&", "||", "&", "|", "^"} | ASSIGN_OPS
NEXT_EQ = {")", ",", ";", "]", "?", ":", "&&", "||", "&", "|", "^"}
PREV_MUL = PREV_PLUS | {"+", "-"}
PREV_SHIFT = PREV_EQ | {"[", "case", "==", "!=", "<", ">", "<=", ">="}
NEXT_SHIFT = NEXT_PLUS
# binary operators that bind tighter than the compound operator (x = x OP rest needs no parens)
TIGHTER = {"+": {"*", "/", "%"}, "-": {"*", "/", "%"},
           "&": {"*", "/", "%", "+", "-", "<<", ">>"},
           "^": {"*", "/", "%", "+", "-", "<<", ">>", "&"},
           "|": {"*", "/", "%", "+", "-", "<<", ">>", "&", "^"}}
BINARY_OPS = {"*", "/", "%", "+", "-", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "&", "^",
              "|", "&&", "||", "?", ":", ","} | ASSIGN_OPS
STATEMENT_ENDS = (";", "{", "}", ":")
NO_STATEMENT_START = ("else", "}", "{", "case", "default", "#", "while")


def mask(text):
    """`text` with comments and string/char literal contents blanked; offsets and newlines kept."""
    out = list(text)
    i, n = 0, len(text)
    while i < n:
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
        elif text[i] in "\"'":
            q, j = text[i], i + 1
            while j < n and text[j] != q and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            for k in range(i + 1, min(j, n)):
                if out[k] != "\n":
                    out[k] = " "
            i = j + 1
            continue
        else:
            i += 1
            continue
        for k in range(i, j):
            if out[k] != "\n":
                out[k] = " "
        i = j
    return "".join(out)


class Tok:
    __slots__ = ("text", "start", "end", "kind")

    def __init__(self, m):
        self.text, self.start, self.end = m.group(0), m.start(), m.end()
        c = self.text[0]
        self.kind = "id" if (c.isalpha() or c == "_") else "num" if c.isdigit() else "op"

    def __repr__(self):
        return "Tok(%r@%d)" % (self.text, self.start)


def tokenize(masked):
    return [Tok(m) for m in TOKEN_RE.finditer(masked)]


def match_close(toks, i):
    """Index after the bracket closing toks[i] (`(` or `[`), or None."""
    opening = toks[i].text
    closing = {"(": ")", "[": "]"}[opening]
    depth = 0
    for j in range(i, len(toks)):
        if toks[j].text == opening:
            depth += 1
        elif toks[j].text == closing:
            depth -= 1
            if depth == 0:
                return j + 1
    return None


def is_primary(tok):
    return tok.kind == "num" or (tok.kind == "id" and tok.text not in KEYWORDS)


def operand_end(toks, i):
    """Index after a postfix expression starting at toks[i] (a name, number, parenthesised
    expression or cast, then `.f`, `->f`, `[i]`, `(args)`); None when no operand starts there."""
    if i >= len(toks):
        return None
    t = toks[i]
    if t.text == "(":
        j = match_close(toks, i)
        if j is None:
            return None
        if j < len(toks) and (is_primary(toks[j]) or toks[j].text == "("):
            return operand_end(toks, j)  # a cast binds to the operand after it
        i = j
    elif is_primary(t):
        i += 1
    else:
        return None
    while i < len(toks):
        t = toks[i]
        if t.text in (".", "->") and i + 1 < len(toks) and toks[i + 1].kind == "id":
            i += 2
        elif t.text == "[" or (t.text == "(" and toks[i - 1].kind == "id"):
            j = match_close(toks, i)
            if j is None:
                return None
            i = j
        else:
            break
    return i


def binary_amp(toks, k):
    """True when toks[k] (`&`) is the binary operator, not the address-of."""
    if k == 0:
        return False
    p = toks[k - 1]
    if p.text == ")":
        # `(u8 *)&x` is a cast before the address-of: names and stars only inside the parens
        depth, j = 0, k - 1
        while j >= 0:
            depth += {")": 1, "(": -1}.get(toks[j].text, 0)
            if depth == 0:
                break
            j -= 1
        inner = toks[j + 1:k - 1]
        return not inner or not all(t.kind == "id" or t.text == "*" for t in inner)
    return p.kind in ("id", "num") and p.text not in KEYWORDS or p.text == "]"


def prev_ok(toks, s, allowed):
    if s == 0:
        return False
    p = toks[s - 1]
    if p.text not in allowed:
        return False
    return p.text != "&" or binary_amp(toks, s - 1)


def int_value(text):
    t = text.rstrip("uUlL")
    try:
        return int(t, 16) if t.lower().startswith("0x") else int(t, 10)
    except ValueError:
        return None


# ---------------------------------------------------------------------------------------------
# The function under edit
# ---------------------------------------------------------------------------------------------

class Variant:
    __slots__ = ("text", "label", "fake")

    def __init__(self, text, label, fake=False):
        self.text, self.label, self.fake = text, label, fake

    def __repr__(self):
        return "Variant(%s)" % self.label


def fake_comment(what):
    return "/* FAKE: %s. %s */" % (what, TICKET)


class Func:
    """One C function definition (signature through the closing brace) and its layout."""

    def __init__(self, text):
        self.text = text
        self.masked = mask(text)
        m = re.match(r"\s*[A-Za-z_][^;{}()]*?(\w+)\s*\(", self.masked)
        if not m:
            raise ValueError("not a function definition")
        self.name = m.group(1)
        self.brace = self.masked.find("{", m.end())
        if self.brace < 0:
            raise ValueError("no body")
        self.lines = text.split("\n")
        self.mlines = self.masked.split("\n")
        self.brace_line = self.masked.count("\n", 0, self.brace)
        self.end_line = max(i for i, l in enumerate(self.mlines) if "}" in l)
        self.toks = tokenize(self.masked)
        self._decls()
        self._params()

    # body lines are brace_line+1 .. end_line-1
    def _decls(self):
        self.decl_lines = []      # [(line index, type, stars, name, array, init, more)]
        self.locals = {}          # name -> (type, pointer?, array?)
        i = self.brace_line + 1
        while i < self.end_line:
            s = self.mlines[i].strip()
            if not s:
                i += 1
                continue
            m = DECL_RE.match(self.mlines[i])
            if not m or m.group(1) in NOT_DECL_HEADS:
                break
            self.decl_lines.append(i)
            names = [(m.group(3), "*" in m.group(2), bool(m.group(4)))]
            for extra in (m.group(6) or "").split(",")[1:]:
                em = re.match(r"\s*(\**)\s*(\w+)\s*(\[)?", extra)
                if em:
                    names.append((em.group(2), bool(em.group(1)), bool(em.group(3))))
            for name, ptr, arr in names:
                self.locals[name] = (m.group(1), ptr, arr)
            i += 1
        self.stmt_line = i        # first statement line

    def _params(self):
        sig = self.masked[:self.brace]
        self.params = {}
        for m in re.finditer(r"\b(\w+)(\s+\**\s*|\s*\*+\s*)(\w+)\s*(?=[,;)])", sig):
            if m.group(3) != self.name:
                self.params[m.group(3)] = (m.group(1), "*" in m.group(2), False)
        open_paren = sig.find("(", sig.find(self.name))
        close = sig.find(")", open_paren)
        for name in re.findall(r"[A-Za-z_]\w*", sig[open_paren:close]):
            # K&R names without a declaration line are int
            if name not in self.params and name not in KEYWORDS and name not in SCALAR_TYPES:
                if re.search(r"(?:^|[(,])\s*%s\s*(?=[,)])" % name, sig[open_paren:close + 1]):
                    self.params[name] = ("int", False, False)

    def scalars(self):
        """Locals and parameters an `if (v)` can test: integers and pointers, not arrays."""
        out = []
        for name, (typ, ptr, arr) in list(self.locals.items()) + list(self.params.items()):
            if not arr and (ptr or typ in SCALAR_TYPES) and name not in out:
                out.append(name)
        return out

    def statement_start(self, i):
        """True when line i starts a statement at which another statement may be inserted."""
        if i < self.stmt_line or i >= self.end_line:
            return False
        s = self.mlines[i].strip()
        if not s or s.startswith(NO_STATEMENT_START):
            return False
        j = i - 1
        while j > self.brace_line and not self.mlines[j].strip():
            j -= 1
        prev = self.mlines[j].rstrip()
        if j == self.brace_line:
            return True
        return prev.endswith(STATEMENT_ENDS)

    def indent(self, i):
        return re.match(r"\s*", self.lines[i]).group(0)

    def body_tokens(self):
        """Index range of the tokens inside the body."""
        lo = next(k for k, t in enumerate(self.toks) if t.start >= self.brace)
        return lo + 1, len(self.toks)

    def with_lines(self, lines):
        return "\n".join(lines)

    def unique(self, base):
        used = set(re.findall(r"\w+", self.masked))
        name, n = base, 2
        while name in used:
            name, n = "%s%d" % (base, n), n + 1
        return name

    def line_of(self, offset):
        return self.masked.count("\n", 0, offset)

    def insert_decl(self, lines, decl, at_end=True):
        """`lines` with a declaration line added at the top or the end of the locals."""
        if self.decl_lines:
            ind = self.indent(self.decl_lines[0])
            pos = self.decl_lines[-1] + 1 if at_end else self.decl_lines[0]
        else:
            ind = self.indent(self.stmt_line) if self.stmt_line < self.end_line else "    "
            pos = self.brace_line + 1
        lines = list(lines)
        lines.insert(pos, ind + decl)
        return lines, pos


# ---------------------------------------------------------------------------------------------
# Transformations: each returns [Variant] for every place it applies, in source order
# ---------------------------------------------------------------------------------------------

def splice(text, edits):
    """Apply [(start, end, replacement)] edits (non-overlapping) to text."""
    for a, b, rep in sorted(edits, reverse=True):
        text = text[:a] + rep + text[b:]
    return text


def t_byteview(f):
    out = []
    lo, hi = f.body_tokens()
    t = f.toks
    for k in range(lo, hi - 2):
        if t[k].text == "switch" and t[k + 1].text == "(":
            e = operand_end(t, k + 2)
            if e is None or e >= hi or t[e].text != ")" or t[k + 2].kind != "id":
                continue
            if t[k + 2].text in f.locals or t[k + 2].text in f.params:
                continue
            if any(x.text == "(" for x in t[k + 2:e]):
                continue
            a, b = t[k + 2].start, t[e - 1].end
            line = f.line_of(t[e].start)
            text = splice(f.text, [(a, b, "*(u8 *)&" + f.text[a:b])])
            lines = text.split("\n")
            lines[line] = lines[line].rstrip() + " " + fake_comment("scalar byte view of the switch selector")
            out.append(Variant("\n".join(lines), "byteview L%d" % line, True))
    return out


def swap_sites(f):
    """[(a0, a1, b0, b1, op)]: text spans of the two operands of every `+`, `==`, `!=` whose
    operands can trade places without changing the parse."""
    out = []
    lo, hi = f.body_tokens()
    t = f.toks
    for s in range(lo, hi):
        if not prev_ok(t, s, PREV_PLUS | PREV_EQ):
            continue
        e = operand_end(t, s)
        if e is None or e >= hi or t[e].text not in ("+", "==", "!="):
            continue
        op = t[e].text
        if not prev_ok(t, s, PREV_PLUS if op == "+" else PREV_EQ):
            continue
        g = operand_end(t, e + 1)
        if g is None or g >= hi or t[g].text not in (NEXT_PLUS if op == "+" else NEXT_EQ):
            continue
        a0, a1, b0, b1 = t[s].start, t[e - 1].end, t[e + 1].start, t[g - 1].end
        if f.text[a0:a1] == f.text[b0:b1]:
            continue
        out.append((a0, a1, b0, b1, op))
    return out


def t_swap(f):
    out = []
    for a0, a1, b0, b1, op in swap_sites(f):
        text = f.text[:a0] + f.text[b0:b1] + f.text[a1:b0] + f.text[a0:a1] + f.text[b1:]
        out.append(Variant(text, "swap%s L%d" % (op, f.line_of(a0))))
    return out


def simple_statement(f, i):
    """True for a line that is one expression statement (assignment or call)."""
    if i < f.stmt_line or i >= f.end_line:
        return False
    s = f.mlines[i].strip()
    if not s.endswith(";") or s.count(";") != 1 or "{" in s or "}" in s:
        return False
    head = re.match(r"\w+", s)
    if head and head.group(0) in KEYWORDS:
        return False
    return f.statement_start(i)


def split_statements(masked_line):
    """[(start, end)] of the `;`-terminated statements of a line at paren depth 0, or None."""
    spans, depth, start = [], 0, None
    for k, c in enumerate(masked_line):
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        elif c in "{}":
            return None
        if start is None and not c.isspace():
            start = k
        if c == ";" and depth == 0:
            spans.append((start, k + 1))
            start = None
    if start is not None and masked_line[start:].strip():
        trailing = masked_line[start:].strip()
        if trailing:
            return None
    return spans


def t_lines(f):
    out = []
    for i in range(f.stmt_line, f.end_line - 1):
        if simple_statement(f, i) and simple_statement(f, i + 1) and f.indent(i) == f.indent(i + 1):
            lines = list(f.lines)
            lines[i] = lines[i].rstrip() + " " + lines[i + 1].strip()
            del lines[i + 1]
            out.append(Variant("\n".join(lines), "join L%d" % i))
    for i in range(f.stmt_line, f.end_line):
        s = f.mlines[i].strip()
        if s.count(";") < 2 or re.match(r"(for|if|while|else|switch|do|case|default)\b", s):
            continue
        spans = split_statements(f.mlines[i])
        if not spans or len(spans) < 2:
            continue
        if any(re.match(r"\s*(if|for|while|else|switch|do|return|break|continue|goto)\b", f.mlines[i][a:b])
               for a, b in spans[:-1]):
            continue
        ind = f.indent(i)
        new = [ind + f.lines[i][a:b].strip() for a, b in spans]
        new[-1] = new[-1] + f.lines[i][spans[-1][1]:].rstrip()  # a trailing comment stays on the last
        lines = f.lines[:i] + new + f.lines[i + 1:]
        out.append(Variant("\n".join(lines), "split L%d" % i))
    return out


def top_level_ops(toks):
    ops, depth = set(), 0
    for k, t in enumerate(toks):
        if t.text in ("(", "["):
            depth += 1
        elif t.text in (")", "]"):
            depth -= 1
        elif depth == 0 and t.text in BINARY_OPS and k > 0:
            ops.add(t.text)
    return ops


def t_compound(f):
    out = []
    for i in range(f.stmt_line, f.end_line):
        if not simple_statement(f, i):
            continue
        line, ml = f.lines[i], f.mlines[i]
        toks = tokenize(ml)
        e = operand_end(toks, 0)
        if e is None or e + 1 >= len(toks):
            continue
        lv = ml[toks[0].start:toks[e - 1].end]
        op_tok = toks[e]
        if op_tok.text == "=":
            e2 = operand_end(toks, e + 1)
            if e2 is None or e2 >= len(toks) - 1 or toks[e2].text not in TIGHTER:
                continue
            if re.sub(r"\s", "", ml[toks[e + 1].start:toks[e2 - 1].end]) != re.sub(r"\s", "", lv):
                continue
            op = toks[e2].text
            rest = toks[e2 + 1:-1]
            if not rest or toks[-1].text != ";" or not top_level_ops(rest) <= TIGHTER[op]:
                continue
            new = line[:op_tok.start] + op + "= " + line[rest[0].start:]
            out.append(Variant(_relines(f, i, new), "compound %s= L%d" % (op, i)))
        elif op_tok.text[:-1] in TIGHTER and op_tok.text.endswith("="):
            op = op_tok.text[:-1]
            rest = toks[e + 1:-1]
            if not rest or toks[-1].text != ";":
                continue
            r = line[rest[0].start:rest[-1].end]
            if not top_level_ops(rest) <= TIGHTER[op]:
                r = "(" + r + ")"
            new = line[:op_tok.start] + "= " + line[toks[0].start:toks[e - 1].end] + " " + op + " " + r + line[rest[-1].end:]
            out.append(Variant(_relines(f, i, new), "expand %s= L%d" % (op, i)))
    return out


def _relines(f, i, new_line):
    lines = list(f.lines)
    lines[i] = new_line
    return "\n".join(lines)


def t_emptytest(f, per_name=3):
    out = []
    for name in f.scalars():
        n = 0
        for i in range(f.stmt_line, f.end_line):
            if n >= per_name:
                break
            if not re.search(r"\b%s\b" % re.escape(name), f.mlines[i]) or not f.statement_start(i):
                continue
            lines = list(f.lines)
            lines.insert(i, "%sif (%s) {} %s" % (f.indent(i), name,
                                                 fake_comment("empty test splits the live range of " + name)))
            out.append(Variant("\n".join(lines), "emptytest %s L%d" % (name, i), True))
            n += 1
    return out


def t_pad(f):
    out = []
    name = f.unique("pad")
    decl = "s32 %s; %s" % (name, fake_comment("unused local reserves a stack word (frame home)"))
    for at_end in (False, True):
        if at_end and not f.decl_lines:
            continue
        lines, pos = f.insert_decl(f.lines, decl, at_end)
        out.append(Variant("\n".join(lines), "pad %s" % ("last" if at_end else "first"), True))
    return out


def t_declswap(f):
    out = []
    for a, b in zip(f.decl_lines, f.decl_lines[1:]):
        if b != a + 1:
            continue
        lines = list(f.lines)
        lines[a], lines[b] = lines[b], lines[a]
        out.append(Variant("\n".join(lines), "declswap L%d" % a))
    return out


def t_shift(f):
    out = []
    lo, hi = f.body_tokens()
    t = f.toks
    for s in range(lo, hi):
        if t[s].kind == "num":
            continue
        e = operand_end(t, s)
        if e is None or e + 2 > hi or e + 1 >= len(t):
            continue
        op, n = t[e].text, t[e + 1]
        if n.kind != "num" or (e + 2 < len(t) and t[e + 2].text in ("[", "(", ".", "->")):
            continue
        v = int_value(n.text)
        if v is None:
            continue
        a0, a1 = t[s].start, t[e - 1].end
        operand = f.text[a0:a1]
        if op == "*" and prev_ok(t, s, PREV_MUL) and v >= 2 and v & (v - 1) == 0:
            rep = "(%s << %d)" % (operand, v.bit_length() - 1)
        elif op == "<<" and prev_ok(t, s, PREV_SHIFT) and 1 <= v <= 15 and \
                e + 2 < len(t) and t[e + 2].text in NEXT_SHIFT:
            m = 1 << v
            rep = "(%s * %s)" % (operand, ("0x%X" % m) if m >= 0x100 else str(m))
        else:
            continue
        text = f.text[:a0] + rep + f.text[n.end:]
        out.append(Variant(text, "%s L%d" % ("mul>shift" if op == "*" else "shift>mul", f.line_of(a0))))
    return out


ASSIGN_LINE = re.compile(r"^(\s*)(\w+)\s*=(?!=)\s*(.+?);\s*$")


def t_temp(f):
    out = []
    # inline a single-use temporary
    for d in f.decl_lines:
        m = DECL_RE.match(f.mlines[d])
        if not m or m.group(4) or m.group(5) or m.group(6):
            continue
        name = m.group(3)
        word = re.compile(r"\b%s\b" % re.escape(name))
        uses = [(i, w) for i in range(f.stmt_line, f.end_line) for w in word.finditer(f.mlines[i])]
        if len(uses) != 2 or uses[0][0] == uses[1][0]:
            continue
        (ai, _), (ui, uw) = uses
        am = ASSIGN_LINE.match(f.mlines[ai])
        if not am or am.group(2) != name or ";" in am.group(3) or not f.statement_start(ai):
            continue
        rhs = f.lines[ai][am.start(3):am.end(3)]
        rtoks = tokenize(am.group(3))
        if operand_end(rtoks, 0) != len(rtoks):
            rhs = "(" + rhs + ")"
        lines = list(f.lines)
        lines[ui] = lines[ui][:uw.start()] + rhs + lines[ui][uw.end():]
        for k in sorted((d, ai), reverse=True):
            del lines[k]
        out.append(Variant("\n".join(lines), "inline %s L%d" % (name, ai)))
    # introduce one
    tmp = f.unique("temp")
    for i in range(f.stmt_line, f.end_line):
        if not f.statement_start(i):
            continue
        ml = f.mlines[i]
        toks = tokenize(ml)
        sites = []   # (expression start, end) in the line, [types], label
        if simple_statement(f, i):
            e = operand_end(toks, 0)
            if e is not None and e < len(toks) and toks[e].text == "=" and toks[-1].text == ";":
                rhs = toks[e + 1:-1]
                if rhs and not (len(rhs) == 1 and (rhs[0].kind == "num" or rhs[0].text in f.locals)):
                    lhs = ml[toks[0].start:toks[e - 1].end].strip()
                    typ = f.locals.get(lhs, ("s32", False, False))
                    types = [typ[0]] if not typ[1] and typ[0] in SCALAR_TYPES else ["s32"]
                    sites.append((rhs[0].start, rhs[-1].end, types, "rhs"))
        if len(toks) > 3 and toks[0].text in ("switch", "if") and toks[1].text == "(":
            e = operand_end(toks, 2)
            if e is not None and e < len(toks) and (
                    (toks[0].text == "switch" and toks[e].text == ")") or
                    (toks[0].text == "if" and toks[e].text in ("==", "!=", "<", ">", "<=", ">="))):
                expr = ml[toks[2].start:toks[e - 1].end].strip()
                if expr not in f.locals and expr not in f.params and toks[2].kind != "num":
                    sites.append((toks[2].start, toks[e - 1].end, ["s32", "u8"], toks[0].text))
        for a, b, types, what in sites:
            for typ in types:
                lines = list(f.lines)
                ind = f.indent(i)
                expr = lines[i][a:b]
                lines[i] = lines[i][:a] + tmp + lines[i][b:]
                lines.insert(i, "%s%s = %s;" % (ind, tmp, expr))
                lines, _ = f.insert_decl(lines, "%s %s;" % (typ, tmp), at_end=True)
                out.append(Variant("\n".join(lines), "temp %s %s L%d" % (typ, what, i)))
    return out


def t_width(f):
    out = []
    regions = [(0, f.brace)]
    starts = [sum(len(l) + 1 for l in f.lines[:d]) for d in f.decl_lines]
    regions += [(s, s + len(f.lines[d])) for s, d in zip(starts, f.decl_lines)]
    for a, b in regions:
        for m in re.finditer(r"\b(s8|u8|s16|u16|s32|u32)(\s+)(\w+)\s*(?=[,;)=])", f.masked[a:b]):
            if m.group(3) == f.name:
                continue
            for new in WIDTHS[m.group(1)]:
                text = f.text[:a + m.start(1)] + new + f.text[a + m.end(1):]
                out.append(Variant(text, "width %s %s->%s" % (m.group(3), m.group(1), new)))
    return out


# ---------------------------------------------------------------------------------------------
# Statement order (T-11200): which adjacent statements may trade places
# ---------------------------------------------------------------------------------------------

class Access:
    """What one simple statement line reads and writes, by name (a conservative token check).

    reads:  every variable name of the statement, and `*name` for each name whose memory it may
            read (everything but the target of a plain store);
    writes: the plain local or parameter it assigns (also each name next to `++`/`--`);
    stores: {name: {left side}} for a store through a pointer, member, index or global
            (`p->x = 0`, `a[i] = 0`, `D_X = 0`): a write to the memory of every name of its left
            side. Another statement that reads `*name` depends on it; another store to the same
            left side text does too, a store to another field of the same name does not.
    call:   a function call (it may touch any memory);
    memory: it touches anything but plain locals (globals, dereferences, members, indexing,
            locals whose address is taken).
    """

    __slots__ = ("reads", "writes", "stores", "call", "memory")

    def __init__(self, reads, writes, stores, call, memory):
        self.reads, self.writes, self.stores, self.call, self.memory = reads, writes, stores, call, memory

    @property
    def store(self):
        return bool(self.stores)


def address_taken(f):
    lo, hi = f.body_tokens()
    t = f.toks
    return {t[k + 1].text for k in range(lo, hi - 1)
            if t[k].text == "&" and t[k + 1].kind == "id" and not binary_amp(t, k)}


def _names(toks):
    """Variable names among toks: no keywords, no types, no member names, no called names."""
    out = set()
    for k, tok in enumerate(toks):
        if tok.kind != "id" or tok.text in KEYWORDS or tok.text in SCALAR_TYPES:
            continue
        if k > 0 and toks[k - 1].text in (".", "->"):
            continue
        if k + 1 < len(toks) and toks[k + 1].text == "(":
            continue
        out.add(tok.text)
    return out


def _is_call(toks):
    """A named call `f(...)`, or `(...)(...)` (a call through a pointer; casts land here too)."""
    for k in range(len(toks) - 1):
        if toks[k + 1].text != "(":
            continue
        tok = toks[k]
        if tok.kind == "id" and tok.text not in KEYWORDS and tok.text not in SCALAR_TYPES or tok.text == ")":
            return True
    return False


def access(f, i, taken=None):
    """Access of the simple statement on line i."""
    taken = address_taken(f) if taken is None else taken
    ml = f.mlines[i]
    toks = tokenize(ml)
    plain = {n for n, (_typ, _ptr, arr) in list(f.locals.items()) + list(f.params.items()) if not arr} - taken
    names = _names(toks)
    unary_star = any(tok.text == "*" and (k == 0 or toks[k - 1].kind == "op" and toks[k - 1].text not in (")", "]"))
                     for k, tok in enumerate(toks))
    memory = bool(names - plain) or unary_star or any(tok.text in (".", "->", "[") for tok in toks)
    k = 0
    while k < len(toks) and toks[k].text == "*":
        k += 1
    e = operand_end(toks, k)
    writes, stores = set(), {}
    deref = set(names)          # names whose memory the statement may read
    if e is not None and e < len(toks) and toks[e].text in ASSIGN_OPS:
        lhs, op = toks[:e], toks[e].text
        rhs_names = _names(toks[e + 1:])
        lhs_names = _names(lhs)
        if len(lhs) == 1 and lhs[0].text in plain:
            writes.add(lhs[0].text)
            names = names if op != "=" or lhs[0].text in rhs_names else names - {lhs[0].text}
            deref = set(names)
        else:
            text = re.sub(r"\s", "", ml[lhs[0].start:lhs[-1].end])
            for n in lhs_names:
                stores.setdefault(n, set()).add(text)
            if op == "=":
                deref = rhs_names
    if any(tok.text in ("++", "--") for tok in toks):
        writes |= names & plain
        for n in names - plain:
            stores.setdefault(n, set()).add(ml.strip())
    reads = set(names) | {"*" + n for n in deref}
    return Access(reads, writes, stores, _is_call(toks), memory)


def independent(a, b):
    """True when two statements (Access) may run in either order."""
    if a.writes & (b.reads | b.writes) or b.writes & a.reads:
        return False
    for x, y in ((a, b), (b, a)):
        for n, lvals in x.stores.items():
            if "*" + n in y.reads or lvals & y.stores.get(n, set()):
                return False
    if a.call and b.call:
        return False
    if a.call:
        return not b.memory
    if b.call:
        return not a.memory
    return True


def t_order(f):
    out = []
    taken = address_taken(f)
    for i in range(f.stmt_line, f.end_line - 1):
        if not (simple_statement(f, i) and simple_statement(f, i + 1) and f.indent(i) == f.indent(i + 1)):
            continue
        if not independent(access(f, i, taken), access(f, i + 1, taken)):
            continue
        lines = list(f.lines)
        lines[i], lines[i + 1] = lines[i + 1], lines[i]
        out.append(Variant("\n".join(lines), "order L%d" % i))
    return out


def order_runs(f, max_len=6):
    """[(first line, last line)] of the runs of 2..max_len consecutive simple statements at one
    indent that are pairwise independent (so every order of a run is legal)."""
    taken = address_taken(f)
    runs, cur, acc = [], [], {}

    def close():
        if len(cur) >= 2:
            runs.append((cur[0], cur[-1]))

    for i in range(f.stmt_line, f.end_line):
        if not simple_statement(f, i):
            close()
            cur = []
            continue
        acc[i] = access(f, i, taken)
        if cur and len(cur) < max_len and i == cur[-1] + 1 and f.indent(i) == f.indent(cur[0]) and \
                all(independent(acc[j], acc[i]) for j in cur):
            cur.append(i)
            continue
        close()
        cur = [i]
    close()
    return runs


def rank_runs(f, runs):
    """Runs best first: more stores (packet and RECT fields, the T-11530 cases), then longer."""
    taken = address_taken(f)

    def key(run):
        a, b = run
        stores = sum(access(f, i, taken).store for i in range(a, b + 1))
        return (-stores, -(b - a + 1), a)
    return sorted(runs, key=key)


def _factorial(n):
    r = 1
    for k in range(2, n + 1):
        r *= k
    return r


def pick_order(f, max_exhaustive=2000, max_runs=2, max_swaps=2):
    """(runs, swap sites) for the order stage: the best run, a second one while the variant count
    stays within max_exhaustive, then up to max_swaps operand swaps (on one line each, nearest the
    runs first) under the same cap. ([], []) when the function has no run."""
    runs = rank_runs(f, order_runs(f))
    if not runs:
        return [], []
    chosen = [runs[0]]
    size = _factorial(runs[0][1] - runs[0][0] + 1)
    for run in runs[1:]:
        if len(chosen) >= max_runs:
            break
        n = _factorial(run[1] - run[0] + 1)
        if size * n <= max_exhaustive:
            chosen.append(run)
            size *= n
    sites, used = [], []
    lines = [i for a, b in chosen for i in range(a, b + 1)]
    for site in sorted(swap_sites(f), key=lambda s: (min(abs(f.line_of(s[0]) - i) for i in lines), s[0])):
        if len(sites) >= max_swaps or size * 2 > max_exhaustive:
            break
        a0, _, _, b1, _ = site
        if "\n" in f.text[a0:b1] or any(a0 < u1 and u0 < b1 for u0, u1 in used):
            continue
        sites.append(site)
        used.append((a0, b1))
        size *= 2
    return chosen, sites


def mark_order(f, runs, sites):
    """The function text with order_search markers: `/* ORDER */ ... /* END ORDER */` around each
    run and `ORDER_SWAP(A, op, B)` at each swap site. Only handed to order_search.Template, whose
    render() removes every marker; it is never written to a file."""
    text = splice(f.text, [(a0, b1, "ORDER_SWAP(%s, %s, %s)" % (f.text[a0:a1], op, f.text[b0:b1]))
                           for a0, a1, b0, b1, op in sites])
    lines = text.split("\n")
    for a, b in sorted(runs, reverse=True):
        ind = f.indent(a)
        lines.insert(b + 1, ind + "/* END ORDER */")
        lines.insert(a, ind + "/* ORDER */")
    return "\n".join(lines)


NO_SCORE = 10 ** 6   # order_search's score for a variant that does not compile


def order_stage(text, score, verify, deadline, clock=time.monotonic, max_exhaustive=2000, budget=3000,
                max_runs=2, max_swaps=2, jobs=1):
    """Search the statement orders of the best runs of `text` (and a few operand orders) with
    order_search.search (T-11530, in-process, one compile at a time), scored by `score`. A 0
    counts only when verify() accepts it. Returns (score, text, label) of the best variant that
    differs from `text`, or None."""
    import order_search
    try:
        f = Func(text)
    except ValueError:
        return None
    runs, sites = pick_order(f, max_exhaustive, max_runs, max_swaps)
    if not runs:
        return None
    try:
        template = order_search.Template(mark_order(f, runs, sites))
    except order_search.MarkerError:
        return None

    def score_state(state):
        if clock() >= deadline:
            return NO_SCORE
        variant = template.render(state)
        if "/* ORDER */" in variant or order_search.SWAP_TOKEN in variant:
            raise AssertionError("order_search left a marker in a variant")
        s = score(variant)
        if s is None or (s == 0 and not verify(variant)):
            return NO_SCORE
        return s

    results = order_search.search(template, score_state, jobs=jobs, max_exhaustive=max_exhaustive,
                                  budget=budget, progress=lambda msg: None)
    for s, state in results:
        if s >= NO_SCORE:
            break
        variant = template.render(state)
        if variant != text:
            return s, variant, "order[%s]" % template.describe(state)
    return None


TRANSFORMS = (t_byteview, t_temp, t_swap, t_width, t_compound, t_lines, t_shift, t_emptytest,
              t_pad, t_declswap, t_order)


def variants(text, cap=None):
    """Single-edit variants of a function text: round robin over the transforms, deduplicated."""
    try:
        f = Func(text)
    except ValueError:
        return []
    lists = []
    for tr in TRANSFORMS:
        try:
            lists.append(tr(f))
        except (ValueError, IndexError, KeyError, StopIteration):
            lists.append([])
    out, seen = [], {text}
    k = 0
    while any(k < len(l) for l in lists):
        for l in lists:
            if k < len(l) and l[k].text not in seen:
                seen.add(l[k].text)
                out.append(l[k])
        k += 1
    return out[:cap] if cap else out


# ---------------------------------------------------------------------------------------------
# regdiff guidance (T-11220): rank the edits by the first register divergence
# ---------------------------------------------------------------------------------------------

# Hint class -> edit kinds to try first, best first. Each row follows a row of the "Symptom ->
# what to try" table in wiki/register-cheatsheet.md; extend both together.
HINT_EDITS = {
    # tie: equal priorities, numbered by first use -> reorder statements, drop or add a temporary
    "tie": ("order", "inline", "temp", "join", "split", "swap", "declswap"),
    # priority order: the wrong value wins -> one use more or less, temporary <-> repeated
    # expression; the order edit stands in for moving a use into or out of a loop
    "priority": ("inline", "temp", "compound", "expand", "emptytest", "order"),
    # extra range in the draft (it holds a lower colour) -> remove a local or share the value
    "extra-range": ("inline", "compound", "emptytest", "temp", "width"),
    # missing range (the original keeps a value the draft recomputes) -> add a local
    "missing-range": ("temp", "emptytest", "inline", "compound"),
    # a value with uses that gets no register in the draft ("not allocated in the draft")
    "unallocated": ("temp", "inline", "emptytest"),
    # the original passes the value straight to a call: a copy or cast sits in between
    "argument": ("inline", "temp", "width"),
    # ugen temporaries t6..t9: operand order of sums, same-line grouping, parameter width
    "ugen": ("swap", "join", "split", "width", "order", "shift", "compound", "expand", "byteview"),
    # commutative operands exchanged, registers right
    "operand": ("swap", "order"),
    # other instructions: fix the shape first (step 1 of the cheat sheet's loop)
    "structure": ("temp", "inline", "compound", "expand", "shift", "byteview", "emptytest", "width"),
    # frame size or stack offsets differ: unused locals and declaration order
    "frame": ("pad", "declswap", "width"),
}

# regdiff report text -> hint class; the first match in the FIRST DIVERGENCE block is the primary
HINT_PATTERNS = (
    ("tie", re.compile(r"hint: (?:tie\.|equal priorities)")),
    ("priority", re.compile(r"hint: .* (?:is coloured before|outranks) ")),
    ("extra-range", re.compile(r"hint: colours are tried lowest first")),
    ("missing-range", re.compile(r"hint: the draft took the lower colour")),
    ("ugen", re.compile(r"is not a uopt colour|no live range of the trace holds")),
)
SECONDARY_PATTERNS = (
    ("argument", re.compile(r"the original passes this value to the call")),
    ("unallocated", re.compile(r"^not allocated in the draft:")),
)
NAME_RE = re.compile(r"\b(?:local|parameter|global|return value) ([A-Za-z_][\w/]*)")
LINES_RE = re.compile(r"\(lines ([\d,]+)\)")
FALLBACK_NAME = re.compile(r"^[MPSR]\d+$")     # regdiff's class+offset name of an unnamed variable
SP_RE = re.compile(r"\$?\bsp\b")


class Hint:
    """The first divergence of one regdiff report: classes (primary first), variable names, and
    source lines as 0-based line indices of the function text."""

    def __init__(self, classes=(), names=(), lines=(), error=None):
        self.classes, self.names, self.lines, self.error = list(classes), list(names), list(lines), error

    @property
    def kinds(self):
        out = []
        for c in self.classes:
            for k in HINT_EDITS.get(c, ()):
                if k not in out:
                    out.append(k)
        return out

    def summary(self):
        """The terse form for the output line, e.g. `tie i,p L4,6`."""
        if self.error:
            return "error"
        if not self.classes:
            return "?"
        parts = ["+".join(self.classes[:2])]
        if self.names:
            parts.append(",".join(self.names[:2]))
        if self.lines:
            parts.append("L" + ",".join(str(l) for l in self.lines[:2]))
        return " ".join(parts)

    def __repr__(self):
        return "Hint(%s)" % self.summary()


def parse_regdiff(text, first_line=1):
    """The Hint of a regdiff report. first_line: the 1-based file line of the function's first
    line in the file regdiff compiled (regdiff names file lines)."""
    head = [l.strip() for l in text.split("\n")]
    if any(l.startswith("MATCH:") for l in head):
        return Hint(["match"])
    errors = [l for l in head if l.startswith("regdiff: ")]
    if errors:
        return Hint(error=errors[0][len("regdiff: "):][:120])
    classes = []
    if any(l.startswith("STRUCTURE differs") for l in head):
        shape = [l.split(None, 2)[-1] for l in head if l.startswith(("- draft", "+ original"))]
        classes.append("frame" if any(SP_RE.search(l) for l in shape) else "structure")
    elif any(l.startswith("constant differs") for l in head):
        classes.append("structure")
    div = next((i for i, l in enumerate(head) if l.startswith("FIRST DIVERGENCE")), None)
    block = head[div:] if div is not None else []
    for cls, pat in HINT_PATTERNS:
        if any(pat.search(l) for l in block):
            classes.append(cls)
            break
    for cls, pat in SECONDARY_PATTERNS:
        if any(pat.search(l) for l in block):
            classes.append(cls)
    if any(l.startswith("operand order:") for l in head):
        classes.append("operand")
    names, nums = [], []
    for l in block:
        for m in NAME_RE.finditer(l):
            for n in m.group(1).split("/"):
                if n and not FALLBACK_NAME.match(n) and n not in names:
                    names.append(n)
        for m in LINES_RE.finditer(l):
            for n in m.group(1).split(","):
                k = int(n) - first_line
                if k >= 0 and k not in nums:
                    nums.append(k)
    return Hint(classes, names, nums)


def edit_kind(label):
    """The kind of a variant label (`swap== L4` -> swap, `mul>shift L5` -> shift)."""
    word = label.split()[0] if label else ""
    if word.startswith("swap"):
        return "swap"
    if word in ("mul>shift", "shift>mul"):
        return "shift"
    return word.split("[")[0]


def changed_lines(base, text):
    """(line indices of base that the edit replaces, identifiers on the changed lines of both)."""
    a, b = base.split("\n"), text.split("\n")
    lo = 0
    while lo < min(len(a), len(b)) and a[lo] == b[lo]:
        lo += 1
    ea, eb = len(a), len(b)
    while ea > lo and eb > lo and a[ea - 1] == b[eb - 1]:
        ea, eb = ea - 1, eb - 1
    span = set(range(lo, ea)) if ea > lo else {lo - 1, lo}     # an insertion touches its neighbours
    names = set(re.findall(r"[A-Za-z_]\w*", mask("\n".join(a[lo:ea] + b[lo:eb]))))
    return span, names


def rank_variants(vs, hint, base):
    """`vs` ordered by `hint`: edits of a suggested kind that touch a named variable or line, then
    the other edits of a suggested kind (both in HINT_EDITS order), then other edits that touch
    the location, then the rest in their original order. Nothing is dropped."""
    if hint is None or hint.error or not (hint.classes or hint.names or hint.lines):
        return list(vs)
    kinds = hint.kinds
    want_lines, want_names = set(hint.lines), set(hint.names)

    def key(item):
        i, v = item
        k = edit_kind(v.label)
        span, names = changed_lines(base, v.text)
        touch = bool(span & want_lines or names & want_names)
        ki = kinds.index(k) if k in kinds else None
        group = (0 if touch else 1) if ki is not None else (2 if touch else 3)
        return (group, ki or 0, i)
    return [v for _i, v in sorted(enumerate(vs), key=key)]


def import_regdiff():
    """tools/regdiff.py, imported with the standard library first on sys.path: tools/queue.py
    shadows the stdlib queue (as in tools/permute.py and tools/bulk_m2c.py)."""
    if "regdiff" in sys.modules:
        return sys.modules["regdiff"]
    here = os.path.dirname(os.path.abspath(__file__))
    saved = sys.path[:]
    sys.path[:] = [p for p in saved if os.path.abspath(p or ".") != here] + [here]
    try:
        import regdiff
    finally:
        sys.path[:] = saved
    return regdiff


def regdiff_uopt():
    """The traced uopt regdiff would use ($REGDIFF_UOPT, else its default under /scratch, which
    is TOKIMEMO_SCRATCH in Docker), or None when it or the MIPS binutils are missing."""
    import shutil
    uopt = os.environ.get("REGDIFF_UOPT") or import_regdiff().DEFAULT_UOPT
    if not os.path.exists(uopt) or shutil.which("mips-linux-gnu-objdump") is None:
        return None
    return uopt


def make_advisor(root, draft, uopt=None, regdiff=None):
    """advise(text) -> Hint for a variant of `draft`: regdiff runs in-process on a copy of the
    draft's source file (build/autotweak/<unit>/<func>/regdiff.c) with the variant in place of the
    guard block. None when no traced uopt is available. Cached per text."""
    uopt = uopt or regdiff_uopt()
    if uopt is None:
        return None
    rd = regdiff or import_regdiff()
    root = os.path.abspath(root)
    work = os.path.join(root, WORK_ROOT, draft.unit, draft.func)
    src = os.path.join(work, "regdiff.c")
    with open(os.path.join(root, draft.src)) as fh:
        original = fh.read()
    cache = {}

    def advise(text):
        if text in cache:
            return cache[text]
        full = rewrite_guard(original, draft.func, text)
        if full is None:
            import permute
            full = permute.replace_function(original, draft.func, text)
        at = full.find(text.strip("\n"))
        first_line = full.count("\n", 0, at) + 1 if at >= 0 else 1
        os.makedirs(work, exist_ok=True)
        with open(src, "w") as fh:
            fh.write(full)
        args = argparse.Namespace(func=draft.func, unit=draft.unit, src=src, uopt=uopt,
                                  scratch=os.path.join(work, "regdiff"), verbose=False)
        lines, cwd = [], os.getcwd()
        try:
            os.chdir(root)
            with contextlib.redirect_stdout(io.StringIO()):
                rd.run(args, out=lines.append)
            hint = parse_regdiff("\n".join(lines), first_line)
        except Exception as exc:  # noqa: BLE001  (guidance only: the climb goes on unguided)
            hint = Hint(error="%s: %s" % (type(exc).__name__, exc))
        finally:
            os.chdir(cwd)
        cache[text] = hint
        return hint
    return advise


# ---------------------------------------------------------------------------------------------
# Search
# ---------------------------------------------------------------------------------------------

class Result:
    def __init__(self, base, best, text, path, tried, matched=False, error=None, unverified=False, hint=None):
        self.base, self.best, self.text, self.path = base, best, text, path
        self.tried, self.matched, self.error = tried, matched, error
        self.unverified = unverified   # the draft scored 0 but failed verification
        self.hint = hint               # regdiff's Hint for the best text (T-11220), or None


def climb(text, score, verify=lambda text: True, budget=300.0, cap=300, pair_seeds=8, pair_cap=200,
          clock=time.monotonic, order=True, order_opts=None, advise=None, prefetch=None):
    """Greedy hill climb over `variants`. score(text) -> int or None (does not compile);
    verify(text) -> bool is asked for every score of 0, and only a verified 0 is a match. When no
    single edit or pair improves, `order` runs order_stage (T-11200) once per best text, with
    `order_opts` (max_exhaustive, budget, max_runs, max_swaps). advise(text) -> Hint (T-11220,
    make_advisor) is asked for the start text and every new best text; its hint orders the single
    edits and, when they stall, the pairs (rank_variants). Returns a Result."""
    deadline = clock() + budget
    base = score(text)
    if base is None:
        return Result(None, None, text, [], 0, error="draft does not compile")
    if base == 0 and verify(text):
        return Result(base, 0, text, [], 0, matched=True)
    # A 0 that fails verification is not a match (T-11200): rank it just above 0, so that only a
    # verified 0 improves on it and the search goes on looking for one.
    best, best_text, path, tried = base or 1, text, [], 0
    seen, order_done = {text}, set()
    hint, hint_text = None, None

    count_lock = threading.Lock()   # the order stage may score from several threads (T-11250)

    def counted(t):
        nonlocal tried
        with count_lock:
            seen.add(t)
            tried += 1
        return score(t)

    def evaluate(v):
        s = counted(v.text)
        if s == 0 and not verify(v.text):
            return None
        return s

    while best > 0 and clock() < deadline:
        found, scored = None, []
        if advise is not None:
            hint, hint_text = advise(best_text), best_text
        ranked = rank_variants([v for v in variants(best_text) if v.text not in seen], hint, best_text)[:cap]
        if prefetch is not None:
            prefetch([v.text for v in ranked])
        for v in ranked:
            if clock() >= deadline:
                break
            s = evaluate(v)
            if s is None:
                continue
            scored.append((s, v))
            if s < best and (found is None or s < found[0]):
                found = (s, [v.label])
                found_text = v.text
                if s == 0:
                    break
        if found is None:
            scored.sort(key=lambda sv: sv[0])
            budget_left = pair_cap
            pairs = {}
            if prefetch is not None:    # the texts the loop below will score, in its order
                planned, left = set(seen), pair_cap
                for s1, v1 in scored[:pair_seeds]:
                    lst = []
                    for v2 in rank_variants(variants(v1.text), hint, v1.text):
                        if left <= 0:
                            break
                        if v2.text not in planned:
                            planned.add(v2.text)
                            left -= 1
                            lst.append(v2)
                    pairs[v1.text] = lst
                prefetch([v2.text for lst in pairs.values() for v2 in lst])
            for s1, v1 in scored[:pair_seeds]:
                for v2 in (pairs[v1.text] if prefetch is not None else rank_variants(variants(v1.text), hint, v1.text)):
                    if budget_left <= 0 or clock() >= deadline:
                        break
                    if v2.text in seen:
                        continue
                    budget_left -= 1
                    s = evaluate(v2)
                    if s is not None and s < best and (found is None or s < found[0]):
                        found = (s, [v1.label, v2.label])
                        found_text = v2.text
                        if s == 0:
                            break
                if found and found[0] == 0:
                    break
        if found is None and order and best_text not in order_done and clock() < deadline:
            order_done.add(best_text)
            o = order_stage(best_text, counted, verify, deadline, clock, **(order_opts or {}))
            if o is not None and o[0] < best:
                found, found_text = (o[0], [o[2]]), o[1]
        if found is None:
            break
        best, best_text = found[0], found_text
        path += found[1]
    if advise is not None and best > 0 and hint_text != best_text:
        hint = advise(best_text)       # report the divergence that is left
    unverified = base == 0 and best_text == text
    return Result(base, 0 if unverified else best, best_text, path, tried, matched=best == 0,
                  unverified=unverified, hint=hint)


# ---------------------------------------------------------------------------------------------
# Drafts in src/ and rewriting the guard on a match
# ---------------------------------------------------------------------------------------------

GUARD_RE = re.compile(r"^[ \t]*#[ \t]*if(?:def[ \t]+NON_MATCHING|[ \t]+defined\(NON_MATCHING\))[^\n]*\n", re.M)
DIRECTIVE_RE = re.compile(r"^[ \t]*#[ \t]*(if|else|endif)\w*[^\n]*\n?", re.M)
RODATA_RE = re.compile(r"^[ \t]*INCLUDE_RODATA\([^)]*\);[ \t]*\n?", re.M)
NOTE_RE = re.compile(r"NON_MATCHING|near miss", re.I)


def is_note(comment):
    """A NON_MATCHING note (dropped on a match); FAKE comments always stay."""
    return bool(NOTE_RE.search(comment)) and not re.match(r"\s*/\*\s*FAKE", comment)


class Guard:
    """One `#ifdef NON_MATCHING` block: offsets of the guard line, `#else`, `#endif` (after)."""

    def __init__(self, start, body, else_start, else_end, end, func):
        self.start, self.body, self.else_start, self.else_end, self.end = start, body, else_start, else_end, end
        self.func = func


def guards(text):
    out = []
    for g in GUARD_RE.finditer(text):
        depth, els = 1, None
        for d in DIRECTIVE_RE.finditer(text, g.end()):
            kind = d.group(1)
            if kind == "if":
                depth += 1
            elif kind == "endif":
                depth -= 1
                if depth == 0:
                    if els is not None:
                        m = re.search(r"INCLUDE_ASM\([^,]*,\s*(\w+)\s*\)", text[els[1]:d.start()])
                        if m:
                            out.append(Guard(g.start(), g.end(), els[0], els[1], d.end(), m.group(1)))
                    break
            elif depth == 1:
                els = (d.start(), d.end())
    return out


def definition_span(text, func, lo, hi):
    """(start, end) of the definition of `func` within text[lo:hi], or None."""
    import permute
    m = permute.def_regex(func).search(mask(text), lo, hi)
    if not m:
        return None
    end = permute.match_brace(mask(text), m.end() - 1)
    return (m.start(), end) if 0 < end <= hi else None


class Draft:
    def __init__(self, src, unit, func, text):
        self.src, self.unit, self.func, self.text = src, unit, func, text

    @property
    def scope(self):
        return "%s:%s" % (self.unit, self.func)


def find_drafts(root, scope=None, exclude=()):
    import funcloc
    want_unit, want_func = None, None
    if scope:
        want_func, want_unit = funcloc.split_scope(scope)
    out = []
    for path in sorted(glob.glob(os.path.join(root, "src", "**", "*.c"), recursive=True)):
        rel = os.path.relpath(path, root)
        unit = funcloc.unit_of_path(rel)
        if unit in exclude or (want_unit and unit != want_unit):
            continue
        with open(path) as fh:
            text = fh.read()
        for g in guards(text):
            if want_func and g.func != want_func:
                continue
            span = definition_span(text, g.func, g.body, g.else_start)
            if span:
                out.append(Draft(rel, unit, g.func, text[span[0]:span[1]]))
    return out


def rewrite_guard(text, func, definition):
    """`text` with the NON_MATCHING block of `func` replaced by `definition`: comments of the
    block are kept except the NON_MATCHING note, INCLUDE_RODATA lines are kept, a NON_MATCHING
    note right above the block is dropped. None when `func` has no such block."""
    for g in guards(text):
        if g.func != func:
            continue
        span = definition_span(text, func, g.body, g.else_start)
        if not span:
            return None
        head = text[g.body:span[0]]
        kept = "".join(c.group(0) + "\n" for c in re.finditer(r"/\*.*?\*/", head, re.S)
                       if not is_note(c.group(0)))
        block = text[g.body:g.end]
        rodata = "".join(m.group(0).rstrip("\n") + "\n" for m in RODATA_RE.finditer(block))
        start = g.start
        before = text[:start]
        m = re.search(r"(^[ \t]*/\*(?:(?!\*/).)*\*/[ \t]*\n)\Z", before, re.S | re.M)
        if m and is_note(m.group(1)):
            start = m.start(1)
        new = kept + rodata + definition.strip("\n") + "\n"
        return text[:start] + new + text[g.end:]
    return None


# ---------------------------------------------------------------------------------------------
# The real scorer (Docker only: permuter, IDO, ninja)
# ---------------------------------------------------------------------------------------------

class BuildScorer:
    """Scores a variant of one draft by compiling only that function (permute.py's pipeline)."""

    # permute.setup refuses a draft whose instruction count is further than this from the target;
    # tools/bulk_m2c.py raises it to score raw m2c drafts (T-11190)
    INSTRUCTION_TOLERANCE = 4

    def __init__(self, root, draft):
        import permute
        self.permute, self.root, self.draft = permute, os.path.abspath(root), draft
        self.dir = os.path.join(self.root, WORK_ROOT, draft.unit, draft.func)
        os.makedirs(self.dir, exist_ok=True)
        self.scratch = os.path.join(self.dir, "context.c")
        with open(os.path.join(self.root, draft.src)) as fh:
            self.original = fh.read()
        self._exact_lock, self._score_lock, self._verify_lock = threading.Lock(), threading.Lock(), threading.Lock()
        self._cache, self._jobs, self._workers, self._builder = {}, {}, None, None
        self.exact, self.jobs = True, 1
        self._write(draft.text)
        with contextlib.redirect_stdout(io.StringIO()):
            permute.setup(self.root, draft.func, src=self.scratch, out=self.dir, unit=draft.unit,
                          instruction_tolerance=self.INSTRUCTION_TOLERANCE)
        permute.install_permuter_compat()
        from src.compiler import Compiler
        from src.scorer import Scorer
        self.compiler = Compiler(os.path.join(self.dir, "compile.sh"), show_errors=False, debug_mode=False)
        self.scorer = Scorer(os.path.join(self.dir, "target.o"), stack_differences=True, algorithm="difflib",
                             debug_mode=False, ign_branch_targets=False, objdump_command=None)
        self.configure(jobs=1, exact=False)

    def configure(self, jobs=1, exact=False):
        """Scoring threads for prefetch/order search and the legacy switch (T-11250). exact: the
        original path (rewrite the scratch file, permute.build_base, one candidate at a time),
        kept to compare scores; the default path builds the same base text from a cached strip."""
        self.exact = exact
        self.jobs = 1 if exact else max(1, jobs)
        self._workers = fastscore.Workers(self.jobs) if self.jobs > 1 else None
        self._builder = None if exact else fastscore.BaseBuilder(self.permute, self.root, self.draft.func)

    def close(self):
        if self._workers is not None:
            self._workers.close()
            self._workers = None

    def _text(self, definition):
        text = rewrite_guard(self.original, self.draft.func, definition)
        if text is None:
            text = self.permute.replace_function(self.original, self.draft.func, definition)
        return text

    def _write(self, definition):
        with open(self.scratch, "w") as fh:
            fh.write(self._text(definition))

    def score_exact(self, definition):
        """The original scoring path (one scratch file, one candidate at a time)."""
        with self._exact_lock:
            self._write(definition)
            try:
                with contextlib.redirect_stdout(io.StringIO()):
                    base = self.permute.build_base(self.root, self.scratch, self.draft.func)
            except SystemExit:
                return None
            return self._compile_score(base)

    def _compile_score(self, base):
        obj = self.compiler.compile(base, show_errors=False)
        if obj is None:
            return None
        try:
            with self._score_lock:      # the permuter's Scorer reuses one SequenceMatcher
                return self.scorer.score(obj)[0]
        finally:
            os.unlink(obj)

    def _score_fast(self, definition):
        text = self._text(definition)
        try:
            base = self._builder.build(text)
        except SystemExit:
            return None
        s = self._compile_score(base)
        self._cache[definition] = s
        return s

    def score(self, definition):
        if self.exact or self._builder is None:
            return self.score_exact(definition)
        if definition in self._cache:
            return self._cache[definition]
        job = self._jobs.pop(definition, None)
        if job is not None:
            job.done.wait()
            if job.state == "done":
                return job.result()
        return self._score_fast(definition)

    def prefetch(self, texts):
        """Queue the scores of `texts` (in the order the climb will ask for them) on the worker
        threads and drop the jobs of an earlier call that have not started."""
        if self._workers is None:
            return
        self._workers.cancel_pending()
        for t in texts:
            job = self._jobs.get(t)
            if t not in self._cache and (job is None or job.state == "cancelled"):
                self._jobs[t] = self._workers.submit(self._score_fast, t)

    def verify(self, definition):
        meta = {"func": self.draft.func, "unit": self.draft.unit, "src": self.draft.src}
        with self._verify_lock, tempfile.TemporaryDirectory(prefix="verify-", dir=self.dir) as cand:
            with open(os.path.join(cand, "source.c"), "w") as fh:
                fh.write(definition + "\n")
            ok, _report = self.permute.verify_candidate(self.root, meta, cand)
        return ok


def run_one(root, draft, args, make_scorer=BuildScorer, out=print):
    try:
        scorer = make_scorer(root, draft)
    except (SystemExit, RuntimeError, OSError, ImportError) as exc:
        out("SKIP %s  setup: %s" % (draft.scope, str(exc).strip().splitlines()[-1][:120] if str(exc).strip() else type(exc).__name__))
        return None
    verify = (lambda text: True) if args.no_verify else scorer.verify
    advise = None if args.no_regdiff else make_advisor(root, draft)
    exact = getattr(args, "exact", False)
    jobs = 1 if exact else getattr(args, "score_jobs", None) or fastscore.default_jobs()
    if hasattr(scorer, "configure"):
        scorer.configure(jobs=jobs, exact=exact)
    try:
        res = climb(draft.text, scorer.score, verify, budget=args.time, cap=args.cap, pair_cap=args.pair_cap,
                    order=not args.no_order, order_opts={"max_exhaustive": args.max_exhaustive,
                                                         "max_swaps": args.order_swaps, "jobs": jobs},
                    advise=advise, prefetch=getattr(scorer, "prefetch", None) if jobs > 1 else None)
    finally:
        if hasattr(scorer, "close"):
            scorer.close()
    if res.error:
        out("SKIP %s  %s" % (draft.scope, res.error))
        return res
    edits = ", ".join(res.path) if res.path else "none"
    if res.matched:
        fakes = [l for l in res.path if l.split()[0] in ("byteview", "emptytest", "pad")]
        if not args.dry_run:
            path = os.path.join(root, draft.src)
            with open(path) as fh:
                new = rewrite_guard(fh.read(), draft.func, res.text)
            if new is None:
                out("ERROR %s  guard block not found for rewrite" % draft.scope)
                return res
            with open(path, "w") as fh:
                fh.write(new)
        out("MATCH %s  %d -> 0  [%s]%s%s" % (draft.scope, res.base, edits, " FAKE" if fakes else "",
                                              " (dry run)" if args.dry_run else ""))
    else:
        if res.best < res.base:
            os.makedirs(os.path.join(root, WORK_ROOT, draft.unit, draft.func), exist_ok=True)
            with open(os.path.join(root, WORK_ROOT, draft.unit, draft.func, "best.c"), "w") as fh:
                fh.write(res.text + "\n")
        out("%s  %d -> %d  [%s]  %d tried%s%s" % (draft.scope, res.base, res.best, edits, res.tried,
                                                   "  regdiff: %s" % res.hint.summary() if res.hint else "",
                                                   "  (score 0 failed verification)" if res.unverified else ""))
    return res


def parse_args(argv):
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    p.add_argument("func", nargs="?", help="[UNIT:]func_X")
    p.add_argument("--unit", help="unit of func (or use a UNIT: prefix)")
    p.add_argument("--all", action="store_true", help="every guarded draft in src/")
    p.add_argument("--exclude", default="", help="comma-separated units to skip with --all")
    p.add_argument("--time", type=float, default=300, help="search seconds per function (default 300)")
    p.add_argument("--cap", type=int, default=300, help="single edits scored per step (default 300)")
    p.add_argument("--pair-cap", type=int, default=200, help="depth-2 edits scored per step (default 200)")
    p.add_argument("--list", action="store_true", help="list drafts and their single-edit count; no compile")
    p.add_argument("--dry-run", action="store_true", help="do not write src/ on a match")
    p.add_argument("--no-verify", action="store_true", help="accept a score of 0 without the ninja check")
    p.add_argument("--no-order", action="store_true", help="skip the statement-order stage (T-11200)")
    p.add_argument("--no-regdiff", action="store_true",
                   help="do not order the edits by regdiff's first divergence (T-11220)")
    p.add_argument("--max-exhaustive", type=int, default=2000,
                   help="order stage: variants per search, as in order_search.py (default 2000)")
    p.add_argument("--order-swaps", type=int, default=2,
                   help="order stage: operand swaps (+, ==, !=) added to the blocks (default 2)")
    p.add_argument("--score-jobs", type=int, default=0,
                   help="candidates compiled in parallel (default: cores - 2; T-11250)")
    p.add_argument("--exact", action="store_true",
                   help="legacy scoring: one candidate at a time through permute.build_base (T-11250)")
    p.add_argument("--root", default=".", help=argparse.SUPPRESS)
    args = p.parse_args(argv)
    if not args.all and not args.func:
        p.error("give [UNIT:]func_X or --all")
    return args


def main(argv=None, make_scorer=BuildScorer, out=print):
    args = parse_args(sys.argv[1:] if argv is None else argv)
    scope = None
    if args.func:
        scope = "%s:%s" % (args.unit, args.func) if args.unit and ":" not in args.func else args.func
    exclude = {u for u in args.exclude.split(",") if u}
    drafts = find_drafts(args.root, None if args.all else scope, exclude)
    if not drafts:
        out("autotweak.py: no guarded draft for %s" % (scope or "--all"))
        return 1
    if not args.all and len(drafts) > 1:
        out("autotweak.py: %s is guarded in several units (%s); add UNIT:" %
            (scope, ", ".join(d.unit for d in drafts)))
        return 1
    if args.list:
        for d in drafts:
            out("%s  %s  %d edits" % (d.scope, d.src, len(variants(d.text))))
        return 0
    matched = 0
    for d in drafts:
        res = run_one(args.root, d, args, make_scorer, out)
        matched += bool(res and res.matched)
    if args.all:
        out("%d/%d matched" % (matched, len(drafts)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
