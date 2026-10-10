#!/usr/bin/env python3
"""Unit tests for tools/cvt_pass.py (T-1321).

The unit tests build binary ucode records by hand (layout in cvt_pass.py); no
game data is involved. IdoIntegration compiles synthetic C with the real IDO
5.3 through tools/cc.py's shims and is skipped when IDO is not installed.

Run (in Docker): python3 tools/test_cvt_pass.py
"""
import os
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cvt_pass as cp  # noqa: E402

J, L, S, M = cp.DT_J, cp.DT_L, cp.MT_S, 1


def rec(op, dtype=0, mtype=0, lexlev=0, *words):
    return cp.Insn([(cp.OP[op] << 24) | (mtype << 21) | (dtype << 16) | lexlev] + list(words))


def lod(dtype, blk, length=1, off=0, mtype=S, lexlev=0):
    return rec("lod", dtype, mtype, lexlev, blk, length, off)


def st(dtype, blk, length=1, off=0, mtype=S):
    return rec("str", dtype, mtype, 0, blk, length, off)


def widen():
    return rec("cvt", J, 0, 0, 0, L << 24, 0)


def ldc(value):
    return rec("ldc", J, 0, 0, 0, 4, 0, 0, value)


def op(name):
    return rec(name, J, 0, 0, 0)


def fjp(label=7):
    return rec("fjp", 0, 0, 0, label)


def names(insns):
    out = []
    for i in insns:
        n = cp.OPS[i.opc]
        if n in ("lod", "rlod"):
            n += "%s%d" % (cp.DTYPES[i.dtype], i.words[1])
        out.append(n)
    return out


def run_pre(insns):
    return cp.pre(cp.parse(cp.serialize(insns)))


class Records(unittest.TestCase):
    def test_roundtrip_with_string(self):
        comm = rec("comm", cp.DTYPES.index("M"), 0, 0, 0, 0, 0, 5, 0)
        comm.extra = b"t.c\0\0\0\0\0"
        data = cp.serialize([comm, lod(L, 6), widen(), op("equ")])
        self.assertEqual(cp.serialize(cp.parse(data)), data)

    def test_unknown_opcode(self):
        with self.assertRaises(cp.PassError):
            cp.parse(bytes([250, 0, 0, 0, 0, 0, 0, 0]))

    def test_truncated(self):
        with self.assertRaises(cp.PassError):
            cp.parse(cp.serialize([lod(L, 6)])[:-4])


class Strip(unittest.TestCase):
    def test_unsigned_byte_global(self):
        out, locs = run_pre([lod(L, 6), widen(), ldc(1), op("equ"), fjp()])
        self.assertEqual(names(out), ["lodL6", "ldc", "equ", "fjp"])
        self.assertEqual(locs, {(S, 6, 0, 1)})

    def test_halfword(self):
        out, _ = run_pre([lod(L, 6, 2), widen(), ldc(1), op("equ"), fjp()])
        self.assertEqual(names(out), ["lodL6", "ldc", "equ", "fjp"])

    def test_word_untouched(self):
        insns = [lod(L, 6, 4), widen(), ldc(1), op("equ"), fjp()]
        out, locs = run_pre(insns)
        self.assertEqual(names(out), names(insns))
        self.assertEqual(locs, set())

    def test_signed_access_elsewhere(self):
        insns = [lod(L, 6), widen(), ldc(1), op("equ"), fjp(), lod(J, 6), st(L, 9)]
        out, _ = run_pre(insns)
        self.assertEqual(names(out), names(insns))

    def test_local_untouched(self):
        insns = [lod(L, 3, mtype=M), widen(), ldc(1), op("equ"), fjp()]
        out, _ = run_pre(insns)
        self.assertEqual(names(out), names(insns))

    def test_store_of_same_type_allowed(self):
        out, _ = run_pre([lod(L, 6), widen(), ldc(1), op("add"), rec("cvt", L, 0, 0, 0, J << 24, 0), st(L, 6)])
        self.assertEqual(names(out)[:3], ["lodL6", "ldc", "add"])


