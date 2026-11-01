#!/usr/bin/env python3
"""T-39410: art fixes for the English MAINROM.NBN drafts (owner review, 2026-11-01).

Rebuilds, in texel-index space and through each variant's own CLUT, the MAINROM
items the owner review sent back:

* Save labels 57-59: "Save" in the same rows the Japanese used, on the lettering
  layer only (the card layer, with the yellow frame and the icon, stays original);
* magazine banners 126-133: one bold condensed headline face (hand-drawn here,
  17 rows, 2 px strokes, 1 px shadow, the weight of the Japanese) at one size, one
  wording pattern ("<Subject> Opens" for 完成, "<Subject> Special" for 特集,
  "Central Park Guide" for 紹介), flush left where the Japanese starts, with the
  banner's own edge tones on the cut corners;
* issue banners 122-125: the three-month runs named by season, "SPRING / ISSUE" and
  so on, in two bold lines with the badge's vertical colour ramp;
* genre tags 134-145: re-lettered with a 1 px margin on both sides, centred
  (TAG_ALIGN is the one switch to move them to the left); 144 is "Wrestling";
* stat labels, frame 61: the approved stat names (Health, Hum., Science, Art,
  Sports, Trivia, Looks, Guts, Stress), left-aligned in the label strips;
* game covers 151-154: English title lettering in each logo's colours and close to
  its weight (SUIKODEN, PARODIUS, POWER / PROS, TWINBEE / PUZZLE).

Round 2 (the second review of the same day) changed items 1-7 of the brief in
wiki/tickets/T-39410-mainrom-art-fixes.md; the checkpoint keeps its revision and ids.

Every variant of every frame is rendered from the one edited index map, so the
variants cannot drift. Pixels outside the declared edit regions equal the exact
source render, which is itself gated byte-for-byte against the original export.
All new lettering is port-made (hand-drawn bitmaps, never scaled or generated)
and is flagged as such in the ledger. Outputs are unreviewed drafts. Run in
Docker only.
"""
import argparse
import json
from pathlib import Path

from PIL import Image

from translation_assets import ROOT, digest, encoded, new_output
import translation_mainrom_drafts as base

VARIANTS = base.VARIANTS
CLEAR = base.CLEAR
CHECKPOINT = 'T39410-mainrom-fixes-v1'
REVISION = 'mainrom-fixes-v1'
TICKET = 'T-39410'
# Genre-tag alignment: the owner decision (2026-11-01) is centred with a 1 px margin
# on both sides. 'left' keeps a 1 px left margin and is the quick alternative.
TAG_ALIGN = 'center'
TAG_MARGIN = 1


# ---- Hand-drawn bitmaps -------------------------------------------------------

def rows(*spec):
    """Glyph rows from patterns and (pattern, count) pairs."""
    out = []
    for item in spec:
        pattern, count = item if isinstance(item, tuple) else (item, 1)
        out += [pattern] * count
    return out


# Bold condensed capitals: 15 rows, 2 px strokes, ink only (the 1 px shadow is added
# by the layout). Drawn to the weight of the Japanese banners and of the legacy
# "CENTRAL PARK GUIDE" banner (2 px stroke, +1,+1 shadow).
BOLD_SOURCE_HEIGHT = 15
BOLD_HEIGHT = 17
BOLD_STRETCH_AT = (3, 10)   # rows of the 15-row drawings that are repeated once


def stretched(bitmap):
    """The 15-row drawing with rows 3 and 10 repeated: the stems get two rows taller, nothing is resampled."""
    out = []
    for n, row in enumerate(bitmap):
        out.append(row)
        if n in BOLD_STRETCH_AT:
            out.append(row)
    return out


BOLD_DRAWN = {
    'A': rows('.####.', '######', ('##..##', 5), ('######', 2), ('##..##', 6)),
    'B': rows('#####.', '######', ('##..##', 4), ('#####.', 2), ('##..##', 5), '######', '#####.'),
    'C': rows('.####.', '######', '##..##', ('##....', 9), '##..##', '######', '.####.'),
    'D': rows('#####.', '######', ('##..##', 11), '######', '#####.'),
    'E': rows(('#####', 2), ('##...', 4), ('####.', 2), ('##...', 5), ('#####', 2)),
    'F': rows(('#####', 2), ('##...', 4), ('####.', 2), ('##...', 7)),
    'G': rows('.####.', '######', '##..##', ('##....', 4), ('##.###', 2), ('##..##', 4), '######', '.####.'),
    'H': rows(('##..##', 6), ('######', 2), ('##..##', 7)),
    'I': rows(('####', 2), ('.##.', 11), ('####', 2)),
    'J': rows(('...##', 12), '##.##', '#####', '.###.'),
    'K': rows(('##..##', 2), ('##.##.', 2), ('####..', 2), ('###...', 2), ('####..', 2), ('##.##.', 2), ('##..##', 3)),
    'L': rows(('##...', 13), ('#####', 2)),
    'M': rows('##...##', '###.###', ('#######', 2), ('##.#.##', 2), ('##...##', 9)),
    'N': rows('##..##', ('###.##', 6), ('##.###', 7), '##..##'),
    'O': rows('.####.', '######', ('##..##', 11), '######', '.####.'),
    'P': rows('#####.', '######', ('##..##', 4), ('#####.', 2), ('##....', 7)),
    'Q': rows('.####..', '######.', ('##..##.', 10), '##.###.', '######.', '.######'),
    'R': rows('#####.', '######', ('##..##', 4), ('#####.', 2), ('##.##.', 2), ('##..##', 5)),
    'S': rows('.####.', '######', '##..##', ('##....', 3), '.####.', '..####', ('....##', 3), '##..##', '######', '.####.', '......')[:BOLD_SOURCE_HEIGHT],
    'T': rows(('######', 2), ('..##..', 13)),
    'U': rows(('##..##', 12), '######', '######', '.####.')[:BOLD_SOURCE_HEIGHT],
    'V': rows(('##..##', 9), ('.####.', 3), ('..##..', 3)),
    'W': rows(('##....##', 6), ('##.##.##', 5), ('########', 2), '###..###', '.##..##.')[:BOLD_SOURCE_HEIGHT],
    'X': rows(('##..##', 3), ('.####.', 3), ('..##..', 3), ('.####.', 3), ('##..##', 3)),
    'Y': rows(('##..##', 6), ('.####.', 3), ('..##..', 6)),
    'Z': rows(('######', 2), ('....##', 2), ('...##.', 2), ('..##..', 3), ('.##...', 2), ('##....', 2), ('######', 2)),
}
# Descender rows below the baseline (round 2, item 3): the Q's tail runs out of the bowl's
# bottom right and down two rows, so the word cannot read "AOUARIUM".
BOLD_DESCENDERS = {'Q': ['....###', '.....##']}
BOLD = {ch: stretched(bitmap) + BOLD_DESCENDERS.get(ch, []) for ch, bitmap in BOLD_DRAWN.items()}
# Round 2 bold small capitals: 6 rows, 2 px stems, 1-row bars (items 3 and 6: the season badges
# and the two-line cover titles). T is 4 wide so TWINBEE fits the cover's 40 px with its outline.
BOLD6_HEIGHT = 6
BOLD6 = {
    'A': ['.###.', '##.##', '##.##', '#####', '##.##', '##.##'],
    'B': ['####.', '##.##', '####.', '##.##', '##.##', '####.'],
    'E': ['####', '##..', '###.', '##..', '##..', '####'],
    'G': ['.####', '##...', '##...', '##.##', '##.##', '.####'],
    'I': ['##'] * 6,
    'L': ['##..'] * 5 + ['####'],
    'M': ['##...##', '###.###', '#######', '##.#.##', '##...##', '##...##'],
    'N': ['##..##', '###.##', '######', '##.###', '##..##', '##..##'],
    'O': ['.###.', '##.##', '##.##', '##.##', '##.##', '.###.'],
    'P': ['####.', '##.##', '##.##', '####.', '##...', '##...'],
    'R': ['####.', '##.##', '##.##', '####.', '##.##', '##.##'],
    'S': ['.####', '##...', '.###.', '...##', '...##', '####.'],
    'T': ['####'] + ['.##.'] * 5,
    'U': ['##.##'] * 5 + ['.###.'],
    'W': ['##...##', '##...##', '##.#.##', '##.#.##', '#######', '.##.##.'],
    'Z': ['#####', '...##', '..##.', '.##..', '##...', '#####'],
}
# Wide form for the season badges (2 px counters): the lines then span 38-44 of the badge's 56 px,
# as the Japanese numerals and 月号 fill it.
BOLD6_WIDE = {
    'A': ['.####.', '##..##', '##..##', '######', '##..##', '##..##'],
    'E': ['#####', '##...', '####.', '##...', '##...', '#####'],
    'G': ['.#####', '##....', '##....', '##.###', '##..##', '.#####'],
    'I': ['##'] * 6,
    'M': ['##....##', '###..###', '########', '##.##.##', '##....##', '##....##'],
    'N': ['##...##', '###..##', '####.##', '##.####', '##..###', '##...##'],
    'P': ['#####.', '##..##', '##..##', '#####.', '##....', '##....'],
    'R': ['#####.', '##..##', '##..##', '#####.', '##..##', '##..##'],
    'S': ['.#####', '##....', '.####.', '....##', '....##', '#####.'],
    'T': ['######'] + ['..##..'] * 5,
    'U': ['##..##'] * 5 + ['.####.'],
    'W': ['##....##', '##....##', '##.##.##', '##.##.##', '########', '.##..##.'],
}
# The 7-row form (cover 154, first line) repeats one stem row: row 4, or row 3 for W.
BOLD6_STRETCH = {'W': 3}
# Round 2 semi-bold capitals for the eight-letter cover titles (SUIKODEN, PARODIUS): 8 rows, 4 wide,
# a 2 px main stem and 1 px elsewhere (N is 5 wide for its diagonal). Eight 2 px-stem letters
# of 5 px would need 47 px; the logos have 40.
SEMI8_HEIGHT = 8
SEMI8 = {
    'A': ['.##.', '##.#', '##.#', '##.#', '####', '##.#', '##.#', '##.#'],
    'D': ['###.'] + ['##.#'] * 6 + ['###.'],
    'E': ['####', '##..', '##..', '###.', '##..', '##..', '##..', '####'],
    'I': ['##'] * 8,
    'K': ['##.#', '##.#', '###.', '##..', '###.', '##.#', '##.#', '##.#'],
    'N': ['##..#', '###.#', '###.#', '##.##', '##.##', '##..#', '##..#', '##..#'],
    'O': ['.##.'] + ['##.#'] * 6 + ['.##.'],
    'P': ['###.', '##.#', '##.#', '##.#', '###.', '##..', '##..', '##..'],
    'R': ['###.', '##.#', '##.#', '##.#', '###.', '##.#', '##.#', '##.#'],
    'S': ['.###', '##..', '##..', '.##.', '..##', '..##', '..##', '###.'],
    'U': ['##.#'] * 7 + ['.###'],
}
# The 10-row form (PARODIUS) repeats rows 1 and 5 of every semi-bold letter.
SEMI_STRETCH = (1, 5)


