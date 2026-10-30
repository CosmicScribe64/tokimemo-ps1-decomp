"""T-39011: index-space FILESEL drafts keep source texels outside declared edits."""
import json
from pathlib import Path
import tempfile
import unittest
from unittest import mock

import numpy as np
from PIL import Image

import translation_filesel_indexed as subject
import translation_inventory

SOURCES = subject.ROOT / '.asset-cache/translation-assets/original-filesel-v1'
NBN, EXE = subject.NBN, subject.EXE
PUBLISHED = subject.ROOT / 'translations/en/graphics/artwork/checkpoints/T39004-filesel-v3'
REGIONS = subject.ROOT / 'translations/en/graphics/filesel_page_regions.json'
DECISIONS = subject.ROOT / 'translations/en/graphics/decisions/filesel.json'
EXPECTED_EDIT_REGIONS = {
    'filesel-frame-0.base': [[1, 1, 40, 16]], 'filesel-frame-1.base': [[4, 1, 40, 16]],
    'filesel-frame-2.base': [[1, 12, 40, 16]], 'filesel-frame-3.base': [[4, 12, 40, 16]],
    'filesel-page-information-0': [[120, 45, 8, 11], [121, 61, 7, 11]],
    'filesel-page-information-1': [[0, 45, 36, 11], [0, 61, 39, 11], [89, 28, 11, 11], [120, 28, 8, 11]],
    'filesel-page-information-2': [[0, 28, 3, 11], [24, 28, 8, 11], [5, 95, 11, 11]],
}


def ramp(height=16, width=40, slope=80, thresholds=(300, 900, 1500, 2100, 2700, 3300), indices=(216, 215, 214, 213, 212, 211, 210)):
    ys, xs = np.mgrid[0:height, 0:width]
    rank = ((100 * xs + slope * ys)[..., None] >= np.array(thresholds)).sum(axis=-1)
    return np.array(indices)[rank]


