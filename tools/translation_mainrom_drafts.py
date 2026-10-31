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

* port-made (--port-made, user decision 2026-10-30): where no usable original
  glyph exists, hand-drawn bitmap glyphs in panel7 style (and 5-row small caps for
  the frame-60 date captions) set the English; each glyph and draft is flagged
  port-made with a description in the manifest and ledger.

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


# ---- Port-made lettering (T-39004 art finish, 2026-10-30) --------------------
#
# User decision 2026-10-30: where no usable original glyph exists, new lettering
# may be drawn by hand in the game's own style and marked port-made. The glyphs
# below are hand-drawn bitmaps (not generated, not scaled from any font). The
# 7-row ones follow panel7 as recovered from frame 61: 1px strokes, caps on rows
# 0-6, lowercase x-height on rows 2-6, descenders folded into the x-height as in
# panel7 'p', one ink index, one blank column between glyphs.

PORT_MADE_REVISION = 'mainrom-port-made-drafts-v1'
PORT_MADE_TICKET = 'T-39009'

PANEL7_ADDED = {
    'B': (['###.', '#..#', '#..#', '###.', '#..#', '#..#', '###.'], 'cap B, 4 wide like panel7 S and L'),
    'D': (['###.', '#..#', '#..#', '#..#', '#..#', '#..#', '###.'], 'cap D, 4 wide, straight stem and bowl like B'),
    'F': (['####', '#...', '#...', '###.', '#...', '#...', '#...'], 'cap F, 4 wide, top bar as wide as panel7 L foot'),
    'J': (['...#', '...#', '...#', '...#', '...#', '#..#', '.##.'], 'cap J, 4 wide, hook like the panel7 S terminals'),
    'O': (['.###.', '#...#', '#...#', '#...#', '#...#', '#...#', '.###.'], 'cap O, 5 wide like panel7 C and G'),
    'I': (['###', '.#.', '.#.', '.#.', '.#.', '.#.', '###'], 'cap I with serifs, 3 wide, so it does not read as panel7 l'),
    'M': (['#...#', '##.##', '#.#.#', '#.#.#', '#...#', '#...#', '#...#'], 'cap M, 5 wide like panel7 A and H'),
    'N': (['#...#', '#...#', '##..#', '#.#.#', '#..##', '#...#', '#...#'], 'cap N, 5 wide, diagonal on rows 2-4'),
    'P': (['###.', '#..#', '#..#', '###.', '#...', '#...', '#...'], 'cap P, 4 wide, bowl matches B and R'),
    'R': (['###.', '#..#', '#..#', '###.', '#.#.', '#..#', '#..#'], 'cap R, 4 wide, bowl matches B and P'),
    'W': (['#...#', '#...#', '#...#', '#.#.#', '#.#.#', '#.#.#', '.#.#.'], 'cap W, 5 wide, mirror logic of panel7 v'),
    'b': (['#...', '#...', '###.', '#..#', '#..#', '#..#', '###.'], 'lowercase b, panel7 h stem with the panel7 o bowl'),
    'd': (['...#', '...#', '.###', '#..#', '#..#', '#..#', '.###'], 'lowercase d, mirror of b'),
    'g': (['....', '....', '.###', '#..#', '.###', '...#', '.##.'], 'lowercase g, tail folded into the x-height like panel7 p'),
    'm': (['.....', '.....', '####.', '#.#.#', '#.#.#', '#.#.#', '#.#.#'], 'lowercase m, double arch of panel7 n'),
    'w': (['.....', '.....', '#...#', '#...#', '#.#.#', '#.#.#', '.#.#.'], 'lowercase w, panel7 v with a centre stroke'),
    'y': (['....', '....', '#..#', '#..#', '.###', '...#', '.##.'], 'lowercase y, tail folded into the x-height like panel7 p'),
}
# Narrow forms, used only where a line does not fit at panel7 widths (fit_face).
# Same rows and stroke; only the bowl or counter is one column narrower.
NARROW_ADDED = {
    'H': (['#..#', '#..#', '#..#', '####', '#..#', '#..#', '#..#'], 'narrow cap H, 4 wide (panel7 H is 5)'),
    'a': (['...', '...', '##.', '..#', '.##', '#.#', '.##'], 'narrow a, 3 wide (panel7 a is 4)'),
    'b': (['#..', '#..', '##.', '#.#', '#.#', '#.#', '##.'], 'narrow b, 3 wide'),
    'e': (['...', '...', '.#.', '#.#', '###', '#..', '.##'], 'narrow e, 3 wide (panel7 e is 4)'),
    'g': (['...', '...', '###', '#.#', '###', '..#', '##.'], 'narrow g, 3 wide, square bowl so it does not read as narrow a'),
    'n': (['...', '...', '##.', '#.#', '#.#', '#.#', '#.#'], 'narrow n, 3 wide (panel7 n is 4)'),
    'o': (['...', '...', '.#.', '#.#', '#.#', '#.#', '.#.'], 'narrow o, 3 wide (panel7 o is 4)'),
    'r': (['..', '..', '##', '#.', '#.', '#.', '#.'], 'narrow r, 2 wide (panel7 r is 3); last resort, loses the arm'),
    's': (['...', '...', '.##', '#..', '.#.', '..#', '##.'], 'narrow s, 3 wide (panel7 s is 4)'),
    't': (['#.', '#.', '##', '#.', '#.', '#.', '.#'], 'narrow t, 2 wide (panel7 t is 3); last resort, loses the crossbar'),
}
# Order in which fit_face narrows letters: bowls and counters first, r and t (whose
# narrow forms lose a stroke) last.
NARROW_ORDER = 'Hoaebgns' + 'rt'
# 5-row small capitals for the frame-60 date captions; the 16px cells have room
# for 4 letters of 3px only. Same 1px stroke; ink and drop shadow are the panel's.
CAPS5 = {
    'A': (['.#.', '#.#', '###', '#.#', '#.#'], 'small cap A'),
    'D': (['##.', '#.#', '#.#', '#.#', '##.'], 'small cap D'),
    'E': (['###', '#..', '##.', '#..', '###'], 'small cap E'),
    'M': (['#...#', '##.##', '#.#.#', '#...#', '#...#'], 'small cap M, 5 wide'),
    'O': (['.#.', '#.#', '#.#', '#.#', '.#.'], 'small cap O'),
    'R': (['##.', '#.#', '##.', '#.#', '#.#'], 'small cap R'),
    'Y': (['#.#', '#.#', '.#.', '.#.', '.#.'], 'small cap Y'),
}

