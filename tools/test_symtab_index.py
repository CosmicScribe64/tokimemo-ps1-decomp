"""T-27077 regressions for immutable SymbolTable snapshot indexes."""
import os
import tempfile
import unittest
from collections import defaultdict
from pathlib import Path

import symtab


SYNTHETIC = {
    "config/overlays.txt": "# name load_address text_size\nAAA 0x80132000 0x200\nEVT 0x800F6000 0x100\n",
    "config/SLPM_86.053.yaml": "options:\n  symbol_addrs_path:\n    - config/symbol_addrs.txt\n    - config/symbol_addrs_main.txt\n",
    "config/overlays/AAA.yaml": "options:\n  symbol_addrs_path:\n    - config/symbol_addrs.txt\n    - config/symbols_AAA.txt\n",
    "config/overlays/EVT.yaml": "options:\n  symbol_addrs_path:\n    - config/symbol_addrs.txt\n    - config/symbols_EVT.txt\n",
    # EVENT sees the shared file, but the EVT-range line belongs to EVT, while
    # the main-only overlay file holds its own line at the same address.
    "config/symbol_addrs.txt": (
        "main_before = 0x800F6100;\n"
        "alias_first = 0x80041100; // alias-view\n"
        "alias_second = 0x80041100; // alias-view\n"
        "aaa_shared = 0x80132010;\n"
    ),
    "config/symbol_addrs_main.txt": (
        "main_after = 0x800F6100;\n"
        "alias_first = 0x80041100; // alias-view\n"
    ),
    "config/symbols_EVT.txt": "evt_owned = 0x800F6100;\n",
    "config/symbols_AAA.txt": "aaa_owned = 0x80132010;\n",
}


def old_owned_lines(table, owner):
    """Independent transcription of the pre-index implementation."""
    out, seen = [], set()
    for unit in table.order:
        for line in table.unit_lines(unit):
            key = (line.path, line.lineno)
            if table.owner(unit, line.addr) == owner and key not in seen:
                seen.add(key)
                out.append(line)
    return out


def old_lines_at(table, owner, addr):
    return [line for line in old_owned_lines(table, owner) if line.addr == addr]


class CandidateLookupTest(unittest.TestCase):
    def make_root(self, files=SYNTHETIC):
        holder = tempfile.TemporaryDirectory(prefix="t27077-symtab-")
        root = Path(holder.name)
        for rel, text in files.items():
            path = root / rel
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text)
        return holder, str(root)

    def test_order_aliases_overlap_and_fresh_lists(self):
        holder, root = self.make_root()
        try:
            table = symtab.SymbolTable(root)
            for owner in table.order:
                self.assertEqual(table.owned_lines(owner), old_owned_lines(table, owner))
            self.assertEqual(
                [line.name for line in table.lines_at("main", 0x80041100)],
                ["alias_first", "alias_second", "alias_first"],
            )
            self.assertEqual(
                [line.name for line in table.lines_at("main", 0x800F6100)],
                ["main_before", "main_after"],
            )
            self.assertEqual(
                [line.name for line in table.lines_at("EVT", 0x800F6100)],
                ["main_before", "evt_owned"],
            )
            returned = table.owned_lines("main")
            cached_line = returned[0]
            returned.clear()
            again = table.owned_lines("main")
            self.assertTrue(again)
            self.assertIs(again[0], cached_line)
            at = table.lines_at("main", 0x80041100)
            repeated_at = table.lines_at("main", 0x80041100)
            self.assertIs(at[0], repeated_at[0])
            at.pop()
            self.assertEqual(len(table.lines_at("main", 0x80041100)), 3)
            names = table.names_at("main", 0x80041100)
            self.assertEqual(names, ["alias_first", "alias_second"])
            names.clear()
            self.assertEqual(table.names_at("main", 0x80041100), ["alias_first", "alias_second"])
        finally:
            holder.cleanup()

    def test_disk_change_requires_new_snapshot(self):
        holder, root = self.make_root()
        try:
            old = symtab.SymbolTable(root)
            first = old.lines_at("main", 0x80041100)
            path = Path(root) / "config/symbol_addrs.txt"
            path.write_text(path.read_text().replace("alias_first", "changed_first"))
            self.assertEqual(old.lines_at("main", 0x80041100), first)
            fresh = symtab.SymbolTable(root)
            self.assertEqual(
                [line.name for line in fresh.lines_at("main", 0x80041100)],
                ["changed_first", "alias_second", "alias_first"],
            )
        finally:
            holder.cleanup()

    def test_real_snapshot_matches_independent_grouping_one_scan_per_owner(self):
        table = symtab.SymbolTable(os.environ.get("T27077_REPO_ROOT", "."))
        original_unit_lines = table.unit_lines
        calls = []

        def counted(unit):
            calls.append(unit)
            return original_unit_lines(unit)

        table.unit_lines = counted
        candidate_scan_calls = 0
        for owner in table.order:
            expected = old_owned_lines(table, owner)
            calls.clear()
            self.assertEqual(table.owned_lines(owner), expected)
            self.assertEqual(len(calls), len(table.order), "owner snapshot should scan units once")
            candidate_scan_calls += len(calls)
            grouped = defaultdict(list)
            for line in expected:
                grouped[line.addr].append(line)
            before = len(calls)
            actual_addrs = {line.addr for line in expected}
            for addr in actual_addrs:
                self.assertEqual(table.lines_at(owner, addr), grouped[addr])
            self.assertEqual(len(calls), before, "address lookup rescanned unit files")
        self.assertEqual(candidate_scan_calls, len(table.order) ** 2,
                         "expected one complete owned-lines scan for each owner")

        # A fresh instance has independent cache state and loads current disk contents.
        fresh = symtab.SymbolTable(os.environ.get("T27077_REPO_ROOT", "."))
        fresh.owned_lines(table.order[0])
        self.assertIsNot(table._owned_lines_snapshot, fresh._owned_lines_snapshot)

    def test_owner_build_scans_once_and_returns_line_identity(self):
        holder, root = self.make_root()
        try:
            table = symtab.SymbolTable(root)
            original = table.unit_lines
            calls = []

            def counted(unit):
                calls.append(unit)
                return original(unit)

            table.unit_lines = counted
            expected = old_owned_lines(table, "main")
            # Reset counting because old oracle itself performs a scan.
            calls.clear()
            first = table.owned_lines("main")
            self.assertEqual(first, expected)
            self.assertEqual(len(calls), len(table.order))
            calls.clear()
            second = table.owned_lines("main")
            table.lines_at("main", expected[0].addr)
            self.assertEqual(calls, [])
            if expected:
                self.assertIs(first[0], second[0])
        finally:
            holder.cleanup()


if __name__ == "__main__":
    unittest.main()
