"""Trusted finite source profile for the weekly school-day BS backgrounds.

This profile is deliberately checked into source code with the exporter patch.
It does not consume an ignored report as evidence. Its pinned source files,
original EXE bytes, selector indices, disc paths, directory locations and file
hashes are checked against the current checkout and the supplied original
MODE2 track on every profiled export.
"""
import hashlib
import struct
from pathlib import Path


PROFILE_SCHEMA = "tokimemo.bs-caller-cohort-profile.v1"
TRACK_SHA256 = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
EXE_PATH = "SLPM_86.053"
EXE_SHA256 = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
EXE_TABLE_OFFSET = 0x75F60
EXE_TABLE_RAW = bytes.fromhex("033f00000d3f0000173f0000203f0000")
TABLE_WORDS = (0x3F03, 0x3F0D, 0x3F17, 0x3F20)

SOURCE_FILES = {
    "src/main/school_day.c": "d5f975a14823d31d03abecfc2502e71c21a1d5ec45dc79653f84dfaa390f479c",
    "src/main/background_images.c": "7eea24dfee7fd3503beedbd1b2f633c40f19102d21f992557d626cba3d036402",
    "src/main/memory_card.c": "32fd2c5f413038eee79d940c3a964e47652b472935b34f8597231c60d5979811",
    "asm/data/data_800B5950.data.s": "4482f3ec44d7a64fbcb1b305e015452e014382d6d819dd8759e730d3f0b2d439",
    "asm/libpress_main.s": "330179552d5f4d42a50394a2ad1907f7e6c9984dbaa84fd53603d55fee4bbd7e",
}

# These anchors are checked in addition to whole-file hashes so the evidence
# roles remain explicit and reviewable in a fresh checkout.
SOURCE_ANCHORS = {
    "src/main/school_day.c": (
        "if (game_state.day_events[x].girl != 4) {",
        "return D_800B6760[game_state.day_events[x].girl];",
        "ret = dec_bg_cd_read(get_weekly_bg_sector(), 0);",
        "ret = dec_bg_cd_read(D_800B5950[hud_season_index()], 0);",
        "dec_bg_decode_to_vram(0)",
    ),
    "src/main/background_images.c": (
        "cd_read_sectors_nowait(0xA, 0x80162000, arg0);",
        "bg_still_decode_to_vram(0x2C0, 0xA0, 0x140, 0x58)",
        "bg_still_decode_to_vram(0x140, 0x1A0, 0x140, 0x58)",
        "setWH(sprite, 0xC0, 0x58);",
        "setWH(sprite, 0x80, 0x58);",
    ),
    "src/main/memory_card.c": (
        "rect.w = 0x10;",
        "rect.h = arg3;",
        "v = ((arg3 + 8) >> 4) << 7;",
        "rect.x += 0x10;",
    ),
    "asm/data/data_800B5950.data.s": (
        "dlabel D_800B6760",
        ".word 0x00003F03",
        ".word 0x00003F0D",
        ".word 0x00003F17",
        ".word 0x00003F20",
    ),
    "asm/libpress_main.s": (
        "glabel func_800868BC",
        "/* 460C4 800868C4 0100A230 */  andi       $v0, $a1, 0x1",
        "/* 4612C 8008692C B81A020C */  jal        func_80086AE0",
        "glabel func_800869EC",
        "/* 46250 80086A50 B81A020C */  jal        func_80086AE0",
        "/* 46254 80086A54 20000524 */   addiu     $a1, $zero, 0x20",
    ),
}

ASSETS = (
    {
        "path": "CDROM/BS/C_01_27.BS", "girl_index": 0, "lba": 0x3F03,
        "size": 20032, "sha256": "7543e723ea06c7db100b148350394f79c1a94ff8961aa65904d4aa1613295992",
        "record_lba": 16122, "record_offset": 96, "record_length": 60,
        "identifier": "C_01_27.BS;1", "file_sectors": 10,
    },
    {
        "path": "CDROM/BS/C_01_28.BS", "girl_index": 1, "lba": 0x3F0D,
        "size": 18732, "sha256": "d7748c6a565f3f3bf8c7c0e39a90a701da9caf4fbaef8f93ad214db313d2daed",
        "record_lba": 16122, "record_offset": 156, "record_length": 60,
        "identifier": "C_01_28.BS;1", "file_sectors": 10,
    },
    {
        "path": "CDROM/BS/C_01_29.BS", "girl_index": 2, "lba": 0x3F17,
        "size": 17612, "sha256": "901521c943e6490e58042500d4e7b5bf4f1da1a121092b9aaca42b2729c12c75",
        "record_lba": 16122, "record_offset": 216, "record_length": 60,
        "identifier": "C_01_29.BS;1", "file_sectors": 9,
    },
    {
        "path": "CDROM/BS/C_01_30.BS", "girl_index": 3, "lba": 0x3F20,
        "size": 16224, "sha256": "2d408231b46b04b2cd9717db036d1f40d06d27d57d5d5e4ec1e61b2bb13a3954",
        "record_lba": 16122, "record_offset": 276, "record_length": 60,
        "identifier": "C_01_30.BS;1", "file_sectors": 8,
    },
)


