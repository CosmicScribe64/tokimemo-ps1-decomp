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
import struct
import tempfile
import zlib
from datetime import datetime, timezone
from pathlib import Path

import admission

MAX_IMAGE = 32 * 1024 * 1024
MAX_VIDEO = 512 * 1024 * 1024
MIN_VIDEO = 4096
SIGNATURES = {".png": b"\x89PNG\r\n\x1a\n", ".webm": b"\x1aE\xdf\xa3"}
ID_RE = re.compile(r"^[a-f0-9]{16}$")
MAX_BLACK_RATIO = 0.15
MAX_BORDER_BLACK_RATIO = 0.85
MAX_FALLBACK_BG_RATIO = 0.30
MIN_RGB444_BUCKETS = 20


def validate_png_visual_quality(path: Path, width: int, height: int) -> None:
    """Reject mostly-black, black-letterboxed, or unrendered fallback PNG captures."""
    if width < 320 or height < 240:
        return
    data = path.read_bytes()
    pos = 8
    bit_depth = color_type = interlace = 0
    idat = bytearray()
    plte: list[tuple[int, int, int]] = []
    while pos + 8 <= len(data):
        length = int.from_bytes(data[pos:pos + 4], "big")
        ctype = data[pos + 4:pos + 8]
        chunk = data[pos + 8:pos + 8 + length]
        pos += 12 + length
        if ctype == b"IHDR" and len(chunk) >= 13:
            _, _, bit_depth, color_type, _, _, interlace = struct.unpack(">IIBBBBB", chunk[:13])
        elif ctype == b"PLTE":
            plte = [(chunk[i], chunk[i + 1], chunk[i + 2]) for i in range(0, len(chunk) - 2, 3)]
        elif ctype == b"IDAT":
            idat.extend(chunk)
        elif ctype == b"IEND":
            break
    if bit_depth != 8 or interlace != 0 or color_type not in (0, 2, 3, 4, 6):
        raise ValueError(f"unsupported PNG encoding for visual verification: {path}")
    try:
        raw = zlib.decompress(bytes(idat))
    except zlib.error as exc:
        raise ValueError(f"corrupt PNG IDAT stream: {path}") from exc
    bpp = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}[color_type]
    stride = width * bpp
    if len(raw) < height * (1 + stride):
        raise ValueError(f"truncated PNG scanlines: {path}")
    prev = bytearray(stride)
    cur = bytearray(stride)
    total = width * height
    border_rows = min(16, height // 8)
    black = top_black = bot_black = fallback_bg = 0
    buckets: set[tuple[int, int, int]] = set()
    idx = 0
    for y in range(height):
        filt = raw[idx]
        idx += 1
        line = raw[idx:idx + stride]
        idx += stride
        if filt == 0:
            cur[:] = line
        elif filt == 1:
            for i in range(stride):
                a = cur[i - bpp] if i >= bpp else 0
                cur[i] = (line[i] + a) & 0xFF
        elif filt == 2:
            for i in range(stride):
                cur[i] = (line[i] + prev[i]) & 0xFF
        elif filt == 3:
            for i in range(stride):
                a = cur[i - bpp] if i >= bpp else 0
                cur[i] = (line[i] + ((a + prev[i]) >> 1)) & 0xFF
        elif filt == 4:
            for i in range(stride):
                a = cur[i - bpp] if i >= bpp else 0
                b = prev[i]
                c = prev[i - bpp] if i >= bpp else 0
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                cur[i] = (line[i] + pr) & 0xFF
        else:
            raise ValueError(f"invalid PNG scanline filter {filt}: {path}")
        prev[:] = cur
        for x in range(width):
            off = x * bpp
            if color_type in (2, 6):
                r, g, b = cur[off], cur[off + 1], cur[off + 2]
            elif color_type == 3:
                idx_c = cur[off]
                r, g, b = plte[idx_c] if idx_c < len(plte) else (0, 0, 0)
            else:
                r = g = b = cur[off]
            if r <= 8 and g <= 8 and b <= 8:
                black += 1
                if y < border_rows:
                    top_black += 1
                if y >= height - border_rows:
                    bot_black += 1
            if (
                (abs(r - 18) <= 3 and abs(g - 22) <= 3 and abs(b - 28) <= 3)
                or (abs(r - 9) <= 3 and abs(g - 14) <= 3 and abs(b - 23) <= 3)
            ):
                fallback_bg += 1
            buckets.add((r >> 4, g >> 4, b >> 4))
    black_ratio = black / total
    top_ratio = top_black / (border_rows * width)
    bot_ratio = bot_black / (border_rows * width)
    fb_ratio = fallback_bg / total
    if black_ratio > MAX_BLACK_RATIO:
        raise ValueError(f"rejected mostly-black capture ({black_ratio:.1%} black): {path}")
    if top_ratio > MAX_BORDER_BLACK_RATIO or bot_ratio > MAX_BORDER_BLACK_RATIO:
        raise ValueError(
            f"rejected black-letterboxed capture (top={top_ratio:.1%}, bottom={bot_ratio:.1%}): {path}"
        )
    if fb_ratio > MAX_FALLBACK_BG_RATIO:
        raise ValueError(f"rejected unrendered fallback capture ({fb_ratio:.1%} fallback bg): {path}")
    if len(buckets) < MIN_RGB444_BUCKETS:
        raise ValueError(f"rejected low-entropy capture ({len(buckets)} RGB444 buckets): {path}")


def inspect_capture(path: Path) -> tuple[str, int, str]:
    """Validate regular file, permitted media signature and byte limit."""
    info = path.lstat()
    if not stat.S_ISREG(info.st_mode) or path.is_symlink():
        raise ValueError(f"capture is not a regular non-symlink file: {path}")
    suffix = path.suffix.lower()
    if suffix not in (".png", ".mp4", ".webm"):
        raise ValueError(f"unsupported capture type: {path.suffix}")
    min_size = 8 if suffix == ".png" else MIN_VIDEO
    limit = MAX_IMAGE if suffix == ".png" else MAX_VIDEO
    if not min_size <= info.st_size <= limit:
        raise ValueError(f"capture size outside permitted bounds ({info.st_size} bytes): {path}")
    with path.open("rb") as source:
        start = source.read(24)
    if suffix in SIGNATURES and not start.startswith(SIGNATURES[suffix]):
        raise ValueError(f"capture signature does not match extension: {path}")
    if suffix == ".mp4":
        if len(start) < 8 or start[4:8] != b"ftyp":
            raise ValueError(f"invalid MP4 signature: {path}")
        raw_video = path.read_bytes()
        if b"moov" not in raw_video or b"mdat" not in raw_video:
            raise ValueError(f"truncated or header-only MP4 missing moov/mdat atoms: {path}")
    if suffix == ".png":
        if len(start) < 24 or start[12:16] != b"IHDR":
            raise ValueError(f"invalid PNG header: {path}")
        with path.open("rb") as source:
            header = source.read(24)
        width = int.from_bytes(header[16:20], "big")
        height = int.from_bytes(header[20:24], "big")
        if width < 1 or height < 1 or width > 16384 or height > 16384:
            raise ValueError(f"PNG dimensions outside permitted bounds: {path}")
        validate_png_visual_quality(path, width, height)
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


def _publish_locked(paths: list[Path], root: Path, evidence_root: Path | None = None) -> int:
    """Atomically add explicit validated media while the cross-process lock is held."""
    root.mkdir(parents=True, exist_ok=True)
    if root.is_symlink() or not root.is_dir():
        raise ValueError("media root must be a real directory")
    manifest = load_manifest(root)
    items = {item["file"]: item for item in manifest["items"] if isinstance(item, dict) and isinstance(item.get("file"), str)}
    added = 0
    gate = admission.Admission(evidence_root)
    prepared = []
    for source in paths:
        suffix, size, digest = inspect_capture(source)
        proof = gate.authorize(digest, suffix, size)
        prepared.append((source, suffix, size, digest, proof))
    # A denied member cannot partially install the other files in its batch.
    for item in items.values():
        expected = item.get('sha256')
        suffix = Path(item['file']).suffix.lower()
        admission.require(isinstance(expected, str) and item.get('id') == expected[:16] and
                          item['file'] == expected[:16] + suffix and type(item.get('bytes')) is int,
                          'existing gallery identity differs')
        gate.authorize(expected, suffix, item['bytes'])
    for source, suffix, size, digest, proof in prepared:
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
                # Direct callers get the same exact-byte protection as watcher copies.
                copied_hash = hashlib.sha256()
                with open(temporary, 'rb') as copied:
                    for chunk in iter(lambda: copied.read(1024 * 1024), b''):
                        copied_hash.update(chunk)
                if Path(temporary).stat().st_size != size or copied_hash.hexdigest() != digest:
                    raise ValueError('capture changed between inspection and publication')
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
                           "created": datetime.now(timezone.utc).isoformat(), "admission": proof}
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


