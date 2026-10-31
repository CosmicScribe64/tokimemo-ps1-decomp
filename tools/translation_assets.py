#!/usr/bin/env python3
"""T-39004: audit versioned English artwork and build an optional external pack.

This does not grant visual approval or establish actual-game coverage. Originals
are identified by source container/frame/palette and their exact PNG hash.
"""
from __future__ import annotations
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import struct

ROOT = Path(__file__).resolve().parents[1]
PACK_V1 = 'tokimemo.translated-image-pack.v1'
# v2 adds a `kind` per row: frame (the v1 row), page, bs and frame_part (one
# part of a multi-part HUD frame). The runtime reader was
# port/godot/services/translated_image_store.gd, removed with the port on 2026-10-31.
PACK_V2 = 'tokimemo.translated-image-pack.v2'
KINDS = ('frame', 'page', 'bs', 'frame_part')
MAX_REGIONS = 64
MAX_FRAME_PARTS = 64
# Pinned MDEC model identity (the removed port pinned the same value).
MDEC_MODEL_ID = 'tokimemo-mdec-fixed-point-v1'
MDEC_IMPLEMENTATIONS = {'image_mdec.py': '6513b1e6434bdd1c65c804e5d637ae3db1f4fa11f69dd80b495cf05a2f8828ec',
                        'image_mdec_model.py': '3b8e5611bf6f763171b8bf3d5a22269211048b0ec21dc342088e90ec043d2222'}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def encoded(value):
    return json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(',', ':')).encode()


def inside(root, relative):
    if not isinstance(relative, str) or not relative or '\\' in relative:
        raise ValueError('invalid relative path')
    parts = relative.split('/')
    if any(p in ('', '.', '..') for p in parts):
        raise ValueError('unsafe relative path')
    path = root
    for part in parts:
        path /= part
        if path.is_symlink():
            raise ValueError('symlink in artwork path')
    if not path.resolve().is_relative_to(root.resolve()):
        raise ValueError('artwork path escaped root')
    return path


def png_info(path):
    data = path.read_bytes()
    if len(data) > 16 * 1024 * 1024 or len(data) < 33 or data[:8] != b'\x89PNG\r\n\x1a\n' or data[12:16] != b'IHDR':
        raise ValueError('invalid or oversized PNG')
    return digest(data), list(struct.unpack('>II', data[16:24]))


def audit(snapshot_root):
    manifest = json.loads((snapshot_root / 'manifest.json').read_text())
    if manifest.get('schema') != 'tokimemo.translated-artwork-snapshot.v1':
        raise ValueError('unsupported artwork snapshot')
    history_bytes = (snapshot_root / 'style-decisions.jsonl').read_bytes()
    if digest(history_bytes) != manifest['review_history_sha256']:
        raise ValueError('review history identity differs')
    history = [json.loads(line) for line in history_bytes.splitlines() if line]
    latest = {}
    for event in history:
        if event.get('category') == 'style':
            latest[event['asset_id']] = event
    records, seen = [], set()
    for version in manifest['versions']:
        if not version.get('current'):
            continue
        for asset in version['assets']:
            identity = 'draft:' + asset['id']
            if identity in seen:
                raise ValueError('duplicate current asset identity')
            seen.add(identity)
            path = inside(snapshot_root, asset['output'])
            actual, size = png_info(path)
            if actual != asset['output_sha256'] or size != asset['dimensions']:
                raise ValueError('artwork bytes or dimensions differ: ' + identity)
            revision = digest(encoded([identity, asset['source_sha256'], actual]))
            event = latest.get(identity)
            exact = bool(event and event.get('revision') == revision and event.get('source_sha256') == asset['source_sha256'] and event.get('output_sha256') == actual)
            mapping = all(key in asset for key in ('source_container', 'source_container_sha256', 'frame', 'palette_id'))
            records.append({'id': identity, 'batch': version['batch_id'], 'revision': revision,
                'output': asset['output'], 'output_sha256': actual, 'dimensions': size,
                'source_png_sha256': asset['source_sha256'],
                'source_container': asset.get('source_container'), 'source_container_sha256': asset.get('source_container_sha256'),
                'frame': int(asset['frame']) if 'frame' in asset else None, 'palette': asset.get('palette_id'),
                'style': event['decision'] if exact else ('stale' if event else 'unreviewed'),
                'exact_review_event': event['event_id'] if exact else None,
                'mapping_complete': mapping, 'runtime_integrated': False, 'in_game_verified': False,
                'context': asset.get('context', []), 'layout': asset.get('layout'),
                'blockers': ([] if mapping else ['missing exact container/frame/palette mapping']) + ['actual-game verification pending']})
    return {'schema': 'tokimemo.translation-asset-audit.v1', 'snapshot_sha256': digest((snapshot_root / 'manifest.json').read_bytes()),
            'known_current_assets': len(records), 'approved_exact': sum(r['style'] == 'approve' for r in records),
            'mapped': sum(r['mapping_complete'] for r in records), 'in_game_verified': 0,
            'coverage_complete': False, 'limitations': ['snapshot is not a complete required-asset catalog', 'approval is not runtime acceptance'], 'assets': records}


