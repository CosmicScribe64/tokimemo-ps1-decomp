#!/usr/bin/env python3
"""Unit tests for tools/entry_rule.py (T-5010). Synthetic instructions only.

Run (in Docker): python3 tools/test_entry_rule.py
"""
import os
import sys
import unittest
from typing import NamedTuple

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import entry_rule as er  # noqa: E402


class Insn(NamedTuple):
    addr: int
    op: str
    args: str


def insns(*ops):
    return [Insn(0x1000 + 4 * n, op, "") for n, op in enumerate(ops)]


class AtEntry(unittest.TestCase):
    def test_prologue_only(self):
        code = insns("lui", "lbu", "addiu", "sw", "beqz")
        self.assertTrue(er.at_entry(code, set(), 1))

    def test_after_call(self):
        code = insns("addiu", "sw", "jal", "nop", "lui", "lbu")
        self.assertFalse(er.at_entry(code, set(), 5))

    def test_after_branch(self):
        code = insns("lui", "lw", "beqz", "nop", "lui", "lbu")
        self.assertFalse(er.at_entry(code, set(), 5))

    def test_after_label(self):
        code = insns("lui", "nop", "lui", "lbu")
        self.assertFalse(er.at_entry(code, {code[2].addr}, 3))
        self.assertTrue(er.at_entry(code, {code[0].addr}, 3))   # the function's own start


class LateGlobals(unittest.TestCase):
    def test_first_load_decides(self):
        code = insns("lui", "lbu", "jal", "nop", "lui", "lbu", "lui", "lbu")
        loads = [(0, "v1", "D_1", False, 1), (1, "v1", "D_1", False, 5), (1, "v0", "D_2", False, 7)]
        self.assertEqual(er.late_globals(code, set(), loads), {"D_2"})


class SelectorFlag(unittest.TestCase):
    def test_reproduced_by_the_build(self):
        self.assertEqual(er.selector_flag("v1", True), "")
        self.assertEqual(er.selector_flag("v0", False), "")

    def test_still_blocked(self):
        self.assertEqual(er.selector_flag("v1", False), "v1")
        self.assertEqual(er.selector_flag("v0", True), "v0")
        self.assertEqual(er.selector_flag("t6", True), "t6")


if __name__ == "__main__":
    unittest.main()
