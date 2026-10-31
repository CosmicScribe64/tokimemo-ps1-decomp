#!/usr/bin/env python3
"""T-39300: generate typed GDScript views of the original RAM layout.

Reads the struct layouts of include/game/*.h and include/main/*.h (offset comments
`/* 0xNNN */`, `/* size 0xNN */`, typedefs from include/common.h and include/psx_types.h)
and the extern data declarations with their addresses (config/symbol_addrs*.txt), and
writes port/godot/core/game_layout.gd: offsets, widths, signedness, arrays, nested
structs and bit-fields of GameState (0x800E6280, 0x1A90 bytes), SystemData and the
gameplay globals of the main executable.

The layout is computed from the member types with C alignment rules; every
`/* 0xNNN */` comment and every `/* size 0xNN */` comment is a check against that
computation (see layout_errors), not an input. The output is deterministic.

usage (Docker, e.g. the tokimemo-recovery image):
  python3 -I tools/port/gen_game_memory.py [--check] [--out PATH]
"""
import argparse
import ast
import operator
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
DEFAULT_OUT = REPO / "port/godot/core/game_layout.gd"
HEADER_GLOBS = ["include/*.h", "include/game/*.h", "include/main/*.h", "include/main/import_views/*.h"]
SYMBOL_FILES = ["config/symbol_addrs.txt", "config/symbol_addrs_sdk.txt", "config/symbol_addrs_obin.txt",
                "config/symbol_addrs_main.txt", "config/symbol_addrs_hypotheses.txt",
                "config/symbol_addrs_main_hypotheses.txt", "config/symbol_addrs_types.txt"]
# Struct views always emitted (with their nested types); the rest only if a global uses them.
ROOT_STRUCTS = ["GameState", "SystemData"]

PRIMITIVES = {"s8": (1, True), "u8": (1, False), "s16": (2, True), "u16": (2, False),
              "s32": (4, True), "u32": (4, False), "char": (1, True), "short": (2, True), "int": (4, True),
              "long": (4, True), "u_char": (1, False), "u_short": (2, False), "u_int": (4, False),
              "u_long": (4, False)}


class ParseError(Exception):
    pass


_BINARY = {ast.Add: operator.add, ast.Sub: operator.sub, ast.Mult: operator.mul, ast.FloorDiv: operator.floordiv,
           ast.LShift: operator.lshift, ast.RShift: operator.rshift, ast.BitOr: operator.or_,
           ast.BitAnd: operator.and_, ast.BitXor: operator.xor}


def eval_int(text, where="constant"):
    """Integer constant expression (literals, + - * / << >> | & ^, unary - and ~, parentheses); nothing else."""
    try:
        tree = ast.parse(text.replace("/", "//"), mode="eval")
    except SyntaxError:
        raise ParseError("%s: cannot evaluate constant %r" % (where, text))

    def walk(node):
        if isinstance(node, ast.Expression):
            return walk(node.body)
        if isinstance(node, ast.Constant) and type(node.value) is int:
            return node.value
        if isinstance(node, ast.BinOp) and type(node.op) in _BINARY:
            right = walk(node.right)
            if isinstance(node.op, ast.FloorDiv) and right == 0:
                raise ParseError("%s: division by zero in %r" % (where, text))
            return _BINARY[type(node.op)](walk(node.left), right)
        if isinstance(node, ast.UnaryOp) and isinstance(node.op, (ast.USub, ast.Invert, ast.UAdd)):
            value = walk(node.operand)
            return -value if isinstance(node.op, ast.USub) else ~value if isinstance(node.op, ast.Invert) else value
        raise ParseError("%s: cannot evaluate constant %r" % (where, text))

    return walk(tree)


# ---------------------------------------------------------------- tokenizing

OFFSET_COMMENT = re.compile(r"/\*\s*(0x[0-9A-Fa-f]+)\s*(?:,[^*]*)?\*/")
SIZE_COMMENT = re.compile(r"/\*\s*size\s+(0x[0-9A-Fa-f]+)\s*\*/")
TOKEN = re.compile(r"\"(?:[^\"\\]|\\.)*\"|'(?:[^'\\]|\\.)*'|@OFF:0x[0-9A-Fa-f]+|@SIZE:0x[0-9A-Fa-f]+|0[xX][0-9A-Fa-f]+|\d+|[A-Za-z_]\w*|\.\.\.|[{}\[\];*:,()=<>+\-/|&~^%!?.]")


