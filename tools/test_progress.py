#!/usr/bin/env python3
"""Tests for tools/progress.py with synthetic asm and C input (no game data).

Run: tools/docker.sh python3 tools/test_progress.py
"""
import tempfile
import unittest
from pathlib import Path

import progress


def asm(path, name, size):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(".section .text\n\nnonmatching %s, 0x%X\n\nglabel %s\n" % (name, size, name))


class ProgressTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)

    def tearDown(self):
        self.tmp.cleanup()

    def test_unit_totals_counts_c_bodies_as_done(self):
        base = self.root / "asm" / "ovl" / "AAA"
        asm(base / "nonmatchings" / "AAA" / "f1.s", "f1", 8)
        asm(base / "matchings" / "AAA" / "f2.s", "f2", 12)
        c = self.root / "AAA.c"
        c.write_text('INCLUDE_ASM("asm/ovl/AAA/nonmatchings/AAA", f1);\n\nint f2(void) {\n    return 0;\n}\n')
        rows = progress.unit_totals(progress.read_sizes(progress.asm_files(base)), [c])
        self.assertEqual(tuple(rows["AAA"]), (2, 20, 1, 12))

    def test_function_without_source_is_an_error(self):
        base = self.root / "asm" / "ovl" / "AAA"
        asm(base / "nonmatchings" / "AAA" / "f1.s", "f1", 8)
        c = self.root / "AAA.c"
        c.write_text("/* empty */\n")
        with self.assertRaises(ValueError):
            progress.unit_totals(progress.read_sizes(progress.asm_files(base)), [c])

    def test_sdk_totals_are_separate(self):
        asm(self.root / "libx.s", "a", 0x10)
        (self.root / "libx.s").write_text((self.root / "libx.s").read_text() + "\nnonmatching b, 0x20\n")
        self.assertEqual(progress.sdk_totals([self.root / "libx.s"]), (2, 0x30))

    def test_sum_and_format(self):
        t = progress.sum_totals([progress.Totals(2, 20, 1, 12), progress.Totals(1, 4, 0, 0)])
        self.assertEqual(tuple(t), (3, 24, 1, 12))
        self.assertIn("50.0%", progress.fmt_row("x", progress.Totals(2, 20, 1, 10)))
        self.assertIn("0.0%", progress.fmt_row("empty", progress.ZERO))


if __name__ == "__main__":
    unittest.main()
