"""Source-backed RPG_BAT 8bpp sheet and bounded crop previews.

This reader accepts only five encounter BINs whose page, frame-table and CLUT
addresses are assigned by `rpg_stage_asset_addresses`. It exports source pages
and eight numeric source regions; it does not identify their semantic contents
or decode other BATTLE files/containers.
"""
import hashlib
from pathlib import Path
import re
import struct

from .images import ImageFormatError, _color, _png

LOAD_BASE = 0x80180000
PAGE_BYTES = 128 * 128  # mode 1 is 8bpp; load_tpage_buf_lock uploads 128x128.
FRAME_COUNT = 66  # frame pointer table ends at the C-assigned animation pointer.

# source size, sheet0 pointer table, frames0 table, animation0 table, palette0,
# first sheet0 pointer target, encounter flag.
PROFILES = {
    'VS_HABU.BIN': (0x40000, 0x35758, 0x3576C, 0x35874, 0x3D94C, 0x20990, 0x10000),
    'VS_SIKA.BIN': (0x38000, 0x2D620, 0x2D634, 0x2D73C, 0x35814, 0x18858, 0x20000),
    'VS_KUMA.BIN': (0x38000, 0x2D638, 0x2D64C, 0x2D754, 0x3582C, 0x18870, 0x40000),
    'VS_UFO.BIN':  (0x4C800, 0x420A4, 0x420B8, 0x421C0, 0x4A298, 0x2D2DC, 0x80000000),
    'VS_ROBO.BIN': (0x4A800, 0x3FFA0, 0x3FFB4, 0x400BC, 0x48194, 0x2B1D8, 0x08000000),
}

# Numeric regions from a read-only external locator; coordinates are relative
# to the locator's +0x80 byte region. The C sheet pointer begins one full
# 128-byte row before it, so the source-page y coordinate is y+1.
CROP_REGIONS = ((99, 0, 26, 12), (98, 26, 27, 6), (97, 32, 30, 16),
                (97, 48, 30, 5), (40, 72, 45, 8), (40, 80, 47, 16),
                (57, 96, 30, 5), (83, 112, 26, 12))


def _parse(data: bytes, source_path: str):
    name = Path(source_path).name.upper()
    if name not in PROFILES:
        raise ImageFormatError('unsupported RPG_BAT source name; only five C-profiled encounter files are supported')
    size, sheet_table, frame_table, animation_table, palette_offset, page_offset, encounter = PROFILES[name]
    if len(data) != size:
        raise ImageFormatError('RPG_BAT encounter source has an unsupported extent')
    if sheet_table + 4 > len(data) or frame_table + FRAME_COUNT * 4 != animation_table:
        raise ImageFormatError('RPG_BAT source table extents differ from the C profile')
    page_pointer = struct.unpack_from('<I', data, sheet_table)[0]
    if page_pointer != LOAD_BASE + page_offset or page_offset + PAGE_BYTES > len(data):
        raise ImageFormatError('RPG_BAT source page pointer/extent differs from C profile')
    if palette_offset + 512 > len(data):
        raise ImageFormatError('RPG_BAT source palette block is truncated')

    frames = []
    for index, (pointer,) in enumerate(struct.iter_unpack('<I', data[frame_table:animation_table])):
        offset = pointer - LOAD_BASE
        if offset < 0 or offset + 8 > len(data):
            raise ImageFormatError('RPG_BAT frame pointer is outside the source file')
        count, origin_x, origin_y, flags = struct.unpack_from('<4h', data, offset)
        if not 1 <= count <= 64 or offset + 8 + count * 14 > len(data):
            raise ImageFormatError('RPG_BAT frame record extent is invalid')
        parts = []
        for part_index in range(count):
            record = offset + 8 + part_index * 14
            x, y, texture, palette, flip, width, height = struct.unpack_from('<2h3H2h', data, record)
            page = texture >> 8
            u, v = (texture & 15) * 8, ((texture >> 4) & 15) * 8
            if (page >= (frame_table - sheet_table) // 4 or flip & ~3
                    or not 0 < width <= 128 or not 0 < height <= 128
                    or u + width > 128 or v + height > 128):
                raise ImageFormatError('RPG_BAT frame has unsupported geometry or flags')
            row = (palette >> 8) & 15
            # The source page is the first page table entry. Frame table0 is
            # rendered with the default row 0. Other pages/rows are inventoried,
            # not rendered by this bounded reader.
            if page == 0 and row != 0:
                raise ImageFormatError('RPG_BAT page0 part uses an unprofiled palette row')
            parts.append({'part_index': part_index, 'source_record_offset': record,
                          'x': x, 'y': y, 'width': width, 'height': height,
                          'texture_id': texture, 'page_index': page, 'u': u, 'v': v,
                          'palette_info': palette, 'palette_row': row,
                          'flip_flags': flip})
        frames.append({'frame_index': index, 'source_frame_offset': offset,
                       'origin': [origin_x, origin_y], 'frame_flags': flags, 'parts': parts})

    source_palette = struct.unpack_from('<256H', data, palette_offset)
    # rpg_stage_palettes_load uses mode 4 for palette0 block 0: clear STP on
    # entries 0..223, set STP on non-black entries 224..255.
    palette = tuple((word & 0x7FFF) if i < 224 else
                    ((word | 0x8000) if word & 0x7FFF else word)
                    for i, word in enumerate(source_palette))
    return {'name': name, 'size': size, 'encounter_flag': encounter,
            'page_offset': page_offset, 'palette_offset': palette_offset,
            'source_palette': source_palette, 'palette': palette, 'frames': frames}


