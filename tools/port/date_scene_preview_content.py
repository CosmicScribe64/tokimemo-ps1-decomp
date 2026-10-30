#!/usr/bin/env python3
"""Export authenticated Tokimeki Memorial holiday appointment + date arrival assets."""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if not (ROOT / "tools" / "assets").is_dir():
    ROOT = Path.cwd().resolve()
for candidate_path in (ROOT, ROOT / "tools"):
    if str(candidate_path) not in sys.path:
        sys.path.insert(0, str(candidate_path))

from assets import image_mdec, image_packs_portrait, text, text_binary


HOLIDAY_ROOM_BG = {
    "id": "holiday_room_16318",
    "cdrom_path": "CDROM/BS/C_02_01H.BS",
    "lba": 16318,
    "size": 16048,
    "width": 320,
    "height": 88,
    "padded_height": 96,
    "screen_x": 0,
    "screen_y": 72,
    "sha256": "98f97915de1e3ec174f47a6fcac79b0dd84052e870e4f6a4babed473ae2a7252",
}

DATE_BACKGROUNDS = [
    {
        "id": "park_17112",
        "place_id": 0,
        "place_name": "近所の公園",
        "cdrom_path": "CDROM/BS/D_09_02.BS",
        "lba": 17112,
        "size": 18560,
        "width": 320,
        "height": 160,
        "padded_height": 160,
        "screen_x": 0,
        "screen_y": 0,
        "sha256": "8d94a5659b04b547446c6d628dc02375a54289f85b8b9e31c9ac59fd8e224d0d",
    },
    {
        "id": "aquarium_17809",
        "place_id": 3,
        "place_name": "水族館",
        "cdrom_path": "CDROM/BS/D_21_02.BS",
        "lba": 17809,
        "size": 17100,
        "width": 320,
        "height": 160,
        "padded_height": 160,
        "screen_x": 0,
        "screen_y": 0,
        "sha256": "f351d3741b1a6dd54c321ea6e830e7741003fa460feb654bd3b0a852c5146b4f",
    },
]

SHIORI_PORTRAIT = {
    "id": "shiori_hu10_21938",
    "girl_id": 0,
    "cdrom_path": "CDROM/BUSTUP/HU10.BIN",
    "lba": 21938,
    "size": 92160,
    "sha256": "44cd14d428c17551399487f8325961a17d3a3c3e87c69e64fdf09c8ff68536e2",
    "actor_slot": 31,
    "actor_x": 0,
    "actor_y": -32,
    "origin_x": 79,
    "origin_y": 80,
    "screen_x": 81,
    "screen_y": 8,
    "width": 160,
    "height": 160,
}

SHIORI_DATE_SCRIPT = {
    "id": "date_fu_44329",
    "girl_id": 0,
    "cdrom_path": "CDROM/SERIFU/DATE_FU.BIN",
    "lba": 44329,
    "size": 47104,
    "sha256": "667597bc307eaa52ef3f11d83e6850048f9e94831578527b58f12cb48256c005",
    "profile": {
        "id": "serifu-date-fu-arrival",
        "path": "CDROM/SERIFU/DATE_FU.BIN",
        "sha256": "667597bc307eaa52ef3f11d83e6850048f9e94831578527b58f12cb48256c005",
        "load_address": 0x80198000,
        "sections": [
            {
                "id": "date_arrival",
                "text_table": 0x8019B658,
                "command_table": 0x8019B6CC,
                "voice_table": 0x8019BAD8,
                "group_count": 14,
            },
        ],
        "evidence": [
            "src/ovl/date/entry.c:date_load_girl_script",
            "src/ovl/date/place_scenes.c:date_fill_sections_shiori,date_arrival_run",
        ],
    },
}


def _sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _sha256_file(path: Path) -> str:
    return _sha256_bytes(path.read_bytes())


def _read_extent(disc_path: Path, lba: int, size: int) -> bytes:
    sectors = (size + 2047) // 2048
    out = bytearray()
    with disc_path.open("rb") as handle:
        for sector_idx in range(sectors):
            handle.seek((lba + sector_idx) * 2352 + 24)
            out.extend(handle.read(2048))
    return bytes(out[:size])


EXE_SHA = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"


