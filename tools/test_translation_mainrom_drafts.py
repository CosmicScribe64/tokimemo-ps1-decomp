"""T-39009: MAINROM indexed drafts keep exact source pixels and source CLUT indices."""
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_mainrom_drafts as subject

SOURCES = subject.ROOT / '.asset-cache/translation-assets/original-mainrom-v1'
PACK = subject.ROOT / 'disc/files/CDROM/FUKUTEST/MAINROM.NBN'
EXECUTABLE = subject.ROOT / 'disc/files/SLPM_86.053'
PUBLISHED = subject.ROOT / 'translations/en/graphics/artwork/checkpoints/T39004-pass3'
RED, GREEN, GREY = 0x001f, 0x03e0, 0x4210


def clut(one):
    return [0, one, GREY] + [GREY] * 13


def synthetic(parts=None, palettes=None):
    """All six variants, one 4x2 part by default; index 1 is red in base and green elsewhere, index 2 grey."""
    texels = [[1, 0, 2, 2], [0, 0, 2, 2]]
    parts = parts or [{'index': 0, 'x': 0, 'y': 0, 'w': 4, 'h': 2, 'texels': texels}]
    palettes = palettes or {v: [clut(RED if v == 'base' else GREEN)] * len(parts) for v in subject.VARIANTS}
    return subject.Frame.from_layers(999, 4, 2, parts, palettes)


def part(x, w, texels, sheet=None):
    row = {'index': x, 'x': x, 'y': 0, 'w': w, 'h': 2, 'texels': texels}
    if sheet is not None:
        row['sheet'] = {'offset': 0, 'u': sheet, 'v': 0, 'flip': 0}
    return row


def face():
    return subject.Face('test', {'I': {'points': [(0, 0), (0, 1)], 'width': 1}}, 1, {'new_glyphs': []})