def stretch_rows(bitmap, repeat):
    """Rows of `bitmap` with the rows numbered in `repeat` drawn twice (no resampling)."""
    out = []
    for n, row in enumerate(bitmap):
        out += [row, row] if n in repeat else [row]
    return out


def small_bold_face(name, table, height, description, repeat=None):
    glyphs, provenance = {}, {}
    for ch, bitmap in table.items():
        if len(bitmap) != height or len({len(r) for r in bitmap}) != 1:
            raise ValueError('%s glyph %r is malformed' % (name, ch))
        drawn = stretch_rows(bitmap, repeat(ch)) if repeat else bitmap
        glyphs[ch] = base.bitmap_glyph(drawn)
        provenance[ch] = {'origin': 'port-made', 'description': description, 'bitmap': list(drawn)}
    return base.Face(name, glyphs, 3, {'style': description, 'rescaled': False, 'generated': False,
                                       'glyphs': provenance})


def round2_faces():
    return {
        'bold6': small_bold_face('bold6', BOLD6, BOLD6_HEIGHT, 'bold capital, 6 rows, 2 px stems, 1-row bars'),
        'bold6w': small_bold_face('bold6w', BOLD6_WIDE, BOLD6_HEIGHT, 'wide bold capital, 6 rows, 2 px stems, '
                                  '2 px counters, 1-row bars'),
        'bold7': small_bold_face('bold7', BOLD6, BOLD6_HEIGHT, 'bold capital, 7 rows (the 6-row drawing with one '
                                 'stem row repeated), 2 px stems', lambda ch: {BOLD6_STRETCH.get(ch, 4)}),
        'semi8': small_bold_face('semi8', SEMI8, SEMI8_HEIGHT, 'semi-bold capital, 8 rows, 2 px main stem'),
        'semi10': small_bold_face('semi10', SEMI8, SEMI8_HEIGHT, 'semi-bold capital, 10 rows (the 8-row drawing '
                                  'with rows 1 and 5 repeated), 2 px main stem', lambda ch: set(SEMI_STRETCH)),
    }


# Small-word additions in panel7 style (7 rows, 1 px stroke), port-made.
PANEL7_PERIOD = (['.', '.', '.', '.', '.', '.', '#'], 'period, 1 wide, on the baseline row')
# Narrow capital W (3 wide), used only where "Wrestling" cannot fit the 30 px (ink plus shadow) a 32 px tag leaves.
NARROW_W = (['#.#', '#.#', '#.#', '###', '###', '###', '#.#'], 'narrow cap W, 3 wide (panel7-style W is 5)')
# Order in which narrow forms are tried: W first (a modest change), r and t last.
NARROW_ORDER = 'WHoaebgns' + 'rt'
# Frame 144 (rounds 2 and 3). "Pro / Wrestling" cannot fit: two 7-row lines with shadows need 16 rows,
# and the 3-wide W read as H or N. The tag says "Wrestling" on one line with the 5-wide W, in the 7 rows
# plus shadow that "Baseball" (frame 145) uses. Round 3: the t is the family's t (centred 3 px crossbar,
# hooked foot), one row shorter than the l. The s has a flat one-pixel top, so its top-right corner is
# empty. That lets the t's left arm sit in the column after the s and stay 1 px clear of it (s-t kerned
# to 0 px). r-e is kerned to 0 px too: only the arm of the r meets the e, corner to corner. Every other
# letter is already at its narrowest form, so the word's ink is 30 px. The tag keeps 1 px clear on both
# sides, so the drop shadow is clipped at the right margin: the g loses the 4 shadow pixels of its right
# edge.
WRESTLING_T = (['...', '.#.', '###', '.#.', '.#.', '.#.', '..#'],
               't, 3 wide: the panel7 t (centred crossbar, hooked foot) one row shorter, so it does not read as l')
WRESTLING_S = (['...', '...', '.#.', '#..', '.#.', '..#', '##.'],
               'narrow s, 3 wide, flat one-pixel top so the t beside it keeps 1 px clear')
WRESTLING = {'text': 'Wrestling', 'narrow': 'reng', 'kern': {('r', 'e'): -1, ('s', 't'): -1}}


class KernedFace(base.Face):
    """A face with per-pair kerning (added to the 1 px gap between two glyphs)."""

    def __init__(self, name, glyphs, space, provenance, kern):
        super().__init__(name, glyphs, space, provenance)
        self.kern = dict(kern)

    def points(self, text):
        pts, x = [], 0
        for n, ch in enumerate(text):
            g = self.glyph(ch)
            pts += [(px + x, py) for px, py in g['points']]
            x += g['width'] + 1 + (self.kern.get((ch, text[n + 1]), 0) if n + 1 < len(text) else 0)
        return pts


