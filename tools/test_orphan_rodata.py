#!/usr/bin/env python3
"""Synthetic ownership tests; no game data required (T-10200)."""
import unittest
import importlib.util
from pathlib import Path
import tempfile
from unittest.mock import patch

import object_boundaries as ob
import orphan_rodata as orphan

spec = importlib.util.spec_from_file_location("work_queue", Path(__file__).with_name("queue.py"))
queue = importlib.util.module_from_spec(spec)
spec.loader.exec_module(queue)


def function(addr, *refs):
    return ob.Func("func_%08X" % addr, addr, 0x40, frozenset(refs), frozenset(), frozenset())


def item(addr, kind="jtbl"):
    return ob.Item(("jtbl_" if kind == "jtbl" else "D_") + "%08X" % addr,
                   addr, kind, frozenset())


class OwnershipTests(unittest.TestCase):
    def setUp(self):
        self.obj = ob.Obj(0x1000, 0x1100, 0x2000, 0x2020, "yes", "pad", "start")
        self.items = [item(0x2000), item(0x2020)]
        self.funcs = [function(0x1000, self.items[0].name), function(0x1040, self.items[1].name)]
        self.orphans = [(0x2020, 0x2040, "second_chunk")]

    def claim(self):
        return orphan.claim([self.obj], self.orphans, self.funcs, self.items)

    def test_preserve_table_chunk_padding(self):
        objects, remaining, _ = self.claim()
        self.assertEqual(objects[0].ro_end, 0x2020)
        self.assertEqual(objects[1].ro_end, 0x2040)
        self.assertEqual(remaining, [])
        self.assertEqual(self.obj.ro_end, 0x2020)

    def test_successive_chunks(self):
        it = item(0x2040)
        self.items.append(it)
        self.funcs.append(function(0x1080, it.name))
        self.orphans.append((0x2040, 0x2060, "second_chunk"))
        self.assertEqual(self.claim()[0][-1].ro_end, 0x2060)
        self.assertEqual(self.claim()[1], [])

    def test_strings_after_tables_split(self):
        self.items[1] = item(0x2020, "str")
        self.funcs[1] = function(0x1040, self.items[1].name)
        objects, remaining, _ = self.claim()
        self.assertEqual([(o.text_start, o.text_end) for o in objects],
                         [(0x1000, 0x1040), (0x1040, 0x1100)])
        self.assertEqual(objects[1].ro_start, 0x2020)
        self.assertEqual(remaining, [])

    def test_shared_function_cannot_split(self):
        self.items[1] = item(0x2020, "str")
        self.funcs = [function(0x1000, *(i.name for i in self.items))]
        self.assertEqual(self.claim()[1], self.orphans)

    def test_word_aligned_cut(self):
        self.items[1] = item(0x2020, "str")
        self.funcs[1] = function(0x1044, self.items[1].name)
        self.assertEqual(self.claim()[0][1].text_start, 0x1044)
        self.assertEqual(self.claim()[1], [])

    def test_external_user_rejected(self):
        self.funcs.append(function(0x1200, self.items[1].name))
        self.assertEqual(self.claim()[1], self.orphans)

    def test_unreferenced_table_rejected(self):
        self.funcs = self.funcs[:1]
        self.assertEqual(self.claim()[1], self.orphans)

    def test_nonadjacent_rejected(self):
        self.obj = self.obj._replace(ro_end=0x2010)
        self.assertEqual(self.claim()[1], self.orphans)

    def test_unaligned_rodata_rejected(self):
        self.orphans = [(0x2020, 0x2044, "unaligned")]
        self.assertEqual(self.claim()[1], self.orphans)

    def test_assign_object_without_island(self):
        self.obj = self.obj._replace(ro_start=None, ro_end=None, island="no:no_rodata")
        objects, remaining, _ = self.claim()
        self.assertEqual((objects[0].ro_start, objects[0].ro_end, objects[0].island),
                         (0x2020, 0x2040, "yes"))
        self.assertEqual(remaining, [])

    def test_preserve_metadata_and_idempotence(self):
        text = "# evidence\nrodata_end 2040 end\nobject old\norphan old\ndata 1000 3000 3010 owner end ref:1\n"
        result = orphan.config_text(text, [self.obj], self.orphans)
        self.assertTrue(result.startswith("# evidence\nrodata_end 2040 end\n"))
        self.assertTrue(result.endswith("data 1000 3000 3010 owner end ref:1\n"))
        self.assertEqual(orphan.config_text(result, [self.obj], self.orphans), result)

    def test_claimed_tables_and_strings_become_workable_in_queue(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            folder = root / "config/objects"
            folder.mkdir(parents=True)
            path = folder / "main.txt"
            original = orphan.config_text("object old\n", [self.obj], self.orphans)
            path.write_text(original)
            for kind in ("jtbl", "str"):
                facts = queue.Facts(64, 0, False, kind == "jtbl", kind == "str", False, False, False,
                                    tables=(0x2020,) if kind == "jtbl" else (),
                                    string_refs=(0x2020,) if kind == "str" else ())
                self.assertTrue(queue.in_orphan(facts, queue.orphan_ranges(root, "main")))
                objects, remaining, _ = self.claim()
                path.write_text(orphan.config_text(original, objects, remaining))
                f = queue.Func("main/1000", "f", "f.s", facts, True, "main",
                               queue.in_orphan(facts, queue.orphan_ranges(root, "main")))
                self.assertFalse(f.orphan)
                self.assertFalse(f.blocked)
                self.assertGreater(queue.p_match(f), 0)
                path.write_text(original)

    def test_planning_error_restores_config(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            folder = root / "config/objects"
            folder.mkdir(parents=True)
            path = folder / "main.txt"
            original = orphan.config_text("object old\n", [self.obj], self.orphans)
            path.write_text(original)
            from types import SimpleNamespace
            with patch.object(ob, "units", return_value=[SimpleNamespace(name="main")]), \
                 patch.object(ob, "load", return_value=(self.funcs, self.items)), \
                 patch.object(orphan.split.srcscan, "unit_c_files",
                              return_value=[SimpleNamespace(start=0x1000, name="main/1000")]), \
                 patch.object(orphan.split, "plan", side_effect=orphan.split.SplitError("unsafe split")):
                with self.assertRaisesRegex(orphan.split.SplitError, "unsafe split"):
                    orphan.plan(root, "main")
            self.assertEqual(path.read_text(), original)


if __name__ == "__main__":
    unittest.main()
