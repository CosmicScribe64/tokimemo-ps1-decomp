"""C -> GDScript generator for the engine modules (WP-B, T-39312).

usage: python3 -I cgen.py --repo REPO --unit hud_panels --out OUT.gd [--only f1,f2]
Preprocesses src/main/<unit>.c with gcc -E, parses it with pycparser, and emits one GDScript method per C
function following wiki/engine-porting-contract.md: memory through engine.memory, calls through engine.call_c,
C integer semantics (K&R promotion, 32-bit wrap) by interval analysis, `halted()` in loops that call out.
Anything it cannot translate faithfully (goto, stack locals, string literals, fall-through, struct copies)
is reported and the function is left out for a hand port.
"""
import argparse
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pycparser import c_ast, c_parser  # noqa: E402
import uninit  # noqa: E402
from ctypes_ import (T, S8, U8, S16, U16, S32, U32, VOID, Types, int_range, sig_name, parse_int, literal_type,  # noqa: E402
                     wrap)

KEYWORDS = {'var', 'func', 'class', 'match', 'in', 'is', 'as', 'signal', 'static', 'const', 'enum', 'self', 'pass',
            'not', 'and', 'or', 'if', 'elif', 'else', 'for', 'while', 'return', 'break', 'continue', 'true', 'false',
            'null', 'await', 'extends', 'super', 'void', 'int', 'float', 'bool', 'min', 'max', 'abs', 'range', 'len',
            'str', 'print', 'sign', 'clamp', 'floor', 'ceil', 'round', 'sqrt', 'pow', 'name', 'size', 'type', 'engine',
            'preload', 'load', 'assert', 'breakpoint', 'sin', 'cos', 'tan', 'rand', 'hash', 'char', 'wait', 'seed'}

READ = {(8, False): 'u8', (8, True): 's8', (16, False): 'u16', (16, True): 's16', (32, True): 's32', (32, False): 'u32'}
INF = 1 << 62

# Locals that live on the stack: function -> {variable: sp offset in the original frame}. Evidence per entry:
# asm/matchings/main/<file>/<function>.s (the `addiu $reg, $sp, N` that passes the array to its callee, or the
# frame the array occupies when it is only used locally; a local-only array may sit anywhere in the frame).
HERE = os.path.dirname(os.path.abspath(__file__))
CFG_PATH = os.path.join(HERE, '..', '..', '..', 'config', 'port', 'engine-gen', 'engine_gen.json')
CFG = json.load(open(CFG_PATH))
# Calls made through an alias symbol that has the same address as a function of this package.
ALIASES = CFG['aliases']
# Locals that live on the stack: function -> {variable: sp offset in the original frame}. Evidence per entry is in
# CFG['stack_locals_evidence'] (asm/matchings/main/<file>/<function>.s).
STACK_LOCALS = CFG['stack_locals']
# Local-only arrays that IDO kept in registers: function -> {variable: [dimensions]}; they become GDScript arrays.
ARRAY_LOCALS = CFG['array_locals']
# Locals that uninit.py flags although every read follows an assignment on all paths.
UNINIT_FALSE_POSITIVES = {tuple(x) for x in CFG['uninit_false_positives']}
# Functions whose callers pass arguments the C definition does not take: the port takes them too (and says so).
SIGNATURE_OVERRIDES = CFG['signature_overrides']


def decode_c_string(lit):
    body = lit[1:-1] if lit[:1] == '"' else lit
    out = []
    i = 0
    simple = {'n': '\n', 't': '\t', 'r': '\r', '\\': '\\', '"': '"', "'": "'", 'a': '\a', 'b': '\b', 'f': '\f', 'v': '\v'}
    while i < len(body):
        c = body[i]
        if c != '\\':
            out.append(c)
            i += 1
            continue
        i += 1
        c = body[i]
        if c in simple:
            out.append(simple[c])
            i += 1
        elif c == 'x':
            j = i + 1
            while j < len(body) and body[j] in '0123456789abcdefABCDEF':
                j += 1
            out.append(chr(int(body[i + 1:j], 16)))
            i = j
        elif c in '01234567':
            j = i
            while j < len(body) and j < i + 3 and body[j] in '01234567':
                j += 1
            out.append(chr(int(body[i:j], 8)))
            i = j
        else:
            out.append(c)
            i += 1
    return ''.join(out)


class Unsupported(Exception):
    pass


class Ex:
    def __init__(self, code, ty, lo=None, hi=None, pre=None, const=None, cond=None, raw=None):
        self.raw = raw  # (code, lo, hi) before a 32-bit wrap (a wrap composes with any narrower or equal wrap)
        self.code = code
        self.ty = ty
        if lo is None:
            lo, hi = int_range(ty) if ty.kind in ('int', 'ptr') else (-INF, INF)
        self.lo, self.hi = lo, hi
        self.pre = pre or []
        self.const = const
        self.cond = cond


class Addr:
    """An address: dynamic code (or ''), named constant terms (struct sizes and field offsets) and a numeric offset."""

    def __init__(self, dyn='', off=0, syms=None):
        self.dyn, self.off = dyn, off
        self.syms = list(syms or [])

    def plus(self, k):
        return Addr(self.dyn, self.off + k, self.syms)

    def plus_sym(self, name, k=1):
        return Addr(self.dyn, self.off, self.syms + [name if k == 1 else '%d * %s' % (k, name)])

    def code(self):
        parts = ([self.dyn] if self.dyn else []) + self.syms
        if not parts:
            return '0x%X' % (self.off & 0xFFFFFFFF)
        text = ' + '.join(lowp(x) for x in parts) if len(parts) > 1 else parts[0]
        if self.off:
            text += (' + 0x%X' % self.off) if self.off > 0 else (' - 0x%X' % -self.off)
        return text


class ArrLocal:
    """A local array kept as a GDScript Array[int] (flattened row-major)."""

    def __init__(self, name, dims, elem):
        self.name, self.dims, self.elem = name, dims, elem


class LV:
    """kind: local | gs | glob | mem"""

    def __init__(self, kind, ty, name=None, addr=None, pre=None, bits=None):
        self.kind, self.ty, self.name, self.addr, self.pre = kind, ty, name, addr, pre or []
        self.bits = bits  # (shift, width) of a bit-field stored in the unit of type ty at addr


LOW_OPS = (' & ', ' | ', ' ^ ', ' << ', ' >> ', ' and ', ' or ', ' if ', ' == ', ' != ', ' < ', ' > ', ' <= ', ' >= ')


def toplevel_has(code, tokens):
    depth = 0
    i = 0
    n = len(code)
    while i < n:
        c = code[i]
        if c in '([':
            depth += 1
        elif c in ')]':
            depth -= 1
        elif c == '"':
            i = code.index('"', i + 1)
        elif depth == 0:
            for t in tokens:
                if code.startswith(t, i):
                    return True
        i += 1
    return False


def lowp(code, right_of_minus=False):
    """Parenthesise an operand of + or - when it holds a lower-precedence operator at its top level."""
    toks = LOW_OPS + ((' + ', ' - ') if right_of_minus else ())
    if toplevel_has(code, toks) or code.startswith('not '):
        return '(%s)' % code
    return code


def hexs(v):
    return ('-0x%X' % -v) if v < 0 else ('0x%X' % v if v > 9 else str(v))


class Layout:
    """GLOBALS and GameState rows of core/game_layout.gd."""

    def __init__(self, path):
        text = open(path).read()
        self.globals = {}
        g = text[text.index('const GLOBALS'):]
        for m in re.finditer(r'"(\w+)": \[(0x[0-9A-Fa-f]+), (\d+), (true|false), (\d+), "(\w*)", (\d+)\]', g):
            self.globals[m.group(1)] = (int(m.group(2), 16), int(m.group(3)), m.group(4) == 'true', int(m.group(5)),
                                        m.group(6), int(m.group(7)))
        self.gs = {}
        s = text[text.index('"GameState": {'):]
        s = s[:s.index('\n\t},')]
        for m in re.finditer(r'"(\w+)": \[(0x[0-9A-Fa-f]+), (\d+), (true|false), (\d+), "(\w*)", (\d+), (\d+), (\d+)\]', s):
            self.gs[m.group(1)] = (int(m.group(2), 16), int(m.group(3)), m.group(4) == 'true', int(m.group(5)),
                                   m.group(6), int(m.group(8)), int(m.group(9)))
        self.gs_base = int(re.search(r'GAME_STATE_BASE: int = (0x[0-9A-Fa-f]+)', text).group(1), 16)


def load_symbols(repo):
    syms = {}
    for f in sorted(os.listdir(os.path.join(repo, 'config'))):
        if f.startswith('symbol_addrs') and f.endswith('.txt'):
            for line in open(os.path.join(repo, 'config', f)):
                m = re.match(r'(\w+)\s*=\s*(0x[0-9A-Fa-f]+)', line)
                if m:
                    syms.setdefault(m.group(1), int(m.group(2), 16))
    return syms