def wrestling_face(faces):
    compact, narrow = faces['panel7-compact'], faces['panel7-narrow']
    glyphs, provenance = dict(compact.glyphs), dict(compact.provenance['glyphs'])
    for ch in WRESTLING['narrow']:
        glyphs[ch], provenance[ch] = narrow.glyphs[ch], narrow.provenance['glyphs'][ch]
    for ch, (rows_, description) in (('t', WRESTLING_T), ('s', WRESTLING_S)):
        glyphs[ch] = base.bitmap_glyph(rows_)
        provenance[ch] = {'origin': 'port-made-narrow', 'description': description, 'bitmap': list(rows_),
                          'replaces': provenance[ch]['origin']}
    kern = WRESTLING['kern']
    return KernedFace('panel7-compact+narrow-%s+st+kern' % WRESTLING['narrow'], glyphs, compact.space,
                      {**compact.provenance, 'glyphs': provenance,
                       'kerning': {a + b: k for (a, b), k in sorted(kern.items())}}, kern)


def check_bitmaps():
    for ch, bitmap in BOLD_DRAWN.items():
        if len(bitmap) != BOLD_SOURCE_HEIGHT:
            raise ValueError('bold drawing %r is malformed (%d rows)' % (ch, len(bitmap)))
    for ch, bitmap in BOLD.items():
        if len(bitmap) != BOLD_HEIGHT + len(BOLD_DESCENDERS.get(ch, ())) or len({len(r) for r in bitmap}) != 1:
            raise ValueError('bold glyph %r is malformed (%d rows)' % (ch, len(bitmap)))


def bold_face():
    check_bitmaps()
    glyphs, provenance = {}, {}
    for ch, bitmap in BOLD.items():
        glyphs[ch] = base.bitmap_glyph(bitmap)
        provenance[ch] = {'origin': 'port-made', 'description': 'bold condensed capital, 17 rows (15-row drawing, stems two rows taller), 2 px strokes',
                          'bitmap': list(bitmap)}
    return base.Face('bold17', glyphs, 4, {
        'style': 'bold condensed capitals, 17 rows, 2 px strokes, ink plus (+1,+1) drop shadow, 1 blank column '
                 '(taken by the shadow) between glyphs',
        'rescaled': False, 'generated': False, 'glyphs': provenance})


def small_faces(panel7):
    """panel7-compact/narrow faces plus the period used by the stat labels."""
    faces = base.port_made_faces(panel7)
    for name in ('panel7-compact', 'panel7-narrow'):
        face = faces[name]
        rows_, description = PANEL7_PERIOD
        face.glyphs['.'] = base.bitmap_glyph(rows_)
        face.provenance['glyphs']['.'] = {'origin': 'port-made', 'description': description,
                                          'bitmap': list(rows_), 'replaces': None}
    narrow = faces['panel7-narrow']
    narrow.glyphs['W'] = base.bitmap_glyph(NARROW_W[0])
    narrow.provenance['glyphs']['W'] = {'origin': 'port-made-narrow', 'description': NARROW_W[1],
                                        'bitmap': list(NARROW_W[0]), 'replaces': 'port-made'}
    return faces


def fit_face(faces, lines, width, shadow=None):
    """translation_mainrom_drafts.fit_face with this tool's narrowing order (W added)."""
    compact, narrow = faces['panel7-compact'], faces['panel7-narrow']
    chosen = []
    while True:
        glyphs, provenance = dict(compact.glyphs), dict(compact.provenance['glyphs'])
        for ch in chosen:
            glyphs[ch], provenance[ch] = narrow.glyphs[ch], narrow.provenance['glyphs'][ch]
        face = base.Face(compact.name + ('+narrow-' + ''.join(chosen) if chosen else ''), glyphs, compact.space,
                         {**compact.provenance, 'glyphs': provenance})
        wide = [line for line in lines if base.line_width(face, line, shadow) > width]
        if not wide:
            return face
        options = [ch for ch in NARROW_ORDER if ch not in chosen and ch in narrow.glyphs and any(ch in line for line in wide)]
        if not options:
            raise ValueError('%r does not fit %dpx even with narrow forms' % (wide, width))
        chosen.append(options[0])


# ---- Layout -----------------------------------------------------------------

def extent(face, text):
    pts = face.points(text)
    xs, ys = [x for x, _ in pts], [y for _, y in pts]
    return min(xs), max(xs), min(ys), max(ys)


