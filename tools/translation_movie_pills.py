#!/usr/bin/env python3
"""T-39004 catalog-153: English cast-credit pills for the OPENXA opening movie, plus a cue table.

The opening movie burns its cast credits into the MDEC video: a magenta pill
with the character name and a larger pill with the actress name, in pairs, for
eight shots. A port cannot edit those pixels, so this tool produces one RGBA
overlay per pill and a cue table (decoded frame range, placement, measured
fade for reference) for a port movie player to draw, opaque, on top of the
decoded frames.

* Source: OPENXA.STR read as raw 2352-byte sectors from track 1 (LBA 2080,
  12435 sectors, hash pinned) and decoded with the container's ffmpeg psxstr
  demuxer. Frame numbers are zero-based decoded output indices, not timing.
* Placement: every pill has a hand-measured cover rectangle that encloses the
  original pill, its Japanese text and shadow. The tool checks against the
  decoded frames that the original caption ink stays inside the English
  pill's opaque area, and measures each cue's fade from the caption ink.
* Art (port-made, user decision 2026-10-30): opaque rounded pill (half-disc
  ends, or a smaller rounding where a full cap at the frame edge would not
  hide the caption; square only where the original runs off the frame) with
  a horizontal magenta-to-pink gradient in colours sampled from the movie; white bold
  italic lettering with a dark purple (+1,+1) shadow. Character names and the
  header use the game's native 12px Latin masks, unscaled, emboldened by one
  texel and row-shifted for italic; actress names use the port-made large face
  in translation_movie_font.py. Every pill is recorded as port-made.

Outputs are unreviewed drafts; nothing here records human approval. The movie
player port is separate (no runtime integration).
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

import numpy as np
from PIL import Image

from translation_assets import ROOT, digest, encoded, new_output
from translation_movie_font import CAP_HEIGHT, LARGE_BASELINE, LARGE_HEIGHT, GAP, large_glyph
from translation_native_font import NativeLatin

REVISION = 'movie-pills-v1'
CATALOG_ITEM = 'catalog-153'
TRACK = ROOT / 'disc/track1.bin'
EXE = ROOT / 'disc/files/SLPM_86.053'
SECTOR, LBA, SECTORS = 2352, 2080, 12435
RAW_SHA = '8b7417223e1335702eb624f7bad81e775dee0f1e6f4fdbcf10ebb86320e57f7f'
FRAMES, FRAME_SIZE = 829, (288, 192)
# Colours sampled from the decoded credits (saturated pill end, pale pill end,
# shadow under the white lettering).
PILL_SATURATED = (250, 40, 246)
PILL_PALE = (236, 146, 240)
TEXT = (255, 255, 255)
SHADOW = (64, 22, 78)
FADE_MARGIN = 4  # frames checked on each side of a cue window
EDGE = 2  # fade frames at each end of a cue window
FULL, OFF = 0.65, 0.12  # caption-ink ratios: interior frames at least FULL, absent frames at most OFF
# Ghost frames: the dissolving white lettering stays visible a frame beyond the
# pill-colour ink. A frame next to the window is a ghost when the cue's median
# lettering contrast exceeds the cue's background contrast by GHOST; at most
# GHOST_REACH frames per side; background from frames 4-8 outside the window
# (the lower of the two sides).
GHOST, GHOST_REACH, GHOST_BACKGROUND = 0.1, 2, (4, 8)
EXTENDS = tuple(range(3, 15))  # pill overhang past the cover, smallest that hides the caption ink
CAPTION_REACH = 4  # texels the Japanese lettering and shadow may overhang the pink pill
BLEED = 0.6  # the original pill runs off the frame when its body fills this share of its rows in the edge column
MIN_RADIUS = 4  # smallest end rounding tried when a full half-disc cap cannot hide the caption at the frame edge

# Cue windows are the first and last decoded frames with any caption ink; the
# cue table widens each to the union of its pills' windows.
# Pills: (id, role, Japanese, English, cover [x0, y0, x1, y1) in frame texels,
# anchor side). Stacked covers of a pair meet at the visual gap
# between the two original pills; pills are drawn in list order, so the
# actress pill overlaps the character pill above it, as in the movie.
# Names are family-name first, as in the project glossary.
CUES = [
    ('cast-1', (251, 274), [
        ('cast', 'header', 'キャスト', 'Cast', (6, 4, 98, 28), 'left'),
        ('fujisaki-shiori', 'character', '藤崎詩織', 'Fujisaki Shiori', (158, 128, 254, 153), 'right'),
        ('kanazuki-mami', 'actress', '金月真美', 'Kanazuki Mami', (172, 153, 288, 186), 'right')]),
    ('cast-2', (279, 302), [
        ('saotome-yoshio', 'character', '早乙女好雄', 'Saotome Yoshio', (0, 0, 104, 23), 'left'),
        ('ueda-yuji', 'actress', '上田祐司', 'Ueda Yuji', (6, 23, 126, 51), 'left'),
        ('saotome-yumi', 'character', '早乙女優美', 'Saotome Yumi', (120, 133, 222, 155), 'right'),
        ('yoshiki-kurin', 'actress', 'よしきくりん', 'Yoshiki Kurin', (128, 155, 288, 184), 'right')]),
    ('cast-3', (309, 332), [
        ('ijuin-rei', 'character', '伊集院レイ', 'Ijuin Rei', (0, 0, 106, 23), 'left'),
        ('tsunoda-narumi', 'actress', '津野田なるみ', 'Tsunoda Narumi', (10, 23, 174, 50), 'left'),
        ('kisaragi-mio', 'character', '如月未緒', 'Kisaragi Mio', (162, 134, 258, 157), 'right'),
        ('sekine-akiko', 'actress', '関根明子', 'Sekine Akiko', (172, 157, 288, 186), 'right')]),
    ('cast-4', (341, 364), [
        ('himoo-yuina', 'character', '紐緒結奈', 'Himoo Yuina', (0, 0, 90, 21), 'left'),
        ('naka-tomoko', 'actress', '中　友子', 'Naka Tomoko', (16, 21, 134, 50), 'left'),
        ('katagiri-ayako', 'character', '片桐彩子', 'Katagiri Ayako', (158, 135, 252, 158), 'right'),
        ('kawaguchi-masayo', 'actress', '川口雅代', 'Kawaguchi Masayo', (170, 158, 288, 186), 'right')]),
    ('cast-5', (367, 389), [
        ('nijino-saki', 'character', '虹野沙希', 'Nijino Saki', (162, 0, 256, 20), 'right'),
        ('sugawara-sachiko', 'actress', '菅原祥子', 'Sugawara Sachiko', (174, 21, 288, 47), 'right'),
        ('koshiki-yukari', 'character', '古式ゆかり', 'Koshiki Yukari', (0, 139, 106, 163), 'left'),
        ('kurosaki-ayako', 'actress', '黒崎彩子', 'Kurosaki Ayako', (14, 163, 132, 189), 'left')]),
    ('cast-6', (393, 415), [
        ('kiyokawa-nozomi', 'character', '清川　望', 'Kiyokawa Nozomi', (0, 0, 92, 22), 'left'),
        ('sasaki-ayako', 'actress', '笹木綾子', 'Sasaki Ayako', (12, 22, 130, 50), 'left'),
        ('kagami-mira', 'character', '鏡　魅羅', 'Kagami Mira', (158, 132, 252, 154), 'right'),
        ('igarashi-rei', 'actress', '五十嵐麗', 'Igarashi Rei', (172, 154, 288, 182), 'right')]),
    ('cast-7', (433, 460), [
        ('asahina-yuko', 'character', '朝日奈夕子', 'Asahina Yuko', (136, 0, 242, 23), 'right'),
        ('teppozuka-yoko', 'actress', '鉄炮塚葉子', 'Teppozuka Yoko', (152, 23, 288, 48), 'right'),
        ('mikihara-megumi', 'character', '美樹原　愛', 'Mikihara Megumi', (0, 133, 106, 157), 'left'),
        ('kurihara-mikiko', 'actress', '栗原みきこ', 'Kurihara Mikiko', (8, 157, 142, 184), 'left')]),
    ('cast-8', (477, 497), [
        ('tatebayashi-miharu', 'character', '館林見晴', 'Tatebayashi Miharu', (162, 6, 268, 27), 'right'),
        ('kikuchi-shiho', 'actress', '菊池志穂', 'Kikuchi Shiho', (170, 27, 288, 55), 'right')]),
]
# Frames where caption-coloured texels are background, not a caption (checked
# by eye on the decoded frames): magenta cherry blossoms behind the cast-1
# header and actress covers before the credits dissolve in. The
# outside-window check skips only these frames.
BACKGROUND_INK = {'cast-1': range(246, 250)}
SMALL_BASELINE = 9  # native 12px cell: capitals on rows 1-9, descenders on row 10


def caption_ink(frame):
    """Caption-coloured texels: saturated magenta pill or pale-pink pill body."""
    r, g, b = frame[..., 0], frame[..., 1], frame[..., 2]
    return (r - g > 60) & (b - g > 40)


def read_raw(track):
    with open(track, 'rb') as handle:
        handle.seek(LBA * SECTOR)
        raw = handle.read(SECTORS * SECTOR)
    if digest(raw) != RAW_SHA:
        raise ValueError('OPENXA raw sectors differ')
    return raw


def decode(raw, scratch):
    """Decode every video frame with ffmpeg's psxstr demuxer; returns (frames array, ffmpeg version)."""
    source = scratch / 'OPENXA.raw'
    source.write_bytes(raw)
    (scratch / 'frames').mkdir()
    subprocess.run(['ffmpeg', '-hide_banner', '-loglevel', 'error', '-f', 'psxstr', '-i', str(source), '-map', '0:v:0',
                    '-fps_mode', 'passthrough', str(scratch / 'frames/f%04d.png')], check=True)
    version = subprocess.run(['ffmpeg', '-version'], check=True, capture_output=True, text=True).stdout.splitlines()[0]
    names = sorted((scratch / 'frames').iterdir())
    if len(names) != FRAMES:
        raise ValueError('decoded frame count differs: %d' % len(names))
    frames = np.stack([np.asarray(Image.open(n).convert('RGB')) for n in names])
    if frames.shape[1:3] != (FRAME_SIZE[1], FRAME_SIZE[0]):
        raise ValueError('decoded frame size differs')
    return frames, version


