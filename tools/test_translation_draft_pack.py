"""Bounded agent reviews select exact artifacts without becoming human approval."""
import json
from pathlib import Path
import tempfile
import unittest

import translation_draft_pack as subject
from translation_assets import ROOT


class ReviewedDraftTests(unittest.TestCase):
    def test_review_selects_nine_and_rejects_changed_identity(self):
        source=ROOT/'.asset-cache/translation-assets/name-native-font-v1'
        if not source.exists(): self.skipTest('generate source-font NAME references first')
        review=ROOT/'translations/en/graphics/reviews/T39004-name-native-font-v1.agent.json'
        with tempfile.TemporaryDirectory(dir=ROOT/'.asset-cache') as temporary:
            folder=Path(temporary)
            result=subject.build(source,folder/'pack','name-entry-actor-clut-10',review)
            self.assertEqual(result['assets'],9)
            manifest=json.loads((folder/'pack/manifest.json').read_text())
            self.assertTrue(manifest['development_drafts'])
            self.assertFalse(manifest['agent_review']['human_approval'])
            self.assertTrue(all(row['style']=='unreviewed' for row in manifest['assets']))
            changed=json.loads(review.read_text())
            changed['assets'][0]['output_sha256']='0'*64
            bad=folder/'changed-review.json';bad.write_text(json.dumps(changed))
            with self.assertRaisesRegex(ValueError,'identity differs'):
                subject.build(source,folder/'rejected','name-entry-actor-clut-10',bad)
            self.assertFalse((folder/'rejected').exists())


CONTAINER = '1' * 64
INK = (200, 200, 200, 255)
BACK = (10, 60, 10, 255)


def synthetic_hud(folder, weekday_sha='5' * 64):
    """A native HUD manifest with frame 60 (three parts) and single-part frame 82.

    Part 0 (top): 4x4 at (0, 0); part 1: 4x4 at (3, 0), overlapping part 0 in
    column 3; part 2: 8x2 at (0, 4), opaque. Returns (manifest path, original composite).
    """
    from PIL import Image
    assets = folder / 'hud/assets'
    assets.mkdir(parents=True)
    images = [Image.new('RGBA', (4, 4), (0, 0, 0, 0)), Image.new('RGBA', (4, 4), (0, 0, 0, 0)), Image.new('RGBA', (8, 2), BACK)]
    images[0].putpixel((1, 1), INK)
    images[1].putpixel((2, 2), INK)
    parts = []
    for index, (image, x, y) in enumerate(zip(images, (0, 3, 0), (0, 0, 4))):
        path = assets / ('p%02d.png' % index)
        image.save(path)
        parts.append({'part_index': index, 'x': 100 + x, 'y': 50 + y, 'width': image.width, 'height': image.height,
                      'png': {'path': 'assets/' + path.name, 'sha256': subject.digest(path.read_bytes())}})
    manifest = {'schema': subject.HUD_SCHEMA, 'provenance': {'inputs': {'mainrom': {'sha256': CONTAINER}}},
                'frames': {'60:base': {'bounds': [100, 50, 8, 6], 'parts': parts},
                           '82:base': {'bounds': [0, 0, 16, 16], 'parts': [
                               {'part_index': 0, 'x': 0, 'y': 0, 'width': 16, 'height': 16,
                                'png': {'path': 'assets/none.png', 'sha256': weekday_sha}}]}}}
    path = folder / 'hud/manifest.json'
    path.write_text(json.dumps(manifest))
    original = Image.new('RGBA', (8, 6), (0, 0, 0, 0))
    original.paste(images[2], (0, 4))
    original.putpixel((1, 1), INK)
    original.putpixel((5, 2), INK)
    return path, original


