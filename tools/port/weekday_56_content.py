"""Immutable original-media capability pack for bounded spring commands5/6.

No emulator, gameplay-state mutation, alternate scene, or fallback art is used.
Run in Docker; originals and all generated content remain external to res://.
"""

import argparse
import hashlib
import json
import os
import re
import shutil
import struct
import sys
import tempfile
from pathlib import Path

if __package__ in (None, ""):
    sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from tools.assets import weekday_56_profile as profile
from tools.assets.input import open_input
from tools.assets.image_mdec import export_mdec
from tools.assets.image_packs_sd import export_sd_ui
from tools.assets.media_mdec_export import _native_cd_read_extent
from tools.assets.visual_text_equivalence import _decode_png
from tools.port.spring_command_content import _safe_output, _copy_png

SCHEMA = "tokimemo.weekday-56-content.v1"
MANIFEST_SHA = "8cbe8f616abae286b37725c44eb54f40aee4a5839fe7d2fb65b788244ce0fad4"
MAX_JSON = 8 * 1024 * 1024
MAX_PNG = 2 * 1024 * 1024
NAME = re.compile(r"^[A-Za-z0-9][A-Za-z0-9_.-]{0,191}\.png$")


def _sha(data):
    return hashlib.sha256(data).hexdigest()


def repository_root():
    return next(
        p for p in Path(__file__).resolve().parents if (p / "tools/assets/input.py").is_file()
    )


def _build_manifest(source, entries, stage, root):
    proved = profile.build_profile(source, entries, root)
    by_path = {e["path"]: e for e in entries}
    exe = source.read(by_path["SLPM_86.053"])
    components = {}
    for route in proved["routes"]:
        command = route["command"]
        component = {"route": route}
        for role in ("background", "sd"):
            asset = next(a for a in profile.ASSETS if a["command"] == command and a["role"] == role)
            entry = by_path[asset["path"]]
            data = source.read(entry)
            raw = stage / "reader" / asset["id"]
            if role == "background":
                context = {
                    "width": 320,
                    "height": 88,
                    "padded_height": 96,
                    "geometry": proved["geometry"],
                    "source_profile": proved,
                    "evidence": [
                        "school_day.c:get_weekly_bg_sector/school_day_background_read/school_day_screen_init -> dec_bg_decode_to_vram(0)"
                    ],
                }
                exported = export_mdec(
                    data, raw, asset["id"], source_path=asset["path"], context=context
                )
                images = {n: _copy_png(raw / n, stage / "assets" / n) for n in exported["files"]}
                image = exported["images"][0]
                padded = exported["metadata"]["padded_decode"]
                component[role] = {
                    "source": {
                        "path": asset["path"],
                        "lba": asset["lba"],
                        "size": len(data),
                        "sha256": _sha(data),
                    },
                    "geometry_context": context,
                    "native_read_extent": _native_cd_read_extent(
                        source, entry, profile.TRACK_SHA256, 10
                    ),
                    "active_size": [320, 88],
                    "padded_size": [320, 96],
                    "image": images[image["png"]],
                    "stp_mask": images[image["stp_png"]],
                    "padded_image": images[padded["png"]],
                    "padded_stp_mask": images[padded["stp_png"]],
                    "decode_bounds": {
                        k: exported["metadata"][k]
                        for k in (
                            "macroblock_count",
                            "block_count",
                            "consumed_stream_bits",
                            "end_marker_bit_offset",
                            "source_size",
                        )
                    },
                    "decoder_model": exported["metadata"]["decoder_model"], "reader": "tools/assets/image_mdec.py:export_mdec",
                }
            else:
                exported = export_sd_ui(data, exe, raw, asset["id"], source_entry=entry)
                if exported.get("status") != "exported":
                    raise ValueError("weekday56 SD reader refused source")
                meta = exported["metadata"]
                caller = meta["caller_profile"]
                if (
                    caller["family"] != "weekly"
                    or caller["index"] != route["sd_profile"]
                    or caller["source_lba"] != route["sd_lba"]
                ):
                    raise ValueError("weekday56 SD caller/layout mismatch")
                frames = []
                for f in exported["images"]:
                    frames.append(
                        {
                            "frame_index": f["frame_index"],
                            "bounds": [*f["bounds"][:2], f["width"], f["height"]],
                            "origin": f["origin"],
                            "parts": f["parts"],
                            "source_palette_words": f["source_palette_words"],
                            "png": _copy_png(raw / f["png"], stage / "assets" / f["png"]),
                            "stp_png": _copy_png(
                                raw / f["stp_png"], stage / "assets" / f["stp_png"]
                            ),
                        }
                    )
                if (
                    not frames
                    or not meta["source_sequence_prefix"]
                    or meta["source_sequence_prefix"][0]["sequence_index"] != 0
                ):
                    raise ValueError("weekday56 required frame/sequence0 missing")
                component[role] = {
                    "source_id": asset["id"],
                    "source": {
                        "path": asset["path"],
                        "lba": asset["lba"],
                        "size": len(data),
                        "sha256": _sha(data),
                        "exe_sha256": profile.EXE_SHA256,
                    },
                    "profile_index": route["sd_profile"],
                    "caller_profile": caller,
                    "frames": frames,
                    "sequences": meta["source_sequence_prefix"],
                    "palette": {
                        "source_words": exported["images"][0]["source_palette_words"],
                        "mode": meta["actor_runtime_palette_mode"],
                        "override": meta["actor_runtime_palette_override"],
                        "queue_mode": meta["palette_queue_mode"],
                        "stp_policy": meta["stp_policy"],
                    },
                    "actor_contract": {
                        "id": 96,
                        "anchor": [-80, -64],
                        "flags": 5,
                        "draw_flags": 132,
                        "sequence_slot": 0,
                    },
                    "reader": "tools/assets/image_packs_sd.py:export_sd_ui",
                }
        components["command_%d" % command] = component
    return {
        "schema": SCHEMA,
        "version": 1,
        "provenance": {
            "original_input_sha256": profile.TRACK_SHA256,
            "original_executable_sha256": profile.EXE_SHA256,
            "source_profile": proved,
        },
        "route_capabilities": [
            "command_5_spring_scene0_extra15",
            "command_6_spring_scene0_extra15",
        ],
        "unsupported": profile.UNSUPPORTED,
        "components": components,
    }


