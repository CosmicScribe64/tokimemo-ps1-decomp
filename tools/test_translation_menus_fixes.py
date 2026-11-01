"""T-39411: menu and screen art fix pass (title, ALL_C, exam, option, name entry, FILESEL, sports)."""
import json
from pathlib import Path
import tempfile
import unittest

import numpy as np
from PIL import Image

import translation_assets as assets
import translation_inventory as inventory
import translation_menus_fixes as subject

CHECKPOINT = subject.ARTWORK / 'checkpoints' / subject.CHECKPOINT
ORIGINALS = subject.ROOT / '.asset-cache/translation-assets'


def solid(size, color):
    return Image.new('RGBA', size, color)


class TightFaceTests(unittest.TestCase):
    def test_every_button_word_fits_its_face_with_the_margin(self):
        face = subject.Tight()
        for left, width, words, _ in subject.GIRL_COLUMNS + subject.STAT_COLUMNS:
            for word in words:
                self.assertLessEqual(face.width(word), width - 2 * subject.FILESEL_MARGIN, word)

    def test_glyphs_are_unscaled_rectangular_bitmaps(self):
        face = subject.Tight()
        for character, (top, rows) in subject.TIGHT.items():
            self.assertEqual(len({len(row) for row in rows}), 1, character)
            self.assertLessEqual(top + len(rows), 9, character)
            self.assertTrue(face.glyph(character)['points'], character)

    def test_cast_spellings_follow_the_script(self):
        names = [text.split(' (')[0] for text in subject.OPTION_CAST.values()]
        self.assertIn('Kanazuki Mami', names)
        self.assertIn('Teppozuka Yoko', names)
        self.assertNotIn('Kingetsu Mami', names)
        self.assertEqual(subject.OPTION_FIXED[103], 'Kiyokawa Nozomi')
        self.assertEqual(subject.OPTION_FIXED[113], 'Tatebayashi Miharu')
        for text in list(subject.OPTION_CAST.values()) + list(subject.OPTION_FIXED.values()):
            self.assertFalse(any(part.endswith('.') for part in text.split()), text)   # no initials


