"""T-39400: single-stroke skeleton letters for port-made English lettering on BS stills.

The glyphs are authored here as centre lines on a ten-unit cap-height grid. They
are not a system font, not a raster font and not an image generator: a glyph is a
list of poly-lines, and the lettering of a sign is made by laying the skeletons
out in the sign's own box (width fitted, rotated when the original sign runs
vertically) and stroking them with a weight chosen from the original lettering.
The strokes are rasterised at 8x and box-filtered, which gives the same soft
edge the MDEC decode gives the original lettering.

Only the capital letters, the digit one, and the hyphen, apostrophe, period are
defined; `text_units` (and so `layout`, `text_extent` and `strokes_for`) refuses
any other character. `render_coverage` strokes poly-lines and knows no characters.

The layout functions take an optional `kern` mapping {(left, right): glyph units}
that is added to the advance of each such pair (the skeleton face sets every pair
the same, which leaves a hole between a slanted leg and a crossbar).
"""
import math

import numpy as np
from PIL import Image, ImageDraw

CAP = 10.0
GAP = 2.4       # letter spacing in glyph units
SPACE = 3.2     # word space in glyph units
SUPERSAMPLE = 8


def arc(cx, cy, rx, ry, start, end, steps=14):
    """Points of an ellipse arc; angles in degrees, counter-clockwise on screen, y down."""
    out = []
    for i in range(steps + 1):
        a = math.radians(start + (end - start) * i / steps)
        out.append((cx + rx * math.cos(a), cy - ry * math.sin(a)))
    return out


GLYPHS = {
    'A': (6.4, [[(0, 10), (3.2, 0), (6.4, 10)], [(1.1, 6.6), (5.3, 6.6)]]),
    'B': (5.8, [[(0, 0), (0, 10)],
                [(0, 0), (2.6, 0)] + arc(2.6, 2.5, 2.9, 2.5, 90, -90, 12)[1:] + [(0, 5.0)],
                [(0, 5.0), (2.9, 5.0)] + arc(2.9, 7.5, 3.0, 2.5, 90, -90, 12)[1:] + [(0, 10)]]),
    'C': (6.0, [arc(3.3, 5, 3.3, 5, 42, 318, 20)]),
    'D': (6.0, [[(0, 0), (0, 10)], [(0, 0), (2.2, 0)] + arc(2.2, 5, 3.6, 5, 90, -90, 14)[1:-1] + [(2.2, 10), (0, 10)]]),
    'E': (5.0, [[(5, 0), (0, 0), (0, 10), (5, 10)], [(0, 5), (4, 5)]]),
    'F': (4.8, [[(4.8, 0), (0, 0), (0, 10)], [(0, 4.8), (4, 4.8)]]),
    'G': (6.4, [arc(3.3, 5, 3.3, 5, 42, 360, 22) + [(3.7, 5.2)]]),
    'H': (6.0, [[(0, 0), (0, 10)], [(6, 0), (6, 10)], [(0, 5), (6, 5)]]),
    'I': (0.0, [[(0, 0), (0, 10)]]),
    'J': (4.4, [[(4.4, 0), (4.4, 6.8)] + arc(2.4, 6.8, 2.0, 3.2, 0, -180, 10)]),
    'K': (6.0, [[(0, 0), (0, 10)], [(5.6, 0), (0.4, 5.6)], [(1.9, 4.3), (6, 10)]]),
    'L': (4.8, [[(0, 0), (0, 10), (4.8, 10)]]),
    'M': (8.0, [[(0, 10), (0, 0), (4, 6.6), (8, 0), (8, 10)]]),
    'N': (6.2, [[(0, 10), (0, 0), (6.2, 10), (6.2, 0)]]),
    'O': (6.6, [arc(3.3, 5, 3.3, 5, 0, 360, 28)]),
    'P': (5.6, [[(0, 10), (0, 0), (2.6, 0)] + arc(2.6, 2.7, 3.0, 2.7, 90, -90, 12)[1:] + [(0, 5.4)]]),
    'Q': (6.6, [arc(3.3, 5, 3.3, 5, 0, 360, 28), [(4.0, 7.2), (6.6, 10.4)]]),
    'R': (6.0, [[(0, 10), (0, 0), (2.7, 0)] + arc(2.7, 2.7, 3.1, 2.7, 90, -90, 12)[1:] + [(0, 5.4)], [(2.7, 5.4), (6, 10)]]),
    'S': (5.6, [arc(2.9, 2.55, 2.7, 2.55, 28, 270, 14) + arc(2.9, 7.45, 2.9, 2.55, 90, -152, 14)]),
    'T': (6.0, [[(0, 0), (6, 0)], [(3, 0), (3, 10)]]),
    'U': (6.0, [[(0, 0), (0, 6.6)] + arc(3, 6.6, 3, 3.4, 180, 360, 14) + [(6, 0)]]),
    'V': (6.4, [[(0, 0), (3.2, 10), (6.4, 0)]]),
    'W': (9.2, [[(0, 0), (2.3, 10), (4.6, 2.6), (6.9, 10), (9.2, 0)]]),
    'X': (6.2, [[(0, 0), (6.2, 10)], [(6.2, 0), (0, 10)]]),
    'Y': (6.2, [[(0, 0), (3.1, 5.2)], [(6.2, 0), (3.1, 5.2)], [(3.1, 5.2), (3.1, 10)]]),
    'Z': (5.8, [[(0, 0), (5.8, 0), (0, 10), (5.8, 10)]]),
    '1': (3.4, [[(0, 2.6), (3.0, 0), (3.0, 10)]]),
    '-': (3.8, [[(0, 5.2), (3.8, 5.2)]]),
    "'": (0.0, [[(0, 0), (0, 2.8)]]),
    '.': (0.0, [[(0, 9.7), (0, 10)]]),
}


