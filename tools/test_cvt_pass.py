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
        # a store of the same unsigned type does not disqualify the variable (its compare is rewritten)
        out, _ = run_pre([lod(L, 6), widen(), ldc(1), op("equ"), fjp(), lod(L, 6), rec("inc", L, 0, 0, 1), st(L, 6)])
        self.assertEqual(names(out)[:3], ["lodL6", "ldc", "equ"])


def cup():
    return rec("cup", cp.DTYPES.index("P"), 0, 0, 7, 0, 0)


class EntryRule(unittest.TestCase):
    """T-5010: only a variable that the procedure touches before its first call, branch or label is
    rewritten (the original's $v0 switches and compare chains follow a call or a branch)."""

    def test_after_a_call_keeps_cvt(self):
        insns = proc(cup(), lod(L, 6), widen(), ldc(1), op("equ"), fjp())
        out, locs = run_pre(insns)
        self.assertEqual(names(out), names(insns))
        self.assertEqual(locs, set())

    def test_after_a_branch_keeps_cvt(self):
        insns = proc(lod(J, 8, 4), fjp(), lod(L, 6), widen(), ldc(1), op("equ"), fjp())
        out, _ = run_pre(insns)
        self.assertEqual(names(out), names(insns))

    def test_entry_reference_covers_later_loads(self):
        # first compare at entry; the compare after the call is rewritten too
        out, _ = run_pre(proc(lod(L, 6), widen(), ldc(0), op("equ"), fjp(), cup(),
                              lod(L, 6), widen(), ldc(1), op("equ"), fjp()))
        self.assertEqual(names(out).count("cvt"), 0)

    def test_per_procedure(self):
        out, _ = run_pre(proc(lod(L, 6), widen(), ldc(0), op("equ"), fjp())
                         + proc(cup(), lod(L, 6), widen(), ldc(0), op("equ"), fjp()))
        self.assertEqual(names(out).count("cvt"), 1)


class CompareRule(unittest.TestCase):
    """T-5010: the widening is removed only where the value is compared or switched on."""

    def test_assignment_keeps_cvt(self):
        insns = [lod(L, 6), widen(), st(J, 3, 4, (-4) & 0xFFFFFFFF, mtype=M)]
        out, locs = run_pre(insns)
        self.assertEqual(names(out), names(insns))
        self.assertEqual(locs, set())

    def test_arithmetic_keeps_cvt(self):
        insns = [lod(L, 6), widen(), ldc(25), op("mpy"), st(J, 9, 4)]
        out, _ = run_pre(insns)
        self.assertEqual(names(out), names(insns))

    def test_compare_after_other_operand(self):
        # the compare consumes the value although the other operand is pushed in between
        out, _ = run_pre([lod(L, 6), widen(), lod(J, 8, 4), op("neq"), fjp()])
        self.assertNotIn("cvt", names(out))

    def test_switch_temporary(self):
        out, _ = run_pre([lod(L, 6), widen(), vreg(), st(J, 3, 4, (-4) & 0xFFFFFFFF, mtype=M)])
        self.assertNotIn("cvt", names(out))


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


def vreg(blk=3, off=-4):
    return rec("vreg", J, 0, 0, blk, 4, off & 0xFFFFFFFF)


def proc(*body):
    return [rec("ent", cp.DTYPES.index("P"), 0, 0, 3, 0, 1)] + list(body) + [rec("end", 0, 0, 0, 3)]


def zcopy(insns):
    return [i.words[2] for i in insns if cp.OPS[i.opc] == "optn" and i.words[1] == cp.UCO_ZCOPY]


class SwitchTemporaries(unittest.TestCase):
    def temp_store(self, load, widen_it=True):
        return [load] + ([widen()] if widen_it else []) + [vreg(), st(J, 3, 4, (-4) & 0xFFFFFFFF, mtype=M)]

    def test_unsigned_global_turns_copy_propagation_off(self):
        out = cp.copy_propagation_options(proc(*self.temp_store(lod(L, 6))))
        self.assertEqual(zcopy(out), [0])
        self.assertEqual(cp.OPS[out[0].opc], "optn")

    def test_unwidened_unsigned_word(self):
        out = cp.copy_propagation_options(proc(*self.temp_store(lod(L, 6, 4), widen_it=False)))
        self.assertEqual(zcopy(out), [0])

    def test_signed_global_keeps_it(self):
        out = cp.copy_propagation_options(proc(*self.temp_store(lod(J, 6), widen_it=False)))
        self.assertEqual(zcopy(out), [1])

    def test_local_copy_keeps_it(self):
        out = cp.copy_propagation_options(proc(*self.temp_store(lod(L, 3, off=8, mtype=M))))
        self.assertEqual(zcopy(out), [1])

    def test_store_to_a_user_local_keeps_it(self):
        out = cp.copy_propagation_options(proc(lod(L, 6), widen(), st(J, 3, 4, (-4) & 0xFFFFFFFF, mtype=M)))
        self.assertEqual(zcopy(out), [1])

    def test_switch_after_a_call_keeps_it(self):
        out = cp.copy_propagation_options(proc(cup(), *self.temp_store(lod(L, 6))))
        self.assertEqual(zcopy(out), [1])

    def test_switch_after_a_branch_keeps_it(self):
        out = cp.copy_propagation_options(proc(lod(J, 8, 4), fjp(), *self.temp_store(lod(L, 6))))
        self.assertEqual(zcopy(out), [1])

    def test_one_record_per_procedure(self):
        out = cp.copy_propagation_options(proc(*self.temp_store(lod(L, 6))) + proc(lod(J, 6), st(J, 7)))
        self.assertEqual(zcopy(out), [0, 1])


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
            env = cc.ido_frame_env("5.3", lib, {"uopt": cc.CVT_PASS if with_pass else None})
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

    def test_switch_on_unsigned_global(self):
        dis = self.compile("""extern unsigned char D; extern void g0(void), g1(void);
void f(void) { switch (D) { case 0: g0(); break; case 1: g1(); break; } }
""")
        self.assertIn("lbu\tv1,0(v1)", dis)

    def test_switch_on_local_copy(self):
        dis = self.compile("""extern unsigned char D; extern void g0(void), g1(void);
void f(void) { unsigned char m = D; switch (m) { case 0: g0(); break; case 1: g1(); break; } }
""")
        self.assertIn("lbu\tv0,0(v0)", dis)

    def test_switch_after_a_call(self):
        dis = self.compile("""extern unsigned char D; extern void g0(void), g1(void), h(void);
void f(void) { h(); switch (D) { case 0: g0(); break; case 1: g1(); break; } }
""")
        self.assertIn("lbu\tv0,0(v0)", dis)

    def test_copied_value_not_promoted(self):
        # assignments and call arguments keep the CVT: base register $v0, value in $a0
        dis = self.compile("""extern unsigned char D, E, F; extern void g(int);
void f(void) { E = D; F = D; g(D); }
""")
        self.assertIn("lbu\ta0,0(v0)", dis)

    def test_without_pass_ido_differs(self):
        dis = self.compile(self.SOURCE, with_pass=False)
        self.assertIn("lbu\tv0,0(v0)", dis)


if __name__ == "__main__":
    unittest.main()
