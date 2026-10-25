"""Strict, read-only CUE/BINARY CDDA extraction to ordinary PCM WAV.

Only an explicitly selected AUDIO track is exported. The CUE supplies sector
indices; CUE BINARY sample bytes are retained as little-endian PCM. This
reader does not infer audio extents from ISO
files or claim that a game runtime path plays the selected track.
"""
import hashlib
import json
import os
import re
import stat
import wave
import zipfile
from pathlib import Path, PurePosixPath

SECTOR_BYTES = 2352
FRAMES_PER_SECTOR = 588
SAMPLE_RATE = 44100
MAX_SOURCE_BYTES = 512 * 1024 * 1024
MAX_CUE_BYTES = 1024 * 1024
_ASSET_ID = re.compile(r"[A-Za-z0-9][A-Za-z0-9._-]{0,127}\Z")


class CddaError(ValueError):
    """Malformed, unsafe, ambiguous, or unsupported CDDA source."""


def _sha(data):
    return hashlib.sha256(data).hexdigest()


def _read_cue(path):
    if path.stat().st_size > MAX_CUE_BYTES:
        raise CddaError("CUE exceeds size limit")
    return path.read_bytes()


def _safe_member(name):
    p = PurePosixPath(name)
    return (name and not p.is_absolute() and ".." not in p.parts
            and all(part not in ("", ".") for part in p.parts))


def _cue_path(path, base_name):
    if not _safe_member(path) or "\\" in path:
        raise CddaError("CUE FILE path must be a relative, traversal-free path")
    base = PurePosixPath(base_name).parent
    combined = base / path
    if not _safe_member(str(combined)):
        raise CddaError("CUE FILE path escapes its source directory")
    return str(combined)


def _msf(value):
    m = re.fullmatch(r"(\d+):(\d{2}):(\d{2})", value)
    if not m:
        raise CddaError("invalid CUE time: %s" % value)
    minute, second, frame = map(int, m.groups())
    if second >= 60 or frame >= 75:
        raise CddaError("CUE time field outside minute/second/frame bounds")
    return (minute * 60 + second) * 75 + frame


def _parse_cue(raw):
    try:
        text = raw.decode("ascii")
    except UnicodeDecodeError as exc:
        raise CddaError("CUE must be ASCII") from exc
    files, tracks = [], []
    current_file = None
    current_track = None
    for lineno, line in enumerate(text.splitlines(), 1):
        line = line.strip()
        ignored = ("REM ", "TITLE ", "PERFORMER ", "CATALOG ", "ISRC ")
        if not line or line.upper().startswith(ignored):
            continue
        match = re.fullmatch(r'FILE\s+"([^"\r\n]+)"\s+(BINARY)', line, re.I)
        if match:
            current_file = match.group(1)
            files.append(current_file)
            current_track = None
            continue
        match = re.fullmatch(r"TRACK\s+(\d{2})\s+(AUDIO|MODE1/2352|MODE2/2352)", line, re.I)
        if match:
            if current_file is None:
                raise CddaError("TRACK appears before FILE at CUE line %d" % lineno)
            number = int(match.group(1))
            if tracks and number <= tracks[-1]["number"]:
                raise CddaError("track numbers must be unique and increasing")
            current_track = {"number": number, "type": match.group(2).upper(),
                             "file": current_file, "indices": {}, "flags": []}
            tracks.append(current_track)
            continue
        match = re.fullmatch(r"FLAGS\s+(.+)", line, re.I)
        if match and current_track is not None:
            flags = match.group(1).upper().split()
            if any(flag not in ("DCP", "SCMS", "PRE", "4CH") for flag in flags):
                raise CddaError("unsupported CUE FLAGS value at line %d" % lineno)
            current_track["flags"].extend(flags)
            continue
        match = re.fullmatch(r"INDEX\s+(\d{2})\s+(\d+:\d{2}:\d{2})", line, re.I)
        if match and current_track is not None:
            index = int(match.group(1))
            if index in current_track["indices"]:
                raise CddaError("duplicate INDEX %02d" % index)
            current_track["indices"][index] = _msf(match.group(2))
            continue
        raise CddaError("unsupported CUE directive at line %d: %s" % (lineno, line[:40]))
    if not files or not tracks:
        raise CddaError("CUE has no FILE/TRACK records")
    for track in tracks:
        if 1 not in track["indices"]:
            raise CddaError("track %02d has no INDEX 01" % track["number"])
        ordered = sorted(track["indices"].items())
        if any(b[1] < a[1] for a, b in zip(ordered, ordered[1:])):
            raise CddaError("track indices are not increasing")
    return files, tracks