def advance(text, i, gap=GAP, kern=None):
    """Advance in glyph units from character i of text to the next one (kern pair included)."""
    ch = text[i]
    step = SPACE if ch == ' ' else GLYPHS[ch][0] + gap
    if kern and i + 1 < len(text):
        step += kern.get((ch, text[i + 1]), 0.0)
    return step


def text_units(text, gap=GAP, kern=None):
    """Total advance of text in glyph units, without a trailing gap."""
    for ch in text:
        if ch != ' ' and ch not in GLYPHS:
            raise ValueError('no skeleton for character %r' % ch)
    total = 0.0
    for i in range(len(text)):
        total += advance(text, i, gap, kern)
    return total - gap


def layout(text, length, cap, aspect, gap=GAP, kern=None):
    """(x scale px per unit, y scale, left offsets in units) for text fitted into length px.

    The horizontal scale is the largest one not above cap/10*aspect that keeps the
    text inside length; a text that is shorter than length is centred.
    """
    units = text_units(text, gap, kern)
    sy = cap / CAP
    sx = min(sy * aspect, length / units) if units > 0 else sy * aspect
    start = (length - units * sx) / 2.0
    return sx, sy, start


def text_extent(text, length, cap, aspect, gap=GAP, kern=None):
    sx, sy, start = layout(text, length, cap, aspect, gap, kern)
    return {'x_scale': round(sx, 4), 'y_scale': round(sy, 4), 'length_px': round(text_units(text, gap, kern) * sx, 3), 'offset_px': round(start, 3)}


def strokes_for(text, length, cap, aspect, gap=GAP, kern=None):
    """Skeleton poly-lines of text in a local frame: u along the text (px), v down the cap (px)."""
    sx, sy, start = layout(text, length, cap, aspect, gap, kern)
    cursor = start
    lines = []
    for i, ch in enumerate(text):
        if ch != ' ':
            for part in GLYPHS[ch][1]:
                lines.append([(cursor + px * sx, py * sy) for px, py in part])
        cursor += advance(text, i, gap, kern) * sx
    return lines


def to_image(lines, origin, rotation):
    """Map local (u, v) lines into image pixels.

    rotation 0: u goes right, v goes down from origin. rotation 90: the text runs down
    the image and the tops of the letters face right; origin is the top-right corner.
    """
    ox, oy = origin
    if rotation == 0:
        return [[(ox + u, oy + v) for u, v in line] for line in lines]
    if rotation == 90:
        return [[(ox - v, oy + u) for u, v in line] for line in lines]
    raise ValueError('unsupported rotation')


def render_coverage(shape, lines, stroke, offset=(0, 0)):
    """Coverage 0..1 (float, shape = (height, width)) of lines stroked at `stroke` px.

    Lines are in image coordinates; `offset` is the image position of the array's top-left.
    """
    height, width = shape
    big = Image.new('L', (width * SUPERSAMPLE, height * SUPERSAMPLE), 0)
    draw = ImageDraw.Draw(big)
    w = max(1, int(round(stroke * SUPERSAMPLE)))
    radius = w / 2.0
    for line in lines:
        pts = [((x - offset[0]) * SUPERSAMPLE, (y - offset[1]) * SUPERSAMPLE) for x, y in line]
        if len(pts) == 1 or all(p == pts[0] for p in pts):
            x, y = pts[0]
            draw.ellipse([x - radius, y - radius, x + radius, y + radius], fill=255)
            continue
        draw.line(pts, fill=255, width=w, joint='curve')
        for x, y in (pts[0], pts[-1]):
            draw.ellipse([x - radius, y - radius, x + radius, y + radius], fill=255)
    small = big.reduce(SUPERSAMPLE)
    return np.asarray(small, np.float64) / 255.0
