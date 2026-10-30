"""T-39012: macroblock-local splice of Sony BS v1/v2 still streams.

BS v1/v2 code every block's DC as an absolute signed 10-bit value, so each
16x16 macroblock decodes without reference to the others. This module

* splits a stream into per-macroblock bit spans and quantized levels;
* re-emits untouched macroblocks bit-for-bit, so any MDEC decoder (hardware,
  emulator or tools/assets/image_mdec_model.py) produces identical pixels there;
* re-encodes chosen macroblocks at the frame's own quantizer, starting from the
  original levels (weighted least-squares delta, then greedy +-1 descent scored
  in exact RGB555 words from the pinned fixed-point model plus lambda * VLC bits);
* keeps the header layout, rewrites only the declared MDEC extent, and keeps
  any original bytes after the stream at their original offsets.

BS v3 (DC DPCM) is refused: changing one DC would re-code every later DC.
Only the repository parser (image_mdec._coefficients) and model decide whether a
spliced stream is valid; nothing here bypasses them.
"""
import hashlib
import struct

import numpy as np

from . import image_mdec as bs
from . import image_mdec_model as model

END_MARKER = '0111111111'  # 0x1FF as a 10-bit DC
ESCAPE = '000001'
EOB = '10'
_AC_BY_VALUE = {}
for _code, _value in sorted(bs.AC_CODES.items(), key=lambda item: (len(item[0]), item[0])):
    _AC_BY_VALUE.setdefault(_value, _code)


class SpliceError(ValueError):
    """A splice cannot be produced without changing data it must keep."""


