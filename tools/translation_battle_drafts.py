#!/usr/bin/env python3
"""T-39004: draft native-font battle labels from authenticated frame geometry."""
import argparse
from pathlib import Path
import sys

from PIL import Image

from translation_assets import ROOT, digest, encoded, new_output
from translation_native_font import NativeLatin

sys.path.insert(0, str(ROOT))
from tools.assets.image_packs_battle import PROFILES, _parse
from tools.assets.images import _color

# Frame/part identities, not the obsolete catalog's numeric crop guesses.
LABELS = ((5, 1, 'Fight'), (6, 1, 'Guard'), (7, 1, 'Skill'),
          (2, 9, 'Fight'), (2, 9, 'Guard'), (4, 0, 'Skill'))


def render_label(original, text, font):
    colors = set(original.getdata()) - {(0, 0, 0, 0)}
    if len(colors) != 2:
        raise ValueError('command lettering must have two original colors')
    shadow, ink = sorted(colors, key=lambda color: sum(color[:3]))
    light = {(x, y) for y in range(16) for x in range(32)
             if original.getpixel((x, y)) == ink}
    dark = {(x, y) for y in range(16) for x in range(32)
            if original.getpixel((x, y)) == shadow}
    if {(x + 1, y + 1) for x, y in light} - light != dark:
        raise ValueError('source command shadow differs from one-pixel offset')
    result = Image.new('RGBA', (32, 16))
    x = (32 - font.width(text) - 1) // 2
    points = []
    for character in text:
        glyph = font.glyph(character)
        points.extend((x + dx, 2 + dy) for dx, dy in glyph['points'])
        x += glyph['width'] + 1
    if any(x < 0 or y < 0 or x + 1 >= 32 or y + 1 >= 16 for x, y in points):
        raise ValueError('native label exceeds source sprite')
    for x, y in points:
        result.putpixel((x + 1, y + 1), shadow)
    for x, y in points:
        result.putpixel((x, y), ink)
    return result


def build(input_dir, executable, output):
    output = new_output(output)
    font = NativeLatin(executable)
    prepared = []
    for name in PROFILES:
        data = (input_dir / name).read_bytes()
        parsed = _parse(data, name)
        page = data[parsed['page_offset']:parsed['page_offset'] + 16384]
        rgba = b''.join(_color(parsed['palette'][index]) for index in page)
        source = Image.frombytes('RGBA', (128, 128), rgba)
        for index, (frame_id, part_id, text) in enumerate(LABELS):
            part = parsed['frames'][frame_id]['parts'][part_id]
            u, v = part['u'], part['v']
            if index == 4:
                v += 16
            if part['page_index'] != 0 or part['width'] != 32 or part['height'] not in (16, 32):
                raise ValueError('command frame geometry differs')
            original = source.crop((u, v, u + 32, v + 16))
            replacement = render_label(original, text, font)
            asset_id = name.lower().removesuffix('.bin') + '-' + str(index)
            prepared.append((original, replacement, {
                'id': asset_id, 'container': 'CDROM/BATTLE/' + name,
                'source_container_sha256': digest(data), 'frame': frame_id,
                'part': part_id, 'region': [u, v, 32, 16], 'text': text,
                'source_record_offset': part['source_record_offset'],
                'original': asset_id + '.original.png', 'output': asset_id + '.png',
                'style': 'unreviewed', 'runtime_integrated': False, 'in_game_verified': False}))
    output.mkdir(parents=True)
    for original, replacement, record in prepared:
        original.save(output / record['original'])
        replacement.save(output / record['output'])
        record['source_png_sha256'] = digest((output / record['original']).read_bytes())
        record['output_sha256'] = digest((output / record['output']).read_bytes())
    (output / 'manifest.json').write_bytes(encoded({
        'schema': 'tokimemo.battle-translation-drafts.v1',
        'font_provenance': font.provenance(), 'assets': [row for _, _, row in prepared],
        'pending': 'Two 16x8 miss variants per encounter need source-compatible compact lettering.'}) + b'\n')
    return {'drafts': len(prepared), 'output': str(output)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', type=Path, required=True)
    parser.add_argument('--executable', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    print(build(args.input, args.executable, args.output))
