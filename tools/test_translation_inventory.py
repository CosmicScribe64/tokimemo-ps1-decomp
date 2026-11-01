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
        stale = set(json.loads(subject.DECISIONS.read_text())['superseded_historical'])
        self.assertEqual({h['id'] for h in self.built['historical_assets']} | stale, {a['id'] for a in report['assets']})
        self.assertEqual(len(stale), 12)
        self.assertFalse({h['id'] for h in self.built['historical_assets']} & stale)
        # the 12 stale three-letter weekday drafts are superseded by the date panel ledger and listed there
        self.assertEqual(len(self.built['historical_assets']), 59 - 12)
        by_status = self.built['summary']['historical_assets']['by_status']
        self.assertEqual((by_status['english-reviewed'], by_status['unmapped']), (56 - 12, 3))
        ledger_ids = {(e['ledger'], e['id'], e['revision']) for e in subject.load_ledgers()}
        inv_ids = {(c['ledger'], c['id'], c['revision']) for c in self.built['candidate_records']}
        superseded = {(c['ledger'], c['id'], c['revision']) for c in self.built['superseded_records'] if not c['ledger'].startswith('snapshot:')}
        self.assertFalse(inv_ids & superseded)
        self.assertEqual(ledger_ids, inv_ids | superseded)
        referenced = {c['id'] + '@' + c['revision'] for i in self.built['artwork_items'] for c in i['english']['candidates']}
        self.assertEqual(referenced, {c['id'] + '@' + c['revision'] for c in self.built['candidate_records']})

    def test_candidate_files_exist_with_recorded_hashes(self):
        for candidate in self.built['candidate_records']:
            path = subject.ROOT / 'translations/en/graphics/artwork/checkpoints' / candidate['ledger'].split('checkpoints/')[1].rsplit('/', 1)[0] / candidate['path']
            self.assertEqual(digest(path.read_bytes()), candidate['output_sha256'], candidate['id'])
            # Human approval exists only for the date panel art, each with its own exact approve event.
            if candidate['human_approved']:
                self.assertIn('T39004-date-panel-v1', candidate['ledger'], candidate['id'])
                self.assertEqual(candidate['status'], 'english-reviewed')
                self.assertTrue(candidate['human_review_event'])
            else:
                self.assertEqual(candidate['status'], 'candidate-only')

    def test_agent_review_is_never_human_approval(self):
        reviewed = [c for c in self.built['candidate_records'] if c['review_level'] == 'agent-bounded-in-game']
        self.assertEqual(len(reviewed), 4)    # five were replaced by the T-39410 redraws
        self.assertFalse([c for c in reviewed if c['human_approved']])
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
        self.assertEqual(summary['artwork_items']['total'], 272)    # + name_ent-abc (T-39411 ABC selector cell)
        self.assertEqual(summary['artwork_items']['by_status'], {
            'english-reviewed': 52, 'candidate-only': 192, 'missing': 1, 'unmapped': 0, 'deliberately-untranslated': 27})
        self.assertEqual(summary['historical_assets']['by_status']['english-reviewed'], 44)
        self.assertEqual(summary['historical_assets']['by_status']['unmapped'], 3)
        self.assertEqual(summary['candidate_records'], {'total': 447, 'distinct_assets': 443, 'agent_reviewed_revisions': 4, 'human_approved': 53})
        self.assertEqual(summary['superseded_records'], 239)    # 23 + 165 mainrom + 39 menus + 12 stale weekday snapshot assets
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
        hist = {h['id']: h for h in built['historical_assets']}
        for item in weekday.values():
            # The older reviewed assets cover only some variants; review needs the approved
            # two-letter cells of every variant.
            covered = {hist[h]['source_png_sha256'] for h in item['english']['historical_assets']}
            self.assertTrue(set(item['source_png_sha256'].values()) - covered, item['id'])
            self.assertEqual(item['status'], 'english-reviewed', item['id'])
            self.assertEqual(set(item['variant_status'].values()), {'english-reviewed'}, item['id'])
            self.assertEqual(len(item['english']['approved_candidates']), 5, item['id'])

    def test_ledger_approval_needs_an_exact_approve_event(self):
        date_panel = next(p for p in subject.LEDGERS if 'date-panel' in str(p))
        with tempfile.TemporaryDirectory() as directory:
            base = Path(directory)
            (base / 'date-panel-drafts-v1').symlink_to(date_panel.parent / 'date-panel-drafts-v1')
            ledger = json.loads(date_panel.read_text())
            ledger['assets'][0]['human_approval_event'] = '0' * 32
            (base / 'ledger.json').write_text(json.dumps(ledger))
            with self.assertRaisesRegex(ValueError, 'without an exact approve event'):
                subject.load_ledgers([base / 'ledger.json'])
            ledger['assets'][0]['status'] = 'draft'
            (base / 'ledger.json').write_text(json.dumps(ledger))
            with self.assertRaisesRegex(ValueError, 'claims or omits human approval'):
                subject.load_ledgers([base / 'ledger.json'])

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
        self.assertEqual(subject.candidate_key({'id': 'mainrom-fix-0126.hud-43', 'frame': 126, 'palette_id': 'hud-43'}),
                         ('mainrom-0126', 'MAINROM', 126, 'hud-43'))
        with self.assertRaisesRegex(ValueError, 'disagrees with its frame or palette'):
            subject.candidate_key({'id': 'mainrom-fix-0126.hud-43', 'frame': 127, 'palette_id': 'hud-43'})

    def test_missing_art_is_drafted_except_the_cut_off_ride_sign(self):
        items = {i['id']: i for i in subject.build()['artwork_items']}
        tags = {'mainrom-%04d' % n: 6 for n in (134, 135, 138, 139, 141, 142, 143, 144, 145)}    # one T-39410 file per palette variant
        for item_id, count in tags.items():
            self.assertEqual(items[item_id]['status'], 'candidate-only', item_id)
            self.assertEqual(len(items[item_id]['english']['candidates']), count, item_id)
        signs = {'catalog-130': ('P_01_02', 6), 'catalog-133': ('D_01_47', 1), 'catalog-134': ('D_01_48', 1),
                 'catalog-136': ('D_01_70', 1), 'catalog-139': ('D_03_08', 1), 'catalog-141': ('D_29_09', 1),
                 'catalog-143': ('D_15_03', 1), 'catalog-144': ('D_19_02', 2), 'catalog-145': ('D_24_03', 1),
                 'catalog-146': ('I_28_02', 1), 'catalog-149': ('I_17_01', 1)}
        for item_id, (name, count) in signs.items():
            self.assertEqual(items[item_id]['status'], 'candidate-only', item_id)
            self.assertTrue(items[item_id]['container'].endswith('/%s.BS' % name), item_id)
            self.assertEqual(len(items[item_id]['english']['candidates']), count, item_id)
            self.assertTrue(items[item_id]['container_sha256'], item_id)
        self.assertEqual(items['catalog-142']['status'], 'missing')
        self.assertTrue(items['catalog-142']['container'].endswith('/D_11_11.BS'))
        self.assertEqual([i for i, v in items.items() if v['status'] == 'missing'], ['catalog-142'])
        self.assertTrue(items['catalog-135']['container'].endswith('/D_01_69.BS'))

    def test_registered_checkpoints_keep_their_drafts_unapproved(self):
        built = subject.build()
        records = [c for c in built['candidate_records'] if c['ledger'].endswith(('T39400-bs-signs-v1/ledger.json', 'T39004-mainrom-port-made-v1/ledger.json'))]
        # 17 sign crops minus the 5 redrawn by T-39412; the 47 registered port-made MAINROM rows are all superseded by T-39410
        # (the six month-name composites of frame 60 stay out)
        self.assertEqual(len(records), 17 - 5)
        self.assertFalse(any(c['human_approved'] for c in records))
        self.assertEqual({c['status'] for c in records}, {'candidate-only'})

    def test_date_panel_style_drafts_replace_older_weekday_drafts(self):
        built = subject.build()
        items = {i['id']: i for i in built['artwork_items']}
        date_panel = next(p for p in subject.LEDGERS if 'date-panel' in str(p))
        ledger = json.loads(date_panel.read_text())
        self.assertIn('style only', ledger['style_decision']['scope'])
        approval = ledger['art_approval']
        self.assertEqual((approval['approver'], approval['date'], approval['source'], approval['words']),
                         ('user (project owner)', '2026-10-30', 'chat', 'Final approved'))
        self.assertEqual(approval['events'], 53)
        self.assertTrue(all(a['human_approved'] is True and a['status'] == 'approved' and a['port_made'] for a in ledger['assets']))
        frame60 = items['mainrom-0060']
        self.assertEqual(frame60['status'], 'english-reviewed')
        self.assertEqual(set(frame60['variant_status'].values()), {'english-reviewed'})
        self.assertEqual(len(frame60['english']['approved_candidates']), 18)
        self.assertTrue(all(c['id'].startswith('mainrom-alt-0060-captions-part') for c in frame60['english']['candidates']))
        self.assertEqual(len(frame60['english']['candidates']), 18)
        for frame in range(82, 89):
            candidates = items['mainrom-%04d' % frame]['english']['candidates']
            self.assertEqual({c['revision'] for c in candidates}, {'date-panel-drafts-v1'}, frame)
            self.assertEqual(len(candidates), 5, frame)
        superseded = [s for s in built['superseded_records'] if s['superseded_by'] == str(date_panel.relative_to(subject.ROOT))]
        self.assertEqual(len(superseded), 35)    # 23 older drafts and the 12 stale three-letter snapshot assets
        self.assertTrue(all(s['atom'] in ['mainrom-%04d' % n for n in range(82, 89)] for s in superseded))
        self.assertEqual({s['superseded_by'] for s in built['superseded_records']} - {str(date_panel.relative_to(subject.ROOT))},
                         {'translations/en/graphics/artwork/checkpoints/T39410-mainrom-fixes-v1/ledger.json',
                          'translations/en/graphics/artwork/checkpoints/T39410-menus-fixes-v1/ledger.json'})
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
                others = [p for p in subject.LEDGERS if p != source]
                entries = subject.load_ledgers(others)
                with self.assertRaisesRegex(ValueError, 'superseded reference'):
                    subject.split_superseded(entries, others + [target])

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
        built = subject.build()
        # No mapped historical asset is left on a multi-palette item (T-39410 superseded the weekday drafts), so
        # a second palette is added to the item of the first mapped asset.
        item = next(i for i in built['artwork_items'] if i['english']['historical_assets'])
        hid_, other = item['english']['historical_assets'][0], 'hud-44'
        broken = copy.deepcopy(built)
        asset = next(h for h in broken['historical_assets'] if h['id'] == hid_)
        asset['palette'] = other
        next(i for i in broken['artwork_items'] if i['id'] == item['id'])['source_png_sha256'][other] = '1' * 64
        self.assertTrue(any('names palette %s' % other in p for p in subject.validate(broken)))
        broken = copy.deepcopy(built)
        asset = next(h for h in broken['historical_assets'] if h['id'] == hid_)
        asset['source_png_sha256'] = '0' * 64
        self.assertTrue(any('not a known source image' in p for p in subject.validate(broken)))

    def test_t39410_checkpoints_are_registered_with_their_alternatives_resolved(self):
        built = subject.build()
        ledgers = {str(p.relative_to(subject.ROOT)) for p in subject.LEDGERS}
        base = 'translations/en/graphics/artwork/checkpoints/'
        for name in ('T39410-mainrom-fixes-v1', 'T39410-menus-fixes-v1', 'T39410-stills-fixes-v1'):
            self.assertIn(base + name + '/ledger.json', ledgers)
        self.assertNotIn(base + 'T39410-menus-fixes-v1/fallback-ledger.json', ledgers)
        ids = {c['id'] for c in built['candidate_records']}
        # owner decision 2026-11-01: layout A; the stacked and large alternatives stay unregistered
        self.assertLessEqual({'bs-p-01-03-roster-a-rotated', 'bs-p-01-03b-name-a-rotated'}, ids)
        self.assertFalse(ids & {'bs-p-01-03-roster-b-stacked', 'bs-p-01-03b-name-b-stacked',
                                'bs-p-01-03b-name-a-large', 'bs-p-01-03b-name-b-large'})
        # the T-39400 crops and the T-39004 exam heading redrawn by T-39412 are not evidence any more
        self.assertFalse(ids & {'bs-d-15-03-monument', 'bs-d-03-08-march-placard', 'bs-d-24-03-ticket-plaque',
                                'bs-d-19-02-maiko-board', 'bs-i-17-01-hanging-sign', 'bs-c-01-28-final-exams'})
        self.assertLessEqual({'bs-d-15-03-monument-v2', 'bs-c-01-28-final-exams-v2'}, ids)
        # the 12 stale weekday snapshot assets are superseded records, not item evidence
        stale = [s for s in built['superseded_records'] if s['ledger'].startswith('snapshot:')]
        self.assertEqual(len(stale), 12)
        self.assertTrue(all(s['superseded_by'].endswith('T39004-date-panel-v1/ledger.json') and not s['human_approved'] for s in stale))
        items = {i['id']: i for i in built['artwork_items']}
        for item_id in ('catalog-129', 'catalog-131'):
            self.assertEqual(items[item_id]['status'], 'candidate-only', item_id)
        self.assertEqual(len(items['catalog-131']['english']['candidates']), 2)
        self.assertEqual(len(items['mainrom-0126']['english']['candidates']), 6)
        # every option pair 0063..0090 is bound to exactly one catalog row
        lines = [items['option-%04d' % n]['catalog_line'] for n in range(63, 91)]
        self.assertTrue(all(lines[i] == lines[i + 1] for i in range(0, 28, 2)))
        self.assertEqual(len(set(lines)), 14)

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
        approved = [c for c in built['candidate_records'] if c['human_approved'] is not False]
        self.assertEqual(len(approved), 53)
        self.assertTrue(all('T39004-date-panel-v1' in c['ledger'] for c in approved))

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
