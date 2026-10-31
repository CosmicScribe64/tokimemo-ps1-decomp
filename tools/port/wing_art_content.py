#!/usr/bin/env python3
"""T-39220: export the optional wing-art pack (decorative widescreen side art).

Every pixel comes from the user's original disc. The exporter reads a fixed
list of source files, checks each against its pinned SHA-256, and records the
identities in the pack manifest. Nothing is drawn: the only transforms are
crop, 1x mirror/reflect, integer grading (desaturate, tint, gain, vignette,
drop shadow) and the PS1 4x4 ordered dither before the 5-bit snap.

All grading is integer maths on 5-bit channels (Q12 fixed point), so the same
input always yields the same bytes. The recipes come from the round-2 mock-ups
(.asset-cache/wing-art-r2-2026-10-30, dir-2/make_dir2.py) with these fixes:

- yozakura: lower-third lift against flat bushes, petal alpha capped at 0.8 and
  dimmer petals, and a non-mirrored centre zone in the top/bottom bands so the
  mirror seam no longer meets itself at the centre line;
- summer: HANABI bursts at additive strength 0.25, pre-cropped to two radii so
  the runtime only draws one that fits inside the visible side area, and left
  and right bursts alternate at mirrored positions;
- winter: no particles. TARA_OBJ/YUKI.BIN holds a window sprite and non-image
  data, not snowflakes, and no other snow sprite is known in the data.

Pack layout (all paths relative to the pack root):
  manifest.json              schema tokimemo.wing-art.v1, sources, styles, files
  plates/<style>.png         720x300 RGB, aperture at (200, 30)
  atlas/petals.png           12 O_01 petal frames, RGBA (alpha 0 or 255)
  atlas/leaves.png           the same frames graded amber (autumn)
  atlas/fireworks-<r>.png    16 HANABI steps, RGBA (alpha = radial edge fade)
  overrides.json             scene/location/stage -> style table

Run in Docker (repository mounted at /work, disc read-only):
  python3 tools/port/wing_art_content.py --input disc/files --output .asset-cache/<new dir>
"""
from __future__ import annotations

import argparse
import hashlib
import json
import math
import os
from pathlib import Path
import shutil
import struct
import sys
import tempfile
import zlib

import numpy as np

ROOT = Path(__file__).resolve().parents[2]
SCHEMA = "tokimemo.wing-art.v1"
OVERRIDES_SCHEMA = "tokimemo.wing-art.overrides.v1"

# Plate geometry, in logical (native) pixels. 720 columns cover a 21:9 window
# (160 visible columns per side) plus margin; 300 rows cover the 15-row bands
# of 16:9 and 21:9 windows at 1080p with margin.
PLATE_W, PLATE_H = 720, 300
AX, AY = 200, 30
CENTRE_X = PLATE_W // 2
ZONE_START, ZONE_W = CENTRE_X - 72, 144  # non-mirrored band zone
LOOP_TICKS = 120  # 4 s at the 30 Hz presentation clock
FW_LOOP_TICKS = 240

SOURCES = {
    "D_10_08": ("CDROM/BS/D_10_08.BS", "5c4b5fea51ecaa0a469abecd886c60972980b491d8e20007cc62b630e372322d"),
    "D_10_07B": ("CDROM/BS/D_10_07B.BS", "5fce7b7a3ec831fdfc7ad8f2f2b5359cb8d7d9d98d6edfc68754816760aa5a8a"),
    "D_10_07": ("CDROM/BS/D_10_07.BS", "104a2d74a022ea0bc1f4361177a3ac169d784719d72a836b34918a4a3993959a"),
    "I_06_13": ("CDROM/BS/I_06_13.BS", "d8b511b0470ca72e50f554626d1a390538f46813d6c9b87844a8fa516efea5f8"),
    "D_18_05": ("CDROM/BS/D_18_05.BS", "25186c1ad62b0ef917182252c35f837f752d0de0f2d41f488909e6c9ac8414e6"),
    "D_01_54": ("CDROM/BS/D_01_54.BS", "efee894d42097ce1fd06aebe19447272f48ad5801e31a20380e1f6a5cc71b2c4"),
    "D_02_05": ("CDROM/BS/D_02_05.BS", "223d28caa9ee5cfdbe71c8b90db60ccf090c17146d77cd5c6110f0d43d49dcef"),
    "D_22_06": ("CDROM/BS/D_22_06.BS", "787a8af36cf1aaf4529abc42d13309d0adc03a1c8d8fe1b0ce9a78992e8df96c"),
    "X_01_13": ("CDROM/BS/X_01_13.BS", "2be9eaa707dd2ffd064f287aa9ccfbba191ee7ad67a76bcce1c8805096dfabcb"),
    "O_01": ("CDROM/PROLODIR/O_01.NBN", "bb224a47fa1ac9c102dd4701d3d3e0819ba30cac1e5637183886b8be4f95340c"),
    "HANABI": ("CDROM/TARA_OBJ/HANABI.BIN", "a0d399de88616291e822064140cf91746043398d59287e4d7ba0bc49244c7ae6"),
}
# BS stills have no stored size; these are the decoded shapes (macroblock count
# checked against the stream before decoding).
STILL_SHAPES = {"X_01_13": (320, 240)}
DEFAULT_STILL_SHAPE = (320, 160)

