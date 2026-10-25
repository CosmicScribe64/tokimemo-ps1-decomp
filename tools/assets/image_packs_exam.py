"""Original EXAMINE.NBN actor frames with paired executable CLUTs (T-28002).

The exam-results loader fixes the source extent and three actor tables. The
actor renderer establishes 4bpp texels, eight-pixel cells and default CLUTs.
This reader previews static frame geometry, not the runtime score/name screen.
Run in Docker; no external libraries or fan-patch implementation are used.
"""
import hashlib
from pathlib import Path
import re
import struct

from .image_packs import _reader
from .images import ImageFormatError, _color, _png

SOURCE_SIZE = 0x6800
LOAD_BASE = 0x801E0000
FRAME_COUNT = 32
PAGE_OFFSETS = (0, 0x2000, 0x4000)


def _parse(data, exe):
    if len(data) != SOURCE_SIZE:
        raise ImageFormatError('EXAMINE profile requires exactly 0x6800 source bytes')
    read, exe_base = _reader(exe)
    pages = tuple(value - LOAD_BASE for value in struct.unpack_from('<3I', data, 0x6494))
    if set(pages) != set(PAGE_OFFSETS):
        raise ImageFormatError('EXAMINE texture pointers differ from three source pages')
    frames = []
    for index, address in enumerate(struct.unpack_from('<32I', data, 0x64A0)):
        offset = address - LOAD_BASE
        if not 0x6000 <= offset <= 0x6494 - 8:
            raise ImageFormatError('EXAMINE frame pointer outside record region')
        count, ox, oy, flags = struct.unpack_from('<4h', data, offset)
        if not 1 <= count <= 64 or offset + 8 + count * 14 > 0x6494:
            raise ImageFormatError('EXAMINE frame extent outside record region')
        parts = []
        for number in range(count):
            record = offset + 8 + number * 14
            x, y, texture, palette_info, flip, width, height = struct.unpack_from('<2h3H2h', data, record)
            page = texture >> 8
            u, v = (texture & 15) * 8, ((texture & 255) >> 4) * 8
            if (page >= len(pages) or flip & ~3 or not 0 < width <= 128
                    or not 0 < height <= 128 or u + width > 128 or v + height > 128):
                raise ImageFormatError('unsupported EXAMINE part geometry or flags')
            row, column = (palette_info >> 8) & 15, palette_info & 0xF0
            palette_address = 0x800C6020 + row * 512 + column * 2
            original = struct.unpack('<16H', read(palette_address, 32))
            transformed = tuple(word | 0x8000 if word & 0x7FFF and
                                (0x800C6200 <= palette_address < 0x800C6220
                                 or 0x800C62C0 <= palette_address < 0x800C6340)
                                else word for word in original)
            parts.append({'part_index': number, 'source_record_offset': record,
                          'x': x, 'y': y, 'width': width, 'height': height,
                          'texture_id': texture, 'sheet_index': page,
                          'sheet_source_offset': pages[page], 'u': u, 'v': v,
                          'palette_info': palette_info, 'flip_flags': flip,
                          'clut_vram': [column, 0x1E0 + row],
                          'palette_exe_address': palette_address,
                          'palette_exe_offset': palette_address - exe_base + 0x800,
                          'source_palette_words': list(original),
                          'preview_palette_words': list(transformed)})
        left, top = min(p['x'] for p in parts), min(p['y'] for p in parts)
        width = max(p['x'] + p['width'] for p in parts) - left
        height = max(p['y'] + p['height'] for p in parts) - top
        if width > 1024 or height > 512:
            raise ImageFormatError('EXAMINE frame exceeds preview bounds')
        frames.append({'frame_index': index, 'source_frame_offset': offset,
                       'origin': [ox, oy], 'frame_flags': flags,
                       'parts': parts, 'bounds': [left, top, width, height]})
    return pages, frames


def probe_exam_ui(data, exe):
    """Validate only the original source/table profile; report unsupported geometry."""
    try:
        _, frames = _parse(data, exe)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'tokimemo-exam-ui', 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-exam-ui',
            'metadata': {'frames': len(frames), 'parts': sum(len(f['parts']) for f in frames)}}