def _extract_source_strings(exe_raw: bytes) -> dict:
    binary_spec = (exe_raw, 0x80041000, 0x800, EXE_SHA, "SLPM_86.053")

    def _span_map(rel_path: str) -> dict:
        out = {}
        for span in text_binary._spans(ROOT / rel_path, ROOT, binary_spec):
            raw = bytes(span["raw"])
            end = raw.find(b"\0")
            if end > 0:
                raw = raw[:end]
            out[span["label"]] = {
                "symbol": span["label"],
                "address": hex(span["address"]) if span.get("address") is not None else None,
                "text": raw.decode("shift_jis", errors="replace"),
                "bytes_hex": raw.hex(),
            }
        return out

    rodata_spans = _span_map("asm/data/main/holiday.rodata.s")
    data_spans = _span_map("asm/data/main/holiday.data.s")
    global_data_spans = _span_map("asm/data/data.data.s")
    return {
        "holiday_rodata": {
            key: rodata_spans[key]
            for key in ("D_800B16B8", "D_800B16C4", "D_800B16DC", "D_800B16E8", "D_800B16F0")
            if key in rodata_spans
        },
        "holiday_data": {
            key: data_spans[key]
            for key in ("D_800AA8E0", "D_800AA8EC")
            if key in data_spans
        },
        "shiori_called_name": global_data_spans.get("D_800A92A0"),
        "shiori_speaker_name": global_data_spans.get("D_800A9120"),
    }


