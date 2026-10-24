"""Invented MAINROM+EXE profile tests; no original game bytes."""
import struct
import tempfile
import unittest
from pathlib import Path
from tools.assets.image_packs import (FRAME_COUNT, FRAME_TABLE, PACK_SIZE,
                                     TEXTURE_OFFSETS, export_main_ui, probe_main_ui)
from tools.assets.images import ImageFormatError
from tools.test_asset_images import png_pixels


def fixture():
    base = 0x80041000
    exe = bytearray(0x90000)
    exe[:8] = b'PS-X EXE'
    struct.pack_into('<II', exe, 24, base, len(exe) - 0x800)
    def put(address, fmt, *values):
        struct.pack_into(fmt, exe, address - base + 0x800, *values)
    put(0x800c9730, '<11I', *(0x80180000 + o for o in TEXTURE_OFFSETS))
    put(0x800c5fcc, '<2I', 0x80186000, 0x80190000)
    put(FRAME_TABLE, '<174I', *([0x800c7420] * FRAME_COUNT))
    put(0x800c7420, '<4h', 1, 3, 4, 0)
    put(0x800c7428, '<7h', 0, 0, 0, 0, 2, 2, 1)
    put(0x800c6020, '<16H', 0, 31, *([0] * 14))
    put(0x800c9a14, '<16I', *([0x800c95e0] * 16))
    put(0x800c95e0, '<3H', 1, 0, 6)
    pack = bytearray(PACK_SIZE)
    pack[0x8000] = 0x01  # red, transparent; flip-x reverses the preview.
    return bytes(pack), bytes(exe)


class MainUiTests(unittest.TestCase):
    def test_invented_profile_parts_palettes_flip_and_source_mapping(self):
        pack, exe = fixture()
        info = probe_main_ui(pack, exe)
        self.assertEqual(info['metadata'], {'frames': 174, 'sequences': 16, 'parts': 174})
        with tempfile.TemporaryDirectory() as temporary:
            result = export_main_ui(pack, exe, temporary, 'invented')
            image = result['images'][0]
            self.assertEqual(image['origin'], [3, 4])
            self.assertEqual(image['parts'][0]['sheet_source_offset'], 0x8000)
            self.assertEqual(image['parts'][0]['clut_vram'], [0, 0x1e0])
            self.assertEqual(png_pixels(Path(temporary) / image['png']), b'\0\0\0\0\0\xff\0\0\xff')
            self.assertEqual(len(result['images']), 174 * 6 + 12)
            diagnostic = result['images'][-1]
            self.assertIsNone(diagnostic['frame_index'])
            self.assertEqual(diagnostic['palette_status'], 'unknown')
            self.assertEqual(diagnostic['preview_kind'], 'source_index_diagnostic')
            self.assertNotIn('stp_png', diagnostic)
            self.assertEqual(result['metadata']['trailing_extent']['offset'], 0x1a000)
            self.assertEqual(result['metadata']['frame_table_referenced_sheet_offsets'], [0x8000])
            self.assertEqual(len(result['metadata']['unmapped_sheet_extents']), 12)
            self.assertIn('unrepresented pixels', result['metadata']['coverage_policy'])
            self.assertFalse(result['metadata']['round_trip_verified'])

    def test_rejects_profile_extent_pointer_and_invalid_geometry(self):
        pack, exe = fixture()
        self.assertEqual(probe_main_ui(pack[:-1], exe)['status'], 'invalid')
        for address, fmt, value in ((0x800c9730, '<I', 0x80190001),
                                    (FRAME_TABLE, '<I', 0x80041000),
                                    (0x800c7428 + 10, '<h', 129)):
            bad = bytearray(exe)
            struct.pack_into(fmt, bad, address - 0x80041000 + 0x800, value)
            self.assertEqual(probe_main_ui(pack, bytes(bad))['status'], 'invalid')
        with self.assertRaises(ImageFormatError):
            export_main_ui(pack, exe, '/unused', '../escape')


if __name__ == '__main__':
    unittest.main()
