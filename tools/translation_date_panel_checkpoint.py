#!/usr/bin/env python3
"""T-39004: publish the English date panel drafts the user chose as a checkpoint.

User style decision (chat, 2026-10-30): the date panel reads "95 YR / 4 MO /
4 DAY" (the frame-60 captions alternate) with two-letter weekdays, set A (Mo Tu
We Th Fr Sa Su). That approves the style only: every asset stays a draft with
`human_approved: false` until the art itself is reviewed.

Inputs are the local, ignored draft outputs:

* `--frame60`: the MAINROM port-made draft directory whose manifest lists the
  `mainrom-alt-0060-captions.*` alternates (all six palette variants);
* `--weekdays`: tools/translation_weekday_drafts.py output; only set
  `two-letter` is taken (35 PNGs, all palette groups);
* `--hud-manifest`: the installed native command HUD manifest, used to find
  which of frame 60's 13 parts the captions edit and to cut exactly those parts;
* `--original-mainrom`: the original MAINROM frame export
  (.asset-cache/translation-assets/original-mainrom-v1), used to prove that no
  palette variant changes anything outside those parts.

Frame 60 is committed part by part. Each draft is a composite of the whole
40x139 date pillar, and most of that is original art: the border, the brackets
and the weekday cell. Only the three 16x16 kanji cells change. These are parts
3, 4 and 5, where 年 月 日 become YR MO DAY. So the checkpoint holds one ledger
asset per palette variant and edited part. Each PNG is that part as it is
replaced at run time: the caption texels only, with the kanji cleared. No
unedited part and no composite is committed (CODING_STANDARDS section 10).
`--preview` rebuilds the base composite locally from the installed HUD when a
preview is needed.

The month-name form of frame 60 (`mainrom-draft-0060.*` with the
`mainrom-port-month-*` sprites) and the three-letter weekday set are not
published. Older weekday drafts in the committed ledgers (T39004-pass2,
T39004-pass3) are listed under `supersedes`, so the inventory stops counting
them as candidates. Approved historical weekday images keep their approval; the
development pack replaces them only where a ledger asset names their output
hash in `supersedes_output_sha256`.

Writes a new checkpoint directory (ledger.json and one revision directory with
the PNGs and manifest.json). Run in Docker (tokimemo-content: Pillow).
"""
import argparse
import io
import json
from pathlib import Path
import shutil
import sys

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT / 'tools') not in sys.path:
    sys.path.insert(0, str(ROOT / 'tools'))
from translation_assets import audit, digest, encoded, inside, png_info  # noqa: E402
import translation_draft_pack as draft_pack  # noqa: E402

REVISION = 'date-panel-drafts-v1'
TICKET = 'T-39004'
SNAPSHOT = ROOT / 'translations/en/graphics/artwork'
CHECKPOINTS = SNAPSHOT / 'checkpoints'
FRAME60_PREFIX = 'mainrom-alt-0060-captions.'
WEEKDAY_SET = 'two-letter'
WEEKDAY_FRAMES = range(82, 89)
DATE_PANEL_FRAME = 60
FRAME60_PART_TEXT = {3: 'YR', 4: 'MO', 5: 'DAY'}
STYLE_DECISION = {
    'date': '2026-10-30', 'approver': 'user (project owner)', 'source': 'chat instruction',
    'words': 'Two letter and I like 95 yr 4 mo 4 day',
    'decision': 'English date panel: frame-60 captions "95 YR / 4 MO / 4 DAY" with two-letter weekdays, set A '
                '(Mo Tu We Th Fr Sa Su).',
    'scope': 'style only; the art is not human-approved and every asset stays human_approved false'}
PUBLICATION = ('User style choice 2026-10-30 (chat): two-letter weekdays, 95 YR / 4 MO / 4 DAY; '
               'style only, no art approval.')
ORIGINAL_NOTE = ('frame 60 is committed as its three edited 16x16 parts (caption texels only, kanji cleared); '
                 'the border, the brackets, the weekday cell and the other ten parts are not included')
