"""Source/native-caller-only profile for bounded ordinary spring commands 5 and 6.

The profile admits scene0/extra15, normal health and no vacation only. Original
selector values and payload are read from the pinned track; source carries only
locators, dimensions established by the existing caller, and hashes.
"""

import hashlib
import struct
from pathlib import Path
from .bs_cohort_profile import (
    TRACK_SHA256,
    EXE_SHA256,
    build_profile_context,
    _validate_iso_directory_record,
)

SCHEMA = "tokimemo.weekday-56-source-profile.v1"
SOURCE_FILES = {
    "asm/data/data_800B5950.data.s": "4482f3ec44d7a64fbcb1b305e015452e014382d6d819dd8759e730d3f0b2d439",
    "asm/libpress_main.s": "330179552d5f4d42a50394a2ad1907f7e6c9984dbaa84fd53603d55fee4bbd7e",
    "config/symbol_addrs.txt": "003c6169775031a58015faebba3d8674754a5d3ed166a9b06f9528055cbc3bfe",
    "config/symbol_addrs_obin.txt": "90c8ea5803cb3446e851a8bca6b37eac706ccb76427ab44b74b73ff3830a2861",
    "include/game/day_event.h": "efe0cdf794e1bf5f35bd5ff16feeecf3946f4695e98e066c6eca50effdf6c86d",
    "include/main/bg.h": "bd778800b70dba2bb6b2811b79086fd3c391b2448dc2ecca808698c5100237bd",
    "include/main/school_day.h": "ebfc2451ef48395db1b4d8ca7e809374ef470ddf9db13194a2bdec0fb97839e9",
    "src/main/actor_sprites.c": "ec04dd6c419b8079ef9430a484eb084c44d7a52029cc1c6ea359fd8571dab546",
    "src/main/background_images.c": "7eea24dfee7fd3503beedbd1b2f633c40f19102d21f992557d626cba3d036402",
    "src/main/frame_render_audio.c": "b0932ca59d48bec7b3a3be55b8188b09f55f15721dbe96d84de14f2a0526d08e",
    "src/main/hud_panels.c": "b942acb5863f26326c044720768384cf23b156f4736c69f1eda0d34831016ae1",
    "src/main/memory_card.c": "32fd2c5f413038eee79d940c3a964e47652b472935b34f8597231c60d5979811",
    "src/main/scene_library.c": "c8a0315a4d4542dc13c8e1788ad199fafd28d5104bce64f08bb3d3b63e4e1a18",
    "src/main/school_day.c": "d5f975a14823d31d03abecfc2502e71c21a1d5ec45dc79653f84dfaa390f479c",
}

TABLES = [
    {
        "exe_offset": 483168,
        "name": "weekly_background",
        "sha256": "1f630f159b05aac5a0e64cda0a128c2e569437dfbd8e87fd4c034e5cac9f3f5f",
        "size": 32,
        "vaddr": 2148231008,
    },
    {
        "exe_offset": 483536,
        "name": "non_summer",
        "sha256": "9fee0fe4e7999add256635ff53653b8c014828bec478b25f36180aa798316352",
        "size": 16,
        "vaddr": 2148231376,
    },
    {
        "exe_offset": 483520,
        "name": "summer",
        "sha256": "e75ff55397b38128c9d6a0be0a2c7ce5cff461ce0bd6126ecdaf82bb718f479e",
        "size": 16,
        "vaddr": 2148231360,
    },
    {
        "exe_offset": 483244,
        "name": "weekly_sd_lbas",
        "sha256": "fb38f1b6f5b9141e5e31d2e2bb1928cc68f50b6d69749d880eb9338541513e6d",
        "size": 100,
        "vaddr": 2148231084,
    },
    {
        "exe_offset": 563040,
        "name": "weekly_layouts",
        "sha256": "124705fe841e948f8bc8fca5557d77ce77bb096ab11f93526f6303021f485c2c",
        "size": 150,
        "vaddr": 2148310880,
    },
]

