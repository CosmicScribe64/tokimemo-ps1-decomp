"""T-39400: port-made BS sign lettering (skeleton font, face fill, plans, published checkpoint).

The synthetic tests need no game data. The original-data tests skip when the disc files are
absent and otherwise rebuild every draft from the source and compare it with the publication.
"""
import json
from pathlib import Path
import tempfile
import unittest

import numpy as np
from PIL import Image

import translation_bs_common as common
import translation_bs_font as font
import translation_bs_signs as subject

PUBLISHED = subject.ROOT / 'translations/en/graphics/artwork/checkpoints/T39400-bs-signs-v1'
WANTED_CHARACTERS = set(''.join(run['text'] for plan in subject.SIGNS for run in plan['runs']))


class FontTests(unittest.TestCase):
    def test_every_character_the_plans_use_has_a_skeleton(self):
        for character in WANTED_CHARACTERS - {' '}:
            self.assertIn(character, font.GLYPHS)
        for letter in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
            self.assertIn(letter, font.GLYPHS)

    def test_unknown_character_is_refused(self):
        with self.assertRaises(ValueError):
            font.text_units('Aあ')

    def test_every_glyph_draws_ink_inside_its_cap_box(self):
        for character, (width, parts) in font.GLYPHS.items():
            lines = font.to_image(font.strokes_for(character, 40, 20, 1.0), (10, 10), 0)
            coverage = font.render_coverage((40, 60), lines, 2.0)
            self.assertGreater(coverage.sum(), 2, character)
            ys, xs = np.nonzero(coverage > 0.05)
            self.assertGreaterEqual(ys.min(), 10 - 2, character)
            self.assertLessEqual(ys.max(), 10 + 20 + 3, character)

    def test_text_fits_its_length_and_is_centred(self):
        facts = font.text_extent('KIRAMEKI', 29, 8, 9, 2.6)
        self.assertLessEqual(facts['length_px'], 29.001)
        wide = font.text_extent('IT', 100, 10, 1.0, 2.4)
        self.assertAlmostEqual(wide['offset_px'], (100 - wide['length_px']) / 2, places=2)

    def test_rotation_runs_the_text_down_with_letter_tops_to_the_right(self):
        local = [[(0, 0), (4, 10)]]
        self.assertEqual(font.to_image(local, (20, 5), 90), [[(20, 5), (10, 9)]])
        self.assertEqual(font.to_image(local, (20, 5), 0), [[(20, 5), (24, 15)]])
        with self.assertRaises(ValueError):
            font.to_image(local, (0, 0), 45)

    def test_stroke_weight_sets_the_ink_amount(self):
        thin = font.render_coverage((20, 20), [[(2, 10), (18, 10)]], 1.0).sum()
        thick = font.render_coverage((20, 20), [[(2, 10), (18, 10)]], 3.0).sum()
        self.assertGreater(thick, 2.5 * thin)
        self.assertAlmostEqual(thin, 16 + 1.0, delta=2.5)


class PlanTests(unittest.TestCase):
    def test_plans_are_consistent(self):
        ids = [plan['id'] for plan in subject.SIGNS]
        self.assertEqual(len(ids), len(set(ids)))
        for plan in subject.SIGNS:
            width, height = (640, 160) if plan['file'] == 'P_01_02' else (320, 160)
            x, y, w, h = plan['rect']
            self.assertTrue(0 <= x and 0 <= y and x + w <= width and y + h <= height, plan['id'])
            for run in plan['runs']:
                bx, by, bw, bh = run['box']
                self.assertTrue(x <= bx and bx + bw <= x + w and y <= by and by + bh <= y + h, plan['id'])
        by_file = {}
        for plan in subject.SIGNS:
            for other in by_file.setdefault(plan['file'], []):
                ax, ay, aw, ah = plan['rect']
                bx, by, bw, bh = other['rect']
                disjoint = ax + aw <= bx or bx + bw <= ax or ay + ah <= by or by + bh <= ay
                self.assertTrue(disjoint, '%s overlaps %s' % (plan['id'], other['id']))
            by_file[plan['file']].append(plan)

    def test_every_plan_names_a_known_file_with_caller_evidence(self):
        for plan in subject.SIGNS:
            if plan['file'] != 'P_01_02':
                self.assertIn(plan['file'], subject.CALLERS)

    def test_cited_source_lines_still_hold_the_calls(self):
        self.assertEqual(subject.check_callers(), [])

    def test_catalog_bindings_match_the_plans(self):
        rows = [json.loads(line) for line in (subject.ROOT / 'translations/en/graphics/image_text_catalog.jsonl').read_bytes().splitlines() if line]
        bound = {int(plan['catalog_item'].split('-')[1]): plan['file'] for plan in subject.SIGNS}
        for line, name in bound.items():
            path = rows[line - 1]['asset_path']
            self.assertIn(name + '.BS', path, 'catalog line %d' % line)


