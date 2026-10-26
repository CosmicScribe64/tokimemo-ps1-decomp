"""Export source-backed April school-day media for Command 4 (club activity).

This is an isolated T-29014 candidate covering all 11 non-summer club routes
(clubs 0..10) for opening-week solo activity (extra 0xF).
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import sys
import tempfile
from pathlib import Path

TRACK_SHA = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
EXE_SHA = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
SCHEMA = "tokimemo.club-command-content.v1"
MAX_PNG = 2 * 1024 * 1024
MAX_JSON = 64 * 1024 * 1024
NAME_PATTERN = re.compile(r"^[A-Za-z0-9][A-Za-z0-9_.-]{0,191}\.png$")

# Source-backed native tables from src/main/school_day.c and asm/data/data_800B5950.data.s
# D_800B6780: Background LBAs for clubs 0..10
CLUB_BACKGROUND_LBAS = (
    16215,  # Club 0: 文芸 (Literature) -> C_01_36.BS
    16224,  # Club 1: 演劇 (Drama)      -> C_01_37.BS
    16234,  # Club 2: 科学 (Science)    -> C_01_38.BS
    16244,  # Club 3: 電脳 (Computer)   -> C_01_39.BS
    16252,  # Club 4: 美術 (Art)        -> C_01_40.BS
    16260,  # Club 5: 吹奏楽 (Brass)    -> C_01_41.BS
    16168,  # Club 6: 野球 (Baseball)   -> C_01_31.BS
    16177,  # Club 7: サッカー (Soccer) -> C_01_32.BS
    16206,  # Club 8: テニス (Tennis)   -> C_01_35.BS
    16197,  # Club 9: 水泳 (Swimming)   -> C_01_34.BS
    16187,  # Club 10: バスケ (Basketball) -> C_01_33.BS
)

# D_800B68A8: SD set indices for non-summer [club_id][sd_variant]
# sd_variant 0 = companion (extra == 0), 1 = solo (extra != 0)
CLUB_SD_SELECT_NON_SUMMER = (
    (1, 3),    # Club 0:  SD_P0EW / SD_P0FW
    (30, 31),  # Club 1:  SD_P22  / SD_P23
    (5, 7),    # Club 2:  SD_P10W / SD_P11W
    (9, 11),   # Club 3:  SD_P12W / SD_P13W
    (27, 29),  # Club 4:  SD_P20W / SD_P21W
    (23, 25),  # Club 5:  SD_P1EW / SD_P1FW
    (12, 13),  # Club 6:  SD_P14  / SD_P15
    (14, 15),  # Club 7:  SD_P16  / SD_P17
    (20, 21),  # Club 8:  SD_P1C  / SD_P1D
    (18, 19),  # Club 9:  SD_P1A  / SD_P1B
    (16, 17),  # Club 10: SD_P18  / SD_P19
)

# D_800B6810: Base LBA and sector length for Club SD files
CLUB_SD_BASE_LBA = 27834
CLUB_SD_SECTORS = 41
CLUB_SD_EXTENT_BYTES = CLUB_SD_SECTORS * 2048  # 83968 bytes


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def resolve_club_route(
    tables: dict | None,
    club_id: int,
    extra: int = 15,
    month: int = 4,
    vacation: int = 0,
    health: int = 0,
) -> dict:
    """Resolve the native non-summer table entry for a club activity route."""
    if type(club_id) is not int or not (0 <= club_id <= 10):
        raise ValueError(f"club_id {club_id} is outside valid club domain 0..10 (club 11 is none)")
    if type(extra) is not int or extra != 15:
        raise ValueError("only opening-week solo variant (extra 0xF) is supported in this tranche")
    if type(month) is not int or month not in (3, 4, 5):
        raise ValueError("seasonal variant outside spring months 3-5 is not supported")
    if vacation != 0:
        raise ValueError("vacation content is unavailable")
    if health != 0:
        raise ValueError("sick/rest variants are not supported in this tranche")

    bg_table = CLUB_BACKGROUND_LBAS
    sd_table = CLUB_SD_SELECT_NON_SUMMER
    if isinstance(tables, dict):
        if "club_background_lbas" in tables:
            bg_table = tuple(tables["club_background_lbas"])
        if "club_sd_select_non_summer" in tables:
            sd_table = tuple(tuple(pair) for pair in tables["club_sd_select_non_summer"])

    if len(bg_table) != 11 or len(sd_table) != 11:
        raise ValueError("club tables do not match expected 11-club shape")

    variant = 1  # extra != 0 -> variant 1 (solo)
    sd_set = sd_table[club_id][variant]
    background_lba = bg_table[club_id]
    sd_lba = CLUB_SD_BASE_LBA + sd_set * CLUB_SD_SECTORS

    return {
        "command": 4,
        "club_id": club_id,
        "event_extra": extra,
        "variant": variant,
        "background_lba": background_lba,
        "sd_set_id": sd_set,
        "sd_lba": sd_lba,
        "sectors": CLUB_SD_SECTORS,
        "season_domain": "months 3–5 (non-summer table)",
        "route_domain": "ordinary school day, non-vacation, non-sick-rest",
    }


def _safe_output(root: Path, value: str) -> Path:
    supplied = Path(value).expanduser()
    if ".." in supplied.parts:
        raise ValueError("output path must not contain parent-directory segments")
    lexical = supplied if supplied.is_absolute() else root / supplied
    cache = root / ".asset-cache" / "port"
    try:
        lexical.relative_to(cache)
    except ValueError as exc:
        raise ValueError("output must be beneath repository .asset-cache/port") from exc
    cursor = root
    for part in lexical.relative_to(root).parts:
        cursor = cursor / part
        if cursor.is_symlink():
            raise ValueError("output path contains a symlink")
    out = lexical.resolve(strict=False)
    try:
        out.relative_to(cache.resolve(strict=False))
    except ValueError as exc:
        raise ValueError("resolved output escaped repository .asset-cache/port") from exc
    if out.exists():
        raise ValueError("output must be a new immutable directory")
    return out


def _copy_png(source: Path, destination: Path) -> dict:
    data = source.read_bytes()
    if not data.startswith(b"\x89PNG\r\n\x1a\n") or not data or len(data) > MAX_PNG:
        raise ValueError("reader output is missing, oversized, or not PNG")
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(data)
    return {"path": destination.name, "size": len(data), "sha256": _sha(data)}


def export_club_pack(
    input_path: str,
    output_path: str,
    month: int = 4,
) -> dict:
    root_value = os.environ.get("TOKIMEMO_ROOT")
    if root_value:
        root = Path(root_value).resolve()
    else:
        root = next(
            (p for p in Path(__file__).resolve().parents if (p / "tools/assets/input.py").is_file()),
            None,
        )
        if root is None:
            raise ValueError("repository root unavailable; set TOKIMEMO_ROOT")
    sys.path.insert(0, str(root))
    from tools.assets.input import open_input
    from tools.assets.image_packs_sd import export_sd_ui
    from tools.assets.image_mdec import export_mdec

    out = _safe_output(root, output_path)
    out.parent.mkdir(parents=True, exist_ok=True)
    stage = Path(tempfile.mkdtemp(prefix=".club-stage-", dir=out.parent))
    try:
        (stage / "assets").mkdir(parents=True)
        components = {}
        routes = []

        with open_input(input_path) as source:
            entries = source.entries()
            by_path = {e["path"]: e for e in entries if "path" in e}
            by_lba = {
                e.get("location", {}).get("lba"): e
                for e in entries
                if "location" in e and "lba" in e["location"]
            }

            exe_entry = by_path.get("SLPM_86.053")
            if not exe_entry:
                raise ValueError("executable SLPM_86.053 not found on disc")
            exe_data = source.read(exe_entry)
            if _sha(exe_data) != EXE_SHA:
                raise ValueError("executable hash does not match pinned EXE_SHA")

            for club_id in range(11):
                route = resolve_club_route(None, club_id, extra=15, month=month)
                bg_lba = route["background_lba"]
                sd_lba = route["sd_lba"]
                sd_set = route["sd_set_id"]

                bg_entry = by_lba.get(bg_lba)
                if not bg_entry:
                    raise ValueError(f"club {club_id} background LBA {bg_lba} not found on disc")
                sd_entry = by_lba.get(sd_lba)
                if not sd_entry:
                    raise ValueError(f"club {club_id} SD LBA {sd_lba} not found on disc")

                bg_data = source.read(bg_entry)
                sd_data = source.read(sd_entry)

                if len(sd_data) != CLUB_SD_EXTENT_BYTES:
                    raise ValueError(
                        f"club {club_id} SD extent is {len(sd_data)} bytes, expected {CLUB_SD_EXTENT_BYTES}"
                    )

                # Export background
                bg_id = f"command_4_club_{club_id}_bg"
                bg_dir = stage / "reader" / bg_id
                bg_context = {
                    "width": 320,
                    "height": 88,
                    "padded_height": 96,
                    "evidence": [
                        "src/main/school_day.c:get_weekly_bg_sector",
                        "src/main/school_day.c:school_day_screen_init",
                    ],
                }
                bg_result = export_mdec(
                    bg_data,
                    bg_dir,
                    bg_id,
                    source_path=bg_entry["path"],
                    context=bg_context,
                )
                bg_files = {
                    name: _copy_png(bg_dir / name, stage / "assets" / name)
                    for name in bg_result["files"]
                }

                # Export SD pack
                sd_id = f"command_4_club_{club_id}_sd"
                sd_dir = stage / "reader" / sd_id
                sd_result = export_sd_ui(
                    sd_data,
                    exe_data,
                    sd_dir,
                    sd_id,
                    source_entry=sd_entry,
                )
                if sd_result.get("status") != "exported":
                    raise ValueError(f"SD reader rejected club {club_id} pack")

                frames = []
                for frame in sd_result["images"]:
                    files = {
                        "png": _copy_png(sd_dir / frame["png"], stage / "assets" / frame["png"]),
                        "stp_png": _copy_png(sd_dir / frame["stp_png"], stage / "assets" / frame["stp_png"]),
                    }
                    frames.append(
                        {
                            "frame_index": frame["frame_index"],
                            "bounds": [*frame["bounds"][:2], frame["width"], frame["height"]],
                            "origin": frame["origin"],
                            "parts": frame["parts"],
                            "source_palette_words": frame["source_palette_words"],
                            **files,
                        }
                    )

                meta = sd_result["metadata"]
                if not meta.get("source_sequence_prefix"):
                    raise ValueError(f"club {club_id} SD has no decoded sequence prefix")

                key = f"club_{club_id}"
                components[key] = {
                    "route": route,
                    "background": {
                        "source": {
                            "path": bg_entry["path"],
                            "lba": bg_lba,
                            "size": len(bg_data),
                            "sha256": _sha(bg_data),
                        },
                        "geometry_context": bg_context,
                        "reader": "tools/assets/image_mdec.py:export_mdec",
                        "files": bg_files,
                    },
                    "sd": {
                        "source": {
                            "path": sd_entry["path"],
                            "lba": sd_lba,
                            "size": len(sd_data),
                            "sha256": _sha(sd_data),
                            "exe_sha256": EXE_SHA,
                        },
                        "source_id": sd_id,
                        "profile_index": sd_set,
                        "frames": frames,
                        "sequences": meta["source_sequence_prefix"],
                        "palette_mode": meta["actor_runtime_palette_mode"],
                        "stp_policy": meta["stp_policy"],
                        "reader": "tools/assets/image_packs_sd.py:export_sd_ui",
                    },
                }
                routes.append(route)

        manifest = {
            "schema": SCHEMA,
            "version": 1,
            "provenance": {
                "original_input_sha256": TRACK_SHA,
                "original_executable_sha256": EXE_SHA,
                "readers": [
                    "tools/assets/input.py:open_input",
                    "tools/assets/image_packs_sd.py:export_sd_ui",
                    "tools/assets/image_mdec.py:export_mdec",
                ],
            },
            "route_capabilities": [f"command_4_club_{i}_april_school_day" for i in range(11)],
            "unsupported": {
                "club_none": "club_id 11 (CLUB_NONE) has no club activity",
                "commands_0_1_2_3_5_6_7": "non-club commands not included",
                "seasonal": "only non-summer table; summer selector variants not included",
                "vacation": "not included",
                "sick_rest": "not included",
                "conditional_companion_variants": "only opening-week event extra 0xF; companion variants not included",
                "scene_composition": "not established",
                "full_route": "not claimed",
            },
            "routes": routes,
            "components": components,
        }

        manifest_raw = (
            json.dumps(manifest, sort_keys=True, separators=(",", ":")) + "\n"
        ).encode("utf-8")
        (stage / "manifest.json").write_bytes(manifest_raw)

        shutil.rmtree(stage / "reader", ignore_errors=True)
        if out.exists():
            raise ValueError("output appeared during export; refusing overwrite")
        os.rename(stage, out)
        manifest_sha = _sha((out / "manifest.json").read_bytes())
        return {
            "path": str(out),
            "manifest_sha256": manifest_sha,
            "route_capabilities": manifest["route_capabilities"],
        }
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise


def validate_pack(pack_path: Path | str, expected_manifest_sha256: str | None = None) -> dict:
    """Validate pack structure, integrity, and non-empty assets."""
    pack = Path(pack_path).resolve()
    manifest_file = pack / "manifest.json"
    if not manifest_file.is_file():
        raise ValueError("manifest.json missing in pack")
    raw = manifest_file.read_bytes()
    if expected_manifest_sha256 and _sha(raw) != expected_manifest_sha256:
        raise ValueError("manifest sha256 mismatch")
    doc = json.loads(raw.decode("utf-8"))
    if doc.get("schema") != SCHEMA or doc.get("version") != 1:
        raise ValueError(f"invalid schema or version: {doc.get('schema')}")
    components = doc.get("components", {})
    if len(components) != 11:
        raise ValueError(f"expected 11 club components, got {len(components)}")
    assets_dir = pack / "assets"
    if not assets_dir.is_dir():
        raise ValueError("assets directory missing")
    for key, comp in components.items():
        bg = comp.get("background", {})
        for fname, file_spec in bg.get("files", {}).items():
            p = assets_dir / file_spec["path"]
            if not p.is_file():
                raise ValueError(f"background PNG missing: {p}")
            data = p.read_bytes()
            if len(data) != file_spec["size"] or _sha(data) != file_spec["sha256"]:
                raise ValueError(f"background PNG corrupted: {p}")
        sd = comp.get("sd", {})
        for frame in sd.get("frames", []):
            for role in ("png", "stp_png"):
                file_spec = frame[role]
                p = assets_dir / file_spec["path"]
                if not p.is_file():
                    raise ValueError(f"SD PNG missing: {p}")
                data = p.read_bytes()
                if len(data) != file_spec["size"] or _sha(data) != file_spec["sha256"]:
                    raise ValueError(f"SD PNG corrupted: {p}")
    return doc


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, help="original MODE2 track or disc container")
    parser.add_argument("--output", required=True, help="new directory under ignored .asset-cache/port")
    parser.add_argument("--month", type=int, default=4, help="month (default: 4)")
    args = parser.parse_args()
    result = export_club_pack(args.input, args.output, month=args.month)
    print(json.dumps(result, sort_keys=True))


if __name__ == "__main__":
    main()
