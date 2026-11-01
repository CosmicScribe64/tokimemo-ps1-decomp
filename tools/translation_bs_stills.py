#!/usr/bin/env python3
"""T-39412: texture-keeping English drafts for BS background stills (signs, gate plate, roster boards, chalkboard).

Follow-up to tools/translation_bs_signs.py (T-39400). That tool rebuilt each sign face from a fitted plane,
which flattened stone and gradient. Here every plan keeps the picture as it is and changes only

1. the pixels of the Japanese lettering (found as pixels of the lettering colour against the smooth local
   background and grown by a margin for the soft MDEC edge), rebuilt from the clean pixels around them with
   the grain of nearby clean pixels (tools/translation_bs_brush.py: `stroke_mask`, `inpaint`), and
2. the pixels the English strokes cover.

The stroke letters are the single-stroke skeletons of tools/translation_bs_font.py, drawn either with a
constant stroke (monument, ticket plaque, chalk heading) or with the broad-nib brush stroker of
tools/translation_bs_brush.py (gate plate, calligraphy close-ups). Signs with 5 to 6 px of lettering
(placard, library sign, shop line, chalk date) use the hand-set pixel glyphs of PIXEL_FONT instead, so that
no two letters touch at 1x.

`build` decodes the original BS files with the pinned fixed-point MDEC model and writes, under the output
directory only (ignored, never committed: they hold the original picture), the whole English stills, source
stills, original | English side-by-sides at 2x, zoomed pairs, and the per-plan crops. A crop is an RGBA
PNG of the plan rectangle in which alpha 255 marks a port-made pixel (changed against the source decode) and
alpha 0 marks "keep the source pixel": a committed crop therefore holds no original picture pixel. The manifest
holds source and decode identity, the caller-proved size, the rectangles, the 16x16 macroblocks they touch and
the lettering description. No BS file is written; the later patch step re-encodes the touched macroblocks.

`geometry` prints the size proofs; `publish` copies crops, manifest and ledger into a new checkpoint folder.
"""
import argparse
import json
import math
from pathlib import Path
import sys

import numpy as np
from PIL import Image, ImageDraw

from translation_assets import ROOT, MDEC_MODEL_ID, digest, encoded, new_output
import translation_bs_font as font
import translation_bs_brush as brush
import translation_bs_signs as signs
sys.path.insert(0, str(ROOT))
from tools.assets import image_mdec_model as mdec_model  # noqa: E402
from tools.assets import mdec_splice  # noqa: E402

SCHEMA = 'tokimemo.bs-still-drafts.v1'
TICKET = 'T-39412'
CHECKPOINT = 'stills-v1'
BS_DIR = signs.BS_DIR


# --- plans ----------------------------------------------------------------------
# Every plan: rect = the rectangle that is rebuilt (nothing outside it changes), erase = how the Japanese lettering is
# found (see draw_plan), runs = the English lines. `group` marks alternatives for the owner to choose between: two plans
# of one group never combine, each is drawn on the untouched source. Run keys beyond tools/translation_bs_signs.py:
# angle/slant/center/length (see text_lines) and brush (see tools/translation_bs_brush.py: brush_coverage).
BRUSH = {'entry': 0.35, 'taper': 0.6, 'contrast': 0.5, 'wobble': 0.14}
PLATE_ERASE = {'polarity': 'dark', 'threshold': 12, 'margin': 1}


# Pair kerning in glyph units for runs with `kern` (the skeleton face sets every pair the same, which leaves a hole
# between a slanted leg and a crossbar: PRIVATE read as PRIVA TE).
KERN = {('A', 'T'): -2.4, ('T', 'A'): -2.4, ('A', 'V'): -1.6, ('V', 'A'): -1.6}


def text_units(text, gap, kern=False):
    """font.text_units plus the KERN adjustments when `kern`."""
    units = font.text_units(text, gap)
    return units + (sum(KERN.get(pair, 0.0) for pair in zip(text, text[1:])) if kern else 0.0)


def kerned_strokes(text, length, cap, aspect, gap):
    """font.strokes_for with KERN applied: (local poly-lines, extent facts)."""
    units = text_units(text, gap, True)
    sy = cap / font.CAP
    sx = min(sy * aspect, length / units)
    start = (length - units * sx) / 2.0
    cursor, lines = start, []
    for i, ch in enumerate(text):
        if ch == ' ':
            cursor += font.SPACE * sx
            continue
        width, parts = font.GLYPHS[ch]
        lines.extend([[(cursor + px * sx, py * sy) for px, py in part] for part in parts])
        cursor += (width + gap + (KERN.get((ch, text[i + 1]), 0.0) if i + 1 < len(text) else 0.0)) * sx
    return lines, {'x_scale': round(sx, 4), 'y_scale': round(sy, 4), 'length_px': round(units * sx, 3), 'offset_px': round(start, 3)}


def natural_length(text, cap, aspect, gap, kern=False):
    """Length in px of text at cap height with the given aspect and letter gap (no fitting)."""
    return text_units(text, gap, kern) * cap / font.CAP * aspect


def brush_run(text, center, angle, cap, aspect, gap, weight, slant=0.0, kern=False, **brush_options):
    """A brush-lettered line of natural length centred on `center` (angle: 0 text goes right, 90 down the picture)."""
    run = {'text': text, 'box': [0, 0, 0, 0], 'length': round(natural_length(text, cap, aspect, gap, kern), 3), 'center': list(center),
           'angle': round(angle, 2), 'slant': slant, 'cap': cap, 'aspect': aspect, 'stroke': weight, 'gap': gap,
           'brush': {**BRUSH, **brush_options}}
    if kern:
        run['kern'] = True
    return run