class DrawingTests(unittest.TestCase):
    @staticmethod
    def flat(width=64, height=32, value=(28, 29, 30)):
        image = np.zeros((height, width, 3), int)
        image[:] = value
        return image

    def plan(self, **changes):
        plan = {'id': 'test', 'rect': [8, 6, 40, 16], 'init': 'light',
                'runs': [{'text': 'ART', 'box': [10, 8, 36, 12], 'cap': 10, 'aspect': 1.0, 'stroke': 1.6}]}
        plan.update(changes)
        return plan

    def test_only_the_rectangle_changes_and_the_face_is_flat(self):
        source = self.flat()
        source[10:14, 12:30] = 2  # old lettering
        english, record = subject.draw_plan(source, self.plan(fit=[6, 4, 44, 20]))
        outside = np.ones(source.shape[:2], bool)
        outside[6:22, 8:48] = False
        self.assertTrue((english[outside] == source[outside]).all())
        face = english[6:22, 8:48]
        self.assertTrue((face == np.array([28, 29, 30])).all(-1).any())
        self.assertGreater(int((face.sum(-1) < 40).sum()), 20)  # English ink is drawn
        self.assertEqual(record['lines'][0]['text'], 'ART')

    def test_plane_fill_follows_a_gradient_and_ignores_lettering(self):
        source = self.flat()
        ramp = np.linspace(20, 30, 64).astype(int)
        source[:, :, :] = ramp[None, :, None]
        source[8:20, 10:46] = 0
        face, used = subject.fit_plane(source, [8, 6, 40, 16], [0, 0, 64, 32])
        self.assertLess(float(np.abs(face[..., 0] - ramp[None, 8:48]).max()), 1.2)
        self.assertGreater(used, 500)

    def test_plane_fit_reports_the_samples_its_coefficients_came_from(self):
        source = self.flat(value=(20, 20, 20))
        source.reshape(-1, 3)[::10] = 23  # within the median start band (2 x tolerance), outside the fitted band
        face, used = subject.fit_plane(source, [8, 6, 40, 16], [0, 0, 64, 32], rounds=1)
        self.assertEqual(used, 64 * 32)  # the one round fitted every sample; the narrower refit set was never used

    def test_diffuse_fill_ignores_dark_neighbours_and_stays_smooth(self):
        source = self.flat(value=(26, 26, 28))
        source[12:30, 30:36] = 0  # a stray dark mark touching the margin ring
        face, known = subject.fill_diffuse(source, [8, 6, 20, 16], 3)
        self.assertLess(float(np.abs(face - np.array([26, 26, 28])).max()), 1.0)
        self.assertGreater(known, 20)

    def test_plan_without_lettering_only_refills(self):
        source = self.flat()
        source[8:14, 10:20] = 3
        english, record = subject.draw_plan(source, self.plan(runs=[], fit=[6, 4, 44, 20]))
        self.assertIsNone(record['ink_rgb555'])
        self.assertTrue((english[6:22, 8:48] == np.array([28, 29, 30])).all())

    def test_ink_is_the_colour_furthest_from_the_face(self):
        face = np.full((10, 20, 3), 30.0)
        source = np.full((20, 40, 3), 30, int)
        source[5:9, 5:12] = (6, 5, 4)
        ink, count = subject.sample_ink(source, [0, 0, 20, 10], face)
        self.assertLess(float(ink.max()), 8)
        with self.assertRaises(ValueError):
            subject.sample_ink(np.full((20, 40, 3), 30, int), [0, 0, 20, 10], face)

    def test_class_mark_removals_name_the_diffusion_fill(self):
        for plan in subject.SIGNS:
            self.assertIn(plan.get('fill', 'plane'), subject.FILL_NOTES, plan['id'])
            if not plan['runs']:
                self.assertEqual(plan.get('fill'), 'diffuse', plan['id'])


