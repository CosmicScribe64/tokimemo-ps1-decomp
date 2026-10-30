"""Export source-backed seasonal, condition, and vacation media variants for ordinary commands 1–6 (and 0, 7).

This exporter covers:
- Summer variants (is_summer == True, months 6..9) for commands 1-3, 5-6, 0, 7, and command 4 (clubs 0..10).
- Vacation variants (vacation_kind in [1, 2, 3]) for summer and non-summer.
- Condition / sick variants (condition_bucket == 1) across all four seasons.
All assets are extracted directly from original PS1 disc tracks with SHA-256 provenance.
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
SCHEMA = "tokimemo.seasonal-command-content.v1"
MAX_PNG = 2 * 1024 * 1024
MAX_JSON = 64 * 1024 * 1024
NAME_PATTERN = re.compile(r"^[A-Za-z0-9][A-Za-z0-9_.-]{0,191}\.png$")

# Source-backed native tables from src/main/school_day.c, src/main/holiday.c, and asm/data/data_800B5950.data.s

# D_800B6760: Weekday background LBAs for commands 0..7 (non-club)
WEEKDAY_BG_LBAS = (
    16131,  # 0: C_01_22.BS
    16141,  # 1: C_01_23.BS
    16151,  # 2: C_01_24.BS
    16160,  # 3: C_01_25.BS
    16292,  # 4: C_01_30.BS (unused for active club, clubs use D_800B6780)
    16268,  # 5: C_01_42.BS
    16277,  # 6: C_01_43.BS
    16283,  # 7: C_01_44.BS
)

# D_800B6780: Background LBAs for clubs 0..10
CLUB_BG_LBAS = (
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

# D_800B5950: Weekday room background for sick player (condition_bucket == 1, cmd 7) by season_index 0..3
WEEKDAY_SICK_BG_LBAS = (
    16361,  # Spring (months 3..5)  -> C_02_02H.BS
    16370,  # Summer (months 6..8)  -> C_02_02N.BS
    16343,  # Autumn (months 9..11) -> C_02_02A.BS
    16352,  # Winter (months 12..2) -> C_02_02F.BS
)

# D_800B5960: Vacation room background for sick player (condition_bucket == 1, cmd 7) by season_index 0..3
VACATION_SICK_BG_LBAS = (
    16318,  # Spring -> C_02_01H.BS
    16326,  # Summer -> C_02_01N.BS
    16301,  # Autumn -> C_02_01A.BS
    16310,  # Winter -> C_02_01F.BS
)

# holiday_activity_bg_sectors (src/main/holiday.c): Vacation background LBAs for commands 0..7
HOLIDAY_ACTIVITY_BG_LBAS = (
    16379,  # 0: C_02_03.BS
    16379,  # 1: C_02_03.BS
    16379,  # 2: C_02_03.BS
    16387,  # 3: C_29_02.BS
    16292,  # 4: (club uses CLUB_BG_LBAS)
    16396,  # 5: C_29_10.BS
    16277,  # 6: C_01_43.BS
    16283,  # 7: C_01_44.BS
)

# D_800B67AC: Base LBA for weekday SD sets (commands 0..3, 5..7)
WEEKDAY_SD_BASE_LBA = 42418  # 0xA5B2
WEEKDAY_SD_SECTORS = 41

# D_800B6810: Base LBA for club SD sets (clubs 0..10)
CLUB_SD_BASE_LBA = 27834  # 0x6CBA
CLUB_SD_SECTORS = 41

# holiday_sd_set_sectors (src/main/holiday.c): LBAs for vacation SD sets 0..12
HOLIDAY_SD_SECTORS_TABLE = (
    41446,  # 0:  SD_P24.NBN  (0xA1E6)
    41487,  # 1:  SD_P24W.NBN (0xA20F)
    41528,  # 2:  SD_P25.NBN  (0xA238)
    41569,  # 3:  SD_P25W.NBN (0xA261)
    41610,  # 4:  SD_P26.NBN  (0xA28A)
    41651,  # 5:  SD_P26W.NBN (0xA2B3)
    41692,  # 6:  SD_P27.NBN  (0xA2DC)
    41733,  # 7:  SD_P27W.NBN (0xA305)
    41774,  # 8:  SD_P28.NBN  (0xA32E)
    41815,  # 9:  SD_P28W.NBN (0xA357)
    41856,  # 10: SD_P29.NBN  (0xA380)
    41897,  # 11: SD_P29W.NBN (0xA3A9)
    42910,  # 12: SD_P06.NBN  (0xA79E)
)

# D_800B68C0: Summer SD set table for commands 0..3, 5..7 (index: 2*cmd + variant; for 5: 9+variant; 6: 11+variant; 7: 13)
# For solo (variant 1):
# cmd 0: D_800B68C0[1] = 2
# cmd 1: D_800B68C0[3] = 6
# cmd 2: D_800B68C0[5] = 10
# cmd 3: D_800B68C0[7] = 13
# cmd 5: D_800B68C0[10] = 23
# cmd 6: D_800B68C0[12] = 19
# cmd 7: D_800B68C0[13] = 15
SUMMER_WEEKDAY_SOLO_SD_SETS = {
    0: 2,
    1: 6,
    2: 10,
    3: 13,
    5: 23,
    6: 19,
    7: 15,
}

# D_800B68D0: Non-summer SD set table for commands 0..3, 5..7 (solo variant 1)
NON_SUMMER_WEEKDAY_SOLO_SD_SETS = {
    0: 3,
    1: 7,
    2: 11,
    3: 13,
    5: 24,
    6: 20,
    7: 16,
}

# D_800B6890: Summer Club SD set table [club_id][variant]
# Column 1 = solo (extra != 0)
SUMMER_CLUB_SOLO_SD_SETS = (
    2,   # Club 0:  SD_P0F.NBN (27916)
    31,  # Club 1:  SD_P23.NBN (29105)
    6,   # Club 2:  SD_P11.NBN (28080)
    10,  # Club 3:  SD_P13.NBN (28244)
    28,  # Club 4:  SD_P21.NBN (28982)
    24,  # Club 5:  SD_P1F.NBN (28818)
    13,  # Club 6:  SD_P15.NBN (28367)
    15,  # Club 7:  SD_P17.NBN (28449)
    21,  # Club 8:  SD_P1D.NBN (28695)
    19,  # Club 9:  SD_P1B.NBN (28613)
    17,  # Club 10: SD_P19.NBN (28531)
)

# D_800B68A8: Non-summer Club SD set table [club_id][variant]
NON_SUMMER_CLUB_SOLO_SD_SETS = (
    3,   # Club 0:  SD_P0FW.NBN (27957)
    31,  # Club 1:  SD_P23.NBN  (29105)
    7,   # Club 2:  SD_P11W.NBN (28121)
    11,  # Club 3:  SD_P13W.NBN (28285)
    29,  # Club 4:  SD_P21W.NBN (29023)
    25,  # Club 5:  SD_P1FW.NBN (28859)
    13,  # Club 6:  SD_P15.NBN  (28367)
    15,  # Club 7:  SD_P17.NBN  (28449)
    21,  # Club 8:  SD_P1D.NBN  (28695)
    19,  # Club 9:  SD_P1B.NBN  (28613)
    17,  # Club 10: SD_P19.NBN  (28531)
)

# holiday_sd_set_summer: Vacation SD set index [command]
HOLIDAY_SD_SET_SUMMER = (0, 2, 4, 12, 12, 10, 8, 6)

# holiday_sd_set_other: Vacation SD set index [command]
HOLIDAY_SD_SET_OTHER = (1, 3, 5, 12, 12, 11, 9, 7)

# Sick rest SD constants (school_day_sd_load: cd_read_sectors(9, 0x801E0000, 0x7D8A))
SICK_REST_SD_LBA = 32138  # 0x7D8A -> CDROM/ETC_SD/SD_P31.NBN
SICK_REST_SD_SECTORS = 9


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def resolve_seasonal_route(
    command: int,
    club_id: int = 0,
    is_summer: bool = False,
    condition_bucket: int = 0,
    vacation_kind: int = 0,
    season_index: int = 0,
    companion_slot: int = 0,
) -> dict:
    """Resolve exact background and SD sprite selection matching PS1 native code."""
    if type(command) is not int or not (0 <= command <= 7):
        raise ValueError(f"unsupported command {command}, expected 0..7")
    if command == 4:
        if type(club_id) is not int or not (0 <= club_id <= 10):
            raise ValueError(f"unsupported club_id {club_id}, expected 0..10")
    if type(condition_bucket) is not int or condition_bucket not in (0, 1):
        raise ValueError(f"condition_bucket must be 0 (normal) or 1 (sick), got {condition_bucket}")
    if type(vacation_kind) is not int or not (0 <= vacation_kind <= 3):
        raise ValueError(f"vacation_kind must be 0..3, got {vacation_kind}")
    if type(season_index) is not int or not (0 <= season_index <= 3):
        raise ValueError(f"season_index must be 0..3, got {season_index}")
    if companion_slot != 0:
        raise ValueError(f"companion variants for command {command} are not supported in this tranche")

    # 1. Resolve Background LBA
    if condition_bucket == 1 and command == 7:
        if vacation_kind != 0:
            bg_lba = VACATION_SICK_BG_LBAS[season_index]
            bg_id = f"vacation_sick_season_{season_index}_bg"
        else:
            bg_lba = WEEKDAY_SICK_BG_LBAS[season_index]
            bg_id = f"weekday_sick_season_{season_index}_bg"
    elif vacation_kind != 0:
        if command == 4:
            bg_lba = CLUB_BG_LBAS[club_id]
            bg_id = f"command_4_club_{club_id}_bg"
        else:
            bg_lba = HOLIDAY_ACTIVITY_BG_LBAS[command]
            bg_id = f"vacation_command_{command}_bg"
    else:
        if command == 4:
            bg_lba = CLUB_BG_LBAS[club_id]
            bg_id = f"command_4_club_{club_id}_bg"
        else:
            bg_lba = WEEKDAY_BG_LBAS[command]
            bg_id = f"weekday_command_{command}_bg"

    # 2. Resolve SD Set and LBA
    if condition_bucket == 1 and command == 7:
        sd_set_id = -1
        sd_lba = SICK_REST_SD_LBA
        sd_sectors = SICK_REST_SD_SECTORS
        sd_id = "sick_rest_sd"
    elif vacation_kind != 0:
        if command == 4:
            sd_set_id = SUMMER_CLUB_SOLO_SD_SETS[club_id] if is_summer else NON_SUMMER_CLUB_SOLO_SD_SETS[club_id]
            sd_lba = CLUB_SD_BASE_LBA + CLUB_SD_SECTORS * sd_set_id
            sd_sectors = CLUB_SD_SECTORS
            season_str = "summer" if is_summer else "non_summer"
            sd_id = f"club_{club_id}_{season_str}_sd"
        else:
            sd_set_id = HOLIDAY_SD_SET_SUMMER[command] if is_summer else HOLIDAY_SD_SET_OTHER[command]
            sd_lba = HOLIDAY_SD_SECTORS_TABLE[sd_set_id]
            sd_sectors = 41
            season_str = "summer" if is_summer else "non_summer"
            sd_id = f"vacation_cmd_{command}_{season_str}_sd"
    else:
        if command == 4:
            sd_set_id = SUMMER_CLUB_SOLO_SD_SETS[club_id] if is_summer else NON_SUMMER_CLUB_SOLO_SD_SETS[club_id]
            sd_lba = CLUB_SD_BASE_LBA + CLUB_SD_SECTORS * sd_set_id
            sd_sectors = CLUB_SD_SECTORS
            season_str = "summer" if is_summer else "non_summer"
            sd_id = f"club_{club_id}_{season_str}_sd"
        else:
            table = SUMMER_WEEKDAY_SOLO_SD_SETS if is_summer else NON_SUMMER_WEEKDAY_SOLO_SD_SETS
            sd_set_id = table[command]
            sd_lba = WEEKDAY_SD_BASE_LBA + WEEKDAY_SD_SECTORS * sd_set_id
            sd_sectors = WEEKDAY_SD_SECTORS
            season_str = "summer" if is_summer else "non_summer"
            sd_id = f"weekday_cmd_{command}_{season_str}_sd"

    return {
        "command": command,
        "club_id": club_id,
        "is_summer": is_summer,
        "condition_bucket": condition_bucket,
        "vacation_kind": vacation_kind,
        "season_index": season_index,
        "companion_slot": companion_slot,
        "background_lba": bg_lba,
        "background_id": bg_id,
        "sd_set_id": sd_set_id,
        "sd_lba": sd_lba,
        "sd_sectors": sd_sectors,
        "sd_id": sd_id,
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


def export_seasonal_pack(input_path: str, output_path: str) -> dict:
    """Export complete seasonal, vacation, and condition variant pack from disc track1."""
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
    stage = Path(tempfile.mkdtemp(prefix=".seasonal-stage-", dir=out.parent))
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

            # Collect all unique background LBAs and SD LBAs across all variants
            all_routes = []

            # 1. Summer weekday routes: commands 1..3, 5..6, 0, 7
            for cmd in (0, 1, 2, 3, 5, 6, 7):
                all_routes.append(resolve_seasonal_route(cmd, is_summer=True))
            # 2. Summer club routes: clubs 0..10
            for cid in range(11):
                all_routes.append(resolve_seasonal_route(4, club_id=cid, is_summer=True))
            # 3. Sick weekday rest routes: command 7, condition_bucket 1, season_index 0..3
            for s_idx in range(4):
                all_routes.append(resolve_seasonal_route(7, condition_bucket=1, season_index=s_idx))
            # 4. Sick vacation rest routes: command 7, condition_bucket 1, vacation_kind 1, season_index 0..3
            for s_idx in range(4):
                all_routes.append(resolve_seasonal_route(7, condition_bucket=1, vacation_kind=1, season_index=s_idx))
            # 5. Vacation non-summer routes: commands 0..3, 5..7
            for cmd in (0, 1, 2, 3, 5, 6, 7):
                all_routes.append(resolve_seasonal_route(cmd, vacation_kind=1, is_summer=False))
            # 6. Vacation summer routes: commands 0..3, 5..7
            for cmd in (0, 1, 2, 3, 5, 6, 7):
                all_routes.append(resolve_seasonal_route(cmd, vacation_kind=2, is_summer=True))
            # 7. Vacation club routes: clubs 0..10 (summer and non-summer)
            for cid in range(11):
                all_routes.append(resolve_seasonal_route(4, club_id=cid, vacation_kind=1, is_summer=False))
                all_routes.append(resolve_seasonal_route(4, club_id=cid, vacation_kind=2, is_summer=True))

            exported_backgrounds = {}
            exported_sds = {}

            # Export unique backgrounds
            for r in all_routes:
                bg_lba = r["background_lba"]
                bg_id = r["background_id"]
                if bg_id not in exported_backgrounds:
                    bg_entry = by_lba.get(bg_lba)
                    if not bg_entry:
                        raise ValueError(f"background LBA {bg_lba} ({bg_id}) not found on disc")
                    bg_data = source.read(bg_entry)
                    bg_dir = stage / "reader" / bg_id
                    bg_context = {
                        "width": 320,
                        "height": 88,
                        "padded_height": 96,
                        "evidence": [
                            "src/main/school_day.c:school_day_background_read",
                            "src/main/school_day.c:school_day_vacation_background_read",
                            "src/main/holiday.c:holiday_activity_bg_sector",
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
                    exported_backgrounds[bg_id] = {
                        "source": {
                            "path": bg_entry["path"],
                            "lba": bg_lba,
                            "size": len(bg_data),
                            "sha256": _sha(bg_data),
                        },
                        "geometry_context": bg_context,
                        "decoder_model": bg_result["metadata"]["decoder_model"], "reader": "tools/assets/image_mdec.py:export_mdec",
                        "files": bg_files,
                    }

            # Export unique SD sets
            for r in all_routes:
                sd_lba = r["sd_lba"]
                sd_id = r["sd_id"]
                sd_sectors = r["sd_sectors"]
                if sd_id not in exported_sds:
                    sd_entry = by_lba.get(sd_lba)
                    if not sd_entry:
                        raise ValueError(f"SD LBA {sd_lba} ({sd_id}) not found on disc")
                    sd_data = source.read(sd_entry)
                    expected_bytes = sd_sectors * 2048
                    if len(sd_data) != expected_bytes:
                        raise ValueError(
                            f"SD {sd_id} extent is {len(sd_data)} bytes, expected {expected_bytes}"
                        )
                    sd_dir = stage / "reader" / sd_id
                    sd_result = export_sd_ui(
                        sd_data,
                        exe_data,
                        sd_dir,
                        sd_id,
                        source_entry=sd_entry,
                    )
                    if sd_result.get("status") != "exported":
                        raise ValueError(f"SD reader rejected {sd_id} pack")

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
                    exported_sds[sd_id] = {
                        "source": {
                            "path": sd_entry["path"],
                            "lba": sd_lba,
                            "size": len(sd_data),
                            "sha256": _sha(sd_data),
                            "exe_sha256": EXE_SHA,
                        },
                        "source_id": sd_id,
                        "profile_index": r["sd_set_id"],
                        "sectors": sd_sectors,
                        "frames": frames,
                        "sequences": meta.get("source_sequence_prefix", []),
                        "palette_mode": meta.get("actor_runtime_palette_mode", 0),
                    }

            # Build components and routes
            for r in all_routes:
                route_key = (
                    f"cmd_{r['command']}_club_{r['club_id']}_sum_{int(r['is_summer'])}"
                    f"_cond_{r['condition_bucket']}_vac_{r['vacation_kind']}_s_{r['season_index']}"
                )
                components[route_key] = {
                    "route": r,
                    "background_id": r["background_id"],
                    "sd_id": r["sd_id"],
                }
                routes.append(r)

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
                "routes": routes,
                "backgrounds": exported_backgrounds,
                "sd_sets": exported_sds,
                "components": components,
            }

            manifest_bytes = json.dumps(manifest, indent=2, sort_keys=True).encode("utf-8")
            if len(manifest_bytes) > MAX_JSON:
                raise ValueError("manifest payload exceeds MAX_JSON limit")
            (stage / "manifest.json").write_bytes(manifest_bytes)

            shutil.rmtree(stage / "reader", ignore_errors=True)
            os.rename(stage, out)

            manifest_sha = _sha(manifest_bytes)
            print(f"Exported seasonal pack to {out} (manifest SHA-256: {manifest_sha})")
            return {
                "path": str(out),
                "manifest_sha256": manifest_sha,
                "routes_count": len(routes),
                "backgrounds_count": len(exported_backgrounds),
                "sd_sets_count": len(exported_sds),
            }
    finally:
        if stage.exists():
            shutil.rmtree(stage, ignore_errors=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", default="disc/track1.bin", help="Original disc track 1 image")
    parser.add_argument(
        "--output",
        default=".asset-cache/port/seasonal-command-v1",
        help="Destination directory under .asset-cache/port/",
    )
    args = parser.parse_args()
    res = export_seasonal_pack(args.input, args.output)
    print(json.dumps(res, indent=2))


if __name__ == "__main__":
    main()
