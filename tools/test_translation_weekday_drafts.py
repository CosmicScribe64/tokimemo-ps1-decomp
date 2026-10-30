"""Weekday lettering: port-made glyphs fit the bracket interior without touching it, in source style."""
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_mainrom_drafts as indexed
import translation_weekday_drafts as subject

SOURCES = subject.ROOT / '.asset-cache/translation-assets/original-mainrom-v1'
PACK = subject.ROOT / 'disc/files/CDROM/FUKUTEST/MAINROM.NBN'
INK, SHADOW = 0x6734, 0x14a5
# The measured bracket part in cell coordinates (frame 60 part 6 at (-2, -1)).
STEMS = [(-2, y) for y in range(1, 12)] + [(-1, y) for y in range(1, 12)] + \
        [(12, y) for y in range(1, 12)] + [(13, y) for y in range(1, 12)] + \
        [(-1, -1), (-2, 0), (0, 0), (-1, 12), (0, 13), (11, -1), (12, 0), (11, 12), (13, 12), (12, 13)]


def kanji_frame(ink, shadow_points=None):
    texels = [[0] * 16 for _ in range(16)]
    for x, y in ink:
        texels[y][x] = 9
    for x, y in (shadow_points if shadow_points is not None else {(x + 1, y + 1) for x, y in ink} - set(ink)):
        texels[y][x] = 10
    clut = [0] * 9 + [INK, SHADOW] + [0] * 5
    return indexed.Frame.from_layers(0x52, 16, 16, [{'index': 0, 'x': 0, 'y': 0, 'w': 16, 'h': 16, 'texels': texels}],
                                     {v: [clut] for v in indexed.VARIANTS})


class GlyphTests(unittest.TestCase):
    def test_glyphs_share_baseline_and_heights(self):
        for capitals in (subject.CAPITALS_WIDE, subject.CAPITALS_NARROW):
            table = subject.glyph_table(capitals)
            for ch in capitals:
                self.assertEqual(len(table[ch]), 8, ch)
            for ch in 'aenoru':
                self.assertEqual(len(table[ch]), 6, ch)
            for ch in 'dh':
                self.assertEqual(len(table[ch]), 8, ch)
            for ch, rows in table.items():
                self.assertEqual(max(y for _, y in subject.glyph_points(rows)), subject.BASELINE, ch)

    def test_wide_capitals_are_consistent(self):
        widths = {ch: len(rows[0]) for ch, rows in subject.CAPITALS_WIDE.items()}
        self.assertEqual({widths[c] for c in 'MTW'}, {5})
        self.assertEqual({widths[c] for c in 'FS'}, {4})

    def word(self, set_name, text):
        spec = subject.SETS[set_name]
        allowed = subject.allowed_cells(STEMS)
        return subject.compose(kanji_frame([(5, 5)]), text, subject.glyph_table(spec['capitals']), spec['gap'],
                               {'ink_index': 9, 'shadow_index': 10}, allowed, [1, 10])

    def test_two_letter_gaps_keep_a_clean_column_where_they_fit(self):
        gaps = {text: self.word('two-letter', text)[1]['gap'] for text in subject.SETS['two-letter']['words']}
        self.assertEqual(gaps, {'Mo': 1, 'Tu': 1, 'We': 1, 'Th': 1, 'Fr': 2, 'Sa': 2, 'Su': 2})
        for text in ('Fr', 'Sa', 'Su'):
            model, _ = self.word('two-letter', text)
            capital = len(subject.CAPITALS_WIDE[text[0]][0])
            clean = 1 + capital + 1  # box left + capital + its shadow column
            self.assertTrue(all(row[clean] == 0 for row in model.parts[0]['texels']), text)

    def test_two_letter_words_fill_the_clear_box_symmetrically(self):
        for text in subject.SETS['two-letter']['words']:
            self.assertEqual(self.word('two-letter', text)[1]['lettering_bbox'][::2], [1, 10], text)

    def test_three_letter_set_is_flagged_where_letters_abut(self):
        self.assertFalse(subject.SETS['three-letter']['recommended'])
        abut = [t for t in subject.SETS['three-letter']['words'] if not self.word('three-letter', t)[1]['letters_separated']]
        self.assertEqual(abut, ['Mon', 'Tue', 'Wed', 'Thu', 'Sat', 'Sun'])

    def test_malformed_glyph_is_refused(self):
        with self.assertRaises(ValueError):
            subject.glyph_table({'M': ['X.X', 'XX']})

    def test_shadow_is_under_ink_at_plus_one(self):
        pixels = subject.with_shadow([(0, 0), (1, 1)])
        self.assertEqual(pixels, {(0, 0): 'ink', (1, 1): 'ink', (2, 2): 'shadow'})

    def test_every_word_fits_the_clear_interior(self):
        allowed = subject.allowed_cells(STEMS)
        box = subject.measure(STEMS, range(2, 11))['clear']['columns']
        self.assertEqual(box, [1, 10])
        for name, spec in subject.SETS.items():
            table = subject.glyph_table(spec['capitals'])
            for text in spec['words']:
                model, plan = subject.compose(kanji_frame([(5, 5)]), text, table, spec['gap'],
                                              {'ink_index': 9, 'shadow_index': 10}, allowed, box)
                for y, row in enumerate(model.parts[0]['texels']):
                    for x, t in enumerate(row):
                        if t:
                            self.assertIn((x, y), allowed, '%s %s' % (name, text))
                self.assertEqual(plan['port_made_glyphs'], sorted(set(text)))