Q = 4096  # Q12 one

# Look parameters. Fractions are Q12 integers. edge_col is the source column
# shown at the aperture edge (the left wing reads outward from it, the band
# reads inward); row0 is the plate row of source row 0. sky = (rows, dx, fade)
# fills above the still with the still's top `rows` rows, shifted dx columns
# and reflected at each fold (never tiled, so no hard seam), cross-faded into
# the still over `fade` rows. zone_col is the first source column of the non-mirrored
# centre zone of the bands.
LOOKS = {
    "yozakura": dict(src="D_10_08", label="Night sakura avenue", kind="seasonal", edge_col=159, zone_col=176, row0=115,
                     sky=(72, 41, 22), base=2540, tint=(2703, 2867, 4506), desat=1638,
                     lift=(195, 230, 4669, 1024), particles="petals", outdoor=True),
    "spring_day": dict(src="D_10_08", label="Sakura avenue (day)", kind="seasonal", edge_col=159, zone_col=176, row0=115,
                       sky=(72, 41, 22), base=2458, tint=(4096, 3809, 4178), desat=737,
                       particles="petals", outdoor=True),
    "summer": dict(src="I_06_13", label="Summer night sky", kind="seasonal", edge_col=275, row0=100,
                   sky=(110, 131, 16), base=2867, tint=(4096, 4096, 4096), desat=205,
                   particles="fireworks", outdoor=True),
    "autumn": dict(src="D_10_07B", label="Ginkgo avenue", kind="seasonal", edge_col=159, zone_col=176, row0=60,
                   sky=(34, 40, 22), base=2458, tint=(4424, 3973, 3441), desat=328,
                   particles="leaves", outdoor=True),
    "winter": dict(src="D_18_05", label="Snowfield at dusk", kind="seasonal", edge_col=315, row0=111,
                   sky=(40, 97, 14), base=2130, tint=(2867, 3359, 4588), desat=1434,
                   particles="none", outdoor=True),
    "school": dict(src="D_01_54", label="School grounds", kind="location", edge_col=159, zone_col=176, row0=95,
                   sky=(40, 70, 18), base=2376, tint=(4096, 4014, 3891), desat=1229,
                   particles="season", outdoor=True),
    "park": dict(src="D_10_07", label="Park avenue", kind="location", edge_col=159, zone_col=176, row0=60,
                 sky=(34, 40, 22), base=2376, tint=(4014, 4096, 3973), desat=1229,
                 particles="season", outdoor=True),
    "town": dict(src="D_02_05", label="Home street", kind="location", edge_col=159, zone_col=176, row0=95,
                 sky=(40, 60, 18), base=2376, tint=(4096, 4014, 3891), desat=1229,
                 particles="season", outdoor=True),
    "seaside": dict(src="D_22_06", label="Seaside", kind="location", edge_col=159, zone_col=176, row0=95,
                    sky=(40, 0, 20), base=3072, tint=(3973, 4055, 4178), desat=819,
                    particles="none", outdoor=True),
    "legend_tree": dict(src="X_01_13", label="Legendary tree", kind="location", edge_col=319, row0=30, zone_col=176,
                        base=2867, tint=(4178, 3973, 3891), desat=1024, particles="season", outdoor=True),
}
SEASONS = {"spring": "yozakura", "summer": "summer", "autumn": "autumn", "winter": "winter"}
SEASON_PARTICLES = {"spring": "petals", "summer": "none", "autumn": "leaves", "winter": "none"}
# Months are 1..12; Mar-May spring, Jun-Aug summer, Sep-Nov autumn, Dec-Feb winter.
MONTH_SEASONS = {3: "spring", 4: "spring", 5: "spring", 6: "summer", 7: "summer", 8: "summer",
                 9: "autumn", 10: "autumn", 11: "autumn", 12: "winter", 1: "winter", 2: "winter"}