class SyntheticTests(unittest.TestCase):
    def test_render_and_lift_are_pure(self):
        frame = synthetic()
        before = [row[:] for row in frame.parts[0]['texels']]
        english = {'base': frame.render('base'), 'hud-43': frame.render('hud-43')}
        english['base'].putpixel((1, 0), subject.color(RED))
        english['hud-43'].putpixel((1, 0), subject.color(GREEN))
        lifted = subject.lift(frame, english, [[0, 0, 2, 1]])
        self.assertEqual(lifted.texel(0, 1, 0), 1)
        self.assertEqual(frame.parts[0]['texels'], before)
        self.assertEqual(lifted.render('hud-43').getpixel((1, 0)), subject.color(GREEN))

    def test_lift_rejects_cross_variant_conflict(self):
        frame = synthetic()
        english = {'base': frame.render('base'), 'hud-43': frame.render('hud-43')}
        english['base'].putpixel((1, 0), subject.color(RED))
        english['hud-43'].putpixel((1, 0), subject.color(GREY))
        with self.assertRaises(ValueError):
            subject.lift(frame, english, [[0, 0, 4, 2]])

    def test_lift_rejects_change_outside_regions(self):
        frame = synthetic()
        english = {'base': frame.render('base')}
        english['base'].putpixel((3, 1), subject.color(RED))
        with self.assertRaises(ValueError):
            subject.lift(frame, english, [[0, 0, 2, 2]])

    def test_check_rejects_off_palette_and_outside_pixels(self):
        frame = synthetic()
        original = frame.render('base')
        off = original.copy()
        off.putpixel((0, 1), (1, 2, 3, 255))
        with self.assertRaises(ValueError):
            subject.check(original, off, [[0, 0, 4, 2]], 'off-palette')
        outside = original.copy()
        outside.putpixel((3, 1), subject.color(RED))
        with self.assertRaises(ValueError):
            subject.check(original, outside, [[0, 0, 2, 2]], 'outside')
        subject.check(original, outside, [[2, 0, 2, 2]], 'inside')

    def test_stamp_uses_index_and_refuses_clipping(self):
        frame = synthetic()
        subject.clear(frame, [0, 0, 4, 2])
        box = subject.stamp(frame, face(), 'I', [0, 0, 4, 2], 2, {0})
        self.assertEqual(box, [1, 0, 1, 2])
        self.assertEqual(frame.texel(0, 1, 1), 2)
        with self.assertRaises(ValueError):
            subject.stamp(synthetic(), face(), 'II', [0, 0, 2, 2], 2, {0}, outline=1)

    def test_stamp_refuses_fitted_text_where_no_part_covers(self):
        # The text fits the rect, but its column x=1 lies outside the only part (x 2..3).
        frame = synthetic([part(2, 2, [[0, 0], [0, 0]])])
        with self.assertRaisesRegex(ValueError, 'clipped'):
            subject.stamp(frame, face(), 'I', [0, 0, 4, 2], 2, {0})

    def test_stamp_refuses_text_wider_than_its_rect(self):
        # Both strokes of 'II' land on covered pixels, so only the fit check can refuse it.
        with self.assertRaisesRegex(ValueError, 'does not fit'):
            subject.stamp(synthetic(), face(), 'II', [1, 0, 1, 2], 2, {0})

    def test_lift_rejects_a_colour_no_index_has(self):
        frame = synthetic()
        english = {'base': frame.render('base')}
        english['base'].putpixel((1, 1), (8, 8, 8, 255))
        with self.assertRaisesRegex(ValueError, 'no source index'):
            subject.lift(frame, english, [[0, 0, 4, 2]])

    def test_stamp_refuses_hidden_lettering(self):
        # Part 0 is opaque grey over part 1; lettering written into part 1 cannot show.
        layers = [part(0, 4, [[2] * 4, [2] * 4]), part(0, 4, [[0] * 4, [0] * 4])]
        with self.assertRaisesRegex(ValueError, 'hidden'):
            subject.stamp(synthetic(layers), face(), 'I', [0, 0, 4, 2], 1, {1}, part=1)
        occluded = []
        subject.stamp(synthetic(layers), face(), 'I', [0, 0, 4, 2], 1, {1}, part=1, occluders=(0,), occluded=occluded)
        self.assertEqual(occluded, [(1, 0, 1), (1, 1, 1)])

    def test_put_refuses_a_different_clut(self):
        layers = [part(0, 2, [[0, 0], [0, 0]]), part(2, 2, [[1, 0], [0, 0]])]
        palettes = {v: [clut(RED), clut(GREEN)] for v in subject.VARIANTS}
        with self.assertRaisesRegex(ValueError, 'CLUT'):
            subject.put(synthetic(layers, palettes), 0, 0, 1, {1})
        frame = synthetic(layers, {v: [clut(RED), clut(RED)] for v in subject.VARIANTS})
        subject.put(frame, 0, 0, 1, {1})
        self.assertEqual(frame.texel(0, 0, 0), 1)

    def test_clear_index_must_be_transparent(self):
        palettes = {v: [[GREY] * 16] for v in subject.VARIANTS}
        with self.assertRaisesRegex(ValueError, 'clear index'):
            synthetic(palettes=palettes)

    def test_overlay_needs_one_shared_clut(self):
        layers = [part(0, 2, [[0, 0], [0, 0]]), part(2, 2, [[0, 0], [0, 0]])]
        frame = synthetic(layers, {v: [clut(RED), clut(GREEN)] for v in subject.VARIANTS})
        with self.assertRaisesRegex(ValueError, 'overlay CLUT'):
            frame.add_overlay((0, 1))

    def test_lift_rejects_transparency_disagreement(self):
        frame = synthetic()
        english = {'base': frame.render('base'), 'hud-43': frame.render('hud-43')}
        english['base'].putpixel((1, 0), (0, 0, 0, 0))
        english['hud-43'].putpixel((1, 0), subject.color(GREY))
        with self.assertRaisesRegex(ValueError, 'transparency'):
            subject.lift(frame, english, [[0, 0, 4, 2]])

    def test_lift_rejects_art_it_cannot_reproduce(self):
        # A wider English image matches every frame pixel but not the whole image.
        frame = synthetic()
        wide = Image.new('RGBA', (5, 2), subject.color(GREY))
        wide.paste(frame.render('base'), (0, 0))
        with self.assertRaisesRegex(ValueError, 'do not reproduce'):
            subject.lift(frame, {'base': wide}, [[0, 0, 4, 2]])

    def test_check_rejects_size_change(self):
        original = synthetic().render('base')
        padded = Image.new('RGBA', (5, 2), (0, 0, 0, 0))
        padded.paste(original, (0, 0))
        with self.assertRaisesRegex(ValueError, 'dimensions'):
            subject.check(original, padded, [[0, 0, 5, 2]], 'padded')

    def test_gate_rejects_a_render_that_differs_from_the_export(self):
        frame = synthetic()
        with tempfile.TemporaryDirectory() as directory:
            for v in subject.VARIANTS:
                frame.render(v).save(Path(directory) / ('mainrom.frame-999.%s.png' % v))
            self.assertEqual(len(subject.gate({999: frame}, Path(directory))), len(subject.VARIANTS))
            changed = frame.render('icon-53')
            changed.putpixel((0, 1), subject.color(RED))
            changed.save(Path(directory) / 'mainrom.frame-999.icon-53.png')
            with self.assertRaisesRegex(ValueError, 'decoder'):
                subject.gate({999: frame}, Path(directory))

    def test_render_group_rejects_variants_that_differ(self):
        frame = synthetic()
        self.assertEqual(subject.render_group(frame, ['hud-43', 'icon-51']).getpixel((0, 0)), subject.color(GREEN))
        with self.assertRaisesRegex(ValueError, 'render differently'):
            subject.render_group(frame, ['base', 'hud-43'])

    def test_sheet_scope_refuses_disagreeing_texels_and_reports_sharing(self):
        def drafted_pair(second):
            originals = {1: synthetic([part(0, 4, [[0] * 4, [0] * 4], sheet=0)]),
                         2: synthetic([part(0, 4, [[0] * 4, [0] * 4], sheet=0)])}
            edits = {f: frame.copy() for f, frame in originals.items()}
            edits[1].set(0, 1, 0, 2)
            edits[2].set(0, 1, 0, second)
            return {f: (frame, {}) for f, frame in edits.items()}, originals
        users = {1: {(0, 0, 1)}, 2: {(0, 0, 1)}, 3: {(0, 0, 1)}}
        drafted, originals = drafted_pair(1)
        with self.assertRaisesRegex(ValueError, 'sheet texels different values'):
            subject.sheet_scope(drafted, originals, users)
        drafted, originals = drafted_pair(2)
        scopes = subject.sheet_scope(drafted, originals, users)
        self.assertEqual(scopes[1]['same_edit_frames'], {'2': 1})
        self.assertEqual(scopes[1]['affected_frames'], {'3': 1})
        self.assertEqual(scopes[1]['scope'], 'frame-composite-only')
        self.assertEqual(subject.sheet_scope(drafted, originals, {1: {(0, 0, 1)}, 2: {(0, 0, 1)}})[1]['scope'], 'sheet-patch')
        overlay = synthetic([part(0, 4, [[0] * 4, [0] * 4], sheet=0)])
        overlay.add_overlay((0,))
        edited = overlay.copy()
        edited.set(1, 0, 0, 2)
        self.assertEqual(subject.sheet_scope({4: (edited, {})}, {4: overlay}, {})[4]['overlay_texels'], 1)

    def test_missing_glyph_fails(self):
        with self.assertRaises(ValueError):
            face().points('Ix')

    def test_outputs_must_be_new(self):
        with self.assertRaises(ValueError):
            subject.new_output(subject.ROOT / 'tools')


