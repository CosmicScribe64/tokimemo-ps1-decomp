"""Invented TITLE pointers/frames/CLUTs; no game data or text."""
import struct
import tempfile
import unittest
from pathlib import Path
from tools.assets.image_packs_title import (TITLE_SIZE, ACTOR_SHEETS, BACKGROUND_SHEETS,
                                            export_title_ui, probe_title_ui)
from tools.test_asset_images import png_pixels


def fixture():
    data = bytearray(TITLE_SIZE)
    struct.pack_into('<7I', data, 0x6ab20, *(0x80180000 + x for x in ACTOR_SHEETS))
    pointers = []
    for i in range(12):
        offset = 0x6a200 + i * 24
        pointers.append(0x80180000 + offset)
        struct.pack_into('<4h', data, offset, 1, 4, 5, 0)
        palette_row = 15 if i == 1 else 14
        struct.pack_into('<7h', data, offset + 8, 0, 0, 0, palette_row << 8, 0, 1, 1)
    struct.pack_into('<12I', data, 0x6ab3c, *pointers)
    struct.pack_into('<I', data, 0x6ab6c, 0x801eab00)
    sequence = [7]
    for frame in range(5, 12):
        sequence += [frame, 7]
    sequence += [0]
    struct.pack_into('<16H', data, 0x6ab00, *sequence)
    struct.pack_into('<H', data, 0x4c000 + 2, 0x3e0)
    struct.pack_into('<H', data, 0x4de00 + 2, 0x801f)
    struct.pack_into('<H', data, 0x4e000 + 2, 0x7c00)
    for offset in BACKGROUND_SHEETS + ACTOR_SHEETS:
        data[offset] = 1
    return bytes(data)


class TitleTests(unittest.TestCase):
    def test_established_palettes_background_pages_and_actor_parts(self):
        data = fixture()
        self.assertEqual(probe_title_ui(data)['metadata']['actor_parts'], 12)
        with tempfile.TemporaryDirectory() as temporary:
            result = export_title_ui(data, None, temporary, 'invented')
            self.assertEqual(len(result['images']), 18)
            page, actor = result['images'][0], result['images'][6]
            self.assertEqual(png_pixels(Path(temporary) / page['png'])[1:5], b'\0\xff\0\xff')
            self.assertEqual(png_pixels(Path(temporary) / page['stp_png'])[1], 255)
            self.assertEqual(png_pixels(Path(temporary) / actor['png'])[1:5], b'\xff\0\0\xff')
            self.assertEqual(png_pixels(Path(temporary) / actor['stp_png'])[1], 0)
            self.assertEqual(actor['parts'][0]['clut_vram'], [0, 0x1fe])
            second_actor = result['images'][7]
            self.assertEqual(second_actor['parts'][0]['clut_vram'], [0, 0x1ff])
            self.assertEqual(png_pixels(Path(temporary) / second_actor['png'])[1:5], b'\0\0\xff\xff')
            self.assertEqual(png_pixels(Path(temporary) / page['png'])[5:9], b'\0\0\0\0')
            self.assertEqual(result['images'][3]['runtime_bg_cell']['v'], 120)
            self.assertEqual(result['images'][3]['vram_rectangle_words'][1], 128)
            self.assertEqual(len(result['metadata']['unmapped_model_palette_uploads']), 13)
            self.assertIn('13 model pages', result['metadata']['coverage_policy'])
            gap = result['metadata']['uninterpreted_frame_record_gaps'][0]
            self.assertEqual((gap['offset'], gap['size']), (0x6a216, 2))

    def test_rejects_size_pointer_unknown_palette_and_bad_geometry(self):
        data = fixture()
        self.assertEqual(probe_title_ui(data[:-1])['status'], 'invalid')
        for offset, fmt, value in ((0x6ab20, '<I', 0x80180000),
                                    (0x6a200, '<h', 65),
                                    (0x6a208 + 6, '<H', 13 << 8),
                                    (0x6a208 + 10, '<h', 129),
                                    (0x6ab00, '<H', 8)):
            bad = bytearray(data)
            struct.pack_into(fmt, bad, offset, value)
            self.assertEqual(probe_title_ui(bytes(bad))['status'], 'invalid')


if __name__ == '__main__':
    unittest.main()