# P_01_03 / P_01_03B: calligraphy columns. Each column of kanji runs down and to the left; the axis is the line through
# `point` (the middle of the column, measured on the decode) with slope dx/dy. Names are family first (the script's
# spelling, no long-vowel marks). The right-edge column (x >= 303, cut off by the frame) stays as it is. MOGAMI sits right
# of and below its kanji axis and MIZUKAWA 4 px down its axis, so that both keep a margin from the top and left edges.
ROSTER_COLUMNS = [  # (family, given, axis point, slope dx/dy)
    ('MOGAMI', '', (21, 34), -0.30), ('MIZUKAWA', 'MIHO', (46.1, 62), -0.47), ('HONDA', 'MEGUMI', (92, 68), -0.50),
    ('FUJISAKI', 'SHIORI', (147, 78), -0.44), ('HASHIMOTO', 'YOSHIKO', (210, 83), -0.38), ('TANAKA', 'SAKI', (266, 92), -0.40)]
# the brush ink of the Japanese names: median colour of the stroke cores (channel sum < 30) of each decode
ROSTER_INK = {'P_01_03': [5, 5, 6], 'P_01_03B': [6, 6, 6]}
# broad-nib calligraphy: thick across the nib edge, thin along it, swelling entry, tapered exit
CALLIGRAPHY = {'nib': 20, 'contrast': 1.2, 'entry': 0.25, 'taper': 0.7, 'wobble': 0.04}
# `smooth`: erased pixels there take the smooth fill without donor grain (the cut glyph at the left edge, where the copied
# grain showed as vertical streaks)
SKY_ERASE = {'region': [0, 0, 303, 160], 'below': 48, 'margin': 2, 'smooth': [[0, 48, 14, 32]]}


def rotated_runs(columns, cap=13, aspect=0.74, gap=4.0, weight=2.0, only=None):
    runs = []
    for family, given, point, slope in columns:
        if only and family != only:
            continue
        text = family + ('  ' + given if given else '')
        runs.append(brush_run(text, point, 90 + math.degrees(math.atan(-slope)), cap, aspect, gap, weight, **CALLIGRAPHY))
    return runs


# P_01_01 gate plate: a white plate seen at an angle, corners measured on the decode (white = every channel >= 26). The
# text runs along the top and bottom edges (9.5 degrees) and leans like the sides. Both lines share one face, size and set
# width; they sit one leading above and below the plate middle, and each is centred between the left and right plate
# edges at its own height, measured at the top and bottom of its letters (the left edge leans much more than the right one).
GATE_PLATE = [(114, 67), (236, 88), (221, 147), (73, 123)]  # top-left, top-right, bottom-right, bottom-left
GATE_ANGLE = 9.5
GATE_LEADING = 14.0
GATE_FACE = {'cap': 13.5, 'aspect': 0.71, 'gap': 2.7, 'weight': 1.75, 'slant': 12}
GATE_LINES = ('KIRAMEKI  PRIVATE', 'HIGH  SCHOOL')


def along(point, angle, du, dv):
    """`point` moved du along a text at `angle` and dv down its letters."""
    a = math.radians(angle)
    return (point[0] + du * math.cos(a) - dv * math.sin(a), point[1] + du * math.sin(a) + dv * math.cos(a))


def edge_span(point, angle, corners=GATE_PLATE):
    """(t left, t right): where the line through `point` at `angle` crosses the plate's left and right edges, as
    distances along the line from `point`."""
    a = math.radians(angle)
    ux, uy = math.cos(a), math.sin(a)
    out = []
    for (ax, ay), (bx, by) in ((corners[0], corners[3]), (corners[1], corners[2])):
        ex, ey = bx - ax, by - ay
        det = ux * -ey - uy * -ex
        out.append(((ax - point[0]) * -ey - (ay - point[1]) * -ex) / det)
    return tuple(out)


def line_margins(center, length, angle, cap, slant, corners=GATE_PLATE):
    """(left, right): the smallest gap in px, along the line, between a slanted text line and the plate's side edges,
    taken at the top and at the bottom of the letters (the left edge leans more than the letters, the right one less)."""
    t = math.tan(math.radians(slant))
    lefts, rights = [], []
    for dv in (-cap / 2.0, cap / 2.0):
        left, right = edge_span(along(center, angle, 0, dv), angle, corners)
        lefts.append((-length / 2.0 - dv * t) - left)
        rights.append(right - (length / 2.0 - dv * t))
    return min(lefts), min(rights)


def gate_centres(lines=GATE_LINES, face=GATE_FACE, angle=GATE_ANGLE, leading=GATE_LEADING, corners=GATE_PLATE):
    """Centre of each line: the plate middle moved one leading up / down, then moved along the line until the clearest
    gap to the left edge equals the one to the right edge."""
    middle = (sum(c[0] for c in corners) / 4.0, sum(c[1] for c in corners) / 4.0)
    centres = []
    for text, dv in zip(lines, (-leading, leading)):
        point = along(middle, angle, 0, dv)
        length = natural_length(text, face['cap'], face['aspect'], face['gap'], kern=True)
        left, right = line_margins(point, length, angle, face['cap'], face['slant'], corners)
        x, y = along(point, angle, (right - left) / 2.0, 0)
        centres.append((round(x, 2), round(y, 2)))
    return centres


def gate_runs(lines=GATE_LINES, face=GATE_FACE):
    return [brush_run(text, centre, GATE_ANGLE, face['cap'], face['aspect'], face['gap'], face['weight'], face['slant'], kern=True, **CALLIGRAPHY)
            for text, centre in zip(lines, gate_centres(lines, face))]


