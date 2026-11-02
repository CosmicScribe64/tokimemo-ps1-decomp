"""The English art step of the disc build (T-39508, wiki/english-build.md).

`plan_art` places every registered English image (tools/patch/art/items.py) into its container:
the indexed sprite containers through tools/patch/art/sprites.py, the BS stills through
tools/patch/art/stills.py. It returns the new container bytes, the MAINROM part records that live
in the executable, and one report entry per artwork item of the inventory:

  placed                    every image the game draws is on the disc
  partly_placed             some images placed, the others deferred (reasons per image)
  deferred                  nothing placed; the reasons say why
  untranslated_by_decision  the inventory keeps the original on purpose (deliberately-untranslated)
  missing                   the inventory has no English art for it

build_english.py puts the containers on the disc with the text data files (cd_layout checks, a
grown still moves and is remapped) and calls `patch_executable` on the built English executable
before the executable goes on the disc. Run in Docker (Pillow and numpy, tools/Dockerfile.patch).
"""
from __future__ import annotations

import hashlib
import json
import os
import pickle
import struct
import time
from collections import Counter
from pathlib import Path

from tools.assets import mdec_splice

from tools.patch.art import ArtBuildError
from tools.patch.art import items as art_items
from tools.patch.art.items import Item
from tools.patch.art import sprites, stills, prologue, skating, festival, sports_callouts

UNSUPPORTED = {
    "CDROM/OPENXA.STR": "handled by the separate movie build step; final disc verification pending",
}
NOT_DRAWN = "not_drawn"
STILL_CACHE = Path(".asset-cache/art/stills")   # gitignored; holds re-encoded game data


def plan_art(root: Path, files_root: Path, exe: bytes) -> dict:
    """Place every English image; returns {"files": {path: bytes}, "exe_records": {...}, "report": {...}}."""
    started = time.monotonic()
    items = art_items.load(root)
    items.append(skating.item(root, files_root))
    items.extend(sports_callouts.items(root, files_root))
    items.extend(festival.items(root, files_root))
    fallbacks = art_items.fallbacks(root)
    by_container = art_items.by_container(items)
    files, exe_records, images, reasons, containers = {}, {}, {}, {}, {}
    exe_frames = {}
    # a still's crops name their own file (P_01_03B is listed under the P_01_03 item)
    stills_by_file: dict = {}
    for path, group in by_container.items():
        if path.endswith(".BS"):
            for item in group:
                for image in item.images:
                    stills_by_file.setdefault(image.meta.get("source_container", path), []).append(image)
    by_container = {p: g for p, g in by_container.items() if not p.endswith(".BS")}
    for path, images_of_file in stills_by_file.items():
        by_container[path] = [Item(path, "candidate-only", path, "group", None, images_of_file)]
    results = run_jobs(root, files_root, exe, by_container, fallbacks)
    fallback_used = []
    for path, group in sorted(by_container.items()):
        if path not in results:
            why = UNSUPPORTED.get(path, "no writer for this container")
            for item in group:
                for image in item.images:
                    images[image.id], reasons[image.id] = "deferred", why
            continue
        data = (files_root / path).read_bytes()
        result = results[path]
        if path.endswith(".BS"):
            stats = {key: result.facts.get(key) for key in ("edited_macroblocks", "changed_pixels",
                                                              "english_max_abs_rgb555", "english_mean_abs_rgb555",
                                                              "ring_max_abs_rgb555", "ring_tolerance", "lambda", "ring_weight",
                                                              "file_bytes")}
        else:
            fallback_used += result.fallback
            exe_records.update(result.exe_records)
            exe_frames.update(result.exe_frames)
            stats = dict(result.stats, fallback=result.fallback)
        images.update(result.images)
        reasons.update(result.reasons)
        if result.data != data:
            files[path] = result.data
        containers[path] = dict(stats, sha256=hashlib.sha256(result.data).hexdigest(),
                                original_sha256=hashlib.sha256(data).hexdigest(),
                                bytes=len(data), new_bytes=len(result.data))
    report = item_report(items, images, reasons, set(fallback_used))
    original_banner = (files_root / prologue.PATH).read_bytes()
    files[prologue.PATH], banner_facts = prologue.place(original_banner)
    containers[prologue.PATH] = dict(banner_facts, bytes=len(original_banner), new_bytes=len(files[prologue.PATH]))
    report["items"].append({"id": prologue.IMAGE_ID, "container": prologue.PATH, "status": "placed",
                            "images": {prologue.IMAGE_ID: "generated"}, "generator": "tools/patch/art/prologue.py"})
    report["totals"]["placed"] = report["totals"].get("placed", 0) + 1
    report["images"]["generated"] = 1
    report["containers"] = containers
    report["exe_records"] = len(exe_records)
    report["seconds"] = round(time.monotonic() - started, 1)
    sprite_results = {p: r for p, r in results.items() if not p.endswith(".BS")}
    still_crops = {p: [(image.id, image.path, image.meta) for item in by_container[p] for image in item.images]
                   for p in results if p.endswith(".BS")}
    report["exe_frames"] = len(exe_frames)
    return {"files": files, "exe_records": exe_records, "exe_frames": exe_frames, "report": report,
            "sprites": sprite_results,
            "stills": still_crops}


