"""Type model and struct layout for the C -> GDScript generator (WP-B, T-39312).

Built from the pycparser AST of a preprocessed translation unit. Sizes follow the PS1 (o32): char 1, short 2,
int/long/pointer 4. Compiled K&R (-cckr), so u8/u16 promote to unsigned int (CODING_STANDARDS section 2).
"""
from pycparser import c_ast


class T:
    """kind: int | ptr | struct | union | array | void | func"""

    def __init__(self, kind, bits=0, signed=True, to=None, n=None, tag=None, node=None, fields=None):
        self.kind = kind
        self.bits = bits
        self.signed = signed
        self.to = to
        self.n = n
        self.tag = tag
        self.node = node
        self.fields = fields  # struct/union: list of (name, T, offset) after layout (anonymous members flattened)
        self.size_ = None
        self.bitfields = {}
        self.align_ = None

    def __repr__(self):
        if self.kind == 'int':
            return ('s' if self.signed else 'u') + str(self.bits)
        if self.kind == 'ptr':
            return repr(self.to) + '*'
        if self.kind == 'array':
            return '%r[%s]' % (self.to, self.n)
        return '%s %s' % (self.kind, self.tag)


S8, U8, S16, U16, S32, U32 = (T('int', 8, True), T('int', 8, False), T('int', 16, True), T('int', 16, False),
                              T('int', 32, True), T('int', 32, False))
VOID = T('void')


def int_range(t):
    bits = t.bits if t.kind == 'int' else 32
    signed = t.signed if t.kind == 'int' else False
    if signed:
        return (-(1 << (bits - 1)), (1 << (bits - 1)) - 1)
    return (0, (1 << bits) - 1)


def sig_name(t):
    """C_SIGNATURES type name."""
    if t.kind == 'void':
        return 'void'
    if t.kind == 'int':
        return ('s' if t.signed else 'u') + str(t.bits)
    return 'ptr'


