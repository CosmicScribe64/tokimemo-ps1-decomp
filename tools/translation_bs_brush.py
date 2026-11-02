"""T-39412: texture-keeping erase and brush-style lettering helpers for BS stills.

Everything here works on RGB555 integer pictures (height, width, 3) as the pinned
MDEC decode returns them, and on float coverage planes.

* `known_background` / `inpaint`: rebuild the picture under an erase mask from the
  clean pixels around it. The smooth part (gradient, shading) is a normalised
  Gaussian average of the clean pixels. The grain is synthetic: seeded Gaussian
  noise, blurred so that its pixel-to-pixel correlation along x and y matches the
  residual (pixel minus smooth part) of the clean neighbours, coloured by their
  residual covariance and scaled to their local residual energy (`grain_model`,
  `synthetic_grain`). No source pixel or residual is copied into the fill, so stone,
  paper and sky keep a texture of the same strength and fineness without carrying
  any of the original picture's grain. `shift_correlation` measures that: it is
  near 1 for a fill that copies the grain from a fixed step and near 0 here.
* `stroke_mask`: the pixels of a lettering colour (dark or light against the local
  background), grown by a margin so the soft MDEC edge is erased too.
* `brush_coverage`: variable-width strokes for handwritten and brush lettering
  (entry blob, thick down-strokes or a broad-nib thick / thin, tapered ends, a little
  deterministic wobble).
* `place`: map the local (u along the text, v down the cap) strokes of
  tools/translation_bs_font.py into the picture at any angle, with an optional slant.

Everything is deterministic: the grain comes from a fixed seed per item and the wobble is a
fixed function of the stroke index and position.
"""
import math

import numpy as np
from PIL import Image, ImageDraw

SUPERSAMPLE = 8
GRAIN_BAND = (2, 10)    # clean neighbours that set the grain statistics: known pixels 2 to 10 px from the erase mask
ENERGY_SIGMA = 6.0      # reach (px) of the local residual energy that scales the grain
QUANTIZATION = 1.0 / 12  # variance the final rounding to whole RGB555 steps adds per channel
OUTLIER = 3.0           # robust deviations beyond which a neighbour does not set the grain statistics
GRAIN_SIGMA = 1.5       # px; the grain is what the clean pixels hold below this scale (shading and sheen above it are not grain)


def gaussian_kernel(sigma):
    radius = max(1, int(math.ceil(sigma * 3)))
    xs = np.arange(-radius, radius + 1, dtype=np.float64)
    kernel = np.exp(-0.5 * (xs / sigma) ** 2)
    return kernel / kernel.sum()


def blur_axes(plane, sigma_y, sigma_x):
    """Separable Gaussian blur of a 2-D or 3-D float array, sigma_y down axis 0 and sigma_x along axis 1 (zero padded).
    An axis with a sigma below 0.3 px is left as it is."""
    out = plane.astype(np.float64)
    for axis, sigma in ((0, sigma_y), (1, sigma_x)):
        if sigma < 0.3:
            continue
        kernel = gaussian_kernel(sigma)
        radius = (len(kernel) - 1) // 2
        pad = [(0, 0)] * out.ndim
        pad[axis] = (radius, radius)
        padded = np.pad(out, pad)
        total = np.zeros_like(out)
        for i, k in enumerate(kernel):
            total += k * np.take(padded, range(i, i + out.shape[axis]), axis=axis)
        out = total
    return out


def blur(plane, sigma):
    """Separable Gaussian blur of a 2-D or 3-D float array over its first two axes (zero padded)."""
    return blur_axes(plane, sigma, sigma)


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


def lag_sigma(rho):
    """Blur sigma (px) that gives Gaussian-blurred white noise a lag-1 autocorrelation of rho (exp(-1 / (4 sigma^2)));
    0 (white noise) at rho <= 0.05, capped at rho 0.9."""
    if not rho > 0.05:
        return 0.0
    return 1.0 / (2.0 * math.sqrt(-math.log(min(rho, 0.9))))


