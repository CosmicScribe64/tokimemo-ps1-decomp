"""Invented EXAMINE frame/palette fixtures; no original game bytes (Docker)."""
from pathlib import Path
import struct
import tempfile
import unittest

from assets.image_packs_exam import SOURCE_SIZE, LOAD_BASE, export_exam_ui, probe_exam_ui
from assets.images import ImageFormatError
from tools.test_asset_images import png_pixels


def fixture():
    data = bytearray(SOURCE_SIZE)
    data[0] = 0x01
    struct.pack_into('<3I', data, 0x6494, *(LOAD_BASE + x for x in (0, 0x2000, 0x4000)))
    for index in range(32):
        offset = 0x6000 + index * 24
        struct.pack_into('<I', data, 0x64A0 + index * 4, LOAD_BASE + offset)
        struct.pack_into('<4h', data, offset, 1, 0, 0, 0)
        struct.pack_into('<2h3H2h', data, offset + 8, 0, 0, 0, 0, 2, 2, 1)
    exe = bytearray(0x90000)
    exe[:8] = b'PS-X EXE'
    struct.pack_into('<II', exe, 24, 0x80041000, len(exe) - 0x800)
    struct.pack_into('<16H', exe, 0x800C6020 - 0x80041000 + 0x800, 0, 31, *([0] * 14))
    return bytes(data), bytes(exe)


class ExamImageTests(unittest.TestCase):
    def test_original_palette_flip_and_explicit_unrepresented_extents(self):
        data, exe = fixture()
        self.assertEqual(probe_exam_ui(data, exe)['metadata'], {'frames': 32, 'parts': 32})
        with tempfile.TemporaryDirectory() as temporary:
            result = export_exam_ui(data, exe, temporary, 'invented')
            self.assertEqual(len(result['images']), 32)
            image = result['images'][0]
            self.assertEqual(png_pixels(Path(temporary) / image['png']), b'\0\0\0\0\0\xff\0\0\xff')
            self.assertEqual(image['parts'][0]['clut_vram'], [0, 0x1E0])
            self.assertEqual(result['metadata']['unrepresented_extents'][0]['offset'], 0x6520)
            self.assertTrue(result['metadata']['uninterpreted_frame_record_gaps'])
            self.assertFalse(result['metadata']['round_trip_verified'])

    def test_rejects_unproved_tables_and_bad_geometry(self):
        data, exe = fixture()
        self.assertEqual(probe_exam_ui(data[:-1], exe)['status'], 'invalid')
        for offset, fmt, value in ((0x6494, '<I', LOAD_BASE + 1), (0x64A0, '<I', LOAD_BASE),
                                    (0x6008 + 10, '<h', 129)):
            bad = bytearray(data)
            struct.pack_into(fmt, bad, offset, value)
            self.assertEqual(probe_exam_ui(bytes(bad), exe)['status'], 'invalid')
        with tempfile.TemporaryDirectory() as temporary, self.assertRaises(ImageFormatError):
            export_exam_ui(data, exe, temporary, '../escape')


if __name__ == '__main__':
    unittest.main()
