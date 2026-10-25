"""Finite importer for source-proved school-day command 0 and 7 media.

The importer re-reads the original MODE2 input through tools.assets.input and
uses the checked-in SD and MDEC readers. The output is data-only and stays in
ignored .asset-cache/port. It does not claim complete route or scene coverage.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import sys
import tempfile
from pathlib import Path, PurePosixPath

SCHEMA = "tokimemo.school-day-media.v2"
MEDIA_SHA = "0c85bbc54be82c7b2597528ccbfc915cd1fb5f99405bd18d845452026b56be37"
NATIVE_PROFILE_SHA = "ac246a30ed894bf5b631beaf4f053a8622324094859b11a65a4c0c03d55cb4a3"
TRACK_SHA = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
EXE_SHA = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
MAX_JSON = 64 * 1024 * 1024  # pinned reviewed source manifest is 47.9 MiB
MAX_PNG = 2 * 1024 * 1024
SELECTED = {
    "CDROM/MAIN_SD/SD_P01W.NBN": (3, 0),
    "CDROM/MAIN_SD/SD_P09W.NBN": (16, 7),
}
BG_PATH = "CDROM/BS/C_01_27.BS"
COMMAND7_BG_PATH = "CDROM/BS/C_01_44.BS"


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _root() -> Path:
    value = os.environ.get("TOKIMEMO_ROOT")
    if value:
        return Path(value).resolve()
    for p in Path(__file__).resolve().parents:
        if (p / "tools/assets/input.py").is_file():
            return p
    raise ValueError("repository root unavailable; set TOKIMEMO_ROOT for candidate runs")


def _json(path: Path):
    if path.is_symlink() or not path.is_file() or path.stat().st_size > MAX_JSON:
        raise ValueError("unsafe or oversized JSON input: " + str(path))
    return json.loads(path.read_text(encoding="utf-8"))


def _no_symlink_path(path: Path) -> Path:
    path = path.absolute()
    current = Path(path.anchor)
    for part in path.parts[1:]:
        current = current / part
        if current.is_symlink():
            raise ValueError("input path contains a symlink: " + str(current))
    return path


def _safe_copy(src: Path, dest: Path) -> dict:
    data = src.read_bytes()
    if not data or len(data) > MAX_PNG or not data.startswith(b"\x89PNG\r\n\x1a\n"):
        raise ValueError("missing, oversized, or non-PNG reader output")
    dest.parent.mkdir(parents=True, exist_ok=True)
    dest.write_bytes(data)
    return {"path": dest.name, "size": len(data), "sha256": _sha(data)}


def _validate_command7_route(route):
    """Bind this finite profile to the reviewed ordinary first-rest event."""
    scalar_fields = {"command": 7, "actual_last_command": 7, "sequence_slot": 0, "sd_set_id": 16}
    if not isinstance(route, dict) or any(type(route.get(k)) is not int or route[k] != v for k, v in scalar_fields.items()):
        raise ValueError("command-7 native event/selector differs from the reviewed ordinary route")
    event = route.get("first_day_event")
    actors = route.get("actors")
    if (not isinstance(event, list) or event != [0, 7, 0, 15] or any(type(v) is not int for v in event)
            or not isinstance(actors, list) or actors != [96] or any(type(v) is not int for v in actors)):
        raise ValueError("command-7 native event/selector differs from the reviewed ordinary route")


def import_pack(input_path: str, media_path: str, native_profile_path: str, output_path: str) -> dict:
    root = _root()
    sys.path.insert(0, str(root))
    from tools.assets.input import open_input
    from tools.assets.image_packs_sd import export_sd_ui
    from tools.assets.image_mdec import export_mdec
    from tools.assets.bs_cohort_profile import build_profile_context
    from tools.assets.media_mdec_export import _native_cd_read_extent

    media_file = _no_symlink_path(Path(media_path))
    native_file = _no_symlink_path(Path(native_profile_path))
    if not media_file.is_file() or media_file.stat().st_size > MAX_JSON:
        raise ValueError("reviewed media manifest is unsafe or exceeds the fixed bound")
    media_bytes = media_file.read_bytes()
    if _sha(media_bytes) != MEDIA_SHA:
        raise ValueError("school-day media source manifest is not the reviewed publication")
    if not native_file.is_file() or native_file.stat().st_size > MAX_JSON:
        raise ValueError("native profile is unsafe or oversized")
    native_bytes = native_file.read_bytes()
    if _sha(native_bytes) != NATIVE_PROFILE_SHA:
        raise ValueError("native command profile is not the reviewed source profile")
    native = json.loads(native_bytes)
    if not isinstance(native, dict):
        raise ValueError("native profile must be an object")
    native_routes = {r.get("command"): r for r in native.get("routes", []) if isinstance(r, dict)}
    if native.get("source_executable_sha256") != EXE_SHA:
        raise ValueError("native route profile does not pin the reviewed executable")
    media = _json(media_file)
    if media.get("schema") != "tokimemo.media.v1" or media.get("input", {}).get("source_sha256") != TRACK_SHA:
        raise ValueError("source media manifest schema/input mismatch")
    original = {a["source"]["path"]: a for a in media.get("assets", []) if isinstance(a, dict) and isinstance(a.get("source"), dict)}
    out = _no_symlink_path(Path(output_path))
    if ".." in out.parts:
        raise ValueError("output path must not contain parent-directory segments")
    out = out.resolve()
    cache = root / ".asset-cache/port"
    if cache.is_symlink() or cache not in out.parents or out.exists():
        raise ValueError("output must be a new immutable directory beneath .asset-cache/port")
    for parent in (cache, *out.parents):
        if parent == root.parent:
            break
        if parent.is_symlink():
            raise ValueError("output path contains a symlink")
    out.parent.mkdir(parents=True, exist_ok=True)
    stage = Path(tempfile.mkdtemp(prefix=".school-day-media-", dir=out.parent))
    try:
        stage_assets = stage / "assets"
        with open_input(input_path) as source:
            desc = source.describe()
            if desc.get("source_sha256") != TRACK_SHA or desc.get("format") != "mode2-2352":
                raise ValueError("original source is not the reviewed MODE2 track")
            entries = source.entries()
            by_path = {e.get("path"): e for e in entries if isinstance(e, dict)}
            exe_entry = by_path.get("SLPM_86.053")
            if exe_entry is None:
                raise ValueError("source executable is missing")
            exe = source.read(exe_entry)
            if _sha(exe) != EXE_SHA:
                raise ValueError("original executable hash mismatch")
            components = {}
            for path, (profile_index, command) in SELECTED.items():
                entry = by_path.get(path)
                accepted = original.get(path)
                if entry is None or accepted is None:
                    raise ValueError("selected SD source absent from fresh inventory/publication")
                data = source.read(entry)
                src_meta = accepted["source"]
                if len(data) != src_meta["size"] or _sha(data) != src_meta["sha256"]:
                    raise ValueError("fresh SD source differs from reviewed identity")
                route = native_routes.get(command)
                route_assets = route.get("assets", []) if isinstance(route, dict) else []
                native_asset = next((a for a in route_assets if a.get("path") == path and a.get("role") == "sd"), None)
                if (not route or route.get("actual_last_command") != command or route.get("actors") != [96]
                        or not native_asset or native_asset.get("lba") != src_meta["location"]["lba"]
                        or native_asset.get("sha256") != _sha(data) or native_asset.get("size") != len(data)):
                    raise ValueError("selected SD does not match the fresh-verified native route profile")
                aid = accepted["id"]
                raw_dir = stage / "reader" / aid
                exported = export_sd_ui(data, exe, raw_dir, aid, source_entry=entry)
                if exported.get("status") != "exported":
                    raise ValueError("existing SD reader did not export selected source")
                metadata = exported["metadata"]
                caller = metadata.get("caller_profile", {})
                if caller.get("index") != profile_index or caller.get("source_lba") != src_meta["location"]["lba"]:
                    raise ValueError("SD caller profile/LBA does not match reviewed route source")
                # Keep every physical frame so the consumer can resolve validated sequence indices.
                frame_list = []
                for frame in exported["images"]:
                    png = _safe_copy(raw_dir / frame["png"], stage_assets / frame["png"])
                    mask = _safe_copy(raw_dir / frame["stp_png"], stage_assets / frame["stp_png"])
                    frame_list.append({
                        "frame_index": frame["frame_index"], "id": f"{aid}:frame:{frame['frame_index']}",
                        "png": png, "stp_png": mask,
                        "bounds": [frame["bounds"][0], frame["bounds"][1], frame["width"], frame["height"]],
                        "origin": frame["origin"], "parts": frame["parts"],
                        "source_palette_words": frame["source_palette_words"],
                    })
                if not frame_list:
                    raise ValueError("selected SD has no physical frames")
                sequences = metadata.get("source_sequence_prefix", [])
                if not sequences or sequences[0].get("sequence_index") != 0:
                    raise ValueError("selected sequence zero is not in the validated pointer prefix")
                components[f"command_{command}_sd"] = {
                    "kind": "sd_animation", "id": aid, "source": {
                        "path": path, "lba": src_meta["location"]["lba"], "size": len(data),
                        "sha256": _sha(data), "exe_sha256": _sha(exe),
                    }, "profile_index": profile_index, "sequence_index": 0,
                    "native_route": {"command": command, "actual_last_command": route["actual_last_command"],
                                     "actors": route["actors"], "profile": native["profile"],
                                     "evidence": route.get("evidence", [])},
                    "frames": frame_list, "sequences": sequences,
                    "palette": {"source_words": metadata["source_sequence_prefix"] and exported["images"][0]["source_palette_words"],
                                "mode": metadata["actor_runtime_palette_mode"], "stp_policy": metadata["stp_policy"]},
                    "reader": "tools/assets/image_packs_sd.py:export_sd_ui",
                    "capability": "validated_static_sd_frames_and_sequence_prefix",
                }

            # Each built-in profile rechecks current C/ASM, EXE/table and raw ISO records.
            for command, bg_path, profile_kind in ((0, BG_PATH, "weekly"), (7, COMMAND7_BG_PATH, "command7")):
                expected_lba = 0x3F03 if command == 0 else 0x3F9B
                if command == 7:
                    _validate_command7_route(native_routes.get(7))
                profile_context = build_profile_context(source, entries, root, profile_kind)
                bg_rec = next((x for x in profile_context["assets"] if x.get("path") == bg_path), None)
                if bg_rec is None:
                    raise ValueError("background not found in freshly checked profile")
                bg_entry = by_path.get(bg_path)
                bg_data = source.read(bg_entry)
                if _sha(bg_data) != bg_rec["sha256"] or bg_rec["source"]["location"]["lba"] != expected_lba:
                    raise ValueError("background identity/LBA mismatch")
                native_bg = next((a for a in native_routes[command]["assets"] if a.get("path") == bg_path and a.get("role") == "background"), None)
                if (native_routes[command].get("actual_last_command") != command or native_routes[command].get("actors") != [96]
                        or not native_bg or native_bg.get("lba") != expected_lba or native_bg.get("sha256") != _sha(bg_data)
                        or native_bg.get("size") != len(bg_data)):
                    raise ValueError("background does not match native command source profile")
                context = {
                    "width": 320, "height": 88, "padded_height": 96,
                    "source_profile": profile_context["source_profile"],
                    "geometry": profile_context["geometry"],
                    "evidence": bg_rec["source_evidence"],
                }
                bg_id = original[bg_path]["id"]
                bg_raw = stage / "reader" / bg_id
                bg_result = export_mdec(bg_data, bg_raw, bg_id, source_path=bg_path, context=context)
                img = bg_result["images"][0]
                bgfiles = {f: _safe_copy(bg_raw / f, stage_assets / f) for f in bg_result["files"]}
                components[f"command_{command}_background"] = {
                    "kind": "mdec_background", "id": bg_id,
                    "source": {"path": bg_path, "lba": expected_lba, "size": len(bg_data), "sha256": _sha(bg_data)},
                    "active_size": [320, 88], "padded_size": [320, 96], "active_crop": [0, 0, 320, 88],
                    "native_route": {"command": command, "actual_last_command": native_routes[command]["actual_last_command"],
                                     "actors": native_routes[command]["actors"], "profile": native["profile"],
                                     "evidence": native_routes[command].get("evidence", [])},
                    "image": bgfiles[img["png"]], "stp_mask": bgfiles[img["stp_png"]],
                    "padded_image": bgfiles[bg_result["metadata"]["padded_decode"]["png"]],
                    "padded_stp_mask": bgfiles[bg_result["metadata"]["padded_decode"]["stp_png"]],
                    "geometry_context": context, "reader": "tools/assets/image_mdec.py:export_mdec",
                    "capability": "source_profiled_static_background_preview",
                }
                if command == 7:
                    # The extra native sector is proved separately and never enters export_mdec.
                    components["command_7_background"]["native_read_extent"] = _native_cd_read_extent(
                        source, bg_entry, TRACK_SHA, 10)

        manifest = {
            "schema": SCHEMA, "version": 2,
            "provenance": {"original_input_sha256": TRACK_SHA, "original_executable_sha256": EXE_SHA,
                           "reviewed_media_manifest_sha256": MEDIA_SHA, "native_profile_sha256": NATIVE_PROFILE_SHA,
                           "readers": ["tools/assets/input.py:open_input", "tools/assets/image_packs_sd.py:export_sd_ui",
                                       "tools/assets/bs_cohort_profile.py:build_profile_context", "tools/assets/image_mdec.py:export_mdec"]},
            "route_capabilities": ["command_0_static_background", "command_0_sd_sequence_0", "command_7_sd_sequence_0", "command_7_static_background"],
            "unsupported": {"scene_composition": "runtime placement/compositing not established by this media adapter",
                            "full_route": "not claimed"},
            "components": components,
        }
        (stage / "manifest.json").write_text(json.dumps(manifest, sort_keys=True, separators=(",", ":")) + "\n", encoding="utf-8")
        from tools.port.validate_school_day_media import validate_pack
        validate_pack(str(stage))
        # Publish with same-filesystem rename; never replace an existing pack.
        if out.exists():
            raise ValueError("destination appeared during import; refusing overwrite")
        os.rename(stage, out)
        return {"path": str(out), "manifest_sha256": _sha((out / "manifest.json").read_bytes()),
                "components": sorted(components), "schema": SCHEMA}
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--input", required=True)
    p.add_argument("--media-manifest", required=True)
    p.add_argument("--native-profile", required=True)
    p.add_argument("--output", required=True)
    a = p.parse_args()
    print(json.dumps(import_pack(a.input, a.media_manifest, a.native_profile, a.output), sort_keys=True))


if __name__ == "__main__":
    main()
