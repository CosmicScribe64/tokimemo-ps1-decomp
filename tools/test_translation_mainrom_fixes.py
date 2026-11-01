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
        self.assertEqual({len(b) for b in subject.BOLD.values()}, {subject.BOLD_HEIGHT})
        self.assertEqual({len(b) for b in subject.DIGITS.values()}, {subject.DIGIT_HEIGHT})

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
        cover = subject.cover_faces()
        for spec in subject.COVERS.values():
            for text, face, *_ in spec['lines']:
                for ch in text.replace(' ', ''):
                    self.assertIn(ch, cover[face].glyphs)


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
            self.assertEqual(down - up + 1, subject.BOLD_HEIGHT)

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

    def test_issue_word_fits_beside_the_longest_numerals(self):
        digits = subject.digit_face()
        longest = max(subject.extent(digits, t)[1] + 1 for t in subject.ISSUE_DIGITS.values())
        face = subject.fit_face(faces(), [subject.ISSUE_WORD], 56 - longest - 1)
        self.assertLessEqual(longest + 1 + base.line_width(face, subject.ISSUE_WORD, None), 56)

    def test_digits_are_even_widths_except_one(self):
        widths = {ch: len(rows[0]) for ch, rows in subject.DIGITS.items() if ch != '.'}
        self.assertEqual({w for ch, w in widths.items() if ch != '1'}, {5})
        self.assertEqual(widths['1'], 3)


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
        # the card layer of the Save frames is untouched: only rows inside the lettering part change
        for image, original, row in prepared:
            if row['frame'] in subject.FRAMES['save'] and row['palette_id'] == 'base':
                self.assertEqual(image.getpixel((2, 1)), original.getpixel((2, 1)))


if __name__ == '__main__':
    unittest.main()
