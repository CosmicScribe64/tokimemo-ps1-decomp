#!/usr/bin/env python3
"""English weekday lettering for the date panel (MAINROM.NBN frames 82-88).

The HUD draws the weekday frame (actor 23, frame 0x52 + weekday) inside the
bracket pair of the date panel (actor 16, frame 0x3C, part 6); see
port/godot/ui/native_hud.gd build_calendar. The weekday frame is one 16x16 part
drawn through a per-variant CLUT: base for an ordinary weekday, hud-43 for
Saturday, hud-44 for Sunday or a calendar holiday (day kind 1 or 2). The icon-51/
52/53 variants exist in the export but the HUD never draws a weekday with them;
they are still drawn here for completeness.

This tool replaces the kanji with port-made pixel lettering, in texel-index space
(translation_mainrom_drafts.Frame), so every variant is rendered from one edited
index map with its own CLUT:

* the bracket pixels are read from frame 60 part 6 and placed in weekday-cell
  coordinates from the two actor positions and the frames' origins; lettering
  and its shadow may only use cell pixels that neither are nor touch (8-neighbour)
  a bracket pixel;
* ink and shadow indices, the shadow offset (+1, +1, under the ink) and the
  baseline are derived from the seven source kanji and asserted, not assumed;
* the source lettering is cleared to index 0 and the new lettering written; every
  pixel outside the edit region equals the exact source render.

Two sets are built: 'two-letter' (Mo Tu We Th Fr Sa Su) and 'three-letter' (Mon
Tue ...). Set A is the recommended set; set B cannot separate its letters inside
the 9 usable ink columns and is flagged (faces.three-letter.abutting_words). All glyphs are new port-made lettering (user-approved creation,
2026-10-30); the ledger marks each one. Outputs are unreviewed drafts. Run in
Docker only.
"""
import argparse
import json
from pathlib import Path

from PIL import Image

from translation_assets import PACK_V1, ROOT, digest, encoded, new_output
import translation_mainrom_drafts as indexed

WEEKDAY_FRAMES = tuple(range(0x52, 0x59))
PANEL_FRAME = 0x3C
BRACKET_PART = 6
# port/godot/ui/native_hud.gd build_calendar: actor 16 draws frame 0x3C at
# (0x90, -0x12); actor 23 draws frame 0x52 + weekday at (0x92, 0x1F).
PANEL_ACTOR = (0x90, -0x12)
WEEKDAY_ACTOR = (0x92, 0x1F)
SCREEN_CENTRE = (160, 120)
DAYS = ('mon', 'tue', 'wed', 'thu', 'fri', 'sat', 'sun')
# CLUT variant -> when the HUD draws a weekday with it (native_hud.gd).
RUNTIME_USE = {'base': 'ordinary weekday', 'hud-43': 'Saturday', 'hud-44': 'Sunday or calendar holiday'}
REVISION = 'mainrom-weekday-lettering-v2'
CLEARANCE = 1

