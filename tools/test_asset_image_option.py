"""Synthetic OPTION.NBN profile tests; no game artwork or strings."""
import os
import struct
import tempfile
import unittest
import zlib

from tools.assets.image_packs_option import (
    FRAME_COUNT, FRAME_TABLE_OFFSET, LOAD_ADDRESS, OPTION_SIZE, TPAGE_TABLE_OFFSET,
    export_option_ui, probe_option_ui,
)


def synthetic_option():
    data = bytearray(OPTION_SIZE)
    page_offsets = (0x200, 0xC200, 0x13CF0, 0x8200)
    struct.pack_into("<4I", data, TPAGE_TABLE_OFFSET,
                     *(LOAD_ADDRESS + offset for offset in page_offsets))
    record_start = 0x10200
    record_size = 8 + 14
    for index in range(FRAME_COUNT):
        record_offset = record_start + index * record_size
        struct.pack_into("<I", data, FRAME_TABLE_OFFSET + index * 4, LOAD_ADDRESS + record_offset)
        struct.pack_into("<4h", data, record_offset, 1, 0, 0, 0)
        struct.pack_into("<2h3H2h", data, record_offset + 8,
                         0, 0, 0x0300, 0x2000, 0, 1, 1)
    # One synthetic 8bpp pixel and a synthetic red source CLUT entry.
    data[0x8200] = 5
    struct.pack_into("<256H", data, 0x134F0, *([0] * 5 + [0x801F] + [0] * 250))
    return bytes(data)


class OptionImagePackTests(unittest.TestCase):
    def test_probe_and_export_keep_source_backed_geometry_and_palette(self):
        data = synthetic_option()
        self.assertEqual(probe_option_ui(data)["metadata"]["frames"], FRAME_COUNT)
        with tempfile.TemporaryDirectory() as output:
            result = export_option_ui(data, output, "synthetic-option")
            self.assertEqual(result["status"], "exported")
            self.assertEqual(len(result["images"]), FRAME_COUNT)
            self.assertEqual(len(result["files"]), FRAME_COUNT * 2)
            self.assertEqual(result["metadata"]["source_lba"], 0x9EA4)
            self.assertEqual(result["images"][0]["parts"][0]["clut_row"], 0x1F0)
            self.assertEqual(result["images"][0]["parts"][0]["palette_source_offset"], 0x134F0)
            self.assertEqual(result["images"][0]["parts"][0]["palette_mode"], 1)
            self.assertEqual(result["images"][0]["parts"][0]["source_palette_words"],
                             result["images"][0]["parts"][0]["preview_palette_words"])
            self.assertEqual(result["metadata"]["texture_pages"][3]["source_offset"], 0x8200)
            self.assertIn("partial/unknown", result["metadata"]["texture_pages"][0]["coverage"])
            gap = result["metadata"]["frame_table_padding"]
            self.assertEqual(gap["offset"] + gap["size"], TPAGE_TABLE_OFFSET)
            self.assertEqual(len(gap["sha256"]), 64)
            self.assertEqual(result["metadata"]["uninterpreted_extents"][0]["offset"], 0x4200)
            self.assertEqual(result["metadata"]["uninterpreted_extents"][0]["size"], 0x4000)
            self.assertEqual(result["metadata"]["direct_texture_page_upload"]["source_offset"], 0x17CF0)
            with open(os.path.join(output, result["files"][0]), "rb") as image:
                self.assertTrue(image.read(8).startswith(b"\x89PNG\r\n\x1a\n"))
            with open(os.path.join(output, result["files"][1]), "rb") as mask_file:
                mask = mask_file.read()
            self.assertEqual(mask[25], 0)  # grayscale PNG, one channel
            offset, compressed = 8, bytearray()
            while offset < len(mask):
                size = struct.unpack_from(">I", mask, offset)[0]
                chunk_type = mask[offset + 4:offset + 8]
                if chunk_type == b"IDAT":
                    compressed.extend(mask[offset + 8:offset + 8 + size])
                offset += 12 + size
            self.assertEqual(zlib.decompress(compressed), b"\x00\xff")
            with self.assertRaises(FileExistsError):
                export_option_ui(data, output, "synthetic-option")

    def test_invalid_pointer_table_is_rejected(self):
        data = bytearray(synthetic_option())
        struct.pack_into("<I", data, FRAME_TABLE_OFFSET, LOAD_ADDRESS + OPTION_SIZE)
        self.assertEqual(probe_option_ui(bytes(data))["status"], "invalid")

    def test_unknown_palette_row_is_preserved_as_unresolved(self):
        data = bytearray(synthetic_option())
        first_frame = struct.unpack_from("<I", data, FRAME_TABLE_OFFSET)[0] - LOAD_ADDRESS
        struct.pack_into("<H", data, first_frame + 8 + 6, 0x2400)
        with tempfile.TemporaryDirectory() as output:
            result = export_option_ui(bytes(data), output, "synthetic-option")
            self.assertEqual(result["status"], "partial")
            self.assertEqual(len(result["images"]), FRAME_COUNT - 1)
            self.assertEqual(result["metadata"]["unresolved_frames"][0]["frame_index"], 0)


if __name__ == "__main__":
    unittest.main()