NATIVE_CALLERS = [
    {
        "exe_offset": 209376,
        "name": "get_weekly_bg_sector",
        "sha256": "1610f30bdfeb8d3839d18926c3794d3e2f5e67ad7b7d0451b88d9edfb8c84966",
        "size": 96,
        "vaddr": 2147957216,
    },
    {
        "exe_offset": 209472,
        "name": "school_day_background_read",
        "sha256": "a522e463f34f3795ef8c6f100a13c0c3c7ac6146b0121637d6ee567e4b198528",
        "size": 152,
        "vaddr": 2147957312,
    },
    {
        "exe_offset": 209624,
        "name": "school_day_screen_init",
        "sha256": "8743dd001b5d436d37ff73ad6e4dd9f38aafd9d4d32e5b834b726ad4c797a871",
        "size": 472,
        "vaddr": 2147957464,
    },
    {
        "exe_offset": 210096,
        "name": "school_day_sd_set_select",
        "sha256": "603e17411fe743a5c9d93189f46652fe79b61a103992b1705d934b1cdf0bd7ca",
        "size": 1288,
        "vaddr": 2147957936,
    },
    {
        "exe_offset": 211384,
        "name": "school_day_sd_load",
        "sha256": "9b415587d74dd86de05cd5196aff83a06c4c328b2aa56e2e7dbbc5ba52f5e433",
        "size": 452,
        "vaddr": 2147959224,
    },
    {
        "exe_offset": 211836,
        "name": "school_day_sd_actors_init",
        "sha256": "c8be2850bfd14e8bcef55b47070d42039dd719309a27475e2fd44364378fd8fe",
        "size": 732,
        "vaddr": 2147959676,
    },
    {
        "exe_offset": 245088,
        "name": "addr_init_weekly_sd",
        "sha256": "727f4f9b26fdc690921f5ae0e9a56f5d3b04177359fefc2525cd3ded25484ca7",
        "size": 108,
        "vaddr": 2147992928,
    },
    {
        "exe_offset": 93468,
        "name": "dec_bg_decode_to_vram",
        "sha256": "c3494337aa13bb7b0d83a6fa91714d5dbf2cf8280f40de991bf38ae31df9ec4c",
        "size": 292,
        "vaddr": 2147841308,
    },
    {
        "exe_offset": 94452,
        "name": "dec_bg_draw",
        "sha256": "095ab488cbd45680a44ceb11eff793b83d53c28ae4c11bb1308ab0ec482f722f",
        "size": 1064,
        "vaddr": 2147842292,
    },
]

ASSETS = (
    {
        "command": 5,
        "file_sectors": 9,
        "id": "asset-17db7eb936e6f5be3e7a25c16b6befe686303a4a8d39048cf9313506412a7bbc",
        "identifier": "C_01_42.BS;1",
        "lba": 16268,
        "path": "CDROM/BS/C_01_42.BS",
        "record_lba": 16122,
        "record_length": 60,
        "record_offset": 996,
        "role": "background",
        "sha256": "91cd791faa088328f4ff229caa96898f1ecbb1cd9fbf54e5479240b0e36f2897",
        "size": 18008,
    },
    {
        "command": 5,
        "file_sectors": 41,
        "id": "asset-f7bb66bdee0625e11df2695a0bcab99383f7f232b09bdad8ee91eda789e3b8a2",
        "identifier": "SD_P0DW.NBN;1",
        "lba": 43402,
        "path": "CDROM/MAIN_SD/SD_P0DW.NBN",
        "record_lba": 42417,
        "record_length": 60,
        "record_offset": 1536,
        "role": "sd",
        "sha256": "ac6148de4e181b8bdb4f9df633269ac4f216a54f39d079d605276c3b64e5c304",
        "size": 83968,
    },
    {
        "command": 6,
        "file_sectors": 6,
        "id": "asset-dc5aae95a5e93e9680f22698f89ac2d0c6afd210962c7ae4f64713fb355f4f1a",
        "identifier": "C_01_43.BS;1",
        "lba": 16277,
        "path": "CDROM/BS/C_01_43.BS",
        "record_lba": 16122,
        "record_length": 60,
        "record_offset": 1056,
        "role": "background",
        "sha256": "1dfb89945070062dc86b04f842d13ce0c867851bf39ad21b4b0ecd76bb8658dd",
        "size": 12004,
    },
    {
        "command": 6,
        "file_sectors": 41,
        "id": "asset-5e0df4f5f9bc221fcdbb91b77936841fa941ec0f25c93faf7290269b89ae341d",
        "identifier": "SD_P0BW.NBN;1",
        "lba": 43238,
        "path": "CDROM/MAIN_SD/SD_P0BW.NBN",
        "record_lba": 42417,
        "record_length": 60,
        "record_offset": 1296,
        "role": "sd",
        "sha256": "7ae2ed9d832a4aa24f2339acc0155fba5266130682822775824f1cfdda62d42d",
        "size": 83968,
    },
)

