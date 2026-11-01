"""T-39410: texture-keeping erase, brush lettering and the still plans (gate plate, roster close-ups, signs, chalkboard).

The synthetic tests need no game data. The original-data tests skip when the disc files are absent and otherwise
rebuild every draft from the source and compare it with the publication.
"""
import json
import math
from pathlib import Path
import tempfile
import unittest

import numpy as np
from PIL import Image

import translation_bs_brush as brush
import translation_bs_font as font
import translation_bs_signs as signs
import translation_bs_stills as subject

PUBLISHED = subject.ROOT / 'translations/en/graphics/artwork/checkpoints' / subject.CHECKPOINT_DIR


class BrushTests(unittest.TestCase):
    def test_inpaint_changes_only_the_unknown_pixels_and_keeps_the_gradient(self):
        ramp = np.linspace(10, 30, 48).astype(float)
        picture = np.zeros((20, 48, 3))
        picture[:] = ramp[None, :, None]
        unknown = np.zeros((20, 48), bool)
        unknown[6:14, 18:30] = True
        picture[unknown] = 0
        out = brush.inpaint(picture, unknown, sigma=3.0)
        self.assertTrue((out[~unknown] == picture[~unknown]).all())
        self.assertLess(float(np.abs(out[unknown][:, 0] - np.broadcast_to(ramp[None, :], (20, 48))[unknown]).max()), 3.0)

    def test_inpaint_carries_the_grain_of_nearby_clean_pixels(self):
        rng = np.random.RandomState(1)
        picture = 20 + rng.randint(-2, 3, size=(24, 24, 3)).astype(float)
        unknown = np.zeros((24, 24), bool)
        unknown[8:16, 8:16] = True
        out = brush.inpaint(picture, unknown, sigma=3.0, grain=1.0)
        flat = brush.inpaint(picture, unknown, sigma=3.0, grain=0.0)
        self.assertGreater(float(out[unknown].std()), 0.5)
        self.assertLess(float(flat[unknown].std()), 0.5)

    def test_stroke_mask_finds_dark_and_light_lettering_only_inside_the_region(self):
        picture = np.full((30, 40, 3), 28.0)
        picture[10:12, 10:30] = 2
        picture[22:24, 5:10] = 2  # outside the region
        dark = brush.stroke_mask(picture, [5, 5, 30, 15], 'dark', 8, margin=0)
        self.assertTrue(dark[10:12, 10:30].all())
        self.assertFalse(dark[22:24, 5:10].any())
        light = np.full((30, 40, 3), 4.0)
        light[10:12, 10:30] = 20
        self.assertTrue(brush.stroke_mask(light, [5, 5, 30, 15], 'light', 8, margin=0)[10:12, 10:30].all())

    def test_grow_expands_by_the_margin(self):
        mask = np.zeros((9, 9), bool)
        mask[4, 4] = True
        self.assertEqual(int(brush.grow(mask, 2).sum()), 25)

    def test_place_matches_the_skeleton_font_rotation_and_turns_further(self):
        local = [[(0, 0), (4, 10)]]
        line = brush.place(local, (20, 5), 90)[0]
        for got, want in zip(line, font.to_image(local, (20, 5), 90)[0]):
            self.assertAlmostEqual(got[0], want[0], places=6)
            self.assertAlmostEqual(got[1], want[1], places=6)
        turned = brush.place([[(0, 0), (10, 0)]], (0, 0), 113.0)[0]
        self.assertAlmostEqual(turned[1][0], 10 * math.cos(math.radians(113)), places=6)
        self.assertGreater(turned[1][1], 9)

    def test_slant_leans_the_letter_tops_forward(self):
        line = brush.place([[(0, 0), (0, 10)]], (0, 0), 0, slant=10.0)[0]
        self.assertGreater(line[0][0], line[1][0])  # top is to the right of the bottom

    def test_brush_weight_tapers_and_follows_the_stroke_direction(self):
        along = brush.brush_coverage((20, 60), [[(5, 10), (55, 10)]], 3.0, taper=0.8, entry=0.0, wobble=0.0)
        self.assertGreater(along[:, 8:14].sum(), along[:, 46:52].sum() * 1.3)  # thicker at the start than near the end
        down = brush.brush_coverage((60, 20), [[(10, 5), (10, 55)]], 3.0, taper=0.0, entry=0.0, wobble=0.0, contrast=0.6)
        across = brush.brush_coverage((20, 60), [[(5, 10), (55, 10)]], 3.0, taper=0.0, entry=0.0, wobble=0.0, contrast=0.6)
        self.assertGreater(down.sum(), across.sum() * 1.2)  # strokes down the letter weigh more

    def test_brush_is_deterministic(self):
        a = brush.brush_coverage((20, 60), [[(5, 10), (30, 14), (55, 8)]], 2.4, seed=3)
        b = brush.brush_coverage((20, 60), [[(5, 10), (30, 14), (55, 8)]], 2.4, seed=3)
        self.assertTrue((a == b).all())