class _Source:
    def __init__(self, path, track_number):
        self.path = Path(path)
        self.archive = None
        self.members = None
        self.track_number = track_number
        if self.path.is_symlink():
            raise CddaError("source path must not be a symlink")
        if self.path.is_file() and self.path.suffix.lower() == ".cue":
            self.cue_name = self.path.name
            self.cue_bytes = _read_cue(self.path)
            self.container_hash = _hash_path(self.path)
            self.kind = "cue"
            self.root = self.path.parent.resolve()
        elif self.path.is_dir():
            cues = sorted(p for p in self.path.iterdir() if p.suffix.lower() == ".cue")
            if len(cues) != 1:
                raise CddaError("directory input requires exactly one CUE file")
            cue = cues[0]
            if cue.is_symlink() or not cue.is_file():
                raise CddaError("CUE must be a regular, non-symlink file")
            self.cue_name = cue.name
            self.cue_bytes = _read_cue(cue)
            self.container_hash = _hash_path(cue)
            self.kind = "directory"
            self.root = self.path.resolve()
        elif zipfile.is_zipfile(self.path):
            if self.path.stat().st_size > MAX_SOURCE_BYTES:
                raise CddaError("ZIP container exceeds size limit")
            self.archive = zipfile.ZipFile(self.path)
            try:
                infos = self.archive.infolist()
                if len(infos) > 100000:
                    raise CddaError("ZIP member count exceeds limit")
                names = [i.filename for i in infos]
                if len(names) != len(set(names)) or any(not _safe_member(n) for n in names):
                    raise CddaError("ZIP has duplicate or unsafe member paths")
                if any(stat.S_ISLNK(i.external_attr >> 16) for i in infos):
                    raise CddaError("ZIP symlink members are unsupported")
                cues = [i for i in infos if i.filename.lower().endswith(".cue")]
                if len(cues) != 1:
                    raise CddaError("ZIP input requires exactly one CUE member")
                if cues[0].file_size > MAX_CUE_BYTES:
                    raise CddaError("CUE exceeds size limit")
                self.cue_name = cues[0].filename
                self.cue_bytes = self.archive.read(cues[0])
                self.container_hash = _hash_path(self.path)
                self.kind = "zip"
                self.members = {i.filename: i for i in infos}
            except Exception:
                self.close()
                raise
        else:
            raise CddaError("input must be a directory or ZIP containing one CUE")
        try:
            self.files, self.tracks = _parse_cue(self.cue_bytes)
            self.track = next((t for t in self.tracks if t["number"] == track_number), None)
            if self.track is None:
                raise CddaError("selected track number is absent")
            if self.track["type"] != "AUDIO":
                raise CddaError("selected track is not AUDIO")
            if any(flag in self.track["flags"] for flag in ("PRE", "4CH")):
                raise CddaError("selected track has unsupported playback-changing FLAGS")
            self.member_path = _cue_path(self.track["file"], self.cue_name)
            self._check_member()
        except Exception:
            self.close()
            raise

    def _check_member(self):
        if self.kind in ("directory", "cue"):
            full = self.root.joinpath(*PurePosixPath(self.member_path).parts)
            relative_parts = PurePosixPath(self.member_path).parts
            cursor = self.root
            if any((cursor := cursor / part).is_symlink() for part in relative_parts):
                raise CddaError("referenced track path contains a symlink")
            if not full.is_file():
                raise CddaError("referenced track must be a regular file without symlinks")
            self.member_size = full.stat().st_size
            self.member_full = full
        else:
            info = self.members.get(self.member_path)
            if info is None or info.is_dir() or info.file_size > MAX_SOURCE_BYTES:
                raise CddaError("referenced track member is absent or too large")
            self.member_size = info.file_size
            self.member_full = None
        if self.member_size > MAX_SOURCE_BYTES:
            raise CddaError("referenced track exceeds size limit")
        if self.member_size % SECTOR_BYTES:
            raise CddaError("AUDIO file is not sector aligned (2352 bytes)")

    def read_range(self, start, length):
        if start < 0 or length < 0 or start > self.member_size or length > self.member_size - start:
            raise CddaError("requested byte range exceeds track file")
        if self.kind in ("directory", "cue"):
            with self.member_full.open("rb") as f:
                f.seek(start)
                data = f.read(length)
        else:
            with self.archive.open(self.members[self.member_path]) as f:
                f.seek(start)
                data = f.read(length)
        if len(data) != length:
            raise CddaError("short read from referenced audio file")
        return data

    def chunks(self, size=1024 * 1024):
        if self.kind in ("directory", "cue"):
            stream = self.member_full.open("rb")
        else:
            stream = self.archive.open(self.members[self.member_path])
        with stream:
            while True:
                chunk = stream.read(size)
                if not chunk:
                    break
                yield chunk

    def close(self):
        if self.archive is not None:
            self.archive.close()