def tokenize(text):
    text = re.sub(r"\\\n", " ", text)
    text = SIZE_COMMENT.sub(lambda m: " @SIZE:" + m.group(1) + " ", text)
    text = OFFSET_COMMENT.sub(lambda m: " @OFF:" + m.group(1) + " ", text)
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    text = re.sub(r"//[^\n]*", " ", text)
    text = "\n".join("" if line.lstrip().startswith("#") else line for line in text.split("\n"))
    out = []
    pos = 0
    for m in TOKEN.finditer(text):
        gap = text[pos:m.start()]
        if gap.strip():
            raise ParseError("unexpected text %r" % gap.strip()[:40])
        out.append(m.group(0))
        pos = m.end()
    return out


# ---------------------------------------------------------------- model

class Prim:
    def __init__(self, width, signed, pointer=False):
        self.width, self.signed, self.pointer = width, signed, pointer


class StructRef:
    def __init__(self, name):
        self.name = name


class Member:
    def __init__(self, name, type_, dims, offset_comment, bits=None):
        self.name = name
        self.type = type_          # Prim | StructRef
        self.dims = dims           # list of ints (empty for scalars)
        self.offset_comment = offset_comment
        self.bits = bits           # bit-field width or None
        self.offset = None         # filled by layout
        self.bit_shift = None
        self.element_size = None
        self.count = None


class Struct:
    def __init__(self, name, is_union, members, size_comment, source):
        self.name = name
        self.is_union = is_union
        self.members = members
        self.size_comment = size_comment
        self.source = source
        self.size = None
        self.align = None


class Global:
    def __init__(self, name, type_, dims, source):
        self.name, self.type, self.dims, self.source = name, type_, dims, source
        self.address = None


