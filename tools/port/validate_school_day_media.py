"""Strict validator for tokimemo.school-day-media.v1 external packs."""
import hashlib
import json
import re
from pathlib import Path, PurePosixPath

SCHEMA = "tokimemo.school-day-media.v1"
MAX_MANIFEST = 8 * 1024 * 1024
MAX_PNG = 2 * 1024 * 1024
MAX_PIXELS = 4 * 1024 * 1024
SHA = re.compile(r"^[0-9a-f]{64}$")
SAFE_NAME = re.compile(r"^[A-Za-z0-9][A-Za-z0-9_.-]{0,191}\.png$")
EXPECTED_PROVENANCE = {
    "original_input_sha256": "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72",
    "original_executable_sha256": "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae",
    "reviewed_media_manifest_sha256": "0c85bbc54be82c7b2597528ccbfc915cd1fb5f99405bd18d845452026b56be37",
    "native_profile_sha256": "ac246a30ed894bf5b631beaf4f053a8622324094859b11a65a4c0c03d55cb4a3",
    "readers": ["tools/assets/input.py:open_input", "tools/assets/image_packs_sd.py:export_sd_ui", "tools/assets/bs_cohort_profile.py:build_profile_context", "tools/assets/image_mdec.py:export_mdec"],
}
EXPECTED_CAPABILITIES = ["command_0_static_background", "command_0_sd_sequence_0", "command_7_sd_sequence_0"]
NATIVE_ROUTE_PROFILE = "OpenBIOS d93840921b5ad7ed515e95b0e414f1b1e2038b1a"
EXPECTED_UNSUPPORTED = {
    "command_7_background": "no fresh accepted geometry profile in this tranche",
    "full_route": "not claimed",
    "scene_composition": "runtime placement/compositing not established by this media adapter",
}
PART_KEYS = {"part_index", "source_record_offset", "x", "y", "width", "height", "texture_id", "sheet_index", "sheet_source_offset", "u", "v", "palette_info", "flip_flags", "clut_vram", "palette_source_offset", "runtime_palette_override", "palette_mapping"}
SOURCE_PINS = {
    "command_0_background": ("CDROM/BS/C_01_27.BS", 16131, 20032, "7543e723ea06c7db100b148350394f79c1a94ff8961aa65904d4aa1613295992"),
    "command_0_sd": ("CDROM/MAIN_SD/SD_P01W.NBN", 42541, 83968, "6fea04f95b7eb68731816fa21395605942b1b912b0b915afcfea4d540da7eb6f"),
    "command_7_sd": ("CDROM/MAIN_SD/SD_P09W.NBN", 43074, 83968, "1ad4b906e8aefbb57fd0b9a22afea9af83a0d5109234f9621abc5f91829e7d28"),
}


def _read_png(root: Path, spec: dict, dims=None):
    if not isinstance(spec, dict) or set(spec) != {"path", "size", "sha256"}:
        raise ValueError("PNG descriptor fields differ from v1 contract")
    path = spec.get("path")
    if (not isinstance(path, str) or not SAFE_NAME.fullmatch(path) or PurePosixPath(path).name != path
            or not _exact_int(spec.get("size"), 1, MAX_PNG) or not isinstance(spec.get("sha256"), str)
            or not SHA.fullmatch(spec["sha256"])):
        raise ValueError("unsafe media payload path")
    file = root / "assets" / path
    if file.is_symlink() or not file.is_file() or file.stat().st_size > MAX_PNG:
        raise ValueError("missing, linked, or oversized PNG payload")
    data = file.read_bytes()
    if len(data) != spec.get("size") or hashlib.sha256(data).hexdigest() != spec.get("sha256"):
        raise ValueError("payload size/hash mismatch")
    width, height = _png_header_size(data)
    if width * height > MAX_PIXELS or (dims and [width, height] != dims):
        raise ValueError("PNG header dimensions exceed or differ from descriptor")
    from tools.assets.visual_text_equivalence import _decode_png
    _decode_png(data)


def _png_header_size(data):
    if len(data) < 24 or data[:8] != b"\x89PNG\r\n\x1a\n" or data[8:12] != b"\x00\x00\x00\r" or data[12:16] != b"IHDR":
        raise ValueError("PNG header is invalid or truncated")
    import struct
    width, height = struct.unpack_from(">II", data, 16)
    if not width or not height or width > 8192 or height > 8192 or width * height > MAX_PIXELS:
        raise ValueError("PNG dimensions exceed decoder bound")
    return width, height


