"""T-39410: MAINROM art fixes. Layout rules are tested without game data; the build test needs the disc."""
import hashlib
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_mainrom_drafts as base
import translation_mainrom_fixes as subject

SOURCES = subject.ROOT / '.asset-cache/translation-assets/original-mainrom-v1'
PACK = subject.ROOT / 'disc/files/CDROM/FUKUTEST/MAINROM.NBN'
EXECUTABLE = subject.ROOT / 'disc/files/SLPM_86.053'
PUBLISHED = subject.CHECKPOINTS / subject.CHECKPOINT
GREY, RED = 0x4210, 0x001f

TAG_WIDTHS = {134: 40, 135: 48, 136: 40, 137: 40, 138: 24, 139: 24, 140: 40, 141: 32, 142: 24, 143: 64, 144: 32, 145: 32}
TAG_SHADOW = {144, 145}   # the 32x16 event tags draw their lettering with a (+1,+1) shadow
BANNER_WIDTH = {126: 136, 127: 120, 128: 112, 129: 144, 130: 112, 131: 144, 132: 152, 133: 152}


def synthetic(width=12, height=9, parts=1):
    palette = [0, RED] + [GREY] * 14
    layers = [{'index': n, 'x': 0, 'y': 0, 'w': width, 'h': height,
               'texels': [[0] * width for _ in range(height)]} for n in range(parts)]
    return base.Frame.from_layers(999, width, height, layers, {v: [palette] * parts for v in subject.VARIANTS})


def faces():
    return subject.small_faces(base.panel7_face())


class BitmapTests(unittest.TestCase):
    def test_bitmaps_are_well_formed(self):
        subject.check_bitmaps()
        for ch, bitmap in subject.BOLD.items():
            self.assertEqual(len(bitmap), subject.BOLD_HEIGHT + len(subject.BOLD_DESCENDERS.get(ch, ())), ch)
        faces = subject.round2_faces()   # raises on a malformed drawing
        self.assertEqual({max(y for _, y in g['points']) + 1 for g in faces['bold6'].glyphs.values()}, {6})
        self.assertEqual({max(y for _, y in g['points']) + 1 for g in faces['bold7'].glyphs.values()}, {7})
        self.assertEqual({max(y for _, y in g['points']) + 1 for g in faces['semi10'].glyphs.values()}, {10})

    def test_q_has_a_tail_below_the_baseline(self):
        q = subject.bold_face().glyphs['Q']['points']
        tail = [(x, y) for x, y in q if y >= subject.BOLD_HEIGHT]
        self.assertGreaterEqual(len(tail), 4)
        self.assertGreater(max(x for x, _ in tail), max(x for x, _ in subject.bold_face().glyphs['O']['points']))

    def test_bold_face_has_every_capital_of_the_headlines(self):
        face = subject.bold_face()
        for text, *_ in subject.HEADLINES.values():
            for ch in text.replace(' ', ''):
                self.assertIn(ch, face.glyphs)

    def test_stretch_repeats_rows_only(self):
        drawing = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o']
        out = subject.stretched(drawing)
        self.assertEqual(len(out), 17)
        self.assertEqual(sorted(set(out)), sorted(drawing))

    def test_cover_glyphs_exist_for_every_title(self):
        faces = subject.round2_faces()
        for spec in subject.COVERS.values():
            for line in spec['lines']:
                for ch in line['text'].replace(' ', ''):
                    self.assertIn(ch, faces[line['face']].glyphs)

    def test_cover_titles_fit_the_logo_columns(self):
        faces = subject.round2_faces()
        for frame, spec in subject.COVERS.items():
            for line in spec['lines']:
                xs = [x for x, _ in subject.layout_line(faces[line['face']], line)]
                rim = 1 if line['outline'] is not None else 0
                right = max(xs) + rim + (1 if line['shadow'] is not None else 0)
                self.assertGreaterEqual(min(xs) - rim, subject.COVER_X[0], (frame, line['text']))
                self.assertLess(right, sum(subject.COVER_X), (frame, line['text']))
        self.assertEqual(subject.COVERS[153]['title'], 'POWER PROS')


