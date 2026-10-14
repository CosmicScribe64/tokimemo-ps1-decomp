#!/usr/bin/env python3
"""Type-mismatch fixes for tools/bulk_m2c.py and tools/autotweak.py (T-11240).

Part A, post-m2c rewrites of a function text (`rewrite`). m2c prints `*(D + i)` for a word that is
a pointer, `(&sp2C[0])[i](x)` for a call through a table copied to the stack, `arg0 + 3` for byte
arithmetic on a `void *`, `(unaligned s32)` for lwl/lwr pairs and `func_800573F8` for a function the
project has renamed. IDO rejects each of these (or the header check refuses the declaration m2c's
name needs). The rewrites keep the instruction stream: a cast through the address, a call through
a function-pointer cast, a `u8 *` cast for the arithmetic, the current name of a renamed symbol.
They are driven by a small C expression parser with just enough typing to know when `*E` has an
integer operand.

Part B, autotweak edit kinds (`TRANSFORMS`, registered by `register`): the type-driven mismatches
of a score. lh vs lhu and lb vs lbu (width and signedness of a local, parameter, cast or the view
of a global), sra vs srl and sltu vs slt (a `(u32)`/`(s32)` cast on an operand, a `U` suffix),
andi 0xFF / 0xFFFF and sll+sra pairs that are missing or extra (`(u8)`, `(s16)` casts, `& 0xFF`).
The scorer exposes no positions, so each edit applies to every candidate; autotweak orders them
round robin with the other edit kinds and its regdiff guidance.

Usage: library only (python3 tools/test_typefix.py for the tests).
"""
import glob
import os
import re

TICKET = "T-11240"

_AT = None


def at():
    """tools/autotweak.py (imported on first use; `register` sets it when autotweak loads us)."""
    global _AT
    if _AT is None:
        import autotweak
        _AT = autotweak
    return _AT


# ---------------------------------------------------------------------------------------------
# Types
# ---------------------------------------------------------------------------------------------

INT_BASES = {"s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64"}
NARROW = {"s8", "u8", "s16", "u16"}
SIGNED = {"u8": "s8", "s8": "u8", "u16": "s16", "s16": "u16", "u32": "s32", "s32": "u32"}
BASE_WORDS = {"unsigned char": "u8", "signed char": "s8", "char": "s8", "unsigned short": "u16",
              "short": "s16", "short int": "s16", "unsigned short int": "u16", "int": "s32",
              "unsigned": "u32", "unsigned int": "u32", "signed": "s32", "signed int": "s32",
              "long": "s32", "unsigned long": "u32", "long int": "s32"}
TYPE_WORDS = {"s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64", "float", "double",
              "void", "char", "short", "int", "long", "unsigned", "signed", "struct", "union", "enum",
              "const", "volatile"}
QUAL_WORDS = {"const", "volatile", "struct", "union", "enum", "register", "static", "extern"}
ADDRESS_NAME = re.compile(r"^D_[0-9A-Fa-f]{8}$")
FN = "fn"


def norm_base(words):
    """Canonical base name of the words of a type (`unsigned char` -> u8)."""
    words = [w for w in words if w not in QUAL_WORDS] or ["int"]
    text = " ".join(words)
    return BASE_WORDS.get(text, text if len(words) == 1 else words[-1])


def is_int(t):
    return t is not None and t[1] == 0 and t[0] in INT_BASES


def deref(t):
    return (t[0], t[1] - 1)


# ---------------------------------------------------------------------------------------------
# Lexing and the expression tree
# ---------------------------------------------------------------------------------------------

class ParseError(Exception):
    pass


class Node:
    """An expression: source span [s, e), child nodes in source order, and the rewrite flags."""
    __slots__ = ("kind", "s", "e", "kids", "op", "ty", "cast", "wide", "fnptr", "u8")

    def __init__(self, kind, s, e, kids=(), op=None):
        self.kind, self.s, self.e, self.kids, self.op = kind, s, e, list(kids), op
        self.ty = None
        self.cast = None      # '*' node: cast the operand to `cast *` (integer operand)
        self.wide = False     # '*' node: it loads a pointer, so read a whole word
        self.fnptr = False    # call node: the callee is an integer, call through a pointer cast
        self.u8 = False       # id node: byte arithmetic on a void * (cast to u8 *)


BINARY = {"||": 1, "&&": 2, "|": 3, "^": 4, "&": 5, "==": 6, "!=": 6, "<": 7, ">": 7, "<=": 7, ">=": 7,
          "<<": 8, ">>": 8, "+": 9, "-": 9, "*": 10, "/": 10, "%": 10}