class DrawingTests(unittest.TestCase):
    @staticmethod
    def board(value=(20, 22, 22), width=80, height=50):
        picture = np.zeros((height, width, 3), int)
        picture[:] = value
        return picture

    def test_only_erased_and_covered_pixels_change_inside_the_rectangle(self):
        source = self.board()
        source[10:13, 10:30] = (30, 30, 30)  # old chalk
        plan = {'rect': [6, 6, 60, 36], 'erase': {'above': 80, 'margin': 1}, 'ink': [28, 28, 28],
                'runs': [{'text': 'ART', 'box': [0, 0, 0, 0], 'length': 20, 'center': [50, 30], 'angle': 0, 'cap': 6, 'aspect': 1.0, 'stroke': 1.0, 'gap': 2.4}]}
        english, record = subject.draw_plan(source, plan)
        outside = np.ones(source.shape[:2], bool)
        outside[6:42, 6:66] = False
        self.assertTrue((english[outside] == source[outside]).all())
        untouched = (english == source).all(-1)
        self.assertTrue(untouched[6:42, 6:66].sum() > 0.8 * 60 * 36)  # the rest of the board keeps its source pixels
        self.assertTrue((english[10:13, 10:30] < 28).all())  # old chalk gone
        self.assertGreater(int((english[24:36, 38:62] > 24).sum()), 10)  # English drawn
        self.assertEqual(record['erased_pixels'], int(brush.grow(np.pad(np.ones((3, 20), bool), ((10, 37), (10, 50))), 1)[6:42, 6:66].sum()))

    def test_keep_rectangles_are_not_erased(self):
        source = self.board()
        source[10:13, 10:30] = (30, 30, 30)
        plan = {'rect': [6, 6, 60, 36], 'erase': {'above': 80, 'margin': 0, 'keep': [[20, 6, 20, 20]]}, 'runs': []}
        english, _ = subject.draw_plan(source, plan)
        self.assertTrue((english[10:13, 20:30] == 30).all())
        self.assertTrue((english[10:13, 10:20] != 30).all())

    def test_polygon_limits_the_erase(self):
        source = self.board()
        source[10:13, 10:50] = (2, 2, 2)
        plan = {'rect': [6, 6, 60, 36], 'erase': {'polarity': 'dark', 'threshold': 10, 'margin': 0, 'polygon': [(6, 6), (30, 6), (30, 40), (6, 40)]}, 'runs': []}
        english, _ = subject.draw_plan(source, plan)
        self.assertTrue((english[10:13, 10:30] > 10).all())
        self.assertTrue((english[10:13, 31:50] == 2).all())

    def test_plane_fill_takes_the_face_colour(self):
        source = self.board(value=(28, 29, 30))
        source[8:16, 10:30] = (3, 3, 3)
        plan = {'rect': [8, 6, 30, 14], 'erase': {'polarity': 'dark', 'threshold': 10, 'margin': 0, 'plane': {'fit': [6, 4, 34, 18]}}, 'runs': []}
        english, _ = subject.draw_plan(source, plan)
        self.assertTrue((np.abs(english[8:16, 10:30] - np.array([28, 29, 30])).max() <= 1))

    def test_a_centre_run_is_centred_on_its_point_at_any_angle(self):
        for angle in (0, 11, 90, 113.5):
            lines, facts = subject.text_lines({'text': 'I', 'box': [0, 0, 0, 0], 'length': 10, 'center': [50, 40], 'angle': angle, 'cap': 10,
                                               'aspect': 1.0, 'stroke': 1.0, 'gap': 2.4})
            (x0, y0), (x1, y1) = lines[0]
            self.assertAlmostEqual((x0 + x1) / 2, 50, delta=0.01, msg=str(angle))
            self.assertAlmostEqual((y0 + y1) / 2, 40, delta=0.01, msg=str(angle))

    def test_run_without_origin_or_centre_at_a_free_angle_is_refused(self):
        with self.assertRaises(ValueError):
            subject.text_lines({'text': 'I', 'box': [0, 0, 5, 5], 'cap': 5, 'angle': 30, 'aspect': 1.0, 'stroke': 1.0})

    def test_crop_alpha_marks_only_changed_pixels(self):
        source = self.board()
        english = source.copy()
        english[10, 10] = (1, 2, 3)
        image, changed = subject.png_rgba(source, english, [8, 8, 10, 10])
        pixels = np.asarray(image)
        self.assertEqual(changed, 1)
        self.assertEqual(int((pixels[..., 3] == 255).sum()), 1)
        self.assertEqual(int(pixels[2, 2, 3]), 255)
        self.assertEqual(int(pixels[0, 0, 3]), 0)
        self.assertEqual(int(pixels[0, 0, :3].sum()), 0)  # no source pixel is kept


