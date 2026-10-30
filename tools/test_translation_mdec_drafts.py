"""Phase 4 WP-L: v2 rows from the T-39012 BS splice outputs.

Synthetic tests need no game data. The original-data tests read the ignored
T-39012 build (.asset-cache/translation-assets/bs-drafts-v1) and disc/files and
skip when either is absent.
"""
import json
from pathlib import Path
import shutil
import tempfile
import unittest

import numpy as np
from PIL import Image

import translation_mdec_drafts as subject
from translation_assets import ROOT, digest, pinned_decoder_identity, rgba_sha256

DRAFTS = ROOT / '.asset-cache/translation-assets/bs-drafts-v1'
BS_DIR = ROOT / 'disc/files/CDROM/BS'


class ExpansionTests(unittest.TestCase):
    def test_expansions(self):
        values = np.array([0, 1, 16, 31])
        self.assertEqual(subject.expand(values, 'replicate').tolist(), [0, 8, 132, 255])
        self.assertEqual(subject.expand(values, 'shift').tolist(), [0, 8, 128, 248])
        with self.assertRaises(ValueError):
            subject.expand(values, 'linear')

    def test_region_mask(self):
        mask = subject.region_mask([[1, 2, 3, 1]], 8, 4)
        self.assertEqual(int(mask.sum()), 3)
        self.assertTrue(mask[2, 1] and mask[2, 3] and not mask[2, 4])


class SyntheticPageTests(unittest.TestCase):
    """The ALL_C page composition on synthetic pages."""

    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        (self.root / 'all-c-source').mkdir()
        page = np.zeros((16, 16, 4), np.uint8)
        page[4:12, :, :3] = 50
        page[4:12, :, 3] = 255
        Image.fromarray(page, 'RGBA').save(self.root / 'all-c-source/all-c.source-page-3.png')
        crop = np.full((4, 8, 3), 50, np.uint8)
        crop[1:3, 2:6] = 200
        Image.fromarray(crop, 'RGB').save(self.root / 'crop.png')
        self.asset = {'id': 'banner', 'catalog_item': 'c', 'gates_passed': True, 'source_container': 'X.NBN',
                      'source_container_sha256': '1' * 64, 'english_container_sha256': '2' * 64, 'palette_id': 'p',
                      'source_pages': [{'variant': 'source-page-3', 'source_png_sha256': digest((self.root / 'all-c-source/all-c.source-page-3.png').read_bytes())}],
                      'edit_regions': [{'page': 3, 'page_rect': [4, 6, 8, 4], 'png': 'crop.png', 'sha256': digest((self.root / 'crop.png').read_bytes())}]}

    def tearDown(self):
        self.temp.cleanup()

    def test_composes_inside_the_rect_only(self):
        rows, images = subject.page_rows(self.root, self.asset)
        self.assertEqual(rows[0]['regions'], [[4, 6, 8, 4]])
        self.assertEqual((rows[0]['kind'], rows[0]['page'], rows[0]['style']), ('page', 3, 'unreviewed'))
        self.assertEqual(rows[0]['source_rgba_sha256'], rgba_sha256(Image.open(self.root / 'all-c-source/all-c.source-page-3.png')))
        result = np.asarray(images[rows[0]['output']])
        source = np.asarray(Image.open(self.root / 'all-c-source/all-c.source-page-3.png'))
        changed = (result != source).any(-1)
        self.assertTrue(changed[7, 6] and changed.sum() == 8)
        self.assertFalse(changed[:6].any() or changed[10:].any())

    def test_rejections(self):
        bad = json.loads(json.dumps(self.asset))
        bad['edit_regions'][0]['page_rect'] = [4, 6, 6, 4]
        with self.assertRaisesRegex(ValueError, 'crop size'):
            subject.page_rows(self.root, bad)
        bad = json.loads(json.dumps(self.asset))
        bad['edit_regions'][0]['sha256'] = '0' * 64
        with self.assertRaisesRegex(ValueError, 'crop differs'):
            subject.page_rows(self.root, bad)
        bad = json.loads(json.dumps(self.asset))
        bad['source_pages'][0]['source_png_sha256'] = '0' * 64
        with self.assertRaisesRegex(ValueError, 'source page differs'):
            subject.page_rows(self.root, bad)
        bad = json.loads(json.dumps(self.asset))
        bad['gates_passed'] = False
        with self.assertRaisesRegex(ValueError, 'gates'):
            subject.page_rows(self.root, bad)


