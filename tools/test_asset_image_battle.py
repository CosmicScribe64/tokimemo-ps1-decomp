"""Invented-data tests for the bounded RPG_BAT source page reader."""
import struct
import tempfile
import unittest
import zlib
from pathlib import Path

from tools.assets.image_packs_battle import (
    CROP_REGIONS, LOAD_BASE, PAGE_BYTES, PROFILES, export_battle_ui,
    probe_battle_ui,
)
from tools.assets.images import ImageFormatError


def _fake(name='VS_HABU.BIN'):
    size, sheet_table, frame_table, animation_table, palette_offset, page_offset, _ = PROFILES[name]
    data = bytearray(size)
    struct.pack_into('<I', data, sheet_table, LOAD_BASE + page_offset)
    frame = 0x100
    # All 66 table entries point at one invented, bounded 8bpp part.
    struct.pack_into('<66I', data, frame_table, *([LOAD_BASE + frame] * 66))
    struct.pack_into('<4h', data, frame, 1, 0, 0, 0)
    struct.pack_into('<2h3H2h', data, frame + 8, 0, 0, 12, 0, 0, 16, 16)
    page = bytearray(PAGE_BYTES)
    page[:] = bytes([1]) * PAGE_BYTES
    page[1 * 128 + 99] = 225
    data[page_offset:page_offset + PAGE_BYTES] = page
    palette = [0] * 256
    palette[1] = 0x001F
    palette[225] = 0x03E0
    struct.pack_into('<256H', data, palette_offset, *palette)
    return bytes(data)


def _png_gray_samples(path):
    payload = path.read_bytes()
    width, height = struct.unpack_from('>II', payload, 16)
    cursor, compressed = 8, bytearray()
    while cursor < len(payload):
        length = struct.unpack_from('>I', payload, cursor)[0]
        kind = payload[cursor + 4:cursor + 8]
        chunk = payload[cursor + 8:cursor + 8 + length]
        cursor += 12 + length
        if kind == b'IDAT':
            compressed.extend(chunk)
    raw = zlib.decompress(compressed)
    return b''.join(raw[row * (width + 1) + 1:(row + 1) * (width + 1)] for row in range(height))


class BattleImageTests(unittest.TestCase):
    def test_exact_profile_probe_and_export_regions(self):
        data = _fake()
        result = probe_battle_ui(data, 'CDROM/BATTLE/VS_HABU.BIN')
        self.assertEqual(result['status'], 'supported')
        self.assertEqual(result['candidates'][0]['frame_count'], 66)
        with tempfile.TemporaryDirectory() as temp:
            output = export_battle_ui(data, None, temp, 'habu', source_path='VS_HABU.BIN')
            self.assertEqual(output['status'], 'exported')
            self.assertEqual(len(output['images']), 9)
            self.assertEqual(len(output['files']), 18)
            variants = [image['variant'] for image in output['images']]
            self.assertEqual(len(set(variants)), 9)
            self.assertEqual(variants[0], 'source-page-0')
            self.assertEqual(variants[1:], ['locator-region-%02d' % i for i in range(8)])
            self.assertEqual(output['images'][0]['preview_kind'], 'source_texture_page')
            self.assertTrue(all(image['preview_kind'] == 'source_region_crop' for image in output['images'][1:]))
            self.assertTrue(all(image['palette_status'] == 'caller-established' for image in output['images']))
            self.assertTrue(all(image['source_sheet_offset'] == PROFILES['VS_HABU.BIN'][5]
                                and image['source_sheet_size'] == PAGE_BYTES
                                for image in output['images']))
            crop = next(i for i in output['images'] if i['source_region'].get('numeric_locator_region') == list(CROP_REGIONS[0]))
            self.assertEqual(crop['source_region']['y'], 1)
            self.assertEqual(crop['source_region']['source_offset'], PROFILES['VS_HABU.BIN'][5] + 128 + 99)
            self.assertTrue(crop['intersecting_frame_parts'])
            self.assertEqual(crop['intersecting_frame_parts'][0]['palette_row'], 0)
            self.assertEqual(crop['palette_mode'], 4)
            self.assertEqual(crop['preview_palette_words'][1], 0x001F)
            self.assertEqual(crop['preview_palette_words'][225], 0x83E0)
            self.assertEqual(_png_gray_samples(Path(temp) / crop['stp_png']), bytes([255]) + bytes(26 * 12 - 1))
            self.assertEqual(Path(temp, crop['png']).read_bytes()[:8], b'\x89PNG\r\n\x1a\n')

    def test_all_five_names_are_profiled(self):
        for name in PROFILES:
            with self.subTest(name=name):
                self.assertEqual(probe_battle_ui(_fake(name), name)['status'], 'supported')

    def test_unprofiled_battle_source_is_unsupported(self):
        result = probe_battle_ui(b'not-a-game-container', 'CDROM/BATTLE/UTYU.BIN')
        self.assertEqual(result['status'], 'unsupported')
        self.assertEqual(result['candidates'], [])

    def test_wrong_page_pointer_is_invalid(self):
        data = bytearray(_fake())
        struct.pack_into('<I', data, PROFILES['VS_HABU.BIN'][1], LOAD_BASE + 0x80)
        self.assertEqual(probe_battle_ui(bytes(data), 'VS_HABU.BIN')['status'], 'invalid')

    def test_unprofiled_page_palette_row_is_invalid(self):
        data = bytearray(_fake())
        struct.pack_into('<H', data, 0x108 + 6, 0x0100)
        self.assertEqual(probe_battle_ui(bytes(data), 'VS_HABU.BIN')['status'], 'invalid')

    def test_frame_page_index_cannot_exceed_five_pointer_table_entries(self):
        data = bytearray(_fake())
        struct.pack_into('<H', data, 0x108 + 4, 0x0500)
        self.assertEqual(probe_battle_ui(bytes(data), 'VS_HABU.BIN')['status'], 'invalid')

    def test_asset_id_is_single_safe_component(self):
        with tempfile.TemporaryDirectory() as temp:
            with self.assertRaises(ImageFormatError):
                export_battle_ui(_fake(), None, temp, '../escape', source_path='VS_HABU.BIN')

    def test_output_files_do_not_overwrite(self):
        with tempfile.TemporaryDirectory() as temp:
            export_battle_ui(_fake(), None, temp, 'habu', source_path='VS_HABU.BIN')
            with self.assertRaises(FileExistsError):
                export_battle_ui(_fake(), None, temp, 'habu', source_path='VS_HABU.BIN')


if __name__ == '__main__':
    unittest.main()
