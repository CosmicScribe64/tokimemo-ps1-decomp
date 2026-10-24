#!/usr/bin/env python3
"""Unit tests for tools/ugen_pass.py (T-24092, T-26000).

The unit tests build binary ucode and binasm records by hand; no game data is
involved. IdoIntegration compiles synthetic C with the real IDO 5.3 through
tools/cc.py's shims and is skipped when IDO is not installed.

Run (in Docker): python3 tools/test_ugen_pass.py
"""
import os
import struct
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ugen_pass as up  # noqa: E402

A, I, J, L = up.DT_A, up.DT_I, up.DT_J, up.DT_L
M, R, S = up.MT_M, up.MT_R, 4


def rec(op, dtype=0, mtype=0, lexlev=0, *words):
    return up.Insn([(up.OP[op] << 24) | (mtype << 21) | (dtype << 16) | lexlev] + list(words))


def proc(blk=2, *body):
    return [rec("ent", up.DTYPES.index("P"), 0, 0, blk, 0, 1)] + list(body) + [rec("end", 0, 0, 0, blk)]


def lda(blk=5, off=0, length=68):
    return rec("lda", A, S, 0, blk, length, off, 0, 0)


def ldc(val, dtype=J):
    return rec("ldc", dtype, 0, 0, 0, 4, 0, val & 0xFFFFFFFF, 0)


def lod(dtype=J, blk=5, length=4, off=0, mtype=S):
    return rec("lod", dtype, mtype, 0, blk, length, off)


def st(dtype=J, blk=5, length=4, off=0, mtype=S):
    return rec("str", dtype, mtype, 0, blk, length, off)


def istr(dtype=J, off=0, length=4):
    return rec("istr", dtype, 0, 8, off & 0xFFFFFFFF, length, 0)


def mov(length=68, align=16):
    return rec("mov", up.DTYPES.index("M"), 0, align, 0, length, 0)


def op(name, dtype=J):
    return rec(name, dtype, 0, 0, 0)


def fjp(label=10):
    return rec("fjp", 0, 0, 0, label)


def tjp(label=10):
    return rec("tjp", 0, 0, 0, label)


def opnames(insns):
    return [up.OPS[i.opc] for i in insns]


class Records(unittest.TestCase):
    def test_roundtrip_with_string(self):
        comm = rec("comm", up.DTYPES.index("M"), 0, 0, 0, 0, 0, 5, 0)
        comm.extra = b"t.c\0\0\0\0\0"
        data = up.serialize_ucode([comm, lda(5), lda(6), mov()])
        self.assertEqual(up.serialize_ucode(up.parse_ucode(data)), data)

    def test_unknown_opcode_raises(self):
        with self.assertRaises(up.PassError):
            up.parse_ucode(bytes([250, 0, 0, 0, 0, 0, 0, 0]))

    def test_truncated_raises(self):
        with self.assertRaises(up.PassError):
            up.parse_ucode(up.serialize_ucode([lda(5)])[:-4])


class NoOpFastPath(unittest.TestCase):
    def test_procedure_without_mov_is_untouched(self):
        insns = proc(2, lod(J, 5), ldc(1), op("add"), st(J, 5))
        out, changed = up.rewrite_ucode(insns)
        self.assertFalse(changed)
        self.assertEqual(out, insns)


def reg(dtype=J, off=8):
    """A value uopt keeps in a register (`Ulod` of memory type R)."""
    return lod(dtype, 0, 4, off, R)


def cup():
    return rec("cup", 0, 0, 0, 0, 0, 0)


def lab(n=20):
    return rec("lab", 0, 0, 0, n, 0, 0)


def ujp(n=20):
    return rec("ujp", 0, 0, 0, n)


