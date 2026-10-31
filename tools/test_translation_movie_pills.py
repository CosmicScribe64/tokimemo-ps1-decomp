"""T-39004 catalog-153: port-made OPENXA cast-credit pills and their cue table."""
import json
from pathlib import Path
import tempfile
import unittest
from unittest import mock

import numpy as np
from PIL import Image

import translation_inventory
import translation_movie_font as font
import translation_movie_pills as subject

CHECKPOINT = subject.ROOT / 'translations/en/graphics/artwork/checkpoints/T39004-movie-pills-v1'


def frames_with_caption(count, window, cover, colour=(250, 40, 246)):
    """Grey frames with a solid caption-coloured block inside cover on the window's frames."""
    frames = np.full((count, 192, 288, 3), 120, np.uint8)
    x0, y0, x1, y1 = cover
    for f in range(window[0], window[1] + 1):
        frames[f, y0 + 2:y1 - 2, x0 + 4:x1 - 4] = colour
    return frames


class SyntheticTests(unittest.TestCase):
    def test_every_large_glyph_is_well_formed_and_every_name_is_drawable(self):
        for character in font.LARGE:
            glyph = font.large_glyph(character)
            self.assertTrue(glyph['points'], character)
            self.assertLess(max(y for _, y in glyph['points']), font.LARGE_HEIGHT)
        needed = {c for _, _, pills in subject.CUES for p in pills if p[1] == 'actress' for c in p[3]} - {' '}
        self.assertLessEqual(needed, set(font.LARGE))
        with mock.patch.dict(font.LARGE, {'Q': (0, ['##', '#'])}):
            with self.assertRaisesRegex(ValueError, 'malformed'):
                font.large_glyph('Q')
        with mock.patch.dict(font.LARGE, {'Q': (14, ['#'] * 3)}):
            with self.assertRaisesRegex(ValueError, 'malformed'):
                font.large_glyph('Q')

    def test_cap_height_matches_the_glyph_data_and_the_ledger_text(self):
        self.assertEqual(font.CAP_HEIGHT, 13)
        for character in 'HKT':
            self.assertEqual(font.CAP_HEIGHT, len(font.LARGE[character][1]), character)
        self.assertEqual(font.LARGE_BASELINE + 1, font.CAP_HEIGHT)

    def test_italic_shifts_rows_above_the_baseline(self):
        mask = np.zeros((13, 2), bool)
        mask[:, 0] = True
        out = subject.italic(mask, 12)
        self.assertEqual([int(np.nonzero(row)[0][0]) for row in out], [3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0])

    def test_large_mask_is_bold_and_unscaled(self):
        mask = subject.large_mask('I')
        self.assertEqual(mask.shape[0], font.LARGE_HEIGHT)
        self.assertEqual(int(mask[font.LARGE_BASELINE].sum()), 3)  # 2-texel stem plus one bold texel

    def test_capsule_ends_are_round_unless_open_and_radius_narrows_the_rounding(self):
        alpha = subject.capsule_alpha(30, 10, False, False)
        self.assertLess(alpha[0, 0], 0.2)
        self.assertEqual(alpha[5, 15], 1.0)
        self.assertLess(alpha[1, 1], 1.0)
        square = subject.capsule_alpha(30, 10, True, False)
        self.assertEqual(square[0, 0], 1.0)
        self.assertLess(square[0, 29], 0.2)
        rounded = subject.capsule_alpha(30, 10, False, False, radius=2)
        self.assertLess(rounded[0, 0], 1.0)
        self.assertEqual(rounded[1, 1], 1.0)  # inside the small corner arc
        self.assertEqual(rounded[5, 0], 1.0)

    def test_pill_at_the_frame_edge_keeps_its_cap_unless_it_bleeds(self):
        mask = subject.large_mask('Sekine Akiko')
        art, (left, _) = subject.render_pill(mask, (172, 157, 288, 186), 'right', (158, 183))
        self.assertEqual(left + art.shape[1], 288)
        self.assertLess(art[0, -1, 3], 255)
        self.assertLess(art[-1, -1, 3], 255)
        bled, _ = subject.render_pill(mask, (172, 157, 288, 186), 'right', (158, 183), bleed=(False, True))
        self.assertEqual(int(bled[0, -1, 3]), 255)

    def test_bleed_needs_the_pill_body_down_the_edge_column(self):
        frames = np.full((30, 192, 288, 3), 120, np.uint8)
        frames[5:25, 10:30, 200:288] = (250, 40, 246)
        self.assertEqual(subject.bleeds(frames, (5, 24), (190, 5, 288, 35)), (False, True))
        frames[5:25, 10:30, 284:288] = 120
        frames[5:25, 18:22, 284:288] = (250, 40, 246)  # a cap tip: a few rows only
        self.assertEqual(subject.bleeds(frames, (5, 24), (190, 5, 288, 35)), (False, False))
        self.assertEqual(subject.bleeds(frames, (5, 24), (190, 5, 280, 35)), (False, False))

    def test_render_keeps_lettering_on_opaque_pill_and_encloses_the_cover(self):
        mask = subject.large_mask('Sekine Akiko')
        art, (left, top) = subject.render_pill(mask, (172, 157, 288, 186), 'right', (158, 183))
        self.assertEqual(left + art.shape[1], 288)
        self.assertLessEqual(left, 172)
        self.assertLessEqual(top, 158)
        self.assertGreaterEqual(top + art.shape[0], 184)
        colours = {tuple(p) for p in art[..., :3].reshape(-1, 3)}
        self.assertIn(subject.TEXT, colours)
        self.assertIn(subject.SHADOW, colours)
        with self.assertRaisesRegex(ValueError, 'does not fit'):
            subject.render_pill(np.ones((40, 10), bool), (0, 0, 20, 10), 'left', (0, 8))

    def test_coverage_rejects_uncovered_or_translucent_caption(self):
        art = np.zeros((10, 20, 4), np.uint8)
        art[..., 3] = 255
        art[0, 0, 3] = 100
        caption = np.zeros((4, 4), bool)
        caption[1, 1] = True
        self.assertEqual(subject.check_coverage(caption, (5, 5, 9, 9), art, [0, 0]), 1)
        with self.assertRaisesRegex(ValueError, 'outside the English pill'):
            subject.check_coverage(caption, (5, 5, 9, 9), art, [10, 0])
        with self.assertRaisesRegex(ValueError, 'translucent'):
            subject.check_coverage(caption, (5, 5, 9, 9), art, [6, 6])

    def test_measure_fails_closed_on_window_errors(self):
        cover = (10, 10, 60, 30)
        pill = ('p', 'character', '', 'P', cover, 'left')
        frames = frames_with_caption(40, (10, 25), cover)
        timing = subject.measure(frames, (10, 25), [pill])['p']
        self.assertEqual((timing['first_frame'], timing['last_frame']), (10, 25))
        self.assertEqual(timing['estimated_frames'], [])
        self.assertEqual(sorted(timing['fade_ink_ratio']), [10, 11, 24, 25])
        with self.assertRaisesRegex(ValueError, 'outside every pill window'):
            subject.measure(frames, (12, 25), [pill])
        with self.assertRaisesRegex(ValueError, 'cue window frame|below full'):
            gap = frames.copy()
            gap[17] = 120
            subject.measure(gap, (10, 25), [pill])
        # Caption-coloured scenery outside the window fails unless declared background.
        scenery = frames.copy()
        scenery[5:8] = frames[15]
        with self.assertRaisesRegex(ValueError, 'outside every pill window'):
            subject.measure(scenery, (10, 25), [pill])
        subject.measure(scenery, (10, 25), [pill], background=range(5, 8))

    def test_ghost_frames_widen_the_window(self):
        cover = (10, 10, 70, 30)
        frames = np.full((40, 192, 288, 3), 120, np.uint8)
        for f in range(10, 26):
            frames[f, 12:28, 14:66] = (250, 40, 246)
            frames[f, 16:24, 20:60:4] = 255  # white lettering strokes
        frames[9, 16:24, 20:60:4] = 150  # a faint ghost of the lettering, no pill colour
        self.assertEqual(subject.ghost_window(frames, (10, 25), [('p', 'character', '', 'P', cover, 'left')]), (9, 25))

    def test_raw_identity_is_pinned(self):
        with tempfile.TemporaryDirectory() as directory:
            track = Path(directory) / 'track1.bin'
            track.write_bytes(bytes(16))
            with self.assertRaisesRegex(ValueError, 'raw sectors differ'):
                subject.read_raw(track)

    def test_publish_refuses_changed_or_unflagged_outputs(self):
        with tempfile.TemporaryDirectory() as directory:
            built = Path(directory) / 'built'
            (built / 'pills').mkdir(parents=True)
            (built / 'pills/a.png').write_bytes(b'pill')
            (built / 'cue_table.json').write_bytes(b'{}')
            row = {'id': 'openxa-pill-a', 'output': 'pills/a.png', 'output_sha256': subject.digest(b'pill'),
                   'human_approved': False, 'port_made': False}
            manifest = {'assets': [row], 'cue_table_sha256': subject.digest(b'{}')}
            (built / 'manifest.json').write_text(json.dumps(manifest))
            target = CHECKPOINT.parent / ('test-refused-' + Path(directory).name)
            with self.assertRaisesRegex(ValueError, 'omits port-made'):
                subject.publish(built, target)
            (built / 'cue_table.json').write_bytes(b'[]')
            with self.assertRaisesRegex(ValueError, 'cue table changed'):
                subject.publish(built, target)
            self.assertFalse(target.exists())


