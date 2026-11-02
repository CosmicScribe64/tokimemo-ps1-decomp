"""Place a container's English images: in place where possible, else on fresh texels (T-39508).

Input: the original container bytes, its profile (tools/patch/art/sheets.py) and targets, each an
English RGBA image for one view. Targets of one frame (its palette variants, the parts of it that
an image covers) form a group; a group is placed as a whole or not at all.

1. Part images are turned into frame images: the frame composed with the part's texels replaced.
2. In place (indexed.plan): every group whose texels can take the English indices without
   touching a texel that another view still draws (any frame without English art, or a part a
   repacked group keeps) is written in place.
3. The other groups are repacked when the profile can rewrite part records:
   - relocate: every part that has to change gets a fresh copy of its texels with the English
     pixels, at the same place on screen and with its own palette (records keep their count);
   - rebuild: the frame is redrawn by new parts cut from the English image, one per palette
     (and per 128 texels), when the image is wider than the frame, draws where no part is, or a
     part's palette cannot draw its colours; the frame record keeps at most its part count.
   A group that fits neither way, or finds no free texels, is deferred with the reason.
4. Every view of the container is composed again from the new bytes and records: each target must
   equal its English image exactly, and every other view its original. A difference is a bug and
   raises ArtError.
"""
from __future__ import annotations

import copy
import struct
from dataclasses import dataclass, field, replace

from tools.patch.art.indexed import (TRANSPARENT, ArtError, Part, Sheet, View, compose, covering, plan,
                                     read_index, rgba, write_index)


@dataclass
class Target:
    image_id: str
    item_id: str
    view: View
    pixels: list
    width: int
    part: int | None = None       # the image covers this part of view's frame only
    group: str = ""               # frames drawn from the same texels (set by place)
    prefer: dict = field(default_factory=dict)   # view pixel -> part that should draw it (part images)


@dataclass
class Result:
    data: bytearray
    records: dict = field(default_factory=dict)       # record offset -> bytes (in the record space)
    placed: dict = field(default_factory=dict)        # image id -> "in_place" / "relocated" / "rebuilt"
    deferred: dict = field(default_factory=dict)      # image id -> reason
    texels: int = 0                                   # texels written
    new_parts: int = 0
    ambiguous: int = 0
    touched: set = field(default_factory=set)        # byte offsets of every texel written
    frames: dict = field(default_factory=dict)       # frame index -> new frame record (profile.grow)


def frame_image(data, view: View, part: int, image: list, width: int, others: dict | None = None) -> list:
    """The frame's composite with parts drawn from part-local images instead of their texels.

    `part`/`image` is one replaced part; `others` maps further part numbers to (image, width).
    """
    images = dict(others or {})
    images[part] = (image, width)
    for number, (local, local_width) in images.items():
        target = view.parts[number]
        if local_width != target.width or len(local) != target.width * target.height:
            raise ArtError("%s: part image is %dx%d, part %dx%d" % (view.key, local_width,
                                                                   len(local) // max(local_width, 1),
                                                                   target.width, target.height))
    pixels = [TRANSPARENT] * (view.width * view.height)
    for number in reversed(range(len(view.parts))):
        p = view.parts[number]
        for ly in range(p.height):
            for lx in range(p.width):
                if number in images:
                    local, local_width = images[number]
                    colour = local[ly * local_width + lx]
                    colour = TRANSPARENT if colour[3] == 0 else colour
                else:
                    word = p.palette[read_index(data, p.texel(lx, ly))]
                    colour = rgba(word) if word else TRANSPARENT
                if colour != TRANSPARENT:
                    pixels[(p.y - view.top + ly) * view.width + p.x - view.left + lx] = colour
    return pixels


def _texels(part: Part) -> set:
    return {part.texel(lx, ly) for ly in range(part.height) for lx in range(part.width)}


def _merge_parts(data, targets: list[Target]) -> list[Target]:
    """One frame target per view key: part images folded into their frame's composite."""
    out: dict[str, Target] = {}
    parts: dict[str, list[Target]] = {}
    for target in targets:
        if target.part is None:
            out[target.view.key] = target
        else:
            parts.setdefault(target.view.key, []).append(target)
    for key, group in parts.items():
        first = group[0]
        others = {t.part: (t.pixels, t.width) for t in group[1:]}
        pixels = frame_image(data, first.view, first.part, first.pixels, first.width, others)
        merged = Target("+".join(t.image_id for t in group), first.item_id, first.view, pixels, first.view.width)
        for t in group:
            p = t.view.parts[t.part]
            for ly in range(p.height):
                for lx in range(p.width):
                    merged.prefer[(p.x - t.view.left + lx, p.y - t.view.top + ly)] = t.part
        out[key] = merged
    return list(out.values())