ASSIGN = {"=", "+=", "-=", "*=", "/=", "%=", "|=", "&=", "^=", "<<=", ">>="}
UNARY = {"-", "+", "!", "~", "*", "&", "++", "--"}
OPERAND_START = {"(", "*", "&", "-", "+", "!", "~", "++", "--"}


class Parser:
    """Recursive descent over the tokens of one expression. `types` maps names to (base, stars, dims)."""

    def __init__(self, toks, text, types):
        self.toks, self.text, self.types, self.i = toks, text, types, 0

    # -- token access
    def peek(self, k=0):
        j = self.i + k
        return self.toks[j].text if j < len(self.toks) else None

    def take(self):
        if self.i >= len(self.toks):
            raise ParseError("eof")
        t = self.toks[self.i]
        self.i += 1
        return t

    def expect(self, text):
        t = self.take()
        if t.text != text:
            raise ParseError("expected %s got %s" % (text, t.text))
        return t

    def span(self, a, b):
        return a, b

    # -- grammar
    def expression(self):
        left = self.assign()
        while self.peek() == ",":
            self.take()
            right = self.assign()
            left = Node("comma", left.s, right.e, [left, right], ",")
        return left

    def assign(self):
        left = self.conditional()
        if self.peek() in ASSIGN:
            op = self.take().text
            right = self.assign()
            return Node("assign", left.s, right.e, [left, right], op)
        return left

    def conditional(self):
        cond = self.binary(1)
        if self.peek() == "?":
            self.take()
            a = self.expression()
            self.expect(":")
            b = self.conditional()
            return Node("cond", cond.s, b.e, [cond, a, b], "?")
        return cond

    def binary(self, minprec):
        left = self.unary()
        while True:
            op = self.peek()
            prec = BINARY.get(op)
            if prec is None or prec < minprec:
                return left
            self.take()
            right = self.binary(prec + 1)
            left = Node("bin", left.s, right.e, [left, right], op)

    def is_type_start(self, k):
        t = self.toks[self.i + k] if self.i + k < len(self.toks) else None
        if t is None or t.kind != "id":
            return False
        w = t.text
        if w in TYPE_WORDS:
            return True
        if w in self.types or ADDRESS_NAME.match(w) or w.startswith(("func_", "temp_", "var_", "arg", "sp")):
            return False
        return w[:1].isupper()

    def cast_end(self):
        """Index of the `)` closing a cast that starts at the current `(`, else None."""
        if self.peek() != "(" or not self.is_type_start(1):
            return None
        depth, j = 0, self.i
        while j < len(self.toks):
            t = self.toks[j].text
            if t == "(":
                depth += 1
            elif t == ")":
                depth -= 1
                if depth == 0:
                    break
            elif not (self.toks[j].kind == "id" or t in ("*", "[", "]") or self.toks[j].kind == "num"):
                return None
            j += 1
        else:
            return None
        nxt = self.toks[j + 1] if j + 1 < len(self.toks) else None
        if nxt is None or not (nxt.kind in ("id", "num") or nxt.text in OPERAND_START):
            return None
        return j

    def unary(self):
        t = self.toks[self.i] if self.i < len(self.toks) else None
        if t is None:
            raise ParseError("eof")
        if t.text == "(":
            j = self.cast_end()
            if j is not None:
                first = self.take()
                inner = self.toks[self.i:j]
                self.i = j + 1
                operand = self.unary()
                node = Node("cast", first.start, operand.e, [operand], "cast")
                node.ty = cast_type(inner)
                return node
        if t.text in UNARY:
            self.take()
            operand = self.unary()
            return Node("un", t.start, operand.e, [operand], t.text)
        if t.text == "sizeof":
            self.take()
            if self.peek() == "(":
                self.take()
                depth = 1
                while depth:
                    x = self.take().text
                    depth += {"(": 1, ")": -1}.get(x, 0)
                node = Node("num", t.start, self.toks[self.i - 1].end)
                node.ty = ("s32", 0)
                return node
            operand = self.unary()
            node = Node("num", t.start, operand.e)
            node.ty = ("s32", 0)
            return node
        return self.postfix()

    def postfix(self):
        node = self.primary()
        while True:
            p = self.peek()
            if p == "[":
                self.take()
                idx = self.expression()
                close = self.expect("]")
                node = Node("index", node.s, close.end, [node, idx], "[")
            elif p == "(":
                self.take()
                args = []
                if self.peek() != ")":
                    args.append(self.assign())
                    while self.peek() == ",":
                        self.take()
                        args.append(self.assign())
                close = self.expect(")")
                node = Node("call", node.s, close.end, [node] + args, "(")
            elif p in (".", "->"):
                self.take()
                name = self.take()
                node = Node("member", node.s, name.end, [node], p)
            elif p in ("++", "--"):
                t = self.take()
                node = Node("post", node.s, t.end, [node], p)
            else:
                return node

    def primary(self):
        t = self.take()
        if t.kind == "num":
            node = Node("num", t.start, t.end)
            node.ty = ("u32", 0) if t.text.lower().rstrip("l").endswith("u") else ("s32", 0)
            return node
        if t.kind == "id":
            return Node("id", t.start, t.end, op=t.text)
        if t.text == "(":
            inner = self.expression()
            close = self.expect(")")
            return Node("paren", t.start, close.end, [inner], "(")
        raise ParseError("unexpected %s" % t.text)