# State-op building blocks, in the shapes uopt writes for the case files in
# tools/ido_eval_cases/ugen_copy_state/ (IDO 5.3 front end and uopt, cvt pass).
IST = [reg(A), ldc(8), istr(L)]               # p[4] = 8;
GST = [ldc(1), st(J)]                          # G = 1; (direct store)
CALL = [cup()]
COPY = [lda(5), lda(6), mov()]


def br_const(cmp, c, jump=fjp, lhs=None):
    """Branch on `lhs cmp c` against a constant (ugen: beqz, blez, slti, li at)."""
    return [lhs or reg(), ldc(c), op(cmp), jump()]


def br_reg(cmp, jump=fjp):
    """Branch on a compare of two values (ugen: bne/beq of two registers, slt)."""
    return [reg(J, 8), reg(J, 12), op(cmp), jump()]


# A store loop on a variable bound as uopt unrolls it by 4: the guard `n >= 1`, the
# remainder loop and the unrolled loop, each closed by a register compare `i != end`.
STORE_LOOP = (
    br_const("geq", 1) + br_const("neq", 0) + [lab(21)] + IST + br_reg("neq", tjp)
    + br_reg("neq") + [lab(22), lab(23)] + IST + IST + IST + IST + br_reg("neq", tjp) + [lab(24)]
)

# (case file, ucode before and around the copies, IDO 4.1's state of each copy)
CASES_41 = [
    ("copy_entry", COPY, "B"),
    ("copy_after_call", CALL + COPY, "B"),
    ("copy_after_if", br_const("neq", 0) + CALL + [lab()] + COPY, "B"),
    ("copy_after_istr", IST + IST + COPY, "F"),
    ("copy_after_loop", STORE_LOOP + [lab()] + COPY, "B"),
    ("copy_after_store", STORE_LOOP + [lab()] + GST + COPY, "B"),
    ("copy_ge1", IST + br_const("geq", 1) + CALL + [lab()] + COPY, "F"),
    ("copy_gt0", IST + br_const("geq", 1) + CALL + [lab()] + COPY, "F"),
    ("copy_ge2", IST + br_const("geq", 2) + CALL + [lab()] + COPY, "F"),
    ("copy_if_then_ge2", IST + br_const("neq", 0) + CALL + [lab()] + br_const("geq", 2) + CALL + [lab()] + COPY, "F"),
    ("copy_if_then_lt8_loop", IST + br_const("neq", 0) + CALL + [lab(), lab()] + CALL + br_const("les", 8, tjp, lod(J)) + COPY, "F"),
    ("copy_lt1_loop", IST + [lab()] + CALL + br_const("les", 1, tjp, lod(J)) + COPY, "F"),
    ("copy_lt2_loop", IST + [lab()] + CALL + br_const("les", 2, tjp, lod(J)) + COPY, "F"),
    ("copy_lt8_loop", IST + [lab()] + CALL + br_const("les", 8, tjp, lod(J)) + COPY, "F"),
    ("copy_lt8_loop_then_call", IST + [lab()] + CALL + br_const("les", 8, tjp, lod(J)) + CALL + GST + COPY, "F"),
    ("copy_store_loop_then_lt8_loop", STORE_LOOP + [lab(), lab()] + CALL + br_const("les", 8, tjp, lod(J)) + COPY, "B"),
    ("copy_store_loop_then_var_loop", STORE_LOOP + [lab()] + br_const("geq", 1) + [lab()] + CALL + br_reg("neq", tjp) + [lab()] + COPY, "B"),
    ("copy_store_then_call", IST + CALL + COPY, "F"),
    ("copy_store_then_eq_reg_if", IST + [reg(), lod(J), op("equ"), fjp()] + CALL + [lab()] + COPY, "B"),
    ("copy_store_then_ge8_loop", IST + [lab()] + CALL + br_const("geq", 8, tjp, lod(J)) + COPY, "F"),
    ("copy_store_then_gstore", IST + GST + COPY, "F"),
    ("copy_store_then_if", IST + br_const("neq", 0) + CALL + [lab()] + COPY, "F"),
    ("copy_store_then_lt_reg_if", IST + [reg(), lod(J), op("les"), fjp()] + CALL + [lab()] + COPY, "B"),
    ("copy_store_then_ne3_if", IST + br_const("neq", 3) + CALL + [lab()] + COPY, "F"),
    ("copy_store_then_ne_loop", IST + [lab()] + CALL + [lod(J), reg(J, 16), op("neq"), tjp()] + COPY, "B"),
    ("copy_two_after_store", IST + COPY + COPY, "FF"),
    ("copy_ujp_then_lt8_loop", [lab()] + br_const("neq", 0) + IST + CALL + [ujp(), lab()] + GST + CALL + [lab()] + br_const("les", 8, tjp) + COPY, "F"),
    ("copy_var_loop_guard", IST + br_const("geq", 1) + [lab()] + CALL + br_reg("neq", tjp) + [lab()] + COPY, "B"),
    ("copy_while_lt8", IST + br_const("les", 8, fjp, lod(J)) + [lab()] + CALL + br_const("les", 8, tjp, lod(J)) + [lab()] + COPY, "F"),
]