def _sha256(data):
    return hashlib.sha256(data).hexdigest()


def _check_source_evidence(repository_root):
    root = Path(repository_root).resolve()
    checked = {}
    for relative, expected_hash in SOURCE_FILES.items():
        path = root / relative
        data = path.read_bytes()
        actual_hash = _sha256(data)
        if actual_hash != expected_hash:
            raise ValueError("trusted BS source evidence hash differs: " + relative)
        text = data.decode("utf-8")
        if any(anchor not in text for anchor in SOURCE_ANCHORS[relative]):
            raise ValueError("trusted BS source evidence anchor is missing: " + relative)
        checked[relative] = actual_hash
    return checked


def _validate_asset_inventory(entries):
    if tuple(asset["girl_index"] for asset in ASSETS) != (0, 1, 2, 3):
        raise ValueError("trusted BS profile selector indices are not the exact four rows")
    if tuple(asset["lba"] for asset in ASSETS) != TABLE_WORDS:
        raise ValueError("trusted BS profile rows do not match the executable table order")
    by_path = {entry.get("path"): entry for entry in entries}
    checked = []
    for expected in ASSETS:
        entry = by_path.get(expected["path"])
        location = entry.get("location", {}) if entry else {}
        if (entry is None or entry.get("kind") != "file"
                or entry.get("size") != expected["size"]
                or entry.get("sha256") != expected["sha256"]
                or location.get("lba") != expected["lba"]
                or location.get("extent_bytes") != expected["size"]
                or location.get("record_lba") != expected["record_lba"]
                or location.get("record_offset") != expected["record_offset"]
                or len(location.get("sectors", [])) != expected["file_sectors"]):
            raise ValueError("input inventory differs from trusted BS profile: " + expected["path"])
        checked.append((expected, entry))
    return checked


def _validate_executable_bytes(exe_data):
    if _sha256(exe_data) != EXE_SHA256:
        raise ValueError("trusted original executable hash differs")
    actual_table = exe_data[EXE_TABLE_OFFSET:EXE_TABLE_OFFSET + len(EXE_TABLE_RAW)]
    if actual_table != EXE_TABLE_RAW or struct.unpack("<4I", actual_table) != TABLE_WORDS:
        raise ValueError("trusted original executable table bytes differ")
    return actual_table


def _validate_iso_directory_record(source, expected):
    sector_entry = {"path": "<trusted-BS-directory-record>", "kind": "file",
                    "size": 2048,
                    "location": {"lba": expected["record_lba"], "extent_bytes": 2048}}
    sectors = list(source.iter_raw_sectors(sector_entry, TRACK_SHA256))
    if (len(sectors) != 1 or sectors[0].get("form") != 1
            or not sectors[0].get("subheader_valid")
            or sectors[0].get("payload_size") != 2048):
        raise ValueError("trusted ISO directory record sector is not verified Form1")
    payload = sectors[0]["payload"]
    offset = expected["record_offset"]
    length = payload[offset]
    record = payload[offset:offset + length]
    if (length != expected["record_length"] or len(record) != length
            or len(record) < 34):
        raise ValueError("trusted ISO directory record length differs: " + expected["path"])
    identifier_length = record[32]
    identifier = record[33:33 + identifier_length].decode("ascii")
    extent_lba = int.from_bytes(record[2:6], "little")
    extent_size = int.from_bytes(record[10:14], "little")
    if (identifier != expected["identifier"]
            or extent_lba != expected["lba"]
            or int.from_bytes(record[6:10], "big") != expected["lba"]
            or extent_size != expected["size"]
            or int.from_bytes(record[14:18], "big") != expected["size"]):
        raise ValueError("trusted ISO directory record identity or extent differs: " + expected["path"])
    return {"record_lba": expected["record_lba"], "record_offset": offset,
            "record_length": length, "identifier": identifier,
            "extent_lba": extent_lba, "extent_size_bytes": extent_size,
            "sector_payload_sha256": _sha256(payload)}


