"""English speech bubbles in the three-legged-race sprite page (T-39521).

The instruction cards use later pages in NINSETU. These four bubbles share
its first 8-bit page; TAOPD supplies their unchanged animation/frame records.
Generated crops stay in the ignored asset cache.
"""
import hashlib
import struct
from collections import Counter

import numpy as np
from PIL import Image, ImageFont

from tools.patch.art import ArtBuildError
from tools.patch.art.indexed import rgba
from tools.patch.art.items import Item, Image as ArtImage

PATH = 'CDROM/TAIIKU/NINSETU.BIN'
SOURCE_SHA256 = '6058e64e8ce2f839a64bf4f942dce0b12def8fea7ed863df19ac4ec809115611'
PAGE = 0x2000
# View rectangle, interior text rectangle, English label, original ink color.
BUBBLES = {
    'sports-callout-go': ((0, 0, 58, 24), (4, 10, 51, 12), "Let's go!", 'blue'),
    'sports-callout-ready': ((80, 0, 48, 16), (83, 2, 42, 12), 'Ready!', 'red'),
    'sports-callout-right': ((88, 16, 40, 16), (91, 18, 34, 12), 'Right', 'red'),
    'sports-callout-left': ((88, 32, 40, 16), (91, 34, 34, 12), 'Left', 'red'),
}


def render(data):
    """Erase colored lettering and center English within the original bubbles."""
    if hashlib.sha256(data).hexdigest() != SOURCE_SHA256:
        raise ArtBuildError('Sports callout source differs from the verified NINSETU')
    indices = np.frombuffer(data, dtype=np.uint8, count=0x4000, offset=PAGE).reshape(128, 128)
    palette = np.array([rgba(v) for v in struct.unpack_from('<256H', data)], dtype=np.uint8)
    source = palette[indices]
    goal = source.copy()
    font = ImageFont.load_default()
    for key, (rect, interior, label, color) in BUBBLES.items():
        x, y, w, h = rect
        region = source[y:y+h, x:x+w]
        r, g, b = region[:, :, :3].astype(int).transpose(2, 0, 1)
        lettering = ((b > r + 45) & (b > g + 15) if color == 'blue'
                     else (r > b + 60) & (r > g + 25))
        if not lettering.any():
            raise ArtBuildError('Missing colored lettering: ' + key)
        colors = Counter(indices[y:y+h, x:x+w][lettering])
        channel = 2 if color == 'blue' else 0
        others = [index for index in range(3) if index != channel]
        ink_index = max(colors, key=lambda index: (
            int(palette[index, channel]) - max(int(palette[index, other]) for other in others),
            colors[index]))
        ink = palette[ink_index]
        tx, ty, tw, th = interior
        background = palette[Counter(indices[ty:ty+th, tx:tx+tw].flat).most_common(1)[0][0]]
        if max(background[:3]) - min(background[:3]) > 8 or not background[3]:
            raise ArtBuildError('Sports bubble background is not opaque neutral paper')
        goal[y:y+h, x:x+w][lettering] = background
        mask = Image.new('L', (128, 128))
        glyph_mask = font.getmask(label)
        glyphs = Image.frombytes('L', glyph_mask.size, bytes(glyph_mask))
        glyphs = glyphs.crop(glyphs.getbbox())
        if glyphs.width > tw or glyphs.height > th:
            raise ArtBuildError('English callout exceeds its bubble: ' + key)
        mask.paste(glyphs, (tx+(tw-glyphs.width)//2, ty+(th-glyphs.height)//2))
        written = np.asarray(mask) != 0
        allowed = np.zeros((128, 128), dtype=bool)
        allowed[ty:ty+th, tx:tx+tw] = True
        if (written & ~allowed).any():
            raise ArtBuildError('English callout escaped its text rectangle')
        goal[written] = ink
    return source, goal


def items(root, files_root):
    source, goal = render((files_root / PATH).read_bytes())
    output = root / '.asset-cache/art/generated/sports-callouts'
    output.mkdir(parents=True, exist_ok=True)
    result = []
    for key, (rect, interior, label, color) in BUBBLES.items():
        x, y, w, h = rect
        path = output / (key + '.png')
        Image.fromarray(goal[y:y+h, x:x+w]).save(path)
        image = ArtImage(key, path, hashlib.sha256(path.read_bytes()).hexdigest(),
                         'generated-v1', {'speech_bubble': key})
        result.append(Item(key, 'candidate-only', PATH, 'speech bubble', None, [image]))
    return result
