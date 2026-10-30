"""Bounded Sony BS v1/v2/v3 coefficient reader and background preview (T-28002).

Format facts: https://psx-spx.consoledev.net/cdromfileformats/#cdrom-file-video-bs-compression-ac-values
MDEC math: https://psx-spx.consoledev.net/macroblockdecodermdec/
The Huffman/quant tables below are format constants, not copied decoder code.
Game geometry comes from src/main/background_images.c and direct scene callers,
not the extension or file size.
The reviewed fixed-point model does not claim hardware or Redux pixel parity.
"""
import copy
import hashlib
import re
import struct
from pathlib import Path
from .images import ImageFormatError, _png

# Prefix, number of following index bits, packed run/level values (before sign).
AC_GROUPS = (
    ('11', 0, (0x0001,)), ('011', 0, (0x0401,)),
    ('010', 1, (0x0002, 0x0801)), ('0011', 1, (0x1001, 0x0c01)),
    ('00101', 0, (0x0003,)),
    ('00100', 3, (0x3401, 0x0006, 0x3001, 0x2c01, 0x0c02, 0x0403, 0x0005, 0x2801)),
    ('0001', 2, (0x1c01, 0x1801, 0x0402, 0x1401)),
    ('00001', 2, (0x0802, 0x2401, 0x0004, 0x2001)),
    ('0000001', 3, (0x4001, 0x1402, 0x0007, 0x0803, 0x0404, 0x3c01, 0x3801, 0x1002)),
    ('00000001', 4, (0x000b, 0x2002, 0x1003, 0x000a, 0x0804, 0x1c02, 0x5401, 0x5001,
                      0x0009, 0x4c01, 0x4801, 0x0405, 0x0c03, 0x0008, 0x1802, 0x4401)),
    ('000000001', 4, (0x2802, 0x2402, 0x1403, 0x0c04, 0x0805, 0x0407, 0x0406, 0x000f,
                       0x000e, 0x000d, 0x000c, 0x6801, 0x6401, 0x6001, 0x5c01, 0x5801)),
    ('0000000001', 4, tuple(range(0x1f, 0x0f, -1))),
    ('00000000001', 4, (0x28, 0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21,
                         0x20, 0x040e, 0x040d, 0x040c, 0x040b, 0x040a, 0x0409, 0x0408)),
    ('000000000001', 4, (0x0412, 0x0411, 0x0410, 0x040f, 0x1803, 0x4002, 0x3c02, 0x3802,
                          0x3402, 0x3002, 0x2c02, 0x7c01, 0x7801, 0x7401, 0x7001, 0x6c01)),
)
AC_CODES = {prefix + (format(index, f'0{count}b') if count else ''): value
            for prefix, count, values in AC_GROUPS for index, value in enumerate(values)}
# SDK default quant values in incoming zigzag order. Verified at main EXE
# 0x800ca384 by libpress_main.s:func_800869EC (MDEC reset/upload).
QUANT = (2,16,16,19,16,19,22,22,22,22,22,22,26,24,26,27,
         27,27,26,26,26,26,27,27,27,29,29,29,34,34,34,29,
         29,29,27,27,29,29,32,32,34,34,37,38,37,35,35,34,
         35,38,38,40,40,40,48,48,46,46,56,56,58,69,69,83)
ZIGZAG = tuple(y * 8 + x for diagonal in range(15)
               for y, x in ([(y, diagonal - y) for y in range(8)] if diagonal % 2
                            else [(diagonal - x, x) for x in range(8)])
               if 0 <= x < 8 and 0 <= y < 8)
# Standard MDEC scale matrix (13 significant bits), frequency-major.
_SCALE_WORDS = (
 '5A82 5A82 5A82 5A82 5A82 5A82 5A82 5A82',
 '7D8A 6A6D 471C 18F8 E707 B8E3 9592 8275',
 '7641 30FB CF04 89BE 89BE CF04 30FB 7641',
 '6A6D E707 8275 B8E3 471C 7D8A 18F8 9592',
 '5A82 A57D A57D 5A82 5A82 A57D A57D 5A82',
 '471C 8275 18F8 6A6D 9592 E707 7D8A B8E3',
 '30FB 89BE 7641 CF04 CF04 7641 89BE 30FB',
 '18F8 B8E3 6A6D 8275 7D8A 9592 471C E707')
