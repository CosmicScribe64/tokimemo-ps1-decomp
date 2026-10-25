"""Original FILESEL.NBN pages and static UI frames (T-28002).

Independently derived from NAME_ENT/file_select.c read/setup/draw bodies.
The source establishes twelve 8bpp background pages, four actor source pages,
45 static frames, and two unchanged UI palettes. No translation/patch data is
used. The shared BG helper's 120-vs-128 row sampling is retained as unresolved.
"""
import hashlib
import re
import struct
from pathlib import Path
from .image_packs import _reader
from .images import ImageFormatError, _color, _png

FILESEL_SIZE = 0x44800
ACTOR_SHEETS = (0x1a000, 0x3c000, 0x1e000, 0x40550)
BACKGROUND_GROUPS = (('primary', 0x2000, 0, 0x1ff, 0xd),
                     ('information', 0x24000, 0x22000, 0x1f0, 0xa))
HIGHLIGHT_FRAMES = tuple(range(4, 18)) + (30, 31)


def _parse(data, exe=None):
    if len(data) != FILESEL_SIZE:
        raise ImageFormatError('FILESEL profile requires exactly 0x44800 source bytes')
    if exe is not None:
        _reader(exe)
    if struct.unpack_from('<4I', data, 0x4047c) != tuple(0x80180000 + x for x in ACTOR_SHEETS):
        raise ImageFormatError('FILESEL actor sheet-pointer profile differs')
    frames = []
    for index, pointer in enumerate(struct.unpack_from('<45I', data, 0x4048c)):
        offset = pointer - 0x80180000
        if not 0x40000 <= offset <= 0x4047c - 8:
            raise ImageFormatError('FILESEL frame pointer outside source geometry region')
        count, ox, oy, flags = struct.unpack_from('<4h', data, offset)
        if not 1 <= count <= 64 or offset + 8 + count * 14 > 0x4047c:
            raise ImageFormatError('FILESEL frame extent outside source geometry region')
        parts = []
        for number in range(count):
            record = offset + 8 + number * 14
            x, y, texture, palette, flip, width, height = struct.unpack_from('<2h3H2h', data, record)
            sheet = texture >> 6
            u, v = (texture & 7) * 16, ((texture & 63) >> 3) * 16
            if sheet >= 4 or (palette >> 8) & 15 or flip & ~3 or not 0 < width <= 128 or not 0 < height <= 128 or u + width > 128 or v + height > 128:
                raise ImageFormatError('unsupported FILESEL part geometry/base palette')
            parts.append({'part_index': number, 'source_record_offset': record, 'x': x, 'y': y,
                          'width': width, 'height': height, 'texture_id': texture,
                          'sheet_index': sheet, 'sheet_source_offset': ACTOR_SHEETS[sheet],
                          'u': u, 'v': v, 'palette_info': palette, 'flip_flags': flip})
        left, top = min(p['x'] for p in parts), min(p['y'] for p in parts)
        width = max(p['x'] + p['width'] for p in parts) - left
        height = max(p['y'] + p['height'] for p in parts) - top
        if width > 640 or height > 512:
            raise ImageFormatError('FILESEL composed frame exceeds preview bound')
        frames.append({'frame_index': index, 'source_frame_offset': offset, 'origin': [ox, oy],
                       'frame_flags': flags, 'bounds': [left, top, width, height], 'parts': parts})
    total = sum(f['bounds'][2] * f['bounds'][3] * (2 if f['frame_index'] in HIGHLIGHT_FRAMES else 1) for f in frames)
    if total > 16 * 1024 * 1024:
        raise ImageFormatError('FILESEL preview corpus exceeds 16-million-pixel budget')
    return frames


def probe_filesel_ui(data: bytes, exe=None) -> dict:
    """Validate the fixed original file-selection UI format and source palettes."""
    try:
        frames = _parse(data, exe)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'tokimemo-filesel-ui', 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-filesel-ui',
            'metadata': {'background_pages': 12, 'static_frames': 45,
                         'parts': sum(len(f['parts']) for f in frames), 'proved_highlight_variants': 16}}


