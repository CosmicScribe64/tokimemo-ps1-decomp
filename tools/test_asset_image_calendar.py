"""Invented bounded CALROM tiles/records and paired executable tests."""
import struct
import tempfile
import unittest
from pathlib import Path
from tools.assets.image_packs_calendar import (CALENDAR_SIZE, export_calendar_ui,
                                                probe_calendar_ui)
from tools.test_asset_image_packs import fixture as main_fixture
from tools.test_asset_images import png_pixels


def fixture():
    _, exe = main_fixture()
    exe = bytearray(exe)
    def put(address, fmt, *values):
        struct.pack_into(fmt, exe, address - 0x80041000 + 0x800, *values)
    put(0x800b3748, '<I', 0x801a0000)
    put(0x800c61a0, '<16H', 0, 31, *([0] * 14))
    put(0x800c6180, '<16H', 0, 0x3e0, *([0] * 14))
    pack = bytearray(CALENDAR_SIZE)
    for index in range(36):
        offset = 0x90 + index * 0x2100
        struct.pack_into('<I', pack, index * 4, 0x801a0000 + offset)
        struct.pack_into('<2I', pack, offset + 0x2000, 28, 3)
        pack[offset] = 0x01
        pack[offset + 24] = 0x01  # x48 uses label CLUT, distinct from grid CLUT.
    return bytes(pack), bytes(exe)


class CalendarTests(unittest.TestCase):
    def test_month_table_records_regions_and_separate_palettes(self):
        pack, exe = fixture()
        self.assertEqual(probe_calendar_ui(pack, exe)['status'], 'supported')
        with tempfile.TemporaryDirectory() as temporary:
            result = export_calendar_ui(pack, exe, temporary, 'invented')
            self.assertEqual(len(result['images']), 72)
            grid, label = result['images'][:2]
            self.assertEqual((grid['year'], grid['month']), (1995, 4))
            self.assertEqual((result['images'][-1]['year'], result['images'][-1]['month']), (1998, 3))
            self.assertEqual(grid['numeric_records']['day_count'], 28)
            self.assertEqual(grid['runtime_sprite_regions'][0], [0, 0, 126, 126])
            self.assertEqual(label['source_region'], [48, 0, 24, 16])
            self.assertEqual(png_pixels(Path(temporary) / grid['png'])[1:5], b'\xff\0\0\xff')
            self.assertEqual(png_pixels(Path(temporary) / label['png'])[1:5], b'\0\xff\0\xff')
            self.assertTrue(result['metadata']['uninterpreted_extents'])

    def test_rejects_bad_source_extents_overlaps_and_records(self):
        pack, exe = fixture()
        self.assertEqual(probe_calendar_ui(pack[:-1], exe)['status'], 'invalid')
        for offset, value in ((0, 0x801a0001), (4, 0x801a0090),
                              (0x90 + 0x2000, 32), (0x90 + 0x2004, 7)):
            bad = bytearray(pack)
            struct.pack_into('<I', bad, offset, value)
            self.assertEqual(probe_calendar_ui(bytes(bad), exe)['status'], 'invalid')


if __name__ == '__main__':
    unittest.main()
