"""T-39516: missing school-name plates beside both festival standing banners.

The existing background callers prove 320x160 geometry (translation_bs_signs).
Generate only the plate face in the ignored cache; preserve its stone border.
The standard still encoder combines this crop with the existing banner crop.
"""
import hashlib
from pathlib import Path

import numpy as np
from PIL import Image

from tools.assets import mdec_splice
from tools.patch.art import ArtBuildError
from tools.patch.art.items import Item, Image as ArtImage

SOURCES = {
    "D_01_47": "190e627d195bf36dbbdd8c4698a67c06e771287c1af9527c039080e542cf6179",
    "D_01_48": "edea5fabcbf03ad14f10b79a89cc44e3dc9903ea82f4fbf49b069f189c4bfd9b",
}
RECT = (242, 64, 46, 20)
LINES = ("PRIVATE", "KIRAMEKI", "HIGH SCHOOL")
# Newly drawn five-pixel capitals; a one-pixel gap separates letters.
LETTERS = {
    "A": (".#.", "#.#", "###", "#.#", "#.#"),
    "C": (".##", "#..", "#..", "#..", ".##"),
    "E": ("###", "#..", "##.", "#..", "###"),
    "G": (".##.", "#...", "#.##", "#..#", ".##."),
    "H": ("#.#", "#.#", "###", "#.#", "#.#"),
    "I": ("#",) * 5,
    "K": ("#.#", "#.#", "##.", "#.#", "#.#"),
    "L": ("#.", "#.", "#.", "#.", "##"),
    "M": ("#...#", "##.##", "#.#.#", "#...#", "#...#"),
    "O": (".#.", "#.#", "#.#", "#.#", ".#."),
    "P": ("##.", "#.#", "##.", "#..", "#.."),
    "R": ("##.", "#.#", "##.", "#.#", "#.#"),
    "S": (".##", "#..", ".#.", "..#", "##."),
    "T": ("###", ".#.", ".#.", ".#.", ".#."),
    "V": ("#.#", "#.#", "#.#", "#.#", ".#."),
}


def render(source):
    if source.shape != (160, 320, 3):
        raise ArtBuildError("Festival plate requires its proven 320x160 geometry")
    x, y, w, h = RECT
    face = source[y:y + h, x:x + w]
    clean = face.min(axis=2) >= 27
    if clean.sum() < 12:
        raise ArtBuildError("Festival plate has too few clean color samples")
    yy, xx = np.indices((h, w))
    design = np.stack([np.ones((h, w)), xx, yy], axis=-1)
    coeff = np.linalg.lstsq(design[clean], face[clean], rcond=None)[0]
    goal = source.copy()
    plate = goal[y:y + h, x:x + w]
    plate[:] = np.clip(np.rint(design @ coeff), 0, 31).astype(int)
    for line_index, text in enumerate(LINES):
        widths = [2 if char == ' ' else len(LETTERS[char][0]) for char in text]
        width = sum(widths) + len(text) - 1
        if width > w - 2:
            raise ArtBuildError("Festival plate lettering exceeds its face")
        pen = (w - width) // 2
        top = 1 + line_index * 6
        for char, advance in zip(text, widths):
            if char != ' ':
                for row, pixels in enumerate(LETTERS[char]):
                    for col, pixel in enumerate(pixels):
                        if pixel == '#':
                            plate[top + row, pen + col] = (1, 1, 1)
            pen += advance + 1
    return goal


def items(root: Path, files_root: Path):
    result = []
    for name, digest in SOURCES.items():
        path = "CDROM/BS/" + name + ".BS"
        data = (files_root / path).read_bytes()
        if hashlib.sha256(data).hexdigest() != digest:
            raise ArtBuildError("Festival plate source differs from the verified original: " + name)
        source = mdec_splice.decode_rgb5(data, 320, 160)
        goal = render(source)
        x, y, w, h = RECT
        crop = goal[y:y + h, x:x + w]
        changed = (crop != source[y:y + h, x:x + w]).any(axis=2)
        rgba = np.zeros((h, w, 4), dtype='uint8')
        rgba[changed, :3] = ((crop << 3) | (crop >> 2))[changed]
        rgba[changed, 3] = 255
        image_id = "festival-gate-plate-" + name.lower().replace('_', '-')
        png = root / '.asset-cache/art/generated' / (image_id + '.png')
        png.parent.mkdir(parents=True, exist_ok=True)
        Image.fromarray(rgba, 'RGBA').save(png)
        meta = {'geometry': {'width': 320, 'height': 160}, 'crop': {'rect': list(RECT)}}
        image = ArtImage(image_id, png, hashlib.sha256(png.read_bytes()).hexdigest(), 'generated-v1', meta)
        result.append(Item(image_id, 'candidate-only', path, 'school-name plate', None, [image]))
    return result
