"""T-39004: legacy batch-001 rebinding keeps the exact current source outside the lettering."""
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_legacy_remap as subject


class LegacyRemapTests(unittest.TestCase):
    def test_rebind_preserves_source_and_palette(self):
        sources = subject.ROOT / '.asset-cache/translation-assets/original-mainrom-v1'
        if not sources.exists():
            self.skipTest('restore original MAINROM legacy export first')
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            output = Path(directory) / 'out'
            subject.build(sources, output)
            manifest = json.loads((output / 'manifest.json').read_text())
            self.assertEqual(len(manifest['assets']), 3)
            for row in manifest['assets']:
                source = Image.open(sources / ('mainrom.frame-%03d.base.png' % row['frame'])).convert('RGBA')
                result = Image.open(output / row['output']).convert('RGBA')
                self.assertEqual(source.size, result.size)
                self.assertLessEqual(set(result.getdata()), set(source.getdata()) | {(0, 0, 0, 0)})
                for y in range(source.height):
                    for x in range(source.width):
                        if not subject.inside_regions(row['edit_regions'], x, y):
                            self.assertEqual(source.getpixel((x, y)), result.getpixel((x, y)))
                self.assertFalse(row['human_approved'])
                self.assertEqual(row['style'], 'unreviewed')

    def test_compose_is_cache_free(self):
        clear = (0, 0, 0, 0)
        a, b, c = (10, 10, 10, 255), (200, 100, 50, 255), (201, 101, 51, 255)
        source = Image.new('RGBA', (4, 2), a)
        source.putpixel((3, 1), b)
        old = Image.new('RGBA', (4, 2), clear)
        old.putpixel((0, 0), c)  # within one step of b
        old.putpixel((1, 0), (10, 10, 10, 255))
        old.putpixel((3, 1), (9, 9, 9, 255))  # outside region: legacy difference only
        result, outside = subject.compose(source, old, [[0, 0, 2, 1]])
        self.assertEqual(result.getpixel((0, 0)), b)
        self.assertEqual(result.getpixel((1, 0)), a)
        self.assertEqual(result.getpixel((3, 1)), b)
        self.assertEqual(result.getpixel((2, 1)), a)
        self.assertEqual(outside, 6)
        bad = Image.new('RGBA', (4, 2), clear)
        bad.putpixel((0, 0), (90, 90, 90, 255))
        with self.assertRaises(ValueError):
            subject.compose(source, bad, [[0, 0, 2, 1]])

    def test_published_candidates_reproduce_byte_for_byte(self):
        sources = subject.ROOT / '.asset-cache/translation-assets/original-mainrom-v1'
        if not sources.exists():
            self.skipTest('restore original MAINROM export first (tools/assets/image_packs.py export_main_ui)')
        published = subject.ROOT / 'translations/en/graphics/artwork/checkpoints/T39004-pass2/legacy-rebind-v1'
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            output = Path(directory) / 'out'
            subject.build(sources, output)
            names = sorted(p.name for p in output.iterdir())
            self.assertEqual(names, sorted(p.name for p in published.iterdir()))
            for name in names:
                self.assertEqual((output / name).read_bytes(), (published / name).read_bytes(), name)

    def test_outputs_must_be_new(self):
        with self.assertRaises(ValueError):
            subject.new_output(subject.ROOT / 'tools')


if __name__ == '__main__':
    unittest.main()
