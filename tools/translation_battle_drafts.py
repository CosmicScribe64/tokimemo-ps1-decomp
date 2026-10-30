#!/usr/bin/env python3
"""T-39004: draft native-font battle labels from authenticated frame geometry.

T-39010 adds the 16x8 Miss regions (frames 64/65 part 0). They are too small
for the twelve-row executable font, so they are lettered from the compact
5-row capitals of an original MAINROM sheet (see CompactLatin). The letters
are copied mask for mask: no scaling, no clipping, no added pixels.
"""
import argparse
import json
from pathlib import Path
import shutil
import sys

from PIL import Image

from translation_assets import ROOT, digest, encoded, inside, new_output, png_info
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


# T-39010: battle Miss lettering.
MAINROM_PATH = 'CDROM/FUKUTEST/MAINROM.NBN'
MAINROM_SHA = 'f425e1d19d64afcbafeadece7d72602e50e26ce88093047f73dbb0d21a9b9c1c'
COMPACT_SHEET = 0x16000  # 4bpp 128x128 sheet, low nibble is the left texel
COMPACT_SHEET_SHA = '313084e95eaaf05ee5cca358c28cc85b6db481a3ed53bf9d256d05142810a1cf'
COMPACT_INK = 1
# MAINROM frame 118 part 19 draws this sheet at u0 v0 112x32. That logo line is
# the only verified compact Latin M, I and S found in the original sheets. Its
# only clean M is the 3-wide #.#/###/#.#/#.#/#.# shape, which can read as H; a
# search of all ten sheets found no isolated wider M (T-39010 ticket).
COMPACT_REFERENCE = {'frame': 118, 'part': 19, 'sheet_offset': COMPACT_SHEET, 'region': [0, 0, 112, 32]}
COMPACT_GLYPHS = {
    'M': (17, 21, ('#.#', '###', '#.#', '#.#', '#.#')),
    'I': (37, 21, ('#', '#', '#', '#', '#')),
    'S': (78, 21, ('###', '#..', '###', '..#', '###')),
}
MISS_TEXT = 'MISS'
MISS_SIZE = (16, 8)
# frame: (page u, page v, ink index, shadow index, sha256 of the 16x8 source indices).
MISS_FRAMES = {
    64: (40, 72, 0x10, 0x12, '5e799a76f358b7cb02df3b437cebe0a159a5b1f20487c429d660f466606ea98c'),
    65: (40, 80, 0xEE, 0xEF, 'ada7cff2f42232209887ff74df16155a0e6c8ffee64a0af7db5444fd8af6a467'),
}
# Frame 8 part 0 is the original Latin "HP" on the same page; a fail-closed
# reference for the shadow rule (its shadow adds two hand-placed pixels).
HP_REFERENCE = (8, 96, 56, 0x10, 0x12, '1cc2921660aa57e8ab328bda93e9f247e4e26730053cd520846c1ddee5a5758f', 2)
# Same two 16x8 index blocks, outside PROFILES; a scope question, not drafted.
UNPROFILED_CONTAINERS = ('PARK.BIN', 'UTYU.BIN', 'VS_PARK.BIN', 'VS_UTYU.BIN')
MISS_REVISION = 'battle-miss-drafts-v1'


def extract_glyph(pixels, x, y, rows):
    """Return one pinned mask; its rectangle and a 1-px ring around it must match exactly."""
    width, height = len(rows[0]), len(rows)
    for yy in range(y - 1, y + height + 1):
        for xx in range(x - 1, x + width + 1):
            inner = x <= xx < x + width and y <= yy < y + height
            want = COMPACT_INK if inner and rows[yy - y][xx - x] == '#' else 0
            if pixels[yy * 128 + xx] != want:
                raise ValueError('compact glyph mask or gutter differs at %d,%d' % (xx, yy))
    mask = bytes(pixels[(y + dy) * 128 + x + dx] for dy in range(height) for dx in range(width))
    return {'points': [(dx, dy) for dy in range(height) for dx in range(width) if rows[dy][dx] == '#'],
            'width': width, 'height': height, 'rect': [x, y, width, height], 'mask_sha256': digest(mask)}


