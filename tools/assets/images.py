"""Bounded standard PlayStation TIM export (T-28002), using only the stdlib.

Only a TIM header at source offset zero is accepted. PNGs are previews: STP is
preserved separately because GPU blend modes cannot be represented by PNG alpha.
Game NBN packs, actor/frame tables, MDEC and STR are deliberately unsupported.
"""
from pathlib import Path
import hashlib
import re
import struct
import zlib


class ImageFormatError(ValueError):
    """A recognized image is malformed or outside the supported bounds."""


def _block(data, offset, label):
    if offset + 12 > len(data):
        raise ImageFormatError(f"truncated {label} block header")
    size, x, y, width, height = struct.unpack_from('<I4H', data, offset)
    if not width or not height or x + width > 1024 or y + height > 512:
        raise ImageFormatError(f"{label} rectangle outside VRAM")
    if size != 12 + width * height * 2:
        raise ImageFormatError(f"{label} block size does not match rectangle")
    if offset + size > len(data):
        raise ImageFormatError(f"truncated {label} block payload")
    return {'offset': offset, 'size': size, 'x': x, 'y': y,
            'width_words': width, 'height': height}, offset + size


def _parse(data):
    if len(data) < 8:
        raise ImageFormatError('truncated TIM header')
    magic, flags = struct.unpack_from('<II', data)
    if magic != 0x10:
        raise ImageFormatError('no standard TIM header at source offset zero')
    mode = flags & 7
    if flags & ~15 or mode > 3:
        raise ImageFormatError('unsupported TIM flags or pixel mode')
    indexed = mode < 2
    if bool(flags & 8) != indexed:
        raise ImageFormatError('indexed TIM requires CLUT; direct TIM with CLUT unsupported')
    cursor = 8
    clut = None
    if indexed:
        clut, cursor = _block(data, cursor, 'CLUT')
        palette_size = 16 if mode == 0 else 256
        if clut['width_words'] % palette_size:
            raise ImageFormatError('CLUT width is not a whole number of palettes')
    image, cursor = _block(data, cursor, 'image')
    trailing = data[cursor:]
    width = image['width_words'] * 2
    width = width * 2 if mode == 0 else width if mode == 1 else width // 2 if mode == 2 else width // 3
    if not width:
        raise ImageFormatError('24-bit TIM row has no complete pixel')
    palette_count = (clut['width_words'] * clut['height'] // palette_size) if clut else 0
    if palette_count > 256:
        raise ImageFormatError('TIM exceeds 256 palette preview limit')
    if width * image['height'] * max(1, palette_count) > 16 * 1024 * 1024:
        raise ImageFormatError('decoded palette previews exceed 16 million pixel limit')
    return {'format': 'psx-tim', 'flags': flags, 'bits_per_pixel': (4, 8, 16, 24)[mode],
            'width': width, 'height': image['height'], 'image_block': image,
            'clut_block': clut, 'palette_count': palette_count,
            'source_encoding': 'little-endian TIM; packed low nibble first; BGR555+STP or RGB888',
            'source_sha256': hashlib.sha256(data).hexdigest(), 'source_size': len(data),
            'tim_size': cursor,
            'trailing_extent': {'offset': cursor, 'size': len(trailing),
                                'sha256': hashlib.sha256(trailing).hexdigest(),
                                'interpretation': 'uninterpreted; outside TIM export'},
            'row_padding_bytes': image['width_words'] * 2 - width * 3 if mode == 3 else 0,
            'alpha_policy': 'BGR555 word zero transparent; all other words opaque; RGB888 opaque',
            'stp_policy': 'separate grayscale mask: bit 15 set = 255; GPU blend mode is external',
            'round_trip_verified': False}


def probe_images(data: bytes, source_path: str) -> dict:
    """Identify one whole-file TIM; never search arbitrary offsets for magic."""
    if data[:4] != b'\x10\0\0\0':
        return {'status': 'unsupported', 'format': None, 'source_path': source_path,
                'candidates': [], 'reason': 'no TIM at offset zero; game containers/compression unsupported'}
    try:
        metadata = _parse(data)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'psx-tim', 'source_path': source_path,
                'candidates': [], 'reason': str(error)}
    return {'status': 'supported', 'format': 'psx-tim', 'source_path': source_path,
            'candidates': [metadata]}


def _png(width, height, pixels, channels):
    def chunk(kind, payload):
        return struct.pack('>I', len(payload)) + kind + payload + struct.pack('>I', zlib.crc32(kind + payload) & 0xffffffff)
    stride = width * channels
    rows = b''.join(b'\0' + pixels[i:i + stride] for i in range(0, len(pixels), stride))
    return (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 6 if channels == 4 else 0, 0, 0, 0))
            + chunk(b'IDAT', zlib.compress(rows, 9)) + chunk(b'IEND', b''))


def _color(word):
    channels = [(word >> shift) & 31 for shift in (0, 5, 10)]
    return bytes([(v << 3) | (v >> 2) for v in channels] + [0 if word == 0 else 255])


def export_images(data: bytes, output_dir, asset_id: str) -> dict:
    """Write deterministic palette previews and STP masks; return JSON metadata."""
    metadata = _parse(data)
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('asset_id must be a safe single filename component')
    output_dir = Path(output_dir)
    block = metadata['image_block']
    payload = data[block['offset'] + 12:block['offset'] + block['size']]
    depth = metadata['bits_per_pixel']
    indices = []
    words = []
    if depth == 4:
        indices = [value for byte in payload for value in (byte & 15, byte >> 4)]
    elif depth == 8:
        indices = list(payload)
    elif depth == 16:
        words = [value[0] for value in struct.iter_unpack('<H', payload)]
    variants = []
    clut = metadata['clut_block']
    palette_size = 16 if depth == 4 else 256
    palettes = []
    if clut:
        colors = [value[0] for value in struct.iter_unpack('<H', data[clut['offset'] + 12:clut['offset'] + clut['size']])]
        palettes = [colors[i:i + palette_size] for i in range(0, len(colors), palette_size)]
    for number in range(max(1, len(palettes))):
        variant_words = [palettes[number][i] for i in indices] if clut else words
        if depth == 24:
            stride = block['width_words'] * 2
            rgba = b''.join(payload[y * stride + x * 3:y * stride + x * 3 + 3] + b'\xff'
                            for y in range(metadata['height']) for x in range(metadata['width']))
        else:
            rgba = b''.join(_color(word) for word in variant_words)
        name = f'{asset_id}.palette-{number:03d}' if clut else asset_id
        variant = {'palette_index': number if clut else None, 'png': name + '.png'}
        if clut:
            variant['palette_words'] = palettes[number]
            variant['clut_vram'] = {'x': clut['x'] + (number * palette_size) % clut['width_words'],
                                    'y': clut['y'] + (number * palette_size) // clut['width_words']}
        # Parse and decode before creating files so malformed inputs leave no output.
        variants.append((variant, rgba, bytes(255 if word & 0x8000 else 0 for word in variant_words)))
    output_dir.mkdir(parents=True, exist_ok=True)
    files = []
    images = []
    for variant, rgba, stp in variants:
        (output_dir / variant['png']).write_bytes(_png(metadata['width'], metadata['height'], rgba, 4))
        files.append(variant['png'])
        if depth != 24:
            variant['stp_png'] = variant['png'][:-4] + '.stp.png'
            (output_dir / variant['stp_png']).write_bytes(_png(metadata['width'], metadata['height'], stp, 1))
            files.append(variant['stp_png'])
        images.append(variant)
    return {'status': 'exported', 'format': 'psx-tim', 'files': files, 'images': images, 'metadata': metadata}
