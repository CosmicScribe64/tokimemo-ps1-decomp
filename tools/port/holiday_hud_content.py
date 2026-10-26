"""Export the source-bound default holiday prompt with the canonical text reader.

Run in Docker. Original text is published only to a new ignored asset-cache
version; runtime inputs never include RAM, screenshots or a captured session.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import stat
import sys

EXE_SHA = 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae'
EXE_SIZE = 667648
LABEL = 'D_800B1678'
ADDRESS = 0x800b1678
ASSEMBLY = 'asm/data/main/holiday.rodata.s'
CALLER = 'asm/matchings/main/holiday/holiday_open_init.s'
SOURCES = [ASSEMBLY, CALLER, 'src/main/holiday.c', 'src/main/text_system.c',
           'tools/assets/text_binary.py']


def require(value: bool, message: str) -> None:
    if not value:
        raise ValueError(message)


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def absolute(value: str | Path, *, exists: bool = True) -> Path:
    raw = str(value)
    require(not raw.startswith('//'), 'leading slash aliases are unsupported')
    path = Path(value)
    require(path.is_absolute() and '..' not in path.parts, 'path must be absolute without traversal')
    current = Path(path.anchor)
    for part in path.parts[1:]:
        current /= part
        require(not current.is_symlink(), 'symlink path component is forbidden')
    require(not exists or path.exists(), 'input path is missing')
    return path


def read(path: Path, limit: int) -> bytes:
    absolute(path)
    fd = os.open(path, os.O_RDONLY | getattr(os, 'O_NOFOLLOW', 0))
    with os.fdopen(fd, 'rb') as stream:
        before = os.fstat(stream.fileno())
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= limit,
                'input must be a bounded regular file')
        data = stream.read(limit + 1)
        after = os.fstat(stream.fileno())
        require((before.st_ino, before.st_size, before.st_mtime_ns) ==
                (after.st_ino, after.st_size, after.st_mtime_ns), 'input changed during read')
    require(len(data) == before.st_size, 'input read was incomplete')
    return data


def export(repository: str, executable: str, output: str) -> dict:
    repo = absolute(repository)
    exe = read(absolute(executable), EXE_SIZE)
    require(len(exe) == EXE_SIZE and digest(exe) == EXE_SHA, 'original executable profile differs')
    destination = absolute(output, exists=False)
    require(destination.parent == repo / '.asset-cache/port' and
            not destination.exists() and destination.name not in ['', '.', '..'],
            'output must be a new direct version under repository .asset-cache/port')
    require(destination.parent.is_dir(), 'asset-cache parent is missing')
    witnesses = {name: digest(read(repo / name, 4 * 1024 * 1024)) for name in SOURCES}
    caller = read(repo / CALLER, 256 * 1024).decode('utf-8')
    lines = caller.splitlines()
    references = [i for i, line in enumerate(lines) if '%hi(' + LABEL + ')' in line]
    require(len(references) == 1 and any('jal        sndisp' in line
                for line in lines[references[0]:references[0] + 7]),
            'matching caller no longer proves the exact default prompt argument')
    sys.path.insert(0, str(repo))
    from tools.assets.text_binary import _spans
    spans = [span for span in _spans(repo / ASSEMBLY, repo,
             (exe, 0x80041000, 0x800, EXE_SHA, 'SLPM_86.053'))
             if span['label'] == LABEL and span['address'] == ADDRESS]
    require(len(spans) == 1, 'canonical reader did not verify a unique exact label extent')
    raw = bytes(spans[0]['raw'])
    end = raw.find(b'\0')
    require(0 < end <= 128 and end % 2 == 0, 'holiday prompt lacks bounded native code units')
    codes = raw[:end]
    record = {
        'schema': 'tokimemo.holiday-hud-text.v1', 'version': 1,
        'provenance': {
            'executable': {'name': 'SLPM_86.053', 'size': EXE_SIZE, 'sha256': EXE_SHA},
            'reader': 'tools.assets.text_binary._spans',
            'witnesses': [{'path': name, 'sha256': witnesses[name]} for name in SOURCES],
        },
        'strings': {'holiday_default': {
            'label': LABEL, 'address': ADDRESS, 'assembly': ASSEMBLY,
            'assembly_sha256': witnesses[ASSEMBLY], 'length': len(codes),
            'sha256': digest(codes), 'raw_hex': codes.hex(),
        }},
    }
    for name in SOURCES:
        require(digest(read(repo / name, 4 * 1024 * 1024)) == witnesses[name],
                'source evidence changed during export')
    data = (json.dumps(record, sort_keys=True, indent=2) + '\n').encode('utf-8')
    destination.mkdir()
    try:
        with (destination / 'manifest.json').open('xb') as stream:
            stream.write(data)
            stream.flush()
            os.fsync(stream.fileno())
    except BaseException:
        shutil.rmtree(destination)
        raise
    return {'manifest': str(destination / 'manifest.json'), 'sha256': digest(data),
            'label': LABEL, 'length': len(codes)}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repository', required=True)
    parser.add_argument('--executable', required=True)
    parser.add_argument('--output', required=True)
    args = parser.parse_args()
    print(json.dumps(export(args.repository, args.executable, args.output)))


if __name__ == '__main__':
    main()
