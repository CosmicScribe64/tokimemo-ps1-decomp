"""Finite original-source profile for the captured April 9 holiday command 0 route.

Pins technical source/EXE/ISO identities. Original payload stays external. The
shared mode-zero MDEC geometry is revalidated from the current C/ASM witnesses.
"""
import hashlib
import struct
from pathlib import Path
from .bs_cohort_profile import (TRACK_SHA256, EXE_PATH, EXE_SHA256,
    _check_source_evidence, _validate_iso_directory_record, background_geometry_profile)

NATIVE_PROFILE_SHA256 = '10817b861efde8caef3043c45c931e9f340e7dfcae92f0c43f6699b0652ea445'
NATIVE_RESULT_SHA256 = '315ae8c2ad8ab517ea6d14b95d3beba4701a2fe8ae6e9ad6ffc04b65f03cb5d2'
SOURCE_FILES = {'src/main/home_screens.c': 'e3c03da3bdbf63dd4708fa451cdb5a872eadec531534c3e9e3c031b396fa8f6d',
 'asm/data/main/holiday.data.s': '009652561fbc1abe936406f96d04cd5cde3e4189fef1ae1e7e4ca73229452815',
 'include/game/day_event.h': 'efe0cdf794e1bf5f35bd5ff16feeecf3946f4695e98e066c6eca50effdf6c86d',
 'src/main/actor_sprites.c': 'ec04dd6c419b8079ef9430a484eb084c44d7a52029cc1c6ea359fd8571dab546',
 'src/main/frame_render_audio.c': 'b0932ca59d48bec7b3a3be55b8188b09f55f15721dbe96d84de14f2a0526d08e',
 'src/main/holiday.c': '50f2a15282a1a0e1c93dff4c56c2bfce4e4dd83867562983f8caa405d5d67061',
 'src/main/hud_panels.c': 'b942acb5863f26326c044720768384cf23b156f4736c69f1eda0d34831016ae1',
 'src/main/scene_library.c': 'c8a0315a4d4542dc13c8e1788ad199fafd28d5104bce64f08bb3d3b63e4e1a18'}

EXE_TABLES = [{'name': 'weekday_room',
  'offset': 479568,
  'raw_sha256': 'd2e556f1d311928d807dc1e23f33eee2517ea740699bb635fdfe9b194fc49a9a',
  'size': 16,
  'vaddr': 2148227408},
 {'name': 'room',
  'offset': 479584,
  'raw_sha256': '91c37dc04e2daa7f1c5b31945178204eec2a5f26cfd9a7fe2f67d0eae649f55e',
  'size': 16,
  'vaddr': 2148227424},
 {'name': 'activity',
  'offset': 483740,
  'raw_sha256': '8ee6391fd1f8ba7341f91c70331136978b3606b5b714710f86d61ee520c0593c',
  'size': 32,
  'vaddr': 2148231580},
 {'name': 'summer',
  'offset': 483792,
  'raw_sha256': 'c11e0f25935426ff77589a57c934f688b58380c78d929d422b84ca684731fe63',
  'size': 8,
  'vaddr': 2148231632},
 {'name': 'other',
  'offset': 483800,
  'raw_sha256': '7760b08ad67064597454aa52a054e19b23e39f20561020699945d5a05234ba1e',
  'size': 8,
  'vaddr': 2148231640},
 {'name': 'sd_lbas',
  'offset': 483808,
  'raw_sha256': '8cd3c9c949bb8026cb31df7fc508a3db44a18c9ce5945f9ac9bfd463598266f9',
  'size': 52,
  'vaddr': 2148231648},
 {'name': 'sd_layout',
  'offset': 563192,
  'raw_sha256': '86c1dfd7f942147ed8469faca345e02eeaab340154e829deaf0ee262cb94b184',
  'size': 78,
  'vaddr': 2148311032}]

ASSETS = ({'file_sectors': 8,
  'id': 'asset-6acf635b0fa775c476ad8ee4e18488a041d6366b18f22c804c372fde7cf326cb',
  'identifier': 'C_02_01H.BS;1',
  'lba': 16318,
  'path': 'CDROM/BS/C_02_01H.BS',
  'record_lba': 16122,
  'record_length': 60,
  'record_offset': 1356,
  'role': 'seasonal_room',
  'sha256': '98f97915de1e3ec174f47a6fcac79b0dd84052e870e4f6a4babed473ae2a7252',
  'size': 16048},
 {'file_sectors': 8,
  'id': 'asset-0b8b9f9c3c35bd56459635d36c882f7503e544c24f7cc40bdc4ffe39291cc7d4',
  'identifier': 'C_02_03.BS;1',
  'lba': 16379,
  'path': 'CDROM/BS/C_02_03.BS',
  'record_lba': 16122,
  'record_length': 60,
  'record_offset': 1776,
  'role': 'holiday_command0_background',
  'sha256': '1fa26a6b07b8f8114c695b5b7f979616ddb0d3ac676cd7576e7f79fe59d7c3cc',
  'size': 14840},
 {'file_sectors': 41,
  'id': 'asset-a549506ecd420dccd3d1bbf4d52b344142f1257c2484c0c8bd13dc48e7bc618a',
  'identifier': 'SD_P24W.NBN;1',
  'lba': 41487,
  'path': 'CDROM/HOLI_SD/SD_P24W.NBN',
  'record_lba': 41445,
  'record_length': 60,
  'record_offset': 156,
  'role': 'holiday_command0_sd',
  'sha256': 'd3d769779a8fd6d7d088978db69870c1fe2f9008c303d40f14f43a6ef6220d36',
  'size': 83968})

