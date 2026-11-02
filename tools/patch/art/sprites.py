"""English images into the indexed sprite containers (T-39508, wiki/english-build.md).

For every container with a profile in tools/patch/art/sheets.py: each registered English image
is mapped to its view (frame and palette variant, one part of a frame, or a whole page), the
images the game never draws are left out (a palette variant whose manifest says it is not used
at runtime), and tools/patch/art/placer.py writes the rest. The new container is parsed again with
the same profile and every view composed: targets must draw their images, other views must not
change (the placer's own check, repeated here on the re-read records).
"""
from __future__ import annotations

import re
import struct
from dataclasses import dataclass, field
from pathlib import Path

from PIL import Image as PILImage

from tools.patch.art import sheets
from tools.patch.art.indexed import ArtError, TRANSPARENT, compose
from tools.patch.art.placer import Target, place

EXE_PATH = "SLPM_86.053"
PROFILES = {
    "CDROM/FUKUTEST/MAINROM.NBN": lambda data, exe, path: sheets.mainrom_profile(data, exe),
    "CDROM/FUKUTEST/OPTION.NBN": lambda data, exe, path: sheets.option_profile(data),
    "CDROM/FUKUTEST/TITLE.NBN": lambda data, exe, path: sheets.title_profile(data),
    "CDROM/FUKUTEST/FILESEL.NBN": lambda data, exe, path: sheets.filesel_profile(data),
    "CDROM/NAME_ENT/NAME_ENT.NBN": lambda data, exe, path: sheets.name_profile(data),
    "CDROM/FUKUTEST/EXAMINE.NBN": lambda data, exe, path: sheets.exam_profile(data, exe),
}
for _name in ("VS_HABU", "VS_SIKA", "VS_KUMA", "VS_UFO", "VS_ROBO"):
    PROFILES["CDROM/BATTLE/%s.BIN" % _name] = lambda data, exe, path: sheets.battle_profile(data, path)
PROFILES["CDROM/LASTPHOT/ALL_C.NBN"] = lambda data, exe, path: sheets.allc_profile(data, path)
for _name in ("KASETU", "SPSETU", "TASETU", "NINSETU"):
    PROFILES["CDROM/TAIIKU/%s.BIN" % _name] = lambda data, exe, path: sheets.sports_profile(data, path)


@dataclass
class ContainerResult:
    path: str
    data: bytes
    exe_records: dict = field(default_factory=dict)   # executable file offset -> bytes
    images: dict = field(default_factory=dict)        # image id -> status string
    reasons: dict = field(default_factory=dict)       # image id -> reason
    stats: dict = field(default_factory=dict)
    fallback: list = field(default_factory=list)      # image ids shipped as their same-size fallback
    touched: set = field(default_factory=set)         # container byte offsets the writer may change
    exe_frames: dict = field(default_factory=dict)    # frame index -> new frame record (MAINROM)
    targets: list = field(default_factory=list)       # (image id, view key, part, png path) placed


def pixels(path: Path) -> tuple[list, int, int]:
    with PILImage.open(path) as image:
        rgba = image.convert("RGBA")
        return list(rgba.getdata()), rgba.width, rgba.height


def drawn_at_runtime(meta: dict) -> bool:
    """False for a palette variant the manifest says the game never uses."""
    scope = meta.get("runtime_variant_scope") or {}
    if scope.get("runtime_applicable") is False:
        return False
    if "uses" in scope and not scope["uses"]:
        return False
    return True


def mainrom_variant_drawn(path: str, image, item) -> bool:
    """MAINROM palette overrides: the HUD uses hud-43/44 only on frames 0x44-0x58 and icon-51/52/53
    only on frame 2 (the rule the MAINROM manifests record as runtime_variant_scope)."""
    if not path.endswith("MAINROM.NBN") or "." not in image.id:
        return True
    variant = image.meta.get("palette_id") or image.id.rsplit(".", 1)[1]
    frame = image.meta.get("frame", item.frame)
    if variant.startswith("hud-"):
        return frame is not None and 0x44 <= frame <= 0x58
    if variant.startswith("icon-"):
        return frame == 2
    return True


