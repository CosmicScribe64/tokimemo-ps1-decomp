"""T-39004: source-decoration and palette constraints for title/exam artwork."""
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_title_exam_drafts as subject


class TitleExamDraftTests(unittest.TestCase):
    def test_native_palette_and_nontext_decoration(self):
        for kind,count in [('title',4),('exam',2)]:
            sources=subject.ROOT/f'.asset-cache/translation-assets/original-{kind}-v1'
            if not sources.exists(): self.skipTest('restore original artwork first')
            with tempfile.TemporaryDirectory(dir=subject.ROOT/'.asset-cache') as directory:
                output=Path(directory)/'drafts'
                subject.build(sources,kind,output)
                manifest=json.loads((output/'manifest.json').read_text())
                self.assertEqual(len(manifest['assets']),count)
                for row in manifest['assets']:
                    source=Image.open(sources/row['source_png']).convert('RGBA')
                    result=Image.open(output/row['output']).convert('RGBA')
                    self.assertEqual(source.size,result.size)
                    self.assertLessEqual(set(result.getdata()),set(source.getdata()))
                    left,top,width,height=row['edit_region']
                    for y in range(source.height):
                        for x in range(source.width):
                            pixel=source.getpixel((x,y))
                            outside=not(left<=x<left+width and top<=y<top+height)
                            underline=kind=='title' and row['frame'] in (1,3) and pixel[3] and pixel[0]>pixel[2]
                            if outside or underline:
                                self.assertEqual(result.getpixel((x,y)),pixel)
                    self.assertEqual(row['style'],'unreviewed')
                    self.assertFalse(row['in_game_verified'])


if __name__=='__main__': unittest.main()