def _exact_int(value, low, high):
    return type(value) is int and low <= value <= high


def _int_vector(value, length, low, high):
    return isinstance(value, list) and len(value) == length and all(_exact_int(v, low, high) for v in value)


def validate_pack(pack: str):
    root = Path(pack).absolute()
    current = Path(root.anchor)
    for part in root.parts[1:]:
        current = current / part
        if current.is_symlink():
            raise ValueError("pack path contains symlink")
    manifest = root / "manifest.json"
    if manifest.is_symlink() or not manifest.is_file() or manifest.stat().st_size > MAX_MANIFEST:
        raise ValueError("missing or oversized manifest")
    doc = json.loads(manifest.read_text(encoding="utf-8"))
    if not isinstance(doc, dict):
        raise ValueError("manifest JSON must be an object")
    if set(doc) != {"schema", "version", "provenance", "route_capabilities", "unsupported", "components"}:
        raise ValueError("manifest keys differ from v1 contract")
    if doc.get("schema") != SCHEMA or type(doc.get("version")) is not int or doc["version"] != 1:
        raise ValueError("unsupported media schema/version")
    if doc.get("provenance") != EXPECTED_PROVENANCE:
        raise ValueError("original input/reader provenance differs from reviewed source pins")
    if doc.get("route_capabilities") != EXPECTED_CAPABILITIES:
        raise ValueError("route capabilities differ from bounded supported sources")
    if doc.get("unsupported") != EXPECTED_UNSUPPORTED:
        raise ValueError("unsupported route capability declarations differ from reviewed scope")
    assets_dir = root / "assets"
    if assets_dir.is_symlink() or not assets_dir.is_dir():
        raise ValueError("assets path is missing or a symlink")
    if not isinstance(doc["components"], dict):
        raise ValueError("invalid provenance/components")
    if set(doc["components"]) != {"command_0_background", "command_0_sd", "command_7_sd"}:
        raise ValueError("component set differs from bounded capability contract")
    bg = doc["components"]["command_0_background"]
    if not isinstance(bg, dict) or set(bg) != {"kind", "id", "source", "active_size", "padded_size", "active_crop", "native_route", "image", "stp_mask", "padded_image", "padded_stp_mask", "geometry_context", "reader", "capability"}:
        raise ValueError("background fields differ from v1 contract")
    if (bg.get("kind") != "mdec_background" or bg.get("id") != "asset-cc9a7f7ff39dfb8c072ef6675934282792dfd7afef494b0c0b180643ea54135d"
            or bg.get("reader") != "tools/assets/image_mdec.py:export_mdec" or bg.get("capability") != "source_profiled_static_background_preview"
            or not _int_vector(bg.get("active_size"), 2, 1, 8192)
            or not _int_vector(bg.get("padded_size"), 2, 1, 8192)
            or not _int_vector(bg.get("active_crop"), 4, 0, 8192)
            or bg["active_size"] != [320, 88] or bg["padded_size"] != [320, 96]
            or bg["active_crop"] != [0, 0, 320, 88]):
        raise ValueError("invalid command-0 background descriptor")
    _validate_source("command_0_background", bg)
    _validate_native_route(bg, 0)
    _read_png(root, bg.get("image", {}), [320, 88])
    _read_png(root, bg.get("stp_mask", {}), [320, 88])
    _read_png(root, bg.get("padded_image", {}), [320, 96])
    _read_png(root, bg.get("padded_stp_mask", {}), [320, 96])
    for key, expected_index in (("command_0_sd", 3), ("command_7_sd", 16)):
        c = doc["components"][key]
        if not isinstance(c, dict) or set(c) != {"kind", "id", "source", "profile_index", "sequence_index", "native_route", "frames", "sequences", "palette", "reader", "capability"}:
            raise ValueError("SD fields differ from v1 contract")
        expected_id = "asset-0c557c34ef671750c707a8dc3573c2c2ea0c31af6e773654523f3024f68bf4e5" if key == "command_0_sd" else "asset-c15fc44db85fe2b1485503e27b84a50e5a7bf9d28866d5efef247d9e172a8c28"
        if (c.get("kind") != "sd_animation" or c.get("id") != expected_id
                or c.get("reader") != "tools/assets/image_packs_sd.py:export_sd_ui"
                or c.get("capability") != "validated_static_sd_frames_and_sequence_prefix"
                or not _exact_int(c.get("profile_index"), 0, 32767) or c.get("profile_index") != expected_index
                or not _exact_int(c.get("sequence_index"), 0, 32767) or c.get("sequence_index") != 0):
            raise ValueError("invalid SD source/profile descriptor")
        _validate_source(key, c)
        _validate_native_route(c, 0 if key == "command_0_sd" else 7)
        if not SHA.fullmatch(c.get("source", {}).get("sha256", "")) or not SHA.fullmatch(c.get("source", {}).get("exe_sha256", "")):
            raise ValueError("invalid original SD source hashes")
        frames = c.get("frames")
        if not isinstance(frames, list) or not frames or len(frames) > 512:
            raise ValueError("invalid bounded physical frame list")
        seen = set()
        source_palette = None
        for f in frames:
            if not isinstance(f, dict) or set(f) != {"frame_index", "id", "png", "stp_png", "bounds", "origin", "parts", "source_palette_words"}:
                raise ValueError("SD frame fields differ from v1 contract")
            idx = f.get("frame_index")
            if not _exact_int(idx, 0, 32767) or idx in seen:
                raise ValueError("invalid or duplicate frame index")
            seen.add(idx)
            b = f.get("bounds")
            origin = f.get("origin")
            if not _int_vector(origin, 2, -32768, 32767):
                raise ValueError("frame origin must be two signed 16-bit values")
            if not _int_vector(b, 4, -32768, 32767) or b[2] < 1 or b[3] < 1 or b[2] * b[3] > MAX_PIXELS:
                raise ValueError("invalid frame bounds")
            if f.get("id") != f"{c['id']}:frame:{idx}":
                raise ValueError("frame canonical identity does not match source asset/index")
            palette = f.get("source_palette_words")
            if not _int_vector(palette, 256, 0, 65535):
                raise ValueError("frame palette must contain 256 unsigned source words")
            if source_palette is None:
                source_palette = palette
            elif palette != source_palette:
                raise ValueError("frame palette changed within one source SD pack")
            parts = f.get("parts")
            if not isinstance(parts, list) or len(parts) > 128:
                raise ValueError("frame parts must be a bounded array")
            for part in parts:
                if not isinstance(part, dict) or set(part) != PART_KEYS:
                    raise ValueError("frame part metadata fields differ from reader contract")
                for coord in ("x", "y"):
                    if not _exact_int(part.get(coord), -32768, 32767):
                        raise ValueError("frame part origin exceeds signed 16-bit range")
                for number, lo, hi in (("part_index",0,255),("source_record_offset",0,83967),("width",1,128),("height",1,128),("texture_id",0,255),("sheet_index",0,1),("sheet_source_offset",0,83967),("u",0,127),("v",0,127),("palette_info",0,65535),("flip_flags",0,3),("palette_source_offset",0,511),("runtime_palette_override",0,255)):
                    if not _exact_int(part.get(number), lo, hi):
                        raise ValueError("invalid bounded SD part field: " + number)
                if part["u"] + part["width"] > 128 or part["v"] + part["height"] > 128:
                    raise ValueError("SD part exceeds 128x128 texture sheet")
                if not _int_vector(part.get("clut_vram"), 2, 0, 1023) or part.get("palette_mapping") != "override0x10 and palette_mode2 without bit0x80 select full256-color row at x0; encoded row/column ignored":
                    raise ValueError("invalid selected SD palette mapping metadata")
            _read_png(root, f.get("png", {}), b[2:])
            _read_png(root, f.get("stp_png", {}), b[2:])
        seqs = c.get("sequences")
        if not isinstance(seqs, list) or not seqs or len(seqs) > 512 or not isinstance(seqs[0], dict) or seqs[0].get("sequence_index") != 0:
            raise ValueError("sequence zero missing")
        if any(not isinstance(seq, dict) or set(seq) != {"sequence_index", "source_pointer_offset", "source_offset", "source_size", "steps"} for seq in seqs):
            raise ValueError("sequence fields differ from reader contract")
        steps = seqs[0].get("steps")
        if (not isinstance(steps, list) or not steps or len(steps) > 128 or any(not isinstance(s,dict) or set(s) != {"frame_index", "duration"}
                or not _exact_int(s.get("frame_index"),0,32767) or s["frame_index"] not in seen
                or not _exact_int(s.get("duration"),1,32767) for s in steps)):
            raise ValueError("sequence zero must contain 1..128 supported steps referencing valid frames")
        seq_indexes = set()
        for seq in seqs:
            if (not isinstance(seq, dict) or not _exact_int(seq.get("sequence_index"),0,32767)
                    or seq["sequence_index"] in seq_indexes
                    or not _exact_int(seq.get("source_pointer_offset"),0,c["source"]["size"] - 1)
                    or not _exact_int(seq.get("source_offset"),0,c["source"]["size"])
                    or not _exact_int(seq.get("source_size"),1,c["source"]["size"])
                    or seq["source_offset"] + seq["source_size"] > c["source"]["size"]):
                raise ValueError("sequence native source extent is invalid")
            seq_indexes.add(seq["sequence_index"])
            seq_steps = seq.get("steps")
            if not isinstance(seq_steps, list) or len(seq_steps) > 1024:
                raise ValueError("sequence step list exceeds native reader bounds")
            for step in seq_steps:
                if (not isinstance(step, dict) or set(step) != {"frame_index", "duration"}
                        or not _exact_int(step.get("frame_index"),0,32767) or step["frame_index"] not in seen
                        or not _exact_int(step.get("duration"),1,32767)):
                    raise ValueError("sequence contains invalid s16 frame index/duration")
        palette_meta = c.get("palette")
        if (not isinstance(palette_meta, dict) or set(palette_meta) != {"mode", "source_words", "stp_policy"}
                or not _exact_int(palette_meta.get("mode"),0,255) or palette_meta.get("mode") != 2
                or not _int_vector(palette_meta.get("source_words"),256,0,65535)
                or palette_meta["source_words"] != frames[0]["source_palette_words"]
                or palette_meta.get("stp_policy") != "source palette load mode1 leaves STP unchanged; separate bit15 mask"):
            raise ValueError("selected SD palette metadata is invalid")
    return doc


