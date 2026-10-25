"""Decode verified Form 2 XA/STR streams with the pinned FFmpeg demuxer.

The wrapper validates staged raw sectors, keeps XA file numbers and channels
separate, and delegates codec work to FFmpeg. Video is emitted as a lossless
PNG frame sequence without a claimed frame rate; the PSX STR demuxer supplies
a 1/15 time base, but that does not establish the game's presentation timing.
"""
import hashlib
import json
import os
import re
import shutil
import struct
import subprocess
import tempfile
import time
import wave
from collections import OrderedDict
from pathlib import Path, PurePosixPath

from .manifest import CACHE, REPOSITORY, output_root, write_manifest

SCHEMA = "tokimemo.media.stream-decode.v1"
RAW_SCHEMA = "tokimemo.raw-sector-stream.v1"
RAW_SECTOR_BYTES = 2352
FORM2_PAYLOAD_BYTES = 2324
MAX_RAW_BYTES = 128 * 1024 * 1024
MAX_SECTORS = MAX_RAW_BYTES // RAW_SECTOR_BYTES
MAX_GROUPS = 4096
MAX_VIDEO_FRAMES = 4096
MAX_VIDEO_WIDTH = 640
MAX_VIDEO_HEIGHT = 480
MAX_VIDEO_FRAME_BYTES = 16 * 1024 * 1024
MAX_OUTPUT_BYTES = 512 * 1024 * 1024
MAX_MANIFEST_BYTES = 32 * 1024 * 1024
BS_CHUNK_HEADER_OFFSET = 0x38
BS_CHUNK_MAGIC_OFFSET = 0x3A
BS_CHUNK_QUANT_OFFSET = 0x3C
BS_CHUNK_VERSION_OFFSET = 0x3E
BS_CHUNK_MAGIC = 0x3800
BS_SUPPORTED_VERSIONS = (1, 2, 3)
BS_VERSION_ADAPTER = {
    "kind": "ffmpeg-codec-input-only",
    "original_version": 1,
    "derived_version": 2,
    "sdk_source": "asm/libpress_vlc.s",
    "sdk_source_sha256": "9ff152e8b0344c36e4cc327913a38b3bf193bd8917c7da272738fb4f29c3b9f0",
    "sdk_v1_branch_vma": "80086F04",
    "sdk_v2_fallthrough_vma": "80086F10",
    "sdk_shared_entry_vma": "80086F18",
    "sdk_shared_dc_vma": "80086F28",
    "sdk_shared_ac_vma": "80086F5C",
    "equivalence_scope": "v1/v2 coefficient processing convergence only; no general decode or hardware parity claim",
}
FFMPEG_VERSION_PREFIX = "4.4.2-0ubuntu0.22.04.1"
_ASSET_ID = re.compile(r"[A-Za-z0-9][A-Za-z0-9._-]{0,127}\Z")
_SHA256 = re.compile(r"[0-9a-f]{64}\Z")
_SECTOR_SYNC = b"\x00" + b"\xff" * 10 + b"\x00"
_ENV = {"PATH": "/usr/bin:/bin", "LC_ALL": "C", "LANG": "C"}


class StreamDecodeError(ValueError):
    """Staged source is invalid, unsafe, unsupported, or failed to decode."""


def _int(value):
    return isinstance(value, int) and not isinstance(value, bool)


def _inside_cache_input(path):
    candidate = Path(path)
    if not candidate.is_absolute():
        candidate = REPOSITORY / candidate
    input_root_lexical = CACHE / "input"
    if input_root_lexical.is_symlink():
        raise StreamDecodeError(".asset-cache/input may not be a symlink")
    lexical = Path(os.path.abspath(candidate))
    try:
        relative = lexical.relative_to(input_root_lexical)
    except ValueError as exc:
        raise StreamDecodeError("staged input must be beneath .asset-cache/input/") from exc
    cursor = input_root_lexical
    for part in relative.parts:
        cursor = cursor / part
        if cursor.is_symlink():
            raise StreamDecodeError("staged input path may not contain symlinks")
    checked = output_root(candidate)
    input_root = (CACHE / "input").resolve()
    if checked == input_root or input_root not in checked.parents:
        raise StreamDecodeError("staged input must be beneath .asset-cache/input/")
    return checked


def _media_output_path(path):
    candidate = Path(path)
    if not candidate.is_absolute():
        candidate = REPOSITORY / candidate
    media_root = CACHE / "media"
    if media_root.is_symlink():
        raise StreamDecodeError(".asset-cache/media may not be a symlink")
    lexical = Path(os.path.abspath(candidate))
    try:
        relative = lexical.relative_to(media_root)
    except ValueError as exc:
        raise StreamDecodeError("decoded output must be beneath .asset-cache/media/") from exc
    cursor = media_root
    for part in relative.parts:
        cursor = cursor / part
        if cursor.is_symlink():
            raise StreamDecodeError("decoded output path may not contain symlinks")
    checked = output_root(lexical)
    resolved_media = media_root.resolve()
    if checked == resolved_media or resolved_media not in checked.parents:
        raise StreamDecodeError("decoded output must be beneath .asset-cache/media/")
    return checked


def _digest_file(path):
    h = hashlib.sha256()
    size = 0
    with Path(path).open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            size += len(block)
            h.update(block)
    return size, h.hexdigest()


