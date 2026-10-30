#!/usr/bin/env python3
"""T-39004: rebind three batch-001 artworks to the current exact MAINROM source identity.

The three historical batch-001 assets (parameters frame 61, Monday frame 82,
Central Park guide frame 126) carry human approvals bound to source PNGs that the
current composed-source exporter cannot reproduce. Their pixels are still
exactly the lettering of those frames. This tool derives NEW candidates:

* the lettering regions of the approved PNG are copied unchanged in shape;
* each colour is mapped to the one current base-palette colour within +-1 per
  channel (the old exporter expanded RGB555 with different rounding);
* everything outside the declared edit regions is taken from the exact current
  source PNG.

Outputs are unreviewed drafts with the current source identity. They never
inherit the old approval; the old approvals stay bound to the old identities.
"""
import argparse
import json
from pathlib import Path

from PIL import Image

from translation_assets import ROOT, digest, encoded, inside, new_output

MAINROM_SHA = 'f425e1d19d64afcbafeadece7d72602e50e26ce88093047f73dbb0d21a9b9c1c'
# Sanity anchor: the exporter reproduces batch-002's approved Monday hud-44 source.
ANCHOR = ('mainrom.frame-082.hud-44.png', 'b6647598d68dd9500bba86a787bb2e54cb1b152790c90ed9e87112aa63cb3bac')
LEGACY = {61: 'parameters-panel-296x67', 82: 'day-monday-16x16', 126: 'central-park-guide-136x24'}


def inside_regions(regions, x, y):
    return any(a <= x < a + w and b <= y < b + h for a, b, w, h in regions)


def compose(source, old, regions, label='asset'):
    """Pure composition: lettering from `old` inside regions, exact `source` elsewhere.

    Returns (image, number of legacy pixels differing from the source outside the regions).
    """
    palette = sorted({c for c in source.getdata() if c[3]})

    def snap(color):
        if color[3] == 0:
            return (0, 0, 0, 0)
        near = [p for p in palette if max(abs(p[i] - color[i]) for i in range(3)) <= 1]
        if len(near) != 1:
            raise ValueError('lettering colour has no unique base-palette equivalent: %s %r' % (label, color))
        return near[0]
    result = source.copy()
    for y in range(source.height):
        for x in range(source.width):
            if inside_regions(regions, x, y):
                result.putpixel((x, y), snap(old.getpixel((x, y))))
    outside = [(x, y) for y in range(source.height) for x in range(source.width) if not inside_regions(regions, x, y)]
    if any(result.getpixel(q) != source.getpixel(q) for q in outside):
        raise ValueError('pixels changed outside edit regions: ' + label)
    if not set(result.getdata()) <= set(source.getdata()) | {(0, 0, 0, 0)}:
        raise ValueError('palette changed: ' + label)
    legacy_outside = 0
    for q in outside:
        o, t = old.getpixel(q), source.getpixel(q)
        if o[3] == 0 and t[3] == 0:
            continue
        if o[3] != t[3] or max(abs(o[i] - t[i]) for i in range(3)) > 1:
            legacy_outside += 1
    return result, legacy_outside


def build(sources, output, snapshot=None):
    snapshot = snapshot or ROOT / 'translations/en/graphics/artwork'
    manifest = json.loads((snapshot / 'manifest.json').read_text())
    legacy = {a['id']: a for v in manifest['versions'] if v.get('current') for a in v['assets']}
    anchor = digest((sources / ANCHOR[0]).read_bytes())
    if anchor != ANCHOR[1]:
        raise ValueError('legacy source exporter identity differs from the approved batch-002 anchor')
    output = new_output(output)
    plans = []
    for frame, asset_id in sorted(LEGACY.items()):
        asset = legacy[asset_id]
        old = Image.open(inside(snapshot, asset['output'])).convert('RGBA')
        source_path = sources / ('mainrom.frame-%03d.base.png' % frame)
        source = Image.open(source_path).convert('RGBA')
        if old.size != source.size or list(old.size) != asset['dimensions']:
            raise ValueError('dimension mismatch: ' + asset_id)
        regions = asset['edit_regions']
        result, legacy_outside = compose(source, old, regions, asset_id)
        name = 'mainrom-frame-%04d-base.png' % frame
        plans.append((result, {
            'id': 'legacy-rebind-' + asset_id, 'frame': frame, 'palette_id': 'base',
            'source_container': 'CDROM/FUKUTEST/MAINROM.NBN', 'source_container_sha256': MAINROM_SHA,
            'source_png_sha256': digest(source_path.read_bytes()), 'dimensions': list(source.size),
            'output': name, 'edit_regions': regions,
            'derived_from_legacy_asset': asset_id, 'legacy_output_sha256': asset['output_sha256'],
            'legacy_source_sha256': asset['source_sha256'],
            'legacy_pixels_differing_outside_edit_regions': legacy_outside,
            'style': 'unreviewed', 'human_approved': False,
            'runtime_integrated': False, 'in_game_verified': False}))
    output.mkdir(parents=True)
    for image, row in plans:
        image.save(output / row['output'])
        row['output_sha256'] = digest((output / row['output']).read_bytes())
    (output / 'manifest.json').write_bytes(encoded({
        'schema': 'tokimemo.legacy-rebind-drafts.v1', 'human_approved': False,
        'anchor': {'file': ANCHOR[0], 'sha256': ANCHOR[1]},
        'assets': [row for _, row in plans]}) + b'\n')
    return {'output': str(output), 'drafts': len(plans)}


def publish(built, destination):
    """Copy a built draft set and its candidate ledger into the repository tree."""
    manifest_bytes = (built / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    destination = destination.absolute()
    if destination.exists():
        raise ValueError('destination must be new')
    revision = 'legacy-rebind-v1'
    (destination / revision).mkdir(parents=True)
    assets = []
    for row in manifest['assets']:
        data = (built / row['output']).read_bytes()
        if digest(data) != row['output_sha256']:
            raise ValueError('built output changed: ' + row['id'])
        (destination / revision / row['output']).write_bytes(data)
        assets.append({'dimensions': row['dimensions'], 'human_approved': False, 'id': row['id'],
                       'manifest_sha256': digest(manifest_bytes), 'output_sha256': row['output_sha256'],
                       'path': revision + '/' + row['output'], 'revision': revision,
                       'frame': row['frame'], 'palette_id': row['palette_id'],
                       'source_container_sha256': row['source_container_sha256'],
                       'source_png_sha256': row['source_png_sha256'], 'status': 'draft',
                       'reason': 'Candidate rebinding of an approved batch-001 asset to the current exact source identity; the old approval does not transfer.'})
    (destination / revision / 'manifest.json').write_bytes(manifest_bytes)
    ledger = {'schema': 'tokimemo.translation-checkpoint.v1', 'ticket': 'T-39004', 'assets': assets,
              'candidate_assets': len(assets), 'original_images_included': False,
              'complete_translation_inventory': False, 'separate_reviewed_revisions': 0,
              'publication_authorization': 'User explicitly requested translated assets in repository, 2026-10-29.'}
    (destination / 'ledger.json').write_bytes(encoded(ledger) + b'\n')
    return {'destination': str(destination), 'assets': len(assets)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sources', type=Path, default=ROOT / '.asset-cache/translation-assets/original-mainrom-v1')
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--publish', type=Path, help='also copy outputs and a ledger into this new repository directory')
    args = parser.parse_args()
    result = build(args.sources, args.output)
    if args.publish:
        result['published'] = publish(args.output, args.publish)
    print(json.dumps(result))