def lag_correlation(values, clean, axis):
    """Correlation of `values` with its neighbour one pixel further along `axis`, over pairs of two clean pixels."""
    if axis == 0:
        a, b, pair = values[:-1], values[1:], clean[:-1] & clean[1:]
    else:
        a, b, pair = values[:, :-1], values[:, 1:], clean[:, :-1] & clean[:, 1:]
    a, b = a[pair], b[pair]
    if len(a) < 16 or a.std() == 0 or b.std() == 0:
        return 0.0
    return float(np.corrcoef(a, b)[0, 1])


def grain_model(picture, unknown, barred, within=None):
    """Grain statistics of the clean neighbours of `unknown` (known pixels GRAIN_BAND px from it, not `barred`, inside
    `within` when given: the surface the lettering is on). The residual is the picture less its normalised Gaussian
    average at GRAIN_SIGMA, so a gradient or sheen across the surface does not count as grain.

    Outliers (lettering edges the erase mask missed, a border) are dropped: a neighbour whose channel-sum residual lies
    more than OUTLIER robust deviations (1.4826 x median absolute deviation) from the median does not count. Returns mean
    and covariance of that residual per channel, the blur sigma along x and y that reproduces its
    lag-1 correlation, and the local residual energy (normalised Gaussian average, ENERGY_SIGMA) relative to its mean,
    which scales the grain where the texture is stronger or weaker.
    """
    allowed = ~barred if within is None else ~barred & within
    clean = allowed & grow(unknown, GRAIN_BAND[1]) & ~grow(unknown, GRAIN_BAND[0])
    if clean.sum() < 32:
        clean = allowed
    residual = picture.astype(np.float64) - known_background(picture, barred, GRAIN_SIGMA)
    total = residual.sum(-1)
    if clean.sum() >= 32:
        middle = np.median(total[clean])
        spread = 1.4826 * np.median(np.abs(total[clean] - middle))
        clean = clean & (np.abs(total - middle) <= OUTLIER * spread if spread > 0 else total == middle)
    sample = residual[clean]
    mean = sample.mean(0) if len(sample) else np.zeros(3)
    covariance = np.cov((sample - mean).T) if len(sample) > 1 else np.zeros((3, 3))
    total = residual.sum(-1) - mean.sum()
    energy_pixel = ((residual - mean) ** 2).sum(-1)
    weight = blur(clean.astype(np.float64), ENERGY_SIGMA)
    local = blur(energy_pixel * clean, ENERGY_SIGMA)
    level = float(energy_pixel[clean].mean()) if clean.any() else 0.0
    relative = np.where(weight > 1e-3, local / np.where(weight > 1e-3, weight, 1.0), level) / (level or 1.0)
    return {'mean': mean, 'covariance': covariance, 'clean_pixels': int(clean.sum()),
            'sigma_x': lag_sigma(lag_correlation(total, clean, 1)), 'sigma_y': lag_sigma(lag_correlation(total, clean, 0)),
            'relative_energy': relative}


def synthetic_grain(shape, model, seed):
    """Grain field (float, shape + (3,)) with the statistics of `model` from noise seeded with `seed`.

    White Gaussian noise (numpy's legacy RandomState stream, which numpy keeps stable across versions) is blurred by the
    model's sigma along x and y, normalised to unit variance, coloured by the Cholesky factor of the residual covariance
    (less the variance the final rounding adds), offset by the residual mean and scaled by the local energy.
    """
    height, width = shape
    pad = int(math.ceil(3 * max(model['sigma_x'], model['sigma_y']))) + 1
    noise = np.random.RandomState(seed).standard_normal((height + 2 * pad, width + 2 * pad, 3))
    field = blur_axes(noise, model['sigma_y'], model['sigma_x'])[pad:pad + height, pad:pad + width]
    field = (field - field.mean((0, 1))) / field.std((0, 1))
    covariance = model['covariance']
    spread = float(np.trace(covariance))
    keep = max(0.0, (spread - 3 * QUANTIZATION) / spread) if spread > 0 else 0.0
    factor = np.linalg.cholesky(covariance * keep + np.eye(3) * 1e-9)
    scale = np.sqrt(np.clip(model['relative_energy'], 0.25, 2.0))
    return (field @ factor.T) * scale[..., None] + model['mean']


