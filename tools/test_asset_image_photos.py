"""Invented source-bound photo pages/native tables, without original pictures."""
import hashlib
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

from tools.assets.image_packs_photos import (
    ALL_CLEAR_TABLE, ETC_BASE, ETC_SIZE, LAST_TABLE, STAFF_LBAS,
    STAFF_PALETTE_LBA, STAFF_PALETTE_PATH, export_photos, photo_profile_rows, probe_photos,
)
from tools.assets.images import ImageFormatError
from tools.test_asset_images import png_pixels


def entry(data, path, lba):
    return {'path': path, 'size': len(data), 'sha256': hashlib.sha256(data).hexdigest(),
            'location': {'lba': lba, 'extent_bytes': len(data)}}


def fixture(kind='last'):
    overlay = bytearray(ETC_SIZE)
    struct.pack_into('<13I', overlay, LAST_TABLE - ETC_BASE, *range(1000, 1013))
    struct.pack_into('<I', overlay, ALL_CLEAR_TABLE - ETC_BASE, 1100)
    overlay = bytes(overlay)
    overlay_entry = entry(overlay, 'CDROM/EXEDIR/ETC.EXN', 2000)
    palette = bytearray(2048)
    struct.pack_into('<5H', palette, 0, 0, 0x001F, 0x83E0, 0x7C00, 0x8000)
    if kind == 'staff':
        data = bytearray(0x6800)
        data[0], data[0x2000] = 0x21, 0x43
        path, lba = 'CDROM/STAFF2/STAF2AS.NBN', STAFF_LBAS[0]
    else:
        data = bytearray(0x18800 if kind == 'all' else 0x10800)
        offset = 0x18000 if kind == 'all' else 0x10000
        data[:4] = bytes([0, 1, 2, 4])
        data[offset:offset + 512] = palette[:512]
        path, lba = ('CDROM/LASTPHOT/ALL_C.NBN', 1100) if kind == 'all' else ('CDROM/LASTPHOT/LASTP_AS.NBN', 1000)
    data = bytes(data)
    kwargs = {'source_entry': entry(data, path, lba), 'overlay_data': overlay, 'overlay_entry': overlay_entry}
    if kind == 'staff':
        kwargs.update(palette_data=bytes(palette), palette_entry=entry(palette, STAFF_PALETTE_PATH, STAFF_PALETTE_LBA))
    spec = {'sha1': hashlib.sha1(overlay).hexdigest(), 'load_base': ETC_BASE, 'size': ETC_SIZE,
            'source_evidence_hashes': {'invented_config': 'a' * 64}}
    return data, kwargs, spec