def build_pack(disc_path: Path, exe_path: Path, out_dir: Path) -> dict:
    if out_dir.exists():
        shutil.rmtree(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    bg_dir = out_dir / "backgrounds"
    portrait_dir = out_dir / "portraits" / "shiori_hu10"
    script_dir = out_dir / "scripts"
    bg_dir.mkdir(parents=True, exist_ok=True)
    portrait_dir.mkdir(parents=True, exist_ok=True)
    script_dir.mkdir(parents=True, exist_ok=True)

    exe_raw = exe_path.read_bytes()

    # 1. Holiday room background (320x88 at (0, 72))
    room_raw = _read_extent(disc_path, HOLIDAY_ROOM_BG["lba"], HOLIDAY_ROOM_BG["size"])
    room_sha = _sha256_bytes(room_raw)
    if room_sha != HOLIDAY_ROOM_BG["sha256"]:
        raise ValueError(f"Holiday room SHA mismatch: {room_sha}")
    room_res = image_mdec.export_mdec(
        room_raw,
        bg_dir,
        HOLIDAY_ROOM_BG["id"],
        source_path=HOLIDAY_ROOM_BG["cdrom_path"],
        context={
            "width": HOLIDAY_ROOM_BG["width"],
            "height": HOLIDAY_ROOM_BG["height"],
            "padded_height": HOLIDAY_ROOM_BG["padded_height"],
            "evidence": [
                "src/main/holiday.c:holiday_open_init",
                "src/main/background_images.c:dec_bg_draw",
            ],
        },
    )
    room_png = bg_dir / room_res["images"][0]["png"]

    # 2. Date backgrounds (320x160 at (0, 0))
    exported_bgs = []
    for bg in DATE_BACKGROUNDS:
        raw = _read_extent(disc_path, bg["lba"], bg["size"])
        sha = _sha256_bytes(raw)
        if sha != bg["sha256"]:
            raise ValueError(f"Date background {bg['id']} SHA mismatch: {sha}")
        bg_res = image_mdec.export_mdec(
            raw,
            bg_dir,
            bg["id"],
            source_path=bg["cdrom_path"],
            context={
                "width": bg["width"],
                "height": bg["height"],
                "padded_height": bg["padded_height"],
                "evidence": [
                    "src/ovl/date/place_scenes.c:date_scene_park_loop",
                    "src/main/background_images.c:dec_bg_draw",
                ],
            },
        )
        png_path = bg_dir / bg_res["images"][0]["png"]
        exported_bgs.append({
            **bg,
            "decoder_model": bg_res["metadata"]["decoder_model"],
            "png": png_path.relative_to(out_dir).as_posix(),
            "png_sha256": _sha256_file(png_path),
        })

    # 3. Shiori Spring portrait (CDROM/BUSTUP/HU10.BIN)
    portrait_raw = _read_extent(disc_path, SHIORI_PORTRAIT["lba"], SHIORI_PORTRAIT["size"])
    portrait_sha = _sha256_bytes(portrait_raw)
    if portrait_sha != SHIORI_PORTRAIT["sha256"]:
        raise ValueError(f"Shiori portrait SHA mismatch: {portrait_sha}")
    portrait_meta = image_packs_portrait.export_portrait(
        portrait_raw,
        exe_raw,
        portrait_dir,
        SHIORI_PORTRAIT["id"],
    )
    static_img = next(img for img in portrait_meta["images"] if img["variant"] == "static-frame")
    static_png = portrait_dir / static_img["png"]

    # 4. Shiori date script (CDROM/SERIFU/DATE_FU.BIN)
    script_raw = _read_extent(disc_path, SHIORI_DATE_SCRIPT["lba"], SHIORI_DATE_SCRIPT["size"])
    script_sha = _sha256_bytes(script_raw)
    if script_sha != SHIORI_DATE_SCRIPT["sha256"]:
        raise ValueError(f"Shiori script SHA mismatch: {script_sha}")
    parsed_script = text.parse_script(
        script_raw,
        SHIORI_DATE_SCRIPT["cdrom_path"],
        SHIORI_DATE_SCRIPT["profile"],
    )
    # Group 6 (index 6 in initializer-D_8015BE1C = DATE_ARRIVAL_DIRECT_GREETING), line 1 (Shiori arrival response)
    arrival_line = next(
        rec for rec in parsed_script["records"]
        if rec.get("section") == "date_arrival"
        and rec.get("group") == 6
        and rec.get("line") == 1
    )
    script_json_path = script_dir / "date_fu_arrival.json"
    script_json_path.write_text(
        json.dumps(parsed_script, ensure_ascii=False, indent=2) + "\n",
        encoding="utf-8",
    )

    manifest = {
        "schema": "tokimemo.port.date_preview.v1",
        "holiday_room_background": {
            **HOLIDAY_ROOM_BG,
            "decoder_model": room_res["metadata"]["decoder_model"],
            "png": room_png.relative_to(out_dir).as_posix(),
            "png_sha256": _sha256_file(room_png),
        },
        "date_backgrounds": exported_bgs,
        "shiori_portrait": {
            **SHIORI_PORTRAIT,
            "static_png": static_png.relative_to(out_dir).as_posix(),
            "static_png_sha256": _sha256_file(static_png),
            "bounds": static_img["bounds"],
            "origin": static_img["origin"],
        },
        "shiori_date_script": {
            "id": SHIORI_DATE_SCRIPT["id"],
            "girl_id": SHIORI_DATE_SCRIPT["girl_id"],
            "cdrom_path": SHIORI_DATE_SCRIPT["cdrom_path"],
            "lba": SHIORI_DATE_SCRIPT["lba"],
            "size": SHIORI_DATE_SCRIPT["size"],
            "sha256": SHIORI_DATE_SCRIPT["sha256"],
            "arrival_section_json": script_json_path.relative_to(out_dir).as_posix(),
            "arrival_direct_greeting": {
                "group_index": 6,
                "command_index": 1,
                "command": arrival_line["command"],
                "speaker_text": "藤崎",
                "speaker_bytes_hex": "93a18de8",
                "text": arrival_line["text"]["source_decoded_text"],
                "bytes_hex": arrival_line["text"]["raw_hex"],
                "offset": arrival_line["text"]["offset"],
                "voice": arrival_line.get("voice"),
            },
        },
        "source_strings": _extract_source_strings(exe_raw),
    }
    manifest_path = out_dir / "manifest.json"
    manifest_path.write_text(json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
    return manifest


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--disc", type=Path, default=Path("disc/track1.bin"))
    parser.add_argument("--exe", type=Path, default=Path("disc/files/SLPM_86.053"))
    parser.add_argument("--output", type=Path, default=Path(".asset-cache/port/date-preview-v1"))
    args = parser.parse_args()
    manifest = build_pack(args.disc, args.exe, args.output)
    print(json.dumps({
        "manifest": str(args.output / "manifest.json"),
        "holiday_room_png": manifest["holiday_room_background"]["png"],
        "date_backgrounds": [b["png"] for b in manifest["date_backgrounds"]],
        "shiori_portrait": manifest["shiori_portrait"]["static_png"],
        "arrival_greeting": manifest["shiori_date_script"]["arrival_direct_greeting"]["text"],
    }, ensure_ascii=False))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
