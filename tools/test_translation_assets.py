"""T-39004: preserved approvals and guarded optional pack publication."""
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import translation_assets as subject


class ArtworkTests(unittest.TestCase):
    def setUp(self):
        self.snapshot = subject.ROOT / 'translations/en/graphics/artwork'

    def test_retained_reviews_are_exact(self):
        report = subject.audit(self.snapshot)
        self.assertEqual(report['known_current_assets'], 59)
        self.assertEqual(report['approved_exact'], 59)
        self.assertEqual(report['mapped'], 56)
        self.assertFalse(report['coverage_complete'])

    def test_pack_relative_path_and_no_overwrite(self):
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            path = Path(directory) / 'pack'
            result = subject.build_pack(self.snapshot, path)
            self.assertEqual(result['assets'], 56)
            pack = json.loads((path / 'manifest.json').read_text())
            self.assertTrue(all(row['style'] == 'approve' for row in pack['assets']))
            self.assertTrue(all(row['mapping_complete'] for row in pack['assets']))
            with self.assertRaises(ValueError):
                subject.build_pack(self.snapshot, path)

    def test_failed_copy_does_not_leave_partial_pack(self):
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            path = Path(directory) / 'pack'
            with patch.object(subject.shutil, 'copyfile', side_effect=OSError('test write failure')):
                with self.assertRaises(OSError):
                    subject.build_pack(self.snapshot, path)
            self.assertFalse(path.exists())

    def test_path_guards(self):
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            root = Path(directory)
            (root / 'linked').symlink_to(root, target_is_directory=True)
            with self.assertRaises(ValueError):
                subject.new_output(root / 'linked' / 'output')
            for path in ('../outside', '/absolute', 'a//b', 'a/./b'):
                with self.assertRaises(ValueError):
                    subject.inside(root, path)

    def test_group_inventory_does_not_claim_completion(self):
        report = subject.inventory(self.snapshot, self.snapshot.parent / 'image_text_catalog.jsonl')
        self.assertEqual(report['catalog_groups'], 153)
        self.assertEqual(len({row['id'] for row in report['requirements']}), 153)
        self.assertFalse(report['coverage_complete'])
        self.assertTrue(all(not row['in_game_verified'] for row in report['requirements']))


if __name__ == '__main__':
    unittest.main()
