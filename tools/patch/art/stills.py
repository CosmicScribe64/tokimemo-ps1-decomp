"""English lettering into the BS (MDEC) background stills (T-39508, wiki/english-build.md).

Each registered still image is a crop of one still: an RGB rectangle that replaces every pixel
of its rectangle (tools/translation_bs_signs.py), or an RGBA crop whose alpha-255 pixels replace
the decoded still and whose other pixels keep it (tools/translation_bs_stills.py). For each still:

1. decode the original with the pinned fixed-point model (tools/assets/mdec_splice.py), at the
   picture size its manifest proves from the game code;
2. composite every crop of that still; the goal differs from the source only at English pixels;
3. re-encode only the 16x16 macroblocks that hold a changed pixel (mdec_splice.encode_macroblock,
   changed pixels weighted 1, the rest of those macroblocks `ring_weight`; a pixel that still
   moves past the tolerance gets more weight, and where the fixed-point IDCT overflows the
   English is softened; up to four rounds), every other macroblock keeps its bits;
4. gates, as tools/translation_bs_drafts.py: the stream parses, pixels outside the edited
   macroblocks are bit-identical, unchanged pixels inside them (outside the 2x2 chroma cells of
   the English pixels, which share their Cb/Cr) stay within RING_TOLERANCE RGB555 steps, a float
   IDCT decode stays within FLOAT_TOLERANCE of the fixed-point one. The first (lambda, ring
   weight) that passes is used. When none does, the best attempt within CODEC_RING_TOLERANCE and
   CODEC_FLOAT_TOLERANCE is used and `ring_tolerance` in the facts says so; otherwise the still
   is deferred. A still may not gain a sector: the game reads every still with a fixed 10-sector
   read into a buffer the MDEC run-length buffer follows, and the CD hook would lengthen it.
5. When no attempt passes even the codec tier, the whole still is re-encoded at a finer quantizer
   that divides its own (`requantized`): the untouched macroblocks decode exactly as before, the
   English ones get the finer step. It must still fit the file's sectors (point 4); on the stills
   tried so far (P_01_02, D_03_08) it does not.
"""
from __future__ import annotations

import math
import struct
from dataclasses import dataclass, field

import numpy as np
from PIL import Image as PILImage

from tools.assets import mdec_splice

RING_TOLERANCE = 1          # tools/translation_bs_drafts.py
FLOAT_TOLERANCE = 2
# When no attempt keeps the unchanged pixels within one step, the best attempt within these is
# taken and the report says so: the frame's quantizer cannot hold them closer.
CODEC_RING_TOLERANCE = 4
CODEC_FLOAT_TOLERANCE = 4
ATTEMPTS = tuple((lam, ring) for lam in (2.0, 8.0, 32.0) for ring in (50.0, 1000.0))


class StillError(ValueError):
    """A crop does not fit its still."""


@dataclass
class StillResult:
    path: str
    data: bytes
    images: dict = field(default_factory=dict)     # image id -> "re_encoded" / "deferred"
    reasons: dict = field(default_factory=dict)
    facts: dict = field(default_factory=dict)


def crop_pixels(png_path, rect) -> tuple[np.ndarray, np.ndarray]:
    """(RGB555 values h x w x 3, written mask h x w) of one crop."""
    x, y, w, h = rect
    with PILImage.open(png_path) as image:
        if image.size != (w, h):
            raise StillError("%s is %dx%d, its rectangle %dx%d" % (png_path, image.width, image.height, w, h))
        array = np.asarray(image.convert("RGBA"))
        rgba = image.mode == "RGBA"
    written = array[..., 3] == 255 if rgba else np.ones((h, w), bool)
    if rgba and not ((array[..., 3] == 0) | written).all():
        raise StillError("%s has partial alpha" % png_path)
    return array[..., :3].astype(int) >> 3, written


def goal_image(source: np.ndarray, crops) -> np.ndarray:
    goal = source.copy()
    for rect, values, written in crops:
        x, y, w, h = rect
        if x + w > source.shape[1] or y + h > source.shape[0]:
            raise StillError("rectangle %s is outside the %dx%d still" % (rect, source.shape[1], source.shape[0]))
        block = goal[y:y + h, x:x + w]
        block[written] = values[written]
    return goal