# Frame -> lines of text. Wording is the user's revisable default (2026-10-30).
# Glyph widths are chosen per item by fit_face.
PORT_MADE_TAGS = {134: ('Comedy',), 135: ('Romance',), 138: ('Horror',), 139: ('Anime',), 141: ('Idol',),
                  142: ('Rock',), 143: ('New Music',), 144: ('Pro', 'Wrestling'), 145: ('Baseball',)}
DATE_FRAME = 60
# Frame-60 kanji cells 年/月/日 (parts 3-5). The game draws the year, month and day
# digits above these cells (port/godot/ui/native_hud.gd build_calendar). Part 6, the
# weekday parentheses, is kept.
DATE_CELLS = (3, 4, 5)
# Default English form "95 / Apr / 4 / (Mon)": the cells are cleared and the port
# draws a month-name sprite in place of the two month digit actors (19, 20).
# Alternate form: small-cap captions under each number, "95 YR / 4 MO / 4 DAY".
DATE_CAPTIONS = {3: 'YR', 4: 'MO', 5: 'DAY'}
# Interior column of the panel that the digits are centred on (frame x 9..24).
DATE_CAPTION_X = (9, 16)
# Month names in game order (the school year starts in April).
MONTHS = ('Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec', 'Jan', 'Feb', 'Mar')
# Month digit pair geometry (native_hud.gd: actors 19/20 at x 0x8C/0x94, y -0x22; digit
# frames 0x44.. are 8x16 with origin (4, 8) and bounds x 1). The pair covers screen
# x 0x89..0x98, y -0x2A..-0x1B, which is frame-60 rect (9, 40, 16, 16).
DIGIT_FRAMES = tuple(range(0x44, 0x4E))
MONTH_SPRITE = {'size': (16, 16), 'frame60_rect': [9, 40, 16, 16], 'screen_left_top': [0x89, -0x2A],
                'replaces_actors': [19, 20], 'digit_ink_rows': [0, 10]}


def bitmap_glyph(rows):
    width = len(rows[0])
    if any(len(r) != width for r in rows) or any(set(r) - {'#', '.'} for r in rows):
        raise ValueError('malformed glyph bitmap')
    return {'points': [(x, y) for y, r in enumerate(rows) for x, c in enumerate(r) if c == '#'], 'width': width}