# Tiny signs (5 to 8 px of lettering): hand-set pixel glyphs, one ink colour, 1 px letter gaps, so that no two letters
# touch at 1x. Rows are top to bottom, '#' = ink.
PIXEL_FONT = {
    5: {'A': ['.#.', '#.#', '###', '#.#', '#.#'], 'E': ['###', '#..', '##.', '#..', '###'], 'G': ['.##.', '#...', '#.##', '#..#', '.##.'],
        'H': ['#.#', '#.#', '###', '#.#', '#.#'], 'I': ['#', '#', '#', '#', '#'], 'K': ['#.#', '#.#', '##.', '#.#', '#.#'],
        'L': ['#.', '#.', '#.', '#.', '##'], 'M': ['#...#', '##.##', '#.#.#', '#...#', '#...#'], 'O': ['.#.', '#.#', '#.#', '#.#', '.#.'],
        'P': ['##.', '#.#', '##.', '#..', '#..'], 'R': ['##.', '#.#', '##.', '#.#', '#.#'], 'S': ['.##', '#..', '.#.', '..#', '##.'],
        'V': ['#.#', '#.#', '#.#', '#.#', '.#.'], 'Y': ['#.#', '#.#', '.#.', '.#.', '.#.'],
        '(': ['.#', '#.', '#.', '#.', '.#'], ')': ['#.', '.#', '.#', '.#', '#.'], '/': ['..#', '..#', '.#.', '#..', '#..']},
    6: {'A': ['.#.', '#.#', '#.#', '###', '#.#', '#.#'], 'E': ['###', '#..', '##.', '#..', '#..', '###'], 'F': ['###', '#..', '##.', '#..', '#..', '#..'],
        'I': ['#', '#', '#', '#', '#', '#'], 'N': ['#..#', '##.#', '##.#', '#.##', '#.##', '#..#'], 'R': ['##.', '#.#', '#.#', '##.', '#.#', '#.#'],
        'S': ['.##', '#..', '.#.', '..#', '..#', '##.'], 'T': ['###', '.#.', '.#.', '.#.', '.#.', '.#.']},
}


def pixel_run(text, height, center, gap=1, space=2):
    return {'text': text, 'height': height, 'center': list(center), 'gap': gap, 'space': space}


FULL = [0, 0, 320, 160]
PLANS = [
    # --- catalog-129: school gate plate (P_01_01) ---
    {'id': 'bs-p-01-01-gate-plate', 'file': 'P_01_01', 'catalog_item': 'catalog-129', 'sign': 'school gate plate, two lines',
     'rect': [76, 68, 160, 82], 'ink_share': 0.1,
     'erase': {'polarity': 'dark', 'threshold': 7, 'margin': 2, 'polygon': [(118, 72), (231, 93), (217, 143), (86, 122)]},
     'runs': gate_runs()},
    # --- catalog-131: roster close-up (owner chose the rotated layout, 2026-11-01) ---
    {'id': 'bs-p-01-03-roster-a-rotated', 'file': 'P_01_03', 'catalog_item': 'catalog-131', 'sign': 'brush roster, names run down their columns',
     'rect': FULL, 'erase': SKY_ERASE, 'ink': ROSTER_INK['P_01_03'], 'runs': rotated_runs(ROSTER_COLUMNS)},
    # P_01_03B: the game cross-fades P_01_03 to P_01_03B and the name keeps its place, so it repeats the P_01_03 line
    {'id': 'bs-p-01-03b-name-a-rotated', 'file': 'P_01_03B', 'catalog_item': 'catalog-131', 'sign': 'Fujisaki Shiori alone, name runs down (same line as the roster)',
     'rect': FULL, 'erase': {'region': FULL, 'below': 48, 'margin': 2}, 'ink': ROSTER_INK['P_01_03B'], 'runs': rotated_runs(ROSTER_COLUMNS, only='FUJISAKI')},
    # --- catalog-143/139/145/144/149: small signs, texture kept (replace T-39400 flat-face crops) ---
    {'id': 'bs-d-15-03-monument-v2', 'file': 'D_15_03', 'catalog_item': 'catalog-143', 'supersedes': 'bs-d-15-03-monument',
     'sign': 'stone monument plaque, three lines', 'rect': [229, 127, 55, 33],
     'erase': {'polarity': 'light', 'threshold': 14, 'margin': 1, 'also': [[229, 130, 6, 10]], 'smooth': [[229, 130, 6, 10]]},
     'runs': [{'text': 'CENTRAL', 'box': [236, 130, 42, 6], 'cap': 6, 'aspect': 1.0, 'stroke': 0.95, 'gap': 2.4},
              {'text': 'CITY ART', 'box': [236, 141, 42, 6], 'cap': 6, 'aspect': 1.0, 'stroke': 0.95, 'gap': 2.4},
              {'text': 'MUSEUM', 'box': [236, 152, 42, 6], 'cap': 6, 'aspect': 1.0, 'stroke': 0.95, 'gap': 2.4}]},
    # white plate x 211-245, y 57-69 (13 rows): two 5 px lines with a 1 px margin above, between and below
    {'id': 'bs-d-03-08-march-placard-v2', 'file': 'D_03_08', 'catalog_item': 'catalog-139', 'supersedes': 'bs-d-03-08-march-placard',
     'sign': 'march placard, two lines', 'rect': [211, 57, 35, 13], 'ink': [1, 1, 1],
     'erase': {**PLATE_ERASE, 'plane': {'fit': [211, 57, 35, 13], 'init': 'light'}},
     'runs': [pixel_run('KIRAMEKI', 5, (228, 60)), pixel_run('HIGH', 5, (228, 66))]},
    {'id': 'bs-d-24-03-ticket-plaque-v2', 'file': 'D_24_03', 'catalog_item': 'catalog-145', 'supersedes': 'bs-d-24-03-ticket-plaque',
     'sign': 'ticket booth plaque', 'rect': [235, 62, 20, 11],
     'erase': {'polarity': 'dark', 'threshold': 8, 'margin': 1, 'plane': {'fit': [235, 62, 20, 11], 'init': 'light'}},
     'runs': [{'text': 'TICKET', 'box': [236, 62, 18, 10], 'cap': 6, 'aspect': 9, 'stroke': 0.8, 'gap': 2.2}]},
    # white band x 136-179, y 40-46 above MAIKO: 5 px pixel letters (a 2 px L and a 2 px word space keep 3 px clear at each
    # end) in the strongest pink of the Japanese line. The Japanese is so
    # faint that a relative rule misses its long-vowel bar, so every pixel of the band below a channel sum of 84 is erased.
    {'id': 'bs-d-19-02-maiko-board-v2', 'file': 'D_19_02', 'catalog_item': 'catalog-144', 'supersedes': 'bs-d-19-02-maiko-board',
     'sign': 'rink dasher board, small shop line', 'rect': [136, 41, 44, 5], 'ink': [18, 12, 12],
     'erase': {'below': 84, 'margin': 1, 'plane': {'fit': [136, 40, 44, 7]}},
     'runs': [pixel_run('LOVELY SHOP', 5, (157.5, 43), space=2)]},
    # white plate x 108-143, y 42-51: 6 px pixel letters, black like the kanji, 2 px margins
    {'id': 'bs-i-17-01-hanging-sign-v2', 'file': 'I_17_01', 'catalog_item': 'catalog-149', 'supersedes': 'bs-i-17-01-hanging-sign',
     'sign': 'library aisle hanging sign', 'rect': [109, 43, 34, 8], 'ink': [2, 2, 2],
     'erase': {**PLATE_ERASE, 'plane': {'fit': [109, 43, 34, 8], 'init': 'light'}},
     'runs': [pixel_run('FINE ARTS', 6, (125.5, 46.5), space=3)]},
    # --- catalog-147: final exams chalkboard, chalk-sized lettering and the date column ---
    # The column x 305-311 held 月 / 日 / ︵ ︶ (month, day, weekday in vertical brackets). English: a blank date "( ) / ( )"
    # top to bottom in chalk (2 px inside each pair, so that a pair does not close into a 0); the brackets are erased too.
    # Only board pixels feed the fill (not the chalk ledge at y >= 40).
    {'id': 'bs-c-01-28-final-exams-v2', 'file': 'C_01_28', 'catalog_item': 'catalog-147', 'supersedes': 'bs-c-01-28-final-exams',
     'sign': 'blackboard: final exams heading and a blank month / day', 'rect': [268, 0, 45, 40], 'ink': [19, 20, 20],
     'erase': {'region': [268, 0, 45, 40], 'above': 27, 'margin': 1, 'fill_from': [268, 0, 45, 40]},
     'runs': [{'text': 'FINAL', 'box': [0, 0, 0, 0], 'length': 22, 'center': [283, 10], 'angle': 0, 'cap': 6, 'aspect': 1.0, 'stroke': 0.9, 'gap': 2.6},
              {'text': 'EXAMS', 'box': [0, 0, 0, 0], 'length': 28, 'center': [283, 19], 'angle': 0, 'cap': 6, 'aspect': 1.0, 'stroke': 0.9, 'gap': 2.6},
              {**pixel_run('( )', 5, (308, 8), space=2), 'opacity': 0.85},
              {**pixel_run('/', 5, (308.5, 20)), 'opacity': 0.85},
              {**pixel_run('( )', 5, (308, 32), space=2), 'opacity': 0.85}]},
]


