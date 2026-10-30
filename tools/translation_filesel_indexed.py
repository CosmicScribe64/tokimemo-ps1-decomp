#!/usr/bin/env python3
"""T-39011: FILESEL button and information-page drafts edited in 8bpp index space.

Edits are made on the raw palette indices of FILESEL.NBN, never on RGBA:

* Buttons (frames 0-3): the core (177) and outline (178) lettering inside the
  40x16 bevel interior is removed. Each removed texel that the new lettering
  does not cover is refilled from an ordinal half-plane model of the source's
  own diagonal index ramp (one slope, one threshold per band boundary), fitted
  only on texels that never held lettering. Known texels never change. English
  lettering is NativeLatin (twelve source rows, unscaled, unclipped): core 177,
  outline = 8-neighbour dilation minus core in 178.
* Information pages 0-2: only label cells whose Japanese ink is an exact
  native-font glyph match are edited (ink index 2 on the cell's single paper
  index). Every other label stays source and is reported missing with a reason.

The patched bytes exist only in memory. The project's own probe/export reader
re-decodes them, and every exported PNG and STP mask that is not an edited
asset must equal the cached original-filesel-v1 export byte for byte. Outputs
are unreviewed draft candidates; nothing here records human approval.
"""
import argparse
from fractions import Fraction
import json
from pathlib import Path
import shutil
import sys
import tempfile

import numpy as np

from translation_assets import ROOT, digest, encoded, inside, new_output
from translation_native_font import NativeLatin
sys.path.insert(0, str(ROOT))
from tools.assets import image_packs_filesel as packs
from tools.assets.text_font import font_reference

SOURCE_SHA = '1a0e42ade6320df564cdb1017f73ba05083394c1974d3bc7a4af21618725e2a6'
REVISION = 'filesel-indexed-v3'
NBN = ROOT / 'disc/files/CDROM/FUKUTEST/FILESEL.NBN'
EXE = ROOT / 'disc/files/SLPM_86.053'
CLUT_OFFSETS = (0x22000, 0x22200)
CORE, OUTLINE = 177, 178
BEVEL = frozenset((0, 176, 189, 217, 218, 220))
GRADIENT = frozenset((147, 161) + tuple(range(195, 217)))
# Source order of frames 0-3 (verified against the button lettering); wording
# from the translator catalog rows 99-102 (revisable default, see the ticket).
BUTTON_WORDS = ('Copy', 'New', 'Clear', 'Load')
SLOPES = range(0, 301)  # half-plane slope n/100 for p = 100*x + n*y
GATE = {'min_exact': 0.93, 'max_band_error': 1, 'max_slope_spread': 5, 'shifts': (-3, 0, 3)}

PAGE_OFFSET, PAGE_SIZE, PAGE_SIDE, MAP_ROWS = 0x24000, 0x4000, 128, 120
INK = 2
# Montage x = string x + 160 and y = string y + 120: the name, nickname and
# club strings (y -0x4B/-0x3B/-0x2B) land exactly on the label cell rows 45/61/77
# and the date digits fall just left of the year/month/day cells. Inferred, not
# traced through the BG map setup.
STRING_ORIGIN = (160, 120)
DYNAMIC_STRING_LEFT = 0x20 + STRING_ORIGIN[0]
# file_select_number_draw: ones digit at x, tens at x-8, 8 texels each; rows
# taken conservatively as 16 from the draw y. [left, top, width, height].
DIGIT_SPANS = [[x + STRING_ORIGIN[0] - 8, y + STRING_ORIGIN[1], 16, 16]
               for x, y in ((0x30, -0x60), (0x50, -0x60), (0x70, -0x60), (0x5A, -0x1D))]
EVIDENCE = ['src/ovl/name_entry/file_select.c:file_select_number_draw',
            'src/ovl/name_entry/file_select.c:file_select_slot_summary_draw (year/month/day 0x30/0x50/0x70,-0x60; count 0x5A,-0x1D)',
            'src/ovl/name_entry/file_select.c set_kanji_string name/nickname/club at x 0x20, y -0x4B/-0x3B/-0x2B']