def gadget_states(insns):
    """The state of each gadget rewrite_ucode put before a Umov, in order."""
    out = []
    for i in insns:
        if i.opc == up.OP["str"] and i.words[3] == up.SENTINEL_OFF:
            out.append("B")
        elif i.opc == up.OP["istr"] and i.words[1] == up.SENTINEL_OFF:
            out.append("F")
    return "".join(out)


class IdoFourOneCases(unittest.TestCase):
    """The 30 cases of tools/ido_eval_cases/ugen_copy_state/ as synthetic ucode (T-26000)."""

    def test_the_rule_gives_ido_41s_state_in_every_case(self):
        self.assertEqual(len(CASES_41) + 1, 30)  # + copy_after_proc_store, below
        for name, body, want in CASES_41:
            with self.subTest(case=name):
                insns = proc(2, *body)
                self.assertEqual("".join(up.copy_states(insns, 0, len(insns) - 1)), want)
                out, changed = up.rewrite_ucode(insns)
                self.assertTrue(changed)
                self.assertEqual(gadget_states(out), want)

    def test_copy_after_proc_store_starts_in_b(self):
        # nothing carries over from the previous procedure, even after an indirect store
        insns = proc(2, *IST) + proc(3, *COPY)
        out, _ = up.rewrite_ucode(insns)
        self.assertEqual(gadget_states(out), "B")