# --- drawing --------------------------------------------------------------------
def text_lines(run):
    """(picture-space stroke lines, facts) of one English line; `run` has box, text, cap and optionally
    aspect, gap, angle (text rotation, 0 = left to right, 90 = down the picture, tops facing right), slant (degrees),
    origin ([x, y] of the text start; default: the box centre line for angle 0 and the box top-right for 90)."""
    spec = {**signs.DEFAULTS, **run}
    bx, by, bw, bh = spec['box']
    angle = spec.get('angle', spec.get('rotation', 0))
    length = spec.get('length', bh if angle == 90 else bw)
    if spec.get('kern'):
        local, facts = kerned_strokes(spec['text'], length, spec['cap'], spec['aspect'], spec['gap'])
    else:
        local = font.strokes_for(spec['text'], length, spec['cap'], spec['aspect'], spec['gap'])
        facts = font.text_extent(spec['text'], length, spec['cap'], spec['aspect'], spec['gap'])
    if 'origin' in spec:
        origin = tuple(spec['origin'])
    elif angle == 0:
        origin = (bx, by + (bh - spec['cap']) / 2.0)
    elif angle == 90:
        origin = (bx + (bw + spec['cap']) / 2.0, by)
    elif 'center' in spec:
        origin = (0.0, 0.0)
    else:
        raise ValueError('a text at angle %s needs an origin or a center' % angle)
    if 'center' in spec:
        # centre of the block (middle of the text length, middle of the cap height), slant included
        a = math.radians(angle)
        t = math.tan(math.radians(spec.get('slant', 0.0)))
        half = length / 2.0 - spec['cap'] / 2.0 * t
        origin = (spec['center'][0] - half * math.cos(a) + spec['cap'] / 2.0 * math.sin(a),
                  spec['center'][1] - half * math.sin(a) - spec['cap'] / 2.0 * math.cos(a))
    lines = brush.place(local, origin, angle, spec.get('slant', 0.0))
    return lines, {'text': spec['text'], 'box': spec['box'], 'cap_px': spec['cap'], 'angle': angle, 'slant_deg': spec.get('slant', 0.0),
                   'origin_px': [round(origin[0], 2), round(origin[1], 2)], 'stroke_px': spec['stroke'], 'letter_gap_units': spec['gap'], **facts}