def cast_type(inner):
    """(base, stars) of the tokens between the parentheses of a cast."""
    depth, stars, words, paren = 0, 0, [], False
    for t in inner:
        if t.text == "(":
            depth += 1
            paren = True
        elif t.text == ")":
            depth -= 1
        elif t.text == "*":
            stars += 1
        elif t.kind == "id" and depth == 0:
            words.append(t.text)
    if paren:
        return (FN, max(stars, 1))
    return (norm_base(words), stars)


# ---------------------------------------------------------------------------------------------
# Statements: the expression token ranges of a function body
# ---------------------------------------------------------------------------------------------

def match_paren(toks, i):
    depth = 0
    for j in range(i, len(toks)):
        if toks[j].text in ("(", "["):
            depth += 1
        elif toks[j].text in (")", "]"):
            depth -= 1
            if depth == 0:
                return j
    raise ParseError("unbalanced")


def until(toks, i, stop):
    """Index of the first top-level token `stop` at or after i (parentheses and braces nest)."""
    depth = 0
    for j in range(i, len(toks)):
        t = toks[j].text
        if t in ("(", "[", "{"):
            depth += 1
        elif t in (")", "]", "}"):
            depth -= 1
            if depth < 0:
                return j
        elif t == stop and depth == 0:
            return j
    return len(toks)


def expression_ranges(toks, lo, hi):
    """[(first token, end token)] of every expression in the statements toks[lo:hi]."""
    out, i = [], lo
    while i < hi:
        w = toks[i].text
        if w in ("{", "}", ";", "else", "do"):
            i += 1
        elif w in ("if", "while", "switch") and i + 1 < hi and toks[i + 1].text == "(":
            j = match_paren(toks, i + 1)
            out.append((i + 2, j))
            i = j + 1
        elif w == "for" and i + 1 < hi and toks[i + 1].text == "(":
            j = match_paren(toks, i + 1)
            a = i + 2
            for _ in range(2):
                b = until(toks, a, ";")
                out.append((a, b))
                a = b + 1
            out.append((a, j))
            i = j + 1
        elif w == "return":
            j = until(toks, i + 1, ";")
            out.append((i + 1, j))
            i = j + 1
        elif w in ("goto", "break", "continue"):
            i = until(toks, i, ";") + 1
        elif w == "case":
            j = until(toks, i + 1, ":")
            out.append((i + 1, j))
            i = j + 1
        elif w == "default" and i + 1 < hi and toks[i + 1].text == ":":
            i += 2
        elif toks[i].kind == "id" and i + 1 < hi and toks[i + 1].text == ":" and w not in at().KEYWORDS:
            i += 2
        else:
            j = until(toks, i, ";")
            out.append((i, j))
            i = j + 1
    return [(a, b) for a, b in out if b > a]


def parse_ranges(func, types):
    """[(range, Node)] of the parsable expressions in the body of `func` (an autotweak.Func)."""
    lo, hi = func.body_tokens()
    start = next((k for k in range(lo, hi) if func.toks[k].start >= len_to_line(func, func.stmt_line)), hi)
    out = []
    try:
        ranges = expression_ranges(func.toks, start, hi)
    except ParseError:
        return out
    for a, b in ranges:
        p = Parser(func.toks[a:b], func.text, types)
        try:
            node = p.expression()
        except ParseError:
            continue
        if p.i == len(p.toks):
            out.append(((a, b), node))
    return out


def len_to_line(func, line):
    return sum(len(l) + 1 for l in func.lines[:line])


# ---------------------------------------------------------------------------------------------
# Typing: which `*E` has an integer E, which call has an integer callee, which void * is summed
# ---------------------------------------------------------------------------------------------