# Scene override table shipped with the pack. Rules are tried most specific
# first (stage, background, location, mode, tag, class); a style is a look id,
# "seasonal" or "black". Users may prepend their own rules at runtime.
DEFAULT_OVERRIDES = {
    "schema": OVERRIDES_SCHEMA,
    "default": "seasonal",
    "rules": [
        {"match": {"stage": "prologue"}, "style": "legend_tree"},
        {"match": {"stage": "ending"}, "style": "legend_tree"},
        {"match": {"stage": "name_entry"}, "style": "seasonal"},
        {"match": {"background_prefix": "X_01_"}, "style": "legend_tree"},
        {"match": {"background_prefix": "P_01_"}, "style": "legend_tree"},
        {"match": {"background_prefix": "D_01_"}, "style": "school"},
        {"match": {"background_prefix": "C_01_"}, "style": "school"},
        {"match": {"background_prefix": "C_02_"}, "style": "school"},
        {"match": {"background_prefix": "I_01_"}, "style": "school"},
        {"match": {"background_prefix": "D_10_"}, "style": "park"},
        {"match": {"background_prefix": "D_02_"}, "style": "town"},
        {"match": {"background_prefix": "D_22_"}, "style": "seaside"},
        {"match": {"background_prefix": "I_22_"}, "style": "seaside"},
        {"match": {"location": "school"}, "style": "school"},
        {"match": {"location": "park"}, "style": "park"},
        {"match": {"location": "town"}, "style": "town"},
        {"match": {"location": "seaside"}, "style": "seaside"},
        {"match": {"location": "legend_tree"}, "style": "legend_tree"},
        {"match": {"class": "A"}, "style": "school"},
    ],
}
MATCH_KEYS = ("stage", "background_prefix", "location", "mode", "tag", "class")

DITHER = np.array([[-4, 0, -3, 1], [2, -2, 3, -1], [-3, 1, -4, 0], [3, -1, 2, -2]], np.int64)
LUMA = (1225, 2404, 467)  # 0.299, 0.587, 0.114 in Q12
SIN_STEPS = 64


class WingArtError(ValueError):
    pass


# ------------------------------------------------------------------ integers
def smooth_q12(t):
    """Integer smoothstep on Q12 input (clamped to 0..4096), Q12 output."""
    t = np.clip(np.asarray(t, np.int64), 0, Q)
    return (t * t * (3 * Q - 2 * t)) >> 24


def tdiv(a, b):
    """Integer division truncating toward zero (the GDScript rule)."""
    q = abs(a) // b
    return -q if a < 0 else q


def expand5(v):
    v = np.asarray(v, np.int64)
    return (v << 3) | (v >> 2)


def reflect(i, n):
    i = np.mod(np.asarray(i, np.int64), 2 * n)
    return np.where(i >= n, 2 * n - 1 - i, i)


def sin_table():
    """64-step sine in Q12, rounded half away from zero (stored in the pack)."""
    return [int(math.floor(math.sin(2 * math.pi * k / SIN_STEPS) * Q + 0.5)) for k in range(SIN_STEPS)]


def envelope_table():
    """Petal fade in/out, sin(pi*u)^0.8, Q8 over 64 steps."""
    return [int(math.floor((math.sin(math.pi * k / SIN_STEPS) ** 0.8) * 256 + 0.5)) for k in range(SIN_STEPS)]


# ------------------------------------------------------------------ sources
def rgb555(words):
    w = np.asarray(words, np.int64)
    return np.stack([(w >> s) & 31 for s in (0, 5, 10)], -1).astype(np.uint8)