class Types:
    def __init__(self, ast):
        self.typedefs = {}
        self.structs = {}  # tag -> T (complete when seen)
        self.enums = {}
        self.globals = {}  # name -> T
        self.funcs = {}  # name -> (ret T, [param T], known_params bool)
        self.funcdefs = {}
        for ext in ast.ext:
            if isinstance(ext, c_ast.Typedef):
                self.typedefs[ext.name] = ext.type
            # register structs/enums found anywhere in the declarations lazily through build()
        for ext in ast.ext:
            if isinstance(ext, c_ast.Typedef):
                self.build(ext.type)  # registers struct tags
            elif isinstance(ext, c_ast.Decl):
                self._register_decl(ext)
            elif isinstance(ext, c_ast.FuncDef):
                self.funcdefs[ext.decl.name] = ext
                self._register_decl(ext.decl)

    def _register_decl(self, d):
        if d.name is None:
            self.build(d.type)
            return
        if isinstance(d.type, c_ast.FuncDecl):
            ret, params, known = self.func_sig(d.type)
            self.funcs[d.name] = (ret, params, known)
        elif 'typedef' not in (d.storage or []):
            self.globals[d.name] = self.build(d.type)

    def func_sig(self, fd):
        ret = self.build(fd.type)
        params = []
        known = False
        if fd.args is not None:
            for p in fd.args.params:
                if isinstance(p, (c_ast.EllipsisParam, c_ast.ID)):
                    continue
                pt = self.build(p.type)
                if pt.kind == 'void' and not p.name:
                    continue
                if pt.kind == 'array':
                    pt = T('ptr', to=pt.to)
                params.append(pt)
                known = True
        return ret, params, known

    # ---- type construction
    def build(self, node):
        if isinstance(node, c_ast.TypeDecl):
            return self.build(node.type)
        if isinstance(node, c_ast.IdentifierType):
            return self.ident(node.names)
        if isinstance(node, c_ast.PtrDecl):
            return T('ptr', to=self.build(node.type))
        if isinstance(node, c_ast.ArrayDecl):
            n = None
            if node.dim is not None:
                try:
                    n = self.const(node.dim)
                except Exception:
                    n = None
            return T('array', to=self.build(node.type), n=n)
        if isinstance(node, c_ast.FuncDecl):
            ret, params, known = self.func_sig(node)
            t = T('func', to=ret)
            t.fields = params
            return t
        if isinstance(node, (c_ast.Struct, c_ast.Union)):
            return self.struct(node)
        if isinstance(node, c_ast.Enum):
            self.enum(node)
            return T('int', 32, True)
        if isinstance(node, c_ast.Typename):
            return self.build(node.type)
        raise ValueError('type node %r' % node)

    def ident(self, names):
        names = [n for n in names if n not in ('const', 'volatile', 'register', 'static', 'extern')]
        if len(names) == 1 and names[0] in self.typedefs:
            return self.build(self.typedefs[names[0]])
        words = set(names)
        if 'void' in words:
            return VOID
        unsigned = 'unsigned' in words
        if 'char' in words:
            return U8 if unsigned else S8
        if 'short' in words:
            return U16 if unsigned else S16
        if 'long' in words or 'int' in words or 'signed' in words or unsigned:
            return U32 if unsigned else S32
        if len(names) == 1 and names[0] in self.typedefs:
            return self.build(self.typedefs[names[0]])
        raise ValueError('unknown type %s' % names)

    def struct(self, node):
        kind = 'struct' if isinstance(node, c_ast.Struct) else 'union'
        tag = node.name
        if node.decls is None:
            if tag in self.structs:
                return self.structs[tag]
            t = T(kind, tag=tag)
            self.structs[tag] = t
            return t
        t = self.structs.get(tag) if tag else None
        if t is None:
            t = T(kind, tag=tag)
            if tag:
                self.structs[tag] = t
        if t.node is node:
            return t
        t.node = node
        t.fields = None  # computed lazily by layout
        t.size_ = None
        return t

    def enum(self, node):
        if node.values is None:
            return
        v = 0
        for e in node.values.enumerators:
            if e.value is not None:
                v = self.const(e.value)
            self.enums[e.name] = v
            v += 1

    # ---- constant evaluation (integer constant expressions)
    def const(self, n):
        if isinstance(n, c_ast.Constant):
            return parse_int(n.value)
        if isinstance(n, c_ast.ID):
            if n.name in self.enums:
                return self.enums[n.name]
            raise ValueError('not constant: ' + n.name)
        if isinstance(n, c_ast.UnaryOp):
            v = self.const(n.expr) if n.op != 'sizeof' else None
            if n.op == '-':
                return -v
            if n.op == '+':
                return v
            if n.op == '~':
                return ~v
            if n.op == '!':
                return int(not v)
            if n.op == 'sizeof':
                if isinstance(n.expr, c_ast.Typename):
                    return self.size(self.build(n.expr.type))
                raise ValueError('sizeof expr')
        if isinstance(n, c_ast.BinaryOp):
            a, b = self.const(n.left), self.const(n.right)
            op = n.op
            if op == '/':
                return int(a / b)
            if op == '%':
                return a - int(a / b) * b
            return {'+': lambda: a + b, '-': lambda: a - b, '*': lambda: a * b, '<<': lambda: a << b,
                    '>>': lambda: a >> b, '&': lambda: a & b, '|': lambda: a | b, '^': lambda: a ^ b,
                    '==': lambda: int(a == b), '!=': lambda: int(a != b), '<': lambda: int(a < b),
                    '>': lambda: int(a > b), '<=': lambda: int(a <= b), '>=': lambda: int(a >= b),
                    '&&': lambda: int(bool(a and b)), '||': lambda: int(bool(a or b))}[op]()
        if isinstance(n, c_ast.Cast):
            t = self.build(n.to_type)
            v = self.const(n.expr)
            if t.kind == 'int':
                return wrap(v, t)
            return v
        if isinstance(n, c_ast.TernaryOp):
            return self.const(n.iftrue) if self.const(n.cond) else self.const(n.iffalse)
        raise ValueError('not constant: %r' % n)

    # ---- layout
    def size(self, t):
        self.layout(t)
        return t.size_

    def align(self, t):
        self.layout(t)
        return t.align_

    def layout(self, t):
        if t.size_ is not None:
            return
        if t.kind == 'int':
            t.size_ = t.align_ = t.bits // 8
        elif t.kind == 'ptr':
            t.size_ = t.align_ = 4
        elif t.kind == 'void':
            t.size_ = t.align_ = 1
        elif t.kind == 'array':
            self.layout(t.to)
            t.align_ = t.to.align_
            t.size_ = t.to.size_ * (t.n if t.n is not None else 0)
        elif t.kind in ('struct', 'union'):
            if t.node is None or t.node.decls is None:
                raise ValueError('incomplete %s %s' % (t.kind, t.tag))
            fields = []
            t.bitfields = {}
            off = 0
            maxalign = 1
            maxsize = 0
            unit = None  # [unit offset, unit size, bits used]
            for d in t.node.decls:
                ft = self.build(d.type)
                self.layout(ft)
                a = ft.align_
                maxalign = max(maxalign, a)
                if d.bitsize is not None:
                    w = self.const(d.bitsize)
                    if t.kind != 'struct':
                        raise ValueError('bit-field in union %s' % t.tag)
                    if unit is None or unit[1] != ft.size_ or unit[2] + w > ft.size_ * 8:
                        uoff = (off + a - 1) // a * a
                        unit = [uoff, ft.size_, 0]
                        off = uoff + ft.size_
                    if d.name:
                        fields.append((d.name, ft, unit[0]))
                        t.bitfields[d.name] = (unit[2], w)
                    unit[2] += w
                    continue
                unit = None
                if t.kind == 'struct':
                    off = (off + a - 1) // a * a
                    foff = off
                    off += ft.size_
                else:
                    foff = 0
                    maxsize = max(maxsize, ft.size_)
                if d.name is None:  # anonymous member: flatten
                    for (n2, t2, o2) in ft.fields:
                        fields.append((n2, t2, foff + o2))
                        if n2 in getattr(ft, 'bitfields', {}):
                            t.bitfields[n2] = ft.bitfields[n2]
                else:
                    fields.append((d.name, ft, foff))
            size = off if t.kind == 'struct' else maxsize
            size = (size + maxalign - 1) // maxalign * maxalign
            t.fields = fields
            t.size_ = size
            t.align_ = maxalign
        elif t.kind == 'func':
            t.size_ = t.align_ = 4

    def bitfield(self, t, name):
        self.layout(t)
        return t.bitfields.get(name)

    def field(self, t, name):
        self.layout(t)
        for (n, ft, off) in t.fields:
            if n == name:
                return ft, off
        raise KeyError('%s has no field %s' % (t.tag, name))


