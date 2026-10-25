#!/usr/bin/env python3
"""Publish explicit PNG/MP4/WebM captures into an ignored media directory."""
from __future__ import annotations

import argparse
import fcntl
import hashlib
import json
import os
import re
import shutil
import stat
import tempfile
from datetime import datetime, timezone
from pathlib import Path

MAX_IMAGE = 32 * 1024 * 1024
MAX_VIDEO = 512 * 1024 * 1024
SIGNATURES = {".png": b"\x89PNG\r\n\x1a\n", ".webm": b"\x1aE\xdf\xa3"}
ID_RE = re.compile(r"^[a-f0-9]{16}$")


def inspect_capture(path: Path) -> tuple[str, int, str]:
    """Validate regular file, permitted media signature and byte limit."""
    info = path.lstat()
    if not stat.S_ISREG(info.st_mode) or path.is_symlink():
        raise ValueError(f"capture is not a regular non-symlink file: {path}")
    suffix = path.suffix.lower()
    if suffix not in (".png", ".mp4", ".webm"):
        raise ValueError(f"unsupported capture type: {path.suffix}")
    limit = MAX_IMAGE if suffix == ".png" else MAX_VIDEO
    if not 8 <= info.st_size <= limit:
        raise ValueError(f"capture size outside permitted bounds: {path}")
    with path.open("rb") as source:
        start = source.read(24)
    if suffix in SIGNATURES and not start.startswith(SIGNATURES[suffix]):
        raise ValueError(f"capture signature does not match extension: {path}")
    if suffix == ".mp4" and (len(start) < 8 or start[4:8] != b"ftyp"):
        raise ValueError(f"invalid MP4 signature: {path}")
    if suffix == ".png":
        if len(start) < 24 or start[12:16] != b"IHDR":
            raise ValueError(f"invalid PNG header: {path}")
        with path.open("rb") as source:
            header = source.read(24)
        width = int.from_bytes(header[16:20], "big")
        height = int.from_bytes(header[20:24], "big")
        if width < 1 or height < 1 or width > 16384 or height > 16384:
            raise ValueError(f"PNG dimensions outside permitted bounds: {path}")
    digest_builder = hashlib.sha256()
    with path.open("rb") as source:
        for chunk in iter(lambda: source.read(1024 * 1024), b""):
            digest_builder.update(chunk)
    digest = digest_builder.hexdigest()
    return suffix, info.st_size, digest


def load_manifest(root: Path) -> dict[str, object]:
    path = root / "manifest.json"
    if path.is_symlink():
        raise ValueError("manifest path cannot be a symlink")
    try:
        data = json.loads(path.read_text("utf-8"))
    except FileNotFoundError:
        return {"version": 1, "items": []}
    if not isinstance(data, dict) or data.get("version") != 1 or not isinstance(data.get("items"), list):
        raise ValueError("invalid progress manifest")
    return data


def _publish_locked(paths: list[Path], root: Path) -> int:
    """Atomically add explicit validated media while the cross-process lock is held."""
    root.mkdir(parents=True, exist_ok=True)
    if root.is_symlink() or not root.is_dir():
        raise ValueError("media root must be a real directory")
    manifest = load_manifest(root)
    items = {item["file"]: item for item in manifest["items"] if isinstance(item, dict) and isinstance(item.get("file"), str)}
    added = 0
    for source in paths:
        suffix, size, digest = inspect_capture(source)
        item_id = digest[:16]
        if not ID_RE.fullmatch(item_id):
            raise ValueError("generated unsafe media id")
        name = item_id + suffix
        destination = root / name
        if destination.exists() or destination.is_symlink():
            if destination.is_symlink() or not destination.is_file() or destination.stat().st_size != size or hashlib.sha256(destination.read_bytes()).hexdigest() != digest:
                raise ValueError(f"refusing to overwrite a different existing capture: {name}")
        else:
            fd, temporary = tempfile.mkstemp(prefix=".capture-", dir=root)
            try:
                with os.fdopen(fd, "wb") as target, source.open("rb") as original:
                    shutil.copyfileobj(original, target, length=1024 * 1024)
                    target.flush()
                    os.fsync(target.fileno())
                try:
                    os.link(temporary, destination)
                except FileExistsError:
                    if destination.is_symlink() or not destination.is_file() or destination.stat().st_size != size or hashlib.sha256(destination.read_bytes()).hexdigest() != digest:
                        raise ValueError(f"refusing to overwrite a different existing capture: {name}")
            finally:
                if os.path.exists(temporary):
                    os.unlink(temporary)
        if name not in items:
            items[name] = {"id": item_id, "file": name, "sha256": digest, "bytes": size,
                           "created": datetime.now(timezone.utc).isoformat()}
            added += 1
    manifest_data = {"version": 1, "items": sorted(items.values(), key=lambda item: str(item.get("created", "")), reverse=True)}
    fd, temporary = tempfile.mkstemp(prefix=".manifest-", dir=root)
    try:
        with os.fdopen(fd, "w", encoding="utf-8") as target:
            json.dump(manifest_data, target, separators=(",", ":"))
            target.write("\n")
            target.flush()
            os.fsync(target.fileno())
        os.replace(temporary, root / "manifest.json")
    finally:
        if os.path.exists(temporary):
            os.unlink(temporary)
    return added


def publish(paths: list[Path], root: Path) -> int:
    """Serialize publishers so concurrent writers cannot lose manifest entries."""
    root.mkdir(parents=True, exist_ok=True)
    if root.is_symlink() or not root.is_dir():
        raise ValueError("media root must be a real directory")
    lock_path = root / ".publish.lock"
    fd = os.open(lock_path, os.O_CREAT | os.O_RDWR | getattr(os, "O_NOFOLLOW", 0), 0o600)
    with os.fdopen(fd, "a+b") as lock_file:
        fcntl.flock(lock_file.fileno(), fcntl.LOCK_EX)
        try:
            return _publish_locked(paths, root)
        finally:
            fcntl.flock(lock_file.fileno(), fcntl.LOCK_UN)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--media-root", default="/media")
    parser.add_argument("captures", nargs="+", type=Path)
    args = parser.parse_args()
    print(f"Published {publish(args.captures, Path(args.media_root))} new capture(s)")


if __name__ == "__main__":
    main()