def _actual_sector(raw, index, source_lba):
    if len(raw) != RAW_SECTOR_BYTES or raw[:12] != _SECTOR_SYNC or raw[15] != 2:
        raise StreamDecodeError("raw stream contains a bad CD sync or MODE2 sector")
    sub = raw[16:20]
    if sub != raw[20:24]:
        raise StreamDecodeError("raw stream sector has mismatched duplicated subheaders")
    form = 2 if sub[2] & 0x20 else 1
    payload_size = FORM2_PAYLOAD_BYTES if form == 2 else 2048
    payload = raw[24:24 + payload_size]
    return {
        "sector_index": index,
        "lba": source_lba + index,
        "raw_size": RAW_SECTOR_BYTES,
        "payload_size": payload_size,
        "raw_sha256": hashlib.sha256(raw).hexdigest(),
        "payload_sha256": hashlib.sha256(payload).hexdigest(),
        "form": form,
        "form_name": "Form2" if form == 2 else "Form1",
        "file_number": sub[0],
        "channel": sub[1],
        "submode": sub[2],
        "coding": sub[3],
        "edc_validation": "not_performed",
    }


def _opaque_sector_record(raw, index, source_lba):
    if len(raw) != RAW_SECTOR_BYTES or raw[:12] != _SECTOR_SYNC or raw[15] != 2:
        raise StreamDecodeError("opaque raw sector has invalid CD sync/MODE2 framing")
    return {
        "sector_index": index,
        "lba": source_lba + index,
        "raw_size": RAW_SECTOR_BYTES,
        "raw_sha256": hashlib.sha256(raw).hexdigest(),
        "subheader_primary": raw[16:20].hex(),
        "subheader_secondary": raw[20:24].hex(),
    }


def _classify(record):
    file_number = record["file_number"]
    channel = record["channel"]
    if not 0 <= file_number <= 255 or not 0 <= channel < 32:
        raise StreamDecodeError("XA file number/channel is outside FFmpeg PSX STR bounds")
    kind_bits = record["submode"] & 0x0E
    if kind_bits == 0:
        return None
    if kind_bits == 0x02:
        return "video"
    if kind_bits == 0x04:
        if record["form"] != 2:
            raise StreamDecodeError("Form 1 XA audio sectors are unsupported by this decoder")
        coding = record["coding"]
        # FFmpeg 4.4.2 psxstr accepts its mono/stereo and 18.9/37.8 kHz bits;
        # reject all other coding combinations rather than rely on ignored bits.
        if coding & ~0x05:
            raise StreamDecodeError("unsupported XA audio coding flags 0x%02x" % coding)
        return "audio"
    if kind_bits == 0x08:
        return None
    raise StreamDecodeError("mixed or unknown XA sector type flags 0x%02x" % kind_bits)


def _video_header(raw):
    header_id, header_type = struct.unpack_from("<HH", raw, 0x18)
    current, count = struct.unpack_from("<HH", raw, 0x1C)
    frame_number = struct.unpack_from("<I", raw, 0x20)[0]
    frame_size = struct.unpack_from("<I", raw, 0x24)[0]
    width, height = struct.unpack_from("<HH", raw, 0x28)
    if (count == 0 or current >= count or frame_size == 0
            or frame_size > MAX_VIDEO_FRAME_BYTES or count * 0x7E0 < frame_size):
        raise StreamDecodeError("invalid or oversized STR video frame-sector header")
    if not 0 < width <= MAX_VIDEO_WIDTH or not 0 < height <= MAX_VIDEO_HEIGHT:
        raise StreamDecodeError("STR dimensions exceed the bounded movie profile")
    return current, count, frame_size, width, height, header_id, header_type, frame_number


def _validate_video_order(group):
    current_frame = None
    frames = []
    for offset, header in group["video_headers"]:
        current, count, size, width, height, header_id, header_type, frame_number = header
        if current == 0:
            if current_frame is not None:
                raise StreamDecodeError("STR video frame ended before all source sectors arrived")
            current_frame = {"next": 0, "count": count, "size": size,
                             "width": width, "height": height, "header_id": header_id,
                             "header_type": header_type, "frame_number": frame_number,
                             "sector_indices": []}
        if current_frame is None:
            raise StreamDecodeError("STR video stream starts with a nonzero frame sector")
        if (current != current_frame["next"] or count != current_frame["count"]
                or size != current_frame["size"] or width != current_frame["width"]
                or height != current_frame["height"] or header_id != current_frame["header_id"]
                or header_type != current_frame["header_type"]
                or frame_number != current_frame["frame_number"]):
            raise StreamDecodeError("STR frame sectors are missing, reordered, or inconsistent")
        current_frame["next"] += 1
        current_frame["sector_indices"].append(offset)
        if current_frame["next"] == current_frame["count"]:
            frames.append({"width": width, "height": height, "frame_size": size,
                           "sector_count": count, "source_header_id": header_id,
                           "source_header_type": header_type,
                           "source_frame_number": frame_number,
                           "source_sector_indices": current_frame["sector_indices"]})
            if len(frames) > MAX_VIDEO_FRAMES:
                raise StreamDecodeError("video frame count exceeds the bounded PNG export limit")
            current_frame = None
    if current_frame is not None or not frames:
        raise StreamDecodeError("STR video stream ends with an incomplete frame")
    group["frames"] = frames