def pixel_rows(text, height, gap=1, space=2):
    """Boolean bitmap (height, width) of text in the PIXEL_FONT face of that height."""
    glyphs = PIXEL_FONT[height]
    columns = []
    for i, ch in enumerate(text):
        if ch == ' ':
            columns.extend([[False] * height] * space)
            continue
        if ch not in glyphs:
            raise ValueError('no %d px pixel glyph for %r' % (height, ch))
        rows = glyphs[ch]
        columns.extend([[row[c] == '#' for row in rows] for c in range(len(rows[0]))])
        if i + 1 < len(text) and text[i + 1] != ' ':
            columns.extend([[False] * height] * gap)
    return np.array(columns, bool).T if columns else np.zeros((height, 0), bool)


def pixel_coverage(shape, run, offset):
    """Coverage of a pixel-font run: 1 on the glyph pixels. run: text, height, origin = [x, y] of the top-left of the
    first glyph or center = [x, y] of the bitmap (rounded to whole pixels), optional gap and space (px)."""
    bitmap = pixel_rows(run['text'], run['height'], run.get('gap', 1), run.get('space', 2))
    h, w = bitmap.shape
    if 'center' in run:
        x0, y0 = int(math.floor(run['center'][0] - w / 2.0 + 0.5)), int(math.floor(run['center'][1] - h / 2.0 + 0.5))
    else:
        x0, y0 = run['origin']
    cover = np.zeros(shape)
    for yy, xx in zip(*np.nonzero(bitmap)):
        py, px = y0 + yy - offset[1], x0 + xx - offset[0]
        if not (0 <= py < shape[0] and 0 <= px < shape[1]):
            raise ValueError('pixel text %r leaves its rectangle' % run['text'])
        cover[py, px] = 1.0
    return cover, {'text': run['text'], 'face': 'port-pixel-%d' % run['height'], 'bbox_px': [x0, y0, w, h]}


def run_coverage(shape, run, offset):
    if run.get('height'):
        return pixel_coverage(shape, run, offset)
    lines, facts = text_lines(run)
    angle = run.get('angle', run.get('rotation', 0))
    if run.get('brush'):
        cover = brush.brush_coverage(shape, lines, run['stroke'], offset, angle=angle, **run['brush'])
    else:
        cover = font.render_coverage(shape, lines, run['stroke'], offset)
    return cover, facts


def draw_plan(source, plan):
    """(English rgb5 picture, record). Changes only plan['rect'], and inside it only erased and covered pixels."""
    x, y, w, h = plan['rect']
    height, width = source.shape[:2]
    erase = plan.get('erase')
    mask = np.zeros((height, width), bool)
    if erase:
        region = erase.get('region', plan['rect'])
        if 'above' in erase:
            # absolute rule: channel sum above `above` (chalk or paint on a dark background), grown by the margin
            rx, ry, rw, rh = region
            box = np.zeros((height, width), bool)
            box[ry:ry + rh, rx:rx + rw] = True
            mask = brush.grow((source.sum(-1) > erase['above']) & box, erase.get('margin', 1)) & box
        elif 'below' in erase:
            # absolute rule: channel sum below `below` (ink on a pale background), grown by the margin
            rx, ry, rw, rh = region
            box = np.zeros((height, width), bool)
            box[ry:ry + rh, rx:rx + rw] = True
            mask = brush.grow((source.sum(-1) < erase['below']) & box, erase.get('margin', 1)) & box
        else:
            mask = brush.stroke_mask(source, region, erase.get('polarity', 'any'), erase.get('threshold', 8), erase.get('margin', 1),
                                     erase.get('sigma', 4.0))
        if erase.get('polygon'):
            poly = Image.new('L', (width, height), 0)
            ImageDraw.Draw(poly).polygon([tuple(p) for p in erase['polygon']], fill=255)
            mask &= np.asarray(poly) > 0
        for rx, ry, rw, rh in erase.get('keep', []):
            mask[ry:ry + rh, rx:rx + rw] = False
        for rx, ry, rw, rh in erase.get('also', []):
            mask[ry:ry + rh, rx:rx + rw] = True
        box = np.zeros_like(mask)
        box[y:y + h, x:x + w] = True
        mask &= box
    base = source.astype(np.float64)
    if mask.any() and erase.get('plane'):
        # a face that the lettering covers almost entirely: erased pixels take the plane fitted to the clean face pixels
        spec = erase['plane']
        face, _ = signs.fit_plane(source, plan['rect'], spec.get('fit'), tolerance=spec.get('tolerance', 2.0), init=spec.get('init', 'median'))
        inside = mask[y:y + h, x:x + w]
        block = base[y:y + h, x:x + w]
        block[inside] = face[inside]
    elif mask.any():
        # only the surface the lettering is on feeds the fill: the erase polygon and / or the fill_from rectangle
        exclude = None
        if erase.get('polygon'):
            exclude = ~(np.asarray(poly) > 0)
        if erase.get('fill_from'):
            fx, fy, fw, fh = erase['fill_from']
            outside = np.ones((height, width), bool)
            outside[fy:fy + fh, fx:fx + fw] = False
            exclude = outside if exclude is None else exclude | outside
        base = brush.inpaint(source, mask, erase.get('fill_sigma', 3.0), erase.get('grain', 1.0), exclude)
        if erase.get('smooth'):
            # these rectangles take the smooth part only (no donor grain)
            flat = brush.known_background(source, mask if exclude is None else mask | exclude, erase.get('fill_sigma', 3.0))
            for rx, ry, rw, rh in erase['smooth']:
                inside = mask[ry:ry + rh, rx:rx + rw]
                base[ry:ry + rh, rx:rx + rw][inside] = flat[ry:ry + rh, rx:rx + rw][inside]
    coverage = np.zeros((h, w))
    placed = []
    for run in plan['runs']:
        cover, facts = run_coverage((h, w), run, (x, y))
        coverage = np.maximum(coverage, cover * run.get('opacity', 1.0))
        placed.append(facts)
    ink = None
    ink_count = 0
    if plan['runs']:
        if isinstance(plan.get('ink'), (list, tuple)):
            ink = np.array(plan['ink'], float)
        else:
            reference = base[y:y + h, x:x + w]
            ink, ink_count = signs.sample_ink(source, plan['rect'], reference, plan.get('ink_share', 0.06))
    block = base[y:y + h, x:x + w]
    if ink is not None:
        tint = np.zeros_like(block) + ink
        texture = plan.get('texture', 0.0)
        if texture:
            tint = tint + texture * (source[y:y + h, x:x + w] - block) * (~mask[y:y + h, x:x + w])[..., None]
        block = block * (1 - coverage[..., None]) + tint * coverage[..., None]
    english = source.copy()
    changed = mask[y:y + h, x:x + w] | (coverage > 0.004)
    new = np.clip(np.rint(block), 0, 31).astype(int)
    region = english[y:y + h, x:x + w]
    region[changed] = new[changed]
    record = {'erased_pixels': int(mask.sum()), 'ink_rgb555': None if ink is None else [round(float(v), 2) for v in ink],
              'ink_pixels_used': ink_count, 'lines': placed, 'erase': erase and {k: v for k, v in erase.items() if k in ('polarity', 'threshold', 'margin')}}
    return english, record