def _sprite_job(args):
    path, data_path, exe, group, fallbacks = args
    return sprites.place_container(path, Path(data_path).read_bytes(), exe, group, fallbacks)


def _still_job(args):
    path, data_path, crops = args
    return stills.place_still(path, Path(data_path).read_bytes(), [(i, Path(p), m) for i, p, m in crops])


def _process_pool(workers: int):
    """A ProcessPoolExecutor, imported with the standard library's own `queue`.

    tools/patch/disc.py puts tools/ on sys.path, where tools/queue.py would shadow the `queue`
    module that concurrent.futures.process needs.
    """
    import sys
    tools = str(Path(__file__).resolve().parents[2])
    saved_path, saved_queue = list(sys.path), sys.modules.get("queue")
    if saved_queue is not None and str(getattr(saved_queue, "__file__", "")).startswith(tools):
        del sys.modules["queue"]
    sys.path[:] = [p for p in sys.path if str(Path(p or ".").resolve()) != tools]
    try:
        from concurrent.futures.process import ProcessPoolExecutor
    finally:
        sys.path[:] = saved_path
    return ProcessPoolExecutor(workers)


def run_jobs(root: Path, files_root: Path, exe: bytes, by_container: dict, fallbacks: dict) -> dict:
    """Every container in parallel processes; stills cached under .asset-cache/art/stills.

    A still's cache key is its source file, every crop and the code that encodes it (stills.py,
    mdec_splice.py), so a cached still is the bytes this build would compute.
    """
    cache = root / STILL_CACHE
    code = b"".join(Path(module.__file__).read_bytes() for module in (stills, mdec_splice))
    results, jobs = {}, []
    for path, group in sorted(by_container.items()):
        data_path = files_root / path
        if path in sprites.PROFILES:
            jobs.append((path, None, _sprite_job, (path, str(data_path), exe, group, fallbacks)))
        elif path.endswith(".BS"):
            crops = [(image.id, str(image.path), image.meta) for item in group for image in item.images]
            key = hashlib.sha256(code + data_path.read_bytes() + json.dumps(
                [(i, hashlib.sha256(Path(p).read_bytes()).hexdigest(), m.get("crop"), m.get("geometry"))
                 for i, p, m in crops], sort_keys=True).encode()).hexdigest()
            stored = cache / (key + ".pickle")
            if stored.is_file():
                results[path] = pickle.loads(stored.read_bytes())
            else:
                jobs.append((path, stored, _still_job, (path, str(data_path), crops)))
    # stills first: they take longest
    jobs.sort(key=lambda job: (job[1] is None, job[0]))
    if jobs:
        with _process_pool(max(1, min(len(jobs), os.cpu_count() or 1))) as pool:
            futures = [(path, stored, pool.submit(function, args)) for path, stored, function, args in jobs]
            for path, stored, future in futures:
                results[path] = future.result()
                if stored is not None:
                    stored.parent.mkdir(parents=True, exist_ok=True)
                    stored.write_bytes(pickle.dumps(results[path]))
    return results


def item_report(items: list, images: dict, reasons: dict, fallback: set) -> dict:
    """One entry per artwork item, and the totals."""
    entries = []
    for item in items:
        entry = {"id": item.id, "container": item.container}
        if item.status == "deliberately-untranslated":
            entry["status"] = "untranslated_by_decision"
        elif not item.images:
            entry["status"] = "missing"
        else:
            states = {image.id: images.get(image.id, "deferred") for image in item.images}
            drawn = {k: v for k, v in states.items() if v != NOT_DRAWN}
            placed = [k for k, v in drawn.items() if v != "deferred"]
            deferred = [k for k, v in drawn.items() if v == "deferred"]
            entry["status"] = ("placed" if not deferred else "partly_placed" if placed else "deferred")
            entry["images"] = {k: v + (" (fallback)" if k in fallback else "") for k, v in sorted(states.items())}
            if deferred:
                entry["reasons"] = {k: reasons.get(k, "not placed") for k in deferred}
        if item.reason and entry["status"] in ("untranslated_by_decision", "missing"):
            entry["reason"] = item.reason
        entries.append(entry)
    totals = Counter(e["status"] for e in entries)
    modes = Counter(v for v in images.values())
    return {"totals": dict(totals), "images": dict(modes), "fallback_images": sorted(fallback), "items": entries}


# MAINROM frame records in the original executable (tools/assets/image_packs.py _parse), file offsets
MAINROM_RECORDS = (0x800C7420 - 0x80041000 + 0x800, 0x800C95E0 - 0x80041000 + 0x800)


# MAINROM frame pointer table (sprite_frame_table, 174 words) in the original executable
FRAME_TABLE = (0x800C975C - 0x80041000 + 0x800, 174)
HUD_FRAMES_MAGIC = b"TKM-EN-HUDFRAMES"     # src/english/hud_frames.c
HUD_FRAMES_SIZE = 0x800


