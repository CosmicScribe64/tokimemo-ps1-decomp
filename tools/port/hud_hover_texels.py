"""Hover-pulse texels for the scaled command icon (func_80098680 scaled path).

hud_icon_bar_update (src/main/hud_panels.c:1112-1117) scales the hovered icon
actor 80 + i by rsin((32 - D_800B5AA0) << 6) + 0x1000. actor_draw_all
(src/main/actor_sprites.c) then widens every part of a scaled actor to
w + 1, h + 1 texels, and func_80098680 (GsSortSprite, asm/libspu.s) maps
u..u + w, v..v + h onto a POLY_FT4. The GPU therefore samples one texel
column and one texel row beyond each part: the next column/row of the same
MAINROM sheet in VRAM. The HUD manifest (tools/port/hud_content.py) holds
only the w x h part, so this pack exports, for every frame of the fourteen
icon sequences, each part as (w + 1) x (h + 1) texels with the same base
palette as the HUD manifest. The port checks that the inner w x h equals
the pinned HUD manifest part image before it uses the extra texels.

Run in Docker with the original files mounted read-only and a NEW output dir:

  docker run --rm --name hud-hover-texels -v "$PWD:/work:ro" \
    -v <main>/disc:/disc:ro -v <main>/.asset-cache/port:/out -w /work \
    tokimemo-recovery python3 -I tools/port/hud_hover_texels.py \
      --executable /disc/files/SLPM_86.053 \
      --mainrom /disc/files/CDROM/FUKUTEST/MAINROM.NBN --output /out/hud-hover-texels-v1
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import sys

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.port.hud_content import (INPUTS, base_palette_address, hud_palette, read_checked,  # noqa: E402
                                    require, safe_absolute)
from tools.assets.image_packs import _parse  # noqa: E402
from tools.assets.images import _color, _png  # noqa: E402

SCHEMA = 'tokimemo.hud-hover-texels.v1'
MANIFEST = 'hover_texels.json'
SHEET = 128
ICON_SEQUENCES = 14
SOURCE_FILES = ['src/main/hud_panels.c', 'src/main/actor_sprites.c',
                'tools/assets/image_packs.py', 'tools/port/hud_content.py', 'tools/port/hud_hover_texels.py']


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def export(executable: str, mainrom: str, output: str) -> dict:
    exe = read_checked(executable, INPUTS['executable'][2], INPUTS['executable'][1])
    pack = read_checked(mainrom, INPUTS['mainrom'][2], INPUTS['mainrom'][1])
    destination = safe_absolute(output, exists=False)
    require(not destination.exists(), 'output directory must be new')
    read, _, frames, sequences = _parse(pack, exe)
    wanted = sorted({step['frame_index'] for sequence in sequences[:ICON_SEQUENCES] for step in sequence['steps']})
    destination.mkdir(parents=True)
    try:
        assets = destination / 'assets'
        assets.mkdir()

        def publish(name: str, data: bytes, width: int, height: int, kind: str) -> dict:
            with (assets / name).open('xb') as stream:
                stream.write(data)
            return {'path': 'assets/' + name, 'size': len(data), 'sha256': digest(data),
                    'width': width, 'height': height, 'kind': kind}

        records = {}
        for index in wanted:
            frame = frames[index]
            parts = []
            for part in frame['parts']:
                width, height = part['width'] + 1, part['height'] + 1
                require(part['flip_flags'] == 0, 'mirrored icon part needs the mirrored texel edge')
                require(part['u'] + width <= SHEET and part['v'] + height <= SHEET,
                        'scaled icon part edge leaves its MAINROM sheet')
                address = base_palette_address(part['palette_info'])
                words, palette = hud_palette(read, address)
                rgba, stp = bytearray(), bytearray()
                for y in range(height):
                    for x in range(width):
                        texel = (part['v'] + y) * SHEET + part['u'] + x
                        packed = pack[part['sheet_source_offset'] + texel // 2]
                        word = palette[(packed >> (4 * (texel & 1))) & 15]
                        rgba.extend(_color(word))
                        stp.append(255 if word & 0x8000 else 0)
                stem = f'f{index:03d}.p{part["part_index"]:02d}.hover'
                parts.append({'part_index': part['part_index'], 'x': part['x'], 'y': part['y'],
                              'width': part['width'], 'height': part['height'],
                              'u': part['u'], 'v': part['v'], 'sheet_index': part['sheet_index'],
                              'exe_record_address': part['exe_record_address'],
                              'palette_exe_address': address, 'source_palette_words': words,
                              'png': publish(stem + '.png', _png(width, height, rgba, 4), width, height, 'color'),
                              'stp_png': publish(stem + '.stp.png', _png(width, height, stp, 1), width, height, 'stp')})
            records[f'{index}:base'] = {'frame_index': index, 'origin': frame['origin'], 'parts': parts}
        evidence = [{'path': name, 'sha256': digest((ROOT / name).read_bytes())} for name in SOURCE_FILES]
        manifest = {'schema': SCHEMA, 'version': 1,
                    'provenance': {'inputs': {key: {'path': INPUTS[key][0], 'size': INPUTS[key][1], 'sha256': INPUTS[key][2]}
                                              for key in ('executable', 'mainrom')},
                                   'reader': 'tools.assets.image_packs._parse', 'source_evidence': evidence,
                                   'icon_sequences': ICON_SEQUENCES, 'edge': 'w + 1, h + 1 (actor_draw_all scaled part)'},
                    'frames': records}
        encoded = (json.dumps(manifest, sort_keys=True, indent=2) + '\n').encode()
        with (destination / MANIFEST).open('xb') as stream:
            stream.write(encoded)
    except BaseException:
        shutil.rmtree(destination)
        raise
    return {'manifest': str(destination / MANIFEST), 'sha256': digest(encoded), 'frames': len(records)}


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['executable', 'mainrom', 'output']:
        parser.add_argument('--' + name, required=True)
    args = parser.parse_args(argv)
    try:
        result = export(args.executable, args.mainrom, args.output)
    except (ValueError, OSError) as error:
        parser.exit(2, f'hover texel export failed: {error}\n')
    print(json.dumps(result, sort_keys=True))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