def _hash_path(path):
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def _write_range(src, path, start, length):
    """Stream a source slice to a cache path without replacing different data."""
    from tools.assets.manifest import output_root
    path = Path(path)
    checked = output_root(path)
    checked.parent.mkdir(parents=True, exist_ok=True)
    if checked.is_symlink():
        raise CddaError("output path must not be a symlink")
    temp = checked.with_name(checked.name + ".tmp")
    if temp.exists() or temp.is_symlink():
        raise CddaError("temporary output path already exists")
    h = hashlib.sha256()
    try:
        with temp.open("xb") as out:
            offset, remaining = start, length
            while remaining:
                size = min(1024 * 1024, remaining)
                chunk = src.read_range(offset, size)
                out.write(chunk)
                h.update(chunk)
                offset += size
                remaining -= size
        digest = h.hexdigest()
        if checked.exists():
            differs = (not checked.is_file() or checked.stat().st_size != length
                       or _hash_path(checked) != digest)
            if differs:
                raise CddaError("output already exists with different bytes")
            temp.unlink()
        else:
            os.replace(temp, checked)
        return {"path": str(checked.relative_to(Path(__file__).resolve().parents[2])),
                "size": length, "sha256": digest}
    except Exception:
        if temp.exists():
            temp.unlink()
        raise


class _CachedRaw:
    def __init__(self, path, size):
        self.path, self.size = Path(path), size

    def read_range(self, start, length):
        if start < 0 or length < 0 or start > self.size or length > self.size - start:
            raise CddaError("requested byte range exceeds cached source")
        with self.path.open("rb") as stream:
            stream.seek(start)
            data = stream.read(length)
        if len(data) != length:
            raise CddaError("short read from cached source")
        return data


def _copy_track(src, path):
    """Copy and hash the complete referenced FILE in one sequential read."""
    from tools.assets.manifest import output_root
    checked = output_root(path)
    checked.parent.mkdir(parents=True, exist_ok=True)
    if checked.is_symlink():
        raise CddaError("source-cache output must not be a symlink")
    temp = checked.with_name(checked.name + ".tmp")
    if temp.exists() or temp.is_symlink():
        raise CddaError("temporary output path already exists")
    h = hashlib.sha256()
    count = 0
    try:
        with temp.open("xb") as output:
            for chunk in src.chunks():
                count += len(chunk)
                if count > src.member_size:
                    raise CddaError("source track grew while copying")
                output.write(chunk)
                h.update(chunk)
        if count != src.member_size:
            raise CddaError("source track size changed while copying")
        digest = h.hexdigest()
        if checked.exists():
            differs = (not checked.is_file() or checked.stat().st_size != count
                       or _hash_path(checked) != digest)
            if differs:
                raise CddaError("cached source already exists with different bytes")
            temp.unlink()
        else:
            os.replace(temp, checked)
        return {"path": str(checked.relative_to(Path(__file__).resolve().parents[2])),
                "size": count, "sha256": digest}
    except Exception:
        if temp.exists():
            temp.unlink()
        raise


def _reject_overlap(src, output):
    output = Path(output).resolve()
    paths = [src.path.resolve()]
    if src.kind in ("cue", "directory"):
        paths.append(src.root.resolve())
    for path in paths:
        if output == path or output in path.parents or path in output.parents:
            raise CddaError("input and output paths must not overlap")


