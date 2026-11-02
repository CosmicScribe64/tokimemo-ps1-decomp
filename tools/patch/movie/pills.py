"""English cast pills composited into decoded OPENXA frames (T-39509).

The pills, their placement and the caption windows come from the T-39004 checkpoint
(`cue_table.json` of movie-pills-v1, made by tools/translation_movie_pills.py on ffmpeg-decoded
frames). Frame indices there are zero-based decoded frames; STR frame numbers start at 1, so
decoded frame i is STR frame i + 1.

The movie dissolves each cue (all its Japanese pills) in and out over a few frames, over a
background that moves and sometimes cuts. A frame is modelled as O = (1 - a) B + a J, with J the
Japanese caption (median of the cue's full frames), B the unknown background and a the cue's
opacity in that frame. The English frame is T = (1 - a A) B + a A E, with E and A the English
layer and its alpha. On the Japanese caption texels that opaque English texels cover (mask M; the
report counts the caption texels outside it) this is T = a E + clip(O - a J), so B is never
needed; elsewhere B = O.

a is measured per frame by a least-squares fit of the high-pass luma of the frame, inside the
caption masks, to the high-pass luma of J of every cue near that frame (two cues overlap where
one dissolves out as the next dissolves in) and of the nearest caption-free frame (it takes a
static background; after a cut it fits to about zero). Frames inside a cue's full window (two
frames in from each end) take a = 1. A single-cue fit over shifts of up to SEARCH texels gives
the caption's position in each frame; the English layer, J and the mask move with it.
"""
import hashlib
import json
from pathlib import Path

import numpy as np
from PIL import Image

CHECKPOINT = Path('translations/en/graphics/artwork/checkpoints/T39004-movie-pills-v1/movie-pills-v1')
EDGE = 2  # frames at each end of a caption window that may be fading (T-39004)
MARGIN = 4  # frames looked at outside each caption window
MIN_ALPHA = 0.03  # below this a frame is left as it is (fit noise on caption-free frames is about 0.03)
FULL_ALPHA = 0.97
SEARCH = 2  # texels searched each way for the caption position
SHIFT_ALPHA = 0.3  # position is measured only on frames at least this opaque
REFERENCE_REACH = 8  # how far a caption-free background frame is looked for
LUMA = np.array([0.299, 0.587, 0.114])


class PillError(ValueError):
    """The checkpoint or a measurement does not support a safe composite."""


def caption_ink(frame):
    """Caption-coloured texels: saturated magenta or pale pink (tools/translation_movie_pills.py)."""
    r, g, b = frame[..., 0], frame[..., 1], frame[..., 2]
    return (r - g > 60) & (b - g > 40)


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


def caption_mask(full):
    """Japanese caption texels over the full frames (stack N x H x W x 3, int): pill-coloured in
    at least 60% of them, or white/dark-purple lettering in 60% within 4 texels of the pill
    (persistent_caption of tools/translation_movie_pills.py, on the whole frame)."""
    pill = caption_ink(full).mean(axis=0) >= 0.6
    r, g, b = full[..., 0], full[..., 1], full[..., 2]
    text = np.minimum(np.minimum(r, g), b) > 200
    shadow = (r + g + b < 260) & (b > g + 15) & (r > g + 10)
    lettering = (text | shadow).mean(axis=0) >= 0.6
    return pill | (lettering & dilate(pill, 4))


def high_pass(luma):
    """Luma minus its 5x5 box mean (edges replicated)."""
    padded = np.pad(luma, 2, mode='edge')
    box = sum(padded[dy:dy + luma.shape[0], dx:dx + luma.shape[1]] for dy in range(5) for dx in range(5)) / 25.0
    return luma - box


def shifted(array, dx, dy, fill=0):
    """array moved by (dx, dy) texels, uncovered texels set to fill."""
    out = np.full_like(array, fill)
    h, w = array.shape[:2]
    out[max(dy, 0):h + min(dy, 0), max(dx, 0):w + min(dx, 0)] = array[max(-dy, 0):h - max(dy, 0), max(-dx, 0):w - max(dx, 0)]
    return out


def load_cues(root):
    """The cue table and every pill's RGBA art, hashes checked."""
    folder = Path(root) / CHECKPOINT
    table = json.loads((folder / 'cue_table.json').read_text())
    for cue in table['cues']:
        for pill in cue['pills']:
            data = (folder / pill['png']).read_bytes()
            if hashlib.sha256(data).hexdigest() != pill['sha256']:
                raise PillError('pill art changed: ' + pill['id'])
            art = np.asarray(Image.open(folder / pill['png']).convert('RGBA'))
            if art.shape[:2] != (pill['height'], pill['width']):
                raise PillError('pill art size differs: ' + pill['id'])
            pill['art'] = art
    return table


def english_layer(cue, size):
    """(rgb H x W x 3 float, alpha H x W in 0..1) of the cue's pills drawn in order."""
    width, height = size
    rgb = np.zeros((height, width, 3))
    alpha = np.zeros((height, width))
    for pill in cue['pills']:
        art = pill['art'].astype(float)
        x, y = pill['x'], pill['y']
        h, w = art.shape[:2]
        if x < 0 or y < 0 or x + w > width or y + h > height:
            raise PillError('pill outside the frame: ' + pill['id'])
        a = art[..., 3] / 255.0
        region = (slice(y, y + h), slice(x, x + w))
        rgb[region] = rgb[region] * (1 - a[..., None]) + art[..., :3] * a[..., None]
        alpha[region] = alpha[region] * (1 - a) + a
    return rgb, alpha