# Replaced drafts: {old id: checkpoint folder that holds it}.
SUPERSEDED_IN = {'bs-d-15-03-monument': 'T39400-bs-signs-v1', 'bs-d-03-08-march-placard': 'T39400-bs-signs-v1',
                 'bs-d-24-03-ticket-plaque': 'T39400-bs-signs-v1', 'bs-d-19-02-maiko-board': 'T39400-bs-signs-v1',
                 'bs-i-17-01-hanging-sign': 'T39400-bs-signs-v1', 'bs-c-01-28-final-exams': 'T39004-bs-v1'}
CHECKPOINT_DIR = 'T39410-stills-fixes-v1'


# --- build ----------------------------------------------------------------------
STILL_SIZES = {'P_01_01': (320, 160), 'P_01_03': (320, 160), 'P_01_03B': (320, 160), 'C_01_28': (320, 88), 'P_01_02': (640, 160)}
STILL_ANCHORS = {**signs.LAYOUT_ANCHORS, 'P_01_01': 0x494B}
# (file, line, text on that line) of the code that reads and decodes each still of this module.
STILL_CALLERS = {
    'P_01_01': {'read': ('src/ovl/name_entry/prologue.c', 224, 'dec_bg_cd_read(0x494B, 1)'), 'decode': ('src/ovl/name_entry/prologue.c', 233, 'dec_bg_decode_to_vram(1)')},
    'P_01_03': {'read': ('src/ovl/name_entry/prologue.c', 413, 'dec_bg_cd_read(0x495D, 1)'), 'decode': ('src/ovl/name_entry/prologue.c', 425, 'dec_bg_decode_to_vram(1)')},
    'P_01_03B': {'read': ('src/ovl/name_entry/prologue.c', 463, 'dec_bg_cd_read(0x4966, 1)'), 'decode': ('src/ovl/name_entry/prologue.c', 472, 'dec_bg_decode_to_vram(1)')},
    'C_01_28': {'read': ('src/main/school_day.c', 126, 'dec_bg_cd_read(D_800B5950[hud_season_index()], 0)'), 'decode': ('src/main/school_day.c', 178, 'dec_bg_decode_to_vram(0)')},
}
CHALK_EVIDENCE = ['src/main/background_images.c:dec_bg_decode_to_vram(0) calls bg_still_decode_to_vram(0x2C0, 0xA0, 0x140, 0x58) or (0x140, 0x1A0, 0x140, 0x58): width 0x140 = 320, height 0x58 = 88',
                  'src/main/background_images.c:dec_bg_cd_read(lba, 0) reads the still with cd_read_sectors_nowait(0xA, 0x80162000, lba): a ten-sector read',
                  'src/main/hud_panels.c:hud_season_index maps the month to the four seasonal files C_01_27..C_01_30 through D_800B5950 (tools/assets/bs_cohort_profile.py pins C_01_28 at LBA 0x3F0D)']
CHALK_CHECKS = [('src/main/background_images.c', 'bg_still_decode_to_vram(0x2C0, 0xA0, 0x140, 0x58)'),
                ('src/main/background_images.c', 'bg_still_decode_to_vram(0x140, 0x1A0, 0x140, 0x58)')]


def check_callers(root=ROOT):
    problems = signs.check_callers(root)
    for pair in STILL_CALLERS.values():
        for path, line, text in pair.values():
            lines = (Path(root) / path).read_text().splitlines()
            if line > len(lines) or text not in lines[line - 1]:
                problems.append('%s:%d does not contain %s' % (path, line, text))
    for path, text in CHALK_CHECKS + signs.SHARED_CHECKS:
        if text not in (Path(root) / path).read_text():
            problems.append('%s lacks %s' % (path, text))
    return problems


def trailer_facts(data):
    """What follows the end marker: its length, the count of non-zero bytes and the last four bytes (a 4-byte file trailer
    after zero padding in P_01_01 and P_01_03B; the macroblock count and end marker already prove the picture is complete)."""
    trailing = mdec_splice.parse(data)['trailing']
    return {'trailing_bytes': len(trailing), 'trailing_nonzero_bytes': sum(1 for b in trailing if b), 'trailing_last4_hex': bytes(trailing[-4:]).hex()}