DROPPED = [
    {'form': 'frame-60 month-name ("95 / Apr / 4") with the mainrom-port-month-* sprites',
     'reason': 'not the chosen style; not published'},
    {'form': 'weekday set B, three-letter (Mon Tue ...)',
     'reason': 'not the chosen style; its letters abut inside the bracket; not published'}]


def _verified(directory, record):
    path = inside(directory, record['output'])
    actual, size = png_info(path)
    if actual != record['output_sha256'] or size != record['dimensions']:
        raise ValueError('draft PNG differs from its manifest: ' + record['id'])
    return path


def _hud_frame(hud_manifest, frame, palette):
    hud = json.loads(hud_manifest.read_bytes())
    if hud.get('schema') != draft_pack.HUD_SCHEMA:
        raise ValueError('not a native command HUD manifest')
    return hud, hud['frames'].get('%d:%s' % (frame, palette))


def _rgba(path):
    from PIL import Image
    return Image.open(path).convert('RGBA')


def _blank(width, height):
    from PIL import Image
    return Image.new('RGBA', (width, height), (0, 0, 0, 0))


def png_bytes(image):
    buffer = io.BytesIO()
    image.save(buffer, 'PNG', optimize=False, compress_level=9)
    return buffer.getvalue()


def original_frame(original, record):
    """The original MAINROM export of a draft's frame and palette, checked against the draft's source hash."""
    path = inside(original, 'mainrom.frame-%03d.%s.png' % (record['frame'], record['palette_id']))
    if digest(path.read_bytes()) != record['source_png_sha256']:
        raise ValueError('original MAINROM frame differs from the draft source: ' + record['id'])
    return _rgba(path)


def outside_parts_unchanged(draft, original, rects, label):
    """Refuse a draft that changes any pixel outside the edited part rectangles."""
    if draft.size != original.size:
        raise ValueError('draft size differs from the original frame: ' + label)
    covered = set()
    for left, top, width, height in rects:
        covered.update((x, y) for y in range(top, top + height) for x in range(left, left + width))
    for y in range(draft.height):
        for x in range(draft.width):
            if (x, y) not in covered and draft.getpixel((x, y)) != original.getpixel((x, y)):
                raise ValueError('%s changes pixel (%d, %d) outside the edited parts' % (label, x, y))


def frame60_parts(frame60, captions, hud_manifest, original):
    """Cut every captions variant into its edited parts.

    The base variant is split against the installed HUD frame
    (translation_draft_pack.split_frame), which gives the edited parts and
    their images as replaced at run time. A replaced part holds only caption
    texels, so each other variant takes its own texels at the same places.
    Every variant must equal its original MAINROM export outside the edited
    parts. Returns (edited part indices, the base frame's part hashes, part
    rectangles, {palette: {part: PNG bytes}}).
    """
    _, record = _hud_frame(hud_manifest, DATE_PANEL_FRAME, 'base')
    parts = draft_pack.hud_frame_parts(hud_manifest.parent, record)
    size = record['bounds'][2:]
    base = next(r for r in captions if r['palette_id'] == 'base')
    draft = _rgba(_verified(frame60, base))
    source = draft_pack.compose_parts(parts, size)
    changed = set()
    for y in range(size[1]):
        for x in range(size[0]):
            if draft.getpixel((x, y)) != source.getpixel((x, y)):
                changed.update(parts[n][0] for n in draft_pack._covering(parts, x, y))
    edited = sorted(changed)
    images = draft_pack.split_frame(draft, parts, size, edited)
    by_index = {p[0]: p for p in parts}
    rects = {n: [by_index[n][1], by_index[n][2], by_index[n][3].width, by_index[n][3].height] for n in edited}
    masks = {n: [(x, y) for y in range(images[n].height) for x in range(images[n].width) if images[n].getpixel((x, y))[3]]
             for n in edited}
    out = {}
    for record in captions:
        variant = _rgba(_verified(frame60, record))
        outside_parts_unchanged(variant, original_frame(original, record), rects.values(), record['id'])
        cut = {}
        for n in edited:
            left, top, width, height = rects[n]
            if record is base:
                part = images[n]
            else:
                part = _blank(width, height)
                for x, y in masks[n]:
                    part.putpixel((x, y), variant.getpixel((left + x, top + y)))
            if any(a not in (0, 255) for a in part.getchannel('A').getdata()):
                raise ValueError('part alpha is not binary: %s part %d' % (record['id'], n))
            cut[n] = png_bytes(part)
        out[record['palette_id']] = cut
    return edited, [p[4] for p in parts], rects, out


