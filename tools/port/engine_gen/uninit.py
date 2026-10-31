"""Definite-assignment check: reports local scalars that may be read before they are written (the C then reads stale
stack bytes; the port starts every local at 0, which would invent a value)."""
from pycparser import c_ast

ALL = None  # "unreachable": every variable counts as assigned


def inter(a, b):
    if a is ALL:
        return b
    if b is ALL:
        return a
    return a & b


class Checker:
    def __init__(self, params, stack_names):
        self.locals = {}
        self.params = set(params)
        self.stack = set(stack_names)
        self.warn = []

    def use(self, name, st, where):
        if name in self.locals and st is not ALL and name not in st and name not in self.params and name not in self.stack:
            self.warn.append((name, where))

    def expr(self, n, st):
        """returns the assigned set after n."""
        if n is None or st is ALL:
            return st
        if isinstance(n, c_ast.ID):
            self.use(n.name, st, 'read')
            return st
        if isinstance(n, c_ast.Assignment):
            if isinstance(n.lvalue, c_ast.ID):
                if n.op != '=':
                    self.use(n.lvalue.name, st, 'compound')
                st = self.expr(n.rvalue, st)
                if st is not ALL:
                    st = st | {n.lvalue.name}
                return st
            st = self.expr(n.lvalue, st)
            return self.expr(n.rvalue, st)
        if isinstance(n, c_ast.BinaryOp) and n.op in ('&&', '||'):
            st = self.expr(n.left, st)
            self.expr(n.right, st)
            return st
        if isinstance(n, c_ast.TernaryOp):
            st = self.expr(n.cond, st)
            a = self.expr(n.iftrue, st)
            b = self.expr(n.iffalse, st)
            return inter(a, b)
        if isinstance(n, c_ast.UnaryOp) and n.op == '&' and isinstance(n.expr, c_ast.ID):
            return st
        if isinstance(n, c_ast.StructRef):
            return self.expr(n.name, st)  # the field name is not a variable
        if isinstance(n, c_ast.FuncCall):
            st = self.expr(n.args, st) if n.args is not None else st
            return self.expr(n.name, st) if not isinstance(n.name, c_ast.ID) else st
        for _, ch in n.children():
            st = self.expr(ch, st)
        return st

    def stmts(self, items, st):
        for it in items:
            if st is ALL and isinstance(it, c_ast.Label):
                st = set()  # a label after a return/goto is reachable by a goto: nothing is known to be assigned there
            st = self.stmt(it, st)
        return st

    def stmt(self, n, st):
        if st is ALL:
            if isinstance(n, c_ast.Label):
                return self.stmt(n.stmt, set())
            return ALL
        if isinstance(n, c_ast.Compound):
            return self.stmts(n.block_items or [], st)
        if isinstance(n, c_ast.Decl):
            if isinstance(n.type, c_ast.TypeDecl) or isinstance(n.type, c_ast.PtrDecl):
                self.locals[n.name] = True
            if n.init is not None:
                st = self.expr(n.init, st)
                st = st | {n.name}
            return st
        if isinstance(n, c_ast.Return):
            self.expr(n.expr, st)
            return ALL
        if isinstance(n, (c_ast.Break, c_ast.Continue, c_ast.Goto)):
            return ALL
        if isinstance(n, c_ast.If):
            st = self.expr(n.cond, st)
            a = self.stmt(n.iftrue, st)
            b = self.stmt(n.iffalse, st) if n.iffalse is not None else st
            return inter(a, b)
        if isinstance(n, c_ast.While):
            st = self.expr(n.cond, st)
            self.stmt(n.stmt, st)
            return st
        if isinstance(n, c_ast.DoWhile):
            st = self.stmt(n.stmt, st)
            st = self.expr(n.cond, st if st is not ALL else ALL)
            return st if st is not ALL else ALL
        if isinstance(n, c_ast.For):
            if isinstance(n.init, c_ast.DeclList):
                for d in n.init.decls:
                    st = self.stmt(d, st)
            else:
                st = self.expr(n.init, st)
            st = self.expr(n.cond, st)
            body = self.stmt(n.stmt, st)
            if body is not ALL:
                self.expr(n.next, body)
            return st
        if isinstance(n, c_ast.Switch):
            st = self.expr(n.cond, st)
            res = None
            for it in (n.stmt.block_items or []):
                end = self.stmts(it.stmts or [], st)
                # break at the end: the arm's set flows out; ALL (return) does not
            return st
        if isinstance(n, (c_ast.Case, c_ast.Default)):
            return self.stmts(n.stmts or [], st)
        if isinstance(n, c_ast.Label):
            return self.stmt(n.stmt, st if st is not ALL else set())
        if isinstance(n, c_ast.EmptyStatement):
            return st
        return self.expr(n, st)


def check(fd, stack_names):
    params = []
    if fd.decl.type.args:
        for p in fd.decl.type.args.params:
            if hasattr(p, 'name') and p.name:
                params.append(p.name)
    ck = Checker(params, stack_names)
    ck.stmt(fd.body, set())
    seen = []
    for w in ck.warn:
        if w not in seen:
            seen.append(w)
    return seen
