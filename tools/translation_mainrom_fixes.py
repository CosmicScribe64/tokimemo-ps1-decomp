#!/usr/bin/env python3
"""T-39410: art fixes for the English MAINROM.NBN drafts (owner review, 2026-11-01).

Rebuilds, in texel-index space and through each variant's own CLUT, the MAINROM
items the owner review sent back:

* Save labels 57-59: "Save" in the same rows the Japanese used, on the lettering
  layer only (the card layer, with the yellow frame and the icon, stays original);
* magazine banners 126-133: one bold condensed headline face (hand-drawn here,
  17 rows, 2 px strokes, 1 px shadow, the weight of the Japanese) at one size, one
  wording pattern ("<Subject> Opens" for 完成, "<Subject> Special" for 特集,
  "Central Park Guide" for 紹介);
* issue banners 122-125: the original month numerals are replaced by even-width
  bold digits and the word "Issue" restores the meaning of 月号;
* genre tags 134-145: re-lettered with a 1 px margin on both sides, centred
  (TAG_ALIGN is the one switch to move them to the left);
* stat labels, frame 61: the approved stat names (Health, Hum., Science, Art,
  Sports, Trivia, Looks, Guts, Stress), left-aligned in the label strips;
* game covers 151-154: English title lettering in each logo's style.

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
    'Q': rows('.####.', '######', ('##..##', 10), '##.###', '######', '.#####'),
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
BOLD = {ch: stretched(bitmap) for ch, bitmap in BOLD_DRAWN.items()}
# Bold digits for the issue banners: 14 rows, 5 wide, 2 px strokes, no shadow
# (the Japanese numerals have none; they carry a vertical colour ramp instead).
DIGIT_HEIGHT = 14
DIGITS = {
    '0': rows('.###.', '#####', ('##.##', 10), '#####', '.###.'),
    # '1' is the one proportional digit (3 wide, flag, no foot): with it the longest numeral row
    # 9-10-11 leaves room for the word; every other digit is 5 wide.
    '1': rows('.##', '###', ('.##', 12)),
    '2': rows('.###.', '#####', ('...##', 2), '..###', '.###.', '.##..', ('##...', 5), ('#####', 2)),
    '3': rows('.###.', '#####', ('...##', 2), '..###', '..##.', '..###', ('...##', 4), '##.##', '#####', '.###.'),
    '4': rows(('##.##', 7), ('#####', 2), ('...##', 5)),
    '5': rows(('#####', 2), ('##...', 2), '####.', '#####', ('...##', 5), '##.##', '#####', '.###.'),
    '6': rows('.###.', '#####', ('##...', 3), '####.', '#####', ('##.##', 5), '#####', '.###.'),
    '7': rows(('#####', 2), ('...##', 2), ('..##.', 2), ('.##..', 8)),
    '8': rows('.###.', '#####', ('##.##', 3), '#####', '.###.', '#####', ('##.##', 4), '#####', '.###.'),
    '9': rows('.###.', '#####', ('##.##', 4), '#####', '.####', ('...##', 4), '#####', '.###.'),
    # middle dot of 3・4・5, on the rows of the digit centre
    '.': rows(('..', 6), ('##', 2), ('..', 6)),
}
# Small-word additions in panel7 style (7 rows, 1 px stroke), port-made.
PANEL7_PERIOD = (['.', '.', '.', '.', '.', '.', '#'], 'period, 1 wide, on the baseline row')
# Narrow capital W (3 wide), used only where "Wrestling" cannot fit the 30 px (ink plus shadow) a 32 px tag leaves.
NARROW_W = (['#.#', '#.#', '#.#', '###', '###', '###', '#.#'], 'narrow cap W, 3 wide (panel7-style W is 5)')
# Order in which narrow forms are tried: W first (a modest change), r and t last.
NARROW_ORDER = 'WHoaebgns' + 'rt'


def check_bitmaps():
    for ch, bitmap in BOLD_DRAWN.items():
        if len(bitmap) != BOLD_SOURCE_HEIGHT:
            raise ValueError('bold drawing %r is malformed (%d rows)' % (ch, len(bitmap)))
    for ch, bitmap in BOLD.items():
        if len(bitmap) != BOLD_HEIGHT or len({len(r) for r in bitmap}) != 1:
            raise ValueError('bold glyph %r is malformed (%d rows)' % (ch, len(bitmap)))
    for ch, bitmap in DIGITS.items():
        if len(bitmap) != DIGIT_HEIGHT or len({len(r) for r in bitmap}) != 1:
            raise ValueError('digit glyph %r is malformed (%d rows)' % (ch, len(bitmap)))


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


def digit_face():
    check_bitmaps()
    glyphs, provenance = {}, {}
    for ch, bitmap in DIGITS.items():
        glyphs[ch] = base.bitmap_glyph(bitmap)
        provenance[ch] = {'origin': 'port-made', 'description': 'bold digit, 14 rows, 2 px strokes'
                          if ch != '.' else 'middle dot 2x2 on the digit centre rows', 'bitmap': list(bitmap)}
    return base.Face('digits14', glyphs, 3, {
        'style': 'bold digits, 14 rows x 5 (the 1 is 3 wide, no foot), 2 px strokes, vertical colour ramp of the source numerals',
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
          align='center', margin=0, top=None, **target):
    """Draw `text` into rect and return the written bbox.

    align 'center' centres ink plus shadow/outline; 'left' puts the lettering's left
    edge `margin` px from the rect. `top` is the frame row of the first ink row (the
    default centres vertically). The lettering plus its shadow must keep `margin` px
    from the left and right edges of the rect.
    """
    left, right, up, down = extent(face, text)
    ml = mt = 1 if outline is not None else 0
    mr = mb = 1 if (outline is not None or shadow is not None) else 0
    full_w = right - left + 1 + ml + mr
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
            writes[(x + 1, y + 1)] = shadow
    for x, y in ink_px:
        writes[(x, y)] = ramp_table[y] if ink == 'ramp' else ink
    for (x, y), index in sorted(writes.items(), key=lambda kv: (kv[0][1], kv[0][0])):
        base.put(frame, x, y, index, clut_parts, **target)
    xs, ys = [x for x, _ in writes], [y for _, y in writes]
    return [min(xs), min(ys), max(xs) - min(xs) + 1, max(ys) - min(ys) + 1]


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
    132: ('KARAOKE OPENS', 'カラオケBOX完成', 'Opens'),
    133: ('PLANETARIUM SPECIAL', 'プラネタリウム特集', 'Special'),
}


def plan_headline(frame, face):
    text = HEADLINES[frame.frame][0]
    rect = [0, 0, frame.width, frame.height]
    ink, shadow, source = source_ink(frame, rect)
    expected = base.CATALOG_INK.get(frame.frame)
    if expected and expected != (ink, shadow):
        raise ValueError('derived ink/shadow %r differs from catalog %r: frame %d' % ((ink, shadow), expected, frame.frame))
    base.clear(frame, rect)
    box = place(frame, face, text, rect, ink, base.source_parts(source), shadow=shadow)
    return {'item': 2, 'text': text, 'font': 'bold17', 'ink': ink, 'shadow': shadow, 'edit_regions': [rect],
            'lettering_bbox': box, 'wording_rule': HEADLINES[frame.frame][1:]}


# Item 3: issue banners.
ISSUE_DIGITS = {122: '3.4.5', 123: '6.7.8', 124: '9.10.11', 125: '12.1.2'}
ISSUE_WORD = 'Issue'
ISSUE_SHOWS = {122: '3-4-5', 123: '6-7-8', 124: '9-10-11', 125: '12-1-2'}


def plan_issue(frame, digits, faces):
    text = ISSUE_DIGITS[frame.frame]
    rect = [0, 0, frame.width, frame.height]
    source = base.lettering(frame, rect)
    table = base.ramp(source, range(frame.height))
    top = min(y for _, y in source)
    bottom = max(y for _, y in source)
    word_width = 0
    # The word sits on the digits' baseline and ends at the banner's right edge, where 月号 ended.
    # It takes the narrowest panel7 forms only as far as the longest numeral row needs.
    longest = max(extent(digits, t)[1] + 1 for t in ISSUE_DIGITS.values())
    room = frame.width - longest - 1
    face = fit_face(faces, [ISSUE_WORD], room)
    word_width = extent(face, ISSUE_WORD)[1] + 1
    parts = base.source_parts(source)
    base.clear(frame, rect)
    box = place(frame, digits, text, rect, 'ramp', parts, ramp_table=table, align='left', top=top)
    word_top = bottom - 6
    box2 = place(frame, face, ISSUE_WORD, [frame.width - word_width, 0, word_width, frame.height], 'ramp', parts,
                 ramp_table=table, align='left', top=word_top)
    return {'item': 3, 'text': ISSUE_SHOWS[frame.frame] + ' Issue', 'font': 'digits14 + ' + face.name, 'ink': 'ramp',
            'ramp': [table[y] for y in range(frame.height)], 'edit_regions': [rect], 'lettering_bbox': [box, box2],
            'digits_top': top, 'word_top': word_top, 'word_width': word_width,
            'port_made_elements': base.used_port_made(face, [ISSUE_WORD])}


# Item 4: genre tags.
TAG_TEXT = {**base.PORT_MADE_TAGS, 136: ('Action',), 137: ('Suspense',), 140: ('Classical',)}


def plan_tag(frame, faces):
    lines = TAG_TEXT[frame.frame]
    rect = [0, 0, frame.width, frame.height]
    source = base.lettering(frame, rect)
    ink, shadow = base.ink_and_shadow(source)
    face = fit_face(faces, lines, frame.width - 2 * TAG_MARGIN, shadow)
    base.clear(frame, rect)
    line_h = frame.height // len(lines)
    boxes = [place(frame, face, text, [0, n * line_h, frame.width, line_h], ink, base.source_parts(source),
                   shadow=shadow, align=TAG_ALIGN, margin=TAG_MARGIN) for n, text in enumerate(lines)]
    elements = base.used_port_made(face, lines)
    return {'item': 4, 'text': ' / '.join(lines), 'lines': list(lines), 'font': face.name, 'ink': ink, 'shadow': shadow,
            'alignment': TAG_ALIGN, 'margin': TAG_MARGIN, 'edit_regions': [rect], 'lettering_bbox': boxes,
            'port_made_elements': elements}


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


# Item 7: game covers 151-154. The titles are small multi-colour pixel logos (40x16);
# they are re-lettered in small hand-drawn capitals with the logo's own indices.
CAPS8 = {   # 8 rows, 4 wide (I 3, W 5), 1 px strokes
    'A': ['.##.', '#..#', '#..#', '####', '#..#', '#..#', '#..#', '#..#'],
    'B': ['###.', '#..#', '#..#', '###.', '#..#', '#..#', '#..#', '###.'],
    'D': ['###.', '#..#', '#..#', '#..#', '#..#', '#..#', '#..#', '###.'],
    'E': ['####', '#...', '#...', '###.', '#...', '#...', '#...', '####'],
    'I': ['###', '.#.', '.#.', '.#.', '.#.', '.#.', '.#.', '###'],
    'K': ['#..#', '#..#', '#.#.', '##..', '##..', '#.#.', '#..#', '#..#'],
    'N': ['#..#', '##.#', '##.#', '#.##', '#.##', '#..#', '#..#', '#..#'],
    'O': ['.##.', '#..#', '#..#', '#..#', '#..#', '#..#', '#..#', '.##.'],
    'P': ['###.', '#..#', '#..#', '###.', '#...', '#...', '#...', '#...'],
    'R': ['###.', '#..#', '#..#', '###.', '##..', '#.#.', '#..#', '#..#'],
    'S': ['.###', '#...', '#...', '.##.', '...#', '...#', '...#', '###.'],
    'T': ['####', '.##.', '.##.', '.##.', '.##.', '.##.', '.##.', '.##.'],
    'U': ['#..#', '#..#', '#..#', '#..#', '#..#', '#..#', '#..#', '.##.'],
    'W': ['#...#', '#...#', '#...#', '#.#.#', '#.#.#', '#.#.#', '##.##', '#...#'],
}
CAPS5 = {   # 5 rows, 3 wide, 1 px strokes
    'A': ['.#.', '#.#', '###', '#.#', '#.#'],
    'B': ['##.', '#.#', '##.', '#.#', '##.'],
    'E': ['###', '#..', '##.', '#..', '###'],
    'L': ['#..', '#..', '#..', '#..', '###'],
    'P': ['##.', '#.#', '##.', '#..', '#..'],
    'S': ['.##', '#..', '.#.', '..#', '##.'],
    'U': ['#.#', '#.#', '#.#', '#.#', '###'],
    'Z': ['###', '..#', '.#.', '#..', '###'],
}


def cover_faces():
    out = {}
    for name, table, height in (('caps8', CAPS8, 8), ('caps5', CAPS5, 5)):
        glyphs, provenance = {}, {}
        for ch, bitmap in table.items():
            if len(bitmap) != height or len({len(r) for r in bitmap}) != 1:
                raise ValueError('cover glyph %r is malformed' % ch)
            glyphs[ch] = base.bitmap_glyph(bitmap)
            provenance[ch] = {'origin': 'port-made', 'description': '%d-row capital, 1 px strokes' % height,
                              'bitmap': list(bitmap)}
        out[name] = base.Face(name, glyphs, 3, {'style': 'small capitals for cover logos, 1 px strokes',
                                                'rescaled': False, 'generated': False, 'glyphs': provenance})
    return out


# frame -> title spec. Lines are (text, face, first ink row, fill, outline, shadow). 'fill' is an index,
# a list of indices by ink row, or ('letters', [index per letter]). Rects are frame-relative and
# list the title pixels to clear; everything else of the cover is kept.
COVERS = {
    151: {'title': 'SUIKODEN', 'clear': [[8, 0, 40, 10]],
          'lines': [('SUIKODEN', 'caps8', 1, 'ramp', None, None)]},
    152: {'title': 'PARODIUS', 'clear': [[8, 0, 40, 16]],
          'lines': [('PARODIUS', 'caps8', 4, 12, None, 2)]},
    153: {'title': 'PRO BASEBALL', 'clear': [[8, 0, 32, 16], [40, 0, 8, 10], [45, 10, 3, 1]],
          'lines': [('PRO', 'caps8', 1, 15, None, 2), ('BASEBALL', 'caps5', 10, 15, None, 2)]},
    154: {'title': 'TWINBEE PUZZLE', 'clear': [[8, 0, 40, 16]],
          'lines': [('TWINBEE', 'caps8', 1, ('letters', [15, 14, 13, 12, 15, 14, 13]), 1, None),
                    ('PUZZLE', 'caps5', 10, 8, 1, None)]},
}


def plan_cover(frame, faces):
    spec = COVERS[frame.frame]
    source = {}
    for rect in spec['clear']:
        source.update(base.lettering(frame, rect))
    table = base.ramp(source, range(frame.height)) if any(l[3] == 'ramp' for l in spec['lines']) else None
    parts = base.source_parts(source)
    for rect in spec['clear']:
        base.clear(frame, rect)
    boxes, regions = [], [list(r) for r in spec['clear']]
    for text, face_name, top, fill, outline, shadow in spec['lines']:
        face = faces[face_name]
        left = 8
        width = 40
        if frame.frame == 153:
            width = 32 if face_name == 'caps5' or True else 40
        # per-letter placement so that each letter can take its own colour
        if isinstance(fill, tuple):
            colours = fill[1]
        else:
            colours = None
        pts = []
        x = 0
        for n, ch in enumerate(text):
            g = face.glyph(ch)
            pts.append((n, ch, x, g))
            x += g['width'] + 1
        total = x - 1
        ox0 = left + (width - total) // 2
        height = max(y for _, _, _, g in pts for _, y in g['points']) + 1
        letters = []
        for n, ch, dx, g in pts:
            for gx, gy in g['points']:
                letters.append((ox0 + dx + gx, top + gy, n))
        occupied = {(x_, y_) for x_, y_, _ in letters}
        writes = {}
        if outline is not None:
            for x_, y_ in occupied:
                for ddx in (-1, 0, 1):
                    for ddy in (-1, 0, 1):
                        q = (x_ + ddx, y_ + ddy)
                        if q not in occupied and frame.covering(*q) if 0 <= q[0] < frame.width and 0 <= q[1] < frame.height else False:
                            writes[q] = outline
        if shadow is not None:
            for x_, y_ in occupied:
                q = (x_ + 1, y_ + 1)
                if q not in occupied and 0 <= q[0] < frame.width and 0 <= q[1] < frame.height and frame.covering(*q):
                    writes[q] = shadow
        for x_, y_, n in letters:
            if colours is not None:
                writes[(x_, y_)] = colours[n % len(colours)]
            elif isinstance(fill, list):
                writes[(x_, y_)] = fill[y_ - top]
            elif fill == 'ramp':
                writes[(x_, y_)] = table[y_]
            else:
                writes[(x_, y_)] = fill
        for (x_, y_), index in sorted(writes.items(), key=lambda kv: (kv[0][1], kv[0][0])):
            base.put(frame, x_, y_, index, parts)
        xs, ys = [x_ for x_, _ in writes], [y_ for _, y_ in writes]
        boxes.append([min(xs), min(ys), max(xs) - min(xs) + 1, max(ys) - min(ys) + 1])
    elements = [{'kind': 'glyph', 'face': n, 'char': c, 'origin': 'port-made', 'description': 'cover capital'}
                for n, table_ in (('caps8', CAPS8), ('caps5', CAPS5)) for c in sorted(table_)
                if any(c in l[0] and l[1] == n for l in spec['lines'])]
    return {'item': 7, 'text': spec['title'], 'lines': [l[0] for l in spec['lines']], 'font': 'caps8/caps5',
            'ink': 'logo indices', 'edit_regions': regions, 'lettering_bbox': boxes, 'port_made_elements': elements,
            'logo': 'cover art outside the title rows kept unchanged; bottom strip not touched'}


# ---- Build ------------------------------------------------------------------

FRAMES = {'save': (57, 58, 59), 'headline': tuple(sorted(HEADLINES)), 'issue': tuple(sorted(ISSUE_DIGITS)),
          'tag': tuple(sorted(TAG_TEXT)), 'stats': (61,)}


def compose(frames, panel7, native=None):
    """{frame: (edited Frame, plan)} for every frame this tool redraws (no covers)."""
    small = small_faces(panel7)
    bold, digits = bold_face(), digit_face()
    out = {}
    for f in FRAMES['headline']:
        model = frames[f].copy()
        out[f] = (model, plan_headline(model, bold))
    for f in FRAMES['issue']:
        model = frames[f].copy()
        out[f] = (model, plan_issue(model, digits, small))
    for f in FRAMES['tag']:
        model = frames[f].copy()
        out[f] = (model, plan_tag(model, small))
    cover = cover_faces()
    for f in sorted(COVERS):
        model = frames[f].copy()
        # The bottom strip keeps its English ("Out in '95" / "Out Now!") exactly as the T-39009 draft had it.
        strip = base.plan_bar(model, native, strip_top=50)
        plan = plan_cover(model, cover)
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
    return out, {'bold17': bold, 'digits14': digits, **small}


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
    for f in sorted(set(sum(FRAMES.values(), ())) | set(COVERS)):
        item = items.get('mainrom-%04d' % f)
        if item is None:
            continue
        rows_ = []
        for c in item['english']['candidates']:
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

## 3. Catalog wording (image_text_catalog.jsonl / .md; not edited here)

| frame | catalog now | drawn |
| :--- | :--- | :--- |
| 0126 | Central Park Guide (suggestion column says "Central Park") | CENTRAL PARK GUIDE; 紹介 is a feature heading like the others, so it keeps "Guide" |
| 0127 | Art Museum Completed | ART MUSEUM OPENS |
| 0128 | Aquarium Feature | AQUARIUM SPECIAL |
| 0129 | Stadium Completed | STADIUM OPENS |
| 0130 | Botanical Garden Feature | GARDEN SPECIAL |
| 0131 | Bowling Alley Feature | BOWLING SPECIAL |
| 0132 | Karaoke Box Completed | KARAOKE OPENS |
| 0133 | Planetarium Feature | PLANETARIUM SPECIAL |
| 0122-0125 | Mar / Apr / May Issue ... | 3-4-5 Issue, 6-7-8 Issue, 9-10-11 Issue, 12-1-2 Issue |
| 0061 | Condition / Liberal Arts / Athletics | Health, Hum., Science, Art, Sports, Trivia, Looks, Guts, Stress |
| 0140 | Classical | Classical (the T-39009 draft said Classic; the full word fits) |
| 0151-0154 | Gensosuikoden, Gokujou Parodius, Jikkyou Powerful Pro Yakyuu '95, TwinBee Taisen Puzzle-Dama | SUIKODEN, PARODIUS, PRO BASEBALL, TWINBEE PUZZLE |

Wording rule of the headline family: 完成 -> "Opens", 特集 -> "Special", 紹介 -> "Guide". "Hum." is used in
frame 61 because "Humanities" does not fit the 26 px a label strip leaves before the arrow marker; the text
dictionary has the full word.

## 4. Inventory decisions

- Frame 0057-0059: the earlier reading note (セーブ中, "Save" kept) still holds; "Save" is now in the Japanese band.
- Tags 0134-0145 are centred with a 1 px margin on both sides (owner decision 2026-11-01). If the in-game check
  prefers left alignment, set `TAG_ALIGN = 'left'` in the tool and rebuild; only the tag files change.
- All 0151-0154 title lettering is thin 1 px capitals (eight letters must fit 40 px); it keeps each logo's colours and
  the cover art outside the title rows and the bottom strip exactly as T-39009 drew them.
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