def _validate_stream_manifest(manifest_path):
    manifest_file = _inside_cache_input(manifest_path)
    if not manifest_file.is_file():
        raise StreamDecodeError("raw stream manifest is not a regular file")
    if manifest_file.stat().st_size > MAX_MANIFEST_BYTES:
        raise StreamDecodeError("staged raw stream manifest exceeds size limit")
    try:
        raw_manifest_bytes = manifest_file.read_bytes()
        manifest = json.loads(raw_manifest_bytes.decode("utf-8"))
    except (OSError, UnicodeError, ValueError) as exc:
        raise StreamDecodeError("cannot read staged raw stream manifest") from exc
    status = manifest.get("status") if isinstance(manifest, dict) else None
    if (not isinstance(manifest, dict) or manifest.get("schema") != RAW_SCHEMA
            or status not in ("staged", "partial")):
        raise StreamDecodeError("unsupported raw stream manifest schema/status")
    source = manifest.get("source")
    raw_record = manifest.get("raw_stream")
    payload_record = manifest.get("payload_stream")
    if not isinstance(source, dict) or not isinstance(raw_record, dict) or not isinstance(payload_record, dict):
        raise StreamDecodeError("staged raw stream provenance records are missing")
    input_hash = source.get("input_sha256")
    source_path = source.get("path")
    lba = source.get("lba")
    declared_size = source.get("declared_iso_size_bytes")
    count = source.get("sector_count")
    baseline_asset_id = source.get("baseline_asset_id")
    source_parts = PurePosixPath(source_path).parts if isinstance(source_path, str) else ()
    if (not isinstance(input_hash, str) or not _SHA256.fullmatch(input_hash)
            or source.get("format") not in ("mode2-2352", "zip-mode2-2352")
            or not isinstance(source_path, str) or not source_path or "\\" in source_path
            or "\x00" in source_path or PurePosixPath(source_path).is_absolute()
            or any(part in ("", ".", "..") for part in source_parts)
            or not _int(lba) or lba < 0 or not _int(declared_size) or declared_size <= 0
            or declared_size % 2048 or not _int(count) or count != declared_size // 2048
            or not isinstance(baseline_asset_id, str) or not _ASSET_ID.fullmatch(baseline_asset_id)):
        raise StreamDecodeError("staged stream source bounds or provenance are invalid")
    if count > MAX_SECTORS:
        raise StreamDecodeError("staged raw stream exceeds the decode sector limit")
    raw_size = count * RAW_SECTOR_BYTES
    if (raw_record.get("size") != raw_size
            or raw_record.get("concatenation") != "original 2352-byte sectors in ascending LBA order"
            or not isinstance(raw_record.get("sha256"), str)
            or not _SHA256.fullmatch(raw_record["sha256"])):
        raise StreamDecodeError("raw stream size/hash/concatenation metadata is invalid")
    if payload_record.get("concatenation") != "each sector's Mode2 payload in ascending LBA order":
        raise StreamDecodeError("Form 2 payload size/hash metadata is invalid")
    if manifest.get("edc_validation") != "not_performed":
        raise StreamDecodeError("unexpected EDC validation claim")
    sector_records = manifest.get("sectors")
    if not isinstance(sector_records, list) or len(sector_records) != count:
        raise StreamDecodeError("per-sector metadata count does not match source extent")
    raw_path_text = raw_record.get("path")
    raw_parts = PurePosixPath(raw_path_text).parts if isinstance(raw_path_text, str) else ()
    if (not isinstance(raw_path_text, str) or not raw_path_text or "\\" in raw_path_text
            or "\x00" in raw_path_text or PurePosixPath(raw_path_text).is_absolute()
            or any(part in ("", ".", "..") for part in raw_parts)):
        raise StreamDecodeError("raw stream manifest has no relative path")
    raw_path = _inside_cache_input(REPOSITORY / PurePosixPath(raw_path_text))
    if not raw_path.is_file() or raw_path.stat().st_size != raw_size:
        raise StreamDecodeError("staged raw stream file is missing or has wrong size")
    declared_invalid = source.get("invalid_subheader_sector_count", 0)
    partial = status == "partial"
    if not _int(declared_invalid) or declared_invalid < 0 or (partial != (declared_invalid > 0)):
        raise StreamDecodeError("opaque-sector count does not match staged manifest status")
    if partial:
        if (payload_record.get("complete") is not False
                or payload_record.get("invalid_sector_count") != declared_invalid
                or payload_record.get("sha256") is not None):
            raise StreamDecodeError("partial payload metadata does not declare its unresolved sector gaps")
    elif (not _int(payload_record.get("size")) or payload_record.get("size") <= 0
          or not isinstance(payload_record.get("sha256"), str)
          or not _SHA256.fullmatch(payload_record["sha256"])):
        raise StreamDecodeError("complete sector-payload size/hash metadata is invalid")
    groups = OrderedDict()
    raw_hash = hashlib.sha256()
    payload_hash = hashlib.sha256()
    payload_size = 0
    unrepresented_sector_counts = {}
    opaque_sectors = []
    segment_number = 0
    last_coding = {}
    coding_segments = {}
    with raw_path.open("rb") as stream:
        for index, record in enumerate(sector_records):
            raw = stream.read(RAW_SECTOR_BYTES)
            if len(raw) != RAW_SECTOR_BYTES:
                raise StreamDecodeError("short read from staged raw stream")
            if not isinstance(record, dict):
                raise StreamDecodeError("staged per-sector metadata is malformed")
            raw_hash.update(raw)
            invalid = record.get("subheader_valid") is False
            if invalid:
                opaque = _opaque_sector_record(raw, index, lba)
                expected_opaque = {
                    **opaque,
                    "metadata_status": "opaque_invalid_duplicate_subheader",
                    "subheader_valid": False,
                    "form": None,
                    "form_name": None,
                    "payload_size": None,
                    "payload_sha256": None,
                    "file_number": None,
                    "channel": None,
                    "submode": None,
                    "coding": None,
                    "edc_validation": "not_performed",
                }
                if record != expected_opaque or opaque["subheader_primary"] == opaque["subheader_secondary"]:
                    raise StreamDecodeError("opaque sector record does not match its ambiguous raw subheaders")
                opaque_sectors.append({"sector_index": index, "lba": lba + index,
                                       "raw_sha256": opaque["raw_sha256"],
                                       "subheader_primary": opaque["subheader_primary"],
                                       "subheader_secondary": opaque["subheader_secondary"],
                                       "reason": "duplicate XA subheader copies differ; fields were not interpreted"})
                segment_number += 1
                continue
            actual = _actual_sector(raw, index, lba)
            expected = {**actual, "metadata_status": "validated", "subheader_valid": True,
                        "subheader_primary": raw[16:20].hex(),
                        "subheader_secondary": raw[20:24].hex()}
            if record != expected and record != actual:
                raise StreamDecodeError("staged per-sector metadata disagrees with raw sector bytes")
            payload_hash.update(raw[24:24 + actual["payload_size"]])
            payload_size += actual["payload_size"]
            kind = _classify(actual)
            if kind is None:
                category = "%s:type-%02x" % (actual["form_name"], actual["submode"] & 0x0E)
                unrepresented_sector_counts[category] = unrepresented_sector_counts.get(category, 0) + 1
                continue
            base_key = (actual["file_number"], actual["channel"], kind, segment_number)
            previous_coding = last_coding.get(base_key)
            if previous_coding is not None and previous_coding != actual["coding"]:
                coding_segments[base_key] = coding_segments.get(base_key, 0) + 1
            last_coding[base_key] = actual["coding"]
            coding_segment = coding_segments.get(base_key, 0)
            key = base_key + (coding_segment,)
            group = groups.setdefault(key, {"file_number": key[0], "channel": key[1],
                                            "kind": kind, "segment": key[3],
                                            "coding_segment": key[4], "indices": [],
                                            "coding": actual["coding"],
                                            "sector_hashes": {},
                                            "video_headers": [], "submode_counts": {},
                                            "eof_sector_indices": []})
            if group["coding"] != actual["coding"]:
                raise StreamDecodeError("coding changes within one XA file/channel stream")
            if kind == "video":
                group["video_headers"].append((index, _video_header(raw)))
            group["submode_counts"][str(actual["submode"])] = (
                group["submode_counts"].get(str(actual["submode"]), 0) + 1)
            if actual["submode"] & 0x80:
                group["eof_sector_indices"].append(index)
            group["indices"].append(index)
            group["sector_hashes"][index] = actual["raw_sha256"]
        if stream.read(1):
            raise StreamDecodeError("staged raw stream has trailing bytes")
    if raw_hash.hexdigest() != raw_record["sha256"] or payload_size != payload_record.get("size"):
        raise StreamDecodeError("staged raw/payload stream hash does not match the manifest")
    if not partial and payload_hash.hexdigest() != payload_record["sha256"]:
        raise StreamDecodeError("staged payload stream hash does not match the manifest")
    if len(opaque_sectors) != declared_invalid:
        raise StreamDecodeError("opaque subheader sector count does not match source provenance")
    if not groups:
        raise StreamDecodeError("stream contains no supported XA audio or STR video sectors")
    if len(groups) > MAX_GROUPS:
        raise StreamDecodeError("file/channel stream count exceeds the bounded export limit")
    for group in groups.values():
        if group["kind"] == "video":
            _validate_video_order(group)
    if sum(len(group.get("frames", [])) for group in groups.values()) > MAX_VIDEO_FRAMES:
        raise StreamDecodeError("aggregate video frame count exceeds the bounded PNG export limit")
    return {
        "manifest": manifest,
        "manifest_path": manifest_file,
        "manifest_sha256": hashlib.sha256(raw_manifest_bytes).hexdigest(),
        "raw_path": raw_path,
        "raw_sha256": raw_hash.hexdigest(),
        "source": source,
        "groups": list(groups.values()),
        "opaque_sectors": opaque_sectors,
        "partial": partial,
        "sector_count": count,
        "raw_size": raw_size,
        "empty_sector_count": count - len(opaque_sectors)
                              - sum(len(group["indices"]) for group in groups.values())
                              - sum(unrepresented_sector_counts.values()),
        "unrepresented_sector_counts": unrepresented_sector_counts,
    }


