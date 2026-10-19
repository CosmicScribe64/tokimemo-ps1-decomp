#!/usr/bin/env python3
"""Unit tests for tools/ugen_pass.py (T-24092).

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


class StateRules(unittest.TestCase):
    def test_entry_mov_gets_state_b_gadget(self):
        insns = proc(2, lda(5), lda(6), mov())
        out, changed = up.rewrite_ucode(insns)
        self.assertTrue(changed)
        # _gadget_b is [ldc, str] inserted right before the first lda
        self.assertEqual(opnames(out), ["ent", "ldc", "str", "lda", "lda", "mov", "end"])
        self.assertEqual(out[2].mtype, M)
        self.assertEqual(out[2].words[1], 2)
        self.assertEqual(out[2].words[3], up.SENTINEL_OFF)

    def test_mov_after_if_without_uist_gets_state_b_gadget(self):
        insns = proc(2, lod(J, 5), ldc(0), op("equ"), fjp(12), lda(5), lda(6), mov())
        out, changed = up.rewrite_ucode(insns)
        self.assertTrue(changed)
        self.assertEqual(
            opnames(out),
            ["ent", "lod", "ldc", "equ", "fjp", "ldc", "str", "lda", "lda", "mov", "end"],
        )

    def test_mov_with_unal_places_gadget_before_unal(self):
        insns = proc(2, rec("unal", 0, 0, 16), lda(5), lda(6), mov())
        out, changed = up.rewrite_ucode(insns)
        self.assertTrue(changed)
        self.assertEqual(opnames(out), ["ent", "ldc", "str", "unal", "lda", "lda", "mov", "end"])

    def test_mov_immediately_after_istr_gets_state_f_gadget(self):
        insns = proc(2, lod(A, 5), ldc(1), istr(J, 0, 4), lda(5), lda(6), mov())
        out, changed = up.rewrite_ucode(insns)
        self.assertTrue(changed)
        # _gadget_f is [ldc, ldc, istr] inserted right before the first lda
        self.assertEqual(
            opnames(out),
            ["ent", "lod", "ldc", "istr", "ldc", "ldc", "istr", "lda", "lda", "mov", "end"],
        )

    def test_mov_after_istr_then_direct_str_reverts_to_state_b(self):
        insns = proc(
            2,
            lod(A, 5), ldc(1), istr(J, 0, 4),
            ldc(0), st(J, 6),
            lda(5), lda(6), mov(),
        )
        out, changed = up.rewrite_ucode(insns)
        self.assertTrue(changed)
        self.assertEqual(
            opnames(out),
            ["ent", "lod", "ldc", "istr", "ldc", "str", "ldc", "str", "lda", "lda", "mov", "end"],
        )

    def test_mov_after_uist_and_Variable_neq_loop_gets_state_b(self):
        insns = proc(
            2,
            lod(A, 5), ldc(1), istr(J, 0, 4),
            lod(J, 7), lod(J, 8), op("neq"), tjp(9),
            lda(5), lda(6), mov(),
        )
        out, changed = up.rewrite_ucode(insns)
        self.assertTrue(changed)
        self.assertEqual(opnames(out)[-6:], ["ldc", "str", "lda", "lda", "mov", "end"])

    def test_uist_plus_signed_blt_pos_imm_loop_locks_state_f(self):
        # for (i = 0; i < 6; i++) { *p++ = x; } followed by direct store and mov -> State F
        insns = proc(
            2,
            lod(A, 5), ldc(1), istr(J, 0, 4),
            lod(J, 7), ldc(6, J), op("les", J), tjp(9),
            ldc(0), st(J, 6),
            lda(5), lda(6), mov(),
        )
        out, changed = up.rewrite_ucode(insns)
        self.assertTrue(changed)
        self.assertEqual(opnames(out)[-7:], ["ldc", "ldc", "istr", "lda", "lda", "mov", "end"])

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
