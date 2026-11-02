"""Generated lettering for the omitted rink billboard (T-39513).

D_19_03 is read at LBA 0x4574 by date_skating_rink_enter_spot through
bg_read_sub2, the 320x160 background path. Only the three billboard text
bands change; the crop is generated in the ignored asset cache. The tiny
cropped suffix of the bus band is unresolved and requires owner review.
"""
import hashlib
from pathlib import Path

import numpy as np
from PIL import Image, ImageDraw, ImageFont, ImageFilter

from tools.assets import mdec_splice
from tools.patch.art import ArtBuildError
from tools.patch.art.items import Item, Image as ArtImage

PATH = "CDROM/BS/D_19_03.BS"
IMAGE_ID = "skating-rink-advertisement"
SOURCE_SHA256 = "79a65609c52a2a38d674bb549da0f60a17383f54c5fa0734f659b4de5419c151"
RECT = [181, 0, 139, 53]
BANDS = (
    ((194, 7, 316, 23), "ASPIRIN SNOW!", (201, 10)),
    ((224, 25, 320, 38), "SUN VALLEY SKI", (228, 27)),
    ((191, 40, 320, 50), "DIRECT SKI BUS", (195, 40)),
)


def render(source):
    """Replace dark lettering with local billboard color, then draw the draft."""
    if source.shape != (160, 320, 3):
        raise ArtBuildError("Rink billboard requires its proven 320x160 geometry")
    goal = source.copy()
    for (x0, y0, x1, y1), text, (x, y) in BANDS:
        region = goal[y0:y1, x0:x1]
        dark = region.max(axis=2) < 21
        mask = np.asarray(Image.fromarray((dark * 255).astype('uint8')).filter(ImageFilter.MaxFilter(3))) > 0
        yy, xx = np.indices(mask.shape)
        design = np.stack([np.ones(mask.shape), xx, yy], axis=-1)
        if (~mask).sum() < 3:
            raise ArtBuildError("Rink billboard has too few clean color samples")
        coeff = np.linalg.lstsq(design[~mask], region[~mask], rcond=None)[0]
        clean = np.clip(np.rint(design @ coeff), 0, 31).astype(int)
        region[mask] = clean[mask]
        ink = Image.new('L', (320, 160))
        ImageDraw.Draw(ink).text((x, y - 2), text, font=ImageFont.load_default(), fill=255)
        written = np.asarray(ink) > 0
        allowed = np.zeros((160, 320), bool)
        allowed[y0:y1, x0:x1] = True
        if (written & ~allowed).any():
            raise ArtBuildError("Rink lettering exceeds its declared text band")
        goal[written] = (3, 4, 4)
    return goal


def item(root: Path, files_root: Path):
    """Register generated pixels with the standard still encoder/readback gates."""
    data = (files_root / PATH).read_bytes()
    if hashlib.sha256(data).hexdigest() != SOURCE_SHA256:
        raise ArtBuildError("Rink billboard source differs from the verified original")
    goal = render(mdec_splice.decode_rgb5(data, 320, 160))
    x, y, w, h = RECT
    crop = goal[y:y + h, x:x + w]
    png = root / '.asset-cache/art/generated/skating-rink-advertisement.png'
    png.parent.mkdir(parents=True, exist_ok=True)
    Image.fromarray(((crop << 3) | (crop >> 2)).astype('uint8')).save(png)
    meta = {'geometry': {'width': 320, 'height': 160}, 'crop': {'rect': RECT}}
    image = ArtImage(IMAGE_ID, png, hashlib.sha256(png.read_bytes()).hexdigest(), 'generated-v1', meta)
    return Item(IMAGE_ID, 'candidate-only', PATH, 'billboard', None, [image])