def new_output(output):
    output = output.absolute()
    if output.exists() or output.is_symlink():
        raise ValueError('output must be a new immutable directory')
    output.resolve().relative_to((ROOT / '.asset-cache').resolve())
    # Reject a linked parent before creating any output.
    current = output.parent
    while current != current.parent:
        if current.is_symlink():
            raise ValueError('linked output parent')
        current = current.parent
    return output


def decoder_identity(model_id, implementations):
    """SHA-256 of the model id plus one `name=sha` line per implementation file, in name order.

    TranslatedImageStore.decoder_identity() computes the same value at runtime.
    """
    text = model_id + ''.join('\n%s=%s' % (name, implementations[name]) for name in sorted(implementations))
    return digest(text.encode())


def pinned_decoder_identity(root=ROOT):
    """Decoder identity of the pinned model; refuses if the implementation files drifted."""
    for name, expected in MDEC_IMPLEMENTATIONS.items():
        if digest((root / 'tools/assets' / name).read_bytes()) != expected:
            raise ValueError('MDEC implementation differs from the pinned model: ' + name)
    return decoder_identity(MDEC_MODEL_ID, MDEC_IMPLEMENTATIONS)


def rgba_sha256(image):
    """SHA-256 of 8-bit RGBA bytes, as Godot hashes Image.get_data() after convert(FORMAT_RGBA8)."""
    return digest(image.convert('RGBA').tobytes())


def _hash(value):
    return isinstance(value, str) and len(value) == 64 and all(c in '0123456789abcdef' for c in value)


def _index(value):
    return isinstance(value, int) and not isinstance(value, bool) and 0 <= value <= 65535


def check_regions(regions, dimensions):
    """Region rects [x, y, w, h], each inside the image; same rule as the runtime reader."""
    width, height = dimensions
    if not isinstance(regions, list) or not 0 < len(regions) <= MAX_REGIONS:
        raise ValueError('replacement needs 1..%d region rects' % MAX_REGIONS)
    for rect in regions:
        if not isinstance(rect, list) or len(rect) != 4 or not all(isinstance(v, int) and not isinstance(v, bool) for v in rect):
            raise ValueError('region rect must be four integers')
        x, y, w, h = rect
        if x < 0 or y < 0 or w < 1 or h < 1 or x + w > width or y + h > height:
            raise ValueError('region rect outside the image')
    return regions


def v2_identity(row):
    """Source identity key of one v2 row (mirrors the runtime lookup keys)."""
    kind = row.get('kind', 'frame')
    if kind == 'frame':
        return (kind, row['source_container_sha256'], row['frame'], row['palette'], row['source_png_sha256'])
    if kind == 'page':
        return (kind, row['source_container_sha256'], row['page'], row['source_png_sha256'])
    if kind == 'frame_part':
        return (kind, row['source_container_sha256'], row['frame'], row['palette'], row['part_index'], row['source_png_sha256'])
    return (kind, row['source_container_sha256'], row['bs_index'], row['decoder_identity_sha256'], row['source_rgba_sha256'])


def check_v2_row(row):
    """Validate one v2 row the way translated_image_store.gd does; raises ValueError."""
    kind = row.get('kind')
    if kind not in KINDS:
        raise ValueError('unsupported translation record kind')
    if not _hash(row.get('source_container_sha256')) or not _hash(row.get('output_sha256')):
        raise ValueError('invalid translation source identity')
    output = row.get('output')
    if not isinstance(output, str) or not output or '\\' in output or ':' in output or output.startswith('/') \
            or any(part in ('', '.', '..') for part in output.split('/')):
        raise ValueError('invalid replacement output path')
    size = row.get('dimensions')
    if not isinstance(size, list) or len(size) != 2 or not all(isinstance(v, int) and not isinstance(v, bool) and 1 <= v <= 4096 for v in size):
        raise ValueError('invalid replacement dimensions')
    if 'supersedes_output_sha256' in row and (kind != 'frame' or not _hash(row['supersedes_output_sha256'])):
        raise ValueError('invalid superseded output identity')
    if kind in ('frame', 'frame_part'):
        if not _hash(row.get('source_png_sha256')) or not _index(row.get('frame')) or not isinstance(row.get('palette'), str) or not row['palette'] or '|' in row['palette']:
            raise ValueError('invalid frame source identity')
    if kind == 'frame_part':
        # The row binds only to the exact frame: every part PNG hash, in part order.
        shas = row.get('frame_parts_sha256')
        if not isinstance(shas, list) or not 1 < len(shas) <= MAX_FRAME_PARTS or not all(_hash(v) for v in shas) \
                or not _index(row.get('part_index')) or row['part_index'] >= len(shas) \
                or shas[row['part_index']] != row['source_png_sha256']:
            raise ValueError('invalid frame part identity')
    elif kind == 'page':
        # source_rgba_sha256 binds the row to the source page pixels; the
        # runtime refuses a source image whose RGBA8 bytes hash differently.
        if not _hash(row.get('source_png_sha256')) or not _index(row.get('page')) or not _hash(row.get('source_rgba_sha256')):
            raise ValueError('invalid page source identity')
        check_regions(row.get('regions'), size)
    elif kind == 'bs':
        if not _index(row.get('bs_index')) or not _hash(row.get('decoder_identity_sha256')) or not _hash(row.get('source_rgba_sha256')):
            raise ValueError('invalid bs source identity')
        check_regions(row.get('regions'), size)
    return row