# Port-made glyphs. Rows are drawn top to bottom and bottom-aligned on BASELINE;
# 'X' is ink. Capitals and ascenders are 8 rows, the x-height 6 rows, strokes 1px.
BASELINE = 9
CAPITALS_WIDE = {
    'M': ['X...X', 'XX.XX', 'X.X.X', 'X.X.X', 'X...X', 'X...X', 'X...X', 'X...X'],
    'T': ['XXXXX'] + ['..X..'] * 7,
    'W': ['X...X'] * 4 + ['X.X.X', 'X.X.X', 'XX.XX', 'X...X'],
    'F': ['XXXX', 'X...', 'X...', 'XXX.', 'X...', 'X...', 'X...', 'X...'],
    # 4 columns like F, so Sa and Su keep a 2px gap that their capital's shadow cannot fill.
    'S': ['.XX.', 'X..X', 'X...', '.XX.', '...X', '...X', 'X..X', '.XX.'],
}
CAPITALS_NARROW = {
    'M': ['X.X', 'XXX', 'XXX', 'X.X', 'X.X', 'X.X', 'X.X', 'X.X'],
    'T': ['XXX'] + ['.X.'] * 7,
    'W': ['X.X'] * 5 + ['XXX', 'XXX', 'X.X'],
    'F': ['XXX', 'X..', 'X..', 'XX.', 'X..', 'X..', 'X..', 'X..'],
    'S': ['.XX', 'X..', 'X..', '.X.', '..X', '..X', '..X', 'XX.'],
}
LOWERCASE = {
    'a': ['XX.', '..X', '.XX', 'X.X', 'X.X', '.XX'],
    'd': ['..X', '..X', '.XX', 'X.X', 'X.X', 'X.X', 'X.X', '.XX'],
    'e': ['.X.', 'X.X', 'XXX', 'X..', 'X..', '.XX'],
    'h': ['X..', 'X..', 'XX.', 'X.X', 'X.X', 'X.X', 'X.X', 'X.X'],
    'i': ['X', '.', 'X', 'X', 'X', 'X', 'X', 'X'],
    'n': ['XX.', 'X.X', 'X.X', 'X.X', 'X.X', 'X.X'],
    'o': ['.X.', 'X.X', 'X.X', 'X.X', 'X.X', '.X.'],
    'r': ['X.X', 'XX.', 'X..', 'X..', 'X..', 'X..'],
    't': ['X.', 'XX', 'X.', 'X.', 'X.', 'X.', '.X'],
    'u': ['X.X', 'X.X', 'X.X', 'X.X', 'X.X', '.XX'],
}
# Inter-letter gap options in columns; compose uses the first that fits. The shadow
# falls at +1 column, so a 1px gap after a capital with a full right stem (M, W)
# holds that capital's shadow; a 2px gap always keeps one clean column. Set A uses
# 2px where it fits (Fr, Sa, Su) and 1px otherwise. The clear interior has 9 ink
# columns (the 10th takes the last shadow), so three letters with 1px gaps need
# widths summing to 7 or less: only Fri fits, and every other three-letter word has
# abutting letters. Set B is kept for comparison and flagged as not legible.
SETS = {
    'two-letter': {'words': ('Mo', 'Tu', 'We', 'Th', 'Fr', 'Sa', 'Su'), 'capitals': CAPITALS_WIDE, 'gap': (2, 1),
                   'recommended': True},
    'three-letter': {'words': ('Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'), 'capitals': CAPITALS_NARROW,
                     'gap': (1, 0), 'recommended': False},
}


def glyph_table(capitals):
    table = {}
    for ch, rows in list(capitals.items()) + list(LOWERCASE.items()):
        if len({len(r) for r in rows}) != 1 or set(''.join(rows)) - {'X', '.'}:
            raise ValueError('malformed glyph %r' % ch)
        table[ch] = rows
    return table


def glyph_points(rows):
    top = BASELINE - len(rows) + 1
    return [(x, top + y) for y, row in enumerate(rows) for x, c in enumerate(row) if c == 'X']


def with_shadow(points):
    """Pixel -> 'ink' or 'shadow' under the source convention: shadow at (+1, +1), under the ink."""
    ink = set(points)
    out = {(x + 1, y + 1): 'shadow' for x, y in ink}
    out.update({p: 'ink' for p in ink})
    return out


def layout(table, text, gaps):
    """Ink points of `text` from x = 0; uses the first gap in `gaps` whose width fits (caller checks)."""
    points, x = [], 0
    for ch in text:
        if ch not in table:
            raise ValueError('glyph %r missing' % ch)
        rows = table[ch]
        points += [(x + px, py) for px, py in glyph_points(rows)]
        x += len(rows[0]) + gaps
    return points


def extent(pixels):
    xs = [x for x, _ in pixels]
    return min(xs), max(xs)


# ---- Geometry -----------------------------------------------------------------

def screen_offset(actor, record, part):
    """Screen position of a part's top-left texel (hud_compositor.gd: centre + actor + part - origin)."""
    return (SCREEN_CENTRE[0] + actor[0] + part['x'] - record['origin'][0],
            SCREEN_CENTRE[1] + actor[1] + part['y'] - record['origin'][1])


