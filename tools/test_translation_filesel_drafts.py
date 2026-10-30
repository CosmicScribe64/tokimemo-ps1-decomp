"""T-39004: translated save labels preserve native borders and badge silhouettes."""
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_filesel_drafts as subject


class FileSelectDraftTests(unittest.TestCase):
    def test_all_panel_and_badge_variants(self):
        sources=subject.ROOT/'.asset-cache/translation-assets/original-filesel-v1'
        if not sources.exists(): self.skipTest('restore FILESEL source images first')
        with tempfile.TemporaryDirectory(dir=subject.ROOT/'.asset-cache') as directory:
            output=Path(directory)/'drafts';subject.build(sources,output)
            manifest=json.loads((output/'manifest.json').read_text())
            self.assertEqual(len(manifest['assets']),8)
            for row in manifest['assets']:
                source=Image.open(sources/row['source_png']).convert('RGBA')
                result=Image.open(output/row['output']).convert('RGBA')
                self.assertEqual(source.size,result.size)
                self.assertLessEqual(set(result.getdata()),set(source.getdata()))
                if row['frame'] in (30,31): self.assertEqual(source.getchannel('A').tobytes(),result.getchannel('A').tobytes())
                for y in range(source.height):
                    for x in range(source.width):
                        if not any(left<=x<left+width and top<=y<top+height for left,top,width,height in [edit['rect'] for edit in row['edit_regions']]):
                            self.assertEqual(source.getpixel((x,y)),result.getpixel((x,y)))
                self.assertEqual(row['style'],'unreviewed')
                self.assertFalse(row['in_game_verified'])


if __name__=='__main__': unittest.main()
