"""Invented name-entry tables/CLUTs, with no original glyphs or game strings."""
import struct
import tempfile
import unittest
from pathlib import Path
from tools.assets.image_packs_name import NAME_SIZE, LOAD_BASE, export_name_ui, probe_name_ui
from tools.test_asset_images import png_pixels


def fixture():
    data = bytearray(NAME_SIZE)
    struct.pack_into('<I', data, 0x1af58, LOAD_BASE + 0x200)
    cursor = 0x1a400
    for index in range(29):
        offset = cursor
        count = 65 if index == 1 else 1
        struct.pack_into('<I', data, 0x1af5c + index * 4, LOAD_BASE + offset)
        struct.pack_into('<4h', data, offset, count, 4, 5, 0)
        palette = 0x08f0 if index == 1 else 0x0800
        for part in range(count):
            struct.pack_into('<2h3H2h', data, offset + 8 + part * 14, 0, 0, 0, palette, 2, 2, 1)
        cursor = (offset + 8 + count * 14 + 3) & ~3
    data[0x200] = 0x01
    struct.pack_into('<H', data, 2, 0x801f)
    struct.pack_into('<H', data, 241 * 2, 0x7c00)
    struct.pack_into('<H', data, 0x1a200 + 2, 0x3e0)
    struct.pack_into('<H', data, 0x1a200 + 240 * 2, 0x7c00)
    for index in range(6):
        offset = 0x2200 + index * 0x4000
        data[offset:offset + 2] = b'\x01\xf0'
    return bytes(data)


class NameImageTests(unittest.TestCase):
    def test_runtime_override_retains_column_and_mode2_stp_and_background_depth(self):
        data = fixture()
        self.assertEqual(probe_name_ui(data)['metadata']['actor_page_offset'], 0x200)
        with tempfile.TemporaryDirectory() as temporary:
            result = export_name_ui(data, None, temporary, 'invented')
            self.assertEqual(len(result['images']), 35)
            background, actor, last_column = result['images'][0], result['images'][6], result['images'][7]
            self.assertEqual(background['bits_per_pixel'], 8)
            self.assertEqual(png_pixels(Path(temporary) / background['png'])[1:9], b'\0\xff\0\xff\0\0\xff\xff')
            self.assertEqual(png_pixels(Path(temporary) / background['stp_png'])[1:3], b'\0\xff')
            self.assertEqual(actor['parts'][0]['clut_vram'], [0, 0x1f0])
            self.assertEqual(last_column['parts'][0]['clut_vram'], [240, 0x1f0])
            self.assertEqual(len(last_column['parts']), 65)
            self.assertEqual(png_pixels(Path(temporary) / actor['png']), b'\0\0\0\0\0\xff\0\0\xff')
            self.assertEqual(png_pixels(Path(temporary) / last_column['png']), b'\0\0\0\0\0\0\0\xff\xff')
            self.assertEqual(png_pixels(Path(temporary) / last_column['stp_png']), b'\0\0\xff')
            self.assertEqual(result['metadata']['unrepresented_extents'][0]['offset'], 0x1afd0)

    def test_empty_frame_is_explicit_and_malformed_extents_are_rejected(self):
        data = fixture()
        empty = bytearray(data)
        struct.pack_into('<h', empty, 0x1a400, 0)
        with tempfile.TemporaryDirectory() as temporary:
            result = export_name_ui(bytes(empty), None, temporary, 'invented-empty')
            self.assertEqual(result['metadata']['empty_source_frames'], [0])
            self.assertEqual(len(result['images']), 34)
        self.assertEqual(probe_name_ui(data[:-1])['status'], 'invalid')
        for offset, fmt, value in ((0x1af58, '<I', LOAD_BASE + NAME_SIZE),
                                    (0x1af5c, '<I', LOAD_BASE),
                                    (0x1a408 + 4, '<H', 256),
                                    (0x1a408 + 10, '<h', 129)):
            bad = bytearray(data)
            struct.pack_into(fmt, bad, offset, value)
            self.assertEqual(probe_name_ui(bytes(bad))['status'], 'invalid')


if __name__ == '__main__':
    unittest.main()