NATIVE_ROUTE = {
    "profile": "tokimemo.native-holiday-profile.v1", "profile_sha256": NATIVE_PROFILE_SHA256,
    "command": 0, "date": [95, 4, 9, 6], "mode": [70, 5, 1, 0], "queue_index": 9,
    "event": [16, 0, 0, 0], "sd_profile": 1, "sequence_index": 0,
    "season_index": 0, "health_nibble": 0,
    "actor_contract": {"actor_id": 96, "sequence_slot": 0, "anchor": [-80, -64],
                       "palette_override": 16, "palette_mode": 2, "draw_flags": 132, "flags": 5},
}
ANCHORS = {
    "src/main/home_screens.c": ("r = dec_bg_cd_read(D_800B5950[hud_season_index()], 0);",),
    "src/main/holiday.c": (
        "r = dec_bg_cd_read(D_800B5960[hud_season_index()], 0);",
        "dec_bg_show_set(0, dec_bg_decode_to_vram(0));",
        "return holiday_activity_bg_sectors[game_state.day_events[queue_index].girl];",
        "r = dec_bg_cd_read(holiday_activity_bg_sector(), 0);",
        "scene_work = holiday_sd_set_other[holiday_command];",
        "cd_read_sectors(0x29, 0x801E0000, holiday_sd_set_sectors[scene_work]);",
        "addr_init_holiday_sd(scene_work);", "load_palette(D_80125CA4, 0x10, 1, 1, 0);",
        "((Actor *)actor_pool)[96].x = -0x50;", "((Actor *)actor_pool)[96].y = -0x40;"),
    "src/main/scene_library.c": ("void addr_init_holiday_sd(s32 arg0)", "p = sd_table_holiday + arg0 % 13 * 3;"),
    "asm/data/main/holiday.data.s": ("dlabel holiday_activity_bg_sectors", "dlabel holiday_sd_set_other", "dlabel holiday_sd_set_sectors"),
}


def validate_native_profile(native):
    """Check the finite selector projection after the caller pins profile bytes."""
    if (not isinstance(native, dict) or native.get("schema") != NATIVE_ROUTE["profile"]
            or type(native.get("sd_profile")) is not int or native["sd_profile"] != 1):
        raise ValueError("holiday native SD selector differs from the captured route")
    expected = [{k: a[k] for k in ("role", "path", "lba", "size", "sha256")} for a in ASSETS]
    if native.get("assets") != expected:
        raise ValueError("holiday native asset identities differ")
    points = native.get("checkpoints")
    if not isinstance(points, list) or not points or not isinstance(points[0], dict):
        raise ValueError("holiday activity checkpoint is missing")
    point = points[0]
    for k, expected in (("label", "holiday_activity_sd_setup"), ("mode", NATIVE_ROUTE["mode"]),
                        ("date", NATIVE_ROUTE["date"]), ("queue_index", 9), ("last_command", 0)):
        actual = point.get(k)
        if (actual != expected or (type(expected) is int and type(actual) is not int)
                or (isinstance(expected, list) and (not isinstance(actual, list) or any(type(v) is not int for v in actual)))):
            raise ValueError("holiday native activity checkpoint differs: " + k)


def validate_executable(exe):
    if hashlib.sha256(exe).hexdigest() != EXE_SHA256:
        raise ValueError("holiday original executable hash differs")
    for table in EXE_TABLES:
        raw = exe[table["offset"]:table["offset"] + table["size"]]
        if len(raw) != table["size"] or hashlib.sha256(raw).hexdigest() != table["raw_sha256"]:
            raise ValueError("holiday executable selector/layout table differs: " + table["name"])


def validate_inventory(entries):
    """Require unique, exact selected original file and raw-directory locators."""
    by_path = {}
    for entry in entries:
        if entry.get("path") in by_path:
            raise ValueError("holiday source inventory paths must be unique")
        by_path[entry.get("path")] = entry
    for expected in ASSETS:
        entry = by_path.get(expected["path"])
        loc = entry.get("location", {}) if entry else {}
        if (not entry or entry.get("kind") != "file" or entry.get("size") != expected["size"]
                or entry.get("sha256") != expected["sha256"] or loc.get("lba") != expected["lba"]
                or loc.get("extent_bytes") != expected["size"] or loc.get("record_lba") != expected["record_lba"]
                or loc.get("record_offset") != expected["record_offset"]
                or len(loc.get("sectors", [])) != expected["file_sectors"]):
            raise ValueError("holiday original inventory identity/extent differs: " + expected["path"])
    return by_path


