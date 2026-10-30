"""T-39012: BS macroblock splice and BS/ALL_C lettering drafts.

The synthetic tests need no game data. The original-data tests skip when the
disc files are absent and otherwise rebuild every draft from the source.
"""
import json
from pathlib import Path
import struct
import tempfile
import unittest
from unittest import mock

import numpy as np

import translation_bs_drafts as subject
from tools.assets import image_mdec as bs
from tools.assets import mdec_splice as splice

WIDTH, HEIGHT, MACROBLOCKS = 320, 88, 120  # a caller-proved decode shape (C_01 cohort)
PUBLISHED = subject.ROOT / 'translations/en/graphics/artwork/checkpoints/T39004-bs-v1'


def synthetic(levels_for, version=1, quant=2, trailing=b''):
    """A valid BS stream whose blocks carry levels_for(macroblock, block)."""
    bits, halfwords = [], 0
    for macroblock in range(MACROBLOCKS):
        for block in range(6):
            code, count = splice.encode_block(levels_for(macroblock, block))
            bits.append(code)
            halfwords += count
    body = splice._pack(''.join(bits) + splice.END_MARKER)
    declared = (halfwords * 2 + 127) // 128 * 128 // 4
    return struct.pack('<4H', declared, 0x3800, quant, version) + body + trailing


def flat(macroblock, block):
    levels = [0] * 64
    levels[0] = (macroblock * 7 + block * 13) % 200 - 100
    levels[1 + macroblock % 5] = 2
    return levels


def textured(macroblock, block):
    levels = [0] * 64
    for index in range(0, 64, 3):
        levels[index] = (index + macroblock + block) % 9 - 4
    return levels


class FakeFont:
    def glyph(self, character):
        return {'points': [(0, 0), (1, 1), (2, 2)], 'width': 3}