# Exact-match label cells (montage coordinates of the 12x12 font cell).
MATCHED = [
    {'id': 'name', 'japanese': '名前', 'cells': [('名', 120, 45), ('前', 153, 45)], 'english': 'Name', 'mode': 'label'},
    {'id': 'nickname', 'japanese': 'あだ名', 'cells': [('あ', 121, 61), ('だ', 137, 61), ('名', 153, 61)], 'english': 'Nickname', 'mode': 'label'},
    {'id': 'date-year-suffix', 'japanese': '年', 'cells': [('年', 217, 28)], 'english': '/', 'mode': 'cell'},
    {'id': 'date-month-suffix', 'japanese': '月', 'cells': [('月', 248, 28)], 'english': '/', 'mode': 'cell'},
    {'id': 'date-day-suffix', 'japanese': '日', 'cells': [('日', 279, 28)], 'english': '', 'mode': 'cell'},
    {'id': 'clear-count-suffix', 'japanese': '人', 'cells': [('人', 261, 95)], 'english': '', 'mode': 'cell'},
]
UNMATCHED = [
    {'id': 'club', 'japanese': '所属部', 'search': [118, 75, 52, 14], 'ink': [INK], 'matched_cells': [('所', 121, 77), ('属', 137, 77)],
     'reason': 'the third glyph has no exact native-font match, so the label region is not exact glyph-matched'},
    {'id': 'clear-count-label', 'japanese': 'クリア人数', 'search': [118, 93, 50, 14], 'ink': [INK], 'matched_cells': [('数', 155, 95)],
     'reason': 'only the last glyph is an exact native-font match; the katakana are hand-drawn'},
    {'id': 'system-tab', 'japanese': 'システム', 'search': [14, 86, 40, 16], 'ink': [219, 134], 'matched_cells': [],
     'reason': 'hand-drawn blue lettering with shadow; no native-font match'},
    {'id': 'button-panel', 'japanese': 'コピー/ニュー/クリア/ロード', 'search': [14, 20, 92, 44], 'ink': [58, 166], 'matched_cells': [],
     'reason': 'outlined lettering on a gradient, not native-font glyphs; default builds page candidates only for exact glyph-matched cells'},
]
KEPT = [{'id': 'year-prefix', 'search': [180, 26, 20, 16], 'ink': [INK], 'reason': 'baked numerals, kept as source'}]


def dil8(mask):
    """8-neighbour dilation of a boolean array (no wrap)."""
    out = mask.copy()
    height, width = mask.shape
    for dy in (-1, 0, 1):
        for dx in (-1, 0, 1):
            out[max(dy, 0):height + min(dy, 0), max(dx, 0):width + min(dx, 0)] |= \
                mask[max(-dy, 0):height + min(-dy, 0), max(-dx, 0):width + min(-dx, 0)]
    return out


def _split(p, rank, bands):
    """Per band boundary, the threshold with fewest errors; thresholds made monotone."""
    order = np.argsort(p, kind='stable')
    sp, sr = p[order], rank[order]
    n = len(sp)
    valid = np.concatenate(([True], sp[1:] != sp[:-1], [True]))
    thresholds = []
    for k in range(1, bands):
        ones = (sr >= k).astype(np.int64)
        ones_below = np.concatenate(([0], np.cumsum(ones)))
        zeros_above = (n - ones.sum()) - (np.arange(n + 1) - ones_below)
        errors = np.where(valid, ones_below + zeros_above, n + 1)
        i = int(np.argmin(errors))
        thresholds.append(int(sp[i]) if i < n else int(sp[-1]) + 1)
    return [int(t) for t in np.maximum.accumulate(thresholds)] if thresholds else []


def fit_ramp(values, known):
    """Fit the ordinal half-plane model on known texels of a 2D index array."""
    ys, xs = np.nonzero(known)
    unique, inverse, counts = np.unique(values[ys, xs], return_inverse=True, return_counts=True)
    best = None
    for n in SLOPES:
        p = 100 * xs + n * ys
        totals = np.bincount(inverse, weights=p).astype(np.int64)
        position = sorted(range(len(unique)), key=lambda i: (Fraction(int(totals[i]), int(counts[i])), int(unique[i])))
        order = [int(unique[i]) for i in position]
        rank = np.argsort(position)[inverse]
        thresholds = _split(p, rank, len(order))
        predicted = (p[:, None] >= np.array(thresholds or [1 << 40])[None, :]).sum(axis=1) if thresholds else np.zeros(len(p), int)
        exact = int((predicted == rank).sum())
        if best is None or exact > best['exact']:
            best = {'slope': n, 'order': order, 'thresholds': thresholds, 'exact': exact, 'known': len(p)}
    best['fit_accuracy'] = round(best['exact'] / best['known'], 6)
    return best