def view_for(profile, item, image):
    """(view, part number or None) of an English image, or (None, reason)."""
    meta = image.meta
    if "/BATTLE/" in profile.container:
        return sheets.page_region(profile, 0, meta["region"]), None
    if profile.container.endswith("ALL_C.NBN"):
        page = int(re.search(r"source-page-(\d)", image.id + str(meta.get("png", ""))).group(1))
        width = 64 if page == 5 else 128
        return sheets.page_region(profile, page, (0, sheets.ALLC_BANNER[0], width, sheets.ALLC_BANNER[1])), None
    if "/TAIIKU/" in profile.container:
        return profile.views.get("card-%d" % meta.get("frame", 0)), None
    if profile.container.endswith("FILESEL.NBN"):
        page = re.match(r"filesel-page-(\w+)-(\d)$", image.id)
        if page:
            return profile.views.get("page-%s-%s" % page.groups()), None
        frame = meta.get("frame", item.frame)
        return profile.frame(frame, image.id.rsplit(".", 1)[1]), None
    frame = meta.get("frame", item.frame)
    if frame is None:
        return None, "no source frame (new sprite)"
    suffix = image.id.rsplit(".", 1)[-1] if "." in image.id else ""
    variant = meta.get("palette_id") or (suffix if profile.frame(frame, suffix) is not None else "base")
    view = profile.frame(frame, variant)
    if view is None:
        return None, "frame %s variant %s is not in the container" % (frame, variant)
    return view, meta.get("part_index")


# Sprites shipped at their source width (fallback-v1) by decision: the name-entry selector cells
# sit at fixed positions in name_entry.c and the 4bpp page has no room for ten 56-texel cells next
# to the panels; the exam suffix is drawn right after the score digits with an 8-texel advance.
FALLBACK_FIRST = re.compile(r"name-label-\d{4}$|name-label-abc\.|exam-0001$")


def place_container(path: str, data: bytes, exe: bytes, items: list, fallbacks: dict | None = None) -> ContainerResult:
    """Place every English image of one container; see the module docstring."""
    fallbacks = fallbacks or {}
    chosen = {}
    for item in items:
        for image in item.images:
            chosen[image.id] = fallbacks[image.id] if FALLBACK_FIRST.match(image.id) and image.id in fallbacks else image
    result = _place_once(path, data, exe, items, chosen)
    retry = [i for i, status in result.images.items()
             if status == "deferred" and i in fallbacks and chosen[i] is not fallbacks[i]]
    if retry:
        for image_id in retry:
            chosen[image_id] = fallbacks[image_id]
        result = _place_once(path, data, exe, items, chosen)
    result.fallback = sorted(i for i, image in chosen.items() if image is fallbacks.get(i))
    return result


