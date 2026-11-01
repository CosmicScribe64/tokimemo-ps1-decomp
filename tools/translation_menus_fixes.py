#!/usr/bin/env python3
"""T-39410: art fix pass for menu and screen items of the English translation.

One reproducible build for the nine items of the owner/reviewer fix pass:

1. title-0001..0004   bold outlined title-screen lettering at the Japanese height
2. catalog-127        ALL_C banner, stroke weight of the Japanese lettering
3. exam-0001          `points` where the score suffix 点 was removed
4. option-label-*     cast list with the approved spellings and full names
5. name-label-*       full selector labels on one widened revision
6. filesel-frame-20   given-name buttons in a narrower undistorted face
7. filesel-page-information-0   System tab left margin
8. filesel-frame-19   stat names (approved set)
9. kasetu-0           sports card title `Scavenger Race`

Run in the Pillow tooling image with the disc extracted and the original-image
exports restored under --sources (the same `original-*-v1` folders the other
translation tools read; they hold original game pixels and stay uncommitted).
The output folder must be new and under `.asset-cache`; `publish` copies the
English PNGs, the manifest, ledger.json and REGISTER.md into the checkpoint.

Lettering sources, all port-made and recorded as such:

* `bold`: the single-stroke skeleton capitals of translation_bs_font.py stroked
  at a weight chosen per item (title screen, ALL_C banner);
* `tight`: a 3-texel lowercase / 4-texel capital face authored below for the
  narrow button faces of FILESEL (undistorted; never a squashed wide face);
* `menu9`/`native`: the existing recovered menu face and the unscaled game font
  (option cast names, exam suffix, sports title).

Every output keeps the original palette: no colour outside the original image
is introduced, and pixels outside the declared edit regions are unchanged.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import shutil
import sys

import numpy as np
from PIL import Image

from translation_assets import ROOT, digest, encoded, new_output
import translation_bs_font as skeleton
from translation_native_font import NativeLatin

SCHEMA = 'tokimemo.menus-fixes-drafts.v1'
TICKET = 'T-39410'
REVISION = 'menus-fixes-v1'
CHECKPOINT = 'T39410-menus-fixes-v1'
EXE = ROOT / 'disc/files/SLPM_86.053'
TRANSPARENT = (0, 0, 0, 0)
ARTWORK = ROOT / 'translations/en/graphics/artwork'
OLD = ARTWORK / 'checkpoints/T39004-2026-10-29'

TITLE_SOURCE_SHA = '7116859fbb555b3e7b33dabbcd1bd2afc10862d4496e26156fb5f442f8b03206'
OPTION_SOURCE_SHA = '285aebdee243903fc1eba7b38ea9a6e432c6961d16021aa1a812c36194a420fe'
NAME_SOURCE_SHA = '8bfdd21c0fc2b63fb9b3041a21c7fdba82f3a071e6375193cdc4e679e165cb2f'
EXAM_SOURCE_SHA = 'f990c8a8eb5cc547204102d1e25fc2d5fda5fa18ed076bc543a77eb76e0dc37a'
FILESEL_SOURCE_SHA = '1a0e42ade6320df564cdb1017f73ba05083394c1974d3bc7a4af21618725e2a6'
ALLC_SOURCE_SHA = 'e5a8440f4babcd56bea233e95a610cd70f14ab5ec1c959df9eebf3f955c26b52'
SPORTS_SOURCE_SHA = '37b8e4313ad0b321c686a5f3475e79fa02364a8061f54248cf6e2724f9742a9a'


# --- shared pixel helpers ---------------------------------------------------------------

def load(path):
    return Image.open(path).convert('RGBA')


def colors_of(image):
    return set(image.getdata())


def dilate(mask, diagonal=True):
    """Grow a boolean mask by one texel (8-neighbourhood by default)."""
    grown = mask.copy()
    grown[1:] |= mask[:-1]; grown[:-1] |= mask[1:]
    grown[:, 1:] |= mask[:, :-1]; grown[:, :-1] |= mask[:, 1:]
    if diagonal:
        grown[1:, 1:] |= mask[:-1, :-1]; grown[:-1, :-1] |= mask[1:, 1:]
        grown[1:, :-1] |= mask[:-1, 1:]; grown[:-1, 1:] |= mask[1:, :-1]
    return grown


def bold_mask(shape, text, left, top, length, height, stroke, aspect=1.0, gap=skeleton.GAP):
    """Boolean mask of skeleton lettering.

    The ink box starts at (left, top), is `height` rows tall (centre-line cap
    height + stroke) and fits `length` columns; coverage >= 1/2 is ink.
    """
    cap = height - stroke
    lines = skeleton.strokes_for(text, length - stroke, cap, aspect, gap)
    lines = [[(left + stroke / 2.0 + u, top + stroke / 2.0 + v) for u, v in line] for line in lines]
    return skeleton.render_coverage(shape, lines, stroke) >= 0.5


def ink_box(mask):
    ys, xs = np.nonzero(mask)
    return int(xs.min()), int(ys.min()), int(xs.max()) + 1, int(ys.max()) + 1


def put(image, mask, colors):
    """Paint mask texels; `colors` is one RGBA or a callable (x, y) -> RGBA."""
    for y, x in zip(*np.nonzero(mask)):
        image.putpixel((int(x), int(y)), colors(int(x), int(y)) if callable(colors) else colors)


def record_id(prefix, frame):
    return '%s-%04d' % (prefix, frame)


def changed_box(original, result):
    a, b = np.asarray(original), np.asarray(result)
    diff = (a != b).any(-1)
    if not diff.any():
        return None
    x0, y0, x1, y1 = ink_box(diff)
    return [x0, y0, x1 - x0, y1 - y0]


# --- 1. title screen --------------------------------------------------------------------
# The Japanese lettering is outlined bevel lettering: a 1 texel dark outline around a fill
# that brightens and darkens down the rows. Frames 1 and 3 are the highlighted state: the
# same lettering in blue, with the orange slab drawn over the lower rows (the slab pixels
# are kept untouched). Frames 2 and 4 are the plain grey state.
TITLE_FRAMES = {1: ('GAME START', True), 2: ('GAME START', False), 3: ('OPTIONS', True), 4: ('OPTIONS', False)}
TITLE_STROKE = 3.0
TITLE_GAP = 3.6                              # letter spacing in skeleton units (the outlines must not merge)
TITLE_FILL_TOP, TITLE_FILL_HEIGHT = 2, 12   # fill rows 2..13, outline rows 1..14 (as the Japanese)
SLAB_VISIBLE_ROWS = 10                       # rows below this belong to the slab on highlighted frames


def title_slab(pixel):
    return pixel[3] != 0 and pixel[0] > pixel[2]


def title_lettering(original, selected):
    """Original lettering texels (everything but the slab) and its outline colour."""
    ink = np.zeros((original.height, original.width), bool)
    for y in range(original.height):
        for x in range(original.width):
            p = original.getpixel((x, y))
            ink[y, x] = p[3] != 0 and not (selected and title_slab(p))
    outline = (0, 0, 115, 255) if selected else (24, 24, 24, 255)
    return ink, outline


def title_row_colors(original, ink, outline):
    """Fill colour of each row: the commonest non-outline lettering colour in that row."""
    rows = {}
    for y in range(original.height):
        counts = Counter(original.getpixel((x, y)) for x in range(original.width) if ink[y, x] and original.getpixel((x, y)) != outline)
        if counts:
            rows[y] = counts.most_common(1)[0][0]
    return rows


def title_frame(original, text, selected, stroke=TITLE_STROKE):
    ink, outline = title_lettering(original, selected)
    x0, _, x1, _ = ink_box(ink)
    colors = title_row_colors(original, ink, outline)
    # Rows the slab hides on the highlighted frames reuse the last visible row's colour.
    last = max(colors)
    shape = (original.height, original.width)
    fill = bold_mask(shape, text.replace(' ', '  '), x0 + 1, TITLE_FILL_TOP, x1 - x0 - 2, TITLE_FILL_HEIGHT, stroke, aspect=2.5, gap=TITLE_GAP)
    edge = dilate(fill) & ~fill
    result = original.copy()
    for y in range(original.height):
        for x in range(original.width):
            if not (selected and title_slab(original.getpixel((x, y)))):
                result.putpixel((x, y), TRANSPARENT)
    kept = 0
    for y, x in zip(*np.nonzero(fill | edge)):
        x, y = int(x), int(y)
        slab = selected and (title_slab(original.getpixel((x, y))) or y >= SLAB_VISIBLE_ROWS + 1)
        if selected and y == SLAB_VISIBLE_ROWS and not edge[y, x]:
            slab = True   # the slab starts on this row: only the outline may peek out left of it
        if slab:
            kept += int(fill[y, x])
            continue
        result.putpixel((x, y), outline if edge[y, x] else colors[min(y, last)])
    return result, {'ink_box': [x0, x1], 'fill_pixels': int(fill.sum()), 'fill_pixels_under_slab': kept, 'stroke': stroke}


def build_title(sources):
    out = []
    for frame, (text, selected) in TITLE_FRAMES.items():
        path = sources / 'original-title-v1' / ('title.frame-%02d.png' % frame)
        original = load(path)
        result, facts = title_frame(original, text, selected)
        out.append((record_id('title', frame), result, original, path, {
            'text': text, 'frame': frame, 'highlighted': selected, 'source_container_sha256': TITLE_SOURCE_SHA, **facts}))
    return out


# --- 2. ALL_C banner (catalog-127) ------------------------------------------------------
# LASTPHOT/ALL_C.NBN: the banner strip is 320x38 texels across source pages 3 (x 0-127),
# 4 (x 128-255) and 5 (x 256-319) at page rows 74-111. The Japanese lettering is flat
# 3-4 texel strokes 18 rows tall (rows 3-20 of the strip) with a few anti-alias tints.
ALLC_PAGES = ((3, 0, 128), (4, 128, 128), (5, 256, 64))
ALLC_ROW0, ALLC_ROWS, ALLC_WIDTH = 74, 38, 320
ALLC_RUNS = (('ALL CLEAR', 'blue'), ('CONGRATULATIONS', 'red'))
# Ink ramps, white to solid, taken from the colours the original strip already holds.
ALLC_RAMPS = {
    'blue': [(255, 255, 255, 255), (198, 231, 255, 255), (132, 206, 255, 255), (82, 165, 255, 255), (16, 115, 247, 255)],
    'red': [(255, 255, 255, 255), (255, 214, 231, 255), (255, 165, 165, 255), (255, 107, 107, 255), (255, 41, 41, 255)],
    'purple': [(255, 255, 255, 255), (206, 206, 255, 255), (173, 132, 255, 255), (132, 41, 255, 255)],
}
ALLC_TOP, ALLC_HEIGHT, ALLC_STROKE = 3, 18, 3.0
ALLC_RUN_SPACE = 9      # texels between the two words and before the exclamation mark
ALLC_GAP = 2.8
ALLC_ASPECT = 0.9      # horizontal share of the natural letter width: leaves a 12 texel margin each side


def coverage_ramp(coverage, ramp):
    """Anti-aliased texels from a coverage plane, quantised to the original's own tints."""
    levels = np.rint(np.where(coverage < 0.2, 0.0, coverage) * (len(ramp) - 1)).astype(int)   # no faint spill beyond the cap rows
    return levels


