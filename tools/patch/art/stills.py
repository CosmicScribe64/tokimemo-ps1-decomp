"""English lettering into the BS (MDEC) background stills (T-39508, wiki/english-build.md).

Each registered still image is a crop of one still: an RGB rectangle that replaces every pixel
of its rectangle (tools/translation_bs_signs.py), or an RGBA crop whose alpha-255 pixels replace
the decoded still and whose other pixels keep it (tools/translation_bs_stills.py). For each still:

1. decode the original with the pinned fixed-point model (tools/assets/mdec_splice.py), at the
   picture size its manifest proves from the game code;
2. composite every crop of that still; the goal differs from the source only at English pixels;
3. re-encode only the 16x16 macroblocks that hold a changed pixel (mdec_splice.encode_macroblock,
   changed pixels weighted 1, the rest of those macroblocks `ring_weight`), every other
   macroblock keeps its bits;
4. gates, as tools/translation_bs_drafts.py: the stream parses, pixels outside the edited
   macroblocks are bit-identical, unchanged pixels inside them stay within RING_TOLERANCE RGB555
   steps, a float IDCT decode stays within FLOAT_TOLERANCE of the fixed-point one. The first
   (lambda, ring weight) that passes is used; the file's sector count is checked by the build
   (tools/patch/cd_layout.py) like any other grown file.
"""
from __future__ import annotations

from dataclasses import dataclass, field

import numpy as np
from PIL import Image as PILImage

from tools.assets import mdec_splice

RING_TOLERANCE = 1          # tools/translation_bs_drafts.py
FLOAT_TOLERANCE = 2
ATTEMPTS = tuple((lam, ring) for lam in (2.0, 8.0, 24.0) for ring in (50.0, 200.0, 1000.0))


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
                weights = np.where(changed[my:my + 16, mx:mx + 16], 1.0, ring)
                index = mx // 16 * rows + my // 16
                replacements[index] = mdec_splice.encode_macroblock(parsed["macroblocks"][index]["levels"],
                                                                    goal[my:my + 16, mx:mx + 16], weights, quant,
                                                                    lam, basis=basis)
    result, facts = mdec_splice.rebuild(data, replacements)
    decoded = mdec_splice.decode_rgb5(result, width, height)
    edited = np.zeros(changed.shape, bool)
    float_error = 0
    for index, levels in replacements.items():
        mx, my = index // rows * 16, index % rows * 16
        edited[my:my + 16, mx:mx + 16] = True
        second = mdec_splice.float_macroblock_rgb5(levels, quant)
        float_error = max(float_error, int(np.abs(second - decoded[my:my + 16, mx:mx + 16]).max()))
    ring_mask = edited & ~changed
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


def failures(facts: dict) -> list[str]:
    out = []
    if not facts["outside_identical"]:
        out.append("pixels changed outside the edited macroblocks")
    if facts["ring_max_abs_rgb555"] > RING_TOLERANCE:
        out.append("unchanged pixels of edited macroblocks moved by %d" % facts["ring_max_abs_rgb555"])
    if facts["float_idct_max_abs_rgb555"] > FLOAT_TOLERANCE:
        out.append("float IDCT cross-check differs by %d" % facts["float_idct_max_abs_rgb555"])
    return out


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
    tried = []
    for lam, ring in ATTEMPTS:
        out, facts = splice(data, source, goal, width, height, lam, ring)
        problems = failures(facts)
        tried.append({"lambda": lam, "ring_weight": ring, "failures": problems})
        if not problems:
            result.data = out
            facts["attempts"] = tried
            result.facts = facts
            for image_id, _p, _m in images:
                result.images[image_id] = "re_encoded"
            return result
    for image_id, _p, _m in images:
        result.images[image_id] = "deferred"
        result.reasons[image_id] = "re-encode gates fail: %s" % "; ".join(tried[-1]["failures"])
    result.facts = {"attempts": tried}
    return result
