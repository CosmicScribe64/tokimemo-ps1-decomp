"""Source-proved MAINROM UI texture/frame export (T-28002).

The boot loader in src/main/80059B40.c reads MAINROM at LBA 0x9d85 to
0x80180000. Its 13 sheets are raw 128x128 4bpp; paired main-executable tables
supply frame geometry, texture-pointer order and palettes. This is one explicit
release profile, never a search for embedded signatures or guessed containers.
"""
import hashlib
import re
import struct
from pathlib import Path
from .images import ImageFormatError, _color, _png

PACK_SIZE = 0x1a800
FRAME_TABLE = 0x800c975c
FRAME_COUNT = (0x800c9a14 - FRAME_TABLE) // 4
TEXTURE_OFFSETS = (0x8000, 0x12000, 0, 0xa000, 0x14000, 0x2000,
                   0xc000, 0x16000, 0x4000, 0xe000, 0x18000)
# These are candidate palette overrides used by specific HUD actors, not a
# claim that every source frame appears with every override during gameplay.
OVERRIDES = {'hud-43': (0x800c6280, 0x120, 0x1e3),
             'hud-44': (0x800c62a0, 0x120, 0x1e4),
             'icon-51': (0x800c6200, 0x120, 0x1f1),
             'icon-52': (0x800c6220, 0x120, 0x1f2),
             'icon-53': (0x800c6240, 0x120, 0x1f3)}


def _reader(exe):
    if len(exe) < 0x800 or exe[:8] != b'PS-X EXE':
        raise ImageFormatError('paired source is not a PS-X EXE')
    base, size = struct.unpack_from('<II', exe, 0x18)
    if base != 0x80041000 or size > len(exe) - 0x800:
        raise ImageFormatError('unsupported main executable address/extent profile')
    def read(address, length):
        offset = address - base + 0x800
        if offset < 0x800 or offset + length > min(len(exe), size + 0x800):
            raise ImageFormatError('main UI table pointer outside paired executable')
        return exe[offset:offset + length]
    return read, base


def _parse(pack, exe):
    if len(pack) != PACK_SIZE:
        raise ImageFormatError('MAINROM profile requires exactly 0x1a800 source bytes')
    read, base = _reader(exe)
    pointers = struct.unpack('<11I', read(0x800c9730, 44))
    if tuple(p - 0x80180000 for p in pointers) != TEXTURE_OFFSETS:
        raise ImageFormatError('paired main UI texture-pointer profile differs')
    if struct.unpack('<2I', read(0x800c5fcc, 8)) != (0x80186000, 0x80190000):
        raise ImageFormatError('paired extra main UI sheet pointers differ')
    frames = []
    for index, address in enumerate(struct.unpack('<174I', read(FRAME_TABLE, FRAME_COUNT * 4))):
        if not 0x800c7420 <= address < 0x800c95e0:
            raise ImageFormatError('frame pointer outside known main UI frame region')
        count, ox, oy, flags = struct.unpack('<4h', read(address, 8))
        if not 1 <= count <= 64 or address + 8 + count * 14 > 0x800c95e0:
            raise ImageFormatError('main UI frame count/extent outside profile')
        parts = []
        for number in range(count):
            x, y, texture, palette, flip, width, height = struct.unpack('<2h3H2h', read(address + 8 + number * 14, 14))
            sheet = texture >> 8
            u, v = (texture & 15) * 8, ((texture & 255) >> 4) * 8
            if sheet >= len(pointers) or not 0 < width <= 128 or not 0 < height <= 128 or u + width > 128 or v + height > 128 or flip & ~3:
                raise ImageFormatError('unsupported main UI part geometry/flags')
            parts.append({'part_index': number, 'x': x, 'y': y, 'width': width,
                          'height': height, 'texture_id': texture, 'sheet_index': sheet,
                          'sheet_source_offset': TEXTURE_OFFSETS[sheet], 'u': u, 'v': v,
                          'palette_info': palette, 'flip_flags': flip,
                          'exe_record_address': address + 8 + number * 14,
                          'exe_record_offset': address + 8 + number * 14 - base + 0x800})
        left, top = min(p['x'] for p in parts), min(p['y'] for p in parts)
        width = max(p['x'] + p['width'] for p in parts) - left
        height = max(p['y'] + p['height'] for p in parts) - top
        if width > 1024 or height > 512:
            raise ImageFormatError('main UI composed frame exceeds preview bound')
        frames.append({'frame_index': index, 'exe_frame_address': address,
                       'exe_frame_offset': address - base + 0x800, 'origin': [ox, oy],
                       'frame_flags': flags, 'bounds': [left, top, width, height], 'parts': parts})
    sequences = []
    for index, address in enumerate(struct.unpack('<16I', read(0x800c9a14, 64))):
        if not 0x800c95e0 <= address < 0x800c9730:
            raise ImageFormatError('main UI sequence pointer outside profile')
        count = struct.unpack('<H', read(address, 2))[0]
        if not 1 <= count <= 32 or address + 2 + count * 4 > 0x800c9730:
            raise ImageFormatError('main UI sequence extent outside profile')
        steps = list(struct.iter_unpack('<HH', read(address + 2, count * 4)))
        if any(frame >= FRAME_COUNT for frame, duration in steps):
            raise ImageFormatError('main UI sequence references unknown frame')
        sequences.append({'sequence_index': index, 'exe_address': address,
                          'steps': [{'frame_index': f, 'duration': t} for f, t in steps]})
    return read, base, frames, sequences