def export_filesel_ui(data: bytes, exe, output_dir, asset_id: str) -> dict:
    """Export established original palettes; no speculative color variants."""
    frames = _parse(data, exe)
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe FILESEL asset_id')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    images, files = [], []
    palettes = {offset: struct.unpack_from('<256H', data, offset) for offset in (0, 0x22000, 0x22200)}
    def publish(name, width, height, words, detail):
        png, mask = f'{asset_id}.{name}.png', f'{asset_id}.{name}.stp.png'
        for path, payload in ((png, _png(width, height, b''.join(_color(w) for w in words), 4)),
                              (mask, _png(width, height, bytes(255 if w & 0x8000 else 0 for w in words), 1))):
            with (output_dir / path).open('xb') as output:
                output.write(payload)
            files.append(path)
        images.append(dict(detail, png=png, stp_png=mask, width=width, height=height,
                           variant=name, palette_status='established by file_select_init_menus; mode1 unchanged'))
    for group, start, palette_offset, clut_y, first_slot in BACKGROUND_GROUPS:
        palette = palettes[palette_offset]
        for index in range(6):
            offset = start + index * 0x4000
            row, column = divmod(index, 3)
            publish(f'source-page-{group}-{index}', 128, 128,
                    [palette[value] for value in data[offset:offset + 0x4000]],
                    {'frame_index': None, 'parts': [], 'preview_kind': 'source_texture_page',
                     'source_sheet_offset': offset, 'source_sheet_size': 0x4000,
                     'source_sheet_sha256': hashlib.sha256(data[offset:offset + 0x4000]).hexdigest(),
                     'palette_source_offset': palette_offset, 'source_palette_words': list(palette),
                     'clut_vram': [0, clut_y], 'upload_slot': first_slot + column + row * 32,
                     'vram_rectangle_words': [(first_slot + column) * 64, row * 128, 64, 128],
                     'runtime_bg_cell': {'map_column': column, 'map_row': row, 'u': 0,
                                         'v': row * 120, 'width': 128, 'height': 120,
                                         'tpage': first_slot + column},
                     'composition_status': 'unresolved: source upload row1 starts128 while map v uses120'})
    for frame in frames:
        for highlight in (False, True) if frame['frame_index'] in HIGHLIGHT_FRAMES else (False,):
            palette_offset, clut_y = (0x22200, 0x1f1) if highlight else (0x22000, 0x1f0)
            palette = palettes[palette_offset]
            left, top, width, height = frame['bounds']
            words = [0] * (width * height)
            for part in reversed(frame['parts']):
                for y in range(part['height']):
                    sy = part['height'] - y - 1 if part['flip_flags'] & 1 else y
                    for x in range(part['width']):
                        sx = part['width'] - x - 1 if part['flip_flags'] & 2 else x
                        pixel = (part['v'] + sy) * 128 + part['u'] + sx
                        word = palette[data[part['sheet_source_offset'] + pixel]]
                        if word:
                            words[(part['y'] - top + y) * width + part['x'] - left + x] = word
            publish(f'frame-{frame["frame_index"]:02d}' + ('.selected' if highlight else '.base'),
                    width, height, words, dict(frame, preview_kind='static_frame_composite',
                    palette_source_offset=palette_offset, source_palette_words=list(palette),
                    clut_vram=[0, clut_y], palette_override=(0x11 if highlight else 0),
                    runtime_selection_evidence=('file_select_slot_summary_draw and file_select_init_menus set override0x11 only on slot/status actors'
                                                if highlight else 'file_select_init_menus initializes frame palette_mode0x80/override0'),
                    render_policy='source frame geometry plus source-proved base/selected palette; dynamic strings, fades, iris and external icons not composed'))
    def extent(start, end, status):
        return {'offset': start, 'size': end - start, 'sha256': hashlib.sha256(data[start:end]).hexdigest(), 'status': status}
    cursor, gaps = 0x40000, []
    for start, end in sorted((f['source_frame_offset'], f['source_frame_offset'] + 8 + len(f['parts']) * 14) for f in frames):
        if start > cursor:
            gaps.append(extent(cursor, start, 'opaque bytes outside parsed actor frame records'))
        cursor = max(cursor, end)
    if cursor < 0x4047c:
        gaps.append(extent(cursor, 0x4047c, 'opaque bytes outside parsed actor frame records'))
    metadata = {'source_sha256': hashlib.sha256(data).hexdigest(), 'source_size': len(data),
                'source_path_profile': 'CDROM/FUKUTEST/FILESEL.NBN', 'source_lba': 0x9cb5,
                'runtime_load_address': 0x80180000, 'bits_per_pixel': 8,
                'actor_sheet_offsets': list(ACTOR_SHEETS), 'actor_texture_table_offset': 0x4047c,
                'actor_frame_table_offset': 0x4048c, 'actor_frame_count': 45,
                'palette_source_offsets': [0, 0x22000, 0x22200], 'palette_transform': 'load_palette mode1: source STP/color words unchanged',
                'alpha_policy': 'BGR555 word zero transparent; all other words opaque',
                'stp_policy': 'separate source bit15 mask; runtime sprite blending external',
                'proved_selected_palette_frame_indices': list(HIGHLIGHT_FRAMES),
                'unrepresented_extents': [extent(0x200, 0x2000, 'palette-area bytes not uploaded by this setup'),
                                         extent(0x22400, 0x24000, 'palette-area bytes not uploaded by this setup'),
                                         extent(0x40540, 0x40550, 'unused static-actor sequence/header words; uninterpreted'),
                                         extent(0x44550, len(data), 'opaque post-page trailer')],
                'uninterpreted_frame_record_gaps': gaps,
                'partially_represented_extents': [extent(offset, offset + 0x4000, 'only actor part rectangles represented; remaining texels unresolved') for offset in sorted(ACTOR_SHEETS)],
                'external_runtime_assets': [{'lba': 0x9dba, 'sector_count': 5, 'load_address': 0x801e0000,
                                             'status': 'separate memory-card icon texture/palette; outside FILESEL'},
                                            {'source': 'MAINROM+main executable', 'status': 'shared star actors not composed here'},
                                            {'source': 'NAME_ENT overlay and runtime state', 'status': 'digit UV table and runtime strings are outside this source pack'}],
                'unrepresented_runtime_uses': [{'symbol': 'file_select_number_draw', 'source_sheet_offset': 0x40550,
                                               'clut_vram': [0, 0x1f0],
                                               'status': 'direct digit sprites use overlay UV table; separate digit crops not mapped here'}],
                'coverage_policy': '12 source background pages,45 base actor frames and16 source-proved selected variants; background map sampling, unused texels, external icons and runtime composition remain unresolved',
                'round_trip_verified': False,
                'evidence': ['src/ovl/NAME_ENT/file_select.c:file_select_init_assets,file_select_init_menus,file_select_slot_summary_draw',
                             'src/main/actor_sprites.c:actor_draw_all', 'src/main/frame_render_audio.c:palette_load_vram',
                             'src/main/gpu_effects.c:func_8004A1A0']}
    if exe is not None:
        metadata['paired_exe_sha256'] = hashlib.sha256(exe).hexdigest()
    return {'status': 'exported', 'format': 'tokimemo-filesel-ui', 'files': files, 'images': images, 'metadata': metadata}