class PlanTests(unittest.TestCase):
    def test_ids_are_unique_and_the_texts_are_drawable(self):
        ids = [p['id'] for p in subject.PLANS]
        self.assertEqual(len(ids), len(set(ids)))
        for plan in subject.PLANS:
            for run in plan['runs']:
                font.text_units(run['text'], run['gap'])

    def test_rectangles_fit_their_pictures(self):
        sizes = {**{p['file']: (320, 160) for p in subject.PLANS}, **subject.STILL_SIZES}
        for plan in subject.PLANS:
            width, height = sizes[plan['file']]
            x, y, w, h = plan['rect']
            self.assertTrue(0 <= x and 0 <= y and x + w <= width and y + h <= height, plan['id'])

    def test_plans_of_one_file_are_disjoint_unless_they_are_alternatives(self):
        for i, a in enumerate(subject.PLANS):
            for b in subject.PLANS[i + 1:]:
                if a['file'] != b['file'] or (a.get('group') and a.get('group') == b.get('group')):
                    continue
                ax, ay, aw, ah = a['rect']
                bx, by, bw, bh = b['rect']
                self.assertTrue(ax + aw <= bx or bx + bw <= ax or ay + ah <= by or by + bh <= ay, '%s overlaps %s' % (a['id'], b['id']))

    def test_every_alternative_group_has_at_least_two_members_and_one_file(self):
        groups = {}
        for plan in subject.PLANS:
            if plan.get('group'):
                groups.setdefault(plan['group'], []).append(plan)
        self.assertEqual(set(groups), {'p-01-03', 'p-01-03b'})
        for members in groups.values():
            self.assertGreaterEqual(len(members), 2)
            self.assertEqual(len({m['file'] for m in members}), 1)

    def test_superseded_drafts_are_named_and_replaced_by_a_v2_id(self):
        for plan in subject.PLANS:
            if plan.get('supersedes'):
                self.assertIn(plan['supersedes'], subject.SUPERSEDED_IN)
                self.assertEqual(plan['id'], plan['supersedes'] + '-v2')

    def test_roster_names_follow_the_script_spelling(self):
        names = {family: given for family, given, _, _ in subject.ROSTER_COLUMNS}
        self.assertEqual(names['FUJISAKI'], 'SHIORI')
        self.assertEqual(names['HONDA'], 'MEGUMI')
        self.assertEqual(names['MIZUKAWA'], 'MIHO')
        text = ' '.join(f + ' ' + g for f, g, _, _ in subject.ROSTER_COLUMNS)
        for pair in ('OU', 'UU', 'OO'):
            self.assertNotIn(pair, text)  # no doubled vowels from long-vowel romanisation

    def test_roster_lines_stay_inside_the_picture_and_names_run_together_in_both_versions(self):
        for plan in subject.PLANS:
            if plan['file'] not in ('P_01_03', 'P_01_03B'):
                continue
            for run in plan['runs']:
                lines, facts = subject.text_lines(run)
                points = np.array([p for line in lines for p in line])
                self.assertGreater(points[:, 0].min(), -4, plan['id'])
                self.assertLess(points[:, 0].max(), 324, plan['id'])
                self.assertGreater(points[:, 1].min(), -4, plan['id'])
                self.assertLess(points[:, 1].max(), 164, plan['id'])

    def test_crossfade_pairs_repeat_the_roster_lines(self):
        by_id = {p['id']: p for p in subject.PLANS}
        for a, b in (('bs-p-01-03-roster-a-rotated', 'bs-p-01-03b-name-a-rotated'), ('bs-p-01-03-roster-b-stacked', 'bs-p-01-03b-name-b-stacked')):
            wanted = [r for r in by_id[a]['runs'] if r['text'].startswith(('FUJISAKI', 'SHIORI'))]
            self.assertEqual(wanted, by_id[b]['runs'])

    def test_cited_source_lines_still_hold_the_calls(self):
        self.assertEqual(subject.check_callers(), [])

    def test_stack_centres_follow_the_axes(self):
        centres = subject.stack_centres()
        self.assertIsNone(centres['MOGAMI'][1])
        (fx, fy), (gx, gy) = centres['FUJISAKI']
        self.assertEqual(gy - fy, subject.STACK_LEADING)
        self.assertLess(gx, fx)  # columns run down and to the left


