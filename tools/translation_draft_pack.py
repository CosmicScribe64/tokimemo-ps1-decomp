#!/usr/bin/env python3
"""T-39004: explicit development-only runtime pack for reviewing English drafts.

Frame drafts (NAME, OPTION) become kind-`frame` rows; `--schema-version 2`
writes them into a v2 pack, otherwise the v1 pack is unchanged. Page drafts
(tools/translation_filesel_drafts.py pages) and MDEC drafts
(tools/translation_mdec_drafts.py) carry their own `kind` and always produce a
v2 pack. Every pack here is `development_drafts: true`, every row is
`style: unreviewed`, and an agent review never becomes human approval.
"""
import argparse
import json
from pathlib import Path

from translation_assets import (PACK_V1, PACK_V2, check_v2_row, digest, encoded, inside, new_output,
                                pinned_decoder_identity, png_info, write_pack)

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
