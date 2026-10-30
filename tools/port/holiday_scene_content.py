#!/usr/bin/env python3
"""Export the source-proved holiday-screen state profile (T-39014).

The legacy holiday_scene role was an April 4 RAM capture. This producer
derives only what the original executable fixes when holiday_open_init
finishes: the dial-key ring, its write position, the icon count and the
background brightness. Each value is decoded from the authenticated
executable (whole-track pin, ISO directory entry, PS-X EXE header) and
cross-checked against pinned C/ASM witnesses. The three store addresses are
decoded from their lui/immediate pairs; the stored values are pinned whole
instruction words.

The remaining boundary fields (icon_mode, input_device, screen_brightness,
tile_layer_enabled, boot_hold, clubs) belong to the live session. They are
listed as session-owned and never given values here.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import sys
import tempfile

SCHEMA = "tokimemo.holiday-scene-source.v1"
TRACK_SHA256 = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
EXE_PATH = "SLPM_86.053"
EXE_SHA256 = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
# ISO 9660 directory record of the executable in the pinned track.
EXE_ENTRY = {"lba": 24, "size": 667648, "record_lba": 22, "record_offset": 270}
LOAD_ADDRESS = 0x80041000
TEXT_SIZE = 0xA2800
HEADER_SIZE = 0x800
GAME_STATE = 0x800E6280
ICON_COUNT_OFFSET = 0xF6D

SOURCE_FILES = {
    "src/main/holiday.c": "50f2a15282a1a0e1c93dff4c56c2bfce4e4dd83867562983f8caa405d5d67061",
    "src/main/background_images.c": "7eea24dfee7fd3503beedbd1b2f633c40f19102d21f992557d626cba3d036402",
    "include/game/game_state.h": "e58484d902e1c47696a3ae5086fa93626bebc107ab83cb0d8dc91cc0a3e4a6af",
    "asm/data/main/holiday.data.s": "009652561fbc1abe936406f96d04cd5cde3e4189fef1ae1e7e4ca73229452815",
    "asm/matchings/main/holiday/holiday_open_init.s": "b1afe3a42aaff9258f240f04c9028289036f622d32a896407e41534dae0aaf6c",
}
SOURCE_ANCHORS = {
    "src/main/holiday.c": (
        "u8 holiday_dial_keys[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0 };",
        "u8 holiday_dial_key_pos = 0;",
        "    holiday_dial_key_pos = 0;\n    for (i = 0; i < 7; i++) { holiday_dial_keys[i] = 0xFF; }",
        "        if (holiday_dial_key_pos >= 7) {\n            holiday_dial_key_pos = 0;",
        "    game_state.icon_count = 0xE;\n    hud_icon_bar_init();",
        "            set_dec_bri(0x80U);",
        "            set_dec_bri(0x80);\n            mode_stage_next();",
    ),
    "src/main/background_images.c": ("void set_dec_bri(u8 arg0) {\n    bg_brightness = arg0;\n}",),
    "include/game/game_state.h": ("/* 0xF6D */ u8 icon_count;",),
    "asm/data/main/holiday.data.s": ("/* 76197 800B6997 */ .byte 0x00", "dlabel holiday_dial_key_pos\n    /* 76198 800B6998 */ .byte 0x00"),
    "asm/matchings/main/holiday/holiday_open_init.s": ("/* 35224 80075A24 80000424 */   addiu     $a0, $zero, 0x80",),
}
# Original instruction words: vaddr -> (word, pinned mask, meaning). Address
# words pin only opcode/rs/rt (mask 0xFFFF0000); their immediates are decoded
# by derive() and must resolve to the source symbols. All other words are
# pinned whole, so their values equal the C witnesses checked by check_source().
ADDR = 0xFFFF0000
WHOLE = 0xFFFFFFFF
CODE = {
    0x800755B4: (0x3C01800B, ADDR, "lui at,0x800B"),
    0x800755B8: (0xA0206998, ADDR, "sb zero,0x6998(at): holiday_dial_key_pos = 0"),
    0x800755C0: (0x3C19800B, ADDR, "lui t9,0x800B"),
    0x800755C4: (0x27396990, ADDR, "addiu t9,t9,0x6990: &holiday_dial_keys"),
    0x80075678: (0x2409000E, WHOLE, "addiu t1,zero,0xE"),
    0x8007567C: (0x3C01800E, ADDR, "lui at,0x800E"),
    0x80075684: (0xA02971ED, ADDR, "sb t1,0x71ED(at): game_state.icon_count = 0xE"),
    0x80075A24: (0x24040080, WHOLE, "addiu a0,zero,0x80"),
    0x80075A28: (0x0C015CE4, WHOLE, "jal set_dec_bri (cached background)"),
    0x80075AD8: (0x0C015CE4, WHOLE, "jal set_dec_bri (after CD decode)"),
    0x80075ADC: (0x24040080, WHOLE, "addiu a0,zero,0x80 (delay slot)"),
    0x80057390: (0x3C01800B, WHOLE, "set_dec_bri: lui at,0x800B"),
    0x80057394: (0x03E00008, WHOLE, "set_dec_bri: jr ra"),
    0x80057398: (0xA024593C, WHOLE, "set_dec_bri: sb a0,0x593C(at): bg_brightness"),
}
DIAL_KEYS_VADDR = 0x800B6990
DIAL_POS_VADDR = 0x800B6998
SESSION_FIELDS = {
    "icon_mode": "player setting (boot_settings_defaults_set, options, memory-card load)",
    "input_device": "updated each frame from the connected controller (pad_input.c)",
    "screen_brightness": "global fade state written by many scenes before the holiday",
    "tile_layer_enabled": "global layer state written by preceding scenes",
    "boot_hold": "boot/reset latch outside the holiday scene",
    "clubs": "per-save club records (boot_event_state_init, club scenes)",
}


class HolidaySceneError(ValueError):
    pass


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def check_source(root: Path) -> dict:
    """Return pinned witness hashes; reject any changed or missing anchor."""
    checked = {}
    for relative, expected in SOURCE_FILES.items():
        path = root / relative
        if path.is_symlink() or not path.is_file():
            raise HolidaySceneError("source witness is unavailable: " + relative)
        data = path.read_bytes()
        if _sha(data) != expected:
            raise HolidaySceneError("source witness hash differs: " + relative)
        text = data.decode("utf-8")
        if any(anchor not in text for anchor in SOURCE_ANCHORS[relative]):
            raise HolidaySceneError("source witness anchor is missing: " + relative)
        checked[relative] = expected
    return checked


def _offset(vaddr: int, size: int) -> int:
    if vaddr < LOAD_ADDRESS or vaddr + size > LOAD_ADDRESS + TEXT_SIZE:
        raise HolidaySceneError("address is outside the loaded executable")
    return HEADER_SIZE + vaddr - LOAD_ADDRESS


def _word(exe: bytes, vaddr: int) -> int:
    return struct.unpack_from("<I", exe, _offset(vaddr, 4))[0]


def _imm(word: int) -> int:
    value = word & 0xFFFF
    return value - 0x10000 if value & 0x8000 else value


def derive(exe: bytes) -> dict:
    """Decode the scene-owned values from authenticated executable bytes."""
    if len(exe) < HEADER_SIZE or exe[:8] != b"PS-X EXE":
        raise HolidaySceneError("executable lacks the PS-X EXE signature")
    if struct.unpack_from("<II", exe, 0x18) != (LOAD_ADDRESS, TEXT_SIZE) or len(exe) != HEADER_SIZE + TEXT_SIZE:
        raise HolidaySceneError("PS-X EXE load address or extent differs")
    for vaddr, (expected, mask, meaning) in CODE.items():
        if _word(exe, vaddr) & mask != expected & mask:
            raise HolidaySceneError("original instruction differs at 0x%08X (%s)" % (vaddr, meaning))
    # The address immediates are not pinned above; decode them from the
    # lui/immediate pairs and require the source symbol addresses.
    pos_addr = ((_word(exe, 0x800755B4) & 0xFFFF) << 16) + _imm(_word(exe, 0x800755B8))
    keys_addr = ((_word(exe, 0x800755C0) & 0xFFFF) << 16) + _imm(_word(exe, 0x800755C4))
    icon_addr = ((_word(exe, 0x8007567C) & 0xFFFF) << 16) + _imm(_word(exe, 0x80075684))
    if (pos_addr, keys_addr, icon_addr) != (DIAL_POS_VADDR, DIAL_KEYS_VADDR, GAME_STATE + ICON_COUNT_OFFSET):
        raise HolidaySceneError("decoded holiday addresses differ from the source symbols")
    jal = _word(exe, 0x80075A28)
    if jal != _word(exe, 0x80075AD8) or 0x80000000 | ((jal & 0x03FFFFFF) << 2) != 0x80057390:
        raise HolidaySceneError("both background paths must call set_dec_bri")
    ring = exe[_offset(keys_addr, 8):_offset(keys_addr, 8) + 8]
    initial_pos = exe[_offset(pos_addr, 1)]
    if initial_pos != 0:
        raise HolidaySceneError("original dial position initializer differs")
    # holiday_open_init stores 0xFF to keys 0..6; key 7 is never written
    # (the write index wraps at 7), so it keeps its .data initializer.
    dial_keys = [0xFF] * 7 + [ring[7]]
    if list(ring[:7]) != [0xFF] * 7:
        raise HolidaySceneError("original dial ring initializer differs")
    # The position store's source register (rt) is $zero.
    if (_word(exe, 0x800755B8) >> 16) & 0x1F != 0:
        raise HolidaySceneError("dial position reset is not a store of $zero")
    return {
        "values": {"dial_keys": dial_keys, "dial_pos": 0,
                   "icon_count": _imm(_word(exe, 0x80075678)),
                   "background_brightness": _imm(_word(exe, 0x80075A24))},
        "evidence": {
            "dial_keys": {"code": ["0x800755C0", "0x800755C4"], "data_vaddr": "0x%08X" % keys_addr,
                          "data_sha256": _sha(ring), "rule": "keys 0..6 = 0xFF at open; key 7 = .data initializer"},
            "dial_pos": {"code": ["0x800755B4", "0x800755B8"], "data_vaddr": "0x%08X" % pos_addr,
                         "rule": "sb zero at open"},
            "icon_count": {"code": ["0x80075678", "0x8007567C", "0x80075684"],
                           "target_vaddr": "0x%08X" % icon_addr},
            "background_brightness": {"code": ["0x80075A24", "0x80075A28", "0x80075AD8", "0x80075ADC",
                                               "0x80057390", "0x80057394", "0x80057398"],
                                      "rule": "both cached and decoded background paths call set_dec_bri(0x80)"},
        },
        "instructions": {"0x%08X" % v: {"word": "0x%08X" % _word(exe, v), "meaning": m}
                         for v, (_w, _mask, m) in CODE.items()},
    }


def build_profile(input_path: str, root: Path) -> dict:
    sys.path.insert(0, str(root))
    from tools.assets.input import open_input
    witnesses = check_source(root)
    with open_input(input_path) as source:
        description = source.describe()
        if description.get("format") != "mode2-2352" or description.get("source_sha256") != TRACK_SHA256:
            raise HolidaySceneError("original input is not the pinned MODE2 release track")
        matches = [e for e in source.entries() if e.get("path") == EXE_PATH]
        if len(matches) != 1 or matches[0].get("kind") != "file":
            raise HolidaySceneError("original track lacks one readable executable entry")
        entry = matches[0]
        location = entry.get("location", {})
        actual = {"lba": location.get("lba"), "size": entry.get("size"),
                  "record_lba": location.get("record_lba"), "record_offset": location.get("record_offset")}
        if actual != EXE_ENTRY:
            raise HolidaySceneError("executable ISO directory entry differs from the pinned record")
        exe = source.read(entry)
    if _sha(exe) != EXE_SHA256:
        raise HolidaySceneError("original executable hash mismatch")
    derived = derive(exe)
    return {
        "schema": SCHEMA, "version": 1,
        "boundary": "holiday_open_init completed: mode 0x46 entering stage 1",
        "provenance": {"original_input_sha256": TRACK_SHA256,
                       "iso_entry": dict(EXE_ENTRY, path=EXE_PATH, sha256=EXE_SHA256),
                       "load_address": "0x%08X" % LOAD_ADDRESS, "source_witnesses": witnesses,
                       "readers": ["tools/assets/input.py:open_input", "tools/port/holiday_scene_content.py:derive"]},
        "scene_owned": derived["values"],
        "evidence": derived["evidence"],
        "instructions": derived["instructions"],
        "session_owned_fields": dict(SESSION_FIELDS),
        "limitations": ["no session-owned field has a value; the consumer must take them from the live session",
                        "this profile does not reproduce the April 4 RAM capture"],
    }


def export(input_path: str, root: Path, output: Path) -> dict:
    if output.is_symlink() or output.exists():
        raise HolidaySceneError("output must be a new directory")
    profile = build_profile(input_path, root)
    output.parent.mkdir(parents=True, exist_ok=True)
    stage = Path(tempfile.mkdtemp(prefix=".holiday-scene-", dir=output.parent))
    try:
        payload = (json.dumps(profile, sort_keys=True, indent=2) + "\n").encode("utf-8")
        (stage / "manifest.json").write_bytes(payload)
        if output.exists() or output.is_symlink():
            raise HolidaySceneError("output appeared during export")
        os.rename(stage, output)
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise
    return {"path": str(output), "manifest_sha256": _sha(payload)}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, help="pinned original MODE2 data track")
    parser.add_argument("--source-root", type=Path, required=True, help="repository with pinned C/ASM witnesses")
    parser.add_argument("--output", type=Path, required=True, help="new output directory")
    args = parser.parse_args()
    try:
        result = export(args.input, args.source_root.resolve(), args.output)
    except (HolidaySceneError, OSError, ValueError) as exc:
        print("holiday scene export failed: " + str(exc), file=sys.stderr)
        return 2
    print(json.dumps(result, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