def synthetic_checkpoint(folder, hud, draft, edited, frame_shas, extra_assets=()):
    """A HUD checkpoint as tools/translation_date_panel_checkpoint.py writes it.

    Frame 60 is committed part-wise: `draft` (a whole-frame composite) is cut
    against the HUD at `hud` and only the `edited` parts are saved, each its
    own asset bound to `frame_shas`. A weekday asset for single-part frame 82
    follows.
    """
    from PIL import Image
    checkpoint = folder / 'checkpoint'
    (checkpoint / 'rev').mkdir(parents=True)
    record = json.loads(hud.read_text())['frames']['60:base']
    images = subject.split_frame(draft, subject.hud_frame_parts(hud.parent, record), record['bounds'][2:], edited)
    rows = []
    for index in edited:
        path = checkpoint / ('rev/f60-p%02d.png' % index)
        images[index].save(path)
        rows.append({'id': 'mainrom-alt-0060-captions-part%02d.base' % index, 'frame': 60, 'palette_id': 'base',
                     'palette_ids': ['base', 'hud-43'], 'path': path.relative_to(checkpoint).as_posix(),
                     'output_sha256': subject.digest(path.read_bytes()), 'dimensions': list(images[index].size),
                     'source_png_sha256': '6' * 64, 'source_container_sha256': CONTAINER, 'status': 'draft',
                     'human_approved': False, 'port_made': True, 'runtime_applicable': True, 'part_index': index,
                     'edited_parts': edited, 'source_part_sha256': frame_shas[index],
                     'frame_parts_sha256': {'base': frame_shas}})
    weekday = checkpoint / 'rev/mon.png'
    Image.new('RGBA', (16, 16), INK).save(weekday)
    rows += [{'id': 'mainrom-weekday-two-letter-mon.base', 'frame': 82, 'palette_id': 'base', 'palette_ids': ['base', 'icon-52'],
             'path': 'rev/mon.png', 'output_sha256': subject.digest(weekday.read_bytes()), 'dimensions': [16, 16],
             'source_png_sha256': '5' * 64, 'source_container_sha256': CONTAINER, 'status': 'draft',
             'human_approved': False, 'port_made': True, 'runtime_applicable': True}, *extra_assets]
    (checkpoint / 'ledger.json').write_text(json.dumps({'schema': subject.CHECKPOINT_SCHEMA, 'runtime_hud': True, 'assets': rows}))
    return checkpoint