def build_holiday_profile(source, entries, repository_root):
    """Re-read exact original files, raw directory rows, tables and current sources."""
    root = Path(repository_root)
    source_files = _check_source_evidence(root)
    for relative, expected in SOURCE_FILES.items():
        data = (root / relative).read_bytes()
        if hashlib.sha256(data).hexdigest() != expected:
            raise ValueError("holiday source evidence hash differs: " + relative)
        if any(anchor not in data.decode() for anchor in ANCHORS.get(relative, ())):
            raise ValueError("holiday source evidence anchor is missing: " + relative)
        source_files[relative] = expected
    desc = source.describe()
    if desc.get("format") != "mode2-2352" or desc.get("source_sha256") != TRACK_SHA256:
        raise ValueError("holiday content requires the exact original MODE2 track")
    by_path = validate_inventory(entries)
    exe = by_path.get(EXE_PATH)
    if not exe or exe.get("kind") != "file":
        raise ValueError("holiday original executable is absent")
    validate_executable(source.read(exe))
    records, sd = [], None
    for expected in ASSETS:
        entry = by_path.get(expected["path"])
        if hashlib.sha256(source.read(entry)).hexdigest() != expected["sha256"]:
            raise ValueError("holiday original file bytes differ: " + expected["path"])
        directory = _validate_iso_directory_record(source, expected)
        rec = {"path": expected["path"], "role": expected["role"], "profile_kind": "holiday_command0",
               "lba_word": expected["lba"], "sha256": expected["sha256"], "size_bytes": expected["size"],
               "source": {"path": expected["path"], "sha256": expected["sha256"], "size": expected["size"],
                          "location": {"lba": expected["lba"]}},
               "iso_directory_record": {"record_lba": expected["record_lba"], "record_payload_offset": expected["record_offset"],
                    "record_length": directory["record_length"], "identifier": directory["identifier"],
                    "extent_lba": expected["lba"], "extent_size_bytes": expected["size"],
                    "sector_count": expected["file_sectors"], "sector_payload_sha256": directory["sector_payload_sha256"]},
               "source_evidence": ["src/main/holiday.c:holiday_open_wait_bg/holiday_activity_load/holiday_activity_sd_setup",
                   "src/main/background_images.c:dec_bg_decode_to_vram/dec_bg_draw mode0",
                   "src/main/scene_library.c:addr_init_holiday_sd", "asm/data/main/holiday.data.s:holiday selector tables"]}
        if expected["path"].endswith(".BS"):
            rec["fixed_reader_extent"] = {"requested_sectors": 10, "sectors_after_iso_file_extent": 2,
                                           "post_file_sectors_claimed_consumed_by_decoder": False}
            records.append(rec)
        else:
            sd = rec
    return {"schema": "tokimemo.bs-caller-cohort-profile.v1", "assets": records, "sd": sd,
            "geometry": background_geometry_profile(),
            "source_profile": {"profile_kind": "holiday_command0", "pinned_source_files": source_files,
                "executable": {"path": EXE_PATH, "sha256": EXE_SHA256, "tables": EXE_TABLES},
                "native_profile_sha256": NATIVE_PROFILE_SHA256, "native_result_sha256": NATIVE_RESULT_SHA256,
                "native_route": NATIVE_ROUTE, "assets": records, "sd": sd}}


ROUTE_TABLE_FIELDS = {
    "weekday_room": ("weekday_room_background_lbas", "s32", [4]),
    "room": ("seasonal_room_background_lbas", "s32", [4]),
    "activity": ("holiday_activity_background_lbas", "s32", [8]),
    "summer": ("holiday_sd_summer", "u8", [8]),
    "other": ("holiday_sd_non_summer", "u8", [8]),
    "sd_lbas": ("holiday_sd_lbas", "s32", [13]),
}


def route_tables(exe):
    """Return the exact finite route dependency tables with source byte provenance."""
    validate_executable(exe)
    values, provenance = {}, {}
    for table in EXE_TABLES:
        if table["name"] not in ROUTE_TABLE_FIELDS:
            continue
        name, element_type, shape = ROUTE_TABLE_FIELDS[table["name"]]
        raw = exe[table["offset"]:table["offset"] + table["size"]]
        values[name] = list(struct.unpack("<" + ("i" if element_type == "s32" else "B") * shape[0], raw))
        provenance[name] = {"vaddr": table["vaddr"], "exe_offset": table["offset"], "size": len(raw),
                            "raw_sha256": hashlib.sha256(raw).hexdigest(), "type": element_type, "shape": shape}
    return {"schema": "tokimemo.holiday-route-tables.v1", "values": values, "provenance": provenance}
