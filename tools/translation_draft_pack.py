#!/usr/bin/env python3
"""T-39004: explicit development-only runtime pack for reviewing English drafts.

Frame drafts (NAME, OPTION) become kind-`frame` rows; `--schema-version 2`
writes them into a v2 pack, otherwise the v1 pack is unchanged. Page drafts
(tools/translation_filesel_drafts.py pages) and MDEC drafts
(tools/translation_mdec_drafts.py) carry their own `kind` and always produce a
v2 pack. Every pack here is `development_drafts: true`, every row is
`style: unreviewed`, and an agent review never becomes human approval.

HUD checkpoint drafts (`hud_checkpoint_rows`, used by the removed
tools/port/build_english_bundle.py) bind a committed checkpoint ledger to the
installed HUD manifest (tools/port/hud_content.py export). A single-part frame
becomes a kind-`frame` row keyed by its part PNG hash. A part of a multi-part
frame is committed on its own (ledger `part_index`; the checkpoint tool cuts it
with `split_frame`, so no unedited part is committed) and becomes one
kind-`frame_part` row; the frame's other parts stay the original. Each row
repeats the hashes of all the frame's part PNGs, so the runtime binds it only
to that exact frame. A checkpoint asset may be `approved` (`human_approved:
true`) only with an exact approve event for its output hash (latest per hash)
in the snapshot's style-decisions.jsonl; its row then carries `style: approve`
and stays in the development pack.
"""
import argparse
import io
import json
from pathlib import Path

from translation_assets import (PACK_V1, PACK_V2, check_v2_row, digest, encoded, inside, new_output,
                                pinned_decoder_identity, png_info, write_pack)

HUD_SCHEMA = 'tokimemo.native-command-hud.v1'
CHECKPOINT_SCHEMA = 'tokimemo.translation-checkpoint.v1'
MAINROM = 'CDROM/FUKUTEST/MAINROM.NBN'


def _rgba_png(path, expected_sha):
    from PIL import Image
    data = path.read_bytes()
    if digest(data) != expected_sha:
        raise ValueError('HUD part PNG differs from its manifest: ' + path.name)
    return Image.open(io.BytesIO(data)).convert('RGBA')


def hud_frame_parts(hud_root, record):
    """[(part_index, x, y, RGBA image, png sha256)] of one HUD frame, offsets relative to its bounds."""
    left, top = record['bounds'][:2]
    parts = []
    for part in record['parts']:
        image = _rgba_png(inside(hud_root, part['png']['path']), part['png']['sha256'])
        if list(image.size) != [part['width'], part['height']]:
            raise ValueError('HUD part size differs from its manifest')
        parts.append((part['part_index'], part['x'] - left, part['y'] - top, image, part['png']['sha256']))
    return parts


def _covering(parts, x, y):
    return [n for n, (_, px, py, image, _) in enumerate(parts)
            if px <= x < px + image.width and py <= y < py + image.height]


def compose_parts(parts, size, images=None):
    """Frame composite with part 0 on top (the convention of the frame-composite drafts)."""
    from PIL import Image
    images = images or {}
    out = Image.new('RGBA', tuple(size), (0, 0, 0, 0))
    for y in range(size[1]):
        for x in range(size[0]):
            for n in _covering(parts, x, y):
                index, px, py, image, _ = parts[n]
                pixel = images.get(index, image).getpixel((x - px, y - py))
                if pixel[3]:
                    out.putpixel((x, y), pixel)
                    break
    return out


def split_frame(draft, parts, size, edited_parts):
    """Cut a frame-composite draft into replacement images for `edited_parts` only.

    Every pixel where the draft differs from the original composite must be
    covered by exactly one part, and that part must be listed; a listed part
    with no changed pixel is refused. The result recomposes to the draft.
    """
    if draft.mode != 'RGBA' or list(draft.size) != list(size):
        raise ValueError('frame draft size differs from the HUD frame')
    if any(a not in (0, 255) for a in draft.getchannel('A').getdata()):
        raise ValueError('frame draft alpha is not binary')
    source = compose_parts(parts, size)
    by_index = {p[0]: p for p in parts}
    if not edited_parts or len(set(edited_parts)) != len(edited_parts) or not set(edited_parts) <= set(by_index):
        raise ValueError('edited parts are not parts of this frame')
    images = {index: by_index[index][3].copy() for index in edited_parts}
    touched = set()
    for y in range(size[1]):
        for x in range(size[0]):
            pixel = draft.getpixel((x, y))
            if pixel == source.getpixel((x, y)):
                continue
            covering = _covering(parts, x, y)
            if len(covering) != 1 or parts[covering[0]][0] not in images:
                raise ValueError('changed pixel (%d, %d) is not inside exactly one edited part' % (x, y))
            index, px, py, _, _ = parts[covering[0]]
            images[index].putpixel((x - px, y - py), pixel if pixel[3] else (0, 0, 0, 0))
            touched.add(index)
    if touched != set(edited_parts):
        raise ValueError('edited parts without a changed pixel: %s' % sorted(set(edited_parts) - touched))
    if compose_parts(parts, size, images).tobytes() != draft.tobytes():
        raise ValueError('split parts do not recompose to the draft')
    return images


