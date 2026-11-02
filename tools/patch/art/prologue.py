"""Code-drawn English Ijuin banner in CUT_10.NBN (T-39508).

The original loads six 128x128 8bpp pages and a 256-color CLUT. The 3x2
background cells are 128x120: each lower cell starts in the last eight rows
of its upper page. Only the name-lettering regions change. The original
palette, roses, unused page rows and file size are retained. All generated
pixels use original palette entries; no extracted artwork is committed.
"""
import hashlib
import struct

import numpy as np
from PIL import Image, ImageFilter

from tools.patch.art import ArtBuildError
from tools.patch.art.indexed import rgba
from tools.patch.text.image import has_cd_trailer, write_cd_trailer

PATH = "CDROM/PROLODIR/CUT_10.NBN"
IMAGE_ID = "prologue-ijuin-rei-banner"
SOURCE_SHA256 = "4feef785ad85d06129b0e13ac88c7984e3284ae71d9dfac8c272d300835341c7"
# Port-made block capitals; outlined and filled with the original gold bands.
LETTERS = {
    "I": ["11111", "00100", "00100", "00100", "00100", "00100", "11111"],
    "J": ["00111", "00010", "00010", "00010", "10010", "10010", "01100"],
    "U": ["10001"] * 6 + ["01110"],
    "N": ["10001", "11001", "11001", "10101", "10011", "10011", "10001"],
    "R": ["11110", "10001", "10001", "11110", "10100", "10010", "10001"],
    "E": ["11111", "10000", "10000", "11110", "10000", "10000", "11111"],
}


def offsets():
    """Source byte address for every pixel in the 384x240 native background."""
    y, x = np.indices((240, 384))
    return ((y // 128) * 3 + x // 128) * 0x4000 + (y % 128) * 128 + x % 128


def decode(data):
    if len(data) != 0x18800:
        raise ArtBuildError("CUT_10: unexpected background extent")
    palette = np.array([rgba(word) for word in struct.unpack_from("<256H", data, 0x18000)], dtype=np.uint8)
    return palette[np.frombuffer(data, dtype=np.uint8)[offsets()]], palette


def place(data):
    if hashlib.sha256(data).hexdigest() != SOURCE_SHA256:
        raise ArtBuildError("CUT_10: source differs from the verified prologue background")
    source, palette = decode(data)
    r, g, b = source[:, :, :3].astype(int).transpose(2, 0, 1)
    gold = (r > 150) & (g > 100) & (r >= g) & (g > b + 40)
    gold[145:] = False
    regions = np.zeros((240, 384), dtype=bool)
    regions[23:145, :165] = True
    regions[23:145, 211:320] = True
    flowers = ((r > g + 40) & (b > g + 20)) | ((g > r + 25) & (g > b + 20))
    near_flowers = np.asarray(Image.fromarray(flowers.astype(np.uint8) * 255).filter(ImageFilter.MaxFilter(3))) > 0
    outline = np.asarray(Image.fromarray(gold.astype(np.uint8) * 255).filter(ImageFilter.MaxFilter(5))) > 0
    erase = regions & ~flowers & (~near_flowers | outline)
    goal = source.copy()
    goal[erase] = source[0, 0]
    mask = Image.new("L", (384, 240))
    # Keep the surname left of the portrait's hair at the top of the banner.
    for word, left, width in (("IJUIN", 2, 124), ("REI", 213, 104)):
        rows = ["0".join(LETTERS[c][y] for c in word) for y in range(7)]
        bitmap = np.array([[255 if c == "1" else 0 for c in row] for row in rows], dtype=np.uint8)
        glyphs = Image.fromarray(bitmap).resize((width, 108), Image.NEAREST)
        mask.paste(glyphs, (left, 27))
    ink = np.asarray(mask) > 0
    stroke = np.asarray(mask.filter(ImageFilter.MaxFilter(3))) > 0
    black = min((p for p in palette if p[3] and p[0] == p[1] == p[2]), key=lambda p: int(p[0]))
    if max(black[:3]) > 16 or not gold.any():
        raise ArtBuildError("CUT_10: expected black outline and gold lettering colors")
    goal[stroke] = black
    for y in range(27, 135):
        choices = source[y][gold[y]]
        if not len(choices):
            raise ArtBuildError("CUT_10: missing source gold band")
        goal[y, ink[y]] = choices[len(choices) // 2]
    changed = (goal != source).any(axis=2)
    if (changed & ~regions).any():
        raise ArtBuildError("CUT_10: lettering escaped its declared regions")
    # Keep every unchanged index, including palette aliases and transparency bits.
    lookup = {tuple(color): index for index, color in reversed(list(enumerate(palette)))}
    out = bytearray(data)
    addresses = offsets()
    for y, x in zip(*np.where(changed)):
        color = tuple(goal[y, x])
        if color not in lookup:
            raise ArtBuildError("CUT_10: generated color is outside the source palette")
        out[int(addresses[y, x])] = lookup[color]
    if has_cd_trailer(data):
        write_cd_trailer(out)
    facts = {"changed_pixels": int(changed.sum()), "source_sha256": hashlib.sha256(data).hexdigest(),
             "target_sha256": hashlib.sha256(out).hexdigest(), "palette_unchanged": out[0x18000:0x18200] == data[0x18000:0x18200],
             "regions": [[0, 23, 165, 122], [211, 23, 109, 122]], "wording": "Ijuin Rei"}
    return bytes(out), facts


def verify(original, written):
    expected, facts = place(original)
    if written != expected:
        raise ArtBuildError("CUT_10: final disc does not contain the generated English banner")
    return {"images": {IMAGE_ID: "verified"}, "facts": facts}
