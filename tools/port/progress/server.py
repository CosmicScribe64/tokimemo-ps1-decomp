#!/usr/bin/env python3
"""Serve a fixed local media viewer and validated capture manifest."""
from __future__ import annotations

import argparse
import json
import mimetypes
import re
import stat
import hashlib
import os
import threading
from collections import OrderedDict
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import unquote, urlsplit

STATIC = Path(__file__).resolve().parent / "static"
ID_RE = re.compile(r"^[a-f0-9]{16,64}$")
MEDIA = {".png": ("image/png", "image"), ".mp4": ("video/mp4", "video"), ".webm": ("video/webm", "video")}
MAX_HASH_CACHE = 512
_HASH_CACHE: OrderedDict[tuple[object, ...], bool] = OrderedDict()
_HASH_CACHE_LOCK = threading.Lock()
PROGRESS_PATH = Path(os.environ.get("PORT_PROGRESS_FILE", "/media/progress.json"))
DEFAULT_PROGRESS = {"overall_percent": 0, "first_slice_percent": 0, "estimated": True}


def safe_progress(path: Path | None = None) -> dict[str, object]:
    """Read only the numeric, bounded progress fields used by the viewer."""
    path = path or PROGRESS_PATH
    try:
        data = json.loads(path.read_text("utf-8"))
    except (OSError, ValueError):
        return dict(DEFAULT_PROGRESS)
    if not isinstance(data, dict) or data.get("schema") != "tokimemo.port-progress.v1":
        return dict(DEFAULT_PROGRESS)
    overall = data.get("overall_percent")
    first = data.get("first_slice_percent")
    estimated = data.get("estimated")
    if (type(overall) is not int or not 0 <= overall <= 100 or
            type(first) is not int or not 0 <= first <= 100 or type(estimated) is not bool):
        return dict(DEFAULT_PROGRESS)
    return {"overall_percent": overall, "first_slice_percent": first, "estimated": estimated}


def _stat_key(info: os.stat_result) -> tuple[int, int, int, int, int]:
    """Return metadata that changes when a normal file replacement/edit occurs."""
    return info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns


def _matches_digest(path: Path, expected: str, initial: os.stat_result) -> bool:
    """Verify file bytes, caching by full file identity and mutation timestamps."""
    signature = _stat_key(initial)
    cache_key = (str(path), *signature, expected)
    with _HASH_CACHE_LOCK:
        cached = _HASH_CACHE.get(cache_key)
        if cached is not None:
            _HASH_CACHE.move_to_end(cache_key)
            return cached
    digest = hashlib.sha256()
    try:
        with path.open("rb") as source:
            opened = os.fstat(source.fileno())
            if not stat.S_ISREG(opened.st_mode) or _stat_key(opened) != signature:
                return False
            for chunk in iter(lambda: source.read(1024 * 1024), b""):
                digest.update(chunk)
            finished = os.fstat(source.fileno())
            if _stat_key(finished) != signature:
                return False
    except OSError:
        return False
    matches = digest.hexdigest() == expected
    with _HASH_CACHE_LOCK:
        _HASH_CACHE[cache_key] = matches
        _HASH_CACHE.move_to_end(cache_key)
        while len(_HASH_CACHE) > MAX_HASH_CACHE:
            _HASH_CACHE.popitem(last=False)
    return matches


def safe_entries(media_root: Path) -> list[dict[str, object]]:
    """Return only manifest-listed regular media files directly under media_root."""
    manifest_path = media_root / "manifest.json"
    try:
        root_info = media_root.lstat()
        if not stat.S_ISDIR(root_info.st_mode) or manifest_path.is_symlink() or not manifest_path.is_file():
            return []
        data = json.loads(manifest_path.read_text("utf-8"))
    except (OSError, ValueError):
        return []
    if not isinstance(data, dict) or data.get("version") != 1 or not isinstance(data.get("items"), list):
        return []
    safe: list[dict[str, object]] = []
    for item in data["items"]:
        if not isinstance(item, dict):
            continue
        name, item_id = item.get("file"), item.get("id")
        if not isinstance(name, str) or not isinstance(item_id, str) or not ID_RE.fullmatch(item_id):
            continue
        suffix = Path(name).suffix.lower()
        if name != item_id + suffix or suffix not in MEDIA:
            continue
        path = media_root / name
        try:
            info = path.lstat()
            if not stat.S_ISREG(info.st_mode) or info.st_size <= 0 or info.st_size > (32 * 1024 * 1024 if suffix == ".png" else 512 * 1024 * 1024) or info.st_size != item.get("bytes"):
                continue
            expected_hash = item.get("sha256")
            if not isinstance(expected_hash, str) or not re.fullmatch(r"[a-f0-9]{64}", expected_hash) or not expected_hash.startswith(item_id):
                continue
            if not _matches_digest(path, expected_hash, info):
                continue
        except OSError:
            continue
        safe.append({"id": item_id, "file": name, "kind": MEDIA[suffix][1], "mime": MEDIA[suffix][0],
                     "bytes": info.st_size, "created": item.get("created", ""), "sha256": item["sha256"]})
    return sorted(safe, key=lambda entry: str(entry.get("created", "")), reverse=True)


