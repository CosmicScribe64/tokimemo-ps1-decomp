"""Verify the English art on the built disc, item by item (T-39508, wiki/english-build.md).

Every container the art step wrote is read back from the built track 1 (tools/patch/disc.py) and
decoded again with the same readers the art was drawn on (tools/patch/art/sheets.py profiles over
tools/assets/image_packs*.py; the MDEC model for stills). For MAINROM the part records are read
from the English executable on the disc.

Indexed containers, for every image the art step placed:
  * the view composed from the disc equals the English PNG pixel for pixel (every palette
    variant the game draws; part images through their frame);
  * no opaque pixel changes its STP bit when the palette could have kept it;
  * every view without English art composes as before, and every byte the writer did not plan
    to change is the original byte.
Stills: pixels outside the re-encoded macroblocks equal the original decode, unchanged pixels
inside them stay within the ring tolerance, and the English pixels are reported as max error
and PSNR against the composited target.

A placed image that fails is a build failure (ArtBuildError). The report lists all artwork
items: placed_verified, partly_placed (placed images verified, others deferred), deferred,
untranslated_by_decision or missing, and the route checkpoints that can show each container.
"""
from __future__ import annotations

import math
import struct
from collections import Counter
from pathlib import Path

import numpy as np

from tools.assets import mdec_splice
from tools.patch import disc as disc_mod
from tools.patch.art import ArtBuildError, sheets, sprites, stills
from tools.patch.art.build import MAINROM_RECORDS
from tools.patch.art.indexed import TRANSPARENT, View, compose, read_index
from tools.patch.art.placer import frame_image

# Where tools/emu/routes/main.route shows each container (checkpoint names of the route).
ROUTE = {
    "CDROM/FUKUTEST/TITLE.NBN": "title_menu",
    "CDROM/FUKUTEST/OPTION.NBN": "options_*",
    "CDROM/FUKUTEST/FILESEL.NBN": "file_select_*, save_*, load_*",
    "CDROM/NAME_ENT/NAME_ENT.NBN": "name_entry_*",
    "CDROM/FUKUTEST/MAINROM.NBN": "command_screen, school_day, week_1_*, save_*, holiday",
    "CDROM/BS/P_01_01.BS": "prologue_*", "CDROM/BS/P_01_02.BS": "prologue_*", "CDROM/BS/P_01_03.BS": "prologue_*",
    "CDROM/BS/P_01_03B.BS": "prologue_*",
}


def read_back(track1: Path, paths) -> dict:
    track = disc_mod.Track(str(track1))
    try:
        iso = disc_mod.Iso(track)
        return {path: iso.read(path) for path in paths}
    finally:
        track.close()


def _same(a, b) -> bool:
    return [TRANSPARENT if p[3] == 0 else p for p in a] == [TRANSPARENT if p[3] == 0 else p for p in b]


def _words(data, view: View, width: int) -> list:
    words = [0] * (width * view.height)
    for part in reversed(view.parts):
        for ly in range(part.height):
            for lx in range(part.width):
                word = part.palette[read_index(data, part.texel(lx, ly))]
                x = part.x - view.left + lx
                if word and 0 <= x < width:
                    words[(part.y - view.top + ly) * width + x] = word
    return words


def _stp_changes(original, view0: View, data, view1: View, width: int) -> int:
    """Opaque pixels whose STP bit differs from the original pixel's though the palette has both."""
    before, after = _words(original, view0, width), _words(data, view1, width)
    palette = {w & 0x7FFF: set() for part in view1.parts for w in part.palette}
    for part in view1.parts:
        for w in part.palette:
            if w:
                palette[w & 0x7FFF].add(w & 0x8000)
    bad = 0
    for old, new in zip(before, after):
        if old and new and (old ^ new) & 0x8000 and (old & 0x8000) in palette.get(new & 0x7FFF, ()):
            bad += 1
    return bad


