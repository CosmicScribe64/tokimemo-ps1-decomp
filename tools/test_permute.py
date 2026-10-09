"""Unit tests for the setup logic of tools/permute.py (synthetic files only, no game data).

Run: tools/docker.sh python3 tools/test_permute.py   (from the repo root)
"""
import os
import tempfile
import unittest

import permute


def touch(root, rel, text=""):
    path = os.path.join(root, rel)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write(text)
    return path


class FindSource(unittest.TestCase):
    def test_finds_c_definition_not_include_asm_or_call(self):
        with tempfile.TemporaryDirectory() as root:
            touch(root, "src/main/80041000.c",
                  'INCLUDE_ASM("asm/nonmatchings/main/80041000", func_1);\nvoid g(void) { func_1(); }\n')
            c = touch(root, "src/ovl/TT.c", "#ifdef NON_MATCHING\nvoid func_1(s32 a) {\n}\n#endif\n")
            self.assertEqual(permute.find_source(root, "func_1"), c)
            self.assertIsNone(permute.find_source(root, "func_2"))

    def test_name_is_not_a_prefix_match(self):
        with tempfile.TemporaryDirectory() as root:
            touch(root, "src/main/a.c", "void func_10(void) {\n}\n")
            self.assertIsNone(permute.find_source(root, "func_1"))

    def test_pointer_return(self):
        with tempfile.TemporaryDirectory() as root:
            c = touch(root, "src/main/a.c", "u8 *func_1(u8 *a) {\n return a;\n}\n")
            self.assertEqual(permute.find_source(root, "func_1"), c)


class FindAsm(unittest.TestCase):
    def test_nonmatchings_overlay_and_missing(self):
        with tempfile.TemporaryDirectory() as root:
            a = touch(root, "asm/ovl/TT/nonmatchings/TT/func_1.s")
            self.assertEqual(permute.find_asm(root, "func_1"), a)
            self.assertIsNone(permute.find_asm(root, "func_2"))


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