def predict(model, ys, xs):
    p = 100 * np.asarray(xs) + model['slope'] * np.asarray(ys)
    rank = (p[:, None] >= np.array(model['thresholds'])[None, :]).sum(axis=1) if model['thresholds'] else np.zeros(len(p), int)
    return rank, [model['order'][r] for r in rank.tolist()]


def leave_out(interiors):
    """Mask each button with the other buttons' lettering masks shifted by GATE shifts."""
    total = exact = worst = 0
    for i, values in enumerate(interiors):
        known = ~np.isin(values, (CORE, OUTLINE))
        for j, other in enumerate(interiors):
            if i == j:
                continue
            for shift in GATE['shifts']:
                held = np.roll(np.isin(other, (CORE, OUTLINE)), shift, axis=1)
                if shift > 0:
                    held[:, :shift] = False
                elif shift < 0:
                    held[:, shift:] = False
                held &= known
                model = fit_ramp(values, known & ~held)
                ys, xs = np.nonzero(held)
                rank, _ = predict(model, ys, xs)
                truth = np.array([model['order'].index(v) if v in model['order'] else -99 for v in values[ys, xs].tolist()])
                total += len(ys)
                exact += int((rank == truth).sum())
                worst = max(worst, int(np.abs(rank - truth).max()) if len(ys) else 0)
    return {'held_out_texels': total, 'index_exact': round(exact / total, 6), 'max_band_error': worst}


def word_mask(font, word, gap):
    glyphs = [font.glyph(c) for c in word]
    width = sum(g['width'] for g in glyphs) + gap * (len(glyphs) - 1)
    mask = np.zeros((12, width), bool)
    cursor = 0
    for g in glyphs:
        for dx, dy in g['points']:
            mask[dy, cursor + dx] = True
        cursor += g['width'] + gap
    return mask


def layout_buttons(font, words, height=16, width=40):
    """Core masks on one shared baseline, each word centred; outline must stay inside."""
    rows = [r for w in words for r in np.nonzero(word_mask(font, w, 1).any(axis=1))[0].tolist()]
    top, bottom = min(rows), max(rows)
    if bottom - top + 3 > height:
        raise ValueError('button lettering rows exceed the interior')
    y0 = (height - (bottom - top + 3)) // 2 + 1 - top
    placed = []
    for word in words:
        for gap in (2, 1):
            mask = word_mask(font, word, gap)
            if mask.shape[1] + 2 <= width:
                break
        else:
            raise ValueError('button word does not fit without scaling: ' + word)
        x0 = (width - mask.shape[1] - 2) // 2 + 1
        core = np.zeros((height, width), bool)
        core[y0:y0 + 12, x0:x0 + mask.shape[1]] = mask
        # The row and width checks above keep the dilated outline off the border.
        outline = dil8(core) & ~core
        placed.append({'word': word, 'gap': gap, 'core': core, 'outline': outline,
                       'core_rect': [x0, y0 + top, mask.shape[1], bottom - top + 1]})
    return placed


def outline_deviation(values):
    core = values == CORE
    rule = dil8(core) & ~core
    source = values == OUTLINE
    as_list = lambda m: [[int(x), int(y)] for y, x in zip(*np.nonzero(m))]
    return {'missing_from_source': as_list(rule & ~source), 'extra_in_source': as_list(source & ~rule)}


def compose_button(values, model, placed):
    """New interior: kept source, reconstructed former strokes, then new lettering.

    Only removed-stroke and new-lettering texels are written, so kept texels and
    the index set (ramp bands come from known texels) cannot change by construction.
    """
    removed = np.isin(values, (CORE, OUTLINE))
    result = values.copy()
    provenance = np.full(values.shape, 'k')
    lettering = placed['core'] | placed['outline']
    ys, xs = np.nonzero(removed & ~lettering)
    _, indices = predict(model, ys, xs)
    for y, x, index in zip(ys.tolist(), xs.tolist(), indices):
        result[y, x] = index
        provenance[y, x] = 'r'
    result[placed['outline']] = OUTLINE
    provenance[placed['outline']] = 'o'
    result[placed['core']] = CORE
    provenance[placed['core']] = 'c'
    return result, [''.join(row) for row in provenance.tolist()]


