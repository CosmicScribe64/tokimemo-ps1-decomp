"""Synthetic font-table and bitmap-link tests; no original font bytes."""
import struct
import unittest

from tools.assets.text import TextFormatError
from tools.assets.text_font import font_metadata, font_reference, attach_font_references


class FontTests(unittest.TestCase):
    def fixture(self):
        data = bytearray(128)
        struct.pack_into("<HHHHHH", data, 0, 0x8140, 0, 0x8180, 1, 0x8182, 3)
        profile = {"path": "INVENTED.EXE", "load_address": 0x80000000, "header_size": 0,
                   "range_address": 0x80000000, "range_count": 3,
                   "glyph_address": 0x80000020, "glyph_count": 4}
        return bytes(data), profile

    def test_font_source_offsets_and_lookup(self):
        data, profile = self.fixture()
        meta = font_metadata(data, profile)
        self.assertEqual(meta["glyph_offset"], 32)
        ref = font_reference(bytes.fromhex("8181"), meta)
        self.assertEqual(ref["glyph_index"], 2)
        self.assertEqual(ref["offset"], 68)
        self.assertEqual(ref["id"], "font:INVENTED.EXE:g0002")

    def test_loader_error_and_sentinel(self):
        data, profile = self.fixture()
        meta = font_metadata(data, profile)
        self.assertEqual(font_reference(b"\x09\x82", meta)["status"], "renderer_error_and_string_end")
        self.assertEqual(font_reference(bytes.fromhex("8182"), meta)["status"], "ignored_above_font_sentinel")
        self.assertEqual(font_reference(bytes.fromhex("8148"), meta)["status"], "renderer_read_outside_font")
        self.assertEqual(font_reference(b"\x81", meta)["status"], "terminal_byte_before_nul")

    def test_invalid_table_and_bank_fail(self):
        data, profile = self.fixture()
        bad = bytearray(data)
        struct.pack_into("<H", bad, 4, 0x8140)
        with self.assertRaises(TextFormatError):
            font_metadata(bytes(bad), profile)
        profile["glyph_count"] = 100
        with self.assertRaises(TextFormatError):
            font_metadata(data, profile)

    def test_catalog_tokens_share_bitmap_identity(self):
        data, profile = self.fixture()
        catalog = {"files": [{"script": {"records": [{"text": {"tokens": [
            {"kind": "glyph", "raw_hex": "8140"}, {"kind": "substitution", "code": "P"}]}}]}}]}
        result = attach_font_references(catalog, font_metadata(data, profile))
        tokens = result["files"][0]["script"]["records"][0]["text"]["tokens"]
        self.assertEqual(tokens[0]["font_reference"]["glyph_index"], 0)
        self.assertNotIn("font_reference", tokens[1])
        self.assertEqual(result["font_link_coverage"]["referenced_glyphs"], 1)


if __name__ == "__main__":
    unittest.main()
