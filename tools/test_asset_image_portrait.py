"""Invented-data tests for exact BUSTUP portrait profile and active CLUT."""
import struct
import tempfile
import unittest
from pathlib import Path
from tools.assets.image_packs_portrait import (PORTRAIT_SIZE, SHEET_OFFSETS,
                                               export_portrait, probe_portrait)
from tools.test_asset_image_packs import fixture as main_fixture
from tools.test_asset_images import png_pixels


def fixture():
    _, exe = main_fixture()
    exe = bytearray(exe)
    struct.pack_into('<5I', exe, 0x800ca120 - 0x80041000 + 0x800,
                     0x80180084, 0x80180098, 0x8018009c, 0, 0x801800ac)
    data = bytearray(PORTRAIT_SIZE)
    struct.pack_into('<4h', data, 0, 6, 4, 5, 0)
    for index in range(6):
        struct.pack_into('<7h', data, 8 + index * 14, index, 0, 0, 0x1000, 0, 1, 1)
    for offset in (0x5c, 0x70):
        struct.pack_into('<10H', data, offset, 4, 0, 1, 0, 1, 0, 1, 0, 2, 0)
    struct.pack_into('<5I', data, 0x84, *(0x80180000 + o for o in SHEET_OFFSETS))
    struct.pack_into('<3I', data, 0x98, 0x80180000, 0x8018005c, 0x80180070)
    struct.pack_into('<H', data, 0xac + 2, 0x801f)
    struct.pack_into('<H', data, 0xac + 240 * 2, 0x3e0)
    for offset in SHEET_OFFSETS:
        data[offset:offset + 2] = b'\x01\xf0'
    return bytes(data), bytes(exe)


class PortraitTests(unittest.TestCase):
    def test_active_palette_actual_transform_pages_and_static_geometry(self):
        data, exe = fixture()
        self.assertEqual(probe_portrait(data, exe)['status'], 'supported')
        with tempfile.TemporaryDirectory() as temporary:
            result = export_portrait(data, exe, temporary, 'invented')
            self.assertEqual(len(result['images']), 6)
            page, frame = result['images'][0], result['images'][-1]
            self.assertEqual(page['variant'], 'source-page-0')
            self.assertEqual(frame['preview_kind'], 'static_frame_composite')
            self.assertEqual(frame['bounds'], [0, 0, 6, 1])
            self.assertEqual(png_pixels(Path(temporary) / page['png'])[1:9], b'\xff\0\0\xff\0\xff\0\xff')
            self.assertEqual(png_pixels(Path(temporary) / page['stp_png'])[1:3], b'\0\xff')
            self.assertEqual(result['metadata']['source_palette_words'][1], 0x801f)
            self.assertEqual(result['metadata']['preview_palette_words'][1], 0x1f)
            self.assertEqual(result['metadata']['preview_palette_words'][240], 0x83e0)
            self.assertFalse(result['metadata']['round_trip_verified'])

    def test_rejects_unknown_pointers_parts_palettes_sequences_and_extent(self):
        data, exe = fixture()
        self.assertEqual(probe_portrait(data[:-1], exe)['status'], 'invalid')
        for offset, fmt, value in ((0x84, '<I', 0x80180000), (0, '<h', 5),
                                    (8 + 4, '<H', 1024), (8 + 6, '<H', 0x1100),
                                    (0x5c, '<H', 8)):
            bad = bytearray(data)
            struct.pack_into(fmt, bad, offset, value)
            self.assertEqual(probe_portrait(bytes(bad), exe)['status'], 'invalid')


if __name__ == '__main__':
    unittest.main()