def _inspect_source(src, file_hash):
    track = src.track
    start = track["indices"][1]
    pregap_start = track["indices"].get(0, start)
    if pregap_start > start:
        raise CddaError("INDEX 00 follows INDEX 01")
    next_track = next((t for t in src.tracks
                       if t["number"] > src.track_number and t["file"] == track["file"]), None)
    end = (next_track["indices"].get(0, next_track["indices"][1])
           if next_track else src.member_size // SECTOR_BYTES)
    if start > end or end > src.member_size // SECTOR_BYTES:
        raise CddaError("track INDEX lies outside its source FILE")
    if any(sector > end for sector in track["indices"].values()):
        raise CddaError("selected track has an INDEX beyond its track end")
    if any(sector > src.member_size // SECTOR_BYTES for sector in track["indices"].values()):
        raise CddaError("selected track INDEX lies outside its source FILE")
    return {
            "format": "CDDA/BINARY",
            "status": "supported",
            "source": {"container": str(src.path), "container_kind": src.kind,
                       "container_sha256": None if src.kind == "directory" else src.container_hash,
                       "cue_member": src.cue_name, "cue_sha256": _sha(src.cue_bytes),
                       "file_member": src.member_path, "file_size": src.member_size,
                       "file_sha256": file_hash,
                       "read_only": True},
            "track_number": src.track_number,
            "track_type": track["type"],
            "track_flags": list(track["flags"]),
            "track_indices": {"%02d" % i: sector for i, sector in sorted(track["indices"].items())},
            "index00_sector": pregap_start,
            "index01_sector": start,
            "end_sector_exclusive": end,
            "pregap_sectors": start - pregap_start,
            "program_sectors": end - start,
            "sample_rate": SAMPLE_RATE,
            "channels": 2,
            "bits_per_sample": 16,
            "source_sample_byte_order": "little-endian",
            "wav_sample_byte_order": "little-endian",
            "runtime_use": "not established by this source parser",
        }


def inspect_cdda(source_path, *, track_number=2):
    """Return bounded metadata for a selected AUDIO track without exporting it."""
    src = _Source(source_path, track_number)
    try:
        return _inspect_source(src, _hash_member(src))
    finally:
        src.close()


def _hash_member(src):
    h = hashlib.sha256()
    count = 0
    for chunk in src.chunks():
        h.update(chunk)
        count += len(chunk)
    if count != src.member_size:
        raise CddaError("source track size changed while hashing")
    return h.hexdigest()


def export_cdda(source_path, output_dir, asset_id, *, track_number=2):
    """Export the INDEX 01 program area as 44.1 kHz stereo PCM WAV.

    The raw INDEX 00 pregap bytes, when present, are separately preserved.
    Output paths must be beneath the repository .asset-cache.
    """
    if not isinstance(asset_id, str) or not _ASSET_ID.fullmatch(asset_id):
        raise CddaError("asset_id must be a safe filename component")
    from tools.assets.manifest import output_root
    root = output_root(output_dir)
    src = _Source(source_path, track_number)
    try:
        _reject_overlap(src, root)
        root.mkdir(parents=True, exist_ok=True)
        raw_record = _copy_track(src, root / (asset_id + ".source.bin"))
        cached = _CachedRaw(root / (asset_id + ".source.bin"), src.member_size)
        meta = _inspect_source(src, raw_record["sha256"])
        start = meta["index01_sector"]
        end = meta["end_sector_exclusive"]
        pstart = meta["index00_sector"]
        pregap_length = (start - pstart) * SECTOR_BYTES
        pregap_record = None
        if pregap_length:
            pregap_record = _write_range(cached, root / (asset_id + ".pregap.bin"),
                                         pstart * SECTOR_BYTES, pregap_length)
        wav_path = root / (asset_id + ".wav")
        if wav_path.is_symlink():
            raise CddaError("output path must not be a symlink")
        total = (end - start) * SECTOR_BYTES
        # Use the repository's create-or-verify behavior without buffering the WAV.
        temp = root / (asset_id + ".wav.tmp")
        if temp.exists() or temp.is_symlink():
            raise CddaError("temporary output path already exists")
        try:
            with temp.open("xb") as f:
                with wave.open(f, "wb") as out:
                    out.setnchannels(2)
                    out.setsampwidth(2)
                    out.setframerate(SAMPLE_RATE)
                    left = total
                    offset = start * SECTOR_BYTES
                    while left:
                        amount = min(left, 1024 * 1024)
                        amount -= amount % 2
                        chunk = cached.read_range(offset, amount)
                        # GNU ccd2cue documents CUE FILE BINARY as little-endian;
                        # MOTOROLA sources are unsupported by this reader:
                        # Source: https://www.gnu.org/software/ccd2cue/manual/html_node/
                        # FILE-_0028CUE-Command_0029.html
                        out.writeframesraw(chunk)
                        offset += amount
                        left -= amount
            if wav_path.exists():
                if not wav_path.is_file() or _hash_path(wav_path) != _hash_path(temp):
                    raise CddaError("output already exists with different bytes")
                temp.unlink()
            else:
                os.replace(temp, wav_path)
        except Exception:
            if temp.exists():
                temp.unlink()
            raise
        wav_record = {"path": str(wav_path.relative_to(Path(__file__).resolve().parents[2])),
                      "size": wav_path.stat().st_size, "sha256": _hash_path(wav_path)}
        result = dict(meta)
        result.update({"status": "exported", "asset_id": asset_id,
                       "raw_track_copy": raw_record, "wav": wav_record, "pregap": pregap_record,
                       "pregap_sha256": pregap_record["sha256"] if pregap_record else None,
                       "wav_sample_frames": (end - start) * FRAMES_PER_SECTOR})
        manifest = root / (asset_id + ".json")
        if manifest.is_symlink():
            raise CddaError("manifest path must not be a symlink")
        manifest_bytes = (json.dumps(result, sort_keys=True, indent=2) + "\n").encode("utf-8")
        from tools.assets.manifest import write_bytes
        write_bytes(manifest, manifest_bytes)
        result["manifest"] = str(manifest.relative_to(Path(__file__).resolve().parents[2]))
        return result
    finally:
        src.close()
