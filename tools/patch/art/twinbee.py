"""Place English TwinBee lettering in the original title sprites (T-39548).

The title draws a 128x64 piece and a 64x64 piece from one 4bpp page.
Keep the original CLUT, subtitle, animation records and all other texels.
"""
import hashlib
import struct
from pathlib import Path

import numpy as np
from PIL import Image

from tools.patch.art import ArtBuildError
from tools.patch.art.indexed import rgba

PATH = "CDROM/TWINSGD/TBTA.DAT"
IMAGE_ID = "twinbee-english-title"
SOURCE_SHA256 = "5bc8e2ee7723b07956b54c8df06d3a4245a527a630f50d7682d8af8badc2d7ba"
ART = Path(__file__).resolve().parents[3] / "translations/en/graphics/artwork/twinbee-title/wordmark.png"
SIZE = (192, 64)


def texel(x, y):
    """Return the packed byte offset and nibble shift for a title pixel."""
    u, v = (x, y) if x < 128 else (x - 128, y + 64)
    return 0x50000 + v * 64 + u // 2, (u & 1) * 4


def palette(data):
    colors = [rgba(w) for w in struct.unpack_from("<16H", data, 0xE0)]
    # tt_vram_upload_palettes clears entry zero before uploading each CLUT.
    colors[0] = (0, 0, 0, 0)
    return colors


def decode(data):
    colors = palette(data)
    image = Image.new("RGBA", SIZE)
    for y in range(SIZE[1]):
        for x in range(SIZE[0]):
            offset, shift = texel(x, y)
            image.putpixel((x, y), colors[(data[offset] >> shift) & 15])
    return image


def place(data):
    if hashlib.sha256(data).hexdigest() != SOURCE_SHA256:
        raise ArtBuildError("TwinBee: unexpected source texture container")
    with Image.open(ART) as image:
        if image.size != SIZE:
            raise ArtBuildError("TwinBee: title image must be 192 by 64 pixels")
        pixels = np.asarray(image.convert("RGBA"), dtype=np.int32)
    colors = np.asarray(palette(data)[1:], dtype=np.int32)
    distances = ((pixels[:, :, None, :3] - colors[None, None, :, :3]) ** 2).sum(axis=3)
    indices = distances.argmin(axis=2) + 1
    indices[pixels[:, :, 3] < 160] = 0
    output = bytearray(data)
    changed = 0
    for y in range(SIZE[1]):
        for x in range(SIZE[0]):
            offset, shift = texel(x, y)
            index = int(indices[y, x])
            changed += ((output[offset] >> shift) & 15) != index
            output[offset] = (output[offset] & ~(15 << shift)) | (index << shift)
    return bytes(output), {
        "source_sha256": SOURCE_SHA256,
        "target_sha256": hashlib.sha256(output).hexdigest(),
        "art_sha256": hashlib.sha256(ART.read_bytes()).hexdigest(),
        "changed_pixels": changed,
        "palette_unchanged": output[0xE0:0x100] == data[0xE0:0x100],
        "regions": [[0, 0, 128, 64], [0, 64, 64, 64]],
        "wording": "TWINBEE",
    }


def verify(original, written):
    expected, facts = place(original)
    if written != expected:
        raise ArtBuildError("TwinBee: final disc differs from the verified title replacement")
    return {"images": {IMAGE_ID: "verified"}, "facts": facts}
