"""Synthetic source identity and confined-publication tests for original UI export."""
from pathlib import Path
import tempfile
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


if __name__ == '__main__':
    unittest.main()
