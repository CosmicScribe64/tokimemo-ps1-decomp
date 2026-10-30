"""T-39002 mode 0x34 command reload: export its two CD reads for the port.

home_screens.c::settings_command_screen_reload_calendar substep 1 calls
cd_read_sectors(0x95U, 0x801A0000, 0x9B94) (CDROM/FUKUTEST/CALROM.NBN), and
settings_room_bg_load_start (called by settings_command_screen_build) calls
dec_bg_cd_read(D_800B5950[hud_season_index()], 0). On a slot-0 cache miss
background_images.c::dec_bg_cd_read issues cd_read_sectors_nowait(0xA,
0x80162000, lba). D_800B5950 is read from the executable on the disc
(main .data, load address 0x80041000, header 0x800); the spring entry
(hud_season_index() 0 for months 3..5) is the room of the fresh New Game.

The port needs the exact sector bytes (an over-read lands in RAM too), so each
read is exported whole, with the ISO 9660 identity of every file whose sectors
it covers. The call arguments are parsed from the C source, not copied by hand.

Run in Docker with the disc mounted read-only and a NEW output directory:

  docker run --rm --name command-reload-export \
    -v "$PWD:/work:ro" -v <main>/disc:/disc:ro -v <main>/.asset-cache/port:/out \
    -w /work tokimemo-recovery python3 -I tools/port/command_reload_export.py \
      --track /disc/track1.bin --output /out/command-reload-v1
"""
import argparse
import hashlib
import json
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.assets.input import open_input  # noqa: E402

SCHEMA = 'tokimemo.command-reload-reads.v1'
DATA = 2048
SECTOR = 2352
EXECUTABLE = 'SLPM_86.053'
EXE_LOAD = 0x80041000
EXE_HEADER = 0x800
ROOM_TABLE = 0x800B5950  # D_800B5950, s32[4] by hud_season_index()
SPRING_SEASON = 0
# Pinned results of a verified export (sha256 of the whole sector read).
PINS = {
    'hud_calendar': '775356bc13a15499b79172e57ea9d42e753fd916e0c01c166f42bb426fe5c867',
    'room_spring': '31cf1a0907a6c754325b363c282567f52d60da8089cdebe6cd456999945c56b2',
}


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _body(text: str, signature: str) -> str:
    if text.count(signature) != 1:
        raise ValueError('source function missing or ambiguous: ' + signature)
    return text.split(signature, 1)[1].split('\n}\n', 1)[0]


def _int(token: str) -> int:
    return int(token.rstrip('Uu'), 0)


def source_reads(root: Path) -> dict:
    home = (root / 'src/main/home_screens.c').read_text()
    calendar = _body(home, 'void settings_command_screen_reload_calendar(void) {')
    calls = re.findall(r'cd_read_sectors\((0x[0-9A-Fa-f]+U?), (0x[0-9A-Fa-f]+), (0x[0-9A-Fa-f]+)\);', calendar)
    if len(calls) != 1:
        raise ValueError('settings_command_screen_reload_calendar must make exactly one cd_read_sectors call')
    sectors, destination, lba = (_int(t) for t in calls[0])
    start = _body(home, 'void settings_room_bg_load_start(void) {')
    if 'dec_bg_cd_read(D_800B5950[hud_season_index()], 0)' not in start:
        raise ValueError('settings_room_bg_load_start no longer reads D_800B5950[hud_season_index()] into slot 0')
    bg = (root / 'src/main/background_images.c').read_text()
    reader = _body(bg, 's32 dec_bg_cd_read(s32 arg0, s32 arg1) {')
    slot0 = reader.split('if (arg0 == bg_lba_cache[0]) {', 1)
    if len(slot0) != 2:
        raise ValueError('dec_bg_cd_read slot-0 branch not found')
    room = re.findall(r'cd_read_sectors_nowait\((0x[0-9A-Fa-f]+), (0x[0-9A-Fa-f]+), arg0\);', slot0[1])
    if len(room) != 1:
        raise ValueError('dec_bg_cd_read slot-0 miss must issue one read')
    return {'hud_calendar': {'field': 'hud_calendar', 'lba': lba, 'sectors': sectors, 'destination': destination,
                             'source': 'home_screens.c settings_command_screen_reload_calendar substep 1'},
            'room_spring': {'field': 'room_spring', 'lba': None, 'sectors': _int(room[0][0]),
                            'destination': _int(room[0][1]),
                            'source': 'home_screens.c settings_room_bg_load_start -> dec_bg_cd_read(D_800B5950[0], 0)'}}


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


