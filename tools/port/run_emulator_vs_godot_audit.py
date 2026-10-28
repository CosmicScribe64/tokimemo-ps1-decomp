#!/usr/bin/env python3
"""
T-29018 Emulator-vs-Godot Visual Parity, Progress Manifest, & CP932 Linter Audit.

Performs three deterministic verification gates inside Docker (tokimemo-decomp:latest):
  1. 16x16 tile-by-tile comparison (`tools/port/verify_tile_diff.py`) between PS1
     emulator hardware reference frames in `.claude/orchestrator/state/` and the
     corresponding Godot 320x240 HUD/scene captures (`hotspot_count`, `total_tiles`,
     `mean_abs_diff`, `max_tile_mae`, `max_channel_diff`, and per-region MAE).
  2. Visual quality audit of all published items in `.asset-cache/port-progress/manifest.json`
     using `validate_png_visual_quality` and `inspect_capture` from
     `tools/port/progress/publish.py`, checking 320x240 resolution, non-blank entropy,
     palette diversity (`rgb444_buckets`), absence of magenta/cyan placeholder blocks,
     MP4 `ftyp`/`moov`/`mdat` atom validation, and zero SHA-256 collisions against
     all PS1 emulator reference captures.
  3. CP932 hex literal linting via `tools/port/extract_decomp_tables.py --lint-godot-hex`
     across all GDScript files in `port/godot/` and `tools/port/`, verifying 0 mismatches.

Writes the full structured JSON audit report to:
  `.claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/emulator-vs-godot-audit.json`
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import sys
from pathlib import Path
from typing import Any

SCRIPT_DIR = Path(__file__).resolve().parent


def _resolve_repo_root() -> Path:
    env_root = os.environ.get("TOKIMEMO_REPO_ROOT", "").strip()
    if env_root:
        cand = Path(env_root).resolve()
        if (cand / ".asset-cache").exists() or (cand / "port" / "godot").exists():
            return cand
    for parent in [SCRIPT_DIR.parent.parent, *SCRIPT_DIR.parents]:
        if (parent / ".asset-cache").exists() and (parent / "port" / "godot").exists():
            return parent
    return SCRIPT_DIR.parent.parent


REPO_ROOT = _resolve_repo_root()
CANONICAL_TOOLS_PORT = REPO_ROOT / "tools" / "port"
for _p in (SCRIPT_DIR, CANONICAL_TOOLS_PORT, CANONICAL_TOOLS_PORT / "progress"):
    if _p.is_dir() and str(_p) not in sys.path:
        sys.path.insert(0, str(_p))

import extract_decomp_tables as edt
import verify_tile_diff as vtd
from progress import publish as pub

Image = vtd.Image

DEFAULT_OUTPUT_JSON = (
    REPO_ROOT
    / ".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/emulator-vs-godot-audit.json"
)
DEFAULT_MANIFEST = REPO_ROOT / ".asset-cache/port-progress/manifest.json"
DEFAULT_MEDIA_DIR = REPO_ROOT / ".asset-cache/port-progress"
DEFAULT_CANDIDATE_ROOT = (
    REPO_ROOT / ".claude/orchestrator/state/T-29022/telephone-live-state-and-audit-provenance-v1"
)
PROVENANCE_SCHEMA = "tokimemo.emulator_vs_godot_capture_provenance.v1"
PRODUCER_SCRIPT_REL = "tests/overlays/run_emulator_vs_godot_capture_producer.gd"

EMULATOR_VS_GODOT_PAIRS: list[dict[str, str]] = [
    {
        "id": "native_command_screen",
        "category": "hud_command",
        "description": "Settled Sunday command selection screen (1995-04-09, 14 animated icons + top bar + date pillar + prompt)",
        "emulator_ref": ".claude/orchestrator/state/T-29013/evidence/next-command-settled/shot_final.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_native_command_screen.png",
    },
    {
        "id": "oracle_command_0",
        "category": "hud_command",
        "description": "PS1 hardware oracle command screen cursor=0 (1995-04-03 Monday, command HUD + room + animated icons)",
        "emulator_ref": ".claude/orchestrator/state/T-29010/dynamic-hud/oracle-command-0/shot_final.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_oracle_command_0.png",
    },
    {
        "id": "oracle_command_7b",
        "category": "hud_command",
        "description": "PS1 hardware oracle command screen cursor=7 rest selected (1995-04-03 Monday, command HUD + room + animated icons)",
        "emulator_ref": ".claude/orchestrator/state/T-29010/dynamic-hud/oracle-command-7b/shot_final.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_oracle_command_7b.png",
    },
    {
        "id": "command0_first_activity",
        "category": "hud_command",
        "description": "Week-day entry frame before school_day_screen_init (1995-04-04 Tuesday transition, command 0 selected)",
        "emulator_ref": ".claude/orchestrator/state/T-29004/evidence/command0-first-activity/shot_final.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_command0_first_activity.png",
    },
    {
        "id": "command0_weekday_activity",
        "category": "weekday_activity",
        "description": "Week-day activity execution frame at week_day_exit0 (1995-04-05 Wednesday, Actor 96 classroom sprite + highlighted stats)",
        "emulator_ref": ".claude/orchestrator/state/T-29004/evidence/command0-first-result/shot_final.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_command0_weekday_activity.png",
    },
    {
        "id": "holiday_menu_settled",
        "category": "holiday_hud",
        "description": "Sunday holiday 3-icon command screen settled (1995-04-09 Sunday, phone/calendar/magazine icons + room + prompt)",
        "emulator_ref": ".claude/orchestrator/state/T-29013/evidence/menu-settled/shot_final.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/emulator-vs-godot-audit-v1/godot_holiday_menu_settled.png",
    },
    {
        "id": "prologue_stage0_arrival",
        "category": "prologue_vn",
        "description": "Prologue Stage 0 school gate arrival background + message window",
        "emulator_ref": ".claude/orchestrator/state/T-29012/prologue-visual-v1/emu_ref/emu_prologue_stage0_arrival.png",
        "godot_capture": ".claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures/godot_prologue_stage0_arrival.png",
    },
    {
        "id": "prologue_stage0_arrival_with_badge",
        "category": "prologue_vn",
        "description": "Prologue Stage 0 school gate arrival with date badge + narration",
        "emulator_ref": ".claude/orchestrator/state/T-29012/prologue-visual-v1/emu_ref/emu_prologue_stage0_arrival_with_badge.png",
        "godot_capture": ".claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures/godot_prologue_stage0_arrival_with_badge.png",
    },
    {
        "id": "prologue_stage1_board",
        "category": "prologue_vn",
        "description": "Prologue Stage 1 class assignment board scene + dialogue",
        "emulator_ref": ".claude/orchestrator/state/T-29012/prologue-visual-v1/emu_ref/emu_prologue_stage1_board.png",
        "godot_capture": ".claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures/godot_prologue_stage1_board.png",
    },
    {
        "id": "prologue_stage13_shiori_line18",
        "category": "prologue_vn",
        "description": "Prologue Stage 13 Shiori Fujisaki portrait + dialogue line 18",
        "emulator_ref": ".claude/orchestrator/state/T-29012/prologue-visual-v1/emu_ref/emu_prologue_stage13_shiori_line18.png",
        "godot_capture": ".claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures/godot_prologue_stage13_shiori_line18.png",
    },
    {
        "id": "prologue_stage14_shiori_line19",
        "category": "prologue_vn",
        "description": "Prologue Stage 14 Shiori Fujisaki portrait + dialogue line 19",
        "emulator_ref": ".claude/orchestrator/state/T-29012/prologue-visual-v1/emu_ref/emu_prologue_stage14_shiori_line19.png",
        "godot_capture": ".claude/orchestrator/state/T-29012/prologue-visual-v1/godot_captures/godot_prologue_stage14_shiori_line19.png",
    },
    {
        "id": "calendar_april_may",
        "category": "calendar_phone",
        "description": "Two-month spring calendar screen (April/May 1995)",
        "emulator_ref": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/emu-reference/emu_calendar.png",
        "godot_capture": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/captures/T-29013-calendar-screen-april-may.png",
    },
    {
        "id": "holiday_phone_book",
        "category": "calendar_phone",
        "description": "Sunday holiday telephone directory notebook overlay",
        "emulator_ref": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/emu-reference/emu_phone_book.png",
        "godot_capture": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/captures/T-29013-holiday-phone-book.png",
    },
    {
        "id": "holiday_phone_shiori",
        "category": "calendar_phone",
        "description": "Sunday holiday telephone dial confirmation for Shiori Fujisaki",
        "emulator_ref": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/emu-reference/emu_phone_shiori.png",
        "godot_capture": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/captures/T-29013-holiday-phone-shiori.png",
    },
    {
        "id": "holiday_magazine",
        "category": "calendar_phone",
        "description": "Sunday holiday date-info magazine viewer",
        "emulator_ref": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/emu-reference/emu_magazine.png",
        "godot_capture": ".claude/orchestrator/state/T-29013/calendar-phone-screens-v1/captures/T-29013-holiday-magazine.png",
    },
    {
        "id": "system_options_menu",
        "category": "system_overlay",
        "description": "System options configuration menu overlay",
        "emulator_ref": ".claude/orchestrator/state/T-29018/emu_ref/emu_system_settings.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/shared-vn-and-system-overlays-v1/captures/capture_options_menu.png",
    },
    {
        "id": "system_help_screen",
        "category": "system_overlay",
        "description": "System mini-knowledge / help screen overlay",
        "emulator_ref": ".claude/orchestrator/state/T-29018/emu_ref/emu_help_screen.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/shared-vn-and-system-overlays-v1/captures/capture_help_screen.png",
    },
    {
        "id": "tel_notebook_profile",
        "category": "telephone_overlay",
        "description": "Telephone overlay notebook profile page",
        "emulator_ref": ".claude/orchestrator/state/T-29018/emu_ref/emu_tel_notebook_profile.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/tel-overlay-v1/captures/tel_01_notebook_profile.png",
    },
    {
        "id": "tel_date_place_menu",
        "category": "telephone_overlay",
        "description": "Telephone overlay date destination selection menu",
        "emulator_ref": ".claude/orchestrator/state/T-29018/emu_ref/emu_tel_date_place_menu.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/tel-overlay-v1/captures/tel_02_date_place_menu.png",
    },
    {
        "id": "tel_date_day_menu",
        "category": "telephone_overlay",
        "description": "Telephone overlay date calendar day selection menu",
        "emulator_ref": ".claude/orchestrator/state/T-29018/emu_ref/emu_tel_date_day_menu.png",
        "godot_capture": ".claude/orchestrator/state/T-29018/tel-overlay-v1/captures/tel_03_date_day_menu.png",
    },
]


def sha256_file(path: Path) -> str:
    h = hashlib.sha256()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def compute_channel_diff_metrics(emu_path: Path, godot_path: Path) -> dict[str, int | float]:
    """Compute max single-channel absolute difference and pixel diff stats."""
    im1 = Image.open(emu_path).convert("RGB")
    im2 = Image.open(godot_path).convert("RGB")
    w, h = im1.size
    p1 = im1.load()
    p2 = im2.load()

    max_channel_diff = 0
    raw_border_max_channel_diff = 0
    exact_pixel_matches = 0
    within_dither_8_pixels = 0
    active_pixels = (w - 1) * (h - 1)

    for y in range(h):
        for x in range(w):
            r1, g1, b1 = p1[x, y]
            r2, g2, b2 = p2[x, y]
            cd = max(abs(r1 - r2), abs(g1 - g2), abs(b1 - b2))
            if cd > raw_border_max_channel_diff:
                raw_border_max_channel_diff = cd
            if x >= 1 and y >= 1:
                if cd > max_channel_diff:
                    max_channel_diff = cd
                if cd == 0:
                    exact_pixel_matches += 1
                if cd <= 8:
                    within_dither_8_pixels += 1

    return {
        "max_channel_diff": max_channel_diff,
        "raw_border_max_channel_diff": raw_border_max_channel_diff,
        "exact_pixel_matches": exact_pixel_matches,
        "within_dither_8_pixels": within_dither_8_pixels,
        "within_dither_8_ratio": round(within_dither_8_pixels / max(1, active_pixels), 6),
    }


def _resolve_godot_capture_path(
    repo_root: Path, entry: dict[str, str], captures_dir: Path | None = None
) -> tuple[Path, str]:
    default_rel = entry["godot_capture"]
    file_name = Path(default_rel).name
    if captures_dir is not None:
        cand = captures_dir / file_name
        if cand.is_file():
            try:
                rel = str(cand.resolve().relative_to(repo_root.resolve()))
            except ValueError:
                rel = str(cand)
            return cand, rel
    return repo_root / default_rel, default_rel


def run_step1_tile_diff_audit(
    repo_root: Path,
    threshold: float = 20.0,
    captures_dir: Path | None = None,
    provenance_map: dict[str, Any] | None = None,
) -> dict[str, Any]:
    """Step 1: Run verify_tile_diff.py across all PS1 emulator vs Godot 320x240 pairs."""
    pair_results: list[dict[str, Any]] = []
    total_hotspots = 0
    total_tiles_checked = 0
    mae_values: list[float] = []
    max_tile_mae_overall = 0.0
    max_channel_diff_overall = 0

    for entry in EMULATOR_VS_GODOT_PAIRS:
        emu_path = repo_root / entry["emulator_ref"]
        godot_path, godot_resolved_rel = _resolve_godot_capture_path(
            repo_root, entry, captures_dir=captures_dir
        )
        if not emu_path.is_file():
            raise FileNotFoundError(f"Missing emulator reference: {emu_path}")
        if not godot_path.is_file():
            raise FileNotFoundError(f"Missing Godot capture: {godot_path}")

        tile_report = vtd.compare_16x16_tiles(emu_path, godot_path, hotspot_threshold=threshold)
        ch_metrics = compute_channel_diff_metrics(emu_path, godot_path)

        hotspot_count = int(tile_report["hotspot_count"])
        tile_count = int(tile_report["tile_count"])
        mean_abs_diff = round(float(tile_report["overall_mae"]), 4)
        max_tile_mae = round(float(tile_report["max_tile_mae"]), 4)
        max_ch_diff = int(ch_metrics["max_channel_diff"])

        total_hotspots += hotspot_count
        total_tiles_checked += tile_count
        mae_values.append(float(tile_report["overall_mae"]))
        if max_tile_mae > max_tile_mae_overall:
            max_tile_mae_overall = max_tile_mae
        if max_ch_diff > max_channel_diff_overall:
            max_channel_diff_overall = max_ch_diff

        regions_clean: dict[str, Any] = {}
        for rname, rdata in tile_report["regions"].items():
            regions_clean[rname] = {
                "rect": rdata["rect"],
                "mae": round(float(rdata["mae"]), 4),
                "max_px_diff": round(float(rdata["max_px_diff"]), 4),
                "px_over_10": int(rdata["px_over_10"]),
            }

        pair_record: dict[str, Any] = {
            "id": entry["id"],
            "category": entry["category"],
            "description": entry["description"],
            "emulator_ref": entry["emulator_ref"],
            "emulator_sha256": sha256_file(emu_path),
            "godot_capture": entry["godot_capture"],
            "godot_resolved_capture": godot_resolved_rel,
            "godot_sha256": sha256_file(godot_path),
            "width": vtd.SCREEN_W,
            "height": vtd.SCREEN_H,
            "total_tiles": tile_count,
            "hotspot_count": hotspot_count,
            "hotspot_threshold": threshold,
            "mean_abs_diff": mean_abs_diff,
            "max_tile_mae": max_tile_mae,
            "max_channel_diff": max_ch_diff,
            "raw_border_max_channel_diff": ch_metrics["raw_border_max_channel_diff"],
            "within_dither_8_ratio": ch_metrics["within_dither_8_ratio"],
            "regions": regions_clean,
            "passed": hotspot_count == 0,
        }
        if provenance_map and entry["id"] in provenance_map:
            prov = provenance_map[entry["id"]]
            pair_record["provenance"] = {
                "producer_script": prov.get("producer_script"),
                "suite_script": prov.get("suite_script"),
                "presenter_script": prov.get("presenter_script"),
                "presenter_script_sha256": prov.get("presenter_script_sha256"),
                "produced_file": prov.get("produced_file"),
                "receipt_sha256": prov.get("sha256"),
                "live_state": prov.get("live_state"),
            }
        pair_results.append(pair_record)

    mean_mae_across_pairs = round(sum(mae_values) / max(1, len(mae_values)), 4)
    max_mae_across_pairs = round(max(mae_values) if mae_values else 0.0, 4)
    min_mae_across_pairs = round(min(mae_values) if mae_values else 0.0, 4)

    return {
        "passed": total_hotspots == 0 and len(pair_results) == len(EMULATOR_VS_GODOT_PAIRS),
        "pairs_evaluated": len(pair_results),
        "total_tiles_evaluated": total_tiles_checked,
        "tiles_per_frame": 300,
        "hotspot_threshold_mae": threshold,
        "total_hotspot_count": total_hotspots,
        "mean_abs_diff_across_pairs": mean_mae_across_pairs,
        "min_abs_diff_across_pairs": min_mae_across_pairs,
        "max_abs_diff_across_pairs": max_mae_across_pairs,
        "max_tile_mae_across_pairs": max_tile_mae_overall,
        "max_channel_diff_across_pairs": max_channel_diff_overall,
        "pairs": pair_results,
    }


_EMU_SHA_CACHE: dict[Path, dict[str, str]] = {}


def collect_emulator_reference_shas(repo_root: Path) -> dict[str, str]:
    """Collect SHA-256 digests of all PS1 emulator reference PNGs in the workspace."""
    resolved_root = repo_root.resolve()
    if resolved_root in _EMU_SHA_CACHE:
        return _EMU_SHA_CACHE[resolved_root]

    emu_shas: dict[str, str] = {}
    for entry in EMULATOR_VS_GODOT_PAIRS:
        ep = repo_root / entry["emulator_ref"]
        if ep.is_file():
            emu_shas[sha256_file(ep)] = entry["emulator_ref"]

    emu_dirs = [
        repo_root / ".claude/orchestrator/state/T-29018/emu_ref",
        repo_root / ".claude/orchestrator/state/T-29012/prologue-visual-v1/emu_reference",
        repo_root / ".claude/orchestrator/state/T-29013/evidence",
        repo_root / ".claude/orchestrator/state/T-29010/dynamic-hud",
        repo_root / ".claude/orchestrator/state/T-29004/evidence",
        repo_root / "build/emu",
    ]
    for d in emu_dirs:
        if not d.is_dir():
            continue
        for png in sorted(d.rglob("*.png")):
            rel = str(png.relative_to(repo_root))
            name_lower = png.name.lower()
            parts_lower = [p.lower() for p in png.parts]
            if (
                d.name == "emu"
                or name_lower == "shot_final.png"
                or name_lower.startswith("emu_")
                or "emu_ref" in parts_lower
                or "emu-reference" in parts_lower
                or "oracle-command" in rel.lower()
            ):
                emu_shas[sha256_file(png)] = rel

    _EMU_SHA_CACHE[resolved_root] = emu_shas
    return emu_shas


def inspect_png_visual_metrics(path: Path) -> dict[str, Any]:
    """Compute detailed visual quality metrics matching publish.validate_png_visual_quality."""
    im = Image.open(path).convert("RGB")
    w, h = im.size
    pixels = im.load()
    total = w * h

    black_px = 0
    top_black_px = 0
    bottom_black_px = 0
    fallback_bg_px = 0
    magenta_cyan_placeholder_px = 0
    color_buckets: set[tuple[int, int, int]] = set()

    top_h = max(1, h // 4)
    bot_y = h - max(1, h // 4)

    for y in range(h):
        for x in range(w):
            r, g, b = pixels[x, y]
            if r <= 4 and g <= 4 and b <= 4:
                black_px += 1
                if y < top_h:
                    top_black_px += 1
                if y >= bot_y:
                    bottom_black_px += 1
            if abs(r - 20) <= 3 and abs(g - 18) <= 3 and abs(b - 30) <= 3:
                fallback_bg_px += 1
            if (r >= 240 and g <= 15 and b >= 240) or (r <= 15 and g >= 240 and b >= 240):
                magenta_cyan_placeholder_px += 1
            color_buckets.add((r >> 4, g >> 4, b >> 4))

    return {
        "width": w,
        "height": h,
        "black_ratio": round(black_px / total, 4),
        "top_black_ratio": round(top_black_px / (w * top_h), 4),
        "bottom_black_ratio": round(bottom_black_px / (w * (h - bot_y)), 4),
        "fallback_bg_ratio": round(fallback_bg_px / total, 4),
        "rgb444_buckets": len(color_buckets),
        "magenta_cyan_placeholder_pixels": magenta_cyan_placeholder_px,
    }


def inspect_mp4_atoms(path: Path) -> dict[str, Any]:
    """Verify MP4 top-level atoms (`ftyp`, `moov`, `mdat`) and byte sizes."""
    data = path.read_bytes()
    size = len(data)
    atoms: list[dict[str, Any]] = []
    offset = 0
    while offset + 8 <= size:
        box_size = int.from_bytes(data[offset : offset + 4], "big")
        box_type = data[offset + 4 : offset + 8].decode("latin1", errors="replace")
        if box_size == 1 and offset + 16 <= size:
            box_size = int.from_bytes(data[offset + 8 : offset + 16], "big")
        elif box_size == 0:
            box_size = size - offset
        if box_size < 8 or offset + box_size > size:
            break
        atoms.append({"type": box_type, "offset": offset, "size": box_size})
        offset += box_size

    atom_types = [a["type"] for a in atoms]
    mdat_bytes = sum(a["size"] for a in atoms if a["type"] == "mdat")
    return {
        "file_size_bytes": size,
        "atoms": atoms,
        "has_ftyp": "ftyp" in atom_types,
        "has_moov": "moov" in atom_types,
        "has_mdat": "mdat" in atom_types,
        "mdat_bytes": mdat_bytes,
    }


def run_step2_manifest_audit(
    repo_root: Path, manifest_path: Path, media_dir: Path
) -> dict[str, Any]:
    """Step 2: Audit all published items in `.asset-cache/port-progress/manifest.json`."""
    if not manifest_path.is_file():
        raise FileNotFoundError(f"Missing progress manifest: {manifest_path}")

    manifest_data = json.loads(manifest_path.read_text(encoding="utf-8"))
    items = manifest_data.get("items", [])
    emu_shas = collect_emulator_reference_shas(repo_root)

    audited_items: list[dict[str, Any]] = []
    png_count = 0
    mp4_count = 0
    passed_count = 0
    failures: list[dict[str, Any]] = []
    sha_collisions: list[dict[str, str]] = []

    black_ratios: list[float] = []
    bucket_counts: list[int] = []
    total_placeholder_pixels = 0

    for item in items:
        filename = str(item.get("file") or item.get("filename"))
        media_path = media_dir / filename
        ext = media_path.suffix.lower()
        kind = "image" if ext == ".png" else ("video" if ext in (".mp4", ".webm") else "unknown")
        item_errors: list[str] = []

        if not media_path.is_file():
            item_errors.append(f"Missing media file: {media_path}")
            failures.append({"id": item.get("id", filename), "errors": item_errors})
            continue

        # Run canonical inspect_capture (which calls validate_png_visual_quality for PNGs)
        try:
            suffix, inspected_size, file_sha = pub.inspect_capture(media_path)
            if inspected_size != int(item.get("bytes", inspected_size)):
                item_errors.append(
                    f"Byte size mismatch: manifest={item.get('bytes')}, actual={inspected_size}"
                )
            if file_sha != str(item.get("sha256", file_sha)):
                item_errors.append(
                    f"SHA-256 mismatch: manifest={item.get('sha256')}, actual={file_sha}"
                )
        except Exception as exc:
            item_errors.append(str(exc))
            file_sha = sha256_file(media_path)
            inspected_size = media_path.stat().st_size

        if file_sha in emu_shas:
            collision = {
                "manifest_item": filename,
                "sha256": file_sha,
                "emulator_file": emu_shas[file_sha],
            }
            sha_collisions.append(collision)
            item_errors.append(f"SHA-256 collision with emulator reference {emu_shas[file_sha]}")

        record: dict[str, Any] = {
            "id": item.get("id", filename),
            "file": filename,
            "kind": kind,
            "sha256": file_sha,
            "size_bytes": inspected_size,
            "created": item.get("created", ""),
        }

        if kind == "image":
            png_count += 1
            metrics = inspect_png_visual_metrics(media_path)
            record["width"] = metrics["width"]
            record["height"] = metrics["height"]
            record["visual_metrics"] = metrics
            black_ratios.append(float(metrics["black_ratio"]))
            bucket_counts.append(int(metrics["rgb444_buckets"]))
            total_placeholder_pixels += int(metrics["magenta_cyan_placeholder_pixels"])

            w_px, h_px = int(metrics["width"]), int(metrics["height"])
            if w_px < 320 or h_px < 240 or (w_px * 3 != h_px * 4):
                item_errors.append(
                    f"Invalid 4:3 resolution: {w_px}x{h_px} (expected >= 320x240 4:3)"
                )
            if metrics["magenta_cyan_placeholder_pixels"] > 0:
                item_errors.append(
                    f"Detected {metrics['magenta_cyan_placeholder_pixels']} magenta/cyan placeholder pixels"
                )
        elif kind == "video":
            mp4_count += 1
            mp4_info = inspect_mp4_atoms(media_path)
            record["mp4_metrics"] = mp4_info
            if not (mp4_info["has_ftyp"] and mp4_info["has_moov"] and mp4_info["has_mdat"]):
                item_errors.append(f"MP4 missing required atoms: {mp4_info['atoms']}")
            if mp4_info["mdat_bytes"] < 32768:
                item_errors.append(f"MP4 mdat payload too small: {mp4_info['mdat_bytes']} bytes")

        record["passed"] = len(item_errors) == 0
        if item_errors:
            record["errors"] = item_errors
            failures.append({"id": record["id"], "file": filename, "errors": item_errors})
        else:
            passed_count += 1

        audited_items.append(record)

    return {
        "passed": len(failures) == 0 and len(sha_collisions) == 0 and len(audited_items) == len(items),
        "manifest_path": str(manifest_path.relative_to(repo_root)),
        "total_items": len(items),
        "png_items": png_count,
        "mp4_items": mp4_count,
        "passed_items": passed_count,
        "failed_items": len(failures),
        "emulator_reference_pngs_indexed": len(emu_shas),
        "emulator_oracle_sha_collisions": len(sha_collisions),
        "total_magenta_cyan_placeholder_pixels": total_placeholder_pixels,
        "png_summary": {
            "all_valid_4_3": all(
                int(i.get("visual_metrics", {}).get("width", 0)) >= 320
                and int(i.get("visual_metrics", {}).get("height", 0)) >= 240
                and int(i.get("visual_metrics", {}).get("width", 0)) * 3
                == int(i.get("visual_metrics", {}).get("height", 0)) * 4
                for i in audited_items
                if i["kind"] == "image"
            ),
            "max_black_ratio": round(max(black_ratios) if black_ratios else 0.0, 4),
            "mean_black_ratio": round(sum(black_ratios) / max(1, len(black_ratios)), 4),
            "min_rgb444_buckets": min(bucket_counts) if bucket_counts else 0,
            "max_rgb444_buckets": max(bucket_counts) if bucket_counts else 0,
            "mean_rgb444_buckets": round(sum(bucket_counts) / max(1, len(bucket_counts)), 2),
        },
        "failures": failures,
        "sha_collisions": sha_collisions,
        "items": audited_items,
    }


def run_step3_cp932_hex_lint(repo_root: Path, project_root: Path | None = None) -> dict[str, Any]:
    """Step 3: Run extract_decomp_tables.py --lint-godot-hex across GDScript files."""
    canonical_godot = (repo_root / "port" / "godot").resolve()
    scan_dirs: list[Path] = [repo_root / "port" / "godot", repo_root / "tools" / "port"]
    if project_root is not None and project_root.is_dir() and project_root.resolve() != canonical_godot:
        scan_dirs.append(project_root)
    gd_files_count = sum(len(list(d.rglob("*.gd"))) for d in scan_dirs if d.is_dir())
    mismatch_count, entries = edt.lint_godot_hex(
        target_dirs=scan_dirs, allow_known=False, root=repo_root
    )
    ok_count = sum(1 for e in entries if e["status"] == "ok")
    mismatch_entries = [e for e in entries if e["status"] in ("mismatch", "decode_error")]

    rel_dirs: list[str] = []
    for d in scan_dirs:
        try:
            rel_dirs.append(str(d.resolve().relative_to(repo_root.resolve())))
        except ValueError:
            rel_dirs.append(str(d))

    return {
        "passed": int(mismatch_count) == 0 and len(entries) > 0,
        "scanned_directories": rel_dirs,
        "gdscript_files_scanned": gd_files_count,
        "total_checked_pairs": len(entries),
        "verified_matches": ok_count,
        "mismatch_count": int(mismatch_count),
        "mismatches": mismatch_entries,
    }


def _resolve_script_path(repo_root: Path, project_root: Path, rel_script: str) -> Path | None:
    for root in (project_root, repo_root / "port" / "godot"):
        cand = root / rel_script
        if cand.is_file():
            return cand
    return None


def run_step4_capture_provenance_audit(
    repo_root: Path,
    receipt_path: Path,
    project_root: Path,
    captures_dir: Path | None = None,
) -> dict[str, Any]:
    """Step 4: Verify that every EMULATOR_VS_GODOT_PAIRS capture is bound to a live producer script,
    presenter script SHA-256, live state descriptor, and matching PNG SHA-256."""
    if not receipt_path.is_file():
        return {
            "passed": False,
            "error": f"Missing provenance receipt: {receipt_path}",
            "receipt_path": str(receipt_path),
            "verified_pairs": 0,
            "expected_pairs": len(EMULATOR_VS_GODOT_PAIRS),
            "failures": [{"reason": f"Missing provenance receipt: {receipt_path}"}],
        }

    receipt = json.loads(receipt_path.read_text(encoding="utf-8"))
    failures: list[dict[str, Any]] = []
    if receipt.get("schema") != PROVENANCE_SCHEMA:
        failures.append(
            {
                "id": "__schema__",
                "reason": f"Invalid provenance schema: {receipt.get('schema')} (expected {PROVENANCE_SCHEMA})",
            }
        )

    producer_rel = str(receipt.get("producer_script", ""))
    producer_path = _resolve_script_path(repo_root, project_root, producer_rel)
    producer_sha_actual = sha256_file(producer_path) if producer_path is not None else ""
    if producer_rel != PRODUCER_SCRIPT_REL or producer_path is None:
        failures.append(
            {
                "id": "__producer_script__",
                "reason": f"Producer script missing or invalid: {producer_rel}",
            }
        )
    elif receipt.get("producer_script_sha256") != producer_sha_actual:
        failures.append(
            {
                "id": "__producer_script_sha256__",
                "reason": (
                    f"Producer script SHA-256 mismatch for {producer_rel}: "
                    f"receipt={receipt.get('producer_script_sha256')} actual={producer_sha_actual}"
                ),
            }
        )

    source_scripts: dict[str, str] = receipt.get("source_scripts", {})
    verified_source_scripts = 0
    for s_rel, expected_sha in sorted(source_scripts.items()):
        s_path = _resolve_script_path(repo_root, project_root, s_rel)
        if s_path is None:
            failures.append({"id": f"source:{s_rel}", "reason": f"Missing source script {s_rel}"})
            continue
        actual_sha = sha256_file(s_path)
        if actual_sha != expected_sha:
            failures.append(
                {
                    "id": f"source:{s_rel}",
                    "reason": f"Source script SHA-256 mismatch for {s_rel}: receipt={expected_sha} actual={actual_sha}",
                }
            )
        else:
            verified_source_scripts += 1

    emu_shas = collect_emulator_reference_shas(repo_root)
    captures_map: dict[str, Any] = receipt.get("captures", {})
    pair_verifications: list[dict[str, Any]] = []
    verified_pairs = 0

    for entry in EMULATOR_VS_GODOT_PAIRS:
        pair_id = entry["id"]
        rec = captures_map.get(pair_id)
        if not isinstance(rec, dict):
            failures.append({"id": pair_id, "reason": "Missing capture provenance record in receipt"})
            continue

        godot_path, godot_resolved_rel = _resolve_godot_capture_path(
            repo_root, entry, captures_dir=captures_dir
        )
        if not godot_path.is_file():
            failures.append({"id": pair_id, "reason": f"Missing Godot capture on disk: {godot_path}"})
            continue

        disk_sha = sha256_file(godot_path)
        receipt_sha = str(rec.get("sha256", ""))
        pres_rel = str(rec.get("presenter_script", ""))
        suite_rel = str(rec.get("suite_script", ""))
        rec_prod_rel = str(rec.get("producer_script", ""))
        pres_path = _resolve_script_path(repo_root, project_root, pres_rel)
        suite_path = _resolve_script_path(repo_root, project_root, suite_rel)
        pres_sha_actual = sha256_file(pres_path) if pres_path is not None else ""
        live_state = rec.get("live_state")

        pair_errors: list[str] = []
        if disk_sha != receipt_sha:
            pair_errors.append(f"PNG SHA-256 mismatch: disk={disk_sha} receipt={receipt_sha}")
        if disk_sha in emu_shas:
            pair_errors.append(f"PNG SHA-256 collides with emulator reference {emu_shas[disk_sha]}")
        if rec_prod_rel != PRODUCER_SCRIPT_REL:
            pair_errors.append(f"Invalid producer_script: {rec_prod_rel}")
        if pres_path is None:
            pair_errors.append(f"Missing presenter_script: {pres_rel}")
        elif rec.get("presenter_script_sha256") != pres_sha_actual:
            pair_errors.append(
                f"Presenter script SHA-256 mismatch for {pres_rel}: "
                f"receipt={rec.get('presenter_script_sha256')} actual={pres_sha_actual}"
            )
        if suite_path is None:
            pair_errors.append(f"Missing suite_script: {suite_rel}")
        if not isinstance(live_state, dict) or "game_mode" not in live_state:
            pair_errors.append("Missing or incomplete live_state projection")
        elif not (("year" in live_state and "month" in live_state and "day" in live_state) or ("line" in live_state and "speaker" in live_state)):
            pair_errors.append("live_state lacks date or dialogue line/speaker binding")

        pair_passed = len(pair_errors) == 0
        if pair_passed:
            verified_pairs += 1
        else:
            for err_msg in pair_errors:
                failures.append({"id": pair_id, "reason": err_msg})

        pair_verifications.append(
            {
                "id": pair_id,
                "category": entry["category"],
                "godot_resolved_capture": godot_resolved_rel,
                "sha256": disk_sha,
                "receipt_sha256": receipt_sha,
                "producer_script": rec_prod_rel,
                "suite_script": suite_rel,
                "presenter_script": pres_rel,
                "presenter_script_sha256": pres_sha_actual,
                "live_state": live_state,
                "passed": pair_passed,
                "errors": pair_errors,
            }
        )

    try:
        receipt_rel = str(receipt_path.resolve().relative_to(repo_root.resolve()))
    except ValueError:
        receipt_rel = str(receipt_path)

    return {
        "passed": len(failures) == 0 and verified_pairs == len(EMULATOR_VS_GODOT_PAIRS),
        "schema": receipt.get("schema"),
        "receipt_path": receipt_rel,
        "producer_script": producer_rel,
        "producer_script_sha256": producer_sha_actual,
        "verified_source_scripts": verified_source_scripts,
        "total_source_scripts": len(source_scripts),
        "verified_pairs": verified_pairs,
        "expected_pairs": len(EMULATOR_VS_GODOT_PAIRS),
        "failures": failures,
        "pairs": pair_verifications,
    }


def _resolve_default_captures_dir(repo_root: Path) -> Path | None:
    env_dir = os.environ.get("TOKIMEMO_AUDIT_CAPTURE_DIR", "").strip()
    if env_dir:
        p = Path(env_dir)
        if p.is_dir():
            return p
    cand = DEFAULT_CANDIDATE_ROOT / "captures"
    if cand.is_dir():
        return cand
    return None


def _resolve_default_project_root(repo_root: Path) -> Path:
    env_proj = os.environ.get("TOKIMEMO_AUDIT_PROJECT_ROOT", "").strip()
    if env_proj:
        p = Path(env_proj)
        if p.is_dir():
            return p
    canon = repo_root / "port" / "godot"
    if (canon / PRODUCER_SCRIPT_REL).is_file():
        return canon
    cand = DEFAULT_CANDIDATE_ROOT / "project"
    if (cand / PRODUCER_SCRIPT_REL).is_file():
        return cand
    return canon


def _resolve_default_provenance_receipt(repo_root: Path, captures_dir: Path | None) -> Path:
    env_rec = os.environ.get("TOKIMEMO_PROVENANCE_RECEIPT", "").strip()
    if env_rec:
        return Path(env_rec)
    if captures_dir is not None and (captures_dir / "producer-provenance.json").is_file():
        return captures_dir / "producer-provenance.json"
    return DEFAULT_CANDIDATE_ROOT / "captures" / "producer-provenance.json"


def main() -> int:
    default_captures_dir = _resolve_default_captures_dir(REPO_ROOT)
    default_project_root = _resolve_default_project_root(REPO_ROOT)
    default_receipt = _resolve_default_provenance_receipt(REPO_ROOT, default_captures_dir)

    parser = argparse.ArgumentParser(
        description="Run T-29018 / T-29022 Emulator-vs-Godot Visual Parity, Manifest Quality, CP932 Linter, & Capture Provenance Audit."
    )
    parser.add_argument(
        "--output-json",
        type=Path,
        default=DEFAULT_OUTPUT_JSON,
        help="Path to write structured JSON audit report",
    )
    parser.add_argument(
        "--hotspot-threshold",
        type=float,
        default=20.0,
        help="16x16 tile MAE threshold for hotspot detection (default: 20.0)",
    )
    parser.add_argument(
        "--captures-dir",
        type=Path,
        default=default_captures_dir,
        help="Directory containing freshly produced Godot captures from run_emulator_vs_godot_capture_producer.gd",
    )
    parser.add_argument(
        "--provenance-receipt",
        type=Path,
        default=default_receipt,
        help="Path to producer-provenance.json binding each capture to its producer script, presenter SHA-256, and live state",
    )
    parser.add_argument(
        "--project-root",
        type=Path,
        default=default_project_root,
        help="Godot project root containing producer and presenter GDScript files",
    )
    args = parser.parse_args()

    provenance_map: dict[str, Any] = {}
    if args.provenance_receipt and args.provenance_receipt.is_file():
        try:
            provenance_map = json.loads(args.provenance_receipt.read_text(encoding="utf-8")).get("captures", {})
        except Exception:
            provenance_map = {}

    print("==> [Step 1/4] Running 16x16 Tile Diff Audit (PS1 Emulator vs Godot 320x240)...")
    step1 = run_step1_tile_diff_audit(
        REPO_ROOT,
        threshold=args.hotspot_threshold,
        captures_dir=args.captures_dir,
        provenance_map=provenance_map,
    )
    for pair in step1["pairs"]:
        status = "PASS" if pair["passed"] else "FAIL"
        print(
            f"  [{status}] {pair['id']:34s} | hotspots={pair['hotspot_count']:2d}/{pair['total_tiles']} "
            f"| mean_abs_diff={pair['mean_abs_diff']:6.4f} | max_tile_mae={pair['max_tile_mae']:6.2f} "
            f"| max_ch_diff={pair['max_channel_diff']:3d}"
        )
    print(
        f"  Summary: {step1['pairs_evaluated']} pairs ({step1['total_tiles_evaluated']} tiles), "
        f"total_hotspots={step1['total_hotspot_count']}, "
        f"mean_abs_diff={step1['mean_abs_diff_across_pairs']:.4f}, "
        f"max_channel_diff={step1['max_channel_diff_across_pairs']}"
    )

    print("\n==> [Step 2/4] Auditing Published Progress Manifest (.asset-cache/port-progress/manifest.json)...")
    step2 = run_step2_manifest_audit(REPO_ROOT, DEFAULT_MANIFEST, DEFAULT_MEDIA_DIR)
    print(
        f"  Summary: {step2['passed_items']}/{step2['total_items']} items passed "
        f"({step2['png_items']} PNGs, {step2['mp4_items']} MP4), "
        f"placeholder_pixels={step2['total_magenta_cyan_placeholder_pixels']}, "
        f"emulator_sha_collisions={step2['emulator_oracle_sha_collisions']}"
    )

    print("\n==> [Step 3/4] Running CP932 Hex Literal Linter (extract_decomp_tables.py --lint-godot-hex)...")
    step3 = run_step3_cp932_hex_lint(REPO_ROOT, project_root=args.project_root)
    print(
        f"  Summary: {step3['verified_matches']}/{step3['total_checked_pairs']} verified matches "
        f"across {step3['gdscript_files_scanned']} GDScript files, "
        f"mismatches={step3['mismatch_count']}"
    )

    print("\n==> [Step 4/4] Verifying Capture Producer Provenance & Live-State Binding...")
    step4 = run_step4_capture_provenance_audit(
        REPO_ROOT,
        receipt_path=args.provenance_receipt,
        project_root=args.project_root,
        captures_dir=args.captures_dir,
    )
    for p in step4.get("pairs", []):
        st = "PASS" if p["passed"] else "FAIL"
        print(
            f"  [{st}] {p['id']:34s} | presenter={p['presenter_script']:38s} | sha256={p['sha256'][:16]}..."
        )
    print(
        f"  Summary: {step4['verified_pairs']}/{step4['expected_pairs']} pairs provenance-verified, "
        f"source_scripts={step4.get('verified_source_scripts', 0)}/{step4.get('total_source_scripts', 0)}, "
        f"failures={len(step4['failures'])}"
    )

    overall_passed = bool(step1["passed"] and step2["passed"] and step3["passed"] and step4["passed"])
    report = {
        "schema": "tokimemo.emulator_vs_godot_audit.v1",
        "ticket": "T-29022",
        "overall_passed": overall_passed,
        "step1_tile_diff_audit": step1,
        "step2_progress_manifest_audit": step2,
        "step3_cp932_hex_linter": step3,
        "step4_capture_provenance_audit": step4,
    }

    args.output_json.parent.mkdir(parents=True, exist_ok=True)
    args.output_json.write_text(json.dumps(report, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    print(f"\n==> Wrote audit report to {args.output_json}")

    if not overall_passed:
        print("==> [FAIL] One or more audit gates failed!", file=sys.stderr)
        return 1

    print("==> [PASS] All T-29018 / T-29022 emulator-vs-Godot audit gates passed!")
    return 0


if __name__ == "__main__":
    sys.exit(main())
