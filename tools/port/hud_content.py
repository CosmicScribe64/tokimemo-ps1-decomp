"""Versioned native HUD export from verified original inputs and canonical readers.

No RAM state, screenshot, name, or game-session fixture is a runtime export input.
The CLI prints the absolute manifest path and its SHA-256 for the runtime caller.
All binary and image outputs must remain in the selected repository's ignored
asset cache. Dependencies and execution belong in Docker.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import stat
import struct
import sys
import tempfile

SCHEMA = 'tokimemo.native-command-hud.v1'
PROFILE = 'spring_type_a_ordinary.v1'
INPUTS = {
    'executable': ('SLPM_86.053', 667648, 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae'),
    'mainrom': ('CDROM/FUKUTEST/MAINROM.NBN', 108544, 'f425e1d19d64afcbafeadece7d72602e50e26ce88093047f73dbb0d21a9b9c1c'),
    'calendar': ('CDROM/FUKUTEST/CALROM.NBN', 305152, '775356bc13a15499b79172e57ea9d42e753fd916e0c01c166f42bb426fe5c867'),
    'room': ('CDROM/BS/C_02_02H.BS', 16936, 'ea4f9201486e8bfc21b03706b4427a2e3e0865f2be7a30f5057802618747ec96'),
}
ROOM_MANIFEST_SHA = '27eff05f51fcd4904a5c54215bcd9d0506d413d131143c3ff51f864ea1823e63'
ROOM_METADATA_SHA = '4d37c46cf3b582e40d94206d509255340a7c9af236f2a3d6facc130185a5a510'
ROOM_IMAGE_SHA = 'd1e8cc097c6602e257923b8c2cb422c6b476e0666fad8e4e568e697df1160202'
CURRENT_ROOM_IMAGE_SHA = 'd43c4a35204a86a074cda80f8104dcd3a5275fed4c43d8681ee8a440e8fefb77'
ROOM_IMAGE_NAME = 'assets/spring-room/asset-2223d133624701512989264fd4cfdcf74bd30490999c09cd0fe34529940fca51.background.png'
READERS = ['tools.assets.image_packs._parse', 'tools.assets.image_packs_calendar.export_calendar_ui',
           'tools.assets.text_binary._spans']
SOURCE_FILES = ['src/main/home_screens.c', 'src/main/hud_panels.c', 'src/main/hud_screens.c',
                'src/main/actor_sprites.c', 'src/main/menu_speaker_girl.c', 'src/main/text_system.c',
                'src/main/gpu_primitives.c', 'src/main/frame_render_audio.c',
                'tools/assets/image_packs.py', 'tools/assets/image_packs_calendar.py',
                'tools/assets/text_binary.py']
STRING_LABELS = {
    'ordinary_command': ('asm/data/main/home_screens.rodata.s', 'D_800AFF24', 0x800aff24),
    'narration_prefix': ('asm/data/main/menu_speaker_girl.rodata.s', 'D_800AFB74', 0x800afb74),
}


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def require(condition: bool, message: str) -> None:
    if not condition:
        raise ValueError(message)


def safe_absolute(path: str | Path, *, exists: bool = True) -> Path:
    value = Path(path)
    require(value.is_absolute() and '..' not in value.parts, 'path must be absolute and normalized')
    current = Path(value.anchor)
    for part in value.parts[1:]:
        current /= part
        require(not current.is_symlink(), f'symlink path component is forbidden: {current}')
        if current.exists():
            continue
        require(not exists, f'input path is missing: {current}')
    if exists:
        require(value.exists(), f'input path is missing: {value}')
    return value


def read_checked(path: str | Path, expected_sha: str, expected_size: int | None = None,
                 limit: int = 4 * 1024 * 1024) -> bytes:
    value = safe_absolute(path)
    require(re.fullmatch(r'[0-9a-f]{64}', expected_sha) is not None, 'expected SHA-256 is malformed')
    descriptor = os.open(value, os.O_RDONLY | getattr(os, 'O_NOFOLLOW', 0))
    with os.fdopen(descriptor, 'rb') as stream:
        before = os.fstat(stream.fileno())
        require(stat.S_ISREG(before.st_mode) and 0 < before.st_size <= limit, 'input is not a bounded regular file')
        if expected_size is not None:
            require(before.st_size == expected_size, 'input size differs from reviewed profile')
        data = stream.read(limit + 1)
        after = os.fstat(stream.fileno())
        require((before.st_size, before.st_mtime_ns, before.st_ino) ==
                (after.st_size, after.st_mtime_ns, after.st_ino), 'input changed during read')
    require(len(data) == before.st_size and digest(data) == expected_sha, 'input hash differs from reviewed profile')
    return data


def json_object(data: bytes) -> dict:
    def unique(pairs):
        result = {}
        for key, value in pairs:
            require(key not in result, f'duplicate JSON key: {key}')
            result[key] = value
        return result
    value = json.loads(data, object_pairs_hook=unique)
    require(isinstance(value, dict), 'JSON root must be an object')
    return value


def relative_path(value: str) -> str:
    require(isinstance(value, str) and value != '' and '\\' not in value,
            'manifest path must be a POSIX relative path')
    path = Path(value)
    require(not path.is_absolute() and '..' not in path.parts and path.as_posix() == value,
            'manifest path escapes packet root')
    return value


def png_size(data: bytes) -> tuple[int, int]:
    require(len(data) >= 33 and data[:8] == b'\x89PNG\r\n\x1a\n' and data[12:16] == b'IHDR',
            'image is not a bounded PNG')
    width, height = struct.unpack_from('>II', data, 16)
    require(0 < width <= 640 and 0 < height <= 512, 'PNG exceeds native image bounds')
    return width, height


def verified_room(manifest_path: Path, expected_sha: str) -> tuple[bytes, dict]:
    require(expected_sha == ROOM_MANIFEST_SHA, 'room packet is not the reviewed v1 source profile')
    manifest = json_object(read_checked(manifest_path, expected_sha, limit=256 * 1024))
    require(manifest.get('schema') == 'tokimemo.ignored-evidence-packet-manifest.v1', 'room manifest schema differs')
    files = manifest.get('files')
    require(isinstance(files, list) and len(files) == manifest.get('file_count'), 'room manifest file table differs')
    by_path = {}
    for record in files:
        require(isinstance(record, dict) and set(record) == {'path', 'size', 'sha256'}, 'room file record differs')
        name = relative_path(record['path'])
        require(name not in by_path, 'room manifest repeats a path')
        by_path[name] = record

    def get(name):
        require(name in by_path, 'reviewed room file is absent from manifest')
        spec = by_path[name]
        return read_checked(manifest_path.parent / name, spec['sha256'], spec['size'])

    original = get('assets/spring-room/original.bin')
    require(len(original) == INPUTS['room'][1] and digest(original) == INPUTS['room'][2], 'room original differs')
    metadata_bytes = get('assets/spring-room/metadata.json')
    require(digest(metadata_bytes) == ROOM_METADATA_SHA, 'room decode metadata differs')
    image = get(ROOM_IMAGE_NAME)
    require(digest(image) == ROOM_IMAGE_SHA and png_size(image) == (320, 88), 'room preview differs')
    return image, {'manifest_sha256': expected_sha, 'metadata_sha256': ROOM_METADATA_SHA,
                   'image_sha256': ROOM_IMAGE_SHA, 'original_sha256': INPUTS['room'][2]}


def verified_source_room(track: Path, repo: Path) -> tuple[bytes, dict]:
    """T-39002: regenerate the exact reviewed room pixels from original sectors."""
    if str(repo) not in sys.path:
        sys.path.insert(0, str(repo))
    from tools.assets.input import open_input
    from tools.assets.bs_cohort_profile import build_profile_context, TRACK_SHA256
    from tools.assets.image_mdec import export_mdec
    from tools.assets.manifest import asset_id
    with open_input(str(track)) as source:
        require(source.describe().get('source_sha256') == TRACK_SHA256, 'room track differs')
        entries = source.entries()
        profile = build_profile_context(source, entries, repo, 'seasonal')
        path, size, expected = INPUTS['room']
        records = [entry for entry in entries if entry.get('path') == path]
        require(len(records) == 1, 'room source entry is ambiguous or missing')
        original = source.read(records[0])
        require(len(original) == size and digest(original) == expected, 'room original differs')
        backgrounds = [entry for entry in profile['assets'] if entry.get('path') == path]
        require(len(backgrounds) == 1, 'room source profile is missing')
        context = {'width': 320, 'height': 88, 'padded_height': 96,
                   'source_profile': profile['source_profile'], 'geometry': profile['geometry'],
                   'evidence': backgrounds[0]['source_evidence']}
        with tempfile.TemporaryDirectory(prefix='tokimemo-room-') as scratch:
            identity = asset_id(path, expected)
            export_mdec(original, Path(scratch), identity, source_path=path, context=context)
            image = (Path(scratch) / (identity + '.background.png')).read_bytes()
    require(digest(image) == CURRENT_ROOM_IMAGE_SHA and png_size(image) == (320, 88), 'regenerated room preview differs')
    return image, {'track_sha256': TRACK_SHA256, 'image_sha256': CURRENT_ROOM_IMAGE_SHA,
                   'original_sha256': INPUTS['room'][2]}


def source_strings(repo: Path, exe: bytes, spans_reader) -> dict:
    result = {}
    binary = (exe, 0x80041000, 0x800, INPUTS['executable'][2], INPUTS['executable'][0])
    for key, (assembly, label, address) in STRING_LABELS.items():
        path = safe_absolute(repo / assembly)
        candidates = [s for s in spans_reader(path, repo, binary)
                      if s['label'] == label and s['address'] == address]
        require(len(candidates) == 1, 'source string label has no unique verified extent')
        raw = bytes(candidates[0]['raw'])
        end = raw.find(b'\0')
        require(0 < end <= 128 and end % 2 == 0, 'source string has no bounded two-byte terminator')
        codes = raw[:end]
        require(len(codes) == (2 if key == 'narration_prefix' else 26),
                'source string length differs from reviewed release label')
        result[key] = {'raw_hex': codes.hex(), 'address': address, 'label': label,
                       'assembly': assembly, 'assembly_sha256': digest(path.read_bytes()),
                       'length': len(codes), 'sha256': digest(codes)}
    return result


def output_directory(repo: Path, output: Path) -> Path:
    value = safe_absolute(output, exists=False)
    cache = safe_absolute(repo / '.asset-cache', exists=False) / 'port'
    require(value.is_relative_to(cache) and value != cache, 'output must be a version directory under repository .asset-cache/port')
    require(len(value.relative_to(cache).parts) == 1 and re.fullmatch(r'[a-z0-9][a-z0-9_.-]{0,95}', value.name) is not None,
            'output must name one new version directory directly below .asset-cache/port')
    require(not value.exists(), 'immutable output already exists')
    return value


def base_palette_address(info: int) -> int:
    """Executable address of a part's base-variant CLUT, from its palette_info word."""
    return 0x800c6020 + ((info >> 8) & 15) * 512 + (info & 0xf0) * 2