def brackets(panel, records):
    """Opaque texels of the bracket part (any variant), in weekday-cell coordinates, and the part's offset."""
    panel_record = records[(PANEL_FRAME, 'base')]
    part = panel_record['parts'][BRACKET_PART]
    left, top = screen_offset(PANEL_ACTOR, panel_record, part)
    offsets = set()
    for f in WEEKDAY_FRAMES:
        record = records[(f, 'base')]
        if len(record['parts']) != 1:
            raise ValueError('weekday frame %d is not one part' % f)
        cell = screen_offset(WEEKDAY_ACTOR, record, record['parts'][0])
        offsets.add((left - cell[0], top - cell[1]))
    if len(offsets) != 1:
        raise ValueError('weekday frames are not drawn at one position')
    offset = offsets.pop()
    n = [i for i, p in enumerate(panel.parts) if p['index'] == BRACKET_PART][0]
    texels = panel.parts[n]['texels']
    pixels = {(offset[0] + x, offset[1] + y) for y, row in enumerate(texels) for x, t in enumerate(row)
              if any(panel.palettes[v][n][t] for v in indexed.VARIANTS)}
    return sorted(pixels), offset


def allowed_cells(bracket_pixels, size=16, clearance=CLEARANCE):
    """Cell pixels that neither are nor lie within `clearance` (Chebyshev) of a bracket pixel."""
    near = {(x + dx, y + dy) for x, y in bracket_pixels
            for dx in range(-clearance, clearance + 1) for dy in range(-clearance, clearance + 1)}
    return {(x, y) for y in range(size) for x in range(size) if (x, y) not in near}


def measure(bracket_pixels, rows):
    """Interior columns of the bracket pair over `rows` (cell coordinates)."""
    report = {}
    for name, clearance in (('interior', 0), ('clear', CLEARANCE)):
        free = allowed_cells(bracket_pixels, clearance=clearance)
        columns = [x for x in range(16) if all((x, y) in free for y in rows)]
        run = longest_run(columns)
        report[name] = {'columns': [run[0], run[-1]], 'width': len(run)}
    return report


def longest_run(columns):
    best, current = [], []
    for x in columns:
        current = current + [x] if current and x == current[-1] + 1 else [x]
        if len(current) > len(best):
            best = current
    return best


# ---- Source style ---------------------------------------------------------------

def source_style(frames):
    """Ink/shadow indices, shadow offset and baseline shared by the seven kanji."""
    styles = []
    for f in WEEKDAY_FRAMES:
        texels = frames[f].parts[0]['texels']
        used = {}
        for y, row in enumerate(texels):
            for x, t in enumerate(row):
                if t != indexed.CLEAR:
                    used.setdefault(t, set()).add((x, y))
        if len(used) != 2:
            raise ValueError('weekday frame %d uses %d lettering indices' % (f, len(used)))
        ink, shadow = sorted(used, key=lambda i: -len(used[i]))
        expected = {(x + 1, y + 1) for x, y in used[ink]} - used[ink]
        expected = {(x, y) for x, y in expected if x < 16 and y < 16}
        if used[shadow] != expected:
            raise ValueError('weekday frame %d shadow is not ink at (+1, +1)' % f)
        styles.append((ink, shadow, max(y for _, y in used[ink])))
    if len(set(styles)) != 1:
        raise ValueError('weekday kanji disagree on style: %r' % styles)
    ink, shadow, baseline = styles[0]
    return {'ink_index': ink, 'shadow_index': shadow, 'shadow_offset': [1, 1], 'kanji_baseline_row': baseline}


# ---- Compose --------------------------------------------------------------------