class OriginalDataTests(unittest.TestCase):
    def setUp(self):
        if not (SOURCES.exists() and PACK.exists() and EXECUTABLE.exists()):
            self.skipTest('restore disc files and the original MAINROM export first')

    def test_rebuild_matches_published_checkpoint(self):
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            output = Path(directory) / 'out'
            result = subject.build(SOURCES, PACK, EXECUTABLE, output)
            self.assertEqual(result['gated_renders'], len(subject.GATED_FRAMES) * len(subject.VARIANTS))
            manifest = json.loads((output / 'manifest.json').read_text())
            for row in manifest['assets']:
                self.assertFalse(row['human_approved'])
                self.assertEqual(row['style'], 'unreviewed')
                source = Image.open(SOURCES / ('mainrom.frame-%03d.%s.png' % (row['frame'], row['palette_id']))).convert('RGBA')
                draft = Image.open(output / row['output']).convert('RGBA')
                subject.check(source, draft, row['edit_regions'], row['id'])
                if row['frame'] in subject.SAVE_FRAMES:
                    # The card is frame 56's art and stays untouched on top of the lettering.
                    self.assertEqual(row['written_part'], 1)
                    self.assertEqual(row['sheet_scope']['scope'], 'sheet-patch')
            if not PUBLISHED.exists():
                self.skipTest('checkpoint not published yet')
            revision = PUBLISHED / subject.REVISION
            names = sorted(p.name for p in output.iterdir())
            self.assertEqual(names, sorted(p.name for p in revision.iterdir()))
            for name in names:
                self.assertEqual((output / name).read_bytes(), (revision / name).read_bytes(), name)
            ledger = json.loads((PUBLISHED / 'ledger.json').read_text())
            self.assertTrue(all(a['human_approved'] is False and a['status'] == 'draft' for a in ledger['assets']))


if __name__ == '__main__':
    unittest.main()