class SyntheticSourceTests(unittest.TestCase):
    def test_title_frame_keeps_the_slab_and_the_palette(self):
        outline, blue, slab = (0, 0, 115, 255), (90, 165, 255, 255), (255, 165, 0, 255)
        original = solid((144, 16), subject.TRANSPARENT)
        for x in range(9, 117):
            for y in range(1, 10):
                original.putpixel((x, y), blue if 2 <= y <= 9 else outline)
        for y in range(10, 16):
            for x in range(12 - 2 * (y - 10), 118 - (y - 10)):
                original.putpixel((x, y), slab)
        result, facts = subject.title_frame(original, 'GAME START', True)
        self.assertLessEqual(subject.colors_of(result), subject.colors_of(original))
        for y in range(original.height):
            for x in range(original.width):
                if original.getpixel((x, y)) == slab:
                    self.assertEqual(result.getpixel((x, y)), slab)
        letters = np.array([[result.getpixel((x, y))[3] != 0 and result.getpixel((x, y)) != slab for x in range(result.width)]
                            for y in range(result.height)])
        box = subject.ink_box(letters)
        self.assertGreaterEqual(box[0], 9)
        self.assertLessEqual(box[2], 117)
        self.assertGreater(facts['fill_pixels'], 400)

    def test_title_frame_plain_state_fills_the_japanese_rows(self):
        grey, outline = (140, 140, 140, 255), (24, 24, 24, 255)
        original = solid((112, 16), subject.TRANSPARENT)
        for x in range(1, 109):
            for y in range(1, 15):
                original.putpixel((x, y), outline if y in (1, 14) else grey)
        result, _ = subject.title_frame(original, 'GAME START', False)
        rows = [y for y in range(16) if any(result.getpixel((x, y))[3] for x in range(112))]
        self.assertEqual((rows[0], rows[-1]), (1, 14))

    def test_allc_banner_uses_only_strip_tints_and_the_japanese_rows(self):
        strip = solid((subject.ALLC_WIDTH, subject.ALLC_ROWS), (255, 255, 255, 255))
        english, runs = subject.allc_english(strip)
        allowed = {c for ramp in subject.ALLC_RAMPS.values() for c in ramp}
        self.assertLessEqual(subject.colors_of(english), allowed)
        rows = [y for y in range(english.height) if any(english.getpixel((x, y)) != (255, 255, 255, 255) for x in range(english.width))]
        self.assertEqual((rows[0], rows[-1]), (subject.ALLC_TOP, subject.ALLC_TOP + subject.ALLC_HEIGHT - 1))
        cols = [x for x in range(english.width) if any(english.getpixel((x, y)) != (255, 255, 255, 255) for y in range(english.height))]
        self.assertGreaterEqual(cols[0], 12)
        self.assertLess(cols[-1], english.width - 12)
        self.assertEqual([r['family'] for r in runs], ['blue', 'red', 'purple'])

    def test_option_label_widens_and_keeps_the_bottom_rows(self):
        shadow, ink = subject.OPTION_SHADOW, (255, 200, 0, 255)
        original = solid((56, 16), subject.TRANSPARENT)
        for y in range(2, 11):
            for x in range(0, 40):
                original.putpixel((x, y), ink)
        original.putpixel((3, 14), (7, 7, 7, 255))
        face = subject.Menu9.__new__(subject.Menu9)
        face.glyphs = {c: ['1' * 3] * 9 for c in 'KiyokawNzm'}
        face.proof = {}
        result, facts = subject.option_label(original, 'Kiyokawa Nozomi', face)
        self.assertTrue(facts['widened'])
        self.assertGreater(result.width, 56)
        self.assertEqual(result.getpixel((3, 14)), (7, 7, 7, 255))
        self.assertEqual(result.height, 16)

    def test_name_label_keeps_only_the_outline_and_clears_the_frame_edge(self):
        frame, ink, shadow = (255, 206, 41, 255), (16, 231, 255, 255), (0, 24, 99, 255)
        original = solid((32, 16), subject.TRANSPARENT)
        for x in range(3, 30):
            original.putpixel((x, 0), frame); original.putpixel((x, 15), frame)
        for y in range(1, 15):
            original.putpixel((2, y), frame); original.putpixel((30, y), frame)
        original.putpixel((10, 5), ink); original.putpixel((11, 6), shadow)
        face = subject.Tight()
        for width in (32, 56):
            result, facts = subject.name_label(original, 'Hira' if width == 32 else 'Hiragana', face, width)
            self.assertEqual(result.size, (width, 16))
            self.assertGreaterEqual(min(facts['margins_to_frame'].values()), 0)
            colors = subject.colors_of(result)
            self.assertLessEqual(colors, {subject.TRANSPARENT, frame, ink, shadow})
            self.assertEqual(result.getpixel((2, 8)), frame)
            self.assertEqual(result.getpixel((width - 2, 8)), frame)

    def test_stp_audit_refuses_ambiguous_or_black_texels(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / 'cell.png'
            original = solid((4, 1), subject.TRANSPARENT)
            red = (200, 0, 0, 255)
            original.putpixel((1, 0), red); original.putpixel((2, 0), red)
            original.save(source)
            mask = Image.new('L', (4, 1), 0)
            mask.save(Path(directory) / 'cell.stp.png')
            facts = subject.stp_audit(source, original)
            self.assertEqual((facts['transparent_texels'], facts['opaque_stp_clear_texels']), (2, 2))
            black = original.copy(); black.putpixel((0, 0), (0, 0, 0, 255))
            with self.assertRaisesRegex(ValueError, 'opaque black'):
                subject.stp_audit(source, black)
            mask.putpixel((2, 0), 255); mask.save(Path(directory) / 'cell.stp.png')
            with self.assertRaisesRegex(ValueError, 'two STP values'):
                subject.stp_audit(source, original)

    def test_kerning_adds_texels_only_between_the_listed_pairs(self):
        face = subject.Tight()
        self.assertEqual(face.width('Hi', kern={'Hi': 1}), face.width('Hi') + 1)
        self.assertEqual(face.width('iH', kern={'Hi': 1}), face.width('iH'))
        self.assertEqual(max(x for x, _ in face.points('Hi', 0, 0, kern={'Hi': 1})), face.width('Hi') )

    def test_filesel_buttons_leave_the_bevel_alone(self):
        original = solid((60, 40), subject.TRANSPARENT)
        bevel = (198, 189, 189, 255)
        for y in range(0, 14):
            for x in range(1, 44):
                original.putpixel((x, y), subject.PAPER)
        for x in range(1, 44):
            original.putpixel((x, 14), bevel)
        for y in range(2, 12):
            original.putpixel((20, y), subject.INK)
        result, placed = subject.filesel_buttons(original, ((1, 43, ('Ayako',), 2),), subject.Tight())
        for x in range(1, 44):
            self.assertEqual(result.getpixel((x, 14)), bevel)
        self.assertGreaterEqual(min(placed[0]['margins']), subject.FILESEL_MARGIN)
        with self.assertRaises(ValueError):
            subject.filesel_buttons(original, ((1, 20, ('Humanities',), 2),), subject.Tight())


class CheckpointTests(unittest.TestCase):
    def setUp(self):
        self.ledger = json.loads((CHECKPOINT / 'ledger.json').read_text())

    def test_ledger_loads_in_the_inventory_and_supersedes_exactly(self):
        ledgers = list(inventory.LEDGERS)
        self.assertIn(CHECKPOINT / 'ledger.json', ledgers)    # registered (T-39411 integration)
        entries = inventory.load_ledgers(ledgers)
        active, superseded = inventory.split_superseded(entries, ledgers)
        mine = [e['id'] for e in active if 'T39410-menus-fixes-v1' in e['ledger']]
        self.assertEqual(len(mine), 32)    # 29 drafts (two of them the new ABC cell) and the three ALL_C crops
        self.assertEqual(inventory.candidate_key({'id': 'name-label-abc.lit'}), ('name_ent-abc', 'NAME_ENT', None, 'lit'))
        replaced = {(e['ledger'].split('/')[-2], e['id'], e['revision']) for e in superseded if e['superseded_by'].endswith('T39410-menus-fixes-v1/ledger.json')}
        self.assertEqual(len(replaced), len(self.ledger['supersedes']))
        for entry in active:
            if 'T39410-menus-fixes-v1' in entry['ledger']:
                inventory.candidate_key(entry)

    def test_output_hashes_and_manifest_bindings(self):
        manifest_bytes = (CHECKPOINT / 'menus-fixes-v1/manifest.json').read_bytes()
        for asset in self.ledger['assets']:
            self.assertFalse(asset['human_approved'])
            self.assertEqual(asset['manifest_sha256'], assets.digest(manifest_bytes))
            for path in asset.get('paths') or [asset['path']]:
                self.assertTrue((CHECKPOINT / path).is_file(), path)
            if 'path' in asset:
                self.assertEqual(assets.digest((CHECKPOINT / asset['path']).read_bytes()), asset['output_sha256'])
        fallback = json.loads((CHECKPOINT / 'fallback-ledger.json').read_text())
        for asset in fallback['assets']:
            self.assertEqual(assets.digest((CHECKPOINT / asset['path']).read_bytes()), asset['output_sha256'])
        self.assertNotIn('supersedes', fallback)

    def test_pngs_are_opaque_or_transparent_only_and_keep_the_palette_policy(self):
        for folder in ('menus-fixes-v1', 'fallback-v1'):
            for path in sorted((CHECKPOINT / folder).glob('*.png')):
                image = Image.open(path).convert('RGBA')
                self.assertTrue({p[3] for p in image.getdata()} <= {0, 255}, path.name)

    def test_required_widths_and_names_in_the_manifest(self):
        manifest = json.loads((CHECKPOINT / 'menus-fixes-v1/manifest.json').read_text())
        by_id = {}
        for item in manifest['items']:
            by_id.setdefault(item['id'], item)
        self.assertEqual(by_id['option-label-0103']['required_width'], 72)
        self.assertEqual(by_id['option-label-0113']['required_width'], 83)
        self.assertEqual(by_id['option-label-0063']['text'], 'Kanazuki Mami (Fujisaki)')
        self.assertEqual(by_id['option-label-0079']['text'], 'Teppozuka Yoko (Asahina)')
        self.assertEqual(by_id['exam-0001']['text'], 'points')
        self.assertEqual(by_id['kasetu-0']['text'], 'Scavenger Race')
        self.assertEqual({r['text'] for r in manifest['items'] if r['id'].startswith('name-label')},
                         set(subject.NAME_LABELS.values()) | {'ABC'})
        self.assertEqual(len((CHECKPOINT / 'REGISTER.md').read_text().splitlines()) > 40, True)
        self.assertTrue(manifest['indexed_assets'][0]['gates_passed'])

    def test_widened_option_sprites_keep_a_clear_right_column(self):
        for name in ('option-label-0103', 'option-label-0104', 'option-label-0113', 'option-label-0114'):
            image = Image.open(CHECKPOINT / 'menus-fixes-v1' / (name + '.png')).convert('RGBA')
            last = [image.getpixel((image.width - 1, y))[3] for y in range(subject.OPTION_TEXT_ROWS)]
            before = [image.getpixel((image.width - 2, y))[3] for y in range(subject.OPTION_TEXT_ROWS)]
            self.assertEqual(set(last), {0}, name)
            self.assertIn(255, before, name)

    def test_name_labels_keep_a_clear_column_between_stems(self):
        manifest = json.loads((CHECKPOINT / 'menus-fixes-v1/manifest.json').read_text())
        fallback = json.loads((CHECKPOINT / 'fallback-v1/manifest.json').read_text())
        texts = {(r['id'], r['variant']): r['text'] for r in manifest['items'] + fallback['items'] if r['id'].startswith('name-label')}
        self.assertEqual(texts[('name-label-0019', 'compact')], 'Sym')
        self.assertEqual({texts[('name-label-abc.unlit', 'wide')], texts[('name-label-abc.lit', 'compact')]}, {'ABC'})
        for folder, name in (('menus-fixes-v1', 'name-label-0015'), ('fallback-v1', 'name-label-0015'), ('menus-fixes-v1', 'name-label-abc.lit')):
            image = Image.open(CHECKPOINT / folder / (name + '.png')).convert('RGBA')
            frame = image.getpixel((3, 0))
            columns = [any(image.getpixel((x, y))[3] and image.getpixel((x, y)) != frame for y in range(1, 15)) for x in range(3, image.width - 3)]
            runs = [r for r in ''.join('#' if c else '.' for c in columns).split('.') if r]
            if name.endswith('abc.lit'):
                self.assertEqual(len(runs), 3, name)       # A | B | C, each with its shadow column
            else:
                self.assertEqual(len(runs[0]), 6, name)    # H (5 wide) and its shadow, then a clear column before i

    def test_every_output_passes_the_stp_audit(self):
        for folder in ('menus-fixes-v1', 'fallback-v1'):
            manifest = json.loads((CHECKPOINT / folder / 'manifest.json').read_text())
            for item in manifest['items']:
                stp = item['stp']
                self.assertEqual(stp['opaque_black_texels'], 0)
                expected = 'set on every texel' if item['id'] == 'kasetu-0' else 'clear on every texel'
                self.assertEqual(stp['source_stp'], expected, item['id'])
                self.assertEqual(stp['opaque_stp_set_texels'] > 0, item['id'] == 'kasetu-0', item['id'])
        allc = json.loads((CHECKPOINT / 'menus-fixes-v1/manifest.json').read_text())['indexed_assets'][0]['stp']
        self.assertFalse(any(allc['index_stp_bit'].values()))
        self.assertIn('## 9. Transparency and STP', (CHECKPOINT / 'REGISTER.md').read_text())

    def test_sports_title_is_outlined_at_the_japanese_rows(self):
        manifest = json.loads((CHECKPOINT / 'menus-fixes-v1/manifest.json').read_text())
        card = next(r for r in manifest['items'] if r['id'] == 'kasetu-0')
        x, y, w, h = card['title_ink_box']
        self.assertEqual((y, h), (41, 14))
        self.assertEqual(card['lettering'], 'SCAVENGER RACE')

    def test_system_tab_region_is_registered(self):
        self.assertEqual(subject.registered_system_tab(), subject.SYSTEM_TAB_RECT)

    def test_script_names_the_event_scavenger_race(self):
        for path in (subject.ROOT / 'translations/en/supplemental.jsonl', subject.ROOT / 'translations/en/supplemental/tai_tx.md'):
            text = path.read_text()
            self.assertNotIn('Scavenger Hunt', text, path.name)
            self.assertIn('Scavenger Race', text, path.name)

    def test_movie_pills_agree_with_the_cast_spellings(self):
        agreement = subject.pill_agreement()
        self.assertTrue(agreement['actress_names_equal'])
        self.assertTrue(agreement['character_names_in_pills'])


class RegenerationTests(unittest.TestCase):
    def test_build_reproduces_the_committed_outputs(self):
        sources = ORIGINALS
        if not (sources / 'original-title-v1').exists() or not (sources / 'allc').exists() or not (subject.ROOT / 'disc/files').exists():
            self.skipTest('restore original artwork and the disc first')
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            output = Path(directory) / 'build'
            subject.build(sources, output)
            for path in sorted(output.rglob('*.png')) + [output / 'ledger.json', output / 'REGISTER.md']:
                self.assertEqual(path.read_bytes(), (CHECKPOINT / path.relative_to(output)).read_bytes(), str(path))
            with self.assertRaises(ValueError):
                subject.build(sources, output)


if __name__ == '__main__':
    unittest.main()