def port_made_face(name, panel7, added, narrow=None, space=3):
    """panel7 plus hand-drawn glyphs; every glyph records whether it is recovered or port-made."""
    glyphs, provenance = {}, {}
    for ch, glyph in sorted(panel7.glyphs.items()):
        glyphs[ch] = glyph
        provenance[ch] = {'origin': 'recovered-panel7', 'witness': panel7.provenance['witness'][ch]}
    for table, kind in ((added, 'port-made'), (narrow or {}, 'port-made-narrow')):
        for ch, (rows, description) in sorted(table.items()):
            if kind == 'port-made' and ch in panel7.glyphs:
                raise ValueError('port-made glyph would replace recovered panel7 glyph %r' % ch)
            if len(rows) != 7:
                raise ValueError('panel7-style glyph %r must have 7 rows' % ch)
            glyphs[ch] = bitmap_glyph(rows)
            provenance[ch] = {'origin': kind, 'description': description, 'bitmap': list(rows),
                              'replaces': 'recovered-panel7' if ch in panel7.glyphs else None}
    return Face(name, glyphs, space, {
        'base_face': {'name': 'panel7', 'source': panel7.provenance['source'],
                      'source_output_sha256': panel7.provenance['source_output_sha256']},
        'style': 'panel7: 1px stroke, 7 rows, x-height rows 2-6, one ink index, 1 blank column between glyphs',
        'rescaled': False, 'generated': False, 'glyphs': provenance})


def caps5_face():
    glyphs, provenance = {}, {}
    for ch, (rows, description) in sorted(CAPS5.items()):
        if len(rows) != 5:
            raise ValueError('caps5 glyph %r must have 5 rows' % ch)
        glyphs[ch] = bitmap_glyph(rows)
        provenance[ch] = {'origin': 'port-made', 'description': description, 'bitmap': list(rows)}
    return Face('caps5', glyphs, 3, {
        'style': '5-row small capitals, 1px stroke, drawn with the frame-60 kanji ink and (+1,+1) drop shadow',
        'rescaled': False, 'generated': False, 'glyphs': provenance})


def port_made_faces(panel7):
    return {'panel7-compact': port_made_face('panel7-compact', panel7, PANEL7_ADDED),
            'panel7-narrow': port_made_face('panel7-narrow', panel7, PANEL7_ADDED, NARROW_ADDED),
            'caps5': caps5_face()}


def line_width(face, text, shadow):
    xs = [x for x, _ in face.points(text)]
    return max(xs) - min(xs) + 1 + (1 if shadow is not None else 0)


def fit_face(faces, lines, width, shadow=None):
    """panel7-compact with narrow forms swapped in one letter at a time, only while a line is too wide.

    Letters are tried in NARROW_ORDER, and only letters of a line that does not fit;
    a letter once narrowed is narrow in every line of the item.
    """
    compact, narrow = faces['panel7-compact'], faces['panel7-narrow']
    chosen = []
    while True:
        glyphs, provenance = dict(compact.glyphs), dict(compact.provenance['glyphs'])
        for ch in chosen:
            glyphs[ch], provenance[ch] = narrow.glyphs[ch], narrow.provenance['glyphs'][ch]
        face = Face(compact.name + ('+narrow-' + ''.join(chosen) if chosen else ''), glyphs, compact.space,
                    {**compact.provenance, 'glyphs': provenance})
        wide = [line for line in lines if line_width(face, line, shadow) > width]
        if not wide:
            return face
        options = [ch for ch in NARROW_ORDER if ch not in chosen and any(ch in line for line in wide)]
        if not options:
            raise ValueError('%r does not fit %dpx even with narrow forms' % (wide, width))
        chosen.append(options[0])


def ink_and_shadow(pixels):
    """(ink, shadow) of lettering with a (+1,+1) drop shadow; (ink, None) for one index.

    The shadow index is the one most often found one pixel down-right of the other.
    """
    indices = sorted({i for _, i in pixels.values()})
    if len(indices) == 1:
        return indices[0], None
    tally = Counter()
    for (x, y), (_, i) in pixels.items():
        below = pixels.get((x + 1, y + 1))
        if below and below[1] != i:
            tally[(i, below[1])] += 1
    if not tally:
        raise ValueError('lettering has several indices but no drop shadow')
    (ink, shadow), _ = sorted(tally.items(), key=lambda kv: (-kv[1], kv[0]))[0]
    return ink, shadow


