"""Original NAME_ENT.NBN name-input frames and background pages (T-28002).

Derived from NAME_ENT/name_entry.c setup bodies, without patch code/data.
Actors are 4bpp with eight-pixel cells. Their explicit CLUT-row override 0x10
selects row 0x1f0 while preserving the part's 16-color column; encoded row
bits must not select another palette. Background source pages are 8bpp.
"""
import hashlib
import re
import struct
from pathlib import Path
from .image_packs import _reader
from .images import ImageFormatError, _color, _png

NAME_SIZE = 0x1b000
LOAD_BASE = 0x80190000
FRAME_COUNT = 29
ACTOR_PAGE_OFFSET = 0x200


def _mode2_palette(data, offset):
    raw = struct.unpack_from('<256H', data, offset)
    preview = tuple(word & 0x7fff if i < 240 else word | 0x8000 if word & 0x7fff else word
                    for i, word in enumerate(raw))
    return raw, preview


def _parse(data, exe=None):
    if len(data) != NAME_SIZE:
        raise ImageFormatError('NAME_ENT profile requires exactly 0x1b000 source bytes')
    if exe is not None:
        _reader(exe)
    page = struct.unpack_from('<I', data, 0x1af58)[0] - LOAD_BASE
    if page != ACTOR_PAGE_OFFSET or page + 0x2000 > len(data):
        raise ImageFormatError('NAME_ENT 4bpp actor-page pointer differs from original profile')
    frames = []
    for index, pointer in enumerate(struct.unpack_from('<29I', data, 0x1af5c)):
        offset = pointer - LOAD_BASE
        if not 0x1a400 <= offset <= 0x1af58 - 8:
            raise ImageFormatError('NAME_ENT frame pointer outside record region')
        count, ox, oy, flags = struct.unpack_from('<4h', data, offset)
        if not 0 <= count <= 128 or offset + 8 + count * 14 > 0x1af58:
            raise ImageFormatError('NAME_ENT frame extent outside record region')
        parts = []
        for number in range(count):
            record = offset + 8 + number * 14
            x, y, texture, palette_info, flip, width, height = struct.unpack_from('<2h3H2h', data, record)
            u, v = (texture & 15) * 8, ((texture & 255) >> 4) * 8
            if texture >> 8 or flip & ~3 or not 0 < width <= 128 or not 0 < height <= 128 or u + width > 128 or v + height > 128:
                raise ImageFormatError('unsupported NAME_ENT 4bpp part geometry')
            column = palette_info & 0xf0
            parts.append({'part_index': number, 'source_record_offset': record,
                          'x': x, 'y': y, 'width': width, 'height': height,
                          'texture_id': texture, 'sheet_index': 0, 'sheet_source_offset': page,
                          'u': u, 'v': v, 'palette_info': palette_info, 'flip_flags': flip,
                          'palette_source_offset': column * 2, 'clut_vram': [column, 0x1f0],
                          'runtime_palette_override': 0x10, 'runtime_palette_mode': 0x81})
        if parts:
            left, top = min(p['x'] for p in parts), min(p['y'] for p in parts)
            width = max(p['x'] + p['width'] for p in parts) - left
            height = max(p['y'] + p['height'] for p in parts) - top
        else:
            left = top = width = height = 0
        if width > 640 or height > 512:
            raise ImageFormatError('NAME_ENT composed frame exceeds preview bound')
        frames.append({'frame_index': index, 'source_frame_offset': offset, 'origin': [ox, oy],
                       'frame_flags': flags, 'parts': parts, 'bounds': [left, top, width, height]})
    return page, frames


def probe_name_ui(data: bytes, exe=None) -> dict:
    """Validate original name-input tables and runtime palette selection rules."""
    try:
        page, frames = _parse(data, exe)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'tokimemo-name-ui', 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-name-ui',
            'metadata': {'frames': FRAME_COUNT, 'parts': sum(len(f['parts']) for f in frames),
                         'actor_page_offset': page, 'background_pages': 6,
                         'empty_source_frames': [f['frame_index'] for f in frames if not f['parts']]}}