class LayoutTests(unittest.TestCase):
    def test_place_centres_and_keeps_the_margin(self):
        frame = synthetic()
        face = base.Face('t', {'I': {'points': [(0, 0), (0, 1)], 'width': 1}}, 1, {})
        box = subject.place(frame, face, 'II', [0, 0, 12, 9], 1, {0}, align='center', margin=1)
        self.assertEqual(box[0], (12 - 3) // 2)
        with self.assertRaises(ValueError):
            subject.place(frame, face, 'I' * 6, [0, 0, 12, 9], 1, {0}, margin=1)

    def test_left_alignment_is_one_parameter(self):
        frame = synthetic()
        face = base.Face('t', {'I': {'points': [(0, 0)], 'width': 1}}, 1, {})
        box = subject.place(frame, face, 'II', [0, 0, 12, 9], 1, {0}, align='left', margin=1)
        self.assertEqual(box[0], 1)
        self.assertEqual(subject.TAG_ALIGN, 'center')

    def test_every_tag_keeps_a_one_pixel_margin_each_side(self):
        small = faces()
        for frame, lines in subject.TAG_TEXT.items():
            shadow = 1 if frame in TAG_SHADOW else None
            face = subject.fit_face(small, lines, TAG_WIDTHS[frame] - 2 * subject.TAG_MARGIN, shadow)
            for line in lines:
                self.assertLessEqual(base.line_width(face, line, shadow), TAG_WIDTHS[frame] - 2, (frame, line))

    def test_headlines_fit_their_banners_in_one_size(self):
        face = subject.bold_face()
        for frame, (text, *_) in subject.HEADLINES.items():
            _, right, up, down = subject.extent(face, text)
            self.assertLessEqual(right + 2, BANNER_WIDTH[frame], text)
            self.assertEqual(down - up + 1, subject.BOLD_HEIGHT + (2 if 'Q' in text else 0), text)
        self.assertEqual(subject.HEADLINES[132][0], 'KARAOKE BOX OPENS')
        self.assertEqual(subject.HEADLINE_ALIGN, 'left')

    def test_soft_edges_take_only_corner_notches(self):
        o = base.bitmap_glyph(['.####.', '######', '##..##', '##..##', '######', '.####.'])['points']
        ink = set(o)
        shadow = {(x + 1, y + 1) for x, y in ink} - ink
        edges = subject.soft_edges(ink, shadow)
        self.assertEqual(edges.get((0, 0)), 'highlight')
        self.assertEqual(edges.get((5, 0)), 'highlight')
        self.assertEqual(edges.get((0, 5)), 'mid')
        for counter in ((2, 2), (3, 2), (2, 3), (3, 3)):
            self.assertNotIn(counter, edges)   # the counter stays open

    def test_headline_wording_follows_the_suffix_rule(self):
        suffix = {'完成': 'OPENS', '特集': 'SPECIAL', '紹介': 'GUIDE'}
        for text, japanese, word in subject.HEADLINES.values():
            self.assertTrue(text.endswith(word.upper()))
            self.assertEqual(suffix[japanese[-2:]], word.upper())

    def test_stat_labels_use_the_approved_names(self):
        labels = [t for column in subject.STATS.values() for t in column]
        approved = {'Health', 'Hum.', 'Science', 'Art', 'Sports', 'Trivia', 'Looks', 'Guts', 'Stress'}
        self.assertEqual(set(labels), approved)
        self.assertEqual(len(labels), 9)
        face = subject.fit_face(faces(), labels, subject.STAT_MAX_WIDTH)
        for text in labels:
            self.assertLessEqual(base.line_width(face, text, None), subject.STAT_MAX_WIDTH)

    def test_issue_badges_name_the_seasons(self):
        self.assertEqual(subject.ISSUE_SEASONS, {122: 'SPRING', 123: 'SUMMER', 124: 'AUTUMN', 125: 'WINTER'})
        self.assertEqual(subject.ISSUE_MONTHS[124], '9-10-11')

    def test_season_lines_fill_the_badge_rows_in_one_size(self):
        face = subject.round2_faces()['bold6w']
        widths = [base.line_width(face, t, None) for t in list(subject.ISSUE_SEASONS.values()) + [subject.ISSUE_WORD]]
        self.assertLessEqual(max(widths), 56 - 2)   # 1 px clear each side, as the Japanese
        self.assertGreaterEqual(max(widths), 40)
        first, second = subject.ISSUE_ROWS
        self.assertEqual((first, second + subject.BOLD6_HEIGHT - 1), (1, 14))   # the Japanese rows
        self.assertGreater(second, first + subject.BOLD6_HEIGHT)

    def test_line_ramp_keeps_the_dark_edges(self):
        table = {y: (7 if y in (1, 14) else 2) for y in range(16)}
        self.assertEqual(subject.line_ramp(table, 1, 14, 6), [7, 2, 2, 2, 2, 7])

    def test_wrestling_fits_with_a_five_pixel_w(self):
        face = subject.wrestling_face(faces())
        self.assertEqual(face.glyphs['W']['width'], 5)
        self.assertLessEqual(base.line_width(face, 'Wrestling', None), 32 - 2 * subject.TAG_MARGIN)
        plain = base.Face('p', face.glyphs, face.space, {})
        self.assertEqual(base.line_width(plain, 'Wrestling', 1) - base.line_width(face, 'Wrestling', 1), 2)
        self.assertEqual(subject.TAG_TEXT[144], ('Wrestling',))

    def test_wrestling_t_is_not_an_l_and_keeps_clear_of_the_s(self):
        face = subject.wrestling_face(faces())
        top = lambda ch: min(y for _, y in face.glyphs[ch]['points'])
        self.assertGreater(top('t'), top('l'))   # shorter than the l
        t = set(face.glyphs['t']['points'])
        self.assertTrue({(0, 2), (1, 2), (2, 2)} <= t)   # two-sided crossbar
        pts = face.points('st')
        s_pts = set(pts[:len(face.glyphs['s']['points'])])
        t_pts = set(pts[len(s_pts):])
        near = {(x + dx, y + dy) for x, y in s_pts for dx in (-1, 0, 1) for dy in (-1, 0, 1)}
        self.assertFalse(near & t_pts)   # 1 px clear between s and t, corners included


class ResolutionTests(unittest.TestCase):
    def test_every_weekday_variant_resolves_to_the_two_letter_set(self):
        result = subject.weekday_resolution()
        self.assertEqual(len(result['resolves_to']), 7 * len(subject.VARIANTS))
        for key, row in result['resolves_to'].items():
            self.assertIn('two-letter', row['id'], key)
            self.assertTrue(row['human_approved'])
        # base and icon-52 share one image and therefore one file
        for f in subject.WEEKDAY_FRAMES:
            self.assertEqual(result['resolves_to']['%04d.base' % f], result['resolves_to']['%04d.icon-52' % f])

    def test_stale_three_letter_drafts_are_listed(self):
        stale = subject.weekday_resolution()['stale_three_letter_to_supersede']
        self.assertEqual(len(stale), 12)
        self.assertTrue(all(s['id'].startswith('draft:mainrom-weekday-') for s in stale))

    def test_superseded_candidates_exist_in_their_ledgers(self):
        old = subject.superseded_candidates()
        self.assertIn(61, old)
        for rows in old.values():
            for c in rows:
                self.assertTrue((subject.ROOT / c['ledger']).exists())
                self.assertTrue(c['palette_ids'])


class PublishedTests(unittest.TestCase):
    def setUp(self):
        if not PUBLISHED.exists():
            self.skipTest('checkpoint not published')
        self.ledger = json.loads((PUBLISHED / 'ledger.json').read_text())

    def test_one_file_per_variant_with_exact_hashes(self):
        seen = set()
        for a in self.ledger['assets']:
            data = (PUBLISHED / a['path']).read_bytes()
            self.assertEqual(hashlib.sha256(data).hexdigest(), a['output_sha256'], a['id'])
            self.assertEqual(a['id'], 'mainrom-fix-%04d.%s' % (a['frame'], a['palette_id']))
            self.assertFalse(a['human_approved'])
            self.assertTrue(a['port_made'])
            seen.add((a['frame'], a['palette_id']))
        frames = {f for f, _ in seen}
        self.assertEqual(len(seen), len(frames) * len(subject.VARIANTS))
        self.assertEqual(len(list((PUBLISHED / subject.REVISION).glob('*.png'))), len(self.ledger['assets']))

    def test_supersedes_name_ledgers_and_hashes(self):
        for s in self.ledger['supersedes']:
            self.assertEqual(len(s['output_sha256']), 64)
            self.assertTrue((subject.ROOT / s['ledger']).exists())

    def test_register_note_exists(self):
        text = (PUBLISHED / 'REGISTER.md').read_text()
        self.assertIn('Supersedes', text)
        self.assertIn('weekday-resolution.json', text)

    def test_images_stay_inside_the_original_palette_counts(self):
        for a in self.ledger['assets'][:12]:
            image = Image.open(PUBLISHED / a['path']).convert('RGBA')
            self.assertEqual(list(image.size), a['dimensions'])


@unittest.skipUnless(PACK.exists() and SOURCES.exists(), 'needs the extracted disc and original MAINROM renders')
class BuildTests(unittest.TestCase):
    def test_build_reproduces_the_published_checkpoint(self):
        if not PUBLISHED.exists():
            self.skipTest('checkpoint not published')
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as tmp:
            result, prepared, _ = subject.build(SOURCES, PACK, EXECUTABLE, Path(tmp) / 'out')
        published = {a['id']: a['output_sha256'] for a in json.loads((PUBLISHED / 'ledger.json').read_text())['assets']}
        built = {row['id']: row['output_sha256'] for _, _, row in prepared}
        self.assertEqual(built, published)
        self.assertEqual(result['assets'], len(published))

    def test_pixels_outside_edit_regions_equal_the_source(self):
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as tmp:
            _, prepared, _ = subject.build(SOURCES, PACK, EXECUTABLE, Path(tmp) / 'out')
        for image, original, row in prepared:
            base.check(original, image, row['edit_regions'], row['id'])
        for image, original, row in prepared:
            if row['palette_id'] != 'base':
                continue
            if row['item'] == 2:   # flush left where the Japanese starts
                self.assertEqual(row['lettering_bbox'][0], row['japanese_left'], row['id'])
            if row['frame'] == 144:   # 1 px clear on top and 2 px at the bottom, as the original
                alpha = [[image.getpixel((x, y))[3] for x in range(32)] for y in range(16)]
                self.assertFalse(any(alpha[0]) or any(alpha[14]) or any(alpha[15]))
                self.assertFalse(any(r[0] for r in alpha) or any(r[31] for r in alpha))
        # the card layer of the Save frames is untouched: only rows inside the lettering part change
        for image, original, row in prepared:
            if row['frame'] in subject.FRAMES['save'] and row['palette_id'] == 'base':
                self.assertEqual(image.getpixel((2, 1)), original.getpixel((2, 1)))

    def test_every_variant_uses_only_the_colours_of_its_japanese(self):
        """Per frame and palette variant: the colours inside the edit regions of the English are a
        subset of the colours the original shows in the same regions of the same variant."""
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as tmp:
            _, prepared, _ = subject.build(SOURCES, PACK, EXECUTABLE, Path(tmp) / 'out')
        for image, original, row in prepared:
            pts = {(x, y) for x0, y0, w, h in row['edit_regions'] for x in range(x0, x0 + w)
                   for y in range(y0, y0 + h) if 0 <= x < image.width and 0 <= y < image.height}
            english = {image.getpixel(p) for p in pts if image.getpixel(p)[3]}
            japanese = {original.getpixel(p) for p in pts if original.getpixel(p)[3]}
            self.assertLessEqual(english, japanese, row['id'])


if __name__ == '__main__':
    unittest.main()
