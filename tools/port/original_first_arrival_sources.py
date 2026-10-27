"""Export the six original first-arrival files, not rendered/looping BGM.

Run in Docker. The importer may call export_bytes with explicit original data;
the CLI takes an extracted original root. No developer-cache fallback exists.
"""
import argparse
import hashlib
import json
from pathlib import Path

SOURCES = [
    ('EXE_sha256', 'executable.bin', 'SLPM_86.053', 667648, 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae'),
    ('SE_header_sha256', 'SYS_MIX.VH', 'CDROM/VAB_SE/SYS_MIX.VH', 3104, '3d38d81c115f91918c318d106fad8e03c0f7ed686eeb4f7bc9bb0efe46eef404'),
    ('SE_body_sha256', 'SYS_MIX.VB', 'CDROM/VAB_SE/SYS_MIX.VB', 35376, '23df753d5e39259ce010f716707d151cba7a1a8c29fd2ebfdf5a0bf864bb0b86'),
    ('music_header_sha256', 'PRO_VOL.VH', 'CDROM/VAB_SEQ/PRO_VOL.VH', 9760, '05f7477864b4ca41a60fdbf943949b9f9a9cd83dcb74088d6529b3d957081032'),
    ('music_body_sha256', 'PRO_VOL.VB', 'CDROM/VAB_SEQ/PRO_VOL.VB', 160288, 'dc7a06a4d0d90c7a21ac2415cc0f78c2c6bf3ef7f207c6443727fd3d69b588b9'),
    ('sequence_sha256', 'PRO_VOL.SEQ', 'CDROM/VAB_SEQ/PRO_VOL.SEQ', 27624, '16feb604805496721c8797d481fd4244dc6b501c8a5f609d46f51a22c3acc70a'),
]


def manifest_bytes():
    value = {'schema': 'tokimemo.original-first-arrival-source.v1', 'version': 1,
             'files': [{'identity': key, 'path': name, 'bytes': size, 'sha256': digest}
                       for key, name, _, size, digest in SOURCES]}
    return (json.dumps(value, sort_keys=True, separators=(',', ':')) + '\n').encode()


def export_bytes(inputs, output):
    """Validate all bytes before creating a fresh importer-owned directory."""
    if set(inputs) != {row[0] for row in SOURCES}:
        raise ValueError('all six exact original identities are required')
    for key, _, _, size, digest in SOURCES:
        raw = inputs[key]
        if not isinstance(raw, bytes) or len(raw) != size or hashlib.sha256(raw).hexdigest() != digest:
            raise ValueError('original source differs: ' + key)
    output.mkdir(parents=True, exist_ok=False)
    for key, name, _, _, _ in SOURCES:
        (output / name).write_bytes(inputs[key])
    (output / 'source.json').write_bytes(manifest_bytes())
    return {'manifest_sha256': hashlib.sha256(manifest_bytes()).hexdigest(), 'files': len(SOURCES),
            'schema': 'tokimemo.original-first-arrival-source.v1', 'BGM_PCM_exported': False}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--original-root', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    result = export_bytes({key: (args.original_root / path).read_bytes()
                           for key, _, path, _, _ in SOURCES}, args.output)
    print(json.dumps(result))