class HudCheckpointTests(unittest.TestCase):
    def setUp(self):
        try:
            import PIL  # noqa: F401
        except ImportError:
            self.skipTest('needs Pillow (tokimemo-content image)')
        self.temp = tempfile.TemporaryDirectory()
        self.folder = Path(self.temp.name)
        self.hud, self.original = synthetic_hud(self.folder)
        record = json.loads(self.hud.read_text())['frames']['60:base']
        self.parts = subject.hud_frame_parts(self.hud.parent, record)
        self.shas = [p[4] for p in self.parts]

    def tearDown(self):
        self.temp.cleanup()

    def test_original_composite_puts_part_zero_on_top(self):
        self.assertEqual(subject.compose_parts(self.parts, [8, 6]).tobytes(), self.original.tobytes())

    def test_only_edited_parts_are_cut_and_recompose_to_the_draft(self):
        draft = self.original.copy()
        draft.putpixel((1, 1), (0, 0, 0, 0))  # part 0 ink cleared
        draft.putpixel((2, 3), INK)           # new ink in part 0
        images = subject.split_frame(draft, self.parts, [8, 6], [0])
        self.assertEqual(sorted(images), [0])
        self.assertEqual(images[0].getpixel((1, 1)), (0, 0, 0, 0))
        self.assertEqual(images[0].getpixel((2, 3)), INK)
        self.assertEqual(subject.compose_parts(self.parts, [8, 6], images).tobytes(), draft.tobytes())
        self.assertEqual(self.parts[0][3].getpixel((1, 1)), INK, 'the original part is not modified')

    def test_changes_outside_the_listed_parts_are_refused(self):
        draft = self.original.copy()
        draft.putpixel((5, 2), (0, 0, 0, 0))  # part 1 changes but only part 0 is listed
        with self.assertRaisesRegex(ValueError, 'exactly one edited part'):
            subject.split_frame(draft, self.parts, [8, 6], [0])
        overlap = self.original.copy()
        overlap.putpixel((3, 1), INK)  # column 3 belongs to parts 0 and 1
        with self.assertRaisesRegex(ValueError, 'exactly one edited part'):
            subject.split_frame(overlap, self.parts, [8, 6], [0, 1])
        with self.assertRaisesRegex(ValueError, 'without a changed pixel'):
            subject.split_frame(self.original.copy(), self.parts, [8, 6], [0])
        translucent = self.original.copy()
        translucent.putpixel((0, 0), (1, 1, 1, 128))
        with self.assertRaisesRegex(ValueError, 'alpha is not binary'):
            subject.split_frame(translucent, self.parts, [8, 6], [0])

    def test_checkpoint_rows_bind_parts_and_single_frames(self):
        draft = self.original.copy()
        draft.putpixel((2, 3), INK)
        checkpoint = synthetic_checkpoint(self.folder, self.hud, draft, [0], self.shas)
        rows, files, report = subject.hud_checkpoint_rows(checkpoint, self.hud)
        kinds = sorted((row['kind'], row['frame'], row['palette']) for row in rows)
        self.assertEqual(kinds, [('frame', 82, 'base'), ('frame_part', 60, 'base')])
        part = next(row for row in rows if row['kind'] == 'frame_part')
        self.assertEqual((part['part_index'], part['frame_parts_sha256'], part['source_png_sha256']), (0, self.shas, self.shas[0]))
        self.assertTrue(all(row['style'] == 'unreviewed' and row['human_approved'] is False for row in rows))
        self.assertEqual(report['skipped_not_drawn'], ['mainrom-alt-0060-captions-part00.base:hud-43', 'mainrom-weekday-two-letter-mon.base:icon-52'])
        self.assertEqual(files, [checkpoint / 'rev/f60-p00.png', checkpoint / 'rev/mon.png'])
        self.assertEqual(part['output_sha256'], subject.digest(files[0].read_bytes()))
        for row in rows:
            subject.check_v2_row(row)

    def test_only_edited_parts_are_committed(self):
        draft = self.original.copy()
        draft.putpixel((2, 3), INK)
        checkpoint = synthetic_checkpoint(self.folder, self.hud, draft, [0], self.shas)
        self.assertEqual(sorted(p.name for p in (checkpoint / 'rev').iterdir()), ['f60-p00.png', 'mon.png'])

    def _broken_part(self, change):
        draft = self.original.copy()
        draft.putpixel((2, 3), INK)
        checkpoint = synthetic_checkpoint(self.folder, self.hud, draft, [0], self.shas)
        ledger = json.loads((checkpoint / 'ledger.json').read_text())
        change(ledger['assets'])
        (checkpoint / 'ledger.json').write_text(json.dumps(ledger))
        return checkpoint

    def test_multi_part_assets_must_name_one_installed_part(self):
        cases = {'no part index': lambda a: a[0].pop('part_index'),
                 'part index outside': lambda a: a[0].update(part_index=3),
                 'part source differs': lambda a: a[0].update(source_part_sha256='0' * 64),
                 'another part size': lambda a: a[0].update(part_index=2, source_part_sha256=self.shas[2])}
        for label, change in cases.items():
            with self.subTest(label), tempfile.TemporaryDirectory() as directory:
                self.folder = Path(directory)
                self.hud, self.original = synthetic_hud(self.folder)
                with self.assertRaisesRegex(ValueError, 'does not name one installed part'):
                    subject.hud_checkpoint_rows(self._broken_part(change), self.hud)

    def test_a_part_given_twice_is_refused(self):
        def twice(assets):
            assets.insert(1, {**assets[0], 'id': assets[0]['id'] + 'b'})
        with self.assertRaisesRegex(ValueError, 'frame part given twice'):
            subject.hud_checkpoint_rows(self._broken_part(twice), self.hud)

    def test_checkpoint_bound_to_another_hud_fails(self):
        draft = self.original.copy()
        draft.putpixel((2, 3), INK)
        stale = list(self.shas)
        stale[1] = '0' * 64
        with self.assertRaisesRegex(ValueError, 'frame parts differ'):
            subject.hud_checkpoint_rows(synthetic_checkpoint(self.folder, self.hud, draft, [0], stale), self.hud)
        other_hud, _ = synthetic_hud(self.folder / 'other', weekday_sha='7' * 64)
        with self.assertRaisesRegex(ValueError, 'differs from the ledger source'):
            subject.hud_checkpoint_rows(synthetic_checkpoint(self.folder / 'other', other_hud, draft, [0], self.shas), other_hud)


if __name__=='__main__': unittest.main()