def splice(data: bytes, source: np.ndarray, goal: np.ndarray, width: int, height: int, lam: float, ring: float):
    """Re-encode the macroblocks with a changed pixel; returns (bytes, facts)."""
    padded = source.shape[0]
    parsed = mdec_splice.parse(data)
    quant = parsed["quantization"]
    changed = (goal != source).any(-1)
    basis = mdec_splice._linear_basis(quant)
    replacements = {}
    rows = padded // 16
    for mx in range(0, source.shape[1], 16):
        for my in range(0, padded, 16):
            if changed[my:my + 16, mx:mx + 16].any():
                index = mx // 16 * rows + my // 16
                replacements[index] = _encode(parsed["macroblocks"][index]["levels"], source[my:my + 16, mx:mx + 16],
                                              goal[my:my + 16, mx:mx + 16], changed[my:my + 16, mx:mx + 16],
                                              quant, lam, ring, basis)
    result, facts = mdec_splice.rebuild(data, replacements)
    decoded = mdec_splice.decode_rgb5(result, width, height)
    edited = np.zeros(changed.shape, bool)
    float_error = 0
    for index, levels in replacements.items():
        mx, my = index // rows * 16, index % rows * 16
        edited[my:my + 16, mx:mx + 16] = True
        second = mdec_splice.float_macroblock_rgb5(levels, quant)
        float_error = max(float_error, int(np.abs(second - decoded[my:my + 16, mx:mx + 16]).max()))
    ring_mask = edited & ~_chroma_cells(changed)
    english_error = np.abs(decoded - goal)[changed]
    facts.update({
        "lambda": lam, "ring_weight": ring, "edited_macroblocks": len(replacements),
        "changed_pixels": int(changed.sum()),
        "outside_identical": bool((decoded[~edited] == source[~edited]).all()),
        "ring_max_abs_rgb555": int(np.abs(decoded - source)[ring_mask].max()) if ring_mask.any() else 0,
        "english_max_abs_rgb555": int(english_error.max()) if changed.any() else 0,
        "english_mean_abs_rgb555": round(float(english_error.mean()), 4) if changed.any() else 0.0,
        "float_idct_max_abs_rgb555": float_error})
    return result, facts


def _encode(levels, source, goal, changed, quant, lam, ring, basis, rounds=4):
    """Re-encode one macroblock, fixing what the decode gets wrong, up to `rounds` times:
    unchanged pixels that move more than RING_TOLERANCE get a hundred times their weight; when the
    fixed-point decode drifts from the float one (the IDCT overflows on 1-texel black and white
    strokes), the English pixels of the goal are softened (3x3 box over the changed pixels) and
    lambda grows, which keeps the coefficients in range."""
    free = _chroma_cells(changed)
    weights = np.where(changed, 1.0, ring)
    best = None
    target = goal
    for _round in range(rounds):
        out = mdec_splice.encode_macroblock(levels, target, weights, quant, lam, basis=basis)
        decoded = mdec_splice.macroblock_rgb5([mdec_splice.block_output(block, quant) for block in out])
        drift = np.abs(mdec_splice.float_macroblock_rgb5(out, quant) - decoded).max(-1) > FLOAT_TOLERANCE
        moved = ~free & (np.abs(decoded - source).max(-1) > RING_TOLERANCE)
        score = (int(drift.sum()), int(moved.sum()))
        if best is None or score < best[0]:
            best = (score, out)
        if not any(score):
            break
        if drift.any():
            target = np.where(changed[..., None], _soften(target), source)
            lam *= 4
        weights = np.where(moved, weights * 100.0, weights)
    return best[1]


def _soften(image: np.ndarray) -> np.ndarray:
    """3x3 box average of a 16x16x3 block (edges repeated), rounded."""
    padded = np.pad(image.astype(float), ((1, 1), (1, 1), (0, 0)), mode="edge")
    total = sum(padded[dy:dy + 16, dx:dx + 16] for dy in range(3) for dx in range(3))
    return np.rint(total / 9.0).astype(image.dtype)