def preview(checkpoint, hud_manifest, palette='base'):
    """The frame-60 composite rebuilt from the installed HUD and the committed parts (local preview only)."""
    from PIL import Image
    _, record = _hud_frame(hud_manifest, DATE_PANEL_FRAME, palette)
    if record is None:
        raise ValueError('the installed HUD does not draw frame 60 in palette ' + palette)
    parts = draft_pack.hud_frame_parts(hud_manifest.parent, record)
    ledger = json.loads((checkpoint / 'ledger.json').read_bytes())
    images = {}
    for asset in ledger['assets']:
        if asset['frame'] != DATE_PANEL_FRAME or asset['palette_id'] != palette:
            continue
        if asset.get('frame_parts_sha256', {}).get(palette) != [p[4] for p in parts]:
            raise ValueError('installed HUD frame parts differ from the ledger: ' + asset['id'])
        path = inside(checkpoint, asset['path'])
        if digest(path.read_bytes()) != asset['output_sha256']:
            raise ValueError('checkpoint PNG differs from its ledger: ' + asset['id'])
        images[asset['part_index']] = Image.open(path).convert('RGBA')
    if not images:
        raise ValueError('the checkpoint has no frame-60 parts for palette ' + palette)
    return draft_pack.compose_parts(parts, record['bounds'][2:], images)


def superseded(ledger_paths, frames):
    """Active candidates of committed ledgers drafted for `frames`."""
    out = []
    for path in ledger_paths:
        ledger = json.loads(path.read_text())
        if any(asset.get('revision') == REVISION for asset in ledger['assets']):
            continue  # this checkpoint itself (a rebuild)
        for asset in ledger['assets']:
            if asset.get('frame') in frames:
                out.append({'ledger': str(path.relative_to(ROOT)), 'id': asset['id'], 'revision': asset['revision'],
                            'output_sha256': asset['output_sha256']})
    return sorted(out, key=lambda r: (r['ledger'], r['id']))


def approved_outputs(snapshot):
    """{(frame, palette, source sha): output sha} of the approved, mapped snapshot images."""
    report = audit(snapshot)
    return {(a['frame'], a['palette'], a['source_png_sha256']): a['output_sha256']
            for a in report['assets'] if a['mapping_complete'] and a['style'] == 'approve'}


