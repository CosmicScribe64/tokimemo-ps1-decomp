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

from tools.patch import cd_layout
from tools.patch.art import ArtBuildError

NOT_DRAWN = "not_drawn"     # tools/patch/art/build.py; the art modules need numpy and Pillow, so they load late


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
        plan = cd_layout.plan_file(path, lba, size, len(new), sites, limits, checksum_sites, files_by_lba)
        if not plan.accept:
            kept.append({"path": path, "reason": plan.reason})
            if path == "CDROM/FUKUTEST/MAINROM.NBN":
                planned["exe_records"] = {}     # the records only fit the new sheets
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
    return {"report": report, "exe_records": planned["exe_records"], "plan": planned}


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
    """Write the MAINROM records into the built English executable; returns how many."""
    from tools.patch.art import build as art_build
    records = art["exe_records"]
    if records:
        built, offset = art_build.patch_executable(exe_out.read_bytes(), original_exe.read_bytes(), records)
        exe_out.write_bytes(built)
        art["report"]["mainrom_block_offset"] = offset
    return len(records)


def verify(root: Path, track1: Path, files_root: Path, art: dict) -> dict:
    """Read every art container back from the built track and check it (tools/patch/art/verify.py)."""
    from tools.patch.art import verify as art_verify
    report = art_verify.verify(root, track1, files_root, art)
    art.pop("plan", None)
    return report