def geometry_proof(name, data, lba, sectors):
    proof = base_geometry_proof(name, data, lba, sectors)
    proof['stream'].update(trailer_facts(data))
    return proof


def base_geometry_proof(name, data, lba, sectors):
    if name == 'P_01_02' or name in signs.CALLERS:
        return signs.geometry_proof(name, data, lba, sectors)
    width, height = STILL_SIZES[name]
    calls = list(STILL_CALLERS[name].values())
    if name == 'C_01_28':
        evidence, read_sectors = list(CHALK_EVIDENCE), 10
    else:
        evidence, read_sectors = list(signs.DECODE_EVIDENCE), 10
    stream = signs.stream_facts(data, width, height)
    return {'width': width, 'height': height, 'lba': lba, 'lba_hex': '0x%X' % lba, 'file_sectors': sectors, 'read_sectors': read_sectors,
            'lba_basis': 'back-to-back layout from 0x3F03, pinned by %d game-code LBAs plus P_01_01 (prologue.c:224)' % len(signs.LAYOUT_ANCHORS),
            'callers': [{'path': p, 'line': n, 'text': t} for p, n, t in calls], 'code_evidence': evidence, 'stream': stream}


def png_rgba(source, english, rect):
    """RGBA of the plan rectangle: alpha 255 where the English pixel differs from the source, 0 elsewhere (no source pixel kept)."""
    x, y, w, h = rect
    changed = (english[y:y + h, x:x + w] != source[y:y + h, x:x + w]).any(-1)
    rgb = signs.expand(english[y:y + h, x:x + w])
    out = np.zeros((h, w, 4), np.uint8)
    out[..., :3] = np.where(changed[..., None], rgb, 0)
    out[..., 3] = np.where(changed, 255, 0)
    return Image.fromarray(out, 'RGBA'), int(changed.sum())


