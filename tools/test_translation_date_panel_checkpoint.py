"""T-39004: the published date panel checkpoint matches the user's style decision.

The committed-checkpoint checks need no game data. The reproduction check runs
only when the local draft directories are named in the environment
(TOKIMEMO_DATE_PANEL_FRAME60, TOKIMEMO_DATE_PANEL_WEEKDAYS,
TOKIMEMO_DATE_PANEL_HUD, TOKIMEMO_DATE_PANEL_ORIGINAL) and needs Pillow
(tokimemo-content image).
"""
import json
import os
from pathlib import Path
import tempfile
import unittest

import translation_date_panel_checkpoint as subject
from translation_assets import digest


def png_size(path):
    data = path.read_bytes()
    return int.from_bytes(data[16:20], 'big'), int.from_bytes(data[20:24], 'big')

CHECKPOINT = subject.CHECKPOINTS / 'T39004-date-panel-v1'


class PublishedCheckpointTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.ledger = json.loads((CHECKPOINT / 'ledger.json').read_text())
        cls.manifest_bytes = (CHECKPOINT / subject.REVISION / 'manifest.json').read_bytes()

    def test_style_decision_and_exact_art_approval_are_recorded(self):
        decision = self.ledger['style_decision']
        self.assertEqual(decision['date'], '2026-10-30')
        self.assertIn('95 YR / 4 MO / 4 DAY', decision['decision'])
        self.assertIn('two-letter', decision['decision'])
        self.assertTrue(decision['scope'].startswith('style only'))
        # The user approved the art on 2026-10-30 ("Final approved"); each image has its own
        # approve event in style-decisions.jsonl bound to its exact hash.
        approval = self.ledger['art_approval']
        self.assertEqual((approval['approver'], approval['date'], approval['source'], approval['words']),
                         ('user (project owner)', '2026-10-30', 'chat', 'Final approved'))
        history = [json.loads(l) for l in (subject.CHECKPOINTS.parent / 'style-decisions.jsonl').read_text().splitlines() if l]
        events = {e['event_id']: e for e in history}
        for asset in self.ledger['assets']:
            self.assertIs(asset['human_approved'], True, asset['id'])
            self.assertEqual(asset['status'], 'approved')
            self.assertTrue(asset['port_made'])
            event = events[asset['human_approval_event']]
            self.assertEqual((event['decision'], event['output_sha256'], event['asset_id']),
                             ('approve', asset['output_sha256'], 'checkpoint:T39004-date-panel-v1/' + asset['id']))
            self.assertEqual(event['approval']['words'], 'Final approved')
        self.assertEqual(approval['events'], len(self.ledger['assets']))
        self.assertNotIn('ship', self.ledger['publication_authorization'].lower())

    def test_assets_are_the_chosen_forms_only(self):
        ids = [a['id'] for a in self.ledger['assets']]
        frame60 = [a for a in self.ledger['assets'] if a['frame'] == 60]
        self.assertEqual(sorted((a['palette_id'], a['part_index']) for a in frame60),
                         [(p, n) for p in ['base', 'hud-43', 'hud-44', 'icon-51', 'icon-52', 'icon-53'] for n in (3, 4, 5)])
        self.assertTrue(all(i.startswith('mainrom-alt-0060-captions-part') for i in ids if 'weekday' not in i))
        weekdays = [a for a in self.ledger['assets'] if a['frame'] != 60]
        self.assertEqual(len(weekdays), 35)
        self.assertTrue(all(a['id'].startswith('mainrom-weekday-two-letter-') and len(a['text']) == 2 for a in weekdays))
        self.assertFalse([i for i in ids if 'month' in i or 'three-letter' in i or i.startswith('mainrom-draft-0060')])

    def test_files_match_the_ledger_and_manifest(self):
        for asset in self.ledger['assets']:
            path = CHECKPOINT / asset['path']
            self.assertEqual(digest(path.read_bytes()), asset['output_sha256'], asset['id'])
            self.assertEqual(asset['manifest_sha256'], digest(self.manifest_bytes))
        files = sorted(p.relative_to(CHECKPOINT / subject.REVISION).as_posix()
                       for p in (CHECKPOINT / subject.REVISION).rglob('*.png'))
        self.assertEqual(files, sorted(a['path'].split('/', 1)[1] for a in self.ledger['assets']))

    def test_frame60_edits_parts_three_to_five_only(self):
        for asset in self.ledger['assets']:
            if asset['frame'] != 60:
                continue
            self.assertEqual(asset['edited_parts'], [3, 4, 5])
            self.assertEqual(asset['text'], subject.FRAME60_PART_TEXT[asset['part_index']])
            self.assertEqual(asset['runtime_applicable'], asset['palette_id'] == 'base')
            if asset['runtime_applicable']:
                shas = asset['frame_parts_sha256']['base']
                self.assertEqual(len(shas), 13)
                self.assertEqual(asset['source_part_sha256'], shas[asset['part_index']])

    def test_frame60_commits_no_unedited_original_art(self):
        """Only the three edited 16x16 parts are committed, never the 40x139 composite (section 10)."""
        self.assertIs(self.ledger['original_images_included'], False)
        self.assertIn('other ten parts are not included', self.ledger['original_images_note'])
        manifest = json.loads(self.manifest_bytes)
        self.assertTrue(all(d['composite_committed'] is False and len(d['parts']) == 3 for d in manifest['frame60_drafts']))
        for asset in self.ledger['assets']:
            if asset['frame'] == 60:
                self.assertEqual(asset['dimensions'], [16, 16], asset['id'])
                self.assertEqual(png_size(CHECKPOINT / asset['path']), (16, 16), asset['id'])

    def test_superseded_reviewed_images_are_named_by_output(self):
        named = [s for a in self.ledger['assets'] for s in a.get('supersedes_output_sha256', {}).values()]
        self.assertEqual(len(named), 12)
        self.assertEqual(len(set(named)), 12)
        self.assertTrue(all(s['ledger'].endswith(('T39004-pass2/ledger.json', 'T39004-pass3/ledger.json'))
                            for s in self.ledger['supersedes']))