def verify_sprites(path: str, original: bytes, written: bytes, exe: bytes, exe_written: bytes, planned,
                   grown: dict) -> dict:
    """Images of one indexed container: id -> "verified" or the failure."""
    original_file = original
    if path.endswith("NAME_ENT.NBN"):
        from tools.patch.art import name_entry
        original = name_entry.prepare(original)
    before = sprites.PROFILES[path](original, exe, path)
    after = sprites.PROFILES[path](written, exe_written, path)
    out = {}
    targets = {}
    regions = {}
    for image_id, key, part, png, region in planned.targets:
        targets.setdefault(key, []).append((image_id, part, png))
        regions[key] = region
    for key, images in targets.items():
        if regions[key]:
            page, x, y, w, h = regions[key]
            view0, view1 = sheets.page_region(before, page, (x, y, w, h)), sheets.page_region(after, page, (x, y, w, h))
        else:
            view0, view1 = before.views[key], after.views[key]
            if view0.frame in grown:
                view1 = View.of(key, sheets.mainrom_parts(grown[view0.frame], exe, key.split(".")[1]),
                                frame=view0.frame)
        frames = [i for i in images if i[1] is None]
        parts = [i for i in images if i[1] is not None]
        if frames:
            image_id, _part, png = frames[0]
            want, width, _h = sprites.pixels(png)
        else:
            locals_ = {part: sprites.pixels(png)[:2] for _i, part, png in parts}
            first = parts[0][1]
            others = {k: v for k, v in locals_.items() if k != first}
            want, width = frame_image(original, view0, first, *locals_[first], others), view0.width
        shifted = View(key, view1.parts, view0.left, view0.top, view0.width, view0.height, view1.frame, view1.record)
        got = compose(written, shifted, width)
        status = "verified"
        if not _same(got, want):
            status = "differs from the English image in %d pixels" % sum(
                1 for a, b in zip(got, want) if (TRANSPARENT if a[3] == 0 else a) != (TRANSPARENT if b[3] == 0 else b))
        elif width == view0.width and _stp_changes(original, view0, written, shifted, width):
            status = "STP bit changed on %d pixels" % _stp_changes(original, view0, written, shifted, width)
        for image_id, _part, _png in images:
            out[image_id] = status
    target_groups = {k.split(".")[0] for k in targets}
    unchanged = 0
    for key, view in before.views.items():
        if key.split(".")[0] in target_groups or key not in after.views:
            continue
        if compose(written, after.views[key]) != compose(original, view):
            raise ArtBuildError("%s %s: a view without English art changed on the disc" % (path, key))
        unchanged += 1
    stray = [i for i, (a, b) in enumerate(zip(original_file[:-4], written)) if a != b and i not in planned.touched]
    grown = len(written) != len(original_file)
    if stray or (grown and (written[-4:] != original_file[-4:] or len(written) % 2048)):
        raise ArtBuildError("%s: %d bytes changed outside the planned texels and records" % (path, len(stray)))
    return {"images": out, "other_views_unchanged": unchanged}