def var_type(types, name):
    got = types.get(name)
    if got is None:
        return None
    base, stars, dims = got
    return (norm_base(base.split()), stars + len(dims))


def strip_parens(node):
    while node.kind == "paren":
        node = node.kids[0]
    return node


def infer(node, types, want=None):
    """Type of `node` as (base, stars), or None when unknown; sets the rewrite flags of the nodes."""
    k = node.kind
    if k == "num":
        return node.ty
    if k == "id":
        return var_type(types, node.op)
    if k == "paren":
        return infer(node.kids[0], types, want)
    if k == "cast":
        ty = node.ty
        infer(node.kids[0], types, ty if is_int(ty) else want)
        return ty
    if k == "un":
        return infer_unary(node, types, want)
    if k == "index":
        bt = infer(node.kids[0], types)
        infer(node.kids[1], types)
        return (bt[0], bt[1] - 1) if bt is not None and bt[1] > 0 else None
    if k == "call":
        callee = node.kids[0]
        ct = infer(callee, types)
        for a in node.kids[1:]:
            infer(a, types)
        if is_int(ct) and (callee.kind != "id" or callee.op in types):
            node.fnptr = True
        return None
    if k in ("member", "post"):
        t = infer(node.kids[0], types)
        return t if k == "post" else None
    if k == "comma":
        infer(node.kids[0], types)
        return infer(node.kids[1], types, want)
    if k == "cond":
        infer(node.kids[0], types)
        a = infer(node.kids[1], types, want)
        b = infer(node.kids[2], types, want)
        return a or b
    if k == "assign":
        left, right = node.kids
        rc = strip_parens(right)
        lw = rc.ty if rc.kind == "cast" and is_int(rc.ty) else None
        lt = infer(left, types, lw)
        infer(right, types, lt if lt is not None else want)
        return lt
    return infer_binary(node, types)


def infer_unary(node, types, want):
    op, operand = node.op, node.kids[0]
    if op == "*":
        t = infer(operand, types, ("s32", 0))
        if t is None:
            return None
        if t[1] > 0:
            return (t[0], t[1] - 1)
        if t[0] in INT_BASES:
            # an integer holds the address: read it through a pointer of the wanted type
            inner = strip_parens(operand)
            if inner.kind == "un" and inner.op == "*" and inner.cast is None and t[0] in NARROW:
                inner.wide = True      # the inner load fetches a pointer table entry: a whole word
            via = want if is_int(want) else ("s32", 0)
            node.cast = via[0]
            return via
        return None
    t = infer(operand, types)
    if op == "&":
        return (t[0], t[1] + 1) if t is not None else None
    return t if op in ("++", "--") else ("s32", 0)


def infer_binary(node, types):
    left, right = node.kids
    lt, rt = infer(left, types), infer(right, types)
    if node.op not in ("+", "-"):
        return ("s32", 0)
    for side, t in ((left, lt), (right, rt)):
        if t == ("void", 1) and strip_parens(side).kind == "id":
            strip_parens(side).u8 = True      # m2c's byte arithmetic; IDO refuses it on void *
    if lt is not None and lt[1] > 0:
        if rt is not None and rt[1] > 0:
            return ("s32", 0)
        return ("u8", 1) if lt == ("void", 1) else lt
    if rt is not None and rt[1] > 0 and node.op == "+" and lt is not None:
        return ("u8", 1) if rt == ("void", 1) else rt
    return lt if is_int(lt) and is_int(rt) else None


def render(node, text):
    """Source text of `node` with the rewrites its flags (and its descendants') ask for."""
    out, pos = [], node.s
    for n, kid in enumerate(node.kids):
        out.append(text[pos:kid.s])
        piece = render(kid, text)
        if node.kind == "call" and n == 0 and node.fnptr:
            piece = "((s32 (*)())%s)" % piece
        out.append(piece)
        pos = kid.e
    out.append(text[pos:node.e])
    s = "".join(out)
    if node.kind == "id" and node.u8:
        return "((u8 *)%s)" % node.op
    if node.kind == "un" and node.op == "*" and (node.cast or node.wide):
        return "*(%s *)%s" % (node.cast or "s32", render(node.kids[0], text))
    return s


# ---------------------------------------------------------------------------------------------
# Part A drivers
# ---------------------------------------------------------------------------------------------

VAR_RE = re.compile(r"^[ \t]*((?:[A-Za-z_]\w*[ \t]+)+?)(\**)[ \t]*([A-Za-z_]\w*)((?:\[[^\]]*\])*)[ \t]*(?:;|=[^=]|[,)])")
NOT_TYPES = {"return", "typedef", "goto", "else", "case", "default", "break", "continue", "sizeof", "do"}
QUALS = {"extern", "static", "const", "volatile", "register"}