def allc_strip(sources):
    strip = Image.new('RGBA', (ALLC_WIDTH, ALLC_ROWS))
    pages = {}
    for page, x, width in ALLC_PAGES:
        path = sources / 'allc' / ('all-c.source-page-%d.png' % page)
        image = load(path)
        pages[page] = (path, image)
        strip.paste(image.crop((0, ALLC_ROW0, width, ALLC_ROW0 + ALLC_ROWS)), (x, 0))
    return strip, pages


def allc_english(original):
    paper = (255, 255, 255, 255)
    cap = ALLC_HEIGHT - ALLC_STROKE
    sy = cap / skeleton.CAP
    widths = [skeleton.text_units(text, ALLC_GAP) * sy * ALLC_ASPECT for text, _ in ALLC_RUNS]
    mark = ALLC_STROKE
    total = sum(widths) + ALLC_RUN_SPACE * 2 + ALLC_STROKE + mark
    left = (ALLC_WIDTH - total) / 2.0
    result = Image.new('RGBA', original.size, paper)
    shape = (ALLC_ROWS, ALLC_WIDTH)
    cursor, placed = left, []
    for (text, family), width in zip(ALLC_RUNS, widths):
        lines = skeleton.strokes_for(text, width, cap, ALLC_ASPECT, ALLC_GAP)
        lines = [[(cursor + ALLC_STROKE / 2.0 + u, ALLC_TOP + ALLC_STROKE / 2.0 + v) for u, v in line] for line in lines]
        placed.append((family, skeleton.render_coverage(shape, lines, ALLC_STROKE), [round(cursor, 2), round(width + ALLC_STROKE, 2)]))
        cursor += width + ALLC_STROKE + ALLC_RUN_SPACE
    # exclamation mark: bar and dot
    x = round(cursor) + ALLC_STROKE / 2.0      # pixel aligned: a solid three-texel bar
    bar = [[(x, ALLC_TOP + ALLC_STROKE / 2.0), (x, ALLC_TOP + 11.5)], [(x, ALLC_TOP + ALLC_HEIGHT - ALLC_STROKE / 2.0 - 0.01), (x, ALLC_TOP + ALLC_HEIGHT - ALLC_STROKE / 2.0)]]
    placed.append(('purple', skeleton.render_coverage(shape, bar, ALLC_STROKE), [round(cursor, 2), ALLC_STROKE]))
    for family, coverage, _ in placed:
        ramp = ALLC_RAMPS[family]
        levels = coverage_ramp(coverage, ramp)
        for y, x in zip(*np.nonzero(levels)):
            current = result.getpixel((int(x), int(y)))
            new = ramp[levels[y, x]]
            if current == paper or sum(new[:3]) < sum(current[:3]):
                result.putpixel((int(x), int(y)), new)
    return result, [{'family': f, 'x': box[0], 'width': box[1]} for f, _, box in placed]


def build_allc(sources):
    strip, pages = allc_strip(sources)
    english, runs = allc_english(strip)
    if not colors_of(english) <= colors_of(strip):
        raise ValueError('ALL_C banner colour outside the original strip')
    crops = []
    for page, x, width in ALLC_PAGES:
        path, image = pages[page]
        crop = english.crop((x, 0, x + width, ALLC_ROWS))
        crops.append((page, crop, image.crop((0, ALLC_ROW0, width, ALLC_ROW0 + ALLC_ROWS)), path, [0, ALLC_ROW0, width, ALLC_ROWS]))
    return strip, english, crops, runs


# --- tight face ---------------------------------------------------------------------------
# Authored here for button faces that are too narrow for the menu face: 7-row capitals,
# 5-row x-height lowercase on a 3 texel grid (m is 5, i and l are 1), descenders to row 8.
# Each entry is (first row, rows); glyphs sit in a nine-row box (rows 0-8, baseline row 6).
TIGHT = {
    'A': (0, ['.##.', '#..#', '#..#', '####', '#..#', '#..#', '#..#']),
    'G': (0, ['.###', '#...', '#...', '#.##', '#..#', '#..#', '.###']),
    'H': (0, ['#..#', '#..#', '#..#', '####', '#..#', '#..#', '#..#']),
    'K': (0, ['#..#', '#.#.', '##..', '##..', '#.#.', '#..#', '#..#']),
    'L': (0, ['#..', '#..', '#..', '#..', '#..', '#..', '###']),
    'M': (0, ['#...#', '##.##', '#.#.#', '#.#.#', '#...#', '#...#', '#...#']),
    'N': (0, ['#..#', '##.#', '##.#', '#.##', '#.##', '#..#', '#..#']),
    'R': (0, ['###.', '#..#', '#..#', '###.', '#.#.', '#..#', '#..#']),
    'S': (0, ['.###', '#...', '#...', '.##.', '...#', '...#', '###.']),
    'T': (0, ['###', '.#.', '.#.', '.#.', '.#.', '.#.', '.#.']),
    'Y': (0, ['#.#', '#.#', '#.#', '.#.', '.#.', '.#.', '.#.']),
    'a': (2, ['###', '..#', '###', '#.#', '###']),
    'c': (2, ['###', '#..', '#..', '#..', '###']),
    'e': (2, ['###', '#.#', '###', '#..', '###']),
    'g': (2, ['###', '#.#', '#.#', '###', '..#', '..#', '##.']),
    'h': (0, ['#..', '#..', '###', '#.#', '#.#', '#.#', '#.#']),
    'i': (0, ['#', '.', '#', '#', '#', '#', '#']),
    'k': (0, ['#..', '#..', '#.#', '##.', '##.', '#.#', '#.#']),
    'l': (0, ['#', '#', '#', '#', '#', '#', '#']),
    'm': (2, ['#####', '#.#.#', '#.#.#', '#.#.#', '#.#.#']),
    'n': (2, ['###', '#.#', '#.#', '#.#', '#.#']),
    'o': (2, ['###', '#.#', '#.#', '#.#', '###']),
    'p': (2, ['###', '#.#', '#.#', '###', '#..', '#..', '#..']),
    'r': (2, ['.##', '#..', '#..', '#..', '#..']),
    's': (2, ['###', '#..', '###', '..#', '###']),
    't': (1, ['.#.', '###', '.#.', '.#.', '.#.', '.##']),
    'u': (2, ['#.#', '#.#', '#.#', '#.#', '###']),
    'v': (2, ['#.#', '#.#', '#.#', '#.#', '.#.']),
    'y': (2, ['#.#', '#.#', '#.#', '###', '..#', '..#', '##.']),
    'z': (2, ['###', '..#', '.#.', '#..', '###']),
    '.': (6, ['#']),
}
TIGHT_SPACE = 3