class CompactLatin:
    """Pinned 5-row capitals from original MAINROM disc art."""

    def __init__(self, mainrom):
        data = Path(mainrom).read_bytes()
        if digest(data) != MAINROM_SHA:
            raise ValueError('MAINROM identity differs')
        sheet = data[COMPACT_SHEET:COMPACT_SHEET + 8192]
        if digest(sheet) != COMPACT_SHEET_SHA:
            raise ValueError('compact sheet identity differs')
        pixels = [nibble for byte in sheet for nibble in (byte & 15, byte >> 4)]
        self.glyphs = {c: extract_glyph(pixels, x, y, rows) for c, (x, y, rows) in COMPACT_GLYPHS.items()}
        self.height = 5

    def glyph(self, character):
        return self.glyphs[character]

    def width(self, text):
        return sum(self.glyphs[c]['width'] for c in text) + len(text) - 1

    def provenance(self):
        return {'source': MAINROM_PATH, 'source_sha256': MAINROM_SHA,
                'sheet_offset': COMPACT_SHEET, 'sheet_sha256': COMPACT_SHEET_SHA,
                'referenced_by': COMPACT_REFERENCE,
                'wording': 'original disc art referenced by MAINROM frame 118 part 19; on-screen use not proven',
                'rescaled': False, 'derived_from_executable_font': False,
                'glyphs': {c: {'rect': g['rect'], 'mask_sha256': g['mask_sha256']}
                           for c, g in sorted(self.glyphs.items())}}


def diagonal(points):
    return {(x + 1, y + 1) for x, y in points}