class Parser:
    def __init__(self):
        self.structs = {}      # name -> Struct
        self.typedefs = {}     # name -> Prim | StructRef
        self.globals = {}      # name -> Global
        self._anon = 0

    # -- token helpers
    def parse_file(self, path, rel):
        self.toks = tokenize(path.read_text(encoding="utf-8", errors="replace"))
        self.i = 0
        self.rel = rel
        while self.i < len(self.toks):
            self.top_level()

    def peek(self, k=0):
        return self.toks[self.i + k] if self.i + k < len(self.toks) else None

    def take(self):
        t = self.toks[self.i]
        self.i += 1
        return t

    def expect(self, t):
        if self.peek() != t:
            raise ParseError("%s: expected %r, found %r near token %d" % (self.rel, t, self.peek(), self.i))
        self.i += 1

    def skip_statement(self):
        depth = 0
        while self.i < len(self.toks):
            t = self.take()
            if t in "{([":
                depth += 1
            elif t in "})]":
                depth -= 1
            elif t == ";" and depth <= 0:
                return

    # -- top level
    def top_level(self):
        t = self.peek()
        if t.startswith("@"):
            self.i += 1
            return
        if t == "typedef":
            self.i += 1
            return self.typedef()
        if t == "extern":
            self.i += 1
            return self.extern()
        if t in ("struct", "union") and self.peek(2) == "{" or (t in ("struct", "union") and self.peek(1) == "{"):
            self.aggregate(None)
            self.skip_statement() if self.peek() != ";" else self.take()
            return
        self.skip_statement()

    def typedef(self):
        t = self.peek()
        if t in ("struct", "union"):
            # typedef struct [Tag] { ... } Name; or typedef struct Tag Name;
            if self.peek(1) == "{" or (self.peek(2) == "{"):
                agg = self.aggregate(None)
                names = []
                while self.peek() not in (";", None):
                    if self.peek() in ("*", ","):
                        self.take()
                        continue
                    names.append(self.take())
                self.expect(";")
                if self.peek() and self.peek().startswith("@SIZE:"):
                    agg.size_comment = int(self.take()[6:], 16)
                if names:
                    if agg.name is None or agg.name.startswith("<anon"):
                        agg.name = names[0]
                        self.structs[agg.name] = agg
                    else:
                        self.typedefs[names[0]] = StructRef(agg.name)
                        if names[0] != agg.name:
                            self.typedefs[names[0]] = StructRef(agg.name)
                    for extra in names[1:]:
                        self.typedefs[extra] = Prim(4, False, True)
                    if agg.name not in self.structs:
                        self.structs[agg.name] = agg
                return
            kw = self.take()
            tag = self.take()
            names = []
            while self.peek() != ";":
                names.append(self.take())
            self.expect(";")
            real = [n for n in names if n not in "*,"]
            if real and "(" not in names:
                self.typedefs[real[0]] = StructRef(tag)
            return
        if t == "enum":
            self.skip_statement()
            return
        # typedef <prim> Name;  or function-pointer typedefs
        start = self.i
        stmt = []
        depth = 0
        while True:
            x = self.take()
            if x == ";" and depth == 0:
                break
            stmt.append(x)
            if x in "([{":
                depth += 1
            elif x in ")]}":
                depth -= 1
        if "(" in stmt:
            name = None
            for k, x in enumerate(stmt):
                if x == "(" and k + 2 < len(stmt) and stmt[k + 1] == "*":
                    name = stmt[k + 2]
                    break
            if name:
                self.typedefs[name] = Prim(4, False, True)
            return
        if "[" in stmt:
            return
        name = stmt[-1]
        base = [x for x in stmt[:-1] if x not in ("const", "volatile", "unsigned", "signed")]
        ptr = "*" in stmt
        if ptr:
            self.typedefs[name] = Prim(4, False, True)
        elif len(base) == 1 and base[0] in PRIMITIVES:
            w, s = PRIMITIVES[base[0]]
            self.typedefs[name] = Prim(w, s)
        elif len(base) == 1 and base[0] in self.typedefs:
            self.typedefs[name] = self.typedefs[base[0]]
        elif len(base) == 1 and base[0] in self.structs:
            self.typedefs[name] = StructRef(base[0])

    def aggregate(self, name_hint):
        kind = self.take()
        tag = None
        if self.peek() != "{":
            tag = self.take()
        self.expect("{")
        members = []
        while self.peek() != "}":
            members.extend(self.member())
        self.expect("}")
        size_comment = None
        if self.peek() and self.peek().startswith("@SIZE:"):
            size_comment = int(self.take()[6:], 16)
        elif self.peek() == ";" and self.peek(1) and self.peek(1).startswith("@SIZE:"):
            pass
        if tag is None:
            self._anon += 1
            tag = "<anon%d>" % self._anon
        agg = Struct(tag, kind == "union", members, size_comment, self.rel)
        if not tag.startswith("<anon"):
            self.structs[tag] = agg
        return agg

    def type_specifier(self):
        """Returns a Prim or StructRef (anonymous nested aggregates are registered)."""
        quals = ("const", "volatile", "unsigned", "signed", "register", "static")
        words = []
        while self.peek() in quals:
            words.append(self.take())
        t = self.peek()
        if t in ("struct", "union"):
            if self.peek(1) == "{" or self.peek(2) == "{":
                agg = self.aggregate(None)
                if agg.name.startswith("<anon"):
                    self.structs[agg.name] = agg
                return StructRef(agg.name)
            self.take()
            return StructRef(self.take())
        if t == "enum":
            self.take()
            self.take()
            return Prim(4, True)
        base = self.take()
        if base in PRIMITIVES:
            w, s = PRIMITIVES[base]
            if "unsigned" in words:
                s = False
            return Prim(w, s)
        if base in ("unsigned", "signed"):
            return Prim(4, base == "signed")
        return StructRef("typedef:" + base)

    def declarators(self, type_, offset_comment):
        out = []
        while True:
            ptr = False
            while self.peek() == "*":
                self.take()
                ptr = True
            while self.peek() in ("const", "volatile"):
                self.take()
            if self.peek() == "(":
                # function pointer member: ( * name ) ( ... )
                self.take()
                self.expect("*")
                name = self.take()
                self.expect(")")
                self.skip_group()
                out.append(Member(name, Prim(4, False, True), [], offset_comment))
            else:
                name = self.take()
                dims = []
                while self.peek() == "[":
                    self.take()
                    if self.peek() == "]":
                        dims.append(0)
                    else:
                        dims.append(self.const_expr())
                    self.expect("]")
                bits = None
                if self.peek() == ":":
                    self.take()
                    bits = self.const_expr()
                out.append(Member(name, Prim(4, False, True) if ptr else type_, dims, offset_comment, bits))
            if self.peek() == ",":
                self.take()
                offset_comment = None
                continue
            break
        return out

    def skip_group(self):
        self.expect("(")
        depth = 1
        while depth:
            t = self.take()
            depth += (t == "(") - (t == ")")

    def const_expr(self):
        toks = []
        while self.peek() not in ("]", ";", ",", ")", None):
            toks.append(self.take())
        text = " ".join(toks)
        if not text.strip():
            raise ParseError("%s: empty constant expression" % self.rel)
        return eval_int(text, self.rel)

    def member(self):
        offset = None
        while self.peek() and self.peek().startswith("@OFF:"):
            offset = int(self.take()[5:], 16)
        type_ = self.type_specifier()
        members = self.declarators(type_, offset)
        self.expect(";")
        return members

    def extern(self):
        stmt = []
        depth = 0
        while True:
            x = self.take()
            if x == ";" and depth == 0:
                break
            stmt.append(x)
            if x in "([{":
                depth += 1
            elif x in ")]}":
                depth -= 1
        if "(" in stmt:
            return          # function or function pointer
        saved = self.toks, self.i
        self.toks, self.i = stmt + [";"], 0
        try:
            type_ = self.type_specifier()
            for m in self.declarators(type_, None):
                self.globals[m.name] = Global(m.name, m.type, m.dims, self.rel)
        finally:
            self.toks, self.i = saved


