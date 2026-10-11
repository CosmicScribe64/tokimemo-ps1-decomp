"""Unit tests for tools/gcc_fingerprint.py (T-9200) on synthetic splat listings (no game data).

Run from tools/: python3 test_gcc_fingerprint.py
"""
import os
import tempfile
import unittest

import gcc_fingerprint as gf


def listing(*ops):
    return ["    /* 0 80000000 00000000 */  %s" % o for o in ops]


class Features(unittest.TestCase):
    def test_addu_zero_is_a_move_in_either_operand_slot(self):
        self.assertTrue(gf.is_move("addu", "$t7, $a0, $zero"))
        self.assertTrue(gf.is_move("addu", "$v0, $zero, $a1"))
        self.assertFalse(gf.is_move("or", "$v0, $a1, $zero"))
        self.assertFalse(gf.is_move("addu", "$v0, $a1, $a2"))
        self.assertFalse(gf.is_move("addiu", "$v0, $a1, 0x0"))

    def test_local_jump_only(self):
        self.assertTrue(gf.is_local_jump("j", ".L8015D2D4"))
        self.assertFalse(gf.is_local_jump("j", "func_80040000"))
        self.assertFalse(gf.is_local_jump("b", ".L8015D2D4"))

    def test_counts(self):
        f = listing("addu       $t7, $a0, $zero", "j          .L1", "nop", "or $v0, $a0, $zero")
        self.assertEqual(gf.features(f), (4, 1, 1))


class Rule(unittest.TestCase):
    def test_local_jump_alone_is_enough(self):
        self.assertTrue(gf.is_sdk_style(100, 0, 1))

    def test_one_move_in_a_large_function_is_not(self):
        self.assertFalse(gf.is_sdk_style(259, 1, 0))
        self.assertFalse(gf.is_sdk_style(1097, 2, 0))

    def test_one_move_in_a_small_function_is(self):
        self.assertTrue(gf.is_sdk_style(20, 1, 0))
        self.assertFalse(gf.is_sdk_style(0, 0, 0))


class Scan(unittest.TestCase):
    def test_scan_finds_hit_and_skips_clean_function(self):
        with tempfile.TemporaryDirectory() as root:
            d = os.path.join(root, "asm/ovl/TACO/nonmatchings/TACO/8015D270")
            os.makedirs(d)
            with open(os.path.join(d, "func_8015D270.s"), "w") as f:
                f.write("glabel func_8015D270\n" + "\n".join(listing("addu $t7, $a0, $zero", "j .L1")) + "\n")
            with open(os.path.join(d, "func_8015D300.s"), "w") as f:
                f.write("glabel func_8015D300\n" + "\n".join(listing("or $v0, $a0, $zero", "jr $ra")) + "\n")
            hits = gf.scan(root)
            self.assertEqual([(h[0], h[1], h[2]) for h in hits], [("TACO", "8015D270", "func_8015D270")])
            self.assertEqual(gf.scan(root, {"DATE"}), [])


if __name__ == "__main__":
    unittest.main()