def parse_var(line):
    """(name, (base, stars, dims)) of a declaration or parameter, or None."""
    if "(" in line.split(";")[0].split("=")[0]:
        return None
    m = VAR_RE.match(line)
    if not m:
        return None
    words = [w for w in m.group(1).split() if w not in QUALS]
    if not words or words[0] in NOT_TYPES:
        return None
    return m.group(3), (" ".join(words), len(m.group(2)), re.findall(r"\[[^\]]*\]", m.group(4)))


def local_types(text):
    """{name: (base, stars, dims)} of the parameters and declaration lines of a function text."""
    types = {}
    head = re.search(r"\w+\(([^)]*)\)\s*\{", text)
    if head and head.group(1).strip() not in ("", "void"):
        depth, cur, parts = 0, "", []
        for ch in head.group(1):
            depth += {"(": 1, ")": -1}.get(ch, 0)
            if ch == "," and depth == 0:
                parts.append(cur)
                cur = ""
            else:
                cur += ch
        parts.append(cur)
        for part in parts:
            got = parse_var(part.strip() + ";")
            if got:
                types[got[0]] = got[1]
    for line in text.splitlines():
        got = parse_var(line)
        if got:
            types.setdefault(got[0], got[1])
    return types


def fix_expressions(text, types=None):
    """Dereferences of integers, calls of integers and sums on `void *` fixed in the expression
    statements of the function `text`; `types` adds the globals' types."""
    all_types = dict(types or {})
    all_types.update(local_types(text))
    try:
        func = at().Func(text)
    except ValueError:
        return text
    edits = []
    for _rng, node in parse_ranges(func, all_types):
        infer(node, all_types)
        new = render(node, func.text)
        if new != func.text[node.s:node.e]:
            edits.append((node.s, node.e, new))
    return at().splice(text, edits) if edits else text


def read_renames(root="."):
    """{old name: new name} of config/obin_renames.txt."""
    out = {}
    path = os.path.join(root, "config", "obin_renames.txt")
    if os.path.exists(path):
        with open(path) as fh:
            for line in fh:
                parts = line.split()
                if len(parts) == 2 and not line.lstrip().startswith("#"):
                    out[parts[0]] = parts[1]
    return out


_RENAMES = {}


def renames(root="."):
    key = os.path.abspath(root)
    if key not in _RENAMES:
        _RENAMES[key] = read_renames(root)
    return _RENAMES[key]


NAME_RE = re.compile(r"\b(?:func|D)_[0-9A-Fa-f]{8}\b")


def rename_symbols(text, table, keep=()):
    """The current names of renamed symbols (config/obin_renames.txt). The header check refuses a
    declaration under the old name of a symbol that has a new one (alias conflict)."""
    if not table:
        return text
    masked = at().mask(text)
    out, pos = [], 0
    for m in NAME_RE.finditer(masked):
        new = table.get(m.group(0))
        if new and m.group(0) not in keep:
            out.append(text[pos:m.start()] + new)
            pos = m.end()
    return "".join(out) + text[pos:] if out else text


UNSET_RE = re.compile(r"M2C_ERROR\(/\* Read from unset register \$a[0-3] \*/\)")


def drop_unset_args(text):
    """`f(a, M2C_ERROR(/* Read from unset register $a1 */))` -> `f(a)`: the caller never sets the
    register (the asm passes whatever it holds), so the C call has fewer arguments."""
    pat = re.compile(r"(?:,[ \t]*)?" + UNSET_RE.pattern + r"(?=[ \t]*\))")
    while True:
        new = pat.sub("", text, count=1)
        if new == text:
            return text
        text = new


def strip_unaligned(text):
    """`(unaligned s32) x` -> `x`: m2c's marker for an lwl/lwr pair; IDO has no such type."""
    return re.sub(r"\(unaligned [A-Za-z_]\w*(?: \*+)?\)[ \t]*", "", text)


LABEL_END = re.compile(r"((?:^|[;{}])[ \t\n]*(?:default|case\b[^:;{}?]*|[A-Za-z_]\w*)[ \t]*:)([ \t\n]*\})")


def fix_labels(text):
    """A label (case, default, goto target) right before `}` needs an empty statement in C89."""
    masked = at().mask(text)
    edits = [(m.end(1), m.end(1), " ;") for m in LABEL_END.finditer(masked)
             if m.group(1).split()[-1] not in ("else:",)]
    return at().splice(text, edits) if edits else text