class StateRules(unittest.TestCase):
    def states(self, *body):
        insns = proc(2, *body)
        return "".join(up.copy_states(insns, 0, len(insns) - 1))

    def test_entry_mov_gets_state_b_gadget(self):
        insns = proc(2, lda(5), lda(6), mov())
        out, changed = up.rewrite_ucode(insns)
        self.assertTrue(changed)
        # _gadget_b is [ldc, str] inserted right before the first lda
        self.assertEqual(opnames(out), ["ent", "ldc", "str", "lda", "lda", "mov", "end"])
        self.assertEqual(out[2].mtype, M)
        self.assertEqual(out[2].words[1], 2)
        self.assertEqual(out[2].words[3], up.SENTINEL_OFF)

    def test_mov_immediately_after_istr_gets_state_f_gadget(self):
        insns = proc(2, lod(A, 5), ldc(1), istr(J, 0, 4), lda(5), lda(6), mov())
        out, changed = up.rewrite_ucode(insns)
        self.assertTrue(changed)
        # _gadget_f is [ldc, ldc, istr] inserted right before the first lda
        self.assertEqual(
            opnames(out),
            ["ent", "lod", "ldc", "istr", "ldc", "ldc", "istr", "lda", "lda", "mov", "end"],
        )

    def test_mov_with_unal_places_gadget_before_unal(self):
        insns = proc(2, rec("unal", 0, 0, 16), lda(5), lda(6), mov())
        out, changed = up.rewrite_ucode(insns)
        self.assertTrue(changed)
        self.assertEqual(opnames(out), ["ent", "ldc", "str", "unal", "lda", "lda", "mov", "end"])

    def test_istv_sets_f(self):
        self.assertEqual(self.states(reg(A), ldc(1), rec("istv", J, 0, 8, 0, 4, 0), *COPY), "F")

    def test_register_compare_sets_b_for_every_compare(self):
        for cmp in ("equ", "neq", "les", "leq", "grt", "geq"):
            for jump in (fjp, tjp):
                with self.subTest(cmp=cmp, jump=jump.__name__):
                    self.assertEqual(self.states(*IST + br_reg(cmp, jump) + COPY), "B")

    def test_constant_compare_keeps_the_state(self):
        for cmp in ("equ", "neq", "les", "leq", "grt", "geq"):
            with self.subTest(cmp=cmp):
                self.assertEqual(self.states(*IST + br_const(cmp, 5) + COPY), "F")
                self.assertEqual(self.states(*br_const(cmp, 5) + COPY), "B")

    def test_constant_on_the_left_counts_as_a_register_compare(self):
        # uopt writes `x != 0` as `ldc 0; x; neq` for some register values (main strCallback,
        # func_8005938C); IDO 4.1's ugen sets B there although it emits beqz.
        self.assertEqual(self.states(*IST + [ldc(0), reg(), op("neq"), fjp()] + COPY), "B")

    def test_compare_under_lnot_and_rstr_is_still_read(self):
        lnot = op("lnot")
        rstr = rec("rstr", J, R, 0, 0, 4, 8)
        self.assertEqual(self.states(*IST + [reg(), reg(J, 12), op("neq"), lnot, fjp()] + COPY), "B")
        self.assertEqual(self.states(*IST + [reg(), reg(J, 12), op("neq"), rstr, fjp()] + COPY), "B")
        self.assertEqual(self.states(*IST + [reg(), ldc(0), op("neq"), rstr, fjp()] + COPY), "F")

    def test_branch_on_a_value_keeps_the_state(self):
        # IDO 4.1 sets F here (T-26000 probes); the pass keeps the state, see the docstring.
        self.assertEqual(self.states(*[lod(J), ldc(1), op("and"), fjp()] + COPY), "B")
        self.assertEqual(self.states(*IST + [reg(), fjp()] + COPY), "F")

    def test_state_carries_across_copies_and_resets_per_procedure(self):
        insns = proc(2, *IST + COPY + br_reg("neq") + COPY + IST + COPY) + proc(3, *COPY)
        out, _ = up.rewrite_ucode(insns)
        self.assertEqual(gadget_states(out), "FBFB")

    def test_compare_without_operands_raises(self):
        insns = proc(2, op("neq"), fjp(), *COPY)
        with self.assertRaises(up.PassError):
            up.copy_states(insns, 0, len(insns) - 1)

    def test_branch_without_condition_raises(self):
        for body in ([fjp()], [ldc(1), st(J), tjp()]):
            insns = proc(2, *body + COPY)
            with self.subTest(body=opnames(body)), self.assertRaises(up.PassError):
                up.copy_states(insns, 0, len(insns) - 1)

    def test_undelimited_mov_operand_raises(self):
        with self.assertRaises(up.PassError):
            up.rewrite_ucode(proc(2, rec("dup", A), mov()))