def approved_hashes(history_path):
    """{output_sha256: approve event ids since that hash's last other style decision}."""
    standing = {}
    for line in history_path.read_bytes().splitlines():
        if line:
            event = json.loads(line)
            if event.get('category') == 'style' and isinstance(event.get('output_sha256'), str):
                if event.get('decision') == 'approve':
                    standing.setdefault(event['output_sha256'], set()).add(event.get('event_id'))
                else:
                    standing.pop(event['output_sha256'], None)
    return standing


def hud_checkpoint_rows(checkpoint, hud_manifest):
    """Development-pack rows for a checkpoint whose ledger sets `runtime_hud`.

    Binds each runtime-applicable ledger asset to the installed HUD manifest:
    a single-part frame by its part PNG hash (kind `frame`), one part of a
    multi-part frame by the hashes of every part of that frame (kind
    `frame_part`). Returns (rows, files, report). A ledger asset whose recorded source differs
    from the installed HUD fails; one whose frame/palette the HUD never draws
    is skipped and counted.
    """
    ledger_bytes = (checkpoint / 'ledger.json').read_bytes()
    ledger = json.loads(ledger_bytes)
    if ledger.get('schema') != CHECKPOINT_SCHEMA or not ledger.get('runtime_hud'):
        raise ValueError('checkpoint ledger is not a HUD draft checkpoint')
    hud_bytes = hud_manifest.read_bytes()
    hud = json.loads(hud_bytes)
    if hud.get('schema') != HUD_SCHEMA:
        raise ValueError('not a native command HUD manifest')
    container = hud['provenance']['inputs']['mainrom']['sha256']
    rows, files, skipped = [], [], []
    bound_parts = set()
    approvals = None
    for asset in ledger['assets']:
        approved = asset.get('status') == 'approved'
        if asset.get('human_approved') is not approved or asset.get('status') not in ('draft', 'approved'):
            raise ValueError('HUD checkpoint asset is neither an unapproved draft nor approved: ' + asset['id'])
        if approved:
            # Human approval counts only through an exact approve event (latest per hash) in the snapshot history.
            if approvals is None:
                approvals = approved_hashes(checkpoint.parent.parent / 'style-decisions.jsonl')
            if asset.get('human_approval_event') not in approvals.get(asset['output_sha256'], ()):
                raise ValueError('approved HUD checkpoint asset has no exact approve event: ' + asset['id'])
        if asset['source_container_sha256'] != container:
            raise ValueError('HUD checkpoint asset is bound to another MAINROM: ' + asset['id'])
        path = inside(checkpoint, asset['path'])
        actual, size = png_info(path)
        if actual != asset['output_sha256'] or size != asset['dimensions']:
            raise ValueError('checkpoint PNG differs from its ledger: ' + asset['id'])
        for palette in asset['palette_ids']:
            record = hud['frames'].get('%d:%s' % (asset['frame'], palette))
            if record is None or not asset.get('runtime_applicable'):
                skipped.append('%s:%s' % (asset['id'], palette))
                continue
            # Ledger ids end in their palette group's first palette; a row names its own.
            base = {'id': asset['id'].rsplit('.', 1)[0] + '.' + palette, 'source_container': MAINROM,
                    'source_container_sha256': container, 'frame': asset['frame'], 'palette': palette,
                    'style': 'approve' if approved else 'unreviewed', 'human_approved': approved, 'port_made': bool(asset.get('port_made')),
                    'checkpoint_ledger_sha256': digest(ledger_bytes), 'checkpoint_output_sha256': asset['output_sha256']}
            if asset.get('supersedes_output_sha256', {}).get(palette):
                base['supersedes_output_sha256'] = asset['supersedes_output_sha256'][palette]
            if len(record['parts']) == 1:
                part = record['parts'][0]
                if 'part_index' in asset or part['png']['sha256'] != asset['source_png_sha256'] \
                        or size != [part['width'], part['height']]:
                    raise ValueError('installed HUD frame differs from the ledger source: ' + asset['id'])
                rows.append({**base, 'kind': 'frame', 'dimensions': size, 'source_png_sha256': part['png']['sha256'],
                             'output': 'drafts/' + actual + '.png', 'output_sha256': actual})
                files.append(path)
                continue
            # One committed part of a multi-part frame; the other parts stay the original.
            shas = [part['png']['sha256'] for part in record['parts']]
            index = asset.get('part_index')
            if shas != asset.get('frame_parts_sha256', {}).get(palette):
                raise ValueError('installed HUD frame parts differ from the ledger: ' + asset['id'])
            if not isinstance(index, int) or isinstance(index, bool) or not 0 <= index < len(shas) \
                    or [p['part_index'] for p in record['parts']] != list(range(len(shas))) \
                    or asset.get('source_part_sha256') != shas[index] \
                    or size != [record['parts'][index]['width'], record['parts'][index]['height']]:
                raise ValueError('multi-part frame asset does not name one installed part: ' + asset['id'])
            key = (asset['frame'], palette, index)
            if key in bound_parts:
                raise ValueError('frame part given twice: ' + asset['id'])
            bound_parts.add(key)
            rows.append({**base, 'kind': 'frame_part', 'part_index': index, 'dimensions': size,
                         'source_png_sha256': shas[index], 'frame_parts_sha256': shas,
                         'output': 'drafts/' + actual + '.png', 'output_sha256': actual})
            files.append(path)
    for row in rows:
        check_v2_row(row)
    report = {'ledger_sha256': digest(ledger_bytes), 'hud_manifest_sha256': digest(hud_bytes),
              'rows': len(rows), 'skipped_not_drawn': sorted(skipped)}
    return rows, files, report