def place(frame, face, text, rect, ink, clut_parts, shadow=None, outline=None, ramp_table=None,
          align='center', margin=0, top=None, tones=None, report=None, clip_shadow=False, **target):
    """Draw `text` into rect and return the written bbox.

    align 'center' centres ink plus shadow/outline; 'left' puts the lettering's left
    edge `margin` px from the rect. `top` is the frame row of the first ink row (the
    default centres vertically). The lettering plus its shadow must keep `margin` px
    from the left and right edges of the rect; with `clip_shadow` only the ink must, and
    shadow pixels inside the right margin are dropped (counted in report['clipped_shadow']).
    """
    left, right, up, down = extent(face, text)
    ml = mt = 1 if outline is not None else 0
    mr = mb = 1 if (outline is not None or shadow is not None) else 0
    full_w = right - left + 1 + ml + (0 if clip_shadow and outline is None else mr)
    full_h = down - up + 1 + mt + mb
    x0, y0, w, h = rect
    if full_w > w - 2 * margin:
        raise ValueError('lettering %r (%d px) does not fit %r with margin %d' % (text, full_w, rect, margin))
    if align == 'center':
        ox = x0 + (w - full_w) // 2 + ml - left
    elif align == 'left':
        ox = x0 + margin + ml - left
    else:
        raise ValueError('unknown alignment ' + align)
    oy = (top if top is not None else y0 + (h - full_h) // 2 + mt) - up
    ink_px = {(ox + x, oy + y) for x, y in face.points(text)}
    writes = {}
    if outline is not None:
        for x, y in ink_px:
            for dx in (-1, 0, 1):
                for dy in (-1, 0, 1):
                    writes[(x + dx, y + dy)] = outline
    if shadow is not None:
        for x, y in ink_px:
            if clip_shadow and x + 1 >= x0 + w - margin:
                if report is not None and (x + 1, y + 1) not in ink_px:
                    report['clipped_shadow'] = report.get('clipped_shadow', 0) + 1
                continue
            writes[(x + 1, y + 1)] = shadow
    for x, y in ink_px:
        writes[(x, y)] = ramp_table[y] if ink == 'ramp' else ink
    if tones is not None:
        shadow_px = {p for p, i in writes.items() if p not in ink_px}
        for p, role in soft_edges(ink_px, shadow_px).items():
            if 0 <= p[0] < frame.width and 0 <= p[1] < frame.height and frame.covering(*p):
                writes[p] = tones[role]
                if report is not None:
                    report[role] = report.get(role, 0) + 1
    for (x, y), index in sorted(writes.items(), key=lambda kv: (kv[0][1], kv[0][0])):
        base.put(frame, x, y, index, clut_parts, **target)
    xs, ys = [x for x, _ in writes], [y for _, y in writes]
    return [min(xs), min(ys), max(xs) - min(xs) + 1, max(ys) - min(ys) + 1]


# The Japanese headlines carry three edge tones besides fill and shadow: a light one on the upper
# edges of slanted strokes, a mid one where fill turns into shadow, and a soft one outside the
# shadow's steps. In every banner CLUT they sit next to the fill and shadow: highlight = fill - 1,
# mid = fill + 1, soft = shadow + 1 (checked against the source lettering by headline_tones).
TONE_OFFSETS = {'highlight': ('ink', -1), 'mid': ('ink', 1), 'soft': ('shadow', 1)}


def soft_edges(ink_px, shadow_px):
    """{pixel: role} for the clear notches of a drawn word: (round 2, item 7)

    * highlight: a notch with ink below and on one side (the cut top corners of round letters,
      the upper steps of slanted strokes);
    * mid: a notch with ink above and to the right (cut bottom-left corners, lower steps);
    * soft: a notch with shadow above and to the left (the steps of the shadow).
    A notch has the three neighbours of one corner set (two sides and the diagonal between
    them) and no other fill around it, so counters and gaps between letters are never filled.
    """
    out = {}
    taken = ink_px | shadow_px
    xs = [x for x, _ in taken]
    ys = [y for _, y in taken]
    corners = {'highlight': [((0, 1), (1, 0)), ((0, 1), (-1, 0))], 'mid': [((0, -1), (1, 0))],
               'soft': [((0, -1), (-1, 0))]}
    for y in range(min(ys) - 1, max(ys) + 2):
        for x in range(min(xs) - 1, max(xs) + 2):
            if (x, y) in taken:
                continue
            for role, pairs in corners.items():
                cells = shadow_px if role == 'soft' else ink_px
                for (ax, ay), (bx, by) in pairs:
                    corner = {(x + ax, y + ay), (x + bx, y + by), (x + ax + bx, y + ay + by)}
                    near = {(x + dx, y + dy) for dx in (-1, 0, 1) for dy in (-1, 0, 1) if dx or dy}
                    # the other five neighbours hold no fill (a neighbouring letter's shadow may touch)
                    if corner <= cells and not (near - corner) & (ink_px if role != 'soft' else taken):
                        out[(x, y)] = role
    return out


def source_ink(frame, rect):
    """(ink, shadow, pixels) of a lettering region with a (+1,+1) drop shadow, as the source draws it."""
    pixels = base.lettering(frame, rect)
    ink, shadow = base.dominant(pixels, 2)
    return ink, shadow, pixels


# ---- Plans ------------------------------------------------------------------

# Item 2: one wording pattern for the whole headline family.
HEADLINES = {
    126: ('CENTRAL PARK GUIDE', '中央公園紹介', 'Guide'),
    127: ('ART MUSEUM OPENS', '美術館完成', 'Opens'),
    128: ('AQUARIUM SPECIAL', '水族館特集', 'Special'),
    129: ('STADIUM OPENS', 'スタジアム完成', 'Opens'),
    130: ('GARDEN SPECIAL', '植物園特集', 'Special'),
    131: ('BOWLING SPECIAL', 'ボーリング特集', 'Special'),
    132: ('KARAOKE BOX OPENS', 'カラオケBOX完成', 'Opens'),
    133: ('PLANETARIUM SPECIAL', 'プラネタリウム特集', 'Special'),
}


# Round 2 (owner decision, 2026-11-01): headlines are flush left, starting at the column where the
# Japanese starts in the same banner. The ink keeps the vertical position of round 1 (centred on the
# 17 cap rows, so a descender such as the Q's tail hangs below without moving the baseline).
HEADLINE_ALIGN = 'left'


def headline_tones(source, ink, shadow):
    """{role: index} of the source's edge tones (see TONE_OFFSETS); each must be in the source lettering."""
    used = {i for _, i in source.values()}
    tones = {}
    for role, (anchor, offset) in TONE_OFFSETS.items():
        index = (ink if anchor == 'ink' else shadow) + offset
        if index not in used or index in (ink, shadow):
            raise ValueError('edge tone %s (index %d) is not in the source lettering' % (role, index))
        tones[role] = index
    return tones


def plan_headline(frame, face):
    text = HEADLINES[frame.frame][0]
    rect = [0, 0, frame.width, frame.height]
    ink, shadow, source = source_ink(frame, rect)
    expected = base.CATALOG_INK.get(frame.frame)
    if expected and expected != (ink, shadow):
        raise ValueError('derived ink/shadow %r differs from catalog %r: frame %d' % ((ink, shadow), expected, frame.frame))
    tones = headline_tones(source, ink, shadow)
    left = min(x for x, _ in source)
    top = (frame.height - (BOLD_HEIGHT + 1)) // 2
    base.clear(frame, rect)
    counts = {}
    box = place(frame, face, text, [left, 0, frame.width - left, frame.height], ink, base.source_parts(source),
                shadow=shadow, align=HEADLINE_ALIGN, top=top, tones=tones, report=counts)
    return {'item': 2, 'text': text, 'font': 'bold17', 'ink': ink, 'shadow': shadow, 'edge_tones': tones,
            'edge_tone_pixels': dict(sorted(counts.items())),
            'alignment': HEADLINE_ALIGN, 'japanese_left': left, 'cap_top': top, 'edit_regions': [rect],
            'lettering_bbox': box, 'wording_rule': HEADLINES[frame.frame][1:]}


# Item 3: issue banners. Round 2 (owner decision, 2026-11-01): the three-month runs are named by
# season, in two bold lines (season over ISSUE) that fill the badge's rows 1-14 like the Japanese
# numerals. One size for all four; SUMMER and AUTUMN are the widest lines.
ISSUE_SEASONS = {122: 'SPRING', 123: 'SUMMER', 124: 'AUTUMN', 125: 'WINTER'}
ISSUE_MONTHS = {122: '3-4-5', 123: '6-7-8', 124: '9-10-11', 125: '12-1-2'}
ISSUE_WORD = 'ISSUE'
ISSUE_ROWS = (1, 9)   # first ink row of each 6-row line; rows 7-8 are the gap


def line_ramp(table, first, last, height):
    """The source's vertical ramp (rows first..last) squeezed onto a line of `height` rows."""
    span = last - first
    return [table[first + round(k * span / (height - 1))] for k in range(height)]


def plan_issue(frame, face):
    rect = [0, 0, frame.width, frame.height]
    source = base.lettering(frame, rect)
    table = base.ramp(source, range(frame.height))
    first = min(y for _, y in source)
    last = max(y for _, y in source)
    parts = base.source_parts(source)
    base.clear(frame, rect)
    lines = (ISSUE_SEASONS[frame.frame], ISSUE_WORD)
    boxes, ramps = [], []
    for text, top in zip(lines, ISSUE_ROWS):
        colours = line_ramp(table, first, last, BOLD6_HEIGHT)
        ramps.append(colours)
        boxes.append(place(frame, face, text, rect, 'ramp', parts, top=top,
                           ramp_table={top + k: c for k, c in enumerate(colours)}))
    return {'item': 3, 'text': ' '.join(lines), 'lines': list(lines), 'months': ISSUE_MONTHS[frame.frame],
            'font': face.name, 'ink': 'ramp', 'line_ramps': ramps, 'line_tops': list(ISSUE_ROWS),
            'edit_regions': [rect], 'lettering_bbox': boxes,
            'port_made_elements': [{'kind': 'glyph', 'face': face.name, 'char': c, 'origin': 'port-made',
                                    'description': face.provenance['glyphs'][c]['description']}
                                   for c in sorted(set(''.join(lines)))]}


# Item 4: genre tags.
TAG_TEXT = {**base.PORT_MADE_TAGS, 136: ('Action',), 137: ('Suspense',), 140: ('Classical',),
            144: (WRESTLING['text'],)}


def plan_tag(frame, faces):
    lines = TAG_TEXT[frame.frame]
    rect = [0, 0, frame.width, frame.height]
    source = base.lettering(frame, rect)
    ink, shadow = base.ink_and_shadow(source)
    clip = lines == (WRESTLING['text'],)
    if clip:
        face = wrestling_face(faces)
        if base.line_width(face, lines[0], None) > frame.width - 2 * TAG_MARGIN:
            raise ValueError('Wrestling does not fit frame %d' % frame.frame)
    else:
        face = fit_face(faces, lines, frame.width - 2 * TAG_MARGIN, shadow)
    base.clear(frame, rect)
    line_h = frame.height // len(lines)
    report = {}
    boxes = [place(frame, face, text, [0, n * line_h, frame.width, line_h], ink, base.source_parts(source),
                   shadow=shadow, align=TAG_ALIGN, margin=TAG_MARGIN, clip_shadow=clip, report=report)
             for n, text in enumerate(lines)]
    elements = base.used_port_made(face, lines)
    plan = {'item': 4, 'text': ' / '.join(lines), 'lines': list(lines), 'font': face.name, 'ink': ink, 'shadow': shadow,
            'alignment': TAG_ALIGN, 'margin': TAG_MARGIN, 'edit_regions': [rect], 'lettering_bbox': boxes,
            'port_made_elements': elements}
    if clip:
        plan['clipped_shadow_pixels'] = report.get('clipped_shadow', 0)
    return plan


# Item 1: Save labels. Part-local rows of the lettering layer (the Japanese spans 2-12).
SAVE_BAND = (2, 12)


def plan_save(frame, face, card_frame):
    card, letters = frame.parts[0], frame.parts[1]
    if card['texels'] != card_frame.parts[0]['texels']:
        raise ValueError('frame %d part 0 is not the frame-56 card' % frame.frame)
    region = [letters['x'], letters['y'], letters['w'], letters['h']]
    source = {k: v for k, v in base.lettering(frame, region).items() if v[0] == 1}
    outline, ink = base.dominant(source, 2)
    for row in letters['texels']:
        row[:] = [CLEAR] * len(row)
    lo, hi = SAVE_BAND
    left, right, up, down = extent(face, 'Save')
    height = down - up + 3  # ink rows plus the outline above and below
    top = letters['y'] + hi - height + 1 + 1  # outline bottom row on the band's last row
    occluded = []
    box = place(frame, face, 'Save', [letters['x'], letters['y'], letters['w'], letters['h']], ink, {1},
                outline=outline, top=top, part=1, occluders=(0,), occluded=occluded)
    return {'item': 1, 'text': 'Save', 'font': face.name, 'ink': ink, 'outline': outline,
            'bounce_dy': letters['y'] - card['y'], 'written_part': 1, 'band_part_local_rows': list(SAVE_BAND),
            'hidden_under_card': {'ink': sum(1 for *_, i in occluded if i == ink),
                                  'outline': sum(1 for *_, i in occluded if i != ink)},
            'edit_regions': [region], 'lettering_bbox': box}


# Item 6: stat labels (frame 61). Label strips are parts 25-27 (32x40 each, three 12-row labels at a 14 row pitch).
STATS = {25: ('Health', 'Art', 'Looks'), 26: ('Hum.', 'Sports', 'Guts'), 27: ('Science', 'Trivia', 'Stress')}
STAT_NAMES = ('Health', 'Humanities', 'Science', 'Art', 'Sports', 'Trivia', 'Looks', 'Guts', 'Stress')
STAT_MAX_WIDTH = 26  # strip columns kept clear of the arrow marker (it starts at strip column 29; 3 px gap)


def plan_stats(frame, faces):
    ink = 9
    labels = [t for column in STATS.values() for t in column]
    face = fit_face(faces, labels, STAT_MAX_WIDTH)
    regions, boxes = [], []
    source_parts = set(STATS)
    for n, column in STATS.items():
        strip = frame.parts[n]
        for row in strip['texels']:
            row[:] = [CLEAR] * len(row)
        regions.append([strip['x'], strip['y'], strip['w'], strip['h']])
        for k, text in enumerate(column):
            rect = [strip['x'], strip['y'] + k * 14, STAT_MAX_WIDTH, 12]
            boxes.append(place(frame, face, text, rect, ink, source_parts, align='left', part=n))
    return {'item': 6, 'text': ' / '.join(labels), 'font': face.name, 'ink': ink, 'shadow': None, 'edit_regions': regions,
            'lettering_bbox': boxes, 'alignment': 'left', 'port_made_elements': base.used_port_made(face, labels)}


# Item 7: game covers 151-154. The titles are small multi-colour pixel logos (40x16, frame x 8-47);
# they are re-lettered with the logo's own indices. Round 2 (owner review, 2026-11-01): bolder
# letters, close to each logo's weight; 0153 says POWER PROS (owner decision) on two lines, like
# TWINBEE / PUZZLE; PARODIUS keeps the logo's yellow and white; no letter may touch the artwork
# that stays (the 0153 baseball): every written pixel keeps one clear pixel from it.
COVER_X = (8, 40)   # title columns of every cover logo


def cover_line(text, face, top, fill, outline=None, shadow=None, left=None):
    """One title line. fill: an index, 'ramp' (the source's row ramp), ('rows', [index per ink row])
    or ('letters', [index per letter]). left: first ink column; default centres the line (with its
    outline and shadow) on the logo columns."""
    return {'text': text, 'face': face, 'top': top, 'fill': fill, 'outline': outline, 'shadow': shadow, 'left': left}


PARODIUS_FILL = ('rows', [8, 8, 8, 7, 7, 6, 6, 5, 5, 4])   # yellow to orange, like the logo's band
COVERS = {
    151: {'title': 'SUIKODEN', 'clear': [[8, 0, 40, 10]],
          'lines': [cover_line('SUIKODEN', 'semi8', 1, 'ramp')]},
    152: {'title': 'PARODIUS', 'clear': [[8, 0, 40, 16]],
          'lines': [cover_line('PARODIUS', 'semi10', 2, PARODIUS_FILL, outline=15, shadow=2)]},
    153: {'title': 'POWER PROS', 'clear': [[8, 0, 32, 16], [40, 0, 8, 10], [45, 10, 3, 1]],
          'lines': [cover_line('POWER', 'bold6', 1, 15, outline=11, shadow=2, left=9),
                    cover_line('PROS', 'bold6', 8, 15, outline=11, shadow=2, left=9)]},
    154: {'title': 'TWINBEE PUZZLE', 'clear': [[8, 0, 40, 16]],
          'lines': [cover_line('TWINBEE', 'bold7', 1, ('letters', [15, 14, 13, 12, 15, 14, 13]), outline=1),
                    cover_line('PUZZLE', 'bold6', 9, 8, outline=1)]},
}
ARTWORK_GAP = 1   # clear pixels between new lettering and any kept artwork pixel of the title rows


def layout_line(face, line):
    """{(x, y): letter number} of the line's ink."""
    letters, x = {}, 0
    for n, ch in enumerate(line['text']):
        g = face.glyph(ch)
        for gx, gy in g['points']:
            letters[(x + gx, line['top'] + gy)] = n
        x += g['width'] + 1
    width = x - 1
    if line['left'] is not None:
        dx = line['left']
    else:
        rim = (1 if line['outline'] is not None else 0)
        full = width + 2 * rim + (1 if line['shadow'] is not None else 0)
        dx = COVER_X[0] + (COVER_X[1] - full) // 2 + rim
    return {(x_ + dx, y_): n for (x_, y_), n in letters.items()}


def plan_cover(frame, faces):
    spec = COVERS[frame.frame]
    source = {}
    for rect in spec['clear']:
        source.update(base.lettering(frame, rect))
    title_rows = [COVER_X[0], 0, COVER_X[1], 16]
    kept = {p for p in base.lettering(frame, title_rows) if p not in source}
    table = base.ramp(source, range(frame.height))
    parts = base.source_parts(source)
    for rect in spec['clear']:
        base.clear(frame, rect)
    layers = {'shadow': {}, 'outline': {}, 'ink': {}}
    boxes = []
    for line in spec['lines']:
        ink = layout_line(faces[line['face']], line)
        rim = set()
        if line['outline'] is not None:
            rim = {(x + dx, y + dy) for x, y in ink for dx in (-1, 0, 1) for dy in (-1, 0, 1)} - set(ink)
            layers['outline'].update({p: line['outline'] for p in rim})
        if line['shadow'] is not None:
            body = set(ink) | rim
            layers['shadow'].update({(x + 1, y + 1): line['shadow'] for x, y in body if (x + 1, y + 1) not in body})
        fill = line['fill']
        top = line['top']
        for (x, y), n in ink.items():
            if fill == 'ramp':
                layers['ink'][(x, y)] = table[y]
            elif isinstance(fill, tuple) and fill[0] == 'rows':
                layers['ink'][(x, y)] = fill[1][y - top]
            elif isinstance(fill, tuple) and fill[0] == 'letters':
                layers['ink'][(x, y)] = fill[1][n % len(fill[1])]
            else:
                layers['ink'][(x, y)] = fill
        drawn = set(ink) | rim | ({(x + 1, y + 1) for x, y in set(ink) | rim} if line['shadow'] is not None else set())
        xs, ys = [x for x, _ in drawn], [y for _, y in drawn]
        boxes.append([min(xs), min(ys), max(xs) - min(xs) + 1, max(ys) - min(ys) + 1])
    writes = {**layers['shadow'], **layers['outline'], **layers['ink']}
    near = {(x + dx, y + dy) for x, y in kept for dx in range(-ARTWORK_GAP, ARTWORK_GAP + 1)
            for dy in range(-ARTWORK_GAP, ARTWORK_GAP + 1)}
    if near & set(writes):
        raise ValueError('cover %d title touches kept artwork at %r' % (frame.frame, sorted(near & set(writes))[:4]))
    if any(not any(r[0] <= x < r[0] + r[2] and r[1] <= y < r[1] + r[3] for r in spec['clear']) for x, y in writes):
        raise ValueError('cover %d title leaves its cleared rects' % frame.frame)
    for (x, y), index in sorted(writes.items(), key=lambda kv: (kv[0][1], kv[0][0])):
        base.put(frame, x, y, index, parts)
    used = {line['face']: set(line['text']) for line in spec['lines']}
    elements = [{'kind': 'glyph', 'face': name, 'char': c, 'origin': 'port-made',
                 'description': faces[name].provenance['glyphs'][c]['description']}
                for name in sorted(used) for c in sorted(used[name])]
    return {'item': 7, 'text': spec['title'], 'lines': [l['text'] for l in spec['lines']],
            'font': '/'.join(sorted(used)), 'ink': 'logo indices',
            'line_styles': [{k: l[k] for k in ('text', 'face', 'top', 'fill', 'outline', 'shadow', 'left')}
                            for l in spec['lines']],
            'edit_regions': [list(r) for r in spec['clear']], 'lettering_bbox': boxes,
            'artwork_gap': ARTWORK_GAP, 'port_made_elements': elements,
            'logo': 'cover art outside the title rows kept unchanged; bottom strip not touched'}


# ---- Build ------------------------------------------------------------------

FRAMES = {'save': (57, 58, 59), 'headline': tuple(sorted(HEADLINES)), 'issue': tuple(sorted(ISSUE_SEASONS)),
          'tag': tuple(sorted(TAG_TEXT)), 'stats': (61,)}


def compose(frames, panel7, native=None):
    """{frame: (edited Frame, plan)} for every frame this tool redraws (no covers)."""
    small = small_faces(panel7)
    bold, round2 = bold_face(), round2_faces()
    out = {}
    for f in FRAMES['headline']:
        model = frames[f].copy()
        out[f] = (model, plan_headline(model, bold))
    for f in FRAMES['issue']:
        model = frames[f].copy()
        out[f] = (model, plan_issue(model, round2['bold6w']))
    for f in FRAMES['tag']:
        model = frames[f].copy()
        out[f] = (model, plan_tag(model, small))
    for f in sorted(COVERS):
        model = frames[f].copy()
        # The bottom strip keeps its English ("Out in '95" / "Out Now!") exactly as the T-39009 draft had it.
        strip = base.plan_bar(model, native, strip_top=50)
        plan = plan_cover(model, round2)
        plan['edit_regions'] = strip['edit_regions'] + plan['edit_regions']
        plan['strip'] = {'text': strip['text'], 'face': 'native', 'outline': strip['outline'],
                         'lettering_bbox': strip['lettering_bbox'], 'source': 'unchanged from T-39009 (translation_mainrom_drafts.plan_bar)'}
        plan['text'] = plan['text'] + ' / ' + strip['text']
        out[f] = (model, plan)
    model = frames[61].copy()
    out[61] = (model, plan_stats(model, small))
    for f in FRAMES['save']:
        model = frames[f].copy()
        out[f] = (model, plan_save(model, small['panel7-compact'], frames[56]))
    wrestling = wrestling_face(small)
    return out, {'bold17': bold, **round2, **small, wrestling.name: wrestling}


def rows_for(f, model, plan, original_frame, hashes, sources, users):
    """One output row per palette variant (never shared between variants), checked against the source."""
    scope = base.isolated_scope(f, model, original_frame, users)
    out = []
    for v in VARIANTS:
        original = Image.open(sources / ('mainrom.frame-%03d.%s.png' % (f, v))).convert('RGBA')
        image = model.render(v)
        base.check(original, image, plan['edit_regions'], 'frame %d %s' % (f, v))
        out.append((image, original, {
            'id': 'mainrom-fix-%04d.%s' % (f, v), 'frame': f, 'palette_id': v, 'source_container': base.CONTAINER,
            'source_container_sha256': base.MAINROM_SHA, 'source_png_sha256': hashes[(f, v)],
            'dimensions': [model.width, model.height], 'output': 'mainrom-frame-%04d-%s.png' % (f, v),
            'runtime_variant_scope': base.applicability(f, [v], model), 'style': 'unreviewed',
            'human_approved': False, 'runtime_integrated': False, 'in_game_verified': False,
            'port_made': True, 'sheet_scope': scope, **plan}))
    return out


def load(sources, pack_path):
    frames = base.load_frames(sources, pack_path)
    hashes = base.gate(frames, sources)
    records = json.loads((sources / 'manifest.json').read_text())['images']
    frames[56] = base.Frame(56, {(i['frame_index'], i['variant']): i for i in records}, pack_path.read_bytes())
    users = {r['frame_index']: base.record_keys(r) for r in records if r['variant'] == 'base'}
    return frames, hashes, users


# ---- Build, ledger and register note ------------------------------------------

CHECKPOINTS = ROOT / 'translations/en/graphics/artwork/checkpoints'
INVENTORY = ROOT / 'translations/en/graphics/translation_inventory.json'
REVISION_LEDGERS = {
    'mainrom-indexed-drafts-v1': 'translations/en/graphics/artwork/checkpoints/T39004-pass3/ledger.json',
    'legacy-rebind-v1': 'translations/en/graphics/artwork/checkpoints/T39004-pass2/ledger.json',
    'mainrom-port-made-drafts-v1': 'translations/en/graphics/artwork/checkpoints/T39004-mainrom-port-made-v1/ledger.json',
}
DATE_PANEL_LEDGER = CHECKPOINTS / 'T39004-date-panel-v1/ledger.json'
WEEKDAY_FRAMES = tuple(range(82, 89))


def superseded_candidates(inventory_path=INVENTORY):
    """Every earlier candidate of the redrawn items, from the committed inventory (read only).

    Returns {frame: [ {id, revision, ledger, output_sha256, palette_ids} ]}. palette_ids come from
    the candidate's own ledger entry (shared source images list every variant they cover).
    """
    inventory = json.loads(inventory_path.read_text())
    items = {i['id']: i for i in inventory['artwork_items']}
    ledgers = {}
    out = {}
    own = 'translations/en/graphics/artwork/checkpoints/%s/ledger.json' % CHECKPOINT
    for f in sorted(set(sum(FRAMES.values(), ())) | set(COVERS)):
        item = items.get('mainrom-%04d' % f)
        if item is None:
            continue
        rows_ = []
        # Once this checkpoint is registered, the inventory lists the candidates it replaced as
        # superseded records (superseded_by this ledger) instead of as candidates of the item.
        replaced = [r for r in inventory.get('superseded_records', [])
                    if r['atom'] == item['id'] and r.get('superseded_by') == own]
        for c in item['english']['candidates'] + replaced:
            if c['revision'] == REVISION:
                continue    # this checkpoint's own rows, once it is registered in the inventory
            ledger_path = REVISION_LEDGERS.get(c['revision'])
            if ledger_path is None:
                raise ValueError('candidate %s of frame %d has an unknown revision %s' % (c['id'], f, c['revision']))
            if ledger_path not in ledgers:
                ledgers[ledger_path] = {a['id']: a for a in json.loads((ROOT / ledger_path).read_text())['assets']}
            entry = ledgers[ledger_path][c['id']]
            palettes = entry.get('palette_ids') or [entry['palette_id']]
            rows_.append({'id': c['id'], 'revision': c['revision'], 'ledger': ledger_path,
                          'output_sha256': c['output_sha256'], 'palette_ids': palettes})
        out[f] = rows_
    return out


def weekday_resolution(inventory_path=INVENTORY, date_ledger=DATE_PANEL_LEDGER, sources=None):
    """Item 5: every variant of frames 82-88 resolves to the approved two-letter file.

    The approved file of a variant is the T39004-date-panel-v1 asset whose palette_ids hold it
    (base and icon-52 share one source image and one file). The stale three-letter drafts are
    the batch-002 historical assets still listed on the items; they must be marked superseded.
    """
    inventory = json.loads(inventory_path.read_text())
    items = {i['id']: i for i in inventory['artwork_items']}
    ledger = json.loads(date_ledger.read_text())
    two = {}
    for a in ledger['assets']:
        if a['id'].startswith('mainrom-weekday-two-letter-'):
            for v in a['palette_ids']:
                two[(a['frame'], v)] = a
    resolution, stale = {}, []
    for f in WEEKDAY_FRAMES:
        item = items['mainrom-%04d' % f]
        for v in VARIANTS:
            a = two.get((f, v))
            if a is None:
                raise ValueError('no approved two-letter weekday for frame %d %s' % (f, v))
            if item['source_png_sha256'][v] != a['source_png_sha256']:
                raise ValueError('approved weekday is bound to another source image: frame %d %s' % (f, v))
            path = date_ledger.parent / a['path']
            if digest(path.read_bytes()) != a['output_sha256']:
                raise ValueError('approved weekday changed: ' + a['id'])
            resolution['%04d.%s' % (f, v)] = {'id': a['id'], 'text': a['text'], 'path': str(path.relative_to(ROOT)),
                                              'output_sha256': a['output_sha256'], 'human_approved': a['human_approved']}
        for hid in item['english']['historical_assets']:
            stale.append({'frame': f, 'id': hid})
        # After registration the inventory lists them as superseded snapshot assets instead.
        for rec in inventory.get('superseded_records', []):
            if rec['atom'] == item['id'] and rec['ledger'].startswith('snapshot:') \
                    and rec['superseded_by'] == str(date_ledger.relative_to(ROOT)):
                stale.append({'frame': f, 'id': rec['id']})
    return {'rule': 'two-letter set (Mo Tu We Th Fr Sa Su), approved 2026-10-30 (T39004-date-panel-v1)',
            'resolves_to': resolution, 'stale_three_letter_to_supersede': stale,
            'stale_ledger_note': 'ids are batch-002 historical assets (translations/en/graphics/artwork/manifest.json, '
                                 'approved three-letter art); mark each superseded_by T39004-date-panel-v1',
            'already_superseded': 'the pass2/pass3 weekday drafts are already in inventory superseded_records'}


def build(sources, pack_path, executable, output):
    frames, hashes, users = load(sources, pack_path)
    panel7 = base.panel7_face()
    native = base.NativeFace(executable)
    drafted, faces = compose(frames, panel7, native)
    output = new_output(output)
    prepared = []
    for f, (model, plan) in sorted(drafted.items()):
        prepared += rows_for(f, model, plan, frames[f], hashes, sources, users)
    old = superseded_candidates()
    for image, original, row in prepared:
        row['supersedes'] = [c['id'] for c in old.get(row['frame'], []) if row['palette_id'] in c['palette_ids']]
    (output / REVISION).mkdir(parents=True)
    for image, _, row in prepared:
        image.save(output / REVISION / row['output'])
        row['output_sha256'] = digest((output / REVISION / row['output']).read_bytes())
    same = {}
    for _, _, row in prepared:
        same.setdefault((row['frame'], row['output_sha256']), []).append(row['palette_id'])
    manifest = {'schema': 'tokimemo.mainrom-fixes.v1', 'ticket': TICKET, 'human_approved': False,
                'source_container': base.CONTAINER, 'source_container_sha256': base.MAINROM_SHA,
                'decode_gate': {'frames': list(base.GATED_FRAMES), 'variants': list(VARIANTS),
                                'renders_byte_identical': len(hashes)},
                'tag_alignment': {'mode': TAG_ALIGN, 'margin': TAG_MARGIN},
                'faces': {name: face.proof() for name, face in sorted(faces.items())},
                'identical_variant_outputs': {'%04d' % f: v for (f, _), v in sorted(same.items()) if len(v) > 1},
                'assets': [row for _, _, row in prepared]}
    (output / REVISION / 'manifest.json').write_bytes(encoded(manifest) + b'\n')
    return {'output': str(output), 'assets': len(prepared), 'frames': len(drafted)}, prepared, old


def previews(prepared, destination, zoom=8):
    """Original | English at `zoom`x per frame, one row per variant (local review aid, never committed)."""
    destination.mkdir(parents=True, exist_ok=True)
    by_frame = {}
    for image, original, row in prepared:
        by_frame.setdefault(row['frame'], []).append((row['palette_id'], original, image))
    gap = 2 * zoom
    for f, items in sorted(by_frame.items()):
        w, h = items[0][1].size
        sheet = Image.new('RGBA', (2 * w * zoom + 3 * gap, len(items) * (h * zoom + gap) + gap), (64, 64, 96, 255))
        y = gap
        for _, original, image in items:
            size = (w * zoom, h * zoom)
            sheet.alpha_composite(original.resize(size, Image.NEAREST), (gap, y))
            sheet.alpha_composite(image.resize(size, Image.NEAREST), (2 * gap + w * zoom, y))
            y += h * zoom + gap
        sheet.save(destination / ('mainrom-%04d-original-english.png' % f))
    return len(by_frame)


def ledger_for(built, prepared, old, weekday):
    manifest_bytes = (built / REVISION / 'manifest.json').read_bytes()
    assets = []
    for _, _, row in prepared:
        assets.append({'dimensions': row['dimensions'], 'frame': row['frame'], 'human_approved': False, 'id': row['id'],
                       'item': row['item'], 'manifest_sha256': digest(manifest_bytes), 'output_sha256': row['output_sha256'],
                       'palette_id': row['palette_id'], 'palette_ids': [row['palette_id']],
                       'path': REVISION + '/' + row['output'], 'port_made': True,
                       'port_made_elements': row.get('port_made_elements', []), 'revision': REVISION,
                       'sheet_scope': row['sheet_scope']['scope'], 'source_container': row['source_container'],
                       'source_container_sha256': row['source_container_sha256'],
                       'source_png_sha256': row['source_png_sha256'], 'status': 'draft', 'supersedes': row['supersedes'],
                       'text': row['text'],
                       'reason': 'Owner-review fix of the MAINROM draft (item %d); unreviewed, no approval transfers.' % row['item']})
    supersedes = [{'id': c['id'], 'ledger': c['ledger'], 'output_sha256': c['output_sha256'], 'revision': c['revision']}
                  for f in sorted(old) for c in old[f]]
    return {'schema': 'tokimemo.translation-checkpoint.v1', 'ticket': TICKET, 'assets': assets,
            'candidate_assets': len(assets), 'original_images_included': False,
            'complete_translation_inventory': False, 'separate_reviewed_revisions': 0,
            'port_made_assets': len(assets), 'supersedes': supersedes,
            'weekday_resolution': 'weekday-resolution.json',
            'tag_alignment': {'mode': TAG_ALIGN, 'margin': TAG_MARGIN,
                              'switch': 'TAG_ALIGN in tools/translation_mainrom_fixes.py; owner decision 2026-11-01 is centred'},
            'publication_authorization': 'User explicitly requested translated assets in repository, 2026-10-29; '
                                         'owner art review and fix pass, 2026-11-01.',
            'weekday_stale_three_letter': weekday['stale_three_letter_to_supersede']}


REGISTER_TEXT = """# T39410 register note for the integrator

This checkpoint redraws the MAINROM items the 2026-11-01 owner review sent back. It does not edit any shared
registration file. Every PNG is an unreviewed draft (`human_approved: false`), one file per palette variant.
Run `python3 tools/translation_mainrom_fixes.py --output .asset-cache/<new dir> [--publish <new dir>]` in the
Pillow Docker image to rebuild it.

## 1. Supersedes (translation_inventory.py / inventory_decisions.json)

Register `translations/en/graphics/artwork/checkpoints/T39410-mainrom-fixes-v1/ledger.json` as a ledger and bind
`mainrom-fix-NNNN.<variant>` to the item `mainrom-NNNN` (frame NNNN) for NNNN in {frames}. Candidate ids
follow `mainrom-fix-<frame>.<palette_id>` (`candidate_key` needs that pattern). The `supersedes` list of the
ledger names the {count} earlier candidates they replace (each asset also lists the ones for its own variant);
mark every one `superseded` with `superseded_by` this ledger:

{lists}

Every variant of every frame is covered, so the variant groups of the inventory items can be one per palette
(the previous `hud-43|hud-44|icon-52|icon-53` groups of frame 126 are now four files; the manifest's
`identical_variant_outputs` names the variants whose files are byte-identical because their source image is).

## 2. Weekday items 82-88 (mainrom-0082 .. 0088)

`weekday-resolution.json` proves that every variant resolves to the approved two-letter file
(T39004-date-panel-v1; base and icon-52 share one file). The stale three-letter drafts are the {stale} batch-002
historical assets still listed on the items (`english.historical_assets`). Mark each of them superseded by
`T39004-date-panel-v1/ledger.json` and drop them from the items so no packer can choose them:

{stale_list}

## 3. Catalog wording (image_text_catalog.jsonl / .md)

"catalog now" is the catalog after rounds 2 and 3, which edited the rows of 0122-0125, 0132, 0144 and 0153 to match the art.

| frame | catalog now | drawn |
| :--- | :--- | :--- |
| 0126 | Central Park Guide | CENTRAL PARK GUIDE; 紹介 is a feature heading like the others, so it keeps "Guide" |
| 0127 | Art Museum Opens | ART MUSEUM OPENS |
| 0128 | Aquarium Special | AQUARIUM SPECIAL |
| 0129 | Stadium Opens | STADIUM OPENS |
| 0130 | Botanical Garden Special | GARDEN SPECIAL |
| 0131 | Bowling Alley Special | BOWLING SPECIAL |
| 0132 | Karaoke Box Opens | KARAOKE BOX OPENS |
| 0133 | Planetarium Special | PLANETARIUM SPECIAL |
| 0122-0125 | Spring Issue (3-4-5), Summer Issue (6-7-8), Autumn Issue (9-10-11), Winter Issue (12-1-2) | SPRING / ISSUE, SUMMER / ISSUE, AUTUMN / ISSUE, WINTER / ISSUE |
| 0061 | Health, Humanities, Science, Art, Sports, Trivia, Looks, Guts, Stress | Health, Hum., Science, Art, Sports, Trivia, Looks, Guts, Stress |
| 0140 | Classical | Classical (the T-39009 draft said Classic; the full word fits) |
| 0144 | Pro Wrestling (suggestion: Wrestling) | Wrestling (one line; "Pro / Wrestling" cannot keep the tag's margins) |
| 0151-0154 | Suikoden, Parodius, Power Pros, TwinBee Puzzle | SUIKODEN, PARODIUS, POWER / PROS, TWINBEE / PUZZLE |

Wording rule of the headline family: 完成 -> "Opens", 特集 -> "Special", 紹介 -> "Guide". "Hum." is used in
frame 61 because "Humanities" does not fit the 26 px a label strip leaves before the arrow marker; the text
dictionary has the full word.

## 4. Inventory decisions

- Frame 0057-0059: the earlier reading note (セーブ中, "Save" kept) still holds; "Save" is now in the Japanese band.
- Tags 0134-0145 are centred with a 1 px margin on both sides (owner decision 2026-11-01). If the in-game check
  prefers left alignment, set `TAG_ALIGN = 'left'` in the tool and rebuild; only the tag files change.
- 0151-0154 title lettering (round 2): SUIKODEN and PARODIUS are 4 px semi-bold capitals (eight 2 px-stem letters of
  5 px would need 47 px of the logo's 40); POWER / PROS and TWINBEE / PUZZLE are two lines of 2 px-stem capitals.
  PARODIUS is yellow to orange with a white outline and a navy shadow. Every title keeps 1 px clear of the artwork
  that stays (the 0153 baseball). The cover art outside the title rows and the bottom strip are as T-39009 drew them.
- Headlines 0126-0133 (round 2) are flush left at the column where the Japanese starts (x 0 in all eight) and carry the
  banner's edge tones (highlight = fill - 1, mid = fill + 1, soft = shadow + 1) on the cut corners; the manifest lists
  the tone pixels per banner.
- 0144 reads "Wrestling" in one line with a 5 px W, rows 4-11 like 0145 (round 2). Round 3: the t is the family's t
  (centred crossbar, hooked foot), one row shorter than the l, and the s has a flat one-pixel top, so the t keeps 1 px
  clear of the s; e, n, g and r take their narrow forms and r-e and s-t are kerned to 0 px. The ink is 30 px, so the
  4 shadow pixels of the g's right edge are clipped to keep 1 px clear at the tag's right edge.
- Icon-51 STP: the icon-51 variant's semi-transparency follows the colour. QA pass 3 (`tools/stp.py`) found that
  every English colour maps to exactly one STP value in its source variant, with 0 ambiguous colours across the 32
  frames, so the STP bits can be restored from the colour alone. This closes G1.
- Sheet scope: every row records `sheet_scope`; frame-composite-only drafts must be integrated as whole-frame
  replacements, as before.
- The text dictionary change (item 8) is already in `translations/en/returned-v3.jsonl`: records
  `dictionary:SLPM_86.053:parameter_names:00/01/04` and `SLPM_86.053:00072ad0/00072ad8/00072af0` now read Health,
  Humanities, Sports (they read Condition, Liberal Arts, Athletics); the other stat names already matched. The SLPM
  tables that `translation_tree.py` builds from are gitignored (`SLPM_*`), so the JSONL was edited directly.
  `translations/en/manifest.json` was already out of date with the file and is unchanged.
- Rebuild `translations/en/graphics/translation_inventory.json` (it pins the `returned-v3.jsonl` hash):
  `test_translation_inventory.InventoryTests.test_committed_inventory_matches_inputs` fails until it is rebuilt,
  because the JSONL hash changed (inventory `ed180f9f...`, now `7f3ed304...`).
"""


def register_text(frames, old, weekday):
    lists = []
    for f in sorted(old):
        lists.append('- mainrom-%04d: %s' % (f, ', '.join(c['id'] for c in old[f])))
    stale = weekday['stale_three_letter_to_supersede']
    return REGISTER_TEXT.format(frames=', '.join('%04d' % f for f in frames), count=sum(len(v) for v in old.values()),
                                lists='\n'.join(lists), stale=len(stale),
                                stale_list='\n'.join('- mainrom-%04d: %s' % (s['frame'], s['id']) for s in stale))


def publish(built, prepared, old, weekday, destination):
    destination = destination.absolute()
    if destination.exists():
        raise ValueError('destination must be new')
    (destination / REVISION).mkdir(parents=True)
    for _, _, row in prepared:
        data = (built / REVISION / row['output']).read_bytes()
        if digest(data) != row['output_sha256']:
            raise ValueError('built output changed: ' + row['id'])
        (destination / REVISION / row['output']).write_bytes(data)
    (destination / REVISION / 'manifest.json').write_bytes((built / REVISION / 'manifest.json').read_bytes())
    ledger = ledger_for(built, prepared, old, weekday)
    (destination / 'ledger.json').write_bytes(encoded(ledger) + b'\n')
    (destination / 'weekday-resolution.json').write_bytes(encoded(weekday) + b'\n')
    frames = sorted({row['frame'] for _, _, row in prepared})
    (destination / 'REGISTER.md').write_text(register_text(frames, old, weekday))
    return {'destination': str(destination), 'assets': len(ledger['assets'])}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sources', type=Path, default=ROOT / '.asset-cache/translation-assets/original-mainrom-v1')
    parser.add_argument('--pack', type=Path, default=ROOT / 'disc/files/CDROM/FUKUTEST/MAINROM.NBN')
    parser.add_argument('--executable', type=Path, default=ROOT / 'disc/files/SLPM_86.053')
    parser.add_argument('--output', type=Path, required=True, help='new directory under .asset-cache')
    parser.add_argument('--publish', type=Path, help='also write the checkpoint (ledger, register note) to this new directory')
    parser.add_argument('--previews', type=Path, help='write original | English previews (x8) here')
    args = parser.parse_args()
    result, prepared, old = build(args.sources, args.pack, args.executable, args.output)
    weekday = weekday_resolution()
    if args.previews:
        result['previews'] = previews(prepared, args.previews)
    if args.publish:
        result['published'] = publish(args.output, prepared, old, weekday, args.publish)
    print(json.dumps(result))