class GeometryTests(unittest.TestCase):
    def test_measure_reports_interior_and_clearance(self):
        report = subject.measure(STEMS, range(2, 11))
        self.assertEqual(report['interior'], {'columns': [0, 11], 'width': 12})
        self.assertEqual(report['clear'], {'columns': [1, 10], 'width': 10})

    def test_allowed_cells_exclude_diagonal_contact(self):
        allowed = subject.allowed_cells([(5, 5)])
        self.assertNotIn((6, 6), allowed)
        self.assertIn((7, 5), allowed)

    def test_compose_refuses_lettering_that_touches_a_bracket(self):
        allowed = subject.allowed_cells([(5, y) for y in range(16)])
        table = subject.glyph_table(subject.CAPITALS_WIDE)
        with self.assertRaises(ValueError):
            subject.compose(kanji_frame([(1, 1)]), 'Mo', table, (1,), {'ink_index': 9, 'shadow_index': 10},
                            allowed, [1, 10])

    def test_compose_refuses_text_wider_than_the_box(self):
        table = subject.glyph_table(subject.CAPITALS_WIDE)
        with self.assertRaises(ValueError):
            subject.compose(kanji_frame([(1, 1)]), 'Mo', table, (1,), {'ink_index': 9, 'shadow_index': 10},
                            subject.allowed_cells([]), [1, 6])


class StyleTests(unittest.TestCase):
    def frames(self, shadow=None):
        return {f: kanji_frame([(3, 3), (4, 3)], shadow) for f in subject.WEEKDAY_FRAMES}

    def test_source_style_is_derived(self):
        style = subject.source_style(self.frames())
        self.assertEqual((style['ink_index'], style['shadow_index'], style['kanji_baseline_row']), (9, 10, 3))

    def test_source_style_refuses_other_shadow_offsets(self):
        with self.assertRaises(ValueError):
            subject.source_style(self.frames(shadow=[(5, 3)]))


class SourceTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not (SOURCES.exists() and PACK.exists()):
            raise unittest.SkipTest('original MAINROM export or container missing')
        cls.tmp = tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache')
        cls.out = Path(cls.tmp.name) / 'build'
        cls.result = subject.build(SOURCES, PACK, cls.out)
        cls.manifest = json.loads((cls.out / 'manifest.json').read_text())

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def test_geometry_and_style_from_source(self):
        geometry = self.manifest['geometry']
        self.assertEqual(geometry['interior'], {'columns': [0, 11], 'width': 12})
        self.assertEqual(geometry['clear'], {'columns': [1, 10], 'width': 10})
        self.assertEqual(self.manifest['source_style']['ink_index'], 9)
        self.assertEqual(self.manifest['source_style']['shadow_index'], 10)

    def test_every_frame_and_variant_is_drafted_in_both_sets(self):
        self.assertEqual(self.manifest['faces']['three-letter']['abutting_words'],
                         ['Mon', 'Sat', 'Sun', 'Thu', 'Tue', 'Wed'])
        self.assertEqual(self.manifest['faces']['two-letter']['abutting_words'], [])
        for name in subject.SETS:
            rows = [r for r in self.manifest['assets'] if r['set'] == name]
            covered = {(r['frame'], v) for r in rows for v in r['palette_ids']}
            self.assertEqual(covered, {(f, v) for f in subject.WEEKDAY_FRAMES for v in indexed.VARIANTS})
            pack = json.loads((self.out / ('pack-%s.json' % name)).read_text())
            self.assertEqual(len(pack['assets']), 7 * len(subject.RUNTIME_USE))

    def test_outputs_keep_source_pixels_and_avoid_brackets(self):
        brackets = [tuple(p) for p in self.manifest['geometry']['bracket_pixels_in_cell']]
        allowed = subject.allowed_cells(brackets)
        for row in self.manifest['assets']:
            image = Image.open(self.out / row['output']).convert('RGBA')
            original = Image.open(SOURCES / ('mainrom.frame-%03d.%s.png' % (row['frame'], row['palette_id']))).convert('RGBA')
            indexed.check(original, image, row['edit_regions'], row['id'])
            opaque = {(x, y) for y in range(16) for x in range(16) if image.getpixel((x, y))[3]}
            self.assertLessEqual(opaque, allowed, row['id'])
            self.assertEqual(row['glyph_origin'], 'port-made')

    def test_build_is_deterministic(self):
        again = Path(self.tmp.name) / 'again'
        subject.build(SOURCES, PACK, again)
        for name in ('manifest.json', 'ledger.json', 'pack-two-letter.json', 'pack-three-letter.json'):
            self.assertEqual((again / name).read_bytes(), (self.out / name).read_bytes(), name)


if __name__ == '__main__':
    unittest.main()
