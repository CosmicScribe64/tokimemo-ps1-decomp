#!/usr/bin/env python3
"""16x16 tile hotspot and HUD sub-region pixel verifier against PS1 emulator captures."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any

from PIL import Image

TILE_SIZE = 16
SCREEN_W = 320
SCREEN_H = 240

DEFAULT_REGIONS: dict[str, tuple[int, int, int, int]] = {
    "title_badge": (8, 20, 84, 45),
    "param_panel": (28, 23, 264, 42),
    "date_pillar": (294, 45, 20, 114),
    "center_scene": (36, 72, 254, 88),
    "message_window": (27, 166, 266, 52),
}


def normalize_320x240(img: Image.Image) -> Image.Image:
    rgb = img.convert("RGB")
    if rgb.size == (SCREEN_W, SCREEN_H):
        return rgb
    w, h = rgb.size
    if w % SCREEN_W == 0 and h % SCREEN_H == 0:
        sx = w // SCREEN_W
        sy = h // SCREEN_H
        out = Image.new("RGB", (SCREEN_W, SCREEN_H))
        src = rgb.load()
        dst = out.load()
        ox = sx // 2
        oy = sy // 2
        for y in range(SCREEN_H):
            for x in range(SCREEN_W):
                dst[x, y] = src[x * sx + ox, y * sy + oy]
        return out
    return rgb.resize((SCREEN_W, SCREEN_H), Image.Resampling.NEAREST)


def compute_region_stats(
    emu: Image.Image, gd: Image.Image, rect: tuple[int, int, int, int]
) -> dict[str, Any]:
    rx, ry, rw, rh = rect
    p_emu = emu.load()
    p_gd = gd.load()
    total_err = 0.0
    max_diff = 0.0
    px_over_10 = 0
    for y in range(ry, ry + rh):
        for x in range(rx, rx + rw):
            pe = p_emu[x, y]
            pg = p_gd[x, y]
            d = (abs(pe[0] - pg[0]) + abs(pe[1] - pg[1]) + abs(pe[2] - pg[2])) / 3.0
            total_err += d
            if d > max_diff:
                max_diff = d
            if d > 10.0:
                px_over_10 += 1
    area = max(1, rw * rh)
    return {
        "rect": [rx, ry, rw, rh],
        "mae": round(total_err / area, 4),
        "max_px_diff": round(max_diff, 2),
        "px_over_10": px_over_10,
    }


def compare_16x16_tiles(
    emu_path: Path | str,
    gd_path: Path | str,
    hotspot_threshold: float = 20.0,
) -> dict[str, Any]:
    emu = normalize_320x240(Image.open(emu_path))
    gd = normalize_320x240(Image.open(gd_path))
    p_emu = emu.load()
    p_gd = gd.load()

    tiles: list[dict[str, Any]] = []
    hotspots: list[dict[str, Any]] = []
    total_err = 0.0

    for ty in range(0, SCREEN_H, TILE_SIZE):
        for tx in range(0, SCREEN_W, TILE_SIZE):
            t_err = 0.0
            t_max = 0.0
            t_cnt = 0
            for y in range(max(1, ty), ty + TILE_SIZE):
                for x in range(max(1, tx), tx + TILE_SIZE):
                    pe = p_emu[x, y]
                    pg = p_gd[x, y]
                    d = (
                        abs(pe[0] - pg[0])
                        + abs(pe[1] - pg[1])
                        + abs(pe[2] - pg[2])
                    ) / 3.0
                    t_err += d
                    t_cnt += 1
                    if d > t_max:
                        t_max = d
            total_err += t_err
            mae = t_err / max(1, t_cnt)
            rec = {
                "x": tx,
                "y": ty,
                "w": TILE_SIZE,
                "h": TILE_SIZE,
                "mae": round(mae, 3),
                "max_px_diff": round(t_max, 2),
            }
            tiles.append(rec)
            if mae > hotspot_threshold:
                hotspots.append(rec)

    hotspots.sort(key=lambda r: r["mae"], reverse=True)
    regions = {
        name: compute_region_stats(emu, gd, rect)
        for name, rect in DEFAULT_REGIONS.items()
    }

    return {
        "emulator_image": str(emu_path),
        "godot_image": str(gd_path),
        "overall_mae": round(total_err / (SCREEN_W * SCREEN_H), 4),
        "tile_count": len(tiles),
        "hotspot_threshold": hotspot_threshold,
        "hotspot_count": len(hotspots),
        "max_tile_mae": hotspots[0]["mae"] if hotspots else (max(t["mae"] for t in tiles) if tiles else 0.0),
        "top_hotspots": hotspots[:12],
        "regions": regions,
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Run 16x16 tile hotspot diff between a PS1 emulator capture and a Godot port capture."
    )
    parser.add_argument("emu_image", type=Path, help="Path to PS1 emulator reference PNG")
    parser.add_argument("godot_image", type=Path, help="Path to Godot port PNG")
    parser.add_argument(
        "--threshold",
        type=float,
        default=20.0,
        help="Tile MAE threshold for flagging hotspots (default: 20.0)",
    )
    parser.add_argument(
        "--max-hotspots",
        type=int,
        default=None,
        help="Fail with exit code 1 if hotspot_count exceeds this limit",
    )
    parser.add_argument("--json", action="store_true", help="Output JSON report")
    args = parser.parse_args(argv)

    report = compare_16x16_tiles(args.emu_image, args.godot_image, args.threshold)
    if args.json:
        print(json.dumps(report, indent=2))
    else:
        print(
            f"Overall MAE: {report['overall_mae']:.3f} | "
            f"Hotspot tiles (> {args.threshold:.1f} MAE): {report['hotspot_count']} / {report['tile_count']}"
        )
        for name, rstat in report["regions"].items():
            print(
                f"  Region {name:16s} {tuple(rstat['rect'])}: "
                f"MAE={rstat['mae']:.3f}, max_px={rstat['max_px_diff']:.1f}, px>10={rstat['px_over_10']}"
            )
        for h in report["top_hotspots"]:
            print(
                f"  Hotspot tile ({h['x']:3d},{h['y']:3d})..({h['x']+16:3d},{h['y']+16:3d}): "
                f"MAE={h['mae']:.2f}, max_px={h['max_px_diff']:.1f}"
            )

    if args.max_hotspots is not None and report["hotspot_count"] > args.max_hotspots:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
