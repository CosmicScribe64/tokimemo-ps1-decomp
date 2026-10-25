"""Source-proved TITLE.NBN color pages and actor frames (T-28002).

The title loader and gpu_effects.c establish the background pages and CLUTs.
Actor frames use their actual row 0x1fe/0x1ff CLUTs. Model texture CLUT/TMD
associations and the background map's 120-vs-128 row sampling remain unresolved;
this module does not guess either composition or palette assignment.
"""
import hashlib
import re
import struct
from pathlib import Path
from .image_packs import _reader
from .images import ImageFormatError, _color, _png

TITLE_SIZE = 0x6b000
ACTOR_SHEETS = (0x52200, 0x5a200, 0x62200, 0x4e200, 0x56200, 0x5e200, 0x66200)
BACKGROUND_SHEETS = tuple(0x34000 + i * 0x4000 for i in range(6))


def _parse(data, exe=None):
    if len(data) != TITLE_SIZE:
        raise ImageFormatError('TITLE profile requires exactly 0x6b000 source bytes')
    if exe is not None:
        _reader(exe)  # paired release header sanity; TITLE carries its own CLUTs.
    if struct.unpack_from('<7I', data, 0x6ab20) != tuple(0x80180000 + x for x in ACTOR_SHEETS):
        raise ImageFormatError('TITLE actor sheet pointer profile differs')
    if struct.unpack_from('<I', data, 0x6ab6c)[0] != 0x801eab00:
        raise ImageFormatError('TITLE actor sequence pointer differs')
    sequence = struct.unpack_from('<16H', data, 0x6ab00)
    if sequence[0] != 7 or any(sequence[1 + i * 2] >= 12 for i in range(7)):
        raise ImageFormatError('TITLE sequence count/frame index outside profile')
    frames = []
    for index, pointer in enumerate(struct.unpack_from('<12I', data, 0x6ab3c)):
        offset = pointer - 0x80180000
        if not 0x6a200 <= offset <= 0x6ab00 - 8:
            raise ImageFormatError('TITLE frame pointer outside known region')
        count, ox, oy, flags = struct.unpack_from('<4h', data, offset)
        if not 1 <= count <= 64 or offset + 8 + count * 14 > 0x6ab00:
            raise ImageFormatError('TITLE frame part extent outside known region')
        parts = []
        for number in range(count):
            record = offset + 8 + number * 14
            x, y, texture, palette, flip, width, height = struct.unpack_from('<2h3H2h', data, record)
            sheet = texture >> 8
            u, v = (texture & 15) * 8, ((texture & 255) >> 4) * 8
            row = (palette >> 8) & 15
            if sheet >= 7 or row not in (14, 15) or flip & ~3 or not 0 < width <= 128 or not 0 < height <= 128 or u + width > 128 or v + height > 128:
                raise ImageFormatError('unsupported TITLE part/palette geometry')
            parts.append({'part_index': number, 'source_record_offset': record, 'x': x, 'y': y,
                          'width': width, 'height': height, 'texture_id': texture,
                          'sheet_index': sheet, 'sheet_source_offset': ACTOR_SHEETS[sheet],
                          'u': u, 'v': v, 'palette_info': palette, 'palette_row_index': row,
                          'palette_source_offset': 0x4c200 + row * 512, 'clut_vram': [0, 0x1f0 + row],
                          'flip_flags': flip})
        left, top = min(p['x'] for p in parts), min(p['y'] for p in parts)
        width = max(p['x'] + p['width'] for p in parts) - left
        height = max(p['y'] + p['height'] for p in parts) - top
        if width > 640 or height > 512:
            raise ImageFormatError('TITLE composed frame exceeds preview bound')
        frames.append({'frame_index': index, 'source_frame_offset': offset, 'origin': [ox, oy],
                       'frame_flags': flags, 'parts': parts, 'bounds': [left, top, width, height]})
    return frames, {'source_offset': 0x6ab00, 'steps': [{'frame_index': sequence[i * 2 + 1], 'duration': sequence[i * 2 + 2]} for i in range(7)],
                    'uninterpreted_final_halfword': sequence[-1]}


def probe_title_ui(data: bytes, exe=None) -> dict:
    """Validate the fixed TITLE profile without inventing a model palette map."""
    try:
        frames, _ = _parse(data, exe)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'tokimemo-title-ui', 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-title-ui',
            'metadata': {'background_pages': 6, 'actor_frames': 12,
                         'actor_parts': sum(len(f['parts']) for f in frames), 'unresolved_model_pages': 13}}


def _palette(data, offset, mode):
    original = struct.unpack_from('<256H', data, offset)
    if mode == 3:
        transformed = tuple(word | 0x8000 if word & 0x7fff else word for word in original)
    else:
        transformed = tuple(word & 0x7fff if i < 240 else word | 0x8000 if word & 0x7fff else word
                            for i, word in enumerate(original))
    return original, transformed