class FuncGen:
    def __init__(self, unit, fname, fd):
        self.u = unit
        self.types = unit.types
        self.name = fname
        self.fd = fd
        self.lines = []
        self.ind = 1
        self.scopes = [{}]
        self.hoisted = []  # (gd name, comment)
        self.used_names = set()
        self.tmp = 0
        self.ctx = []  # break/continue contexts
        self.has_value_return = False
        self.has_void_return = False
        self.callees = set()
        self.uses_div = False
        self.notes = []
        self.call_notes = {}
        self.labels = {}
        self.return_values = []
        self.loop_returns_at_end = False
        self.guarded = set()  # C names of locals the C may read before assigning (refuse instead of inventing 0)
        self.guard_flag = {}  # GDScript local name -> its `_set` flag
        self.uses_uninit = False

    # ---- output
    def emit(self, line):
        self.lines.append('\t' * self.ind + line)

    def emit_pre(self, pre):
        for p in pre:
            self.emit(p)

    def fresh(self, base='_t'):
        self.tmp += 1
        return '%s%d' % (base, self.tmp)

    def gdname(self, cname):
        n = cname
        if n in KEYWORDS:
            n += '_'
        base, k = n, 1
        while n in self.used_names:
            k += 1
            n = '%s_%d' % (base, k)
        self.used_names.add(n)
        return n

    def lookup_local(self, name):
        for sc in reversed(self.scopes):
            if name in sc:
                return sc[name]
        return None

    # ---- type conversions
    def conv(self, ex, to):
        """Convert an expression to the C type `to` (assignment/cast/argument conversion)."""
        if to.kind == 'void':
            return ex
        if to.kind not in ('int', 'ptr'):
            raise Unsupported('conversion to %r' % to)
        if ex.ty.kind not in ('int', 'ptr'):
            raise Unsupported('conversion from %r' % ex.ty)
        tlo, thi = int_range(to)
        if ex.const is not None:
            v = wrap(ex.const, to if to.kind == 'int' else U32)
            return Ex(hexs(v), to, v, v, ex.pre, const=v)
        if ex.lo >= tlo and ex.hi <= thi:
            r = Ex(ex.code, to, ex.lo, ex.hi, ex.pre, cond=ex.cond)
            return r
        t = to if to.kind == 'int' else U32
        fn = READ[(t.bits, t.signed)]
        base = ex.raw if ex.raw else (ex.code, ex.lo, ex.hi)
        return Ex('Math.%s(%s)' % (fn, base[0]), to, tlo, thi, ex.pre, raw=base if t.bits == 32 else None)

    def value(self, ex):
        """An int-valued expression (comparisons carry cond and an int() code)."""
        return ex

    def cond_of(self, ex):
        if ex.cond is not None:
            return ex.cond
        if ex.const is not None:
            return 'true' if ex.const else 'false'
        if ex.lo > 0 or ex.hi < 0:
            return 'true'
        return '%s != 0' % self.paren(ex.code)

    @staticmethod
    def paren(code):
        if re.fullmatch(r'-?\w+(\.\w+)*(\[[^\[\]()]*\])?', code):
            return code
        if re.fullmatch(r'[\w\.]+(\([^()]*\))?|-?\w+|[\w\.]+\(.*\)', code) and code.count('(') == code.count(')') \
                and not re.search(r'[+\-*/&|^<>=% ]', re.sub(r'\([^()]*\)', '', code)):
            return code
        return '(%s)' % code

    # ---- promotion helpers
    def promote(self, ex):
        t = ex.ty
        if t.kind == 'ptr':
            return ex
        if t.kind != 'int':
            raise Unsupported('arithmetic on %r' % t)
        if t.bits < 32:
            return self.conv(ex, U32 if not t.signed else S32) if not t.signed else Ex(ex.code, S32, ex.lo, ex.hi, ex.pre, ex.const, ex.cond)
        return ex

    def common(self, a, b):
        a, b = self.promote(a), self.promote(b)
        uns = (a.ty.kind == 'ptr' or b.ty.kind == 'ptr' or (a.ty.kind == 'int' and not a.ty.signed and a.ty.bits == 32)
               or (b.ty.kind == 'int' and not b.ty.signed and b.ty.bits == 32))
        t = U32 if uns else S32
        return self.conv(a, t), self.conv(b, t), t

    def wrapped(self, code, t, lo, hi, pre=None, const=None):
        tlo, thi = int_range(t)
        if lo >= tlo and hi <= thi:
            return Ex(code, t, lo, hi, pre, const)
        fn = 's32' if t.signed else 'u32'
        return Ex('Math.%s(%s)' % (fn, code), t, tlo, thi, pre, raw=(code, lo, hi))

    def binop(self, op, a, b):
        """C binary operator on two Ex values."""
        if b.pre:
            a = self.spill(a)
        pre = a.pre + b.pre
        # pointer arithmetic
        if op in ('+', '-') and (a.ty.kind == 'ptr' or b.ty.kind == 'ptr'):
            if a.ty.kind == 'ptr' and b.ty.kind == 'ptr':
                size = self.types.size(a.ty.to)
                d = self.wrapped('%s - %s' % (a.code, b.code), S32, -INF, INF, pre)
                if size != 1:
                    d = Ex('%s / %d' % (self.paren(d.code), size), S32, pre=pre)
                return d
            if b.ty.kind == 'ptr':
                a, b = b, a
            size = self.types.size(a.ty.to if a.ty.to.kind != 'void' else U8)
            idx = self.rawview(self.promote(b))
            if idx.const is not None:
                k = idx.const * size * (1 if op == '+' else -1)
                if a.const is not None:
                    v = (a.const + k) & 0xFFFFFFFF
                    return Ex(hexs(v), a.ty, v, v, pre, const=v)
                return Ex(self.addcode(a.code, k), a.ty, 0, 0xFFFFFFFF, pre)
            term = idx.code if size == 1 else '%s * %d' % (self.paren(idx.code), size)
            code = '%s %s %s' % (lowp(a.code), op, lowp(term, op == '-'))
            return Ex('Math.u32(%s)' % code, a.ty, 0, 0xFFFFFFFF, pre)
        if op in ('<<', '>>'):
            a = self.promote(a)
            b = self.promote(b)
            t = a.ty
            if b.const is not None:
                n = b.const & 31
                ncode = str(n)
            else:
                n = None
                ncode = '(%s & 31)' % b.code
            if op == '>>':
                if n is not None:
                    lo, hi = a.lo >> n, a.hi >> n
                else:
                    lo, hi = min(a.lo, 0), max(a.hi, 0)
                if a.const is not None and n is not None:
                    v = a.const >> n
                    return Ex(hexs(v), t, v, v, pre, const=v)
                return Ex('%s >> %s' % (self.paren(a.code), ncode), t, lo, hi, pre)
            if a.const is not None and n is not None:
                v = wrap(a.const << n, t)
                return Ex(hexs(v), t, v, v, pre, const=v)
            if n is not None:
                lo, hi = a.lo << n, a.hi << n
            else:
                lo, hi = -INF, INF
            return self.wrapped('%s << %s' % (self.paren(a.code), ncode), t, lo, hi, pre)
        a, b, t = self.common(a, b)
        pre = a.pre + b.pre
        if op in ('==', '!=', '<', '>', '<=', '>='):
            if a.const is not None and b.const is not None:
                v = int({'==': a.const == b.const, '!=': a.const != b.const, '<': a.const < b.const,
                         '>': a.const > b.const, '<=': a.const <= b.const, '>=': a.const >= b.const}[op])
                return Ex(str(v), S32, v, v, pre, const=v, cond='true' if v else 'false')
            cond = '%s %s %s' % (self.paren(a.code), op, self.paren(b.code))
            return Ex('int(%s)' % cond, S32, 0, 1, pre, cond=cond)
        if a.const is not None and b.const is not None and op not in ('/', '%'):
            ops = {'+': lambda x, y: x + y, '-': lambda x, y: x - y, '*': lambda x, y: x * y, '&': lambda x, y: x & y,
                   '|': lambda x, y: x | y, '^': lambda x, y: x ^ y}
            v = wrap(ops[op](a.const, b.const), t)
            return Ex(hexs(v), t, v, v, pre, const=v)
        if op in ('+', '-', '*'):
            a, b = self.rawview(a), self.rawview(b)
        if op == '+':
            lo, hi = a.lo + b.lo, a.hi + b.hi
            code = '%s + %s' % (lowp(a.code), lowp(b.code))
        elif op == '-':
            lo, hi = a.lo - b.hi, a.hi - b.lo
            code = '%s - %s' % (lowp(a.code), lowp(b.code, True))
        elif op == '*':
            c = [a.lo * b.lo, a.lo * b.hi, a.hi * b.lo, a.hi * b.hi]
            lo, hi = min(c), max(c)
            code = '%s * %s' % (self.paren(a.code), self.paren(b.code))
        elif op in ('/', '%'):
            self.uses_div = True
            fn = '_div' if op == '/' else '_mod'
            if a.const is not None and b.const not in (None, 0):
                q = int(a.const / b.const)
                v = q if op == '/' else a.const - q * b.const
                return Ex(hexs(v), t, v, v, pre, const=v)
            if op == '/':
                m = max(abs(a.lo), abs(a.hi))
                lo, hi = (-m, m) if t.signed else (0, a.hi)
            else:
                m = max(abs(b.lo), abs(b.hi))
                lo, hi = ((-m, m) if t.signed else (0, max(0, m)))
            return Ex('%s(%s, %s)' % (fn, a.code, b.code), t, max(lo, int_range(t)[0]), min(hi, int_range(t)[1]), pre)
        elif op == '&':
            if a.lo >= 0 and b.lo >= 0:
                lo, hi = 0, min(a.hi, b.hi)
            elif b.lo >= 0:
                lo, hi = 0, b.hi
            elif a.lo >= 0:
                lo, hi = 0, a.hi
            else:
                lo, hi = int_range(t)
            return Ex('%s & %s' % (self.paren(a.code), self.paren(b.code)), t, lo, hi, pre)
        elif op in ('|', '^'):
            if a.lo >= 0 and b.lo >= 0:
                lo, hi = 0, (1 << max(a.hi, b.hi).bit_length()) - 1
            else:
                lo, hi = int_range(t)
            return Ex('%s %s %s' % (self.paren(a.code), op, self.paren(b.code)), t, lo, hi, pre)
        else:
            raise Unsupported('operator ' + op)
        return self.wrapped(code, t, lo, hi, pre)

    @staticmethod
    def rawview(ex):
        if ex.raw:
            return Ex(ex.raw[0], ex.ty, ex.raw[1], ex.raw[2], ex.pre, ex.const)
        return ex

    @staticmethod
    def addcode(code, k):
        if k == 0:
            return code
        return '%s %s 0x%X' % (lowp(code), '+' if k > 0 else '-', abs(k))

    def spill(self, ex):
        """Evaluate an expression into a temp now (keeps C evaluation order when a later operand has statements)."""
        if ex.const is not None:
            return ex
        t = self.fresh()
        pre = list(ex.pre) + ['var %s: int = %s' % (t, ex.code)]
        return Ex(t, ex.ty, ex.lo, ex.hi, pre, cond=None if ex.cond is None else '%s != 0' % t)

    # ---- addresses and lvalues
    def mem_read(self, addr, ty):
        if ty.kind == 'ptr':
            fn = 'u32'
        elif ty.kind == 'int':
            fn = READ[(ty.bits, ty.signed)]
        else:
            raise Unsupported('read of %r' % ty)
        return 'engine.memory.%s(%s)' % (fn, addr.code())

    def mem_write(self, addr, ty, value):
        if ty.kind == 'ptr':
            fn = 'u32'
        elif ty.kind == 'int':
            fn = READ[(ty.bits, ty.signed)].replace('s', 'u')
        else:
            raise Unsupported('write of %r' % ty)
        return 'engine.memory.write_%s(%s, %s)' % (fn, addr.code(), value)

    def lv_addr(self, lv):
        if lv.kind == 'mem':
            return lv.addr
        if lv.kind == 'gs':
            return Addr(self.u.GS, self.u.layout.gs[lv.name][0])
        if lv.kind == 'glob':
            return Addr(self.u.gconst(lv.name), 0)
        raise Unsupported('address of %s' % lv.kind)

    def read_lv(self, lv):
        t = lv.ty
        if t.kind == 'array':
            a = self.lv_addr(lv)
            return Ex(a.code(), T('ptr', to=t.to), 0, 0xFFFFFFFF, lv.pre)
        if t.kind in ('struct', 'union'):
            raise Unsupported('struct value')
        lo, hi = int_range(t)
        if lv.bits:
            sh, w = lv.bits
            mask = (1 << w) - 1
            raw = self.mem_read(lv.addr, t)
            code = '(%s >> %d) & 0x%X' % (raw, sh, mask) if sh else '%s & 0x%X' % (raw, mask)
            return Ex(code, t, 0, mask, lv.pre)
        if lv.kind == 'arr':
            return Ex('%s[%s]' % (lv.name, lv.addr), t, lo, hi, lv.pre)
        if lv.kind == 'local':
            flag = self.guard_flag.get(lv.name)
            if flag:
                self.uses_uninit = True
                return Ex('_uninit("%s", "%s", %s, %s)' % (self.name, getattr(lv, 'cname', lv.name), lv.name, flag), t, lo, hi, lv.pre)
            return Ex(lv.name, t, lo, hi, lv.pre)
        if lv.kind == 'gs':
            return Ex('engine.gs_get("%s")' % lv.name, t, lo, hi, lv.pre)
        if lv.kind == 'glob':
            return Ex('engine.memory.read_global("%s")' % lv.name, t, lo, hi, lv.pre)
        return Ex(self.mem_read(lv.addr, t), t, lo, hi, lv.pre)

    def write_lv(self, lv, ex):
        """Statement storing `ex` (already converted to lv.ty) into lv."""
        v = ex.code
        if lv.bits:
            sh, w = lv.bits
            mask = (1 << w) - 1
            full = (1 << lv.ty.bits) - 1
            keep = full & ~(mask << sh)
            rd = self.mem_read(lv.addr, lv.ty)
            val = '(%s & 0x%X)' % (v, mask) if ex.const is None or ex.const > mask or ex.const < 0 else v
            if sh:
                val = '(%s << %d)' % (val, sh)
            return self.mem_write(lv.addr, lv.ty, '(%s & 0x%X) | %s' % (rd, keep, val))
        if lv.kind == 'arr':
            return '%s[%s] = %s' % (lv.name, lv.addr, v)
        if lv.kind == 'local':
            flag = self.guard_flag.get(lv.name)
            if flag:
                return '%s = %s; %s = true' % (lv.name, v, flag)
            return '%s = %s' % (lv.name, v)
        if lv.kind == 'gs':
            return 'engine.gs_set("%s", %s)' % (lv.name, v)
        if lv.kind == 'glob':
            return 'engine.memory.write_global("%s", %s)' % (lv.name, v)
        return self.mem_write(lv.addr, lv.ty, v)

    @staticmethod
    def snake(tag):
        import re as _re
        return _re.sub(r'(?<=[a-z0-9])(?=[A-Z])|(?<=[A-Z])(?=[A-Z][a-z])', '_', tag).upper()

    def size_const(self, t):
        """Name of the size constant of a struct/union type with a tag ('' for anything else)."""
        if t.kind in ('struct', 'union') and t.tag:
            name = self.snake(t.tag) + '_SIZE'
            self.u.consts[name] = self.types.size(t)
            return name
        return ''

    def field_addr(self, a, bt, field, off):
        if off and bt.tag:
            name = '%s_%s' % (self.snake(bt.tag), field.upper())
            self.u.consts[name] = off
            return a.plus_sym(name)
        return a.plus(off)

    def lvalue(self, n):
        if isinstance(n, c_ast.ID):
            loc = self.lookup_local(n.name)
            if loc is not None:
                gd, ty = loc
                if isinstance(gd, ArrLocal):
                    raise Unsupported('array local %s used as a value' % gd.name)
                if isinstance(gd, int):
                    return LV('mem', ty, addr=Addr('engine.frame_base()', gd))
                if ty.kind not in ('int', 'ptr'):
                    raise Unsupported('local %s of type %r lives on the stack' % (n.name, ty))
                lv = LV('local', ty, gd)
                lv.cname = n.name
                return lv
            if n.name == 'game_state':
                return LV('mem', self.types.globals[n.name], addr=Addr(self.u.GS, 0))
            if n.name not in self.types.globals:
                raise Unsupported('unknown identifier %s' % n.name)
            ty = self.types.globals[n.name]
            row = self.u.layout.globals.get(n.name)
            addr = self.u.gaddr(n.name)
            if (row and ty.kind in ('int', 'ptr') and row[3] == 1 and not row[4]
                    and row[1] == (ty.bits // 8 if ty.kind == 'int' else 4)
                    and (ty.kind == 'ptr' or row[2] == ty.signed)):
                self.u.note_global(n.name)
                return LV('glob', ty, n.name)
            return LV('mem', ty, addr=addr)
        if isinstance(n, c_ast.StructRef):
            if n.type == '.':
                base = self.lvalue(n.name)
                if base.kind == 'local':
                    raise Unsupported('member of local')
                bt = base.ty
                a = self.lv_addr(base)
                pre = base.pre
            else:
                p = self.expr(n.name)
                bt = p.ty.to if p.ty.kind == 'ptr' else None
                if bt is None or bt.kind not in ('struct', 'union'):
                    raise Unsupported('-> on %r' % p.ty)
                a = Addr(p.code, 0) if p.const is None else Addr('', p.const)
                pre = p.pre
            ft, off = self.types.field(bt, n.field.name)
            bit = self.types.bitfield(bt, n.field.name)
            if bit is not None:
                if ft.kind != 'int' or ft.signed:
                    raise Unsupported('signed bit-field %s' % n.field.name)
                return LV('mem', ft, addr=self.field_addr(a, bt, n.field.name, off), pre=pre, bits=bit)
            if bt.tag == 'GameState' and n.type == '.' and isinstance(n.name, c_ast.ID) and n.name.name == 'game_state':
                row = self.u.layout.gs.get(n.field.name)
                if (row and ft.kind in ('int', 'ptr') and row[3] == 1 and not row[4] and row[5] == 0
                        and row[0] == off and row[1] == (ft.bits // 8 if ft.kind == 'int' else 4)
                        and (ft.kind == 'ptr' or row[2] == ft.signed)):
                    return LV('gs', ft, n.field.name, pre=pre)
                if row and row[0] != off:
                    raise Unsupported('GameState.%s offset 0x%X differs from the layout 0x%X' % (n.field.name, off, row[0]))
            return LV('mem', ft, addr=self.field_addr(a, bt, n.field.name, off), pre=pre)
        if isinstance(n, c_ast.ArrayRef):
            subs = []
            root = n
            while isinstance(root, c_ast.ArrayRef):
                subs.insert(0, root.subscript)
                root = root.name
            loc = self.lookup_local(root.name) if isinstance(root, c_ast.ID) else None
            if loc is not None and isinstance(loc[0], ArrLocal):
                arr = loc[0]
                if len(subs) != len(arr.dims):
                    raise Unsupported('partial index of array local %s' % arr.name)
                pre = []
                terms = []
                stride = 1
                for k in range(len(subs) - 1, -1, -1):
                    ix = self.promote(self.expr(subs[k]))
                    pre += ix.pre
                    terms.insert(0, ix.code if stride == 1 else '%s * %d' % (self.paren(ix.code), stride))
                    stride *= arr.dims[k]
                return LV('arr', arr.elem, name=arr.name, addr=' + '.join(lowp(t) for t in terms), pre=pre)
            base = self.expr(n.name)
            if base.ty.kind != 'ptr':
                raise Unsupported('index of %r' % base.ty)
            et = base.ty.to
            size = self.types.size(et)
            idx = self.promote(self.expr(n.subscript))
            pre = base.pre + idx.pre
            if idx.pre:
                base = self.spill(base)
                pre = base.pre + idx.pre
            if base.const is not None:
                a = Addr('', base.const)
            else:
                a = Addr(base.code, 0)
            size_name = self.size_const(et)
            if idx.const is not None:
                a = a.plus_sym(size_name, idx.const) if size_name and idx.const else a.plus(idx.const * size)
            else:
                unit = size_name or str(size)
                term = idx.code if size == 1 else '%s * %s' % (self.paren(idx.code), unit)
                a = Addr(('%s + %s' % (lowp(a.dyn), lowp(term))) if a.dyn else term, a.off)
            return LV('mem', et, addr=a, pre=pre)
        if isinstance(n, c_ast.UnaryOp) and n.op == '*':
            p = self.expr(n.expr)
            if p.ty.kind != 'ptr':
                raise Unsupported('deref of %r' % p.ty)
            a = Addr(p.code, 0) if p.const is None else Addr('', p.const)
            return LV('mem', p.ty.to, addr=a, pre=p.pre)
        if isinstance(n, c_ast.Cast):
            raise Unsupported('cast as lvalue')
        raise Unsupported('lvalue %s' % type(n).__name__)

    # ---- expressions
    def expr(self, n):
        if isinstance(n, c_ast.Constant):
            if n.type == 'string':
                addr = self.u.string_addr(n.value, n)
                return Ex(hexs(addr), T('ptr', to=S8), addr, addr, const=addr)
            if n.type in ('float', 'double'):
                raise Unsupported('float')
            v = parse_int(n.value)
            t = literal_type(n.value) if n.type != 'char' else S32
            v = wrap(v, t) if t.signed else v
            return Ex(hexs(v), t, v, v, const=v)
        if isinstance(n, c_ast.ID):
            if n.name in self.types.enums and self.lookup_local(n.name) is None:
                v = self.types.enums[n.name]
                return Ex(hexs(v), S32, v, v, const=v)
            loc = self.lookup_local(n.name)
            if loc is None and n.name in self.types.funcs and n.name not in self.types.globals:
                self.u.need_function_address = True
                return Ex('engine.function_address("%s")' % n.name, T('ptr', to=VOID))
            return self.read_lv(self.lvalue(n))
        if isinstance(n, (c_ast.StructRef, c_ast.ArrayRef)):
            return self.read_lv(self.lvalue(n))
        if isinstance(n, c_ast.UnaryOp):
            return self.unary(n)
        if isinstance(n, c_ast.BinaryOp):
            return self.binary(n)
        if isinstance(n, c_ast.Cast):
            to = self.types.build(n.to_type)
            e = self.expr(n.expr)
            if to.kind == 'void':
                return e
            if to.kind == 'ptr' or to.kind == 'int':
                if e.ty.kind == 'array':
                    e = Ex(e.code, T('ptr', to=e.ty.to), pre=e.pre)
                r = self.conv(e, to)
                return r
            raise Unsupported('cast to %r' % to)
        if isinstance(n, c_ast.FuncCall):
            return self.call(n)
        if isinstance(n, c_ast.Assignment):
            return self.assign_expr(n)
        if isinstance(n, c_ast.TernaryOp):
            c = self.expr(n.cond)
            a, b = self.expr(n.iftrue), self.expr(n.iffalse)
            if a.pre or b.pre:
                raise Unsupported('ternary with side effects')
            if a.ty.kind == 'ptr' or b.ty.kind == 'ptr':
                t = a.ty if a.ty.kind == 'ptr' else b.ty
                a, b = self.conv(a, U32), self.conv(b, U32)
            else:
                a, b, t = self.common(a, b)
            return Ex('(%s if %s else %s)' % (a.code, self.cond_of(c), b.code), t, min(a.lo, b.lo), max(a.hi, b.hi), c.pre)
        if isinstance(n, c_ast.ExprList):
            pre = []
            for e in n.exprs[:-1]:
                pre += self.stmt_expr_lines(e)
            last = self.expr(n.exprs[-1])
            return Ex(last.code, last.ty, last.lo, last.hi, pre + last.pre, last.const, last.cond)
        raise Unsupported('expression %s' % type(n).__name__)

    def unary(self, n):
        op = n.op
        if op == 'sizeof':
            if isinstance(n.expr, c_ast.Typename):
                v = self.types.size(self.types.build(n.expr.type))
            else:
                v = self.types.size(self.expr_type(n.expr))
            return Ex(str(v), U32, v, v, const=v)
        if op == '&':
            lv = self.lvalue(n.expr)
            if lv.bits:
                raise Unsupported('address of a bit-field')
            if lv.kind == 'local':
                raise Unsupported('address of local %s' % lv.name)
            if lv.kind == 'gs':
                row = self.u.layout.gs[lv.name]
            a = self.lv_addr(lv)
            if a.dyn == '' :
                return Ex(hexs(a.off & 0xFFFFFFFF), T('ptr', to=lv.ty), a.off & 0xFFFFFFFF, a.off & 0xFFFFFFFF, lv.pre,
                          const=a.off & 0xFFFFFFFF)
            return Ex(a.code(), T('ptr', to=lv.ty), 0, 0xFFFFFFFF, lv.pre)
        if op == '*':
            if (isinstance(n.expr, c_ast.Cast) and isinstance(n.expr.expr, c_ast.UnaryOp) and n.expr.expr.op == '&'
                    and isinstance(n.expr.expr.expr, c_ast.ID)):
                # *(u8 *)&param: the low bytes of a K&R parameter (little endian, spilled to its home slot)
                loc = self.lookup_local(n.expr.expr.expr.name)
                to = self.types.build(n.expr.to_type)
                if loc is not None and not isinstance(loc[0], int) and to.kind == 'ptr' and to.to.kind == 'int' \
                        and loc[1].kind == 'int':
                    return self.conv(self.read_lv(self.lvalue(n.expr.expr.expr)), to.to)
            return self.read_lv(self.lvalue(n))
        if op in ('p++', 'p--', '++', '--'):
            return self.incdec(n, True)
        e = self.expr(n.expr)
        if op == '!':
            c = self.cond_of(e)
            nc = 'not %s' % self.paren(c) if ' ' in c else 'not %s' % c
            if e.const is not None:
                v = int(not e.const)
                return Ex(str(v), S32, v, v, e.pre, const=v, cond='true' if v else 'false')
            return Ex('int(%s)' % nc, S32, 0, 1, e.pre, cond=nc)
        e = self.promote(e)
        if op == '-':
            if e.const is not None:
                v = wrap(-e.const, e.ty)
                return Ex(hexs(v), e.ty, v, v, e.pre, const=v)
            return self.wrapped('-%s' % self.paren(e.code), e.ty, -e.hi, -e.lo, e.pre)
        if op == '+':
            return e
        if op == '~':
            if e.const is not None:
                v = wrap(~e.const, e.ty)
                return Ex(hexs(v), e.ty, v, v, e.pre, const=v)
            if e.ty.signed:
                return Ex('~%s' % self.paren(e.code), e.ty, ~e.hi, ~e.lo, e.pre)
            return Ex('Math.u32(~%s)' % self.paren(e.code), e.ty, pre=e.pre)
        raise Unsupported('unary ' + op)

    def expr_type(self, n):
        """Type of an expression without emitting (used by sizeof)."""
        saved = (self.lines, self.tmp)
        try:
            return self.expr(n).ty
        finally:
            self.lines, self.tmp = saved

    def binary(self, n):
        op = n.op
        if op in ('&&', '||'):
            a = self.expr(n.left)
            b = self.expr(n.right)
            ca, cb = self.cond_of(a), self.cond_of(b)
            word = 'and' if op == '&&' else 'or'
            if a.const is not None and b.const is not None:
                v = int(bool(a.const and b.const) if op == '&&' else bool(a.const or b.const))
                return Ex(str(v), S32, v, v, a.pre + b.pre, const=v, cond='true' if v else 'false')
            if b.pre:
                t = self.fresh('_c')
                body = list(b.pre) + ['%s = %s' % (t, cb)]
                guard = t if op == '&&' else 'not %s' % t
                pre = a.pre + ['var %s: bool = %s' % (t, ca), 'if %s:' % guard] + ['\t' + x for x in body]
                return Ex('int(%s)' % t, S32, 0, 1, pre, cond=t)
            cond = '%s %s %s' % (self.wrap_cond(ca), word, self.wrap_cond(cb))
            return Ex('int(%s)' % cond, S32, 0, 1, a.pre, cond=cond)
        a = self.expr(n.left)
        b = self.expr(n.right)
        if a.ty.kind == 'array':
            a = Ex(a.code, T('ptr', to=a.ty.to), pre=a.pre)
        if b.ty.kind == 'array':
            b = Ex(b.code, T('ptr', to=b.ty.to), pre=b.pre)
        return self.binop(op, a, b)

    @staticmethod
    def wrap_cond(c):
        if ' and ' in c or ' or ' in c:
            return '(%s)' % c
        return c

    def call(self, n):
        pre = []
        args = []
        if isinstance(n.name, c_ast.ID) and (self.lookup_local(n.name.name) is None) and (
                n.name.name in self.types.funcs or n.name.name not in self.types.globals):
            fname = n.name.name
            ret, params, known = self.types.funcs.get(fname, (S32, [], False))
            fname = ALIASES.get(fname, fname)
            self.callees.add(fname)
            exprs = n.args.exprs if n.args else []
            vals = []
            for i, a in enumerate(exprs):
                e = self.expr(a)
                if e.ty.kind == 'array':
                    e = Ex(e.code, T('ptr', to=e.ty.to), pre=e.pre)
                if known and i < len(params) and params[i].kind in ('int', 'ptr'):
                    e = self.conv(e, params[i])
                elif e.ty.kind in ('int', 'ptr') and e.ty.kind == 'int' and e.ty.bits < 32:
                    e = self.promote(e)
                vals.append(e)
            # keep evaluation order when a later argument has statements
            for i in range(len(vals)):
                if vals[i].pre:
                    for j in range(i):
                        if vals[j].const is None and not vals[j].code.isidentifier():
                            vals[j] = self.spill(vals[j])
            for v in vals:
                pre += v.pre
                args.append(v.code)
            arity = self.u.def_arity(fname)
            note = ''
            if arity is not None and len(args) > arity:
                for extra in exprs[arity:]:
                    if self.has_calls(extra) or not self.pure(extra):
                        raise Unsupported('extra argument with side effects in call to %s' % fname)
                note = len(args) - arity
                args = args[:arity]
            elif arity is not None and len(args) < arity and fname not in ('printf', 'sprintf'):
                raise Unsupported('call to %s passes %d of %d arguments (stale register arguments)' % (fname, len(args), arity))
            code = 'engine.call_c("%s"%s)' % (fname, (', [%s]' % ', '.join(args)) if args else '')
            if note:
                self.call_notes[code] = note
            if ret.kind == 'void':
                return Ex(code, VOID, 0, 0, pre)
            r = Ex(code, ret if ret.kind in ('int', 'ptr') else S32, -INF, INF, pre)
            return self.conv(r, ret if ret.kind in ('int', 'ptr') else S32)
        # call through a pointer
        target = n.name
        if isinstance(target, c_ast.UnaryOp) and target.op == '*':
            target = target.expr
        via = target.name if isinstance(target, c_ast.ID) else 'indirect'
        p = self.expr(target)
        exprs = n.args.exprs if n.args else []
        vals = [self.expr(a) for a in exprs]
        pre = list(p.pre)
        for v in vals:
            pre += v.pre
        args = [v.code for v in vals]
        self.callees.add('<indirect %s>' % via)
        code = 'engine.call_address("%s", %s%s)' % (via, p.code, (', [%s]' % ', '.join(args)) if args else '')
        return Ex(code, S32, -INF, INF, pre)

    # assignment as a statement list + resulting value
    def assign_expr(self, n):
        lv = self.lvalue(n.lvalue)
        if lv.ty.kind in ('struct', 'union') and n.op == '=':
            src = self.lvalue(n.rvalue)
            size = self.types.size(lv.ty)
            pre = list(lv.pre) + list(src.pre)
            pre.append('engine.memory.write_bytes(%s, engine.memory.read_bytes(%s, %d))' % (
                self.lv_addr(lv).code(), self.lv_addr(src).code(), size))
            return Ex('0', VOID, 0, 0, pre)
        if lv.ty.kind not in ('int', 'ptr'):
            raise Unsupported('assignment to %r' % lv.ty)
        pre = list(lv.pre)
        if lv.kind == 'mem' and 'call_c(' in lv.addr.code():
            t = self.fresh()
            pre.append('var %s: int = %s' % (t, lv.addr.code()))
            lv = LV('mem', lv.ty, addr=Addr(t, 0))
        rhs = self.expr(n.rvalue)
        if rhs.ty.kind == 'array':
            rhs = Ex(rhs.code, T('ptr', to=rhs.ty.to), pre=rhs.pre)
        if n.op == '=':
            val = self.conv(rhs, lv.ty) if lv.kind in ('local', 'arr') else rhs
        else:
            cur = self.read_lv(lv)
            cur = Ex(cur.code, cur.ty, cur.lo, cur.hi, [])
            res = self.binop(n.op[:-1], cur, Ex(rhs.code, rhs.ty, rhs.lo, rhs.hi, rhs.pre, rhs.const, rhs.cond))
            val = self.conv(res, lv.ty) if lv.kind in ('local', 'arr') else res
        pre += val.pre
        val = Ex(val.code, val.ty, val.lo, val.hi, [], val.const)
        pre.append(self.write_lv(lv, val))
        again = self.read_lv(lv)
        return Ex(again.code, again.ty, again.lo, again.hi, pre)

    def incdec(self, n, want_value):
        lv = self.lvalue(n.expr)
        if lv.ty.kind not in ('int', 'ptr'):
            raise Unsupported('increment of %r' % lv.ty)
        pre = list(lv.pre)
        if lv.kind == 'mem' and 'call_c(' in lv.addr.code():
            t = self.fresh()
            pre.append('var %s: int = %s' % (t, lv.addr.code()))
            lv = LV('mem', lv.ty, addr=Addr(t, 0))
        cur = self.read_lv(lv)
        cur = Ex(cur.code, cur.ty, cur.lo, cur.hi, [])
        one = Ex('1', S32, 1, 1, const=1)
        res = self.binop('+' if '++' in n.op else '-', cur, one)
        val = self.conv(res, lv.ty) if lv.kind in ('local', 'arr') else res
        old = None
        if want_value and n.op.startswith('p'):
            old = self.fresh()
            pre.append('var %s: int = %s' % (old, cur.code))
        pre.append(self.write_lv(lv, Ex(val.code, val.ty, val.lo, val.hi, [], val.const)))
        if want_value:
            if old:
                return Ex(old, lv.ty, int_range(lv.ty)[0], int_range(lv.ty)[1], pre)
            again = self.read_lv(lv)
            return Ex(again.code, again.ty, again.lo, again.hi, pre)
        return pre

    # ---- statements
    def stmt_expr_lines(self, e):
        """Lines (unindented) for an expression evaluated for its side effects."""
        if isinstance(e, c_ast.Assignment):
            ex = self.assign_expr(e)
            return ex.pre
        if isinstance(e, c_ast.UnaryOp) and e.op in ('p++', 'p--', '++', '--'):
            return self.incdec(e, False)
        if isinstance(e, c_ast.FuncCall):
            ex = self.call(e)
            line = ex.raw[0] if ex.raw else ex.code
            note = self.call_notes.get(line)
            if note:
                line += ' # the C also passes %d argument(s) the callee ignores' % note
            return ex.pre + [line]
        if isinstance(e, c_ast.ExprList):
            out = []
            for x in e.exprs:
                out += self.stmt_expr_lines(x)
            return out
        if isinstance(e, c_ast.Cast):
            return self.stmt_expr_lines(e.expr)
        ex = self.expr(e)
        if self.has_calls(e) or not self.pure(e):
            raise Unsupported('expression statement with side effects that is not an assignment, increment or call')
        return ex.pre

    def emit_lines(self, lines):
        for l in lines:
            self.emit(l)

    def stmt_list(self, items):
        """Statements of one block, with C labels: a label that a later `goto` jumps back to starts a loop (the rest of
        the block is its body; `goto` is `continue`); a label that only earlier-written gotos reach is recorded and its
        statements are written out again at each goto."""
        for k, it in enumerate(items):
            if isinstance(it, c_ast.Label):
                rest = [it.stmt] + list(items[k + 1:])
                name = it.name
                if any(self.has_goto(r, name) for r in rest):
                    self.u.loops_with_calls.append(self.name + ' (goto ' + name + ')')
                    self.emit('while true: # C label %s: the goto is a continue' % name)
                    self.ind += 1
                    self.emit('if engine.halted():')
                    self.emit('\tbreak')
                    self.ctx.append(('loop', [], name))
                    self.stmt_list(rest)
                    self.ctx.pop()
                    if self.lines[-1].strip().startswith('return'):
                        self.loop_returns_at_end = True  # the loop body ends with a return: no way out but a refusal
                    else:
                        self.emit('break')
                    self.ind -= 1
                else:
                    self.labels[name] = rest
                    self.stmt_list(rest)
                return
            self.stmt(it)

    def has_goto(self, node, name):
        found = []

        class V(c_ast.NodeVisitor):
            def visit_Goto(s, x):
                if x.name == name:
                    found.append(x)
        V().visit(node)
        return bool(found)

    def block(self, n, new_scope=True):
        start = len(self.lines)
        if new_scope:
            self.scopes.append({})
        items = n.block_items or [] if isinstance(n, c_ast.Compound) else [n]
        self.stmt_list(items)
        if new_scope:
            self.scopes.pop()
        if len(self.lines) == start:
            self.emit('pass')

    def body(self, n):
        self.ind += 1
        self.block(n if isinstance(n, c_ast.Compound) else c_ast.Compound([n]))
        self.ind -= 1

    def decl(self, d):
        if isinstance(d.type, c_ast.FuncDecl):
            return
        ty = self.types.build(d.type)
        dims = ARRAY_LOCALS.get(self.name, {}).get(d.name)
        if dims is not None:
            et = ty
            while et.kind == 'array':
                et = et.to
            if et.kind != 'int' or d.init is not None:
                raise Unsupported('array local %s' % d.name)
            gd = ArrLocal(self.gdname(d.name), dims, et)
            self.scopes[-1][d.name] = (gd, ty)
            count = 1
            for n_ in dims:
                count *= n_
            self.hoisted.append('var %s: Array[int] = []' % gd.name)
            self.hoisted.append('%s.resize(%d)' % (gd.name, count))
            return
        off = STACK_LOCALS.get(self.name, {}).get(d.name)
        if off is not None:
            self.scopes[-1][d.name] = (off, ty)
            if d.init is not None:
                self.emit('engine.memory.write_bytes(engine.frame_base() + 0x%X, PackedByteArray([%s]))' % (
                    off, ', '.join(str(b) for b in self.init_bytes(ty, d.init))))
            return
        if ty.kind not in ('int', 'ptr'):
            raise Unsupported('local %s of type %r (stack local)' % (d.name, ty))
        gd = self.gdname(d.name)
        self.scopes[-1][d.name] = (gd, ty)
        self.hoisted.append('var %s: int = 0' % gd)
        if d.name in self.guarded:
            flag = self.gdname(d.name + '_set')
            self.guard_flag[gd] = flag
            self.hoisted.append('var %s: bool = false' % flag)
        if d.init is not None:
            if isinstance(d.init, c_ast.InitList):
                raise Unsupported('initializer list')
            ex = self.conv(self.expr(d.init), ty)
            self.emit_lines(ex.pre)
            self.emit('%s = %s' % (gd, ex.code))

    def init_bytes(self, ty, init):
        size = self.types.size(ty)
        if isinstance(init, c_ast.Constant) and init.type == 'string' and ty.kind == 'array':
            data = list(decode_c_string(init.value).encode('cp932')) + [0]
        elif isinstance(init, c_ast.InitList) and ty.kind == 'array' and ty.to.kind == 'int':
            data = []
            for e in init.exprs:
                v = self.types.const(e)
                data += list((v & ((1 << ty.to.bits) - 1)).to_bytes(ty.to.bits // 8, 'little'))
        else:
            raise Unsupported('initializer of stack local')
        return (data + [0] * size)[:size]

    def stmt(self, n):
        if isinstance(n, c_ast.Compound):
            self.scopes.append({})
            self.stmt_list(n.block_items or [])
            self.scopes.pop()
        elif isinstance(n, c_ast.Decl):
            self.decl(n)
        elif isinstance(n, c_ast.EmptyStatement):
            pass
        elif isinstance(n, c_ast.Return):
            if n.expr is None:
                self.has_void_return = True
                self.emit('return')
            else:
                self.has_value_return = True
                rt = self.ret_type
                ex = self.expr(n.expr)
                if rt.kind == 'void':
                    self.emit_lines(ex.pre)
                    self.emit('return')
                else:
                    ex = self.conv(ex, rt)
                    self.emit_lines(ex.pre)
                    self.emit('return %s' % ex.code)
                    self.return_values.append(ex.const)
        elif isinstance(n, c_ast.If):
            self.if_stmt(n, 'if')
        elif isinstance(n, c_ast.While):
            self.loop(None, n.cond, None, n.stmt, False)
        elif isinstance(n, c_ast.DoWhile):
            self.loop(None, n.cond, None, n.stmt, True)
        elif isinstance(n, c_ast.For):
            init = n.init
            if isinstance(init, c_ast.DeclList):
                for d in init.decls:
                    self.decl(d)
                init = None
            self.loop(init, n.cond, n.next, n.stmt, False)
        elif isinstance(n, c_ast.Switch):
            self.switch(n)
        elif isinstance(n, c_ast.Break):
            if not self.ctx:
                raise Unsupported('break outside loop/switch')
            top = self.ctx[-1]
            if top[0] == 'switch' and not top[1]:
                raise Unsupported('break inside an unwrapped switch arm')
            self.emit('break')
        elif isinstance(n, c_ast.Continue):
            loops = [c for c in self.ctx if c[0] == 'loop']
            if not loops:
                raise Unsupported('continue outside loop')
            idx = max(i for i, c in enumerate(self.ctx) if c[0] == 'loop')
            if any(c[0] == 'switch' for c in self.ctx[idx + 1:]):
                # GDScript reads `continue` in a match arm as "try the next pattern", never as the loop's continue
                raise Unsupported('continue inside a switch arm')
            self.emit_lines(self.ctx[idx][1])
            self.emit('continue')
        elif isinstance(n, c_ast.Goto):
            for entry in reversed(self.ctx):
                if entry[0] == 'loop' and len(entry) > 2 and entry[2] == n.name:
                    if any(c[0] == 'switch' for c in self.ctx[self.ctx.index(entry) + 1:]):
                        raise Unsupported('goto (a continue) inside a switch arm')
                    self.emit_lines(entry[1])
                    self.emit('continue')
                    return
            if n.name in self.labels:
                for item in self.labels[n.name]:  # a forward label: its statements are written out at the goto
                    self.stmt(item)
                return
            raise Unsupported('goto %s' % n.name)
        elif isinstance(n, c_ast.Label):
            self.stmt_list([n])
        elif isinstance(n, c_ast.Case) or isinstance(n, c_ast.Default):
            raise Unsupported('case outside switch')
        else:
            lines = self.stmt_expr_lines(n)
            self.emit_lines(lines)

    def if_stmt(self, n, kw):
        if (kw == 'if' and n.iffalse is None and not self.has_calls(n.cond) and self.pure(n.cond)
                and (isinstance(n.iftrue, c_ast.Compound) and not n.iftrue.block_items)):
            return  # `if (x) {}` with a side-effect free condition: nothing to do
        c = self.expr(n.cond)
        self.emit_lines(c.pre)
        if c.const is not None and False:
            pass
        self.emit('%s %s:' % (kw, self.cond_of(c)))
        self.body(n.iftrue)
        if n.iffalse is not None:
            if isinstance(n.iffalse, c_ast.If):
                # elif only when the nested condition needs no statements
                saved = len(self.lines)
                probe = FuncProbe(self)
                if not probe.has_pre(n.iffalse.cond):
                    self.if_stmt(n.iffalse, 'elif')
                    return
            self.emit('else:')
            self.body(n.iffalse)

    def pure(self, node):
        found = []

        class V(c_ast.NodeVisitor):
            def visit_Assignment(s, x):
                found.append(x)

            def visit_UnaryOp(s, x):
                if x.op in ('p++', 'p--', '++', '--'):
                    found.append(x)
                s.generic_visit(x)
        V().visit(node)
        return not found

    def has_calls(self, node):
        found = []

        class V(c_ast.NodeVisitor):
            def visit_FuncCall(s, x):
                found.append(x)
                s.generic_visit(x)
        if node is not None:
            V().visit(node)
        return bool(found)

    def loop(self, init, cond, step, body, dowhile):
        if init is not None:
            self.emit_lines(self.stmt_expr_lines(init))
        calls = self.has_calls(cond) or self.has_calls(body) or self.has_calls(step)
        step_lines = self.stmt_expr_lines(step) if step is not None else []
        guard = ['if engine.halted():', '\tbreak'] if calls else []
        if calls:
            self.u.loops_with_calls.append(self.name)
        if dowhile:
            c = self.expr(cond)
            tail = c.pre + ['if not %s:' % self.paren(self.cond_of(c)) if ' ' in self.cond_of(c) else 'if not %s:' % self.cond_of(c), '\tbreak']
            self.emit('while true:')
            self.ind += 1
            self.emit_lines(guard)
            self.ctx.append(('loop', tail))
            self.block(body if isinstance(body, c_ast.Compound) else c_ast.Compound([body]))
            self.ctx.pop()
            self.emit_lines(tail)
            self.ind -= 1
            return
        c = self.expr(cond) if cond is not None else None
        if c is None or c.pre:
            self.emit('while true:')
            self.ind += 1
            if c is not None:
                self.emit_lines(c.pre)
                cc = self.cond_of(c)
                self.emit('if not %s:' % (self.paren(cc) if ' ' in cc else cc))
                self.emit('\tbreak')
            self.emit_lines(guard)
        else:
            self.emit('while %s:' % self.cond_of(c))
            self.ind += 1
            self.emit_lines(guard)
        self.ctx.append(('loop', step_lines))
        self.block(body if isinstance(body, c_ast.Compound) else c_ast.Compound([body]))
        self.ctx.pop()
        self.emit_lines(step_lines)
        self.ind -= 1

    def switch(self, n):
        sel = self.expr(n.cond)
        self.emit_lines(sel.pre)
        items = n.stmt.block_items if isinstance(n.stmt, c_ast.Compound) else [n.stmt]
        arms = []  # [patterns(list of const or None for default), stmts]
        pending = []
        for it in items:
            if not isinstance(it, (c_ast.Case, c_ast.Default)):
                raise Unsupported('statement before first case')
            pat = None if isinstance(it, c_ast.Default) else self.types.const(it.expr)
            stmts = list(it.stmts or [])
            # `case A: case B: stmts` parses as Case(A, [Case(B, stmts)])
            while len(stmts) == 1 and isinstance(stmts[0], (c_ast.Case, c_ast.Default)):
                pending.append(pat)
                inner = stmts[0]
                pat = None if isinstance(inner, c_ast.Default) else self.types.const(inner.expr)
                stmts = list(inner.stmts or [])
            pending.append(pat)
            if not stmts:
                continue
            arms.append((pending, stmts))
            pending = []
        if pending:
            arms.append((pending, []))
        # arm bodies: strip the tail break; refuse fall-through
        prepared = []
        wrapped = False
        for i, (pats, stmts) in enumerate(arms):
            tail = self.flatten(stmts)
            last = tail[-1] if tail else None
            ends = isinstance(last, (c_ast.Break, c_ast.Return, c_ast.Continue, c_ast.Goto))
            if isinstance(last, c_ast.Break):
                tail = tail[:-1]
            elif not ends and i != len(arms) - 1:
                raise Unsupported('switch fall-through between arms')
            if any(self.inner_break(s) for s in tail):
                wrapped = True
            prepared.append((pats, tail))
        if wrapped:
            self.emit('while true: # a C break inside the switch leaves it')
            self.ind += 1
        self.emit('match %s:' % sel.code)
        self.ind += 1
        default = None
        self.ctx.append(('switch', wrapped))
        order = [a for a in prepared if None not in a[0]] + [a for a in prepared if None in a[0]]
        for pats, stmts in order:
            vals = [p for p in pats if p is not None]
            has_default = None in pats
            if has_default:
                head = '_:' # `case N: default:` is the default arm (the case values it adds are covered by it)
            else:
                head = ', '.join(hexs(wrap(v, S32) if v < 0 else v) for v in vals) + ':'
                if len(head) > 100:
                    head = 'var _case when _case in [%s]:' % ', '.join(hexs(v) for v in vals)
            self.emit(head)
            self.ind += 1
            self.scopes.append({})
            start = len(self.lines)
            for s in stmts:
                self.stmt(s)
            if len(self.lines) == start:
                self.emit('pass')
            self.scopes.pop()
            self.ind -= 1
        self.ctx.pop()
        self.ind -= 1
        if wrapped:
            self.emit('break')
            self.ind -= 1

    def flatten(self, stmts):
        out = []
        for s in stmts:
            if isinstance(s, c_ast.Compound) and not any(isinstance(x, c_ast.Decl) for x in (s.block_items or [])):
                out += self.flatten(s.block_items or [])
            else:
                out.append(s)
        return out

    def inner_break(self, s):
        """True when a `break` that targets the enclosing switch occurs in s (not nested in a loop/switch)."""
        if isinstance(s, c_ast.Break):
            return True
        if isinstance(s, (c_ast.For, c_ast.While, c_ast.DoWhile, c_ast.Switch)):
            return False
        for _, ch in s.children():
            if self.inner_break(ch):
                return True
        return False


class FuncProbe:
    """Checks whether an expression needs statements (ran on a scratch generator state)."""

    def __init__(self, g):
        self.g = g

    def has_pre(self, n):
        g = self.g
        saved = (list(g.lines), g.tmp, len(g.hoisted), set(g.used_names), set(g.callees))
        try:
            return bool(g.expr(n).pre)
        finally:
            g.lines, g.tmp = saved[0], saved[1]
            del g.hoisted[saved[2]:]
            g.used_names = saved[3]
            g.callees = saved[4]


class Unit:
    def __init__(self, repo, unit, only=None, asm_dir=''):
        self.repo = repo
        self.unit = unit
        self.layout = Layout(os.path.join(repo, 'port/godot/core/game_layout.gd'))
        self.symbols = load_symbols(repo)
        self.GS = 'GS'
        self.gconsts = {}
        self.missing = []
        self.uninit = {}
        self.consts = {}
        self.uses_uninit = False
        self.loops_with_calls = []
        self.need_function_address = False
        self.used_globals = set()
        src = os.path.join(repo, 'src/main/%s.c' % unit)
        self.src_text = open(src).read()
        pp = subprocess.run(['gcc', '-E', '-I' + os.path.join(repo, 'include'), src], capture_output=True, text=True,
                            cwd=repo)
        if pp.returncode != 0:
            raise SystemExit(pp.stderr[:2000])
        text = '\n'.join(l for l in pp.stdout.split('\n') if not l.startswith('__asm__'))
        # pycparser handles '# 12 "file"' directives; drop gcc builtin flags it dislikes
        text = re.sub(r'^# (\d+) "([^"]*)"[ \d]*$', r'# \1 "\2"', text, flags=re.M)
        ast = c_parser.CParser().parse(text, filename=src)
        self.types = Types(ast)
        self.ast = ast
        self.order = []
        for ext in ast.ext:
            if isinstance(ext, c_ast.FuncDef) and str(ext.coord.file).endswith('src/main/%s.c' % unit):
                self.order.append(ext.decl.name)
        self.only = only
        self.strings = {}
        self.string_diff = []
        self.string_report = (0, 0)
        self.ro_list = []
        self.string_nodes = {}
        ro = os.path.join(repo, 'asm/data/main/%s.rodata.s' % unit)
        main_ro = os.path.join(repo, 'asm/data/main')
        for cand in (ro, os.path.join(asm_dir or '/nonexistent', 'data/main/%s.rodata.s' % unit)):
            if os.path.exists(cand):
                txt = open(cand, encoding='utf-8').read()
                for m in re.finditer(r'dlabel (\w+)\n\s*/\* [0-9A-F]+ ([0-9A-F]{8})?(?: [0-9A-F]{8})?\s*\*/ (?:\.asciz "((?:[^"\\]|\\.)*)"|\.word 0x00000000\n)', txt):
                    name = m.group(1)
                    addr = int(name.split('_')[-1], 16) if re.fullmatch(r'D_[0-9A-F]{8}', name) else None
                    if addr is None:
                        am = re.search(r'/\* [0-9A-F]+ ([0-9A-F]{8}) \*/', txt[m.start():m.end()])
                        addr = int(am.group(1), 16) if am else None
                    self.strings.setdefault(decode_c_string(m.group(3) or ''), []).append(addr)
                    self.ro_list.append((addr, decode_c_string(m.group(3) or '')))
                break

    def assign_string_nodes(self):
        """Strings are not pooled: the i-th literal of the file in source order is the i-th rodata string."""
        nodes = []

        class V(c_ast.NodeVisitor):
            def visit_Decl(s, d):
                if isinstance(d.init, c_ast.Constant) and d.init.type == 'string' and isinstance(d.type, c_ast.ArrayDecl):
                    return  # array initialiser: bytes copied, not a rodata pointer
                s.generic_visit(d)

            def visit_Constant(s, n):
                if n.type == 'string':
                    nodes.append(n)
        for name in self.order:
            V().visit(self.types.funcdefs[name])
        nodes.sort(key=lambda n: (n.coord.line, n.coord.column))
        texts = [decode_c_string(n.value) for n in nodes]
        ro = [t for _, t in self.ro_list]
        # array initializers (u8 x[3] = "...") are not in rodata: drop literals that initialise arrays
        self.string_report = (len(nodes), len(ro))
        import difflib
        sm = difflib.SequenceMatcher(None, texts, ro, autojunk=False)
        self.string_diff = []
        for tag, a0, a1, b0, b1 in sm.get_opcodes():
            if tag == 'equal':
                for k in range(a1 - a0):
                    self.string_nodes[id(nodes[a0 + k])] = self.ro_list[b0 + k][0]
            else:
                self.string_diff.append((tag, [(nodes[k].coord.line, texts[k]) for k in range(a0, a1)],
                                         [(hex(self.ro_list[k][0]), ro[k]) for k in range(b0, b1)]))

    def string_addr(self, lit, node=None):
        if node is not None and id(node) in self.string_nodes:
            return self.string_nodes[id(node)]
        text = decode_c_string(lit)
        found = self.strings.get(text)
        if not found:
            raise Unsupported('string literal %r has no rodata entry' % text)
        if len(set(found)) != 1:
            raise Unsupported('string literal %r is ambiguous in rodata: %s' % (text, ['0x%X' % a for a in found]))
        return found[0]

    def def_arity(self, name):
        """Number of parameters of the C definition of `name` in src/main/*.c (None when not found)."""
        if not hasattr(self, '_arity'):
            self._arity = {}
            self._srcs = ''
            d = os.path.join(self.repo, 'src/main')
            for f in sorted(os.listdir(d)):
                if f.endswith('.c'):
                    self._srcs += open(os.path.join(d, f), errors='ignore').read() + '\n'
        if name in self._arity:
            return self._arity[name]
        pat = re.compile(r'^([A-Za-z_][\w \t\*]*?)[ \t\*]\**%s\s*\(([^;{]*?)\)\s*(\n[^{;]*;\s*)*\{' % re.escape(name), re.M | re.S)
        res = None
        for m in pat.finditer(self._srcs):
            head = m.group(1).strip()
            if head.split()[-1] in ('return', 'else', 'if', 'while', 'switch'):
                continue
            params = m.group(2).strip()
            res = 0 if params in ('', 'void') else len([q for q in params.split(',') if q.strip()])
            res += len(SIGNATURE_OVERRIDES.get(name, {}).get('extra_params', []))
            break
        self._arity[name] = res
        return res

    def note_global(self, name):
        self.used_globals.add(name)

    def gaddr(self, name):
        row = self.layout.globals.get(name)
        addr = row[0] if row else self.symbols.get(name)
        if addr is None:
            if name not in self.missing:
                self.missing.append(name)
            raise Unsupported('data symbol %s has no address (missing data symbol)' % name)
        return Addr(self.gconst(name, addr), 0)

    def gconst(self, name, addr=None):
        if name == 'GS':
            return 'GS'
        cname = 'G_' + name.upper()
        self.gconsts[cname] = (name, addr)
        return cname

    def gen_function(self, name):
        fd = self.types.funcdefs[name]
        g = FuncGen(self, name, fd)
        fdecl = fd.decl.type
        ret, params, known = self.types.func_sig(fdecl)
        g.ret_type = ret
        # parameter list (also K&R style definitions)
        plist = []
        if fdecl.args is not None:
            for p in fdecl.args.params:
                if isinstance(p, c_ast.EllipsisParam):
                    raise Unsupported('varargs')
                if isinstance(p, c_ast.ID):
                    plist.append((p.name, S32))
                    continue
                pt = self.types.build(p.type)
                if pt.kind == 'void' and not p.name:
                    continue
                if pt.kind == 'array':
                    pt = T('ptr', to=pt.to)
                plist.append((p.name, pt))
        if fd.param_decls:
            kr = {d.name: self.types.build(d.type) for d in fd.param_decls}
            plist = [(nm, kr.get(nm, S32)) for nm, _ in plist]
        for extra_name, extra_type in SIGNATURE_OVERRIDES.get(name, {}).get('extra_params', []):
            plist.append((extra_name, S32 if extra_type == 's32' else T('ptr', to=U8)))
        gd_params = []
        entry = []
        for nm, pt in plist:
            gd = g.gdname(nm)
            g.scopes[0][nm] = (gd, pt)
            gd_params.append((gd, pt))
            t = pt if pt.kind == 'int' else U32
            if not (t.kind == 'int' and t.bits == 32 and False):
                entry.append('%s = Math.%s(%s)' % (gd, READ[(t.bits, t.signed)], gd))
        warned = uninit.check(fd, STACK_LOCALS.get(name, {}).keys())
        g.guarded = {v for v, _ in warned if (name, v) not in UNINIT_FALSE_POSITIVES}
        g.ind = 1
        g.block(fd.body, new_scope=False)
        self.uninit[name] = warned
        self.uses_uninit = self.uses_uninit or g.uses_uninit
        body = g.lines
        # does control reach the end?
        last = body[-1].strip() if body else ''
        falls_off = False
        if ret.kind != 'void' and g.loop_returns_at_end:
            body.append('\treturn 0 # only reached after a refusal (the C loop above ends in a return)')
        elif ret.kind != 'void' and not last.startswith('return'):
            falls_off = True
            body.append('\treturn 0 # the C falls off the end here (v0 undefined)')
        head = 'func %s(%s) -> %s:' % (name, ', '.join('%s: int' % gd for gd, _ in gd_params),
                                       'void' if ret.kind == 'void' else 'int')
        out = [head]
        out += ['\t' + e for e in entry]
        out += ['\t' + h for h in g.hoisted]
        out += body
        if len(out) == 1:
            out.append('\tpass')
        sig = {'args': [sig_name(pt) for _, pt in plist], 'ret': sig_name(ret)}
        if name in SIGNATURE_OVERRIDES:
            sig['args_extra'] = len(SIGNATURE_OVERRIDES[name]['extra_params'])
            sig['args_extra_reason'] = SIGNATURE_OVERRIDES[name]['reason']
        if ret.kind != 'void' and not g.has_value_return:
            sig['ret_undefined'] = 'the C returns no value (v0 is stale)'
        elif ret.kind != 'void' and falls_off:
            if all(v is not None for v in g.return_values):
                sig['ret_defined'] = sorted(set(g.return_values))  # the other paths fall off the end (v0 is stale)
            else:
                sig['ret_undefined'] = 'some paths return no value and the others a computed one'
        return '\n'.join(out), sig, g

    def comment_of(self, name):
        import textwrap
        m = re.search(r'((?:/\*(?:(?!\*/).)*\*/\s*\n)+)(?:[A-Za-z_][\w \t\*]*?)\b%s\(' % re.escape(name), self.src_text, re.S)
        if not m:
            return []
        out = []
        for c in re.findall(r'/\*(.*?)\*/', m.group(1), re.S):
            c = ' '.join(c.replace(' * ', ' ').split())
            if c.startswith('FAKE') or 'FAKE:' in c or c.startswith('temp:'):
                continue  # match tricks of the C, no behaviour
            out += textwrap.wrap(c, 110)
        return out


def wrap_line(line, limit=120):
    """Break a long generated line inside its brackets (GDScript continues a line inside (), [] and {})."""
    if len(line) <= limit:
        return [line]
    indent = line[:len(line) - len(line.lstrip('\t'))]
    body = line.lstrip('\t')
    comment = ''
    if ' # ' in body and body.count('"') % 2 == 0:
        body, comment = body.split(' # ', 1)
        comment = ' # ' + comment
    m = re.match(r'(if|elif|while) (.*):$', body)
    if m:
        cond = m.group(2)
        depth = 0
        whole = cond.startswith('(')
        for i, ch in enumerate(cond):
            depth += ch in '(['
            depth -= ch in ')]'
            if depth == 0 and i < len(cond) - 1:
                whole = False
                break
        if not whole:
            body = '%s (%s):' % (m.group(1), cond)
    elif body.startswith('return ') and toplevel_has(body[7:], LOW_OPS + (' + ', ' - ')):
        body = 'return (%s)' % body[7:]
    cands = []
    depth = 0
    in_str = False
    for i, ch in enumerate(body):
        if ch == '"':
            in_str = not in_str
        if in_str:
            continue
        if ch in '([':
            depth += 1
        elif ch in ')]':
            depth -= 1
        elif depth >= 1:
            if body.startswith(', ', i):
                cands.append((i + 1, depth))
            else:
                for op in (' and ', ' or ', ' + ', ' - '):
                    if body.startswith(op, i):
                        cands.append((i, depth))
                        break
    out = []
    start = 0
    first = True
    while True:
        pad = indent if first else indent + '\t\t'
        avail = limit - len(pad.replace('\t', '    '))
        if len(body) - start <= avail:
            out.append(pad + body[start:] + comment)
            break
        ok = [c for c in cands if start < c[0] <= start + avail]
        if not ok:
            out.append(pad + body[start:] + comment)
            break
        low = min(c[1] for c in ok)
        cut = max(c[0] for c in ok if c[1] == low)
        out.append(pad + body[start:cut].rstrip())
        start = cut
        while body[start] == ' ':
            start += 1
        first = False
    return out


def summary_of(name, g):
    calls = sorted(c for c in g.callees if not c.startswith('<'))
    if calls:
        return 'Calls %s.' % ', '.join(calls)
    return 'Reads and writes memory only (no calls).'


def render(u, funcs, sigs):
    lines = []
    uses_div = any(g.uses_div for _, _, g in funcs)
    uses_uninit = any(g.uses_uninit for _, _, g in funcs)
    lines.append('extends RefCounted')
    first = u.src_text.split('\n', 1)[0]
    m = re.search(r'Original object (0x[0-9A-Fa-f]+-0x[0-9A-Fa-f]+)', first)
    rng = m.group(1) if m else '?'
    dm = re.search(r'original object \[[^\]]*\];\s*(.*?)(?:\s*Original object|\s*\*/)', first)
    desc = dm.group(1).strip(' ;.') if dm else ''
    lines.append('## Port of src/main/%s.c (original object %s, T-39312).' % (u.unit, rng))
    lines.append('## ' + (desc[:1].upper() + desc[1:] + '.' if desc else ''))
    lines.append('## Contract: wiki/engine-porting-contract.md. Tests: config/port/engine-diff/%s.json (emulator captures) and' % u.unit)
    lines.append('## tests/engine/%s_test.gd (hand checks written from the C).' % u.unit)
    lines.append('## GENERATED by tools/port/engine_gen/cgen.py (run tools/port/run_engine_gen.sh; --check fails when this file differs')
    lines.append('## from its output). Change the generator or config/port/engine-gen/, not this file.')
    lines.append('## Each function follows the C statement by statement: integer widths and K&R promotion are applied where the C applies')
    lines.append('## them (a 32-bit wrap is written out only where the value range allows an overflow), memory is read and written')
    lines.append('## with the C types, calls go through engine.call_c, loops that call out test engine.halted().')
    lines.append('')
    lines.append('const Math = preload("res://core/native_math.gd")')
    lines.append('')
    lines.append('const C_FILE: String = "src/main/%s.c"' % u.unit)
    lines.append('const C_FUNCTIONS: Array[String] = [')
    for nm, _, _ in funcs:
        lines.append('\t"%s",' % nm)
    lines.append(']')
    lines.append('const C_UNPORTED: Array[String] = [')
    done = {f[0] for f in funcs}
    for nm in u.order:
        if nm not in done:
            lines.append('\t"%s",' % nm)
    lines.append(']')
    lines.append('const C_SIGNATURES: Dictionary = {')
    for nm, _, _ in funcs:
        s = sigs[nm]
        parts = ['"args": [%s]' % ', '.join('"%s"' % x for x in s['args']), '"ret": "%s"' % s['ret']]
        if 'args_extra' in s:
            parts.append('"args_extra": %d, "args_extra_reason": "%s"' % (s['args_extra'], s['args_extra_reason']))
        if 'ret_undefined' in s:
            parts.append('"ret_undefined": "%s"' % s['ret_undefined'])
        if 'ret_defined' in s:
            parts.append('"ret_defined": [%s]' % ', '.join(str(v) for v in s['ret_defined']))
        lines.append('\t"%s": {%s},' % (nm, ', '.join(parts)))
    lines.append('}')
    lines.append('')
    lines.append('## game_state (GAME_STATE_BASE of core/game_layout.gd, config/symbol_addrs.txt).')
    lines.append('const GS: int = 0x%X' % u.layout.gs_base)
    for cname, (name, addr) in sorted(u.gconsts.items(), key=lambda kv: kv[1][1] or 0):
        a = addr if addr is not None else u.layout.globals[name][0]
        lines.append('## %s (config/symbol_addrs*.txt)' % name)
        lines.append('const %s: int = 0x%X' % (cname, a))
    lines.append('')
    lines.append('## Struct sizes and field offsets of the C types this file indexes (include/game/*.h, include/main/*.h).')
    for cname, value in sorted(u.consts.items(), key=lambda kv: (kv[1], kv[0])):
        lines.append('const %s: int = 0x%X' % (cname, value))
    lines.append('')
    lines.append('var engine # GameEngine (engine/engine.gd); set when the engine registers this module')
    lines.append('')
    if uses_div:
        lines += ['@warning_ignore("integer_division")', 'func _div(left: int, right: int) -> int:', '\tif right == 0:',
                  '\t\tengine.refuse("division", "C integer division by zero")', '\t\treturn 0',
                  '\treturn left / right # GDScript integer division truncates toward zero, as the C does', '',
                  'func _mod(left: int, right: int) -> int:', '\tif right == 0:',
                  '\t\tengine.refuse("modulo", "C integer remainder by zero")', '\t\treturn 0',
                  '\treturn left % right # the sign follows the dividend, as in C', '']
    if uses_uninit:
        lines += ['## A local the C may read before it assigns it holds stale stack bytes there; the port refuses instead of',
                  '## inventing a value (the harness shows the refusal if a capture ever takes that path).',
                  'func _uninit(function_name: String, local_name: String, value: int, is_set: bool) -> int:',
                  '\tif not is_set:',
                  '\t\tengine.refuse("uninitialised local " + local_name,',
                  '\t\t\t\t"%s reads %s before the C assigns it (stale stack bytes)" % [function_name, local_name])',
                  '\treturn value', '']
    for nm, text, g in funcs:
        import textwrap
        docs = u.comment_of(nm) or textwrap.wrap(summary_of(nm, g), 110)
        for c in docs:
            lines.append('## ' + c)
        for tl in text.split('\n'):
            lines += wrap_line(tl)
        lines.append('')
    return '\n'.join(lines)


def main():
    here = os.path.dirname(os.path.abspath(__file__))
    root = os.path.normpath(os.path.join(here, '..', '..', '..'))
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--repo', default=root)
    ap.add_argument('--unit', action='append', help='hud_panels, scene_display, scene_library (default: all three)')
    ap.add_argument('--asm', default=os.environ.get('TOKIMEMO_ASM_DIR', ''), help='asm/ directory of the user build (rodata strings)')
    ap.add_argument('--out-dir', default=None, help='default: <repo>/port/godot/engine')
    ap.add_argument('--report-dir', default=None, help='write <unit>.json (order, callees, ...) for mkspec.py and capture_cmd.py')
    ap.add_argument('--check', action='store_true', help='fail if the committed module differs from the generated one')
    a = ap.parse_args()
    out_dir = a.out_dir or os.path.join(a.repo, 'port', 'godot', 'engine')
    bad = 0
    for unit in (a.unit or ['hud_panels', 'scene_display', 'scene_library']):
        u = Unit(a.repo, unit, asm_dir=a.asm)
        u.assign_string_nodes()
        funcs, skipped, sigs, callees = [], [], {}, {}
        for nm in u.order:
            try:
                text, sig, g = u.gen_function(nm)
                funcs.append((nm, text, g))
                sigs[nm] = sig
                callees[nm] = sorted(g.callees)
            except Unsupported as e:
                skipped.append((nm, str(e)))
        if u.string_diff:
            print('%s: string literal mismatch with the rodata: %s' % (unit, u.string_diff))
            bad += 1
        text = render(u, funcs, sigs) + '\n'
        path = os.path.join(out_dir, unit + '.gd')
        if a.report_dir:
            os.makedirs(a.report_dir, exist_ok=True)
            rep = {'functions': [f[0] for f in funcs], 'skipped': skipped, 'missing': u.missing, 'callees': callees, 'order': u.order,
                   'uninit': {k: v for k, v in u.uninit.items() if v}}
            open(os.path.join(a.report_dir, unit + '.json'), 'w').write(json.dumps(rep, indent=1))
        if a.check:
            have = open(path).read() if os.path.exists(path) else ''
            if have != text:
                print('%s: %s differs from the generated module' % (unit, path))
                bad += 1
            else:
                print('%s: up to date (%d functions)' % (unit, len(funcs)))
        else:
            open(path, 'w').write(text)
            print('%s: generated %d of %d functions' % (unit, len(funcs), len(u.order)))
        for nm, why in skipped:
            print('  SKIP %s: %s' % (nm, why))
        if skipped and a.check:
            bad += 1
    sys.exit(1 if bad else 0)


if __name__ == '__main__':
    main()