def inspect_staged_stream(manifest_path):
    """Verify raw bytes against the staged manifest and summarize streams."""
    verified = _validate_stream_manifest(manifest_path)
    return {
        "schema": SCHEMA,
        "status": "partial" if verified["partial"] else "verified",
        "source": verified["source"],
        "raw_stream_manifest": str(verified["manifest_path"].relative_to(REPOSITORY)),
        "raw_stream_manifest_sha256": verified["manifest_sha256"],
        "raw_stream_sha256": verified["raw_sha256"],
        "raw_stream_size": verified["raw_size"],
        "sector_count": verified["sector_count"],
        "empty_sector_count": verified["empty_sector_count"],
        "unrepresented_sector_counts": verified["unrepresented_sector_counts"],
        "quarantined_sectors": verified["opaque_sectors"],
        "streams": [{k: group[k] for k in ("file_number", "channel", "kind", "coding",
                                                "segment", "coding_segment")}
                    | {"sector_count": len(group["indices"]),
                       "submode_counts": group["submode_counts"],
                       "eof_sector_indices": group["eof_sector_indices"],
                       "frame_count": len(group.get("frames", [])),
                       "frame_dimensions": sorted({(f["width"], f["height"])
                                                    for f in group.get("frames", [])})}
                    for group in verified["groups"]],
        "limitations": ["source provenance does not prove runtime clip/movie use",
                        "EDC validation was not performed",
                        "video frame timing is not established by the extracted sector stream"]
                      + (["ambiguous duplicate-subheader sectors remain quarantined; valid runs are separate"]
                         if verified["partial"] else []),
    }