class OriginalDataTests(unittest.TestCase):
    def setUp(self):
        if not (subject.ROOT / subject.BS_DIR / 'D_01_47.BS').exists():
            self.skipTest('original disc files are not present')

    def test_layout_model_reproduces_every_game_code_lba(self):
        lbas = subject.contiguous_lbas(subject.ROOT / subject.BS_DIR)
        for name, lba in subject.LAYOUT_ANCHORS.items():
            self.assertEqual(lbas[name][0], lba, name)

    def test_every_proof_matches_its_stream_and_callers(self):
        proofs = subject.geometry()
        for name, proof in proofs.items():
            stream = proof['stream']
            self.assertEqual(stream['macroblocks'], (proof['width'] // 16) * (proof['height'] // 16), name)
            self.assertTrue(stream['end_marker'])
            self.assertGreaterEqual(proof['read_sectors'] * 2048, stream['consumed_bytes'], name)
            for caller in proof['callers']:
                self.assertIn(caller['text'], common.function_body((subject.ROOT / caller['path']).read_text(), caller['function']))

    def test_drafts_rebuild_deterministically_and_match_publication(self):
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            first, second = Path(directory) / 'a', Path(directory) / 'b'
            subject.build(first)
            subject.build(second)
            manifest_bytes = (first / 'manifest.json').read_bytes()
            self.assertEqual(manifest_bytes, (second / 'manifest.json').read_bytes())
            manifest = json.loads(manifest_bytes)
            sources = {}
            for asset in manifest['assets']:
                self.assertFalse(asset['human_approved'])
                self.assertEqual(asset['edit_regions'], [asset['crop']['rect']])
                self.assertGreater(asset['changed_pixels'], 0)
                # the recorded measure is the count of crop pixels equal to the source decode at their position
                name = asset['source_container'].split('/')[-1][:-3]
                if name not in sources:
                    geometry = manifest['geometry_proofs'][name]
                    sources[name] = common.decode_still((subject.ROOT / subject.BS_DIR / (name + '.BS')).read_bytes(),
                                                        geometry['width'], geometry['height'])
                x, y, w, h = asset['crop']['rect']
                crop = Image.open(first / 'crops' / asset['crop']['png'])
                self.assertEqual(asset['crop']['source_equal_pixels'], common.source_equal_pixels(crop, sources[name][y:y + h, x:x + w]))
                self.assertEqual(asset['crop']['fill'], subject.FILL_NOTES[next(p for p in subject.SIGNS if p['id'] == asset['id']).get('fill', 'plane')])
            if PUBLISHED.exists():
                mismatch = common.environment_mismatch(json.loads((PUBLISHED / 'bs-signs-v1/manifest.json').read_bytes()))
                if mismatch:
                    self.skipTest(mismatch)
                self.assertEqual(subject.digest(manifest_bytes),
                                 subject.digest((PUBLISHED / 'bs-signs-v1/manifest.json').read_bytes()))
                for asset in manifest['assets']:
                    self.assertEqual((first / 'crops' / asset['crop']['png']).read_bytes(),
                                     (PUBLISHED / 'bs-signs-v1' / asset['crop']['png']).read_bytes())


class PublishedCheckpointTests(unittest.TestCase):
    def setUp(self):
        if not PUBLISHED.exists():
            self.skipTest('checkpoint not published')

    def test_ledger_binds_manifest_and_crops(self):
        ledger = json.loads((PUBLISHED / 'ledger.json').read_text())
        manifest_bytes = (PUBLISHED / 'bs-signs-v1/manifest.json').read_bytes()
        manifest = json.loads(manifest_bytes)
        self.assertFalse(ledger['original_images_included'])
        self.assertEqual(ledger['candidate_assets'], len(ledger['assets']))
        self.assertEqual(sorted(a['id'] for a in ledger['assets']), sorted(a['id'] for a in manifest['assets']))
        for asset in ledger['assets']:
            self.assertFalse(asset['human_approved'])
            self.assertTrue(asset['port_made'])
            self.assertEqual(asset['manifest_sha256'], subject.digest(manifest_bytes))
            data = (PUBLISHED / asset['path']).read_bytes()
            self.assertEqual(subject.digest(data), asset['crop_sha256'])
            record = next(r for r in manifest['assets'] if r['id'] == asset['id'])
            self.assertEqual(record['crop']['sha256'], asset['crop_sha256'])
            self.assertEqual(record['source_container_sha256'], asset['source_container_sha256'])
            with Image.open(PUBLISHED / asset['path']) as image:
                self.assertEqual(list(image.size), asset['dimensions'])
        for path in PUBLISHED.rglob('*'):
            self.assertIn(path.suffix, ('.png', '.json', ''), str(path))
        self.assertEqual({p.name for p in (PUBLISHED / 'bs-signs-v1').iterdir()} - {'manifest.json'},
                         {a['crop']['png'] for a in manifest['assets']})


if __name__ == '__main__':
    unittest.main()
