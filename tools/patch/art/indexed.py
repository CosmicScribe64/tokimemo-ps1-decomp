"""Write English images into indexed sprite sheets (T-39508, wiki/english-build.md).

A container's sprites are modelled as views: a view is one frame (or one part, or one whole page)
drawn with one set of palettes, the same way the asset readers in tools/assets/image_packs*.py
compose their PNG exports. A part maps a rectangle of the view to texels of a page (4 or 8 bpp)
and has the palette words the GPU sees for it (after the loader's STP rewrite). Parts are drawn
last first; word 0 is transparent and leaves the parts below visible.

`plan` turns English images (RGBA, the size of their view, or wider for a widened sprite) into
texel writes:
  * a pixel equal to the original composite imposes the texel it shows (or nothing when it is
    transparent and no part covers it);
  * a changed opaque pixel needs the visible part's texel (or the top covering part's, when the
    pixel was transparent) to hold an index whose palette word draws that colour, and every part
    above it to stay transparent there;
  * a changed transparent pixel needs every covering part to hold a word-0 index.
Every texel collects the intersection of what the views that see it allow. An empty set is a
conflict (two views need different indices) or a colour the palette cannot draw. The original
index is kept wherever it is allowed; otherwise the index whose STP bit equals the original
texel's is taken, then the lowest. Texels no English view sees are never written, so every
other texel of the container stays byte-identical.

The repack (tools/patch/art/placer.py) gives a view its own texels when it cannot be written in
place (shared tiles, a widened sprite), in free page space found with `Sheet`.
"""
from __future__ import annotations

import struct
from dataclasses import dataclass, field, replace

import numpy as np


class ArtError(ValueError):
    """An English image cannot be written exactly; the message says where and why."""


def rgba(word: int) -> tuple[int, int, int, int]:
    """The colour a BGR555 word draws, as the asset readers export it (word 0 transparent)."""
    r, g, b = word & 31, (word >> 5) & 31, (word >> 10) & 31
    return ((r << 3) | (r >> 2), (g << 3) | (g >> 2), (b << 3) | (b >> 2), 0 if word == 0 else 255)


TRANSPARENT = (0, 0, 0, 0)


@dataclass(frozen=True)
class Part:
    """One textured rectangle of a view: screen rectangle, texel source and palette."""
    x: int
    y: int
    width: int
    height: int
    flip: int            # bit 0 flips rows, bit 1 flips columns (as the readers decode them)
    page: int            # byte offset of the page's texel (0, 0) in the container
    bpp: int             # 4 or 8
    u: int
    v: int
    palette: tuple       # GPU-visible words for every index
    record: int | None = None   # offset of the 14-byte part record, when it can be rewritten
    page_index: int | None = None
    clut: int | None = None     # the record's palette field, kept by a repack

    @property
    def row_texels(self) -> int:
        return 128

    def texel(self, x: int, y: int) -> tuple[int, int]:
        """(byte offset, nibble shift or -1) of the texel drawn at part-local (x, y)."""
        sx = self.width - 1 - x if self.flip & 2 else x
        sy = self.height - 1 - y if self.flip & 1 else y
        index = (self.v + sy) * self.row_texels + self.u + sx
        if self.bpp == 8:
            return self.page + index, -1
        return self.page + index // 2, 4 * (index & 1)


@dataclass
class View:
    """A frame, part or page as one exported image shows it."""
    key: str
    parts: list[Part]
    left: int
    top: int
    width: int
    height: int
    frame: int | None = None
    record: int | None = None   # offset of the frame record (count, origin, flags), when rewritable

    @classmethod
    def of(cls, key: str, parts: list[Part], **extra) -> "View":
        if not parts:
            return cls(key, [], 0, 0, 0, 0, **extra)
        left, top = min(p.x for p in parts), min(p.y for p in parts)
        width = max(p.x + p.width for p in parts) - left
        height = max(p.y + p.height for p in parts) - top
        return cls(key, list(parts), left, top, width, height, **extra)


def read_index(data, address: tuple[int, int]) -> int:
    offset, shift = address
    return data[offset] if shift < 0 else (data[offset] >> shift) & 15


def write_index(data: bytearray, address: tuple[int, int], index: int) -> None:
    offset, shift = address
    if shift < 0:
        data[offset] = index
    else:
        data[offset] = (data[offset] & ~(15 << shift) & 0xFF) | (index << shift)


_COVER: dict = {}


