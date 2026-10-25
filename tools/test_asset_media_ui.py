"""Synthetic source identity and confined-publication tests for original UI export."""
from pathlib import Path
import tempfile
import hashlib
import struct
import unittest
from unittest.mock import patch

from assets import manifest
from assets.media_ui_export import export_ui_source
from tools.test_asset_image_title import fixture


class Source:
    def __init__(self, path):
        self.path = str(path)
        self.data = {'CDROM/FUKUTEST/TITLE.NBN': fixture(), 'OTHER.BIN': b'invented'}
        self.reads = []

    def describe(self):
        return {'source_sha256': 'a' * 64, 'format': 'synthetic', 'read_only': True}

    def entries(self):
        return [{'path': path, 'size': len(data), 'sha256': manifest.digest(data),
                 'kind': 'file', 'location': {'lba': 0xA0CC if path.endswith('TITLE.NBN') else 0}}
                for path, data in self.data.items()]

    def read(self, entry):
        self.reads.append(entry['path'])
        return self.data[entry['path']]


def baseline(source):
    return {'schema': manifest.SCHEMA, 'input': dict(source.describe(), retained_fact=42),
            'assets': [{'id': manifest.asset_id(e['path'], e['sha256']), 'source': e,
                        'status': 'unsupported', 'format': 'unknown'} for e in source.entries()]}


