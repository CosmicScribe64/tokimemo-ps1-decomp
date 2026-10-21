#!/usr/bin/env python3
"""Tests for tools/lever_lab.py (T-25013): guard scanning, score comparison, object normalising."""
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import lever_lab


class LeverLabTest(unittest.TestCase):
    def test_guarded_functions(self):
        with tempfile.TemporaryDirectory() as d:
            root = Path(d)
            (root / "src" / "ovl" / "GEKO").mkdir(parents=True)
            (root / "src" / "main").mkdir(parents=True)
            (root / "src" / "ovl" / "GEKO" / "a.c").write_text(
                '#ifdef NON_MATCHING\nvoid f(void) { }\n#else\nINCLUDE_ASM("asm/x", func_1);\n#endif\n'
                'INCLUDE_ASM("asm/x", func_2);\n')
            (root / "src" / "main" / "b.c").write_text(
                '#ifdef NON_MATCHING\nvoid g(void) { }\n#else\nINCLUDE_ASM("asm/y", func_3);\n#endif\n')
            old = lever_lab.ROOT
            lever_lab.ROOT = root
            try:
                self.assertEqual(lever_lab.guarded_functions(root),
                                 [("GEKO", "func_1"), ("main", "func_3")])
            finally:
                lever_lab.ROOT = old

    def test_compare_scores(self):
        lines = lever_lab.compare_scores({"A:f": 9, "A:g": 4, "A:h": 3, "A:i": 5},
                                         {"A:f": 0, "A:g": 6, "A:h": 2, "A:i": 5})
        self.assertEqual(len(lines), 3)
        self.assertTrue(lines[0].strip().startswith("ZERO"))
        self.assertTrue(any("WORSE" in l for l in lines))
        self.assertTrue(any("better" in l for l in lines))

    def test_count_rows(self):
        self.assertEqual(lever_lab.count_rows("func_1: MATCH (linked)\n"), 0)
        self.assertEqual(lever_lab.count_rows("func_1: DIFF\n  -aa ; x\n  +bb ; y\n"), 2)
        self.assertEqual(lever_lab.count_rows("Traceback ... error"), -1)

    def test_trim(self):
        self.assertEqual(lever_lab.trim(["a", "00000000 nop", "00000000 nop"]), ["a"])


if __name__ == "__main__":
    unittest.main()
