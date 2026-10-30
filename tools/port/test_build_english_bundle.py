"""Phase 4 WP-C: English bundle builder contract tests.

Synthetic text and original files (no game data); the committed artwork
snapshot is copied for the image members. Run in Docker:
  docker run --rm -v "$PWD:/work" -w /work tokimemo-content python3 -m unittest tools/port/test_build_english_bundle.py
"""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import tempfile
import unittest

from PIL import Image

MODULE = Path(__file__).with_name('build_english_bundle.py')
SPEC = importlib.util.spec_from_file_location('build_english_bundle', MODULE)
bundle = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(bundle)
ROOT = bundle.ROOT
assets = bundle.assets


class StubFont:
    """Every code has a glyph 8 px wide except full-width 'Z' (0x8279), which has none."""
    metadata = {'glyph_bank_sha256': '0' * 64}
    MISSING = 0x8279

    def supported(self, code):
        return code != self.MISSING

    def advance(self, code):
        return 4 if code == 0x8140 else (None if code == self.MISSING else 8)


def sha(data):
    return hashlib.sha256(data).hexdigest()


# Prologue source lines (Shift-JIS bytes, NUL-terminated) and their English.
LINES = [
    (b'\x82\xa0\x82\xa2\0', 'Hello there.'),                  # plain: included
    (b'\x82\xa0 \x82\xa2\0', 'Two rows, one line.'),          # row break only: included
    (b'\x89\xb4P\x82\xa0\0', "My name's P."),                 # name token: stays Japanese
    (b'\x82\xa0\x82\xa4\0', 'Keep [[T001:row]] marker'),      # protected marker
    (b'\x82\xa0\x82\xa6\0', 'Café'),                       # character without a glyph
    (b'\x82\xa0\x82\xa8\0', None),                            # no record
    (b'\x82\xa0\xb1\x82\xa2\0', 'Half-width source.'),          # other single-byte control
    (b'\x82\xa0\x82\xaa\0', 'You have %d yen.'),              # printf directive
    (b'\x82\xa0\x82\xac\0', 'Zoo.'),                          # full-width code the font lacks
    (b'\x82\xa0\x82\xae\0', ' '.join(['word'] * 15)),         # three rows of 7 words: overflows
    (b'\x82\xa0\x82\xb0\0', ' '.join(['word'] * 14)),         # two full rows of 7 words: fits
    (b'\x82\xa0\x82\xb2\0', 'Item #1.'),                      # text-layer glyph the presenter does not map
]