def verify_still(path: str, original: bytes, written: bytes, crops: list) -> dict:
    """crops: (image id, png, manifest entry). Pixel facts of the re-encoded still on the disc."""
    meta = crops[0][2]
    width, height = meta["geometry"]["width"], meta["geometry"]["height"]
    source = mdec_splice.decode_rgb5(original, width, height)
    decoded = mdec_splice.decode_rgb5(written, width, height)
    goal = stills.goal_image(source, [(m["crop"]["rect"],) + stills.crop_pixels(p, m["crop"]["rect"])
                                      for _i, p, m in crops])
    changed = (goal != source).any(-1)
    edited = np.zeros(changed.shape, bool)
    for mx in range(0, changed.shape[1], 16):
        for my in range(0, changed.shape[0], 16):
            if changed[my:my + 16, mx:mx + 16].any():
                edited[my:my + 16, mx:mx + 16] = True
    error = np.abs(decoded - goal).astype(float)
    english = error[changed]
    mse = float((english ** 2).mean()) if changed.any() else 0.0
    ring = np.abs(decoded - source)[edited & ~stills._chroma_cells(changed)]
    facts = {"changed_pixels": int(changed.sum()), "edited_macroblocks": int(edited.sum() // 256),
             "outside_identical": bool((decoded[~edited] == source[~edited]).all()),
             "ring_max_abs_rgb555": int(ring.max()) if ring.size else 0,
             "english_max_abs_rgb555": int(english.max()) if english.size else 0,
             "english_psnr_db": round(10 * math.log10(31 ** 2 / mse), 2) if mse else None}
    facts["sectors"] = {"original": -(-len(original) // 2048), "new": -(-len(written) // 2048)}
    ok = (facts["outside_identical"] and facts["ring_max_abs_rgb555"] <= stills.CODEC_RING_TOLERANCE
          and facts["sectors"]["new"] <= facts["sectors"]["original"])
    status = "verified" if ok else "re-encoded still fails its gates on the disc"
    return {"images": {i: status for i, _p, _m in crops}, "facts": facts}


def verify(root: Path, track1: Path, files_root: Path, art: dict) -> dict:
    """Check every written container on the disc; returns the per-item report (raises on failure)."""
    planned = art["plan"]
    report = art["report"]
    paths = sorted(planned["files"])
    exe_path = "SLPM_86.053"
    on_disc = read_back(track1, paths + [exe_path])
    exe = (files_root / exe_path).read_bytes()
    exe_written, grown = exe, {}
    if planned["exe_records"] or planned["exe_frames"]:
        where = report.get("mainrom_exe")
        if not where:
            raise ArtBuildError("the MAINROM records were not written")
        start, end = MAINROM_RECORDS
        english = on_disc[exe_path]
        block = english[where["records"]:where["records"] + end - start]
        exe_written = exe[:start] + block + exe[end:]
        base = struct.unpack_from("<I", english, 0x18)[0]
        for frame, blob in planned["exe_frames"].items():
            pointer = struct.unpack_from("<I", english, where["table"] + 4 * frame)[0]
            at = pointer - base + 0x800
            if english[at:at + len(blob)] != blob:
                raise ArtBuildError("MAINROM frame %d: its new record is not on the disc" % frame)
            grown[frame] = english[at:at + len(blob)]
    results, facts = {}, {}
    for path in paths:
        original = (files_root / path).read_bytes()
        written = on_disc[path]
        if written[:len(planned["files"][path])] != planned["files"][path]:
            raise ArtBuildError("%s on the disc is not the planned container" % path)
        written = planned["files"][path]
        if path.endswith(".BS"):
            checked = verify_still(path, original, written, planned["stills"][path])
            facts[path] = checked["facts"]
        else:
            checked = verify_sprites(path, original, written, exe, exe_written, planned["sprites"][path],
                                     grown if path.endswith("MAINROM.NBN") else {})
            facts[path] = {"other_views_unchanged": checked["other_views_unchanged"]}
        results.update(checked["images"])
    failed = {k: v for k, v in results.items() if v != "verified"}
    if failed:
        raise ArtBuildError("placed art does not verify on the disc: " + "; ".join(
            "%s: %s" % kv for kv in sorted(failed.items())[:10]))
    for entry in report["items"]:
        entry["route"] = ROUTE.get(entry["container"], "not on the route")
        if entry["container"].endswith("MAINROM.NBN") and entry["id"] >= "mainrom-0118":
            entry["route"] = "not on the route"     # the magazine (information) screen
        if "images" not in entry:
            continue
        placed = [k for k, v in entry["images"].items() if not v.startswith(("deferred", "not_drawn"))]
        missing = [k for k in placed if results.get(k) != "verified"]
        if missing:
            raise ArtBuildError("%s: placed images %s were not checked on the disc" % (entry["id"], missing))
        if entry["status"] == "placed":
            entry["status"] = "placed_verified"
    report["totals"] = dict(Counter(e["status"] for e in report["items"]))
    report["verified_images"] = len(results)
    report["verify_facts"] = facts
    report["not_on_route"] = sorted(e["id"] for e in report["items"]
                                    if e["status"] in ("placed_verified", "partly_placed")
                                    and e["route"] == "not on the route")
    return report