def probe_battle_ui(data: bytes, source_path: str) -> dict:
    """Validate one exact RPG_BAT source profile without writing previews."""
    if Path(source_path).name.upper() not in PROFILES:
        return {'status': 'unsupported', 'format': None, 'source_path': source_path,
                'candidates': [], 'reason': 'no exact C-backed RPG_BAT encounter profile'}
    try:
        parsed = _parse(data, source_path)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'tokimemo-rpg-bat-ui', 'source_path': source_path,
                'candidates': [], 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-rpg-bat-ui', 'source_path': source_path,
            'candidates': [{'profile': parsed['name'], 'source_size': parsed['size'],
                            'page_source_offset': parsed['page_offset'], 'frame_count': len(parsed['frames']),
                            'page0_part_count': sum(p['page_index'] == 0 for f in parsed['frames'] for p in f['parts']),
                            'palette_source_offset': parsed['palette_offset'], 'palette_row': 0}]}


def export_battle_ui(data: bytes, exe, output_dir, asset_id: str, *, source_path: str) -> dict:
    """Export original indexed source page and numeric crop regions with CLUT provenance."""
    parsed = _parse(data, source_path)
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('asset_id must be a safe single filename component')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    page = data[parsed['page_offset']:parsed['page_offset'] + PAGE_BYTES]
    palette = parsed['palette']
    files, images = [], []

    def write_preview(stem, variant, preview_kind, width, height, indices, region, refs):
        words = [palette[index] for index in indices]
        rgba = b''.join(_color(word) for word in words)
        mask = bytes(255 if word & 0x8000 else 0 for word in words)
        png, stp = stem + '.png', stem + '.stp.png'
        for filename, content in ((png, _png(width, height, rgba, 4)),
                                  (stp, _png(width, height, mask, 1))):
            with (output_dir / filename).open('xb') as output:
                output.write(content)
            files.append(filename)
        images.append({'png': png, 'stp_png': stp, 'variant': variant,
                       'preview_kind': preview_kind, 'palette_status': 'caller-established',
                       'source_sheet_offset': parsed['page_offset'], 'source_sheet_size': PAGE_BYTES,
                       'source_sheet_sha256': hashlib.sha256(page).hexdigest(),
                       'width': width, 'height': height,
                       'source_region': region, 'source_palette_words': list(parsed['source_palette']),
                       'preview_palette_words': list(palette), 'palette_source_offset': parsed['palette_offset'],
                       'palette_block_index': 0, 'palette_mode': 4, 'clut_vram': [0, 0x1F0],
                       'bits_per_pixel': 8, 'alpha_policy': 'BGR555 zero word transparent; nonzero opaque',
                       'stp_policy': 'source palette mode4: clear entries 0..223 and set non-black entries 224..255',
                       'intersecting_frame_parts': refs})

    page_refs = [{'frame_index': f['frame_index'], 'part_index': p['part_index'],
                  'source_record_offset': p['source_record_offset'], 'uv': [p['u'], p['v'], p['width'], p['height']],
                  'palette_row': p['palette_row']}
                 for f in parsed['frames'] for p in f['parts'] if p['page_index'] == 0]
    write_preview(f'{asset_id}.page0', 'source-page-0', 'source_texture_page', 128, 128, page,
                  {'x': 0, 'y': 0, 'width': 128, 'height': 128, 'source_offset': parsed['page_offset']},
                  page_refs)
    for index, (x, patch_y, width, height) in enumerate(CROP_REGIONS):
        y = patch_y + 1
        if x + width > 128 or y + height > 128:
            raise ImageFormatError('numeric crop region outside source page')
        refs = []
        for frame in parsed['frames']:
            for part in frame['parts']:
                if part['page_index'] != 0:
                    continue
                left, top = max(x, part['u']), max(y, part['v'])
                right, bottom = min(x + width, part['u'] + part['width']), min(y + height, part['v'] + part['height'])
                if left < right and top < bottom:
                    refs.append({'frame_index': frame['frame_index'], 'part_index': part['part_index'],
                                 'source_record_offset': part['source_record_offset'],
                                 'intersection': [left, top, right - left, bottom - top],
                                 'palette_row': part['palette_row']})
        indices = b''.join(page[(y + row) * 128 + x:(y + row) * 128 + x + width]
                           for row in range(height))
        write_preview(f'{asset_id}.region-{index:02d}', f'locator-region-{index:02d}',
                      'source_region_crop', width, height, indices,
                      {'x': x, 'y': y, 'width': width, 'height': height,
                       'source_offset': parsed['page_offset'] + y * 128 + x,
                       'numeric_locator_region': [x, patch_y, width, height],
                       'locator_to_source_y_delta': 1}, refs)
    return {'status': 'exported', 'format': 'tokimemo-rpg-bat-ui', 'files': files, 'images': images,
            'metadata': {'profile': parsed['name'], 'encounter_flag': parsed['encounter_flag'],
                         'source_sha256': hashlib.sha256(data).hexdigest(), 'source_size': len(data),
            'source_encoding': 'raw 128x128 8bpp page and independent numeric locator regions',
                         'page_source_offset': parsed['page_offset'], 'palette_source_offset': parsed['palette_offset'],
                         'frame_table_source_offset': PROFILES[parsed['name']][2], 'frame_count': len(parsed['frames']),
                         'source_evidence': ['src/ovl/rpg_battle/battle_asset_pointers.c',
                                             'src/ovl/rpg_battle/battle_sprites_and_hud.c',
                                             'src/ovl/rpg_battle/battle_palette_loading.c',
                                             'src/main/frame_render_audio.c:csr_load_vram/palette_load_vram'],
                         'coverage_policy': 'one C-backed encounter page plus eight numeric regions; no semantic label assignment',
                         'unrepresented': ['other pages and palette rows', 'other BATTLE files without exact C profile',
                                           'runtime actor composition/animation and screen placement',
                                           'hardware raster parity and round trip'],
                         'round_trip_verified': False, 'hardware_raster_parity_verified': False}}