def used_port_made(face, lines):
    """Port-made glyphs a draft uses, with their descriptions (for the ledger)."""
    out = []
    for ch in sorted({c for line in lines for c in line if c != ' '}):
        origin = face.provenance['glyphs'][ch]
        if origin['origin'] != 'recovered-panel7':
            out.append({'kind': 'glyph', 'face': face.name, 'char': ch, 'origin': origin['origin'],
                        'description': origin['description']})
    return out


def plan_port_made_tag(frame, faces):
    """Tags 134-145: clear the Japanese and set English lines in a port-made panel7 face."""
    lines = PORT_MADE_TAGS[frame.frame]
    rect = [0, 0, frame.width, frame.height]
    source = lettering(frame, rect)
    ink, shadow = ink_and_shadow(source)
    face = fit_face(faces, lines, frame.width, shadow)
    face_name = face.name
    clear(frame, rect)
    line_h = frame.height // len(lines)
    boxes = [stamp(frame, face, text, [0, n * line_h, frame.width, line_h], ink, source_parts(source), shadow=shadow)
             for n, text in enumerate(lines)]
    elements = used_port_made(face, lines)
    return {'path': 'G', 'text': ' / '.join(lines), 'lines': list(lines), 'face': face_name, 'ink': ink,
            'shadow': shadow, 'edit_regions': [rect], 'lettering_bbox': boxes,
            'port_made': bool(elements), 'port_made_elements': elements,
            'design': '%d line(s) of %s lettering centred in the tag, ink%s taken from the source lettering'
                      % (len(lines), face_name, ' and (+1,+1) shadow' if shadow is not None else '')}


def date_cells(frame):
    """(ink, shadow, per-cell lettering pixels) of the frame-60 kanji cells, which must hold only lettering."""
    pixels = {}
    for n in DATE_CELLS:
        p = frame.parts[n]
        pixels.update({k: v for k, v in lettering(frame, [p['x'], p['y'], p['w'], p['h']]).items() if v[0] == n})
    ink, shadow = ink_and_shadow(pixels)
    if any(t not in (CLEAR, ink, shadow) for n in DATE_CELLS for row in frame.parts[n]['texels'] for t in row):
        raise ValueError('frame 60 kanji cells hold more than lettering')
    return ink, shadow, pixels


DATE_DIGITS = ('drawn by the game above each cell (native_hud.gd build_calendar: year y-0x41, '
               'month y-0x22, day y-1, weekday frame 0x52+n inside part 6)')


def plan_date_panel(frame, faces):
    """Frame 60, default "Apr 4" form: clear the 年/月/日 cells; the port draws the month name."""
    ink, shadow, _ = date_cells(frame)
    regions = []
    for n in DATE_CELLS:
        p = frame.parts[n]
        for row in p['texels']:
            row[:] = [CLEAR] * len(row)
        regions.append([p['x'], p['y'], p['w'], p['h']])
    elements = [{'kind': 'layout', 'description': 'year, month and day kanji cells cleared; the port draws a '
                 'port-made month-name sprite (mainrom-port-month-*.png) in place of month digit actors 19/20, '
                 'so the panel reads 95 / Apr / 4 / (Mon); the weekday parentheses are kept'}]
    return {'path': 'H', 'text': '95 / Apr / 4 / (Mon)', 'face': None, 'ink': ink, 'shadow': shadow,
            'edit_regions': regions, 'lettering_bbox': [], 'port_made': True, 'port_made_elements': elements,
            'date_form': {'reads': '95 / Apr / 4 / (Mon)', 'form': 'month-name', 'digits': DATE_DIGITS,
                          'month_sprites': 'mainrom-port-month-<mon>.png, see port_overlay in the manifest',
                          'without_port_overlay': 'reads 95 / 4 / 4 / (Mon) (game digits only)',
                          'alternatives': ['captions: mainrom-alt-0060-captions.* reads 95 YR / 4 MO / 4 DAY / (Mon)']},
            'design': 'empty panel cells; the English month is a separate port-drawn sprite'}