def _validate_source(key, component):
    path, lba, size, digest = SOURCE_PINS[key]
    src = component.get("source", {})
    if not isinstance(src, dict):
        raise ValueError("source must be an object")
    expected_keys = {"path", "lba", "size", "sha256"} if key == "command_0_background" else {"path", "lba", "size", "sha256", "exe_sha256"}
    if set(src) != expected_keys:
        raise ValueError("source fields differ from v1 contract")
    if (src.get("path") != path or not _exact_int(src.get("lba"), 0, 2**31-1)
            or not _exact_int(src.get("size"), 1, 2**31-1)
            or src["lba"] != lba or src["size"] != size or src.get("sha256") != digest):
        raise ValueError("component source differs from reviewed original input pin")
    if key != "command_0_background" and src.get("exe_sha256") != EXPECTED_PROVENANCE["original_executable_sha256"]:
        raise ValueError("SD source does not bind the reviewed executable")


def _validate_native_route(component, command):
    route = component.get("native_route", {})
    if not isinstance(route, dict):
        raise ValueError("native route evidence must be an object")
    if set(route) != {"command", "actual_last_command", "actors", "profile", "evidence"}:
        raise ValueError("native route fields differ from v1 contract")
    if (not _exact_int(route.get("command"),0,7) or route["command"] != command
            or not _exact_int(route.get("actual_last_command"),0,7) or route["actual_last_command"] != command
            or not _int_vector(route.get("actors"),1,96,96) or route.get("profile") != NATIVE_ROUTE_PROFILE
            or not isinstance(route.get("evidence"), list) or not route["evidence"]):
        raise ValueError("component is not bound to reviewed native route evidence")
    if any(not isinstance(e, dict) or set(e) != {"path", "sha256", "size"}
           or not isinstance(e.get("path"), str) or ".." in PurePosixPath(e["path"]).parts
           or not isinstance(e.get("sha256"), str) or not SHA.fullmatch(e["sha256"])
           or not _exact_int(e.get("size"), 1, 100_000_000) for e in route["evidence"]):
        raise ValueError("native evidence references are malformed")