ANCHORS = {
    "src/main/school_day.c": (
        "return D_800B6760[game_state.day_events[x].girl];",
        "ret = dec_bg_cd_read(get_weekly_bg_sector(), 0);",
        "scene_work = (s32) D_800B68D0[9 + sd_variant];",
        "scene_work = (s32) D_800B68D0[11 + sd_variant];",
        "cd_read_sectors(0x29, 0x801E0000, D_800B67AC[scene_work]);",
        "addr_init_weekly_sd(scene_work);",
        "load_palette(D_80125CA4, 0x10, 1, 1, 0);",
        "((Actor *)actor_pool)[96].x = -0x50;",
        "((Actor *)actor_pool)[96].y = -0x40;",
        "game_state.day_events[game_state.day_event_index].extra != 0xF",
    ),
    "src/main/scene_library.c": (
        "void addr_init_weekly_sd(s32 arg0)",
        "p = sd_table_weekday + arg0 % 25 * 3;",
    ),
}
UNSUPPORTED = {
    "other_commands": "only commands5/6 are in this finite source profile",
    "summer_and_other_months": "only months3-5 are admitted; other seasonal SD variants are not exported",
    "other_event_extra": "only extra15 is admitted; companion/zero-extra variants are not exported",
    "other_scene": "only scene0 / sequence0 is admitted",
    "vacation": "vacation callers and resources are not included",
    "other_health": "only normal health nibble0 is admitted",
    "native_runtime_route": "caller bytes/source pins are proved; no command5/6 runtime capture is claimed",
    "scene_composition_audio_timing": "not established by this asset capability",
}


def request_error(command, month=4, extra=15, scene=0, vacation=0, health=0):
    if type(command) is not int or command not in (5, 6):
        return UNSUPPORTED["other_commands"]
    if type(month) is not int or month not in (3, 4, 5):
        return UNSUPPORTED["summer_and_other_months"]
    if type(extra) is not int or extra != 15:
        return UNSUPPORTED["other_event_extra"]
    if type(scene) is not int or scene != 0:
        return UNSUPPORTED["other_scene"]
    if type(vacation) is not int or vacation != 0:
        return UNSUPPORTED["vacation"]
    if type(health) is not int or health != 0:
        return UNSUPPORTED["other_health"]
    return ""


def resolve_selection(tables, command, month=4, extra=15, scene=0, vacation=0, health=0):
    error = request_error(command, month, extra, scene, vacation, health)
    if error:
        raise ValueError(error)
    sizes = {"weekly_background": 8, "non_summer": 16, "weekly_sd_lbas": 25}
    for name, size in sizes.items():
        values = tables.get(name) if isinstance(tables, dict) else None
        if (
            not isinstance(values, list)
            or len(values) != size
            or any(type(v) is not int for v in values)
        ):
            raise ValueError("weekday56 original selector table shape/type differs: " + name)
    index = (9 if command == 5 else 11) + 1
    selected = tables["non_summer"][index]
    if not 0 <= selected < 25:
        raise ValueError("weekday56 SD profile index is outside native25-row table")
    return {
        "command": command,
        "months": [3, 4, 5],
        "event_extra": 15,
        "event_scene": 0,
        "vacation": 0,
        "health_nibble": 0,
        "selector_index": index,
        "sd_profile": selected,
        "background_lba": tables["weekly_background"][command],
        "sd_lba": tables["weekly_sd_lbas"][selected],
    }