# ---------------------------------------------------------------- layout

def resolve_type(parser, type_):
    seen = 0
    while isinstance(type_, StructRef) and type_.name.startswith("typedef:"):
        name = type_.name[8:]
        if name not in parser.typedefs:
            raise ParseError("unknown type %s" % name)
        type_ = parser.typedefs[name]
        seen += 1
        if seen > 16:
            raise ParseError("typedef loop " + name)
    if isinstance(type_, StructRef) and type_.name not in parser.structs:
        raise ParseError("unknown struct %s" % type_.name)
    return type_


def lay_out(parser, struct, busy=()):
    if struct.size is not None:
        return
    if struct.name in busy:
        raise ParseError("recursive struct " + struct.name)
    busy = busy + (struct.name,)
    offset = 0
    align = 1
    size = 0
    unit_off = None        # bit-field storage unit start, width and bits used
    unit_width = 0
    unit_used = 0
    for m in struct.members:
        m.type = resolve_type(parser, m.type)
        if isinstance(m.type, StructRef):
            sub = parser.structs[m.type.name]
            lay_out(parser, sub, busy)
            width, malign = sub.size, sub.align
        else:
            width, malign = m.type.width, m.type.width
        m.element_size = width
        count = 1
        for d in m.dims:
            count *= d
        m.count = count
        if m.bits is not None:
            if struct.is_union:
                raise ParseError("bit-field in union %s" % struct.name)
            if unit_off is None or unit_width != width or unit_used + m.bits > width * 8:
                offset = (offset + width - 1) // width * width
                unit_off, unit_width, unit_used = offset, width, 0
                offset += width
            m.offset = unit_off
            m.bit_shift = unit_used
            unit_used += m.bits
            align = max(align, malign)
            continue
        unit_off = None
        total = width * count
        if struct.is_union:
            m.offset = 0
            size = max(size, total)
        else:
            offset = (offset + malign - 1) // malign * malign
            m.offset = offset
            offset += total
        align = max(align, malign)
    if not struct.is_union:
        size = offset
    struct.align = align
    struct.size = (size + align - 1) // align * align