def _group_key(view: View) -> str:
    return view.key.split(".")[0]


def _signature(view: View) -> tuple:
    """Frames with equal signatures draw the same texels at the same places (palettes may differ)."""
    return tuple((p.x, p.y, p.width, p.height, p.flip, p.page, p.u, p.v) for p in view.parts)


def _desired(data, group: list[Target]) -> tuple[dict, str | None]:
    """part number -> list per target of part-local desired colours; or why a relocate cannot do it."""
    view0 = group[0].view
    desired = {}
    for t in group:
        if t.width != t.view.width:
            return {}, "widened"
    for number, part in enumerate(view0.parts):
        desired[number] = []
    for t in group:
        view = t.view
        local = {n: [rgba(p.palette[read_index(data, p.texel(lx, ly))])
                     for ly in range(p.height) for lx in range(p.width)] for n, p in enumerate(view.parts)}
        for y in range(view.height):
            for x in range(view.width):
                want = t.pixels[y * view.width + x]
                want = TRANSPARENT if want[3] == 0 else want
                parts = covering(view, x, y)
                if not parts:
                    if want != TRANSPARENT:
                        return {}, "draws outside every part"
                    continue
                visible = next((i for i, (_n, p, a) in enumerate(parts) if p.palette[read_index(data, a)]), None)
                if want == TRANSPARENT:
                    for _n, p, _a in parts:
                        local[_n][(view.top + y - p.y) * p.width + view.left + x - p.x] = TRANSPARENT
                    continue
                decide = visible if visible is not None else 0
                preferred = t.prefer.get((x, y))
                if preferred is not None:
                    # a pixel of a part image is drawn by that part (the parts above it stay clear)
                    decide = next((i for i, (n, _p, _a) in enumerate(parts) if n == preferred), decide)
                for i, (n, p, _a) in enumerate(parts[:decide + 1]):
                    local[n][(view.top + y - p.y) * p.width + view.left + x - p.x] = want if i == decide else TRANSPARENT
        for n in local:
            desired[n].append(local[n])
    return desired, None


def _indices_for(part_views: list[Part], colours: list) -> frozenset:
    allowed = None
    for part, colour in zip(part_views, colours):
        fit = frozenset(i for i, w in enumerate(part.palette) if (rgba(w) if w else TRANSPARENT) == colour)
        allowed = fit if allowed is None else allowed & fit
    return allowed or frozenset()


def _pick(allowed: frozenset, palette: tuple, stp: int) -> int:
    same = sorted(i for i in allowed if palette[i] & 0x8000 == stp)
    return same[0] if same else min(allowed)


def _majority_stp(data, view: View) -> int:
    counts = [0, 0]
    for part in view.parts:
        for ly in range(part.height):
            for lx in range(part.width):
                word = part.palette[read_index(data, part.texel(lx, ly))]
                if word:
                    counts[bool(word & 0x8000)] += 1
    return 0x8000 if counts[1] > counts[0] else 0


@dataclass
class Piece:
    """A texel rectangle a repack needs: its indices (row-major, unflipped) and the part it becomes."""
    bpp: int
    width: int
    height: int
    indices: tuple
    part: Part          # geometry and palette; page, u and v are set when it is placed
    number: int | None  # the original part it replaces (relocate), None for a new part (rebuild)
    home: int | None = None   # the part whose palette it uses (frame part number, rebuild)
    fixed: bool = False       # rewritten where it is: no other part draws these texels


@dataclass
class Repack:
    key: str
    group: list
    mode: str                                   # "relocated" or "rebuilt"
    pieces: list
    kept: list = field(default_factory=list)    # original parts the frame still draws
    max_parts: int = 0                          # part records the frame has room for (rebuild)
    alternative: "Repack | None" = None         # the other way to draw the group, tried when this finds no room
    grown: bool = False                         # the frame gets a new, longer record (profile.grow)

    @property
    def area(self) -> int:
        return sum(p.width * p.height for p in self.pieces if not p.fixed)


