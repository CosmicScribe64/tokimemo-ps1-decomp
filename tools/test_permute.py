"""Unit tests for the setup logic of tools/permute.py (synthetic files only, no game data).

Run: tools/docker.sh python3 tools/test_permute.py   (from the repo root)
"""
import os
import shutil
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import funcloc  # noqa: E402
import permute  # noqa: E402


def touch(root, rel, text=""):
    path = os.path.join(root, rel)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write(text)
    return path


def make_repo(root):
    """Synthetic splat configs: main + overlays A and B, which both load at 0x80132000."""
    touch(root, "config/overlays.txt", "A 0x80132000 0x100\nB 0x80132000 0x100\n")

    def yaml(subs, vram):
        return ("segments:\n  - {start: 0, vram: %d, type: code, subsegments: [%s]}\n"
                % (vram, ", ".join("[0, c, %s]" % x for x in subs)))
    touch(root, "config/SLPM_86.053.yaml", yaml(["main/80041000"], 0x80041000))
    touch(root, "config/overlays/A.yaml", yaml(["A"], 0x80132000))
    touch(root, "config/overlays/B.yaml", yaml(["B"], 0x80132000))


class FindSource(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = self.tmp.name
        make_repo(self.root)

    def test_finds_c_definition_not_include_asm_or_call(self):
        root = self.root
        touch(root, "src/main/80041000.c",
              'INCLUDE_ASM("asm/nonmatchings/main/80041000", func_1);\nvoid g(void) { func_1(); }\n')
        c = touch(root, "src/ovl/A.c", "#ifdef NON_MATCHING\nvoid func_1(s32 a) {\n}\n#endif\n")
        self.assertEqual(permute.find_source(root, "func_1"), c)
        self.assertIsNone(permute.find_source(root, "func_2"))

    def test_name_is_not_a_prefix_match(self):
        touch(self.root, "src/main/80041000.c", "void func_10(void) {\n}\n")
        self.assertIsNone(permute.find_source(self.root, "func_1"))

    def test_pointer_return(self):
        c = touch(self.root, "src/main/80041000.c", "u8 *func_1(u8 *a) {\n return a;\n}\n")
        self.assertEqual(permute.find_source(self.root, "func_1"), c)

    def test_colliding_overlay_names_need_a_unit(self):
        a = touch(self.root, "src/ovl/A.c", "void func_80132100(void) {\n}\n")
        b = touch(self.root, "src/ovl/B.c", "void func_80132100(s32 x) {\n}\n")
        with self.assertRaises(funcloc.AmbiguousError):
            permute.find_source(self.root, "func_80132100")
        self.assertEqual(permute.find_source(self.root, "func_80132100", "B"), b)
        self.assertEqual(permute.find_source(self.root, "func_80132100", "src/ovl/A.c"), a)

    def test_a_caller_in_another_overlay_is_not_the_source(self):
        touch(self.root, "src/ovl/A.c", "void g(void) {\n    func_80132100();\n}\n")
        b = touch(self.root, "src/ovl/B.c", "void func_80132100(void) {\n}\n")
        self.assertEqual(permute.find_source(self.root, "func_80132100"), b)

    def test_knr_definition(self):
        c = touch(self.root, "src/ovl/A.c", "void func_1(a, b)\ns32 a;\nu8 *b;\n{\n    D = a;\n}\n")
        self.assertEqual(permute.find_source(self.root, "func_1"), c)
        self.assertTrue(permute.def_regex("func_1").search("void func_1(a)\nregister u8 a;\n{\n}\n"))
        self.assertTrue(permute.def_regex("func_1").search("u8 *func_1()\n{\n}\n"))
        self.assertIsNone(permute.def_regex("func_1").search("void g(void) {\n    func_1(a)\n;\n}\n"))
        self.assertIsNone(permute.def_regex("func_1").search("void func_1(s32 a);\nint x;\n{\n"))


class FindAsm(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = self.tmp.name
        make_repo(self.root)

    def test_nonmatchings_overlay_and_missing(self):
        a = touch(self.root, "asm/ovl/A/nonmatchings/A/func_1.s")
        self.assertEqual(permute.find_asm(self.root, "func_1"), a)
        self.assertIsNone(permute.find_asm(self.root, "func_2"))

    def test_asm_of_the_units_own_source_not_another_overlays(self):
        a = touch(self.root, "asm/ovl/A/nonmatchings/A/func_80132100.s")
        b = touch(self.root, "asm/ovl/B/nonmatchings/B/func_80132100.s")
        src_b = touch(self.root, "src/ovl/B.c", "void func_80132100(void) {\n}\n")
        self.assertEqual(permute.find_asm(self.root, "func_80132100", src_b), b)
        self.assertEqual(permute.find_asm(self.root, "func_80132100", None, "A"), a)
        with self.assertRaises(SystemExit):
            permute.find_asm(self.root, "func_80132100")

    def test_setup_uses_source_and_asm_of_the_same_unit(self):
        touch(self.root, "src/ovl/A.c", "void func_80132100(void) {\n    a();\n}\n")
        touch(self.root, "src/ovl/B.c", "void func_80132100(void) {\n    b();\n}\n")
        touch(self.root, "asm/ovl/A/nonmatchings/A/func_80132100.s")
        b = touch(self.root, "asm/ovl/B/nonmatchings/B/func_80132100.s")
        seen = {}
        with mock.patch.object(permute, "build_base", side_effect=lambda r, src, f: seen.update(src=src) or "x"), \
                mock.patch.object(permute, "build_target", side_effect=lambda r, asm, o: seen.update(asm=asm)):
            permute.setup(self.root, "B:func_80132100", out=os.path.join(self.root, "out"))
        self.assertEqual(seen["asm"], b)
        self.assertTrue(seen["src"].endswith("B.c"))
        with self.assertRaises(SystemExit) as cm:
            permute.setup(self.root, "func_80132100", out=os.path.join(self.root, "out2"))
        self.assertIn("held by 2", str(cm.exception))


class Args(unittest.TestCase):
    def test_bare_name_and_explicit_all_take_the_function(self):
        for argv in (["func_1", "--time", "5"], ["all", "func_1", "--time", "5"]):
            a = permute.parse_args(argv)
            self.assertEqual((a.cmd, a.func, a.time), ("all", "func_1", 5), argv)

    def test_unit_option(self):
        a = permute.parse_args(["all", "func_1", "--unit", "TT"])
        self.assertEqual(a.unit, "TT")
        self.assertEqual(permute.parse_args(["setup", "func_1", "--unit", "TT"]).unit, "TT")

    def test_run_takes_a_directory(self):
        self.assertEqual(permute.parse_args(["run", "build/permute/x", "--time", "3"]).directory, "build/permute/x")


@unittest.skipUnless(os.path.isdir(permute.PERMUTER) and shutil.which("gcc"), "needs decomp-permuter and gcc (Docker)")
class KnrBase(unittest.TestCase):
    def test_build_base_accepts_a_knr_target(self):
        with tempfile.TemporaryDirectory() as root:
            src = touch(root, "a.c", "int D;\nvoid other(void) {\n    D = 2;\n}\n\n"
                                     "void func_1(a, b)\ns32 a;\ns32 b;\n{\n    D = a + b;\n}\n")
            base = permute.build_base(root, src, "func_1")
        self.assertIn("func_1", base)
        self.assertIn("D = a + b", base)
        self.assertNotIn("D = 2", base)   # the other function is stripped to a prototype


class Text(unittest.TestCase):
    def test_strip_include_asm(self):
        src = 'a;\nINCLUDE_ASM("asm/x", f1);\n  INCLUDE_ASM("asm/y", f2)\nb;\n'
        self.assertEqual(permute.strip_include_asm(src), "a;\nb;\n")

    def test_settings(self):
        self.assertEqual(permute.settings_toml("func_1"),
                         'func_name = "func_1"\ncompiler_type = "ido"\n')

    def test_compile_script_uses_project_pipeline(self):
        s = permute.compile_script("/repo")
        self.assertIn('cd "/repo"', s)
        self.assertIn('tools/cc.py', s)
        self.assertIn(" ido 5.3", s)

    def test_target_source_has_prelude_first(self):
        self.assertEqual(permute.target_source("glabel f\n"),
                         '.include "asmproc_prelude.inc"\nglabel f\n')


class Outputs(unittest.TestCase):
    def test_sorted_best_first_and_other_dirs_ignored(self):
        with tempfile.TemporaryDirectory() as d:
            for n in ("output-20-1", "output-0-2", "output-5-1", "base.c", "output-x"):
                os.makedirs(os.path.join(d, n))
            self.assertEqual([(s, n) for s, n, _ in permute.list_outputs(d)],
                             [(0, 2), (5, 1), (20, 1)])


if __name__ == "__main__":
    unittest.main()
