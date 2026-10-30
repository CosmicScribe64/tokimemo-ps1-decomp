"""T-39002: export the stage10/substep8 prologue_shiori CD reads for the port.

prologue.c ~1735 calls audio_bank_load_start(1,0,0,0xCBD4,0xCBAA,0xCBA5).
frame_render_audio.c::audio_bank_load_start issues three whole-sector nowait
reads, so the port needs the exact sector bytes, not the ISO file extents:

  CBA5 x 0x25 sectors -> 0x8002E800 (sequence pack)
  CBAA x 0x50 sectors -> 0x801B8000 (VAB body staging)
  CBD4 x 6    sectors -> 0x80016000 (slot2 VAB header)

The bytes are read from disc/track1.bin (raw 2352-byte Mode 2 Form 1 sectors)
by LBA, checked against the pinned sha256 of each exact read, and written to a
NEW directory below .asset-cache. The pinned digest is the source check, so a
user build needs only the disc. On a developer machine that holds the M2
landing captures (.asset-cache/emu/shiori-landing-m2), each read is also
compared with its emulator fixture; --require-fixtures makes that comparison
mandatory. Run in Docker:

  docker run --rm -v "$PWD:/work" -w /work tokimemo-content \
    python3 -I tools/port/native_audio/export_shiori_bank.py \
      --output .asset-cache/port/opening-shiori-bank-v1
"""
import argparse
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
SECTOR = 2352
USER = 24
DATA = 2048
FIXTURES = ROOT / '.asset-cache/emu/shiori-landing-m2'
READS = (
    # field, lba, sectors, destination, fixture, sha256 of the exact sector read
    ('sequence_pack', 0xCBA5, 0x25, 0x8002E800, 'expect_cba5.bin',
     '0545fd982409dd4d105e2a327ca813f17edd7ab37b0494c7006397319fda796d'),
    ('music_body', 0xCBAA, 0x50, 0x801B8000, 'expect_cbaa.bin',
     '97f14d89b05e5c530a33f8e3ef2411ec03fa908c32fe38aecfc274fcba140450'),
    ('music_header', 0xCBD4, 6, 0x80016000, 'expect_cbd4.bin',
     '622d0390afea51bccf234361a42ec93bbe3024874caa977d61b981af2d662bdd'),
)


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
            out += raw[USER:USER + DATA]
    return bytes(out)


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', required=True)
    parser.add_argument('--track', default='disc/track1.bin')
    parser.add_argument('--require-fixtures', action='store_true',
                        help='fail unless every read also equals its emulator fixture')
    args = parser.parse_args(argv)
    # Developer cross-check only: the pinned per-read sha256 below already
    # binds the bytes. Builds without .asset-cache/emu skip this comparison.
    check_fixtures = args.require_fixtures or FIXTURES.is_dir()
    output = ROOT / args.output
    if '..' in Path(args.output).parts or (ROOT / '.asset-cache').resolve() not in output.resolve().parents:
        raise SystemExit('output must be a new directory below .asset-cache')
    if output.exists():
        raise SystemExit('output exists; choose a new directory')
    track = ROOT / args.track
    records = {}
    blobs = {}
    for field, lba, count, destination, fixture, digest in READS:
        data = read_sectors(track, lba, count)
        actual = hashlib.sha256(data).hexdigest()
        if actual != digest:
            raise SystemExit(f'{field}: disc read {actual} differs from pinned {digest}')
        if check_fixtures:
            fixture_path = FIXTURES / fixture
            if not fixture_path.is_file():
                raise SystemExit(f'{field}: emulator fixture {fixture} is missing')
            if fixture_path.read_bytes() != data:
                raise SystemExit(f'{field}: disc read differs from emulator fixture {fixture}')
        blobs[field] = data
        records[field] = {
            'path': f'reads/prologue_shiori-{field}.bin', 'sha256': actual, 'size': len(data),
            'source_lba': lba, 'sectors': count, 'destination': destination,
        }
        if check_fixtures:
            records[field]['emulator_fixture'] = f'.asset-cache/emu/shiori-landing-m2/{fixture}'
    output.mkdir(parents=True)
    (output / 'reads').mkdir()
    for field, data in blobs.items():
        (output / records[field]['path']).write_bytes(data)
    track_sha = hashlib.sha256(track.read_bytes()).hexdigest()
    (output / 'shiori.json').write_text(json.dumps({
        'schema': 'tokimemo.opening-shiori-bank.v1',
        'source': 'prologue.c ~1735 audio_bank_load_start(1,0,0,0xCBD4,0xCBAA,0xCBA5)',
        'track': args.track, 'track_sha256': track_sha, 'reads': records,
    }, indent=2, sort_keys=True) + '\n')
    print(json.dumps({k: v['sha256'] for k, v in records.items()}, indent=2))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
