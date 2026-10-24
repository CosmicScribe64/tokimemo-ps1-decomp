"""Source-proved BUSTUP portrait pages and static frame preview (T-28002).

The checks in src/main/8007C030.c and portrait_build in 80062CD0.c establish
five 128x128 8bpp sheets, one source frame, and the active 256-color CLUT.
Only this fixed layout is supported; no search, palette guesses or dynamic
mouth/eye/rank playback reconstruction is performed.
"""
import hashlib
import re
import struct
from pathlib import Path
from .image_packs import _reader
from .images import ImageFormatError, _color, _png

PORTRAIT_SIZE = 0x16800
SHEET_OFFSETS = tuple(0x20ac + index * 0x4000 for index in range(5))


def _parse(data, exe):
    if len(data) != PORTRAIT_SIZE:
        raise ImageFormatError('portrait profile requires exactly 0x16800 bytes')
    read, _ = _reader(exe)
    actual = struct.unpack('<5I', read(0x800ca120, 20))
    if actual != (0x80180084, 0x80180098, 0x8018009c, 0, 0x801800ac):
        raise ImageFormatError('paired executable portrait-pointer globals differ')
    expected = tuple(0x80180000 + offset for offset in SHEET_OFFSETS)
    if struct.unpack_from('<5I', data, 0x84) != expected:
        raise ImageFormatError('portrait sheet-pointer profile differs')
    if struct.unpack_from('<3I', data, 0x98) != (0x80180000, 0x8018005c, 0x80180070):
        raise ImageFormatError('portrait frame/sequence-pointer profile differs')
    count, ox, oy, flags = struct.unpack_from('<4h', data)
    if count != 6:
        raise ImageFormatError('portrait source frame must have six parts')
    parts = []
    for index in range(count):
        x, y, texture, palette, flip, width, height = struct.unpack_from('<2h3H2h', data, 8 + index * 14)
        sheet = texture >> 6
        u, v = (texture & 7) * 16, ((texture & 63) >> 3) * 16
        if sheet >= 5 or not 0 < width <= 128 or not 0 < height <= 128 or u + width > 128 or v + height > 128 or flip & ~3 or (palette >> 8) & 15:
            raise ImageFormatError('unsupported portrait part geometry/palette profile')
        parts.append({'part_index': index, 'x': x, 'y': y, 'texture_id': texture,
                      'sheet_index': sheet, 'sheet_source_offset': SHEET_OFFSETS[sheet],
                      'u': u, 'v': v, 'width': width, 'height': height,
                      'palette_info': palette, 'flip_flags': flip, 'source_record_offset': 8 + index * 14})
    bounds = [min(p['x'] for p in parts), min(p['y'] for p in parts)]
    bounds += [max(p['x'] + p['width'] for p in parts) - bounds[0],
               max(p['y'] + p['height'] for p in parts) - bounds[1]]
    if bounds[2] > 512 or bounds[3] > 512:
        raise ImageFormatError('portrait frame bounds exceed preview budget')
    original = struct.unpack_from('<256H', data, 0xac)
    palette = tuple(word & 0x7fff if index < 240 else word | 0x8000 if word & 0x7fff else word
                    for index, word in enumerate(original))
    sequences = []
    for offset in (0x5c, 0x70):
        words = struct.unpack_from('<10H', data, offset)
        if words[0] != 4 or any(words[1 + index * 2] for index in range(4)):
            raise ImageFormatError('unsupported portrait source sequence')
        sequences.append({'source_offset': offset, 'steps': [{'frame_index': 0, 'duration': words[2 + i * 2]} for i in range(4)],
                          'uninterpreted_final_halfword': words[-1]})
    return {'origin': [ox, oy], 'flags': flags, 'parts': parts, 'bounds': bounds}, original, palette, sequences


def probe_portrait(data: bytes, exe: bytes) -> dict:
    """Validate the explicit six-part BUSTUP+main-executable layout."""
    try:
        _parse(data, exe)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'tokimemo-portrait', 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-portrait', 'metadata': {'sheets': 5, 'source_frames': 1, 'parts': 6}}


