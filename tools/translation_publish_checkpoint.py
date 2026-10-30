#!/usr/bin/env python3
"""T-39004: publish explicitly requested English derivatives, never source images."""
import argparse
import json
from pathlib import Path
import shutil

from translation_assets import ROOT, digest, encoded, inside, png_info


SETS = (
    'option-drafts-v1', 'name-drafts-v2', 'title-drafts-v1',
    'exam-drafts-v1', 'filesel-drafts-v2', 'sports-drafts-v2',
    'battle-drafts-v1', 'name-reviewed-development-v1',
)


def publish(cache, output):
    output = output.resolve()
    publication_root = (ROOT / 'translations/en/graphics/artwork/checkpoints').resolve()
    output.relative_to(publication_root)
    if output == publication_root or output.exists():
        raise ValueError('publication requires a new checkpoint directory')
    files = []
    ledger = []
    for name in SETS:
        source = cache / name
        manifest_bytes = (source / 'manifest.json').read_bytes()
        manifest = json.loads(manifest_bytes)
        reviewed = name == 'name-reviewed-development-v1'
        review = None
        if reviewed:
            review_bytes = (source / 'agent-review.json').read_bytes()
            if digest(review_bytes) != manifest['agent_review']['sha256']:
                raise ValueError('agent review sidecar changed')
            review = json.loads(review_bytes)
            if review['human_approval'] or not review['development_only']:
                raise ValueError('review must remain development-only')
            files.append((source / 'agent-review.json', Path(name) / 'agent-review.json'))
        files.append((source / 'manifest.json', Path(name) / 'manifest.json'))
        for asset in manifest['assets']:
            path = inside(source, asset['output'])
            sha, dimensions = png_info(path)
            if sha != asset['output_sha256'] or ('dimensions' in asset and dimensions != asset['dimensions']):
                raise ValueError(f'changed translated output: {path}')
            if not asset.get('source_png_sha256') or sha == asset['source_png_sha256']:
                raise ValueError(f'not a distinct source-bound derivative: {path}')
            if reviewed:
                matches = [row for row in review['assets'] if row['id'] == asset['id']]
                if len(matches) != 1 or any(matches[0].get(key) != asset.get(key) for key in
                        ('frame', 'source_container_sha256', 'source_png_sha256', 'output_sha256')):
                    raise ValueError('agent review asset identity changed')
            target = Path(name) / asset['output']
            files.append((path, target))
            ledger.append({
                'id': asset['id'], 'revision': name, 'path': target.as_posix(),
                'output_sha256': sha, 'dimensions': dimensions,
                'source_png_sha256': asset['source_png_sha256'],
                'source_container_sha256': asset.get('source_container_sha256'),
                'manifest_sha256': digest(manifest_bytes),
                'status': 'agent-reviewed-in-game' if reviewed else 'draft',
                'human_approved': False,
                'reason': ('Observed across seven actual NAME states; source restoration exact. '
                           'Review covers source style and layout, not emulator timing parity.' if reviewed else
                           'Candidate artwork; source-style review and actual game route acceptance remain open.'),
            })
    if len(ledger) != 136:
        raise ValueError('checkpoint count changed; review publication scope')
    output.mkdir(parents=True)
    try:
        for source, target in files:
            destination = output / target
            destination.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(source, destination)
        (output / 'ledger.json').write_bytes(encoded({
            'schema': 'tokimemo.translation-checkpoint.v1',
            'ticket': 'T-39004', 'complete_translation_inventory': False,
            'publication_authorization': 'User explicitly requested translated assets in repository, 2026-10-29.',
            'original_images_included': False, 'candidate_assets': 127,
            'separate_reviewed_revisions': 9, 'assets': ledger,
        }) + b'\n')
    except Exception:
        shutil.rmtree(output)
        raise
    return {'output': str(output), 'translated_images': len(ledger),
            'files': len(files) + 1, 'bytes': sum(p.stat().st_size for p in output.rglob('*') if p.is_file())}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--cache', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(publish(args.cache, args.output)))
