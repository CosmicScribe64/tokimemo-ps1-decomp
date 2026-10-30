"""T-39004: preserve name-entry panel borders and non-text source pixels."""
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_name_drafts as subject


class NameDraftTests(unittest.TestCase):
    def test_all_catalog_frames_preserve_unedited_pixels(self):
        sources=subject.ROOT/'.asset-cache/translation-assets/original-name-v1'
        if not sources.exists():
            self.skipTest('restore original NAME_ENT references to run pixel checks')
        with tempfile.TemporaryDirectory(dir=subject.ROOT/'.asset-cache') as directory:
            output=Path(directory)/'drafts'
            subject.build(sources,output)
            manifest=json.loads((output/'manifest.json').read_text())
            self.assertEqual(manifest['font_provenance']['source_height'],12)
            self.assertFalse(manifest['font_provenance']['rescaled'])
            self.assertTrue(manifest['font_provenance']['glyphs'])
            self.assertEqual({row['frame'] for row in manifest['assets']},{0,1,2,14,*range(15,25)})
            for row in manifest['assets']:
                original=Image.open(sources/row['source_png']).convert('RGBA')
                result=Image.open(output/row['output']).convert('RGBA')
                self.assertEqual(original.size,result.size)
                self.assertLessEqual(set(result.getdata()),set(original.getdata()))
                regions=[region['rect'] for region in row.get('edit_regions',[])]
                if 'edit_region' in row:
                    regions.append(row['edit_region'])
                for y in range(original.height):
                    for x in range(original.width):
                        if not any(left<=x<left+w and top<=y<top+h for left,top,w,h in regions):
                            self.assertEqual(original.getpixel((x,y)),result.getpixel((x,y)))
                self.assertFalse(row['in_game_verified'])
                self.assertEqual(row['style'],'unreviewed')


if __name__=='__main__':
    unittest.main()
