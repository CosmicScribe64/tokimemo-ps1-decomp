#!/usr/bin/env python3
"""Unit tests for tools/funcloc.py (T-5030): synthetic repository, no game data.

Run: tools/docker.sh python3 tools/test_funcloc.py
"""
import os
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import funcloc  # noqa: E402


def write(root, rel, text):
    p = Path(root) / rel
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(text)
    return p


def yaml_for(subs, vram):
    return ("segments:\n  - {start: 0, vram: %d, type: code, subsegments: [%s]}\n"
            % (vram, ", ".join("[0, c, %s]" % s for s in subs)))


def make_repo(root):
    write(root, "config/overlays.txt", "A 0x80132000 0x100\nB 0x80132000 0x100\n")
    write(root, "config/SLPM_86.053.yaml", yaml_for(["main/80041000"], 0x80041000))
    write(root, "config/overlays/A.yaml", yaml_for(["A/80132000", "A/80133000"], 0x80132000))
    write(root, "config/overlays/B.yaml", yaml_for(["B"], 0x80132000))
    write(root, "src/main/80041000.c", 'INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041000);\n'
                                       "void caller(void) {\n    func_80132100();\n    func_80133010();\n}\n")
    # A: defines func_80132100 and calls func_80133010; its second file defines it (K&R)
    write(root, "src/ovl/A/80132000.c", "void func_80132100(void) {\n    func_80133010();\n}\n")
    write(root, "src/ovl/A/80133000.c", "void func_80133010(a)\ns32 a;\n{\n}\n")
    # B: the same address, INCLUDE_ASM; and a mention of an A function
    write(root, "src/ovl/B.c", 'INCLUDE_ASM("asm/ovl/B/nonmatchings/B", func_80132100);\n'
                               "void g(void) {\n    func_80133010();\n}\n")


class Locate(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = self.tmp.name
        make_repo(self.root)

    def test_definition_not_mention(self):
        # main and B call func_80133010, only A/80133000.c defines it (K&R)
        c = funcloc.locate("func_80133010", root=self.root)
        self.assertEqual((c.unit, c.name), ("A", "A/80133000"))

    def test_include_asm_and_body_in_one_file_is_one_holder(self):
        write(self.root, "src/main/80041000.c", '#ifdef NON_MATCHING\nvoid func_80041000(void) {\n}\n#else\n'
                                                'INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041000);\n#endif\n')
        funcloc._index_cache.clear()
        self.assertEqual(funcloc.locate("func_80041000", root=self.root).unit, "main")

    def test_include_asm_counts_as_holder(self):
        c = funcloc.locate("func_80041000", root=self.root)
        self.assertEqual(c.unit, "main")

    def test_colliding_name_is_refused_with_the_candidates(self):
        with self.assertRaises(funcloc.AmbiguousError) as cm:
            funcloc.locate("func_80132100", root=self.root)
        msg = str(cm.exception)
        self.assertIn("A (", msg)
        self.assertIn("B (", msg)
        self.assertIn("--unit", msg)

    def test_unit_scope_name_prefix_and_path(self):
        for scope, unit in (("A", "A"), ("B", "B"), ("src/ovl/B.c", "B"), ("asm/ovl/A/nonmatchings/A/80132000/x.s", "A")):
            self.assertEqual(funcloc.locate("func_80132100", scope, root=self.root).unit, unit, scope)
        self.assertEqual(funcloc.locate("B:func_80132100", root=self.root).unit, "B")

    def test_exact_file_scope(self):
        c = funcloc.locate("func_80132100", "src/ovl/A/80132000.c", root=self.root)
        self.assertEqual(c.name, "A/80132000")

    def test_mention_only_is_not_found(self):
        with self.assertRaises(funcloc.LocateError) as cm:
            funcloc.locate("func_80133010", "B", root=self.root)   # B only calls it
        self.assertIn("no INCLUDE_ASM or C definition", str(cm.exception))
        with self.assertRaises(funcloc.LocateError):
            funcloc.locate("nothing_here", root=self.root)

    def test_conflicting_prefix_and_scope(self):
        with self.assertRaises(funcloc.LocateError):
            funcloc.locate("A:func_80132100", "B", root=self.root)

    def test_unknown_unit(self):
        with self.assertRaises(funcloc.LocateError):
            funcloc.locate("func_80132100", "ZZ", root=self.root)

    def test_defined_only_skips_include_asm(self):
        self.assertEqual(funcloc.locate("func_80132100", root=self.root, defined_only=True).unit, "A")

    def test_index_follows_edits(self):
        funcloc.locate("func_80132100", "B", root=self.root)
        p = write(self.root, "src/ovl/B.c", "void func_80132100(void) {\n}\n")
        os.utime(p, (p.stat().st_atime, p.stat().st_mtime + 5))
        self.assertTrue(funcloc.locate("func_80132100", "B", root=self.root, defined_only=True))


class Includes(unittest.TestCase):
    def test_recursive_includes(self):
        with tempfile.TemporaryDirectory() as root:
            write(root, "include/a.h", '#include "b.h"\n')
            write(root, "include/b.h", "int x;\n")
            c = write(root, "src/x.c", '#include "a.h"\n#include "missing.h"\n')
            got = funcloc.includes_of(c, root)
            self.assertEqual({p.name for p in got}, {"a.h", "b.h"})


if __name__ == "__main__":
    unittest.main()
