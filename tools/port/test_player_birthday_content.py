"""Tests for tools/port/player_birthday_content.py.

The unit tests use invented bytes. The original-disc check runs only when
PLAYER_BIRTHDAY_INPUT names the extracted disc root; that invocation fails
when the input is absent instead of passing silently:
  PLAYER_BIRTHDAY_INPUT=/disc/files python -m unittest \
      tools.port.test_player_birthday_content.OriginalStepTable
"""
import hashlib
import os
import struct
import unittest
from pathlib import Path

from tools.port import player_birthday_content as content


def _fake_geko(functions):
    names = {}
    data = bytearray(content.STEP_TABLE - content.GEKO_LOAD + 4 * content.STEP_COUNT)
    for index, name in enumerate(functions):
        address = 0x80140000 + 4 * index
        names[address] = name
        struct.pack_into("<I", data, content.STEP_TABLE - content.GEKO_LOAD + 4 * index, address)
    return bytes(data), names


def _expected_functions():
    functions = ["shared_%d" % i for i in range(content.STEP_COUNT)]
    for index, name in content.RULE_STEPS.items():
        functions[index] = name
    return functions


class StepTableTests(unittest.TestCase):
    def test_resolves_every_entry(self):
        data, names = _fake_geko(_expected_functions())
        rows = content.step_table(data, names, hashlib.sha256(data).hexdigest())
        self.assertEqual(len(rows), content.STEP_COUNT)
        self.assertEqual(rows[1]["function"], "encounter_player_birthday_girl_select")
        self.assertEqual(rows[1]["address"], "0x80140004")

    def test_refuses_other_bytes(self):
        data, names = _fake_geko(_expected_functions())
        with self.assertRaises(content.ContentError):
            content.step_table(data, names)

    def test_refuses_unknown_address(self):
        data, names = _fake_geko(_expected_functions())
        del names[0x80140000 + 4 * 5]
        with self.assertRaises(content.ContentError):
            content.step_table(data, names, hashlib.sha256(data).hexdigest())

    def test_refuses_moved_rule_step(self):
        functions = _expected_functions()
        functions[1], functions[2] = functions[2], functions[1]
        data, names = _fake_geko(functions)
        with self.assertRaises(content.ContentError):
            content.step_table(data, names, hashlib.sha256(data).hexdigest())

    def test_refuses_short_file(self):
        data = b"\0" * 16
        with self.assertRaises(content.ContentError):
            content.step_table(data, {}, hashlib.sha256(data).hexdigest())


class OutputTests(unittest.TestCase):
    def test_output_must_be_new_child_of_port_cache(self):
        with self.assertRaises(content.ContentError):
            content._new_output(Path("/tmp/elsewhere"))
        with self.assertRaises(content.ContentError):
            content._new_output(content.ROOT / ".asset-cache" / "port" / ".." / "x")

    def test_symbol_files_name_the_rule_functions(self):
        names = set(content.symbols().values())
        for name in set(content.RULE_STEPS.values()):
            self.assertIn(name, names)


class OriginalStepTable(unittest.TestCase):
    def test_original_table(self):
        root = os.environ.get("PLAYER_BIRTHDAY_INPUT")
        self.assertTrue(root, "set PLAYER_BIRTHDAY_INPUT to the extracted disc root")
        data = (Path(root) / content.GEKO_PATH).read_bytes()
        rows = content.step_table(data, content.symbols())
        self.assertEqual([r["function"] for r in rows].count("encounter_player_birthday_talk"), 4)
        self.assertEqual(rows[28]["function"], "game_mode_reset_to_title")


def load_tests(loader, tests, pattern):
    """The original-disc class runs only when named explicitly."""
    suite = unittest.TestSuite()
    for case in (StepTableTests, OutputTests):
        suite.addTests(loader.loadTestsFromTestCase(case))
    return suite