def export_exam_ui(data, exe, output_dir, asset_id):
    """Export per-part original-palette frames, preserving unknown bytes separately."""
    pages, frames = _parse(data, exe)
    if not isinstance(asset_id, str) or not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe EXAMINE asset_id')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    files, images = [], []
    for frame in frames:
        left, top, width, height = frame['bounds']
        words = [0] * (width * height)
        for part in reversed(frame['parts']):
            for y in range(part['height']):
                sy = part['height'] - y - 1 if part['flip_flags'] & 1 else y
                for x in range(part['width']):
                    sx = part['width'] - x - 1 if part['flip_flags'] & 2 else x
                    pixel = (part['v'] + sy) * 128 + part['u'] + sx
                    packed = data[part['sheet_source_offset'] + pixel // 2]
                    word = part['preview_palette_words'][(packed >> (4 * (pixel & 1))) & 15]
                    if word:
                        words[(part['y'] - top + y) * width + part['x'] - left + x] = word
        stem = f'{asset_id}.frame-{frame["frame_index"]:02d}'
        png, mask = stem + '.png', stem + '.stp.png'
        for filename, payload in ((png, _png(width, height, b''.join(_color(w) for w in words), 4)),
                                  (mask, _png(width, height, bytes(255 if w & 0x8000 else 0 for w in words), 1))):
            with (output_dir / filename).open('xb') as output:
                output.write(payload)
            files.append(filename)
        images.append(dict(frame, width=width, height=height, png=png, stp_png=mask,
                           variant='base', preview_kind='static_frame_composite',
                           palette_status='original frame-part default CLUT from paired executable'))
    def extent(offset, size, status):
        return {'offset': offset, 'size': size, 'sha256': hashlib.sha256(data[offset:offset + size]).hexdigest(), 'status': status}
    gaps, cursor = [], 0x6000
    for start, end in sorted((f['source_frame_offset'], f['source_frame_offset'] + 8 + len(f['parts']) * 14) for f in frames):
        if start > cursor:
            gaps.append(extent(cursor, start - cursor, 'opaque bytes outside parsed frame records'))
        cursor = max(cursor, end)
    if cursor < 0x6494:
        gaps.append(extent(cursor, 0x6494 - cursor, 'opaque bytes outside parsed frame records'))
    metadata = {'source_sha256': hashlib.sha256(data).hexdigest(), 'source_size': len(data),
                'paired_exe_sha256': hashlib.sha256(exe).hexdigest(), 'paired_exe_size': len(exe),
                'source_path_profile': 'CDROM/FUKUTEST/EXAMINE.NBN', 'source_lba': 0x9CA8,
                'runtime_load_address': LOAD_BASE, 'bits_per_pixel': 4,
                'sheet_offsets': list(pages), 'sheet_size': [128, 128], 'sheet_bytes': 0x2000,
                'texture_pointer_table_offset': 0x6494, 'frame_table_offset': 0x64A0,
                'sequence_pointer_table_offset': 0x6520,
                'uninterpreted_frame_record_gaps': gaps,
                'partially_represented_extents': [extent(offset, 0x2000, 'only frame-part texels represented; unused texels unresolved') for offset in sorted(pages)],
                'unrepresented_extents': [extent(0x6520, SOURCE_SIZE - 0x6520, 'sequence data, padding and trailer not interpreted')],
                'alpha_policy': 'BGR555 word zero transparent; every other word opaque',
                'stp_policy': 'paired executable default CLUTs with boot STP edits; separate bit15 mask; runtime blending external',
                'coverage_policy': '32 static actor frames only; runtime score digits, player/girl names, animation and unused source texels unresolved',
                'round_trip_verified': False,
                'evidence': ['src/ovl/ETC/final_exam.c:func_8014790C (read, actor depth/cell mode and tables)',
                             'src/main/actor_sprites.c:actor_draw_all,search_tpage8',
                             'src/main/boot_assets.c:func_80059EF0 (shared default CLUTs and STP edits)']}
    return {'status': 'exported', 'format': 'tokimemo-exam-ui', 'images': images, 'files': files, 'metadata': metadata}