def write_pack(output, rows, sources, *, schema, development_drafts, snapshot_sha256, metadata=None, extra_files=None):
    """Write a pack directory atomically: copy each PNG, then manifest.json.

    `sources[i]` is the file to copy for `rows[i]` (destination `rows[i]['output']`).
    On any failure the new directory is removed.
    """
    if schema not in (PACK_V1, PACK_V2):
        raise ValueError('unknown pack schema')
    keys = set()
    for row in rows:
        if schema == PACK_V2:
            check_v2_row(row)
        key = v2_identity(row) if schema == PACK_V2 else v2_identity({**row, 'kind': 'frame'})
        if key in keys:
            raise ValueError('duplicate source replacement mapping')
        keys.add(key)
    output.mkdir(parents=True)
    try:
        for row, source in zip(rows, sources):
            destination = inside(output, row['output'])
            destination.parent.mkdir(parents=True, exist_ok=True)
            if not destination.exists():
                shutil.copyfile(source, destination)
            if digest(destination.read_bytes()) != row['output_sha256']:
                raise ValueError('copied replacement bytes differ')
        for name, data in (extra_files or {}).items():
            inside(output, name).write_bytes(data)
        pack = {'schema': schema, 'language': 'en', 'development_drafts': development_drafts,
                'snapshot_sha256': snapshot_sha256, 'runtime_verified': False, 'assets': rows, **(metadata or {})}
        (output / 'manifest.json').write_bytes(encoded(pack) + b'\n')
    except Exception:
        shutil.rmtree(output)
        raise
    return digest((output / 'manifest.json').read_bytes())


def build_pack(snapshot_root, output, include_drafts=False, schema_version=1):
    if schema_version == 2:
        return build_pack_v2(snapshot_root, output, include_drafts)
    report = audit(snapshot_root)
    output = new_output(output)
    selected = [r for r in report['assets'] if r['mapping_complete'] and (include_drafts or r['style'] == 'approve')]
    records, keys = [], set()
    for row in selected:
        key = '|'.join([row['source_container_sha256'], str(row['frame']), row['palette'], row['source_png_sha256']])
        if key in keys:
            raise ValueError('duplicate source replacement mapping')
        keys.add(key)
        filename = 'images/' + row['output_sha256'] + '.png'
        records.append({**row, 'output': filename})
    pack = {'schema': PACK_V1, 'language': 'en', 'development_drafts': include_drafts,
            'snapshot_sha256': report['snapshot_sha256'], 'runtime_verified': False, 'assets': records}
    output.mkdir(parents=True)
    try:
        (output / 'images').mkdir()
        for row, record in zip(selected, records):
            destination = output / record['output']
            if not destination.exists():
                shutil.copyfile(inside(snapshot_root, row['output']), destination)
        (output / 'manifest.json').write_bytes(encoded(pack) + b'\n')
    except Exception:
        shutil.rmtree(output)
        raise
    return {'pack': str(output), 'manifest_sha256': digest((output / 'manifest.json').read_bytes()), 'assets': len(records)}


def build_pack_v2(snapshot_root, output, include_drafts=False):
    """The v1 selection as a v2 pack: every row becomes kind `frame`, nothing else changes."""
    report = audit(snapshot_root)
    output = new_output(output)
    selected = [r for r in report['assets'] if r['mapping_complete'] and (include_drafts or r['style'] == 'approve')]
    rows = [{**row, 'kind': 'frame', 'output': 'images/' + row['output_sha256'] + '.png'} for row in selected]
    sha = write_pack(output, rows, [inside(snapshot_root, row['output']) for row in selected], schema=PACK_V2,
                     development_drafts=include_drafts, snapshot_sha256=report['snapshot_sha256'])
    return {'pack': str(output), 'manifest_sha256': sha, 'assets': len(rows), 'schema': PACK_V2}


