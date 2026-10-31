#!/usr/bin/env python3
"""Phase 4 WP-L: turn the T-39012 BS splice outputs into v2 `bs`/`page` draft rows.

Input is a `tokimemo.bs-translation-drafts.v1` build directory written by
`tools/translation_bs_drafts.py build` (read only). For each MDEC draft:

1. the original BS (from `--bs-dir`) and the spliced English BS must hash to
   the recorded values, and both must decode through the pinned fixed-point
   model (tools/assets/image_mdec_model.py) to the recorded word hashes;
2. the two decodes may differ only inside the recorded edit regions (the
   edited 16x16 macroblocks), and every decoded word keeps its STP bit;
3. both decodes are expanded to 8-bit RGBA with one declared expansion
   (`replicate`: (v << 3) | (v >> 2), the T-39012 PNG export; `shift`: v << 3)
   and the T-39012 PNGs must equal the `replicate` expansion pixel for pixel.

The row is keyed for the removed port runtime (port/godot/services/translated_image_store.gd)
by the source container hash, bs index, decoder identity hash and the SHA-256
of the decoded source RGBA bytes, so a different decoder or expansion falls
back to the original.

Indexed ALL_C banner drafts become `page` rows: the English edit-region crop is
composed onto the exported source page and may change pixels only inside the
recorded page rect.

Outputs are unreviewed drafts in a new directory under .asset-cache; no row
claims human approval or runtime integration.
"""
import argparse
import json
from pathlib import Path
import shutil
import sys

import numpy as np
from PIL import Image

from translation_assets import (MDEC_MODEL_ID, ROOT, check_regions, digest, encoded, inside, new_output,
                                pinned_decoder_identity, rgba_sha256)
sys.path.insert(0, str(ROOT))
from tools.assets import image_mdec_model as mdec_model  # noqa: E402

SOURCE_SCHEMA = 'tokimemo.bs-translation-drafts.v1'
SCHEMA = 'tokimemo.mdec-translation-drafts.v1'
EXPANSIONS = ('replicate', 'shift')


def expand(rgb5, expansion):
    """8-bit RGB from 5-bit channels."""
    if expansion == 'replicate':
        return ((rgb5 << 3) | (rgb5 >> 2)).astype(np.uint8)
    if expansion == 'shift':
        return (rgb5 << 3).astype(np.uint8)
    raise ValueError('unknown RGB expansion')


def decode(data, width, height):
    """(height x width x 3) 5-bit RGB through the pinned model; every word must carry STP."""
    padded = (height + 15) // 16 * 16
    raw = mdec_model.decode_padded_words(data, width, height)
    words = np.frombuffer(raw, '<u2').reshape(padded, width)[:height].astype(np.int64)
    if not (words & 0x8000).all():
        raise ValueError('decoded word without STP bit')
    return raw, np.stack([words & 31, (words >> 5) & 31, (words >> 10) & 31], -1)


def region_mask(regions, width, height):
    mask = np.zeros((height, width), bool)
    for x, y, w, h in regions:
        mask[y:y + h, x:x + w] = True
    return mask


def rgba(rgb):
    return Image.fromarray(np.concatenate([rgb, np.full(rgb.shape[:2] + (1,), 255, np.uint8)], -1), 'RGBA')


def still_row(drafts, asset, bs_dir, expansion, decoder):
    if not asset.get('gates_passed') or asset.get('decoder_model') != MDEC_MODEL_ID:
        raise ValueError('MDEC draft did not pass its gates under the pinned model: ' + asset['id'])
    width, height = asset['geometry']['width'], asset['geometry']['height']
    original = (bs_dir / Path(asset['source_container']).name).read_bytes()
    if digest(original) != asset['source_container_sha256']:
        raise ValueError('original BS differs: ' + asset['source_container'])
    spliced = inside(drafts, asset['local_files']['spliced_bs']).read_bytes()
    if digest(spliced) != asset['spliced_bs_sha256']:
        raise ValueError('spliced BS differs from the T-39012 manifest: ' + asset['id'])
    source_words, source = decode(original, width, height)
    english_words, english = decode(spliced, width, height)
    if digest(source_words) != asset['source_words_sha256'] or digest(english_words) != asset['english_words_sha256']:
        raise ValueError('decoded words differ from the T-39012 manifest: ' + asset['id'])
    regions = check_regions([list(rect) for rect in asset['edit_regions']], [width, height])
    changed = (source != english).any(-1)
    if (changed & ~region_mask(regions, width, height)).any():
        raise ValueError('English decode changes pixels outside the edit regions: ' + asset['id'])
    # The T-39012 PNGs are the replicate expansion of the same decodes.
    for key, channels in (('source_png', source), ('english_png', english)):
        published = np.asarray(Image.open(inside(drafts, asset['local_files'][key])).convert('RGB'))
        if not np.array_equal(published, expand(channels, 'replicate')):
            raise ValueError('T-39012 %s differs from the pinned decode: %s' % (key, asset['id']))
    name = Path(asset['source_container']).stem
    source_image = rgba(expand(source, expansion))
    english_image = rgba(expand(english, expansion))
    row = {'id': asset['id'] + '.' + expansion, 'kind': 'bs', 'catalog_item': asset['catalog_item'],
           'source_container': asset['source_container'], 'source_container_sha256': asset['source_container_sha256'],
           'bs_index': asset['frame'], 'decoder_model': MDEC_MODEL_ID, 'decoder_identity_sha256': decoder,
           'rgb_expansion': expansion, 'source_rgba_sha256': rgba_sha256(source_image),
           'regions': regions, 'dimensions': [width, height],
           'spliced_bs_sha256': asset['spliced_bs_sha256'], 'english_words_sha256': asset['english_words_sha256'],
           'source_words_sha256': asset['source_words_sha256'], 'lines': [line['text'] for line in asset['lines']],
           'output': '%s.%s.en.png' % (name, expansion), 'source_png': '%s.%s.source.png' % (name, expansion),
           'style': 'unreviewed', 'human_approved': False, 'runtime_integrated': False, 'in_game_verified': False}
    return row, {row['output']: english_image, row['source_png']: source_image}


