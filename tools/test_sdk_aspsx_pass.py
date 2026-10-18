#!/usr/bin/env python3
"""Unit tests for tools/sdk_aspsx_pass.py (T-24086), using synthetic assembly and GCC compiles.

Run: python3 -m unittest discover -s tools -p test_sdk_aspsx_pass.py -v
"""
import os
import subprocess
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cc  # noqa: E402
import sdk_aspsx_pass  # noqa: E402


class IndexedSymbolLoads(unittest.TestCase):
    """Rule 1: `l* rd, sym(rs)` expands through `rd` when `rd != rs`, and stays `$at` when `rd == rs` or storing."""

    def test_load_with_distinct_rd_uses_rd_instead_of_at(self):
        maspsx_s = (
            "# EXPAND_AT START\n"
            ".set\tnoat\n"
            "\tlui\t$at,%hi(sym_table)\n"
            "\taddu\t$at,$at,$2\n"
            "\tlh\t$3,%lo(sym_table)($at)\n"
            ".set\tat\n"
            "# EXPAND_AT END\n"
        )
        got = sdk_aspsx_pass.rewrite(maspsx_s)
        self.assertIn("\tlui\t$3,%hi(sym_table)\n\taddu\t$3,$3,$2\n\tlh\t$3,%lo(sym_table)($3)\n", got)
        self.assertNotIn("$at", got)

    def test_load_with_rd_equal_to_rs_keeps_at(self):
        maspsx_s = (
            ".set\tnoat\n"
            "\tlui\t$at,%hi(sym_table)\n"
            "\taddu\t$at,$at,$2\n"
            "\tlw\t$2,%lo(sym_table)($at)\n"
            ".set\tat\n"
        )
        self.assertEqual(sdk_aspsx_pass.rewrite(maspsx_s), maspsx_s)

    def test_store_always_keeps_at(self):
        maspsx_s = (
            ".set\tnoat\n"
            "\tlui\t$at,%hi(sym_table)\n"
            "\taddu\t$at,$at,$2\n"
            "\tsh\t$3,%lo(sym_table)($at)\n"
            ".set\tat\n"
        )
        self.assertEqual(sdk_aspsx_pass.rewrite(maspsx_s), maspsx_s)


class ExpandedDivMovePlacement(unittest.TestCase):
    """Rule 2: `mflo`/`mfhi` goes before `bnez` when `rd` is not read by the check sequence, and after when it is."""

    def test_signed_div_with_distinct_rd_moves_mflo_before_bnez(self):
        maspsx_s = (
            ".set\tnoat\n"
            "\tdiv\t$zero,$2,$5\n"
            "\tbnez\t$5,.L_NOT_DIV_BY_ZERO_1\n"
            "\tnop\n"
            "\tbreak\t0x7\n"
            ".L_NOT_DIV_BY_ZERO_1:\n"
            "\taddiu\t$at,$zero,-1\n"
            "\tbne\t$5,$at,.L_DIV_BY_POSITIVE_SIGN_1\n"
            "\tlui\t$at,0x8000\n"
            "\tbne\t$2,$at,.L_DIV_BY_POSITIVE_SIGN_1\n"
            "\tnop\n"
            "\tbreak\t0x6\n"
            ".L_DIV_BY_POSITIVE_SIGN_1:\n"
            "\tmflo\t$4\n"
            ".set\tat\n"
            "\tnop # DEBUG: Reuse of '$4'. 'sll\t$2,$4,1' does not use $at\n"
            "\tsll\t$2,$4,1\n"
            "$L15:\n"
        )
        got = sdk_aspsx_pass.rewrite(maspsx_s)
        lines = [line.strip() for line in got.splitlines() if line.strip() and not line.strip().startswith(".")]
        self.assertEqual(lines[:3], ["div\t$zero,$2,$5", "mflo\t$4", "bnez\t$5,.L_NOT_DIV_BY_ZERO_1"])
        # Early mflo is separated from `sll $2,$4,1` by the 9-instruction check sequence, so no nop after break 6.
        self.assertEqual(lines[-3:], ["break\t0x6", "sll\t$2,$4,1", "$L15:"])

    def test_signed_div_with_rd_in_rs_or_rt_keeps_mflo_after_checks(self):
        for rs, rt in (("$4", "$2"), ("$2", "$4")):
            maspsx_s = (
                ".set\tnoat\n"
                "\tdiv\t$zero,%s,%s\n"
                "\tbnez\t%s,.L_NOT_DIV_BY_ZERO_1\n"
                "\tnop\n"
                "\tbreak\t0x7\n"
                ".L_NOT_DIV_BY_ZERO_1:\n"
                "\taddiu\t$at,$zero,-1\n"
                "\tbne\t%s,$at,.L_DIV_BY_POSITIVE_SIGN_1\n"
                "\tlui\t$at,0x8000\n"
                "\tbne\t%s,$at,.L_DIV_BY_POSITIVE_SIGN_1\n"
                "\tnop\n"
                "\tbreak\t0x6\n"
                ".L_DIV_BY_POSITIVE_SIGN_1:\n"
                "\tmflo\t$4\n"
                ".set\tat\n"
                "\taddu\t$6,$7,$8\n"
                "\taddu\t$9,$10,$11\n"
            ) % (rs, rt, rt, rt, rs)
            got = sdk_aspsx_pass.rewrite(maspsx_s)
            lines = [line.strip() for line in got.splitlines() if line.strip() and not line.strip().startswith(".")]
            self.assertEqual(lines[0], "div\t$zero,%s,%s" % (rs, rt))
            self.assertEqual(lines[1], "bnez\t%s,.L_NOT_DIV_BY_ZERO_1" % rt)
            self.assertIn("mflo\t$4", lines[-3:])

    def test_unsigned_divu_with_rd_equal_to_rs_moves_mflo_early(self):
        # Unsigned divu checks only read `rt`, so `rd == rs` (with `rd != rt`) has no WAR hazard and goes early.
        maspsx_s = (
            ".set\tnoat\n"
            "\tdivu\t$zero,$4,$5\n"
            "\tbnez\t$5,.L_NOT_DIV_BY_ZERO_2\n"
            "\tnop\n"
            "\tbreak\t0x7\n"
            ".L_NOT_DIV_BY_ZERO_2:\n"
            "\tmflo\t$4\n"
            ".set\tat\n"
        )
        got = sdk_aspsx_pass.rewrite(maspsx_s)
        lines = [line.strip() for line in got.splitlines() if line.strip() and not line.strip().startswith(".")]
        self.assertEqual(lines[:3], ["divu\t$zero,$4,$5", "mflo\t$4", "bnez\t$5,.L_NOT_DIV_BY_ZERO_2"])