class Face:
    """Bitmap lettering: glyph(c) -> {'points', 'width'}; a letter advances width + gap, a space 3."""
    space = 3

    def glyph(self, character):
        raise NotImplementedError

    def advance(self, character, gap=1):
        return self.space if character == ' ' else self.glyph(character)['width'] + gap

    def width(self, text, gap=1):
        """Ink width: the advances without the gap after the last letter."""
        if not text:
            return 0
        return sum(self.advance(c, gap) for c in text) - (0 if text[-1] == ' ' else gap)

    def points(self, text, left, top, gap=1):
        points = []
        for character in text:
            if character != ' ':
                points.extend((left + x, top + y) for x, y in self.glyph(character)['points'])
            left += self.advance(character, gap)
        return points


class Tight(Face):
    """Narrow undistorted lettering on a nine-row box."""
    def glyph(self, character):
        if character == ' ':
            return {'points': [], 'width': self.space}
        top, rows = TIGHT[character]
        if len({len(row) for row in rows}) != 1 or set(''.join(rows)) - {'#', '.'}:
            raise ValueError('malformed tight glyph: ' + character)
        return {'points': [(x, top + y) for y, row in enumerate(rows) for x, cell in enumerate(row) if cell == '#'], 'width': len(rows[0])}

    def provenance(self):
        return {'face': 'tight', 'origin': 'port-made', 'glyphs': ''.join(sorted(TIGHT)), 'rescaled': False,
                'glyph_sha256': digest(encoded(TIGHT))}


class Menu9(Face):
    """The recovered 9-row menu face (see translation_option_drafts.recover_font)."""
    def __init__(self):
        from translation_option_drafts import recover_font
        self.glyphs, self.proof = recover_font(ARTWORK)

    def glyph(self, character):
        if character == ' ':
            return {'points': [], 'width': self.space}
        rows = self.glyphs[character]
        return {'points': [(x, y) for y, row in enumerate(rows) for x, bit in enumerate(row) if bit == '1'], 'width': len(rows[0])}

    def provenance(self):
        return self.proof


# --- 3. exam score suffix (examine-0001) --------------------------------------------------
# EXAMINE frame 1 is the 8x16 score suffix cell 点 (pink, 10 rows, no shadow); the digit
# cells 2-30 are 8x16 with 11 ink rows (rows 0-10). The English `points` needs more than
# 8 texels, so the primary draft is wider than the source cell (the build widens the cell);
# `pt` is the same-size fallback.
EXAM_INK = (231, 0, 165, 255)
EXAM_WORDS = (('points', 'menu9', 2, 'wide'), ('pt', 'tight', 4, 'compact'))