def _stream_bits(data):
    words = struct.unpack('<%dH' % ((len(data) - 8) // 2), data[8:8 + (len(data) - 8) // 2 * 2])
    return ''.join(format(word, '016b') for word in words)


def _pack(bits):
    bits += '0' * (-len(bits) % 16)
    return b''.join(struct.pack('<H', int(bits[i:i + 16], 2)) for i in range(0, len(bits), 16))


def parse(data):
    """Return header facts and per-macroblock spans/levels for a BS v1/v2 stream."""
    _, metadata = bs._coefficients(data)  # the project reader validates first
    declared, magic, quant, version = struct.unpack_from('<4H', data)
    if version not in (1, 2):
        raise SpliceError('BS v%d DC prediction cannot be spliced per macroblock' % version)
    stream = bs._Bits(data[8:])
    macroblocks = []
    while True:
        start = stream.position
        blocks, halfwords = [], 0
        for index in range(6):
            raw = stream.take(10)
            if raw == 511:
                if index:
                    raise SpliceError('end marker inside a macroblock')
                end_marker = start
                break
            levels = [0] * 64
            levels[0] = bs._signed(raw)
            halfwords += 1
            position = 0
            while True:
                prefix = ''
                while True:
                    prefix += str(stream.take(1))
                    if prefix == EOB:
                        packed = None
                        break
                    if prefix == ESCAPE:
                        packed = stream.take(16)
                        break
                    if prefix in bs.AC_CODES:
                        packed = bs.AC_CODES[prefix]
                        if stream.take(1):
                            packed = (packed & 0xfc00) | ((-(packed & 1023)) & 1023)
                        break
                halfwords += 1
                if packed is None:
                    break
                # Zero-level escapes (an original encoder habit) only advance the run.
                position += (packed >> 10) + 1
                levels[position] = bs._signed(packed & 1023)
            blocks.append(levels)
        else:
            macroblocks.append({'bits': (start, stream.position), 'halfwords': halfwords, 'levels': blocks})
            continue
        break
    consumed = 8 + (end_marker + 10 + 15) // 16 * 2
    return {'declared_words': declared, 'magic': magic, 'quantization': quant, 'version': version,
            'macroblocks': macroblocks, 'end_marker_bit': end_marker, 'consumed_bytes': consumed,
            'trailing': data[consumed:], 'metadata': metadata}


def encode_block(levels):
    """VLC bits and run/level halfword count for one block of zigzag-ordered levels."""
    dc = levels[0]
    if not -512 <= dc <= 510:  # 511 would read as the end marker
        raise SpliceError('DC level outside the codable range')
    bits, halfwords, run = [format(dc & 1023, '010b')], 1, 0
    for level in levels[1:]:
        if level == 0:
            run += 1
            continue
        code = _AC_BY_VALUE.get((run << 10) | abs(level))
        if code is not None:
            bits.append(code + ('1' if level < 0 else '0'))
        elif -512 <= level <= 511:
            bits.append(ESCAPE + format((run << 10) | (level & 1023), '016b'))
        else:
            raise SpliceError('AC level outside the codable range')
        halfwords += 1
        run = 0
    bits.append(EOB)
    return ''.join(bits), halfwords + 1


def rebuild(data, replacements=None):
    """Splice replacement macroblocks (index -> six level lists) into `data`.

    Returns (bytes, facts). Untouched macroblocks keep their original bits. The
    original bytes after the stream stay at their offsets. The stream may grow
    into the zero bytes that lead the trailing data; reaching the first non-zero
    trailing byte fails. Without non-zero trailing bytes the file may grow past
    its original length, which facts['file_extent_grew'] records.
    """
    replacements = replacements or {}
    parsed = parse(data)
    source_bits = _stream_bits(data)
    parts, halfwords, edited_bits = [], 0, {}
    for index, macroblock in enumerate(parsed['macroblocks']):
        if index in replacements:
            encoded = [encode_block(levels) for levels in replacements[index]]
            bits = ''.join(part for part, _ in encoded)
            halfwords += sum(count for _, count in encoded)
            edited_bits[index] = {'original': macroblock['bits'][1] - macroblock['bits'][0], 'new': len(bits)}
        else:
            bits = source_bits[macroblock['bits'][0]:macroblock['bits'][1]]
            halfwords += macroblock['halfwords']
        parts.append(bits)
    if set(replacements) - set(range(len(parsed['macroblocks']))):
        raise SpliceError('replacement macroblock index outside the frame')
    declared = (halfwords * 2 + 127) // 128 * 128 // 4
    if declared > 0xFFFF:
        raise SpliceError('declared MDEC extent overflows the header word')
    body = struct.pack('<4H', declared, parsed['magic'], parsed['quantization'], parsed['version'])
    body += _pack(''.join(parts) + END_MARKER)
    consumed, trailing = parsed['consumed_bytes'], parsed['trailing']
    kept = next((offset for offset, value in enumerate(trailing) if value), len(trailing))  # leading zero run
    if len(body) <= consumed:
        result = body + bytes(consumed - len(body)) + trailing
        trailing_policy = 'stream did not grow; original trailing bytes kept at original offsets'
    elif len(body) <= consumed + kept:
        result = body + bytes(consumed + kept - len(body)) + trailing[kept:]
        trailing_policy = ('stream grew into the leading zero run of the original trailing bytes; '
                           'later bytes kept at original offsets')
    elif kept == len(trailing):
        result = body
        trailing_policy = 'file grew past its original length (no non-zero trailing bytes to keep)'
    else:
        raise SpliceError('spliced stream would overwrite non-zero trailing bytes')
    bs._coefficients(result)  # must parse with the project reader
    return result, {'declared_words': {'original': parsed['declared_words'], 'new': declared},
                    'stream_bytes': {'original': consumed, 'new': len(body)},
                    'file_bytes': {'original': len(data), 'new': len(result)},
                    'sector_budget_bytes': (len(data) + 2047) // 2048 * 2048,
                    'fits_original_sectors': len(result) <= (len(data) + 2047) // 2048 * 2048,
                    'trailing_bytes': {'size': len(trailing), 'sha256': hashlib.sha256(trailing).hexdigest(),
                                       'nonzero': any(trailing), 'leading_zero_bytes': kept,
                                       'policy': trailing_policy},
                    'file_extent_grew': len(result) > len(data),
                    'edited_macroblock_bits': edited_bits}


def _dequantized(levels, quant):
    block = [0] * 64
    block[0] = model.dequantize(levels[0], bs.QUANT[0], quant, True)
    for index in range(1, 64):
        if levels[index]:
            block[bs.ZIGZAG[index]] = model.dequantize(levels[index], bs.QUANT[index], quant)
    return block


def block_output(levels, quant):
    """Exact fixed-point spatial output (signed 8-bit, row-major) for one block."""
    return model.inverse_transform(_dequantized(levels, quant))


def macroblock_rgb5(outputs):
    """16x16x3 RGB555 channels from six block outputs (Cr, Cb, Y0..Y3)."""
    cr, cb, *luma = outputs
    result = np.zeros((16, 16, 3), int)
    for y in range(16):
        for x in range(16):
            chroma = y // 2 * 8 + x // 2
            word = model.color_word(luma[y // 8 * 2 + x // 8][y % 8 * 8 + x % 8], cb[chroma], cr[chroma])
            result[y, x] = (word & 31, (word >> 5) & 31, (word >> 10) & 31)
    return result


def float_macroblock_rgb5(levels6, quant):
    """Second decode: float IDCT and float colour, for a legibility cross-check only."""
    n = np.arange(8)
    basis = np.cos((2 * n[:, None] + 1) * n[None, :] * np.pi / 16) * np.where(n == 0, np.sqrt(1 / 8), np.sqrt(2 / 8))[None, :]
    outputs = []
    for levels in levels6:
        # Dequantized values carry four fractional bits; the fixed-point matrix
        # passes then equal an orthonormal 2-D inverse DCT.
        coefficients = np.array(_dequantized(levels, quant), float).reshape(8, 8) / 16.0
        outputs.append(basis @ coefficients @ basis.T)
    cr, cb = outputs[0], outputs[1]
    result = np.zeros((16, 16, 3), int)
    for y in range(16):
        for x in range(16):
            lum = outputs[2 + y // 8 * 2 + x // 8][y % 8, x % 8]
            r, b = cr[y // 2, x // 2], cb[y // 2, x // 2]
            channels = (lum + 1.402 * r, lum - 0.344 * b - 0.714 * r, lum + 1.772 * b)
            result[y, x] = [min(31, int(np.clip(c, -128, 127) + 128 + 4) >> 3) for c in channels]
    return result


def _linear_basis(quant):
    """Approximate linear response of each zigzag level, for the least-squares start only."""
    rows = np.zeros((64, 64))
    for index in range(64):
        levels = [0] * 64
        levels[index] = 40
        rows[index] = np.array(block_output(levels, quant), float) / 40.0
    return rows


def _ycbcr(rgb5):
    rgb = rgb5.astype(float) * 8 - 128
    y = 0.299 * rgb[..., 0] + 0.587 * rgb[..., 1] + 0.114 * rgb[..., 2]
    return y, (rgb[..., 2] - y) / 1.772, (rgb[..., 0] - y) / 1.402


def encode_macroblock(levels6, goal_rgb5, weights, quant, lam, passes=2, basis=None):
    """Anchored rate-distortion re-encode of one macroblock.

    `goal_rgb5` is the 16x16x3 RGB555 image wanted; `weights` (16x16) scores each
    pixel's squared error. Starting from the original levels keeps highly
    weighted pixels near their original values. Returns six level lists.
    """
    basis = _linear_basis(quant) if basis is None else basis
    blocks = [list(levels) for levels in levels6]
    outputs = [block_output(levels, quant) for levels in blocks]
    goal_y, goal_cb, goal_cr = _ycbcr(goal_rgb5)
    chroma_weight = weights.reshape(8, 2, 8, 2).mean((1, 3)).ravel()
    targets = [goal_cr.reshape(8, 2, 8, 2).mean((1, 3)).ravel(), goal_cb.reshape(8, 2, 8, 2).mean((1, 3)).ravel()]
    targets += [goal_y[by:by + 8, bx:bx + 8].ravel() for by in (0, 8) for bx in (0, 8)]
    block_weights = [chroma_weight, chroma_weight] + [weights[by:by + 8, bx:bx + 8].ravel() for by in (0, 8) for bx in (0, 8)]
    for index in range(6):
        root = np.sqrt(block_weights[index])
        residual = (targets[index] - np.array(outputs[index], float)) * root
        delta = np.linalg.lstsq(basis.T * root[:, None], residual, rcond=None)[0]
        low, high = -512, 510
        blocks[index] = [int(np.clip(round(old + step), low if k == 0 else -511, high if k == 0 else 511))
                         for k, (old, step) in enumerate(zip(blocks[index], delta))]
        outputs[index] = block_output(blocks[index], quant)
    bits = [len(encode_block(levels)[0]) for levels in blocks]

    def cost(candidate_outputs, candidate_bits):
        error = macroblock_rgb5(candidate_outputs) - goal_rgb5
        return float((weights[..., None] * error * error).sum()) + lam * sum(candidate_bits)

    current = cost(outputs, bits)
    for _ in range(passes):
        improved = False
        for index in range(6):
            for k in range(64):
                for step in (-1, 1):
                    trial = list(blocks[index])
                    trial[k] += step
                    if not (-512 <= trial[0] <= 510) or abs(trial[k]) > 511:
                        continue
                    trial_outputs = list(outputs)
                    trial_outputs[index] = block_output(trial, quant)
                    trial_bits = list(bits)
                    trial_bits[index] = len(encode_block(trial)[0])
                    value = cost(trial_outputs, trial_bits)
                    if value < current:
                        blocks[index], outputs, bits, current, improved = trial, trial_outputs, trial_bits, value, True
        if not improved:
            break
    return blocks


def macroblock_index(x, y, padded_height):
    """Column-major macroblock index of the macroblock holding pixel (x, y)."""
    return x // 16 * (padded_height // 16) + y // 16


def decode_rgb5(data, width, height):
    """Padded RGB555 channels (H x W x 3) through the pinned fixed-point model."""
    padded = (height + 15) // 16 * 16
    words = np.frombuffer(model.decode_padded_words(data, width, height), '<u2').reshape(padded, width).astype(int)
    if not (words & 0x8000).all():
        raise SpliceError('decoded word without STP bit')
    return np.stack([words & 31, (words >> 5) & 31, (words >> 10) & 31], -1)
