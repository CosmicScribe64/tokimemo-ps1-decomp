#!/usr/bin/env python3
"""T-39009: indexed English drafts for the missing MAINROM.NBN HUD and magazine frames.

MAINROM frames are 4bpp sheet texels drawn through per-part CLUTs; the palette
variants (base, hud-43/44, icon-51/52/53) differ only in the CLUT. Every edit is
therefore made in texel-index space, once per frame, and each source variant is
rendered from the edited indices with that variant's own CLUT words:

* lift: existing English art (reviewed batch-002 weekdays, T39004-pass2 rebinds)
  is converted back to indices at RGB555 precision, intersected across all English
  variants of a frame, and re-rendered for the variants that still lack English;
* clear: lettering texels inside a declared region are set to index 0, which is
  transparent in every variant (asserted), restoring the transparent background;
* text: lettering from an established face only (panel7 recovered from the
  frame-61 rebind, the authenticated native executable font, or the frame's own
  original digit/dot texels), unscaled, with ink/shadow/outline indices taken from
  the frame's own lettering.

Every changed texel is mapped back to its sheet address: drafts that disagree on a
texel are refused, and each draft records whether it is a sheet patch or only a whole-
frame replacement (overlay texels, or texels other frames draw unedited).

Outputs are unreviewed draft candidates (human_approved false). Pixels outside the
declared edit regions equal the exact source render, which is itself gated
byte-for-byte against the original export. Run in Docker only.
"""
import argparse
from collections import Counter
import json
from pathlib import Path

from PIL import Image

from translation_assets import ROOT, digest, encoded, inside, new_output
from translation_native_font import NativeLatin

MAINROM_SHA = 'f425e1d19d64afcbafeadece7d72602e50e26ce88093047f73dbb0d21a9b9c1c'
CONTAINER = 'CDROM/FUKUTEST/MAINROM.NBN'
VARIANTS = ('base', 'hud-43', 'hud-44', 'icon-51', 'icon-52', 'icon-53')
SNAPSHOT = ROOT / 'translations/en/graphics/artwork'
PASS2 = SNAPSHOT / 'checkpoints/T39004-pass2'
REVISION = 'mainrom-indexed-drafts-v1'
# Frames of the 42 missing MAINROM inventory items; all of them pass the decode gate.
GATED_FRAMES = (57, 58, 59, 60, 61, 82, 83, 84, 85, 86, 87, 88, 118) + tuple(range(121, 146)) + (151, 152, 153, 154)
PANEL7_INK = (165, 206, 206, 255)
CLEAR = 0

# Path A: frames whose English lettering already exists for some variants.
LIFT_FRAMES = (61, 82, 83, 84, 85, 86, 87, 88, 126)
# Frame 61's English labels extend left of the label-strip parts into pixels no part
# covers. Those pixels go to a bottom overlay layer drawn with the strip parts' CLUT
# (parts 25-27 share one CLUT in every variant; asserted).
OVERLAY_CLUT = {61: (25, 26, 27)}
PASS2_REBINDS = {61: 'mainrom-frame-0061-base.png', 82: 'mainrom-frame-0082-base.png', 126: 'mainrom-frame-0126-base.png'}

# Paths B-F. Rects are frame-relative [x, y, w, h]. 'ink' is an index, or 'ramp'
# (per-row index of the cleared source lettering). Wording defaults are recorded
# as revisable decisions in translations/en/graphics/decisions/mainrom.json.
HEADLINE = {127: 'Museum Opens', 128: 'Aquarium', 129: 'New Stadium', 130: 'Garden Guide',
            131: 'Bowling', 132: 'Karaoke Opens', 133: 'Planetarium'}
# Catalog ink/shadow suggestions, used only as a cross-check of the derived indices.
CATALOG_INK = {127: (7, 9), 128: (12, 14), 129: (2, 4), 131: (12, 14), 132: (2, 4), 133: (7, 9)}
TAGS = {136: 'Action', 137: 'Suspense', 140: 'Classic'}
BADGE_KEEP = {122: '3・4・5', 123: '6・7・8', 124: '9・10・11', 125: '12・1・2'}
BARS = {151: "Out in '95", 152: 'Out Now!', 153: 'Out Now!', 154: 'Out Now!'}
SAVE_FRAMES = (57, 58, 59)
# 'Save' with outline, part-local [x, y, w, h] in the part-1 lettering layer: below the
# card label, placed to leave the fewest ink pixels under the card in all three bounces.
SAVE_RECT = (0, 6, 22, 9)


def color(word):
    if word == 0:
        return (0, 0, 0, 0)
    r, g, b = word & 31, (word >> 5) & 31, (word >> 10) & 31
    return (r << 3 | r >> 2, g << 3 | g >> 2, b << 3 | b >> 2, 255)


def quantize(pixel):
    """RGBA -> RGB555 key (None for transparent); the exporter expands q to q<<3|q>>2."""
    if pixel[3] == 0:
        return None
    return tuple(round(c * 31 / 255) for c in pixel[:3])


def key(word):
    return None if word == 0 else (word & 31, (word >> 5) & 31, (word >> 10) & 31)


def record_sheet(part):
    """Sheet address of a manifest part record."""
    return {'offset': part['sheet_source_offset'], 'u': part['u'], 'v': part['v'], 'flip': part['flip_flags']}


