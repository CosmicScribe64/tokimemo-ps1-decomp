#!/usr/bin/env python3
"""Export the source-bound stage-0 background and bounded actor source bytes.

The background is decoded through the canonical Sony BS reader using geometry
proved from the NAME_ENT read and main decoder source. The actor NBN is retained
with bounded source-profile frame previews and loader metadata. Combined scene
composition and hardware pixel parity remain unproven.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import sys
import tempfile


class ArrivalMediaError(ValueError):
    """The original source, source proof, or output destination is invalid."""


def _project_root() -> Path:
    for parent in Path(__file__).resolve().parents:
        if (parent / "tools/assets/input.py").is_file():
            return parent
    raise ArrivalMediaError("candidate project root unavailable")


PROJECT_ROOT = _project_root()
if str(PROJECT_ROOT) not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT))
if str(PROJECT_ROOT / "tools") not in sys.path:
    sys.path.insert(0, str(PROJECT_ROOT / "tools"))

from tools.assets.input import open_input  # noqa: E402
from tools.assets import image_mdec, image_packs_prologue, media_background_context, prologue_o01  # noqa: E402
from tools.port.school_day_tables import _assert_no_symlink_path, _check_export_path  # noqa: E402


SCHEMA = "tokimemo.prologue-arrival-media.v1"
TRACK_SHA256 = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
BACKGROUND_PATH = "CDROM/BS/P_01_01.BS"
BACKGROUND_LBA = 0x494B
BACKGROUND_SIZE = 18432
BACKGROUND_SHA256 = "280b440f4f005d53340afd5389cd755a5bbb574fa30c36013df484d039fd7aab"
BACKGROUND_WIDTH = 320
BACKGROUND_HEIGHT = 160
ACTOR_PATH = "CDROM/PROLODIR/O_01.NBN"
ACTOR_LBA = 0xABAB
ACTOR_SIZE = 26624
ACTOR_SHA256 = "bb224a47fa1ac9c102dd4701d3d3e0819ba30cac1e5637183886b8be4f95340c"
ACTOR_LOAD_ADDRESS = 0x80180000
ACTOR_READ_SECTORS = 13
ACTOR_FRAME_TABLE_ADDRESS = 0x801800AC
ACTOR_SEQUENCE_TABLE_ADDRESS = 0x801800C0
ACTOR_TPAGE_TABLE_ADDRESS = 0x801800A8
ACTOR_PALETTE_OFFSETS = (0x4490, 0x4610)
OVERLAY_LOAD_BASE = 0x80132000

SOURCE_PINS = {
    "src/main/background_images.c": "7eea24dfee7fd3503beedbd1b2f633c40f19102d21f992557d626cba3d036402",
    "src/ovl/name_entry/prologue.c": "cce3a4abaf102b745e0b116fcd1331ec63b35234d45b5bab4cbd9c39fc10470e",
    "include/ovl/name_entry.h": "ced2aa4851cd232c5e3682c99c9cfa944d2b71daa78fe9c48911b134cd60f842",
    "config/overlays/NAME_ENT.yaml": "844d84467d937cd6a6af8cba884c4dbafde062ba898671f20ceadd7f8783809a",
    "config/symbols_NAME_ENT.txt": "bf7df8a2e4db9fe79899b4b65f5bfa5d685e397480edb9b6bfe4446d09e6090b",
    "asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_arrival_scene_init.s": "38586534cee5fa8c3d1fa5dca44ad2ae7aa647917b6e5c45638a7faf56eb7df9",
    "src/main/actor_sprites.c": "ec04dd6c419b8079ef9430a484eb084c44d7a52029cc1c6ea359fd8571dab546",
    "src/main/frame_render_audio.c": "b0932ca59d48bec7b3a3be55b8188b09f55f15721dbe96d84de14f2a0526d08e",
    "include/game/display_records.h": "54590f2da1130d31ded81ba2f52509f1781006646c75865b2f6fb832af8bc77b",
    "include/game/draw_records.h": "77cf6a1567a9877f55fe0238566b08af034c921da8c16f503f2c115f68efdcb2",
    "config/symbol_addrs_main.txt": "d9024a12793c93d95f9c3ff82088b515f0f0891495a73204cd4a58c3cbad2a90",
}
SOURCE_ANCHORS = {
    "src/main/background_images.c": (
        "bg_still_decode_to_vram(0x2C0, 0, 0x140, 0xA0)",
        "bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0)",
    ),
    "src/ovl/name_entry/prologue.c": (
        "dec_bg_cd_read(0x494B, 1);",
        "cd_read_sectors(0xDU, 0x80180000, 0xABAB);",
        "D_8012065C = (u8 *)0x801800AC;",
        "D_80120660 = (u8 *)0x801800C0;",
        "actors[96].tpage_table = (u8 *)0x801800A8;",
        "load_tpage_buf_lock(0x1A, *(s32 *)actors[96].tpage_table, 1);",
        "load_palette((u8 *)0x80184490, (i + 16) & 255, 1, 2, i);",
        "color_palette_to_gray(0x80184610, (s32) game_state.scratch_words);",
        "D_80120653 = 0x24;",
        "D_80120655 = 0x80;",
        "D_80120656 = 2;",
        "D_80120653 |= 0x80;",
    ),
    "src/main/actor_sprites.c": (
        "tpy = ((s16 *)anim)[step * 2 + 1];",
        "if ((((ActorFixed *)actor_pool)[i].unk_04 & 8) == 8)",
        "((ActorFixed *)actor_pool)[i].draw_flags & 0x80",
        "uv = part->unk_04 & 0x3F;",
        "spr.unk_0E = (uv & 7) * 0x10;",
        "spr.unk_0F = ((uv >> 3) * 0x10) + tpy;",
        "p[4] = 0;",
        "p[3] = 0;",
        "*(s16 *)(p + 0x16) = 0;",
        "*(s16 *)(p + 0x18) = 0;",
        "if (!(((ActorFixed *)actor_pool)[i].flags & 0x40) && ((((ActorFixed *)actor_pool)[i].flags & 1) == 1))",
        "if ((((ActorFixed *)actor_pool)[i].unk_06 & 0x80) &&",
        "if (game_state.frame_count % ((ActorFixed *)actor_pool)[i].unk_06 == 0)",
        "if (((ActorFixed *)actor_pool)[i].frame >= seq[0])",
    ),
    "src/main/frame_render_audio.c": (
        "y = (base->palette_queue[index].unk_04 & 31) + 0x1E0;",
        "width = 0x100;",
        "case 2:",
        "w = (u16) base->csr_queue[var_s1].unk_0C >> base->csr_queue[var_s1].unk_05;",
        "LoadSquare(x, y, width, height, base->palette_queue[index].source + first);",
    ),
    "include/game/display_records.h": (
        "typedef struct SpritePart {",
        "typedef struct SpriteFrame {",
        "} SpritePart; /* size 0x0E */",
    ),
    "include/game/draw_records.h": (
        "typedef struct CsrLoad {",
        "typedef struct PaletteLoad {",
    ),
    "config/symbol_addrs_main.txt": (
        "actor_pool = 0x8011ECD0;",
        "actor_pool_96 = 0x80120650;",
    ),
}

# The NAME_ENT loader instructions are checked in the actual original overlay,
# in addition to the complete overlay hash checked by image_packs_prologue.
OVERLAY_WORD_CHECKS = (
    (0x131B8, bytes.fromhex("0d000424"), "read_sector_count_13"),
    (0x131D8, bytes.fromhex("1880053c"), "actor_load_address_high"),
    (0x131E0, bytes.fromhex("abab0634"), "actor_read_lba_abab"),
    (0x13254, bytes.fromhex("18800a3c"), "frame_table_high"),
    (0x13258, bytes.fromhex("ac004a35"), "frame_table_low"),
    (0x13264, bytes.fromhex("18800b3c"), "sequence_table_high"),
    (0x13268, bytes.fromhex("c0006b35"), "sequence_table_low"),
    (0x13274, bytes.fromhex("18800c3c"), "tpage_table_high"),
    (0x13278, bytes.fromhex("a8008c35"), "tpage_table_low"),
    (0x132E4, bytes.fromhex("1880043c"), "palette_source_high"),
    (0x132E8, bytes.fromhex("90448434"), "palette_source_low"),
    (0x13318, bytes.fromhex("1880043c"), "gray_palette_source_high"),
    (0x1331C, bytes.fromhex("10468434"), "gray_palette_source_low"),
)


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _source_root(explicit: str | os.PathLike[str] | None = None) -> Path:
    configured = explicit or os.environ.get("TOKIMEMO_SOURCE_ROOT")
    if configured:
        root = Path(configured).resolve(strict=True)
        if not (root / "src/main/background_images.c").is_file():
            raise ArrivalMediaError("source root does not contain the pinned game C files")
        return root
    for parent in Path(__file__).resolve().parents:
        if (parent / "src/main/background_images.c").is_file():
            return parent
    raise ArrivalMediaError("source repository unavailable; set TOKIMEMO_SOURCE_ROOT")


def _verify_source_tree(root: Path) -> dict:
    checked = {}
    for relative, expected in SOURCE_PINS.items():
        path = root / relative
        try:
            path.resolve(strict=True).relative_to(root.resolve(strict=True))
        except (OSError, ValueError) as exc:
            raise ArrivalMediaError("source proof path is missing or escapes the repository") from exc
        if path.is_symlink() or not path.is_file():
            raise ArrivalMediaError("source proof input must be a regular file")
        data = path.read_bytes()
        actual = _sha(data)
        if actual != expected:
            raise ArrivalMediaError("pinned source hash differs: " + relative)
        checked[relative] = actual
        anchors = SOURCE_ANCHORS.get(relative, ())
        if anchors:
            source_text = data.decode("utf-8")
            if any(anchor not in source_text for anchor in anchors):
                raise ArrivalMediaError("required source operation or layout anchor is absent: " + relative)
    return checked


def _entry(entries: list[dict], path: str, lba: int, size: int) -> dict:
    found = [entry for entry in entries if entry.get("kind") == "file"
             and entry.get("path") == path
             and entry.get("location", {}).get("lba") == lba]
    if len(found) != 1 or found[0].get("size") != size:
        raise ArrivalMediaError("original ISO entry/path/LBA/size differs from the stage-0 profile")
    return found[0]


def _overlay_actor_instructions(overlay: bytes) -> list[dict]:
    checks = []
    for offset, expected, meaning in OVERLAY_WORD_CHECKS:
        if overlay[offset:offset + len(expected)] != expected:
            raise ArrivalMediaError("original NAME_ENT actor instruction differs: " + meaning)
        checks.append({"overlay_file_offset": offset, "size": len(expected),
                       "expected_bytes_sha256": _sha(expected), "fact": meaning})
    return checks


def _background_context(source_root: Path, bg_data: bytes, track_sha: str) -> tuple[dict, dict]:
    sources = {}
    for relative in ("src/ovl/name_entry/prologue.c", "src/main/background_images.c"):
        sources[relative] = (source_root / relative).read_text(encoding="utf-8")
    inventory = {
        "schema": "tokimemo.media.v1",
        "input": {"source_sha256": track_sha, "format": "mode2-2352"},
        "assets": [{"id": "stage0-arrival-background", "source": {
            "path": BACKGROUND_PATH, "size": len(bg_data), "sha256": _sha(bg_data),
            "location": {"lba": BACKGROUND_LBA}}}],
    }
    report = media_background_context.build_background_contexts(inventory, sources)
    if not report.get("geometry_body_proved"):
        raise ArrivalMediaError("main background decoder geometry is not fully source-proved")
    layer = report.get("layer_geometry", {}).get("1", {})
    if (layer.get("width"), layer.get("height")) != (BACKGROUND_WIDTH, BACKGROUND_HEIGHT):
        raise ArrivalMediaError("layer-1 source geometry differs from the stage-0 frame")
    streams = [stream for stream in report.get("streams", [])
               if stream.get("source", {}).get("path") == BACKGROUND_PATH
               and stream.get("lba") == BACKGROUND_LBA and stream.get("source_offset") == 0]
    if (len(streams) != 1 or streams[0].get("context_status") not in
            ("proved_single_shape", "proved_single_shape_with_unresolved_uses")):
        raise ArrivalMediaError("stage-0 literal background read has ambiguous geometry")
    uses = streams[0].get("contexts", [])
    if not any(use.get("file") == "src/ovl/name_entry/prologue.c"
               and use.get("function") == "prologue_arrival_scene_init"
               and use.get("lba") == BACKGROUND_LBA and use.get("layer") == 1
               and (use.get("width"), use.get("height")) == (BACKGROUND_WIDTH, BACKGROUND_HEIGHT)
               for use in uses):
        raise ArrivalMediaError("stage-0 background use is not directly linked to its source decoder context")
    use = next(use for use in uses if use.get("file") == "src/ovl/name_entry/prologue.c"
               and use.get("function") == "prologue_arrival_scene_init")
    if use.get("physical_read_bytes") != 10 * 2048:
        raise ArrivalMediaError("source read protocol differs from the profiled ten-sector request")
    context = {"width": BACKGROUND_WIDTH, "height": BACKGROUND_HEIGHT,
               "evidence": [
                   "src/ovl/name_entry/prologue.c:prologue_arrival_scene_init reads LBA 0x494B into layer 1",
                   "src/main/background_images.c:dec_bg_decode_to_vram(1) selects a 320x160 rectangle",
               ]}
    return context, {"layer_geometry": layer, "source_context_status": streams[0]["context_status"],
                     "unresolved_source_uses_present": streams[0]["context_status"] == "proved_single_shape_with_unresolved_uses",
                     "source_call": {key: use[key] for key in (
                         "file", "line", "function", "lba", "layer", "read_sector_count",
                         "physical_read_bytes", "physical_read_exceeds_logical_file",
                         "file_bytes_available", "geometry_binding")},
                     "geometry_evidence": report["geometry_evidence"],
                     "source_hashes": report["source_hashes"],
                     "coverage_complete": False}


def build_pack(input_path: str | os.PathLike[str], *, source_root: str | os.PathLike[str] | None = None) -> tuple[dict, dict[str, bytes]]:
    """Build an exact stage-0 background preview plus opaque actor source bytes."""
    repo = _source_root(source_root)
    source_proof = _verify_source_tree(repo)
    try:
        with open_input(str(input_path)) as source:
            description = source.describe()
            if (description.get("format") != "mode2-2352"
                    or description.get("source_sha256") != TRACK_SHA256):
                raise ArrivalMediaError("input must be the verified original MODE2/2352 data track")
            entries = source.entries()
            # Reuse the established original release executable/overlay proof
            # and source pins before inspecting the new stage-0 records.
            base_context = image_packs_prologue.build_profile_context(source, entries, repo)
            background_entry = _entry(entries, BACKGROUND_PATH, BACKGROUND_LBA, BACKGROUND_SIZE)
            actor_entry = _entry(entries, ACTOR_PATH, ACTOR_LBA, ACTOR_SIZE)
            background = source.read(background_entry)
            actor = source.read(actor_entry)
            if _sha(background) != BACKGROUND_SHA256 or len(background) != BACKGROUND_SIZE:
                raise ArrivalMediaError("original stage-0 BS background differs from its pinned bytes")
            if _sha(actor) != ACTOR_SHA256 or len(actor) != ACTOR_SIZE:
                raise ArrivalMediaError("original stage-0 O_01.NBN differs from its pinned bytes")
            actor_layout, actor_frame_files = prologue_o01.render_o01_frames(actor)
            overlay_entry = next(entry for entry in entries
                                 if entry.get("path") == image_packs_prologue.OVERLAY_PATH)
            overlay = source.read(overlay_entry)
            actor_instruction_checks = _overlay_actor_instructions(overlay)
            context, context_proof = _background_context(repo, background, TRACK_SHA256)

    except ArrivalMediaError:
        raise
    except Exception as exc:
        raise ArrivalMediaError("verified original stage-0 asset extraction failed") from exc

    cache = PROJECT_ROOT / ".asset-cache" / "port"
    _assert_no_symlink_path(cache)
    cache.mkdir(parents=True, exist_ok=True)
    output_dir = Path(tempfile.mkdtemp(prefix=".arrival-mdec-", dir=cache))
    try:
        background_export = image_mdec.export_mdec(
            background, output_dir / "background", "arrival-stage0",
            source_path=BACKGROUND_PATH, context=context,
        )
        if background_export.get("status") != "exported":
            raise ArrivalMediaError("canonical MDEC reader did not export the source-proved background")
        files: dict[str, bytes] = {}
        for path in sorted((output_dir / "background").iterdir()):
            if path.is_file() and not path.is_symlink():
                files["background/" + path.name] = path.read_bytes()
        files["actor/O_01.NBN"] = actor
        for filename, payload in sorted(actor_frame_files.items()):
            files["actor/" + filename] = payload
    finally:
        shutil.rmtree(output_dir, ignore_errors=True)

    actor_offset = lambda address: address - ACTOR_LOAD_ADDRESS
    manifest = {
        "schema": SCHEMA,
        "source": {"format": "mode2-2352", "sha256": TRACK_SHA256,
                   "read_only": True},
        "source_pins": source_proof,
        "release_identity": {
            "executable_path": image_packs_prologue.EXE_PATH,
            "executable_sha256": image_packs_prologue.EXE_SHA256,
            "overlay_path": image_packs_prologue.OVERLAY_PATH,
            "overlay_sha256": image_packs_prologue.OVERLAY_SHA256,
            "overlay_load_base": OVERLAY_LOAD_BASE,
            "base_profile_source_pins": base_context["source_files"],
        },
        "background": {
            "status": "source_context_mdec_preview",
            "source": {"path": BACKGROUND_PATH, "lba": BACKGROUND_LBA,
                       "size": BACKGROUND_SIZE, "sha256": BACKGROUND_SHA256},
            "geometry": [BACKGROUND_WIDTH, BACKGROUND_HEIGHT],
            "geometry_proof": context_proof,
            "export": background_export,
            "hardware_pixel_parity_verified": False,
            "idct_rounding": "independent integer preview; PS1 hardware IDCT/color rounding unverified",
        },
        "actor_96": {
            "status": "source_profile_static_frames_first_visible_frame_proven_scene_composition_unresolved",
            "source": {"path": ACTOR_PATH, "lba": ACTOR_LBA,
                       "size": ACTOR_SIZE, "sha256": ACTOR_SHA256},
            "native_read_sectors": ACTOR_READ_SECTORS,
            "load_address": ACTOR_LOAD_ADDRESS,
            "frame_table_address": ACTOR_FRAME_TABLE_ADDRESS,
            "frame_table_file_offset": actor_offset(ACTOR_FRAME_TABLE_ADDRESS),
            "sequence_table_address": ACTOR_SEQUENCE_TABLE_ADDRESS,
            "sequence_table_file_offset": actor_offset(ACTOR_SEQUENCE_TABLE_ADDRESS),
            "tpage_table_address": ACTOR_TPAGE_TABLE_ADDRESS,
            "tpage_table_file_offset": actor_offset(ACTOR_TPAGE_TABLE_ADDRESS),
            "palette_source_file_offsets": list(ACTOR_PALETTE_OFFSETS),
            "overlay_instruction_checks": actor_instruction_checks,
            "frame_pixels_exported": True,
            "frame_profile": actor_layout,
            "reason": "source-linked static SpriteFrame parts, one 4bpp page and CLUT row are parsed; runtime actor activation/frame selection and scene composition remain unproven",
        },
        "files": {relative: {"size": len(data), "sha256": _sha(data)}
                  for relative, data in sorted(files.items())},
        "capabilities": {
            "stage0_background_preview": True,
            "actor96_source_bytes": True,
            "actor96_frame_pixels": True,
            "complete_arrival_frame": False,
            "runtime_stage_progression": False,
            "hardware_pixel_parity": False,
        },
        "limitations": [
            "the source performs a ten-sector background read while the exact ISO file is nine sectors; adjacent bytes are not fed to the bounded MDEC decoder",
            "actor source frame records/page/CLUT are exported as static sprite previews; runtime actor activation, frame selection, palette brightness and scene composition remain unresolved",
            "this pack does not wire the prologue background/actor read and draw services",
            "this pack does not duplicate the separate T-29021 audio/XA implementation",
        ],
    }
    return manifest, files


def export(input_path: str | os.PathLike[str], output_path: str | os.PathLike[str], *,
           source_root: str | os.PathLike[str] | None = None) -> Path:
    try:
        destination, cache = _check_export_path(output_path, repository_root=PROJECT_ROOT)
    except Exception as exc:
        raise ArrivalMediaError("output must be a new safe directory under candidate .asset-cache/port") from exc
    cache.mkdir(parents=True, exist_ok=True)
    _assert_no_symlink_path(cache)
    manifest, files = build_pack(input_path, source_root=source_root)
    stage = Path(tempfile.mkdtemp(prefix=".prologue-arrival-media-", dir=cache))
    try:
        for relative, data in sorted(files.items()):
            target = stage / relative
            if target.parent not in (stage / "background", stage / "actor"):
                raise ArrivalMediaError("unsafe generated file path")
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(data)
        manifest_path = stage / "manifest.json"
        manifest_path.write_text(json.dumps(manifest, sort_keys=True, indent=2) + "\n", encoding="utf-8")
        _assert_no_symlink_path(cache)
        _check_export_path(destination, repository_root=PROJECT_ROOT)
        os.rename(stage, destination)
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise
    return destination / "manifest.json"


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, help="verified original MODE2/2352 track image")
    parser.add_argument("--output", required=True, help="new ignored directory under .asset-cache/port")
    parser.add_argument("--source-root", help="source checkout containing pinned C/config/assembly files")
    args = parser.parse_args(argv)
    try:
        print(export(args.input, args.output, source_root=args.source_root))
    except (ArrivalMediaError, OSError, ValueError) as exc:
        print("prologue-arrival-media export failed: " + str(exc), file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