def _relocate(data, group: list[Target], palettes: list[Part], exclusive=lambda number: False, grow: int = 0):
    """Pieces for every part whose desired image differs, at the same place on screen.

    A part keeps its palette when it can draw the part's English pixels; a frame drawn with one
    palette set may take another palette of the container (its record's palette field changes).
    A part whose texels no other part draws is rewritten where it is (`fixed`); a moved part is
    cut to the box of its opaque pixels (its record's position and size change).
    """
    desired, why = _desired(data, group)
    if why:
        return None, why
    view0 = group[0].view
    stp = _majority_stp(data, view0)
    pieces, kept = [], []
    for number, part in enumerate(view0.parts):
        current = [[rgba(t.view.parts[number].palette[read_index(data, part.texel(lx, ly))])
                    for ly in range(part.height) for lx in range(part.width)] for t in group]
        current = [[TRANSPARENT if c[3] == 0 else c for c in image] for image in current]
        if current == desired[number]:
            kept.append(part)
            continue
        options = [part] + ([p for p in palettes if p.bpp == part.bpp and (p.palette, p.clut) != (part.palette, part.clut)]
                            if len(group) == 1 else [])
        images = [desired[number][k] for k in range(len(group))]

        def draw(pixels, options=options, number=number, part=part, images=images):
            """(palette part, indices) of the first option that draws these part-local pixels."""
            for option in options:
                variants = [t.view.parts[number] for t in group] if option is part else [option]
                out = []
                for pixel in pixels:
                    allowed = _indices_for(variants, [image[pixel] for image in images])
                    if not allowed:
                        break
                    out.append(_pick(allowed, option.palette, stp))
                else:
                    return option, out
            return None, None
        option, indices = draw(range(part.width * part.height))
        if option is not None and option is part and exclusive(number):
            pieces.append(Piece(part.bpp, part.width, part.height, tuple(indices), part, number, number, True))
            continue
        box = _opaque_box(images, part.width, part.height)
        if box is None:
            box = (0, 0, 1, 1)      # nothing left to draw: one transparent texel keeps the record valid
        bx, by, bw, bh = box
        if grow:
            # the frame gets a new record: the part becomes tiles of `grow` texels (blank ones left
            # out), each with the first palette that draws it
            for ty in range(by, by + bh, grow):
                for tx in range(bx, bx + bw, grow):
                    tw, th = min(grow, bx + bw - tx), min(grow, by + bh - ty)
                    cells = [(ty + y) * part.width + tx + x for y in range(th) for x in range(tw)]
                    if all(image[c] == TRANSPARENT for image in images for c in cells):
                        continue
                    layers = _layers(cells, draw, options, group, number, part, images, stp)
                    if layers is None:
                        return None, "part %d: no palettes draw a %dx%d tile" % (number, tw, th)
                    for tile_option, tile in layers:
                        moved = replace(part, flip=0, palette=tile_option.palette, clut=tile_option.clut,
                                        x=part.x + tx, y=part.y + ty, width=tw, height=th)
                        pieces.append(Piece(part.bpp, tw, th, tuple(tile), moved, number, number))
            continue
        if option is None:
            return None, "part %d: no palette draws its colours" % number
        moved = replace(part, flip=0, palette=option.palette, clut=option.clut)
        indices = [indices[(by + y) * part.width + bx + x] for y in range(bh) for x in range(bw)]
        moved = replace(moved, x=part.x + bx, y=part.y + by, width=bw, height=bh)
        pieces.append(Piece(part.bpp, bw, bh, tuple(indices), moved, number, number))
    if not pieces:
        return None, "nothing to change"
    return Repack(group[0].group, group, "relocated", pieces, kept, grown=bool(grow)), None


def _layers(cells, draw, options, group, number, part, images, stp):
    """[(palette part, indices)] drawing these pixels: one tile when a palette draws them all,
    else stacked tiles of the same rectangle, each drawing the pixels its palette can (the others
    transparent). None when some pixel has no palette."""
    option, tile = draw(cells)
    if option is not None:
        return [(option, tile)]
    left = [c for c in cells if any(image[c] != TRANSPARENT for image in images)]
    layers = []
    for option in options:
        variants = [t.view.parts[number] for t in group] if option is part else [option]
        zero = _indices_for(variants, [TRANSPARENT] * len(images))
        if not zero:
            continue
        mine = {c for c in left if _indices_for(variants, [image[c] for image in images])}
        if not mine:
            continue
        layers.append((option, [_pick(_indices_for(variants, [image[c] for image in images]) if c in mine else zero,
                                      option.palette, stp) for c in cells]))
        left = [c for c in left if c not in mine]
        if not left:
            return layers
    return None