def _exe_address(exe: bytes, offset: int) -> int:
    """RAM address of a file offset of a PS-X EXE (t_addr at 0x18, text at 0x800)."""
    return struct.unpack_from("<I", exe, 0x18)[0] + offset - 0x800


def locate(english: bytes, original: bytes) -> dict:
    """File offsets in the English executable of the MAINROM frame records, the frame pointer table
    and english_hud_frames. The records hold no pointers, so they are found by their original
    bytes; the table is found as the original pointers moved by the same distance."""
    start, end = MAINROM_RECORDS
    block = original[start:end]
    found = english.find(block)
    if found < 0 or english.find(block, found + 1) >= 0:
        raise ArtBuildError("MAINROM frame records are not exactly once in the English executable")
    moved = _exe_address(english, found) - _exe_address(original, start)
    table, count = FRAME_TABLE
    pointers = struct.unpack_from("<%dI" % count, original, table)
    want = struct.pack("<%dI" % count, *(p + moved for p in pointers))
    table_at = english.find(want)
    if table_at < 0 or english.find(want, table_at + 1) >= 0:
        raise ArtBuildError("the MAINROM frame pointer table is not exactly once in the English executable")
    hud = english.find(HUD_FRAMES_MAGIC)
    if hud >= 0 and english.find(HUD_FRAMES_MAGIC, hud + 1) >= 0:
        raise ArtBuildError("english_hud_frames is not unique in the English executable")
    return {"records": found, "delta": found - start, "table": table_at, "hud_frames": hud}


def grown_layout(frames: dict) -> dict:
    """frame index -> offset inside english_hud_frames of its new record (after the magic)."""
    out, cursor = {}, len(HUD_FRAMES_MAGIC)
    for frame, blob in sorted(frames.items()):
        out[frame] = cursor
        cursor = (cursor + len(blob) + 3) & ~3
    if cursor > HUD_FRAMES_SIZE:
        raise ArtBuildError("grown MAINROM frames need %d bytes, english_hud_frames has %d" % (cursor, HUD_FRAMES_SIZE))
    return out


def patch_executable(english: bytes, original: bytes, records: dict, frames: dict | None = None):
    """Write the MAINROM part records and grown frames into the English executable.

    records: original-executable offset -> 14-byte part record (each must still hold its original
    bytes there). frames: frame index -> new frame record, written into english_hud_frames, with
    the frame's pointer table entry set to it. Returns (executable, where) with the offsets of
    `locate`, or (english, None) when there is nothing to write.
    """
    frames = frames or {}
    if not records and not frames:
        return english, None
    where = locate(english, original)
    start, end = MAINROM_RECORDS
    out = bytearray(english)
    for offset, record in sorted(records.items()):
        if not start <= offset < end:
            raise ArtBuildError("record at 0x%X is outside the MAINROM frame block" % offset)
        at = offset + where["delta"]
        if out[at:at + len(record)] != original[offset:offset + len(record)]:
            raise ArtBuildError("record at 0x%X differs from the original in the English executable" % offset)
        out[at:at + len(record)] = record
    if frames:
        if where["hud_frames"] < 0:
            raise ArtBuildError("english_hud_frames (src/english/hud_frames.c) is not in the English executable")
        layout = grown_layout(frames)
        for frame, blob in frames.items():
            at = where["hud_frames"] + layout[frame]
            if _exe_address(english, at) & 3:
                raise ArtBuildError("english_hud_frames is not word aligned")
            out[at:at + len(blob)] = blob
            struct.pack_into("<I", out, where["table"] + 4 * frame, _exe_address(english, at))
    return bytes(out), where


def main(argv=None) -> int:
    """Plan the art alone and print the report summary (no disc is written)."""
    import argparse
    import sys
    parser = argparse.ArgumentParser(description="Plan the English art (T-39508) and print its report.")
    parser.add_argument("--disc", default="disc/files", help="original files")
    parser.add_argument("--report", help="write the report JSON here")
    args = parser.parse_args(argv)
    root = Path(__file__).resolve().parents[3]
    files_root = root / args.disc
    planned = plan_art(root, files_root, (files_root / "SLPM_86.053").read_bytes())
    report = planned["report"]
    if args.report:
        Path(args.report).write_text(json.dumps(report, indent=1, sort_keys=True) + "\n")
    print("items: " + ", ".join("%s %d" % kv for kv in sorted(report["totals"].items())))
    print("images: " + ", ".join("%s %d" % kv for kv in sorted(report["images"].items())))
    print("containers changed: %d, MAINROM records: %d, %.1f s" % (len(planned["files"]), report["exe_records"],
                                                                   report["seconds"]))
    for entry in report["items"]:
        for image_id, why in sorted(entry.get("reasons", {}).items()):
            print("  deferred %s %s: %s" % (entry["id"], image_id, why), file=sys.stdout)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