def _run(argv, timeout):
    try:
        result = subprocess.run(argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                text=True, timeout=timeout, check=False, env=_ENV)
    except subprocess.TimeoutExpired as exc:
        raise StreamDecodeError("FFmpeg process exceeded its time limit") from exc
    except OSError as exc:
        raise StreamDecodeError("pinned FFmpeg tools are unavailable") from exc
    if result.returncode:
        detail = (result.stderr or "").strip()[-2000:]
        raise StreamDecodeError("media decode command failed: " + detail)
    return result


def _tool_version(tool, timeout=10):
    output = _run([tool, "-version"], timeout=timeout).stdout
    first = output.splitlines()[0] if output.splitlines() else ""
    if not first.startswith(tool + " version " + FFMPEG_VERSION_PREFIX):
        raise StreamDecodeError("unexpected %s version: %s" % (tool, first[:120]))
    return first


def _remaining(deadline):
    remaining = deadline - time.monotonic()
    if remaining <= 0:
        raise StreamDecodeError("media export exceeded its total time limit")
    return remaining


def _probe_int(stream, field):
    value = stream.get(field)
    try:
        return int(value)
    except (TypeError, ValueError):
        return -1


def _probe(path, kind, timeout):
    fields = ("stream=index,codec_name,codec_type,width,height,sample_rate,channels,channel_layout,"
              "time_base,avg_frame_rate,r_frame_rate:format=format_name,duration,size")
    result = _run(["ffprobe", "-v", "error", "-f", "psxstr", "-show_streams",
                   "-show_format", "-show_entries", fields, "-of", "json", str(path)], timeout=timeout)
    try:
        probe = json.loads(result.stdout)
    except ValueError as exc:
        raise StreamDecodeError("ffprobe did not return valid JSON") from exc
    streams = probe.get("streams") if isinstance(probe, dict) else None
    fmt = probe.get("format") if isinstance(probe, dict) else None
    if (not isinstance(streams, list) or len(streams) != 1
            or not isinstance(streams[0], dict) or streams[0].get("codec_type") != kind
            or not isinstance(fmt, dict) or "psxstr" not in fmt.get("format_name", "")):
        raise StreamDecodeError("FFmpeg did not expose exactly one expected media stream")
    expected_codec = "adpcm_xa" if kind == "audio" else "mdec"
    if streams[0].get("codec_name") != expected_codec:
        raise StreamDecodeError("unexpected PSX STR codec: %s" % streams[0].get("codec_name"))
    return probe, streams[0]


def _copy_group_input(raw_path, indices, expected_hashes, destination):
    with raw_path.open("rb") as source, destination.open("xb") as output:
        for index in indices:
            source.seek(index * RAW_SECTOR_BYTES)
            sector = source.read(RAW_SECTOR_BYTES)
            if (len(sector) != RAW_SECTOR_BYTES
                    or hashlib.sha256(sector).hexdigest() != expected_hashes[index]):
                raise StreamDecodeError("source sector changed after staged-manifest verification")
            output.write(sector)