def export_portrait(data: bytes, exe: bytes, output_dir, asset_id: str) -> dict:
    """Export actual-CLUT pages and a static source-frame composition."""
    frame, original, palette, sequences = _parse(data, exe)
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe portrait asset_id')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    images, files = [], []
    def publish(name, width, height, words, metadata):
        png, mask = f'{asset_id}.{name}.png', f'{asset_id}.{name}.stp.png'
        rgba = b''.join(_color(word) for word in words)
        stp = bytes(255 if word & 0x8000 else 0 for word in words)
        for path, content in ((png, _png(width, height, rgba, 4)), (mask, _png(width, height, stp, 1))):
            with (output_dir / path).open('xb') as output:
                output.write(content)
            files.append(path)
        images.append(dict(metadata, png=png, stp_png=mask, width=width, height=height,
                           variant=name, palette_status='established by portrait_build upload', clut_vram=[0, 0x1f0]))
    for index, offset in enumerate(SHEET_OFFSETS):
        words = [palette[value] for value in data[offset:offset + 0x4000]]
        publish(f'source-page-{index}', 128, 128, words,
                {'frame_index': None, 'parts': [], 'source_sheet_offset': offset, 'source_sheet_size': 0x4000,
                 'source_sheet_sha256': hashlib.sha256(data[offset:offset + 0x4000]).hexdigest(),
                 'vram_rectangle_words': [(5 + index) * 64, 0, 64, 128],
                 'preview_kind': 'source_texture_page'})
    left, top, width, height = frame['bounds']
    composed = [0] * (width * height)
    for part in reversed(frame['parts']):
        for y in range(part['height']):
            sy = part['height'] - y - 1 if part['flip_flags'] & 1 else y
            for x in range(part['width']):
                sx = part['width'] - x - 1 if part['flip_flags'] & 2 else x
                texel = (part['v'] + sy) * 128 + part['u'] + sx
                word = palette[data[part['sheet_source_offset'] + texel]]
                if word:
                    composed[(part['y'] - top + y) * width + part['x'] - left + x] = word
    publish('static-frame', width, height, composed,
            dict(frame, frame_index=0, preview_kind='static_frame_composite',
                 render_policy='source six-part frame; runtime actor first-part clipping and dynamic mouth/eye/rank quads not reproduced'))
    metadata = {'source_sha256': hashlib.sha256(data).hexdigest(), 'source_size': len(data),
                'paired_exe_sha256': hashlib.sha256(exe).hexdigest(), 'paired_exe_size': len(exe),
                'runtime_load_address': 0x80180000, 'source_path_profile': 'CDROM/BUSTUP/*.BIN',
                'bits_per_pixel': 8, 'sheet_count': 5, 'sheet_size': [128, 128], 'sheet_offsets': list(SHEET_OFFSETS),
                'source_palette_offset': 0xac, 'source_palette_size': 512,
                'source_palette_words': list(original), 'preview_palette_words': list(palette),
                'palette_transform': 'load_palette mode2: clear STP first240 entries; set STP on nonblack final16 entries',
                'uninterpreted_header_words': {'source_offset': 0xa4, 'words': list(struct.unpack_from('<2I', data, 0xa4))},
                'alpha_policy': 'BGR555 word zero transparent; all other words opaque',
                'stp_policy': 'separate grayscale mask bit15; runtime GPU blending external',
                'unrepresented_extents': [{'offset': 0x2ac, 'size': 0x1e00,
                                          'sha256': hashlib.sha256(data[0x2ac:0x20ac]).hexdigest(),
                                          'status': 'remaining palette-area bytes; not uploaded by this portrait_build call'},
                                         {'offset': 0x160ac, 'size': len(data) - 0x160ac,
                                          'sha256': hashlib.sha256(data[0x160ac:]).hexdigest(),
                                          'status': 'opaque trailer; checksum/reconstruction not interpreted'}],
                'source_sequences': sequences, 'round_trip_verified': False,
                'coverage_policy': 'five uploaded texture pages plus static source frame; dynamic face selection/playback and other palette rows are not reconstructed',
                'evidence': ['src/main/80062CD0.c:portrait_read_start,portrait_build',
                             'src/main/8007C030.c:portrait_show_checked,func_800836A0',
                             'src/main/80043510.c:palette_load_vram', 'src/main/80047550.c:actor_draw_all']}
    return {'status': 'exported', 'format': 'tokimemo-portrait', 'files': files, 'images': images, 'metadata': metadata}