def _segments(iso, entries: list, data: bytes, lba: int, sectors: int, field: str) -> list:
    end = lba + sectors
    covered = sorted((e for e in entries if e['location']['lba'] < end
                      and e['location']['lba'] + max(1, -(-e['size'] // DATA)) > lba),
                     key=lambda e: e['location']['lba'])
    if not covered or covered[0]['location']['lba'] != lba:
        raise ValueError(f'{field}: read does not start at a unique ISO file')
    segments = []
    cursor = lba
    for entry in covered:
        start = entry['location']['lba']
        if start != cursor:
            raise ValueError(f"{field}: sector gap before {entry['path']}")
        raw = iso.read(entry)
        file_sectors = max(1, -(-len(raw) // DATA))
        used = min(file_sectors, end - start)
        offset = (start - lba) * DATA
        part = data[offset:offset + min(len(raw), used * DATA)]
        if part != raw[:len(part)]:
            raise ValueError(f"{field}: sector bytes differ from ISO file {entry['path']}")
        pad = data[offset + len(part):offset + used * DATA]
        segments.append({'path': entry['path'], 'lba': start, 'file_size': len(raw),
                         'file_sha256': digest(raw), 'file_sectors': file_sectors,
                         'read_offset': offset, 'sectors_in_read': used,
                         'file_bytes_in_read': len(part),
                         'sector_padding_bytes': len(pad), 'sector_padding_zero': pad == bytes(len(pad))})
        cursor = start + file_sectors
    if cursor < end:
        raise ValueError(f'{field}: read runs past the last covered ISO file')
    return segments


def build(track: Path) -> tuple:
    reads = source_reads(ROOT)
    records = {}
    blobs = {}
    with open_input(str(track)) as iso:
        entries = [e for e in iso.entries() if e.get('kind') == 'file' and isinstance(e.get('location', {}).get('lba'), int)]
        exe = [e for e in entries if e['path'] == EXECUTABLE]
        if len(exe) != 1:
            raise ValueError('executable not found on the disc')
        image = iso.read(exe[0])
        offset = ROOM_TABLE - EXE_LOAD + EXE_HEADER
        table = list(struct.unpack_from('<4i', image, offset))
        reads['room_spring']['lba'] = table[SPRING_SEASON]
        reads['room_spring']['room_table'] = table
        reads['room_spring']['executable_sha256'] = digest(image)
        for field in ('hud_calendar', 'room_spring'):
            read = reads[field]
            data = read_sectors(track, read['lba'], read['sectors'])
            segments = _segments(iso, entries, data, read['lba'], read['sectors'], field)
            sha = digest(data)
            if PINS[field] != sha:
                raise ValueError(f'{field}: sector read {sha} differs from pin')
            primary = segments[0]
            blobs[field] = data
            records[field] = dict(read, path=f'reads/{field}.bin', size=len(data), sha256=sha,
                                  primary_file=primary['path'],
                                  over_read_sectors=read['sectors'] - primary['file_sectors'],
                                  segments=segments)
    return records, blobs


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--track', required=True)
    parser.add_argument('--output', required=True)
    args = parser.parse_args(argv)
    output = Path(args.output)
    if output.exists():
        raise SystemExit('output exists; choose a new directory')
    track = Path(args.track)
    records, blobs = build(track)
    output.mkdir(parents=True)
    (output / 'reads').mkdir()
    for field, data in blobs.items():
        (output / records[field]['path']).write_bytes(data)
    sources = {rel: digest((ROOT / rel).read_bytes()) for rel in
               ('src/main/home_screens.c', 'src/main/background_images.c')}
    manifest = {'schema': SCHEMA, 'release': 'original-ja-ps-the-best',
                'source_sha256': sources, 'order': ['hud_calendar', 'room_spring'],
                'track_sha256': digest(track.read_bytes()), 'reads': records}
    (output / 'command_reload.json').write_text(json.dumps(manifest, indent=2, sort_keys=True) + '\n')
    print(json.dumps({k: [v['sha256'], v['lba'], v['sectors'], v['primary_file'], v['over_read_sectors']]
                      for k, v in records.items()}, indent=2))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
