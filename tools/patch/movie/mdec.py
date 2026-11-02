"""Vectorised MDEC decode and macroblock encode for BS v1/v2 movie frames (T-39509).

The decoder is the fixed-point model of tools/assets/image_mdec_model.py written over numpy
arrays (same dequantisation, the same two rounded 18-bit matrix passes, 9-bit wrap and 8-bit
saturation, the same colour sums), so it gives the model's numbers for a whole frame at once.
`channels` returns the 8-bit channels the MDEC writes in 24-bit mode; `rgb555` rounds them to
the 15-bit words the model returns. tools/test_patch_movie.py checks it against the model.

Levels are the quantised values in zigzag order, shape (macroblocks, 6, 64), blocks Cr, Cb,
Y0 (top left), Y1 (top right), Y2, Y3, as tools/assets/mdec_splice.parse returns them.
Macroblocks run column-major (top to bottom, then left to right).

The encoder works on chosen macroblocks only: it starts from the original levels and adds the
quantised DCT of the error against the wanted image, a few times. The wanted block values are
the original block outputs plus the change in YCbCr, so a luma block whose texels do not
change keeps its levels exactly. `coarse` multiplies the
quantiser steps of those macroblocks (the levels become multiples of it), the per-macroblock
way to spend fewer bits that a v1 frame with one quantiser for all macroblocks allows.
"""
import numpy as np

from tools.assets import image_mdec as bs
from tools.assets import image_mdec_model as model

QUANT = np.array(bs.QUANT, np.int64)
ZIGZAG = np.array(bs.ZIGZAG, np.int64)  # zigzag index -> natural (row-major) index
MATRIX = np.array(model.MATRIX, np.int64)
# Orthonormal DCT basis: the dequantised values carry four fractional bits, and the two
# fixed-point passes then equal an orthonormal 2-D inverse DCT (mdec_splice.float_macroblock_rgb5).
_N = np.arange(8)
BASIS = np.cos((2 * _N[:, None] + 1) * _N[None, :] * np.pi / 16) * np.where(_N == 0, np.sqrt(1 / 8), np.sqrt(2 / 8))[None, :]
LEVEL_MAX = 511
DC_MIN, DC_MAX = -512, 510  # 511 is the end marker


def levels_array(parsed):
    """(macroblocks, 6, 64) int64 levels from mdec_splice.parse output."""
    return np.array([mb['levels'] for mb in parsed['macroblocks']], np.int64).reshape(-1, 6, 64)


def dequantize(levels, scale):
    """Model dequantisation (four fractional bits, half-unit bias toward zero, signed 15-bit),
    in natural block order."""
    levels = np.asarray(levels, np.int64)
    product = (levels * QUANT * scale) >> 3
    product[..., 0] = levels[..., 0] * QUANT[0]
    bias = np.where(levels == 0, 0, np.where(levels < 0, 8, -8))
    value = np.clip(product * 16 + bias, -16384, 16383)
    out = np.zeros(value.shape, np.int64)
    out[..., ZIGZAG] = value
    return out.reshape(value.shape[:-1] + (8, 8))


def idct(coefficients):
    """Model inverse transform: (..., 8, 8) dequantised -> (..., 8, 8) signed 8-bit outputs."""
    rows = np.asarray(coefficients, np.int64)
    for _ in range(2):
        # new[y][x] = sum_z rows[z][y] * M[z][x], rounded at bit 18, clamped to 16 bits
        rows = np.clip((np.swapaxes(rows, -1, -2) @ MATRIX + 131072) >> 18, -32768, 32767)
    wrapped = rows & 511
    wrapped = np.where(wrapped >= 256, wrapped - 512, wrapped)
    return np.clip(wrapped, -128, 127)


def block_outputs(levels, scale):
    return idct(dequantize(levels, scale))


def _assemble(outputs):
    """(M, 6, 8, 8) block outputs -> (M, 16, 16) luma, (M, 8, 8) Cb, (M, 8, 8) Cr."""
    y = np.zeros(outputs.shape[:1] + (16, 16), np.int64)
    y[:, :8, :8], y[:, :8, 8:], y[:, 8:, :8], y[:, 8:, 8:] = outputs[:, 2], outputs[:, 3], outputs[:, 4], outputs[:, 5]
    return y, outputs[:, 1], outputs[:, 0]