def publish(paths: list[Path], root: Path, *, evidence_root: Path | None = None) -> int:
    """Serialize publishers so concurrent writers cannot lose manifest entries."""
    root.mkdir(parents=True, exist_ok=True)
    if root.is_symlink() or not root.is_dir():
        raise ValueError("media root must be a real directory")
    lock_path = root / ".publish.lock"
    fd = os.open(lock_path, os.O_CREAT | os.O_RDWR | getattr(os, "O_NOFOLLOW", 0), 0o600)
    with os.fdopen(fd, "a+b") as lock_file:
        fcntl.flock(lock_file.fileno(), fcntl.LOCK_EX)
        try:
            return _publish_locked(paths, root, evidence_root)
        finally:
            fcntl.flock(lock_file.fileno(), fcntl.LOCK_UN)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--media-root", default="/media")
    parser.add_argument("--evidence-root", type=Path, default=Path(os.environ.get('PORT_PROGRESS_EVIDENCE_ROOT', '/evidence')))
    parser.add_argument("captures", nargs="+", type=Path)
    args = parser.parse_args()
    try:
        added = publish(args.captures, Path(args.media_root), evidence_root=args.evidence_root)
    except (OSError, ValueError) as error:
        parser.exit(2, f'Not published: {error}\n')
    print(f"Published {added} new capture(s)")


if __name__ == "__main__":
    main()
