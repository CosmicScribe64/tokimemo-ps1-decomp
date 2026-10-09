#!/usr/bin/env python3
"""Tests for tools/check_headers.py with synthetic headers (no game data).

Run: tools/docker.sh python3 tools/test_check_headers.py
"""
import tempfile
import unittest
from pathlib import Path

import check_headers


class CheckHeadersTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.inc = Path(self.tmp.name)
        (self.inc / "ovl").mkdir()

    def tearDown(self):
        self.tmp.cleanup()

    def write(self, name, text):
        (self.inc / name).write_text(text)

    def problems(self):
        return check_headers.check(str(self.inc))

    def test_clean(self):
        self.write("game.h", "extern s32 D_1;\nvoid f(s32 a);\n")
        self.write("ovl/A.h", '#include "game.h"\nextern s16 D_2;\n')
        self.assertEqual(self.problems(), [])

    def test_conflict_between_game_and_overlay(self):
        self.write("game.h", "extern s32 D_800CA160;\n")
        self.write("ovl/A.h", '#include "game.h"\nextern u32 D_800CA160;\n')
        out = self.problems()
        self.assertEqual(len(out), 1)
        self.assertIn("conflict D_800CA160", out[0])

    def test_conflict_inside_one_header(self):
        self.write("game.h", "extern u8 D_1[];\nextern s32 D_1;\n")
        self.assertIn("conflict D_1", self.problems()[0])

    def test_array_versus_scalar(self):
        self.write("game.h", "extern u8 D_1[];\n")
        self.write("ovl/A.h", '#include "game.h"\nextern s16 D_1;\n')
        self.assertIn("conflict D_1", self.problems()[0])

    def test_exact_duplicate_fails(self):
        self.write("game.h", "extern s32 D_1;\n")
        self.write("ovl/A.h", '#include "game.h"\nextern s32 D_1;\n')
        self.assertIn("duplicate D_1", self.problems()[0])
        self.write("ovl/A.h", "extern s32 D_9;\nextern s32 D_9;\n")
        self.assertTrue(any("duplicate D_9" in p for p in self.problems()))

    def test_duplicate_in_comma_list(self):
        self.write("game.h", "extern s32 D_1, D_2;\nextern s32 D_2;\n")
        self.assertIn("duplicate D_2", self.problems()[0])

    def test_unrelated_overlays_may_differ(self):
        # separate programs: never in one translation unit
        self.write("ovl/A.h", "extern s32 D_8013A428;\n")
        self.write("ovl/B.h", "extern s16 D_8013A428;\n")
        self.assertEqual(self.problems(), [])

    def test_prototype_return_and_params(self):
        self.write("game.h", "s32 f(s32 a);\n")
        self.write("ovl/A.h", '#include "game.h"\nvoid f(s32 a);\n')
        self.write("ovl/B.h", '#include "game.h"\ns32 f(u8 a);\n')
        out = self.problems()
        self.assertEqual(len(out), 2)

    def test_unprototyped_is_compatible_unless_narrow(self):
        self.write("game.h", "void f(s32 a);\nvoid g(u8 a);\n")
        self.write("ovl/A.h", '#include "game.h"\nvoid f();\nvoid g();\n')
        out = self.problems()
        self.assertEqual(len(out), 1)
        self.assertIn("conflict g", out[0])

    def test_ignores_comments_typedefs_and_bodies(self):
        self.write("game.h", "/* extern u8 D_1; */\ntypedef struct S {\n    s32 D_1;\n} S;\nextern s32 D_1;\n")
        self.assertEqual(self.problems(), [])

    def test_pointer_types_differ(self):
        self.write("game.h", "extern void *D_1;\nextern u8 *D_1;\n")
        self.assertIn("conflict D_1", self.problems()[0])

    def test_source_definition_conflicts_with_header(self):
        self.write("game.h", "void f();\nvoid g(s32 a);\n")
        (self.inc / "a.c").write_text('#include "game.h"\nvoid f(u8 a) {\n}\nvoid g(s32 a) {\n}\n')
        out = check_headers.check(str(self.inc), str(self.inc))
        self.assertEqual(len(out), 1)
        self.assertIn("conflict f", out[0])

    def test_repo_headers_are_clean(self):
        repo = Path(__file__).resolve().parent.parent / "include"
        if repo.is_dir():
            self.assertEqual(check_headers.check(str(repo), str(repo.parent / "src")), [])


if __name__ == "__main__":
    unittest.main()