def _place_once(path: str, data: bytes, exe: bytes, items: list, chosen: dict) -> ContainerResult:
    profile = PROFILES[path](data, exe, path)
    result = ContainerResult(path, data)
    targets = []
    for item in items:
        for original in item.images:
            image = chosen[original.id]
            if not drawn_at_runtime(image.meta) or not mainrom_variant_drawn(path, image, item):
                result.images[image.id] = "not_drawn"
                result.reasons[image.id] = "palette variant the game does not use for this frame"
                continue
            view, part = view_for(profile, item, image)
            if view is None:
                result.images[image.id] = "deferred"
                result.reasons[image.id] = part or "no view"
                continue
            colours, width, _height = pixels(image.path)
            targets.append(Target(image.id, item.id, view, colours, width, part))
            region = view.parts[0] if view.key.startswith("region-") else None
            result.targets.append((image.id, view.key, part, str(image.path),
                                   (region.page_index, region.x, region.y, region.width, region.height)
                                   if region else None))
    protect = [v for v in profile.views.values() if v.key.endswith(".base") or v.key.startswith(("page-", "card-"))]
    if "/BATTLE/" in path or path.endswith("ALL_C.NBN"):
        protect = []    # the frames that draw the command labels are the point of the regions
    placed = place(data, profile, targets, protect)
    for target in targets:
        for image_id in target.image_id.split("+"):
            if image_id in placed.deferred or target.image_id in placed.deferred:
                result.images[image_id] = "deferred"
                result.reasons[image_id] = placed.deferred.get(image_id) or placed.deferred.get(target.image_id)
    for merged, mode in placed.placed.items():
        for image_id in merged.split("+"):
            result.images[image_id] = mode
    for merged, why in placed.deferred.items():
        for image_id in merged.split("+"):
            result.images[image_id] = "deferred"
            result.reasons[image_id] = why
    exe_records = {}
    container = bytearray(placed.data)
    for offset, record in placed.records.items():
        if profile.exe is not None:
            exe_records[offset] = record
        else:
            container[offset:offset + len(record)] = record
    result.touched = set(placed.touched)
    if profile.exe is None and placed.frames:
        container = _append_frames(container, profile, placed.frames, result.touched)
    result.data = bytes(container)
    result.exe_records = exe_records
    result.exe_frames = dict(placed.frames) if profile.exe is not None else {}
    placed_ids = {i for merged in placed.placed for i in merged.split("+")}
    result.targets = [t for t in result.targets if t[0] in placed_ids]
    if profile.exe is None:
        for offset, record in placed.records.items():
            result.touched.update(range(offset, offset + len(record)))
    result.stats = {"texels_written": placed.texels, "new_part_records": placed.new_parts,
                    "ambiguous_texels": placed.ambiguous,
                    "bytes_changed": sum(a != b for a, b in zip(data, result.data))}
    _reread(path, data, exe, result, targets, placed)
    return result


def _append_frames(container: bytearray, profile, frames: dict, touched: set) -> bytearray:
    """Grown frame records appended to the container, before its last word (the 0x1012 end word a
    checked read ends on), padded to whole sectors; the frame table entries point at them."""
    if profile.frame_table is None:
        raise ArtError("%s: grown frames but no frame table" % profile.container)
    table, load = profile.frame_table
    body, tail = container[:-4], container[-4:]
    for frame, blob in sorted(frames.items()):
        body += bytes(-len(body) % 4)
        struct.pack_into("<I", body, table + 4 * frame, load + len(body))
        touched.update(range(table + 4 * frame, table + 4 * frame + 4))
        body += blob
    body += bytes(-(len(body) + 4) % 2048)
    return bytearray(body + tail)


def patched_exe(exe: bytes, records: dict) -> bytes:
    out = bytearray(exe)
    for offset, record in records.items():
        out[offset:offset + len(record)] = record
    return bytes(out)


def _reread(path, data, exe, result: ContainerResult, targets, placed) -> None:
    """Parse the written container again and compose every view from the re-read records."""
    before = PROFILES[path](data, exe, path)
    after = PROFILES[path](result.data, patched_exe(exe, result.exe_records), path)
    placed_ids = {i for merged in placed.placed for i in merged.split("+")}
    done = {t.view.key: t for t in targets if t.image_id in placed_ids}
    for key, view in before.views.items():
        new = after.views.get(key)
        if new is None:
            raise ArtError("%s %s: view lost after writing" % (path, key))
        if key in done or view.frame in placed.frames:
            continue
        group = key.split(".")[0]
        if any(k.split(".")[0] == group for k in done):
            continue   # another variant of a placed frame: not drawn by the game
        if compose(result.data, new) != compose(data, view):
            raise ArtError("%s %s: changed after writing" % (path, key))
    for key, t in done.items():
        if t.part is not None or key not in before.views or t.view.frame in placed.frames:
            continue    # merged part images and page regions are checked by the placer
        old = before.views[key]
        new = after.views[key]
        width = max(t.width, old.width)
        shifted = type(new)(key, new.parts, old.left, old.top, width, old.height, new.frame, new.record)
        got = compose(result.data, shifted)
        want = [TRANSPARENT if p[3] == 0 else p for p in t.pixels]
        if t.width == width and got != want:
            raise ArtError("%s %s: re-read image differs" % (path, key))