class PhotoImageTests(unittest.TestCase):
    def test_native_rows_and_hex_decimal_read_counts_are_exact(self):
        data, kwargs, spec = fixture()
        with patch('tools.assets.image_packs_photos.ETC_SHA1', spec['sha1']):
            rows = photo_profile_rows(kwargs['overlay_data'], overlay_entry=kwargs['overlay_entry'])
            self.assertEqual(len(rows), 28)
            self.assertEqual((rows[0]['read_sector_count'], rows[0]['read_sector_count_hex'], rows[0]['source_byte_count']), (33, '0x21', 67584))
            self.assertEqual((rows[13]['read_sector_count'], rows[13]['read_sector_count_hex']), (49, '0x31'))
            self.assertEqual((rows[14]['read_sector_count'], rows[14]['read_sector_count_hex']), (13, '0xd'))
            self.assertEqual(rows[0]['native_lba_pointer_address'], LAST_TABLE)
            self.assertEqual(probe_photos(data, **kwargs)['status'], 'supported')
            with patch('pathlib.Path.read_text', side_effect=AssertionError('runtime config read')):
                self.assertEqual(probe_photos(data, **kwargs)['status'], 'supported')

    def test_last_pages_mode3_alpha_stp_and_exact_opaque_trailer(self):
        for kind, count in (('last', 4), ('all', 6)):
            data, kwargs, spec = fixture(kind)
            with patch('tools.assets.image_packs_photos.ETC_SHA1', spec['sha1']), tempfile.TemporaryDirectory() as folder:
                result = export_photos(data, None, folder, 'invented', **kwargs)
                self.assertEqual(len(result['images']), count)
                image = result['images'][0]
                pixels = png_pixels(Path(folder) / image['png'])
                self.assertEqual(pixels[1:17], b'\0\0\0\0\xff\0\0\xff\0\xff\0\xff\0\0\0\xff')
                self.assertEqual(png_pixels(Path(folder) / image['stp_png'])[1:5], bytes([0, 255, 255, 255]))
                self.assertEqual(image['source_palette_words'][1], 0x001F)
                self.assertEqual(image['preview_palette_words'][1], 0x801F)
                self.assertEqual(result['metadata']['uninterpreted_extents'][0]['size'], 0x600)
                self.assertEqual(image['clut_vram'], [0, 0x1FF])
                self.assertFalse(result['metadata']['hardware_raster_parity_verified'])
                self.assertEqual(result['images'][-1]['runtime_bg_cell']['v'], 120)
                self.assertEqual(result['images'][-1]['vram_rectangle_words'][1], 128)

    def test_staff4bpp_uses_separately_hashed_mode1_palette(self):
        data, kwargs, spec = fixture('staff')
        with patch('tools.assets.image_packs_photos.ETC_SHA1', spec['sha1']), tempfile.TemporaryDirectory() as folder:
            result = export_photos(data, None, folder, 'invented', **kwargs)
            self.assertEqual(len(result['images']), 3)
            image = result['images'][0]
            self.assertEqual(png_pixels(Path(folder) / image['png'])[1:9], b'\xff\0\0\xff\0\xff\0\xff')
            self.assertEqual(png_pixels(Path(folder) / image['stp_png'])[1:3], bytes([0, 255]))
            self.assertEqual(image['clut_vram'], [0, 0x1F1])
            self.assertEqual(len(image['source_palette_words']), 16)
            self.assertEqual(len(image['runtime_loaded_palette_row_words']), 256)
            self.assertEqual(image['palette_source']['sha256'], kwargs['palette_entry']['sha256'])
            self.assertEqual(result['metadata']['uninterpreted_extents'][0]['size'], 2048)
            self.assertEqual([e['size'] for e in result['metadata']['paired_palette_source_extents']], [1536, 480])
            self.assertEqual(len(result['metadata']['source_dependencies']), 2)

    def test_changed_hashes_lba_paths_sizes_and_canonical_overlay_fail_closed(self):
        data, kwargs, spec = fixture('staff')
        with patch('tools.assets.image_packs_photos.ETC_SHA1', spec['sha1']):
            changes = [dict(kwargs, source_entry=dict(kwargs['source_entry'], sha256='0' * 64)),
                       dict(kwargs, source_entry=dict(kwargs['source_entry'], location={'lba': 100})),
                       dict(kwargs, source_entry=dict(kwargs['source_entry'], path='OTHER.NBN')),
                       dict(kwargs, palette_data=None),
                       dict(kwargs, palette_entry=dict(kwargs['palette_entry'], location={'lba': STAFF_PALETTE_LBA + 1})),
                       dict(kwargs, palette_entry=dict(kwargs['palette_entry'], sha256='0' * 64)),
                       dict(kwargs, overlay_entry=dict(kwargs['overlay_entry'], path='CDROM/EXEDIR/OTHER.EXN'))]
            changed = bytearray(kwargs['overlay_data'])
            changed[-1] = 1
            changes.append(dict(kwargs, overlay_data=bytes(changed),
                                overlay_entry=entry(changed, 'CDROM/EXEDIR/ETC.EXN', 2000)))
            for options in changes:
                self.assertEqual(probe_photos(data, **options)['status'], 'invalid')
            self.assertEqual(probe_photos(data[:-1], **kwargs)['status'], 'invalid')

    def test_safe_ids_and_exclusive_output_files(self):
        data, kwargs, spec = fixture()
        with patch('tools.assets.image_packs_photos.ETC_SHA1', spec['sha1']), tempfile.TemporaryDirectory() as folder:
            with self.assertRaises(ImageFormatError):
                export_photos(data, None, folder, '../escape', **kwargs)
            export_photos(data, None, folder, 'invented', **kwargs)
            with self.assertRaises(FileExistsError):
                export_photos(data, None, folder, 'invented', **kwargs)

    def test_native_lba_rows_reject_zero_layout_conflict_and_truncation(self):
        data, kwargs, _ = fixture()
        for address, value in ((LAST_TABLE, 0), (ALL_CLEAR_TABLE, 0), (ALL_CLEAR_TABLE, 1000)):
            overlay = bytearray(kwargs['overlay_data'])
            struct.pack_into('<I', overlay, address - ETC_BASE, value)
            changed = dict(kwargs, overlay_data=bytes(overlay),
                           overlay_entry=entry(overlay, 'CDROM/EXEDIR/ETC.EXN', 2000))
            with patch('tools.assets.image_packs_photos.ETC_SHA1', hashlib.sha1(overlay).hexdigest()):
                self.assertEqual(probe_photos(data, **changed)['status'], 'invalid')
        overlay = kwargs['overlay_data'][:ALL_CLEAR_TABLE - ETC_BASE]
        changed = dict(kwargs, overlay_data=overlay,
                       overlay_entry=entry(overlay, 'CDROM/EXEDIR/ETC.EXN', 2000))
        with patch('tools.assets.image_packs_photos.ETC_SHA1', hashlib.sha1(overlay).hexdigest()):
            self.assertEqual(probe_photos(data, **changed)['status'], 'invalid')


if __name__ == '__main__':
    unittest.main()
