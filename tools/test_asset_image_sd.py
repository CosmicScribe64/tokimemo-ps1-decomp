"""Invented source-bound SD animation fixtures; run in Docker."""
import hashlib
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

from assets.image_packs_sd import LOAD_BASE, export_sd_ui, probe_sd_ui, sd_profile_rows
from assets.images import ImageFormatError
from tools.test_asset_images import png_pixels


def fixture():
    data = bytearray(0x14800)
    struct.pack_into('<256H', data, 0, 0, 31, 0x83E0, *([0] * 253))
    struct.pack_into('<4h', data, 0x200, 1, 0, 0, 0)
    struct.pack_into('<2h3H2h', data, 0x208, 0, 0, 0, 0xFE0, 2, 2, 1)
    struct.pack_into('<4h', data, 0x218, 1, 0, 0, 0)
    struct.pack_into('<2h3H2h', data, 0x220, 0, 0, 0x41, 0xFE0, 0, 1, 1)
    struct.pack_into('<3H', data, 0x230, 1, 0, 5)
    struct.pack_into('<3H', data, 0x238, 1, 1, 7)
    struct.pack_into('<2I', data, 0x2A0, LOAD_BASE + 0x2C0, LOAD_BASE + 0x42C0)
    struct.pack_into('<2I', data, 0x2A8, LOAD_BASE + 0x200, LOAD_BASE + 0x218)
    struct.pack_into('<2I', data, 0x2B0, LOAD_BASE + 0x230, LOAD_BASE + 0x238)
    data[0x2C0] = 1
    data[0x42C0 + 16] = 2
    exe = bytearray(0x90000)
    exe[:8] = b'PS-X EXE'
    struct.pack_into('<II', exe, 24, 0x80041000, len(exe) - 0x800)
    struct.pack_into('<I', exe, 0x800B67AC - 0x80041000 + 0x800, 100)
    struct.pack_into('<3H', exe, 0x800C9F60 - 0x80041000 + 0x800, 0x2A0, 0x2A8, 0x2B0)
    entry = {'path': 'INVENTED_SD.NBN', 'size': len(data), 'sha256': hashlib.sha256(data).hexdigest(),
             'location': {'lba': 100}}
    return bytes(data), bytes(exe), entry


class SdImageTests(unittest.TestCase):
    def test_else_native_lba_rows_bind_each_original_executable_layout(self):
        _data, exe, _entry = fixture()
        overlay = bytearray(0x30000)
        base, table = 0x80132000, 0x80150720
        struct.pack_into('<3I', overlay, table - base - 4, 0x7D78, 0x7D81, 0x7D8A)
        digest = hashlib.sha1(overlay).hexdigest()
        with patch('assets.image_packs_photos.ETC_SHA1', digest):
            rows = sd_profile_rows(exe, etc_overlay_data=bytes(overlay))
        else_rows = [row for row in rows if row['family'] == 'else']
        self.assertEqual([(row['index'], row['source_lba'], row['layout_table_exe_address'])
                          for row in else_rows[-2:]], [(0, 0x7D78, 0x800CA108),
                                                       (1, 0x7D81, 0x800CA10E)])
        self.assertEqual([row['lba_table_overlay_vma'] for row in else_rows[-2:]],
                         [0x8015071C, 0x80150720])
        with self.assertRaises(ImageFormatError):
            sd_profile_rows(exe, etc_overlay_data=bytes(overlay[:-1]))

    def test_actual_actor_grid_palette_override_sequences_and_opaque_bytes(self):
        data, exe, entry = fixture()
        info = probe_sd_ui(data, exe, source_entry=entry)
        self.assertEqual(info['metadata'], {'family': 'weekly', 'profile_index': 0, 'pages': 2,
                                           'frames': 2, 'parts': 2, 'validated_sequence_prefix': 2})
        with tempfile.TemporaryDirectory() as temporary:
            result = export_sd_ui(data, exe, temporary, 'invented', source_entry=entry)
            self.assertEqual(len(result['images']), 2)
            first, second = result['images']
            self.assertEqual(png_pixels(Path(temporary) / first['png']), b'\0\0\0\0\0\xff\0\0\xff')
            self.assertEqual(png_pixels(Path(temporary) / second['png']), b'\0\0\xff\0\xff')
            self.assertEqual(png_pixels(Path(temporary) / second['stp_png']), b'\0\xff')
            self.assertEqual(second['parts'][0]['sheet_source_offset'], 0x42C0)
            self.assertEqual((second['parts'][0]['u'], second['parts'][0]['v']), (16, 0))
            self.assertEqual(second['clut_vram'], [0, 0x1F0])
            self.assertEqual(result['metadata']['source_sequence_prefix'][1]['steps'], [{'frame_index': 1, 'duration': 7}])
            self.assertTrue(result['metadata']['uninterpreted_extents'])
            self.assertFalse(result['metadata']['round_trip_verified'])

    def test_source_binding_hash_and_table_bounds_fail_closed(self):
        data, exe, entry = fixture()
        for changed in (dict(entry, location={'lba': 101}), dict(entry, sha256='0' * 64), dict(entry, size=len(data) - 1)):
            with self.subTest(entry=changed):
                self.assertEqual(probe_sd_ui(data, exe, source_entry=changed)['status'], 'invalid')
        for offset, fmt, value in ((0x2A0, '<I', LOAD_BASE), (0x2A8, '<I', LOAD_BASE),
                                    (0x232, '<H', 99), (0x208 + 10, '<h', 129)):
            bad = bytearray(data)
            struct.pack_into(fmt, bad, offset, value)
            changed = dict(entry, sha256=hashlib.sha256(bad).hexdigest())
            self.assertEqual(probe_sd_ui(bytes(bad), exe, source_entry=changed)['status'], 'invalid')
        with tempfile.TemporaryDirectory() as temporary, self.assertRaises(ImageFormatError):
            export_sd_ui(data, exe, temporary, '../escape', source_entry=entry)

    def test_reused_source_keeps_identical_caller_profiles_and_rejects_conflicts(self):
        data, exe, entry = fixture()
        changed = bytearray(exe)
        struct.pack_into('<I', changed, 0x800B67AC - 0x80041000 + 0x800 + 4, 100)
        struct.pack_into('<3H', changed, 0x800C9F60 - 0x80041000 + 0x800 + 6, 0x2A0, 0x2A8, 0x2B0)
        self.assertEqual(probe_sd_ui(data, bytes(changed), source_entry=entry)['status'], 'supported')
        with tempfile.TemporaryDirectory() as temporary:
            result = export_sd_ui(data, bytes(changed), temporary, 'invented', source_entry=entry)
            self.assertEqual(len(result['metadata']['caller_profile']['equivalent_caller_profiles']), 2)
        struct.pack_into('<H', changed, 0x800C9F60 - 0x80041000 + 0x800 + 6, 0x2A4)
        self.assertEqual(probe_sd_ui(data, bytes(changed), source_entry=entry)['status'], 'invalid')


if __name__ == '__main__':
    unittest.main()