def inventory(snapshot_root, catalog_path, option_sources=None):
    """Keep grouped catalog scope explicit; matching a frame is not acceptance."""
    report = audit(snapshot_root)
    catalog_bytes = catalog_path.read_bytes()
    rows = [json.loads(line) for line in catalog_bytes.splitlines() if line]
    requirements = []
    for index, row in enumerate(rows):
        section = row['section']
        container = row['container']
        selector = next((row[k] for k in ('frame_id', 'frame_ids_normal___highlight',
                        'frame___region_id', 'region___cell', 'asset_path') if k in row), '')
        frames = []
        if section.startswith(('1.', '2.')):
            container = 'CDROM/FUKUTEST/' + ('MAINROM.NBN' if section.startswith('1.') else 'OPTION.NBN')
            frames = [int(value) for value in re.findall(r'`(?:frame-)?(\d{4})`', selector)]
            if len(frames) == 2 and ('..' in selector or '–' in selector):
                frames = list(range(frames[0], frames[1] + 1))
        matching = [asset for asset in report['assets']
                    if asset['source_container'] == container and asset['frame'] in frames]
        recommendation = row.get('suggested_graphic___abbreviation_same_size', '')
        no_change = 'no change required' in recommendation.lower()
        requirements.append({'id': 'catalog-' + digest(encoded([section, container, selector]))[:20],
            'catalog_line': index + 1, 'section': section, 'container': container,
            'selector': selector, 'frames': frames, 'scope': 'catalog-group',
            'catalog_recommends_no_change': no_change,
            'existing_asset_ids': [asset['id'] for asset in matching],
            'status': 'catalog-no-change-needs-source-review' if no_change else ('existing-variants-need-context-verification' if matching else 'requires-source-and-artwork-review'),
            'in_game_verified': False,
            'blockers': ['palette variant coverage and actual route not verified'] if matching else ['source mapping and required replacement scope not verified']})
    option_frames = []
    if option_sources is not None:
        sources = json.loads((option_sources / 'manifest.json').read_text())
        frames_by_id = {r['frame_index']: r for r in sources['images'] if 'frame_index' in r}
        for group in requirements:
            if group['container'] != 'CDROM/FUKUTEST/OPTION.NBN':
                continue
            for frame in group['frames']:
                source = frames_by_id[frame]
                sha, dimensions = png_info(inside(option_sources, source['png']))
                retained = [r for r in report['assets'] if r['source_container'] == group['container']
                            and r['frame'] == frame and r['source_png_sha256'] == sha]
                option_frames.append({'id': 'option-frame-%04d' % frame,
                    'requirement': group['id'], 'frame': frame, 'source_png_sha256': sha,
                    'dimensions': dimensions, 'source_png': source['png'],
                    'existing_asset_ids': [r['id'] for r in retained],
                    'replacement_required_by_catalog': not group['catalog_recommends_no_change'],
                    'status': 'no-change-source-review-pending' if group['catalog_recommends_no_change'] else ('retained-exact-source' if retained else 'missing-artwork'),
                    'in_game_verified': False, 'blockers': ['OPTION renderer route unavailable']})
    return {'schema': 'tokimemo.translation-requirements.v1',
            'catalog_sha256': digest(catalog_bytes), 'catalog_groups': len(rows),
            'snapshot_sha256': report['snapshot_sha256'], 'coverage_complete': False,
            'limitations': ['Grouped prose catalog includes suspected and already-Latin regions; groups are not atomic asset counts.',
                           'Existing variants do not establish complete palette coverage or in-game acceptance.'],
            'requirements': requirements, 'option_frames': option_frames}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('operation', choices=['audit', 'pack', 'inventory'])
    parser.add_argument('--snapshot', type=Path, default=ROOT / 'translations/en/graphics/artwork')
    parser.add_argument('--output', required=True, type=Path)
    parser.add_argument('--include-drafts', action='store_true')
    parser.add_argument('--schema-version', type=int, choices=[1, 2], default=1, help='pack schema; 2 writes kind-tagged rows')
    parser.add_argument('--option-sources', type=Path, help='Restored source OPTION export directory')
    args = parser.parse_args()
    if args.operation in ('audit', 'inventory'):
        args.output = new_output(args.output)
        args.output.parent.mkdir(parents=True, exist_ok=True)
        report = audit(args.snapshot) if args.operation == 'audit' else inventory(args.snapshot, ROOT / 'translations/en/graphics/image_text_catalog.jsonl', args.option_sources)
        args.output.write_bytes(encoded(report) + b'\n')
        print(json.dumps({k:v for k,v in report.items() if k not in ('assets', 'requirements', 'option_frames')}, sort_keys=True))
    else:
        print(json.dumps(build_pack(args.snapshot, args.output, args.include_drafts, args.schema_version), sort_keys=True))


if __name__ == '__main__':
    main()