class EnglishBundleTests(unittest.TestCase):
    def setUp(self):
        (ROOT / '.asset-cache').mkdir(exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(dir=ROOT / '.asset-cache', prefix='english-bundle-test-')
        self.root = Path(self.temp.name)
        self.translations = self.root / 'translations'
        (self.translations / 'graphics').mkdir(parents=True)
        shutil.copytree(ROOT / 'translations/en/graphics/artwork', self.translations / 'graphics/artwork')
        self.files = self.root / 'files'
        (self.files / 'CDROM/EXEDIR').mkdir(parents=True)
        executable = b'\0' * 16 + b'\x82\xa0\x82\xa2\0' + b'\0' * 11
        (self.files / 'SLPM_86.053').write_bytes(executable)
        container = bytearray(b'\0' * 32)
        payload_lines, returned = [], [
            {'id': 'SLPM_86.053:00000010:%s' % sha(executable[16:21])[:12], 'target_text': 'Hi.', 'protected_tokens': []}]
        for index, (raw, text) in enumerate(LINES):
            offset = len(container)
            container += raw + b'\0\0'
            payload_lines.append({'id': 'prologue:line:%02d' % index, 'line_id': index,
                                  'storage': {'offset': offset, 'raw_hex': (raw + b'\0\0').hex()}})
            if text is not None:
                returned.append({'id': '%s:%08x:%s' % (bundle.PROLOGUE_CONTAINER, offset, sha(raw)[:12]),
                                 'target_text': text, 'protected_tokens': [], 'review_status': 'ready_for_review'})
        (self.files / bundle.PROLOGUE_CONTAINER).write_bytes(bytes(container))
        self.returned = returned
        supplemental = [{'id': 'supplemental:CDROM/EXEDIR/X.EXN:0000', 'translation': 'Plain'}]
        (self.translations / 'returned-v3.jsonl').write_text(''.join(json.dumps(r) + '\n' for r in returned))
        (self.translations / 'supplemental.jsonl').write_text(''.join(json.dumps(r) + '\n' for r in supplemental))
        total = len(returned) + len(supplemental)
        (self.translations / 'graphics/inventory_decisions.json').write_text(json.dumps({'text_approval': {
            'approver': 'user', 'date': '2026-10-29', 'instruction': 'approved', 'source': 'chat',
            'scope': 'every record in translations/en/returned-v3.jsonl and translations/en/supplemental.jsonl (%d records at this date)' % total}}))
        (self.translations / 'manifest.json').write_text('{}')
        self.installed = self.root / 'installed'
        (self.installed / 'packs/prologue_text').mkdir(parents=True)
        payload = json.dumps({'lines': payload_lines}, sort_keys=True).encode()
        (self.installed / 'packs/prologue_text/prologue.json').write_bytes(payload)
        self.payload_sha = sha(payload)
        (self.installed / 'packs/prologue_text/manifest.json').write_text(json.dumps({
            'schema': bundle.PROLOGUE_SCHEMA, 'payload': 'prologue.json', 'sha256': self.payload_sha,
            'line_count': len(payload_lines), 'runtime_execution_proven': False}))

    def tearDown(self):
        self.temp.cleanup()

    def build(self, name, **overrides):
        options = dict(translations=self.translations, files=self.files, installed=self.installed,
                       font=StubFont(), prologue_payload_sha=self.payload_sha)
        options.update(overrides)
        return bundle.build(self.root / name, **options)

    def test_members_disclosure_and_determinism(self):
        first = self.build('one')
        second = self.build('two')
        self.assertEqual((self.root / 'one/english_bundle.json').read_bytes(), (self.root / 'two/english_bundle.json').read_bytes())
        for member in first['members'].values():
            self.assertEqual((self.root / 'one' / member['path']).read_bytes(), (self.root / 'two' / member['path']).read_bytes())
        self.assertEqual(set(first['members']), {'ui_text', 'dialogue', 'images'})
        self.assertEqual(first['omitted'], [])
        self.assertFalse(first['human_approval'])
        self.assertEqual(first['generator'], 'tools/port/build_english_bundle.py')
        self.assertTrue(first['disclosure_details']['ai_translation'])
        self.assertIn('voices remain Japanese', first['disclosure'])
        for name, member in first['members'].items():
            self.assertEqual(member['sha256'], sha((self.root / 'one' / member['path']).read_bytes()), name)
            self.assertTrue(member['disclosure'])
            self.assertIn('coverage', member)
        self.assertEqual(first['members']['ui_text']['schema'], 'tokimemo.text-layer-pack.v1')
        # Coverage keys checked by port/godot/services/language_bundle_service.gd.
        self.assertEqual(first['members']['images']['schema'], assets.PACK_V1)
        ui_text = first['members']['ui_text']['coverage']
        self.assertEqual(ui_text['approved_records'], ui_text['representable_records'])
        images = first['members']['images']['coverage']
        self.assertEqual(images['reviewed_images'], images['assets'])
        self.assertIsInstance(images['original_japanese_images'], int)
        self.assertEqual(first['preference_keys']['dialogue'], ['dialogue_manifest', 'dialogue_sha256'])
        self.assertEqual(bundle.verify(self.root / 'one')['schema'], bundle.SCHEMA)
        self.assertFalse(any(path.name.endswith('.partial') for path in self.root.iterdir()))

    def test_prologue_lines_stay_japanese_for_each_reason(self):
        result = self.build('out')
        dialogue = result['members']['dialogue']
        pack = json.loads((self.root / 'out' / dialogue['path']).read_text())
        self.assertEqual(pack['schema'], 'tokimemo.text-translation-pack.v1')
        self.assertEqual(pack['source_payload_sha256'], self.payload_sha)
        self.assertEqual(pack['entries'], {'0': {'speaker': '', 'text': 'Hello there.'},
                                           '1': {'speaker': '', 'text': 'Two rows, one line.'},
                                           '10': {'speaker': '', 'text': ' '.join(['word'] * 14)}})
        self.assertEqual(dialogue['coverage']['rejected'], {'format_directive': 1, 'layout_overflow': 1, 'no_record': 1,
                                                            'protected_marker': 1, 'source_name_token': 1,
                                                            'source_single_byte_control': 1, 'unsupported_character': 3})
        self.assertEqual(dialogue['coverage']['layout_overflow_line_ids'], ['9'])
        self.assertEqual((dialogue['coverage']['lines'], dialogue['coverage']['source_lines']), (3, len(LINES)))

    def test_textbox_fit_mirrors_the_presenter(self):
        font = StubFont()
        # 252 px of body row: 31 glyphs of 8 px fit one row, a 32nd wraps.
        self.assertTrue(bundle.fits_textbox('a' * 62, font))
        self.assertFalse(bundle.fits_textbox('a' * 63, font))
        self.assertTrue(bundle.fits_textbox(' '.join(['word'] * 14), font))
        self.assertFalse(bundle.fits_textbox(' '.join(['word'] * 15), font))
        self.assertFalse(bundle.fits_textbox('Zoo', font))
        self.assertIsNone(bundle.presenter_code('#'))  # in the text layer table, not the presenter's

    def test_draft_from_another_decoder_fails(self):
        source = self.root / 'bs-drafts'
        source.mkdir()
        Image.new('RGB', (32, 16), (9, 9, 9)).save(source / 'still.png')
        data = (source / 'still.png').read_bytes()
        (source / 'manifest.json').write_text(json.dumps({'schema': 'tokimemo.mdec-translation-drafts.v1', 'assets': [
            {'id': 'synthetic-still', 'kind': 'bs', 'source_container_sha256': '1' * 64, 'bs_index': 0,
             'decoder_identity_sha256': assets.decoder_identity('another-model', {}), 'source_rgba_sha256': '5' * 64,
             'regions': [[0, 0, 16, 16]], 'dimensions': [32, 16], 'output': 'still.png', 'output_sha256': sha(data),
             'style': 'unreviewed'}]}))
        self.assertFailsCleanly('other-decoder', 'another decoder identity', with_drafts=True, draft_sources=[source])

    def test_missing_inputs_omit_members_with_reasons(self):
        no_installed = self.build('a', installed=None)
        self.assertNotIn('dialogue', no_installed['members'])
        self.assertEqual(no_installed['omitted'], [{'member': 'dialogue', 'reason': 'installed prologue text pack is unavailable'}])
        self.assertIn('ui_text', no_installed['members'])
        no_files = self.build('b', files=None)
        self.assertEqual(set(no_files['members']), {'images'})
        self.assertEqual({row['member'] for row in no_files['omitted']}, {'ui_text', 'dialogue'})
        self.assertEqual(no_files['preference_keys'], {'images': ['text_manifest', 'text_sha256']})

    def assertFailsCleanly(self, name, pattern, **overrides):
        with self.assertRaisesRegex(ValueError, pattern):
            self.build(name, **overrides)
        self.assertFalse((self.root / name).exists())
        self.assertFalse((self.root / ('.' + name + '.partial')).exists())

    def test_hash_mismatches_fail_the_build(self):
        self.assertFailsCleanly('pin', 'prologue payload differs', prologue_payload_sha='0' * 64)
        self.assertFailsCleanly('exe', 'executable differs', font=None)
        container = self.files / bundle.PROLOGUE_CONTAINER
        data = bytearray(container.read_bytes())
        data[32] ^= 1
        container.write_bytes(bytes(data))
        self.assertFailsCleanly('source', 'source bytes differ')
        data[32] ^= 1
        container.write_bytes(bytes(data))
        artwork = self.translations / 'graphics/artwork'
        manifest = json.loads((artwork / 'manifest.json').read_text())
        current = next(asset for version in manifest['versions'] if version.get('current') for asset in version['assets'])
        png = artwork / current['output']
        png.write_bytes(png.read_bytes() + b'\0')
        self.assertFailsCleanly('art', 'artwork bytes or dimensions differ')

    def test_existing_output_is_never_replaced(self):
        (self.root / 'taken').mkdir()
        with self.assertRaisesRegex(ValueError, 'new immutable directory'):
            self.build('taken')

    def test_drafts_never_enter_the_approved_pack(self):
        artwork = self.translations / 'graphics/artwork'
        report = assets.audit(artwork)
        demoted = next(row for row in report['assets'] if row['mapping_complete'])
        history = artwork / 'style-decisions.jsonl'
        event = {'actor': 'test', 'asset_id': demoted['id'], 'category': 'style', 'decision': 'revise',
                 'event_id': 'f' * 32, 'output_sha256': demoted['output_sha256'], 'revision': demoted['revision'],
                 'source_sha256': demoted['source_png_sha256']}
        history.write_bytes(history.read_bytes() + json.dumps(event).encode() + b'\n')
        manifest = json.loads((artwork / 'manifest.json').read_text())
        manifest['review_history_sha256'] = sha(history.read_bytes())
        (artwork / 'manifest.json').write_text(json.dumps(manifest))
        page = self._page_draft_source()
        result = self.build('drafts', with_drafts=True, draft_sources=[page])
        approved = json.loads((self.root / 'drafts' / result['members']['images']['path']).read_text())
        drafts = json.loads((self.root / 'drafts' / result['members']['draft_images']['path']).read_text())
        self.assertFalse(approved['development_drafts'])
        self.assertTrue(all(row['style'] == 'approve' for row in approved['assets']))
        self.assertNotIn(demoted['id'], [row['id'] for row in approved['assets']])
        self.assertNotIn('page', [row.get('kind', 'frame') for row in approved['assets']])
        self.assertTrue(drafts['development_drafts'])
        self.assertIn(demoted['id'], [row['id'] for row in drafts['assets']])
        self.assertIn('page', [row['kind'] for row in drafts['assets']])
        member = result['members']['draft_images']
        self.assertEqual((member['opt_in'], member['default_enabled']), ('allow_drafts', False))
        self.assertEqual(member['schema'], assets.PACK_V2)
        self.assertEqual(member['coverage']['draft_images'], 2)
        without = self.build('plain')
        self.assertNotIn('draft_images', without['members'])
        frames_only = self.build('frames', with_drafts=True)
        self.assertEqual(frames_only['members']['draft_images']['schema'], assets.PACK_V1)

    def test_tampered_draft_source_fails(self):
        page = self._page_draft_source()
        image = page / 'page.png'
        image.write_bytes(image.read_bytes() + b'\0')
        self.assertFailsCleanly('tampered', 'differs from its manifest', with_drafts=True, draft_sources=[page])

    def test_verify_detects_a_changed_member(self):
        result = self.build('out')
        path = self.root / 'out' / result['members']['dialogue']['path']
        path.write_bytes(path.read_bytes() + b' ')
        with self.assertRaisesRegex(ValueError, 'member hash differs: dialogue'):
            bundle.verify(self.root / 'out')

    def test_cli_reports_failure(self):
        code = bundle.main(['--output', str(self.root / 'cli'), '--translations', str(self.translations),
                            '--files', str(self.files), '--installed', str(self.installed)])
        self.assertEqual(code, 1)  # synthetic executable is not the verified release
        self.assertFalse((self.root / 'cli').exists())

    def test_hud_checkpoints_need_the_installed_hud(self):
        result = self.build('nohud', with_drafts=True)
        coverage = result['members']['draft_images']['coverage']
        self.assertEqual(coverage['hud_checkpoints_omitted'], 'installed HUD manifest is unavailable')
        self.assertEqual((coverage['hud_checkpoint_rows'], coverage['superseded_reviewed_images']), (0, 0))

    def _hud_checkpoint(self, supersedes=True, superseded_sha=None):
        """Replace the committed HUD checkpoint by a synthetic one bound to a synthetic installed HUD.

        Its frame-60 draft edits part 0; its weekday draft has the source of an
        approved snapshot image (frame 82 hud-44) and supersedes it when asked.
        """
        import test_translation_draft_pack as synthetic
        artwork = self.translations / 'graphics/artwork'
        shutil.rmtree(artwork / 'checkpoints/T39004-date-panel-v1')
        approved = next(row for row in assets.audit(artwork)['assets']
                        if row['mapping_complete'] and row['style'] == 'approve' and row['frame'] == 82 and row['palette'] == 'hud-44')
        work = self.root / 'synthetic'
        hud, original = synthetic.synthetic_hud(work, weekday_sha=approved['source_png_sha256'])
        manifest = json.loads(hud.read_text())
        manifest['provenance']['inputs']['mainrom']['sha256'] = approved['source_container_sha256']
        manifest['frames']['82:hud-44'] = manifest['frames'].pop('82:base')
        hud.write_text(json.dumps(manifest))
        draft = original.copy()
        draft.putpixel((2, 3), synthetic.INK)
        record = manifest['frames']['60:base']
        shas = [part['png']['sha256'] for part in record['parts']]
        checkpoint = synthetic.synthetic_checkpoint(work, hud, draft, [0], shas)
        ledger = json.loads((checkpoint / 'ledger.json').read_text())
        for row in ledger['assets']:
            row['source_container_sha256'] = approved['source_container_sha256']
        weekday = ledger['assets'][1]
        weekday.update({'id': 'mainrom-weekday-two-letter-mon.hud-44', 'palette_id': 'hud-44', 'palette_ids': ['hud-44'],
                        'source_png_sha256': approved['source_png_sha256']})
        if supersedes:
            weekday['supersedes_output_sha256'] = {'hud-44': superseded_sha or approved['output_sha256']}
        (checkpoint / 'ledger.json').write_text(json.dumps(ledger))
        shutil.copytree(checkpoint, artwork / 'checkpoints/T39004-synthetic-hud')
        (self.installed / 'packs/hud_manifest').mkdir(parents=True)
        shutil.copytree(hud.parent / 'assets', self.installed / 'packs/hud_manifest/assets')
        shutil.copyfile(hud, self.installed / 'packs/hud_manifest/manifest.json')
        return approved

    def test_hud_checkpoint_parts_and_superseded_reviewed_image(self):
        approved = self._hud_checkpoint()
        result = self.build('hud', with_drafts=True)
        member = result['members']['draft_images']
        self.assertEqual(member['schema'], assets.PACK_V2)
        self.assertEqual((member['coverage']['hud_checkpoint_rows'], member['coverage']['superseded_reviewed_images']), (2, 1))
        self.assertNotIn('hud_checkpoints_omitted', member['coverage'])
        drafts = json.loads((self.root / 'hud' / member['path']).read_text())
        part = next(row for row in drafts['assets'] if row.get('kind') == 'frame_part')
        self.assertEqual((part['frame'], part['palette'], part['part_index']), (60, 'base', 0))
        self.assertTrue((self.root / 'hud/draft_images' / part['output']).is_file())
        outputs = [row['output_sha256'] for row in drafts['assets']]
        self.assertNotIn(approved['output_sha256'], outputs)
        weekday = next(row for row in drafts['assets'] if row['id'] == 'mainrom-weekday-two-letter-mon.hud-44')
        self.assertEqual(weekday['supersedes_output_sha256'], approved['output_sha256'])
        reviewed = json.loads((self.root / 'hud' / result['members']['images']['path']).read_text())
        self.assertIn(approved['output_sha256'], [row['output_sha256'] for row in reviewed['assets']])

    def test_hud_checkpoint_collision_without_matching_supersede_fails(self):
        self._hud_checkpoint(supersedes=False)
        self.assertFailsCleanly('collide', 'does not supersede', with_drafts=True)

    def test_hud_checkpoint_superseding_another_output_fails(self):
        self._hud_checkpoint(superseded_sha='0' * 64)
        self.assertFailsCleanly('stale', 'superseded snapshot image differs', with_drafts=True)

    def _page_draft_source(self):
        source = self.root / 'page-drafts'
        source.mkdir()
        Image.new('RGBA', (8, 8), (9, 9, 9, 255)).save(source / 'page.png')
        data = (source / 'page.png').read_bytes()
        (source / 'manifest.json').write_text(json.dumps({'schema': 'tokimemo.filesel-page-drafts.v1', 'assets': [
            {'id': 'synthetic-page', 'kind': 'page', 'source_container_sha256': '1' * 64, 'page': 0,
             'source_png_sha256': '2' * 64, 'source_rgba_sha256': '6' * 64, 'regions': [[0, 0, 4, 4]], 'dimensions': [8, 8],
             'output': 'page.png', 'output_sha256': sha(data), 'style': 'unreviewed'}]}))
        return source


if __name__ == '__main__':
    unittest.main()