def parse_int(text):
    t = text
    if t.startswith("'"):
        body = t[1:-1]
        if body.startswith('\\'):
            esc = body[1:]
            simple = {'n': 10, 't': 9, 'r': 13, '\\': 92, "'": 39, '"': 34, 'a': 7, 'b': 8, 'f': 12, 'v': 11}
            if esc[0] == 'x':
                return int(esc[1:], 16)
            if esc[0] in '01234567':
                return int(esc, 8)
            return simple[esc[0]]
        return ord(body)
    u = False
    while t and t[-1] in 'uUlL':
        if t[-1] in 'uU':
            u = True
        t = t[:-1]
    return int(t, 0) if not (t.startswith('0') and len(t) > 1 and t[1].isdigit()) else int(t, 8)


def literal_type(text):
    """The C type of an integer literal in the PS1 compilers: int unless it needs unsigned."""
    t = text
    unsigned = False
    while t and t[-1] in 'uUlL':
        if t[-1] in 'uU':
            unsigned = True
        t = t[:-1]
    v = parse_int(text)
    if unsigned or v > 0x7FFFFFFF:
        return U32
    return S32


def wrap(v, t):
    bits = t.bits if t.kind == 'int' else 32
    signed = t.signed if t.kind == 'int' else False
    v &= (1 << bits) - 1
    if signed and v >= 1 << (bits - 1):
        v -= 1 << bits
    return v