def macroblock_channels(outputs):
    """(M, 6, 8, 8) block outputs -> (M, 16, 16, 3) 8-bit RGB channels (model colour sums)."""
    y, cb, cr = _assemble(outputs)
    up = lambda c: c.repeat(2, axis=1).repeat(2, axis=2)
    cb, cr = up(cb), up(cr)
    offsets = ((359 * cr + 128) >> 8,
               (((-88 * cb) & ~31) + ((-183 * cr) & ~7) + 128) >> 8,
               (454 * cb + 128) >> 8)
    out = []
    for offset in offsets:
        v = (y + offset) & 511
        v = np.where(v >= 256, v - 512, v)
        out.append(np.clip(v, -128, 127) + 128)
    return np.stack(out, -1)


def place(macroblocks, width, height):
    """(M, 16, 16, C) macroblocks, column-major -> (height, width, C) frame."""
    rows = (height + 15) // 16
    columns = width // 16
    grid = macroblocks.reshape(columns, rows, 16, 16, -1)
    frame = grid.transpose(1, 2, 0, 3, 4).reshape(rows * 16, columns * 16, -1)
    return frame[:height]


def cut(frame, width, height):
    """(height, width, C) frame -> (M, 16, 16, C) column-major macroblocks (edge rows repeated)."""
    rows = (height + 15) // 16
    padded = np.concatenate([frame, frame[-1:].repeat(rows * 16 - height, axis=0)]) if rows * 16 > height else frame
    grid = padded.reshape(rows, 16, width // 16, 16, -1).transpose(2, 0, 1, 3, 4)
    return grid.reshape(-1, 16, 16, frame.shape[-1])


def channels(levels, scale, width, height):
    """Whole frame: (height, width, 3) uint8 channels as the MDEC writes them in 24-bit mode."""
    return place(macroblock_channels(block_outputs(levels, scale)), width, height).astype(np.uint8)


def rgb555(rgb8):
    """8-bit channels -> 5-bit channels (the model's rounding)."""
    return np.minimum((np.asarray(rgb8, np.int64) + 4) >> 3, 31)


def _targets(rgb):
    """(M, 16, 16, 3) wanted 8-bit RGB -> per-block float targets (M, 6, 8, 8) in the MDEC's signed
    YCbCr (inverse of the decoder's colour sums), chroma averaged over 2x2."""
    c = np.asarray(rgb, float) - 128
    y = 0.299 * c[..., 0] + 0.587 * c[..., 1] + 0.114 * c[..., 2]
    cb = (c[..., 2] - y) / (454 / 256)
    cr = (c[..., 0] - y) / (359 / 256)
    half = lambda v: v.reshape(-1, 8, 2, 8, 2).mean((2, 4))
    return np.stack([half(cr), half(cb), y[:, :8, :8], y[:, :8, 8:], y[:, 8:, :8], y[:, 8:, 8:]], 1)


def steps(scale, coarse=1):
    """Level step of each zigzag coefficient, as a coefficient (orthonormal DCT) value."""
    s = QUANT * scale / 8.0
    s[0] = QUANT[0]
    return s * coarse


def encode(levels, rgb, scale, coarse=1, passes=4, deadzone=0.0):
    """New levels for macroblocks `levels` (M, 6, 64) so that they decode close to `rgb`
    (M, 16, 16, 3) 8-bit. The wanted block values are the original block outputs plus the
    YCbCr difference between `rgb` and the original decode, so a block whose texels do not
    change keeps its levels. coarse > 1 restarts from zero with levels that are multiples of it;
    deadzone (0..0.5) rounds small level changes to zero."""
    levels = np.asarray(levels, np.int64)
    original = block_outputs(levels, scale)
    target = original + _targets(rgb) - _targets(macroblock_channels(original))
    step = steps(scale, coarse)
    current = levels.copy() if coarse == 1 else np.zeros(levels.shape, np.int64)
    for _ in range(passes):
        error = target - block_outputs(current, scale)
        coef = BASIS.T @ error @ BASIS  # forward orthonormal DCT, natural order
        delta = coef.reshape(coef.shape[:-2] + (64,))[..., ZIGZAG] / step
        change = np.sign(delta) * np.floor(np.abs(delta) + 0.5 - deadzone)
        if not change.any():
            break
        current = current + change.astype(np.int64) * coarse
        current[..., 0] = np.clip(current[..., 0], DC_MIN, DC_MAX)
        current[..., 1:] = np.clip(current[..., 1:], -LEVEL_MAX, LEVEL_MAX)
    return current