def export_name_ui(data: bytes, exe, output_dir, asset_id: str) -> dict:
    """Export source-established name-entry CLUTs without inventing glyph text."""
    page, frames = _parse(data, exe)
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe NAME_ENT asset_id')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    images, files = [], []
    raw_actor, actor_palette = _mode2_palette(data, 0)
    raw_background, background_palette = _mode2_palette(data, 0x1a200)
    def publish(variant, width, height, words, detail):
        png, mask = f'{asset_id}.{variant}.png', f'{asset_id}.{variant}.stp.png'
        for name, payload in ((png, _png(width, height, b''.join(_color(w) for w in words), 4)),
                              (mask, _png(width, height, bytes(255 if w & 0x8000 else 0 for w in words), 1))):
            with (output_dir / name).open('xb') as output:
                output.write(payload)
            files.append(name)
        images.append(dict(detail, png=png, stp_png=mask, width=width, height=height,
                           variant=variant, palette_status='established by name_entry_background_setup and actor initialization'))
    for index in range(6):
        offset = 0x2200 + index * 0x4000
        row, column = divmod(index, 3)
        publish(f'source-page-background-{index}', 128, 128,
                [background_palette[v] for v in data[offset:offset + 0x4000]],
                {'frame_index': None, 'parts': [], 'preview_kind': 'source_texture_page',
                 'source_sheet_offset': offset, 'source_sheet_size': 0x4000,
                 'source_sheet_sha256': hashlib.sha256(data[offset:offset + 0x4000]).hexdigest(),
                 'bits_per_pixel': 8, 'palette_source_offset': 0x1a200,
                 'source_palette_words': list(raw_background), 'preview_palette_words': list(background_palette),
                 'clut_vram': [0, 0x1f8], 'upload_slot': 0xd + column + row * 32,
                 'vram_rectangle_words': [(0xd + column) * 64, row * 128, 64, 128],
                 'runtime_bg_cell': {'map_column': column, 'map_row': row, 'u': 0, 'v': row * 120,
                                     'width': 128, 'height': 120, 'tpage': 0xd + column},
                 'composition_status': 'unresolved: source upload row1 starts128 while map v uses120'})
    for frame in frames:
        if not frame['parts']:
            continue
        left, top, width, height = frame['bounds']
        words = [0] * (width * height)
        rendered_parts = []
        for part in reversed(frame['parts']):
            column = part['clut_vram'][0]
            palette = actor_palette[column:column + 16]
            rendered_parts.append(dict(part, source_palette_words=list(raw_actor[column:column + 16]),
                                       preview_palette_words=list(palette)))
            for y in range(part['height']):
                sy = part['height'] - y - 1 if part['flip_flags'] & 1 else y
                for x in range(part['width']):
                    sx = part['width'] - x - 1 if part['flip_flags'] & 2 else x
                    pixel = (part['v'] + sy) * 128 + part['u'] + sx
                    packed = data[page + pixel // 2]
                    word = palette[(packed >> (4 * (pixel & 1))) & 15]
                    if word:
                        words[(part['y'] - top + y) * width + part['x'] - left + x] = word
        publish(f'frame-{frame["frame_index"]:02d}', width, height, words,
                dict(frame, parts=list(reversed(rendered_parts)), bits_per_pixel=4,
                     preview_kind='static_frame_composite',
                     render_policy='explicit runtime CLUT-row override0x10 with source column; no dynamic strings, font glyphs, names or screen composition'))
    def extent(start, end, status):
        return {'offset': start, 'size': end - start, 'sha256': hashlib.sha256(data[start:end]).hexdigest(), 'status': status}
    cursor, gaps = 0x1a400, []
    for start, end in sorted((f['source_frame_offset'], f['source_frame_offset'] + 8 + len(f['parts']) * 14) for f in frames):
        if start > cursor:
            gaps.append(extent(cursor, start, 'opaque bytes outside parsed frame records'))
        cursor = max(cursor, end)
    if cursor < 0x1af58:
        gaps.append(extent(cursor, 0x1af58, 'opaque bytes outside parsed frame records'))
    metadata = {'source_sha256': hashlib.sha256(data).hexdigest(), 'source_size': len(data),
                'source_path_profile': 'CDROM/NAME_ENT/NAME_ENT.NBN', 'source_lba': 0xa9e2,
                'runtime_load_address': LOAD_BASE, 'actor_bits_per_pixel': 4, 'background_bits_per_pixel': 8,
                'actor_sheet_offset': page, 'actor_sheet_bytes': 0x2000,
                'actor_texture_table_offset': 0x1af58, 'actor_frame_table_offset': 0x1af5c,
                'actor_frame_count': FRAME_COUNT, 'sequence_table_offset': 0x1afd0,
                'empty_source_frames': [f['frame_index'] for f in frames if not f['parts']],
                'actor_palette_source_offset': 0, 'actor_runtime_palette_override': 0x10,
                'actor_runtime_palette_mode': 0x81, 'background_palette_source_offset': 0x1a200,
                'palette_mapping_scope': 'actor override0x10 is source-proved; palette row0x1f8 is established for background pages only; actor override0x18 variants remain unproved and are not exported',
                'palette_transform': 'load_palette mode2 clears STP first240 words and sets STP on nonblack final16',
                'alpha_policy': 'BGR555 word zero transparent; all other words opaque',
                'stp_policy': 'separate bit15 mask after source-proved mode2; runtime blending external',
                'uninterpreted_frame_record_gaps': gaps,
                'partially_represented_extents': [extent(page, page + 0x2000, 'only actor frame-part rectangles represented; remaining texels unresolved')],
                'unrepresented_extents': [extent(0x1afd0, len(data), 'static-actor sequence data/padding/trailer not interpreted')],
                'external_runtime_assets': [{'source': 'CDROM/NAME_ENT/NAME_SAK.BS', 'lba': 0xaa18,
                                             'status': 'separate source-proved320x240 MDEC background'},
                                            {'source': 'CDROM/FUKUTEST/NAME_KAN.NBN', 'lba': 0x9dbf,
                                             'status': 'candidate-name/string data outside this graphic source'},
                                            {'source': 'NAME_ENT overlay and text renderer',
                                             'status': 'runtime glyph grids, names and strings not composed'}],
                'coverage_policy': '6 original background pages plus nonempty actor frames; dynamic glyph/name/string rendering, background map sampling and unused actor texels remain unresolved',
                'round_trip_verified': False,
                'evidence': ['src/ovl/NAME_ENT/name_entry.c:name_entry_load_init,name_entry_background_setup,name_entry_overlay_tables_set',
                             'src/ovl/NAME_ENT/name_entry.c:name_entry_input_actors_init,name_entry_birthday_actors_init',
                             'src/main/actor_sprites.c:actor_draw_all', 'src/main/frame_render_audio.c:palette_load_vram']}
    if exe is not None:
        metadata['paired_exe_sha256'] = hashlib.sha256(exe).hexdigest()
    return {'status': 'exported', 'format': 'tokimemo-name-ui', 'files': files, 'images': images, 'metadata': metadata}