class MfloHazardNopWindow(unittest.TestCase):
    """Rule 3: Two instruction slots after `mflo`/`mfhi` before labels, `.set noreorder` branches, and `mult`/`div`."""

    def test_mflo_before_noreorder_branch_gets_two_nops(self):
        s = (
            "\tmflo\t$4\n"
            ".set\tnoreorder\n"
            ".set\tnomacro\n"
            "\tj\t$L15\n"
            "\tsll\t$2,$4,1\n"
            ".set\tmacro\n"
            ".set\treorder\n"
        )
        got = sdk_aspsx_pass.rewrite(s)
        insns = [sdk_aspsx_pass._insn_op(line) for line in got.splitlines() if sdk_aspsx_pass._insn_op(line)]
        self.assertEqual(insns, ["mflo", "nop", "nop", "j", "sll"])

    def test_mflo_followed_by_one_instruction_then_label_gets_one_nop_before_label(self):
        s = (
            "\tmflo\t$4\n"
            "\tnop # DEBUG: Reuse of '$4'. 'sll\t$2,$4,1' does not use $at\n"
            "\tsll\t$2,$4,1\n"
            "$L16:\n"
            "\tlui\t$3,%hi(sym)\n"
        )
        # Wrapped in a late signed div (rd == rt == $4) so Rule 2 drops the Reuse of '$4' nop and Rule 3 pads before $L16.
        div_prefix = (
            "\tdiv\t$zero,$2,$4\n"
            "\tbnez\t$4,.L1\n"
            "\tnop\n"
            "\tbreak\t0x7\n"
            ".L1:\n"
        )
        got = sdk_aspsx_pass.rewrite(div_prefix + s)
        tail = got.split(".L1:\n", 1)[1].splitlines()
        self.assertEqual([line.strip() for line in tail[:4]], ["mflo\t$4", "sll\t$2,$4,1", "nop", "$L16:"])

    def test_mflo_before_reorder_branch_with_nop_delay_slot_needs_no_extra_nop(self):
        s = (
            "\tmflo\t$2\n"
            "\tj\t$L8\n"
            "\tnop  # DEBUG: branch/jump\n"
        )
        self.assertEqual(sdk_aspsx_pass.rewrite(s), s)


@unittest.skipUnless(os.path.exists("/opt/gcc/2.7.2-psx/cc1"), "old gcc not installed")
class RealGccIntegration(unittest.TestCase):
    """End-to-end check through cc.compile_gcc on synthetic C (no game data)."""

    def test_gcc272_div_and_indexed_load_follow_workstation_rules(self):
        code = (
            "extern short tbl[];\n"
            "int f(int y, int x, int neg) {\n"
            "    int q, v;\n"
            "    if (y & 0x7FE00000) q = y / (x >> 10); else q = (y << 10) / x;\n"
            "    v = tbl[q];\n"
            "    if (neg) v = 2048 - v;\n"
            "    return v;\n"
            "}\n"
        )
        with tempfile.TemporaryDirectory() as tmp:
            src = os.path.join(tmp, "t.c")
            obj = os.path.join(tmp, "t.o")
            with open(src, "w") as f:
                f.write(code)
            cc.compile_gcc(src, obj, "2.7.2-psx", "2.79")
            dis = subprocess.run(
                ["mips-linux-gnu-objdump", "-dr", "-M", "no-aliases", obj],
                check=True, stdout=subprocess.PIPE, text=True,
            ).stdout
        # 1. Indexed load `v = tbl[q]` (rd = $v1, rs = $v0) uses destination register ($v1) instead of $at:
        self.assertIn("lui\tv1,0x0", dis)
        self.assertIn("addu\tv1,v1,v0", dis)
        self.assertIn("lh\tv1,0(v1)", dis)
        # 2. First divide (`y / (x >> 10)`) has rd == rs, so late `mflo a0` followed by `nop; nop; j ...; sll v0,a0,0x1`
        #    and second divide (`(y << 10) / x`) has rd distinct, so early `div zero,v0,a1; mflo a0; bnez a1,...`
        self.assertRegex(dis, r"div\tzero,v0,a1\n\s+[0-9a-f]+:\t[0-9a-f]+\s+mflo\ta0\n\s+[0-9a-f]+:\t[0-9a-f]+\s+bnez\ta1")


if __name__ == "__main__":
    unittest.main()