def verify_patch(source, patched, rects):
    """Every changed byte must sit inside a declared (offset, u, v, width, height) rect."""
    if len(source) != len(patched):
        raise ValueError('patched size differs')
    allowed = set()
    for offset, u, v, width, height in rects:
        for y in range(v, v + height):
            allowed.update(range(offset + y * 128 + u, offset + y * 128 + u + width))
    a, b = np.frombuffer(source, np.uint8), np.frombuffer(patched, np.uint8)
    changed = np.nonzero(a != b)[0].tolist()
    stray = [c for c in changed if c not in allowed]
    if stray:
        raise ValueError('bytes changed outside declared rectangles at 0x%x' % stray[0])
    for offset in CLUT_OFFSETS:
        if source[offset:offset + 0x200] != patched[offset:offset + 0x200]:
            raise ValueError('CLUT bytes changed')
    return len(changed)


def check_page_rect(page, rect):
    x, y, width, height = rect
    if not (0 <= page < 3 and 0 <= x and x + width <= PAGE_SIDE and 0 <= y and y + height <= MAP_ROWS and width > 0 and height > 0):
        raise ValueError('page edit outside its page, across a seam or at row 120 or below')


def split_montage(rect):
    x, y, width, height = rect
    pieces = []
    for page in range(3):
        left, right = max(x, page * PAGE_SIDE), min(x + width, (page + 1) * PAGE_SIDE)
        if left < right:
            piece = [left - page * PAGE_SIDE, y, right - left, height]
            check_page_rect(page, piece)
            pieces.append({'page': page, 'rect': piece})
    return pieces