def build_exam(sources, faces):
    path = sources / 'original-exam-v1' / 'exam.frame-01.png'
    original = load(path)
    if colors_of(original) != {TRANSPARENT, EXAM_INK}:
        raise ValueError('exam suffix colours differ')
    out = []
    for text, face_name, top, variant in EXAM_WORDS:
        face = faces[face_name]
        width = face.width(text) + 1
        canvas_width = max(original.width, -(-width // 8) * 8)
        result = Image.new('RGBA', (canvas_width, original.height), TRANSPARENT)
        points = face.points(text, 1 if variant == 'wide' else 0, top)
        for x, y in points:
            if not (0 <= x < canvas_width and 0 <= y < original.height):
                raise ValueError('exam suffix leaves its cell')
            result.putpixel((x, y), EXAM_INK)
        out.append(('exam-0001' + ('' if variant == 'wide' else '-fallback'), result, original, path, {
            'text': text, 'face': face_name, 'variant': variant, 'frame': 1, 'source_container_sha256': EXAM_SOURCE_SHA,
            'ink_width': width - 1, 'required_width': canvas_width, 'source_width': original.width}))
    return out


# --- 4. option screen cast list -------------------------------------------------------------
# Approved spellings (owner, 2026-11-01): family name first, no long-vowel marks, no initials.
OPTION_CAST = {
    63: 'Kanazuki Mami (Fujisaki)', 65: 'Sekine Akiko (Kisaragi)', 67: 'Naka Tomoko (Himoo)',
    69: 'Kawaguchi Masayo (Katagiri)', 71: 'Sugawara Sachiko (Nijino)', 73: 'Kurosaki Ayako (Koshiki)',
    75: 'Sasaki Ayako (Kiyokawa)', 77: 'Igarashi Rei (Kagami)', 79: 'Teppozuka Yoko (Asahina)',
    81: 'Kurihara Mikiko (Mikihara)', 83: 'Yoshiki Kurin (Saotome)', 85: 'Kikuchi Shiho (Tatebayashi)',
    87: 'Tsunoda Narumi (Ijuin)', 89: 'Ueda Yuji (Yoshio)',
}
OPTION_FIXED = {63: OPTION_CAST[63], 79: OPTION_CAST[79], 103: 'Kiyokawa Nozomi', 113: 'Tatebayashi Miharu'}
OPTION_SHADOW = (24, 24, 24, 255)
OPTION_TEXT_ROWS = 12     # rows 0-11 hold the lettering, rows 12-15 stay as in the source


def option_ink(pixel):
    return pixel[3] != 0 and pixel != OPTION_SHADOW


def option_label(original, text, face):
    """The native-menu-face label of an OPTION frame (same layout rules as translation_option_drafts)."""
    pixels = [(x, y) for y in range(OPTION_TEXT_ROWS) for x in range(original.width) if option_ink(original.getpixel((x, y)))]
    left, baseline = min(x for x, y in pixels), max(y for x, y in pixels)
    colors = []
    everything = Counter(original.getpixel(p) for p in pixels)
    for y in range(11):
        counts = Counter(original.getpixel((x, y)) for x in range(original.width) if option_ink(original.getpixel((x, y))))
        colors.append((counts or everything).most_common(1)[0][0])
    palette = [colors[round(i * 10 / 8)] for i in range(9)]
    top = baseline - 8
    advance = sum(face.advance(c) for c in text)
    ink_right = left + advance - 1            # last ink column + 1 (the trailing gap is not ink)
    needed = max(original.width, ink_right + 1)   # +1: shadow column
    result = Image.new('RGBA', (needed, original.height), TRANSPARENT)
    result.paste(original.crop((0, OPTION_TEXT_ROWS, original.width, original.height)), (0, OPTION_TEXT_ROWS))
    letters = []
    x = left
    for character in text:
        if character != ' ':
            letters.extend((x + dx, top + dy, dy) for dx, dy in face.glyph(character)['points'])
        x += face.advance(character)
    for x, y, _ in letters:
        if y + 1 >= OPTION_TEXT_ROWS:
            raise ValueError('option glyph shadow overflow: ' + text)
        result.putpixel((x + 1, y + 1), OPTION_SHADOW)
    for x, y, row in letters:
        result.putpixel((x, y), palette[row])
    facts = {'text': text, 'source_ink_left': left, 'source_ink_baseline': baseline, 'font_origin': [left, top],
             'row_ink_rgba': palette, 'source_width': original.width, 'ink_right': ink_right, 'required_width': needed,
             'required_tiles_16px': -(-needed // 16), 'source_tiles_16px': -(-original.width // 16),
             'widened': needed > original.width}
    return result, facts


def build_option(sources, face):
    out = []
    for odd, text in OPTION_FIXED.items():
        for frame in (odd, odd + 1):
            path = sources / 'original-option-v1' / ('option.frame-%03d.png' % frame)
            original = load(path)
            result, facts = option_label(original, text, face)
            out.append((record_id('option-label', frame), result, original, path,
                        {'frame': frame, 'source_container_sha256': OPTION_SOURCE_SHA, **facts}))
    return out


# --- 5. name entry selector labels ---------------------------------------------------------
# Frames 15-24 are 32x16 selector cells: a one texel yellow outline (interior transparent)
# and two-kanji lettering in a single ink colour with a (+1,+1) shadow; odd frames are the
# plain state, even frames the lit state. The full English words do not fit 32 texels in a
# readable face, so the primary set is 48 wide (the outline stretches, the interior stays
# transparent); the compact set keeps 32 with the shortest clear forms.
NAME_LABELS = {15: 'Hiragana', 17: 'Katakana', 19: 'Symbols', 21: 'Kanji', 23: 'Done'}
NAME_COMPACT = {15: 'Hira', 17: 'Kata', 19: 'Symb', 21: 'Kanji', 23: 'Done'}
NAME_WIDE = 56
NAME_TOP = 3        # glyph box row of the nine-row face: lettering rows 3-11, shadow row 12


def name_label(original, text, face, width):
    frame_color, ink, shadow = None, None, None
    palette = Counter(p for p in original.getdata() if p[3])
    # colours by role: frame is the outline colour at (3, 0); the shadow is the darker lettering colour
    frame_color = original.getpixel((3, 0))
    others = [c for c in palette if c != frame_color]
    if len(others) != 2:
        raise ValueError('name label colours differ')
    shadow, ink = sorted(others, key=lambda c: sum(c[:3]))
    result = Image.new('RGBA', (width, original.height), TRANSPARENT)
    # Only the outline survives: the left and right halves keep their frame texels (the
    # lettering and its shadow are cleared) and the outline rows are repeated between them.
    for x in range(original.width):
        target = x if x < 16 else x + width - 32
        for y in range(original.height):
            if original.getpixel((x, y)) == frame_color:
                result.putpixel((target, y), frame_color)
    for x in range(16, width - 16):
        for y in (0, original.height - 1):
            result.putpixel((x, y), original.getpixel((16, y)))
    inner_left, inner_right = 3, width - 3     # lettering may use columns 3..width-4
    text_width = face.width(text)
    left = inner_left + (inner_right - inner_left - text_width - 1) // 2   # -1: the shadow column
    points = face.points(text, left, NAME_TOP)
    for x, y in points:
        if not (inner_left <= x + 1 < inner_right and 1 <= y and y + 1 <= original.height - 2):
            raise ValueError('name label touches the frame: ' + text)
    for x, y in points:
        result.putpixel((x + 1, y + 1), shadow)
    for x, y in points:
        result.putpixel((x, y), ink)
    margins = {'left': left - inner_left, 'right': inner_right - (left + text_width + 1)}
    return result, {'text': text, 'ink_rgba': list(ink), 'shadow_rgba': list(shadow), 'frame_rgba': list(frame_color),
                    'text_width': text_width, 'margins_to_frame': margins, 'required_width': width,
                    'source_width': original.width, 'lit': ink[0] > 200}


def build_name_labels(sources, face):
    out = []
    for words, width, variant in ((NAME_LABELS, NAME_WIDE, 'wide'), (NAME_COMPACT, 32, 'compact')):
        for odd, text in words.items():
            for frame in (odd, odd + 1):
                path = sources / 'original-name-v1' / ('name.frame-%02d.png' % frame)
                original = load(path)
                result, facts = name_label(original, text, face, width)
                out.append((record_id('name-label', frame), result, original, path,
                            {'frame': frame, 'variant': variant, 'source_container_sha256': NAME_SOURCE_SHA, **facts}))
    return out


# --- 6./8. FILESEL button faces (frames 19 and 20) -----------------------------------------
# Both frames are sheets of 16-row button faces: paper (231) faces, a 2 row bevel under each,
# dark ink lettering (16). Each button is (face left, face width, ink rect top); the face
# spans rows rect_top-2 .. rect_top+11 and the Japanese ink spans rect_top .. rect_top+10.
PAPER = (231, 231, 231, 255)
INK = (16, 16, 16, 255)
BLUE = (0, 57, 132, 255)
FILESEL_MARGIN = 2          # texels of clear paper kept on each side, as the kanji keep
GIRL_COLUMNS = ((1, 29, ('Shiori', 'Mio', 'Yuina', 'Ayako'), 24),
                (64, 29, ('Saki', 'Yukari', 'Nozomi', 'Mira'), 24),
                (127, 43, ('Rei', 'Yuko', 'Megumi', 'Yumi', 'Miharu'), 7))
STAT_COLUMNS = ((5, 43, ('Health', 'Humanities', 'Science', 'Art'), 24),
                (110, 44, ('Sports', 'Trivia', 'Looks', 'Guts', 'Stress'), 7))


def button_tops(first, count):
    return [first + i * 16 + (1 if first == 7 and i else 0) for i in range(count)]


def filesel_buttons(original, columns, face):
    """Clear the dark ink of every button face and letter the words in `face`, centred."""
    result = original.copy()
    placed = []
    for left, width, words, first in columns:
        for top, text in zip(button_tops(first, len(words)), words):
            for y in range(top - 2, top + 14):      # the Japanese ink reaches the last face row of the bottom buttons
                for x in range(left, left + width):
                    if result.getpixel((x, y)) == INK:
                        result.putpixel((x, y), PAPER)
            text_width = face.width(text)
            room = width - 2 * FILESEL_MARGIN
            if text_width > room:
                raise ValueError('button label does not fit with its margin: ' + text)
            x0 = left + (width - text_width) // 2
            points = face.points(text, x0, top + 1)
            rows = [y for x, y in points]
            if min(rows) < top - 1 or max(rows) > top + 9:
                raise ValueError('button label leaves its face rows: ' + text)
            for x, y in points:
                if result.getpixel((x, y)) != PAPER:
                    raise ValueError('button label touches the bevel or a neighbour: ' + text)
                result.putpixel((x, y), INK)
            placed.append({'text': text, 'face': [left, top - 2, width, 14], 'ink': [x0, min(rows), text_width, max(rows) - min(rows) + 1],
                           'margins': [x0 - left, left + width - (x0 + text_width)]})
    return result, placed


def native_header(result, font, rect, text):
    """The blue `Stats` header of frame 19: the unscaled game font, centred in `rect`."""
    x, y, width, height = rect
    glyphs = [font.glyph(c) for c in text]
    text_width = sum(g['width'] for g in glyphs) + len(glyphs) - 1
    top = min(dy for g in glyphs for dx, dy in g['points'])
    bottom = max(dy for g in glyphs for dx, dy in g['points'])
    for yy in range(y, y + height):
        for xx in range(x, x + width):
            if result.getpixel((xx, yy)) == BLUE:
                result.putpixel((xx, yy), TRANSPARENT)
    cursor, baseline = x + (width - text_width) // 2, y + (height - (bottom - top + 1)) // 2 - top
    for g in glyphs:
        for dx, dy in g['points']:
            result.putpixel((cursor + dx, baseline + dy), BLUE)
        cursor += g['width'] + 1


def build_filesel(sources, face, font):
    out = []
    for frame, columns in ((20, GIRL_COLUMNS), (19, STAT_COLUMNS)):
        path = sources / 'original-filesel-v1' / ('filesel.frame-%02d.base.png' % frame)
        original = load(path)
        result, placed = filesel_buttons(original, columns, face)
        if frame == 19:
            native_header(result, font, [16, 4, 80, 14], 'Stats')
        if not colors_of(result) <= colors_of(original):
            raise ValueError('FILESEL palette changed')
        out.append(('filesel-frame-%d.base' % frame, result, original, path,
                    {'frame': frame, 'source_container_sha256': FILESEL_SOURCE_SHA, 'buttons': placed,
                     'min_margin': min(min(b['margins']) for b in placed)}))
    return out


# --- 7. FILESEL information page 0: System tab ----------------------------------------------
SYSTEM_TAB_SOURCE_RECT = [17, 88, 39, 11]     # the registered region (filesel_page_regions.json, `system-tab`)
SYSTEM_TAB_RECT = [17, 88, 42, 11]            # widened by the shift: the tab interior is white there in the source
SYSTEM_TAB_SHIFT = 2        # the source ink starts 0-1 texels from the tab edge and clips; two clear texels now


def build_page_information(sources, checkpoint_root=ARTWORK / 'checkpoints/T39004-filesel-v4/filesel-indexed-v4'):
    path = sources / 'original-filesel-v1' / 'filesel.source-page-information-0.png'
    original = load(path)
    english_path = checkpoint_root / 'source-page-information-0.png'
    english = load(english_path)
    x, y, w, h = SYSTEM_TAB_RECT
    white = (255, 255, 255, 255)
    if any(original.getpixel((x + i, y + j)) != white or english.getpixel((x + i, y + j)) != white
           for i in range(SYSTEM_TAB_SOURCE_RECT[2], w) for j in range(h)):
        raise ValueError('the widened System tab region is not clear white')
    region = english.crop((x, y, x + w, y + h))
    ink = [(i, j) for j in range(h) for i in range(w) if region.getpixel((i, j)) != white]
    if not ink or any(i + SYSTEM_TAB_SHIFT >= w for i, j in ink):
        raise ValueError('System tab text does not fit after the shift')
    result = english.copy()
    for j in range(h):
        for i in range(w):
            result.putpixel((x + i, y + j), white)
    for i, j in ink:
        result.putpixel((x + i + SYSTEM_TAB_SHIFT, y + j), region.getpixel((i, j)))
    left_before = x + min(i for i, j in ink)
    return [('filesel-page-information-0', result, original, path, {
        'source_container_sha256': FILESEL_SOURCE_SHA, 'page': 0, 'region': SYSTEM_TAB_RECT, 'registered_region': SYSTEM_TAB_SOURCE_RECT, 'shift': SYSTEM_TAB_SHIFT,
        'ink_left_before': left_before, 'ink_left_after': left_before + SYSTEM_TAB_SHIFT, 'base_draft': 'T39004-filesel-v4',
        'base_draft_sha256': digest(english_path.read_bytes())})]


# --- 9. sports card title (kasetu-0) -------------------------------------------------------------
SPORTS_TITLE = 'Scavenger Race'
SPORTS_HEADING = [56, 40, 208, 16]


def build_sports(sources, font, draft=OLD / 'sports-drafts-v2/kasetu-0.png'):
    """The existing kasetu-0 draft with only its heading re-lettered (same face, same centring)."""
    path = sources / 'sports-kasetu' / 'kasetu.instruction-card-0.png'
    original = load(path)
    english = load(draft)
    left, top, width, height = SPORTS_HEADING
    region = [english.getpixel((x, y)) for y in range(top, top + height) for x in range(left, left + width)]
    paper = Counter(region).most_common(1)[0][0]
    inks = Counter(c for c in region if c != paper)
    ink = inks.most_common(1)[0][0]
    result = english.copy()
    result.paste(paper, (left, top, left + width, top + height))
    x = left + (width - font.width(SPORTS_TITLE)) // 2
    for character in SPORTS_TITLE:
        glyph = font.glyph(character)
        for dx, dy in glyph['points']:
            result.putpixel((x + dx, top + dy), ink)
        x += glyph['width'] + 1
    if not colors_of(result) <= colors_of(original):
        raise ValueError('sports title colour outside the card')
    return [('kasetu-0', result, original, path, {
        'source_container_sha256': SPORTS_SOURCE_SHA, 'text': SPORTS_TITLE, 'heading_region': SPORTS_HEADING,
        'ink_rgba': list(ink), 'paper_rgba': list(paper), 'base_draft': 'T39004-2026-10-29/sports-drafts-v2',
        'base_draft_sha256': digest(draft.read_bytes())})]


# --- fallbacks (same cell sizes as the source) ------------------------------------------------
def build_fallbacks(sources, faces):
    """Drafts for builds that cannot widen a sprite: family names only on the option screen, `pt`
    on the exam cell, the compact selector labels on the name screen."""
    out = []
    for odd, family in ((103, 'Kiyokawa'), (113, 'Tatebayashi')):
        for frame in (odd, odd + 1):
            path = sources / 'original-option-v1' / ('option.frame-%03d.png' % frame)
            original = load(path)
            result, facts = option_label(original, family, faces['menu9'])
            if facts['widened']:
                raise ValueError('family-name fallback does not fit the source width: ' + family)
            out.append(('option-label', record_id('option-label', frame), result, original, path,
                        {'frame': frame, 'source_container_sha256': OPTION_SOURCE_SHA, 'fallback_for': 'full name', **facts}))
    for row in build_exam(sources, faces):
        if row[0].endswith('-fallback'):
            out.append(('exam', 'exam-0001', *row[1:4], {**row[4], 'fallback_for': 'points'}))
    for row in build_name_labels(sources, faces['menu9']):
        if row[4]['variant'] == 'compact':
            out.append(('name-label', row[0], *row[1:4], {**row[4], 'fallback_for': 'wide selector labels'}))
    return out


# --- ALL_C container gates ---------------------------------------------------------------------
def allc_container(disc_root, strip_english, strip_original):
    """Map the English strip onto the container's own indices and prove the indexed gates.

    Every English colour must be a colour of an index the original strip already uses;
    nothing outside the strip, the palette or the trailer changes, and the size is the same.
    """
    import struct
    from translation_bs_drafts import page_texture, texture_bytes
    data = (disc_root / 'CDROM/LASTPHOT/ALL_C.NBN').read_bytes()
    if digest(data) != ALLC_SOURCE_SHA:
        raise ValueError('ALL_C source differs')
    words = struct.unpack_from('<256H', data, 0x18000)
    def rgb(index):
        v = words[index]
        return tuple(((c << 3) | (c >> 2)) & 255 for c in ((v & 31), (v >> 5) & 31, (v >> 10) & 31))
    texture = page_texture(data)
    x, y, w, h = 0, 202, ALLC_WIDTH, ALLC_ROWS
    area = texture[y:y + h, x:x + w]
    used = Counter(int(v) for v in area.flatten())
    by_color = {}
    for index, count in used.items():
        by_color.setdefault(rgb(index), []).append((count, index))
    # the decoded original strip must be what the indices say
    for yy in range(h):
        for xx in range(w):
            if strip_original.getpixel((xx, yy))[:3] != rgb(int(area[yy, xx])):
                raise ValueError('ALL_C source page differs from the container indices')
    lettered = texture.copy()
    chosen = {}
    for yy in range(h):
        for xx in range(w):
            color = strip_english.getpixel((xx, yy))[:3]
            if color not in by_color:
                raise ValueError('English colour without an original index: %r' % (color,))
            index = max(by_color[color])[1]
            chosen[color] = index
            lettered[y + yy, x + xx] = index
    english = texture_bytes(data, lettered)
    outside = np.ones(texture.shape, bool)
    outside[y:y + h, x:x + w] = False
    failures = []
    if (lettered[outside] != texture[outside]).any():
        failures.append('pixels changed outside the strip')
    if len(english) != len(data) or english[0x18000:] != data[0x18000:]:
        failures.append('byte extent or palette changed')
    if not set(int(v) for v in np.unique(lettered[y:y + h, x:x + w])) <= set(used):
        failures.append('palette index absent from the source strip')
    if failures:
        raise ValueError('ALL_C draft rejected: ' + '; '.join(failures))
    return {'english_container_sha256': digest(english), 'palette_indices': {'%d,%d,%d' % k: v for k, v in sorted(chosen.items())},
            'checks': ['indices outside strip unchanged', 'only indices already in the source strip',
                       'palette and trailer bytes unchanged', 'file size unchanged']}


# --- build, ledger, supersedes ------------------------------------------------------------------
OLD_LEDGER = 'translations/en/graphics/artwork/checkpoints/T39004-2026-10-29/ledger.json'
FILESEL_V4_LEDGER = 'translations/en/graphics/artwork/checkpoints/T39004-filesel-v4/ledger.json'
BS_V1_LEDGER = 'translations/en/graphics/artwork/checkpoints/T39004-bs-v1/ledger.json'
SUPERSEDED = (
    [(OLD_LEDGER, 'title-%04d' % n, 'title-drafts-v1') for n in (1, 2, 3, 4)]
    + [(OLD_LEDGER, 'exam-0001', 'exam-drafts-v1')]
    + [(OLD_LEDGER, 'option-label-%04d' % n, 'option-drafts-v1') for n in (63, 64, 79, 80, 103, 104, 113, 114)]
    + [(OLD_LEDGER, 'name-label-%04d' % n, 'name-drafts-v2') for n in range(15, 25)]
    + [(OLD_LEDGER, 'name-label-%04d' % n, 'name-reviewed-development-v1') for n in (15, 17, 19, 21, 23)]
    + [(OLD_LEDGER, 'name-panel-%04d' % n, 'name-drafts-v2') for n in (0, 1, 2, 14)]
    + [(OLD_LEDGER, 'filesel-frame-19.base', 'filesel-drafts-v2'), (OLD_LEDGER, 'filesel-frame-20.base', 'filesel-drafts-v2'),
       (OLD_LEDGER, 'kasetu-0', 'sports-drafts-v2'), (FILESEL_V4_LEDGER, 'filesel-page-information-0', 'filesel-indexed-v4')]
    + [(BS_V1_LEDGER, 'all-c-banner.source-page-%d' % n, 'bs-v1') for n in (3, 4, 5)])
# What each superseded id is replaced by (for REGISTER.md): item -> new record ids.
STATUS_NOTE = 'Candidate artwork; port-made menu fix pass (T-39410); unreviewed, not runtime integrated.'


def superseded_refs():
    refs = []
    cache = {}
    for ledger, ident, revision in SUPERSEDED:
        if ledger not in cache:
            cache[ledger] = json.loads((ROOT / ledger).read_text())
        found = [a for a in cache[ledger]['assets'] if a['id'] == ident and a.get('revision') == revision]
        if ident.startswith('all-c-banner.'):
            manifest = json.loads((ROOT / ledger).parent.joinpath('bs-v1/manifest.json').read_text())
            page = int(ident.rsplit('-', 1)[1])
            record = [r for r in manifest['indexed_assets'] if r['id'] == 'all-c-banner'][0]
            sha = [r['sha256'] for r in record['edit_regions'] if r['page'] == page][0]
            refs.append({'id': ident, 'ledger': ledger, 'output_sha256': sha, 'revision': revision})
            continue
        if len(found) != 1:
            raise ValueError('superseded candidate not found: %s %s' % (ident, revision))
        refs.append({'id': ident, 'ledger': ledger, 'output_sha256': found[0]['output_sha256'], 'revision': revision})
    return refs


LETTERING = {
    'title': [{'element': 'lettering', 'face': 'port-skeleton-gothic', 'kind': 'lettering',
               'description': 'single-stroke skeleton capitals of translation_bs_font.py stroked 3 texels, filling rows 2-13, 1 texel dark outline and the row-by-row fill gradient sampled from the Japanese lettering; the slab pixels of the highlighted frames are kept'}],
    'allc': [{'element': 'lettering', 'face': 'port-skeleton-gothic', 'kind': 'lettering',
              'description': 'single-stroke skeleton capitals stroked 3 texels at 18 rows (the Japanese height), anti-aliased with the original strip tints; blue ALL CLEAR, red CONGRATULATIONS, purple !'}],
    'menu9': [{'element': 'lettering', 'face': 'menu9', 'kind': 'lettering',
               'description': 'recovered 9-row menu face, source ink gradient rows and (+1,+1) shadow as the other option labels'}],
    'tight': [{'element': 'lettering', 'face': 'tight', 'kind': 'lettering',
               'description': 'port-made 3-texel lowercase / 4-texel capital face (tools/translation_menus_fixes.py), unscaled, ink 16 on the paper face'}],
    'native': [{'element': 'lettering', 'face': 'native', 'kind': 'lettering',
                'description': 'unscaled native game font, centred, same ink as the existing draft'}],
    'page': [{'element': 'lettering', 'face': 'native', 'kind': 'shift',
              'description': 'the existing System word moved right by two texels inside the tab; no glyph redrawn'}],
}


def write_png(image, path):
    path.parent.mkdir(parents=True, exist_ok=True)
    image.save(path)
    return digest(path.read_bytes())


def build(sources, output, disc_root=ROOT / 'disc/files'):
    """Everything: PNGs, manifest.json, fallback-v1, ledger.json, fallback-ledger.json, REGISTER.md."""
    output = new_output(output)
    menu9, tight, native = Menu9(), Tight(), NativeLatin(EXE)
    faces = {'menu9': menu9, 'tight': tight}
    primary = REVISION
    root = output
    items, ledger_assets = [], []

    def add(item_id, image, original, path, facts, face, name, folder=primary, collection=None, ledger_extra=None):
        sha = write_png(image, root / folder / name)
        box = changed_box(original, image) if image.size == original.size else None
        record = {'id': item_id, 'output': name, 'output_sha256': sha, 'dimensions': list(image.size),
                  'source_png': str(path.relative_to(sources)), 'source_png_sha256': digest(path.read_bytes()), 'edit_box': box,
                  'face': face, 'style': 'unreviewed', 'runtime_integrated': False, 'in_game_verified': False, **facts}
        (collection if collection is not None else items).append(record)
        return record

    # 1 title
    for item_id, image, original, path, facts in build_title(sources):
        add(item_id, image, original, path, facts, 'port-skeleton-gothic', item_id + '.png')
    # 2 ALL_C
    strip, english, crops, runs = build_allc(sources)
    gates = allc_container(disc_root, english, strip)
    page_rows = []
    for page, crop, original_crop, path, rect in crops:
        name = 'all-c.source-page-%d.en.edit-region.png' % page
        page_rows.append({'page': page, 'page_rect': rect, 'png': name, 'sha256': write_png(crop, root / primary / name)})
    pages_sha = {}
    for page in range(6):
        pages_sha['source-page-%d' % page] = digest((sources / 'allc' / ('all-c.source-page-%d.png' % page)).read_bytes())
    allc_record = {'id': 'all-c-banner', 'catalog_item': 'catalog-127', 'source_container': 'CDROM/LASTPHOT/ALL_C.NBN',
                   'source_container_sha256': ALLC_SOURCE_SHA, 'english_container_sha256': gates['english_container_sha256'],
                   'source_pages': [{'variant': k, 'source_png_sha256': v} for k, v in pages_sha.items()],
                   'palette_id': 'source-offset-0x18000', 'strip_texture_rect': [0, 202, ALLC_WIDTH, ALLC_ROWS],
                   'paper_index': 6, 'lettering': {'runs': runs, 'cap_rows': ALLC_HEIGHT, 'stroke': ALLC_STROKE, 'top_row': ALLC_ROW0 + ALLC_TOP,
                                                   'aspect': ALLC_ASPECT, 'strip_rows': [ALLC_TOP, ALLC_TOP + ALLC_HEIGHT - 1]},
                   'palette_indices': gates['palette_indices'], 'edit_regions': page_rows, 'gates_passed': True, 'checks': gates['checks'],
                   'palette_note': 'blue/red/purple of the Japanese lettering kept (the strip holds no gold index); runtime composition not verified',
                   'runtime_composition_verified': False, 'style': 'unreviewed', 'human_approved': False,
                   'runtime_integrated': False, 'in_game_verified': False}
    # 3 exam
    exam = build_exam(sources, faces)
    for item_id, image, original, path, facts in exam:
        if item_id == 'exam-0001':
            add(item_id, image, original, path, facts, 'menu9', 'exam-0001.png')
    # 4 option
    for item_id, image, original, path, facts in build_option(sources, menu9):
        add(item_id, image, original, path, facts, 'menu9', item_id + '.png')
    # 5 name labels
    for item_id, image, original, path, facts in build_name_labels(sources, menu9):
        if facts['variant'] == 'wide':
            add(item_id, image, original, path, facts, 'menu9', item_id + '.png')
    # 6, 8 filesel buttons; 7 information page; 9 sports
    for item_id, image, original, path, facts in build_filesel(sources, tight, native):
        add(item_id, image, original, path, facts, 'tight', item_id + '.png')
    for item_id, image, original, path, facts in build_page_information(sources):
        add(item_id, image, original, path, facts, 'native', 'filesel-page-information-0.png')
    for item_id, image, original, path, facts in build_sports(sources, native):
        add(item_id, image, original, path, facts, 'native', 'kasetu-0.png')

    fallbacks = []
    for kind, item_id, image, original, path, facts in build_fallbacks(sources, faces):
        add(item_id, image, original, path, facts, 'menu9', '%s.png' % item_id, folder='fallback-v1', collection=fallbacks)
    manifest = {'schema': SCHEMA, 'ticket': TICKET, 'revision': primary, 'human_approved': False, 'items': items,
                'indexed_assets': [allc_record], 'font_provenance': {'menu9': menu9.provenance(), 'tight': tight.provenance(),
                'native': native.provenance(), 'skeleton': {'module': 'translation_bs_font', 'glyphs': ''.join(sorted(skeleton.GLYPHS))}},
                'committed_files': 'English crops and cells only; no original pixel'}
    (root / primary / 'manifest.json').write_bytes(encoded(manifest) + b'\n')
    fallback_manifest = {'schema': SCHEMA, 'ticket': TICKET, 'revision': 'fallback-v1', 'human_approved': False, 'items': fallbacks,
                         'note': 'same-size fallbacks for builds that cannot widen a sprite; not superseding anything on their own'}
    (root / 'fallback-v1' / 'manifest.json').write_bytes(encoded(fallback_manifest) + b'\n')
    return finish(root, manifest, fallback_manifest, sources)


# --- option texture room, name consistency -----------------------------------------------------
def option_texture_room(sources):
    """16x16 texture cells of OPTION.NBN: used by the sprites, free, and the demand of the English drafts."""
    manifest = json.loads((sources / 'original-option-v1' / 'manifest.json').read_text())
    frames = {r['frame_index']: r for r in manifest['images'] if r.get('frame_index') is not None and 'parts' in r}
    cells = {}
    for frame, row in frames.items():
        used = set()
        for part in row['parts']:
            for du in range(0, part['width'], 16):
                for dv in range(0, part['height'], 16):
                    used.add((part['page_index'], (part['u'] + du) // 16, (part['v'] + dv) // 16))
        cells[frame] = used
    ledger = json.loads((ROOT / OLD_LEDGER).read_text())
    width = {int(a['id'][-4:]): a['dimensions'][0] for a in ledger['assets'] if a['id'].startswith('option-label-')}
    width.update({103: 71, 104: 71, 113: 82, 114: 82})
    # frames that share all their tiles (normal and highlighted state) count once
    groups = {}
    for frame in width:
        if frame in cells:
            groups.setdefault(frozenset(cells[frame]), []).append(frame)
    english_cells = sum(-(-max(width[f] for f in frames) // 16) for frames in groups.values())
    japanese = set().union(*groups)
    pairs = groups
    everything = set().union(*cells.values())
    others = set().union(*[cells[f] for f in cells if f not in width])
    reusable = japanese - others
    return {'texture_cells_total': 256, 'cells_used_by_sprites': len(everything), 'cells_free': 256 - len(everything),
            'draft_label_pairs': len(pairs), 'english_cells_needed_unshared': english_cells,
            'cells_of_those_japanese_labels': len(japanese), 'cells_reusable_when_those_labels_are_replaced': len(reusable),
            'cells_available_to_them': len(reusable) + 256 - len(everything),
            'room': len(reusable) + 256 - len(everything) >= english_cells}


def pill_agreement():
    """The movie pills and the option labels spell every name the same way."""
    import translation_movie_pills as pills
    actress = {english for _, _, rows in pills.CUES for _, role, _, english, _, _ in rows if role == 'actress'}
    character = {english for _, _, rows in pills.CUES for _, role, _, english, _, _ in rows if role == 'character'}
    manifest = json.loads((OLD / 'option-drafts-v1/manifest.json').read_text())
    texts = {a['frame']: a['text'] for a in manifest['assets']}
    option_actress = {OPTION_CAST[odd].split(' (')[0] for odd in OPTION_CAST}
    option_character = {texts[f] for f in range(91, 117) if f not in OPTION_FIXED and f - 1 not in OPTION_FIXED}
    option_character |= {OPTION_FIXED[103], OPTION_FIXED[113]}
    return {'actress_names_equal': actress == option_actress, 'character_names_in_pills': option_character <= character,
            'actress_only_in_pills': sorted(actress - option_actress), 'actress_only_in_options': sorted(option_actress - actress),
            'character_only_in_options': sorted(option_character - character), 'pill_names': len(actress) + len(character)}


# --- catalog corrections (REGISTER.md) --------------------------------------------------------------
# image_text_catalog.jsonl rows (1-based line) of the option cast list, as the inventory binds them
# today by their frame ids, and the frames the art actually shows (checked on the restored sprites).
OPTION_BINDING = (
    # (catalog line, row says (JP, English), frames it is bound to now, frames its art is on, action)
    (72, '金月真美 (藤崎)', 'Kingetsu Mami (Fujisaki)', '0063/0064', '0063/0064', 'respell: Kanazuki Mami (Fujisaki)'),
    (73, '長沢美樹 (如月)', 'Nagasawa Miki (Kisaragi)', '0065/0066', 'none', 'no sprite shows this name; the Kisaragi role is 関根明子, so retire the row (it duplicates line 79)'),
    (74, '中友子 (紐緒)', 'Naka Tomoko (Himoo)', '0067/0068', '0067/0068', 'none'),
    (75, '川口雅代 (片桐)', 'Kawaguchi Masayo (Katagiri)', '0069/0070', '0069/0070', 'none'),
    (76, '鉄炮塚葉子 (朝日奈)', 'Teppouzuka Yoko (Asahina)', '0071/0072', '0079/0080', 'rebind to 0079/0080; respell: Teppozuka Yoko (Asahina)'),
    (77, '黒崎彩子 (古式)', 'Kurosaki Ayako (Koshiki)', '0073/0074', '0073/0074', 'none'),
    (78, 'よしきくりん (早乙女)', 'Yoshiki Kurin (Saotome)', '0075/0076', '0083/0084', 'rebind to 0083/0084'),
    (79, '関根明子 (如月)', 'Sekine Akiko (Kisaragi)', '0077/0078', '0065/0066', 'rebind to 0065/0066'),
    (80, '栗原みきこ (美樹原)', 'Kurihara Mikiko (Mikihara)', '0079/0080', '0081/0082', 'rebind to 0081/0082'),
    (81, '五十嵐麗 (鏡)', 'Igarashi Rei (Kagami)', '0081/0082', '0077/0078', 'rebind to 0077/0078'),
    (82, '笹木綾子 (清川)', 'Sasaki Ayako (Kiyokawa)', '0083/0084', '0075/0076', 'rebind to 0075/0076'),
    (83, '津野田なるみ (伊集院)', 'Tsunoda Narumi (Ijuin)', '0085/0086', '0087/0088', 'rebind to 0087/0088'),
    (84, '菊池志穂 (館林)', 'Kikuchi Shiho (Tatebayashi)', '0087/0088', '0085/0086', 'rebind to 0085/0086'),
    (85, '上田祐司 (好雄)', 'Ueda Yuji (Yoshio)', '0089/0090', '0089/0090', 'none'),
)
OPTION_MISSING_ROW = ('0071/0072', '菅原祥子 (虹野)', 'Sugawara Sachiko (Nijino)',
                      'no catalog row exists for this sprite: add one (section 2, 122x16, suggested graphic the same words)')


def elements_for(record):
    item = record['id']
    if item.startswith('title-'):
        return LETTERING['title']
    if record['face'] == 'tight':
        return LETTERING['tight']
    if item == 'filesel-page-information-0':
        return LETTERING['page']
    if record['face'] == 'native':
        return LETTERING['native']
    return LETTERING['menu9']


def ledger_entry(record, manifest_sha, folder, revision):
    entry = {'dimensions': record['dimensions'], 'human_approved': False, 'id': record['id'], 'manifest_sha256': manifest_sha,
             'output_sha256': record['output_sha256'], 'path': '%s/%s' % (folder, record['output']), 'port_made': True,
             'port_made_elements': elements_for(record), 'reason': STATUS_NOTE, 'revision': revision,
             'source_container_sha256': record['source_container_sha256'], 'source_png_sha256': record['source_png_sha256'],
             'status': 'draft'}
    if record.get('edit_box'):
        entry['edit_regions'] = [record['edit_box']]
    if record.get('region'):
        entry['edit_regions'] = [record['region']]
    return entry


def finish(root, manifest, fallback_manifest, sources):
    manifest_sha = digest((root / REVISION / 'manifest.json').read_bytes())
    fallback_sha = digest((root / 'fallback-v1' / 'manifest.json').read_bytes())
    assets = [ledger_entry(r, manifest_sha, REVISION, REVISION) for r in manifest['items']]
    allc = manifest['indexed_assets'][0]
    assets.append({'catalog_item': 'catalog-127', 'english_container_sha256': allc['english_container_sha256'], 'human_approved': False,
                   'id': 'all-c-banner', 'manifest_sha256': manifest_sha,
                   'paths': ['%s/%s' % (REVISION, r['png']) for r in allc['edit_regions']],
                   'source_container_sha256': ALLC_SOURCE_SHA, 'status': 'draft', 'port_made': True,
                   'port_made_elements': LETTERING['allc']})
    refs = superseded_refs()
    ledger = {'assets': assets, 'candidate_assets': len(assets), 'complete_translation_inventory': False,
              'original_images_included': False, 'port_made_assets': len(assets),
              'publication_authorization': 'Owner art fix pass, 2026-11-01 (T-39410); owner approved the naming decisions recorded in REGISTER.md.',
              'schema': 'tokimemo.translation-checkpoint.v1', 'separate_reviewed_revisions': 0,
              'supersedes': refs, 'ticket': TICKET}
    (root / 'ledger.json').write_bytes(json.dumps(ledger, indent=1, sort_keys=True, ensure_ascii=False).encode() + b'\n')
    fallback_assets = [ledger_entry(r, fallback_sha, 'fallback-v1', 'fallback-v1') for r in fallback_manifest['items']]
    fallback_ledger = {'assets': fallback_assets, 'candidate_assets': len(fallback_assets), 'complete_translation_inventory': False,
                       'original_images_included': False, 'port_made_assets': len(fallback_assets),
                       'note': 'Same-size fallbacks. Register only if the English build cannot widen the sprite; supersedes nothing by itself.',
                       'publication_authorization': ledger['publication_authorization'],
                       'schema': 'tokimemo.translation-checkpoint.v1', 'separate_reviewed_revisions': 0, 'ticket': TICKET}
    (root / 'fallback-ledger.json').write_bytes(json.dumps(fallback_ledger, indent=1, sort_keys=True, ensure_ascii=False).encode() + b'\n')
    room = option_texture_room(sources)
    agreement = pill_agreement()
    if not (agreement['actress_names_equal'] and agreement['character_names_in_pills']):
        raise ValueError('movie pill names disagree with the option cast list: %r' % (agreement,))
    (root / 'REGISTER.md').write_text(register_text(manifest, fallback_manifest, refs, room, agreement))
    return {'output': str(root), 'items': len(manifest['items']), 'fallbacks': len(fallback_manifest['items']),
            'superseded': len(refs), 'texture_room': room}


def register_text(manifest, fallback_manifest, refs, room, agreement):
    by_id = {}
    for r in manifest['items']:
        by_id.setdefault(r['id'], r)
    widths = [r for r in manifest['items'] if r['id'].startswith(('option-label-', 'name-label-', 'exam-')) and r.get('required_width', 0) > r.get('source_width', 0)]
    lines = ['# REGISTER: T39410-menus-fixes-v1', '',
             'What the integrator (who owns `tools/translation_inventory.py`, `translation_inventory.json`, the image text catalog and',
             '`inventory_decisions.json`) must change to take this checkpoint. Nothing in those files was edited here.',
             'All PNGs are English-only (no original pixel), port-made, unreviewed (`human_approved: false`).', '',
             '## 1. Register the ledger', '',
             '- Add `translations/en/graphics/artwork/checkpoints/T39410-menus-fixes-v1/ledger.json` to `LEDGERS`. It holds %d candidates '
             '(%d frame/cell drafts and the ALL_C banner of `catalog-127`, three crops).' % (len(manifest['items']) + 1, len(manifest['items'])),
             '- `fallback-ledger.json` (%d same-size drafts) is not for `LEDGERS`; register it only if the English build cannot widen sprites (sections 5, 6).' % len(fallback_manifest['items']),
             '- Ids follow the existing candidate patterns (`title-NNNN`, `option-label-NNNN`, `name-label-NNNN`, `exam-0001`, `filesel-frame-N.base`, `filesel-page-information-0`, `kasetu-0`, catalog item `catalog-127`), so `candidate_key` needs no change.', '',
             '## 2. Supersedes (declared in `ledger.json`, %d references)' % len(refs), '']
    groups = {}
    for ref in refs:
        groups.setdefault((ref['ledger'].split('/')[-2], ref['revision']), []).append(ref['id'])
    for (ledger, revision), ids in groups.items():
        lines.append('- `%s` / `%s`: %s' % (ledger, revision, ', '.join('`%s`' % i for i in ids)))
    lines += ['',
              'Name-entry panels `name-panel-0000/0001/0002/0014`: the `name-drafts-v2` face is half the Japanese size. The retained '
              '`name-reviewed-development-v1` panels (11-12 px, same size as the Japanese) stay the active candidates; only v2 is superseded here and no new art was made. '
              'The ten `name-label` selector cells (both older revisions) are replaced by one revision, so lit and unlit states match.', '',
              '## 3. Catalog wording and binding edits (`translations/en/graphics/image_text_catalog.jsonl`, 1-based line = row)', '',
              '### Option cast list (rows 72-85): corrections', '',
              'The bound frames below are what the rows say today (`frame_ids_normal___highlight`); the art column is what the restored sprites show (checked by eye, all 14).', '',
              '| row | Japanese on the row | English on the row | bound to | art is on | change |', '|---|---|---|---|---|---|']
    for row, jp, en, bound, art, action in OPTION_BINDING:
        lines.append('| %d | %s | %s | %s | %s | %s |' % (row, jp, en, bound, art, action))
    frames, jp, en, action = OPTION_MISSING_ROW
    lines += ['| (new) | %s | %s | none | %s | %s |' % (jp, en, frames, action), '',
              'Result: every frame pair 0063..0090 binds to the row that names the person drawn on it. `菅原祥子` reads Sugawara Sachiko (the pill and the sprite agree).', '',
              '### Wording rows', '',
              '- Row 72: `Kingetsu Mami (Fujisaki)` -> `Kanazuki Mami (Fujisaki)` (english and suggested graphic).',
              '- Row 76 (art 0079/0080): `Teppouzuka Yoko (Asahina)` and the suggested `Y. Teppouzuka (Asahina)` -> `Teppozuka Yoko (Asahina)`.',
              '- Rows 75, 76, 80 suggested graphics abbreviate the given name (`M. Kawaguchi`, `Y. Teppouzuka`, `M. Kurihara`): all cast rows use the full `Family Given (Role)` forms the art uses.',
              '- Rows 92 and 97 (0103/0104, 0113/0114): suggested graphic -> `Kiyokawa Nozomi` (sprite widened to 71) and `Tatebayashi Miharu` (widened to 82); fallback `Kiyokawa` / `Tatebayashi` at the source widths.',
              '- Row 3 (MAINROM 0061 panel) and row 104 (FILESEL 0019): stat names are Health, Humanities, Science, Art, Sports, Trivia, Looks, Guts, Stress (rows now say Liberal Arts / Athletics / Lit. / Sci. / Sport / Condition). "Hum." was not needed on the FILESEL buttons: `Humanities` fits (38 of 43 texels). MAINROM 0061 and the text dictionary are outside this checkpoint.',
              '- Row 104 heroine buttons: Option A (given names) is what the art uses; Option B is retired.',
              '- Rows 107-111 (name selector labels): english Hiragana / Katakana / Symbols / Kanji / Done; suggested graphic `56x16` full words (fallback `32x16`: Hira / Kata / Symb / Kanji / Done). There is no Latin/English page selector cell in NAME_ENT frames 15-24; when the English build adds one, "ABC" in the same cell, face and colours is the consistent form.',
              '- Rows 113-116 (title): `GAME START` / `OPTIONS` bold outlined lettering at the Japanese height (replace the `112x16` / `80x16` plain-caps note).',
              '- Row 118 (EXAMINE 0001): english `points`; suggested graphic `points` in a widened cell (see section 5), fallback `pt` in `8x16`.',
              '- Row 123 (sports KASETU card 0): title `Scavenger Race` (approved; the card text itself says nothing about borrowing). Use the same name wherever the event is named.',
              '- Row 127 (ALL_C banner): suggested graphic says bold yellow/gold; the banner keeps the Japanese blue / red / purple, because the strip holds no gold index and the gates allow only indices the strip already uses. Stroke 3 texels, 18 rows tall like the Japanese.', '',
              '## 4. Inventory decisions (`inventory_decisions.json`)', '',
              '- None required: the ledger ids map to existing atoms, no atom changes status class. If the English build keeps `Hum.` anywhere, record it as an override there, not in art.', '',
              '## 5. Required sprite widths (English build must widen these entries)', '',
              '| item | source width | required width | note |', '|---|---|---|---|']
    for r in widths:
        note = ('%s; %d tiles of 16 (source %d)' % (r['text'], r['required_tiles_16px'], r['source_tiles_16px'])) if r['id'].startswith('option') else r['text']
        lines.append('| `%s` | %d | %d | %s |' % (r['id'], r['source_width'], r['required_width'], note))
    lines += ['',
              '- `exam-0001`: `points` is %d texels of ink plus a 1 texel left margin; the cell is drawn 32 wide (8-texel multiple). The rest of the score cells stay 8x16, so the build must place the suffix cell with 32 texels of advance (or 26 minimum).' % by_id['exam-0001']['ink_width'],
              '- `name-label-0015..0024`: all ten selector cells 56x16 (outline stretched, interior transparent); the selector positions need 24 more texels each.',
              '- `filesel-page-information-0`: the `system-tab` region in `filesel_page_regions.json` must become `[17, 88, 42, 11]` (was `[17, 88, 39, 11]`): the word is shifted right two texels, and the three extra columns are white tab interior in the source.', '',
              '## 6. OPTION.NBN texture room', '',
              'Counted from the sprite parts of the restored OPTION.NBN (16x16 cells over four 128x128 pages): %d cells in total, %d used by sprites, %d free. '
              'The %d English label pairs of the option drafts need %d cells unshared (every English sprite owns its tiles; the Japanese sprites share kanji tiles), '
              'while their Japanese labels occupy %d cells of which %d are free once replaced; with the free cells that is %d available. %s'
              % (room['texture_cells_total'], room['cells_used_by_sprites'], room['cells_free'], room['draft_label_pairs'],
                 room['english_cells_needed_unshared'], room['cells_of_those_japanese_labels'], room['cells_reusable_when_those_labels_are_replaced'],
                 room['cells_available_to_them'],
                 'There is room.' if room['room'] else 'There is clearly not enough room for all the option drafts at tile granularity, so the build has to repack the page (or the pixel-level text strips) regardless of the two widened names; the two widened names add 2 cells over the 16-texel rounding of their source widths. Family-name-only fallbacks at the source widths are therefore provided in `fallback-v1` (`option-label-0103/0104/0113/0114`).'), '',
              '## 7. Names: one spelling everywhere', '',
              '- Option cast sprites, option character sprites and the movie pills agree: actress names equal on both sides (%s), every option character name is a pill character name (%s). Checked at build time against `translation_movie_pills.CUES`; the pills needed no change (they already read Kanazuki Mami and Teppozuka Yoko with full given names).' % (agreement['actress_names_equal'], agreement['character_names_in_pills']),
              '- Script rule: family name first, no long-vowel marks or doubled vowels, no initials. `Himoo` is the script spelling and stays.', '',
              '## 8. Per item', '']
    lines += ['- `title-0001..0004`: bold skeleton capitals, 3 texel stroke, rows 2-13 with a 1 texel dark outline (rows 1-14, as the Japanese), per-row gradient sampled from the Japanese lettering (grey on 0002/0004, blue on 0001/0003). Highlighted frames keep the slab pixels untouched and the slab hides the lower rows exactly as it hides the Japanese (only the top eight fill rows show).',
              '- `catalog-127`: one-line `ALL CLEAR CONGRATULATIONS!`, 18 rows tall, stroke 3, centred with 15 texel margins; page 5 crop now holds the tail and the exclamation mark. Indexed gates pass (`english_container_sha256` in the manifest).',
              '- `exam-0001`: `points` (menu face, source pink, no shadow like the source) and the `pt` fallback.',
              '- `option-label-0063/0064/0079/0080`: respelled; `0103/0104/0113/0114`: full names (widened).',
              '- `name-label-0015..0024`: `Hiragana`, `Katakana`, `Symbols`, `Kanji`, `Done` in unlit and lit states, centred, at least 3 texels clear of the outline.',
              '- `filesel-frame-20.base`: given-name buttons in the tight face, glyphs unscaled, 2 or more texels clear of each face edge, nothing in the bevel rows.',
              '- `filesel-frame-19.base`: Health, Humanities, Science, Art, Sports, Trivia, Looks, Guts, Stress in the same face; `Stats` header as before.',
              '- `filesel-page-information-0`: System tab word moved right two texels (ink starts at x=19; the Japanese ink touched x=17).',
              '- `kasetu-0`: title `Scavenger Race`, same face and centring as before; body text unchanged.', '']
    return '\n'.join(lines)


def publish(built, destination):
    """Copy a finished build into a new checkpoint folder (PNGs, manifests, ledgers, REGISTER.md)."""
    built = Path(built)
    destination = Path(destination)
    if destination.exists():
        raise ValueError('destination exists')
    shutil.copytree(built, destination)
    return {'destination': str(destination)}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='operation', required=True)
    b = sub.add_parser('build')
    b.add_argument('--sources', type=Path, required=True, help='folder holding the original-*-v1, allc and sports-* exports')
    b.add_argument('--output', type=Path, required=True, help='new folder under .asset-cache')
    p = sub.add_parser('publish')
    p.add_argument('--built', type=Path, required=True)
    p.add_argument('--destination', type=Path, default=ARTWORK / 'checkpoints' / CHECKPOINT)
    args = parser.parse_args()
    if args.operation == 'build':
        print(json.dumps(build(args.sources, args.output)))
    else:
        print(json.dumps(publish(args.built, args.destination)))


if __name__ == '__main__':
    main()