def validate_executable(exe):
    if hashlib.sha256(exe).hexdigest() != EXE_SHA256:
        raise ValueError("weekday56 original executable hash differs")
    for span in TABLES + NATIVE_CALLERS:
        raw = exe[span["exe_offset"] : span["exe_offset"] + span["size"]]
        if len(raw) != span["size"] or hashlib.sha256(raw).hexdigest() != span["sha256"]:
            raise ValueError("weekday56 native selector/caller bytes differ: " + span["name"])


def read_tables(exe):
    validate_executable(exe)
    result = {}
    for table in TABLES:
        if table["name"] == "weekly_layouts":
            continue
        raw = exe[table["exe_offset"] : table["exe_offset"] + table["size"]]
        byte = table["name"] in ("summer", "non_summer")
        result[table["name"]] = (
            list(raw) if byte else list(struct.unpack("<%dI" % (len(raw) // 4), raw))
        )
    return result


def validate_inventory(entries):
    by_path = {}
    for entry in entries:
        if entry.get("path") in by_path:
            raise ValueError("weekday56 input paths must be unique")
        by_path[entry.get("path")] = entry
    for expected in ASSETS:
        entry = by_path.get(expected["path"])
        loc = entry.get("location", {}) if entry else {}
        if (
            not entry
            or entry.get("kind") != "file"
            or entry.get("size") != expected["size"]
            or entry.get("sha256") != expected["sha256"]
            or loc.get("lba") != expected["lba"]
            or loc.get("extent_bytes") != expected["size"]
            or loc.get("record_lba") != expected["record_lba"]
            or loc.get("record_offset") != expected["record_offset"]
            or len(loc.get("sectors", [])) != expected["file_sectors"]
        ):
            raise ValueError("weekday56 original file/ISO identity differs: " + expected["path"])
    return by_path


def build_profile(source, entries, root):
    root = Path(root)
    for name, expected in SOURCE_FILES.items():
        raw = (root / name).read_bytes()
        if hashlib.sha256(raw).hexdigest() != expected:
            raise ValueError("weekday56 source witness differs: " + name)
        if any(anchor not in raw.decode() for anchor in ANCHORS.get(name, ())):
            raise ValueError("weekday56 source anchor absent: " + name)
    desc = source.describe()
    if desc.get("format") != "mode2-2352" or desc.get("source_sha256") != TRACK_SHA256:
        raise ValueError("weekday56 requires the exact original MODE2 track")
    by_path = validate_inventory(entries)
    exe_entry = by_path.get("SLPM_86.053")
    if not exe_entry:
        raise ValueError("weekday56 original executable is absent")
    exe = source.read(exe_entry)
    tables = read_tables(exe)
    # This accepted profile proves the same mode-zero caller/helper chain.
    shared = build_profile_context(source, entries, root, "command7")
    records = []
    for expected in ASSETS:
        entry = by_path[expected["path"]]
        if hashlib.sha256(source.read(entry)).hexdigest() != expected["sha256"]:
            raise ValueError("weekday56 original bytes differ: " + expected["path"])
        iso = _validate_iso_directory_record(source, expected)
        records.append({**expected, "iso_directory_record": iso})
    routes = [resolve_selection(tables, c) for c in (5, 6)]
    for route in routes:
        bg = next(
            a for a in ASSETS if a["command"] == route["command"] and a["role"] == "background"
        )
        sd = next(a for a in ASSETS if a["command"] == route["command"] and a["role"] == "sd")
        if bg["lba"] != route["background_lba"] or sd["lba"] != route["sd_lba"]:
            raise ValueError(
                "weekday56 table selection differs from fixed original asset identities"
            )
    return {
        "schema": SCHEMA,
        "source_files": SOURCE_FILES,
        "selector_tables": TABLES,
        "native_callers": NATIVE_CALLERS,
        "geometry": shared["geometry"],
        "geometry_authority": shared["source_profile"],
        "assets": records,
        "routes": routes,
        "evidence_kind": "current source plus original native caller/selector bytes; no observed command5/6 route",
    }