class SyntheticSpliceTests(unittest.TestCase):
    def test_identity_and_level_round_trip(self):
        data = synthetic(flat)
        rebuilt, facts = splice.rebuild(data)
        self.assertEqual(rebuilt, data)
        self.assertEqual(facts['declared_words']['new'], facts['declared_words']['original'])
        parsed = splice.parse(data)
        self.assertEqual(parsed['macroblocks'][7]['levels'][3], flat(7, 3))

    def test_replacement_changes_only_its_macroblock(self):
        data = synthetic(flat)
        source = splice.decode_rgb5(data, WIDTH, HEIGHT)
        rebuilt, facts = splice.rebuild(data, {13: [textured(13, block) for block in range(6)]})
        result = splice.decode_rgb5(rebuilt, WIDTH, HEIGHT)
        padded = source.shape[0]
        x, y = 13 // (padded // 16) * 16, 13 % (padded // 16) * 16
        mask = np.zeros(source.shape[:2], bool)
        mask[y:y + 16, x:x + 16] = True
        self.assertTrue((result[~mask] == source[~mask]).all())
        self.assertFalse((result[mask] == source[mask]).all())
        self.assertEqual(list(facts['edited_macroblock_bits']), [13])

    def test_encoder_holds_ring_and_reaches_target(self):
        data = synthetic(flat, quant=1)
        source = splice.decode_rgb5(data, WIDTH, HEIGHT)
        parsed = splice.parse(data)
        goal = source[0:16, 0:16].copy()
        goal[4:12, 4:12] = 28
        weights = np.full((16, 16), 50.0)
        weights[4:12, 4:12] = 1.0
        levels = splice.encode_macroblock(parsed['macroblocks'][0]['levels'], goal, weights, 1, 2.0)
        result = splice.macroblock_rgb5([splice.block_output(block, 1) for block in levels])
        self.assertLessEqual(int(np.abs(result - source[0:16, 0:16])[weights > 1].max()), 1)
        self.assertLess(float(np.abs(result - goal)[weights == 1].mean()), 2.0)

    def test_float_decode_agrees_on_plain_blocks(self):
        data = synthetic(flat)
        parsed = splice.parse(data)
        levels = parsed['macroblocks'][4]['levels']
        fixed = splice.macroblock_rgb5([splice.block_output(block, 2) for block in levels])
        self.assertLessEqual(int(np.abs(splice.float_macroblock_rgb5(levels, 2) - fixed).max()), 1)

    def test_trailing_bytes_kept_or_refused(self):
        kept = synthetic(flat, trailing=b'\x5a\xa5')
        rebuilt, facts = splice.rebuild(kept, {0: [[0] * 64 for _ in range(6)]})  # shorter stream
        self.assertEqual(rebuilt[-2:], b'\x5a\xa5')
        self.assertEqual(len(rebuilt), len(kept))
        with self.assertRaises(splice.SpliceError):
            splice.rebuild(kept, {0: [textured(0, block) for block in range(6)]})
        zero = synthetic(flat, trailing=bytes(4096))
        grown, facts = splice.rebuild(zero, {0: [textured(0, block) for block in range(6)]})
        self.assertEqual(len(grown), len(zero))
        self.assertIn('leading zero run', facts['trailing_bytes']['policy'])
        self.assertFalse(facts['file_extent_grew'])

    def test_growth_uses_zero_run_before_non_zero_trailing_bytes(self):
        roomy = synthetic(flat, trailing=bytes(4096) + b'\x5a\xa5')
        grown, facts = splice.rebuild(roomy, {0: [textured(0, block) for block in range(6)]})
        self.assertGreater(facts['stream_bytes']['new'], facts['stream_bytes']['original'])
        self.assertEqual(len(grown), len(roomy))
        self.assertEqual(grown[-2:], b'\x5a\xa5')
        self.assertEqual(facts['trailing_bytes']['leading_zero_bytes'], 4096)
        self.assertIn('leading zero run', facts['trailing_bytes']['policy'])
        tight = synthetic(flat, trailing=bytes(2) + b'\x5a\xa5')
        with self.assertRaises(splice.SpliceError):
            splice.rebuild(tight, {0: [textured(0, block) for block in range(6)]})

    def test_growth_past_file_end_is_flagged(self):
        data = synthetic(flat)
        grown, facts = splice.rebuild(data, {0: [textured(0, block) for block in range(6)]})
        self.assertGreater(len(grown), len(data))
        self.assertTrue(facts['file_extent_grew'])
        self.assertIn('past its original length', facts['trailing_bytes']['policy'])

    def test_rebuild_rejects_stream_the_project_reader_refuses(self):
        data = synthetic(flat)  # built before the patch so the input stream is valid
        with mock.patch.object(splice, 'encode_block', lambda levels: ('1' * 40, 3)):
            with self.assertRaises(bs.ImageFormatError):
                splice.rebuild(data, {0: [[0] * 64 for _ in range(6)]})

    def test_v3_refused(self):
        stream = ('00' + '10') * 2 + ('100' + '10') * 4
        bits = stream * MACROBLOCKS + '1111111111'
        data = struct.pack('<4H', (MACROBLOCKS * 12 * 2 + 127) // 128 * 128 // 4, 0x3800, 2, 3) + splice._pack(bits)
        bs._coefficients(data)  # valid for the reader
        with self.assertRaises(splice.SpliceError):
            splice.parse(data)

    def test_tampered_stream_rejected(self):
        data = bytearray(synthetic(flat))
        data[0] ^= 1  # declared extent
        with self.assertRaises(ValueError):
            splice.rebuild(bytes(data))
        with self.assertRaises(splice.SpliceError):
            splice.encode_block([511] + [0] * 63)

    def test_lettering_refuses_to_clip(self):
        image = np.zeros((20, 20, 3), int)
        with self.assertRaises(ValueError):
            subject.draw_lines(image, [0, 0, 5, 3], ['ABC'], 1, FakeFont(), [31, 31, 31])
        result, placed = subject.draw_lines(image, [0, 0, 20, 20], ['AB'], 1, FakeFont(), [31, 31, 31])
        self.assertEqual(int((result != 0).any(-1).sum()), 6)
        self.assertEqual(placed[0]['width'], 7)

    def test_erase_records_every_donor(self):
        image = np.full((16, 32, 3), 5, int)
        image[6:9, 10:13] = 25
        cleaned, donors, ink = subject.erase(image, [8, 4, 8, 8], 5)
        self.assertTrue((cleaned == 5).all())
        self.assertEqual(len(donors), 21)  # 3x3 strokes plus a one-pixel cross halo
        for x, y, sx, sy in donors:
            self.assertEqual(list(cleaned[y, x]), list(image[sy, sx]))
        self.assertEqual(ink['rgb555'], [25, 25, 25])

    def test_indexed_banner_keeps_outside_and_palette_indices(self):
        texture = np.full((256, 384), 9, np.uint8)
        texture[202:240, 0:320] = 6
        texture[210:220, 40:60] = 67
        lettered, facts = subject.letter_indexed(texture, [0, 202, 320, 38], 6, [('AB', 67)], FakeFont())
        outside = np.ones(texture.shape, bool)
        outside[202:240, 0:320] = False
        self.assertTrue((lettered[outside] == texture[outside]).all())
        self.assertEqual(set(np.unique(lettered[202:240, 0:320])), {6, 67})
        self.assertEqual(facts['erased_pixels'], 200)
        data = bytes(range(256)) * 392
        self.assertEqual(subject.texture_bytes(data, subject.page_texture(data)), data)


class GateTests(unittest.TestCase):
    @staticmethod
    def facts(**changes):
        facts = {'outside_edited_macroblocks_identical': True, 'ring': {'max_abs_rgb555': 1},
                 'float_idct_max_abs_rgb555': 2, 'fits_original_sectors': True, 'file_bytes': {'new': 20480}}
        facts.update(changes)
        return facts

    def test_gates_pass_at_the_limits(self):
        self.assertEqual(subject.gates(self.facts(), 10), [])

    def test_each_gate_fails_alone(self):
        cases = [(self.facts(outside_edited_macroblocks_identical=False), 'pixels changed outside edited macroblocks'),
                 (self.facts(ring={'max_abs_rgb555': 2}), 'ring error above tolerance'),
                 (self.facts(float_idct_max_abs_rgb555=3), 'float IDCT cross-check differs'),
                 (self.facts(file_bytes={'new': 20481}), 'stream exceeds sector budget or caller read'),
                 (self.facts(fits_original_sectors=False), 'stream exceeds sector budget or caller read')]
        for facts, failure in cases:
            self.assertEqual(subject.gates(facts, 10), [failure])


class IndexedGateTests(unittest.TestCase):
    STRIP = [0, 202, 320, 38]

    def setUp(self):
        self.texture = np.full((256, 384), 9, np.uint8)
        self.texture[202:240, 0:320] = 6
        self.texture[210:220, 40:60] = 67
        self.data = bytes(range(256)) * 392
        self.palette = [0x7FFF] * 256
        self.lettered, _ = subject.letter_indexed(self.texture, self.STRIP, 6, [('AB', 67)], FakeFont())

    def failures(self, lettered=None, english=None, palette=None):
        lettered = self.lettered if lettered is None else lettered
        english = subject.texture_bytes(self.data, lettered) if english is None else english
        return subject.indexed_failures(self.data, english, self.texture, lettered, self.STRIP,
                                        palette or self.palette, [('AB', 67)])

    def test_clean_draft_passes(self):
        self.assertEqual(self.failures(), [])

    def test_stray_index_fails(self):
        lettered = self.lettered.copy()
        lettered[230, 100] = 99
        self.assertEqual(self.failures(lettered), ['palette index absent from the source strip'])

    def test_change_outside_strip_fails(self):
        lettered = self.lettered.copy()
        lettered[10, 10] = 6
        self.assertEqual(self.failures(lettered), ['pixels changed outside the strip'])

    def test_transparent_ink_fails(self):
        palette = list(self.palette)
        palette[67] = 0x8000
        self.assertEqual(self.failures(palette=palette), ['ink index is transparent'])

    def test_extent_or_palette_change_fails(self):
        english = subject.texture_bytes(self.data, self.lettered)
        self.assertEqual(self.failures(english=english + b'\0'), ['byte extent or palette changed'])
        changed = bytearray(english)
        changed[0x18000] ^= 1
        self.assertEqual(self.failures(english=bytes(changed)), ['byte extent or palette changed'])

    def test_text_centres_on_original_lettering_rows(self):
        _, facts = subject.letter_indexed(self.texture, self.STRIP, 6, [('AB', 67)], FakeFont())
        self.assertEqual(facts['original_lettering_rows'], [210, 10])
        self.assertEqual(facts['text_origin'][1], 210 + (10 - 3) // 2)


class OriginalDataTests(unittest.TestCase):
    def setUp(self):
        if not (subject.ROOT / subject.BS_DIR / 'C_01_28.BS').exists() or not (subject.ROOT / 'disc/track1.bin').exists():
            self.skipTest('original disc files are not present')

    def test_identity_splice_for_census_files(self):
        for row in subject.census():
            self.assertTrue(row['identity_splice_exact'], row['file'])
            self.assertEqual(row['bs_version'], 1)

    def test_changed_source_bs_refused(self):
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            data = bytearray((subject.ROOT / subject.BS_DIR / 'C_01_28.BS').read_bytes())
            data[-1] ^= 1
            (Path(directory) / 'C_01_28.BS').write_bytes(bytes(data))
            with self.assertRaisesRegex(ValueError, 'source BS differs'):
                subject.build(Path(directory) / 'out', disc=directory)

    def test_drafts_rebuild_deterministically_and_match_publication(self):
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            first, second = Path(directory) / 'a', Path(directory) / 'b'
            subject.build(first)
            subject.build(second)
            manifest_bytes = (first / 'manifest.json').read_bytes()
            self.assertEqual(manifest_bytes, (second / 'manifest.json').read_bytes())
            manifest = json.loads(manifest_bytes)
            for row in manifest['assets']:
                self.assertTrue(row['gates_passed'], row['id'])
                self.assertFalse(row['human_approved'])
                self.assertEqual(row['style'], 'unreviewed')
                spliced = (first / row['local_files']['spliced_bs']).read_bytes()
                geometry = row['geometry']
                source = splice.decode_rgb5((subject.ROOT / subject.BS_DIR / (row['source_container'].split('/')[-1])).read_bytes(),
                                            geometry['width'], geometry['height'])
                english = splice.decode_rgb5(spliced, geometry['width'], geometry['height'])
                x, y, w, h = row['edit_regions'][0]
                outside = np.ones(source.shape[:2], bool)
                outside[y:y + h, x:x + w] = False
                self.assertTrue((english[outside] == source[outside]).all())
                self.assertLessEqual(len(spliced), geometry['read_sectors'] * 2048)
            for row in manifest['indexed_assets']:
                english = (first / 'ALL_C.en.NBN').read_bytes()
                self.assertEqual(subject.digest(english), row['english_container_sha256'])
            drawn = {character for plan in subject.PLANS for line in plan['lines'] for character in line}
            drawn |= {character for text, _ in subject.ALL_CLEAR['runs'] for character in text}
            self.assertEqual(drawn - {' '} - set(manifest['font_provenance']['glyphs']), set())
            if PUBLISHED.exists():
                self.assertEqual(subject.digest(manifest_bytes),
                                 subject.digest((PUBLISHED / 'bs-v1/manifest.json').read_bytes()))
                for path in sorted((PUBLISHED / 'bs-v1').glob('*.png')):
                    self.assertEqual(path.read_bytes(), (first / path.name).read_bytes(), path.name)
            # Mutation: a changed BS or ALL_C crop must not publish, and must leave nothing behind.
            target = Path(directory) / 'published'
            for crop in (first / manifest['assets'][0]['crop']['png'],
                         first / manifest['indexed_assets'][0]['edit_regions'][-1]['png']):
                original = crop.read_bytes()
                crop.write_bytes(original + b'\0')
                with self.assertRaises(ValueError):
                    subject.publish(first, target)
                self.assertFalse(target.exists())
                self.assertFalse(target.with_name('published.partial').exists())
                crop.write_bytes(original)
            subject.publish(first, target)
            self.assertTrue((target / 'ledger.json').exists())


if __name__ == '__main__':
    unittest.main()
