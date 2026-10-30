"""T-39010: battle Miss drafts from original compact capitals."""
import copy
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

import translation_battle_drafts as subject

DISC = subject.ROOT / 'disc/files'
CHECKPOINT = subject.ROOT / 'translations/en/graphics/artwork/checkpoints/T39004-battle-miss-v1'


class Font:
    """Synthetic two-letter face: 'A' is a 2x5 bar pair, 'B' a 1x5 bar."""
    height = 5
    glyphs = {'A': {'points': [(x, y) for y in range(5) for x in (0, 1)], 'width': 2},
              'B': {'points': [(0, y) for y in range(5)], 'width': 1}}

    def glyph(self, character):
        return self.glyphs[character]

    def width(self, text):
        return sum(self.glyphs[c]['width'] for c in text) + len(text) - 1


def synthetic_region(ink=0x10, shadow=0x12, with_shadow=True):
    region = [0] * 128
    for x in range(1, 5):
        region[6 * 16 + x] = ink
        if with_shadow:
            region[7 * 16 + x + 1] = shadow
    region[1 * 16 + 1] = ink
    if with_shadow:
        region[2 * 16 + 2] = shadow
    return region


def sheet_with(rows, x, y, ink=1):
    pixels = [0] * (128 * 128)
    for dy, row in enumerate(rows):
        for dx, cell in enumerate(row):
            if cell == '#':
                pixels[(y + dy) * 128 + x + dx] = ink
    return pixels


class ComposeTests(unittest.TestCase):
    def test_compose_uses_masks_exact_shadow_and_source_baseline(self):
        result = subject.compose_miss(synthetic_region(), Font(), 0x10, 0x12, text='AB')
        ink = subject.index_points(result, 0x10)
        shadow = subject.index_points(result, 0x12)
        self.assertEqual(set(result), {0, 0x10, 0x12})
        self.assertEqual(shadow, subject.diagonal(ink) - ink)
        self.assertEqual(max(y for _, y in ink), 6)
        self.assertEqual(min(y for _, y in ink), 2)
        # width 4 + shadow column, centred in 16: starts at x=5
        self.assertEqual(min(x for x, _ in ink), 5)
        self.assertEqual(len(ink), 15)

    def test_extra_index_is_rejected(self):
        region = synthetic_region()
        region[0] = 0x33
        with self.assertRaisesRegex(ValueError, 'indices differ'):
            subject.compose_miss(region, Font(), 0x10, 0x12, text='AB')

    def test_source_without_diagonal_shadow_is_rejected(self):
        region = synthetic_region()
        region[7 * 16 + 5] = 0
        with self.assertRaisesRegex(ValueError, 'lacks the one-pixel diagonal'):
            subject.compose_miss(region, Font(), 0x10, 0x12, text='AB')

    def test_text_that_leaves_the_region_is_rejected(self):
        with self.assertRaisesRegex(ValueError, 'exceeds'):
            subject.compose_miss(synthetic_region(), Font(), 0x10, 0x12, text='AAAAAA')

    def test_glyph_extraction_is_exact(self):
        rows = subject.COMPACT_GLYPHS['S'][2]
        glyph = subject.extract_glyph(sheet_with(rows, 10, 10), 10, 10, rows)
        self.assertEqual(glyph['rect'], [10, 10, 3, 5])
        self.assertEqual(len(glyph['points']), 11)

    def test_altered_mask_or_dirty_gutter_is_rejected(self):
        rows = subject.COMPACT_GLYPHS['M'][2]
        altered = sheet_with(rows, 10, 10)
        altered[11 * 128 + 11] = 0
        with self.assertRaisesRegex(ValueError, 'mask or gutter differs'):
            subject.extract_glyph(altered, 10, 10, rows)
        dirty = sheet_with(rows, 10, 10)
        dirty[12 * 128 + 13] = 1
        with self.assertRaisesRegex(ValueError, 'mask or gutter differs'):
            subject.extract_glyph(dirty, 10, 10, rows)


@unittest.skipUnless((DISC / 'CDROM/BATTLE/VS_HABU.BIN').exists() and (DISC / subject.MAINROM_PATH).exists(),
                     'original battle and MAINROM files required')
class OriginalDataTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.temp = tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache')
        cls.output = Path(cls.temp.name) / 'miss'
        subject.build_miss(DISC / 'CDROM/BATTLE', DISC / subject.MAINROM_PATH, cls.output)
        cls.manifest_bytes = (cls.output / 'manifest.json').read_bytes()
        cls.manifest = json.loads(cls.manifest_bytes)

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    def test_ten_items_bound_to_frames_64_and_65(self):
        assets = self.manifest['assets']
        self.assertEqual(len(assets), 10)
        self.assertEqual(sorted(a['item_id'] for a in assets), sorted(
            'battle-%s-miss-%d' % (e, m) for e in ('habu', 'kuma', 'robo', 'sika', 'ufo') for m in (0, 1)))
        for record in assets:
            self.assertEqual(record['frame'], 64 + int(record['item_id'][-1]))
            self.assertEqual(record['part'], 0)
            self.assertEqual((record['style'], record['human_approved'], record['in_game_verified']),
                             ('unreviewed', False, False))
            self.assertFalse(record['rescaled'] or record['derived_from_executable_font'])

    def test_outputs_keep_source_colors_and_exact_shadow(self):
        for record in self.manifest['assets']:
            original = Image.open(self.output / record['original']).convert('RGBA')
            draft = Image.open(self.output / record['output']).convert('RGBA')
            self.assertEqual(draft.size, (16, 8))
            self.assertEqual(set(original.getdata()), set(draft.getdata()))
            shadow, ink = sorted(set(original.getdata()) - {(0, 0, 0, 0)}, key=lambda c: sum(c[:3]))
            points = {(x, y) for y in range(8) for x in range(16) if draft.getpixel((x, y)) == ink}
            dark = {(x, y) for y in range(8) for x in range(16) if draft.getpixel((x, y)) == shadow}
            self.assertEqual(dark, subject.diagonal(points) - points)

    def test_same_bytes_in_all_five_containers(self):
        for frame in (64, 65):
            rows = [a for a in self.manifest['assets'] if a['frame'] == frame]
            self.assertEqual(len({a['output_indices_sha256'] for a in rows}), 1)
            self.assertEqual({a['source_indices_sha256'] for a in rows}, {subject.MISS_FRAMES[frame][4]})

    def test_provenance_and_references_are_pinned(self):
        provenance = self.manifest['font_provenance']
        self.assertEqual(provenance['sheet_sha256'], subject.COMPACT_SHEET_SHA)
        self.assertEqual(provenance['referenced_by']['frame'], 118)
        self.assertEqual({c: g['rect'] for c, g in provenance['glyphs'].items()},
                         {'I': [37, 21, 1, 5], 'M': [17, 21, 3, 5], 'S': [78, 21, 3, 5]})
        self.assertEqual(self.manifest['hp_reference']['hand_placed_shadow_pixels'], 2)
        for container in self.manifest['containers']:
            self.assertNotEqual(container['patched_sha256'], container['source_sha256'])
            self.assertLessEqual(container['changed_bytes'], 256)

    def test_review_is_separate_and_not_approval(self):
        review_bytes = (self.output / 'review.agent.json').read_bytes()
        self.assertEqual(subject.digest(review_bytes), self.manifest['agent_review']['sha256'])
        review = json.loads(review_bytes)
        self.assertFalse(review['human_approval'])
        self.assertEqual(len(review['assets']), 10)

    def test_rebuild_is_deterministic_and_matches_publication(self):
        again = Path(self.temp.name) / 'again'
        subject.build_miss(DISC / 'CDROM/BATTLE', DISC / subject.MAINROM_PATH, again)
        self.assertEqual((again / 'manifest.json').read_bytes(), self.manifest_bytes)
        if CHECKPOINT.exists():
            revision = CHECKPOINT / subject.MISS_REVISION
            self.assertEqual((revision / 'manifest.json').read_bytes(), self.manifest_bytes)
            ledger = json.loads((CHECKPOINT / 'ledger.json').read_text())
            self.assertEqual(len(ledger['assets']), 10)
            for row in ledger['assets']:
                self.assertFalse(row['human_approved'])
                self.assertEqual(subject.digest((CHECKPOINT / row['path']).read_bytes()), row['output_sha256'])
            self.assertFalse(any(p.name.endswith('.original.png') for p in revision.iterdir()))

    def test_publication_refuses_changed_output(self):
        built = Path(self.temp.name) / 'tampered'
        subject.shutil.copytree(self.output, built)
        record = self.manifest['assets'][0]
        image = Image.open(built / record['output']).convert('RGBA')
        image.putpixel((0, 0), image.getpixel((2, 6)))
        image.save(built / record['output'])
        target = CHECKPOINT.parent / ('test-refused-' + Path(self.temp.name).name)
        with self.assertRaisesRegex(ValueError, 'changed'):
            subject.publish_miss(built, target)
        self.assertFalse(target.exists())

    def built_copy(self, label):
        built = Path(self.temp.name) / label
        subject.shutil.copytree(self.output, built)
        return built

    def assert_publication_refused(self, built, pattern, destination=None):
        target = destination or CHECKPOINT.parent / ('test-refused-' + built.name + '-' + Path(self.temp.name).name)
        existed = target.exists()
        try:
            with self.assertRaisesRegex(ValueError, pattern):
                subject.publish_miss(built, target)
            self.assertEqual(target.exists(), existed)
        finally:
            if not existed and target.exists():
                subject.shutil.rmtree(target)

    def test_publication_refuses_review_claiming_approval(self):
        built = self.built_copy('approval')
        review = json.loads((built / 'review.agent.json').read_bytes())
        review['human_approval'] = True
        review_bytes = subject.encoded(review) + b'\n'
        (built / 'review.agent.json').write_bytes(review_bytes)
        manifest = dict(self.manifest, agent_review={'file': 'review.agent.json', 'sha256': subject.digest(review_bytes)})
        (built / 'manifest.json').write_bytes(subject.encoded(manifest) + b'\n')
        self.assert_publication_refused(built, 'claims approval')

    def test_publication_refuses_original_as_output(self):
        built = self.built_copy('underived')
        record = self.manifest['assets'][0]
        subject.shutil.copyfile(built / record['original'], built / record['output'])
        assets = [dict(row, output_sha256=row['source_png_sha256']) if row['id'] == record['id'] else row
                  for row in self.manifest['assets']]
        (built / 'manifest.json').write_bytes(subject.encoded(dict(self.manifest, assets=assets)) + b'\n')
        self.assert_publication_refused(built, 'not a derivative')

    def test_publication_refuses_existing_destination(self):
        with tempfile.TemporaryDirectory(dir=CHECKPOINT.parent) as existing:
            self.assert_publication_refused(self.output, 'must be new', Path(existing))
            self.assertEqual(list(Path(existing).iterdir()), [])

    def test_mainrom_identity_is_pinned(self):
        data = bytearray((DISC / subject.MAINROM_PATH).read_bytes())
        data[0] ^= 1  # outside the compact sheet, so only the whole-file identity can catch it
        altered = Path(self.temp.name) / 'MAINROM.NBN'
        altered.write_bytes(data)
        with self.assertRaisesRegex(ValueError, 'MAINROM identity differs'):
            subject.CompactLatin(altered)

    def lettering_inputs(self, name='VS_HABU.BIN'):
        data = (DISC / 'CDROM/BATTLE' / name).read_bytes()
        return data, subject._parse(data, name), subject.CompactLatin(DISC / subject.MAINROM_PATH)

    def test_altered_source_region_is_rejected(self):
        data, parsed, font = self.lettering_inputs()
        u, v = subject.MISS_FRAMES[64][:2]
        altered = bytearray(data)
        altered[parsed['page_offset'] + v * 128 + u] ^= 1
        with self.assertRaisesRegex(ValueError, 'source region identity differs'):
            subject.letter_container(bytes(altered), parsed, font)

    def test_moved_miss_frame_is_rejected(self):
        data, parsed, font = self.lettering_inputs()
        moved = copy.deepcopy(parsed)
        moved['frames'][65]['parts'][0]['u'] += 1
        with self.assertRaisesRegex(ValueError, 'miss frame geometry differs'):
            subject.letter_container(data, moved, font)

    def test_patch_outside_the_miss_rectangles_is_rejected(self):
        name = 'VS_HABU.BIN'
        data, parsed, font = self.lettering_inputs(name)
        patched, letters = subject.letter_container(data, parsed, font)
        rows = [(l['frame'], l['u'], l['v'], l['lettered'], subject.rgba(l['lettered'], parsed['palette']))
                for l in letters]
        self.assertEqual(subject.verify_patch(name, data, parsed, patched, rows)['changed_bytes'], 146)
        stray = bytearray(patched)
        stray[parsed['page_offset']] ^= 1  # page pixel u0 v0, away from both rectangles
        with self.assertRaisesRegex(ValueError, 'outside the Miss rectangles'):
            subject.verify_patch(name, data, parsed, bytes(stray), rows)


if __name__ == '__main__':
    unittest.main()
