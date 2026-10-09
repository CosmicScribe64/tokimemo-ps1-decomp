"""Unit tests for the Shift-JIS string re-encoding in tools/asm.py (synthetic strings only).

Run: tools/docker.sh python3 tools/test_asm.py
"""
import unittest

import asm


class SjisEscape(unittest.TestCase):
    def conv(self, line):
        return asm.STRING_LINE.sub(asm.sjis_escape, line)

    def test_japanese_literal_becomes_octal_escapes(self):
        line = '    /* 0 80000000 */ .asciz "とき"'
        self.assertEqual(self.conv(line), '    /* 0 80000000 */ .asciz "\\202\\306\\202\\253"')

    def test_ascii_and_escapes_untouched(self):
        line = '    .string "abc\\n\\"x\\""'
        self.assertEqual(self.conv(line), line)

    def test_halfwidth_katakana_is_one_byte(self):
        self.assertEqual(self.conv('.ascii "ｱ"'), '.ascii "\\261"')

    def test_other_lines_untouched(self):
        line = "    /* 1 */ .word 0x12345678"
        self.assertEqual(self.conv(line), line)


if __name__ == "__main__":
    unittest.main()
