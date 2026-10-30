#!/usr/bin/env python3
"""Phase 4 WP-C: build the SHA-pinned English bundle from the committed translations.

Run in Docker (tools/port/setup_content.sh runs it after content setup). The
output is a new directory holding `english_bundle.json` and its members:

- `ui_text`: the T-39016 text layer pack (tools/port/build_text_pack.py), from
  translations/en/returned-v3.jsonl, supplemental.jsonl and
  graphics/inventory_decisions.json, checked against the original font;
- `dialogue`: the prologue dialogue pack (`tokimemo.text-translation-pack.v1`,
  read by port/godot/services/text_translation_store.gd), keyed by the
  installed prologue payload hash and line id. A line is included only when
  its source bytes in NAME_ENT.EXN match the record's physical id, the source
  holds no single-byte renderer control other than the row break (sndisp
  expands 'P'/'G' to names, so such lines stay Japanese), the English has no
  protected marker, every character has an original full-width glyph that
  the prologue presenter maps, and the text fits the presenter's two English
  body rows (a line that would overflow stays Japanese as `layout_overflow`);
- `images`: the approved artwork pack, approved assets only (translated-image-pack
  v1 while it holds only frame rows, v2 once page/bs rows are approved);
- `draft_images` (only with --with-drafts): a development pack with every
  mapped snapshot asset plus the page/bs draft directories given with
  --draft-source (v2 when it holds page/bs rows). The runtime opens it only
  with allow_drafts.

The manifest follows the contract of the settings language bundle service
(port/godot/services/language_bundle_service.gd, T-30002 WP2): a top-level
`disclosure` string, members with path/sha256/schema/coverage and the coverage
keys that service checks against the opened stores, and `omitted` as a list of
{member, reason}. Each member also records its own disclosure and further
counts. A missing input omits that member and records the reason; an input
that is present but differs from its pin (original
executable, prologue payload, artwork bytes, a draft PNG) fails the build.
The build is deterministic: the same inputs give the same bytes. Nothing here
is human approval of any text or artwork.
"""
from __future__ import annotations

import argparse
import collections
import hashlib
import json
import os
from pathlib import Path
import shutil
import sys

ROOT = Path(__file__).resolve().parents[2]
for path in (ROOT, ROOT / 'tools'):
    if str(path) not in sys.path:
        sys.path.insert(0, str(path))
from tools.port import build_text_pack as text_pack  # noqa: E402
import translation_assets as assets  # noqa: E402
import translation_draft_pack as draft_pack  # noqa: E402

SCHEMA = 'tokimemo.english-bundle.v1'
BUNDLE_FILE = 'english_bundle.json'
DIALOGUE_SCHEMA = 'tokimemo.text-translation-pack.v1'
PROLOGUE_SCHEMA = 'tokimemo.prologue-text-pack.v1'
# port/godot/services/prologue_text_store.gd PAYLOAD_SHA.
PROLOGUE_PAYLOAD_SHA = '40428c547eb89f2c1b0363bfdcc9cf3e4e806bc878bc4a6317e312ccaff86855'
PROLOGUE_CONTAINER = 'CDROM/EXEDIR/NAME_ENT.EXN'
GENERATOR = 'tools/port/build_english_bundle.py'
DISCLOSURE = 'English text is AI-translated and not yet human-reviewed; voices remain Japanese.'
ART_DISCLOSURE = 'English artwork: approved assets only; anything else stays in the original Japanese.'
DRAFT_DISCLOSURE = 'Draft English artwork (unreviewed): development preview, shown only when draft art is enabled.'
# Members a settings service binds, with the preference keys they fill today
# (services/user_preferences_service.gd language section).
PREFERENCE_KEYS = {'ui_text': ['ui_text_manifest', 'ui_text_sha256'],
                   'dialogue': ['dialogue_manifest', 'dialogue_sha256'],
                   'images': ['text_manifest', 'text_sha256'],
                   'draft_images': ['text_manifest', 'text_sha256']}
