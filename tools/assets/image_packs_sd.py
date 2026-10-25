"""Original weekly/holiday/club SD animation frames (T-28002).

Paired executable LBA and u16 layout tables bind each source file. The school
day actor setup proves 8bpp, 16-pixel cells and source palette row 0x1F0.
Only those caller contexts are established; runtime compositing is external.
"""
import hashlib
from pathlib import Path
import re
import struct

from .image_packs import _reader
from .images import ImageFormatError, _color, _png

LOAD_BASE = 0x801E0000
TABLES = (('weekly', 25, 0x800B67AC, 0x800C9F60),
          ('holiday', 13, 0x800B69E0, 0x800C9FF8),
          ('club', 32, 0x800B6810, 0x800CA048))


def sd_profile_rows(exe, *, etc_overlay_data=None):
    """Read executable layouts and optional hash-pinned ETC ELSE LBA selectors."""
    read, _ = _reader(exe)
    rows = []
    for family, count, lba_address, layout_address in TABLES:
        lbas = struct.unpack('<%dI' % count, read(lba_address, count * 4))
        layouts = struct.iter_unpack('<3H', read(layout_address, count * 6))
        for index, (lba, layout) in enumerate(zip(lbas, layouts)):
            if lba == 0:
                continue
            rows.append({'family': family, 'index': index, 'source_lba': lba,
                         'lba_table_exe_address': lba_address + index * 4,
                         'layout_table_exe_address': layout_address + index * 6,
                         'tpage_table_offset': layout[0], 'frame_table_offset': layout[1],
                         'sequence_table_offset': layout[2], 'read_sector_count': 0x29})
    layout = struct.unpack('<3H', read(0x800CA108 + 12, 6))
    rows.append({'family': 'else', 'index': 2, 'source_lba': 0x7D8A,
                 'lba_table_exe_address': None, 'layout_table_exe_address': 0x800CA114,
                 'tpage_table_offset': layout[0], 'frame_table_offset': layout[1],
                 'sequence_table_offset': layout[2], 'read_sector_count': 9})
    if etc_overlay_data is not None:
        from .image_packs_photos import ETC_BASE, ETC_SHA1, ETC_SIZE
        if (len(etc_overlay_data) != ETC_SIZE
                or hashlib.sha1(etc_overlay_data).hexdigest() != ETC_SHA1):
            raise ImageFormatError('ETC overlay does not match the original native ELSE LBA table')
        lba_table_vma = 0x80150720
        offset = lba_table_vma - ETC_BASE
        if offset < 4 or offset + 8 > len(etc_overlay_data):
            raise ImageFormatError('ETC native ELSE LBA table falls outside the overlay')
        lbas = (struct.unpack_from('<I', etc_overlay_data, offset - 4)[0],
                struct.unpack_from('<I', etc_overlay_data, offset)[0])
        for index, lba in enumerate(lbas):
            layout_offset = 0x800CA108 + index * 6
            layout = struct.unpack('<3H', read(layout_offset, 6))
            rows.append({'family': 'else', 'index': index, 'source_lba': lba,
                         'lba_table_exe_address': None,
                         'lba_table_overlay_vma': lba_table_vma + index * 4 - 4,
                         'lba_table_overlay_sha1': ETC_SHA1,
                         'layout_table_exe_address': layout_offset,
                         'tpage_table_offset': layout[0], 'frame_table_offset': layout[1],
                         'sequence_table_offset': layout[2], 'read_sector_count': 9,
                         'native_index_binding': 'exam uses preceding word for summer/index0 layout; table word0 for nonsummer/index1 layout'})
    return rows