def export_title_ui(data: bytes, exe, output_dir, asset_id: str) -> dict:
    """Export only source-established TITLE palettes/pages and actor composites."""
    frames, sequence = _parse(data, exe)
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe TITLE asset_id')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    images, files = [], []
    def publish(variant, width, height, words, detail):
        png, mask = f'{asset_id}.{variant}.png', f'{asset_id}.{variant}.stp.png'
        for path, payload in ((png, _png(width, height, b''.join(_color(w) for w in words), 4)),
                              (mask, _png(width, height, bytes(255 if w & 0x8000 else 0 for w in words), 1))):
            with (output_dir / path).open('xb') as output:
                output.write(payload)
            files.append(path)
        images.append(dict(detail, png=png, stp_png=mask, width=width, height=height, variant=variant,
                           palette_status='established by TITLE loader and source part CLUT'))
    raw, background_palette = _palette(data, 0x4c000, 3)
    for index, offset in enumerate(BACKGROUND_SHEETS):
        words = [background_palette[value] for value in data[offset:offset + 0x4000]]
        row, column = divmod(index, 3)
        publish(f'source-page-background-{index}', 128, 128, words,
                {'frame_index': None, 'parts': [], 'preview_kind': 'source_texture_page',
                 'source_sheet_offset': offset, 'source_sheet_size': 0x4000,
                 'source_sheet_sha256': hashlib.sha256(data[offset:offset + 0x4000]).hexdigest(),
                 'palette_source_offset': 0x4c000, 'source_palette_words': list(raw),
                 'preview_palette_words': list(background_palette), 'clut_vram': [0, 0x1ef],
                 'upload_slot': 0xd + column + row * 32,
                 'vram_rectangle_words': [(0xd + column) * 64, row * 128, 64, 128],
                 'runtime_bg_cell': {'map_column': column, 'map_row': row, 'u': 0, 'v': row * 120,
                                     'width': 128, 'height': 120, 'tpage': 0xd + column},
                 'composition_status': 'unresolved: source uploads row1 at128 while BG cell v uses120'})
    palettes = {row: _palette(data, 0x4c200 + row * 512, 2) for row in (14, 15)}
    for frame in frames:
        left, top, width, height = frame['bounds']
        composed = [0] * (width * height)
        parts = []
        for part in reversed(frame['parts']):
            raw, palette = palettes[part['palette_row_index']]
            parts.append(dict(part, source_palette_words=list(raw), preview_palette_words=list(palette)))
            for y in range(part['height']):
                sy = part['height'] - y - 1 if part['flip_flags'] & 1 else y
                for x in range(part['width']):
                    sx = part['width'] - x - 1 if part['flip_flags'] & 2 else x
                    pixel = (part['v'] + sy) * 128 + part['u'] + sx
                    word = palette[data[part['sheet_source_offset'] + pixel]]
                    if word:
                        composed[(part['y'] - top + y) * width + part['x'] - left + x] = word
        publish(f'frame-{frame["frame_index"]:02d}', width, height, composed,
                dict(frame, parts=list(reversed(parts)), preview_kind='static_frame_composite',
                     render_policy='source frame geometry and loaded part CLUTs; no scene brightness/blend/3D composition'))
    def extent(offset, size, status):
        return {'offset': offset, 'size': size, 'sha256': hashlib.sha256(data[offset:offset + size]).hexdigest(), 'status': status}
    # Frame records are halfword-sized and commonly padded to word boundaries.
    # Preserve every uncovered byte as an opaque extent, rather than implicitly
    # treating the full record region as decoded image/geometry content.
    frame_gaps = []
    cursor = 0x6a200
    record_extents = sorted((frame['source_frame_offset'],
                             frame['source_frame_offset'] + 8 + len(frame['parts']) * 14)
                            for frame in frames)
    for start, end in record_extents:
        if start > cursor:
            frame_gaps.append(extent(cursor, start - cursor, 'opaque bytes outside parsed frame records'))
        cursor = max(cursor, end)
    if cursor < 0x6ab00:
        frame_gaps.append(extent(cursor, 0x6ab00 - cursor, 'opaque bytes outside parsed frame records'))
    metadata = {'source_sha256': hashlib.sha256(data).hexdigest(), 'source_size': len(data),
                'source_path_profile': 'CDROM/FUKUTEST/TITLE.NBN', 'source_lba': 0xa0cc,
                'runtime_load_address': 0x80180000, 'bits_per_pixel': 8,
                'background_sheet_offsets': list(BACKGROUND_SHEETS), 'actor_sheet_offsets': list(ACTOR_SHEETS),
                'actor_texture_table_offset': 0x6ab20, 'actor_frame_table_offset': 0x6ab3c,
                'actor_sequence_pointer_offset': 0x6ab6c, 'source_sequence': sequence,
                'alpha_policy': 'BGR555 word zero transparent; all other words opaque',
                'stp_policy': 'separate bit15 mask; source mode3 sets nonblack background STP; actor mode2 clears first240 and sets nonblack last16; runtime blending external',
                'unrepresented_extents': [extent(0, 0x34000, '13 model texture pages; actual TMD-to-CLUT mapping unresolved'),
                                         extent(0x4c200, 14 * 512, 'palette row0 not loaded; rows1..13 uploaded for models but texture/TMD binding unresolved'),
                                         extent(0x6ab70, len(data) - 0x6ab70, 'opaque post-table trailer')],
                'uninterpreted_frame_record_gaps': frame_gaps,
                'partially_represented_extents': [extent(offset, 0x4000, 'only actor frame rectangles represented; remaining sheet texels unresolved') for offset in sorted(ACTOR_SHEETS)],
                'unmapped_model_palette_uploads': [{'source_offset': 0x4c200 + row * 512,
                                                   'source_size': 512, 'clut_vram': [0, 0x1ef + row],
                                                   'mode': 2, 'texture_binding_status': 'unresolved'}
                                                  for row in range(1, 14)],
                'coverage_policy': '6 background source pages and12 actor frames only; 13 model pages, unrepresented actor texels and BG map sampling remain unresolved',
                'round_trip_verified': False,
                'evidence': ['src/main/title_screen.c:func_80061A3C', 'src/main/gpu_effects.c:func_8004A1A0',
                             'src/main/actor_sprites.c:actor_draw_all', 'src/main/frame_render_audio.c:palette_load_vram']}
    if exe is not None:
        metadata['paired_exe_sha256'] = hashlib.sha256(exe).hexdigest()
    return {'status': 'exported', 'format': 'tokimemo-title-ui', 'files': files, 'images': images, 'metadata': metadata}