def _adapt_video_codec_input(path, group):
    """Adapt only the validated first BS version word in derived FFmpeg input."""
    data = bytearray(Path(path).read_bytes())
    original_data = bytes(data)
    original_sha256 = hashlib.sha256(data).hexdigest()
    index_positions = {source_index: position
                       for position, source_index in enumerate(group["indices"])}
    changes = []
    for frame in group["frames"]:
        indices = frame.get("source_sector_indices")
        if (not isinstance(indices, list) or len(indices) != frame["sector_count"]
                or not indices or indices[0] not in index_positions):
            raise StreamDecodeError("STR frame has no validated first-sector position")
        positions = [index_positions.get(index, -1) for index in indices]
        if positions != list(range(positions[0], positions[0] + len(positions))):
            raise StreamDecodeError("STR frame sectors are not contiguous in the derived codec input")
        sector_position = positions[0]
        sector_start = sector_position * RAW_SECTOR_BYTES
        header_start = sector_start + BS_CHUNK_HEADER_OFFSET
        if sector_start + RAW_SECTOR_BYTES > len(data):
            raise StreamDecodeError("STR frame header exceeds derived codec input bounds")
        declared_mdec_words, = struct.unpack_from("<H", data, header_start)
        magic = struct.unpack_from("<H", data, sector_start + BS_CHUNK_MAGIC_OFFSET)[0]
        quant = struct.unpack_from("<H", data, sector_start + BS_CHUNK_QUANT_OFFSET)[0]
        version = struct.unpack_from("<H", data, sector_start + BS_CHUNK_VERSION_OFFSET)[0]
        if (magic != BS_CHUNK_MAGIC or declared_mdec_words == 0
                or declared_mdec_words * 4 > MAX_VIDEO_FRAME_BYTES or not 1 <= quant <= 63):
            raise StreamDecodeError("STR first-chunk BS declaration/magic/quant is outside the verified profile")
        if version not in BS_SUPPORTED_VERSIONS:
            raise StreamDecodeError("unsupported STR BS version %d" % version)
        if version == 1:
            derived_offset = sector_start + BS_CHUNK_VERSION_OFFSET
            changes.append({
                "source_frame_number": frame["source_frame_number"],
                "source_sector_index": indices[0],
                "source_sector_index_start": indices[0],
                "source_sector_index_end_exclusive": indices[-1] + 1,
                "source_sector_indices": indices,
                "raw_sector_byte_offset": BS_CHUNK_VERSION_OFFSET,
                "derived_input_byte_offset": derived_offset,
                "bs_chunk_header_offset": BS_CHUNK_HEADER_OFFSET,
                "declared_mdec_words": declared_mdec_words,
                "declared_mdec_bytes": declared_mdec_words * 4,
                "bs_chunk_magic": magic,
                "bs_quant": quant,
                "before_version": version,
                "after_version": 2,
            })
            struct.pack_into("<H", data, derived_offset, 2)
    derived_sha256 = hashlib.sha256(data).hexdigest()
    if changes:
        allowed_changed_offsets = {
            change["derived_input_byte_offset"] + byte_offset
            for change in changes for byte_offset in (0, 1)
        }
        changed_offsets = [index for index, (before, after) in
                           enumerate(zip(original_data, data)) if before != after]
        if (len(original_data) != len(data)
                or not set(changed_offsets).issubset(allowed_changed_offsets)
                or len(changed_offsets) != len(changes)):
            raise StreamDecodeError("BS version adapter changed bytes outside the permitted version tags")
        Path(path).write_bytes(data)
        return {
            **BS_VERSION_ADAPTER,
            "status": "adapted-v1-to-v2",
            "source_input_size": len(data),
            "source_input_sha256": original_sha256,
            "derived_input_size": len(data),
            "derived_input_sha256": derived_sha256,
            "changed_byte_count": len(changed_offsets),
            "version_word_span_bytes": len(changes) * 2,
            "changed_input_byte_offsets": changed_offsets,
            "source_raw_sector_mutation": "none; only the derived temporary FFmpeg input was changed",
            "version_changes": changes,
            "limitations": ["no CD EDC/ECC or authoring checksum is asserted for derived codec input",
                            "the staged raw source and original input remain unmodified",
                            "the SDK evidence establishes only v1/v2 coefficient-path convergence"],
        }
    return {
        "kind": BS_VERSION_ADAPTER["kind"],
        "status": "not-required",
        "source_input_size": len(data),
        "source_input_sha256": original_sha256,
        "derived_input_size": len(data),
        "derived_input_sha256": original_sha256,
        "source_raw_sector_mutation": "none; codec input was unchanged",
        "version_changes": [],
        "validated_bs_versions": sorted({
            struct.unpack_from("<H", data, index_positions[frame["source_sector_indices"][0]]
                               * RAW_SECTOR_BYTES + BS_CHUNK_VERSION_OFFSET)[0]
            for frame in group["frames"]}),
    }


def _index_runs(indices):
    runs = []
    for index in indices:
        if runs and index == runs[-1]["end_exclusive"]:
            runs[-1]["end_exclusive"] += 1
        else:
            runs.append({"start": index, "end_exclusive": index + 1})
    return runs


def _publish(temp_path, final_path, output_limit):
    size, sha = _digest_file(temp_path)
    if size > output_limit:
        raise StreamDecodeError("decoded output exceeded the bounded size limit")
    checked = _media_output_path(final_path)
    checked.parent.mkdir(parents=True, exist_ok=True)
    if checked.is_symlink():
        raise StreamDecodeError("decoded output path may not be a symlink")
    try:
        os.link(temp_path, checked)
    except FileExistsError:
        existing_size, existing_sha = _digest_file(checked)
        if existing_size != size or existing_sha != sha:
            raise StreamDecodeError("output already exists with different bytes")
    return {"path": str(checked.relative_to(REPOSITORY)), "size": size, "sha256": sha}