class SentinelBinasm(unittest.TestCase):
    def _binasm_rec(self, instr, op=0, form=0, reg1=0, reg2=0, imm=0):
        w1 = (instr << 16) | (op << 1)
        w2 = (reg1 << 25) | (reg2 << 18) | (form << 14)
        return struct.pack(">4I", 0, w1, w2, imm & 0xFFFFFFFF)

    def test_strips_sp_and_zero_sentinels_and_keeps_normal_records(self):
        normal_sb = self._binasm_rec(up.I_IOCODE, up.Z_ZSB, up.F_FROB, reg1=2, reg2=up.SP, imm=16)
        sent_b = self._binasm_rec(up.I_IOCODE, up.Z_ZSB, up.F_FROB, reg1=0, reg2=up.SP, imm=-0x7777 + 64)
        sent_f = self._binasm_rec(up.I_IOCODE, up.Z_ZSB, up.F_FROB, reg1=0, reg2=0, imm=-0x7777)
        ascii_rec = struct.pack(">4I", 0, up.I_IASCII << 16, 20, 0) + b"x" * 32
        stream = normal_sb + sent_b + ascii_rec + sent_f
        stripped = up.strip_sentinel_binasm(stream)
        self.assertEqual(stripped, normal_sb + ascii_rec)

    def test_missing_sentinel_raises(self):
        normal_sb = self._binasm_rec(up.I_IOCODE, up.Z_ZSB, up.F_FROB, reg1=2, reg2=up.SP, imm=16)
        with self.assertRaises(up.PassError):
            up.strip_sentinel_binasm(normal_sb)


@unittest.skipUnless(os.path.exists("/opt/ido/5.3/cc"), "IDO 5.3 not installed")
class IdoIntegration(unittest.TestCase):
    def compile(self, text, with_pass=True):
        import shutil
        import subprocess
        import tempfile
        import cc

        tmp = tempfile.mkdtemp(prefix="ugentest")
        lib = os.path.join(tmp, "lib")
        os.mkdir(lib)
        try:
            src = os.path.join(tmp, "t.c")
            with open(src, "w") as f:
                f.write(text)
            env = cc.ido_frame_env("5.3", lib, {"ugen": cc.UGEN_PASS if with_pass else None})
            flags = [a for a in cc.IDO_CFLAGS if a != "-Iinclude"]
            subprocess.run(
                ["/opt/ido/5.3/cc"] + flags + ["-o", os.path.join(tmp, "t.o"), src],
                check=True,
                env=env,
            )
            return subprocess.run(
                ["mips-linux-gnu-objdump", "-dr", "--no-show-raw-insn", os.path.join(tmp, "t.o")],
                check=True,
                stdout=subprocess.PIPE,
                text=True,
            ).stdout
        finally:
            shutil.rmtree(tmp)

    def test_copy_after_if_uses_state_b(self):
        src = """typedef struct { short a[22]; } S44;
extern S44 g_dst, g_src; extern int g_cond;
void test(void) { if (g_cond == 0) { g_dst = g_src; } }
"""
        dis_pass = self.compile(src, with_pass=True)
        dis_stock = self.compile(src, with_pass=False)
        self.assertIn("bne\tt8,t1,", dis_pass)
        self.assertIn("bne\tt8,t0,", dis_stock)
        self.assertNotIn("sb\tzero,", dis_pass)

    def test_copy_after_istr_uses_state_f(self):
        src = """typedef struct { short a[22]; } S44;
extern S44 g_dst, g_src;
void test(int *p) { *p = 1; g_dst = g_src; }
"""
        dis_pass = self.compile(src, with_pass=True)
        self.assertIn("bne\tt8,t0,", dis_pass)
        self.assertNotIn("sb\tzero,", dis_pass)

    def test_copy_after_uist_counted_for_loop_uses_state_f(self):
        src = """typedef struct { short a[22]; } S44;
extern S44 g_dst, g_src; extern int g_arr[20], g_step;
void test(void) { int i; for (i = 0; i < 20; i += g_step) { g_arr[i] = i; } g_dst = g_src; }
"""
        dis_pass = self.compile(src, with_pass=True)
        dis_stock = self.compile(src, with_pass=False)
        self.assertEqual(dis_pass.splitlines()[2:], dis_stock.splitlines()[2:])
        self.assertNotIn("sb\tzero,", dis_pass)


if __name__ == "__main__":
    unittest.main()
