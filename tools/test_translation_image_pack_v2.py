"""Phase 4 WP-B: v2 pack emitters (translation_assets, translation_draft_pack).

Synthetic drafts plus the committed artwork snapshot; no original data. The
runtime reader is checked separately by
port/godot/tests/translation/run_image_store_v2_tests.gd.
"""
import json
from pathlib import Path
import re
import tempfile
import unittest

from PIL import Image

import translation_assets as subject
import translation_draft_pack as drafts


def sha(data):
    return subject.digest(data)


class DecoderIdentityTests(unittest.TestCase):
    def test_pins_match_the_runtime_identity(self):
        source = (subject.ROOT / 'port/godot/services/mdec_model_identity.gd').read_text()
        self.assertIn('const ID := "%s"' % subject.MDEC_MODEL_ID, source)
        for name, value in subject.MDEC_IMPLEMENTATIONS.items():
            self.assertIn('"%s": "%s"' % (name, value), source)
        test = (subject.ROOT / 'port/godot/tests/translation/run_image_store_v2_tests.gd').read_text()
        pinned = re.search(r'PINNED_DECODER := "([0-9a-f]{64})"', test)[1]
        self.assertEqual(subject.pinned_decoder_identity(), pinned)
        self.assertNotEqual(subject.decoder_identity('other', subject.MDEC_IMPLEMENTATIONS), pinned)


class RowContractTests(unittest.TestCase):
    def row(self, **changes):
        row = {'kind': 'page', 'source_container_sha256': '1' * 64, 'page': 1, 'source_png_sha256': '2' * 64,
               'source_rgba_sha256': '6' * 64, 'regions': [[0, 0, 4, 4]], 'dimensions': [8, 8], 'output': 'p.png', 'output_sha256': '3' * 64}
        row.update(changes)
        return row

    def test_valid_kinds(self):
        subject.check_v2_row(self.row())
        subject.check_v2_row({'kind': 'bs', 'source_container_sha256': '1' * 64, 'bs_index': 0,
                              'decoder_identity_sha256': '4' * 64, 'source_rgba_sha256': '5' * 64,
                              'regions': [[0, 0, 8, 8]], 'dimensions': [8, 8], 'output': 'b.png', 'output_sha256': '3' * 64})

    def test_rejections(self):
        cases = [self.row(kind='str'), self.row(regions=[]), self.row(regions=[[4, 4, 5, 1]]), self.row(regions=[[0, 0, 0, 1]]),
                 self.row(regions=[[0, 0, 1]]), self.row(page=-1), self.row(page=True), self.row(source_png_sha256='x'), self.row(source_rgba_sha256=None), self.row(source_rgba_sha256='y'),
                 self.row(output='../p.png'), self.row(output='/p.png'), self.row(dimensions=[0, 8]), self.row(dimensions=[True, 8]),
                 self.row(regions=[[0, 0, 1, 1]] * 65)]
        for row in cases:
            with self.assertRaises(ValueError, msg=str(row)):
                subject.check_v2_row(row)