def decode_staged_stream(manifest_path, output_dir, asset_id, *,
                         timeout_seconds=120, media_kinds=("audio", "video")):
    """Decode each verified file/channel/type independently using FFmpeg.

    Outputs are cache-confined under `.asset-cache/media/`. Audio is PCM WAV;
    STR video is a sequence of lossless PNG frames with no invented frame rate.
    """
    if not isinstance(asset_id, str) or not _ASSET_ID.fullmatch(asset_id):
        raise StreamDecodeError("asset_id must be a safe filename component")
    if not _int(timeout_seconds) or not 1 <= timeout_seconds <= 600:
        raise StreamDecodeError("timeout_seconds must be between 1 and 600")
    if (not isinstance(media_kinds, (list, tuple, set, frozenset))
            or not media_kinds or any(kind not in ("audio", "video") for kind in media_kinds)):
        raise StreamDecodeError("media_kinds must select audio and/or video")
    selected_kinds = sorted(set(media_kinds))
    requested = Path(output_dir)
    if not requested.is_absolute():
        requested = REPOSITORY / requested
    root = _media_output_path(requested)
    media_root = (CACHE / "media").resolve()
    if root == media_root or media_root not in root.parents:
        raise StreamDecodeError("decoded output must be beneath .asset-cache/media/")
    verified = _validate_stream_manifest(manifest_path)
    selected_groups = [group for group in verified["groups"] if group["kind"] in selected_kinds]
    unselected_groups = [group for group in verified["groups"] if group["kind"] not in selected_kinds]
    if not selected_groups:
        raise StreamDecodeError("no source streams match selected media_kinds")
    estimated_png_bytes = sum(
        frame["width"] * frame["height"] * 4 + 4096
        for group in selected_groups for frame in group.get("frames", []))
    if estimated_png_bytes > MAX_OUTPUT_BYTES:
        raise StreamDecodeError("selected video frame dimensions exceed the bounded PNG output budget")
    unselected_streams = [{
        "kind": group["kind"], "file_number": group["file_number"],
        "channel": group["channel"], "source_coding": group["coding"],
        "source_sector_count": len(group["indices"]),
        "source_sector_index_runs": _index_runs(group["indices"]),
        "source_frame_count": len(group.get("frames", [])),
    } for group in unselected_groups]
    if verified["manifest_path"] == root or verified["manifest_path"] in root.parents:
        raise StreamDecodeError("decoded output overlaps its staged manifest")
    raw_path = verified["raw_path"]
    if raw_path == root or raw_path in root.parents or root in raw_path.parents:
        raise StreamDecodeError("decoded output overlaps staged raw input")
    deadline = time.monotonic() + timeout_seconds
    ffmpeg_version = _tool_version("ffmpeg", min(10, _remaining(deadline)))
    ffprobe_version = _tool_version("ffprobe", min(10, _remaining(deadline)))
    root.mkdir(parents=True, exist_ok=True)
    final_dir = root / asset_id
    if final_dir.exists() and not final_dir.is_dir():
        raise StreamDecodeError("asset output path already exists and is not a directory")
    if final_dir.is_symlink():
        raise StreamDecodeError("asset output directory may not be a symlink")
    work = Path(tempfile.mkdtemp(prefix=".stream-decode-", dir=root))
    generated = []
    pending_outputs = []
    total_size = 0
    try:
        for group in selected_groups:
            file_id, channel, kind = group["file_number"], group["channel"], group["kind"]
            segment = group["segment"]
            coding_segment = group["coding_segment"]
            stem = "%s-file-%02x-channel-%02x-segment-%02d-code-%02x-run-%02d-%s" % (
                asset_id, file_id, channel, segment, group["coding"], coding_segment, kind)
            input_path = work / (stem + ".raw2352")
            _copy_group_input(raw_path, group["indices"], group["sector_hashes"], input_path)
            codec_input_adapter = None
            if kind == "video":
                codec_input_adapter = _adapt_video_codec_input(input_path, group)
            remaining = _remaining(deadline)
            probe, stream = _probe(input_path, kind, min(remaining, 60))
            if kind == "audio":
                expected_channels = 2 if group["coding"] & 1 else 1
                expected_rate = 18900 if group["coding"] & 4 else 37800
                if (_probe_int(stream, "channels") != expected_channels
                        or _probe_int(stream, "sample_rate") != expected_rate):
                    raise StreamDecodeError("ffprobe audio properties disagree with sector coding byte")
                output_path = work / (stem + ".wav")
                # FFmpeg 4.4.2 libavformat/psxstr.c returns EIO on its final
                # short-sector read. One XA packet produces one audio frame;
                # cap output at the staged sector count and still check exact
                # decoded PCM sample-frame count below.
                command = ["ffmpeg", "-nostdin", "-hide_banner", "-loglevel", "error", "-xerror",
                           "-threads", "1", "-filter_threads", "1", "-f", "psxstr", "-i",
                           str(input_path), "-map", "0:a:0", "-vn", "-frames:a",
                           str(len(group["indices"])), "-c:a", "pcm_s16le",
                           "-threads", "1", "-fs", str(MAX_OUTPUT_BYTES), "-f", "wav", str(output_path)]
                _run(command, _remaining(deadline))
                try:
                    with wave.open(str(output_path), "rb") as wav:
                        if (wav.getnchannels(), wav.getframerate(), wav.getsampwidth()) != (
                                expected_channels, expected_rate, 2):
                            raise StreamDecodeError("decoded WAV properties disagree with sector coding")
                        output_frames = wav.getnframes()
                except (OSError, wave.Error) as exc:
                    raise StreamDecodeError("FFmpeg did not produce a valid PCM WAV") from exc
                expected_frames = len(group["indices"]) * (2016 if expected_channels == 2 else 4032)
                if output_frames != expected_frames:
                    raise StreamDecodeError("decoded WAV frame count differs from XA sector coding extent")
                outputs = [(output_path, root / (asset_id + "/" + stem + ".wav"))]
                record = {"kind": "audio", "sample_rate": expected_rate,
                          "channels": expected_channels, "bits_per_sample": 16,
                          "sample_frames": output_frames, "source_coding": group["coding"],
                          "source_sector_count": len(group["indices"]),
                          "ffmpeg_audio_frame_limit": len(group["indices"]),
                          "demux_eof_note": "FFmpeg n4.4.2 psxstr returns EIO when read_packet receives a short final sector; output is capped to the validated XA sector count and exact PCM extent is checked",
                          "ffprobe": probe}
            else:
                first = group["frames"][0]
                if (_probe_int(stream, "width") != first["width"]
                        or _probe_int(stream, "height") != first["height"]):
                    raise StreamDecodeError("ffprobe video dimensions disagree with STR frame header")
                if len(group["frames"]) > MAX_VIDEO_FRAMES:
                    raise StreamDecodeError("video frame count exceeds the bounded PNG export limit")
                pattern = work / (stem + "-%06d.png")
                command = ["ffmpeg", "-nostdin", "-hide_banner", "-loglevel", "error", "-xerror",
                           "-threads", "1", "-filter_threads", "1", "-f", "psxstr", "-i",
                           str(input_path), "-map", "0:v:0", "-an", "-vsync", "0", "-frames:v",
                           str(len(group["frames"])), "-c:v", "png", "-pix_fmt", "rgb24",
                           "-compression_level", "9", "-threads", "1", "-f", "image2", str(pattern)]
                _run(command, _remaining(deadline))
                images = sorted(work.glob(stem + "-*.png"))
                if len(images) != len(group["frames"]):
                    raise StreamDecodeError("FFmpeg decoded a different frame count than the sector headers")
                outputs = []
                for index, image in enumerate(images):
                    frame = group["frames"][index]
                    data = image.read_bytes()
                    if (len(data) < 24 or data[:8] != b"\x89PNG\r\n\x1a\n"
                            or struct.unpack_from(">II", data, 16) != (frame["width"], frame["height"])):
                        raise StreamDecodeError("decoded PNG dimensions/signature disagree with STR headers")
                    outputs.append((image, root / (asset_id + "/" + stem + "-%06d.png" % index)))
                record = {"kind": "video_frames", "frame_count": len(images),
                          "frame_headers": group["frames"],
                          "source_sector_count": len(group["indices"]),
                          "codec_input_adapter": codec_input_adapter,
                          "ffprobe": probe,
                          "timing": "not encoded; FFmpeg psxstr demuxer uses a 1/15 time base, not proven original playback timing",
                          "pixel_decode_claim": "FFmpeg MDEC decode to PNG; no hardware parity claim"}
            output_records = []
            for source_output, final_path in outputs:
                size = source_output.stat().st_size
                total_size += size
                if total_size > MAX_OUTPUT_BYTES:
                    raise StreamDecodeError("aggregate decoded output exceeded size limit")
                size_checked, sha = _digest_file(source_output)
                if size != size_checked:
                    raise StreamDecodeError("decoded output changed while hashing")
                output_records.append({"path": str(final_path.relative_to(REPOSITORY)),
                                       "size": size, "sha256": sha})
                pending_outputs.append((source_output, final_path))
            record["outputs"] = output_records
            generated.append({"file_number": file_id, "channel": channel,
                              "source_segment_index": segment,
                              "source_coding_segment_index": coding_segment,
                              "opaque_gap_before": segment > 0,
                              "source_submode_type": 0x04 if kind == "audio" else 0x02,
                              "source_coding": group["coding"],
                              "source_sector_index_start": group["indices"][0],
                              "source_sector_index_end_exclusive": group["indices"][-1] + 1,
                              "source_sector_index_runs": _index_runs(group["indices"]),
                              "source_lba_start": verified["source"]["lba"] + group["indices"][0],
                              "source_lba_end_exclusive": verified["source"]["lba"] + group["indices"][-1] + 1,
                              "source_submode_counts": group["submode_counts"],
                              "source_eof_sector_indices": group["eof_sector_indices"], **record})
        result = {
            "schema": SCHEMA,
            "status": "partial" if verified["partial"] or unselected_streams else "exported",
            "asset_id": asset_id,
            "media_selection": {
                "selected_kinds": selected_kinds,
                "selected_group_count": len(selected_groups),
                "unselected_streams": unselected_streams,
                "source_stream_coverage": "selected-kinds-only" if unselected_streams else "all-supported-kinds",
            },
            "source": {"raw_stream_manifest": str(verified["manifest_path"].relative_to(REPOSITORY)),
                       "raw_stream_manifest_sha256": verified["manifest_sha256"],
                       "raw_stream_path": str(raw_path.relative_to(REPOSITORY)),
                       "raw_stream_sha256": verified["raw_sha256"],
                       "raw_stream_size": verified["raw_size"],
                       "source_path": verified["source"]["path"],
                       "input_sha256": verified["source"]["input_sha256"],
                       "baseline_asset_id": verified["source"]["baseline_asset_id"],
                       "lba": verified["source"]["lba"],
                       "sector_count": verified["sector_count"]},
            "tools": {"ffmpeg": ffmpeg_version, "ffprobe": ffprobe_version},
            "streams": generated,
            "quarantined_sectors": verified["opaque_sectors"],
            "unrepresented_sector_counts": verified["unrepresented_sector_counts"],
            "limitations": ["XA file numbers and channels are decoded independently; original multiplex ordering is not reconstructed",
                            "outputs do not establish runtime dialogue clips, movie presentation timing, or SEQ music playback",
                            "EDC validation was not performed",
                            "MDEC output is software-decoded; no hardware pixel-parity claim"]
                          + (["opaque duplicate-subheader sectors were not decoded; valid runs remain separate and no gap repair is claimed"]
                             if verified["partial"] else []),
        }
        if unselected_streams:
            result["limitations"].append(
                "some source streams were excluded by media_kinds; this export does not cover the full source")
        for source_output, final_path in pending_outputs:
            _publish(source_output, final_path, MAX_OUTPUT_BYTES)
        final_dir.mkdir(parents=True, exist_ok=True)
        manifest_path_out = _media_output_path(root / (asset_id + ".json"))
        if manifest_path_out.is_symlink():
            raise StreamDecodeError("output manifest path may not be a symlink")
        write_manifest(manifest_path_out, result)
        result["manifest"] = str(manifest_path_out.relative_to(REPOSITORY))
        shutil.rmtree(work)
        return result
    except Exception:
        shutil.rmtree(work, ignore_errors=True)
        raise
