"""T-39002 stage 21: export the five prologue_finish_run bank reads for the port.

prologue.c::prologue_finish_run substep 0 calls
audio_bank_load_start(0, 0xBF98, 0xBF79, 0xD3D5, 0xD387, 0xD37D).
frame_render_audio.c::audio_bank_load_start (a == 0 branch, after
audio_bank_state_reset cleared the LBA cache) issues five whole-sector nowait
reads in this order:

  b 0xBF98 x 3    -> 0x80010000  COM_MIX.VH  (2 sectors) + 1 of EFF1.BAK
  c 0xBF79 x 0x20 -> 0x801B8000  COM_MIX.VB (31 sectors) + 1 of COM_MIX.VH
  f 0xD37D x 0x25 -> 0x8001C000  SPRING.SEQ (10 sectors) + 27 of SPRING.VB
  e 0xD387 x 0x50 -> 0x801C8000  SPRING.VB  (78 sectors) + 2 of SPRING.VH
  d 0xD3D5 x 6    -> 0x80012000  SPRING.VH   (5 sectors) + 1 of STUDIUM.SEQ

The port needs the exact sector bytes (the over-read lands in RAM too), so
each read is exported whole, together with the ISO 9660 identity (path, LBA,
size, SHA-256) of every file whose sectors it covers. The call arguments and
sector counts are parsed from the C source, never copied by hand.

Run in Docker with the disc mounted read-only and a NEW output directory:

  docker run --rm --name stage21-bank-export \
    -v "$PWD:/work:ro" -v <main>/disc:/disc:ro -v <main>/.asset-cache/port:/out \
    -w /work tokimemo-recovery python3 -I tools/port/prologue_finish_bank_export.py \
      --track /disc/track1.bin --output /out/first-arrival-board-stage21-v1
"""
import argparse
import hashlib
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.assets.input import open_input  # noqa: E402

SCHEMA = 'tokimemo.prologue-finish-bank.v1'
DATA = 2048
SECTOR = 2352
# Source read order of the a == 0 branch; field name -> audio_bank_load_start
# parameter letter.
ORDER = (('se_header', 'b'), ('se_body', 'c'), ('sequence', 'f'), ('music_body', 'e'), ('music_header', 'd'))
# Pinned results of a verified export (sha256 of the whole sector read).
PINS = {
    'se_header': '653eda42f86a4de8eef40a49a1efb19fa1d4cda67e8a2de9043a31e3faf87552',
    'se_body': '8a1034f0413cd0232e63448ac54573528bd18248636f5a129f5d34725af392a9',
    'sequence': 'df78bc6a6237b90c5f5fe323ec2ae0760b84159f642665b76a543a7ba904965b',
    'music_body': '69ad06b80c84e616ead7b2132118c07116b02534bfd25a09bddba44c640afbb2',
    'music_header': 'e3958c5a8e7918716f9d2062d43c878c828a2bb611ec242a4084360ee31a6e5c',
}


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _body(text: str, signature: str) -> str:
    if text.count(signature) != 1:
        raise ValueError('source function missing or ambiguous: ' + signature)
    return text.split(signature, 1)[1].split('\n}\n', 1)[0]


def source_reads(root: Path) -> list:
    prologue = (root / 'src/ovl/name_entry/prologue.c').read_text()
    finish = _body(prologue, 'void prologue_finish_run(void) {')
    calls = re.findall(r'audio_bank_load_start\(([^()]*)\);', finish)
    if len(calls) != 1:
        raise ValueError('prologue_finish_run must make exactly one audio_bank_load_start call')
    tokens = [t.strip() for t in calls[0].split(',')]
    if len(tokens) != 6 or any(not re.fullmatch(r'0x[0-9A-Fa-f]+|[0-9]+', t) for t in tokens):
        raise ValueError('audio_bank_load_start arguments must be six integer literals')
    args = dict(zip('abcdef', (int(t, 0) for t in tokens)))
    if args['a'] != 0:
        raise ValueError('stage 21 bank request must use the a == 0 branch')
    loader = _body((root / 'src/main/frame_render_audio.c').read_text(),
                   'void audio_bank_load_start(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {')
    branch = loader.split('if (a == 0) {', 1)[1].split('\n    }\n    cd_load_destination_base_set', 1)[0]
    found = re.findall(r'cd_read_sectors_nowait\((0x[0-9A-Fa-f]+|[0-9]+)U, (0x[0-9A-Fa-f]+), ([bcdef])\);', branch)
    if [letter for _, _, letter in found] != [letter for _, letter in ORDER]:
        raise ValueError('a == 0 branch read order differs: ' + str(found))
    reads = []
    for (count, destination, letter), (field, wanted) in zip(found, ORDER):
        reads.append({'field': field, 'argument': letter, 'lba': args[letter],
                      'sectors': int(count, 0), 'destination': int(destination, 0)})
    return reads