class PackTests(unittest.TestCase):
    def setUp(self):
        (subject.ROOT / '.asset-cache').mkdir(exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache')
        self.root = Path(self.temp.name)
        self.snapshot = subject.ROOT / 'translations/en/graphics/artwork'

    def tearDown(self):
        self.temp.cleanup()

    def test_v2_pack_keeps_the_v1_selection(self):
        one = subject.build_pack(self.snapshot, self.root / 'v1')
        two = subject.build_pack(self.snapshot, self.root / 'v2', schema_version=2)
        first = json.loads((self.root / 'v1/manifest.json').read_text())
        second = json.loads((self.root / 'v2/manifest.json').read_text())
        self.assertEqual(first['schema'], subject.PACK_V1)
        self.assertNotIn('kind', first['assets'][0])
        self.assertEqual(second['schema'], subject.PACK_V2)
        self.assertEqual(one['assets'], two['assets'])
        self.assertTrue(all(row['kind'] == 'frame' and row['style'] == 'approve' for row in second['assets']))
        self.assertEqual([{k: v for k, v in row.items() if k != 'kind'} for row in second['assets']], first['assets'])
        self.assertFalse(second['development_drafts'])
        subject.build_pack(self.snapshot, self.root / 'v2b', schema_version=2)
        self.assertEqual((self.root / 'v2/manifest.json').read_bytes(), (self.root / 'v2b/manifest.json').read_bytes())

    def test_duplicate_identity_leaves_nothing(self):
        image = self.root / 'a.png'
        Image.new('RGBA', (4, 4)).save(image)
        row = {'kind': 'page', 'source_container_sha256': '1' * 64, 'page': 0, 'source_png_sha256': '2' * 64,
               'source_rgba_sha256': '6' * 64, 'regions': [[0, 0, 2, 2]], 'dimensions': [4, 4], 'output': 'a.png', 'output_sha256': sha(image.read_bytes())}
        with self.assertRaisesRegex(ValueError, 'duplicate'):
            subject.write_pack(self.root / 'dup', [row, dict(row)], [image, image], schema=subject.PACK_V2,
                               development_drafts=True, snapshot_sha256='0' * 64)
        self.assertFalse((self.root / 'dup').exists())

    def draft_dir(self, schema, rows):
        source = self.root / ('drafts-%d' % len(list(self.root.iterdir())))
        source.mkdir()
        for index, row in enumerate(rows):
            Image.new('RGBA', tuple(row['dimensions']), (index, 1, 2, 255)).save(source / row['output'])
            row['output_sha256'] = sha((source / row['output']).read_bytes())
        manifest = {'schema': schema, 'assets': rows}
        (source / 'manifest.json').write_text(json.dumps(manifest))
        return source

    def page_row(self, name='p0', page=0):
        return {'id': name, 'kind': 'page', 'source_container_sha256': '1' * 64, 'page': page, 'source_png_sha256': '2' * 64,
                'source_rgba_sha256': '6' * 64, 'regions': [[1, 1, 3, 3]], 'dimensions': [8, 8], 'output': name + '.png'}

    def bs_row(self, decoder=None):
        return {'id': 'b0', 'kind': 'bs', 'source_container_sha256': '1' * 64, 'bs_index': 0,
                'decoder_identity_sha256': decoder or subject.pinned_decoder_identity(), 'source_rgba_sha256': '5' * 64,
                'regions': [[0, 0, 16, 16]], 'dimensions': [32, 16], 'output': 'b0.png'}

    def test_page_and_bs_drafts_emit_v2(self):
        pages = self.draft_dir('tokimemo.filesel-page-drafts.v1', [self.page_row(), self.page_row('p1', 1)])
        result = drafts.build(pages, self.root / 'page-pack')
        manifest = json.loads((self.root / 'page-pack/manifest.json').read_text())
        self.assertEqual((result['schema'], manifest['schema']), (subject.PACK_V2, subject.PACK_V2))
        self.assertTrue(manifest['development_drafts'])
        self.assertTrue(all(row['kind'] == 'page' and row['style'] == 'unreviewed' for row in manifest['assets']))
        still = self.draft_dir('tokimemo.mdec-translation-drafts.v1', [self.bs_row()])
        drafts.build(still, self.root / 'bs-pack')
        self.assertEqual(json.loads((self.root / 'bs-pack/manifest.json').read_text())['assets'][0]['kind'], 'bs')

    def test_draft_negatives(self):
        pages = self.draft_dir('tokimemo.filesel-page-drafts.v1', [self.page_row()])
        with self.assertRaisesRegex(ValueError, 'palette'):
            drafts.build(pages, self.root / 'palette', palette='base')
        stale = self.draft_dir('tokimemo.mdec-translation-drafts.v1', [self.bs_row('6' * 64)])
        with self.assertRaisesRegex(ValueError, 'another decoder identity'):
            drafts.build(stale, self.root / 'stale')
        self.assertFalse((self.root / 'stale').exists())
        (pages / 'p0.png').write_bytes((pages / 'p0.png').read_bytes() + b'\0')
        with self.assertRaisesRegex(ValueError, 'draft bytes changed'):
            drafts.build(pages, self.root / 'tampered')
        self.assertFalse((self.root / 'tampered').exists())
        outside = self.page_row()
        outside['regions'] = [[6, 6, 4, 4]]
        bad = self.draft_dir('tokimemo.filesel-page-drafts.v1', [outside])
        with self.assertRaisesRegex(ValueError, 'outside the image'):
            drafts.build(bad, self.root / 'outside')
        self.assertFalse((self.root / 'outside').exists())

    def test_review_binds_page_regions(self):
        pages = self.draft_dir('tokimemo.filesel-page-drafts.v1', [self.page_row()])
        manifest_sha = sha((pages / 'manifest.json').read_bytes())
        row = json.loads((pages / 'manifest.json').read_text())['assets'][0]
        review = {'schema': 'tokimemo.agent-artwork-review.v1', 'draft_manifest_sha256': manifest_sha, 'human_approval': False,
                  'development_only': True, 'decision': 'source-style-and-layout-accepted', 'id': 'r1',
                  'assets': [{key: row[key] for key in ('id',) + drafts.REVIEW_IDENTITY['page']}]}
        path = self.root / 'review.json'
        path.write_text(json.dumps(review))
        result = drafts.build(pages, self.root / 'reviewed', agent_review=path)
        self.assertEqual(result['assets'], 1)
        self.assertFalse(json.loads((self.root / 'reviewed/manifest.json').read_text())['agent_review']['human_approval'])
        review['assets'][0]['regions'] = [[0, 0, 8, 8]]
        path.write_text(json.dumps(review))
        with self.assertRaisesRegex(ValueError, 'identity differs'):
            drafts.build(pages, self.root / 'moved', agent_review=path)

    def test_frame_drafts_v2_option(self):
        frame = {'id': 'f0', 'frame': 3, 'source_container_sha256': '1' * 64, 'source_png_sha256': '2' * 64,
                 'dimensions': [4, 4], 'output': 'f0.png'}
        source = self.draft_dir('tokimemo.option-translation-drafts.v1', [frame])
        drafts.build(source, self.root / 'f1', palette='base')
        drafts.build(source, self.root / 'f2', palette='base', schema_version=2)
        one = json.loads((self.root / 'f1/manifest.json').read_text())
        two = json.loads((self.root / 'f2/manifest.json').read_text())
        self.assertEqual((one['schema'], two['schema']), (subject.PACK_V1, subject.PACK_V2))
        self.assertNotIn('kind', one['assets'][0])
        self.assertEqual(two['assets'][0]['kind'], 'frame')
        with self.assertRaisesRegex(ValueError, 'need --palette'):
            drafts.build(source, self.root / 'f3')


if __name__ == '__main__':
    unittest.main()