class UiPublicationTests(unittest.TestCase):
    def test_cached_inventory_still_checks_bytes_and_retains_other_sources(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            old = baseline(source)
            source.entries = lambda: self.fail('matching input hash should reuse supplied inventory')
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'):
                target = root / '.asset-cache/run'
                result = export_ui_source(source, target, old)
                first = (target / 'manifest.json').read_bytes()
                export_ui_source(source, target, old)
                self.assertEqual(first, (target / 'manifest.json').read_bytes())
                title, other = result['assets']
                self.assertEqual(title['status'], 'exported')
                self.assertEqual(len(title['outputs']), 36)
                self.assertEqual((root / title['original']['path']).read_bytes(), source.data[title['source']['path']])
                self.assertEqual(other['status'], 'unsupported')
                self.assertEqual(result['input']['retained_fact'], 42)
                self.assertTrue(result['options']['inventory_reused_by_whole_input_hash'])

    def test_selected_path_refresh_requires_matching_baseline_and_preserves_other_exports(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            old = baseline(source)
            old['assets'][1].update(status='exported', export={'sentinel': True}, outputs=[{'path': 'kept.png'}])
            source.entries = lambda: self.fail('matching baseline should be reused')
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'):
                result = export_ui_source(source, root / '.asset-cache/run', old,
                                          only_paths=['CDROM/FUKUTEST/TITLE.NBN'])
                title, other = result['assets']
                self.assertEqual(title['status'], 'exported')
                self.assertEqual(other['export'], {'sentinel': True})
                self.assertEqual(other['outputs'], [{'path': 'kept.png'}])
                self.assertEqual(result['options']['selected_source_paths'], ['CDROM/FUKUTEST/TITLE.NBN'])
                with self.assertRaisesRegex(ValueError, 'exact matching baseline'):
                    export_ui_source(source, root / '.asset-cache/no-baseline', only_paths=['OTHER.BIN'])

    def test_changed_source_fails_even_when_input_description_matches(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            old = baseline(source)
            source.data['CDROM/FUKUTEST/TITLE.NBN'] = b'x' * len(fixture())
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'):
                with self.assertRaisesRegex(ValueError, 'changed after inventory'):
                    export_ui_source(source, root / '.asset-cache/run', old)

    def test_wrong_lba_clears_stale_exports_without_reading(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            old = baseline(source)
            old['assets'][0].update(export={'images': [{}]}, outputs=[{}], original={})
            old['assets'][0]['source']['location']['lba'] += 1
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'):
                record = export_ui_source(source, root / '.asset-cache/run', old)['assets'][0]
                self.assertEqual(record['status'], 'unsupported')
                self.assertIn('LBA differs', record['reason'])
                self.assertNotIn('export', record)
                self.assertNotIn('outputs', record)
                self.assertNotIn('original', record)
                self.assertEqual(source.reads, [])

    def test_unreadable_profile_clears_stale_exports(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            old = baseline(source)
            old['assets'][0].update(status='exported', export={'images': [{}]},
                                    outputs=[{}], original={}, paired_original={}, paired_source={})
            entries = source.entries()
            entries[0].update(kind='unsupported', unsupported='Form2 source extent')
            source.entries = lambda: entries
            source.describe = lambda: {'source_sha256': 'b' * 64, 'format': 'synthetic', 'read_only': True}
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'):
                record = export_ui_source(source, root / '.asset-cache/run', old)['assets'][0]
                self.assertEqual(record['status'], 'unsupported')
                self.assertEqual(record['reason'], 'Form2 source extent')
                for key in ('export', 'outputs', 'original', 'paired_original', 'paired_source'):
                    self.assertNotIn(key, record)
                self.assertEqual(source.reads, [])

    def test_unsafe_or_inconsistent_baseline_identity_is_rejected(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            for bad_id in ('../../escape', 'asset-' + '0' * 64):
                old = baseline(source)
                old['assets'][0]['id'] = bad_id
                with self.subTest(asset_id=bad_id), self.assertRaises(ValueError):
                    export_ui_source(source, root / '.asset-cache/run', old)

    def test_fresh_inventory_rejects_stale_baseline_hash_before_decode(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            old = baseline(source)
            old['input']['source_sha256'] = 'b' * 64
            entry = old['assets'][0]
            entry['source']['sha256'] = 'c' * 64
            entry['id'] = manifest.asset_id(entry['source']['path'], 'c' * 64)
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'):
                with self.assertRaisesRegex(ValueError, 'baseline source differs'):
                    export_ui_source(source, root / '.asset-cache/run', old)

    def test_output_must_be_inside_cache(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'):
                with self.assertRaises(ValueError):
                    export_ui_source(Source(root / 'source.bin'), root / 'outside')

    def test_exam_requires_paired_executable_before_reading_texture_bytes(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            path = 'CDROM/FUKUTEST/EXAMINE.NBN'
            data = bytes(0x6800)
            source.entries = lambda: [{'path': path, 'size': len(data), 'kind': 'file',
                                       'sha256': manifest.digest(data), 'location': {'lba': 0x9CA8}}]
            source.read = lambda entry: self.fail('missing paired CLUT source must stop before image decoding')
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'):
                record = export_ui_source(source, root / '.asset-cache/run')['assets'][0]
                self.assertEqual(record['status'], 'unsupported')
                self.assertIn('paired original executable required', record['reason'])
                self.assertNotIn('outputs', record)

    def test_sd_profiles_dispatch_through_paired_loader_tables(self):
        from tools.test_asset_image_sd import fixture as sd_fixture
        data, exe, entry = sd_fixture()
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            source.data = {entry['path']: data, 'SLPM_86.053': exe}
            entries = [dict(entry, kind='file'),
                       {'path': 'SLPM_86.053', 'size': len(exe), 'kind': 'file',
                        'sha256': manifest.digest(exe), 'location': {'lba': 23}}]
            source.entries = lambda: entries
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'):
                result = export_ui_source(source, root / '.asset-cache/run', include_sd=True)
                record = next(a for a in result['assets'] if a['source']['path'] == entry['path'])
                self.assertEqual(record['status'], 'exported')
                self.assertEqual(record['format'], 'tokimemo-sd-animation')
                self.assertEqual(len(record['export']['images']), 2)
                self.assertEqual(len(record['outputs']), 4)
                self.assertEqual(record['paired_source']['sha256'], manifest.digest(exe))
                self.assertIn('bound by original', result['options']['sd_profile_status'])

    def test_etc_else_lbas_bind_two_sources_to_distinct_exe_layout_rows(self):
        from tools.test_asset_image_sd import LOAD_BASE
        exe = bytearray(0x90000)
        exe[:8] = b'PS-X EXE'
        struct.pack_into('<II', exe, 24, 0x80041000, len(exe) - 0x800)
        # ELSE layouts 0 and 1 are native u16 triples at 0x800CA108.
        struct.pack_into('<3H', exe, 0x800CA108 - 0x80041000 + 0x800, 0x260, 0x264, 0x268)
        struct.pack_into('<3H', exe, 0x800CA10E - 0x80041000 + 0x800, 0x260, 0x264, 0x268)
        exe_data = bytes(exe)
        data = bytearray(0x4800)
        struct.pack_into('<256H', data, 0, 0, 31, 0x83E0, *([0] * 253))
        struct.pack_into('<4h', data, 0x200, 1, 0, 0, 0)
        struct.pack_into('<2h3H2h', data, 0x208, 0, 0, 0, 0xFE0, 2, 2, 1)
        struct.pack_into('<3H', data, 0x230, 1, 0, 5)
        struct.pack_into('<I', data, 0x260, LOAD_BASE + 0x300)
        struct.pack_into('<I', data, 0x264, LOAD_BASE + 0x200)
        struct.pack_into('<I', data, 0x268, LOAD_BASE + 0x230)
        data[0x300] = 1
        overlay = bytearray(0x30000)
        struct.pack_into('<3I', overlay, 0x80150720 - 0x80132000 - 4, 0x7D78, 0x7D81, 0x7D8A)
        overlay_data = bytes(overlay)
        overlay_sha1 = hashlib.sha1(overlay_data).hexdigest()
        records = []
        source_data = {'SLPM_86.053': exe_data, 'CDROM/EXEDIR/ETC.EXN': overlay_data}
        for path, lba in (('CDROM/ETC_SD/SD_P30.NBN', 0x7D78),
                          ('CDROM/ETC_SD/SD_P30W.NBN', 0x7D81)):
            value = bytes(data)
            source_data[path] = value
            records.append({'path': path, 'size': len(value), 'kind': 'file',
                            'sha256': manifest.digest(value), 'location': {'lba': lba}})
        records.extend([
            {'path': 'SLPM_86.053', 'size': len(exe_data), 'kind': 'file',
             'sha256': manifest.digest(exe_data), 'location': {}},
            {'path': 'CDROM/EXEDIR/ETC.EXN', 'size': len(overlay_data), 'kind': 'file',
             'sha256': manifest.digest(overlay_data), 'location': {}},
        ])
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            source.data = source_data
            source.entries = lambda: records
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'), \
                    patch('assets.image_packs_photos.ETC_SHA1', overlay_sha1):
                result = export_ui_source(source, root / '.asset-cache/run', include_sd=True)
            rows = [row for row in result['assets'] if row['source']['path'] in {
                'CDROM/ETC_SD/SD_P30.NBN', 'CDROM/ETC_SD/SD_P30W.NBN'}]
            self.assertEqual([row['source']['location']['lba'] for row in rows], [0x7D78, 0x7D81])
            self.assertTrue(all(row['status'] == 'exported' for row in rows),
                            [(row['source']['path'], row['status'], row.get('reason'), row.get('inspection'))
                             for row in rows])
            self.assertTrue(all(row['export']['metadata']['caller_profile']['index'] in (0, 1) for row in rows))
            self.assertTrue(all(row['source_dependencies'][0]['path'] == 'CDROM/EXEDIR/ETC.EXN'
                                for row in rows))
            self.assertTrue(all(manifest.digest((root / row['dependency_originals'][0]['original']['path']).read_bytes())
                                == row['source_dependencies'][0]['sha256'] for row in rows))
            self.assertIn('ETC native LBA table', result['options']['sd_profile_status'])

    def test_path_bound_profile_requires_lba_and_does_not_pair_executable(self):
        from tools.test_asset_image_sports import fixture as sports_fixture
        path = 'CDROM/TAIIKU/SPSETU.BIN'
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            source.data = {path: sports_fixture(), 'SLPM_86.053': b'invented unused executable'}
            entries = [{'path': name, 'size': len(data), 'kind': 'file',
                        'sha256': manifest.digest(data), 'location': {}}
                       for name, data in source.data.items()]
            source.entries = lambda: entries
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'):
                target = root / '.asset-cache/run'
                record = export_ui_source(source, target)['assets'][0]
                self.assertEqual(record['status'], 'unsupported')
                self.assertIn('LBA differs', record['reason'])
                self.assertEqual(source.reads, [])
                entries[0]['location']['lba'] = 0xB969
                target = root / '.asset-cache/valid-run'
                record = export_ui_source(source, target)['assets'][0]
                self.assertEqual(record['status'], 'exported')
                self.assertEqual(source.reads, [path])
                self.assertNotIn('paired_source', record)
                self.assertNotIn('paired_original', record)
                self.assertFalse((target / 'assets' / record['id'] / 'paired-executable.bin').exists())

    def test_photo_dependencies_publish_and_missing_native_source_clears_stale_claims(self):
        from tools.test_asset_image_photos import fixture as photo_fixture
        from assets.visual_text import build_catalog
        for kind, pages, dependencies in (('last', 4, 1), ('staff', 3, 2)):
            data, kwargs, spec = photo_fixture(kind)
            with self.subTest(kind=kind), tempfile.TemporaryDirectory() as temporary:
                root = Path(temporary).resolve()
                source = Source(root / 'source.bin')
                entries = [dict(kwargs['source_entry'], kind='file'), dict(kwargs['overlay_entry'], kind='file')]
                source.data = {entries[0]['path']: data, entries[1]['path']: kwargs['overlay_data']}
                if kind == 'staff':
                    entries.append(dict(kwargs['palette_entry'], kind='file'))
                    source.data[entries[-1]['path']] = kwargs['palette_data']
                source.entries = lambda: entries
                with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'), \
                        patch('assets.image_packs_photos.ETC_SHA1', spec['sha1']):
                    result = export_ui_source(source, root / '.asset-cache/run', include_photos=True)
                    record = next(r for r in result['assets'] if r['source']['path'] == entries[0]['path'])
                    self.assertEqual(record['status'], 'exported')
                    self.assertEqual(record['format'], 'tokimemo-photo-pages')
                    self.assertEqual(len(record['export']['images']), pages)
                    self.assertEqual(len(record['source_dependencies']), dependencies)
                    self.assertEqual(len(record['dependency_originals']), dependencies)
                    self.assertNotIn('paired_source', record)
                    for dependency in record['dependency_originals']:
                        saved = dependency['original']
                        self.assertEqual(manifest.digest((root / saved['path']).read_bytes()), dependency['source']['sha256'])
                        self.assertIn(saved, record['outputs'])
                    self.assertEqual(len(build_catalog(result)['regions']), pages)
                    # Cached inventory intentionally omits the required native source.
                    old = dict(result, assets=[r for r in result['assets'] if r['source']['path'] != entries[1]['path']])
                    missing = export_ui_source(source, root / '.asset-cache/missing', old, include_photos=True)
                    stale = next(r for r in missing['assets'] if r['source']['path'] == entries[0]['path'])
                    self.assertEqual(stale['status'], 'unsupported')
                    for key in ('export', 'outputs', 'original', 'source_dependencies', 'dependency_originals', 'inspection'):
                        self.assertNotIn(key, stale)

    def test_staff_photo_requires_original_palette_before_reading_pixels(self):
        from tools.test_asset_image_photos import fixture as photo_fixture
        data, kwargs, spec = photo_fixture('staff')
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / 'source.bin')
            entries = [dict(kwargs['source_entry'], kind='file'), dict(kwargs['overlay_entry'], kind='file')]
            source.entries = lambda: entries
            source.data = {entries[0]['path']: data, entries[1]['path']: kwargs['overlay_data']}
            with patch.object(manifest, 'REPOSITORY', root), patch.object(manifest, 'CACHE', root / '.asset-cache'), \
                    patch('assets.image_packs_photos.ETC_SHA1', spec['sha1']):
                result = export_ui_source(source, root / '.asset-cache/run', include_photos=True)
                record = next(r for r in result['assets'] if r['source']['path'] == entries[0]['path'])
                self.assertEqual(record['status'], 'unsupported')
                self.assertIn('STAFF2 palette required', record['reason'])
                self.assertNotIn(entries[0]['path'], source.reads)


if __name__ == '__main__':
    unittest.main()