def page_rows(drafts, asset):
    if not asset.get('gates_passed'):
        raise ValueError('indexed draft did not pass its gates: ' + asset['id'])
    sources = {row['variant']: row['source_png_sha256'] for row in asset['source_pages']}
    rows, images = [], {}
    for region in asset['edit_regions']:
        page = region['page']
        variant = 'source-page-%d' % page
        source_path = inside(drafts, 'all-c-source/all-c.%s.png' % variant)
        if digest(source_path.read_bytes()) != sources.get(variant):
            raise ValueError('exported source page differs: ' + variant)
        crop_path = inside(drafts, region['png'])
        if digest(crop_path.read_bytes()) != region['sha256']:
            raise ValueError('edit-region crop differs: ' + region['png'])
        source = np.asarray(Image.open(source_path).convert('RGBA')).copy()
        crop = np.asarray(Image.open(crop_path).convert('RGB'))
        height, width = source.shape[:2]
        regions = check_regions([list(region['page_rect'])], [width, height])
        x, y, w, h = regions[0]
        if crop.shape[:2] != (h, w):
            raise ValueError('crop size differs from its page rect: ' + region['png'])
        result = source.copy()
        area = result[y:y + h, x:x + w]
        opaque = area[..., 3] != 0
        # A transparent source texel stays transparent unless the crop puts ink on it.
        ink = opaque | crop.any(-1)
        area[ink, :3] = crop[ink]
        area[ink, 3] = 255
        changed = (result != source).any(-1)
        if (changed & ~region_mask(regions, width, height)).any():
            raise ValueError('page composition escaped its rect')
        if not set(map(tuple, area[ink, :3])) <= set(map(tuple, source[..., :3][source[..., 3] != 0])) | set(map(tuple, crop.reshape(-1, 3))):
            raise ValueError('page composition introduced an unknown colour')
        name = 'all-c.%s' % variant
        row = {'id': '%s.page-%d' % (asset['id'], page), 'kind': 'page', 'catalog_item': asset['catalog_item'],
               'source_container': asset['source_container'], 'source_container_sha256': asset['source_container_sha256'],
               'page': page, 'page_variant': variant, 'source_png_sha256': sources[variant],
               'source_rgba_sha256': digest(source.tobytes()), 'regions': regions,
               'dimensions': [width, height], 'palette_id': asset['palette_id'],
               'english_container_sha256': asset['english_container_sha256'],
               'output': name + '.en.png', 'source_png': name + '.png',
               'style': 'unreviewed', 'human_approved': False, 'runtime_integrated': False, 'in_game_verified': False}
        rows.append(row)
        images[row['output']] = Image.fromarray(result, 'RGBA')
        images[row['source_png']] = source_path
    return rows, images


def build(drafts, output, bs_dir=None, expansion='replicate'):
    if expansion not in EXPANSIONS:
        raise ValueError('unknown RGB expansion')
    drafts = Path(drafts)
    manifest_bytes = (drafts / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    if manifest.get('schema') != SOURCE_SCHEMA or manifest.get('human_approved') is not False:
        raise ValueError('unsupported BS draft manifest')
    decoder = pinned_decoder_identity()
    bs_dir = Path(bs_dir or ROOT / 'disc/files/CDROM/BS')
    output = new_output(output)
    rows, files = [], {}
    for asset in manifest['assets']:
        row, images = still_row(drafts, asset, bs_dir, expansion, decoder)
        rows.append(row)
        files.update(images)
    for asset in manifest.get('indexed_assets', []):
        found, images = page_rows(drafts, asset)
        rows.extend(found)
        files.update(images)
    output.mkdir(parents=True)
    try:
        for name, content in files.items():
            if isinstance(content, Path):
                (output / name).write_bytes(content.read_bytes())
            else:
                content.save(output / name, optimize=False)
        for row in rows:
            row['output_sha256'] = digest((output / row['output']).read_bytes())
        (output / 'manifest.json').write_bytes(encoded({
            'schema': SCHEMA, 'source_manifest_sha256': digest(manifest_bytes), 'source_schema': SOURCE_SCHEMA,
            'decoder_identity_sha256': decoder, 'rgb_expansion': expansion, 'font_provenance': manifest['font_provenance'],
            'human_approved': False, 'assets': rows}) + b'\n')
    except Exception:
        shutil.rmtree(output)
        raise
    return {'output': str(output), 'drafts': len(rows), 'bs': sum(r['kind'] == 'bs' for r in rows),
            'page': sum(r['kind'] == 'page' for r in rows)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--bs-drafts', type=Path, required=True, help='T-39012 build directory (read only)')
    parser.add_argument('--output', type=Path, required=True, help='new directory under .asset-cache')
    parser.add_argument('--bs-dir', type=Path, help='original CDROM/BS directory (default disc/files/CDROM/BS)')
    parser.add_argument('--expansion', choices=EXPANSIONS, default='replicate')
    args = parser.parse_args()
    print(json.dumps(build(args.bs_drafts, args.output, args.bs_dir, args.expansion)))