def layout_errors(parser):
    """Offset and size comments that disagree with the computed layout."""
    errors = []
    for name in sorted(parser.structs):
        s = parser.structs[name]
        lay_out(parser, s)
        for m in s.members:
            # A bit-field comment names its unit or the byte that holds its first bit.
            allowed = {m.offset} if m.bits is None else {m.offset, m.offset + m.bit_shift // 8}
            if m.offset_comment is not None and m.offset_comment not in allowed:
                errors.append("%s.%s (%s): comment 0x%X, computed 0x%X" % (name, m.name, s.source, m.offset_comment, m.offset))
        if s.size_comment is not None and s.size_comment != s.size:
            errors.append("%s (%s): size comment 0x%X, computed 0x%X" % (name, s.source, s.size_comment, s.size))
    return errors


def load_model(repo=REPO):
    parser = Parser()
    files = []
    for pattern in HEADER_GLOBS:
        files.extend(sorted(repo.glob(pattern)))
    # Typedef sources first so member types resolve whatever the include order.
    ordered = sorted(files, key=lambda p: (p.name not in ("common.h", "psx_types.h"), str(p)))
    for path in ordered:
        parser.parse_file(path, str(path.relative_to(repo)))
    addresses = {}
    for rel in SYMBOL_FILES:
        path = repo / rel
        if not path.exists():
            continue
        for line in path.read_text().splitlines():
            m = re.match(r"\s*([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;(.*)", line)
            if m and "type:func" not in m.group(3):
                addresses.setdefault(m.group(1), int(m.group(2), 16))
    for g in parser.globals.values():
        if g.name in addresses:
            g.address = addresses[g.name]
        else:
            m = re.fullmatch(r"D_([0-9A-Fa-f]{8})", g.name)
            if m:
                g.address = int(m.group(1), 16)
    for s in parser.structs.values():
        lay_out(parser, s)
    return parser


# ---------------------------------------------------------------- emission

def snake_upper(name):
    s = re.sub(r"(?<=[a-z])(?=[A-Z])|(?<=[A-Z])(?=[A-Z][a-z])", "_", name)
    return s.upper()


def used_structs(parser, roots):
    out = set()
    stack = list(roots)
    while stack:
        n = stack.pop()
        if n in out:
            continue
        out.add(n)
        for m in parser.structs[n].members:
            if isinstance(m.type, StructRef):
                stack.append(m.type.name)
    return out


def global_extent(g):
    count = 1
    for d in g.dims:
        count *= d
    return count


def emit(parser):
    roots = set(ROOT_STRUCTS)
    globals_ = sorted((g for g in parser.globals.values() if g.address is not None), key=lambda g: (g.address, g.name))
    for g in globals_:
        g.type = resolve_type(parser, g.type)
        if isinstance(g.type, StructRef):
            roots.add(g.type.name)
    names = used_structs(parser, roots)
    lines = [
        "extends RefCounted",
        "## GENERATED by tools/port/gen_game_memory.py (T-39300). Do not edit; regenerate and commit.",
        "## Layout of include/game/*.h and include/main/*.h with the addresses of config/symbol_addrs*.txt.",
        "## Constants: SIZEOF_<STRUCT> and <STRUCT>_<FIELD> (byte offset in the struct). Field rows: [offset, width, signed, count, struct, stride, bit_shift, bit_width]",
        "##   width: bytes of one element (a nested struct: its size); count: elements (flattened arrays);",
        "##   struct: nested struct name or \"\"; stride: bytes between elements; bit_width 0: not a bit-field.",
        "",
    ]
    consts = []
    structs = []
    for name in sorted(names):
        s = parser.structs[name]
        pre = snake_upper(name)
        consts.append("const SIZEOF_%s: int = 0x%X" % (pre, s.size))
        rows = []
        for m in s.members:
            sub = m.type.name if isinstance(m.type, StructRef) else ""
            signed = bool(getattr(m.type, "signed", False))
            row = "\t\t\"%s\": [0x%X, %d, %s, %d, \"%s\", %d, %d, %d]" % (
                m.name, m.offset, m.element_size, "true" if signed else "false", m.count, sub,
                m.element_size, m.bit_shift or 0, m.bits or 0)
            rows.append(row)
            consts.append("const %s_%s: int = 0x%X" % (pre, snake_upper(m.name), m.offset))
        structs.append("\t\"%s\": {\n\t\t\"size\": 0x%X,\n\t\t\"union\": %s,\n\t\t\"fields\": {\n%s\n\t\t},\n\t}" % (
            name, s.size, "true" if s.is_union else "false",
            ",\n".join("\t" + r for r in rows)))
    seen = {}
    for c in consts:
        key = c.split(":")[0]
        if key in seen:
            raise ParseError("duplicate generated constant %s" % key)
        seen[key] = 1
    lines.append("const GAME_STATE_BASE: int = 0x800E6280")
    lines.append("const SYSTEM_DATA_BASE: int = 0x800E7D10")
    lines.append("")
    lines.extend(consts)
    lines.append("")
    lines.append("const STRUCTS: Dictionary = {")
    lines.append(",\n".join(structs) + ",")
    lines.append("}")
    lines.append("")
    lines.append("## Gameplay globals: name -> [address, width, signed, count, struct, stride]")
    lines.append("## (count 0: declared without extent, `extern T name[]`).")
    grows = []
    for g in globals_:
        if isinstance(g.type, StructRef):
            width, signed, sub = parser.structs[g.type.name].size, False, g.type.name
        else:
            width, signed, sub = g.type.width, g.type.signed, ""
        count = 0 if (g.dims and 0 in g.dims) else global_extent(g)
        grows.append("\t\"%s\": [0x%08X, %d, %s, %d, \"%s\", %d]" % (g.name, g.address, width,
                     "true" if signed else "false", count, sub, width))
    lines.append("const GLOBALS: Dictionary = {")
    lines.append(",\n".join(grows) + ",")
    lines.append("}")
    lines.append("")
    return "\n".join(lines)


def generate(repo=REPO):
    parser = load_model(repo)
    errors = layout_errors(parser)
    if errors:
        raise ParseError("layout comments disagree with the computed layout:\n" + "\n".join(errors))
    return emit(parser)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--out", type=Path, default=DEFAULT_OUT)
    ap.add_argument("--check", action="store_true", help="fail if the output file differs")
    args = ap.parse_args(argv)
    text = generate()
    if args.check:
        if not args.out.exists() or args.out.read_text() != text:
            print("%s is stale; run tools/port/gen_game_memory.py" % args.out, file=sys.stderr)
            return 1
        return 0
    args.out.write_text(text)
    print("wrote %s (%d bytes)" % (args.out, len(text)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