def _no_link(path):
    current = Path(path.anchor)
    for part in path.parts[1:]:
        current = current / part
        if current.is_symlink():
            raise ValueError("weekday56 path contains a symlink")


def _read_png(root, spec, size):
    if (
        not isinstance(spec, dict)
        or set(spec) != {"path", "size", "sha256"}
        or not isinstance(spec["path"], str)
        or not NAME.fullmatch(spec["path"])
    ):
        raise ValueError("weekday56 PNG descriptor/path invalid")
    path = root / "assets" / spec["path"]
    if path.is_symlink() or not path.is_file() or path.stat().st_size > MAX_PNG:
        raise ValueError("weekday56 PNG missing/linked/oversized")
    with path.open("rb") as stream:
        data = stream.read(MAX_PNG + 1)
    if len(data) != spec["size"] or _sha(data) != spec["sha256"]:
        raise ValueError("weekday56 PNG bytes differ")
    if (
        len(data) < 24
        or data[:8] != b"\x89PNG\r\n\x1a\n"
        or list(struct.unpack_from(">II", data, 16)) != size
        or size[0] * size[1] > 4 * 1024 * 1024
    ):
        raise ValueError("weekday56 PNG dimensions invalid")
    width, height, _ = _decode_png(data)
    if [width, height] != size:
        raise ValueError("weekday56 decoded PNG dimensions differ")


def validate_pack(pack):
    supplied = Path(pack).absolute()
    if ".." in supplied.parts:
        raise ValueError("weekday56 pack parent-directory segments are forbidden")
    _no_link(supplied)
    root = supplied.resolve()
    project = repository_root() / "port/godot"
    if root == project or project in root.parents:
        raise ValueError("weekday56 pack must remain outside the Godot project")
    f = root / "manifest.json"
    if f.is_symlink() or not f.is_file() or f.stat().st_size > MAX_JSON:
        raise ValueError("weekday56 manifest missing/linked/oversized")
    with f.open("rb") as stream:
        data = stream.read(MAX_JSON + 1)
    if len(data) > MAX_JSON:
        raise ValueError("weekday56 manifest is oversized")
    if _sha(data) != MANIFEST_SHA:
        raise ValueError("weekday56 manifest is not the reviewed exact source export")
    doc = json.loads(data)
    if (
        doc["schema"] != SCHEMA
        or doc["version"] != 1
        or set(doc["components"]) != {"command_5", "command_6"}
    ):
        raise ValueError("weekday56 schema/components differ")
    if (root / "assets").is_symlink() or not (root / "assets").is_dir():
        raise ValueError("weekday56 assets directory invalid")
    for c in doc["components"].values():
        bg = c["background"]
        for key, size in (
            ("image", [320, 88]),
            ("stp_mask", [320, 88]),
            ("padded_image", [320, 96]),
            ("padded_stp_mask", [320, 96]),
        ):
            _read_png(root, bg[key], size)
        for frame in c["sd"]["frames"]:
            _read_png(root, frame["png"], frame["bounds"][2:])
            _read_png(root, frame["stp_png"], frame["bounds"][2:])
    return doc


def export_pack(input_path, output_path, month=4, extra=15, scene=0, vacation=0, health=0):
    for command in (5, 6):
        error = profile.request_error(command, month, extra, scene, vacation, health)
        if error:
            raise ValueError(error)
    root = repository_root()
    out = _safe_output(root, output_path)
    out.parent.mkdir(parents=True, exist_ok=True)
    stage = Path(tempfile.mkdtemp(prefix=".weekday56-", dir=out.parent))
    try:
        with open_input(input_path) as source:
            doc = _build_manifest(source, source.entries(), stage, root)
        (stage / "manifest.json").write_text(
            json.dumps(doc, sort_keys=True, separators=(",", ":")) + "\n"
        )
        validate_pack(stage)
        if out.exists():
            raise ValueError("weekday56 immutable destination appeared during export")
        os.rename(stage, out)
        return {
            "path": str(out),
            "schema": SCHEMA,
            "manifest_sha256": _sha((out / "manifest.json").read_bytes()),
        }
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True)
    parser.add_argument("--output", required=True)
    for name, default in (
        ("month", 4),
        ("event-extra", 15),
        ("event-scene", 0),
        ("vacation", 0),
        ("health-nibble", 0),
    ):
        parser.add_argument("--" + name, type=int, default=default)
    args = parser.parse_args()
    print(
        json.dumps(
            export_pack(
                args.input,
                args.output,
                args.month,
                args.event_extra,
                args.event_scene,
                args.vacation,
                args.health_nibble,
            ),
            sort_keys=True,
        )
    )


if __name__ == "__main__":
    main()