class ReproductionTests(unittest.TestCase):
    def test_weekday_generator_reproduces_the_published_set(self):
        import translation_weekday_drafts as weekdays
        sources = subject.ROOT / '.asset-cache/translation-assets/original-mainrom-v1'
        pack = subject.ROOT / 'disc/files/CDROM/FUKUTEST/MAINROM.NBN'
        if not sources.exists() or not pack.exists():
            self.skipTest('restore original MAINROM export and disc files first')
        ledger = json.loads((CHECKPOINT / 'ledger.json').read_text())
        (subject.ROOT / '.asset-cache').mkdir(exist_ok=True)
        with tempfile.TemporaryDirectory(dir=subject.ROOT / '.asset-cache') as directory:
            output = Path(directory) / 'weekdays'
            weekdays.build(sources, pack, output, sets=(subject.WEEKDAY_SET,))
            manifest = json.loads((output / 'manifest.json').read_text())
            built = {row['id']: row['output_sha256'] for row in manifest['assets']}
            published = {a['id']: a['output_sha256'] for a in ledger['assets'] if a['frame'] != 60}
            self.assertEqual(built, published)


    def _local(self):
        names = ('TOKIMEMO_DATE_PANEL_FRAME60', 'TOKIMEMO_DATE_PANEL_WEEKDAYS', 'TOKIMEMO_DATE_PANEL_HUD',
                 'TOKIMEMO_DATE_PANEL_ORIGINAL')
        if not all(os.environ.get(n) for n in names):
            self.skipTest('local draft directories not given')
        return [Path(os.environ[n]) for n in names]

    def test_preview_from_installed_hud_reproduces_the_base_draft(self):
        from PIL import Image
        frame60, _, hud, _ = self._local()
        manifest = json.loads((frame60 / 'manifest.json').read_text())
        record = next(r for r in manifest['alternates'] if r['id'] == subject.FRAME60_PREFIX + 'base')
        draft = Image.open(frame60 / record['output']).convert('RGBA')
        self.assertEqual(subject.preview(CHECKPOINT, hud).tobytes(), draft.tobytes())

    def test_a_variant_changed_outside_its_parts_is_refused(self):
        from PIL import Image
        frame60, _, _, original = self._local()
        manifest = json.loads((frame60 / 'manifest.json').read_text())
        record = next(r for r in manifest['alternates'] if r['id'] == subject.FRAME60_PREFIX + 'hud-43')
        draft = Image.open(frame60 / record['output']).convert('RGBA')
        source = subject.original_frame(original, record)
        subject.outside_parts_unchanged(draft, source, [[10, 24, 16, 16], [10, 56, 16, 16], [10, 88, 16, 16]], 'hud-43')
        draft.putpixel((1, 1), (1, 2, 3, 255))
        with self.assertRaisesRegex(ValueError, r'changes pixel \(1, 1\) outside'):
            subject.outside_parts_unchanged(draft, source, [[10, 24, 16, 16]], 'hud-43')

    def test_rebuild_from_local_drafts_is_byte_identical(self):
        frame60, weekdays, hud, original = self._local()
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory) / 'rebuilt'
            subject.build(frame60, weekdays, hud, original, output)
            rebuilt = sorted(p.relative_to(output).as_posix() for p in output.rglob('*') if p.is_file())
            published = sorted(p.relative_to(CHECKPOINT).as_posix() for p in CHECKPOINT.rglob('*') if p.is_file())
            self.assertEqual(rebuilt, published)
            for name in rebuilt:
                self.assertEqual((output / name).read_bytes(), (CHECKPOINT / name).read_bytes(), name)
            with self.assertRaisesRegex(ValueError, 'must be new'):
                subject.build(frame60, weekdays, hud, original, output)


if __name__ == '__main__':
    unittest.main()
