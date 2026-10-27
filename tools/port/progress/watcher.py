#!/usr/bin/env python3
"""Watch only the project's three approved ignored capture folders."""
from __future__ import annotations

import argparse
import hashlib
import os
import re
import stat
import tempfile
import time
from pathlib import Path

import publish
import admission

WATCH_FOLDER_NAMES = ("port-smoke", "port-preview-output", "port-captures")
MEDIA_SUFFIXES = {".png", ".mp4", ".webm"}
RAW_VIDEO_FRAME = re.compile(r"(?:^|[-_])frame[-_]\d{4,}\.png$", re.IGNORECASE)


def fingerprint(info: os.stat_result) -> tuple[int, int, int, int, int]:
    """Identify a file version using inode, size and high-resolution timestamps."""
    return info.st_dev, info.st_ino, info.st_size, info.st_mtime_ns, info.st_ctime_ns


def hash_fd(fd: int) -> str:
    """Hash an already-open file from byte zero without loading it into memory."""
    digest = hashlib.sha256()
    os.lseek(fd, 0, os.SEEK_SET)
    for chunk in iter(lambda: os.read(fd, 1024 * 1024), b""):
        digest.update(chunk)
    return digest.hexdigest()


def copy_stable(source: Path, expected: tuple[int, int, int, int, int]) -> Path | None:
    """Copy a stable regular capture and compare source/copy hashes before publish."""
    suffix = source.suffix.lower()
    limit = publish.MAX_IMAGE if suffix == ".png" else publish.MAX_VIDEO
    try:
        before_path = source.lstat()
        if not stat.S_ISREG(before_path.st_mode) or fingerprint(before_path) != expected:
            return None
        if before_path.st_size < 8 or before_path.st_size > limit:
            raise ValueError("capture size outside permitted bounds")
        fd = os.open(source, os.O_RDONLY | getattr(os, "O_NOFOLLOW", 0))
    except OSError:
        return None

    temporary_path: Path | None = None
    keep_temporary = False
    try:
        before = os.fstat(fd)
        if not stat.S_ISREG(before.st_mode) or fingerprint(before) != expected:
            return None
        source_hash = hash_fd(fd)
        if fingerprint(os.fstat(fd)) != expected:
            return None

        temp_fd, temp_name = tempfile.mkstemp(prefix="port-progress-capture-", suffix=suffix)
        temporary_path = Path(temp_name)
        copied_hash = hashlib.sha256()
        copied_bytes = 0
        os.lseek(fd, 0, os.SEEK_SET)
        with os.fdopen(temp_fd, "wb") as target:
            while True:
                chunk = os.read(fd, 1024 * 1024)
                if not chunk:
                    break
                target.write(chunk)
                copied_hash.update(chunk)
                copied_bytes += len(chunk)
            target.flush()
            os.fsync(target.fileno())

        after_fd = os.fstat(fd)
        after_path = source.lstat()
        if fingerprint(after_fd) != expected or fingerprint(after_path) != expected:
            return None
        if copied_bytes != before.st_size or copied_hash.hexdigest() != source_hash:
            return None
        with temporary_path.open("rb") as copied:
            verified_copy = hashlib.sha256()
            for chunk in iter(lambda: copied.read(1024 * 1024), b""):
                verified_copy.update(chunk)
        if verified_copy.hexdigest() != source_hash:
            return None
        keep_temporary = True
        return temporary_path
    finally:
        os.close(fd)
        if not keep_temporary and temporary_path is not None:
            temporary_path.unlink(missing_ok=True)


class CaptureWatcher:
    """Publish new stable media while avoiding repeated hashes for unchanged files."""
    def __init__(self, roots: list[Path], media_root: Path, stable_polls: int = 2,
                 *, evidence_root: Path | None = None):
        self.roots = roots
        self.media_root = media_root
        self.stable_polls = stable_polls
        self.evidence_root = evidence_root
        self.registry_revision: str | None = None
        self.observed: dict[Path, tuple[tuple[int, int, int, int, int], int]] = {}
        self.published: dict[Path, tuple[int, int, int, int, int]] = {}

    def poll_once(self) -> int:
        """Process one observation pass; return newly-added content count."""
        try:
            revision = admission.registry_revision()
        except (OSError, ValueError):
            revision = None
        if revision != self.registry_revision:
            self.published.clear()
            self.registry_revision = revision
        current: dict[Path, tuple[int, int, int, int, int]] = {}
        added = 0
        for root in self.roots:
            try:
                root_stat = root.lstat()
                if not stat.S_ISDIR(root_stat.st_mode):
                    continue
                entries = list(os.scandir(root))
            except OSError:
                continue
            for entry in entries:
                if RAW_VIDEO_FRAME.search(entry.name):
                    continue
                suffix = Path(entry.name).suffix.lower()
                if suffix not in MEDIA_SUFFIXES:
                    continue
                source = root / entry.name
                try:
                    info = entry.stat(follow_symlinks=False)
                except OSError:
                    continue
                if not stat.S_ISREG(info.st_mode):
                    continue
                signature = fingerprint(info)
                current[source] = signature
                previous = self.observed.get(source)
                stable_count = previous[1] + 1 if previous and previous[0] == signature else 1
                self.observed[source] = (signature, stable_count)
                if stable_count < self.stable_polls or self.published.get(source) == signature:
                    continue
                copied: Path | None = None
                try:
                    copied = copy_stable(source, signature)
                    if copied is None:
                        continue
                    added += publish.publish([copied], self.media_root, evidence_root=self.evidence_root)
                    self.published[source] = signature
                except (OSError, ValueError):
                    # A stable malformed/incomplete output is retried only if its stat changes.
                    self.published[source] = signature
                finally:
                    if copied is not None:
                        copied.unlink(missing_ok=True)
        for source in set(self.observed) - set(current):
            self.observed.pop(source, None)
            self.published.pop(source, None)
        return added

    def run(self, interval_seconds: float = 2.0) -> None:
        """Poll approved folders indefinitely without writing status text."""
        while True:
            self.poll_once()
            time.sleep(interval_seconds)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--capture-root", default="/watch")
    parser.add_argument("--media-root", default="/media")
    parser.add_argument("--evidence-root", type=Path, default=Path(os.environ.get('PORT_PROGRESS_EVIDENCE_ROOT', '/evidence')))
    parser.add_argument("--interval", type=float, default=2.0)
    args = parser.parse_args()
    if args.interval < 0.5 or args.interval > 60:
        parser.error("poll interval must be from 0.5 to 60 seconds")
    capture_root = Path(args.capture_root)
    roots = [capture_root / name for name in WATCH_FOLDER_NAMES]
    CaptureWatcher(roots, Path(args.media_root), evidence_root=args.evidence_root).run(args.interval)


if __name__ == "__main__":
    main()
