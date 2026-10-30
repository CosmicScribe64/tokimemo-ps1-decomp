"""T-39004: the translation inventory covers every item with exactly one status."""
import copy
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import translation_inventory as subject
from translation_assets import digest


class InventoryTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.built = subject.build()
        cls.committed = json.loads(subject.INVENTORY.read_text())

    def test_committed_inventory_matches_inputs(self):
        self.assertEqual(subject.INVENTORY.read_bytes(), subject.encoded(self.built) + b'\n')

    def test_every_item_has_a_valid_status(self):
        self.assertEqual(subject.validate(self.built), [])

    def test_missing_status_is_detected(self):
        broken = copy.deepcopy(self.built)
        del broken['artwork_items'][0]['status']
        broken['historical_assets'][0]['status'] = 'approved'
        broken['text_groups'][0]['statuses']['missing'] = broken['text_groups'][0]['statuses'].get('missing', 0) + 1
        problems = subject.validate(broken)
        self.assertEqual(len(problems), 3)

    def test_unexplained_untranslated_is_detected(self):
        broken = copy.deepcopy(self.built)
        item = next(i for i in broken['artwork_items'] if i['status'] == 'deliberately-untranslated')
        item['reason'] = ''
        self.assertTrue(subject.validate(broken))

    def test_every_catalog_row_is_inventoried(self):
        rows = [r for r in subject.CATALOG.read_bytes().splitlines() if r]
        self.assertEqual(len(rows), 153)
        lines = {n for i in self.built['artwork_items'] for n in i['catalog_lines']}
        self.assertEqual(lines, set(range(1, 154)))

    def test_every_historical_asset_and_candidate_is_accounted_for(self):
        report = subject.audit(subject.SNAPSHOT)
        self.assertEqual({h['id'] for h in self.built['historical_assets']}, {a['id'] for a in report['assets']})
        self.assertEqual(len(self.built['historical_assets']), 59)
        by_status = self.built['summary']['historical_assets']['by_status']
        self.assertEqual((by_status['english-reviewed'], by_status['unmapped']), (56, 3))
        ledger_ids = {(e['ledger'], e['id'], e['revision']) for e in subject.load_ledgers()}
        inv_ids = {(c['ledger'], c['id'], c['revision']) for c in self.built['candidate_records']}
        superseded = {(c['ledger'], c['id'], c['revision']) for c in self.built['superseded_records']}
        self.assertFalse(inv_ids & superseded)
        self.assertEqual(ledger_ids, inv_ids | superseded)
        referenced = {c['id'] + '@' + c['revision'] for i in self.built['artwork_items'] for c in i['english']['candidates']}
        self.assertEqual(referenced, {c['id'] + '@' + c['revision'] for c in self.built['candidate_records']})

    def test_candidate_files_exist_with_recorded_hashes(self):
        for candidate in self.built['candidate_records']:
            path = subject.ROOT / 'translations/en/graphics/artwork/checkpoints' / candidate['ledger'].split('checkpoints/')[1].rsplit('/', 1)[0] / candidate['path']
            self.assertEqual(digest(path.read_bytes()), candidate['output_sha256'], candidate['id'])
            self.assertFalse(candidate['human_approved'])

    def test_agent_review_is_never_human_approval(self):
        reviewed = [c for c in self.built['candidate_records'] if c['review_level'] != 'none']
        self.assertEqual(len(reviewed), 9)
        for item in self.built['artwork_items']:
            if item['agent_review']:
                self.assertNotEqual(item['status'], 'english-reviewed')

    def test_unmapped_assets_record_resolution_and_do_not_transfer_approval(self):
        unmapped = [h for h in self.built['historical_assets'] if h['status'] == 'unmapped']
        self.assertEqual(len(unmapped), 3)
        # A rebind may since be superseded by a newer draft; it stays listed there.
        candidates = {c['id'] for c in self.built['candidate_records'] + self.built['superseded_records']}
        for asset in unmapped:
            self.assertIn(asset['resolution']['rebind_candidate'], candidates)
            self.assertIn('approval', asset['resolution']['why_not_exact'])

    def test_text_records_are_all_classified(self):
        total = sum(1 for p in (subject.TEXT_MAIN, subject.TEXT_SUPP) for r in p.read_bytes().splitlines() if r)
        self.assertEqual(self.built['summary']['text_records']['total'], total)
        manifest = json.loads(subject.TEXT_MANIFEST.read_text())
        self.assertEqual(manifest['main_rows'] + manifest['supplemental_rows'], total)
        self.assertEqual(self.built['inputs']['text_main_sha256'], digest(subject.TEXT_MAIN.read_bytes()))

    def test_summary_counts_match_items(self):
        for key, section in (('artwork_items', 'artwork_items'), ('historical_assets', 'historical_assets')):
            counted = {}
            for item in self.built[section]:
                counted[item['status']] = counted.get(item['status'], 0) + 1
            expected = {k: v for k, v in self.built['summary'][key]['by_status'].items() if v}
            self.assertEqual(counted, expected)