def italic(mask, baseline):
    """Whole-texel italic: a row k rows above the baseline moves right by k // 4."""
    height, width = mask.shape
    out = np.zeros((height, width + (baseline + 3) // 4), bool)
    for y in range(height):
        shift = max(baseline - y, 0) // 4
        out[y, shift:shift + width] = mask[y]
    return out


def small_mask(font, text):
    """Native 12px Latin masks (unscaled), one-texel bold, italic."""
    glyphs = [font.glyph(c) for c in text]
    width = sum(g['width'] + 1 for g in glyphs) + len(glyphs) * GAP
    mask = np.zeros((12, width), bool)
    cursor = 0
    for g in glyphs:
        for dx, dy in g['points']:
            mask[dy, cursor + dx] = True
            mask[dy, cursor + dx + 1] = True
        cursor += g['width'] + 1 + GAP
    return trim(italic(mask, SMALL_BASELINE))[1:11]  # capitals on rows 1-9, descenders on row 10


def large_mask(text):
    """Port-made large glyphs, one-texel bold like the small line, italic."""
    glyphs = [large_glyph(c) for c in text]
    width = sum(g['width'] + 1 for g in glyphs) + len(glyphs) * GAP
    mask = np.zeros((LARGE_HEIGHT, width), bool)
    cursor = 0
    for g in glyphs:
        for dx, dy in g['points']:
            mask[dy, cursor + dx] = True
            mask[dy, cursor + dx + 1] = True
        cursor += g['width'] + 1 + GAP
    return trim(italic(mask, LARGE_BASELINE))


def trim(mask):
    columns = np.nonzero(mask.any(axis=0))[0]
    return mask[:, columns.min():columns.max() + 1]


def capsule_alpha(width, height, open_left, open_right, radius=None, supersample=4):
    """Coverage of a rounded pill by supersampling; an open end stays square (bleeds off the frame).

    radius defaults to height / 2 (a capsule with half-disc ends); a smaller
    radius gives a rounded rectangle.
    """
    s = supersample
    ys, xs = np.mgrid[0:height * s, 0:width * s]
    cx, cy = (xs + 0.5) / s, (ys + 0.5) / s
    r = height / 2 if radius is None else min(radius, height / 2)
    # Distance from the inner rectangle whose rounding by r gives the pill; open ends do not round.
    nx = np.clip(cx, -np.inf if open_left else r, np.inf if open_right else width - r)
    ny = np.clip(cy, r, height - r)
    inside = (cx - nx) ** 2 + (cy - ny) ** 2 <= r ** 2
    return inside.reshape(height, s, width, s).mean(axis=(1, 3))


def render_pill(text_mask, cover, anchor, band, extend=3, bleed=(False, False), radius=None):
    """Return (RGBA array, [x, y]) for a pill over the caption rows and cover columns (plus extend each side).

    band is the first and last frame row holding the original caption; the pill
    spans it with one spare row each side, or the lettering plus its shadow,
    whichever is taller, centred on the original caption. The pill stays inside
    the frame; an end at the frame edge is cut square only when bleed allows it
    for that side (the original pill runs off the frame there), otherwise it
    keeps its rounded cap. radius as in capsule_alpha.
    """
    x0, _, x1, _ = cover
    th = text_mask.shape[0]
    height = max(band[1] - band[0] + 3, th + 3)
    top = (band[0] + band[1] + 1) // 2 - height // 2
    pad = height // 2
    width = max(x1 - x0 + 2 * extend, text_mask.shape[1] + 2 + 2 * pad)
    left = x1 + extend - width if anchor == 'right' else x0 - extend
    left = min(max(left, 0), FRAME_SIZE[0] - width)
    if top < 0:
        top, height = 0, height + top
    if top + height > FRAME_SIZE[1]:
        height = FRAME_SIZE[1] - top
    alpha = capsule_alpha(width, height, bleed[0] and left == 0, bleed[1] and left + width == FRAME_SIZE[0], radius)
    t = np.linspace(0, 1, width)[None, :, None]
    rgb = (np.array(PILL_SATURATED) * (1 - t) + np.array(PILL_PALE) * t).repeat(height, axis=0)
    tw = text_mask.shape[1]
    tx = (width - tw - 1) // 2
    ty = (height - th - 1) // 2
    if tx < 1 or ty < 0:
        raise ValueError('lettering does not fit its pill')
    shadow = np.zeros((height, width), bool)
    shadow[ty + 1:ty + 1 + th, tx + 1:tx + 1 + tw] = text_mask
    ink = np.zeros((height, width), bool)
    ink[ty:ty + th, tx:tx + tw] = text_mask
    rgb[shadow & ~ink] = SHADOW
    rgb[ink] = TEXT
    out = np.concatenate([np.rint(rgb), np.rint(alpha * 255)[..., None]], axis=2).astype(np.uint8)
    if (alpha[ink | shadow] < 1).any():
        raise ValueError('lettering leaves the opaque pill')
    return out, [int(left), int(top)]


def ink_ratios(frames, cover, window, lo, hi):
    """Caption-ink count inside cover per frame in [lo, hi], over the median of the window interior."""
    x0, y0, x1, y1 = cover
    counts = {f: int(caption_ink(frames[f, y0:y1, x0:x1].astype(int)).sum()) for f in range(lo, hi + 1)}
    median = float(np.median([counts[f] for f in range(window[0] + EDGE, window[1] - EDGE + 1)]))
    return {f: counts[f] / median for f in counts}


def lettering_contrast(frames, window, cover, lo, hi):
    """Per frame in [lo, hi]: mean luma of the caption's white lettering minus a 1-texel ring
    around it, over the median of the window interior. Sees the faint white ghost of a
    dissolve that the pill-colour ink measure misses."""
    x0, y0, x1, y1 = cover
    stack = frames[window[0] + EDGE:window[1] - EDGE + 1, y0:y1, x0:x1].astype(int)
    text = (stack.min(axis=3) > 200).mean(axis=0) >= 0.6
    if not text.any():
        return {f: 0.0 for f in range(lo, hi + 1)}
    ring = dilate(text, 2) & ~dilate(text, 1)
    luma = frames[lo:hi + 1, y0:y1, x0:x1].astype(int).sum(axis=3) / 3.0
    contrast = luma[:, text].mean(axis=1) - luma[:, ring].mean(axis=1)
    full = float(np.median(contrast[window[0] + EDGE - lo:window[1] - EDGE + 1 - lo]))
    return {lo + i: float(c) / full for i, c in enumerate(contrast)}


def ghost_window(frames, window, pills):
    """The cue window widened by its ghost frames (see GHOST)."""
    near, far = GHOST_BACKGROUND
    lo, hi = max(window[0] - far, 0), min(window[1] + far, FRAMES - 1)
    per_pill = [lettering_contrast(frames, window, p[4], lo, hi) for p in pills]
    median = {f: float(np.median([c[f] for c in per_pill])) for f in range(lo, hi + 1)}
    # Per side, then the lower: a neighbouring cue's caption can raise one side.
    background = min(float(np.median([median[f] for f in range(lo, window[0] - near + 1)])),
                     float(np.median([median[f] for f in range(window[1] + near, hi + 1)])))
    first, last = window
    while first > window[0] - GHOST_REACH and median[first - 1] - background > GHOST:
        first -= 1
    while last < window[1] + GHOST_REACH and median[last + 1] - background > GHOST:
        last += 1
    return first, last


def measure(frames, cue_window, pills, background=()):
    """Per-pill frame range (cue window plus ghost frames) and the caption-ink ratio on its fade frames.

    Pills are drawn opaque on every frame of their range: any translucency lets
    the dissolving Japanese show, and the ink ratio (pill colour only)
    undercounts the white lettering, so the ratios are recorded for reference,
    not as opacities. Fails closed when an interior frame lacks caption ink or
    when, outside the range and outside the given background frames
    (caption-coloured scenery checked by eye), the cue's median ink ratio says a
    caption is visible. Ghost frames are flagged as estimated.
    """
    first, last = ghost_window(frames, cue_window, pills)
    lo = max(cue_window[0] - FADE_MARGIN, 0)
    hi = min(cue_window[1] + FADE_MARGIN, FRAMES - 1)
    ratios = {p[0]: ink_ratios(frames, p[4], cue_window, lo, hi) for p in pills}
    result = {}
    for pill_id in ratios:
        fade, estimated = {}, []
        for f in range(first, last + 1):
            ratio = ratios[pill_id][f]
            if cue_window[0] + EDGE <= f <= cue_window[1] - EDGE:
                if ratio < FULL:
                    raise ValueError('cue interior frame below full caption ink: %s %d' % (pill_id, f))
                continue
            fade[f] = round(ratio, 2)
            if not cue_window[0] <= f <= cue_window[1]:
                estimated.append(f)
        result[pill_id] = {'first_frame': first, 'last_frame': last, 'fade_ink_ratio': fade, 'estimated_frames': estimated}
    for f in range(lo, hi + 1):
        if f not in background and not first <= f <= last:
            if float(np.median([ratios[p][f] for p in ratios])) > OFF:
                raise ValueError('caption ink outside every pill window at frame %d' % f)
    return result


def dilate(mask, steps):
    out = mask.copy()
    for _ in range(steps):
        grown = out.copy()
        grown[1:] |= out[:-1]
        grown[:-1] |= out[1:]
        grown[:, 1:] |= out[:, :-1]
        grown[:, :-1] |= out[:, 1:]
        out = grown
    return out


def persistent_caption(frames, window, cover):
    """Caption texels inside cover over the full frames of the window.

    Pill: caption-coloured in at least 60% of the frames. Lettering: white
    (text) or dark purple (shadow) in at least 60% of the frames and within
    CAPTION_REACH texels of the pill; the Japanese text overhangs the pill.
    """
    first, last = window
    x0, y0, x1, y1 = cover
    stack = frames[first + EDGE:last - EDGE + 1, y0:y1, x0:x1].astype(int)
    pill = caption_ink(stack).mean(axis=0) >= 0.6
    r, g, b = stack[..., 0], stack[..., 1], stack[..., 2]
    text = np.minimum(np.minimum(r, g), b) > 200
    shadow = (r + g + b < 260) & (b > g + 15) & (r > g + 10)
    lettering = (text | shadow).mean(axis=0) >= 0.6
    return pill | (lettering & dilate(pill, CAPTION_REACH))


def bleeds(frames, window, cover):
    """(left, right): whether the original pill runs off the frame at that side.

    True only where the cover reaches the frame edge and the pill body fills at
    least BLEED of its rows in the edge column; a rounded cap ending at the
    edge touches only a few rows there.
    """
    x0, y0, x1, y1 = cover
    stack = frames[window[0] + EDGE:window[1] - EDGE + 1, y0:y1, x0:x1].astype(int)
    body = caption_ink(stack).mean(axis=0) >= 0.6
    rows = max(int(body.any(axis=1).sum()), 1)
    return (x0 == 0 and body[:, 0].sum() >= BLEED * rows,
            x1 == FRAME_SIZE[0] and body[:, -1].sum() >= BLEED * rows)


def check_coverage(caption, cover, pill, origin):
    """Every persistent caption texel must lie under fully opaque English pill texels."""
    x0, y0 = cover[:2]
    left, top = origin
    ys, xs = np.nonzero(caption)
    px, py = xs + x0 - left, ys + y0 - top
    if (px < 0).any() or (py < 0).any() or (px >= pill.shape[1]).any() or (py >= pill.shape[0]).any():
        raise ValueError('caption ink outside the English pill')
    thin = pill[py, px, 3] < 255
    if thin.any():
        raise ValueError('caption ink under a translucent pill texel: %d at %s' % (
            int(thin.sum()), sorted(set(zip((px[thin] + left).tolist(), (py[thin] + top).tolist())))[:6]))
    return int(len(ys))


def composite(frame, overlays, opacity=1.0):
    out = frame.astype(float)
    for pill, (left, top) in overlays:
        h, w = pill.shape[:2]
        a = pill[..., 3:4] / 255.0 * opacity
        region = out[top:top + h, left:left + w]
        region[:] = region * (1 - a) + pill[..., :3] * a
    return np.rint(out).astype(np.uint8)


def build(output, track=TRACK, exe=EXE):
    output = new_output(output)
    font = NativeLatin(exe)
    raw = read_raw(track)
    with tempfile.TemporaryDirectory(dir=ROOT / '.asset-cache') as scratch:
        frames, version = decode(raw, Path(scratch))
    output.mkdir(parents=True)
    (output / 'pills').mkdir()
    (output / 'compare').mkdir()
    cues, assets = [], []
    for cue_id, window, pills in CUES:
        timing = measure(frames, window, pills, BACKGROUND_INK.get(cue_id, ()))
        mid = (window[0] + window[1]) // 2
        overlays, rows = [], []
        for pill_id, role, japanese, english, cover, anchor, *_ in pills:
            mask = large_mask(english) if role == 'actress' else small_mask(font, english)
            caption = persistent_caption(frames, window, cover)
            rows_used = np.nonzero(caption.any(axis=1))[0]
            band = (cover[1] + int(rows_used.min()), cover[1] + int(rows_used.max()))
            bleed = bleeds(frames, window, cover)
            art = None
            # Prefer half-disc caps (radius None) at the smallest overhang; round less only if needed.
            for radius in (None,) + tuple(range(LARGE_HEIGHT, MIN_RADIUS - 1, -1)):
                for extend in EXTENDS:
                    candidate, origin = render_pill(mask, cover, anchor, band, extend, bleed, radius)
                    try:
                        covered = check_coverage(caption, cover, candidate, origin)
                    except ValueError:
                        continue
                    art = candidate
                    break
                if art is not None:
                    break
            if art is None:
                raise ValueError('no pill shape hides the caption: ' + pill_id)
            name = 'pills/%s.png' % pill_id
            Image.fromarray(art, 'RGBA').save(output / name)
            sha = digest((output / name).read_bytes())
            overlays.append((art, origin))
            rows.append({'id': pill_id, 'role': role, 'japanese': japanese, 'english': english, 'png': name,
                         'sha256': sha, 'x': origin[0], 'y': origin[1], 'width': int(art.shape[1]),
                         'height': int(art.shape[0]), 'cover_rect': list(cover), 'anchor': anchor,
                         'caption_texels_covered': covered, 'overhang': extend,
                         'end_radius': 'half height' if radius is None else radius,
                         'bleed': {'left': bool(bleed[0]), 'right': bool(bleed[1])}, 'first_frame': timing[pill_id]['first_frame'],
                         'last_frame': timing[pill_id]['last_frame'],
                         'fade_ink_ratio': {str(f): v for f, v in sorted(timing[pill_id]['fade_ink_ratio'].items())},
                         'estimated_frames': timing[pill_id]['estimated_frames']})
            x0, y0, x1, y1 = cover
            assets.append({'id': 'openxa-pill-' + pill_id, 'item_id': CATALOG_ITEM, 'catalog_group': CATALOG_ITEM,
                           'cue': cue_id, 'output': name, 'output_sha256': sha, 'dimensions': [int(art.shape[1]), int(art.shape[0])],
                           'source_crop_rgb_sha256': digest(np.ascontiguousarray(frames[mid, y0:y1, x0:x1]).tobytes()),
                           'english': english, 'japanese': japanese, 'role': role,
                           'lettering': 'native12 masks, 1-texel bold, row-shift italic' if role != 'actress'
                           else 'port-made large face (translation_movie_font.LARGE), row-shift italic',
                           'port_made': True,
                           'port_made_elements': [
                               {'element': 'pill', 'description': 'opaque rounded pill, horizontal gradient %s to %s sampled from the movie pills'
                                % (PILL_SATURATED, PILL_PALE)},
                               {'element': 'lettering', 'description': ('native-font Latin masks, unscaled, with a one-texel bold and '
                                                                         'row-shift italic' if role != 'actress' else
                                                                         'port-made bold sans glyphs, cap height %d, 2-texel stems' % CAP_HEIGHT) +
                                '; white with a dark purple (+1,+1) shadow like the movie lettering'}],
                           'style': 'unreviewed', 'human_approved': False, 'runtime_integrated': False, 'in_game_verified': False})
        frame = frames[mid]
        Image.fromarray(np.concatenate([frame, composite(frame, overlays)], axis=1)).save(output / 'compare' / (cue_id + '.png'))
        cues.append({'id': cue_id, 'first_frame': min(r['first_frame'] for r in rows),
                     'last_frame': max(r['last_frame'] for r in rows), 'caption_window': list(window), 'reference_frame': mid,
                     'reference_frame_sha256': digest(frame.tobytes()), 'pills': rows})
    table = {'schema': 'tokimemo.movie-overlay-cues.v1', 'catalog_item': CATALOG_ITEM, 'revision': REVISION,
             'source': {'path': 'CDROM/OPENXA.STR', 'track': 'track1.bin', 'lba': LBA, 'sectors': SECTORS, 'sector_bytes': SECTOR,
                        'raw_sha256': RAW_SHA, 'decoder': version, 'decoded_frames': FRAMES, 'frame_size': list(FRAME_SIZE)},
             'frame_index': 'zero-based decoded video output index (ffmpeg psxstr); presentation timing is not proved',
             'opacity_rule': 'draw each pill fully opaque on its own frames first_frame..last_frame, so no dissolving '
                             'Japanese shows through. A cue spans the union of its pills; caption_window is the measured '
                             'caption ink. fade_ink_ratio (pill-colour ink inside the cover over the cue median) is '
                             'informative only: it undercounts the white lettering, so it is not an opacity. '
                             'estimated_frames are ghost frames: the white lettering still shows faintly beyond '
                             'the pill-colour ink (lettering contrast %.2f above the background)' % GHOST,
             'shape_rule': 'pills keep rounded ends inside the frame; an end is cut square at the frame edge only where '
                           'bleed is true (the original pill body fills %.0f%% of its rows in the edge column); '
                           'end_radius is reduced from a half-disc only when a full cap cannot hide the caption' % (BLEED * 100),
             'placement': 'x, y in decoded-frame texels; draw the PNG unscaled with its alpha',
             'cues': cues, 'human_approved': False, 'runtime_integrated': False}
    table_bytes = encoded(table) + b'\n'
    (output / 'cue_table.json').write_bytes(table_bytes)
    manifest = {'schema': 'tokimemo.movie-pill-drafts.v1', 'ticket': 'T-39004', 'catalog_item': CATALOG_ITEM, 'revision': REVISION,
                'source_container': 'CDROM/OPENXA.STR', 'source_raw_sha256': RAW_SHA, 'cue_table_sha256': digest(table_bytes),
                'assets': assets, 'font_provenance': font.provenance(), 'human_approved': False,
                'wording': 'romanised names, family name first (GLOSSARY.md order); actress romanisations are agent defaults'}
    (output / 'manifest.json').write_bytes(encoded(manifest) + b'\n')
    return {'output': str(output), 'pills': len(assets), 'cues': len(cues)}


def publish(built, destination):
    """Copy pills, cue table and a candidate ledger (no movie frames) into a new checkpoint directory."""
    manifest_bytes = (built / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    table_bytes = (built / 'cue_table.json').read_bytes()
    if digest(table_bytes) != manifest['cue_table_sha256']:
        raise ValueError('cue table changed')
    destination = destination.resolve()
    destination.relative_to((ROOT / 'translations/en/graphics/artwork/checkpoints').resolve())
    if destination.exists():
        raise ValueError('destination must be new')
    ledger = []
    for row in manifest['assets']:
        data = (built / row['output']).read_bytes()
        if digest(data) != row['output_sha256']:
            raise ValueError('built output changed: ' + row['id'])
        if row['human_approved'] is not False or row['port_made'] is not True:
            raise ValueError('pill record claims approval or omits port-made: ' + row['id'])
    (destination / REVISION / 'pills').mkdir(parents=True)
    for row in manifest['assets']:
        shutil.copyfile(built / row['output'], destination / REVISION / row['output'])
        ledger.append({'id': row['id'], 'item_id': row['item_id'], 'cue': row['cue'], 'path': REVISION + '/' + row['output'],
                       'revision': REVISION, 'output_sha256': row['output_sha256'], 'dimensions': row['dimensions'],
                       'manifest_sha256': digest(manifest_bytes), 'source_raw_sha256': RAW_SHA,
                       # OPENXA.STR has no file-level hash (stream sectors), as the inventory records for catalog-153.
                       'source_container_sha256': None, 'source_png_sha256': row['source_crop_rgb_sha256'],
                       'source_png_sha256_basis': 'sha256 of the raw RGB24 bytes of the cover crop of the decoded reference frame; '
                                                  'no movie image is stored',
                       'status': 'draft', 'human_approved': False, 'port_made': True,
                       'port_made_elements': row['port_made_elements'],
                       'reason': 'Port-made English cast-credit pill for a movie overlay; unreviewed; no movie player port yet.'})
    shutil.copyfile(built / 'manifest.json', destination / REVISION / 'manifest.json')
    shutil.copyfile(built / 'cue_table.json', destination / REVISION / 'cue_table.json')
    (destination / 'ledger.json').write_bytes(encoded({
        'schema': 'tokimemo.translation-checkpoint.v1', 'ticket': 'T-39004', 'catalog_group': CATALOG_ITEM,
        'assets': ledger, 'candidate_assets': len(ledger), 'cue_table': REVISION + '/cue_table.json',
        'cue_table_sha256': digest(table_bytes), 'original_images_included': False,
        'complete_translation_inventory': False, 'human_approved': False,
        'note': 'Overlay pills, not frame replacements: the cue table says when and where each is drawn.',
        'publication_authorization': 'User asked to finish asset translation or creation, 2026-10-30.'}) + b'\n')
    return {'destination': str(destination), 'pills': len(ledger)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--track', type=Path, default=TRACK, help='raw track 1 image holding OPENXA.STR')
    parser.add_argument('--exe', type=Path, default=EXE, help='SLPM executable holding the native font')
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--publish', type=Path, help='also copy pills, cue table and a ledger into this new checkpoint')
    args = parser.parse_args()
    summary = build(args.output, args.track, args.exe)
    if args.publish:
        summary['published'] = publish(args.output, args.publish)
    print(json.dumps(summary))