def index_points(region, index, width=MISS_SIZE[0]):
    return {(i % width, i // width) for i, value in enumerate(region) if value == index}


def compose_miss(region, font, ink, shadow, text=MISS_TEXT):
    """Letter a 16x8 index region: source masks, 1-px tracking, exact (+1,+1) shadow.

    The ink bottom row is kept from the source lettering; everything else is index 0.
    """
    width, height = MISS_SIZE
    if len(region) != width * height or set(region) != {0, ink, shadow}:
        raise ValueError('miss region indices differ from the expected two inks')
    source_ink = index_points(region, ink)
    if diagonal(source_ink) - source_ink - index_points(region, shadow):
        raise ValueError('source shadow lacks the one-pixel diagonal')
    top = max(y for _, y in source_ink) - font.height + 1
    x = (width - font.width(text) - 1) // 2
    points = []
    for character in text:
        glyph = font.glyph(character)
        points.extend((x + dx, top + dy) for dx, dy in glyph['points'])
        x += glyph['width'] + 1
    if any(px < 0 or py < 0 or px + 1 >= width or py + 1 >= height for px, py in points):
        raise ValueError('miss lettering exceeds the source region')
    result = [0] * (width * height)
    for px, py in diagonal(points):
        result[py * width + px] = shadow
    for px, py in points:
        result[py * width + px] = ink
    return result


def page_region(data, page_offset, u, v, size=MISS_SIZE):
    width, height = size
    return [data[page_offset + (v + y) * 128 + u + x] for y in range(height) for x in range(width)]


def rgba(indices, palette, size=MISS_SIZE):
    return Image.frombytes('RGBA', size, b''.join(_color(palette[i]) for i in indices))


def magnified(original, draft, scale=12):
    """Original | draft at integer scale over a neutral ground (review aid, not published)."""
    width, height = original.size
    sheet = Image.new('RGBA', (width * scale * 2 + scale, height * scale), (48, 48, 48, 255))
    for column, image in ((0, original), (width * scale + scale, draft)):
        big = image.resize((width * scale, height * scale), Image.NEAREST)
        sheet.alpha_composite(big, (column, 0))
    return sheet


def check_hp_reference(data, parsed):
    frame, u, v, ink, shadow, expected, extra = HP_REFERENCE
    part = parsed['frames'][frame]['parts'][0]
    if (part['u'], part['v'], part['width'], part['height']) != (u, v) + MISS_SIZE:
        raise ValueError('HP reference geometry differs')
    region = page_region(data, parsed['page_offset'], u, v)
    if digest(bytes(region)) != expected:
        raise ValueError('HP reference identity differs')
    points, dark = index_points(region, ink), index_points(region, shadow)
    if diagonal(points) - points - dark or len(dark - diagonal(points)) != extra:
        raise ValueError('HP reference shadow rule differs')
    return {'frame': frame, 'part': 0, 'region': [u, v] + list(MISS_SIZE), 'indices_sha256': expected,
            'ink_rows': sorted({y for _, y in points}), 'hand_placed_shadow_pixels': extra}


def letter_container(data, parsed, font):
    """Return the patched container bytes and one record per Miss frame (checked geometry and source)."""
    patched = bytearray(data)
    letters = []
    for miss, (frame_id, (u, v, ink, shadow, expected)) in enumerate(sorted(MISS_FRAMES.items())):
        part = parsed['frames'][frame_id]['parts'][0]
        if (part['page_index'], part['u'], part['v'], part['width'], part['height']) != (0, u, v) + MISS_SIZE:
            raise ValueError('miss frame geometry differs')
        region = page_region(data, parsed['page_offset'], u, v)
        if digest(bytes(region)) != expected:
            raise ValueError('miss source region identity differs')
        lettered = compose_miss(region, font, ink, shadow)
        for y in range(MISS_SIZE[1]):
            start = parsed['page_offset'] + (v + y) * 128 + u
            patched[start:start + MISS_SIZE[0]] = bytes(lettered[y * 16:(y + 1) * 16])
        letters.append({'miss': miss, 'frame': frame_id, 'u': u, 'v': v, 'ink': ink, 'shadow': shadow,
                        'expected': expected, 'part': part, 'region': region, 'lettered': lettered})
    return bytes(patched), letters


def build_miss(input_dir, mainrom, output):
    output = new_output(output)
    font = CompactLatin(mainrom)
    prepared, comparisons, containers, hp = [], [], [], None
    for name in PROFILES:
        data = (input_dir / name).read_bytes()
        parsed = _parse(data, name)
        hp = check_hp_reference(data, parsed)
        patched, letters = letter_container(data, parsed, font)
        rows = []
        for letter in letters:
            miss, frame_id, u, v = letter['miss'], letter['frame'], letter['u'], letter['v']
            ink, shadow, expected, part = letter['ink'], letter['shadow'], letter['expected'], letter['part']
            region, lettered = letter['region'], letter['lettered']
            stem = name.lower().removesuffix('.bin')
            asset_id = '%s-miss-%d' % (stem, miss)
            original, draft = rgba(region, parsed['palette']), rgba(lettered, parsed['palette'])
            comparisons.append((asset_id, original, draft))
            rows.append((frame_id, u, v, lettered, draft))
            prepared.append((original, draft, {
                'id': asset_id, 'item_id': 'battle-%s-miss-%d' % (stem[3:], miss),
                'container': 'CDROM/BATTLE/' + name, 'source_container_sha256': digest(data),
                'frame': frame_id, 'part': 0, 'region': [u, v] + list(MISS_SIZE), 'dimensions': list(MISS_SIZE),
                'edit_regions': [[0, 0] + list(MISS_SIZE)], 'text': MISS_TEXT,
                'ink_index': ink, 'shadow_index': shadow, 'shadow_rule': 'pure (+1,+1) diagonal',
                'source_indices_sha256': expected, 'output_indices_sha256': digest(bytes(lettered)),
                'source_record_offset': part['source_record_offset'],
                'glyph_provenance': [{'letter': c, 'rect': font.glyph(c)['rect'],
                                      'mask_sha256': font.glyph(c)['mask_sha256']} for c in MISS_TEXT],
                'rescaled': False, 'derived_from_executable_font': False,
                'original': asset_id + '.original.png', 'output': asset_id + '.png',
                'style': 'unreviewed', 'human_approved': False,
                'runtime_integrated': False, 'in_game_verified': False}))
        containers.append(verify_patch(name, data, parsed, patched, rows))
    output.mkdir(parents=True)
    (output / 'compare').mkdir()
    for original, draft, record in prepared:
        original.save(output / record['original'])
        draft.save(output / record['output'])
        record['source_png_sha256'] = digest((output / record['original']).read_bytes())
        record['output_sha256'] = digest((output / record['output']).read_bytes())
    for asset_id, original, draft in comparisons:
        magnified(original, draft).save(output / 'compare' / (asset_id + '.x12.png'))
    assets = [row for _, _, row in prepared]
    review = agent_review(assets)
    review_bytes = encoded(review) + b'\n'
    (output / 'review.agent.json').write_bytes(review_bytes)
    (output / 'manifest.json').write_bytes(encoded({
        'schema': 'tokimemo.battle-miss-drafts.v1', 'ticket': 'T-39010', 'human_approved': False,
        'font_provenance': font.provenance(), 'hp_reference': hp, 'containers': containers,
        'agent_review': {'file': 'review.agent.json', 'sha256': digest(review_bytes)},
        'scope_notes': ['%s carry the same two 16x8 Miss index blocks but are outside PROFILES; '
                        'not drafted, scope is a human decision.' % ', '.join(UNPROFILED_CONTAINERS)],
        'risks': ['Palette indices 0xEE/0xEF are in the STP range (>=224); runtime transparency is not proven by index equality.',
                  'Battle runtime integration is none; the patch is proven only against _parse/_color.'],
        'assets': assets}) + b'\n')
    return {'drafts': len(assets), 'output': str(output)}


def verify_patch(name, data, parsed, patched, rows):
    """Re-parse the patched copy: same tables and palette, bytes changed only in the Miss rectangles."""
    again = _parse(patched, name)
    if len(patched) != len(data) or again['frames'] != parsed['frames'] or again['palette'] != parsed['palette']:
        raise ValueError('patched container tables differ')
    allowed = {parsed['page_offset'] + (v + y) * 128 + u + x
               for _, u, v, _, _ in rows for y in range(MISS_SIZE[1]) for x in range(MISS_SIZE[0])}
    changed = [i for i in range(len(data)) if data[i] != patched[i]]
    if not changed or set(changed) - allowed:
        raise ValueError('patched bytes outside the Miss rectangles')
    for _, u, v, lettered, draft in rows:
        decoded = rgba(page_region(patched, again['page_offset'], u, v), again['palette'])
        if page_region(patched, again['page_offset'], u, v) != lettered or decoded.tobytes() != draft.tobytes():
            raise ValueError('patched region does not decode to the draft')
    return {'container': 'CDROM/BATTLE/' + name, 'source_sha256': digest(data),
            'patched_sha256': digest(patched), 'changed_bytes': len(changed)}


def agent_review(assets):
    """Agent observations kept apart from approval; never a human decision."""
    return {'schema': 'tokimemo.battle-miss-agent-review.v1', 'ticket': 'T-39010',
            'reviewer': 'agent', 'human_approval': False, 'development_only': True,
            'checks': ['letter masks equal pinned original MAINROM masks with clean gutters',
                       'only the region ink and shadow indices are used; non-lettering is index 0',
                       'shadow equals the (+1,+1) diagonal of the ink minus the ink',
                       'patched containers re-parse with identical tables and palette'],
            'observations': ['The only verified compact M is 3 wide (#.#/###/#.#/#.#/#.#); at native size MISS may read as HISS.',
                             'That M is authentic: it is the first letter of the logo word MEMORIAL on the same sheet line '
                             '(M x17, E x21, M x25, O x29, R x33, I x37, A x39, L x43), so the original art draws M this way.',
                             'The dim frame-65 draft (ink 0xEE over shadow 0xEF) has lower contrast than frame 64.',
                             'The face is 5 ink rows in a slot whose original lettering uses 6.',
                             'Uppercase only: the source face has no lowercase, so MISS replaces the catalog Miss.',
                             'The original hand-placed counter-shadow pixels are not reproduced.'],
            'assets': [{key: row[key] for key in ('id', 'item_id', 'frame', 'source_container_sha256',
                                                   'source_png_sha256', 'output_sha256')}
                       for row in assets]}


def publish_miss(built, destination):
    """Copy built Miss drafts (never source images) and a candidate ledger into a new checkpoint."""
    manifest_bytes = (built / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    review_bytes = (built / 'review.agent.json').read_bytes()
    if digest(review_bytes) != manifest['agent_review']['sha256']:
        raise ValueError('agent review sidecar changed')
    if json.loads(review_bytes)['human_approval'] is not False:
        raise ValueError('agent review claims approval')
    destination = destination.resolve()
    destination.relative_to((ROOT / 'translations/en/graphics/artwork/checkpoints').resolve())
    if destination.exists():
        raise ValueError('destination must be new')
    staged = [(built / 'manifest.json', 'manifest.json'), (built / 'review.agent.json', 'review.agent.json')]
    assets = []
    for row in manifest['assets']:
        path = inside(built, row['output'])
        sha, dimensions = png_info(path)
        if sha != row['output_sha256'] or dimensions != row['dimensions']:
            raise ValueError('built output changed: ' + row['id'])
        if sha == row['source_png_sha256']:
            raise ValueError('built output is not a derivative: ' + row['id'])
        staged.append((path, row['output']))
        assets.append({'id': row['id'], 'item_id': row['item_id'], 'revision': MISS_REVISION,
                       'path': MISS_REVISION + '/' + row['output'], 'output_sha256': sha,
                       'dimensions': dimensions, 'frame': row['frame'],
                       'source_png_sha256': row['source_png_sha256'],
                       'source_container_sha256': row['source_container_sha256'],
                       'manifest_sha256': digest(manifest_bytes), 'status': 'draft', 'human_approved': False,
                       'reason': 'Candidate Miss lettering from original compact capitals; style, '
                                 'readability and in-game acceptance remain open human decisions.'})
    (destination / MISS_REVISION).mkdir(parents=True)
    for source, target in staged:
        shutil.copyfile(source, destination / MISS_REVISION / target)
    (destination / 'ledger.json').write_bytes(encoded({
        'schema': 'tokimemo.translation-checkpoint.v1', 'ticket': 'T-39004', 'subticket': 'T-39010',
        'assets': assets, 'candidate_assets': len(assets), 'original_images_included': False,
        'complete_translation_inventory': False, 'separate_reviewed_revisions': 0,
        'publication_authorization': 'User explicitly requested translated assets in repository, 2026-10-29.'}) + b'\n')
    return {'destination': str(destination), 'assets': len(assets)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', type=Path, required=True)
    parser.add_argument('--executable', type=Path, help='command labels: the paired executable')
    parser.add_argument('--mainrom', type=Path, help='Miss lettering (T-39010): MAINROM.NBN instead of commands')
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--publish', type=Path, help='Miss only: also publish into this new checkpoint directory')
    args = parser.parse_args()
    if (args.executable is None) == (args.mainrom is None) or (args.publish and not args.mainrom):
        parser.error('give exactly one of --executable or --mainrom; --publish needs --mainrom')
    if args.mainrom:
        result = build_miss(args.input, args.mainrom, args.output)
        if args.publish:
            result['published'] = publish_miss(args.output, args.publish)
        print(json.dumps(result))
    else:
        print(build(args.input, args.executable, args.output))