def inpaint(picture, unknown, sigma=3.0, grain=1.0, exclude=None, seed=0, within=None):
    """(float picture with the `unknown` pixels rebuilt, grain facts); known pixels are returned unchanged.

    Each unknown pixel = known_background (smooth part) + `grain` x synthetic_grain (from `seed`, with the statistics
    of the clean neighbours, grain_model). `exclude` (bool mask) marks known pixels that must not feed the fill (another
    surface next to the erased one, such as the chalk ledge under a blackboard). `within` (bool mask) limits the pixels
    that set the grain statistics (grain_model). Deterministic in the inputs.
    """
    barred = unknown if exclude is None else unknown | exclude
    background = known_background(picture, barred, sigma)
    model = grain_model(picture, unknown, barred, within)
    field = synthetic_grain(unknown.shape, model, seed) * grain
    out = picture.astype(np.float64).copy()
    out[unknown] = background[unknown] + field[unknown]
    facts = {'seed': seed, 'grain': grain, 'clean_pixels': model['clean_pixels'],
             'blur_sigma_px': [round(model['sigma_x'], 3), round(model['sigma_y'], 3)],
             'residual_std_rgb555': [round(float(v), 3) for v in np.sqrt(np.diag(model['covariance']))]}
    return out, facts


def shift_correlation(filled, picture, unknown, barred, background, reach=12, minimum=100):
    """Largest |correlation| between the fill residual (filled - background, channel sum) on the `unknown` pixels and the
    source residual (picture - background) at any shift of up to `reach` px onto a clean (not `barred`) pixel.

    A fill that copies the residual of a donor at a fixed step correlates near 1 at that step; independent grain stays
    near 0 (chance level about 2 / sqrt(pairs) for these spatially correlated fields). Shifts with fewer than `minimum`
    pixel pairs are skipped. Returns (value rounded to 3 places, [dx, dy]).
    """
    fill = (filled - background).sum(-1)
    source = (picture.astype(np.float64) - background).sum(-1)
    height, width = unknown.shape
    ys, xs = np.nonzero(unknown)
    best, where = 0.0, [0, 0]
    for dy in range(-reach, reach + 1):
        for dx in range(-reach, reach + 1):
            if dx == 0 and dy == 0:
                continue
            sy, sx = ys + dy, xs + dx
            ok = (sy >= 0) & (sy < height) & (sx >= 0) & (sx < width)
            ok[ok] &= ~barred[sy[ok], sx[ok]]
            if ok.sum() < minimum:
                continue
            a, b = fill[ys[ok], xs[ok]], source[sy[ok], sx[ok]]
            if a.std() == 0 or b.std() == 0:
                continue
            value = abs(float(np.corrcoef(a, b)[0, 1]))
            if value > best:
                best, where = value, [dx, dy]
    return round(best, 3), where


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


def brush_coverage(shape, lines, weight, offset=(0, 0), angle=0.0, entry=0.35, taper=0.6, contrast=0.45, wobble=0.12, seed=0, nib=None):
    """Coverage 0..1 (float, shape = (height, width)) of brush-stroked poly-lines.

    weight: mean stroke width in px. entry: extra width at the stroke start (relative), taper:
    how far the width falls towards the stroke end (0 none, 1 to a point), contrast: how much
    more the thick strokes weigh than the thin ones, wobble: relative wavering of the width.
    `angle` is the text rotation used by `place`, so the letter frame follows the text.
    nib None: the strokes that run down the letters are the thick ones. nib = degrees: a broad
    nib whose edge rises at that angle from the text direction (pens are held at 20-45): a stroke
    along the edge is thinnest and one across it thickest, so in an A the left leg is thin and
    the right leg thick. Deterministic in `seed` and the stroke order.
    """
    a = math.radians(angle)
    vx, vy = -math.sin(a), math.cos(a)
    if nib is not None:
        # the nib edge in picture space: along the text turned towards the letter tops (-v)
        edge = math.radians(nib)
        vx, vy = math.cos(edge) * math.cos(a) - math.sin(edge) * vx, math.cos(edge) * math.sin(a) - math.sin(edge) * vy
        vx, vy = -vy, vx  # thickest across the edge: weigh the component normal to it
    height, width = shape
    big = Image.new('L', (width * SUPERSAMPLE, height * SUPERSAMPLE), 0)
    draw = ImageDraw.Draw(big)
    for index, line in enumerate(lines):
        pts = resample(line)
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