class EqualityOrder(unittest.TestCase):
    """uopt puts a plain variable first in ==/!= unless the left operand already is one."""

    def test_variable_against_widened_global(self):
        # cfe order: (cvt(D), t). With the CVT uopt makes it (t, cvt(D)); without it it
        # would keep (D, t), so the pass hands uopt (t, D).
        out, _ = run_pre([lod(L, 6), widen(), lod(J, 3, 4, mtype=M), op("equ"), fjp()])
        self.assertEqual(names(out), ["lodJ3", "lodL6", "equ", "fjp"])

    def test_expression_before_global_keeps_cvt(self):
        # With the CVT the result is (x & 15, cvt(D)); no input order gives
        # (expression, plain variable), so this CVT stays.
        insns = [lod(L, 6), widen(), lod(J, 8, 4), ldc(15), op("and"), op("neq"), fjp()]
        out, locs = run_pre(insns)
        self.assertEqual(names(out), names(insns))
        self.assertEqual(locs, set())

    def test_expression_first_in_source(self):
        # (x & 15, cvt(D)): uopt swaps with the CVT -> (cvt(D), x & 15); the pass hands
        # uopt (D, x & 15), which it keeps.
        out, _ = run_pre([lod(J, 8, 4), ldc(15), op("and"), lod(L, 6), widen(), op("equ"), fjp()])
        self.assertEqual(names(out), ["lodL6", "lodJ8", "ldc", "and", "equ", "fjp"])

    def test_two_widened_globals(self):
        # (cvt(A), cvt(B)) -> uopt with CVTs: (cvt(B), cvt(A)); pass hands (B, A).
        out, _ = run_pre([lod(L, 6), widen(), lod(L, 7), widen(), op("equ"), fjp()])
        self.assertEqual(names(out), ["lodL7", "lodL6", "equ", "fjp"])

    def test_constant_operand_not_reordered(self):
        out, _ = run_pre([ldc(1), lod(L, 6), widen(), op("equ"), fjp()])
        self.assertEqual(names(out), ["ldc", "lodL6", "equ", "fjp"])

    def test_undelimited_operand_fails(self):
        with self.assertRaises(cp.PassError):
            run_pre([lod(L, 6), widen(), rec("dup", J), op("equ"), fjp()])


class Post(unittest.TestCase):
    def test_loads_get_unsigned_type_back(self):
        insns = [lod(J, 6), rec("rlod", J, S, 0, 6, 1, 0), lod(J, 7), lod(J, 6, 2)]
        cp.post(insns, {(S, 6, 0, 1)})
        self.assertEqual(names(insns), ["lodL6", "rlodL6", "lodJ7", "lodJ6"])


@unittest.skipUnless(os.path.exists("/opt/ido/5.3/cc"), "IDO 5.3 not installed")
class IdoIntegration(unittest.TestCase):
    """Compile synthetic C with the real IDO 5.3 through tools/cc.py's shims."""

    SOURCE = """extern unsigned char D; extern int h(void); extern int g(void);
void f(void) { if (D == 0) { if (h() == 1) { g(); D++; } } else if (D == 1) { g(); D++; } }
"""

    def compile(self, text, with_pass=True):
        import shutil
        import subprocess
        import tempfile
        import cc
        tmp = tempfile.mkdtemp(prefix="cvtest")
        lib = os.path.join(tmp, "lib")
        os.mkdir(lib)
        try:
            src = os.path.join(tmp, "t.c")
            with open(src, "w") as f:
                f.write(text)
            env = cc.ido_frame_env("5.3", lib)
            if not with_pass:
                os.unlink(os.path.join(lib, "uopt"))
                os.symlink("/opt/ido/5.3/uopt", os.path.join(lib, "uopt"))
            flags = [a for a in cc.IDO_CFLAGS if a != "-Iinclude"]
            subprocess.run(["/opt/ido/5.3/cc"] + flags + ["-o", os.path.join(tmp, "t.o"), src],
                           check=True, env=env)
            return subprocess.run(["mips-linux-gnu-objdump", "-d", os.path.join(tmp, "t.o")],
                                  check=True, stdout=subprocess.PIPE, text=True).stdout
        finally:
            shutil.rmtree(tmp)

    def test_global_kept_in_one_register(self):
        dis = self.compile(self.SOURCE)
        # every load of D goes to $v1 (entry and after the calls), always lbu
        self.assertEqual(dis.count("lbu\tv1,0(v1)"), 3, dis)
        self.assertNotIn("lb\t", dis)

    def test_without_pass_ido_differs(self):
        dis = self.compile(self.SOURCE, with_pass=False)
        self.assertIn("lbu\tv0,0(v0)", dis)


if __name__ == "__main__":
    unittest.main()