class SyntheticTests(unittest.TestCase):
    def test_fit_ramp_recovers_a_half_plane_ramp(self):
        values = ramp()
        model = subject.fit_ramp(values, np.ones(values.shape, bool))
        self.assertEqual(model['fit_accuracy'], 1.0)
        self.assertEqual(model['order'], [216, 215, 214, 213, 212, 211, 210])
        ys, xs = np.nonzero(np.ones(values.shape, bool))
        _, predicted = subject.predict(model, ys, xs)
        self.assertEqual(predicted, values[ys, xs].tolist())

    def test_compose_keeps_known_texels_and_fills_strokes(self):
        values = ramp()
        truth = values.copy()
        values[5:9, 10:20] = subject.CORE
        values[4, 9:21] = subject.OUTLINE
        known = ~np.isin(values, (subject.CORE, subject.OUTLINE))
        model = subject.fit_ramp(values, known)
        empty = np.zeros(values.shape, bool)
        result, provenance = subject.compose_button(values, model, {'core': empty, 'outline': empty})
        self.assertTrue((result[known] == values[known]).all())
        self.assertTrue((result == truth).all())
        self.assertEqual(sum(row.count('r') for row in provenance), int((~known).sum()))

    def test_outline_rule_is_eight_neighbour(self):
        mask = np.zeros((5, 5), bool)
        mask[2, 2] = True
        self.assertEqual(int((subject.dil8(mask) & ~mask).sum()), 8)
        values = np.zeros((5, 5), int)
        values[2, 2] = subject.CORE
        values[1:4, 1:4][(subject.dil8(mask) & ~mask)[1:4, 1:4]] = subject.OUTLINE
        self.assertEqual(subject.outline_deviation(values), {'missing_from_source': [], 'extra_in_source': []})
        values[1, 1] = 0
        self.assertEqual(subject.outline_deviation(values)['missing_from_source'], [[1, 1]])

    def test_patch_outside_rect_or_clut_fails(self):
        source = bytes(0x24000)
        with self.assertRaisesRegex(ValueError, 'size differs'):
            subject.verify_patch(source, source + b'\0', [])
        rect = [0x1a000, 1, 1, 40, 16]
        inside = bytearray(source)
        inside[0x1a000 + 128 + 1] = 5
        self.assertEqual(subject.verify_patch(source, bytes(inside), [rect]), 1)
        bevel = bytearray(inside)
        bevel[0x1a000 + 17 * 128 + 1] = 5  # bottom bevel row below the interior
        with self.assertRaises(ValueError):
            subject.verify_patch(source, bytes(bevel), [rect])
        clut = bytearray(source)
        clut[0x22002] = 1
        with self.assertRaises(ValueError):
            subject.verify_patch(source, bytes(clut), [[0x22000, 0, 0, 4, 1]])

    def test_page_rects_stop_at_seams_and_row_120(self):
        self.assertEqual(subject.split_montage([120, 45, 20, 11]),
                         [{'page': 0, 'rect': [120, 45, 8, 11]}, {'page': 1, 'rect': [0, 45, 12, 11]}])
        for page, rect in ((0, [120, 45, 9, 11]), (1, [0, 111, 8, 10]), (2, [0, -1, 4, 4])):
            with self.assertRaises(ValueError):
                subject.check_page_rect(page, rect)
        with self.assertRaises(ValueError):
            subject.split_montage([10, 115, 4, 6])

    def test_layout_rejects_words_that_need_scaling_or_extra_rows(self):
        class WideFont:
            def glyph(self, character):
                return {'width': 11, 'points': [(dx, dy) for dx in range(11) for dy in range(2, 10)]}
        with self.assertRaisesRegex(ValueError, 'does not fit'):
            subject.layout_buttons(WideFont(), ('abcd',))
        with self.assertRaisesRegex(ValueError, 'exceed the interior'):
            subject.layout_buttons(WideFont(), ('a',), height=9)

    def test_gate_rejects_each_limit(self):
        good = {'index_exact': subject.GATE['min_exact'], 'max_band_error': subject.GATE['max_band_error']}
        subject.check_gate(good, [79, 79])
        for gate, slopes in ((dict(good, index_exact=good['index_exact'] - 0.01), [79]),
                             (dict(good, max_band_error=good['max_band_error'] + 1), [79]),
                             (good, [79, 79 + subject.GATE['max_slope_spread'] + 1])):
            with self.assertRaises(ValueError):
                subject.check_gate(gate, slopes)

    def test_export_identity_check(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for name in ('sources', 'original', 'patched'):
                (root / name).mkdir()
                for file in ('a.png', 'b.png'):
                    (root / name / file).write_bytes(b'source ' + file.encode())
            (root / 'patched/a.png').write_bytes(b'edited')
            subject.check_exports(root / 'sources', root / 'original', root / 'patched', ['a.png', 'b.png'], {'a.png'})
            with self.assertRaisesRegex(ValueError, 'export identity'):
                subject.check_exports(root / 'sources', root / 'original', root / 'patched', ['a.png', 'b.png'], {'a.png', 'b.png'})
            (root / 'patched/b.png').write_bytes(b'stray')
            with self.assertRaisesRegex(ValueError, 'export identity'):
                subject.check_exports(root / 'sources', root / 'original', root / 'patched', ['a.png', 'b.png'], {'a.png'})
            (root / 'original/a.png').write_bytes(b'drift')
            with self.assertRaisesRegex(ValueError, 'cached original'):
                subject.check_exports(root / 'sources', root / 'original', root / 'patched', ['a.png'], {'a.png'})

    def test_build_rejects_a_different_container(self):
        with tempfile.TemporaryDirectory() as directory:
            nbn = Path(directory) / 'FILESEL.NBN'
            nbn.write_bytes(bytes(16))
            with self.assertRaisesRegex(ValueError, 'FILESEL source differs'):
                subject.build(Path(directory), Path(directory) / 'out', nbn=nbn)

    def test_publish_needs_a_new_destination_and_unchanged_outputs(self):
        with tempfile.TemporaryDirectory() as directory:
            built = Path(directory) / 'built'
            built.mkdir()
            (built / 'a.png').write_bytes(b'draft')
            row = {'candidate': True, 'id': 'x', 'output': 'a.png', 'output_sha256': subject.digest(b'other')}
            (built / 'manifest.json').write_text(json.dumps({'assets': [row]}))
            with self.assertRaisesRegex(ValueError, 'must be new'):
                subject.publish(built, built)
            with self.assertRaisesRegex(ValueError, 'built output changed'):
                subject.publish(built, Path(directory) / 'published')

    def test_outputs_must_be_new(self):
        with self.assertRaises(ValueError):
            subject.new_output(subject.ROOT / 'tools')


class OriginalDataTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        if not (SOURCES.exists() and NBN.exists() and EXE.exists()):
            raise unittest.SkipTest('restore FILESEL.NBN, the executable and original-filesel-v1 first')
        cls.directory = tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache')
        cls.output = Path(cls.directory.name) / 'out'
        cls.verified = []
        verify = subject.verify_patch
        with mock.patch.object(subject, 'verify_patch', lambda *args: cls.verified.append(verify(*args)) or cls.verified[-1]):
            _, cls.regions = subject.build(SOURCES, cls.output)
        cls.manifest = json.loads((cls.output / 'manifest.json').read_text())

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def test_source_outside_edit_regions_and_palette_subset(self):
        self.assertEqual(len(self.manifest['assets']), 7)
        for row in self.manifest['assets']:
            source = np.asarray(Image.open(SOURCES / row['source_png']).convert('RGBA'))
            result = np.asarray(Image.open(self.output / row['output']).convert('RGBA'))
            self.assertEqual(source.shape, result.shape)
            outside = np.ones(source.shape[:2], bool)
            for x, y, width, height in row['edit_regions']:
                outside[y:y + height, x:x + width] = False
            self.assertTrue((source[outside] == result[outside]).all(), row['id'])
            self.assertLessEqual({tuple(p) for p in result.reshape(-1, 4)}, {tuple(p) for p in source.reshape(-1, 4)})
            self.assertEqual(row['style'], 'unreviewed')
            self.assertFalse(row['human_approved'])
            self.assertFalse(row['runtime_integrated'])

    def test_outline_rule_reproduces_frames_1_and_3(self):
        rows = {row['frame']: row for row in self.manifest['assets'] if row['frame'] is not None}
        for frame in (1, 3):
            self.assertEqual(rows[frame]['source_outline_deviation'], {'missing_from_source': [], 'extra_in_source': []})

    def test_button_glyphs_are_unscaled_font_masks(self):
        font = subject.NativeLatin(EXE)
        placed = subject.layout_buttons(font, subject.BUTTON_WORDS)
        top = min(int(np.nonzero(subject.word_mask(font, w, 1).any(axis=1))[0].min()) for w in subject.BUTTON_WORDS)
        for row, place in zip([r for r in self.manifest['assets'] if r['frame'] is not None], placed):
            x, y, width, height = row['edit_regions'][0]
            image = np.asarray(Image.open(self.output / row['output']).convert('RGB'))[y:y + height, x:x + width]
            core = (image == (255, 255, 0)).all(axis=-1)
            self.assertTrue((core == place['core']).all(), row['id'])
            cx, cy, cw, ch = place['core_rect']
            glyphs = subject.word_mask(font, place['word'], place['gap'])
            self.assertTrue((place['core'][cy:cy + ch, cx:cx + cw] == glyphs[top:top + ch]).all())

    def test_leave_out_gate_and_round_trip(self):
        gate = self.manifest['leave_out_gate']
        self.assertGreaterEqual(gate['index_exact'], subject.GATE['min_exact'])
        self.assertLessEqual(gate['max_band_error'], subject.GATE['max_band_error'])
        self.assertLessEqual(max(gate['slopes_hundredths']) - min(gate['slopes_hundredths']), subject.GATE['max_slope_spread'])
        trip = self.manifest['round_trip']
        self.assertEqual(trip['file_size'], subject.packs.FILESEL_SIZE)
        self.assertEqual(trip['probe'], 'supported')
        self.assertFalse(trip['patched_container_written'])

    def montage(self):
        data = NBN.read_bytes()
        return data, np.concatenate([np.frombuffer(data, np.uint8, subject.PAGE_SIZE, subject.PAGE_OFFSET + i * subject.PAGE_SIZE)
                                     .reshape(128, 128) for i in range(3)], axis=1)

    def edit(self, montage):
        font = subject.NativeLatin(EXE)
        return subject.edit_pages(montage, font, font.data, font.metadata)

    def test_round_trip_changes_exactly_the_declared_regions(self):
        self.assertEqual(self.manifest['round_trip']['changed_bytes'], 1388)
        self.assertEqual(self.verified, [1388])
        self.assertEqual({row['id']: row['edit_regions'] for row in self.manifest['assets']}, EXPECTED_EDIT_REGIONS)

    def test_page_guards_reject_bad_montages(self):
        _, montage = self.montage()
        edited, regions, _ = self.edit(montage)
        name = next(r for r in regions if r['id'] == 'name')
        _, cx, cy = subject.MATCHED[0]['cells'][0]
        flipped = montage.copy()
        flipped[cy + 6, cx + 6] = subject.INK if flipped[cy + 6, cx + 6] != subject.INK else name['paper_index']
        with self.assertRaisesRegex(ValueError, 'not an exact native-font match'):
            self.edit(flipped)
        cells = np.zeros(montage.shape, bool)
        for _, x, y in subject.MATCHED[0]['cells']:
            cells[y:y + 12, x:x + 12] = True
        nx, ny, nw, nh = name['montage_rect']
        drawn = np.zeros(montage.shape, bool)
        drawn[ny:ny + nh, nx:nx + nw] = (edited != montage)[ny:ny + nh, nx:nx + nw]
        ys, xs = np.nonzero(drawn & ~cells)
        blocked = montage.copy()
        blocked[ys[0], xs[0]] = subject.INK + 1
        with self.assertRaisesRegex(ValueError, 'non-paper'):
            self.edit(blocked)
        ex, ey, ew, eh = name['english_rect']
        with mock.patch.object(subject, 'DYNAMIC_STRING_LEFT', ex + ew - 1):
            with self.assertRaisesRegex(ValueError, 'dynamic string'):
                self.edit(montage)
        with mock.patch.object(subject, 'DIGIT_SPANS', [[ex, ey, 1, 12]]):
            with self.assertRaisesRegex(ValueError, 'code-drawn digit'):
                self.edit(montage)
        cover = {'id': 'probe', 'japanese': '', 'search': [ex, ey, ew, eh], 'ink': [subject.INK], 'matched_cells': [], 'reason': 'test'}
        with mock.patch.object(subject, 'UNMATCHED', subject.UNMATCHED + [cover]):
            with self.assertRaisesRegex(ValueError, 'unmatched region changed'):
                self.edit(montage)

    def test_page_guards_reject_paper_and_partial_match_changes(self):
        _, montage = self.montage()
        (_, x0, y0), (_, x1, y1) = subject.MATCHED[0]['cells']
        name_glyph, _ = subject.glyph_bitmap(EXE.read_bytes(), subject.NativeLatin(EXE).metadata, subject.MATCHED[0]['cells'][0][0])
        paper = int(montage[y0:y0 + 12, x0:x0 + 12][~name_glyph][0])
        other = next(v for v in range(256) if v not in (paper, subject.INK))
        mixed = montage.copy()
        py, px = map(int, np.argwhere(~name_glyph)[0])
        mixed[y0 + py, x0 + px] = other
        with self.assertRaisesRegex(ValueError, 'single paper'):
            self.edit(mixed)
        second = montage.copy()
        cell = second[y1:y1 + 12, x1:x1 + 12]
        cell[cell == paper] = other
        with self.assertRaisesRegex(ValueError, 'disagree on paper'):
            self.edit(second)
        _, cx, cy = subject.UNMATCHED[0]['matched_cells'][0]
        moved = montage.copy()
        moved[cy + 6, cx + 6] = subject.INK if moved[cy + 6, cx + 6] != subject.INK else paper
        with self.assertRaisesRegex(ValueError, 'partial glyph match moved'):
            self.edit(moved)
        blank = {'id': 'blank', 'japanese': '', 'search': [0, 0, 1, 1], 'ink': [-1], 'matched_cells': [], 'reason': 'test'}
        with mock.patch.object(subject, 'UNMATCHED', [blank]):
            with self.assertRaisesRegex(ValueError, 'has no ink'):
                self.edit(montage)
        with mock.patch.object(subject, 'font_reference', return_value={'status': 'missing'}):
            with self.assertRaisesRegex(ValueError, 'missing from native font'):
                self.edit(montage)

    def test_build_rejects_cache_probe_and_export_drift(self):
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            root = Path(directory)
            stale = root / 'stale'
            stale.mkdir()
            (stale / 'manifest.json').write_text(json.dumps({'metadata': {'source_sha256': '0' * 64}}))
            with self.assertRaisesRegex(ValueError, 'cached FILESEL export differs'):
                subject.build(stale, root / 'a')
            with mock.patch.object(subject.packs, 'probe_filesel_ui', return_value={'status': 'unsupported'}):
                with self.assertRaisesRegex(ValueError, 'no longer probes'):
                    subject.build(SOURCES, root / 'b')
            export = subject.packs.export_filesel_ui

            def drop_last(data, *args):
                result = export(data, *args)
                return dict(result, files=result['files'][:-1]) if 'patched' in str(args[1]) else result
            with mock.patch.object(subject.packs, 'export_filesel_ui', drop_last):
                with self.assertRaisesRegex(ValueError, 'file list changed'):
                    subject.build(SOURCES, root / 'c')

    def test_button_guards_reject_changed_anatomy_and_shared_texels(self):
        data = NBN.read_bytes()
        frames = subject.packs._parse(data)
        parts, _ = subject.button_interiors(data, frames)
        part, edit, _ = parts[0]
        broken = bytearray(data)
        broken[edit[0] + (edit[2] + 5) * 128 + edit[1] + 5] = 1  # not a gradient or lettering index
        with self.assertRaisesRegex(ValueError, 'anatomy'):
            subject.button_interiors(bytes(broken), frames)
        sampler = {'frame_index': 99, 'parts': [dict(part, u=edit[1] + 39, v=edit[2] + 15, width=1, height=1)]}
        with self.assertRaisesRegex(ValueError, 'samples the edited'):
            subject.button_interiors(data, frames + [sampler])

    def test_ledger_ids_load_in_the_inventory(self):
        ledger = PUBLISHED / 'ledger.json'
        entries = translation_inventory.load_ledgers([ledger])
        frames = [e for e in entries if e['frame'] is not None]
        self.assertEqual([translation_inventory.candidate_key(e)[:3] for e in frames],
                         [('filesel-%04d' % f, 'FILESEL', f) for f in range(4)])
        pages = [e for e in entries if e['frame'] is None]
        self.assertEqual([e['id'] for e in pages], ['filesel-page-information-2'])
        self.assertEqual(translation_inventory.candidate_key(pages[0]),
                         ('filesel-page-information-2', 'FILESEL', None, None))
        self.assertIn(ledger, translation_inventory.LEDGERS)
        proposal = json.loads(DECISIONS.read_text())
        overrides = json.loads(translation_inventory.DECISIONS.read_text())['atom_overrides']
        status = {item['id']: item['status'] for item in translation_inventory.build()['artwork_items']}
        for row in proposal['decisions']:
            self.assertEqual(overrides[row['item_id']]['status'], row['status'], row['item_id'])
            self.assertEqual(status[row['item_id']], row['status'], row['item_id'])
        for item_id in ('filesel-0000', 'filesel-0001', 'filesel-0002', 'filesel-0003', 'filesel-page-information-2'):
            self.assertEqual(status[item_id], 'candidate-only', item_id)

    def test_partial_pages_are_not_published(self):
        ledger = json.loads((PUBLISHED / 'ledger.json').read_text())
        self.assertEqual(ledger['candidate_assets'], 5)
        self.assertEqual(ledger['unpublished_partial_builds'], ['filesel-page-information-0', 'filesel-page-information-1'])
        published = {p.name for p in (PUBLISHED / subject.REVISION).iterdir()}
        self.assertNotIn('source-page-information-0.png', published)
        self.assertNotIn('source-page-information-1.png', published)

    def test_region_map_is_committed_and_below_row_120(self):
        self.assertEqual(REGIONS.read_bytes(), subject.encoded(self.regions) + b'\n')
        self.assertFalse(self.regions['human_confirmed'])
        for region in self.regions['regions']:
            for piece in region['page_rects']:
                subject.check_page_rect(piece['page'], piece['rect'])

    def test_published_checkpoint_rebuilds_byte_for_byte(self):
        published = PUBLISHED / subject.REVISION
        names = sorted(p.name for p in published.iterdir())
        self.assertEqual(names, sorted(['manifest.json'] + [row['output'] for row in self.manifest['assets'] if row['candidate']]))
        for name in names:
            self.assertEqual((self.output / name).read_bytes(), (published / name).read_bytes(), name)
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            copy = Path(directory) / 'checkpoint'
            subject.publish(self.output, copy)
            for name in ('ledger.json', 'review.agent.json'):
                self.assertEqual((copy / name).read_bytes(), (PUBLISHED / name).read_bytes(), name)
        review = json.loads((PUBLISHED / 'review.agent.json').read_text())
        self.assertFalse(review['human_approved'])


if __name__ == '__main__':
    unittest.main()