def decode_still(data, shape):
    """BS still -> (h, w, 3) uint8 of 5-bit channels via the repo decoder."""
    sys.path.insert(0, str(ROOT))
    sys.path.insert(0, str(ROOT / "tools"))
    from assets.image_mdec import _coefficients
    from assets.image_mdec_model import decode_padded_words
    w, h = shape
    _, meta = _coefficients(data)
    if meta["macroblock_count"] != (w // 16) * ((h + 15) // 16):
        raise WingArtError("BS still macroblock count does not match its pinned shape")
    words = np.frombuffer(decode_padded_words(data, w, h), "<u2")[:w * h].reshape(h, w)
    return rgb555(words)


def petal_frames(o01):
    """The 12 O_01.NBN petal frames as FirstArrivalActorRenderer paints them:
    8bpp page at 0xd0, CLUT at 0x4490, frame table at 0x40d0 + i*24, colour 0
    transparent, flip bit1 = H, bit0 = V. Returns [(rgb5 h*w*3, mask h*w)]."""
    b = np.frombuffer(o01, np.uint8)
    if len(b) < 0x4490 + 512:
        raise WingArtError("O_01.NBN is too short")
    page = b[0xd0:0xd0 + 16384].reshape(128, 128)
    clut_w = b[0x4490:0x4490 + 512].view("<u2")
    clut = rgb555(clut_w)
    h16 = lambda o: int.from_bytes(b[o:o + 2].tobytes(), "little")
    out = []
    for i in range(12):
        off = int.from_bytes(b[0x4228 + i * 4:0x422c + i * 4].tobytes(), "little") - 0x80180000
        if off != 0x40d0 + i * 24:
            raise WingArtError("O_01.NBN petal frame table differs")
        row = off + 8
        tex, flip, w, h = h16(row + 4), h16(row + 8), h16(row + 10), h16(row + 12)
        u, v = (tex & 7) * 16, ((tex >> 3) & 7) * 16
        idx = page[v:v + h, u:u + w]
        if flip & 2:
            idx = idx[:, ::-1]
        if flip & 1:
            idx = idx[::-1]
        out.append((clut[idx], clut_w[idx] != 0))
    return out


def leaf_frames(petals):
    """Autumn: the petal frames graded amber on their luma (round-2 recipe)."""
    gains = (6349, 3768, 1556)  # 1.55, 0.92, 0.38
    out = []
    for rgb, mask in petals:
        e = expand5(rgb)
        lum = (e[..., 0] * LUMA[0] + e[..., 1] * LUMA[1] + e[..., 2] * LUMA[2]) >> 12
        ch = [np.clip((lum * g) >> 12, 0, 255) >> 3 for g in gains]
        out.append((np.stack(ch, -1).astype(np.uint8), mask))
    return out


HANABI_SHAPES = ((1622, 1725), (1737, 1863), (1866, 1966), (1997, 2086))
TRAIL_INDICES = (1, 2, 3, 4)  # rocket trail sparks at the shape edges, not the burst


def hanabi_steps(data, radius):
    """HANABI.BIN bursts: 8bpp, 128-px rows; the file's own 16 CLUT steps
    (rows 0, 4, ... 60) animate the burst. Step k uses shape min(k // 4, 3).
    Each step is cropped to a circle of `radius` around the burst centre with a
    6-px edge fade in alpha. Returns (rgba frames, size)."""
    a = np.frombuffer(data, np.uint8)
    if len(a) % 128 or len(a) // 128 < 2086:
        raise WingArtError("HANABI.BIN layout differs")
    a = a.reshape(-1, 128)
    size = 2 * radius + 1
    isqrt = np.vectorize(math.isqrt, otypes=[np.int64])
    frames = []
    for k in range(16):
        r0, r1 = HANABI_SHAPES[min(k // 4, 3)]
        idx = a[r0:r1].astype(np.int64)
        burst = (idx != 0) & ~np.isin(idx, TRAIL_INDICES)
        ys, xs = np.nonzero(burst)
        cy, cx = (int(ys.min()) + int(ys.max())) // 2, (int(xs.min()) + int(xs.max())) // 2
        clut_w = a[k * 4:k * 4 + 4].reshape(-1).view("<u2").astype(np.int64)
        colours = rgb555(clut_w)
        lit = burst & ((clut_w[idx] & 0x7fff) != 0)
        h, w = idx.shape
        yy, xx = np.mgrid[0:h, 0:w].astype(np.int64)
        d16 = isqrt(((xx - cx) ** 2 + (yy - cy) ** 2) * 256)
        fade = smooth_q12(((radius * 16 - d16) * Q) // (6 * 16))
        alpha = np.where(lit, (fade * 255) >> 12, 0)
        frame = np.zeros((size, size, 4), np.uint8)
        # copy the window [cy-radius, cy+radius] x [cx-radius, cx+radius]
        for y in range(size):
            syy = cy - radius + y
            if not 0 <= syy < h:
                continue
            for x in range(size):
                sxx = cx - radius + x
                if 0 <= sxx < w and alpha[syy, sxx] > 0:
                    frame[y, x, :3] = expand5(colours[idx[syy, sxx]])
                    frame[y, x, 3] = alpha[syy, sxx]
        frames.append(frame)
    return frames, size


# ------------------------------------------------------------------ plates
def column_map(edge_col, w, zone_col):
    """Source column per plate column for wing rows (mirrored halves) and for
    band rows (mirrored halves plus a continuous centre zone), with the Q12
    weight of the zone read in band rows."""
    x = np.arange(PLATE_W, dtype=np.int64)
    mx = np.minimum(x, PLATE_W - 1 - x)  # left-half coordinate (mirror)
    # left half: x < AX reads outward (edge_col - (AX - 1 - x)); x >= AX inward.
    wing = reflect(edge_col - (AX - 1 - mx), w)
    zone_start, zone_w, fade = ZONE_START, ZONE_W, 32
    zone = reflect(zone_col + (x - zone_start), w)
    inner = np.minimum(x - zone_start, zone_start + zone_w - 1 - x)
    weight = smooth_q12((inner * Q) // fade)
    weight = np.where((x >= zone_start) & (x < zone_start + zone_w), weight, 0)
    return wing, zone, weight


def panorama(src, look):
    """(PLATE_H, PLATE_W, 3) int64 8-bit field from a 5-bit still."""
    h, w = src.shape[:2]
    field = expand5(src)
    edge_col, row0 = look["edge_col"], look["row0"]
    # The zone starts where the left half would read anyway, so its left
    # cross-fade is invisible and only its right edge blends two reads.
    zone_col = look.get("zone_col", edge_col + ZONE_START - AX + 1)
    wing_cols, zone_cols, zone_weight = column_map(edge_col, w, zone_col)
    y = np.arange(PLATE_H, dtype=np.int64)
    sy = reflect(y - row0, h)
    base = field[sy][:, wing_cols]
    zone = field[sy][:, zone_cols]
    band = (y < AY) | (y >= AY + 240)
    zw = np.where(band[:, None], zone_weight[None, :], 0)
    out = (base * (Q - zw[..., None]) + zone * zw[..., None]) >> 12
    sky = look.get("sky")
    if sky:
        rows, dx, fade = sky
        ys = y - row0
        # A reflected run, not a tiled one: plate row row0-1 reads source
        # row 0, row0-rows reads row rows-1 and folds back. Every fold repeats
        # its edge row, so no row pair above the still has a hard seam, and
        # the alt field is continuous where the fade meets the still.
        alt_rows = reflect(ys, rows)
        alt = field[alt_rows][:, reflect(wing_cols + dx, w)]
        alt_zone = field[alt_rows][:, reflect(zone_cols + dx, w)]
        alt = (alt * (Q - zw[..., None]) + alt_zone * zw[..., None]) >> 12
        # 1 above the still, 0 inside it, smooth over `fade` rows
        wgt = smooth_q12(((fade // 2 - ys) * Q) // fade)[:, None, None]
        out = (out * (Q - wgt) + alt * wgt) >> 12
    return out


def gain_field(look):
    """Q12 gain per plate pixel: radial falloff from the picture, vertical
    band vignette and a 9-px drop shadow cast by the picture."""
    y, x = np.mgrid[0:PLATE_H, 0:PLATE_W].astype(np.int64)
    dx = np.maximum(np.maximum(AX - x, x - (AX + 319)), 0)
    dy = np.maximum(np.maximum(AY - y, y - (AY + 239)), 0)
    d16 = np.vectorize(math.isqrt)((dx * dx + dy * dy) * 256).astype(np.int64)
    r0, r1, outer = 12, 120, 1229  # full gain within 12 px, 0.30 beyond 120 px
    gx = outer + (((Q - outer) * smooth_q12(((r1 * 16 - d16) * Q) // ((r1 - r0) * 16))) >> 12)
    top, band_w = 1802, 22  # 0.44 at 22 rows beyond the picture
    gv = top + (((Q - top) * smooth_q12(((band_w - dy) * Q) // band_w)) >> 12)
    shadow, shadow_w = 2253, 9  # 0.55
    gs = Q - ((shadow * (Q - smooth_q12((d16 * Q) // (shadow_w * 16)))) >> 12)
    g = (look["base"] * gx) >> 12
    g = (g * gv) >> 12
    g = (g * gs) >> 12
    lift = look.get("lift")
    if lift:
        start, end, gain, desat_cut = lift
        ramp = smooth_q12(((y - start) * Q) // (end - start))
        g = (g * (Q + (((gain - Q) * ramp) >> 12))) >> 12
    return g


def grade(field, look):
    """Desaturate, tint and gain (Q12) on the 8-bit field, then the PS1 4x4
    ordered dither and the 5-bit snap. Returns uint8 5-bit channels."""
    desat = np.full((PLATE_H, PLATE_W), look["desat"], np.int64)
    lift = look.get("lift")
    if lift:
        start, end, _gain, desat_cut = lift
        y = np.arange(PLATE_H, dtype=np.int64)[:, None]
        ramp = smooth_q12(((y - start) * Q) // (end - start))
        desat = desat - ((desat_cut * ramp) >> 12)
    lum = (field[..., 0] * LUMA[0] + field[..., 1] * LUMA[1] + field[..., 2] * LUMA[2]) >> 12
    f = (field * (Q - desat[..., None]) + lum[..., None] * desat[..., None]) >> 12
    f = (f * np.array(look["tint"], np.int64)) >> 12
    f = (f * gain_field(look)[..., None]) >> 12
    yy, xx = np.mgrid[0:PLATE_H, 0:PLATE_W]
    f = f + DITHER[yy & 3, xx & 3][..., None]
    return np.clip(f >> 3, 0, 31).astype(np.uint8)


def build_plate(still, look):
    return grade(panorama(still, look), look)


# ------------------------------------------------------------------ particles
class Lcg:
    """Fixed-seed 32-bit LCG (Numerical Recipes constants); presentation only."""
    def __init__(self, seed):
        self.state = seed & 0xffffffff

    def next(self, n):
        self.state = (self.state * 1664525 + 1013904223) & 0xffffffff
        return (self.state >> 8) % n


def style_seed(name):
    return sum((i + 1) * ord(c) for i, c in enumerate(name))


def drifters(name, per_side, fall=(46, 80), bright_q8=(128, 218), cap_q8=255):
    """Petal/leaf drifters. Integer fields (logical px, ticks):
    side 0=left 1=right, ox outward distance from the aperture edge, y0 plate
    row, fall px per loop, drift px per loop (signed), amp sway px, cyc sway
    cycles per loop, ph sway phase (0..63), t0 start tick, f0 first frame,
    k brightness Q8, cap alpha cap Q8, depth 0..255 (draw order)."""
    rng = Lcg(style_seed(name))
    items = []
    for side in (0, 1):
        for _ in range(per_side):
            depth = rng.next(256)
            items.append({
                "side": side, "ox": 2 + rng.next(148), "y0": rng.next(PLATE_H - 70) - 10,
                "fall": (fall[0] + rng.next(fall[1] - fall[0] + 1)) * (179 + (depth * 128 >> 8)) >> 8,
                "drift": (4 + rng.next(11)) * (1 if rng.next(2) else -1),
                "amp": 2 + rng.next(4), "cyc": 1 + rng.next(2), "ph": rng.next(SIN_STEPS),
                "t0": rng.next(LOOP_TICKS), "f0": rng.next(12),
                "k": bright_q8[0] + ((bright_q8[1] - bright_q8[0]) * depth >> 8), "cap": cap_q8,
                "depth": depth})
    items.sort(key=lambda d: (d["depth"], d["side"], d["ox"]))
    return items


def particle_spec(name, kind):
    """Motion spec stored in the manifest; the runtime evaluates it with the
    same integer rules (see presentation/wing_art_provider.gd)."""
    if kind == "petals":
        if name == "yozakura":
            return {"kind": "drift", "atlas": "petals", "loop": LOOP_TICKS, "rate": 10,
                    "items": drifters(name, 16, bright_q8=(96, 160), cap_q8=205)}
        return {"kind": "drift", "atlas": "petals", "loop": LOOP_TICKS, "rate": 10,
                "items": drifters(name, 14)}
    if kind == "leaves":
        return {"kind": "drift", "atlas": "leaves", "loop": LOOP_TICKS, "rate": 10,
                "items": drifters(name, 12, fall=(40, 64), bright_q8=(154, 243))}
    if kind == "fireworks":
        # Left at tick 0, right at the half loop, mirrored positions; the
        # runtime centres each burst in the visible side area and skips any
        # burst whose radius does not fit.
        return {"kind": "fireworks", "loop": FW_LOOP_TICKS, "step_ticks": 4, "strength_q8": 64,
                "bursts": [{"side": 0, "t0": 0, "y": AY + 52}, {"side": 1, "t0": FW_LOOP_TICKS // 2, "y": AY + 52}]}
    return {"kind": "none"}


# ------------------------------------------------------------------ files
def png_bytes(pixels):
    """Deterministic PNG (RGB or RGBA, 8-bit) from an (h, w, c) uint8 array."""
    pixels = np.ascontiguousarray(pixels, np.uint8)
    h, w, c = pixels.shape
    if c not in (3, 4):
        raise WingArtError("PNG needs 3 or 4 channels")

    def chunk(kind, payload):
        return struct.pack(">I", len(payload)) + kind + payload + struct.pack(">I", zlib.crc32(kind + payload) & 0xffffffff)
    raw = b"".join(b"\0" + pixels[y].tobytes() for y in range(h))
    return (b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2 if c == 3 else 6, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b""))


def atlas(frames):
    """Frames side by side (RGBA, 5-bit expanded) and their [x, y, w, h]."""
    width = sum(rgb.shape[1] for rgb, _ in frames)
    height = max(rgb.shape[0] for rgb, _ in frames)
    out = np.zeros((height, width, 4), np.uint8)
    rects, x = [], 0
    for rgb, mask in frames:
        h, w = mask.shape
        out[:h, x:x + w, :3] = np.where(mask[..., None], expand5(rgb), 0)
        out[:h, x:x + w, 3] = np.where(mask, 255, 0)
        rects.append([x, 0, w, h])
        x += w
    return out, rects


def sha(data):
    return hashlib.sha256(data).hexdigest()


def validate_overrides(table, styles):
    """Strict schema check shared with the runtime rules."""
    if not isinstance(table, dict) or table.get("schema") != OVERRIDES_SCHEMA:
        raise WingArtError("override table schema differs")
    allowed = set(styles) | {"seasonal", "black"}
    if table.get("default") not in allowed:
        raise WingArtError("override default style is unknown")
    rules = table.get("rules")
    if not isinstance(rules, list) or len(rules) > 256:
        raise WingArtError("override rules must be a bounded list")
    for rule in rules:
        if not isinstance(rule, dict) or set(rule) != {"match", "style"} or rule["style"] not in allowed:
            raise WingArtError("override rule is malformed or names an unknown style")
        match = rule["match"]
        if not isinstance(match, dict) or len(match) != 1:
            raise WingArtError("override rule must match exactly one key")
        key, value = next(iter(match.items()))
        if key not in MATCH_KEYS or not isinstance(value, str) or not 0 < len(value) <= 64:
            raise WingArtError("override match key or value is invalid")
    return table


def read_sources(input_path, names=None):
    sys.path.insert(0, str(ROOT))
    from tools.assets.input import open_input
    wanted = {k: SOURCES[k] for k in (names or SOURCES)}
    blobs, identities = {}, []
    with open_input(str(input_path)) as original:
        entries = {row["path"]: row for row in original.entries()}
        for key, (path, expected) in sorted(wanted.items()):
            if path not in entries:
                raise WingArtError("original input lacks " + path)
            data = original.read(entries[path])
            if sha(data) != expected:
                raise WingArtError("original source hash differs: " + path)
            blobs[key] = data
            identities.append({"id": key, "path": path, "sha256": expected, "size": len(data)})
    return blobs, identities


def build_pack(blobs, identities, stage):
    """Write the pack into `stage` (an empty directory) and return the manifest."""
    files = {}

    def put(rel, data):
        path = stage / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(data)
        files[rel] = {"sha256": sha(data), "size": len(data)}

    stills = {}
    styles = {}
    for name, look in LOOKS.items():
        src = look["src"]
        if src not in stills:
            stills[src] = decode_still(blobs[src], STILL_SHAPES.get(src, DEFAULT_STILL_SHAPE))
        plate = build_plate(stills[src], look)
        put("plates/%s.png" % name, png_bytes(expand5(plate).astype(np.uint8)))
        particles = look["particles"]
        styles[name] = {"label": look["label"], "kind": look["kind"], "plate": "plates/%s.png" % name,
                        "source": src, "outdoor": look["outdoor"],
                        "particles": particle_spec(name, particles) if particles != "season" else {"kind": "season"}}
    petals = petal_frames(blobs["O_01"])
    atlases = {}
    for key, frames in (("petals", petals), ("leaves", leaf_frames(petals))):
        image, rects = atlas(frames)
        put("atlas/%s.png" % key, png_bytes(image))
        atlases[key] = {"path": "atlas/%s.png" % key, "frames": rects}
    for radius in (30, 58):
        frames, size = hanabi_steps(blobs["HANABI"], radius)
        sheet = np.concatenate(frames, 1)
        rel = "atlas/fireworks-%d.png" % radius
        put(rel, png_bytes(sheet))
        atlases["fireworks-%d" % radius] = {"path": rel, "radius": radius, "size": size, "steps": 16}
    season_drift = {season: (particle_spec("season-" + season, kind) if kind != "none" else {"kind": "none"})
                    for season, kind in SEASON_PARTICLES.items()}
    overrides = validate_overrides(json.loads(json.dumps(DEFAULT_OVERRIDES)), styles)
    put("overrides.json", (json.dumps(overrides, sort_keys=True, indent=2) + "\n").encode())
    manifest = {
        "schema": SCHEMA, "version": 1,
        "geometry": {"plate_width": PLATE_W, "plate_height": PLATE_H, "aperture_x": AX, "aperture_y": AY},
        "sources": identities, "styles": styles, "seasons": SEASONS, "month_seasons":
            {str(k): v for k, v in sorted(MONTH_SEASONS.items())},
        "season_particles": season_drift, "atlases": atlases, "overrides": "overrides.json",
        "tables": {"sin_q12": sin_table(), "envelope_q8": envelope_table()},
        "transforms": ["crop", "1x mirror/reflect", "integer desaturate/tint/gain", "vignette", "drop shadow",
                       "PS1 4x4 ordered dither", "5-bit snap", "alpha/additive compositing"],
        "files": files,
    }
    (stage / "manifest.json").write_text(json.dumps(manifest, sort_keys=True, indent=1) + "\n", encoding="utf-8")
    return manifest


def _check_output(output):
    cache = (ROOT / ".asset-cache")
    if cache.is_symlink():
        raise WingArtError(".asset-cache must not be a symlink")
    output = Path(os.path.abspath(output))
    try:
        rel = output.relative_to(Path(os.path.abspath(cache)))
    except ValueError as exc:
        raise WingArtError("output must be a new directory under .asset-cache/") from exc
    if not rel.parts or any(p in ("", ".", "..") for p in rel.parts):
        raise WingArtError("output must name a directory under .asset-cache/")
    if output.exists() or output.is_symlink():
        raise WingArtError("refusing to overwrite an existing wing-art pack")
    probe = output.parent
    while probe != cache and probe != probe.parent:
        if probe.is_symlink():
            raise WingArtError("output path contains a symlink")
        probe = probe.parent
    return output


def export(input_path, output_path):
    output = _check_output(output_path)
    blobs, identities = read_sources(input_path)
    output.parent.mkdir(parents=True, exist_ok=True)
    stage = Path(tempfile.mkdtemp(prefix=".wing-art-", dir=output.parent))
    try:
        manifest = build_pack(blobs, identities, stage)
        if output.exists():
            raise WingArtError("output appeared while exporting")
        os.replace(stage, output)
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--input", required=True, help="original disc directory (disc/files) or image")
    parser.add_argument("--output", required=True, help="new directory under .asset-cache/")
    args = parser.parse_args()
    try:
        manifest = export(args.input, args.output)
    except (WingArtError, OSError, ValueError) as exc:
        print("wing-art export failed: " + str(exc), file=sys.stderr)
        return 2
    print("wing-art pack: %d styles, %d files" % (len(manifest["styles"]), len(manifest["files"])))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
