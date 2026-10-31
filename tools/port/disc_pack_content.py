#!/usr/bin/env python3
"""T-39303: export the installed disc pack, the original files the game reads.

The pack is built from the user's raw MODE2/2352 track only. Every ISO9660 file
whose sectors are all Form 1 data is stored as its whole sectors (2048 bytes
each, including the tail of the last sector past the ISO size), so a read of N
sectors at any LBA inside the file returns what the drive would. XA and STR
streams (Form 2 sectors) and references past the end of the image are listed as
excluded with the reason; their data is not in the pack and a reader must refuse
them. Nothing is guessed: a sector with a missing sync, a mode other than 2 or an
inconsistent subheader fails the export.

Pack layout (all paths relative to the pack root):
  disc_pack.json      schema tokimemo.disc-pack.v1: source, files, excluded, totals
  files/<disc path>   the stored sectors of each file (stored_size = sectors * 2048)

Each manifest file record: path, lba, sectors, size (ISO size), stored_size, mode,
form, sha256 (of the stored bytes, checked on every read) and content_sha256 (of
the first `size` bytes, equal to the file extracted to disc/files).

Run in Docker (repository mounted at /work, disc read-only):
  python3 tools/port/disc_pack_content.py --track disc/track1.bin \
      --output .asset-cache/<new dir> [--files disc/files]
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.extract_disc import Image, MAX_DEPTH, SECTOR  # noqa: E402

SCHEMA = "tokimemo.disc-pack.v1"
MANIFEST = "disc_pack.json"
DATA_SIZE = 2048
SYNC = b"\x00" + b"\xff" * 10 + b"\x00"
MAX_FILE_SECTORS = 1 << 20


class PackError(ValueError):
    pass


def _safe_name(name: str) -> str:
    if "/" in name or name in ("", ".", "..") or "\\" in name or ":" in name:
        raise PackError("unsafe file name %r" % name)
    return name


def walk(image: Image, lba: int, size: int, prefix: str, depth: int, out: list) -> None:
    """Append (path, lba, size) for every file below the directory at `lba`."""
    if depth > MAX_DEPTH:
        raise PackError("directory nesting too deep at " + prefix)
    for name, r_lba, r_size, flags in image.read_dir(lba, size):
        if name in ("\x00", "\x01"):
            continue
        name = _safe_name(name.split(";")[0])
        path = prefix + "/" + name if prefix else name
        if flags & 2:
            walk(image, r_lba, r_size, path, depth + 1, out)
        else:
            out.append((path, r_lba, r_size))


def classify(image: Image, lba: int, size: int) -> tuple[str, list[bytes] | None]:
    """Return ("data", payloads) for an all-Form-1 file, else (reason, None)."""
    count = (size + DATA_SIZE - 1) // DATA_SIZE
    if count > MAX_FILE_SECTORS:
        raise PackError("file extent of %d sectors exceeds the bound" % count)
    if lba + count > image.sectors:
        return "past the end of the track image (CD-DA reference or truncated)", None
    payloads: list[bytes] = []
    forms: set[int] = set()
    inconsistent = None
    for index in range(count):
        position = (lba + index) * SECTOR
        image.f.seek(position)
        raw = image.f.read(SECTOR)
        if len(raw) != SECTOR:
            raise PackError("short read at LBA %d" % (lba + index))
        if raw[:12] != SYNC:
            raise PackError("LBA %d has no sync pattern" % (lba + index))
        if raw[15] != 2:
            raise PackError("LBA %d is mode %d, not mode 2" % (lba + index, raw[15]))
        if raw[16:20] != raw[20:24] and inconsistent is None:
            inconsistent = lba + index
        form = 2 if raw[18] & 0x20 else 1
        forms.add(form)
        payloads.append(raw[24:24 + DATA_SIZE])
    if forms == {1} or not forms:
        if inconsistent is not None:
            # A file read as data must have a valid subheader in every sector.
            raise PackError("LBA %d has an inconsistent XA subheader" % inconsistent)
        return "data", payloads
    if forms == {2}:
        return "form 2 stream (XA audio or STR video), not readable as data", None
    return "mixed form 1 and form 2 sectors", None


def _write_new(path: Path, data: bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with open(path, "xb") as stream:
        stream.write(data)


def _file_sha(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1 << 20), b""):
            digest.update(block)
    return digest.hexdigest()


def build(track: Path, output: Path, files: Path | None = None) -> dict:
    """Write the pack under `output` (which must not exist) and return its manifest."""
    if track.is_symlink() or not track.is_file():
        raise PackError("track must be a regular file")
    if output.exists() or output.is_symlink():
        raise PackError("output already exists")
    for part in (output, *output.parents):
        if part.is_symlink():
            raise PackError("output cannot traverse symlinks")
    image = Image(str(track))
    try:
        return _build(image, track, output, files)
    finally:
        image.f.close()


def _build(image: Image, track: Path, output: Path, files: Path | None) -> dict:
    if image.sectors <= 16:
        raise PackError("track image is too short")
    pvd, _ = image.read_sector(16)
    if pvd[:6] != b"\x01CD001":
        raise PackError("no ISO9660 primary volume descriptor at LBA 16")
    volume_id = pvd[40:72].decode("latin1").strip()
    root = pvd[156:190]
    entries: list[tuple[str, int, int]] = []
    walk(image, struct.unpack("<I", root[2:6])[0], struct.unpack("<I", root[10:14])[0], "", 0, entries)
    if len({path for path, _, _ in entries}) != len(entries):
        raise PackError("duplicate path in the file tree")
    packed, excluded = [], []
    stage = output.with_name(output.name + ".partial")
    if stage.exists():
        raise PackError("stale partial output exists: " + stage.name)
    try:
        for path, lba, size in sorted(entries, key=lambda e: (e[1], e[0])):
            if size == 0:
                excluded.append({"path": path, "lba": lba, "sectors": 0, "size": 0, "reason": "empty file"})
                continue
            reason, payloads = classify(image, lba, size)
            sectors = (size + DATA_SIZE - 1) // DATA_SIZE
            if payloads is None:
                excluded.append({"path": path, "lba": lba, "sectors": sectors, "size": size, "reason": reason})
                continue
            stored = b"".join(payloads)
            _write_new(stage / "files" / path, stored)
            packed.append({"path": path, "lba": lba, "sectors": sectors, "size": size,
                           "stored_size": len(stored), "mode": 2, "form": 1,
                           "sha256": hashlib.sha256(stored).hexdigest(),
                           "content_sha256": hashlib.sha256(stored[:size]).hexdigest()})
        _check_extents(packed)
        if files is not None:
            _cross_check(packed, files)
        if not any(record["path"] == "SLPM_86.053" for record in packed):
            raise PackError("SLPM_86.053 is not in the pack")
        manifest = {
            "schema": SCHEMA, "version": 1, "sector_size": DATA_SIZE, "volume_id": volume_id,
            "source": {"track_sha256": _file_sha(track), "track_sectors": image.sectors,
                       "raw_sector_size": SECTOR},
            "files": packed, "excluded": excluded,
            "totals": {"files": len(packed), "sectors": sum(r["sectors"] for r in packed),
                       "stored_bytes": sum(r["stored_size"] for r in packed),
                       "excluded": len(excluded)},
        }
        _write_new(stage / MANIFEST, (json.dumps(manifest, sort_keys=True, ensure_ascii=False,
                                                 separators=(",", ":")) + "\n").encode("utf-8"))
        os.replace(stage, output)
    except BaseException:
        _remove_tree(stage)
        raise
    return manifest


def _check_extents(packed: list[dict]) -> None:
    """No two stored extents may share a sector: an LBA must name one file."""
    previous = None
    for record in sorted(packed, key=lambda r: r["lba"]):
        if previous is not None and record["lba"] < previous["lba"] + previous["sectors"]:
            raise PackError("extents of %s and %s overlap" % (previous["path"], record["path"]))
        previous = record


def _cross_check(packed: list[dict], files: Path) -> None:
    """Each file also extracted to `files` (tools/extract_disc.py) must match its content hash."""
    checked = 0
    for record in packed:
        other = files / record["path"]
        if not other.is_file():
            continue
        if _file_sha(other) != record["content_sha256"]:
            raise PackError("%s differs from the extracted file in %s" % (record["path"], files))
        checked += 1
    if checked == 0:
        raise PackError("no packed file exists in " + str(files))


def _remove_tree(path: Path) -> None:
    if not path.exists() or path.is_symlink():
        return
    for current, dirs, names in os.walk(path, topdown=False, followlinks=False):
        for name in names:
            Path(current, name).unlink()
        for name in dirs:
            Path(current, name).rmdir()
    path.rmdir()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--track", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--files", type=Path, help="extracted disc/files to cross-check content hashes")
    args = parser.parse_args()
    try:
        manifest = build(args.track, args.output, args.files)
    except (PackError, OSError, EOFError, struct.error) as exc:
        print("disc pack export failed: " + str(exc), file=sys.stderr)
        return 2
    totals = manifest["totals"]
    print("disc pack: %d files, %d sectors, %d bytes, %d excluded" % (
        totals["files"], totals["sectors"], totals["stored_bytes"], totals["excluded"]))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