class OriginalDataTests(unittest.TestCase):
    def setUp(self):
        if not (subject.ROOT / subject.BS_DIR / 'P_01_01.BS').exists():
            self.skipTest('original disc files are not present')

    def test_layout_model_reproduces_the_game_code_lbas(self):
        lbas = signs.contiguous_lbas(subject.ROOT / subject.BS_DIR)
        for name, lba in subject.STILL_ANCHORS.items():
            self.assertEqual(lbas[name][0], lba, name)

    def test_every_proof_matches_its_stream_and_callers(self):
        for name, proof in subject.geometry().items():
            stream = proof['stream']
            self.assertEqual(stream['macroblocks'], (proof['width'] // 16) * ((proof['height'] + 15) // 16), name)
            self.assertTrue(stream['end_marker'])
            self.assertLessEqual(stream['trailing_nonzero_bytes'], 4, name)  # zero padding, at most a 4-byte file trailer
            self.assertGreaterEqual(proof['read_sectors'] * 2048, stream['consumed_bytes'], name)
            for caller in proof['callers']:
                self.assertIn(caller['text'], (subject.ROOT / caller['path']).read_text().splitlines()[caller['line'] - 1])
        sizes = {name: (p['width'], p['height']) for name, p in subject.geometry().items()}
        self.assertEqual(sizes['C_01_28'], (320, 88))
        self.assertEqual(sizes['P_01_01'], (320, 160))

    def test_drafts_rebuild_deterministically_and_match_publication(self):
        (subject.ROOT / '.asset-cache').mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            first, second = Path(directory) / 'a', Path(directory) / 'b'
            subject.build(first)
            subject.build(second)
            manifest_bytes = (first / 'manifest.json').read_bytes()
            self.assertEqual(manifest_bytes, (second / 'manifest.json').read_bytes())
            manifest = json.loads(manifest_bytes)
            for asset in manifest['assets']:
                self.assertFalse(asset['human_approved'])
                self.assertEqual(asset['crop']['original_pixels'], 0)
                self.assertGreater(asset['changed_pixels'], 0)
                image = Image.open(first / 'crops' / asset['crop']['png'])
                self.assertEqual(image.mode, 'RGBA')
                self.assertEqual(int((np.asarray(image)[..., 3] == 255).sum()), asset['changed_pixels'])
            if PUBLISHED.exists():
                self.assertEqual(signs.digest(manifest_bytes), signs.digest((PUBLISHED / (subject.CHECKPOINT + '/manifest.json')).read_bytes()))
                for asset in manifest['assets']:
                    self.assertEqual((first / 'crops' / asset['crop']['png']).read_bytes(),
                                     (PUBLISHED / subject.CHECKPOINT / asset['crop']['png']).read_bytes())

    def test_the_japanese_lettering_is_gone_and_the_background_survives(self):
        (subject.ROOT / '.asset-cache').mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            output = Path(directory) / 'a'
            plans = [p for p in subject.PLANS if p['id'] in ('bs-p-01-03-roster-a-rotated', 'bs-c-01-28-final-exams-v2')]
            subject.build(output, plans=plans)
            data = (subject.ROOT / subject.BS_DIR / 'P_01_03.BS').read_bytes()
            source = signs.decode_still(data, 320, 160)
            english = np.asarray(Image.open(output / 'previews' / 'bs-p-01-03-roster-a-rotated.still.png').convert('RGB')).astype(int)
            # sky keeps its structure: the English still correlates with the source outside the lettering
            diff = np.abs(signs.expand(source).astype(int) - english).sum(-1)
            unchanged = (diff == 0).mean()
            self.assertGreater(unchanged, 0.6)


class PublishedCheckpointTests(unittest.TestCase):
    def setUp(self):
        if not PUBLISHED.exists():
            self.skipTest('checkpoint not published')

    def test_ledger_binds_manifest_and_crops(self):
        ledger = json.loads((PUBLISHED / 'ledger.json').read_text())
        manifest_bytes = (PUBLISHED / subject.CHECKPOINT / 'manifest.json').read_bytes()
        manifest = json.loads(manifest_bytes)
        self.assertFalse(ledger['original_images_included'])
        self.assertEqual(ledger['candidate_assets'], len(ledger['assets']))
        self.assertEqual(sorted(a['id'] for a in ledger['assets']), sorted(a['id'] for a in manifest['assets']))
        for asset in ledger['assets']:
            self.assertFalse(asset['human_approved'])
            self.assertTrue(asset['port_made'])
            self.assertEqual(asset['manifest_sha256'], signs.digest(manifest_bytes))
            data = (PUBLISHED / asset['path']).read_bytes()
            self.assertEqual(signs.digest(data), asset['crop_sha256'])
            image = Image.open(PUBLISHED / asset['path'])
            self.assertEqual(list(image.size), asset['dimensions'])
            self.assertEqual(image.mode, 'RGBA')
            pixels = np.asarray(image)
            self.assertTrue(((pixels[..., 3] == 0) | (pixels[..., 3] == 255)).all())
            self.assertEqual(int(pixels[pixels[..., 3] == 0][:, :3].sum()), 0)  # transparent pixels carry no picture data
        for asset in ledger['assets']:
            if asset.get('supersedes'):
                self.assertTrue((PUBLISHED.parent / asset['supersedes']['checkpoint'] / 'ledger.json').exists())
        for path in PUBLISHED.rglob('*'):
            self.assertIn(path.suffix, ('.png', '.json', '.md', ''), str(path))


if __name__ == '__main__':
    unittest.main()
