"""Unit tests for the per-object toolchain table of configure.py (T-9200), on temporary files.

Run from the repo root: python3 tools/test_configure_toolchains.py
"""
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), ".."))
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__))))
import configure  # noqa: E402

KNOWN = {"main/80041000", "TACO/8015D270", "TACO/8015CF30"}


def table(text):
    with tempfile.TemporaryDirectory() as d:
        p = os.path.join(d, "toolchains.txt")
        with open(p, "w") as f:
            f.write(text)
        return configure.read_toolchains(KNOWN, p)


class ReadToolchains(unittest.TestCase):
    def test_gcc_and_ido_entries_and_comments(self):
        got = table("# comment\n\nTACO/8015D270 gcc 2.7.2-psx 2.79  # why\nmain/80041000 ido 7.1\n")
        self.assertEqual(got, {"TACO/8015D270": ["gcc", "2.7.2-psx", "2.79"],
                               "main/80041000": ["ido", "7.1"]})

    def test_missing_file_is_empty(self):
        self.assertEqual(configure.read_toolchains(KNOWN, "/nonexistent/toolchains.txt"), {})

    def test_unknown_object_stops(self):
        with self.assertRaises(SystemExit):
            table("TACO/DEADBEEF gcc 2.7.2-psx 2.79\n")

    def test_bad_toolchain_stops(self):
        for bad in ("TACO/8015D270 gcc 2.7.2-psx\n", "TACO/8015D270 clang 1\n", "TACO/8015D270 ido\n"):
            with self.assertRaises(SystemExit):
                table(bad)

    def test_duplicate_stops(self):
        with self.assertRaises(SystemExit):
            table("TACO/8015D270 gcc 2.7.2-psx 2.79\nTACO/8015D270 ido 5.3\n")


if __name__ == "__main__":
    unittest.main()
