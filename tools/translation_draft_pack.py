#!/usr/bin/env python3
"""T-39004: explicit development-only runtime pack for reviewing English drafts."""
import argparse
import json
from pathlib import Path
import shutil

from translation_assets import digest, encoded, inside, new_output, png_info


def build(source, output, palette, agent_review=None):
    manifest_bytes = (source/'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    if manifest.get('schema') not in ('tokimemo.name-translation-drafts.v1', 'tokimemo.option-translation-drafts.v1'):
        raise ValueError('unsupported draft manifest')
    output = new_output(output)
    review_bytes = None
    review = None
    reviewed = {}
    if agent_review is not None:
        review_bytes = agent_review.read_bytes()
        review = json.loads(review_bytes)
        if review.get('schema') != 'tokimemo.agent-artwork-review.v1' or review.get('draft_manifest_sha256') != digest(manifest_bytes) or review.get('human_approval') is not False or review.get('development_only') is not True or review.get('decision') != 'source-style-and-layout-accepted':
            raise ValueError('agent review does not bind this development draft manifest')
        for row in review.get('assets',[]):
            if row['id'] in reviewed: raise ValueError('duplicate reviewed asset identity')
            reviewed[row['id']] = row
        if not reviewed: raise ValueError('agent review has no bounded assets')
    rows = []
    for asset in manifest['assets']:
        if review is not None:
            if asset['id'] not in reviewed: continue
            identity = reviewed.pop(asset['id'])
            if any(identity.get(key) != asset.get(key) for key in ('frame','source_container_sha256','source_png_sha256','output_sha256')):
                raise ValueError('agent review source/output identity differs')
        path = inside(source, asset['output'])
        actual, size = png_info(path)
        if actual != asset['output_sha256'] or size != asset['dimensions']:
            raise ValueError('draft bytes changed')
        rows.append({**asset, 'palette':palette, 'style':'unreviewed'})
    if reviewed: raise ValueError('review contains assets absent from the draft manifest')
    output.mkdir(parents=True)
    try:
        for row in rows:
            shutil.copyfile(inside(source,row['output']),output/row['output'])
        metadata = {}
        if review_bytes is not None:
            (output/'agent-review.json').write_bytes(review_bytes)
            metadata['agent_review'] = {'path':'agent-review.json','sha256':digest(review_bytes),'id':review['id'],'human_approval':False}
        (output/'manifest.json').write_bytes(encoded({'schema':'tokimemo.translated-image-pack.v1',
            'language':'en','development_drafts':True,'snapshot_sha256':digest(manifest_bytes),
            'runtime_verified':False,'assets':rows,**metadata})+b'\n')
    except Exception:
        shutil.rmtree(output)
        raise
    return {'output':str(output),'assets':len(rows),'sha256':digest((output/'manifest.json').read_bytes())}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--palette',required=True)
    parser.add_argument('--agent-review',type=Path,help='Select only exact source/output identities covered by a separate bounded agent review; remains development-only')
    args = parser.parse_args()
    print(json.dumps(build(args.source,args.output,args.palette,args.agent_review)))