def _chroma_cells(mask: np.ndarray) -> np.ndarray:
    """Pixels sharing a 2x2 chroma sample with a masked pixel: MDEC codes Cb and Cr once per 2x2,
    so an English colour reaches its chroma neighbours; they belong to the edit, not the ring."""
    h, w = mask.shape
    cells = mask[:h // 2 * 2, :w // 2 * 2].reshape(h // 2, 2, w // 2, 2).any((1, 3))
    out = mask.copy()
    out[:h // 2 * 2, :w // 2 * 2] |= np.repeat(np.repeat(cells, 2, 0), 2, 1)
    return out


def failures(facts: dict, ring_tolerance: int = RING_TOLERANCE, float_tolerance: int = FLOAT_TOLERANCE) -> list[str]:
    out = []
    if not facts["outside_identical"]:
        out.append("pixels changed outside the edited macroblocks")
    if facts["ring_max_abs_rgb555"] > ring_tolerance:
        out.append("unchanged pixels of edited macroblocks moved by %d" % facts["ring_max_abs_rgb555"])
    sizes = facts.get("file_bytes")
    if sizes and -(-sizes["new"] // 2048) > -(-sizes["original"] // 2048):
        # every still is read with a fixed 10-sector read into the still buffer, which the
        # MDEC run-length buffer follows (0x80162000-0x80167000, wiki/ram-map.md); the CD hook
        # adds the sectors a file gained to that read, so a still may not gain a sector
        out.append("grows from %d to %d bytes, past its sectors" % (sizes["original"], sizes["new"]))
    if facts["float_idct_max_abs_rgb555"] > float_tolerance:
        out.append("float IDCT cross-check differs by %d" % facts["float_idct_max_abs_rgb555"])
    return out


def requantized(data: bytes, source: np.ndarray, goal: np.ndarray, width: int, height: int, quant: int,
                lam: float = 2.0, ring: float = 1000.0):
    """The still re-encoded at a finer quantizer that divides the file's own: every untouched
    macroblock keeps its decode exactly (each AC level times the ratio dequantizes to the same
    value), the macroblocks with English are encoded at the finer step. Returns (bytes, facts),
    or (None, why) when a level leaves the coder's range. The trailing bytes after the stream keep
    their place at the end of the file, which grows by whole sectors when it must."""
    parsed = mdec_splice.parse(data)
    original = parsed["quantization"]
    ratio = original // quant
    basis = mdec_splice._linear_basis(quant)
    changed = (goal != source).any(-1)
    rows = source.shape[0] // 16
    bits, halfwords, new_levels = [], 0, {}
    for index, macroblock in enumerate(parsed["macroblocks"]):
        levels = [[level if i == 0 else level * ratio for i, level in enumerate(block)]
                  for block in macroblock["levels"]]
        if any(abs(level) > 511 for block in levels for level in block[1:]):
            return None, "a level leaves the coder's range at quantizer %d" % quant
        mx, my = index // rows * 16, index % rows * 16
        if changed[my:my + 16, mx:mx + 16].any():
            levels = _encode(levels, source[my:my + 16, mx:mx + 16], goal[my:my + 16, mx:mx + 16],
                             changed[my:my + 16, mx:mx + 16], quant, lam, ring, basis)
            new_levels[index] = levels
        encoded = [mdec_splice.encode_block(block) for block in levels]
        bits.append("".join(part for part, _ in encoded))
        halfwords += sum(count for _, count in encoded)
    declared = (halfwords * 2 + 127) // 128 * 128 // 4
    if declared > 0xFFFF:
        return None, "the declared MDEC extent overflows at quantizer %d" % quant
    body = struct.pack("<4H", declared, parsed["magic"], quant, parsed["version"])
    body += mdec_splice._pack("".join(bits) + mdec_splice.END_MARKER)
    trailing = parsed["trailing"]
    tail = trailing[next((i for i, value in enumerate(trailing) if value), len(trailing)):]
    size = len(data) if len(body) + len(tail) <= len(data) else -(-(len(body) + len(tail)) // 2048) * 2048
    result = body + bytes(size - len(body) - len(tail)) + tail
    decoded = mdec_splice.decode_rgb5(result, width, height)
    edited = np.zeros(changed.shape, bool)
    float_error = 0
    for index, levels in new_levels.items():
        mx, my = index // rows * 16, index % rows * 16
        edited[my:my + 16, mx:mx + 16] = True
        second = mdec_splice.float_macroblock_rgb5(levels, quant)
        float_error = max(float_error, int(np.abs(second - decoded[my:my + 16, mx:mx + 16]).max()))
    ring_mask = edited & ~_chroma_cells(changed)
    english_error = np.abs(decoded - goal)[changed]
    untouched = ~changed
    mse = float(((decoded - source)[untouched] ** 2).mean())
    return result, {
        "quantization": {"original": original, "new": quant}, "edited_macroblocks": len(new_levels),
        "changed_pixels": int(changed.sum()),
        "outside_identical": bool((decoded[~edited] == source[~edited]).all()),
        "ring_max_abs_rgb555": int(np.abs(decoded - source)[ring_mask].max()) if ring_mask.any() else 0,
        "english_max_abs_rgb555": int(english_error.max()) if changed.any() else 0,
        "english_mean_abs_rgb555": round(float(english_error.mean()), 4) if changed.any() else 0.0,
        "untouched_psnr_db": None if mse == 0 else round(10 * math.log10(31 ** 2 / mse), 2),
        "float_idct_max_abs_rgb555": float_error,
        "file_bytes": {"original": len(data), "new": len(result)}}


def _done(result: StillResult, images: list, out: bytes, facts: dict, tried: list) -> StillResult:
    result.data = out
    facts["attempts"] = tried
    result.facts = facts
    for image_id, _p, _m in images:
        result.images[image_id] = "re_encoded"
    return result


def place_still(path: str, data: bytes, images: list) -> StillResult:
    """images: (image id, png path, manifest entry); every crop of one still."""
    result = StillResult(path, data)
    sizes = {(m["geometry"]["width"], m["geometry"]["height"]) for _i, _p, m in images}
    if len(sizes) != 1:
        raise StillError("%s: crops disagree on the picture size %s" % (path, sorted(sizes)))
    width, height = sizes.pop()
    source = mdec_splice.decode_rgb5(data, width, height)
    crops = [(m["crop"]["rect"],) + crop_pixels(png, m["crop"]["rect"]) for _i, png, m in images]
    goal = goal_image(source, crops)
    tried, candidates = [], []
    for lam, ring in ATTEMPTS:
        out, facts = splice(data, source, goal, width, height, lam, ring)
        problems = failures(facts)
        tried.append({"lambda": lam, "ring_weight": ring, "failures": problems})
        if not problems:
            facts["ring_tolerance"] = RING_TOLERANCE
            return _done(result, images, out, facts, tried)
        if not failures(facts, CODEC_RING_TOLERANCE, CODEC_FLOAT_TOLERANCE):
            candidates.append((facts["ring_max_abs_rgb555"], facts["english_mean_abs_rgb555"], len(tried), out, facts))
    if candidates:
        _ring, _mean, _n, out, facts = min(candidates, key=lambda c: c[:3])
        facts["ring_tolerance"] = CODEC_RING_TOLERANCE
        return _done(result, images, out, facts, tried)
    # the file's quantizer is too coarse for the strokes: re-encode at a finer one that divides it
    quant = mdec_splice.parse(data)["quantization"]
    for finer in sorted((q for q in range(1, quant) if quant % q == 0), reverse=True):
        out, facts = requantized(data, source, goal, width, height, finer)
        problems = failures(facts) if out is not None else [facts]
        tried.append({"quantization": finer, "failures": problems})
        if not problems:
            facts["ring_tolerance"] = RING_TOLERANCE
            return _done(result, images, out, facts, tried)
    for image_id, _p, _m in images:
        result.images[image_id] = "deferred"
        result.reasons[image_id] = "re-encode gates fail: %s" % "; ".join(tried[-1]["failures"])
    result.facts = {"attempts": tried}
    return result