def covering(view: View, x: int, y: int):
    """(part number, part, texel address) of every part over view pixel (x, y), top first."""
    entry = _COVER.get(id(view))
    table = entry[1] if entry is not None and entry[0] is view else None
    if table is None:
        if len(_COVER) > 512:
            _COVER.clear()
        table = {}
        for number, part in enumerate(view.parts):
            for ly in range(part.height):
                for lx in range(part.width):
                    table.setdefault((part.x + lx - view.left, part.y + ly - view.top), []).append(
                        (number, part, part.texel(lx, ly)))
        _COVER[id(view)] = (view, table)   # the view is kept, so its id is not reused
    return table.get((x, y), [])


def compose(data, view: View, width: int | None = None) -> list[tuple[int, int, int, int]]:
    """The view's RGBA pixels, row by row (width defaults to the view's)."""
    width = view.width if width is None else width
    pixels = [TRANSPARENT] * (width * view.height)
    for part in reversed(view.parts):
        for ly in range(part.height):
            for lx in range(part.width):
                word = part.palette[read_index(data, part.texel(lx, ly))]
                if word:
                    x, y = part.x - view.left + lx, part.y - view.top + ly
                    if 0 <= x < width:
                        pixels[y * width + x] = rgba(word)
    return pixels


@dataclass
class Plan:
    """Texel writes for a set of English views, and what stopped the others."""
    writes: dict = field(default_factory=dict)        # address -> index
    errors: dict = field(default_factory=dict)        # view key -> reason
    conflicts: dict = field(default_factory=dict)     # view key -> other view keys sharing a texel
    widened: set = field(default_factory=set)         # view keys whose image is wider than the view
    ambiguous: int = 0                                # texels where several indices fit (STP rule used)


def _constrain(allowed: dict, owners: dict, address, indices: frozenset, key: str) -> None:
    if address in allowed:
        allowed[address] = allowed[address] & indices
    else:
        allowed[address] = indices
    owners.setdefault(address, set()).add(key)


def _palette_sets(part: Part, cache: dict) -> tuple[dict, frozenset]:
    found = cache.get(part.palette)
    if found is None:
        by_colour: dict = {}
        for index, word in enumerate(part.palette):
            by_colour.setdefault(rgba(word), set()).add(index)
        found = ({c: frozenset(s) for c, s in by_colour.items()},
                 frozenset(i for i, w in enumerate(part.palette) if w == 0))
        cache[part.palette] = found
    return found