def plan_date_captions(frame, faces):
    """Frame 60, alternate form: small-cap captions YR / MO / DAY under the game's digits."""
    face = faces['caps5']
    ink, shadow, pixels = date_cells(frame)
    frame.add_overlay(DATE_CELLS)
    overlay = len(frame.parts) - 1
    regions, boxes = [], []
    for n, caption in DATE_CAPTIONS.items():
        p = frame.parts[n]
        rows = [y for (x, y), v in pixels.items() if v[0] == n]
        top, bottom = min(rows), max(rows)
        for row in p['texels']:
            row[:] = [CLEAR] * len(row)
        rect = [DATE_CAPTION_X[0], top, DATE_CAPTION_X[1], bottom - top + 1]
        boxes.append(stamp(frame, face, caption, rect, ink, set(DATE_CELLS), shadow=shadow, part=overlay))
        regions += [[p['x'], p['y'], p['w'], p['h']], rect]
    elements = used_port_made(face, DATE_CAPTIONS.values()) + [{
        'kind': 'layout', 'description': 'captions YR / MO / DAY on an overlay layer with the kanji-cell CLUT, '
        'centred on the digit column (frame x 9..24) and on each kanji\'s rows; the weekday parentheses are kept'}]
    return {'path': 'H', 'text': ' / '.join(DATE_CAPTIONS.values()), 'face': 'caps5', 'ink': ink, 'shadow': shadow,
            'overlay_clut_parts': list(DATE_CELLS), 'edit_regions': regions, 'lettering_bbox': boxes,
            'port_made': True, 'port_made_elements': elements,
            'date_form': {'reads': '95 YR / 4 MO / 4 DAY / (Mon)', 'form': 'captions', 'digits': DATE_DIGITS},
            'design': 'small-cap captions in the panel\'s kanji ink with its (+1,+1) drop shadow'}


def load_digit_frames(sources, pack_path):
    """The calendar digit frames 0x44..0x4D, each checked against its exported base PNG."""
    pack = pack_path.read_bytes()
    if digest(pack) != MAINROM_SHA:
        raise ValueError('MAINROM.NBN identity differs')
    images = {(i['frame_index'], i['variant']): i
              for i in json.loads((sources / 'manifest.json').read_text())['images']}
    out = {}
    for f in DIGIT_FRAMES:
        frame = Frame(f, images, pack)
        original = Image.open(sources / ('mainrom.frame-%03d.base.png' % f)).convert('RGBA')
        if list(frame.render('base').getdata()) != list(original.getdata()):
            raise ValueError('decoder does not reproduce digit frame %d' % f)
        out[f] = frame
    return out


def month_sprites(digits, date_frame, faces):
    """Port-made month-name sprites in the calendar digits' CLUT, ink and drop shadow.

    Each is 16x16 like the two-digit month pair it replaces; the name is centred on the
    digit ink rows. Returns [(name, Frame, plan)].
    """
    four = digits[0x48]
    ink, shadow = ink_and_shadow(lettering(four, [0, 0, four.width, four.height]))
    palette = four.palettes['base'][0]
    if any(date_frame.palettes['base'][n] != palette for n in DATE_CELLS):
        raise ValueError('digit CLUT differs from the frame-60 kanji cells')
    w, h = MONTH_SPRITE['size']
    top, bottom = MONTH_SPRITE['digit_ink_rows']
    out = []
    for name in MONTHS:
        face = fit_face(faces, [name], w, shadow)
        model = Frame.from_layers(DATE_FRAME, w, h, [{'index': 0, 'x': 0, 'y': 0, 'w': w, 'h': h,
                                                     'texels': [[CLEAR] * w for _ in range(h)]}],
                                  {v: [palette] for v in VARIANTS})
        box = stamp(model, face, name, [0, top, w, bottom - top + 1], ink, {0}, shadow=shadow)
        out.append((name, model, {'text': name, 'face': face.name, 'ink': ink, 'shadow': shadow,
                                  'lettering_bbox': [box], 'port_made': True,
                                  'port_made_elements': used_port_made(face, [name]) + [{
                                      'kind': 'sprite', 'description': 'new 16x16 port sprite for month %s in '
                                      'the calendar digit CLUT, ink and (+1,+1) shadow, drawn in place of month '
                                      'digit actors 19/20' % name}]}))
    return out