def read_sectors(track: Path, lba: int, count: int) -> bytes:
    out = bytearray()
    with track.open('rb') as source:
        for index in range(count):
            source.seek((lba + index) * SECTOR)
            raw = source.read(SECTOR)
            if len(raw) != SECTOR or raw[:12] != b'\x00' + b'\xff' * 10 + b'\x00':
                raise ValueError(f'LBA {lba + index:#x} is not a raw data sector')
            if raw[15] != 2 or raw[18] & 0x20:
                raise ValueError(f'LBA {lba + index:#x} is not Mode 2 Form 1')
            out += raw[24:24 + DATA]
    return bytes(out)


def build(track: Path) -> tuple:
    reads = source_reads(ROOT)
    records = {}
    blobs = {}
    with open_input(str(track)) as iso:
        entries = [e for e in iso.entries() if e.get('kind') == 'file' and isinstance(e.get('location', {}).get('lba'), int)]
        for read in reads:
            data = read_sectors(track, read['lba'], read['sectors'])
            end = read['lba'] + read['sectors']
            covered = sorted((e for e in entries if e['location']['lba'] < end
                              and e['location']['lba'] + max(1, -(-e['size'] // DATA)) > read['lba']),
                             key=lambda e: e['location']['lba'])
            if not covered or covered[0]['location']['lba'] != read['lba']:
                raise ValueError(f"{read['field']}: read does not start at a unique ISO file")
            segments = []
            cursor = read['lba']
            for entry in covered:
                start = entry['location']['lba']
                if start != cursor:
                    raise ValueError(f"{read['field']}: sector gap before {entry['path']}")
                raw = iso.read(entry)
                sectors = max(1, -(-len(raw) // DATA))
                used = min(sectors, end - start)
                offset = (start - read['lba']) * DATA
                part = data[offset:offset + min(len(raw), used * DATA)]
                if part != raw[:len(part)]:
                    raise ValueError(f"{read['field']}: sector bytes differ from ISO file {entry['path']}")
                pad = data[offset + len(part):offset + used * DATA]
                segments.append({'path': entry['path'], 'lba': start, 'file_size': len(raw),
                                 'file_sha256': digest(raw), 'file_sectors': sectors,
                                 'read_offset': offset, 'sectors_in_read': used,
                                 'file_bytes_in_read': len(part),
                                 'sector_padding_bytes': len(pad), 'sector_padding_zero': pad == bytes(len(pad))})
                cursor = start + sectors
            if cursor < end:
                raise ValueError(f"{read['field']}: read runs past the last covered ISO file")
            sha = digest(data)
            if read['field'] in PINS and PINS[read['field']] != sha:
                raise ValueError(f"{read['field']}: sector read {sha} differs from pin")
            primary = segments[0]
            blobs[read['field']] = data
            records[read['field']] = dict(read, path=f"reads/{read['field']}.bin", size=len(data), sha256=sha,
                                          primary_file=primary['path'],
                                          over_read_sectors=read['sectors'] - primary['file_sectors'],
                                          segments=segments)
    return reads, records, blobs


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--track', required=True)
    parser.add_argument('--output', required=True)
    args = parser.parse_args(argv)
    output = Path(args.output)
    if output.exists():
        raise SystemExit('output exists; choose a new directory')
    track = Path(args.track)
    reads, records, blobs = build(track)
    output.mkdir(parents=True)
    (output / 'reads').mkdir()
    for field, data in blobs.items():
        (output / records[field]['path']).write_bytes(data)
    sources = {rel: digest((ROOT / rel).read_bytes()) for rel in
               ('src/ovl/name_entry/prologue.c', 'src/main/frame_render_audio.c')}
    manifest = {'schema': SCHEMA, 'release': 'original-ja-ps-the-best',
                'source': 'prologue.c prologue_finish_run audio_bank_load_start(0,0xBF98,0xBF79,0xD3D5,0xD387,0xD37D)',
                'source_sha256': sources, 'order': [r['field'] for r in reads],
                'track_sha256': digest(track.read_bytes()), 'reads': records,
                'native_transfer_supported': False}
    (output / 'finish_bank.json').write_text(json.dumps(manifest, indent=2, sort_keys=True) + '\n')
    print(json.dumps({k: v['sha256'] for k, v in records.items()}, indent=2))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