def _parse(data, exe, source_entry, *, etc_overlay_data=None):
    if (not isinstance(source_entry, dict) or not isinstance(source_entry.get('path'), str)
            or not isinstance(source_entry.get('location'), dict)):
        raise ImageFormatError('SD source requires original file path and LBA provenance')
    lba = source_entry['location'].get('lba')
    if not isinstance(lba, int) or isinstance(lba, bool):
        raise ImageFormatError('SD source requires an integer original LBA')
    matches = [row for row in sd_profile_rows(exe, etc_overlay_data=etc_overlay_data)
               if row['source_lba'] == lba]
    if not matches:
        raise ImageFormatError('SD source LBA has no caller/layout profile')
    layouts = {(row['tpage_table_offset'], row['frame_table_offset'], row['sequence_table_offset'],
                row['read_sector_count']) for row in matches}
    if len(layouts) != 1:
        raise ImageFormatError('SD source LBA has conflicting caller/layout profiles')
    profile = dict(matches[0], equivalent_caller_profiles=matches)
    if (len(data) != profile['read_sector_count'] * 2048 or len(data) != source_entry.get('size')
            or hashlib.sha256(data).hexdigest() != source_entry.get('sha256')):
        raise ImageFormatError('SD source size/hash differs from original inventory/profile')
    pages_at, frames_at, sequences_at = (profile[key] for key in
                                        ('tpage_table_offset', 'frame_table_offset', 'sequence_table_offset'))
    if (not 0x200 <= pages_at < frames_at < sequences_at < len(data)
            or (frames_at - pages_at) % 4 or (sequences_at - frames_at) % 4):
        raise ImageFormatError('SD layout tables have unsupported order/alignment')
    page_count, frame_count = (frames_at - pages_at) // 4, (sequences_at - frames_at) // 4
    if not 1 <= page_count <= 8 or not 1 <= frame_count <= 256:
        raise ImageFormatError('SD page/frame count exceeds profile bounds')
    pages = [value - LOAD_BASE for value in struct.unpack_from('<%dI' % page_count, data, pages_at)]
    if any(offset < sequences_at + 4 or offset + 0x4000 > len(data) for offset in pages):
        raise ImageFormatError('SD texture-page pointer outside source pixel region')
    ordered_pages = sorted(set(pages))
    if any(right < left + 0x4000 for left, right in zip(ordered_pages, ordered_pages[1:])):
        raise ImageFormatError('SD texture pages overlap partially')
    frames = []
    for index, address in enumerate(struct.unpack_from('<%dI' % frame_count, data, frames_at)):
        offset = address - LOAD_BASE
        if not 0x200 <= offset <= pages_at - 8:
            raise ImageFormatError('SD frame pointer outside source metadata region')
        count, ox, oy, flags = struct.unpack_from('<4h', data, offset)
        if not 0 <= count <= 128 or offset + 8 + count * 14 > pages_at:
            raise ImageFormatError('SD frame extent exceeds source metadata region')
        parts = []
        for part_index in range(count):
            record = offset + 8 + part_index * 14
            x, y, texture, palette_info, flip, width, height = struct.unpack_from('<2h3H2h', data, record)
            page = texture >> 6
            u, v = (texture & 7) * 16, ((texture & 63) >> 3) * 16
            if (page >= len(pages) or flip & ~3 or not 0 < width <= 128 or not 0 < height <= 128
                    or u + width > 128 or v + height > 128):
                raise ImageFormatError('SD part geometry/depth flags outside supported source profile')
            parts.append({'part_index': part_index, 'source_record_offset': record,
                          'x': x, 'y': y, 'width': width, 'height': height,
                          'texture_id': texture, 'sheet_index': page,
                          'sheet_source_offset': pages[page], 'u': u, 'v': v,
                          'palette_info': palette_info, 'flip_flags': flip,
                          'clut_vram': [0, 0x1F0], 'palette_source_offset': 0,
                          'runtime_palette_override': 0x10,
                          'palette_mapping': 'override0x10 and palette_mode2 without bit0x80 select full256-color row at x0; encoded row/column ignored'})
        if parts:
            left, top = min(p['x'] for p in parts), min(p['y'] for p in parts)
            width = max(p['x'] + p['width'] for p in parts) - left
            height = max(p['y'] + p['height'] for p in parts) - top
        else:
            left = top = width = height = 0
        if width > 640 or height > 512:
            raise ImageFormatError('SD composed frame exceeds preview bounds')
        frames.append({'frame_index': index, 'source_frame_offset': offset,
                       'source_frame_size': 8 + count * 14, 'origin': [ox, oy],
                       'frame_flags': flags, 'parts': parts, 'bounds': [left, top, width, height]})
    if sum(frame['bounds'][2] * frame['bounds'][3] for frame in frames) > 16 * 1024 * 1024:
        raise ImageFormatError('SD frame corpus exceeds 16-million-pixel budget')
    frame_extents = sorted({(frame['source_frame_offset'], frame['source_frame_offset'] + frame['source_frame_size']) for frame in frames})
    if any(right[0] < left[1] for left, right in zip(frame_extents, frame_extents[1:])):
        raise ImageFormatError('SD source frame records partially overlap')
    sequences, cursor = [], sequences_at
    while cursor + 4 <= min(pages):
        address = struct.unpack_from('<I', data, cursor)[0]
        offset = address - LOAD_BASE
        if not 0x200 <= offset <= pages_at - 2:
            break
        count = struct.unpack_from('<H', data, offset)[0]
        end = offset + 2 + count * 4
        if not 1 <= count <= 128 or end > pages_at:
            raise ImageFormatError('SD sequence record outside source metadata bounds')
        if any(offset < high and end > low for low, high in frame_extents):
            raise ImageFormatError('SD sequence overlaps a source frame record')
        steps = list(struct.iter_unpack('<HH', data[offset + 2:end]))
        if any(frame >= frame_count for frame, _ in steps):
            raise ImageFormatError('SD sequence refers to unknown source frame')
        sequences.append({'sequence_index': len(sequences), 'source_pointer_offset': cursor,
                          'source_offset': offset, 'source_size': end - offset,
                          'steps': [{'frame_index': frame, 'duration': duration} for frame, duration in steps]})
        cursor += 4
    return profile, pages, frames, sequences, cursor