ELSE_DEFAULT = re.compile(r"\}[ \t]*else[ \t]*\{[ \t]*\n[ \t]*default:[ \t]*\n[ \t]*\}")
DUP_LABEL = re.compile(r"^([ \t]*(?:default|case[^:\n]*):)[ \t]*\n(?=[ \t]*\1[ \t]*\n)", re.M)


def fix_switch_labels(text):
    """m2c's `} else { default: }` (the default of a bounds-checked switch is the end of the
    function, which a switch without `default` already jumps to) and a repeated `default:`."""
    text = ELSE_DEFAULT.sub("}", text)
    return DUP_LABEL.sub("", text)


BLOCK_RE = re.compile(r"\*\(M2cBlock([0-9A-Fa-f]+) \*\)\(&(\w+)(?:\[0\])?\)")


def grow_block_arrays(text):
    """A block copy to or from `&spXX[0]` needs a stack array as large as the block."""
    for m in BLOCK_RE.finditer(text):
        words = int(m.group(1), 16) // 4
        name = m.group(2)
        decl = re.search(r"^([ \t]*s32[ \t]+%s)\[(\d+)\];" % re.escape(name), text, re.M)
        if decl and int(decl.group(2)) < words:
            text = text[:decl.start()] + "%s[%d];" % (decl.group(1), words) + text[decl.end():]
    return text


def early(text, table=None):
    """Rewrites that must run before the M2C_ERROR check: argument-less calls, renamed callees,
    the unaligned marker."""
    name = re.search(r"\b(\w+)\s*\(", at().mask(text))
    text = drop_unset_args(strip_unaligned(text))
    return rename_symbols(text, renames() if table is None else table,
                          keep=(name.group(1),) if name else ())


def late(text, types=None):
    """The typed rewrites (expression statements) and the layout fixes."""
    return fix_labels(fix_switch_labels(grow_block_arrays(fix_expressions(text, types))))


# ---------------------------------------------------------------------------------------------
# Part B: autotweak edits for type-driven mismatches
# ---------------------------------------------------------------------------------------------

EXTERN_RE = re.compile(r"^[ \t]*extern[ \t]+((?:(?:const|volatile)[ \t]+)*[A-Za-z_]\w*(?:[ \t]+[A-Za-z_]\w*)*?)"
                       r"[ \t]*(\**)[ \t]*(\w+)[ \t]*((?:\[[^\]]*\])*)[ \t]*;", re.M)
_GLOBALS = {}


def global_types(root="."):
    """{name: (base, stars, dims)} of the `extern` objects in include/**/*.h that have one type
    (a name that two headers declare differently is left out)."""
    key = os.path.abspath(root)
    if key not in _GLOBALS:
        seen, bad = {}, set()
        for path in glob.glob(os.path.join(root, "include", "**", "*.h"), recursive=True):
            try:
                with open(path, errors="replace") as fh:
                    text = fh.read()
            except OSError:
                continue
            for m in EXTERN_RE.finditer(at().mask(text)):
                words = [w for w in m.group(1).split() if w not in QUAL_WORDS]
                got = (norm_base(words), len(m.group(2)), tuple(re.findall(r"\[[^\]]*\]", m.group(4))))
                if seen.setdefault(m.group(3), got) != got:
                    bad.add(m.group(3))
        _GLOBALS[key] = {k: v for k, v in seen.items() if k not in bad}
    return _GLOBALS[key]


class Ctx:
    """The parsed expressions of one function and the types of the names they use."""

    def __init__(self, f):
        self.f = f
        self.types = {}
        for name, (typ, ptr, arr) in list(f.params.items()) + list(f.locals.items()):
            self.types[name] = (norm_base([typ]), 1 if ptr else 0, ("[]",) if arr else ())
        self.local = set(self.types)
        for name, got in global_types().items():
            self.types.setdefault(name, got)
        self.trees = parse_ranges(f, {n: (b, s, list(d)) for n, (b, s, d) in self.types.items()})
        for _rng, node in self.trees:
            infer(node, {n: (b, s, list(d)) for n, (b, s, d) in self.types.items()})

    def nodes(self):
        for _rng, root in self.trees:
            yield from walk(root, None, 0)

    def ty(self, name):
        got = self.types.get(name)
        return None if got is None else (got[0], got[1] + len(got[2]))

    def variant(self, edits, label):
        return at().Variant(at().splice(self.f.text, edits), label)

    def line(self, node):
        return self.f.line_of(node.s)

    def src(self, node):
        return self.f.text[node.s:node.e]


