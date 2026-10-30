"""T-39004: translated save labels preserve native borders and badge silhouettes."""
import json
from pathlib import Path
import shutil
import tempfile
import unittest

from PIL import Image

import translation_filesel_drafts as subject
from translation_assets import digest


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


class FileSelectPageTests(unittest.TestCase):
    """Phase 4 WP-L: information-page drafts as v2 page rows."""
    CHECKPOINT=subject.ROOT/'translations/en/graphics/artwork/checkpoints/T39004-filesel-v3/filesel-indexed-v3'
    SOURCES=subject.ROOT/'.asset-cache/translation-assets/original-filesel-v1'

    def setUp(self):
        if not self.SOURCES.exists(): self.skipTest('restore FILESEL source images first')
        self.temp=tempfile.TemporaryDirectory(dir=subject.ROOT/'.asset-cache');self.root=Path(self.temp.name)

    def tearDown(self): self.temp.cleanup()

    def test_checkpoint_page_maps_to_its_label_regions(self):
        result=subject.pages(self.CHECKPOINT,self.SOURCES,self.root/'out')
        self.assertEqual((result['pages'],result['skipped']),(1,2))
        manifest=json.loads((self.root/'out/manifest.json').read_text())
        self.assertEqual(manifest['schema'],subject.PAGE_SCHEMA)
        self.assertFalse(manifest['human_approved'])
        row=manifest['assets'][0]
        self.assertEqual((row['kind'],row['page'],row['style']),('page',2,'unreviewed'))
        regions=json.loads(subject.REGIONS.read_text())
        expected=sorted(rect['rect'] for name in row['region_ids'] for rect in next(r for r in regions['regions'] if r['id']==name)['page_rects'] if rect['page']==2)
        self.assertEqual(sorted(row['regions']),expected)
        self.assertEqual(row['source_rgba_sha256'],subject.rgba_sha256(Image.open(self.root/'out'/row['source_png'])))
        source=Image.open(self.root/'out'/row['source_png']).convert('RGBA');english=Image.open(self.root/'out'/row['output']).convert('RGBA')
        for y in range(source.height):
            for x in range(source.width):
                if not any(left<=x<left+w and top<=y<top+h for left,top,w,h in row['regions']):
                    self.assertEqual(source.getpixel((x,y)),english.getpixel((x,y)))
        self.assertEqual({entry['reason'] for entry in manifest['skipped']},{'English page not in this build'})

    def test_changed_region_map_or_stray_pixel_is_refused(self):
        changed=self.root/'regions.json'
        changed.write_bytes(subject.REGIONS.read_bytes()+b' ')
        with self.assertRaisesRegex(ValueError,'another region map'):
            subject.pages(self.CHECKPOINT,self.SOURCES,self.root/'a',changed)
        copy=self.root/'indexed';shutil.copytree(self.CHECKPOINT,copy)
        manifest=json.loads((copy/'manifest.json').read_text())
        row=next(r for r in manifest['assets'] if r['variant']=='source-page-information-2')
        image=Image.open(copy/row['output']).convert('RGBA');image.putpixel((127,127),(1,2,3,255));image.save(copy/row['output'])
        row['output_sha256']=digest((copy/row['output']).read_bytes())
        (copy/'manifest.json').write_text(json.dumps(manifest))
        with self.assertRaisesRegex(ValueError,'outside its label regions'):
            subject.pages(copy,self.SOURCES,self.root/'b')
        self.assertFalse((self.root/'b').exists())


if __name__=='__main__': unittest.main()