def probe_sd_ui(data, exe, *, source_entry, etc_overlay_data=None):
    """Validate a caller-bound SD source without scanning for guessed frame tables."""
    try:
        profile, pages, frames, sequences, _ = _parse(data, exe, source_entry,
                                                       etc_overlay_data=etc_overlay_data)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'tokimemo-sd-animation', 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-sd-animation',
            'metadata': {'family': profile['family'], 'profile_index': profile['index'],
                         'pages': len(pages), 'frames': len(frames),
                         'parts': sum(len(f['parts']) for f in frames), 'validated_sequence_prefix': len(sequences)}}


def _uninterpreted(data, known):
    gaps, cursor = [], 0
    for low, high in sorted(known):
        if low > cursor:
            gaps.append({'offset': cursor, 'size': low - cursor,
                         'sha256': hashlib.sha256(data[cursor:low]).hexdigest(),
                         'status': 'uninterpreted bytes outside declared source components'})
        cursor = max(cursor, high)
    if cursor < len(data):
        gaps.append({'offset': cursor, 'size': len(data) - cursor,
                     'sha256': hashlib.sha256(data[cursor:]).hexdigest(),
                     'status': 'uninterpreted bytes outside declared source components'})
    return gaps


def export_sd_ui(data, exe, output_dir, asset_id, *, source_entry, etc_overlay_data=None):
    """Export static original-color SD frames and retain validated sequence records."""
    profile, pages, frames, sequences, sequence_end = _parse(
        data, exe, source_entry, etc_overlay_data=etc_overlay_data)
    if not isinstance(asset_id, str) or not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe SD asset_id')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    palette = struct.unpack_from('<256H', data)
    files, images = [], []
    for frame in frames:
        if not frame['parts']:
            continue
        left, top, width, height = frame['bounds']
        words = [0] * (width * height)
        for part in reversed(frame['parts']):
            for y in range(part['height']):
                sy = part['height'] - y - 1 if part['flip_flags'] & 1 else y
                for x in range(part['width']):
                    sx = part['width'] - x - 1 if part['flip_flags'] & 2 else x
                    word = palette[data[part['sheet_source_offset'] + (part['v'] + sy) * 128 + part['u'] + sx]]
                    if word:
                        words[(part['y'] - top + y) * width + part['x'] - left + x] = word
        png = f'{asset_id}.frame-{frame["frame_index"]:03d}.png'
        mask = f'{asset_id}.frame-{frame["frame_index"]:03d}.stp.png'
        for name, content in ((png, _png(width, height, b''.join(_color(word) for word in words), 4)),
                              (mask, _png(width, height, bytes(255 if word & 0x8000 else 0 for word in words), 1))):
            with (output_dir / name).open('xb') as output:
                output.write(content)
            files.append(name)
        images.append(dict(frame, png=png, stp_png=mask, width=width, height=height,
                           variant='base', preview_kind='static_frame_composite', bits_per_pixel=8,
                           palette_source_offset=0, source_palette_words=list(palette),
                           preview_palette_words=list(palette), clut_vram=[0, 0x1F0],
                           palette_status='caller-established source palette; other runtime contexts unverified'))
    known = [(0, 0x200)] + [(offset, offset + 0x4000) for offset in pages]
    known += [(frame['source_frame_offset'], frame['source_frame_offset'] + frame['source_frame_size']) for frame in frames]
    known += [(sequence['source_offset'], sequence['source_offset'] + sequence['source_size']) for sequence in sequences]
    known.append((profile['tpage_table_offset'], sequence_end))
    metadata = {'source_sha256': hashlib.sha256(data).hexdigest(), 'source_size': len(data),
                'source_path_profile': source_entry['path'], 'source_lba': profile['source_lba'],
                'runtime_load_address': LOAD_BASE, 'paired_exe_sha256': hashlib.sha256(exe).hexdigest(),
                'paired_exe_size': len(exe), 'caller_profile': profile, 'bits_per_pixel': 8,
                'sheet_offsets': pages, 'sheet_size': [128, 128], 'sheet_bytes': 0x4000,
                'palette_source_offset': 0, 'palette_source_size': 0x200,
                'actor_runtime_palette_mode': 2, 'actor_runtime_palette_override': 0x10,
                'palette_queue_mode': 1,
                'empty_source_frames': [frame['frame_index'] for frame in frames if not frame['parts']],
                'source_sequence_prefix': sequences, 'sequence_pointer_prefix_end': sequence_end,
                'sequence_table_coverage': 'validated pointer prefix only; remaining bytes before first pixel page remain uninterpreted',
                'uninterpreted_extents': _uninterpreted(data, known),
                'partially_represented_extents': [{'offset': offset, 'size': 0x4000,
                                                 'sha256': hashlib.sha256(data[offset:offset + 0x4000]).hexdigest(),
                                                 'status': 'only frame-part texels represented; unused page texels unresolved'} for offset in sorted(set(pages))],
                'alpha_policy': 'BGR555 word zero transparent; every other word opaque',
                'stp_policy': 'source palette load mode1 leaves STP unchanged; separate bit15 mask',
                'coverage_policy': 'all frame pointers between caller-proved frame and sequence tables; static frames and validated sequence prefix only; runtime compositing and unused texels unresolved',
                'round_trip_verified': False,
                'evidence': ['src/main/scene_library.c:addr_init_weekly_sd,addr_init_holiday_sd,addr_init_club_sd,addr_init_else_sd',
                             'src/main/school_day.c:school_day_sd_load,school_day_sd_actors_init,school_day_vacation_sd_load,school_day_vacation_sd_actors_init',
                             'src/main/actor_sprites.c:actor_draw_all (16-pixel cells,8bpp,explicit CLUT row)',
                             'src/main/frame_render_audio.c:palette_load_vram (mode1 source words)']}
    return {'status': 'exported', 'format': 'tokimemo-sd-animation', 'images': images, 'files': files, 'metadata': metadata}
