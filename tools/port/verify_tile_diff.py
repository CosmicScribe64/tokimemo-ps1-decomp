#!/usr/bin/env python3
"""16x16 tile hotspot and HUD sub-region pixel verifier against PS1 emulator captures."""

from __future__ import annotations

import argparse
import json
import struct
import sys
import zlib
from pathlib import Path
from typing import Any

try:
    from PIL import Image  # type: ignore[import-not-found]
except ImportError:

    class _PixelAccess:
        def __init__(self, buf: bytearray, width: int) -> None:
            self._buf = buf
            self._w = width

        def __getitem__(self, xy: tuple[int, int]) -> tuple[int, int, int]:
            idx = (xy[1] * self._w + xy[0]) * 3
            return (self._buf[idx], self._buf[idx + 1], self._buf[idx + 2])

        def __setitem__(self, xy: tuple[int, int], val: tuple[int, int, int]) -> None:
            idx = (xy[1] * self._w + xy[0]) * 3
            self._buf[idx] = val[0] & 0xFF
            self._buf[idx + 1] = val[1] & 0xFF
            self._buf[idx + 2] = val[2] & 0xFF

    class _PureImage:
        class Resampling:
            NEAREST = 0

        def __init__(self, width: int, height: int, buf: bytearray) -> None:
            self.size = (width, height)
            self._buf = buf

        @classmethod
        def new(
            cls,
            _mode: str,
            size: tuple[int, int],
            color: tuple[int, int, int] = (0, 0, 0),
        ) -> _PureImage:
            w, h = size
            pixel = bytes((color[0] & 0xFF, color[1] & 0xFF, color[2] & 0xFF))
            return cls(w, h, bytearray(pixel * (w * h)))

        @classmethod
        def open(cls, path: Path | str) -> _PureImage:
            raw = Path(path).read_bytes()
            if raw[:8] != b"\x89PNG\r\n\x1a\n":
                raise ValueError(f"Not a PNG file: {path}")
            pos = 8
            width = height = bit_depth = color_type = 0
            plte = b""
            idat_parts: list[bytes] = []
            while pos + 8 <= len(raw):
                length, ctype = struct.unpack(">I4s", raw[pos : pos + 8])
                chunk = raw[pos + 8 : pos + 8 + length]
                pos += 12 + length
                if ctype == b"IHDR":
                    width, height, bit_depth, color_type = struct.unpack(">IIBB", chunk[:10])
                elif ctype == b"PLTE":
                    plte = chunk
                elif ctype == b"IDAT":
                    idat_parts.append(chunk)
                elif ctype == b"IEND":
                    break
            if bit_depth != 8 or color_type not in (0, 2, 3, 4, 6):
                raise ValueError(f"Unsupported PNG format (depth={bit_depth}, color={color_type})")
            bpp = {0: 1, 2: 3, 3: 1, 4: 2, 6: 4}[color_type]
            stride = width * bpp
            decompressed = zlib.decompress(b"".join(idat_parts))
            prev = bytearray(stride)
            cur = bytearray(stride)
            out = bytearray(width * height * 3)
            src_pos = 0
            dst_pos = 0
            for _y in range(height):
                ftype = decompressed[src_pos]
                row = decompressed[src_pos + 1 : src_pos + 1 + stride]
                src_pos += 1 + stride
                if ftype == 0:
                    cur[:] = row
                elif ftype == 1:
                    for i in range(stride):
                        a = cur[i - bpp] if i >= bpp else 0
                        cur[i] = (row[i] + a) & 0xFF
                elif ftype == 2:
                    for i in range(stride):
                        cur[i] = (row[i] + prev[i]) & 0xFF
                elif ftype == 3:
                    for i in range(stride):
                        a = cur[i - bpp] if i >= bpp else 0
                        b = prev[i]
                        cur[i] = (row[i] + ((a + b) >> 1)) & 0xFF
                elif ftype == 4:
                    for i in range(stride):
                        a = cur[i - bpp] if i >= bpp else 0
                        b = prev[i]
                        c = prev[i - bpp] if i >= bpp else 0
                        p = a + b - c
                        pa = abs(p - a)
                        pb = abs(p - b)
                        pc = abs(p - c)
                        pr = a if (pa <= pb and pa <= pc) else (b if pb <= pc else c)
                        cur[i] = (row[i] + pr) & 0xFF
                prev[:] = cur
                if color_type == 2:
                    out[dst_pos : dst_pos + width * 3] = cur
                    dst_pos += width * 3
                elif color_type == 6:
                    for x in range(0, stride, 4):
                        out[dst_pos : dst_pos + 3] = cur[x : x + 3]
                        dst_pos += 3
                elif color_type == 3:
                    for idx in cur:
                        p = idx * 3
                        if p + 3 <= len(plte):
                            out[dst_pos : dst_pos + 3] = plte[p : p + 3]
                        dst_pos += 3
                elif color_type in (0, 4):
                    for x in range(0, stride, bpp):
                        v = cur[x]
                        out[dst_pos] = v
                        out[dst_pos + 1] = v
                        out[dst_pos + 2] = v
                        dst_pos += 3
            return cls(width, height, out)

        def convert(self, _mode: str) -> _PureImage:
            return self

        def copy(self) -> _PureImage:
            return _PureImage(self.size[0], self.size[1], bytearray(self._buf))

        def load(self) -> _PixelAccess:
            return _PixelAccess(self._buf, self.size[0])

        def getpixel(self, xy: tuple[int, int]) -> tuple[int, int, int]:
            return self.load()[xy]

        def putpixel(self, xy: tuple[int, int], val: tuple[int, int, int]) -> None:
            self.load()[xy] = val

        def resize(self, new_size: tuple[int, int], _resample: int = 0) -> _PureImage:
            nw, nh = new_size
            ow, oh = self.size
            out = _PureImage.new("RGB", (nw, nh))
            src = self.load()
            dst = out.load()
            for y in range(nh):
                sy = min(oh - 1, (y * oh) // nh)
                for x in range(nw):
                    sx = min(ow - 1, (x * ow) // nw)
                    dst[x, y] = src[sx, sy]
            return out

        def save(self, path: Path | str) -> None:
            w, h = self.size
            raw_rows = bytearray()
            stride = w * 3
            for y in range(h):
                raw_rows.append(0)
                raw_rows.extend(self._buf[y * stride : (y + 1) * stride])

            def _chunk(ctype: bytes, payload: bytes) -> bytes:
                crc = zlib.crc32(ctype + payload) & 0xFFFFFFFF
                return struct.pack(">I4s", len(payload), ctype) + payload + struct.pack(">I", crc)

            ihdr = struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)
            idat = zlib.compress(bytes(raw_rows), 6)
            png = b"\x89PNG\r\n\x1a\n" + _chunk(b"IHDR", ihdr) + _chunk(b"IDAT", idat) + _chunk(b"IEND", b"")
            Path(path).write_bytes(png)

    Image = _PureImage  # type: ignore[misc,assignment]

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