def hud_palette(read, address: int) -> tuple[list[int], list[int]]:
    """The 16 CLUT words at `address`, and the palette the HUD export draws with:
    the same words with STP set on every non-black colour of a CLUT in
    0x800c6200-0x800c6220 or 0x800c62c0-0x800c6340."""
    words = list(struct.unpack('<16H', read(address, 32)))
    semi = 0x800c6200 <= address < 0x800c6220 or 0x800c62c0 <= address < 0x800c6340
    return words, [w | 0x8000 if w & 0x7fff and semi else w for w in words]


def export_hud(repository: str | Path, executable: str | Path, mainrom: str | Path,
               calendar: str | Path, room_manifest: str | Path, room_manifest_sha256: str,
               output: str | Path, profile: str = PROFILE, fresh_room_track: str | Path | None = None) -> dict:
    repo = safe_absolute(repository)
    require(repo.is_dir() and (repo / 'tools/assets/image_packs.py').is_file(), 'repository has no canonical asset reader')
    require(profile == PROFILE, 'unsupported HUD export profile')
    destination = output_directory(repo, Path(output))
    source_bytes = {}
    for key, path in [('executable', executable), ('mainrom', mainrom), ('calendar', calendar)]:
        _, size, expected = INPUTS[key]
        source_bytes[key] = read_checked(path, expected, size)
    room, room_proof = (verified_source_room(safe_absolute(fresh_room_track), repo)
                        if fresh_room_track is not None else
                        verified_room(safe_absolute(room_manifest), room_manifest_sha256))
    source_proof = [{'path': name, 'sha256': digest(safe_absolute(repo / name).read_bytes())}
                    for name in sorted(set(SOURCE_FILES + [v[0] for v in STRING_LABELS.values()]))]
    if str(repo) not in sys.path:
        sys.path.insert(0, str(repo))
    from tools.assets.image_packs import _parse, OVERRIDES
    from tools.assets.image_packs_calendar import export_calendar_ui
    from tools.assets.images import _color, _png
    from tools.assets.text_binary import _spans
    exe, pack = source_bytes['executable'], source_bytes['mainrom']
    read, _, native_frames, sequences = _parse(pack, exe)
    strings = source_strings(repo, exe, _spans)
    positions = [list(struct.unpack('<2h', read(0x800b5ae4 + i * 4, 4))) for i in range(14)]
    destination.parent.mkdir(parents=True, exist_ok=True)
    safe_absolute(destination.parent)
    destination.mkdir(exist_ok=False)
    try:
        assets = destination / 'assets'
        assets.mkdir()

        def publish(name, data, width, height, kind='color'):
            require(re.fullmatch(r'[a-z0-9_.-]+\.png', name) is not None, 'unsafe output image name')
            require(png_size(data) == (width, height), 'exported PNG shape differs')
            with (assets / name).open('xb') as stream:
                stream.write(data)
            return {'path': 'assets/' + name, 'size': len(data), 'sha256': digest(data),
                    'width': width, 'height': height, 'kind': kind}

        frames = {}
        for frame in native_frames:
            index = frame['frame_index']
            variants = ['base'] + ([v for v in OVERRIDES if v.startswith('icon-')] if index == 2 else [])
            if 0x44 <= index < 0x59:
                variants += [v for v in OVERRIDES if v.startswith('hud-')]
            for variant in variants:
                record = {k: frame[k] for k in ('frame_index', 'bounds', 'origin')}
                record['variant'], record['parts'] = variant, []
                for part in frame['parts']:
                    address = base_palette_address(part['palette_info']) if variant == 'base' else OVERRIDES[variant][0]
                    words, palette = hud_palette(read, address)
                    rgba, stp = bytearray(), bytearray()
                    for y in range(part['height']):
                        sy = part['height'] - y - 1 if part['flip_flags'] & 1 else y
                        for x in range(part['width']):
                            sx = part['width'] - x - 1 if part['flip_flags'] & 2 else x
                            texel = (part['v'] + sy) * 128 + part['u'] + sx
                            packed = pack[part['sheet_source_offset'] + texel // 2]
                            word = palette[(packed >> (4 * (texel & 1))) & 15]
                            rgba.extend(_color(word))
                            stp.append(255 if word & 0x8000 else 0)
                    detail = {k: part[k] for k in ('part_index', 'x', 'y', 'width', 'height', 'exe_record_address')}
                    detail.update(palette_exe_address=address, source_palette_words=words, preview_palette_words=palette)
                    stem = f'f{index:03d}.{variant}.p{part["part_index"]:02d}'
                    detail['png'] = publish(stem + '.png', _png(part['width'], part['height'], rgba, 4), part['width'], part['height'])
                    detail['stp_png'] = publish(stem + '.stp.png', _png(part['width'], part['height'], stp, 1), part['width'], part['height'], 'stp')
                    record['parts'].append(detail)
                frames[f'{index}:{variant}'] = record
        calendar_export = export_calendar_ui(source_bytes['calendar'], exe, assets, 'calendar')
        calendars = []
        for image in calendar_export['images']:
            record = {k: image[k] for k in ('month_index', 'year', 'month', 'variant', 'source_region', 'palette_exe_address', 'source_palette_words')}
            for key, kind in [('png', 'color'), ('stp_png', 'stp')]:
                data = (assets / image[key]).read_bytes()
                require(png_size(data) == (image['width'], image['height']), 'canonical calendar PNG shape differs')
                record[key] = {'path': 'assets/' + image[key], 'size': len(data), 'sha256': digest(data),
                               'width': image['width'], 'height': image['height'], 'kind': kind}
            calendars.append(record)
        manifest = {
            'schema': SCHEMA, 'version': 1,
            'profile': {'id': PROFILE, 'months': [3, 4, 5], 'icon_mode': 0, 'icon_count': 14,
                        'room_draw_mode': 0, 'hover_mode': 'settled', 'memory_card_busy': 0},
            'provenance': {'inputs': {key: {'path': name, 'size': size, 'sha256': expected}
                                      for key, (name, size, expected) in INPUTS.items()},
                           'readers': READERS, 'source_evidence': source_proof, 'room_packet': room_proof},
            'tables': {'icon_positions': positions, 'sequences': sequences, 'draw_origin': [160, 120],
                       'glyph_advance': 14, 'back_clear': False, 'clear_color': [0, 0, 0]},
            'frames': frames,
            'content': {'room': publish('spring-room.png', room, 320, 88), 'calendar': calendars, 'strings': strings},
        }
        for witness in source_proof:
            require(digest(safe_absolute(repo / witness['path']).read_bytes()) == witness['sha256'],
                    'source reader/evidence changed during export')
        encoded = (json.dumps(manifest, sort_keys=True, indent=2) + '\n').encode()
        with (destination / 'manifest.json').open('xb') as stream:
            stream.write(encoded)
    except BaseException:
        shutil.rmtree(destination)
        raise
    return {'manifest_path': str(destination / 'manifest.json'), 'manifest_sha256': digest(encoded),
            'schema': SCHEMA, 'profile': profile, 'image_files': len(list(assets.iterdir()))}


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['repository', 'executable', 'mainrom', 'calendar', 'output']:
        parser.add_argument('--' + name, required=True)
    parser.add_argument('--room-manifest')
    parser.add_argument('--room-manifest-sha256')
    parser.add_argument('--fresh-room-track')
    parser.add_argument('--profile', default=PROFILE, choices=[PROFILE])
    args = parser.parse_args(argv)
    if not args.fresh_room_track and (not args.room_manifest or not args.room_manifest_sha256):
        parser.error('provide --fresh-room-track or both room manifest arguments')
    try:
        result = export_hud(**vars(args))
    except (ValueError, OSError, json.JSONDecodeError) as error:
        parser.exit(2, f'HUD export failed: {error}\n')
    print(json.dumps(result, sort_keys=True))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