def zoom_pair(source, english, rect, margin=8, limit=900):
    x, y, w, h = rect
    x0, y0 = max(0, x - margin), max(0, y - margin)
    x1, y1 = min(source.shape[1], x + w + margin), min(source.shape[0], y + h + margin)
    scale = max(2, min(12, limit // (x1 - x0)))
    size = ((x1 - x0) * scale, (y1 - y0) * scale)
    top = Image.fromarray(signs.expand(source[y0:y1, x0:x1]), 'RGB').resize(size, Image.NEAREST)
    bottom = Image.fromarray(signs.expand(english[y0:y1, x0:x1]), 'RGB').resize(size, Image.NEAREST)
    sheet = Image.new('RGB', (size[0], size[1] * 2 + 6), (255, 0, 255))
    sheet.paste(top, (0, 0))
    sheet.paste(bottom, (0, size[1] + 6))
    return sheet


def build(output, disc=None, plans=None):
    disc = Path(disc or ROOT / BS_DIR)
    output = new_output(output)
    plans = PLANS if plans is None else plans
    problems = check_callers()
    if problems:
        raise ValueError('cited source lines changed: ' + '; '.join(problems))
    lbas = signs.contiguous_lbas(disc)
    for name, lba in STILL_ANCHORS.items():
        if lbas[name][0] != lba:
            raise ValueError('layout model disagrees with game-code LBA for ' + name)
    files = sorted({plan['file'] for plan in plans})
    proofs, decoded, hashes = {}, {}, {}
    for name in files:
        data = (disc / (name + '.BS')).read_bytes()
        proofs[name] = geometry_proof(name, data, *lbas[name])
        width, height = proofs[name]['width'], proofs[name]['height']
        decoded[name] = signs.decode_still(data, width, height)
        hashes[name] = {'container': digest(data), 'words': digest(mdec_model.decode_padded_words(data, width, height)),
                        'rgb8': digest(signs.expand(decoded[name]).tobytes())}
    output.mkdir(parents=True)
    for sub in ('previews', 'crops'):
        (output / sub).mkdir()
    assets = []
    for plan in plans:
        name = plan['file']
        source = decoded[name]
        english, record = draw_plan(source, plan)
        x, y, w, h = plan['rect']
        outside = np.ones(source.shape[:2], bool)
        outside[y:y + h, x:x + w] = False
        if (english[outside] != source[outside]).any():
            raise ValueError('draft changes pixels outside its rectangle: ' + plan['id'])
        crop, changed = png_rgba(source, english, plan['rect'])
        png = plan['id'] + '.en.png'
        crop.save(output / 'crops' / png, optimize=False)
        zoom_pair(source, english, plan['rect']).save(output / 'previews' / (plan['id'] + '.zoom.png'))
        Image.fromarray(signs.expand(english), 'RGB').save(output / 'previews' / (plan['id'] + '.still.png'))
        signs.side_by_side(source, english, None).save(output / 'previews' / (plan['id'] + '.compare-2x.png'))
        padded = proofs[name]['height'] + (-proofs[name]['height'] % 16)
        indices, mb_rect = signs.macroblocks(plan['rect'], padded)
        entry = {
            'id': plan['id'], 'catalog_item': plan['catalog_item'], 'sign': plan['sign'], 'group': plan.get('group'),
            'supersedes': plan.get('supersedes'),
            'source_container': 'CDROM/BS/%s.BS' % name, 'source_container_sha256': hashes[name]['container'],
            'frame': 0, 'palette_id': 'mdec:' + MDEC_MODEL_ID, 'decoder_model': MDEC_MODEL_ID,
            'source_words_sha256': hashes[name]['words'], 'source_png_sha256': hashes[name]['rgb8'],
            'source_png_sha256_basis': 'raw 8-bit RGB pixels of the decoded still (replicate expansion), not PNG file bytes',
            'geometry': {'width': proofs[name]['width'], 'height': proofs[name]['height'], 'proof': 'geometry_proofs.' + name},
            'edit_regions': [plan['rect']], 'edit_macroblocks': indices, 'edit_macroblock_bounds': mb_rect,
            'edit_region_policy': 'only the pixels listed by the crop alpha channel change (Japanese lettering erased, English drawn); every other pixel, including those inside the rectangle, is bit-identical to the source decode',
            'crop': {'png': png, 'rect': plan['rect'], 'format': 'rgba; alpha 255 = port-made pixel to write, alpha 0 = keep the source pixel',
                     'sha256': digest((output / 'crops' / png).read_bytes()), 'original_pixels': 0, 'port_made_pixels': changed},
            'changed_pixels': changed, 'lettering': record, 'port_made': True,
            'port_made_elements': [{'kind': 'lettering', 'face': 'port-skeleton-brush' if any(r.get('brush') for r in plan['runs']) else 'port-skeleton-gothic',
                                    'origin': 'port-made',
                                    'description': ('brush-stroked' if any(r.get('brush') for r in plan['runs']) else 'constant-stroke')
                                    + ' skeleton letters; lines: ' + ', '.join(r['text'].strip() for r in plan['runs'])
                                    + '; Japanese lettering erased and the picture under it rebuilt from the clean pixels around it, with their grain'}],
            'style': 'unreviewed', 'human_approved': False, 'runtime_integrated': False, 'in_game_verified': False}
        assets.append(entry)
    manifest = {'schema': SCHEMA, 'ticket': TICKET, 'human_approved': False, 'assets': assets, 'geometry_proofs': proofs,
                'layout_anchors': {k: '0x%X' % v for k, v in STILL_ANCHORS.items()}, 'decoder_model': MDEC_MODEL_ID,
                'committed_files': 'RGBA crop PNGs (alpha 255 only on port-made pixels) and this manifest; whole stills and comparisons stay local'}
    (output / 'manifest.json').write_bytes(encoded(manifest) + b'\n')
    return {'output': str(output), 'assets': len(assets), 'files': files}


def publish(built, destination):
    """Copy crops, manifest and ledger into a new checkpoint folder (RGBA crops only: no full images, no BS bytes)."""
    manifest_bytes = (built / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    destination = destination.absolute()
    if destination.exists():
        raise ValueError('destination must be new')
    staging = destination.with_name(destination.name + '.partial')
    if staging.exists():
        raise ValueError('stale staging directory: ' + str(staging))
    (staging / CHECKPOINT).mkdir(parents=True)
    ledger = []
    for asset in manifest['assets']:
        crop = built / 'crops' / asset['crop']['png']
        if digest(crop.read_bytes()) != asset['crop']['sha256']:
            raise ValueError('built crop changed: ' + asset['id'])
        (staging / CHECKPOINT / asset['crop']['png']).write_bytes(crop.read_bytes())
        row = {'id': asset['id'], 'catalog_item': asset['catalog_item'], 'path': '%s/%s' % (CHECKPOINT, asset['crop']['png']),
               'crop_sha256': asset['crop']['sha256'], 'dimensions': asset['crop']['rect'][2:], 'crop_format': asset['crop']['format'],
               'source_container_sha256': asset['source_container_sha256'], 'source_png_sha256': asset['source_png_sha256'],
               'decode_identity': {'decoder_model': asset['decoder_model'], 'source_words_sha256': asset['source_words_sha256'],
                                   'width': asset['geometry']['width'], 'height': asset['geometry']['height']},
               'edit_regions': asset['edit_regions'], 'port_made': True, 'port_made_elements': asset['port_made_elements'],
               'manifest_sha256': digest(manifest_bytes), 'status': 'draft', 'human_approved': False}
        if asset.get('group'):
            row['alternative_group'] = asset['group']
        if asset.get('supersedes'):
            row['supersedes'] = {'id': asset['supersedes'], 'checkpoint': SUPERSEDED_IN[asset['supersedes']]}
        ledger.append(row)
    (staging / CHECKPOINT / 'manifest.json').write_bytes(manifest_bytes)
    (staging / 'ledger.json').write_bytes(encoded({
        'schema': 'tokimemo.translation-checkpoint.v1', 'ticket': TICKET, 'assets': ledger,
        'candidate_assets': len(ledger), 'original_images_included': False, 'port_made_assets': len(ledger),
        'full_images_policy': 'only RGBA crops (alpha 255 on port-made pixels only) are committed; run build to rebuild whole stills into .asset-cache',
        'publication_authorization': 'Owner allowed new in-style lettering, 2026-11-01 (T-39400); fixes requested by the owner art review (T-39412).',
        'complete_translation_inventory': False, 'separate_reviewed_revisions': 0}) + b'\n')
    staging.rename(destination)
    return {'destination': str(destination), 'assets': len(ledger)}


def geometry(disc=None):
    disc = Path(disc or ROOT / BS_DIR)
    lbas = signs.contiguous_lbas(disc)
    return {name: geometry_proof(name, (disc / (name + '.BS')).read_bytes(), *lbas[name]) for name in sorted({p['file'] for p in PLANS})}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=('build', 'geometry'))
    parser.add_argument('--output', type=Path)
    parser.add_argument('--publish', type=Path, help='also copy crops, manifest and ledger into this new checkpoint folder')
    args = parser.parse_args()
    if args.command == 'geometry':
        print(json.dumps(geometry(), indent=1))
    else:
        if not args.output:
            parser.error('build needs --output')
        result = build(args.output)
        if args.publish:
            result['published'] = publish(args.output, args.publish)
        print(json.dumps(result))
