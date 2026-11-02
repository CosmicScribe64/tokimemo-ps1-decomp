"""The two calls build_english.py makes for the English art (T-39508, wiki/english-build.md).

place_files: after the text data files, plan the art (tools/patch/art/build.py) and put every
changed container on the disc with the same checks as a text data file (tools/patch/cd_layout.py
plan_file: read sites, load buffers, checksum reads); a container that may not grow keeps its
original bytes and its images are reported deferred. Placed files join data["placed"], so a moved
still is in the CD remap table.

patch_exe: after `ninja english`, write the MAINROM part records into the built executable.
"""
from __future__ import annotations

from pathlib import Path
from collections import Counter

from tools.patch import cd_layout
from tools.patch.art import ArtBuildError

NOT_DRAWN = "not_drawn"     # tools/patch/art/build.py; the art modules need numpy and Pillow, so they load late


def record_movie(report: dict, movie: dict) -> None:
    """Reconcile the separately written movie only after its final-disc read-back gate."""
    if not movie.get("readback_verified"):
        raise ArtBuildError("movie art cannot be marked verified without disc read-back")
    verified = set(movie["image_ids"])
    matched = False
    for entry in report["items"]:
        if entry["container"].removeprefix("CDROM/") != movie["path"].removeprefix("CDROM/"):
            continue
        matched = True
        images = entry.get("images", {})
        if not images or set(images) - verified:
            raise ArtBuildError("movie cue table does not cover every registered movie image")
        entry["images"] = {key: "movie_verified" for key in images}
        entry["status"] = "placed_verified"
        entry["route"] = "opening movie (not on the main route)"
        entry.pop("reasons", None)
        report["verified_images"] += len(images)
        report["not_on_route"] = sorted(set(report["not_on_route"]) | {entry["id"]})
        report["verify_facts"][entry["container"]] = {"raw_sha256": movie["raw_sha256"], "readback_verified": True}
    if not matched:
        raise ArtBuildError("verified movie is absent from the art inventory")
    report["totals"] = dict(Counter(entry["status"] for entry in report["items"]))
    report["images"] = dict(Counter(state for entry in report["items"] for state in entry.get("images", {}).values()))


def place_files(disc, root: Path, files_root: Path, sites, checksum_sites, data: dict, exe_path: str) -> dict:
    """Plan and place the art containers; returns the art section of the report (and the records)."""
    from tools.patch.art import build as art_build
    text_files = {item["path"] for item in data["placed"]}
    planned = art_build.plan_art(root, files_root, (files_root / exe_path).read_bytes())
    limits = cd_layout.buffer_limits(sites)
    files_by_lba = {e.lba: (e.path, e.sectors) for e in disc.iso.files()}
    placed, kept = [], []
    report = planned["report"]
    for path, new in sorted(planned["files"].items()):
        if path in text_files:
            raise ArtBuildError("%s is written by both the text and the art step" % path)
        entry = disc.iso.find(path)
        lba, size = entry.lba, entry.size
        file_limits = limits
        if path == "CDROM/NAME_ENT/NAME_ENT.NBN":
            # name_entry_load_init loads the next resource at 0x801E0000.
            # Background decoding finishes before this file is read at 0x80190000.
            file_limits = dict(limits)
            file_limits[0x80190000] = (0x50000 // 2048, "NAME_ENT: space before the kanji data at 0x801E0000")
        plan = cd_layout.plan_file(path, lba, size, len(new), sites, file_limits, checksum_sites, files_by_lba)
        if not plan.accept:
            kept.append({"path": path, "reason": plan.reason})
            if path == "CDROM/FUKUTEST/MAINROM.NBN":
                planned["exe_records"] = {}     # the records only fit the new sheets
                planned["exe_frames"] = {}
            _defer_container(report, path, "container kept original: %s" % plan.reason)
            del planned["files"][path]
            continue
        new_lba = disc.replace(path, new)
        item = {"path": path, "lba": lba, "sectors": cd_layout.sectors_of(size), "new_lba": new_lba,
                "new_sectors": cd_layout.sectors_of(len(new)), "bytes": size, "new_bytes": len(new),
                "moved_strings": 0}
        placed.append(item)
        data["placed"].append(item)
    report["files_placed"] = placed
    report["files_kept_original"] = kept
    return {"report": report, "exe_records": planned["exe_records"], "exe_frames": planned["exe_frames"],
            "plan": planned}


def _defer_container(report: dict, path: str, why: str) -> None:
    for entry in report["items"]:
        if entry["container"] != path or "images" not in entry:
            continue
        for image_id, state in entry["images"].items():
            if not state.startswith(("deferred", NOT_DRAWN)):
                entry["images"][image_id] = "deferred"
                entry.setdefault("reasons", {})[image_id] = why
        entry["status"] = "deferred"
    report["totals"] = {}
    for entry in report["items"]:
        report["totals"][entry["status"]] = report["totals"].get(entry["status"], 0) + 1


def patch_exe(exe_out: Path, original_exe: Path, art: dict) -> int:
    """Write the MAINROM records and grown frames into the built English executable; returns the
    number of records and frames written."""
    from tools.patch.art import build as art_build
    records, frames = art["exe_records"], art["exe_frames"]
    if records or frames:
        built, where = art_build.patch_executable(exe_out.read_bytes(), original_exe.read_bytes(), records, frames)
        exe_out.write_bytes(built)
        art["report"]["mainrom_exe"] = where
    return len(records) + len(frames)


def verify(root: Path, track1: Path, files_root: Path, art: dict) -> dict:
    """Read every art container back from the built track and check it (tools/patch/art/verify.py)."""
    from tools.patch.art import verify as art_verify
    report = art_verify.verify(root, track1, files_root, art)
    art.pop("plan", None)
    return report
