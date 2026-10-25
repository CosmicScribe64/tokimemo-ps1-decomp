"""Bounded CALROM calendar UI export, from the shipped loader (T-28002).

src/main/hud_screens.c:calendar_month_load dereferences month_data_table, a pointer
variable at 0x800b3748. It points at CALROM's 36-entry pointer table, not an
array in the executable. Each pointer names a 128x128 4bpp tile followed by
numeric month records. cal_sprite_init supplies two distinct CLUT selections.
"""
import hashlib
import re
import struct
from pathlib import Path
from .image_packs import _reader
from .images import ImageFormatError, _color, _png

CALENDAR_SIZE = 0x4a800
LOAD_ADDRESS = 0x801a0000
MONTH_COUNT = 36


def _parse(pack, exe):
    if len(pack) != CALENDAR_SIZE:
        raise ImageFormatError('CALROM profile requires exactly 0x4a800 source bytes')
    read, base = _reader(exe)
    if struct.unpack('<I', read(0x800b3748, 4))[0] != LOAD_ADDRESS:
        raise ImageFormatError('paired executable calendar-table pointer differs')
    pointers = struct.unpack_from('<36I', pack)
    months, occupied = [], [(0, 0x90)]
    for index, pointer in enumerate(pointers):
        offset = pointer - LOAD_ADDRESS
        if offset < 0x90 or offset % 16 or offset + 0x2008 > len(pack):
            raise ImageFormatError('calendar tile pointer outside source profile')
        count, first = struct.unpack_from('<2I', pack, offset + 0x2000)
        if not 28 <= count <= 31 or first > 6:
            raise ImageFormatError('unsupported calendar day count/start record')
        end = offset + 0x2008 + count * 8
        if end > len(pack):
            raise ImageFormatError('truncated calendar numeric records')
        kinds = list(struct.unpack_from('<' + 'I' * count, pack, offset + 0x2008))
        markers = list(struct.unpack_from('<' + 'I' * count, pack, offset + 0x2008 + count * 4))
        occupied.append((offset, end))
        absolute_month = 1995 * 12 + 3 + index
        months.append({'month_index': index, 'year': absolute_month // 12,
                       'month': absolute_month % 12 + 1, 'pointer_address': pointer,
                       'pointer_table_offset': index * 4, 'tile_source_offset': offset,
                       'tile_source_size': 0x2000,
                       'tile_sha256': hashlib.sha256(pack[offset:offset + 0x2000]).hexdigest(),
                       'numeric_records': {'source_offset': offset + 0x2000, 'source_size': end - offset - 0x2000,
                                           'day_count': count, 'start_word': first,
                                           'day_kind_words': kinds, 'marker_words': markers}})
    occupied.sort()
    cursor, gaps = 0, []
    for start, end in occupied:
        if start < cursor:
            raise ImageFormatError('calendar tile/record extents overlap')
        if start > cursor:
            gaps.append({'offset': cursor, 'size': start - cursor,
                         'sha256': hashlib.sha256(pack[cursor:start]).hexdigest(),
                         'status': 'uninterpreted; outside tile/numeric-record exports'})
        cursor = end
    if cursor < len(pack):
        gaps.append({'offset': cursor, 'size': len(pack) - cursor,
                     'sha256': hashlib.sha256(pack[cursor:]).hexdigest(),
                     'status': 'uninterpreted; outside tile/numeric-record exports'})
    return read, base, months, gaps


def probe_calendar_ui(pack: bytes, exe: bytes) -> dict:
    """Validate the CALROM source and its paired executable pointer profile."""
    try:
        _, _, months, _ = _parse(pack, exe)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'tokimemo-calendar-ui', 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-calendar-ui',
            'metadata': {'months': len(months), 'tiles': len(months), 'bits_per_pixel': 4}}


def export_calendar_ui(pack: bytes, exe: bytes, output_dir, asset_id: str) -> dict:
    """Export calendar tiles and month-label crops with their separate CLUTs."""
    read, base, months, gaps = _parse(pack, exe)
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe calendar asset_id')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    images, files = [], []
    variants = (('calendar-grid', 0, 0, 128, 128, 0x800c61a0, 0xc0),
                ('month-label', 48, 0, 24, 16, 0x800c6180, 0xb0))
    for month in months:
        for variant, left, top, width, height, palette_address, clut_x in variants:
            palette = struct.unpack('<16H', read(palette_address, 32))
            rgba, stp = bytearray(), bytearray()
            for y in range(top, top + height):
                for x in range(left, left + width):
                    texel = y * 128 + x
                    packed = pack[month['tile_source_offset'] + texel // 2]
                    word = palette[(packed >> (4 * (texel & 1))) & 15]
                    rgba.extend(_color(word))
                    stp.append(255 if word & 0x8000 else 0)
            name = f'{asset_id}.month-{month["month_index"]:02d}.{variant}'
            png, mask = name + '.png', name + '.stp.png'
            for path, content in ((png, _png(width, height, rgba, 4)), (mask, _png(width, height, stp, 1))):
                with (output_dir / path).open('xb') as output:
                    output.write(content)
                files.append(path)
            image = dict(month, frame_index=month['month_index'], variant=variant,
                         png=png, stp_png=mask, width=width, height=height,
                         source_region=[left, top, width, height], parts=[],
                         palette_exe_address=palette_address, palette_exe_offset=palette_address - base + 0x800,
                         source_palette_words=list(palette), clut_vram=[clut_x, 0x1e0],
                         upload_rectangles_words=[[0x3a0, 0x100, 0x20, 0x80], [0x3a0, 0x180, 0x20, 0x80]],
                         runtime_sprite_regions=([[0, 0, 126, 126], [0, 0, 127, 127]] if variant == 'calendar-grid' else [[48, 0, 24, 16]]),
                         runtime_uv=([[0x80, 0], [0x80, 0x80]] if variant == 'calendar-grid' else [[0xb0, 0], [0xb0, 0x80]]))
            images.append(image)
    metadata = {'source_sha256': hashlib.sha256(pack).hexdigest(), 'source_size': len(pack),
                'paired_exe_sha256': hashlib.sha256(exe).hexdigest(), 'paired_exe_size': len(exe),
                'source_path_profile': 'CDROM/FUKUTEST/CALROM.NBN', 'source_lba': 0x9b94,
                'runtime_load_address': LOAD_ADDRESS, 'month_table_pointer_exe_address': 0x800b3748,
                'pointer_table_source_offset': 0, 'pointer_table_size': 0x90,
                'month_count': MONTH_COUNT, 'bits_per_pixel': 4, 'tile_size': [128, 128],
                'months': months, 'uninterpreted_extents': gaps,
                'coverage_policy': '36 complete source tiles and numeric month records; gaps/trailer uninterpreted; full-grid preview includes borders cropped by runtime sprites',
                'render_policy': 'static source pixels and selected CLUT; separate STP mask; grid and label use different palettes; no runtime biorhythm/marks/composition',
                'alpha_policy': 'BGR555 word zero transparent; all other words opaque',
                'stp_policy': 'separate grayscale mask: bit 15 set = 255; runtime GPU blending external',
                'round_trip_verified': False,
                'evidence': ['src/main/hud_screens.c:calendar_month_load,cal_sprite_init',
                             'src/main/scheduler.c:schedule_month_data_load_step',
                             'src/main/boot_assets.c:boot_hud_init palette initialization']}
    return {'status': 'exported', 'format': 'tokimemo-calendar-ui', 'files': files, 'images': images, 'metadata': metadata}