def compose(frame, text, table, gap_options, style, allowed, box):
    """Clear the kanji and draw `text`; returns (model, plan)."""
    lo, hi = box
    for gaps in gap_options:
        pixels = with_shadow(layout(table, text, gaps))
        left, right = extent(pixels)
        if right - left + 1 <= hi - lo + 1:
            break
    else:
        raise ValueError('%r does not fit the clear interior (%d columns)' % (text, hi - lo + 1))
    width = right - left + 1
    shift = lo + (hi - lo + 1 - width) // 2 - left
    pixels = {(x + shift, y): kind for (x, y), kind in pixels.items()}
    outside = sorted(p for p in pixels if p not in allowed)
    if outside:
        raise ValueError('%r touches a bracket at %r' % (text, outside))
    model = frame.copy()
    source = [(x, y) for y, row in enumerate(frame.parts[0]['texels']) for x, t in enumerate(row) if t != indexed.CLEAR]
    for x, y in source:
        model.set(0, x, y, indexed.CLEAR)
    for (x, y), kind in sorted(pixels.items(), key=lambda kv: (kv[0][1], kv[0][0])):
        indexed.put(model, x, y, style['ink_index' if kind == 'ink' else 'shadow_index'], (0,))
    region_pixels = source + list(pixels)
    xs = [x for x, _ in region_pixels]
    ys = [y for _, y in region_pixels]
    region = [min(xs), min(ys), max(xs) - min(xs) + 1, max(ys) - min(ys) + 1]
    ink = [p for p, kind in pixels.items() if kind == 'ink']
    plan = {'text': text, 'gap': gaps, 'letters_separated': gaps > 0, 'edit_regions': [region],
            'lettering_bbox': [min(x for x, _ in pixels), min(y for _, y in pixels), width,
                               max(y for _, y in pixels) - min(y for _, y in pixels) + 1],
            'ink_pixels': len(ink), 'shadow_pixels': len(pixels) - len(ink),
            'port_made_glyphs': sorted(set(text)),
            'lettering_index_map_sha256': digest(encoded(model.parts[0]['texels']))}
    return model, plan


def runtime_scope(palette_ids):
    used = [v for v in palette_ids if v in RUNTIME_USE]
    return {'drawn_by_hud': bool(used), 'uses': {v: RUNTIME_USE[v] for v in used},
            'evidence': 'port/godot/ui/native_hud.gd build_calendar weekday_color'}