def _opaque_box(images: list, width: int, height: int):
    """Smallest rectangle holding every opaque pixel of the part-local images, or None."""
    xs = [i % width for image in images for i, c in enumerate(image) if c != TRANSPARENT]
    ys = [i // width for image in images for i, c in enumerate(image) if c != TRANSPARENT]
    if not xs:
        return None
    return min(xs), min(ys), max(xs) - min(xs) + 1, max(ys) - min(ys) + 1


def _chunks(x0: int, y0: int, w: int, h: int, limit: int = 128):
    for y in range(y0, y0 + h, limit):
        for x in range(x0, x0 + w, limit):
            yield x, y, min(limit, x0 + w - x), min(limit, y0 + h - y)


def _rebuild(data, group: list[Target], max_parts: int, palettes: list[Part]):
    """Pieces cut from the English image: one per palette and 128-texel chunk.

    Palettes come from the frame's parts, then (for a frame drawn with one palette set) from the
    rest of the container.
    """
    view0 = group[0].view
    homes_from = list(view0.parts) + ([p for p in palettes if p.bpp == view0.parts[0].bpp] if len(group) == 1 else [])
    width = group[0].width
    height = view0.height
    for t in group:
        if t.width != width or len(t.pixels) != width * height:
            return None, "variant images differ in size"
    homes = []          # part numbers whose palettes are used, in order of first use
    assignment = {}     # pixel -> home part number
    for pixel in range(width * height):
        colours = [TRANSPARENT if t.pixels[pixel][3] == 0 else t.pixels[pixel] for t in group]
        if all(c == TRANSPARENT for c in colours):
            continue
        for number in homes + [n for n in range(len(homes_from)) if n not in homes]:
            if _indices_for(_variants(group, homes_from, number), colours):
                if number not in homes:
                    homes.append(number)
                assignment[pixel] = number
                break
        else:
            return None, "colour %s is in no palette of the frame" % (colours[0][:3],)
    if not assignment:
        return None, "the image is empty"
    stp = _majority_stp(data, view0)
    boxes = []          # (home number, x, y, w, h): one per word (column run) of each palette
    for number in homes:
        mine = {p for p, n in assignment.items() if n == number}
        boxes.extend((number,) + box for box in _word_boxes(mine, width))
    boxes = _merge_boxes(boxes, max_parts, assignment, width)
    if boxes is None:
        return None, "needs more parts than the frame record has (%d)" % max_parts
    pieces = []
    for number, bx0, by0, bw0, bh0 in boxes:
        home = homes_from[number]
        variants = _variants(group, homes_from, number)
        zero = _indices_for(variants, [TRANSPARENT] * len(group))
        if not zero:
            return None, "palette of part %d has no transparent index" % number
        for bx, by, bw, bh in _chunks(bx0, by0, bw0, bh0):
            indices = []
            for ly in range(bh):
                for lx in range(bw):
                    pixel = (by + ly) * width + bx + lx
                    if assignment.get(pixel) == number:
                        colours = [TRANSPARENT if t.pixels[pixel][3] == 0 else t.pixels[pixel] for t in group]
                        indices.append(_pick(_indices_for(variants, colours), home.palette, stp))
                    else:
                        indices.append(_pick(zero, home.palette, stp))
            part = replace(home, x=view0.left + bx, y=view0.top + by, width=bw, height=bh, flip=0)
            pieces.append(Piece(home.bpp, bw, bh, tuple(indices), part, None, number))
    if len(pieces) > max_parts:
        return None, "needs %d parts, the frame record has %d" % (len(pieces), max_parts)
    return Repack(group[0].group, group, "rebuilt", pieces, max_parts=max_parts), None


def _box(pixels, width: int) -> tuple[int, int, int, int]:
    xs, ys = [p % width for p in pixels], [p // width for p in pixels]
    return min(xs), min(ys), max(xs) - min(xs) + 1, max(ys) - min(ys) + 1


def _word_boxes(pixels: set, width: int) -> list[tuple[int, int, int, int]]:
    """Ink boxes of the column runs of a pixel set (a word, or a letter group, per box).

    Small boxes pack better, need fewer rows (a word without descenders) and let equal words
    of different labels share texels.
    """
    columns = sorted({p % width for p in pixels})
    runs, start = [], columns[0]
    for previous, column in zip(columns, columns[1:]):
        if column != previous + 1:
            runs.append((start, previous))
            start = column
    runs.append((start, columns[-1]))
    return [_box([p for p in pixels if lo <= p % width <= hi], width) for lo, hi in runs]


def _merge_boxes(boxes: list, limit: int, assignment: dict, width: int):
    """Join neighbouring boxes of one palette (closest first) until the 128-texel chunks fit."""
    def count(items):
        return sum(len(list(_chunks(*b[1:]))) for b in items)
    boxes = sorted(boxes)
    while count(boxes) > limit:
        best = None
        for i in range(len(boxes) - 1):
            a, b = boxes[i], boxes[i + 1]
            if a[0] != b[0]:
                continue
            gap = b[1] - (a[1] + a[3])
            if best is None or gap < best[0]:
                best = (gap, i)
        if best is None:
            return None
        i = best[1]
        a, b = boxes[i], boxes[i + 1]
        pixels = [p for p, n in assignment.items() if n == a[0] and a[1] <= p % width < b[1] + b[3]]
        boxes[i:i + 2] = [(a[0],) + _box(pixels, width)]
    return boxes


def _variants(group: list[Target], homes_from: list[Part], number: int) -> list[Part]:
    """The part's palette in every target of the group (an outside palette: single target)."""
    if number < len(group[0].view.parts):
        return [t.view.parts[number] for t in group]
    return [homes_from[number]]


def _split(piece: Piece, align: int):
    """Two pieces for the halves of a piece (cut on an align boundary), or None when too small."""
    w, h = piece.width, piece.height
    if max(w, h) <= 2 * align:
        return None
    if w >= h:
        cut = (w // 2 + align - 1) // align * align
        parts = ((0, 0, cut, h), (cut, 0, w - cut, h))
    else:
        cut = (h // 2 + align - 1) // align * align
        parts = ((0, 0, w, cut), (0, cut, w, h - cut))
    out = []
    for x, y, cw, ch in parts:
        indices = tuple(piece.indices[(y + j) * w + x + i] for j in range(ch) for i in range(cw))
        part = replace(piece.part, x=piece.part.x + x, y=piece.part.y + y, width=cw, height=ch)
        out.append(Piece(piece.bpp, cw, ch, indices, part, None, piece.home))
    return out


def _allocate(data, repacks: list[Repack], pages: dict, align: int, reserved: list[Part], every_part: list[Part],
              page_sets: dict, referenced: set, policy: str = "split"):
    """Place every piece; returns ({group key: placed parts}, {group key: why it found no room}).

    Pieces go largest first. A piece may overlap texels that already hold the indices it needs
    (an identical piece shares all of them). When a piece finds no room, a group is dropped (see
    _drop; its old texels are reserved again) and the allocation starts over.
    """
    failed: dict[str, str] = {}
    while True:
        live = [r for r in repacks if r.key not in failed]
        held = list(reserved)
        held += [p for r in repacks if r.key in failed for t in r.group for p in t.view.parts]
        held += [p for r in live for p in r.kept]
        sheet = Sheet(data, pages, align, held, every_part)
        for r in live:
            for piece in r.pieces:
                if piece.fixed:
                    sheet.hold(piece.part, piece.indices)
        order = sorted(((piece.width * piece.height, r.key, i) for r in live for i, piece in enumerate(r.pieces)
                        if not piece.fixed),
                       key=lambda e: (-e[0], e[1], e[2]))
        by_key = {r.key: r for r in live}
        placed: dict[str, dict[int, Part]] = {r.key: {i: p.part for i, p in enumerate(r.pieces) if p.fixed}
                                              for r in live}
        seen: dict = {}
        missing = None
        for _area, key, i in order:
            piece = by_key[key].pieces[i]
            allowed = page_sets.get(by_key[key].group[0].view.key, referenced)
            content = (piece.bpp, piece.width, piece.height, piece.indices,
                       None if allowed is None else tuple(sorted(allowed)))
            where = seen.get(content)
            if where is None:
                where = sheet.take(piece.width, piece.height, piece.bpp, piece.indices, allowed)
                if where is None:
                    missing = (key, "no free %dx%d texel rectangle" % (piece.width, piece.height))
                    break
                seen[content] = where
            page_index, u, v = where
            placed[key][i] = replace(piece.part, page=pages[page_index][0], u=u, v=v, page_index=page_index)
        if missing is None:
            return {k: [placed[k][i] for i in range(len(by_key[k].pieces))] for k in placed}, failed
        repack = by_key[missing[0]]
        alone = Sheet(data, pages, align, held, every_part)
        for r in live:
            for piece in r.pieces:
                if piece.fixed:
                    alone.hold(piece.part, piece.indices)
        allowed = page_sets.get(repack.group[0].view.key, referenced)
        hopeless = any(not p.fixed and alone.take(p.width, p.height, p.bpp, p.indices, allowed) is None
                       for p in repack.pieces)
        if policy == "settle" and not hopeless:
            failed[_drop(repacks, failed, reserved, pages, align)] = missing[1]
            continue
        if repack.alternative is not None:
            other, repack.alternative = repack.alternative, None
            repacks[repacks.index(repack)] = other
            continue
        if repack.mode == "rebuilt" and len(repack.pieces) < repack.max_parts:
            biggest = max(range(len(repack.pieces)), key=lambda i: repack.pieces[i].width * repack.pieces[i].height)
            halves = _split(repack.pieces[biggest], align)
            if halves:
                repack.pieces[biggest:biggest + 1] = halves
                continue
        if hopeless:
            failed[missing[0]] = missing[1]
        else:
            failed[_drop(repacks, failed, reserved, pages, align)] = missing[1]


def _allocate_best(data, repacks: list[Repack], *args):
    """Allocate with both failure policies and keep the one that places more groups.

    "split": the failing group first tries its other way and smaller pieces; "settle": while
    its pieces could fit alone, another group is dropped first. Returns the repacks used too.
    """
    best = None
    for policy in ("split", "settle"):
        trial = copy.deepcopy(repacks)
        placed, failed = _allocate(data, trial, *args, policy=policy)
        if best is None or len(failed) < len(best[1]):
            best = (placed, failed, trial)
    return best


def _drop(repacks: list[Repack], failed: dict, reserved: list[Part], pages: dict, align: int) -> str:
    """The group to give up when the pages are full: the one that frees the most room.

    A dropped group keeps its original texels, so dropping costs the texels only it held (not yet
    held by the reserved parts or other dropped groups) and gains the area of its new pieces.
    """
    def mask(parts):
        sheet = Sheet(bytes(0x80000), pages, align, parts)
        return sheet.used
    held = mask(list(reserved) + [p for r in repacks if r.key in failed for t in r.group for p in t.view.parts])
    best = None
    for r in repacks:
        if r.key in failed:
            continue
        own = mask([p for t in r.group for p in t.view.parts])
        cost = sum(int((own[i] & ~held[i]).sum()) for i in own) - r.area
        if best is None or (cost, r.key) < best[0]:
            best = ((cost, r.key), r.key)
    return best[1]


def place(data: bytes, profile, targets: list[Target], protect_views: list[View]) -> Result:
    """Write the targets into a copy of data; see the module docstring."""
    out = Result(bytearray(data))
    targets = _merge_parts(data, targets)
    # a normal and a highlighted frame often draw the same texels with two palettes: one group
    signatures = {}
    for t in targets:
        signatures.setdefault(_group_key(t.view), _signature(t.view))
    canon, first = {}, {}
    for key in sorted(signatures):
        canon[key] = first.setdefault(signatures[key], key)
    for t in targets:
        t.group = canon[_group_key(t.view)]

    def group_of(view: View) -> str:
        return canon.get(_group_key(view), _group_key(view))
    groups: dict[str, list[Target]] = {}
    for t in targets:
        groups.setdefault(t.group, []).append(t)
    protected = set()
    for view in protect_views:
        if group_of(view) not in groups:
            for part in view.parts:
                protected |= _texels(part)
    repack: set[str] = set()
    for _round in range(len(groups) + 1):
        active = [t for key, g in groups.items() if key not in repack for t in g]
        # a repacked or deferred group may keep some of its old texels: nobody else writes them
        held = set(protected)
        for key in repack:
            for t in groups[key]:
                for part in t.view.parts:
                    held |= _texels(part)
        result = plan(data, [(t.view, t.pixels, t.width) for t in active])
        failed = set(result.errors) | set(result.conflicts) | set(result.widened)
        for t in active:
            if t.view.key not in failed and any(a in held for p in t.view.parts for a in _texels(p)
                                                if a in result.writes):
                failed.add(t.view.key)
        newly = {t.group for t in active if t.view.key in failed}
        if not newly:
            break
        repack |= newly
    else:
        raise ArtError("%s: in-place plan did not settle" % profile.container)
    for key, group in groups.items():
        if key not in repack:
            for t in group:
                out.placed[t.image_id] = "in_place"
    out.ambiguous = result.ambiguous
    for address, index in result.writes.items():
        write_index(out.data, address, index)
        out.touched.add(address[0])
    out.texels += len(result.writes)
    new_views: dict[str, View] = {}
    if repack and profile.encode is None:
        for key in repack:
            for t in groups[key]:
                out.deferred[t.image_id] = "texels shared with other frames or outside the sprite; the " \
                                           "container's part records cannot be rewritten"
    elif repack:
        _repack(data, profile, groups, repack, protect_views, out, new_views, group_of)
    _verify(data, out, profile, targets, protect_views, new_views, group_of)
    return out


def _repack(data, profile, groups, repack, protect_views, out: Result, new_views: dict, group_of) -> None:
    prepared, reasons = [], {}
    records = profile.record_bytes(data)
    palettes, seen = [], set()
    for view in protect_views:
        for part in view.parts:
            if part.record is not None and (part.palette, part.clut) not in seen:
                seen.add((part.palette, part.clut))
                palettes.append(part)
    owners: dict = {}       # texel -> (group, part number) of every part that draws it
    for view in list(protect_views) + [t.view for g in groups.values() for t in g]:
        for number, part in enumerate(view.parts):
            for address in _texels(part):
                owners.setdefault(address, set()).add((group_of(view), number))

    def exclusive_to(key):
        def check(number):
            part = groups[key][0].view.parts[number]
            return all(owners.get(a, set()) <= {(key, number)} for a in _texels(part))
        return check
    for key in sorted(repack):
        made, why = _prepare(data, groups[key], records, palettes, exclusive_to(key),
                             getattr(profile, "prefer_rebuild", False), getattr(profile, "grow", 0))
        if made is None and len({t.view.record for t in groups[key]}) > 1:
            # frames that cannot share texels in English: each frame on its own
            subgroups = {}
            for t in groups[key]:
                subgroups.setdefault(_group_key(t.view), []).append(t)
            singles = []
            for sub_key, sub in sorted(subgroups.items()):
                for t in sub:
                    t.group = sub_key
                single, why_single = _prepare(data, sub, records, palettes,
                                             prefer_rebuild=getattr(profile, "prefer_rebuild", False),
                                             grow=getattr(profile, "grow", 0))
                if single is None:
                    singles = None
                    why = "%s; alone: %s" % (why, why_single)
                    break
                singles.append(single)
            if singles:
                prepared.extend(singles)
                continue
            for t in groups[key]:
                t.group = key
        if made is None:
            reasons[key] = why
        else:
            prepared.append(made)
    reserved = [p for v in protect_views if group_of(v) not in repack for p in v.parts]
    reserved += [p for key in reasons for t in groups[key] for p in t.view.parts]
    every_part = [p for v in protect_views for p in v.parts] if profile.direct else []
    # only pages some frame draws: a sheet no frame uses may not be in VRAM when the frame is drawn
    # (MAINROM uploads 13 sheets, but slots of the unreferenced ones are reused at run time)
    referenced = {i for i, (offset, bpp) in profile.pages.items()
                  if any(p.page == offset and p.bpp == bpp for v in protect_views for p in v.parts)}
    placed, failed, prepared = _allocate_best(out.data, prepared, profile.pages, profile.align, reserved,
                                              every_part, profile.page_sets, referenced)
    for key, why in reasons.items():
        for t in groups[key]:
            out.deferred[t.image_id] = "repack: " + why
    for made in prepared:
        if made.key in failed:
            for t in made.group:
                out.deferred[t.image_id] = "repack: " + failed[made.key]
    written: dict = {}
    pack = profile.pack or _record
    users: dict = {}        # record offset -> groups whose views draw through it (aliased frames, shared tiles)
    prepared_group = {t.view.key: made.key for made in prepared for t in made.group}
    for view in list(protect_views) + [t.view for g in groups.values() for t in g]:
        owner = prepared_group.get(view.key, group_of(view))
        if view.record is not None:
            users.setdefault(view.record, set()).add(owner)
        for part in view.parts:
            if part.record is not None:
                users.setdefault(part.record, set()).add(owner)
    for made in prepared:
        if made.key not in placed:
            continue
        parts = placed[made.key]
        records_here, views_here, grown_here = {}, {}, {}

        def put(offset, record):
            if written.get(offset, records_here.get(offset, record)) != record or users.get(offset, set()) - {made.key}:
                raise _SharedRecord(offset)
            records_here[offset] = record
        single = len(made.group) == 1
        try:
            for t in made.group:
                own = t.view.parts
                if made.grown:
                    new = []
                    if made.mode == "relocated":
                        tiles: dict = {}
                        for piece, part in zip(made.pieces, parts):
                            tiles.setdefault(piece.number, []).append(
                                part if single or piece.fixed else replace(part, palette=own[piece.number].palette,
                                                                           clut=own[piece.number].clut))
                        for n, p in enumerate(own):
                            new.extend(tiles.get(n, [p]))
                    else:
                        new = [part if single else replace(part, palette=own[piece.home].palette,
                                                           clut=own[piece.home].clut)
                               for piece, part in zip(made.pieces, parts)]
                    header = struct.unpack_from("<4h", records, t.view.record)
                    blob = struct.pack("<4h", len(new), *header[1:]) + b"".join(
                        pack(p, profile.encode(p.page_index, p.u, p.v)) for p in new)
                    if grown_here.get(t.view.frame, blob) != blob:
                        raise _SharedRecord(t.view.record)
                    grown_here[t.view.frame] = blob
                    views_here[t.view.key] = View(t.view.key, new, t.view.left, t.view.top, t.view.width,
                                                  t.view.height, t.view.frame, t.view.record)
                elif made.mode == "relocated":
                    moved = {piece.number: (part if single else replace(part, palette=own[piece.number].palette,
                                                                         clut=own[piece.number].clut))
                             for piece, part in zip(made.pieces, parts)}
                    fixed = {piece.number for piece in made.pieces if piece.fixed}
                    for number, part in moved.items():
                        if number not in fixed:
                            put(own[number].record, pack(part, profile.encode(part.page_index, part.u, part.v)))
                    new = [moved.get(n, p) for n, p in enumerate(own)]
                    views_here[t.view.key] = View(t.view.key, new, t.view.left, t.view.top, t.view.width,
                                                  t.view.height, t.view.frame, t.view.record)
                else:
                    new = [part if single else replace(part, palette=own[piece.home].palette, clut=own[piece.home].clut)
                           for piece, part in zip(made.pieces, parts)]
                    header = struct.unpack_from("<4h", records, t.view.record)
                    put(t.view.record, struct.pack("<4h", len(new), *header[1:]))
                    for i, part in enumerate(new):
                        put(t.view.record + 8 + i * 14, pack(part, profile.encode(part.page_index, part.u, part.v)))
                    views_here[t.view.key] = View.of(t.view.key, new, frame=t.view.frame, record=t.view.record)
        except _SharedRecord as shared:
            for t in made.group:
                out.deferred[t.image_id] = "repack: the record at 0x%X is used at two places that need different " \
                                           "English" % shared.args[0]
            continue
        for piece, part in zip(made.pieces, parts):
            for ly in range(part.height):
                for lx in range(part.width):
                    address = part.texel(lx, ly)
                    write_index(out.data, address, piece.indices[ly * part.width + lx])
                    out.touched.add(address[0])
            out.texels += part.width * part.height
        out.new_parts += len(parts)
        written.update(records_here)
        new_views.update(views_here)
        out.frames.update(grown_here)
        for t in made.group:
            out.placed[t.image_id] = made.mode
    out.records.update(written)


class _SharedRecord(Exception):
    pass


def _prepare(data, group, records, palettes, exclusive=lambda number: False, prefer_rebuild=False, grow=0):
    """The cheapest way to draw the group (relocate, rebuild, or with `grow` relocate in tiles under
    a new longer frame record), with the others chained as alternatives; or (None, why)."""
    relocated, why = _relocate(data, group, palettes, exclusive)
    tiled = _relocate(data, group, palettes, exclusive, grow)[0] if grow else None
    rebuilt, why_rebuild = _rebuild(data, group, _record_count(records, group[0].view), palettes)
    expanded = _rebuild(data, group, 128, palettes)[0] if grow else None
    if expanded is not None:
        expanded.grown = True
    # a relocated frame keeps its unchanged parts' texels; a rebuilt one frees them all
    options = sorted((m for m in (relocated, tiled, rebuilt, expanded) if m is not None),
                     key=lambda m: (not (prefer_rebuild and m.mode == "rebuilt"), m.grown,
                                    m.area + sum(p.width * p.height for p in m.kept), m.mode))
    if not options:
        return None, "%s; %s" % (why, why_rebuild)
    for first, second in zip(options, options[1:]):
        first.alternative = second
    return options[0], None


def _record_count(records, view: View) -> int:
    if view.record is None:
        return 0
    return struct.unpack_from("<h", records, view.record)[0]


def _record(part: Part, texture: int) -> bytes:
    """The 14-byte part record: x, y, texture, palette, flip, width, height."""
    return struct.pack("<2h3H2h", part.x, part.y, texture, part.clut, part.flip, part.width, part.height)


def _verify(original, out: Result, profile, targets, protect_views, new_views, group_of) -> None:
    """Every placed target draws its image; every other view draws what it drew before."""
    deferred_keys = {t.view.key for t in targets if t.image_id in out.deferred}
    for t in targets:
        if t.image_id in out.deferred:
            continue
        view = new_views.get(t.view.key, t.view)
        got = compose(out.data, _shift(view, t.view), t.width)
        want = [TRANSPARENT if p[3] == 0 else p for p in t.pixels]
        if got != want:
            bad = sum(1 for a, b in zip(got, want) if a != b)
            raise ArtError("%s %s: %d pixels differ after writing" % (profile.container, t.view.key, bad))
    target_keys = {group_of(t.view) for t in targets}
    for view in protect_views:
        if group_of(view) in target_keys and view.key not in deferred_keys:
            continue
        if compose(out.data, view) != compose(original, view):
            raise ArtError("%s %s: a view without English art changed" % (profile.container, view.key))


def _shift(view: View, original: View) -> View:
    """A rebuilt view keeps the original view's top-left as the image origin."""
    return View(view.key, view.parts, original.left, original.top, original.width, original.height,
                view.frame, view.record)
