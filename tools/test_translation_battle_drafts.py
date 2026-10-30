"""T-39004: battle draft geometry, native colors and exact source shadow rule."""
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_battle_drafts as subject


class BattleDraftTests(unittest.TestCase):
    def test_all_profiled_commands_preserve_source_colors_and_shadow(self):
        source = subject.ROOT / 'disc/files'
        if not (source / 'CDROM/BATTLE/VS_HABU.BIN').exists():
            self.skipTest('original battle files required')
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            output = Path(directory) / 'drafts'
            subject.build(source / 'CDROM/BATTLE', source / 'SLPM_86.053', output)
            manifest = json.loads((output / 'manifest.json').read_text())
            self.assertEqual(len(manifest['assets']), 30)
            for record in manifest['assets']:
                original = Image.open(output / record['original']).convert('RGBA')
                translated = Image.open(output / record['output']).convert('RGBA')
                self.assertEqual(translated.size, (32, 16))
                self.assertEqual(set(original.getdata()), set(translated.getdata()))
                shadow, ink = sorted(set(original.getdata()) - {(0, 0, 0, 0)},
                                     key=lambda color: sum(color[:3]))
                points = {(x, y) for y in range(16) for x in range(32)
                          if translated.getpixel((x, y)) == ink}
                actual_shadow = {(x, y) for y in range(16) for x in range(32)
                                 if translated.getpixel((x, y)) == shadow}
                self.assertEqual(actual_shadow, {(x + 1, y + 1) for x, y in points} - points)
                self.assertFalse(record['in_game_verified'])
                self.assertEqual(record['style'], 'unreviewed')

    def test_unknown_source_shadow_is_rejected(self):
        image = Image.new('RGBA', (32, 16))
        image.putpixel((1, 1), (255, 255, 255, 255))
        image.putpixel((5, 5), (64, 64, 64, 255))
        with self.assertRaisesRegex(ValueError, 'shadow differs'):
            subject.render_label(image, 'Fight', None)


if __name__ == '__main__':
    unittest.main()