class Cue:
    """One caption cue: its Japanese caption (median of the full frames), mask and English layer.

    full: the cue's full frames (N x H x W x 3, int)."""

    def __init__(self, cue, full, size):
        self.id = cue['id']
        self.window = tuple(cue['caption_window'])
        self.japanese = np.median(full, axis=0)
        self.english, self.english_alpha = english_layer(cue, size)
        covers = np.zeros(self.english_alpha.shape, bool)
        for pill in cue['pills']:
            x0, y0, x1, y1 = pill['cover_rect']  # encloses the Japanese pill, lettering and shadow
            covers[y0:y1, x0:x1] = True
        mask = caption_mask(full) & covers
        self.uncovered = int((mask & (self.english_alpha < 1)).sum())
        self.mask = mask & (self.english_alpha >= 1)
        self.fit_mask = dilate(self.mask, 2)
        self.japanese_hp = high_pass(self.japanese @ LUMA)
        self.alpha = {}  # decoded index -> opacity used (frames left out are not changed)
        self.offset = {}  # decoded index -> (dx, dy) of the caption
        self.fit = {}  # decoded index -> fitted opacity before the full-window rule

    @property
    def span(self):
        """Frames this cue may show in: the window and MARGIN frames each side."""
        return range(self.window[0] - MARGIN, self.window[1] + MARGIN + 1)

    def full(self, index):
        return self.window[0] + EDGE <= index <= self.window[1] - EDGE

    def area(self, index):
        """Texels the composite may change in decoded frame `index` (English alpha > 0)."""
        dx, dy = self.offset[index]
        return shifted(self.english_alpha, dx, dy) > 0

    def composite(self, index, frame):
        """English frame (H x W x 3 float) for decoded frame `index` from `frame`."""
        a = self.alpha[index]
        dx, dy = self.offset[index]
        english = shifted(self.english, dx, dy)
        alpha = shifted(self.english_alpha, dx, dy) * a
        mask = shifted(self.mask, dx, dy, False)
        japanese = shifted(self.japanese, dx, dy)
        original = np.asarray(frame, float)
        # Under the Japanese caption: (1 - a) B = O - a J, kept inside its range.
        background_part = np.clip(original - a * japanese, 0, 255 * (1 - a))
        out = original * (1 - alpha[..., None]) + english * alpha[..., None]
        out[mask] = (a * english + background_part)[mask]
        return np.clip(out, 0, 255)


def _lstsq(columns, target):
    coef = np.linalg.lstsq(np.stack(columns, 1), target, rcond=None)[0]
    return coef, float(((np.stack(columns, 1) @ coef - target) ** 2).sum())


class Captions:
    """Every cue of the movie, measured on decoded frames.

    frames: {decoded index: H x W x 3 uint8} for every index in `needed(table)`."""

    def __init__(self, table, frames, size):
        self.cues = []
        for cue in table['cues']:
            first, last = cue['caption_window']
            full = np.stack([frames[i] for i in range(first + EDGE, last - EDGE + 1)]).astype(int)
            self.cues.append(Cue(cue, full, size))
        hp = {i: high_pass(frames[i].astype(float) @ LUMA) for i in frames}
        for cue in self.cues:
            self._place(cue, hp)
        for index in sorted({i for cue in self.cues for i in cue.span}):
            self._opacity(index, hp)

    @staticmethod
    def needed(table):
        """Decoded frames the measurement reads."""
        out = set()
        for cue in table['cues']:
            first, last = cue['caption_window']
            out.update(range(first - MARGIN - REFERENCE_REACH, last + MARGIN + REFERENCE_REACH + 1))
        return sorted(i for i in out if i >= 0)

    def _busy(self, index):
        return any(c.window[0] - EDGE - 1 <= index <= c.window[1] + EDGE + 1 for c in self.cues)

    def _reference(self, index, hp):
        """Nearest caption-free frame within REFERENCE_REACH, or None."""
        for step in range(1, REFERENCE_REACH + 1):
            for candidate in (index - step, index + step):
                if candidate in hp and not self._busy(candidate):
                    return candidate
        return None

    def _place(self, cue, hp):
        """Caption position per frame: the shift with the best single-cue fit, on frames where the
        caption is opaque enough to place; (0, 0) elsewhere."""
        for index in cue.span:
            best = None
            for dy in range(-SEARCH, SEARCH + 1):
                for dx in range(-SEARCH, SEARCH + 1):
                    mask = shifted(cue.fit_mask, dx, dy, False)
                    coef, error = _lstsq([shifted(cue.japanese_hp, dx, dy)[mask]], hp[index][mask])
                    if best is None or error < best[0]:
                        best = (error, float(coef[0]), dx, dy)
            cue.offset[index] = (best[2], best[3]) if best[1] >= SHIFT_ALPHA else (0, 0)

    def _opacity(self, index, hp):
        """Joint fit of every cue near `index` (and a caption-free frame for the background)."""
        near = [c for c in self.cues if index in c.span]
        masks = [shifted(c.fit_mask, *c.offset[index], False) for c in near]
        union = np.logical_or.reduce(masks)
        columns = [shifted(c.japanese_hp, *c.offset[index])[union] for c in near]
        reference = self._reference(index, hp)
        if reference is not None:
            columns.append(hp[reference][union])
        coef, _ = _lstsq(columns, hp[index][union])
        for cue, a in zip(near, coef):
            a = float(a)
            cue.fit[index] = round(a, 3)
            if cue.full(index) or a >= FULL_ALPHA:
                a = 1.0
            if a >= MIN_ALPHA:
                cue.alpha[index] = a