def glyph_bitmap(exe, metadata, character):
    reference = font_reference(character.encode('cp932'), metadata)
    if reference['status'] != 'glyph':
        raise ValueError('character missing from native font')
    raw = exe[reference['offset']:reference['offset'] + 18]
    bits = [raw[i // 8] >> (7 - i % 8) & 1 for i in range(144)]
    return np.array(bits, bool).reshape(12, 12), {'code': character.encode('cp932').hex(), 'offset': reference['offset'], 'sha256': digest(raw)}


def bbox(mask, left=0, top=0):
    ys, xs = np.nonzero(mask)
    if not len(ys):
        return None
    return [int(xs.min()) + left, int(ys.min()) + top, int(xs.max() - xs.min()) + 1, int(ys.max() - ys.min()) + 1]


def edit_pages(montage, font, exe, metadata):
    """Erase matched glyph ink to the cell paper and place NativeLatin; returns regions."""
    result = montage.copy()
    regions, witnesses = [], {}
    for label in MATCHED:
        erased = np.zeros(montage.shape, bool)
        papers = set()
        for character, x, y in label['cells']:
            glyph, proof = glyph_bitmap(exe, metadata, character)
            witnesses[character] = proof
            cell = montage[y:y + 12, x:x + 12]
            if not ((cell == INK) == glyph).all():
                raise ValueError('label cell is not an exact native-font match: ' + label['id'])
            paper = set(cell[~glyph].tolist())
            if len(paper) != 1:
                raise ValueError('label cell has no single paper index: ' + label['id'])
            papers |= paper
            erased[y:y + 12, x:x + 12] |= glyph
        if len(papers) != 1:
            raise ValueError('label cells disagree on paper: ' + label['id'])
        paper = papers.pop()
        result[erased] = paper
        first, x0, y0 = label['cells'][0]
        drawn = np.zeros(montage.shape, bool)
        if label['english']:
            if label['mode'] == 'label':
                left = x0 + int(np.nonzero(glyph_bitmap(exe, metadata, first)[0].any(axis=0))[0].min())
                mask = word_mask(font, label['english'], 1)
            else:
                mask = word_mask(font, label['english'], 1)
                left = x0 + font.proof[label['english']]['trim_left']
            drawn[y0:y0 + 12, left:left + mask.shape[1]] = mask
        if drawn.any():
            if (result[drawn] != paper).any():
                raise ValueError('replacement touches non-paper texels: ' + label['id'])
            columns = np.nonzero(drawn.any(axis=0))[0]
            if label['mode'] == 'label' and columns.max() >= DYNAMIC_STRING_LEFT:
                raise ValueError('label reaches the dynamic string x: ' + label['id'])
            for left, top, width, height in DIGIT_SPANS:
                if drawn[top:top + height, left:left + width].any():
                    raise ValueError('lettering overlaps a code-drawn digit: ' + label['id'])
            result[drawn] = INK
        rect = bbox(erased | drawn)
        regions.append({'id': label['id'], 'japanese': label['japanese'], 'english': label['english'], 'status': 'candidate',
                        'match': 'exact-native-glyph', 'cells': [{'glyph': c, 'cell': [x, y, 12, 12]} for c, x, y in label['cells']],
                        'paper_index': int(paper), 'ink_index': INK, 'montage_rect': rect, 'page_rects': split_montage(rect),
                        'english_rect': bbox(drawn), 'letter_gap': 1 if label['mode'] == 'label' and label['english'] else None})
    for item in UNMATCHED:
        x, y, width, height = item['search']
        cells = []
        for character, cx, cy in item['matched_cells']:
            glyph, proof = glyph_bitmap(exe, metadata, character)
            witnesses[character] = proof
            if not ((montage[cy:cy + 12, cx:cx + 12] == INK) == glyph).all():
                raise ValueError('expected partial glyph match moved: ' + item['id'])
            cells.append({'glyph': character, 'cell': [cx, cy, 12, 12]})
        rect = bbox(np.isin(montage[y:y + height, x:x + width], item['ink']), x, y)
        if rect is None:
            raise ValueError('unmatched region has no ink: ' + item['id'])
        regions.append({'id': item['id'], 'japanese': item['japanese'], 'status': 'missing', 'reason': item['reason'],
                        'match': 'partial' if cells else 'none', 'cells': cells, 'ink_indices': item['ink'],
                        'montage_rect': rect, 'page_rects': split_montage(rect)})
    for item in KEPT:
        x, y, width, height = item['search']
        rect = bbox(np.isin(montage[y:y + height, x:x + width], item['ink']), x, y)
        regions.append({'id': item['id'], 'status': 'kept-source', 'reason': item['reason'], 'montage_rect': rect, 'page_rects': split_montage(rect)})
    changed = result != montage
    for region in regions:
        if region['status'] != 'candidate':
            x, y, width, height = region['montage_rect']
            if changed[y:y + height, x:x + width].any():
                raise ValueError('unmatched region changed: ' + region['id'])
    return result, regions, witnesses


def region_map(regions, witnesses):
    return {'schema': 'tokimemo.filesel-page-regions.v1', 'ticket': 'T-39011', 'container_sha256': SOURCE_SHA,
            'coordinate_space': 'montage of source pages information-0..2 side by side (384x128); page_rects in each 128x128 page',
            'page_seams_x': [128, 256], 'map_rows': MAP_ROWS,
            'map_rows_note': 'source manifest: upload row1 starts at 128 while map v uses 120; every region is asserted inside rows 0-119',
            'string_origin': {'montage_offset': list(STRING_ORIGIN), 'status': 'inferred from label rows and digit positions',
                              'dynamic_string_left_x': DYNAMIC_STRING_LEFT, 'digit_rects': DIGIT_SPANS, 'evidence': EVIDENCE},
            'glyph_witnesses': witnesses, 'regions': regions, 'human_confirmed': False}


def button_interiors(data, frames):
    """Frames 0-3: (part, index rect, interior origin) and the 40x16 interior indices."""
    parts, interiors = [], []
    for frame in range(4):
        (part,) = frames[frame]['parts']
        offset, u, v = part['sheet_source_offset'], part['u'], part['v']
        tile = np.frombuffer(data, np.uint8, PAGE_SIZE, offset).reshape(128, 128)[v:v + part['height'], u:u + part['width']]
        box = bbox(tile != 0)
        bx, by = box[0] + 1, box[1] + 1
        interior = tile[by:by + 16, bx:bx + 40].copy()
        border = set(tile[box[1]:box[1] + box[3], box[0]:box[0] + box[2]].ravel().tolist()) - set(interior.ravel().tolist())
        if box[2:] != [42, 18] or not set(interior.ravel().tolist()) <= GRADIENT | {CORE, OUTLINE} or not border <= BEVEL:
            raise ValueError('button anatomy differs from the source profile: frame %d' % frame)
        edit = [offset, u + bx, v + by, 40, 16]
        for other in frames:
            for q in other['parts']:
                if other['frame_index'] != frame and q['sheet_source_offset'] == offset and \
                        q['u'] < edit[1] + 40 and edit[1] < q['u'] + q['width'] and q['v'] < edit[2] + 16 and edit[2] < q['v'] + q['height']:
                    raise ValueError('another frame samples the edited button texels: frame %d' % frame)
        parts.append((part, edit, [bx, by]))
        interiors.append(interior)
    return parts, interiors


def check_gate(gate, slopes):
    if not (gate['index_exact'] >= GATE['min_exact'] and gate['max_band_error'] <= GATE['max_band_error']
            and max(slopes) - min(slopes) <= GATE['max_slope_spread']):
        raise ValueError('leave-out reconstruction gate failed: %r slopes %r' % (gate, slopes))


def check_exports(sources, original, patched, files, edited):
    """Unpatched re-export equals the cache; exactly the edited exports differ from it."""
    for name in files:
        source = inside(sources, name).read_bytes()
        if (original / name).read_bytes() != source:
            raise ValueError('source export differs from the cached original: ' + name)
        same = (patched / name).read_bytes() == source
        if same == (name in edited):
            raise ValueError('unexpected export identity: ' + name)


def build(sources, output, nbn=NBN, exe=EXE, regions_out=None):
    data = nbn.read_bytes()
    if digest(data) != SOURCE_SHA:
        raise ValueError('FILESEL source differs')
    cached = json.loads((sources / 'manifest.json').read_text())
    if cached['metadata']['source_sha256'] != SOURCE_SHA:
        raise ValueError('cached FILESEL export differs')
    output = new_output(output)
    font = NativeLatin(exe)
    exe_bytes = font.data
    frames = packs._parse(data)
    patched = bytearray(data)
    rects, assets = [], []

    # Buttons.
    parts, interiors = button_interiors(data, frames)
    gate = leave_out(interiors)
    placed = layout_buttons(font, BUTTON_WORDS)
    models = []
    for frame, ((part, edit, origin), interior, place) in enumerate(zip(parts, interiors, placed)):
        model = fit_ramp(interior, ~np.isin(interior, (CORE, OUTLINE)))
        models.append(model['slope'])
        new, provenance = compose_button(interior, model, place)
        offset, u, v = edit[:3]
        for y in range(16):
            start = offset + (v + y) * 128 + u
            patched[start:start + 40] = new[y].astype(np.uint8).tobytes()
        rects.append(edit)
        counts = {k: sum(row.count(k) for row in provenance) for k in 'kroc'}
        assets.append({'id': 'filesel-frame-%d.base' % frame, 'item_id': 'filesel-%04d' % frame, 'frame': frame, 'candidate': True,
                       'palette_id': 'base', 'variant': 'frame-%02d.base' % frame, 'source_png': 'filesel.frame-%02d.base.png' % frame,
                       'output': 'frame-%02d.base.png' % frame, 'english': place['word'],
                       'edit_regions': [[origin[0], origin[1], 40, 16]],
                       'index_patch': {'sheet_offset': offset, 'rect': edit[1:], 'before_sha256': digest(interior.astype(np.uint8).tobytes()),
                                       'after_sha256': digest(new.astype(np.uint8).tobytes())},
                       'lettering': {'font': 'native12', 'word': place['word'], 'letter_gap': place['gap'], 'core_index': CORE,
                                     'outline_index': OUTLINE, 'outline_rule': 'dil8(core)-core', 'core_rect_interior': place['core_rect']},
                       'reconstruction': {'method': 'ordinal half-plane p=100*x+slope*y over the source index ramp',
                                          'slope_hundredths': model['slope'], 'band_order': model['order'], 'thresholds': model['thresholds'],
                                          'fit_accuracy_known': model['fit_accuracy'], 'texels': counts,
                                          'provenance_rows': provenance, 'legend': {'k': 'kept source', 'r': 'reconstructed', 'o': 'outline', 'c': 'core'}},
                       'source_outline_deviation': outline_deviation(interior),
                       'review_flags': ['%d reconstructed texels may be one band off; needs magnified human review' % counts['r']]})
    check_gate(gate, models)

    # Information pages.
    montage = np.concatenate([np.frombuffer(data, np.uint8, PAGE_SIZE, PAGE_OFFSET + i * PAGE_SIZE).reshape(128, 128) for i in range(3)], axis=1)
    edited, regions, witnesses = edit_pages(montage, font, exe_bytes, font.metadata)
    for page in range(3):
        before, after = montage[:, page * 128:(page + 1) * 128], edited[:, page * 128:(page + 1) * 128]
        pieces = [dict(piece, region=r['id']) for r in regions if r['status'] == 'candidate' for piece in r['page_rects'] if piece['page'] == page]
        if not pieces:
            continue
        offset = PAGE_OFFSET + page * PAGE_SIZE
        patched[offset:offset + PAGE_SIZE] = after.astype(np.uint8).tobytes()
        rects.extend([offset] + piece['rect'] for piece in pieces)
        missing = [r['id'] for r in regions if r['status'] == 'missing' and any(q['page'] == page for q in r['page_rects'])]
        # A page with any region still Japanese is a partial build: kept in the
        # cache output, never published as a candidate.
        assets.append({'id': 'filesel-page-information-%d' % page, 'item_id': 'filesel-page-information-%d' % page,
                       'frame': None, 'candidate': not missing, 'palette_id': 'base', 'variant': 'source-page-information-%d' % page,
                       'source_png': 'filesel.source-page-information-%d.png' % page, 'output': 'source-page-information-%d.png' % page,
                       'edit_regions': [piece['rect'] for piece in pieces], 'edited_regions': sorted({piece['region'] for piece in pieces}),
                       'source_regions_remaining': missing,
                       'index_patch': {'sheet_offset': offset, 'rect': [0, 0, 128, 128], 'before_sha256': digest(before.astype(np.uint8).tobytes()),
                                       'after_sha256': digest(after.astype(np.uint8).tobytes())},
                       'lettering': {'font': 'native12', 'ink_index': INK},
                       'review_flags': ['partial page: %s stay Japanese' % ', '.join(missing)] if missing else []})

    changed = verify_patch(data, bytes(patched), rects)
    probe = packs.probe_filesel_ui(bytes(patched))
    if probe['status'] != 'supported':
        raise ValueError('patched FILESEL no longer probes')
    output.mkdir(parents=True)
    with tempfile.TemporaryDirectory(dir=ROOT / '.asset-cache') as scratch:
        scratch = Path(scratch)
        original = packs.export_filesel_ui(data, None, scratch / 'original', 'filesel')
        result = packs.export_filesel_ui(bytes(patched), None, scratch / 'patched', 'filesel')
        if original['files'] != result['files']:
            raise ValueError('export file list changed')
        check_exports(sources, scratch / 'original', scratch / 'patched', result['files'], {a['source_png'] for a in assets})
        for asset in assets:
            shutil.copyfile(scratch / 'patched' / asset['source_png'], output / asset['output'])
            asset['source_png_sha256'] = digest(inside(sources, asset['source_png']).read_bytes())
            asset['output_sha256'] = digest((output / asset['output']).read_bytes())
            asset.update({'source_container': 'CDROM/FUKUTEST/FILESEL.NBN', 'source_container_sha256': SOURCE_SHA,
                          'dimensions': [128, 128] if asset['frame'] is None else frames[asset['frame']]['bounds'][2:],
                          'style': 'unreviewed', 'human_approved': False, 'runtime_integrated': False, 'in_game_verified': False})
    regions_doc = region_map(regions, witnesses)
    manifest = {'schema': 'tokimemo.filesel-indexed-drafts.v1', 'ticket': 'T-39011', 'revision': REVISION,
                'source_container_sha256': SOURCE_SHA, 'assets': assets, 'leave_out_gate': dict(gate, thresholds={k: list(v) if isinstance(v, tuple) else v for k, v in GATE.items()}, slopes_hundredths=models),
                'round_trip': {'changed_bytes': changed, 'file_size': len(patched), 'clut_unchanged': True,
                               'other_exports_identical': len(result['files']) - len(assets), 'probe': probe['status'],
                               'patched_container_written': False},
                'page_regions_sha256': digest(encoded(regions_doc) + b'\n'),
                'font_provenance': font.provenance(), 'human_approved': False,
                'runtime_gap': 'port/godot/services/translated_image_store.gd keys on container|frame|palette|png_sha; source pages have no key'}
    (output / 'manifest.json').write_bytes(encoded(manifest) + b'\n')
    if regions_out:
        regions_out.write_bytes(encoded(regions_doc) + b'\n')
    return {'output': str(output), 'drafts': len(assets), 'gate': gate}, regions_doc


def publish(built, destination):
    """Copy the candidate drafts, a candidate ledger and a separate agent review into a new directory.

    Partial page builds (candidate false) stay in the build output only.
    """
    manifest_bytes = (built / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    destination = destination.absolute()
    if destination.exists():
        raise ValueError('destination must be new')
    (destination / REVISION).mkdir(parents=True)
    ledger, review = [], []
    for row in manifest['assets']:
        if not row['candidate']:
            continue
        data = (built / row['output']).read_bytes()
        if digest(data) != row['output_sha256']:
            raise ValueError('built output changed: ' + row['id'])
        (destination / REVISION / row['output']).write_bytes(data)
        ledger.append({'id': row['id'], 'item_id': row['item_id'], 'frame': row['frame'], 'palette_id': row['palette_id'],
                       'dimensions': row['dimensions'], 'path': REVISION + '/' + row['output'], 'revision': REVISION,
                       'manifest_sha256': digest(manifest_bytes), 'output_sha256': row['output_sha256'],
                       'source_container_sha256': row['source_container_sha256'], 'source_png_sha256': row['source_png_sha256'],
                       'edit_regions': row['edit_regions'], 'status': 'draft', 'human_approved': False,
                       'reason': 'Index-space draft candidate; unreviewed; not runtime integrated.'})
        review.append({'id': row['id'], 'output_sha256': row['output_sha256'], 'agent_checks': [
            'pixels outside edit_regions equal the source export', 'output indices are a subset of the source indices',
            'other FILESEL exports byte-identical after the in-memory patch'], 'flags': row['review_flags']})
    (destination / REVISION / 'manifest.json').write_bytes(manifest_bytes)
    (destination / 'ledger.json').write_bytes(encoded({
        'schema': 'tokimemo.translation-checkpoint.v1', 'ticket': 'T-39011', 'assets': ledger, 'candidate_assets': len(ledger),
        'original_images_included': False,
        'source_pixels_note': 'Each draft is a whole source frame or 128x128 source page; only its edit_regions differ from the source export.',
        'unpublished_partial_builds': [row['id'] for row in manifest['assets'] if not row['candidate']],
        'complete_translation_inventory': False, 'separate_reviewed_revisions': 0,
        'publication_authorization': 'User explicitly requested translated assets in repository, 2026-10-29.'}) + b'\n')
    (destination / 'review.agent.json').write_bytes(encoded({
        'schema': 'tokimemo.agent-review.v1', 'ticket': 'T-39011', 'reviewer': 'agent', 'human_approved': False,
        'note': 'Agent checks only; not human visual approval.', 'assets': review}) + b'\n')
    return {'destination': str(destination), 'assets': len(ledger)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sources', type=Path, default=ROOT / '.asset-cache/translation-assets/original-filesel-v1')
    parser.add_argument('--nbn', type=Path, default=NBN, help='FILESEL.NBN container')
    parser.add_argument('--exe', type=Path, default=EXE, help='SLPM executable holding the native font')
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--regions', type=Path, help='also write the page region map to this path')
    parser.add_argument('--publish', type=Path, help='also copy outputs, a ledger and an agent review into this new directory')
    args = parser.parse_args()
    summary, _ = build(args.sources, args.output, args.nbn, args.exe, args.regions)
    if args.publish:
        summary['published'] = publish(args.output, args.publish)
    print(json.dumps(summary))
