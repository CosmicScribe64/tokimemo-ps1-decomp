"""T-39410: texture-keeping erase and brush-style lettering helpers for BS stills.

Everything here works on RGB555 integer pictures (height, width, 3) as the pinned
MDEC decode returns them, and on float coverage planes.

* `known_background` / `inpaint`: rebuild the picture under an erase mask from the
  clean pixels around it. The smooth part (gradient, shading) comes from a
  normalised Gaussian average of the clean pixels; the grain comes from the
  residual (pixel minus smooth part) of clean donor pixels a short step away, so
  stone, paper and sky keep their texture instead of turning into a flat fill.
* `stroke_mask`: the pixels of a lettering colour (dark or light against the local
  background), grown by a margin so the soft MDEC edge is erased too.
* `brush_coverage`: variable-width strokes for handwritten and brush lettering
  (entry blob, thick down-strokes, tapered ends, a little deterministic wobble).
* `place`: map the local (u along the text, v down the cap) strokes of
  tools/translation_bs_font.py into the picture at any angle, with an optional slant.

Nothing is random: the wobble is a fixed function of the stroke index and position.
"""
import math

import numpy as np
from PIL import Image, ImageDraw

SUPERSAMPLE = 8
DONOR_STEPS = [(0, 3), (3, 0), (0, -3), (-3, 0), (3, 3), (-3, -3), (3, -3), (-3, 3), (0, 6), (6, 0), (0, -6), (-6, 0),
               (6, 6), (-6, -6), (6, -6), (-6, 6), (0, 9), (9, 0), (0, -9), (-9, 0), (0, 12), (12, 0), (0, -12), (-12, 0)]


def gaussian_kernel(sigma):
    radius = max(1, int(math.ceil(sigma * 3)))
    xs = np.arange(-radius, radius + 1, dtype=np.float64)
    kernel = np.exp(-0.5 * (xs / sigma) ** 2)
    return kernel / kernel.sum()


def blur(plane, sigma):
    """Separable Gaussian blur of a 2-D or 3-D float array over its first two axes (zero padded)."""
    kernel = gaussian_kernel(sigma)
    radius = (len(kernel) - 1) // 2
    out = np.zeros_like(plane, dtype=np.float64)
    padded = np.pad(plane.astype(np.float64), [(radius, radius)] + [(0, 0)] * (plane.ndim - 1))
    for i, k in enumerate(kernel):
        out += k * padded[i:i + plane.shape[0]]
    padded = np.pad(out, [(0, 0), (radius, radius)] + [(0, 0)] * (plane.ndim - 2))
    out = np.zeros_like(plane, dtype=np.float64)
    for i, k in enumerate(kernel):
        out += k * padded[:, i:i + plane.shape[1]]
    return out


def known_background(picture, unknown, sigma=3.0):
    """Smooth background (float, picture shape) from the pixels where `unknown` is False.

    Normalised convolution: blur(picture * known) / blur(known). Pixels with no known neighbour
    in reach fall back to the mean of the known pixels.
    """
    known = (~unknown).astype(np.float64)
    weight = blur(known, sigma)
    total = blur(picture.astype(np.float64) * known[..., None], sigma)
    mean = picture[~unknown].mean(0) if (~unknown).any() else np.zeros(3)
    safe = np.where(weight > 1e-6, weight, 1.0)[..., None]
    out = np.where((weight > 1e-6)[..., None], total / safe, mean)
    return out


def inpaint(picture, unknown, sigma=3.0, grain=1.0):
    """Float picture with the `unknown` pixels rebuilt; known pixels are returned unchanged.

    Smooth part: known_background. Grain: for each unknown pixel the residual of the first
    clean donor at a fixed step (DONOR_STEPS) scaled by `grain`. The result is a deterministic
    function of the inputs.
    """
    height, width = unknown.shape
    background = known_background(picture, unknown, sigma)
    residual = np.where(unknown[..., None], 0.0, picture - background)
    out = picture.astype(np.float64).copy()
    ys, xs = np.nonzero(unknown)
    for y, x in zip(ys, xs):
        add = np.zeros(3)
        for dx, dy in DONOR_STEPS:
            sx, sy = x + dx, y + dy
            if 0 <= sx < width and 0 <= sy < height and not unknown[sy, sx]:
                add = residual[sy, sx] * grain
                break
        out[y, x] = background[y, x] + add
    return out


def grow(mask, amount):
    """Mask grown by `amount` pixels (square structuring element, Chebyshev distance)."""
    out = mask.copy()
    for _ in range(int(amount)):
        padded = np.pad(out, 1)
        out = (padded[1:-1, 1:-1] | padded[:-2, 1:-1] | padded[2:, 1:-1] | padded[1:-1, :-2] | padded[1:-1, 2:]
               | padded[:-2, :-2] | padded[:-2, 2:] | padded[2:, :-2] | padded[2:, 2:])
    return out