def walk(node, parent, idx):
    yield node, parent, idx
    for n, kid in enumerate(node.kids):
        yield from walk(kid, node, n)


def is_store(node, parent, idx):
    """True when `node` is written or addressed there (an lvalue use, not a plain read)."""
    if parent is None:
        return False
    if parent.kind == "assign" and idx == 0:
        return True
    if parent.kind in ("post",) or (parent.kind == "un" and parent.op in ("&", "++", "--")):
        return True
    if parent.kind in ("call", "index", "member") and idx == 0:
        return True
    return False


def needs_parens(node):
    return node.kind not in ("id", "num", "index", "call", "member", "cast", "paren", "un")


COMPARE_OPS = {"<", ">", "<=", ">="}
SHIFT_DIV = {">>", "/", "%"}
CAST_NARROW = ("u8", "s8", "u16", "s16")
VIEW_TYPES = ("u8", "s8", "u16", "s16", "u32", "s32")
DECL_TYPES = ("s8", "u8", "s16", "u16", "s32", "u32")


def t_castadd(f, ctx=None):
    """`(T)x` on a read of an integer name (lbu/lb, lhu/lh via andi or sll/sra, sltu/slt, srl/sra)."""
    ctx = ctx or Ctx(f)
    out = []
    for node, parent, idx in ctx.nodes():
        if node.kind != "id" or is_store(node, parent, idx):
            continue
        t = ctx.ty(node.op)
        if not is_int(t):
            continue
        if parent is not None and parent.kind == "cast":
            continue
        wide = parent is not None and parent.kind == "bin" and parent.op in COMPARE_OPS | SHIFT_DIV
        for cast in ("u32", "s32") if wide else CAST_NARROW:
            if cast != t[0]:
                out.append(ctx.variant([(node.s, node.e, "(%s)%s" % (cast, node.op))],
                                       "cast L%d %s (%s)" % (ctx.line(node), node.op, cast)))
    return out


def t_castflip(f, ctx=None):
    """An existing integer cast: dropped, or its signedness / width changed."""
    ctx = ctx or Ctx(f)
    out = []
    for node, _parent, _idx in ctx.nodes():
        if node.kind != "cast" or not is_int(node.ty):
            continue
        operand = node.kids[0]
        base = node.ty[0]
        label = "L%d (%s)" % (ctx.line(node), base)
        out.append(ctx.variant([(node.s, operand.s, "")], "castdrop " + label))
        news = [SIGNED[base]] if base in SIGNED else []
        news += [t for t in CAST_NARROW if t != base and t not in news] if base in NARROW else []
        for new in news:
            out.append(ctx.variant([(node.s, operand.s, "(%s)" % new)], "castflip %s->%s" % (label, new)))
    return out


def t_mask(f, ctx=None):
    """`x & 0xFF` <-> `(u8)x`, `x & 0xFFFF` <-> `(u16)x`, and the mask dropped (andi missing or extra)."""
    ctx = ctx or Ctx(f)
    out = []
    text = f.text
    for node, _parent, _idx in ctx.nodes():
        if node.kind == "bin" and node.op == "&":
            for a, b in ((0, 1), (1, 0)):
                num = strip_parens(node.kids[b])
                if num.kind != "num":
                    continue
                value = at().int_value(text[num.s:num.e])
                if value not in (0xFF, 0xFFFF):
                    continue
                other = node.kids[a]
                body = text[other.s:other.e]
                cast = "u8" if value == 0xFF else "u16"
                wrapped = "(%s)%s" % (cast, "(%s)" % body if needs_parens(other) else body)
                out.append(ctx.variant([(node.s, node.e, wrapped)], "mask L%d >cast" % ctx.line(node)))
                out.append(ctx.variant([(node.s, node.e, body if not needs_parens(other) else "(%s)" % body)],
                                       "mask L%d drop" % ctx.line(node)))
        elif node.kind == "cast" and is_int(node.ty) and node.ty[0] in ("u8", "u16"):
            operand = node.kids[0]
            body = text[operand.s:operand.e]
            body = "(%s)" % body if needs_parens(operand) else body
            mask = "0xFF" if node.ty[0] == "u8" else "0xFFFF"
            out.append(ctx.variant([(node.s, node.e, "(%s & %s)" % (body, mask))],
                                   "mask L%d cast>" % ctx.line(node)))
    return out


def view_types(base):
    first = [SIGNED[base]] if base in SIGNED else []
    return first + [t for t in VIEW_TYPES if t != base and t not in first]