def probe_main_ui(pack: bytes, exe: bytes) -> dict:
    """Validate the explicit MAINROM+main-executable profile."""
    try:
        _, _, frames, sequences = _parse(pack, exe)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'tokimemo-main-ui', 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-main-ui',
            'metadata': {'frames': len(frames), 'sequences': len(sequences),
                         'parts': sum(len(f['parts']) for f in frames)}}


def export_main_ui(pack: bytes, exe: bytes, output_dir, asset_id: str) -> dict:
    """Export composed main UI frame previews with exact source/part provenance."""
    read, base, frames, sequences = _parse(pack, exe)
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe main UI asset_id')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    images, files = [], []
    for frame in frames:
        left, top, width, height = frame['bounds']
        for variant in ('base', *OVERRIDES):
            rgba = bytearray(width * height * 4)
            stp = bytearray(width * height)
            parts = []
            # Renderer draws last part first. Zero-word pixels leave lower parts.
            for part in reversed(frame['parts']):
                if variant == 'base':
                    info = part['palette_info']
                    row, column = (info >> 8) & 15, info & 0xf0
                    palette_address = 0x800c6020 + row * 512 + column * 2
                    clut_x, clut_y = column, 0x1e0 + row
                else:
                    palette_address, clut_x, clut_y = OVERRIDES[variant]
                palette = struct.unpack('<16H', read(palette_address, 32))
                # Boot forces STP on nonzero colors in these two palette ranges.
                palette = tuple(word | 0x8000 if word & 0x7fff and
                                (0x800c6200 <= palette_address < 0x800c6220 or
                                 0x800c62c0 <= palette_address < 0x800c6340) else word for word in palette)
                detail = dict(part, clut_vram=[clut_x, clut_y], palette_exe_address=palette_address,
                              palette_exe_offset=palette_address - base + 0x800,
                              source_palette_words=list(struct.unpack('<16H', read(palette_address, 32))),
                              preview_palette_words=list(palette))
                parts.append(detail)
                for y in range(part['height']):
                    sy = part['height'] - 1 - y if part['flip_flags'] & 1 else y
                    for x in range(part['width']):
                        sx = part['width'] - 1 - x if part['flip_flags'] & 2 else x
                        texel = (part['v'] + sy) * 128 + part['u'] + sx
                        packed = pack[part['sheet_source_offset'] + texel // 2]
                        word = palette[(packed >> (4 * (texel & 1))) & 15]
                        if word == 0:
                            continue
                        dest = (part['y'] - top + y) * width + part['x'] - left + x
                        rgba[dest * 4:dest * 4 + 4] = _color(word)
                        stp[dest] = 255 if word & 0x8000 else 0
            name = f'{asset_id}.frame-{frame["frame_index"]:03d}.{variant}'
            png, mask = name + '.png', name + '.stp.png'
            for path, content in ((png, _png(width, height, rgba, 4)), (mask, _png(width, height, stp, 1))):
                with (output_dir / path).open('xb') as output:
                    output.write(content)
                files.append(path)
            images.append(dict(frame, png=png, stp_png=mask, width=width, height=height,
                               variant=variant, palette_variant_scope='base source palette' if variant == 'base' else 'known HUD override candidate; not confirmed for every frame',
                               parts=list(reversed(parts))))
    represented_offsets = sorted({part['sheet_source_offset'] for frame in frames for part in frame['parts']})
    # Diagnostic source indices expose unreferenced sheets without guessing a
    # runtime CLUT, transparency, frame mapping, or intended screen placement.
    for offset in range(0, 0x1a000, 0x2000):
        if offset in represented_offsets:
            continue
        indices = bytes(value * 17 for packed in pack[offset:offset + 0x2000]
                        for value in (packed & 15, packed >> 4))
        variant = f'source-index-sheet-{offset:05x}'
        png = f'{asset_id}.{variant}.png'
        with (output_dir / png).open('xb') as output:
            output.write(_png(128, 128, indices, 1))
        files.append(png)
        images.append({'frame_index': None, 'variant': variant, 'png': png,
                       'width': 128, 'height': 128, 'parts': [],
                       'source_sheet_offset': offset, 'source_sheet_size': 0x2000,
                       'source_sheet_sha256': hashlib.sha256(pack[offset:offset + 0x2000]).hexdigest(),
                       'palette_status': 'unknown', 'preview_kind': 'source_index_diagnostic',
                       'alpha_policy': 'opaque index visualization; source transparency unknown without CLUT',
                       'stp_policy': 'unknown; no CLUT or STP interpretation',
                       'render_policy': 'opaque grayscale index*17; no inferred CLUT, STP, alpha or runtime mapping'})
    metadata = {'source_sha256': hashlib.sha256(pack).hexdigest(),
                'paired_exe_sha256': hashlib.sha256(exe).hexdigest(),
                'source_size': len(pack), 'paired_exe_size': len(exe),
                'source_path_profile': 'CDROM/FUKUTEST/MAINROM.NBN', 'source_lba': 0x9d85,
                'runtime_load_address': 0x80180000, 'sheet_size': [128, 128],
                'bits_per_pixel': 4, 'sheet_bytes': 0x2000, 'sheet_count': 13,
                'texture_pointer_offsets': list(TEXTURE_OFFSETS), 'sequences': sequences,
                'frame_table_referenced_sheet_offsets': represented_offsets,
                'unmapped_sheet_extents': [{'offset': offset, 'size': 0x2000,
                                           'sha256': hashlib.sha256(pack[offset:offset + 0x2000]).hexdigest(),
                                           'status': 'unreviewed; no frame-table part references this sheet'}
                                          for offset in range(0, 0x1a000, 0x2000)
                                          if offset not in represented_offsets],
                'coverage_policy': 'only frame-table part rectangles are represented; referenced sheets may contain other unrepresented pixels or assets',
                'trailing_extent': {'offset': 0x1a000, 'size': 0x800, 'sha256': hashlib.sha256(pack[0x1a000:]).hexdigest()},
                'render_policy': 'static part geometry, flips, zero-word transparency; STP mask separate; GPU blending/tint/runtime selection not rendered',
                'alpha_policy': 'BGR555 word zero transparent; all other words opaque',
                'stp_policy': 'separate grayscale mask: bit 15 set = 255; runtime GPU blending external',
                'round_trip_verified': False,
                'evidence': ['src/main/80059B40.c:func_80059E00,func_80059BE8,func_80059EF0',
                             'src/main/80047550.c:actor_draw_all', 'src/main/80062CD0.c:parameter_show_init,hizuke_init,func_8006509C']}
    return {'status': 'exported', 'format': 'tokimemo-main-ui', 'files': files, 'images': images, 'metadata': metadata}
