"""Invented counted glyph-list fixtures for T-28001."""
import struct
import unittest
from tools.assets.text import TextFormatError
from tools.assets.text_tables import parse_character_lists


class CharacterListTests(unittest.TestCase):
    def fixture(self):
        data = bytearray(64)
        struct.pack_into("<HH", data, 0, 2, 1)
        data[4:10] = "テト".encode("shift_jis") + b"\0\0"
        data[12:16] = "カ".encode("shift_jis") + b"\0\0"
        struct.pack_into("<II", data, 32, 4, 12)
        return bytes(data), {"id": "invented", "path": "TEST.NBN", "counts_offset": 0,
                             "counts_count": 2, "offsets_offset": 32, "offsets_count": 2,
                             "list_count": 2, "load_address": 0x80000000}

    def test_exact_counts_and_provenance(self):
        data, profile = self.fixture()
        parsed = parse_character_lists(data, profile)
        self.assertEqual(parsed["coverage"]["glyph_entries"], 3)
        self.assertEqual(parsed["records"][1]["offset"], 12)
        self.assertEqual(parsed["records"][0]["text"]["source_decoded_text"], "テト")
        self.assertEqual(bytes.fromhex(parsed["records"][0]["text"]["raw_hex"]), data[4:8])

    def test_bad_offsets_terminators_and_overlap_fail(self):
        data, profile = self.fixture()
        for offset, value in ((32, 31), (36, 6)):
            bad = bytearray(data)
            struct.pack_into("<I", bad, offset, value)
            with self.assertRaises(TextFormatError):
                parse_character_lists(bytes(bad), profile)
        bad = bytearray(data)
        bad[8] = 1
        with self.assertRaises(TextFormatError):
            parse_character_lists(bytes(bad), profile)

    def test_single_byte_kana_are_rejected(self):
        data, profile = self.fixture()
        bad = bytearray(data)
        bad[4:8] = bytes.fromhex("a1a2a3a4")
        with self.assertRaises(TextFormatError):
            parse_character_lists(bytes(bad), profile)


class DictionaryTests(unittest.TestCase):
    def test_pointer_and_fixed_width_terms_preserve_every_slot_byte(self):
        from tools.assets.text_tables import parse_dictionary
        data = bytearray(64)
        struct.pack_into("<I", data, 0, 0x1010)
        data[16:19] = "テ".encode("shift_jis") + b"\0"
        data[24:28] = "ト".encode("shift_jis") + b"\0\0"
        profile = {"id": "invented", "path": "TERMS.BIN", "load_address": 0x1000,
                   "header_size": 0, "tables": [
                       {"id": "pointers", "address": 0x1000, "count": 1, "kind": "pointer_strings"},
                       {"id": "slots", "address": 0x1018, "count": 1, "kind": "fixed_strings", "stride": 4}]}
        parsed = parse_dictionary(bytes(data), profile)
        self.assertEqual(parsed["coverage"]["records"], 2)
        self.assertEqual(parsed["records"][0]["offset"], 16)
        self.assertEqual(parsed["records"][1]["padding_raw_hex"], "00")
        bad = bytearray(data)
        struct.pack_into("<I", bad, 0, 0xFFFF)
        with self.assertRaises(TextFormatError):
            parse_dictionary(bytes(bad), profile)


if __name__ == "__main__":
    unittest.main()