def constraints(data, view: View, image: list, width: int) -> tuple[dict, str | None]:
    """address -> allowed indices for one English image, or the reason it cannot be drawn."""
    cache: dict = {}
    out: dict = {}
    if width > view.width:
        return out, "widened"
    if len(image) != width * view.height or width != view.width:
        return out, "image is %dx%d, view %dx%d" % (width, len(image) // max(width, 1), view.width, view.height)
    for y in range(view.height):
        for x in range(width):
            target = image[y * width + x]
            if target[3] == 0:
                target = TRANSPARENT
            elif target[3] != 255:
                return out, "pixel (%d, %d) has alpha %d" % (x, y, target[3])
            parts = covering(view, x, y)
            if not parts:
                if target != TRANSPARENT:
                    return out, "pixel (%d, %d) is outside every part" % (x, y)
                continue
            if target == TRANSPARENT:
                for _n, part, address in parts:
                    _colours, zero = _palette_sets(part, cache)
                    if not zero:
                        return out, "pixel (%d, %d): the palette has no transparent word" % (x, y)
                    out[address] = out.get(address, zero) & zero
                continue
            visible = next((i for i, (_n, part, address) in enumerate(parts)
                            if part.palette[read_index(data, address)]), 0)
            for i, (_n, part, address) in enumerate(parts[:visible + 1]):
                colours, zero = _palette_sets(part, cache)
                want = colours.get(target, frozenset()) if i == visible else zero
                if not want:
                    return out, "pixel (%d, %d): colour %s is not in the part's palette" % (x, y, target[:3])
                out[address] = out.get(address, want) & want
                if not out[address]:
                    return out, "pixel (%d, %d) needs two indices in one texel" % (x, y)
    return out, None


def choose(data, address, allowed: frozenset, palette: tuple) -> tuple[int, bool]:
    """The index to write: the original if allowed, else by the original STP bit, else lowest."""
    original = read_index(data, address)
    if original in allowed:
        return original, False
    stp = palette[original] & 0x8000
    same = sorted(i for i in allowed if palette[i] & 0x8000 == stp)
    pick = same[0] if same else min(allowed)
    return pick, len(allowed) > 1


def plan(data, targets: list[tuple[View, list, int]]) -> Plan:
    """Texel writes for (view, RGBA image, image width) targets; failing views are left out."""
    result = Plan()
    per_view = {}
    for view, image, width in targets:
        found, reason = constraints(data, view, image, width)
        if reason == "widened":
            result.widened.add(view.key)
        elif reason:
            result.errors[view.key] = reason
        else:
            per_view[view.key] = (view, found)
    # intersect across views; a texel two views disagree on drops both views
    allowed: dict = {}
    owners: dict = {}
    for key, (view, found) in per_view.items():
        for address, indices in found.items():
            _constrain(allowed, owners, address, indices, key)
    bad = {address for address, indices in allowed.items() if not indices}
    for address in bad:
        keys = owners[address]
        for key in keys:
            result.conflicts.setdefault(key, set()).update(keys - {key})
    palettes = {}
    for key, (view, found) in per_view.items():
        if key in result.conflicts:
            continue
        for part in view.parts:
            for ly in range(part.height):
                for lx in range(part.width):
                    palettes.setdefault(part.texel(lx, ly), part.palette)
    for address, indices in allowed.items():
        if address in bad or any(k in result.conflicts for k in owners[address]):
            continue
        index, ambiguous = choose(data, address, indices, palettes[address])
        result.ambiguous += ambiguous
        if index != read_index(data, address):
            result.writes[address] = index
    return result


def apply(data: bytearray, writes: dict) -> None:
    for address, index in writes.items():
        write_index(data, address, index)


# ---------------------------------------------------------------- repack

class Sheet:
    """Texels of the pages a repack may use; parts start at multiples of `align`.

    A texel is free, or held with the index it has (an original part still draws it, a placed
    piece wrote it, or no frame draws it but it is not index 0). A piece may go where every texel is free or already holds the index the
    piece needs there, so held texels never change.
    """

    def __init__(self, data, pages: dict, align: int, parts: list, every_part: list = ()):
        self.pages = pages
        self.align = align
        self.used = {}
        self.value = {}
        for index, (offset, bpp) in pages.items():
            if bpp == 8:
                values = np.frombuffer(bytes(data[offset:offset + 0x4000]), dtype=np.uint8).reshape(128, 128)
            else:
                raw = np.frombuffer(bytes(data[offset:offset + 0x2000]), dtype=np.uint8).reshape(128, 64)
                values = np.empty((128, 128), dtype=np.uint8)
                values[:, 0::2] = raw & 15
                values[:, 1::2] = raw >> 4
            self.value[index] = values.copy()
            self.used[index] = np.zeros((128, 128), dtype=bool)
        by_offset = {(offset, bpp): index for index, (offset, bpp) in pages.items()}
        if every_part:
            # texels no frame draws but that hold ink may be drawn by code (EXAMINE's score digits
            # are direct sprites): they stay as they are
            seen = {index: np.zeros((128, 128), dtype=bool) for index in pages}
            for part in every_part:
                index = by_offset.get((part.page, part.bpp))
                if index is not None:
                    seen[index][part.v:part.v + part.height, part.u:part.u + part.width] = True
            for index in pages:
                self.used[index] |= ~seen[index] & (self.value[index] != 0)
        for part in parts:
            index = by_offset.get((part.page, part.bpp))
            if index is not None:
                self.used[index][part.v:part.v + part.height, part.u:part.u + part.width] = True

    def hold(self, part: Part, indices) -> None:
        """Keep a part's own texels with these indices (screen order, the part's flip applied)."""
        by_offset = {(offset, bpp): index for index, (offset, bpp) in self.pages.items()}
        index = by_offset.get((part.page, part.bpp))
        if index is None:
            return
        for ly in range(part.height):
            for lx in range(part.width):
                sx = part.width - 1 - lx if part.flip & 2 else lx
                sy = part.height - 1 - ly if part.flip & 1 else ly
                self.used[index][part.v + sy, part.u + sx] = True
                self.value[index][part.v + sy, part.u + sx] = indices[ly * part.width + lx]

    def free(self, index: int) -> int:
        return int((~self.used[index]).sum())

    def take(self, width: int, height: int, bpp: int, indices, allowed=None) -> tuple[int, int, int] | None:
        """(page index, u, v) where a width x height piece with these indices fits, or None.

        Pages with the most free texels are tried first, then positions row by row.
        """
        need = np.array(indices, dtype=np.uint8).reshape(height, width)
        candidates = sorted((index for index, (_o, b) in self.pages.items()
                             if b == bpp and (allowed is None or index in allowed)),
                            key=lambda i: (-self.free(i), i))
        for index in candidates:
            used, value = self.used[index], self.value[index]
            for v in range(0, 128 - height + 1, self.align):
                for u in range(0, 128 - width + 1, self.align):
                    held = used[v:v + height, u:u + width]
                    if not held.any() or np.all(~held | (value[v:v + height, u:u + width] == need)):
                        used[v:v + height, u:u + width] = True
                        value[v:v + height, u:u + width] = need
                        return index, u, v
        return None