def stroke_mask(picture, region, polarity='any', threshold=8, margin=1, sigma=4.0, rounds=3):
    """Boolean mask (picture shape) of lettering pixels inside `region` = [x, y, w, h].

    The reference is the smooth background of the pixels around and inside the region that are
    not lettering (re-estimated `rounds` times). A pixel is lettering when its channel sum
    differs from the reference by more than `threshold` (`polarity` dark: darker only, light:
    lighter only, any: both), then the mask is grown by `margin`.
    """
    x, y, w, h = region
    height, width = picture.shape[:2]
    box = np.zeros((height, width), bool)
    box[y:y + h, x:x + w] = True
    unknown = np.zeros((height, width), bool)
    total = picture.sum(-1).astype(np.float64)
    for _ in range(rounds):
        background = known_background(picture, unknown, sigma)
        delta = total - background.sum(-1)
        if polarity == 'dark':
            hit = delta < -threshold
        elif polarity == 'light':
            hit = delta > threshold
        else:
            hit = np.abs(delta) > threshold
        unknown = hit & box
    return grow(unknown, margin) & box


def place(lines, origin, angle=0.0, slant=0.0):
    """Map local (u, v) poly-lines into the picture.

    u runs along the text and v down the letters. angle 0: u goes right and v down from origin;
    angle 90: the text runs down the picture with the letter tops facing right (the same as
    tools/translation_bs_font.to_image(.., 90)); any other angle turns the text further.
    `slant` (degrees) shears the letters along u in proportion to their height (v), positive leaning the
    tops forward along the reading direction.
    """
    ox, oy = origin
    a = math.radians(angle)
    ux, uy = math.cos(a), math.sin(a)
    vx, vy = -math.sin(a), math.cos(a)
    t = math.tan(math.radians(slant))
    out = []
    for line in lines:
        out.append([(ox + (u - v * t) * ux + v * vx, oy + (u - v * t) * uy + v * vy) for u, v in line])
    return out


def resample(line, step=0.25):
    """Points along a poly-line at about `step` spacing (with the segment direction at each)."""
    points = []
    for (x0, y0), (x1, y1) in zip(line, line[1:]):
        length = math.hypot(x1 - x0, y1 - y0)
        count = max(1, int(math.ceil(length / step)))
        for i in range(count):
            f = i / count
            points.append((x0 + (x1 - x0) * f, y0 + (y1 - y0) * f, (x1 - x0) / (length or 1), (y1 - y0) / (length or 1)))
    x, y = line[-1]
    last = points[-1] if points else (x, y, 0.0, 1.0)
    points.append((x, y, last[2], last[3]))
    return points


def brush_coverage(shape, lines, weight, offset=(0, 0), angle=0.0, entry=0.35, taper=0.6, contrast=0.45, wobble=0.12, seed=0):
    """Coverage 0..1 (float, shape = (height, width)) of brush-stroked poly-lines.

    weight: mean stroke width in px. entry: extra width at the stroke start (relative), taper:
    how far the width falls towards the stroke end (0 none, 1 to a point), contrast: how much
    more the strokes that run down the letters weigh than the ones that run along the text, wobble:
    relative wavering of the width. `angle` is the text rotation used by `place`, so "down the
    letters" follows the text. Deterministic in `seed` and the stroke order.
    """
    a = math.radians(angle)
    vx, vy = -math.sin(a), math.cos(a)
    height, width = shape
    big = Image.new('L', (width * SUPERSAMPLE, height * SUPERSAMPLE), 0)
    draw = ImageDraw.Draw(big)
    for index, line in enumerate(lines):
        pts = resample(line)
        n = len(pts)
        total = sum(math.hypot(b[0] - a[0], b[1] - a[1]) for a, b in zip(pts, pts[1:])) or 1.0
        run = 0.0
        for i, (x, y, dx, dy) in enumerate(pts):
            if i:
                run += math.hypot(x - pts[i - 1][0], y - pts[i - 1][1])
            f = run / total
            vertical = abs(dx * vx + dy * vy)
            w = weight * (1.0 - contrast / 2 + contrast * vertical)
            w *= 1.0 + entry * max(0.0, 1.0 - f / 0.18) ** 2
            w *= 1.0 - taper * max(0.0, (f - 0.55) / 0.45) ** 1.6
            w *= 1.0 + wobble * math.sin(f * total * 0.9 + index * 1.7 + seed)
            r = max(0.18, w / 2.0) * SUPERSAMPLE
            cx, cy = (x - offset[0]) * SUPERSAMPLE, (y - offset[1]) * SUPERSAMPLE
            draw.ellipse([cx - r, cy - r, cx + r, cy + r], fill=255)
    small = big.reduce(SUPERSAMPLE)
    return np.asarray(small, np.float64) / 255.0