DC_CHROMA = {'00': 0, '01': 1, '10': 2, '110': 3, '1110': 4,
             '11110': 5, '111110': 6, '1111110': 7, '11111110': 8}
DC_LUMA = {'100': 0, '00': 1, '01': 2, '101': 3, '110': 4,
           '1110': 5, '11110': 6, '111110': 7, '1111110': 8}


class UnsupportedMdec(ImageFormatError):
    """Recognized source variant or dimensions have not been established."""


class _Bits:
    def __init__(self, data):
        self.words = tuple(value[0] for value in struct.iter_unpack('<H', data))
        self.position = 0

    def take(self, count):
        if self.position + count > len(self.words) * 16:
            raise ImageFormatError('truncated BS Huffman stream')
        value = 0
        while count:
            word, bit = divmod(self.position, 16)
            step = min(count, 16 - bit)
            value = (value << step) | ((self.words[word] >> (16 - bit - step)) & ((1 << step) - 1))
            self.position += step
            count -= step
        return value


def _signed(value, bits=10):
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def _v3_dc(stream, component, predictors):
    """Decode one standard differential DC; predictors are signed MDEC DC units."""
    codes = DC_LUMA if component == 2 else DC_CHROMA
    prefix = ''
    for _ in range(10):
        prefix += str(stream.take(1))
        if prefix == '1111111111':
            return None
        if prefix in codes:
            count = codes[prefix]
            magnitude = stream.take(count) if count else 0
            if count and magnitude < 1 << (count - 1):
                magnitude -= (1 << count) - 1
            current = predictors[component] + magnitude * 4
            if not -512 <= current <= 508:
                raise UnsupportedMdec('BS v3 wrapped/out-of-range DC prediction is unsupported')
            predictors[component] = current
            return current
    raise ImageFormatError('invalid BS v3 DC Huffman prefix')