def t_view(f, ctx=None):
    """A read of a global through a differently typed view: `*(u16 *)&D_X`, `((u16 *)D_X)[i]`."""
    ctx = ctx or Ctx(f)
    out = []
    for node, parent, idx in ctx.nodes():
        if node.kind != "id" or node.op in ctx.local:
            continue
        got = ctx.types.get(node.op)
        if got is None or got[0] not in INT_BASES or got[1] != 0:
            continue
        array_use = parent is not None and parent.kind == "index" and idx == 0
        if got[2] and not array_use:
            continue
        if not got[2] and (is_store(node, parent, idx) or array_use):
            continue
        for new in view_types(got[0]):
            if got[2]:
                rep = "((%s *)%s)" % (new, node.op)
            else:
                rep = "(*(%s *)&%s)" % (new, node.op)
            out.append(ctx.variant([(node.s, node.e, rep)],
                                   "view L%d %s %s->%s" % (ctx.line(node), node.op, got[0], new)))
    return out


ACCESS_CAST = re.compile(r"\(\s*(?:unsigned\s+)?(s8|u8|s16|u16|s32|u32|char)\s*\*\s*\)$")


def t_access(f, ctx=None):
    """The type of a `*(T *)p` / `((T *)p)[i]` access: signedness first, then the other widths."""
    ctx = ctx or Ctx(f)
    out = []
    text = f.text
    for node, _parent, _idx in ctx.nodes():
        if node.kind != "cast" or node.ty is None or node.ty[1] != 1 or node.ty[0] not in INT_BASES:
            continue
        head = text[node.s:node.kids[0].s].strip()
        if not ACCESS_CAST.match(head):
            continue
        base = node.ty[0]
        news = [SIGNED[base]] if base in SIGNED else []
        news += [t for t in DECL_TYPES if t != base and t not in news]
        for new in news:
            out.append(ctx.variant([(node.s, node.kids[0].s, "(%s *)" % new)],
                                   "access L%d %s->%s" % (ctx.line(node), base, new)))
    return out


def t_usuffix(f, ctx=None):
    """A `U` suffix added to or dropped from a constant in a comparison (sltu vs slt)."""
    ctx = ctx or Ctx(f)
    out = []
    for node, parent, _idx in ctx.nodes():
        if node.kind != "num" or parent is None or parent.kind != "bin" or parent.op not in COMPARE_OPS:
            continue
        lit = f.text[node.s:node.e]
        if lit[-1] in "uU":
            rep = lit[:-1]
        else:
            value = at().int_value(lit)
            if value is None or value > 0x7FFFFFFF:
                continue
            rep = lit + "U"
        out.append(ctx.variant([(node.s, node.e, rep)], "usuffix L%d %s" % (ctx.line(node), lit)))
    return out


def t_decl(f, ctx=None):
    """The declared type of a local or parameter, beyond the sign and width pairs of t_width:
    every other 8/16/32-bit integer type (lb vs lh, lbu vs lhu)."""
    out = []
    regions = [(0, f.brace)]
    starts = [sum(len(l) + 1 for l in f.lines[:d]) for d in f.decl_lines]
    regions += [(s, s + len(f.lines[d])) for s, d in zip(starts, f.decl_lines)]
    for a, b in regions:
        for m in re.finditer(r"\b(s8|u8|s16|u16|s32|u32)(\s+)(\w+)\s*(?=[,;)=])", f.masked[a:b]):
            if m.group(3) == f.name:
                continue
            tried = set(at().WIDTHS.get(m.group(1), ())) | {m.group(1)}
            for new in DECL_TYPES:
                if new in tried:
                    continue
                text = f.text[:a + m.start(1)] + new + f.text[a + m.end(1):]
                out.append(at().Variant(text, "decl %s %s->%s" % (m.group(3), m.group(1), new)))
    return out


TRANSFORMS = (t_castadd, t_castflip, t_mask, t_view, t_access, t_usuffix, t_decl)
KINDS = ("cast", "castdrop", "castflip", "mask", "view", "access", "usuffix", "decl")


def register(module):
    """Add the edits to autotweak's list (`module` is the loaded autotweak) and to the kinds its
    regdiff hints try for a structure, argument or ugen divergence."""
    global _AT
    _AT = module
    if not all(t in module.TRANSFORMS for t in TRANSFORMS):
        module.TRANSFORMS = tuple(module.TRANSFORMS) + TRANSFORMS
    for hint in ("structure", "argument", "ugen"):
        kinds = tuple(module.HINT_EDITS.get(hint, ()))
        module.HINT_EDITS[hint] = kinds + tuple(k for k in KINDS if k not in kinds)