NAME_CONTROLS = {0x47, 0x50}  # 'G' and 'P' expand to names in sndisp
ROW_BREAK = 0x20
# Mirrors port/godot/presentation/prologue_text_presenter.gd translation_layout()
# for the original speaker label: English body rows run from x=34 to x=286 and
# the textbox holds the speaker row plus two body rows. A word is placed after
# a 4 px gap; a word wider than a row breaks inside the word. Godot's
# run_english_bundle_open.gd checks every bundled line against the presenter.
TEXTBOX_LEFT = 34
TEXTBOX_RIGHT = 286
TEXTBOX_BODY_ROWS = 2
WORD_GAP = 4
SPACE_CODE = 0x8140
# prologue_text_presenter.gd _latin_codes(): A-Z, a-z, 0-9 and this punctuation.
PRESENTER_PUNCTUATION = {' ': 0x8140, ',': 0x8143, '.': 0x8144, ':': 0x8146, ';': 0x8147, '?': 0x8148,
                         '!': 0x8149, "'": 0x8166, '\u2019': 0x8166, '"': 0x8168, '-': 0x817C, '\u2013': 0x817C,
                         '(': 0x8169, ')': 0x816A, '[': 0x816D, ']': 0x816E, '/': 0x815E, '+': 0x817B, '=': 0x8181}


class BundleError(ValueError):
    """An input is present but breaks its pin or contract."""


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _json(value) -> bytes:
    return json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(',', ':')).encode('utf-8') + b'\n'


def _member(stage: Path, relative: str, schema: str, disclosure: str, coverage: dict, **extra) -> dict:
    data = (stage / relative).read_bytes()
    return {'path': relative, 'sha256': _sha(data), 'size': len(data), 'schema': schema,
            'disclosure': disclosure, 'coverage': coverage, **extra}


def _jsonl(path: Path) -> list[dict]:
    return [json.loads(line) for line in path.read_bytes().splitlines() if line.strip()]