class PinnedCountTests(unittest.TestCase):
    def test_pinned_totals_and_statuses(self):
        summary = subject.build()['summary']
        self.assertEqual(summary['artwork_items']['total'], 271)
        self.assertEqual(summary['artwork_items']['by_status'], {
            'english-reviewed': 44, 'candidate-only': 174, 'missing': 24, 'unmapped': 0, 'deliberately-untranslated': 29})
        self.assertEqual(summary['historical_assets']['by_status']['english-reviewed'], 56)
        self.assertEqual(summary['historical_assets']['by_status']['unmapped'], 3)
        self.assertEqual(summary['candidate_records'], {'total': 329, 'distinct_assets': 320, 'agent_reviewed_revisions': 9, 'human_approved': 0})
        self.assertEqual(summary['superseded_records'], 23)
        self.assertEqual(summary['text_records']['total'], 55911)
        self.assertEqual(summary['text_records']['by_class'], {
            'english-approved': 55094, 'deliberately-untranslated': 817})

    def test_identity_is_required_with_explicit_exemptions(self):
        built = subject.build()
        for item in built['artwork_items']:
            if item['id'] not in subject.NO_CONTAINER_HASH:
                self.assertTrue(item['container_sha256'], item['id'])
            if item['scope'] in ('frame', 'page'):
                self.assertTrue(item['source_png_sha256'], item['id'])
        self.assertEqual(subject.NO_CONTAINER_HASH, ('catalog-152', 'catalog-153'))

    def test_review_needs_every_source_variant(self):
        built = subject.build()
        weekday = {i['id']: i for i in built['artwork_items'] if i['id'] in ['mainrom-%04d' % n for n in range(82, 89)]}
        for item in weekday.values():
            self.assertNotEqual(item['status'], 'english-reviewed', item['id'])
            self.assertIn('english-reviewed', item['variant_status'].values())
            self.assertTrue(set(item['variant_status'].values()) - {'english-reviewed'}, item['id'])

    def test_reviewed_without_source_identity_is_rejected(self):
        broken = copy.deepcopy(subject.build())
        item = next(i for i in broken['artwork_items'] if i['status'] == 'english-reviewed')
        item['source_png_sha256'] = {}
        self.assertTrue(subject.validate(broken))

    def test_ledger_approval_claims_and_unknown_status_fail(self):
        for field, value in (('human_approved', True), ('status', 'approved')):
            with tempfile.TemporaryDirectory() as directory:
                ledger = json.loads(subject.LEDGERS[0].read_text())
                ledger['assets'][0][field] = value
                path = Path(directory) / 'ledger.json'
                path.write_text(json.dumps(ledger))
                with self.assertRaises(ValueError):
                    subject.load_ledgers([path])

    def test_dropping_a_source_variant_makes_it_missing(self):
        ledger = json.loads(subject.LEDGERS[0].read_text())
        ledger['assets'] = [a for a in ledger['assets'] if a['id'] != 'filesel-frame-30.selected']
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'ledger.json'
            path.write_text(json.dumps(ledger))
            item = next(i for i in subject.build(ledgers=[path] + subject.LEDGERS[1:])['artwork_items'] if i['id'] == 'filesel-0030')
        self.assertEqual(item['variant_status'], {'frame-30.base': 'candidate-only', 'frame-30.selected': 'missing'})
        self.assertEqual(item['status'], 'missing')

    def test_decided_candidate_without_its_ledger_is_reported(self):
        ledgers = [p for p in subject.LEDGERS if 'battle-miss' not in str(p)]
        problems = subject.validate(subject.build(ledgers=ledgers))
        self.assertIn('artwork item battle-habu-miss-0 claims candidate without a file', problems)
        decisions = json.loads(subject.DECISIONS.read_text())
        self.assertEqual(decisions['atom_overrides']['battle-habu-miss-0']['status'], 'candidate-only')

    def test_new_ledger_ids_map_to_items(self):
        items = {i['id']: i for i in subject.build()['artwork_items']}
        for item_id, count in (('mainrom-0057', 6), ('battle-habu-miss-0', 1), ('filesel-page-information-2', 1),
                               ('catalog-127', 3), ('catalog-147', 1)):
            self.assertEqual(items[item_id]['status'], 'candidate-only', item_id)
            self.assertEqual(len(items[item_id]['english']['candidates']), count, item_id)
        self.assertEqual(set(items['catalog-127']['variant_status']),
                         {'all-c-banner.source-page-%d' % n for n in (3, 4, 5)})
        with self.assertRaisesRegex(ValueError, 'disagrees with its frame or palette'):
            subject.candidate_key({'id': 'mainrom-draft-0057.base', 'frame': 58, 'palette_id': 'base'})

    def test_date_panel_style_drafts_replace_older_weekday_drafts(self):
        built = subject.build()
        items = {i['id']: i for i in built['artwork_items']}
        date_panel = next(p for p in subject.LEDGERS if 'date-panel' in str(p))
        ledger = json.loads(date_panel.read_text())
        self.assertIn('style only', ledger['style_decision']['scope'])
        self.assertTrue(all(a['human_approved'] is False and a['status'] == 'draft' and a['port_made'] for a in ledger['assets']))
        frame60 = items['mainrom-0060']
        self.assertEqual(frame60['status'], 'candidate-only')
        self.assertEqual(set(frame60['variant_status'].values()), {'candidate-only'})
        self.assertTrue(all(c['id'].startswith('mainrom-alt-0060-captions-part') for c in frame60['english']['candidates']))
        self.assertEqual(len(frame60['english']['candidates']), 18)
        for frame in range(82, 89):
            candidates = items['mainrom-%04d' % frame]['english']['candidates']
            self.assertEqual({c['revision'] for c in candidates}, {'date-panel-drafts-v1'}, frame)
            self.assertEqual(len(candidates), 5, frame)
        superseded = built['superseded_records']
        self.assertEqual({s['superseded_by'] for s in superseded}, {str(date_panel.relative_to(subject.ROOT))})
        self.assertTrue(all(s['atom'] in ['mainrom-%04d' % n for n in range(82, 89)] for s in superseded))
        # The month-name form of frame 60 was dropped, never published.
        self.assertFalse([c for c in built['candidate_records'] if c['id'].startswith('mainrom-draft-0060')])

    def test_superseded_reference_must_match_its_candidate(self):
        source = next(p for p in subject.LEDGERS if 'date-panel' in str(p))
        for change in ('output', 'unknown'):
            with tempfile.TemporaryDirectory() as directory:
                ledger = json.loads(source.read_text())
                if change == 'output':
                    ledger['supersedes'][0]['output_sha256'] = '0' * 64
                else:
                    ledger['supersedes'][0]['id'] = 'mainrom-draft-0082.nothing'
                target = Path(directory) / 'ledger.json'
                target.write_text(json.dumps(ledger))
                entries = subject.load_ledgers(subject.LEDGERS[:-1])
                with self.assertRaisesRegex(ValueError, 'superseded reference'):
                    subject.split_superseded(entries, subject.LEDGERS[:-1] + [target])

    def test_weekday_and_alternate_ids_check_frame_and_palette(self):
        self.assertEqual(subject.candidate_key({'id': 'mainrom-weekday-two-letter-sun.hud-44', 'frame': 88, 'palette_id': 'hud-44'}),
                         ('mainrom-0088', 'MAINROM', 88, 'hud-44'))
        self.assertEqual(subject.candidate_key({'id': 'mainrom-alt-0060-captions.base', 'frame': 60, 'palette_id': 'base'})[0],
                         'mainrom-0060')
        self.assertEqual(subject.candidate_key({'id': 'mainrom-alt-0060-captions-part03.hud-43', 'frame': 60, 'palette_id': 'hud-43'}),
                         ('mainrom-0060', 'MAINROM', 60, 'hud-43'))
        for wrong in ({'id': 'mainrom-alt-0060-captions-part03.base', 'frame': 61, 'palette_id': 'base'},
                      {'id': 'mainrom-alt-0060-captions-part3.base', 'frame': 60, 'palette_id': 'base'}):
            with self.assertRaises(ValueError):
                subject.candidate_key(wrong)
        for wrong in ({'id': 'mainrom-weekday-two-letter-sun.base', 'frame': 82, 'palette_id': 'base'},
                      {'id': 'mainrom-alt-0060-captions.base', 'frame': 60, 'palette_id': 'hud-43'}):
            with self.assertRaisesRegex(ValueError, 'disagrees with its frame or palette'):
                subject.candidate_key(wrong)

    def _bs_ledger(self, directory, change):
        source = next(p for p in subject.LEDGERS if 'bs-v1' in str(p))
        target = Path(directory) / 'T39004-bs-v1'
        target.mkdir()
        (target / 'bs-v1').symlink_to(source.parent / 'bs-v1')
        ledger = json.loads(source.read_text())
        change(ledger['assets'])
        (target / 'ledger.json').write_text(json.dumps(ledger))
        return target / 'ledger.json'

    def test_whole_file_ledgers_are_bound_to_their_manifest(self):
        def manifest(assets):
            assets[0]['manifest_sha256'] = '0' * 64

        def crop(assets):
            assets[0]['crop_sha256'] = '0' * 64

        def container(assets):
            assets[1]['source_container_sha256'] = '0' * 64

        def approval(assets):
            assets[1]['human_approved'] = True
        for change, message in ((manifest, 'manifest hash differs'), (crop, 'crop of'),
                                (container, 'one unapproved manifest record'), (approval, 'claims or omits')):
            with tempfile.TemporaryDirectory() as directory:
                with self.assertRaisesRegex(ValueError, message):
                    subject.load_ledgers([self._bs_ledger(directory, change)])

    def test_candidate_from_another_source_container_is_rejected(self):
        source = next(p for p in subject.LEDGERS if 'battle-miss' in str(p))
        ledger = json.loads(source.read_text())
        ledger['assets'][0]['source_container_sha256'] = ledger['assets'][2]['source_container_sha256']
        with tempfile.TemporaryDirectory() as directory:
            target = Path(directory) / 'T39004-battle-miss-v1'
            target.mkdir()
            (target / 'ledger.json').write_text(json.dumps(ledger))
            ledgers = [target / 'ledger.json' if p == source else p for p in subject.LEDGERS]
            with self.assertRaisesRegex(ValueError, 'different source container'):
                subject.build(ledgers=ledgers)

    def test_palette_hash_mismatch_is_rejected(self):
        broken = copy.deepcopy(subject.build())
        asset = next(h for h in broken['historical_assets'] if h['id'] == 'draft:mainrom-weekday-tue-base')
        asset['palette'] = 'hud-44'
        self.assertTrue(any('names palette hud-44' in p for p in subject.validate(broken)))
        broken = copy.deepcopy(subject.build())
        asset = next(h for h in broken['historical_assets'] if h['id'] == 'draft:mainrom-weekday-tue-base')
        asset['source_png_sha256'] = '0' * 64
        self.assertTrue(any('not a known source image' in p for p in subject.validate(broken)))

    def test_every_catalog_container_is_bound_through_corrections(self):
        built = subject.build()
        by_id = {i['id']: i for i in built['artwork_items']}
        row = by_id['catalog-131']
        self.assertEqual([c['path'] for c in row['containers']], ['CDROM/BS/P_01_03.BS', 'CDROM/BS/P_01_03B.BS'])
        self.assertEqual([c['catalog_path'] for c in row['containers']], ['CDROM/PROLODIR/P_01_03.BS', 'CDROM/PROLODIR/P_01_03B.BS'])
        self.assertTrue(all(c['sha256'] for c in row['containers']))
        row = by_id['catalog-151']
        self.assertEqual([c['path'] for c in row['containers']],
                         ['CDROM/BUSTUP/AS34.BIN', 'CDROM/BUSTUP/HI05.BIN', 'CDROM/BUSTUP/KO15.BIN'])
        self.assertTrue(all(c['sha256'] for c in row['containers']))

    def test_text_blanks_are_classified_from_source_facts(self):
        record = {'review_status': 'x', 'target_text': None, 'translator_note': 'Empty source slot (no Japanese text).'}
        self.assertEqual(subject.text_class(record), ('deliberately-untranslated', 'translator-note:empty-source-slot', None))
        record = {'review_status': 'unresolved: whitespace_only_source_slot', 'target_text': ' ', 'translator_note': ''}
        self.assertEqual(subject.text_class(record)[0], 'deliberately-untranslated')
        record = {'review_status': 'x', 'target_text': '', 'translator_note': ''}
        self.assertEqual(subject.text_class(record), ('missing', None, None))
        record = {'review_status': 'ready_for_review', 'target_text': 'Hi', 'translator_note': ''}
        self.assertEqual(subject.text_class(record), ('candidate-only:ready', None, 'ready'))
        self.assertEqual(subject.text_class(record, approved=True)[0], 'english-approved')
        # whitespace target: deliberate only when the source is shown blank
        record = {'id': 'x:1', 'review_status': 'unresolved: consumer_context_unclassified', 'target_text': ' ', 'translator_note': ''}
        self.assertEqual(subject.text_class(record)[0], 'candidate-only:blocked')
        self.assertEqual(subject.text_class(record, approved=True), ('english-approved', 'whitespace-target-source-not-proven-blank', 'unproven'))
        checks = {'x:1': {'blank': False, 'length': 2, 'sha256': ''}}
        self.assertEqual(subject.text_class(record, checks), ('candidate-only:blocked', 'whitespace-target-source-not-blank', 'unproven'))
        checks = {'x:1': {'blank': True, 'length': 2, 'sha256': ''}}
        self.assertEqual(subject.text_class(record, checks), ('deliberately-untranslated', 'source-bytes-checked-blank', None))

    def test_multi_container_rows_resolve_siblings(self):
        self.assertEqual(subject.catalog_paths('`CDROM/BUSTUP/AS34.BIN`, `HI05.BIN`, `KO15.BIN`'),
                         ['CDROM/BUSTUP/AS34.BIN', 'CDROM/BUSTUP/HI05.BIN', 'CDROM/BUSTUP/KO15.BIN'])
        self.assertEqual(subject.catalog_paths('`CDROM/PROLODIR/P_01_03.BS` & `P_01_03B.BS`'),
                         ['CDROM/PROLODIR/P_01_03.BS', 'CDROM/PROLODIR/P_01_03B.BS'])
        built = subject.build()
        row = next(i for i in built['artwork_items'] if i['id'] == 'catalog-132')
        self.assertEqual([c['path'] for c in row['containers']], ['CDROM/PROLODIR/P_01_04B.BS', 'CDROM/PROLODIR/P_01_07.BS'])
        self.assertTrue(all(c['sha256'] for c in row['containers']))

    def _without_approval(self):
        decisions = json.loads(subject.DECISIONS.read_text())
        del decisions['text_approval']
        directory = tempfile.mkdtemp()
        path = Path(directory) / 'decisions.json'
        path.write_text(json.dumps(decisions))
        return subject.build(decisions_path=path)

    def test_text_approval_applies_only_via_the_decision_and_reverts(self):
        built, reverted = subject.build(), self._without_approval()
        self.assertEqual(built['text_approval']['approver'], 'user (project owner)')
        self.assertEqual(built['text_approval']['source'], 'chat instruction')
        self.assertEqual(built['text_approval']['date'], '2026-10-29')
        self.assertEqual(built['text_approval']['addenda'][0]['instruction'], 'Approve them')
        approved = built['summary']['text_records']['by_class']['english-approved']
        self.assertEqual(approved, 55094)
        self.assertIsNone(reverted['text_approval'])
        by_class = reverted['summary']['text_records']['by_class']
        self.assertNotIn('english-approved', by_class)
        self.assertEqual(by_class['candidate-only:ready'] + by_class['candidate-only:blocked'], approved)
        self.assertEqual(by_class['deliberately-untranslated'], 817)
        self.assertEqual(subject.validate(built), [])
        self.assertEqual(subject.validate(reverted), [])
        forged = copy.deepcopy(reverted)
        forged['text_groups'][0]['statuses']['english-approved'] = 1
        self.assertTrue(subject.validate(forged))

    def test_approval_leaves_artwork_untouched_and_context_flags_preserved(self):
        built, reverted = subject.build(), self._without_approval()
        for section in ('artwork_items', 'historical_assets', 'candidate_records'):
            self.assertEqual(built[section], reverted[section])
        for key in ('artwork_items', 'historical_assets', 'candidate_records'):
            self.assertEqual(built['summary'][key], reverted['summary'][key])
        self.assertEqual(built['summary']['text_records']['context_status'], reverted['summary']['text_records']['context_status'])
        self.assertEqual(built['summary']['text_records']['context_status'], {'ready': 31757, 'unproven': 23337})
        for a, b in zip(built['text_groups'], reverted['text_groups']):
            self.assertEqual(a.get('context_reasons'), b.get('context_reasons'))
        self.assertEqual(sum(sum(g.get('context_reasons', {}).values()) for g in built['text_groups']), 23337)
        self.assertTrue(all(c['human_approved'] is False for c in built['candidate_records']))

    def test_known_nonblank_whitespace_targets_are_not_deliberate(self):
        built = subject.build()
        evidence = built['text_blank_evidence']
        for record_id in ('CDROM/EXEDIR/TEL.EXN:0000ce28:02de066abbf9', 'SLPM_86.053:000707c4:02de066abbf9',
                          'CDROM/EXEDIR/BUNKA_SD.EXN:00008cbc:1af7c283a15d'):
            self.assertEqual(evidence[record_id], 'whitespace-target-source-not-blank')
        self.assertEqual(sum(v == 'whitespace-target-source-not-blank' for v in evidence.values()), 8)

    def test_capture_fails_before_writing_when_inputs_are_absent(self):
        with tempfile.TemporaryDirectory() as directory:
            before = subject.SOURCES.read_bytes()
            with self.assertRaises(SystemExit) as raised:
                subject.capture_sources(Path(directory), Path(directory))
            self.assertIn('missing inputs', str(raised.exception))
            self.assertEqual(subject.SOURCES.read_bytes(), before)


if __name__ == '__main__':
    unittest.main()