def build_profile_context(source, entries, repository_root):
    """Validate the fixed source profile and return its internally made context."""
    source_files = _check_source_evidence(repository_root)
    checked_assets = _validate_asset_inventory(entries)
    description = source.describe()
    if (description.get("format") != "mode2-2352"
            or description.get("source_sha256") != TRACK_SHA256):
        raise ValueError("original MODE2 track does not match the trusted BS profile")

    by_path = {entry.get("path"): entry for entry in entries}
    exe_entry = by_path.get(EXE_PATH)
    if exe_entry is None or exe_entry.get("kind") != "file":
        raise ValueError("trusted original executable is absent from the input inventory")
    exe_data = source.read(exe_entry)
    _validate_executable_bytes(exe_data)

    records = []
    for expected, entry in checked_assets:
        file_data = source.read(entry)
        if _sha256(file_data) != expected["sha256"]:
            raise ValueError("original BS file bytes differ from trusted profile: " + expected["path"])
        iso_record_proof = _validate_iso_directory_record(source, expected)
        if expected["lba"] != TABLE_WORDS[expected["girl_index"]]:
            raise ValueError("trusted selector index and LBA do not agree")
        records.append({
            "path": expected["path"], "schedule_girl_index": expected["girl_index"],
            "lba_word": expected["lba"], "size_bytes": expected["size"],
            "sha256": expected["sha256"],
            "source": {"path": expected["path"], "sha256": expected["sha256"],
                       "size": expected["size"], "location": {"lba": expected["lba"]}},
            "iso_directory_record": {
                "record_lba": expected["record_lba"],
                "record_payload_offset": expected["record_offset"],
                "record_length": iso_record_proof["record_length"],
                "identifier": expected["identifier"],
                "extent_lba": expected["lba"],
                "extent_size_bytes": expected["size"],
                "sector_count": expected["file_sectors"],
                "sector_payload_sha256": iso_record_proof["sector_payload_sha256"],
            },
            "fixed_reader_extent": {
                "requested_sectors": 10,
                "sectors_after_iso_file_extent": 10 - expected["file_sectors"],
                "post_file_sectors_claimed_consumed_by_decoder": False,
            },
            "source_evidence": [
                "src/main/school_day.c:get_weekly_bg_sector/school_day_background_read/school_day_screen_init",
                "src/main/background_images.c:dec_bg_cd_read/dec_bg_decode_to_vram/dec_bg_draw",
                "src/main/memory_card.c:bg_still_decode_to_vram",
                "asm/data/data_800B5950.data.s:D_800B6760 words",
                "asm/libpress_main.s:func_800868BC/func_800869EC native MIPS helpers",
            ],
        })

    return {
        "schema": PROFILE_SCHEMA,
        "status": "trusted_finite_profile_validated_from_current_source_and_original_input",
        "source_profile": {
            "pinned_source_files": source_files,
            "executable": {"path": EXE_PATH, "sha256": EXE_SHA256,
                            "table_offset": EXE_TABLE_OFFSET,
                            "table_raw_hex": EXE_TABLE_RAW.hex(),
                            "table_words": list(TABLE_WORDS)},
            "assets": records,
        },
        "geometry": {"active_pixels": {"width": 320, "height": 88},
                     "padded_pixels": {"width": 320, "height": 96},
                     "source_derived": True,
                     "caller_mode": 0,
                     "active_decode_rect_height": 0x58,
                     "vram_rectangles_by_dec_bg_decode_to_vram_return": [
                         {"return_value": 0, "rect_xywh": [0x2C0, 0xA0, 0x140, 0x58]},
                         {"return_value": 1, "rect_xywh": [0x140, 0x1A0, 0x140, 0x58]},
                     ],
                     "macroblock_rows_from_source_helper": 6,
                     "display_sprite_pieces_relative_to_draw_origin": [
                         {"origin_offset_xy": [-0xA0, -0x30], "size_wh": [0xC0, 0x58]},
                         {"origin_offset_xy": [0x20, -0x30], "size_wh": [0x80, 0x58]},
                     ],
                     "runtime_draw_origin_values_known": False,
                     "overread_bytes_used_for_decode": False},
        "assets": records,
    }