def digit_preview(digits, month_index):
    """The month as the game draws it (one or two digit frames in a 16x16 box), for previews."""
    month = month_index + 4 if month_index < 9 else month_index - 8
    image = Image.new('RGBA', MONTH_SPRITE['size'], (0, 0, 0, 0))
    if month >= 10:
        image.alpha_composite(digits[0x44 + month // 10].render('base'), (0, 0))
        image.alpha_composite(digits[0x44 + month % 10].render('base'), (8, 0))
    else:
        image.alpha_composite(digits[0x44 + month].render('base'), (4, 0))
    return image


def isolated_scope(f, model, original, users):
    """Sheet scope of one draft on its own: whole-frame replacement unless every edited texel is private."""
    edited, overlay, conflicting = {}, 0, 0
    for n, p in enumerate(model.parts):
        if 'sheet' not in p:
            overlay += sum(1 for row in p['texels'] for t in row if t != CLEAR)
            continue
        before = original.parts[n]['texels']
        for y, row in enumerate(p['texels']):
            for x, t in enumerate(row):
                if t != before[y][x]:
                    k = sheet_key(p['sheet'], p['w'], p['h'], x, y)
                    if edited.get(k, t) != t:
                        conflicting += 1
                    edited[k] = t
    affected = Counter(g for g, keys in users.items() if g != f for k in edited if k in keys)
    reasons = []
    if overlay:
        reasons.append('%d overlay texels have no sheet source' % overlay)
    if conflicting:
        reasons.append('%d sheet texels are drawn by two parts of this frame with different edits' % conflicting)
    if affected:
        reasons.append('edited texels are also drawn by frames %s' % sorted(affected))
    return {'scope': 'frame-composite-only' if reasons else 'sheet-patch', 'reasons': reasons,
            'edited_sheet_texels': len(edited), 'overlay_texels': overlay,
            'affected_frames': {str(g): c for g, c in sorted(affected.items())}}


def compose_port_made(frames, faces):
    out = {}
    for f in sorted(PORT_MADE_TAGS):
        model = frames[f].copy()
        out[f] = (model, plan_port_made_tag(model, faces))
    model = frames[DATE_FRAME].copy()
    out[DATE_FRAME] = (model, plan_date_panel(model, faces))
    return out


def frame_rows(f, model, plan, original_frame, hashes, sources, users, id_format, output_format):
    """One output row per distinct source PNG of frame f, checked against the source outside the edit regions."""
    scope = isolated_scope(f, model, original_frame, users)
    rows = []
    for sha, palette_ids in variant_groups(hashes, f):
        original = Image.open(sources / ('mainrom.frame-%03d.%s.png' % (f, palette_ids[0]))).convert('RGBA')
        image = render_group(model, palette_ids)
        check(original, image, plan['edit_regions'], 'frame %d %s' % (f, palette_ids[0]))
        rows.append((image, {'id': id_format % (f, palette_ids[0]), 'frame': f, 'palette_id': palette_ids[0],
                             'palette_ids': palette_ids, 'source_container': CONTAINER,
                             'source_container_sha256': MAINROM_SHA, 'source_png_sha256': sha,
                             'dimensions': [model.width, model.height], 'output': output_format % (f, palette_ids[0]),
                             'runtime_variant_scope': applicability(f, palette_ids, model),
                             'style': 'unreviewed', 'human_approved': False, 'runtime_integrated': False,
                             'in_game_verified': False, 'sheet_scope': scope, **plan}))
    return rows


def build_port_made(sources, pack_path, output, snapshot=SNAPSHOT, pass2=PASS2):
    """Drafts for the MAINROM items with no usable original glyphs (60, 134-145 minus 136/137/140).

    Also builds the frame-60 caption alternate and the port-made month-name sprites
    that the default frame-60 form needs.
    """
    frames = load_frames(sources, pack_path)
    hashes = gate(frames, sources)
    digits = load_digit_frames(sources, pack_path)
    records = json.loads((sources / 'manifest.json').read_text())['images']
    users = {r['frame_index']: record_keys(r) for r in records if r['variant'] == 'base'}
    faces = port_made_faces(panel7_face(snapshot, pass2))
    drafted = compose_port_made(frames, faces)
    output = new_output(output)
    prepared, overlay = [], []
    for f, (model, plan) in sorted(drafted.items()):
        prepared += frame_rows(f, model, plan, frames[f], hashes, sources, users,
                               'mainrom-draft-%04d.%s', 'mainrom-frame-%04d-%s.png')
    model = frames[DATE_FRAME].copy()
    alternates = frame_rows(DATE_FRAME, model, plan_date_captions(model, faces), frames[DATE_FRAME], hashes, sources,
                            users, 'mainrom-alt-%04d-captions.%s', 'mainrom-alt-%04d-captions-%s.png')
    for n, (name, model, plan) in enumerate(month_sprites(digits, frames[DATE_FRAME], faces)):
        overlay.append((model.render('base'), {
            'id': 'mainrom-port-month-%s' % name.lower(), 'month_order': n,
            'output': 'mainrom-port-month-%s.png' % name.lower(), 'dimensions': [model.width, model.height],
            'palette_id': 'base', 'clut_words': model.palettes['base'][0],
            'clut_source': 'calendar digit frames 0x44..0x4D (equal to the frame-60 kanji-cell CLUT)',
            'placement': MONTH_SPRITE, 'source_container_sha256': MAINROM_SHA, 'style': 'unreviewed',
            'human_approved': False, 'runtime_integrated': False, 'in_game_verified': False, **plan}))
    output.mkdir(parents=True)
    for image, row in prepared + alternates + overlay:
        image.save(output / row['output'])
        row['output_sha256'] = digest((output / row['output']).read_bytes())
    manifest = {'schema': 'tokimemo.mainrom-port-made-drafts.v1', 'ticket': PORT_MADE_TICKET, 'human_approved': False,
                'user_decision': '2026-10-30: finish asset translation or creation; new lettering in the game style '
                                 'is allowed where no usable original glyph exists, marked port-made',
                'source_container': CONTAINER, 'source_container_sha256': MAINROM_SHA,
                'decode_gate': {'frames': list(GATED_FRAMES), 'variants': list(VARIANTS),
                                'renders_byte_identical': len(hashes), 'digit_frames': list(DIGIT_FRAMES)},
                'faces': {name: face.proof() for name, face in sorted(faces.items())},
                'assets': [row for _, row in prepared],
                'alternates': [row for _, row in alternates],
                'port_overlay': {'purpose': 'frame-60 default date form: the port draws the month name instead of '
                                            'month digit actors 19/20 (port/godot/ui/native_hud.gd build_calendar)',
                                 'month_order': list(MONTHS), 'sprites': [row for _, row in overlay]}}
    (output / 'manifest.json').write_bytes(encoded(manifest) + b'\n')
    return {'output': str(output), 'drafts': len(prepared), 'alternates': len(alternates),
            'port_overlay_sprites': len(overlay), 'gated_renders': len(hashes)}


def calendar_mock(panel, digits, month):
    """Frame 60 with the game's year and day digits (95, 4) and a month image drawn on it (review aid)."""
    image = panel.copy()
    for digit, x, y in ((9, 9, 9), (5, 17, 9), (4, 13, 73)):
        image.alpha_composite(digits[0x44 + digit].render('base'), (x, y))
    image.alpha_composite(month, tuple(MONTH_SPRITE['frame60_rect'][:2]))
    return image


def previews(built, sources, destination, pack_path, zoom=4):
    """Original beside draft, nearest x`zoom`, one PNG per output (local review aid, not published)."""
    destination.mkdir(parents=True, exist_ok=True)
    manifest = json.loads((built / 'manifest.json').read_text())
    digits = load_digit_frames(sources, pack_path)
    count = 0
    for row in manifest['assets'] + manifest['alternates']:
        original = Image.open(sources / ('mainrom.frame-%03d.%s.png' % (row['frame'], row['palette_id']))).convert('RGBA')
        contact_sheet([(original, Image.open(built / row['output']).convert('RGBA'), row)],
                      destination / row['output'], zoom=zoom)
        count += 1
    sprites = manifest['port_overlay']['sprites']
    rows = [(digit_preview(digits, row['month_order']), Image.open(built / row['output']).convert('RGBA'), row)
            for row in sprites]
    contact_sheet(rows, destination / 'mainrom-port-months.png', zoom=zoom)
    panel = Image.open(sources / ('mainrom.frame-%03d.base.png' % DATE_FRAME)).convert('RGBA')
    draft = Image.open(built / ('mainrom-frame-%04d-base.png' % DATE_FRAME)).convert('RGBA')
    captions = Image.open(built / ('mainrom-alt-%04d-captions-base.png' % DATE_FRAME)).convert('RGBA')
    april = Image.open(built / sprites[0]['output']).convert('RGBA')
    four = digit_preview(digits, 0)
    contact_sheet([(calendar_mock(panel, digits, four), calendar_mock(draft, digits, april), None),
                   (calendar_mock(captions, digits, four), calendar_mock(draft, digits, four), None)],
                  destination / 'review-0060-date-forms-mock.png', zoom=zoom)
    return count + len(rows)


def publish_port_made(built, destination):
    """Copy a port-made build and its ledger (port-made flags per asset) into a new repository directory."""
    manifest_bytes = (built / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    if manifest['schema'] != 'tokimemo.mainrom-port-made-drafts.v1':
        raise ValueError('not a port-made build')
    destination = destination.absolute()
    if destination.exists():
        raise ValueError('destination must be new')
    (destination / PORT_MADE_REVISION).mkdir(parents=True)

    def copy(row):
        data = (built / row['output']).read_bytes()
        if digest(data) != row['output_sha256']:
            raise ValueError('built output changed: ' + row['id'])
        (destination / PORT_MADE_REVISION / row['output']).write_bytes(data)
        return {'dimensions': row['dimensions'], 'human_approved': False, 'id': row['id'],
                'manifest_sha256': digest(manifest_bytes), 'output_sha256': row['output_sha256'],
                'path': PORT_MADE_REVISION + '/' + row['output'], 'revision': PORT_MADE_REVISION,
                'source_container_sha256': row['source_container_sha256'], 'status': 'draft',
                'port_made': row['port_made'], 'port_made_elements': row['port_made_elements'], 'text': row['text']}

    def frame_entry(row, reason):
        return {**copy(row), 'frame': row['frame'], 'palette_id': row['palette_id'], 'palette_ids': row['palette_ids'],
                'source_png_sha256': row['source_png_sha256'], 'sheet_scope': row['sheet_scope']['scope'],
                'reason': reason % row['path']}

    assets = [frame_entry(row, 'MAINROM draft with port-made lettering (path %s); unreviewed, no approval transfers.')
              for row in manifest['assets']]
    alternates = [frame_entry(row, 'Alternate MAINROM draft (path %s), not an inventory candidate; unreviewed.')
                  for row in manifest['alternates']]
    overlay = [{**copy(row), 'placement': row['placement'], 'month_order': row['month_order'],
                'reason': 'Port-made overlay sprite for the frame-60 month name; not a source frame; unreviewed.'}
               for row in manifest['port_overlay']['sprites']]
    (destination / PORT_MADE_REVISION / 'manifest.json').write_bytes(manifest_bytes)
    ledger = {'schema': 'tokimemo.translation-checkpoint.v1', 'ticket': PORT_MADE_TICKET, 'assets': assets,
              'candidate_assets': len(assets), 'original_images_included': False,
              'complete_translation_inventory': False, 'separate_reviewed_revisions': 0,
              'port_made_assets': sum(1 for a in assets if a['port_made']),
              'alternates': alternates, 'port_overlay_assets': overlay,
              'publication_authorization': 'User explicitly requested translated assets in repository, 2026-10-29; '
                                           'finish translation or creation, 2026-10-30.'}
    (destination / 'ledger.json').write_bytes(encoded(ledger) + b'\n')
    return {'destination': str(destination), 'assets': len(assets), 'alternates': len(alternates),
            'port_overlay_assets': len(overlay)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sources', type=Path, default=ROOT / '.asset-cache/translation-assets/original-mainrom-v1')
    parser.add_argument('--pack', type=Path, default=ROOT / 'disc/files/CDROM/FUKUTEST/MAINROM.NBN')
    parser.add_argument('--executable', type=Path, default=ROOT / 'disc/files/SLPM_86.053')
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--publish', type=Path, help='also copy outputs and a ledger into this new repository directory')
    parser.add_argument('--port-made', action='store_true', help='build the port-made lettering drafts instead')
    parser.add_argument('--previews', type=Path, help='with --port-made: write x4 original/draft previews here')
    args = parser.parse_args()
    if args.port_made:
        result = build_port_made(args.sources, args.pack, args.output)
        if args.previews:
            result['previews'] = previews(args.output, args.sources, args.previews, args.pack)
        if args.publish:
            result['published'] = publish_port_made(args.output, args.publish)
    else:
        result = build(args.sources, args.pack, args.executable, args.output)
        if args.publish:
            result['published'] = publish(args.output, args.publish)
    print(json.dumps(result))
