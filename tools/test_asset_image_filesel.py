"""Invented FILESEL layout/palette fixtures; no patch or original game data."""
import struct
import tempfile
import unittest
from pathlib import Path
from tools.assets.image_packs_filesel import (FILESEL_SIZE, ACTOR_SHEETS, BACKGROUND_GROUPS,
                                              export_filesel_ui, probe_filesel_ui)
from tools.test_asset_images import png_pixels


def fixture():
    data = bytearray(FILESEL_SIZE)
    struct.pack_into('<4I', data, 0x4047c, *(0x80180000 + x for x in ACTOR_SHEETS))
    pointers = []
    for index in range(45):
        offset = 0x40000 + index * 24
        pointers.append(0x80180000 + offset)
        struct.pack_into('<4h', data, offset, 1, 4, 5, 0)
        struct.pack_into('<7h', data, offset + 8, 0, 0, 0, 0x1000, 0, 1, 1)
    struct.pack_into('<45I', data, 0x4048c, *pointers)
    for offset, word in ((0, 0x83e0), (0x22000, 0x1f), (0x22200, 0x7c00)):
        struct.pack_into('<H', data, offset + 2, word)
    for _, start, _, _, _ in BACKGROUND_GROUPS:
        for index in range(6):
            data[start + index * 0x4000] = 1
    for offset in ACTOR_SHEETS:
        data[offset] = 1
    return bytes(data)


class FileselTests(unittest.TestCase):
    def test_8bpp_depth_keeps_actor_16_pixel_cell_texture_addressing(self):
        data = bytearray(fixture())
        # Static FILESEL actors leave actor.unk_04 at zero, selecting the
        # 16-pixel search_tpage path even though quad_flags selects 8bpp color.
        struct.pack_into('<H', data, 0x40000 + 8 + 4, 0x49)  # page 1, u=16, v=16
        data[ACTOR_SHEETS[1] + 16 * 128 + 16] = 2
        struct.pack_into('<H', data, 0x22000 + 2 * 2, 0x03E0)
        with tempfile.TemporaryDirectory() as temporary:
            result = export_filesel_ui(bytes(data), None, temporary, 'multipage')
            frame = next(image for image in result['images'] if image['frame_index'] == 0)
            self.assertEqual(frame['parts'][0]['sheet_index'], 1)
            self.assertEqual((frame['parts'][0]['u'], frame['parts'][0]['v']), (16, 16))
            self.assertEqual((frame['parts'][0]['sheet_source_offset'], frame['palette_source_offset']),
                             (ACTOR_SHEETS[1], 0x22000))
            self.assertEqual(png_pixels(Path(temporary) / frame['png'])[1:5], b'\0\xff\0\xff')

    def test_actual_mode1_palettes_and_only_source_proved_selected_variants(self):
        data = fixture()
        self.assertEqual(probe_filesel_ui(data)['metadata']['parts'], 45)
        with tempfile.TemporaryDirectory() as temporary:
            result = export_filesel_ui(data, None, temporary, 'invented')
            self.assertEqual(len(result['images']), 73)
            pages = [image for image in result['images'] if image['preview_kind'] == 'source_texture_page']
            self.assertEqual(len(pages), 12)
            self.assertEqual(png_pixels(Path(temporary) / pages[0]['png'])[1:5], b'\0\xff\0\xff')
            self.assertEqual(png_pixels(Path(temporary) / pages[0]['stp_png'])[1], 255)
            self.assertEqual(png_pixels(Path(temporary) / pages[6]['png'])[1:5], b'\xff\0\0\xff')
            base = next(image for image in result['images'] if image['frame_index'] == 4 and image['palette_override'] == 0)
            selected = next(image for image in result['images'] if image['frame_index'] == 4 and image['palette_override'] == 0x11)
            self.assertEqual(base['clut_vram'], [0, 0x1f0])
            self.assertEqual(selected['clut_vram'], [0, 0x1f1])
            self.assertEqual(png_pixels(Path(temporary) / base['png'])[1:5], b'\xff\0\0\xff')
            self.assertEqual(png_pixels(Path(temporary) / selected['png'])[1:5], b'\0\0\xff\xff')
            self.assertFalse(any(i.get('palette_override') == 0x11 and i['frame_index'] == 0 for i in result['images']))
            self.assertEqual(result['metadata']['actor_sheet_offsets'][-1] + 0x4000, 0x44550)
            self.assertEqual(result['metadata']['unrepresented_extents'][-1]['offset'], 0x44550)

    def test_rejects_malformed_profile_texture_palette_and_geometry(self):
        data = fixture()
        self.assertEqual(probe_filesel_ui(data[:-1])['status'], 'invalid')
        for offset, fmt, value in ((0x4047c, '<I', 0x80180000),
                                    (0x4048c, '<I', 0x80180000),
                                    (0x40000, '<h', 65),
                                    (0x40008 + 6, '<H', 0x1100),
                                    (0x40008 + 10, '<h', 129)):
            bad = bytearray(data)
            struct.pack_into(fmt, bad, offset, value)
            self.assertEqual(probe_filesel_ui(bytes(bad))['status'], 'invalid')


if __name__ == '__main__':
    unittest.main()