def sheet_key(sheet, w, h, x, y):
    """(sheet offset, row, column) of local texel (x, y) of a w x h part drawn from `sheet`."""
    sy = h - y - 1 if sheet['flip'] & 1 else y
    sx = w - x - 1 if sheet['flip'] & 2 else x
    return sheet['offset'], sheet['v'] + sy, sheet['u'] + sx


def record_keys(record):
    """Every sheet texel a manifest image record draws."""
    return {sheet_key(record_sheet(p), p['width'], p['height'], x, y)
            for p in record['parts'] for y in range(p['height']) for x in range(p['width'])}


class Frame:
    """Per-frame texel layers (top part first) with per-variant CLUTs."""

    def __init__(self, frame, images, pack):
        base = images[(frame, 'base')]
        self.frame = frame
        self.width, self.height = base['width'], base['height']
        left, top = base['bounds'][:2]
        self.parts = []
        for part in base['parts']:
            sheet = record_sheet(part)
            texels = [[0] * part['width'] for _ in range(part['height'])]
            for y in range(part['height']):
                for x in range(part['width']):
                    offset, row, column = sheet_key(sheet, part['width'], part['height'], x, y)
                    t = row * 128 + column
                    texels[y][x] = (pack[offset + t // 2] >> (4 * (t & 1))) & 15
            self.parts.append({'index': part['part_index'], 'x': part['x'] - left, 'y': part['y'] - top,
                               'w': part['width'], 'h': part['height'], 'sheet': sheet, 'texels': texels})
        self.palettes = {}
        for variant in VARIANTS:
            record = images[(frame, variant)]
            if [(p['part_index'], p['x'], p['y'], p['width'], p['height']) for p in record['parts']] != \
               [(p['part_index'], p['x'], p['y'], p['width'], p['height']) for p in base['parts']]:
                raise ValueError('variant geometry differs: frame %d %s' % (frame, variant))
            self.palettes[variant] = [p['preview_palette_words'] for p in record['parts']]
        self.check_clear()

    def check_clear(self):
        """Index 0 must be transparent in every CLUT, so clearing restores the background."""
        for variant, palette in self.palettes.items():
            if any(words[CLEAR] != 0 for words in palette):
                raise ValueError('clear index is not transparent: frame %d %s' % (self.frame, variant))

    @classmethod
    def from_layers(cls, frame, width, height, parts, palettes):
        """Build a frame directly from part layers (top first) and per-variant CLUT lists."""
        model = object.__new__(cls)
        model.frame, model.width, model.height = frame, width, height
        model.parts = [{**p, 'texels': [row[:] for row in p['texels']]} for p in parts]
        model.palettes = {v: [list(words) for words in palettes[v]] for v in palettes}
        model.check_clear()
        return model

    def add_overlay(self, clut_parts):
        """Append a bottom layer over the whole frame, all CLEAR, using a shared part CLUT."""
        for variant in VARIANTS:
            if len({tuple(self.palettes[variant][n]) for n in clut_parts}) != 1:
                raise ValueError('overlay CLUT parts differ: frame %d %s' % (self.frame, variant))
        self.parts.append({'index': 'overlay', 'clut_parts': list(clut_parts), 'x': 0, 'y': 0, 'w': self.width, 'h': self.height,
                           'texels': [[CLEAR] * self.width for _ in range(self.height)]})
        self.palettes = {v: pal + [pal[clut_parts[0]]] for v, pal in self.palettes.items()}

    def copy(self):
        other = object.__new__(Frame)
        other.__dict__.update(self.__dict__)
        other.parts = [{**p, 'texels': [row[:] for row in p['texels']]} for p in self.parts]
        return other

    def covering(self, x, y):
        """Part numbers covering frame pixel (x, y), top first."""
        return [n for n, p in enumerate(self.parts) if p['x'] <= x < p['x'] + p['w'] and p['y'] <= y < p['y'] + p['h']]

    def texel(self, n, x, y):
        p = self.parts[n]
        return p['texels'][y - p['y']][x - p['x']]

    def set(self, n, x, y, index):
        p = self.parts[n]
        p['texels'][y - p['y']][x - p['x']] = index

    def word(self, variant, x, y):
        for n in self.covering(x, y):
            w = self.palettes[variant][n][self.texel(n, x, y)]
            if w:
                return w
        return 0

    def render(self, variant):
        image = Image.new('RGBA', (self.width, self.height), (0, 0, 0, 0))
        image.putdata([color(self.word(variant, x, y)) for y in range(self.height) for x in range(self.width)])
        return image

    def visible(self, x, y):
        """(part, index) shown at (x, y) in the base variant, or None."""
        for n in self.covering(x, y):
            i = self.texel(n, x, y)
            if self.palettes['base'][n][i]:
                return n, i
        return None


def load_frames(sources, pack_path):
    pack = pack_path.read_bytes()
    if digest(pack) != MAINROM_SHA:
        raise ValueError('MAINROM.NBN identity differs')
    manifest = json.loads((sources / 'manifest.json').read_text())
    if manifest['metadata']['source_sha256'] != MAINROM_SHA:
        raise ValueError('source export was made from a different MAINROM.NBN')
    images = {(i['frame_index'], i['variant']): i for i in manifest['images']}
    return {f: Frame(f, images, pack) for f in GATED_FRAMES}


def gate(frames, sources):
    """Every variant of every gated frame must reproduce the original export exactly."""
    hashes = {}
    for f, frame in frames.items():
        for variant in VARIANTS:
            path = sources / ('mainrom.frame-%03d.%s.png' % (f, variant))
            original = Image.open(path).convert('RGBA')
            if list(frame.render(variant).getdata()) != list(original.getdata()):
                raise ValueError('decoder does not reproduce frame %d %s' % (f, variant))
            hashes[(f, variant)] = digest(path.read_bytes())
    return hashes


def inside_regions(regions, x, y):
    return any(a <= x < a + w and b <= y < b + h for a, b, w, h in regions)


# ---- Path A: lift English art back to indices --------------------------------

def lift(frame, english, regions):
    """Return a frame whose texels reproduce every English variant at RGB555.

    `english` maps variant -> RGBA image. Pixels outside `regions` must already match
    the source. Each changed pixel gets the one index, intersected across the
    variants, of the highest covering part whose CLUT has the colour (parts above it
    become transparent there); a transparent target clears every covering part.
    """
    result = frame.copy()
    for y in range(frame.height):
        for x in range(frame.width):
            wanted = {v: quantize(img.getpixel((x, y))) for v, img in english.items()}
            if all(key(frame.word(v, x, y)) == q for v, q in wanted.items()):
                continue
            if not inside_regions(regions, x, y):
                raise ValueError('English differs from source outside edit regions: frame %d (%d,%d)' % (frame.frame, x, y))
            parts = frame.covering(x, y)
            if not parts:
                raise ValueError('English pixel outside every part: frame %d (%d,%d)' % (frame.frame, x, y))
            if all(q is None for q in wanted.values()):
                for n in parts:
                    result.set(n, x, y, CLEAR)
                continue
            if any(q is None for q in wanted.values()):
                raise ValueError('variants disagree on transparency: frame %d (%d,%d)' % (frame.frame, x, y))
            for depth, n in enumerate(parts):
                candidates = {i for i in range(16)
                              if all(key(frame.palettes[v][n][i]) == q for v, q in wanted.items())}
                if frame.texel(n, x, y) in candidates:
                    choice = {frame.texel(n, x, y)}
                else:
                    owners = frame.parts[n].get('clut_parts', [n])
                    choice = candidates & {i for m in owners for row in frame.parts[m]['texels'] for i in row}
                if len(choice) > 1:
                    raise ValueError('ambiguous source index for English pixel: frame %d (%d,%d) %s'
                                     % (frame.frame, x, y, sorted(choice)))
                if choice:
                    # Parts above the one carrying the colour become transparent here.
                    for above in parts[:depth]:
                        result.set(above, x, y, CLEAR)
                    result.set(n, x, y, choice.pop())
                    break
            else:
                raise ValueError('no source index for English pixel: frame %d (%d,%d)' % (frame.frame, x, y))
    for v, img in english.items():
        rendered = result.render(v)
        if [quantize(p) for p in rendered.getdata()] != [quantize(p) for p in img.getdata()]:
            raise ValueError('lifted indices do not reproduce English variant: frame %d %s' % (frame.frame, v))
    return result


def english_inputs(snapshot=SNAPSHOT, pass2=PASS2):
    """frame -> list of (variant, path, sha, asset id, edit regions, status)."""
    manifest = json.loads((snapshot / 'manifest.json').read_text())
    inputs = {}
    for version in manifest['versions']:
        if not version.get('current'):
            continue
        for asset in version['assets']:
            if asset['id'].startswith('mainrom-weekday-'):
                path = inside(snapshot, asset['output'])
                if digest(path.read_bytes()) != asset['output_sha256']:
                    raise ValueError('weekday art changed: ' + asset['id'])
                inputs.setdefault(int(asset['frame']), []).append(
                    (asset['palette_id'], path, asset['output_sha256'], asset['id'],
                     [[int(c) for c in r] for r in asset['edit_regions']], 'english-reviewed'))
    ledger = json.loads((pass2 / 'ledger.json').read_text())
    by_path = {a['path']: a for a in ledger['assets']}
    rebind = json.loads((pass2 / 'legacy-rebind-v1/manifest.json').read_text())
    for row in rebind['assets']:
        if row['frame'] not in PASS2_REBINDS:
            continue
        entry = by_path['legacy-rebind-v1/' + row['output']]
        path = pass2 / entry['path']
        if digest(path.read_bytes()) != entry['output_sha256']:
            raise ValueError('pass2 rebind changed: ' + row['id'])
        inputs.setdefault(row['frame'], []).append(
            (row['palette_id'], path, entry['output_sha256'], row['id'], row['edit_regions'], 'candidate-only'))
    return inputs


# ---- Faces ------------------------------------------------------------------

def panel7_face(snapshot=SNAPSHOT, pass2=PASS2):
    """Recover panel7 glyph masks from the frame-61 rebind by column-run segmentation."""
    manifest = json.loads((snapshot / 'manifest.json').read_text())
    asset = [a for v in manifest['versions'] if v.get('current') for a in v['assets']
             if a['id'] == 'parameters-panel-296x67'][0]
    path = pass2 / 'legacy-rebind-v1' / PASS2_REBINDS[61]
    image = Image.open(path).convert('RGBA')
    glyphs, witness = {}, {}
    for layout in asset['layout']:
        if layout.get('font_id') != 'panel7':
            raise ValueError('frame-61 layout is not panel7')
        ox, oy = layout['origin']
        w, h = layout['size']
        columns = [any(image.getpixel((x, y)) == PANEL7_INK for y in range(oy, oy + h)) for x in range(ox, ox + w)]
        runs, start = [], None
        for i, ink in enumerate(columns + [False]):
            if ink and start is None:
                start = i
            if not ink and start is not None:
                runs.append((start, i))
                start = None
        letters = layout['text'].replace(' ', '')
        if len(runs) != len(letters):
            raise ValueError('panel7 segmentation failed: ' + layout['text'])
        for ch, (a, b) in zip(letters, runs):
            points = tuple((x - a, y) for y in range(h) for x in range(a, b)
                           if image.getpixel((ox + x, oy + y)) == PANEL7_INK)
            glyph = {'points': list(points), 'width': b - a}
            if ch in glyphs and glyphs[ch] != glyph:
                raise ValueError('panel7 glyph differs between witnesses: ' + ch)
            if ch not in glyphs:
                glyphs[ch] = glyph
                witness[ch] = {'text': layout['text'], 'rect': [ox + a, oy, b - a, h]}
    return Face('panel7', glyphs, space=5, provenance={
        'source': 'checkpoints/T39004-pass2/legacy-rebind-v1/' + PASS2_REBINDS[61],
        'source_output_sha256': digest(path.read_bytes()), 'ink_rgba': list(PANEL7_INK),
        'segmentation': 'column runs of ink inside each frame-61 layout box', 'rescaled': False,
        'new_glyphs': [], 'witness': witness})


class Face:
    def __init__(self, name, glyphs, space, provenance):
        self.name, self.glyphs, self.space, self.provenance = name, glyphs, space, provenance

    def proof(self):
        return self.provenance

    def glyph(self, ch):
        if ch == ' ':
            return {'points': [], 'width': self.space}
        if ch not in self.glyphs:
            raise ValueError('glyph %r missing from face %s' % (ch, self.name))
        return self.glyphs[ch]

    def points(self, text):
        pts, x = [], 0
        for ch in text:
            g = self.glyph(ch)
            pts += [(px + x, py) for px, py in g['points']]
            x += g['width'] + 1
        return pts


class NativeFace(Face):
    """The authenticated executable font (12 source rows, unscaled)."""

    def __init__(self, executable):
        self.font = NativeLatin(executable)
        super().__init__('native', {}, 4, None)

    def glyph(self, ch):
        return self.font.glyph(ch)

    def proof(self):
        return self.font.provenance()


# ---- Paths B-F: index-space lettering ----------------------------------------

def lettering(frame, rect):
    """Visible non-clear (part, index) pixels of the source inside rect."""
    x0, y0, w, h = rect
    out = {}
    for y in range(y0, y0 + h):
        for x in range(x0, x0 + w):
            v = frame.visible(x, y)
            if v and v[1] != CLEAR:
                out[(x, y)] = v
    return out


def clear(frame, rect):
    x0, y0, w, h = rect
    for y in range(y0, y0 + h):
        for x in range(x0, x0 + w):
            for n in frame.covering(x, y):
                frame.set(n, x, y, CLEAR)


def ramp(source_pixels, rows):
    """Per-row most common lettering index; empty rows take the nearest lettered row (upper on ties)."""
    by_row = {}
    for (x, y), (n, i) in source_pixels.items():
        by_row.setdefault(y, Counter())[i] += 1
    known = sorted(by_row)
    table = {}
    for y in rows:
        nearest = min(known, key=lambda r: (abs(r - y), r))
        table[y] = sorted(by_row[nearest].items(), key=lambda kv: (-kv[1], kv[0]))[0][0]
    return table


def put(frame, x, y, index, clut_parts, part=None, occluders=(), occluded=None):
    """Write one lettering index at frame pixel (x, y) and prove it shows.

    The index goes into `part`, or the top covering part. It was read from
    `clut_parts`, so the target part must use the same CLUT in every variant. A pixel
    no part covers is clipping; a pixel an upper part hides is refused unless the
    hiding part is one of `occluders`, in which case it is appended to `occluded`.
    """
    parts = frame.covering(x, y) if 0 <= x < frame.width and 0 <= y < frame.height else []
    target = part if part is not None else (parts[0] if parts else None)
    if target not in parts:
        raise ValueError('lettering clipped at (%d,%d) in frame %d' % (x, y, frame.frame))
    for variant, palette in frame.palettes.items():
        if any(palette[target] != palette[n] for n in clut_parts):
            raise ValueError('part %r CLUT differs from source parts %r: frame %d %s'
                             % (target, sorted(clut_parts), frame.frame, variant))
    frame.set(target, x, y, index)
    shown = frame.visible(x, y)
    if shown == (target, index):
        return
    if shown is not None and shown[0] in occluders and shown[0] in parts[:parts.index(target)] and occluded is not None:
        occluded.append((x, y, index))
        return
    raise ValueError('lettering pixel hidden at (%d,%d) in frame %d' % (x, y, frame.frame))


def stamp(frame, face, text, rect, ink, clut_parts, shadow=None, outline=None, ramp_table=None, **target):
    """Draw text centred in rect; returns the written bbox. `target` is passed to put()."""
    pts = face.points(text)
    if not pts:
        raise ValueError('empty lettering')
    left, right = min(x for x, _ in pts), max(x for x, _ in pts)
    top, bottom = min(y for _, y in pts), max(y for _, y in pts)
    margin_l = margin_t = 1 if outline is not None else 0
    margin_r = margin_b = 1 if (outline is not None or shadow is not None) else 0
    full_w = right - left + 1 + margin_l + margin_r
    full_h = bottom - top + 1 + margin_t + margin_b
    x0, y0, w, h = rect
    if full_w > w or full_h > h:
        raise ValueError('lettering %r (%dx%d) does not fit %r unscaled' % (text, full_w, full_h, rect))
    ox = x0 + (w - full_w) // 2 + margin_l - left
    oy = y0 + (h - full_h) // 2 + margin_t - top
    ink_px = {(ox + x, oy + y) for x, y in pts}
    writes = {}
    if outline is not None:
        for x, y in ink_px:
            for ddx in (-1, 0, 1):
                for ddy in (-1, 0, 1):
                    writes[(x + ddx, y + ddy)] = outline
    if shadow is not None:
        for x, y in ink_px:
            writes[(x + 1, y + 1)] = shadow
    for x, y in ink_px:
        writes[(x, y)] = ramp_table[y] if ink == 'ramp' else ink
    for (x, y), index in sorted(writes.items(), key=lambda kv: (kv[0][1], kv[0][0])):
        put(frame, x, y, index, clut_parts, **target)
    xs = [x for x, _ in writes]
    ys = [y for _, y in writes]
    return [min(xs), min(ys), max(xs) - min(xs) + 1, max(ys) - min(ys) + 1]


def dominant(pixels, count):
    tally = Counter(i for _, i in pixels.values())
    return [i for i, _ in sorted(tally.items(), key=lambda kv: (-kv[1], kv[0]))[:count]]


def source_parts(pixels):
    return {n for n, _ in pixels.values()}


def plan_headline(frame, native):
    rect = [0, 0, frame.width, frame.height]
    source = lettering(frame, rect)
    ink, shadow = dominant(source, 2)
    expected = CATALOG_INK.get(frame.frame)
    if expected and expected != (ink, shadow):
        raise ValueError('derived ink/shadow %r differs from catalog %r: frame %d' % ((ink, shadow), expected, frame.frame))
    clear(frame, rect)
    box = stamp(frame, native, HEADLINE[frame.frame], rect, ink, source_parts(source), shadow=shadow)
    return {'path': 'B', 'text': HEADLINE[frame.frame], 'face': 'native', 'ink': ink, 'shadow': shadow,
            'edit_regions': [rect], 'lettering_bbox': box}


def plan_badge_121(frame, native):
    rect = [0, 0, frame.width, frame.height]
    source = lettering(frame, rect)
    table = ramp(source, range(frame.height))
    clear(frame, rect)
    box = stamp(frame, native, 'No.1', rect, 'ramp', source_parts(source), ramp_table=table)
    return {'path': 'B', 'text': 'No.1', 'face': 'native', 'ink': 'ramp', 'ramp': [table[y] for y in range(frame.height)],
            'edit_regions': [rect], 'lettering_bbox': box}


def column_runs(pixels):
    columns = sorted({x for x, _ in pixels})
    runs, start, prev = [], None, None
    for x in columns:
        if start is None:
            start = x
        elif x != prev + 1:
            runs.append((start, prev + 1))
            start = x
        prev = x
    if start is not None:
        runs.append((start, prev + 1))
    return runs


def plan_badge_digits(frame):
    """Keep the original digit and dot texels, drop the trailing 月号 runs, recentre."""
    rect = [0, 0, frame.width, frame.height]
    source = lettering(frame, rect)
    runs = column_runs(source)
    keep_text = BADGE_KEEP[frame.frame]
    # One run per digit or dot, except adjacent digits that touch (none do here).
    kept = runs[:len(keep_text)]
    dropped = runs[len(keep_text):]
    if not dropped or sum(b - a for a, b in dropped) < 14:
        raise ValueError('月号 runs not found after the numerals: frame %d %r' % (frame.frame, runs))
    a, b = kept[0][0], kept[-1][1]
    shift = (frame.width - (b - a)) // 2 - a
    moved = {(x + shift, y): v for (x, y), v in source.items() if a <= x < b}
    clear(frame, rect)
    for (x, y), (n, i) in sorted(moved.items(), key=lambda kv: (kv[0][1], kv[0][0])):
        put(frame, x, y, i, {n})
    return {'path': 'C', 'text': keep_text, 'face': 'original-texels', 'kept_runs': [list(r) for r in kept],
            'dropped_runs': [list(r) for r in dropped], 'shift': shift, 'edit_regions': [rect],
            'glyph_provenance': {'source': 'original MAINROM digit/dot texels of this frame', 'new_glyphs': []}}


def plan_tag(frame, panel7):
    rect = [0, 0, frame.width, frame.height]
    source = lettering(frame, rect)
    (ink,) = dominant(source, 1)
    clear(frame, rect)
    box = stamp(frame, panel7, TAGS[frame.frame], rect, ink, source_parts(source))
    return {'path': 'D', 'text': TAGS[frame.frame], 'face': 'panel7', 'ink': ink, 'shadow': None,
            'edit_regions': [rect], 'lettering_bbox': box}


def plan_bar(frame, native, strip_top):
    rect = [0, strip_top, frame.width, frame.height - strip_top]
    for n, p in enumerate(frame.parts):
        overlaps = p['y'] < rect[1] + rect[3] and p['y'] + p['h'] > rect[1]
        if overlaps and p['y'] < rect[1]:
            raise ValueError('logo part reaches into the bar strip: frame %d part %d' % (frame.frame, n))
    source = lettering(frame, rect)
    outline, = dominant(source, 1)
    fill = {k: v for k, v in source.items() if v[1] != outline}
    table = ramp(fill, range(rect[1], rect[1] + rect[3]))
    clear(frame, rect)
    box = stamp(frame, native, BARS[frame.frame], rect, 'ramp', source_parts(source), outline=outline, ramp_table=table)
    return {'path': 'F', 'text': BARS[frame.frame], 'face': 'native', 'ink': 'ramp', 'outline': outline,
            'ramp': [table[y] for y in range(rect[1], rect[1] + rect[3])], 'edit_regions': [rect],
            'lettering_bbox': box, 'logo': 'kept unchanged (deliberately untranslated)'}


def plan_save(frame, panel7, card_frame):
    """57-59: part 0 is frame 56's card, drawn over part 1, the bouncing セーブ中 lettering.

    'Save' is written into part 1 only, at the same part-local place in all three frames,
    so the shared lettering texels agree and the card texels (shared with frame 56) are
    untouched. The card stays on top, as in the source, whose lettering it also partly
    hides; lettering pixels under opaque card texels are counted, nothing else may hide them.
    """
    card, letters = frame.parts[0], frame.parts[1]
    if card['texels'] != card_frame.parts[0]['texels']:
        raise ValueError('frame %d part 0 is not the frame-56 card' % frame.frame)
    region = [letters['x'], letters['y'], letters['w'], letters['h']]
    source = {k: v for k, v in lettering(frame, region).items() if v[0] == 1}
    outline, ink = dominant(source, 2)
    source_hidden = sum(1 for y, row in enumerate(letters['texels']) for x, t in enumerate(row)
                        if t != CLEAR and frame.visible(letters['x'] + x, letters['y'] + y) != (1, t))
    for row in letters['texels']:
        row[:] = [CLEAR] * len(row)
    x, y, w, h = SAVE_RECT
    occluded = []
    box = stamp(frame, panel7, 'Save', [letters['x'] + x, letters['y'] + y, w, h], ink, {1},
                outline=outline, part=1, occluders=(0,), occluded=occluded)
    return {'path': 'E', 'text': 'Save', 'face': 'panel7', 'ink': ink, 'outline': outline,
            'bounce_dy': letters['y'] - card['y'], 'written_part': 1, 'part_local_rect': list(SAVE_RECT),
            'hidden_under_card': {'ink': sum(1 for *_, i in occluded if i == ink),
                                  'outline': sum(1 for *_, i in occluded if i != ink),
                                  'source_lettering_pixels_hidden': source_hidden},
            'source_reading': 'セーブ中 (Saving) per pixels; catalog セーブ不可 is wrong',
            'edit_regions': [region], 'lettering_bbox': box}


# ---- Build ------------------------------------------------------------------

def variant_groups(frame_hashes, frame):
    """Distinct source PNG identities of a frame -> palette ids sharing it."""
    groups = {}
    for v in VARIANTS:
        groups.setdefault(frame_hashes[(frame, v)], []).append(v)
    return [(sha, ids) for sha, ids in groups.items()]


def applicability(frame, palette_ids, model):
    """Port draw scope (tools/port/hud_content.py) plus the CLUT evidence for it."""
    scoped = [v for v in palette_ids if v == 'base' or (v.startswith('hud-') and 0x44 <= frame < 0x59)
              or (v.startswith('icon-') and frame == 2)]
    words = sorted({w for v in palette_ids for pal in model.palettes[v] for w in pal if w})
    return {'runtime_applicable': bool(scoped),
            'evidence': 'tools/port/hud_content.py exports hud-43/44 only for frames 0x44..0x58 and icon-51/52/53 only for frame 2',
            'clut_opaque_words': words, 'clut_opaque_colours': len(words),
            'requirement_kept': True}


def compose_all(frames, inputs, panel7, native):
    """Return {frame: (edited Frame, plan dict)} for every drafted frame."""
    out = {}
    for f in LIFT_FRAMES:
        english = {}
        regions = []
        derived = []
        for variant, path, sha, asset_id, edit_regions, status in sorted(inputs[f], key=lambda r: (r[0], r[3])):
            english[variant] = Image.open(path).convert('RGBA')
            regions += [r for r in edit_regions if r not in regions]
            derived.append({'asset_id': asset_id, 'palette_id': variant, 'output_sha256': sha, 'status': status})
        base = frames[f]
        if f in OVERLAY_CLUT:
            base = base.copy()
            base.add_overlay(OVERLAY_CLUT[f])
        model = lift(base, english, regions)
        index_map = encoded([p['texels'] for p in model.parts])
        out[f] = (model, {'path': 'A', 'edit_regions': regions, 'derived_from': derived,
                          'overlay_clut_parts': list(OVERLAY_CLUT.get(f, ())),
                          'lettering_index_map_sha256': digest(index_map), 'covered_variants': sorted(english)})
    for f in HEADLINE:
        model = frames[f].copy()
        out[f] = (model, plan_headline(model, native))
    model = frames[121].copy()
    out[121] = (model, plan_badge_121(model, native))
    for f in BADGE_KEEP:
        model = frames[f].copy()
        out[f] = (model, plan_badge_digits(model))
    for f in TAGS:
        model = frames[f].copy()
        out[f] = (model, plan_tag(model, panel7))
    for f in BARS:
        model = frames[f].copy()
        out[f] = (model, plan_bar(model, native, strip_top=50))
    card = frames[56]
    for f in SAVE_FRAMES:
        model = frames[f].copy()
        out[f] = (model, plan_save(model, panel7, card))
    return out


def render_group(model, palette_ids):
    """Render a group of variants whose source PNGs are identical; all must render alike."""
    image = model.render(palette_ids[0])
    for v in palette_ids[1:]:
        if list(model.render(v).getdata()) != list(image.getdata()):
            raise ValueError('variants sharing a source render differently: frame %d %s' % (model.frame, v))
    return image


def sheet_scope(drafted, originals, users):
    """Map every changed texel to its sheet address and say whether a draft is a sheet patch.

    `drafted` is {frame: (edited Frame, plan)}, `originals` the unedited Frames, and
    `users` {frame: set of sheet keys} for every frame in the pack. Drafts that give one
    sheet texel different values are refused. A draft is a 'sheet-patch' only when it has
    no overlay texels (which have no sheet source) and every other frame drawing an edited
    texel carries the identical edit; otherwise it is 'frame-composite-only'.
    """
    writes, edited, overlay = {}, {}, {}
    for f, (model, _) in drafted.items():
        edited[f], overlay[f] = set(), 0
        for n, p in enumerate(model.parts):
            if 'sheet' not in p:
                overlay[f] += sum(1 for row in p['texels'] for t in row if t != CLEAR)
                continue
            before = originals[f].parts[n]['texels']
            for y, row in enumerate(p['texels']):
                for x, t in enumerate(row):
                    if t != before[y][x]:
                        k = sheet_key(p['sheet'], p['w'], p['h'], x, y)
                        writes.setdefault(k, {})[f] = t
                        edited[f].add(k)
    conflicts = sorted(k for k, by in writes.items() if len(set(by.values())) > 1)
    if conflicts:
        raise ValueError('drafts give %d sheet texels different values, first %r: %r'
                         % (len(conflicts), conflicts[0], writes[conflicts[0]]))
    scopes = {}
    for f in drafted:
        same, affected = Counter(), Counter()
        for g, keys in users.items():
            if g == f:
                continue
            for k in edited[f] & keys:
                (same if writes[k].get(g) == writes[k][f] else affected)[g] += 1
        reasons = []
        if overlay[f]:
            reasons.append('%d overlay texels have no sheet source' % overlay[f])
        if affected:
            reasons.append('edited texels are also drawn, unedited, by frames %s' % sorted(affected))
        scopes[f] = {'scope': 'frame-composite-only' if reasons else 'sheet-patch', 'reasons': reasons,
                     'edited_sheet_texels': len(edited[f]), 'overlay_texels': overlay[f],
                     'same_edit_frames': {str(g): c for g, c in sorted(same.items())},
                     'affected_frames': {str(g): c for g, c in sorted(affected.items())}}
    return scopes


def build(sources, pack_path, executable, output, snapshot=SNAPSHOT, pass2=PASS2):
    frames = load_frames(sources, pack_path)
    hashes = gate(frames, sources)
    records = json.loads((sources / 'manifest.json').read_text())['images']
    frames[56] = Frame(56, {(i['frame_index'], i['variant']): i for i in records}, pack_path.read_bytes())
    inputs = english_inputs(snapshot, pass2)
    panel7 = panel7_face(snapshot, pass2)
    native = NativeFace(executable)
    drafted = compose_all(frames, inputs, panel7, native)
    scopes = sheet_scope(drafted, frames, {r['frame_index']: record_keys(r) for r in records if r['variant'] == 'base'})
    output = new_output(output)
    prepared = []
    for f, (model, plan) in sorted(drafted.items()):
        covered = set()
        if plan['path'] == 'A':
            covered = {hashes[(f, v)] for v in plan['covered_variants']}
        for sha, palette_ids in variant_groups(hashes, f):
            original = Image.open(sources / ('mainrom.frame-%03d.%s.png' % (f, palette_ids[0]))).convert('RGBA')
            image = render_group(model, palette_ids)
            check(original, image, plan['edit_regions'], 'frame %d %s' % (f, palette_ids[0]))
            if sha in covered:
                continue
            name = 'mainrom-frame-%04d-%s.png' % (f, palette_ids[0])
            row = {'id': 'mainrom-draft-%04d.%s' % (f, palette_ids[0]), 'frame': f, 'palette_id': palette_ids[0],
                   'palette_ids': palette_ids, 'source_container': CONTAINER, 'source_container_sha256': MAINROM_SHA,
                   'source_png_sha256': sha, 'dimensions': [model.width, model.height], 'output': name,
                   'runtime_variant_scope': applicability(f, palette_ids, model),
                   'style': 'unreviewed', 'human_approved': False, 'runtime_integrated': False,
                   'in_game_verified': False, 'sheet_scope': scopes[f], **plan}
            prepared.append((image, original, row))
    output.mkdir(parents=True)
    for image, _, row in prepared:
        image.save(output / row['output'])
        row['output_sha256'] = digest((output / row['output']).read_bytes())
    manifest = {'schema': 'tokimemo.mainrom-indexed-drafts.v1', 'ticket': 'T-39009', 'human_approved': False,
                'source_container': CONTAINER, 'source_container_sha256': MAINROM_SHA,
                'decode_gate': {'frames': list(GATED_FRAMES), 'variants': list(VARIANTS),
                                'renders_byte_identical': len(hashes)},
                'faces': {'panel7': panel7.proof(), 'native': native.proof()},
                'assets': [row for _, _, row in prepared]}
    (output / 'manifest.json').write_bytes(encoded(manifest) + b'\n')
    contact_sheet([(o, i, r) for i, o, r in prepared], output.parent / (output.name + '-contact-6x.png'))
    return {'output': str(output), 'drafts': len(prepared), 'gated_renders': len(hashes)}


def check(original, image, regions, label):
    if original.size != image.size:
        raise ValueError('dimensions changed: ' + label)
    for y in range(original.height):
        for x in range(original.width):
            if not inside_regions(regions, x, y) and original.getpixel((x, y)) != image.getpixel((x, y)):
                raise ValueError('pixel changed outside edit regions: %s (%d,%d)' % (label, x, y))
    if not set(image.getdata()) <= set(original.getdata()) | {(0, 0, 0, 0)}:
        raise ValueError('palette changed: ' + label)


def contact_sheet(rows, path, zoom=6):
    """Original beside draft for every output, at 6x, for human style review (not published)."""
    gap = 4 * zoom
    width = max(2 * o.width * zoom + 3 * gap for o, _, _ in rows)
    height = sum(o.height * zoom + gap for o, _, _ in rows) + gap
    sheet = Image.new('RGBA', (width, height), (64, 64, 96, 255))
    y = gap
    for original, image, _ in rows:
        size = (original.width * zoom, original.height * zoom)
        sheet.alpha_composite(original.resize(size, Image.NEAREST), (gap, y))
        sheet.alpha_composite(image.resize(size, Image.NEAREST), (2 * gap + size[0], y))
        y += size[1] + gap
    sheet.save(path)


def publish(built, destination):
    """Copy a built draft set and its candidate ledger into the repository tree."""
    manifest_bytes = (built / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    destination = destination.absolute()
    if destination.exists():
        raise ValueError('destination must be new')
    (destination / REVISION).mkdir(parents=True)
    assets = []
    for row in manifest['assets']:
        data = (built / row['output']).read_bytes()
        if digest(data) != row['output_sha256']:
            raise ValueError('built output changed: ' + row['id'])
        (destination / REVISION / row['output']).write_bytes(data)
        assets.append({'dimensions': row['dimensions'], 'human_approved': False, 'id': row['id'],
                       'manifest_sha256': digest(manifest_bytes), 'output_sha256': row['output_sha256'],
                       'path': REVISION + '/' + row['output'], 'revision': REVISION,
                       'frame': row['frame'], 'palette_id': row['palette_id'], 'palette_ids': row['palette_ids'],
                       'source_container_sha256': row['source_container_sha256'],
                       'source_png_sha256': row['source_png_sha256'], 'status': 'draft',
                       'reason': 'Indexed MAINROM draft candidate (path %s); unreviewed, no approval transfers.' % row['path']})
    (destination / REVISION / 'manifest.json').write_bytes(manifest_bytes)
    ledger = {'schema': 'tokimemo.translation-checkpoint.v1', 'ticket': 'T-39009', 'assets': assets,
              'candidate_assets': len(assets), 'original_images_included': False,
              'complete_translation_inventory': False, 'separate_reviewed_revisions': 0,
              'publication_authorization': 'User explicitly requested translated assets in repository, 2026-10-29.'}
    (destination / 'ledger.json').write_bytes(encoded(ledger) + b'\n')
    return {'destination': str(destination), 'assets': len(assets)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sources', type=Path, default=ROOT / '.asset-cache/translation-assets/original-mainrom-v1')
    parser.add_argument('--pack', type=Path, default=ROOT / 'disc/files/CDROM/FUKUTEST/MAINROM.NBN')
    parser.add_argument('--executable', type=Path, default=ROOT / 'disc/files/SLPM_86.053')
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--publish', type=Path, help='also copy outputs and a ledger into this new repository directory')
    args = parser.parse_args()
    result = build(args.sources, args.pack, args.executable, args.output)
    if args.publish:
        result['published'] = publish(args.output, args.publish)
    print(json.dumps(result))
