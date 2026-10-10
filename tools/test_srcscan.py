#!/usr/bin/env python3
"""Unit tests for the C definition scan of tools/srcscan.py (synthetic sources only).

Run: tools/docker.sh python3 tools/test_srcscan.py
"""
import os
import sys
import tempfile
import unittest
from pathlib import Path

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


if __name__ == "__main__":
    unittest.main()