def parse_range(value: str, size: int) -> tuple[int, int] | None:
    """Parse one HTTP byte range and return an inclusive start/end pair."""
    if not value.startswith("bytes=") or "," in value:
        return None
    match = re.fullmatch(r"bytes=(\d*)-(\d*)", value)
    if not match:
        return None
    left, right = match.groups()
    if not left and not right:
        return None
    try:
        if not left:
            length = int(right)
            if length <= 0:
                return None
            return max(0, size - length), size - 1
        start = int(left)
        end = int(right) if right else size - 1
    except ValueError:
        return None
    if start >= size or end < start:
        return None
    return start, min(end, size - 1)


class ViewerHandler(BaseHTTPRequestHandler):
    """Serve only bundled viewer assets, manifest JSON and manifest media."""
    server_version = "PortProgress/1"
    media_root: Path

    def log_message(self, fmt: str, *args: object) -> None:
        pass

    def do_GET(self) -> None:
        raw_path = urlsplit(self.path).path
        try:
            path = unquote(raw_path, errors="strict")
        except UnicodeDecodeError:
            self.send_error(400)
            return
        if path == "/" or path == "/index.html":
            self._static("index.html")
        elif path in ("/app.js", "/style.css"):
            self._static(path.lstrip("/"))
        elif path == "/api/manifest.json":
            self._json({"version": 1, "items": safe_entries(self.media_root), "progress": safe_progress()})
        elif path.startswith("/media/"):
            self._media(path[len("/media/"):])
        else:
            self.send_error(404)

    def _static(self, name: str) -> None:
        path = STATIC / name
        try:
            body = path.read_bytes()
        except OSError:
            self.send_error(404)
            return
        if name == "index.html":
            latest_image = next((entry for entry in safe_entries(self.media_root)
                                 if entry["kind"] == "image"), None)
            initial = (('<img src="/media/' + str(latest_image["file"]) +
                        '" alt="Port capture">').encode("ascii")
                       if latest_image else b"")
            body = body.replace(b"<!--INITIAL_MEDIA-->", initial)
            progress = safe_progress()
            body = body.replace(b"<!--INITIAL_QUALIFIER-->", b"~" if progress["estimated"] else b"")
            body = body.replace(b"<!--INITIAL_PROGRESS-->", str(progress["overall_percent"]).encode("ascii"))
            body = body.replace(b"<!--INITIAL_FIRST_SLICE-->", str(progress["first_slice_percent"]).encode("ascii"))
            dash_offset = 97.39 * (1 - int(progress["overall_percent"]) / 100)
            body = body.replace(b"<!--INITIAL_OFFSET-->", f"{dash_offset:.2f}".encode("ascii"))
        self._send(200, body, {"Content-Type": mimetypes.guess_type(name)[0] or "application/octet-stream",
                               "Cache-Control": "no-cache", "X-Content-Type-Options": "nosniff"})

    def _json(self, value: object) -> None:
        body = json.dumps(value, separators=(",", ":")).encode("utf-8")
        self._send(200, body, {"Content-Type": "application/json; charset=utf-8", "Cache-Control": "no-store",
                               "X-Content-Type-Options": "nosniff"})

    def _media(self, name: str) -> None:
        match = re.fullmatch(r"([a-f0-9]{16,64})\.(png|mp4|webm)", name)
        if not match:
            self.send_error(404)
            return
        entry = next((item for item in safe_entries(self.media_root) if item["file"] == name), None)
        if entry is None:
            self.send_error(404)
            return
        path = self.media_root / name
        try:
            info = path.lstat()
            if not stat.S_ISREG(info.st_mode):
                self.send_error(404)
                return
        except OSError:
            self.send_error(404)
            return
        headers = {"Content-Type": str(entry["mime"]), "Accept-Ranges": "bytes", "X-Content-Type-Options": "nosniff",
                   "Cache-Control": "public, max-age=31536000, immutable", "ETag": '"' + str(entry["sha256"]) + '"'}
        range_header = self.headers.get("Range")
        if range_header:
            byte_range = parse_range(range_header, info.st_size)
            if byte_range is None:
                self._send(416, b"", {"Content-Range": f"bytes */{info.st_size}", **headers})
                return
            start, end = byte_range
            status = 206
            headers["Content-Range"] = f"bytes {start}-{end}/{info.st_size}"
        else:
            start, end, status = 0, info.st_size - 1, 200
        headers["Content-Length"] = str(end - start + 1)
        self.send_response(status)
        for key, value in headers.items():
            self.send_header(key, value)
        self.end_headers()
        with path.open("rb") as source:
            source.seek(start)
            remaining = end - start + 1
            while remaining:
                chunk = source.read(min(65536, remaining))
                if not chunk:
                    break
                self.wfile.write(chunk)
                remaining -= len(chunk)

    def _send(self, status: int, body: bytes, headers: dict[str, str]) -> None:
        self.send_response(status)
        for key, value in headers.items():
            self.send_header(key, value)
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)


class ProgressServer(ThreadingHTTPServer):
    allow_reuse_address = True
    daemon_threads = True


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", default=8765, type=int)
    parser.add_argument("--media-root", default="/media")
    args = parser.parse_args()
    ViewerHandler.media_root = Path(args.media_root)
    ProgressServer((args.host, args.port), ViewerHandler).serve_forever()


if __name__ == "__main__":
    main()
