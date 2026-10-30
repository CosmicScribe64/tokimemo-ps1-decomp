"""T-39004: original font masks, palette and sports-card non-text preservation."""
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_sports_drafts as subject
from translation_native_font import NativeLatin


class SportsDraftTests(unittest.TestCase):
    def test_original_glyph_texels_are_never_rescaled(self):
        executable=subject.ROOT/'disc/files/SLPM_86.053'
        if not executable.exists(): self.skipTest('original executable required')
        font=NativeLatin(executable)
        for character in 'ABCxyz123!?':
            glyph=font.glyph(character);proof=font.proof[character]
            raw=font.data[proof['offset']:proof['offset']+18]
            source={(i%12,i//12) for i in range(144) if raw[i//8]>>(7-i%8)&1}
            self.assertEqual({(x+proof['trim_left'],y) for x,y in glyph['points']},source)

    def test_cards_preserve_art_and_source_palette(self):
        plan=subject.ROOT/'.asset-cache/translation-assets/sports-native-font-plan-v2.json'
        if not plan.exists(): self.skipTest('local source-reviewed wording plan required')
        with tempfile.TemporaryDirectory(dir=subject.ROOT/'.asset-cache') as directory:
            output=Path(directory)/'drafts'
            subject.build(plan,subject.ROOT/'disc/files/SLPM_86.053',output)
            manifest=json.loads((output/'manifest.json').read_text())
            self.assertEqual(len(manifest['assets']),5)
            for row in manifest['assets']:
                source=Image.open(subject.ROOT/row['source']).convert('RGBA')
                result=Image.open(output/row['output']).convert('RGBA')
                self.assertEqual(result.size,(320,240))
                self.assertLessEqual(set(result.getdata()),set(source.getdata()))
                rectangles=[layer['region'] for layer in row['layers']]
                for y in range(240):
                    for x in range(320):
                        if not any(left<=x<left+width and top<=y<top+height for left,top,width,height in rectangles):
                            self.assertEqual(source.getpixel((x,y)),result.getpixel((x,y)))
                self.assertEqual(row['style'],'unreviewed')


if __name__=='__main__': unittest.main()