def build(frame60, weekdays, hud_manifest, original, output, snapshot=SNAPSHOT, ledgers=None):
    output = Path(output).absolute()
    if output.exists():
        raise ValueError('checkpoint directory must be new')
    frame_manifest_bytes = (frame60 / 'manifest.json').read_bytes()
    frame_manifest = json.loads(frame_manifest_bytes)
    captions = sorted((r for r in frame_manifest['alternates'] if r['id'].startswith(FRAME60_PREFIX)),
                      key=lambda r: r['palette_id'])
    if len(captions) != 6 or any(r['frame'] != DATE_PANEL_FRAME or r['human_approved'] is not False for r in captions):
        raise ValueError('expected six unapproved frame-60 captions variants')
    week_manifest_bytes = (weekdays / 'manifest.json').read_bytes()
    week_manifest = json.loads(week_manifest_bytes)
    week_ledger_bytes = (weekdays / 'ledger.json').read_bytes()
    week_ledger_ids = {a['id'] for a in json.loads(week_ledger_bytes)['assets']}
    days = sorted((r for r in week_manifest['assets'] if r['set'] == WEEKDAY_SET), key=lambda r: (r['frame'], r['palette_id']))
    if len(days) != 35 or {r['frame'] for r in days} != set(WEEKDAY_FRAMES) or any(r['human_approved'] is not False for r in days) \
            or not {r['id'] for r in days} <= week_ledger_ids:
        raise ValueError('expected 35 unapproved two-letter weekday drafts')
    base = next(r for r in captions if r['palette_id'] == 'base')
    parts, frame_shas, rects, cut = frame60_parts(frame60, captions, hud_manifest, original)
    if parts != sorted(base['overlay_clut_parts']) or parts != sorted(FRAME60_PART_TEXT):
        raise ValueError('edited parts %s differ from the draft overlay parts %s' % (parts, base['overlay_clut_parts']))
    hud = json.loads(hud_manifest.read_bytes())
    approved = approved_outputs(snapshot)
    ledger_paths = ledgers if ledgers is not None else sorted(CHECKPOINTS.glob('*/ledger.json'))
    stage = output.with_name('.' + output.name + '.partial')
    if stage.exists():
        raise ValueError('a partial checkpoint is in the way: ' + stage.name)
    (stage / REVISION / 'frame-0060').mkdir(parents=True)
    (stage / REVISION / 'weekday-two-letter').mkdir()
    try:
        # (record, path below the checkpoint, ledger fields) per asset; frame 60 one per variant and part.
        assets, frame60_records = [], []
        for record in captions:
            runtime = hud['frames'].get('%d:%s' % (DATE_PANEL_FRAME, record['palette_id'])) is not None
            stem = Path(record['output']).stem
            written = []
            for n in parts:
                data = cut[record['palette_id']][n]
                relative = 'frame-0060/%s-part%02d.png' % (stem, n)
                (stage / REVISION / relative).write_bytes(data)
                written.append({'part_index': n, 'checkpoint_path': relative, 'output_sha256': digest(data)})
                entry = {'id': 'mainrom-alt-0060-captions-part%02d.%s' % (n, record['palette_id']),
                         'frame': DATE_PANEL_FRAME, 'palette_id': record['palette_id'],
                         'palette_ids': record['palette_ids'], 'text': FRAME60_PART_TEXT[n],
                         'frame_text': record['date_form']['reads'], 'part_index': n, 'part_rect': rects[n],
                         'edited_parts': parts, 'runtime_applicable': runtime,
                         'composite_output_sha256': record['output_sha256'],
                         'reason': 'Frame-60 captions alternate, the chosen date panel style: part %d (%s) as '
                                   'replaced at run time, caption texels only; port-made caps5 lettering; '
                                   'unreviewed, no approval transfers.' % (n, FRAME60_PART_TEXT[n])}
                if runtime:
                    entry['frame_parts_sha256'] = {record['palette_id']: frame_shas}
                    entry['source_part_sha256'] = frame_shas[n]
                part_record = {'dimensions': rects[n][2:], 'output_sha256': digest(data),
                               'source_container_sha256': record['source_container_sha256'],
                               'source_png_sha256': record['source_png_sha256']}
                assets.append((part_record, relative, entry))
            frame60_records.append({**{k: v for k, v in record.items() if k != 'output'},
                                    'composite_committed': False, 'parts': written})
        weekday_records = []
        for record in days:
            path = _verified(weekdays, record)
            relative = 'weekday-two-letter/' + Path(record['output']).name
            shutil.copyfile(path, stage / REVISION / relative)
            drawn = [p for p in record['palette_ids'] if '%d:%s' % (record['frame'], p) in hud['frames']]
            entry = {'id': record['id'], 'frame': record['frame'], 'palette_id': record['palette_id'],
                     'palette_ids': record['palette_ids'], 'text': record['text'], 'runtime_applicable': bool(drawn),
                     'reason': 'Two-letter weekday (set A), the chosen style; port-made lettering inside the bracket; '
                               'unreviewed, no approval transfers.'}
            supersedes = {p: approved[(record['frame'], p, record['source_png_sha256'])] for p in drawn
                          if (record['frame'], p, record['source_png_sha256']) in approved}
            if supersedes:
                entry['supersedes_output_sha256'] = supersedes
            assets.append((record, relative, entry))
            weekday_records.append({**record, 'checkpoint_path': relative})
        manifest = {'schema': 'tokimemo.date-panel-drafts.v1', 'ticket': TICKET, 'human_approved': False,
                    'style_decision': STYLE_DECISION, 'dropped': DROPPED,
                    'sources': {'frame60_manifest_sha256': digest(frame_manifest_bytes),
                                'weekday_manifest_sha256': digest(week_manifest_bytes),
                                'weekday_ledger_sha256': digest(week_ledger_bytes),
                                'weekday_generator': 'tools/translation_weekday_drafts.py',
                                'hud_manifest_sha256': digest(hud_manifest.read_bytes())},
                    'faces': {'caps5': frame_manifest['faces']['caps5'], WEEKDAY_SET: week_manifest['faces'][WEEKDAY_SET]},
                    'weekday_geometry': week_manifest['geometry'], 'weekday_source_style': week_manifest['source_style'],
                    'frame60_edited_parts': {'parts': parts, 'part_count': len(frame_shas), 'rects': [rects[n] for n in parts],
                                             'rule': 'every changed pixel lies in exactly one listed part; the other '
                                                     'parts stay the original and are not committed'},
                    'frame60_drafts': frame60_records, 'assets': weekday_records}
        manifest_bytes = encoded(manifest) + b'\n'
        (stage / REVISION / 'manifest.json').write_bytes(manifest_bytes)
        rows = []
        for record, relative, entry in assets:
            rows.append({**entry, 'dimensions': record['dimensions'], 'human_approved': False,
                         'manifest_sha256': digest(manifest_bytes), 'output_sha256': record['output_sha256'],
                         'path': REVISION + '/' + relative, 'port_made': True, 'revision': REVISION,
                         'source_container_sha256': record['source_container_sha256'],
                         'source_png_sha256': record['source_png_sha256'], 'status': 'draft'})
        ledger = {'schema': draft_pack.CHECKPOINT_SCHEMA, 'ticket': TICKET, 'runtime_hud': True,
                  'assets': rows, 'candidate_assets': len(rows), 'original_images_included': False,
                  'original_images_note': ORIGINAL_NOTE,
                  'complete_translation_inventory': False, 'separate_reviewed_revisions': 0,
                  'style_decision': STYLE_DECISION, 'dropped': DROPPED,
                  'supersedes': superseded(ledger_paths, set(WEEKDAY_FRAMES) | {DATE_PANEL_FRAME}),
                  'publication_authorization': PUBLICATION}
        (stage / 'ledger.json').write_bytes(encoded(ledger) + b'\n')
        stage.rename(output)
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise
    return {'checkpoint': str(output), 'assets': len(rows), 'edited_parts': parts,
            'supersedes': len(ledger['supersedes'])}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('--frame60', type=Path, help='MAINROM port-made draft directory')
    parser.add_argument('--weekdays', type=Path, help='tools/translation_weekday_drafts.py output')
    parser.add_argument('--hud-manifest', type=Path, required=True, help='installed packs/hud_manifest/manifest.json')
    parser.add_argument('--original-mainrom', type=Path, help='original MAINROM frame export directory')
    parser.add_argument('--output', type=Path, default=CHECKPOINTS / 'T39004-date-panel-v1')
    parser.add_argument('--preview', type=Path, help='instead of building, write the base frame-60 composite '
                        'rebuilt from --output and --hud-manifest to this new PNG')
    args = parser.parse_args(argv)
    try:
        if args.preview is not None:
            image = preview(args.output, args.hud_manifest)
            with args.preview.open('xb') as stream:
                stream.write(png_bytes(image))
            print(json.dumps({'preview': str(args.preview), 'size': list(image.size)}))
            return 0
        if args.frame60 is None or args.weekdays is None or args.original_mainrom is None:
            parser.error('building needs --frame60, --weekdays and --original-mainrom')
        print(json.dumps(build(args.frame60, args.weekdays, args.hud_manifest, args.original_mainrom, args.output)))
    except (OSError, ValueError, KeyError) as exc:
        print('date panel checkpoint failed: %s' % exc, file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