FRAME_DRAFTS = ('tokimemo.name-translation-drafts.v1', 'tokimemo.option-translation-drafts.v1')
KIND_DRAFTS = ('tokimemo.mdec-translation-drafts.v1', 'tokimemo.filesel-page-drafts.v1')
# Fields an agent review must repeat exactly for each selected asset.
REVIEW_IDENTITY = {
    'frame': ('frame', 'source_container_sha256', 'source_png_sha256', 'output_sha256'),
    'page': ('page', 'source_container_sha256', 'source_png_sha256', 'source_rgba_sha256', 'output_sha256', 'regions'),
    'bs': ('bs_index', 'source_container_sha256', 'decoder_identity_sha256', 'source_rgba_sha256', 'output_sha256', 'regions'),
}


def _review(agent_review, manifest_bytes):
    review_bytes = agent_review.read_bytes()
    review = json.loads(review_bytes)
    if review.get('schema') != 'tokimemo.agent-artwork-review.v1' or review.get('draft_manifest_sha256') != digest(manifest_bytes) or review.get('human_approval') is not False or review.get('development_only') is not True or review.get('decision') != 'source-style-and-layout-accepted':
        raise ValueError('agent review does not bind this development draft manifest')
    reviewed = {}
    for row in review.get('assets', []):
        if row['id'] in reviewed:
            raise ValueError('duplicate reviewed asset identity')
        reviewed[row['id']] = row
    if not reviewed:
        raise ValueError('agent review has no bounded assets')
    return review_bytes, review, reviewed


def build(source, output, palette=None, agent_review=None, schema_version=1):
    manifest_bytes = (source / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    draft_schema = manifest.get('schema')
    if draft_schema in FRAME_DRAFTS:
        if not palette:
            raise ValueError('frame drafts need --palette')
        schema = PACK_V2 if schema_version == 2 else PACK_V1
    elif draft_schema in KIND_DRAFTS:
        if palette:
            raise ValueError('page/bs drafts carry their own identity; --palette is not used')
        schema = PACK_V2
    else:
        raise ValueError('unsupported draft manifest')
    output = new_output(output)
    review_bytes = review = None
    reviewed = {}
    if agent_review is not None:
        review_bytes, review, reviewed = _review(agent_review, manifest_bytes)
    decoder = pinned_decoder_identity() if draft_schema == 'tokimemo.mdec-translation-drafts.v1' else None
    rows, sources = [], []
    for asset in manifest['assets']:
        kind = asset.get('kind', 'frame') if draft_schema in KIND_DRAFTS else 'frame'
        if review is not None:
            if asset['id'] not in reviewed:
                continue
            identity = reviewed.pop(asset['id'])
            if any(identity.get(key) != asset.get(key) for key in REVIEW_IDENTITY[kind]):
                raise ValueError('agent review source/output identity differs')
        path = inside(source, asset['output'])
        actual, size = png_info(path)
        if actual != asset['output_sha256'] or size != asset['dimensions']:
            raise ValueError('draft bytes changed')
        if kind == 'bs' and asset.get('decoder_identity_sha256') != decoder:
            raise ValueError('MDEC draft was decoded by another decoder identity')
        row = {**asset, 'style': 'unreviewed'}
        if draft_schema in FRAME_DRAFTS:
            row['palette'] = palette
        if schema == PACK_V2:
            row['kind'] = kind
            check_v2_row(row)
        rows.append(row)
        sources.append(path)
    if reviewed:
        raise ValueError('review contains assets absent from the draft manifest')
    metadata = {}
    if review_bytes is not None:
        metadata['agent_review'] = {'path': 'agent-review.json', 'sha256': digest(review_bytes), 'id': review['id'], 'human_approval': False}
    sha = write_pack(output, rows, sources, schema=schema, development_drafts=True,
                     snapshot_sha256=digest(manifest_bytes), metadata=metadata,
                     extra_files={'agent-review.json': review_bytes} if review_bytes is not None else None)
    return {'output': str(output), 'assets': len(rows), 'sha256': sha, 'schema': schema}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--palette', help='Palette id for frame drafts (NAME/OPTION); not used for page/bs drafts')
    parser.add_argument('--schema-version', type=int, choices=[1, 2], default=1, help='Frame drafts only; page/bs drafts are always v2')
    parser.add_argument('--agent-review', type=Path, help='Select only exact source/output identities covered by a separate bounded agent review; remains development-only')
    args = parser.parse_args()
    print(json.dumps(build(args.source, args.output, args.palette, args.agent_review, args.schema_version)))