@unittest.skipUnless(subject.TRACK.exists() and subject.EXE.exists(), 'original track 1 and executable required')
class OriginalDataTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache')
        cls.output = Path(cls.temp.name) / 'pills'
        subject.build(cls.output)
        cls.manifest_bytes = (cls.output / 'manifest.json').read_bytes()
        cls.table = json.loads((cls.output / 'cue_table.json').read_text())

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def test_cue_table_windows_and_placements(self):
        cues = self.table['cues']
        self.assertEqual(len(cues), 8)
        self.assertEqual(sum(len(c['pills']) for c in cues), 29)
        previous = -1
        for cue in cues:
            self.assertGreater(cue['first_frame'], previous)
            previous = max(p['last_frame'] for p in cue['pills'])
            for pill in cue['pills']:
                self.assertTrue(0 <= pill['x'] and pill['x'] + pill['width'] <= 288, pill['id'])
                self.assertTrue(0 <= pill['y'] and pill['y'] + pill['height'] <= 192, pill['id'])
                x0, y0, x1, y1 = pill['cover_rect']
                self.assertTrue(pill['x'] <= x0 and x1 <= pill['x'] + pill['width'], pill['id'])
                self.assertGreater(pill['caption_texels_covered'], 0)
                self.assertNotIn('opacity', pill)
                self.assertEqual((pill['first_frame'], pill['last_frame']), (cue['first_frame'], cue['last_frame']))
                self.assertFalse(pill['bleed']['left'] or pill['bleed']['right'], pill['id'])
                # Every end keeps a rounded cap, at the frame edge too.
                art = np.asarray(Image.open(self.output / pill['png']))
                self.assertLess(int(art[0, 0, 3]), 255, pill['id'])
                self.assertLess(int(art[-1, -1, 3]), 255, pill['id'])
            first, last = cue['caption_window']
            self.assertLessEqual(cue['first_frame'], first)
            self.assertLessEqual(last, cue['last_frame'])
        header = cues[0]['pills'][0]
        self.assertEqual((header['english'], header['first_frame'], header['last_frame']), ('Cast', 249, 276))
        self.assertEqual(header['estimated_frames'], [249, 250, 275, 276])

    def test_every_pill_is_port_made_and_unapproved(self):
        manifest = json.loads(self.manifest_bytes)
        self.assertEqual(len(manifest['assets']), 29)
        for row in manifest['assets']:
            self.assertTrue(row['port_made'])
            self.assertEqual([e['element'] for e in row['port_made_elements']], ['pill', 'lettering'])
            if row['role'] == 'actress':
                self.assertIn('cap height %d,' % font.CAP_HEIGHT, row['port_made_elements'][1]['description'])
            self.assertFalse(row['human_approved'])
            self.assertFalse(row['runtime_integrated'])

    def test_rebuild_is_deterministic_and_matches_the_checkpoint(self):
        again = Path(self.temp.name) / 'again'
        subject.build(again)
        self.assertEqual((again / 'manifest.json').read_bytes(), self.manifest_bytes)
        if not CHECKPOINT.exists():
            self.skipTest('checkpoint not published')
        revision = CHECKPOINT / subject.REVISION
        self.assertEqual((revision / 'manifest.json').read_bytes(), self.manifest_bytes)
        self.assertEqual((revision / 'cue_table.json').read_bytes(), (self.output / 'cue_table.json').read_bytes())
        ledger = json.loads((CHECKPOINT / 'ledger.json').read_text())
        self.assertEqual(len(ledger['assets']), 29)
        self.assertFalse(ledger['original_images_included'])
        for row in ledger['assets']:
            self.assertEqual((row['port_made'], row['human_approved'], row['status']), (True, False, 'draft'))
            self.assertEqual(subject.digest((CHECKPOINT / row['path']).read_bytes()), row['output_sha256'])
            self.assertEqual((self.output / row['path'].split('/', 1)[1]).read_bytes(), (CHECKPOINT / row['path']).read_bytes())

    def test_inventory_reads_the_pills_as_catalog_153(self):
        ledger = CHECKPOINT / 'ledger.json'
        self.assertIn(ledger, translation_inventory.LEDGERS)
        entries = translation_inventory.load_ledgers([ledger])
        self.assertEqual({translation_inventory.candidate_key(e) for e in entries}, {('catalog-153', None, None, None)})
        status = {item['id']: item['status'] for item in translation_inventory.build()['artwork_items']}
        self.assertEqual(status['catalog-153'], 'candidate-only')


if __name__ == '__main__':
    unittest.main()