class DialogueFont(text_pack.FontCheck):
    """FontCheck plus the presenter's translation advance per code unit."""

    def advance(self, code: int) -> int | None:
        """prologue_text_presenter.gd _latin_metrics().y: ink width + 2, 4 for space/blank; None if absent."""
        if code == SPACE_CODE:
            return WORD_GAP
        found = text_pack.font_reference(code.to_bytes(2, 'big'), self.metadata)
        if found['status'] != 'glyph':
            return None
        record = self.bank[found['glyph_index'] * 18:found['glyph_index'] * 18 + 18]
        columns = [bit % 12 for bit in range(144) if (record[bit // 8] >> (7 - bit % 8)) & 1]
        return max(columns) - min(columns) + 3 if columns else WORD_GAP


def presenter_code(character: str) -> int | None:
    """Code unit prologue_text_presenter.gd _latin_codes() gives one character, or None."""
    value = ord(character)
    if 0x41 <= value <= 0x5A:
        return 0x8260 + value - 0x41
    if 0x61 <= value <= 0x7A:
        return 0x8281 + value - 0x61
    if 0x30 <= value <= 0x39:
        return 0x824F + value - 0x30
    return PRESENTER_PUNCTUATION.get(character)


def fits_textbox(text: str, font) -> bool:
    """Whether translation_layout() places `text` under the original speaker label."""
    rows, cursor = 1, TEXTBOX_LEFT
    for word in text.split(' '):
        if not word:
            continue
        advances = [font.advance(presenter_code(character)) for character in word]
        if None in advances:
            return False
        if cursor > TEXTBOX_LEFT and cursor + WORD_GAP + sum(advances) > TEXTBOX_RIGHT:
            rows, cursor = rows + 1, TEXTBOX_LEFT
        elif cursor > TEXTBOX_LEFT:
            cursor += WORD_GAP
        for advance in advances:
            if cursor + advance > TEXTBOX_RIGHT:
                rows, cursor = rows + 1, TEXTBOX_LEFT
            if rows > TEXTBOX_BODY_ROWS:
                return False
            cursor += advance
    return rows <= TEXTBOX_BODY_ROWS


def source_controls(raw: bytes) -> set[int]:
    """Single-byte units of a Shift-JIS walk (row break, name tokens, other controls)."""
    found, index = set(), 0
    while index < len(raw):
        byte = raw[index]
        if 0x81 <= byte <= 0x9F or 0xE0 <= byte <= 0xFC:
            index += 2
            continue
        found.add(byte)
        index += 1
    return found


def text_layer(stage: Path, translations: Path, files: Path, font) -> dict:
    pack, coverage = text_pack.build(translations / 'returned-v3.jsonl', translations / 'supplemental.jsonl',
                                     translations / 'graphics/inventory_decisions.json', files, font)
    (stage / 'ui_text').mkdir()
    (stage / 'ui_text/text_layer.json').write_bytes(_json(pack))
    (stage / 'ui_text/coverage.json').write_bytes(json.dumps(coverage, indent=1, sort_keys=True).encode() + b'\n')
    # approved_records is the pack's entry count (every record is approved by
    # inventory_decisions text_approval; only representable ones are entries).
    counts = {'approved_records': coverage['representable'], 'representable_records': coverage['representable'],
              'records': coverage['records'], 'rejected': coverage['rejected'],
              'physical_verified': coverage['physical_verified']}
    return _member(stage, 'ui_text/text_layer.json', text_pack.SCHEMA, DISCLOSURE, counts,
                   coverage_report='ui_text/coverage.json')


def prologue_dialogue(stage: Path, translations: Path, installed: Path, files: Path, font, payload_pin: str) -> dict:
    manifest = json.loads((installed / 'packs/prologue_text/manifest.json').read_text())
    if manifest.get('schema') != PROLOGUE_SCHEMA or manifest.get('payload') != 'prologue.json':
        raise BundleError('installed prologue text manifest is not supported')
    raw_payload = (installed / 'packs/prologue_text/prologue.json').read_bytes()
    if _sha(raw_payload) != payload_pin or manifest.get('sha256') != payload_pin:
        raise BundleError('installed prologue payload differs from the pinned source')
    payload = json.loads(raw_payload)
    container = (files / PROLOGUE_CONTAINER).read_bytes()
    records = {}
    for row in _jsonl(translations / 'returned-v3.jsonl'):
        if row['id'].startswith(PROLOGUE_CONTAINER + ':'):
            records[row['id']] = row
    entries, reasons, statuses, overflow = {}, collections.Counter(), collections.Counter(), []
    for line in payload['lines']:
        storage = line['storage']
        raw = bytes.fromhex(storage['raw_hex'])
        end = raw.find(b'\0')
        if end < 0:
            raise BundleError('prologue line has no terminator: %s' % line['id'])
        raw = raw[:end + 1]
        offset = storage['offset']
        if container[offset:offset + len(raw)] != raw:
            raise BundleError('prologue source bytes differ from NAME_ENT.EXN: %s' % line['id'])
        record = records.get('%s:%08x:%s' % (PROLOGUE_CONTAINER, offset, _sha(raw)[:12]))
        text = record.get('target_text') if record else None
        controls = source_controls(raw[:-1])
        reason = None
        if record is None:
            reason = 'no_record'
        elif not isinstance(text, str) or not text.strip():
            reason = 'no_translation'
        elif controls & NAME_CONTROLS:
            reason = 'source_name_token'
        elif controls - {ROW_BREAK}:
            reason = 'source_single_byte_control'
        elif record.get('protected_tokens') or '[[' in text:
            reason = 'protected_marker'
        elif text_pack.PRINTF.search(text):
            reason = 'format_directive'
        else:
            for character in text:
                code = text_pack.code_for(character)
                if code is None or presenter_code(character) != code or not font.supported(code):
                    reason = 'unsupported_character'
                    break
            else:
                if not fits_textbox(text, font):
                    reason = 'layout_overflow'
                    overflow.append(str(line['line_id']))
        if reason:
            reasons[reason] += 1
            continue
        statuses[str(record.get('review_status', ''))] += 1
        entries[str(line['line_id'])] = {'speaker': '', 'text': text}
    if not entries:
        raise BundleError('no prologue line is representable')
    pack = {'schema': DIALOGUE_SCHEMA, 'language': 'en', 'source_payload_sha256': payload_pin,
            'entries': dict(sorted(entries.items(), key=lambda item: int(item[0])))}
    (stage / 'dialogue').mkdir()
    (stage / 'dialogue/prologue.json').write_bytes(_json(pack))
    coverage = {'lines': len(entries), 'source_lines': len(payload['lines']), 'rejected': dict(sorted(reasons.items())),
                'review_status': dict(sorted(statuses.items())), 'speaker': 'original Japanese speaker label kept',
                'layout_overflow_line_ids': overflow}
    return _member(stage, 'dialogue/prologue.json', DIALOGUE_SCHEMA, DISCLOSURE, coverage,
                   source_payload_sha256=payload_pin)


def _pack_coverage(manifest: dict) -> dict:
    kinds = collections.Counter(row.get('kind', 'frame') for row in manifest['assets'])
    return {'assets': len(manifest['assets']), 'by_kind': dict(sorted(kinds.items())),
            'approved': sum(row.get('style') == 'approve' for row in manifest['assets'])}


def _inventory_left_japanese(translations: Path) -> int:
    """Catalogued artwork items without reviewed English (translation_inventory.json)."""
    path = translations / 'graphics/translation_inventory.json'
    if not path.is_file():
        return 0
    items = json.loads(path.read_text())['summary']['artwork_items']
    return items['total'] - items['by_status'].get('english-reviewed', 0)


def approved_images(stage: Path, snapshot: Path, translations: Path) -> dict:
    report = assets.audit(snapshot)
    # The snapshot holds frame rows only, so the established v1 pack is used.
    assets.build_pack(snapshot, stage / 'images', include_drafts=False)
    manifest = json.loads((stage / 'images/manifest.json').read_text())
    if manifest['development_drafts'] or any(row['style'] != 'approve' for row in manifest['assets']):
        raise BundleError('approved image pack holds a draft')
    coverage = {**_pack_coverage(manifest), 'reviewed_images': len(manifest['assets']),
                'original_japanese_images': _inventory_left_japanese(translations),
                'snapshot_current_assets': report['known_current_assets'],
                'snapshot_approved_exact': report['approved_exact'], 'snapshot_mapped': report['mapped'],
                'complete_catalog': False}
    return _member(stage, 'images/manifest.json', manifest['schema'], ART_DISCLOSURE, coverage, development_drafts=False)


def draft_images(stage: Path, snapshot: Path, sources: list[Path]) -> dict:
    report = assets.audit(snapshot)
    selected = [row for row in report['assets'] if row['mapping_complete']]
    rows = [{**row, 'kind': 'frame', 'output': 'images/' + row['output_sha256'] + '.png'} for row in selected]
    files = [assets.inside(snapshot, row['output']) for row in selected]
    inputs = [report['snapshot_sha256']]
    for source in sources:
        manifest_bytes = (source / 'manifest.json').read_bytes()
        manifest = json.loads(manifest_bytes)
        if manifest.get('schema') not in draft_pack.KIND_DRAFTS:
            raise BundleError('draft source is not a page/bs draft directory: %s' % source.name)
        decoder = assets.pinned_decoder_identity()
        for asset in manifest['assets']:
            path = assets.inside(source, asset['output'])
            actual, size = assets.png_info(path)
            if actual != asset['output_sha256'] or size != asset['dimensions']:
                raise BundleError('draft PNG differs from its manifest: %s' % asset['id'])
            if asset.get('kind') == 'bs' and asset.get('decoder_identity_sha256') != decoder:
                raise BundleError('MDEC draft was decoded by another decoder identity')
            row = {**asset, 'style': 'unreviewed', 'output': 'drafts/' + actual + '.png'}
            row.pop('source_png', None)
            assets.check_v2_row(row)
            rows.append(row)
            files.append(path)
        inputs.append(_sha(manifest_bytes))
    snapshot_sha = _sha(''.join(inputs).encode())
    schema = assets.PACK_V2 if any(row.get('kind', 'frame') != 'frame' for row in rows) else assets.PACK_V1
    if schema == assets.PACK_V1:
        rows = [{key: value for key, value in row.items() if key != 'kind'} for row in rows]
    assets.write_pack(stage / 'draft_images', rows, files, schema=schema, development_drafts=True,
                      snapshot_sha256=snapshot_sha)
    manifest = json.loads((stage / 'draft_images/manifest.json').read_text())
    coverage = {**_pack_coverage(manifest), 'draft_images': sum(row.get('style') != 'approve' for row in manifest['assets'])}
    return _member(stage, 'draft_images/manifest.json', schema, DRAFT_DISCLOSURE, coverage,
                   development_drafts=True, opt_in='allow_drafts', default_enabled=False)


def build(output: Path, *, translations: Path | None = None, files: Path | None = None, installed: Path | None = None,
          with_drafts: bool = False, draft_sources=(), font=None, prologue_payload_sha: str = PROLOGUE_PAYLOAD_SHA) -> dict:
    """Build the bundle into the new directory `output`; returns the bundle manifest."""
    translations = Path(translations or ROOT / 'translations/en')
    output = assets.new_output(Path(output))
    stage = output.with_name('.' + output.name + '.partial')
    if stage.exists() or stage.is_symlink():
        raise BundleError('a partial bundle build is in the way: ' + stage.name)
    stage.mkdir(parents=True)
    members, omitted = {}, {}
    try:
        executable = files / text_pack.EXECUTABLE if files is not None else None
        if files is None or not executable.is_file():
            omitted['ui_text'] = omitted['dialogue'] = 'original extracted files are unavailable'
        else:
            if font is None:
                data = executable.read_bytes()
                if _sha(data) != text_pack.FONT_PROFILE['sha256']:
                    raise BundleError('original executable differs from the verified release')
                font = DialogueFont(data)
            members['ui_text'] = text_layer(stage, translations, files, font)
            prologue = installed / 'packs/prologue_text/manifest.json' if installed is not None else None
            if prologue is None or not prologue.is_file():
                omitted['dialogue'] = 'installed prologue text pack is unavailable'
            elif not (files / PROLOGUE_CONTAINER).is_file():
                omitted['dialogue'] = 'original NAME_ENT.EXN is unavailable'
            else:
                members['dialogue'] = prologue_dialogue(stage, translations, installed, files, font, prologue_payload_sha)
        snapshot = translations / 'graphics/artwork'
        if not (snapshot / 'manifest.json').is_file():
            omitted['images'] = 'artwork snapshot is unavailable'
        else:
            members['images'] = approved_images(stage, snapshot, translations)
        if with_drafts and 'images' in members:
            members['draft_images'] = draft_images(stage, snapshot, [Path(source) for source in draft_sources])
        elif with_drafts:
            omitted['draft_images'] = 'the approved image pack is unavailable'
        inputs = {name: _sha((translations / name).read_bytes()) for name in
                  ('returned-v3.jsonl', 'supplemental.jsonl', 'graphics/inventory_decisions.json', 'manifest.json',
                   'graphics/translation_inventory.json') if (translations / name).is_file()}
        if 'images' in members:
            inputs['graphics/artwork/manifest.json'] = _sha((snapshot / 'manifest.json').read_bytes())
        bundle = {'schema': SCHEMA, 'language': 'en', 'generator': GENERATOR, 'label': 'English (AI translation)',
                  'disclosure': DISCLOSURE,
                  'disclosure_details': {'ai_translation': True, 'human_reviewed': False, 'voice': 'ja'},
                  'inputs': inputs, 'members': members,
                  'omitted': [{'member': name, 'reason': reason} for name, reason in sorted(omitted.items())],
                  'preference_keys': {key: value for key, value in PREFERENCE_KEYS.items() if key in members},
                  'human_approval': False}
        (stage / BUNDLE_FILE).write_bytes(_json(bundle))
        for member in members.values():
            if _sha((stage / member['path']).read_bytes()) != member['sha256']:
                raise BundleError('member changed while building: ' + member['path'])
        os.rename(stage, output)
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise
    return bundle


def verify(bundle_dir: Path) -> dict:
    """Re-hash every member of a built bundle; raises BundleError on any mismatch."""
    bundle = json.loads((bundle_dir / BUNDLE_FILE).read_text())
    if bundle.get('schema') != SCHEMA:
        raise BundleError('not an English bundle')
    for name, member in bundle['members'].items():
        path = assets.inside(bundle_dir, member['path'])
        if not path.is_file() or _sha(path.read_bytes()) != member['sha256']:
            raise BundleError('bundle member hash differs: ' + name)
    return bundle


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('--output', type=Path, required=True, help='new directory under .asset-cache')
    parser.add_argument('--files', type=Path, help='original extracted disc files root (disc/files)')
    parser.add_argument('--installed', type=Path, help='installed content bundle (for the prologue text pack)')
    parser.add_argument('--translations', type=Path, default=ROOT / 'translations/en')
    parser.add_argument('--with-drafts', action='store_true', help='also build the opt-in draft image pack')
    parser.add_argument('--draft-source', type=Path, action='append', default=[], help='page/bs draft directory for the draft pack')
    parser.add_argument('--verify', action='store_true', help='re-hash an existing bundle at --output instead of building')
    args = parser.parse_args(argv)
    try:
        if args.verify:
            bundle = verify(args.output)
        else:
            if args.draft_source and not args.with_drafts:
                parser.error('--draft-source needs --with-drafts')
            bundle = build(args.output, translations=args.translations, files=args.files, installed=args.installed,
                           with_drafts=args.with_drafts, draft_sources=args.draft_source)
    except (BundleError, text_pack.PackError, OSError, ValueError, KeyError) as failure:
        print('English bundle failed: %s' % failure, file=sys.stderr)
        return 1
    summary = {name: {'sha256': member['sha256'][:16], **{k: v for k, v in member['coverage'].items() if isinstance(v, int)}}
               for name, member in bundle['members'].items()}
    print(json.dumps({'bundle': str(args.output / BUNDLE_FILE), 'members': summary, 'omitted': bundle['omitted']}, sort_keys=True))
    return 0


if __name__ == '__main__':
    sys.exit(main())
