#!/usr/bin/env python3
"""Build the bounded, original-backed prologue HUD resource pack.

The pack contains the source-indexed checker, selected MAINROM actor parts,
calendar tiles, and the title extent. It does not compose a complete scene or
claim runtime GPU completion or pixel parity.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import shutil
import sys
import tempfile


MAIN_EXE = "SLPM_86.053"
MAINROM = "CDROM/FUKUTEST/MAINROM.NBN"
CALROM = "CDROM/FUKUTEST/CALROM.NBN"
NAME_ENT = "CDROM/EXEDIR/NAME_ENT.EXN"
INPUT_SHA256 = {
    MAIN_EXE: "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae",
    MAINROM: "f425e1d19d64afcbafeadece7d72602e50e26ce88093047f73dbb0d21a9b9c1c",
    CALROM: "775356bc13a15499b79172e57ea9d42e753fd916e0c01c166f42bb426fe5c867",
    NAME_ENT: "f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2",
}
SOURCE_PINS = {
    "src/main/hud_panels.c": "b942acb5863f26326c044720768384cf23b156f4736c69f1eda0d34831016ae1",
    "src/main/hud_screens.c": "4572545b4566949100d038bff53ab9cdcc0dc63fb31c3de155119ff7141251b0",
    "src/ovl/name_entry/prologue.c": "cce3a4abaf102b745e0b116fcd1331ec63b35234d45b5bab4cbd9c39fc10470e",
    "src/main/game_init.c": "ecd91c6bc45e2670ce332b5d53fb9e5fe2cfb767bb9e0c46e09f3741595bed53",
    "src/main/debug_hud_palette.c": "a2bf8f42fb2857ebe80177d6c1ef5222e5952440bc9c8424a9b88cecf8961f7b",
    "src/main/main_loop.c": "c32d3d6d4d9440ebcbc2f45a5f8a7ebc0774a2cf7e1790c8b5b195c5c93d80a7",
    "src/ovl/name_entry/opening.c": "d4cbc22124d25f33d958c82f07f2f7c5674acf61803b5ac7969ae9bba761e5b1",
    "src/ovl/name_entry/file_select.c": "ff3e5ca243684c1a90023f4fa33a9ce5666d7b5ca773056b972b6f8420b856f5",
    "asm/data/data_800B5950.data.s": "4482f3ec44d7a64fbcb1b305e015452e014382d6d819dd8759e730d3f0b2d439",
    "asm/data/main/text_system.data.s": "9eafc7dd540f7823b7afa43c8b06dc39b9fab41ad7bdf39df1b921e077931510",
    "asm/ovl/NAME_ENT/data/name_entry/prologue.rodata.s": "cfac6e0ef3b45316f6b75ea13e49cfe7f36075008489d32ebefe9d48e98872b4",
    "asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_arrival_scene_init.s": "38586534cee5fa8c3d1fa5dca44ad2ae7aa647917b6e5c45638a7faf56eb7df9",
    "include/main/hud.h": "dd5a7374a8469741052dc7abb8162fe98901eccbdaeb008f1aa97ecd48b84fd3",
}
FRAMES = (60, 62, *range(68, 78), *range(82, 89), *range(113, 116))
FONT_RELATIVE = ".asset-cache/port/fonts/native-font-v2"
PORTABLE_SCHEMA = "tokimemo.prologue-hud-installed-profile.v1"
ASSET_SCHEMA = "tokimemo.prologue-hud-assets.v1"
CHECKER_SCHEMA = "tokimemo.native-hud-checker-indexed.v2"
FONT_DESCRIPTOR_SHA256 = "675a40455cb34ab2f26203e43208bcb9d610ce0c428a4fd4d6bcee1d037a9e77"
FONT_SOURCE_SHA256 = INPUT_SHA256[MAIN_EXE]
FONT_PAYLOAD_SHA256 = "6c60c8863b4e15407c5fced0f316351e9cf2e18492518f812aa7606218d4159a"
CHECKER_ASSEMBLY = "asm/data/data_800B5950.data.s"
TITLE_ASSEMBLY = "asm/ovl/NAME_ENT/data/name_entry/prologue.rodata.s"
TITLE_ADDRESS = 0x8014C920
TITLE_LENGTH = 10


class HudPackError(ValueError):
    pass


def _sha(raw: bytes) -> str:
    return hashlib.sha256(raw).hexdigest()


def _file_sha(path: Path) -> str:
    return _sha(path.read_bytes())


def _safe_rel(value: object) -> bool:
    if not isinstance(value, str) or not value or "\\" in value or ":" in value:
        return False
    item = PurePosixPath(value)
    return (not item.is_absolute() and item.as_posix() == value
            and all(part not in ("", ".", "..") for part in item.parts))


def _validate_output_role(path: Path) -> None:
    if path.name != "prologue_hud" or path.parent.name != "packs":
        raise HudPackError("output must be the final bundle packs/prologue_hud role path")


def _physical_path(path: Path, description: str, *, must_exist: bool) -> Path:
    """Resolve a path only after rejecting traversal and symlink components."""
    candidate = path if path.is_absolute() else Path.cwd() / path
    if ".." in candidate.parts:
        raise HudPackError(description + " path contains parent traversal")
    current = Path(candidate.anchor)
    for component in candidate.parts[1:]:
        current = current / component
        if current.is_symlink():
            raise HudPackError(description + " path has a symlink component")
    if must_exist and not candidate.exists():
        raise HudPackError(description + " path does not exist")
    try:
        return candidate.resolve(strict=must_exist)
    except (OSError, RuntimeError) as error:
        raise HudPackError(description + " path cannot be resolved") from error


def _validate_build_paths(source_root: Path, input_path: Path, destination: Path) -> tuple[Path, Path]:
    """Reject writes into protected data and any physical input/output overlap."""
    source_root = _physical_path(source_root, "source repository", must_exist=True)
    original = _physical_path(input_path, "original input", must_exist=True)
    output = _physical_path(destination, "output", must_exist=False)

    for protected_name in (".git", "disc", "raw"):
        protected = source_root / protected_name
        if output == protected or protected in output.parents:
            raise HudPackError("output cannot be written inside protected repository data")

    overlaps = output == original
    if original.is_dir():
        overlaps = overlaps or original in output.parents or output in original.parents
    if overlaps:
        raise HudPackError("output overlaps the original input")
    return original, output


def _require_source_file(root: Path, rel: str) -> Path:
    if not _safe_rel(rel):
        raise HudPackError("unsafe source evidence path")
    path = root / rel
    if path.is_symlink() or not path.is_file():
        raise HudPackError("required source evidence is missing or linked")
    resolved = path.resolve(strict=True)
    if not resolved.is_relative_to(root):
        raise HudPackError("source evidence escapes repository")
    return resolved


def _verify_source_pins(source_root: Path) -> list[dict]:
    pins = []
    for rel, expected in sorted(SOURCE_PINS.items()):
        path = _require_source_file(source_root, rel)
        digest = _file_sha(path)
        if expected and digest != expected:
            raise HudPackError("source evidence hash differs from accepted source profile")
        pins.append({"path": rel, "sha256": digest})
    # The title label and its address are checked independently of these hashes.
    asm = _require_source_file(source_root, TITLE_ASSEMBLY).read_text(encoding="utf-8")
    if "dlabel D_8014C920" not in asm or "/* 1A920 8014C920 */" not in asm:
        raise HudPackError("title data label is not anchored at the reviewed overlay address")
    call = _require_source_file(source_root,
        "asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_arrival_scene_init.s").read_text(encoding="utf-8")
    if "%hi(D_8014C920)" not in call or "%lo(D_8014C920)" not in call:
        raise HudPackError("title source label is not referenced by the reviewed scene initializer")
    return pins


def _input_bytes(source, wanted: str) -> bytes:
    matches = [item for item in source.entries()
               if item.get("kind") == "file" and item.get("path", "").casefold() == wanted.casefold()]
    if len(matches) != 1:
        raise HudPackError("original input does not contain exactly one required HUD source")
    raw = source.read(matches[0])
    if _sha(raw) != INPUT_SHA256[wanted]:
        raise HudPackError("original input differs from the reviewed release profile")
    return raw


def _asset_record(root: Path, rel: str, width: int, height: int, kind: str) -> dict:
    path = root / rel
    raw = path.read_bytes()
    return {"path": rel, "size": len(raw), "sha256": _sha(raw),
            "width": width, "height": height, "kind": kind}


def _selected_frames(mainrom: bytes, exe: bytes, asset_root: Path) -> dict:
    from tools.assets.image_packs import _parse, _reader
    from tools.assets.images import _color, _png

    read, _base, frames, _sequences = _parse(mainrom, exe)
    reader, _ = _reader(exe)
    result: dict[str, dict] = {}
    files_dir = asset_root / "assets"
    files_dir.mkdir(parents=True)
    for frame_id in FRAMES:
        frame = frames[frame_id]
        parts = []
        for part in frame["parts"]:
            info = int(part["palette_info"])
            row, column = (info >> 8) & 15, info & 0xF0
            palette_address = 0x800C6020 + row * 512 + column * 2
            palette = tuple(int.from_bytes(reader(palette_address + i * 2, 2), "little")
                            for i in range(16))
            if len(palette) != 16:
                raise HudPackError("selected frame palette is truncated")
            preview_palette = tuple(word | 0x8000 if word & 0x7FFF and
                                    (0x800C6200 <= palette_address < 0x800C6220 or
                                     0x800C62C0 <= palette_address < 0x800C6340) else word
                                    for word in palette)
            width, height = int(part["width"]), int(part["height"])
            rgba = bytearray(width * height * 4)
            stp = bytearray(width * height)
            sheet_start = int(part["sheet_source_offset"])
            for y in range(height):
                sy = height - 1 - y if int(part["flip_flags"]) & 1 else y
                for x in range(width):
                    sx = width - 1 - x if int(part["flip_flags"]) & 2 else x
                    texel = (int(part["v"]) + sy) * 128 + int(part["u"]) + sx
                    packed = mainrom[sheet_start + texel // 2]
                    word = preview_palette[(packed >> (4 * (texel & 1))) & 15]
                    if word == 0:
                        continue
                    rgba[(y * width + x) * 4:(y * width + x + 1) * 4] = _color(word)
                    stp[y * width + x] = 255 if word & 0x8000 else 0
            stem = "f%03d.base.p%02d" % (frame_id, int(part["part_index"]))
            color_rel, stp_rel = "assets/" + stem + ".png", "assets/" + stem + ".stp.png"
            (asset_root / color_rel).write_bytes(_png(width, height, rgba, 4))
            (asset_root / stp_rel).write_bytes(_png(width, height, stp, 1))
            parts.append({**part,
                "palette_exe_address": palette_address,
                "palette_exe_offset": palette_address - 0x80041000 + 0x800,
                "source_palette_words": list(palette),
                "preview_palette_words": list(preview_palette),
                "png": _asset_record(asset_root, color_rel, width, height, "color"),
                "stp_png": _asset_record(asset_root, stp_rel, width, height, "stp")})
        result["%d:base" % frame_id] = {
            "frame_index": frame_id, "origin": frame["origin"],
            "bounds": frame["bounds"], "parts": parts,
            "variant": "base", "palette_variant_scope": "base source palette"}
    return result


def _checker(mainrom: bytes, exe: bytes, source_root: Path, root: Path) -> dict:
    from tools.assets.text_binary import _spans

    texture = mainrom[0xC000:0xE000]
    if len(texture) != 0x2000:
        raise HudPackError("checker source extent is truncated")
    indices = bytes(value for packed in texture for value in (packed & 15, packed >> 4))
    palette_offset = 0x75328
    palettes = exe[palette_offset:palette_offset + 128]
    if len(palettes) != 128:
        raise HudPackError("checker palette source extent is truncated")
    source_asm = _require_source_file(source_root, CHECKER_ASSEMBLY)
    if _file_sha(source_asm) != SOURCE_PINS[CHECKER_ASSEMBLY]:
        raise HudPackError("checker assembly proof pin changed")
    binary = (exe, 0x80041000, 0x800, _sha(exe), MAIN_EXE)
    target_address = 0x80041000 + palette_offset - 0x800
    coverage = bytearray(len(palettes))
    for span in _spans(source_asm, source_root, binary):
        address = span.get("address")
        if address is None:
            continue
        start = max(target_address, address)
        end = min(target_address + len(palettes), address + len(span["raw"]))
        if start < end:
            raw = bytes(span["raw"])[start - address:end - address]
            offset = start - target_address
            if exe[palette_offset + offset:palette_offset + offset + len(raw)] != raw:
                raise HudPackError("checker palette ASM bytes disagree with original executable")
            coverage[offset:offset + len(raw)] = b"\1" * len(raw)
    if not all(coverage):
        raise HudPackError("checker palette extent is not fully assembly anchored")
    directory = root / "checker"
    directory.mkdir()
    (directory / "indices.bin").write_bytes(indices)
    (directory / "palettes.bin").write_bytes(palettes)
    return {
        "dimensions": [128, 128],
        "inputs": {"executable": {"path": MAIN_EXE, "sha256": _sha(exe)},
                   "mainrom": {"path": MAINROM, "sha256": _sha(mainrom)}},
        "payloads": {
            "indices": {"path": "indices.bin", "sha256": _sha(indices), "size": len(indices)},
            "palettes": {"path": "palettes.bin", "sha256": _sha(palettes), "size": len(palettes)}},
        "schema": "tokimemo.native-hud-checker-indexed.v2",
        "source_mapping": {"clut_vram": [160, 481], "mainrom_offset": 0xC000,
            "palette_exe_offset": palette_offset, "source_stride_bytes": 64,
            "sprite_size": [32, 32], "tpage": [960, 256],
            "upload_vram_rect": [992, 384, 32, 128], "uv_base": [128, 128]}}


def _font_identity(bundle_root: Path) -> tuple[dict, dict]:
    directory = bundle_root / FONT_RELATIVE
    descriptor = directory / "font.json"
    payload = directory / "glyph_bank.bin"
    if (directory.is_symlink() or not directory.is_dir()
            or descriptor.is_symlink() or payload.is_symlink()
            or not descriptor.is_file() or not payload.is_file()):
        raise HudPackError("fresh installed font pack must exist before HUD pack generation")
    root_resolved = bundle_root.resolve(strict=True)
    if not directory.resolve(strict=True).is_relative_to(root_resolved):
        raise HudPackError("installed font pack escapes the bundle root")
    raw = descriptor.read_bytes()
    if _sha(raw) != FONT_DESCRIPTOR_SHA256:
        raise HudPackError("installed font descriptor differs from the reviewed source pack")
    parsed = json.loads(raw)
    if (parsed.get("schema") != "tokimemo.native-font.v1"
            or parsed.get("metadata", {}).get("source_sha256") != FONT_SOURCE_SHA256
            or parsed.get("payload_sha256") != FONT_PAYLOAD_SHA256
            or parsed.get("payload_size_bytes") != 3418 * 18):
        raise HudPackError("installed font descriptor does not match the supported source profile")
    if _file_sha(payload) != FONT_PAYLOAD_SHA256 or payload.stat().st_size != 3418 * 18:
        raise HudPackError("installed font glyph payload differs from its descriptor")
    return ({"directory": FONT_RELATIVE, "manifest": "font.json",
             "manifest_sha256": FONT_DESCRIPTOR_SHA256,
             "schema": "tokimemo.native-font.v1"},
            {"path": FONT_RELATIVE + "/glyph_bank.bin",
             "sha256": FONT_PAYLOAD_SHA256, "size": 3418 * 18})


def _write_json(path: Path, value: dict) -> bytes:
    raw = (json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(",", ":")) + "\n").encode("utf-8")
    path.write_bytes(raw)
    return raw


def build_from_input(input_path: str | Path, source_root: str | Path, output: str | Path) -> dict:
    """Build ``<bundle>/packs/prologue_hud`` from one supported original input.

    The bundle root must already contain the separately source-generated font
    pack at ``.asset-cache/port/fonts/native-font-v2``. ``output`` must be the
    final ``<bundle>/packs/prologue_hud`` path. Generation occurs in a private
    sibling directory and publishes once.
    """
    source_root = _physical_path(Path(source_root), "source repository", must_exist=True)
    if not (source_root / "tools" / "assets" / "input.py").is_file():
        raise HudPackError("source_root is not the canonical tooling repository")
    if str(source_root) not in sys.path:
        sys.path.insert(0, str(source_root))
    from tools.assets.input import open_input

    destination = Path(output)
    _validate_output_role(destination)
    input_path = Path(input_path)
    input_path, destination = _validate_build_paths(source_root, input_path, destination)
    bundle_root = destination.parents[1]
    if destination.exists() or destination.is_symlink():
        raise HudPackError("prologue HUD role destination must be new")
    pins = _verify_source_pins(source_root)
    font_resource, font_payload = _font_identity(bundle_root)
    try:
        with open_input(str(input_path)) as opened:
            exe, mainrom = _input_bytes(opened, MAIN_EXE), _input_bytes(opened, MAINROM)
            calrom, overlay = _input_bytes(opened, CALROM), _input_bytes(opened, NAME_ENT)
    except Exception as error:
        if isinstance(error, HudPackError):
            raise
        raise HudPackError("original input rejected by bounded input reader") from error
    title_offset = TITLE_ADDRESS - 0x80132000
    title_raw = overlay[title_offset:title_offset + TITLE_LENGTH]
    if len(title_raw) != TITLE_LENGTH or overlay[title_offset + TITLE_LENGTH] != 0:
        raise HudPackError("source-proven title extent has invalid bounds or terminator")

    destination.parent.mkdir(parents=True, exist_ok=True)
    if destination.parent.is_symlink():
        raise HudPackError("role output parent became a symlink")
    stage = Path(tempfile.mkdtemp(prefix=".prologue-hud-stage-", dir=destination.parent))
    try:
        asset_root = stage / "assets"
        frames = _selected_frames(mainrom, exe, stage)
        from tools.assets.image_packs_calendar import export_calendar_ui
        calendar_dir = stage / "calendar"
        calendar = export_calendar_ui(calrom, exe, calendar_dir, "prologue-stage0-calendar")
        calendar_records = []
        for entry in calendar["images"]:
            record = dict(entry)
            record["png"] = _asset_record(stage, "calendar/" + entry["png"],
                                          entry["width"], entry["height"], "color")
            record["stp_png"] = _asset_record(stage, "calendar/" + entry["stp_png"],
                                              entry["width"], entry["height"], "stp")
            calendar_records.append(record)
        # Calendar exports share the asset root used by the runtime provider.
        asset_dir = stage / "assets"
        asset_dir.mkdir(exist_ok=True)
        for child in calendar_dir.iterdir():
            target = asset_dir / child.name
            os.replace(child, target)
        shutil.rmtree(calendar_dir)
        for record in calendar_records:
            record["png"]["path"] = "assets/" + PurePosixPath(record["png"]["path"]).name
            record["stp_png"]["path"] = "assets/" + PurePosixPath(record["stp_png"]["path"]).name
        checker = _checker(mainrom, exe, source_root, stage)
        checker_bytes = _write_json(stage / "checker" / "manifest.json", checker)
        asset_manifest = {
            "schema": ASSET_SCHEMA, "version": 1,
            "profile": {"id": "prologue_stage0_hud_assets.v1",
                        "frames": ["%d:base" % frame_id for frame_id in FRAMES],
                        "calendar_month_count": 36},
            "frames": frames,
            "content": {"calendar": calendar_records},
            "provenance": {"source_reader": "tools.assets.image_packs._parse",
                "calendar_reader": "tools.assets.image_packs_calendar.export_calendar_ui",
                "inputs": {"executable": {"path": MAIN_EXE, "sha256": _sha(exe)},
                           "mainrom": {"path": MAINROM, "sha256": _sha(mainrom)},
                           "calendar": {"path": CALROM, "sha256": _sha(calrom)}},
                "scope": "source parts and calendar resources; no composed-scene or pixel-parity claim"}}
        asset_bytes = _write_json(stage / "assets-manifest.json", asset_manifest)
        title = {"byte_length": len(title_raw), "encoding": "Shift-JIS source bytes",
                 "raw_hex": title_raw.hex(), "source_address": TITLE_ADDRESS,
                 "source_offset": title_offset, "source_path": NAME_ENT,
                 "source_sha256": INPUT_SHA256[NAME_ENT],
                 "source_label": "D_8014C920", "source_assembly": TITLE_ASSEMBLY}
        portable = {
            "schema": PORTABLE_SCHEMA, "version": 1,
            "profile_id": "prologue_stage0_entry.v2",
            "runtime_execution_proven": False,
            "release_profile": "original-ja-ps-the-best",
            "scope": "bounded source resources and draw contract; no full-scene composition, route, GPU completion, or pixel parity",
            "inputs": {
                "executable": {"path": MAIN_EXE, "sha256": _sha(exe)},
                "mainrom": {"path": MAINROM, "sha256": _sha(mainrom)},
                "calrom": {"path": CALROM, "sha256": _sha(calrom)},
                "name_ent_overlay": {"path": NAME_ENT, "sha256": _sha(overlay)}},
            "source_evidence": pins,
            "title": title,
            "stage0_contract": {
                "canvas": [320, 240],
                "initialization_order": ["text_system_reset", "message_window_set_title", "hizuke_init", "message_window_init", "dec_bg_reset", "hizuke_disp_switch(1)"],
                "calendar_month_art": {"first_month": [1995, 4], "month_count": 36,
                    "images_per_month": ["calendar-grid", "month-label"],
                    "uses": "optional calendar-screen resource, not prologue date pillar"},
                "date_pillar": {"frame_id": 60, "frame_local_position": [144, -18],
                    "background_gradient_screen": [294, 45, 20, 114],
                    "date_source": "live GameState year/month/day/weekday and month_calendar[0].day_kind[day]"},
                "message_window": {"frame_id": 62, "frame_local_position": [0, 75],
                    "gradient_rect_screen": [27, 166, 266, 52]},
                "checker": {"raw_scroll_inputs": ["D_800B5BCC", "D_800B5BD0"],
                    "scroll_policy": "must be supplied from live state; do not default or synthesize",
                    "texture_origin": [128, 128], "tile_pixels": [32, 32], "tiles": [10, 8]},
                "title_timer_before_first_update": -64,
                "title_timer_after_first_update": -63,
                "unsupported": ["full scene composition", "runtime GPU completion", "pixel parity"]},
            "resources": {
                "checker": {"directory": "packs/prologue_hud/checker", "manifest": "manifest.json",
                            "manifest_sha256": _sha(checker_bytes), "schema": CHECKER_SCHEMA},
                "prologue_assets": {"directory": "packs/prologue_hud", "manifest": "assets-manifest.json",
                                    "manifest_sha256": _sha(asset_bytes), "schema": ASSET_SCHEMA},
                "font": font_resource},
            "font_payload": font_payload,
        }
        _write_json(stage / "manifest.json", portable)
        # Canonical installed layout stores the role at <bundle>/packs/prologue_hud.
        os.replace(stage, destination)
    except Exception:
        if stage.exists():
            shutil.rmtree(stage, ignore_errors=True)
        raise
    return {"status": "exported", "role": "prologue_hud",
            "profile": "packs/prologue_hud/manifest.json",
            "profile_sha256": _file_sha(destination / "manifest.json"),
            "frame_count": len(frames), "calendar_image_count": len(calendar_records),
            "checker_index_bytes": 16384, "checker_palette_bytes": 128,
            "source_sha256": {path: INPUT_SHA256[path] for path in INPUT_SHA256}}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, required=True, help="original extracted directory, MODE2 track, or ZIP")
    parser.add_argument("--source-root", type=Path, required=True, help="read-only canonical source/tooling checkout")
    parser.add_argument("--output", type=Path, required=True, help="new bundle/packs/prologue_hud role directory")
    args = parser.parse_args()
    try:
        result = build_from_input(args.input, args.source_root, args.output)
    except Exception as error:
        print("prologue HUD export rejected: " + str(error), file=sys.stderr)
        return 1
    print(json.dumps(result, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
