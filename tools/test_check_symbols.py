"""Synthetic shared-symbol build guard regressions (T-9400). Run in Docker."""
import contextlib
import io
import tempfile
import unittest
from pathlib import Path

import check_symbols


class SymbolsTest(unittest.TestCase):
    def test_overlay_threshold_and_renamed_symbols(self):
        problems = check_symbols.check("main = 0x80131FFF;\nfunc_80132000 = 0x80132000;\nrenamed = 0x80150000;\n")
        self.assertEqual(len(problems), 2)
        self.assertIn(":2: func_80132000", problems[0])
        self.assertIn("config/symbols_<OVL>.txt", problems[1])

    def test_comments_and_main_bss_are_allowed(self):
        self.assertEqual(check_symbols.check("// bad = 0x80132000;\n# bad = 0x80132000;\n"
                                             "/* bad = 0x80132000; */\nmain = 0x8012B530; // main bss\n"), [])

    def test_cli_exits_nonzero_for_contamination(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "symbol_addrs.txt"
            path.write_text("renamed = 0x80132000;\n")
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(check_symbols.main([str(path)]), 1)
                path.write_text("main = 0x80041000;\n")
                self.assertEqual(check_symbols.main([str(path)]), 0)


if __name__ == "__main__":
    unittest.main()