@unittest.skipUnless((DRAFTS / 'manifest.json').is_file() and BS_DIR.is_dir(), 'T-39012 build or original BS files absent')
class OriginalDataTests(unittest.TestCase):
    def setUp(self):
        (ROOT / '.asset-cache').mkdir(exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(dir=ROOT / '.asset-cache')
        self.root = Path(self.temp.name)

    def tearDown(self):
        self.temp.cleanup()

    def test_rows_bind_the_pinned_decode(self):
        result = subject.build(DRAFTS, self.root / 'out')
        self.assertEqual((result['bs'], result['page']), (1, 3))
        manifest = json.loads((self.root / 'out/manifest.json').read_text())
        self.assertFalse(manifest['human_approved'])
        still = next(row for row in manifest['assets'] if row['kind'] == 'bs')
        self.assertEqual(still['decoder_identity_sha256'], pinned_decoder_identity())
        source = Image.open(self.root / 'out' / still['source_png'])
        english = Image.open(self.root / 'out' / still['output'])
        self.assertEqual(still['source_rgba_sha256'], rgba_sha256(source))
        # The source decode is the T-39012 source PNG (replicate expansion).
        self.assertEqual(rgba_sha256(Image.open(DRAFTS / 'C_01_28.source.png')), still['source_rgba_sha256'])
        mask = subject.region_mask(still['regions'], *english.size)
        changed = (np.asarray(source) != np.asarray(english)).any(-1)
        self.assertTrue(changed.any())
        self.assertFalse((changed & ~mask).any())
        self.assertTrue(all(row['style'] == 'unreviewed' and not row['runtime_integrated'] for row in manifest['assets']))
        subject.build(DRAFTS, self.root / 'again')
        self.assertEqual((self.root / 'out/manifest.json').read_bytes(), (self.root / 'again/manifest.json').read_bytes())

    def test_expansion_changes_the_source_identity(self):
        subject.build(DRAFTS, self.root / 'shift', expansion='shift')
        subject.build(DRAFTS, self.root / 'rep')
        shift = json.loads((self.root / 'shift/manifest.json').read_text())['assets'][0]
        replicate = json.loads((self.root / 'rep/manifest.json').read_text())['assets'][0]
        self.assertNotEqual(shift['source_rgba_sha256'], replicate['source_rgba_sha256'])

    def copy(self):
        copy = self.root / 'drafts'
        shutil.copytree(DRAFTS, copy)
        return copy

    def test_tampered_inputs_are_refused(self):
        copy = self.copy()
        spliced = copy / 'C_01_28.en.BS'
        data = bytearray(spliced.read_bytes())
        data[100] ^= 0xFF
        spliced.write_bytes(bytes(data))
        with self.assertRaisesRegex(ValueError, 'spliced BS differs'):
            subject.build(copy, self.root / 'a')
        self.assertFalse((self.root / 'a').exists())
        copy2 = self.root / 'drafts2'
        shutil.copytree(DRAFTS, copy2)
        manifest = json.loads((copy2 / 'manifest.json').read_text())
        x, y, w, h = manifest['assets'][0]['edit_regions'][0]
        manifest['assets'][0]['edit_regions'] = [[x + 16, y, w - 16, h]]
        (copy2 / 'manifest.json').write_text(json.dumps(manifest))
        with self.assertRaisesRegex(ValueError, 'outside the edit regions'):
            subject.build(copy2, self.root / 'b')
        (self.root / 'C_01_28.BS').write_bytes((BS_DIR / 'C_01_28.BS').read_bytes() + b'\0')
        with self.assertRaisesRegex(ValueError, 'original BS differs'):
            subject.build(DRAFTS, self.root / 'c', bs_dir=self.root)


if __name__ == '__main__':
    unittest.main()