def _coefficients(data):
    if len(data) < 10 or len(data) > 1024 * 1024 or len(data) % 2:
        raise ImageFormatError('BS input must be an even bounded 10-byte..1MiB extent')
    declared, magic, quant, version = struct.unpack_from('<4H', data)
    if magic != 0x3800:
        raise UnsupportedMdec('no standard BS header at source offset zero')
    if version not in (1, 2, 3):
        raise UnsupportedMdec(f'BS version {version} unsupported; only v1/v2/v3 DC coding implemented')
    if not 1 <= quant <= 63 or not declared:
        raise UnsupportedMdec('zero/fractional/out-of-range quantization unsupported')
    stream = _Bits(data[8:])
    blocks, halfwords, escapes, zero_escapes = [], 0, 0, 0
    predictors = [0, 0, 0]  # independent Cr, Cb and shared Y history per frame
    while True:
        if version == 3:
            block_in_macroblock = len(blocks) % 6
            component = block_in_macroblock if block_in_macroblock < 2 else 2
            dc_value = _v3_dc(stream, component, predictors)
        else:
            raw_dc = stream.take(10)
            dc_value = None if raw_dc == 511 else _signed(raw_dc)
        if dc_value is None:
            if len(blocks) % 6:
                raise ImageFormatError('BS end marker inside a six-block macroblock')
            break
        if len(blocks) >= 3072:
            raise ImageFormatError('BS exceeds 512-macroblock decode budget')
        block = [0] * 64
        block[0] = dc_value * QUANT[0]
        halfwords += 1
        index = 0
        while True:
            prefix = ''
            packed = None
            for _ in range(16):
                prefix += str(stream.take(1))
                if prefix == '10':
                    packed = 0xfe00
                    break
                if prefix == '000001':
                    packed = stream.take(16)
                    escapes += 1
                    if not packed & 1023:
                        zero_escapes += 1
                    break
                if prefix in AC_CODES:
                    packed = AC_CODES[prefix]
                    if stream.take(1):
                        packed = (packed & 0xfc00) | ((-(packed & 1023)) & 1023)
                    break
            if packed is None:
                raise ImageFormatError('invalid BS AC Huffman prefix')
            halfwords += 1
            if packed == 0xfe00:
                break
            index += (packed >> 10) + 1
            if index > 63:
                raise ImageFormatError('BS AC run exceeds 64 coefficients')
            value = (_signed(packed & 1023) * QUANT[index] * quant + 4) >> 3
            block[ZIGZAG[index]] = max(-1024, min(1023, value))
        blocks.append(block)
    if not blocks:
        raise ImageFormatError('BS frame contains no macroblocks')
    rounded_bytes = ((halfwords * 2 + 127) // 128) * 128
    if rounded_bytes != declared * 4:
        raise ImageFormatError('BS declared MDEC extent differs from decoded run/level count')
    # Bits run MSB-first within little-endian source halfwords. A partially
    # consumed halfword cannot be described as a contiguous consumed byte prefix.
    consumed = 8 + (stream.position + 15) // 16 * 2
    return blocks, {'bs_version': version, 'quantization': quant,
                    'bit_order': 'MSB-first within little-endian 16-bit source words; bit offsets are logical stream offsets',
                    'dc_coding': ('v3 differential Cr/Cb/shared-Y predictors reset to zero per frame; DC must remain in signed 10-bit range, no wrap guessed'
                                  if version == 3 else 'v1/v2 signed 10-bit absolute DC'),
                    'declared_mdec_bytes': declared * 4, 'decoded_runlevel_halfwords': halfwords,
                    'block_count': len(blocks), 'macroblock_count': len(blocks) // 6,
                    'escape_codes': escapes, 'zero_level_escape_codes': zero_escapes,
                    'end_marker_bit_offset': 64 + stream.position - 10,
                    'consumed_stream_bits': 64 + stream.position,
                    'unused_bits_in_final_source_halfword': (-stream.position) % 16,
                    'trailing_extent': {'offset': consumed, 'size': len(data) - consumed,
                                        'sha256': hashlib.sha256(data[consumed:]).hexdigest(),
                                        'status': 'after final source halfword containing VLC end marker'}}


def _context(metadata, context):
    if (not isinstance(context, dict) or not isinstance(context.get('evidence'), list)
            or not context['evidence'] or not all(isinstance(value, str) and value.strip() for value in context['evidence'])):
        raise UnsupportedMdec('dimensions require source-backed caller context; BS has no width/height')
    width, height = context.get('width'), context.get('height')
    if type(width) is not int or type(height) is not int:
        raise UnsupportedMdec('caller dimensions must be integers')
    # Literal dimensions in background_images.c plus direct callers in
    # NAME_ENT/80132000.c, NAME_ENT/80144AD0.c and DATE[2] scene code.
    if (width, height) not in ((320, 88), (320, 160), (320, 240), (384, 160), (640, 160)):
        raise UnsupportedMdec('dimensions are outside source-proved background decode shapes')
    padded = (height + 15) // 16 * 16
    if (width // 16) * (padded // 16) != metadata['macroblock_count']:
        raise ImageFormatError('caller dimensions disagree with complete VLC macroblock count')
    return width, height, padded


def probe_mdec(data: bytes, source_path: str, context=None) -> dict:
    """Validate BS v1/v2/v3 and require explicit geometry evidence for export."""
    try:
        _, metadata = _coefficients(data)
        metadata['source_path'] = source_path
        try:
            width, height, padded = _context(metadata, context)
        except UnsupportedMdec as error:
            return {'status': 'unsupported', 'format': 'sony-bs-mdec', 'reason': str(error), 'metadata': metadata}
        metadata.update(width=width, height=height, padded_height=padded)
        return {'status': 'supported', 'format': 'sony-bs-mdec', 'metadata': metadata}
    except UnsupportedMdec as error:
        return {'status': 'unsupported', 'format': 'sony-bs-mdec', 'reason': str(error)}
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'sony-bs-mdec', 'reason': str(error)}


def export_mdec(data: bytes, output_dir, asset_id: str, *, source_path='', context=None) -> dict:
    """Export one source-context-proved background with an explicitly identified fixed-point numerical model."""
    blocks, metadata = _coefficients(data)
    width, height, padded = _context(metadata, context)
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe MDEC asset_id')
    # Import after format constants are initialized: the independent numerical
    # model reuses this module's bounded BS parser and immutable SDK tables.
    from .image_mdec_model import MODEL, decode_padded_words
    model = copy.deepcopy(MODEL)
    model["implementation_sha256"] = {name: hashlib.sha256(Path(__file__).with_name(name).read_bytes()).hexdigest()
                                       for name in ("image_mdec.py", "image_mdec_model.py")}
    words = decode_padded_words(data, width, height)
    padded_rgba = bytearray(width * padded * 4)
    for index, (word,) in enumerate(struct.iter_unpack('<H', words)):
        channels = [(word >> shift) & 31 for shift in (0, 5, 10)]
        padded_rgba[index * 4:index * 4 + 4] = bytes(
            [(value << 3) | (value >> 2) for value in channels] + [255])
    rgba = padded_rgba[:width * height * 4]
    png = asset_id + '.background.png'
    mask = asset_id + '.background.stp.png'
    padded_png = asset_id + '.background.padded.png'
    padded_mask = asset_id + '.background.padded.stp.png'
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    with (output_dir / png).open('xb') as output:
        output.write(_png(width, height, rgba, 4))
    with (output_dir / mask).open('xb') as output:
        output.write(_png(width, height, bytes([255]) * (width * height), 1))
    with (output_dir / padded_png).open('xb') as output:
        output.write(_png(width, padded, padded_rgba, 4))
    with (output_dir / padded_mask).open('xb') as output:
        output.write(_png(width, padded, bytes([255]) * (width * padded), 1))
    metadata.update(source_sha256=hashlib.sha256(data).hexdigest(), source_size=len(data),
                    source_path=source_path, width=width, height=height, padded_height=padded,
                    unrepresented_pixel_regions=([[0, height, width, padded - height]] if padded > height else []),
                    coverage_policy='complete VLC coefficient stream validated; active caller rectangle and complete padded MDEC output are both exported; source-read sectors beyond the declared file extent are recorded but never fed to this decoder',
                    padded_decode={'png': padded_png, 'stp_png': padded_mask,
                                   'width': width, 'height': padded,
                                   'active_crop': [0, 0, width, height],
                                   'active_crop_height': height,
                                   'padded_rows_not_uploaded_by_caller': padded - height},
                    geometry_context=context, coefficient_order='column-major macroblocks; Cr,Cb,Ytop-left,Ytop-right,Ybottom-left,Ybottom-right',
                    decoder_model=model,
                    color_policy='reviewed fixed-point RGB555 model; PNG channels use bit replication (q<<3)|(q>>2); no screenshot normalization',
                    alpha_policy='opaque decoded background; no CLUT or source alpha',
                    source_output_bit15=1,
                    stp_policy='game DecDCT input mode2 forces output bit15=1 for every RGB555 pixel; runtime sprite blending external',
                    hardware_pixel_parity_verified=False, round_trip_verified=False,
                    limitations=['hardware and Redux AAN pixel parity not verified', 'BS-to-runlevel transport not independently compared with game output', 'padding rows cropped only by caller-proved height'],
                    evidence=['src/main/memory_card.c:bg_still_decode_to_vram', 'src/main/background_images.c:dec_bg_decode_to_vram',
                              'asm/libpress_main.s:func_800869EC,func_800868BC',
                              'https://psx-spx.consoledev.net/cdromfileformats/',
                              'https://psx-spx.consoledev.net/macroblockdecodermdec/'])
    return {'status': 'exported', 'format': 'sony-bs-mdec', 'files': [png, mask, padded_png, padded_mask],
            'images': [{'png': png, 'stp_png': mask, 'frame_index': 0, 'variant': 'background', 'width': width,
                        'height': height, 'parts': [], 'palette_status': 'not applicable; YCbCr coded',
                        'stp_status': 'constant_one_from_game_decode_mode',
                        'preview_kind': 'decoded_background', 'source_region': [0, 0, width, height]}],
            'metadata': metadata}
