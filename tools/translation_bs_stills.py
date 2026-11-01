#!/usr/bin/env python3
"""T-39412: texture-keeping English drafts for BS background stills (signs, gate plate, roster boards, chalkboard).

Follow-up to tools/translation_bs_signs.py (T-39400). That tool rebuilt each sign face from a fitted plane,
which flattened stone and gradient. Here every plan keeps the picture as it is and changes only

1. the pixels of the Japanese lettering (found as pixels of the lettering colour against the smooth local
   background and grown by a margin for the soft MDEC edge), rebuilt from the clean pixels around them with
   the grain of nearby clean pixels (tools/translation_bs_brush.py: `stroke_mask`, `inpaint`), and
2. the pixels the English strokes cover.

The stroke letters are the single-stroke skeletons of tools/translation_bs_font.py, drawn either with a
constant stroke (signs, chalk) or with the brush stroker of tools/translation_bs_brush.py (gate plate,
roster boards, calligraphy close-ups).

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


def natural_length(text, cap, aspect, gap):
    """Length in px of text at cap height with the given aspect and letter gap (no fitting)."""
    return font.text_units(text, gap) * cap / font.CAP * aspect


def brush_run(text, center, angle, cap, aspect, gap, weight, slant=0.0, **brush_options):
    """A brush-lettered line of natural length centred on `center` (angle: 0 text goes right, 90 down the picture)."""
    return {'text': text, 'box': [0, 0, 0, 0], 'length': round(natural_length(text, cap, aspect, gap), 3), 'center': list(center),
            'angle': round(angle, 2), 'slant': slant, 'cap': cap, 'aspect': aspect, 'stroke': weight, 'gap': gap,
            'brush': {**BRUSH, **brush_options}}


# P_01_03 / P_01_03B: calligraphy columns. Each column of kanji runs down and to the left; the axis is the line through
# `point` (the middle of the column, measured on the decode) with slope dx/dy. Names are family first (the script's
# spelling, no long-vowel marks). The right-edge column (x >= 303, cut off by the frame) stays as it is.
ROSTER_COLUMNS = [  # (family, given, axis point, slope dx/dy)
    ('MOGAMI', '', (18, 28), -0.30), ('MIZUKAWA', 'MIHO', (48, 58), -0.47), ('HONDA', 'MEGUMI', (92, 68), -0.50),
    ('FUJISAKI', 'SHIORI', (147, 78), -0.44), ('HASHIMOTO', 'YOSHIKO', (210, 83), -0.38), ('TANAKA', 'SAKI', (266, 92), -0.40)]
# stacked version: the columns alternate between an upper and a lower band so that neighbouring stacks never share a
# row; the centre of each line sits on the column axis at the band's height (family line, then given line below it).
STACK_BANDS = {'MOGAMI': 26, 'MIZUKAWA': 96, 'HONDA': 26, 'FUJISAKI': 96, 'HASHIMOTO': 26, 'TANAKA': 96}
STACK_LEADING = 22
STACK_MARGIN = 3


def stack_centres(columns=ROSTER_COLUMNS, bands=STACK_BANDS, leading=STACK_LEADING):
    """{family: (family line centre, given line centre or None)} on the slanted column axes."""
    out = {}
    for family, given, (px, py), slope in columns:
        y = bands[family]
        out[family] = ((round(px + slope * (y - py), 1), y), (round(px + slope * (y + leading - py), 1), y + leading) if given else None)
    return out


ROSTER_STACKS = stack_centres()
INK_BLACK = [1, 1, 2]
SKY_ERASE = {'region': [0, 0, 303, 160], 'below': 48, 'margin': 2}


def rotated_runs(columns, cap=13.5, aspect=0.75, gap=3.3, weight=2.8, only=None):
    runs = []
    for family, given, point, slope in columns:
        if only and family != only:
            continue
        text = family + ('  ' + given if given else '')
        runs.append(brush_run(text, point, 90 + math.degrees(math.atan(-slope)), cap, aspect, gap, weight))
    return runs


def stacked_runs(columns, stacks, cap=14, aspect=0.7, gap=2.8, weight=2.1, slant=8, only=None):
    runs = []
    for family, given, point, slope in columns:
        if only and family != only:
            continue
        for text, center in zip((family, given), stacks[family]):
            if text and center:
                run = brush_run(text, center, 0, cap, aspect, gap, weight, slant)
                half = run['length'] / 2.0 + STACK_MARGIN
                run['center'][0] = round(min(max(center[0], half), 320 - half), 1)
                runs.append(run)
    return runs


FULL = [0, 0, 320, 160]
PLANS = [
    # --- catalog-129: school gate plate (P_01_01) ---
    {'id': 'bs-p-01-01-gate-plate', 'file': 'P_01_01', 'catalog_item': 'catalog-129', 'sign': 'school gate plate, two lines',
     'rect': [76, 68, 160, 82], 'ink_share': 0.1,
     'erase': {'polarity': 'dark', 'threshold': 7, 'margin': 2, 'polygon': [(118, 72), (231, 93), (217, 143), (86, 122)]},
     'runs': [brush_run('KIRAMEKI  PRIVATE', (170, 92), 11, 11, 0.78, 3.0, 1.6, 6, taper=0.65),
              brush_run('HIGH  SCHOOL', (162, 125), 12, 11.5, 0.9, 3.4, 1.7, 6, taper=0.65)]},
    # --- catalog-131: roster close-ups, rotated (a) and stacked (b) ---
    {'id': 'bs-p-01-03-roster-a-rotated', 'file': 'P_01_03', 'catalog_item': 'catalog-131', 'group': 'p-01-03', 'sign': 'brush roster, names run down their columns',
     'rect': FULL, 'erase': SKY_ERASE, 'ink': INK_BLACK, 'runs': rotated_runs(ROSTER_COLUMNS)},
    {'id': 'bs-p-01-03-roster-b-stacked', 'file': 'P_01_03', 'catalog_item': 'catalog-131', 'group': 'p-01-03', 'sign': 'brush roster, family name over given name',
     'rect': FULL, 'erase': SKY_ERASE, 'ink': INK_BLACK, 'runs': stacked_runs(ROSTER_COLUMNS, ROSTER_STACKS)},
    # P_01_03B: the game cross-fades P_01_03 to P_01_03B and the name keeps its place, so a and b repeat the P_01_03 lines
    {'id': 'bs-p-01-03b-name-a-rotated', 'file': 'P_01_03B', 'catalog_item': 'catalog-131', 'group': 'p-01-03b', 'sign': 'Fujisaki Shiori alone, name runs down (same line as roster a)',
     'rect': FULL, 'erase': {'region': FULL, 'below': 48, 'margin': 2}, 'ink': INK_BLACK, 'runs': rotated_runs(ROSTER_COLUMNS, only='FUJISAKI')},
    {'id': 'bs-p-01-03b-name-b-stacked', 'file': 'P_01_03B', 'catalog_item': 'catalog-131', 'group': 'p-01-03b', 'sign': 'Fujisaki Shiori alone, family over given (same lines as roster b)',
     'rect': FULL, 'erase': {'region': FULL, 'below': 48, 'margin': 2}, 'ink': INK_BLACK, 'runs': stacked_runs(ROSTER_COLUMNS, ROSTER_STACKS, only='FUJISAKI')},
    {'id': 'bs-p-01-03b-name-a-large', 'file': 'P_01_03B', 'catalog_item': 'catalog-131', 'group': 'p-01-03b', 'sign': 'Fujisaki Shiori alone, large, runs down the middle',
     'rect': FULL, 'erase': {'region': FULL, 'below': 48, 'margin': 2}, 'ink': INK_BLACK,
     'runs': [brush_run('FUJISAKI  SHIORI', (148, 82), 90 + math.degrees(math.atan(0.40)), 19, 0.62, 3.0, 3.0, 0, taper=0.65)]},
    {'id': 'bs-p-01-03b-name-b-large', 'file': 'P_01_03B', 'catalog_item': 'catalog-131', 'group': 'p-01-03b', 'sign': 'Fujisaki Shiori alone, large, family over given, centred',
     'rect': FULL, 'erase': {'region': FULL, 'below': 48, 'margin': 2}, 'ink': INK_BLACK,
     'runs': [brush_run('FUJISAKI', (160, 56), 0, 26, 0.85, 2.8, 3.8, 8, taper=0.65), brush_run('SHIORI', (160, 108), 0, 26, 0.85, 2.8, 3.8, 8, taper=0.65)]},
    # --- catalog-143/139/145/144/149: small signs, texture kept (replace T-39400 flat-face crops) ---
    {'id': 'bs-d-15-03-monument-v2', 'file': 'D_15_03', 'catalog_item': 'catalog-143', 'supersedes': 'bs-d-15-03-monument',
     'sign': 'stone monument plaque, three lines', 'rect': [229, 127, 55, 33],
     'erase': {'polarity': 'light', 'threshold': 14, 'margin': 1},
     'runs': [{'text': 'CENTRAL', 'box': [236, 130, 42, 6], 'cap': 6, 'aspect': 1.0, 'stroke': 0.95, 'gap': 2.4},
              {'text': 'CITY ART', 'box': [236, 141, 42, 6], 'cap': 6, 'aspect': 1.0, 'stroke': 0.95, 'gap': 2.4},
              {'text': 'MUSEUM', 'box': [236, 152, 42, 6], 'cap': 6, 'aspect': 1.0, 'stroke': 0.95, 'gap': 2.4}]},
    {'id': 'bs-d-03-08-march-placard-v2', 'file': 'D_03_08', 'catalog_item': 'catalog-139', 'supersedes': 'bs-d-03-08-march-placard',
     'sign': 'march placard', 'rect': [213, 57, 31, 13],
     'erase': {**PLATE_ERASE, 'plane': {'fit': [213, 57, 31, 13], 'init': 'light'}},
     'runs': [{'text': 'KIRAMEKI', 'box': [215, 58, 27, 11], 'cap': 7, 'aspect': 9, 'stroke': 0.85, 'gap': 3.0}]},
    {'id': 'bs-d-24-03-ticket-plaque-v2', 'file': 'D_24_03', 'catalog_item': 'catalog-145', 'supersedes': 'bs-d-24-03-ticket-plaque',
     'sign': 'ticket booth plaque', 'rect': [235, 62, 20, 11],
     'erase': {'polarity': 'dark', 'threshold': 8, 'margin': 1, 'plane': {'fit': [235, 62, 20, 11], 'init': 'light'}},
     'runs': [{'text': 'TICKET', 'box': [236, 62, 18, 10], 'cap': 6, 'aspect': 9, 'stroke': 0.8, 'gap': 2.2}]},
    {'id': 'bs-d-19-02-maiko-board-v2', 'file': 'D_19_02', 'catalog_item': 'catalog-144', 'supersedes': 'bs-d-19-02-maiko-board',
     'sign': 'rink dasher board, small shop line', 'rect': [139, 41, 37, 5],
     'erase': {'polarity': 'dark', 'threshold': 8, 'margin': 1, 'plane': {'fit': [138, 40, 40, 7]}},
     'runs': [{'text': 'LOVELY SHOP', 'box': [140, 41, 35, 5], 'cap': 5, 'aspect': 9, 'stroke': 0.8, 'gap': 1.5}]},
    {'id': 'bs-i-17-01-hanging-sign-v2', 'file': 'I_17_01', 'catalog_item': 'catalog-149', 'supersedes': 'bs-i-17-01-hanging-sign',
     'sign': 'library aisle hanging sign', 'rect': [109, 43, 34, 8],
     'erase': {**PLATE_ERASE, 'plane': {'fit': [109, 43, 34, 8], 'init': 'light'}},
     'runs': [{'text': 'FINE ARTS', 'box': [110, 43, 32, 8], 'cap': 7, 'aspect': 9, 'stroke': 0.8, 'gap': 2.4}]},
    # --- catalog-147: final exams chalkboard, chalk-sized lettering and the date column ---
    {'id': 'bs-c-01-28-final-exams-v2', 'file': 'C_01_28', 'catalog_item': 'catalog-147', 'supersedes': 'bs-c-01-28-final-exams',
     'sign': 'blackboard: final exams heading and month / day column', 'rect': [268, 0, 45, 40], 'ink': [19, 20, 20],
     'erase': {'region': [268, 0, 45, 38], 'above': 27, 'margin': 1, 'keep': [[305, 26, 7, 13]]},
     'runs': [{'text': 'FINAL', 'box': [0, 0, 0, 0], 'length': 22, 'center': [283, 10], 'angle': 0, 'cap': 6, 'aspect': 1.0, 'stroke': 0.9, 'gap': 2.6},
              {'text': 'EXAMS', 'box': [0, 0, 0, 0], 'length': 28, 'center': [283, 19], 'angle': 0, 'cap': 6, 'aspect': 1.0, 'stroke': 0.9, 'gap': 2.6},
              {'text': 'M', 'box': [0, 0, 0, 0], 'length': 5, 'center': [308.5, 6], 'angle': 0, 'cap': 5, 'aspect': 1.0, 'stroke': 0.9, 'gap': 2.0},
              {'text': 'D', 'box': [0, 0, 0, 0], 'length': 5, 'center': [308.5, 20.5], 'angle': 0, 'cap': 5, 'aspect': 1.0, 'stroke': 0.9, 'gap': 2.0}]},
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


def run_coverage(shape, run, offset):
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
        base = brush.inpaint(source, mask, erase.get('fill_sigma', 3.0), erase.get('grain', 1.0))
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
