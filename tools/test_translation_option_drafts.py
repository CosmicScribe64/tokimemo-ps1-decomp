"""T-39004: independent source/output geometry and palette checks."""
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_option_drafts as subject


class OptionDraftTests(unittest.TestCase):
    def test_font_recovery_preserves_published_shapes(self):
        glyphs, proof = subject.recover_font(subject.ROOT / 'translations/en/graphics/artwork')
        self.assertEqual(glyphs['i'], ['1','1','0','1','1','1','1','1','1'])
        self.assertEqual(set(proof['recovered']), set('ABCDEFLMNRSTVXabcdefghiklmnoprstuwy'))
        self.assertEqual(proof['historical_variants'], {'B': 2})
        for mask in glyphs.values():
            self.assertEqual(len(mask), 9)
            self.assertTrue(all(len(row) == len(mask[0]) and set(row) <= {'0','1'} for row in mask))

    def test_source_bound_draft_invariants(self):
        sources = subject.ROOT / '.asset-cache/translation-assets/original-option-v1'
        if not sources.exists():
            self.skipTest('restore original OPTION references to run pixel checks')
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            output = Path(directory) / 'drafts'
            subject.draft_all(sources, output, subject.ROOT / 'translations/en/graphics/artwork')
            manifest = json.loads((output / 'manifest.json').read_text())
            self.assertEqual(len(manifest['assets']), 64)
            self.assertFalse(manifest['human_approved'])
            for row in manifest['assets']:
                source = Image.open(sources / row['source_png']).convert('RGBA')
                result = Image.open(output / row['output']).convert('RGBA')
                self.assertEqual(source.size, result.size)
                self.assertEqual(source.crop((0,12,source.width,16)).tobytes(), result.crop((0,12,result.width,16)).tobytes())
                self.assertEqual(subject.digest((sources / row['source_png']).read_bytes()), row['source_png_sha256'])
                self.assertEqual(subject.digest((output / row['output']).read_bytes()), row['output_sha256'])
                self.assertEqual(row['style'], 'unreviewed')
                self.assertFalse(row['in_game_verified'])
                allowed = set(source.getdata()) | {(0,0,0,0), subject.SHADOW}
                self.assertLessEqual(set(result.getdata()), allowed)
                ink = [(x,y) for y in range(16) for x in range(result.width)
                       if result.getpixel((x,y))[3] and result.getpixel((x,y)) != subject.SHADOW]
                self.assertTrue(ink)
                self.assertEqual(min(x for x,y in ink),row['source_ink_left'])
                self.assertEqual(max(y for x,y in ink),row['source_ink_baseline'])
                self.assertLessEqual(max(y for y in range(16) for x in range(result.width) if result.getpixel((x,y))[3]),11)


if __name__ == '__main__':
    unittest.main()
