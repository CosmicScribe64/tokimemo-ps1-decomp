#!/usr/bin/env python3
"""Unit tests for the C definition scan of tools/srcscan.py (synthetic sources only).

Run: tools/docker.sh python3 tools/test_srcscan.py
"""
import os
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import srcscan  # noqa: E402

SRC = """#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041000);

void func_ansi(void) {
    D_1 = 1;
}

s32 func_two(s32 a, u8 *b)
{
    return a;
}

void func_knr(a, b)
s32 a;
u8 *b;
{
    D_2 = a;
}

u8 *func_knr_ptr(p)
register u8 *p;
{
    return p;
}

void func_knr_noargs()
{
}

void func_knr_one(arg0)
u8 arg0;
{
    D_3 = arg0;
}

void proto_only(s32 a);
extern u8 D_4;
struct S {
    s32 x;
};
"""


class Defs(unittest.TestCase):
    def defined(self, text):
        with tempfile.TemporaryDirectory() as tmp:
            p = Path(tmp) / "a.c"
            p.write_text(text)
            return srcscan.defined_functions(p)

    def test_ansi_and_knr_definitions(self):
        self.assertEqual(self.defined(SRC), {"func_ansi", "func_two", "func_knr", "func_knr_ptr",
                                             "func_knr_noargs", "func_knr_one"})

    def test_prototypes_and_structs_are_not_definitions(self):
        self.assertEqual(self.defined("void f(s32 a);\nextern u8 D;\nstruct S {\n    s32 x;\n};\n"
                                      "void g(void);\nu8 h;\nstruct T {\n};\n"), set())

    def test_include_asm_not_a_definition(self):
        self.assertEqual(self.defined('INCLUDE_ASM("asm/x", func_1);\n'), set())

    def test_profile_unit_headers_are_plural_explicit_roots(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            include = root / "include"
            (include / "main").mkdir(parents=True)
            (include / "ovl").mkdir()
            (include / "common.h").write_text("typedef int s32;\n")
            (include / "main" / "first.h").write_text('#include "common.h"\n')
            (include / "main" / "second.h").write_text('#include "common.h"\n')
            (include / "ovl" / "AA.h").write_text('#include "common.h"\n')
            (root / "config").mkdir()
            (root / "config" / "header_owners.json").write_text(
                '{"mode":"topic_only","owners":[{"path":"main/first.h","role":"main"},'
                '{"path":"main/second.h","role":"main"}],"required_types":["common.h"]}')
            src = root / "src" / "ovl" / "AA.c"
            src.parent.mkdir(parents=True)
            (include / "main" / "comment.h").write_text("ignored\n")
            src.write_text('/*\n#include "main/comment.h"\n*/\n'
                           '// #include "main/comment.h"\n'
                           '#include \\\n"ovl/AA.h"\n#include "common.h"\n')
            self.assertEqual(srcscan.unit_headers("main", tmp), ("main/first.h", "main/second.h"))
            with patch.object(srcscan, "unit_c_files", return_value=[SimpleNamespace(src=src)]):
                self.assertEqual(srcscan.unit_headers("AA", tmp), ("common.h", "ovl/AA.h"))


if __name__ == "__main__":
    unittest.main()
