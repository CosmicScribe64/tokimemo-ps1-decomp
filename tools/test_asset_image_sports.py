"""Invented sports tile maps and CLUTs; no original pixels or text."""
import struct
import tempfile
import unittest
from pathlib import Path
from tools.assets.image_packs_sports import LOAD_BASE, PROFILES, export_sports_ui, probe_sports_ui
from tools.assets.images import ImageFormatError, _color
from tools.test_asset_images import png_pixels


def fixture(name='SPSETU'):
    size, descriptors, _ = PROFILES[name]
    data = bytearray(size)
    for i in range(256):
        struct.pack_into('<H', data, i * 2, (i & 31) | (0x8000 if i & 32 else 0))
    for branch, (table, mod, screen) in enumerate(descriptors):
        pages = ([0xe000, 0x12000] if branch == 0 else [0x16000, 0x1a000]) if name == 'NINSETU' else [0x2000, 0x6000]
        struct.pack_into('<2I', data, table, *(LOAD_BASE + offset for offset in pages))
        struct.pack_into('<4I', data, mod, 2, 0x04040808, 0x400, 2)
        for i in range(16):
            struct.pack_into('<I', data, mod + 16 + (16 + i) * 4, 0x0000a500 | (i % 4))
        grid = 16 if name in ('KASETU', 'TASETU') else 512
        struct.pack_into('<5I', data, screen, 0x303, 0x04042020,
                         0x08080201 if grid == 16 else 0x08084001, 0x3000, 0xe0a)
        for y in range(8):
            for x in range(10):
                struct.pack_into('<I', data, screen + 20 + (y * grid + x) * 4, 1)
    for y in range(8):
        for x in range(8):
            data[0x2000 + y * 128 + x] = y * 8 + x
    return bytes(data)


class SportsImageTests(unittest.TestCase):
    def test_clut_flip_viewport_and_original_stp(self):
        data = fixture()
        self.assertEqual(probe_sports_ui(data, 'SPSETU.BIN')['status'], 'supported')
        with tempfile.TemporaryDirectory() as temporary:
            result = export_sports_ui(data, None, temporary, 'invented', source_path='SPSETU.BIN')
            image = result['images'][0]
            rgba = png_pixels(Path(temporary) / image['png'])[1:]
            mask = png_pixels(Path(temporary) / image['stp_png'])[1:]
            palette = struct.unpack_from('<256H', data)
            # Each tile selects the same glyph, but flags alter its source coordinate.
            for x, index in ((0, 0), (8, 56), (16, 7), (24, 63)):
                self.assertEqual(rgba[x * 4:x * 4 + 4], _color(palette[index]))
                self.assertEqual(mask[x], 255 if palette[index] & 0x8000 else 0)
            self.assertEqual((image['width'], image['height']), (320, 240))
            self.assertEqual(image['clut_vram'], [0, 0x1f0])
            self.assertEqual(image['parts'][0]['ignored_mod_palette_byte'], 0xa5)
            self.assertEqual(image['cropped_source_rows']['height'], 16)
            with self.assertRaises(FileExistsError):
                export_sports_ui(data, None, temporary, 'invented', source_path='SPSETU.BIN')

    def test_all_profiles_and_fail_closed_map_and_source_bounds(self):
        for name in PROFILES:
            self.assertEqual(probe_sports_ui(fixture(name), name + '.BIN')['status'], 'supported')
        original = fixture()
        self.assertEqual(probe_sports_ui(original, 'unknown.BIN')['status'], 'invalid')
        self.assertEqual(probe_sports_ui(original[:-1], 'SPSETU.BIN')['status'], 'invalid')
        for offset, value in ((0xf190 + 20, 0), (0xf190 + 20, 2),
                              (0x129cc, LOAD_BASE + 0xa000),
                              (0x129cc, LOAD_BASE + len(original) - 1), (0xe000, 0x01000002),
                              (0xe000 + 16, 0x02000000)):
            changed = bytearray(original)
            struct.pack_into('<I', changed, offset, value)
            self.assertEqual(probe_sports_ui(changed, 'SPSETU.BIN')['status'], 'invalid')
        with tempfile.TemporaryDirectory() as temporary:
            with self.assertRaises(ImageFormatError):
                export_sports_ui(original, None, temporary, '../escape', source_path='SPSETU.BIN')


if __name__ == '__main__':
    unittest.main()