def build(sources, pack_path, output, sets=tuple(SETS)):
    frames = indexed.load_frames(sources, pack_path)
    subset = {f: frames[f] for f in (PANEL_FRAME,) + WEEKDAY_FRAMES}
    hashes = indexed.gate(subset, sources)
    records_list = json.loads((sources / 'manifest.json').read_text())['images']
    records = {(r['frame_index'], r['variant']): r for r in records_list}
    bracket_pixels, panel_offset = brackets(frames[PANEL_FRAME], records)
    allowed = allowed_cells(bracket_pixels)
    style = source_style(frames)
    letter_rows = range(BASELINE - 7, BASELINE + 2)
    geometry = measure(bracket_pixels, letter_rows)
    box = geometry['clear']['columns']
    users = {r['frame_index']: indexed.record_keys(r) for r in records_list if r['variant'] == 'base'}
    output = new_output(output)
    prepared, faces = [], {}
    for set_name in sets:
        spec = SETS[set_name]
        table = glyph_table(spec['capitals'])
        drafted = {}
        for f, text in zip(WEEKDAY_FRAMES, spec['words']):
            drafted[f] = compose(frames[f], text, table, spec['gap'], style, allowed, box)
        scopes = indexed.sheet_scope(drafted, frames, users)
        used = sorted({ch for text in spec['words'] for ch in text})
        faces[set_name] = {'name': 'weekday-port-%s' % set_name, 'origin': 'port-made',
                           'authorization': 'User approved new pixel lettering for the weekday frames, 2026-10-30.',
                           'baseline_row': BASELINE, 'shadow': 'source convention (+1, +1) under ink',
                           'glyphs': {ch: {'rows': table[ch], 'origin': 'port-made'} for ch in used},
                           'recommended': spec['recommended'],
                           'abutting_words': [plan['text'] for _, plan in sorted(drafted.values(), key=lambda mp: mp[1]['text'])
                                              if not plan['letters_separated']]}
        for f, (model, plan) in sorted(drafted.items()):
            day = DAYS[f - WEEKDAY_FRAMES[0]]
            for sha, palette_ids in indexed.variant_groups(hashes, f):
                original = Image.open(sources / ('mainrom.frame-%03d.%s.png' % (f, palette_ids[0]))).convert('RGBA')
                image = indexed.render_group(model, palette_ids)
                indexed.check(original, image, plan['edit_regions'], 'frame %d %s' % (f, palette_ids[0]))
                name = '%s/mainrom-weekday-%s-%s.png' % (set_name, day, palette_ids[0])
                row = {'id': 'mainrom-weekday-%s-%s.%s' % (set_name, day, palette_ids[0]), 'set': set_name,
                       'frame': f, 'weekday': day, 'palette_id': palette_ids[0], 'palette_ids': palette_ids,
                       'source_container': indexed.CONTAINER, 'source_container_sha256': indexed.MAINROM_SHA,
                       'source_png_sha256': sha, 'dimensions': [model.width, model.height], 'output': name,
                       'runtime_variant_scope': runtime_scope(palette_ids), 'sheet_scope': scopes[f],
                       'style': 'unreviewed', 'human_approved': False, 'runtime_integrated': False,
                       'in_game_verified': False, 'glyph_origin': 'port-made', **plan}
                prepared.append((image, original, row))
    output.mkdir(parents=True)
    for set_name in sets:
        (output / set_name).mkdir()
    for image, _, row in prepared:
        image.save(output / row['output'])
        row['output_sha256'] = digest((output / row['output']).read_bytes())
    manifest = {'schema': 'tokimemo.mainrom-weekday-drafts.v1', 'revision': REVISION, 'human_approved': False,
                'source_container': indexed.CONTAINER, 'source_container_sha256': indexed.MAINROM_SHA,
                'decode_gate': {'frames': sorted(subset), 'variants': list(indexed.VARIANTS),
                                'renders_byte_identical': len(hashes)},
                'geometry': {'bracket_frame': PANEL_FRAME, 'bracket_part': BRACKET_PART,
                             'bracket_part_offset_in_cell': list(panel_offset),
                             'bracket_pixels_in_cell': [list(p) for p in bracket_pixels if 0 <= p[0] < 16 and 0 <= p[1] < 16],
                             'letter_rows': [letter_rows[0], letter_rows[-1]], 'clearance': CLEARANCE, **geometry},
                'source_style': style, 'faces': faces, 'assets': [row for _, _, row in prepared]}
    (output / 'manifest.json').write_bytes(encoded(manifest) + b'\n')
    ledger = {'schema': 'tokimemo.translation-checkpoint.v1', 'revision': REVISION, 'human_approved': False,
              'port_made_glyphs': {s: sorted(faces[s]['glyphs']) for s in sets},
              'assets': [{'id': row['id'], 'path': row['output'], 'output_sha256': row['output_sha256'],
                          'frame': row['frame'], 'palette_ids': row['palette_ids'],
                          'source_png_sha256': row['source_png_sha256'], 'status': 'draft',
                          'glyph_origin': 'port-made', 'port_made_glyphs': row['port_made_glyphs']}
                         for _, _, row in prepared]}
    (output / 'ledger.json').write_bytes(encoded(ledger) + b'\n')
    for set_name in sets:
        runtime_pack(output, set_name, [row for _, _, row in prepared if row['set'] == set_name])
    indexed.contact_sheet([(o, i, r) for i, o, r in prepared], output.parent / (output.name + '-contact-6x.png'))
    return {'output': str(output), 'drafts': len(prepared), 'gated_renders': len(hashes), 'geometry': geometry}


def runtime_pack(output, set_name, rows):
    """A development translated-image pack (services/translated_image_store.gd) for one set."""
    assets = []
    for row in rows:
        for palette in row['palette_ids']:
            if palette not in RUNTIME_USE:
                continue
            assets.append({'id': '%s.%s' % (row['id'], palette), 'frame': row['frame'], 'palette': palette,
                           'source_container': row['source_container'],
                           'source_container_sha256': row['source_container_sha256'],
                           'source_png_sha256': row['source_png_sha256'], 'dimensions': row['dimensions'],
                           'output': row['output'], 'output_sha256': row['output_sha256'], 'style': 'unreviewed',
                           'glyph_origin': 'port-made'})
    pack = {'schema': PACK_V1, 'language': 'en', 'development_drafts': True, 'snapshot_sha256': '0' * 64,
            'assets': assets}
    (output / ('pack-%s.json' % set_name)).write_bytes(encoded(pack) + b'\n')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--sources', type=Path, default=ROOT / '.asset-cache/translation-assets/original-mainrom-v1')
    parser.add_argument('--pack', type=Path, default=ROOT / 'disc/files/CDROM/FUKUTEST/MAINROM.NBN')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(build(args.sources, args.pack, args.output)))
