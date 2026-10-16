"""Synthetic literal tests for source-preserving K&R byte-escape normalization."""
import unittest
from draft_strings import octal_hex_escapes


class DraftStringTests(unittest.TestCase):
    def test_mixed_shift_jis_fragment_preserves_byte_escape_intent(self):
        source='f("パ'+r'\x83\\'+'コン占い");'
        fixed,changes=octal_hex_escapes(source)
        self.assertEqual(fixed,'f("パ'+r'\203\\'+'コン占い");')
        self.assertEqual([c['value'] for c in changes],[0x83])

    def test_comments_and_literal_backslash_are_unchanged(self):
        source=r'/* "\x83" */ f("\\x83"); // "\x95"'
        self.assertEqual(octal_hex_escapes(source),(source,[]))

    def test_characters_and_fixed_octal_width(self):
        source=r'''f('\xff', "\x1bZ");'''
        fixed,changes=octal_hex_escapes(source)
        self.assertEqual(fixed,r'''f('\377', "\033Z");''')
        self.assertEqual([c['value'] for c in changes],[255,27])

    def test_out_of_range_hex_escape_is_not_guessed(self):
        source=r'f("\x834a");'
        self.assertEqual(octal_hex_escapes(source),(source,[]))


if __name__=='__main__':
    unittest.main()
