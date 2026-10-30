#!/usr/bin/env python3
"""T-39026: checkpoint contact sheets for the human visual review queue.

One PNG per checkpoint row, cells left to right:

    native | port-ja | port-en | port 16:9 full window | exact diff heatmap | rgb555 residual

The 320x240 cells are drawn at integer 2x, nearest neighbour. The full-window
cell is resized with nearest neighbour to the row height and labelled with its
factor. Every cell is labelled with the row key, sync status, defect id and the
sha256 of its input, and linter findings are drawn as outlined boxes.

The exact diff compares native and port-ja RGBA byte for byte (changed pixels
red, unchanged pixels dimmed grey); it is the same rule as compare_capture_pair.py
and needs equal sizes, never rescaling. The rgb555 residual decodes both through
the encodings the row declares (tools/port/rgb555_diagnostic.py rules) and marks
differing 5-bit codes red and out-of-encoding pixels magenta. It is diagnostic,
never a parity gate. A missing input or a size mismatch is drawn as an
"unavailable" cell and recorded, never passed.

Input: a JSON spec (``tokimemo.contact-sheet-input.v1``) with ``rows``; image
paths are relative to the spec. Output: a NEW directory under an ``.asset-cache``
directory holding ``rows/<key>.png`` and ``sheet.json`` (row image sha256,
input hashes, findings, severity), which tools/port/progress/review_queue.py
reads. Nothing here is committed or published.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

from PIL import Image, ImageDraw, ImageFont

from tools.port.rgb555_diagnostic import decode_channel

SPEC_SCHEMA = "tokimemo.contact-sheet-input.v1"
SHEET_SCHEMA = "tokimemo.contact-sheet.v1"
NATIVE_SIZE = (320, 240)
SCALE = 2
CELL = (NATIVE_SIZE[0] * SCALE, NATIVE_SIZE[1] * SCALE)
LABEL_HEIGHT = 48
HEADER_HEIGHT = 20
GAP = 6
CELLS = ("native", "port_ja", "port_en", "port_wide", "exact_diff", "rgb555_residual")
SYNC_STATUSES = ("exact", "known_diff", "unsynchronizable", "unknown")
ENCODINGS = ("zero_extended", "bit_replicated", "source_proven_mixed")
KEY_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._-]{0,95}$")


class SheetError(ValueError):
    """Invalid spec or output location."""


def sha256_file(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def require_cache_output(output: Path) -> Path:
    """Output must be new and inside a directory named .asset-cache (never the repo tree)."""
    output = output.absolute()
    if output.exists() or output.is_symlink():
        raise SheetError("output must be a new directory: " + str(output))
    if ".asset-cache" not in output.parts[:-1]:
        raise SheetError("contact sheets go under .asset-cache only")
    return output


def _font():
    return ImageFont.load_default()


def unavailable(reason: str, size=CELL) -> Image.Image:
    cell = Image.new("RGB", size, (48, 48, 48))
    draw = ImageDraw.Draw(cell)
    draw.line([(0, 0), (size[0] - 1, size[1] - 1)], fill=(90, 90, 90), width=2)
    draw.text((12, 12), "UNAVAILABLE: " + reason, fill=(255, 200, 0), font=_font())
    return cell


def scaled(image: Image.Image) -> Image.Image:
    return image.convert("RGB").resize((image.width * SCALE, image.height * SCALE), Image.Resampling.NEAREST)


def exact_diff(native: Image.Image, port: Image.Image) -> tuple[Image.Image, int]:
    """Byte-exact RGBA comparison, as compare_capture_pair.py; no tolerance."""
    left, right = native.convert("RGBA"), port.convert("RGBA")
    out = Image.new("RGB", left.size)
    changed = 0
    pixels = []
    for a, b in zip(left.getdata(), right.getdata()):
        if a != b:
            changed += 1
            level = max(abs(x - y) for x, y in zip(a, b))
            pixels.append((max(96, level), 0, 0))
        else:
            grey = (a[0] * 3 + a[1] * 6 + a[2]) // 40
            pixels.append((grey, grey, grey))
    out.putdata(pixels)
    return out, changed


def rgb555_residual(native: Image.Image, port: Image.Image, native_encoding: str,
                    port_encoding: str) -> tuple[Image.Image, dict]:
    """5-bit code residual under declared encodings; out-of-encoding pixels are marked, not dropped."""
    left, right = native.convert("RGB"), port.convert("RGB")
    out = Image.new("RGB", left.size)
    counts = {"residual": 0, "out_of_encoding": 0}
    pixels = []
    for a, b in zip(left.getdata(), right.getdata()):
        try:
            codes_a = [decode_channel(c, native_encoding) for c in a]
            codes_b = [decode_channel(c, port_encoding) for c in b]
        except ValueError:
            counts["out_of_encoding"] += 1
            pixels.append((255, 0, 255))
            continue
        if codes_a != codes_b:
            counts["residual"] += 1
            delta = max(abs(x - y) for x, y in zip(codes_a, codes_b))
            pixels.append((min(255, 120 + delta * 8), 0, 0))
        else:
            grey = (a[0] * 3 + a[1] * 6 + a[2]) // 40
            pixels.append((grey, grey, grey))
    out.putdata(pixels)
    return out, counts


def _load(base: Path, relative, where: str):
    if relative is None:
        return None, None
    if not isinstance(relative, str) or relative.startswith("/") or ".." in Path(relative).parts:
        raise SheetError(where + ": image path must be relative to the spec")
    path = base / relative
    if not path.is_file():
        return None, None
    with Image.open(path) as image:
        image.load()
        return image.copy(), sha256_file(path)


def _label(draw, x: int, y: int, lines: list[str]) -> None:
    for index, line in enumerate(lines):
        draw.text((x + 4, y + 2 + index * 14), line[:100], fill=(230, 230, 230), font=_font())


def render_row(row: dict, base: Path) -> tuple[Image.Image, dict]:
    key = row.get("key")
    if not isinstance(key, str) or not KEY_RE.fullmatch(key):
        raise SheetError("row key must match " + KEY_RE.pattern)
    sync = row.get("sync_status", "unknown")
    if sync not in SYNC_STATUSES:
        raise SheetError(f"{key}: sync_status must be one of {SYNC_STATUSES}")
    findings = row.get("findings", [])
    if not isinstance(findings, list):
        raise SheetError(f"{key}: findings must be a list")
    # Objective means "carries linter findings"; it is derived, and an unbacked claim is refused.
    severity, kind = row.get("severity", 1), row.get("kind") or ("objective" if findings else "subjective")
    if type(severity) is not int or not 0 <= severity <= 3 or kind not in ("objective", "subjective"):
        raise SheetError(f"{key}: severity must be 0..3 and kind objective or subjective")
    if kind == "objective" and not findings:
        raise SheetError(f"{key}: kind objective needs linter findings on the row")
    encodings = (row.get("native_encoding"), row.get("port_encoding"))
    if any(e not in ENCODINGS for e in encodings):
        raise SheetError(f"{key}: native_encoding and port_encoding must be declared from {ENCODINGS}")
    images, hashes, notes = {}, {}, {}
    for cell in ("native", "port_ja", "port_en", "port_wide"):
        images[cell], hashes[cell] = _load(base, row.get(cell), f"{key}.{cell}")
        if images[cell] is not None and cell != "port_wide" and images[cell].size != NATIVE_SIZE:
            notes[cell] = f"size {images[cell].size} is not 320x240"
            images[cell] = None
        elif images[cell] is None:
            notes[cell] = "input missing"
    tiles: dict[str, Image.Image] = {}
    for cell in ("native", "port_ja", "port_en"):
        tiles[cell] = scaled(images[cell]) if images[cell] is not None else unavailable(notes[cell])
    wide = images["port_wide"]
    wide_factor = None
    if wide is not None:
        wide_factor = CELL[1] / wide.height
        width = max(1, round(wide.width * wide_factor))
        tiles["port_wide"] = wide.convert("RGB").resize((width, CELL[1]), Image.Resampling.NEAREST)
    else:
        tiles["port_wide"] = unavailable(notes["port_wide"], (CELL[1] * 16 // 9, CELL[1]))
    metrics: dict = {}
    if images["native"] is not None and images["port_ja"] is not None:
        diff, changed = exact_diff(images["native"], images["port_ja"])
        residual, counts = rgb555_residual(images["native"], images["port_ja"], *encodings)
        tiles["exact_diff"], tiles["rgb555_residual"] = scaled(diff), scaled(residual)
        metrics = {"exact_changed_pixels": changed, **{"rgb555_" + k: v for k, v in counts.items()}}
    else:
        tiles["exact_diff"] = unavailable("needs native and port-ja")
        tiles["rgb555_residual"] = unavailable("needs native and port-ja")
    widths = [tiles[c].width for c in CELLS]
    sheet = Image.new("RGB", (sum(widths) + GAP * (len(CELLS) - 1), HEADER_HEIGHT + CELL[1] + LABEL_HEIGHT), (16, 16, 16))
    draw = ImageDraw.Draw(sheet)
    defect = row.get("defect_id") or "-"
    draw.text((4, 4), f"{key} | sync {sync} | defect {defect} | encodings {encodings[0]}/{encodings[1]}",
              fill=(255, 255, 255), font=_font())
    x = 0
    offsets = {}
    for cell, width in zip(CELLS, widths):
        offsets[cell] = x
        sheet.paste(tiles[cell], (x, HEADER_HEIGHT))
        extra = {"port_wide": f"nearest x{wide_factor:.3f} to row height" if wide_factor else "",
                 "exact_diff": f"changed {metrics.get('exact_changed_pixels', '-')}",
                 "rgb555_residual": f"residual {metrics.get('rgb555_residual', '-')} out-of-enc {metrics.get('rgb555_out_of_encoding', '-')}"}.get(cell, "")
        digest = hashes.get(cell) or "-"
        _label(draw, x, HEADER_HEIGHT + CELL[1], [f"{cell} | {key} | {sync} | {defect}", "sha256 " + digest[:16], extra])
        x += width + GAP
    for finding in findings:
        cell, rect = finding.get("cell"), finding.get("rect")
        if cell not in ("native", "port_ja", "port_en", "port_wide") or not (
                isinstance(rect, list) and len(rect) == 4 and all(type(v) is int for v in rect)):
            raise SheetError(f"{key}: finding needs a source cell and an integer [x, y, w, h] rect")
        factor = wide_factor if cell == "port_wide" else SCALE
        if factor is None:
            continue
        left = offsets[cell] + round(rect[0] * factor)
        top = HEADER_HEIGHT + round(rect[1] * factor)
        draw.rectangle([left, top, left + max(1, round(rect[2] * factor)) - 1, top + max(1, round(rect[3] * factor)) - 1],
                       outline=(255, 230, 0), width=2)
        draw.text((left + 2, max(HEADER_HEIGHT, top - 12)), str(finding.get("rule", "finding"))[:40], fill=(255, 230, 0), font=_font())
    record = {"key": key, "sync_status": sync, "defect_id": row.get("defect_id"),
              "input_sha256": hashes, "unavailable": notes, "metrics": metrics,
              "findings": findings, "severity": severity, "kind": kind}
    return sheet, record


def build(spec_path: Path, output: Path) -> dict:
    spec = json.loads(spec_path.read_text())
    if not isinstance(spec, dict) or spec.get("schema") != SPEC_SCHEMA or not isinstance(spec.get("rows"), list):
        raise SheetError("spec is not " + SPEC_SCHEMA)
    output = require_cache_output(output)
    rendered = [render_row(row, spec_path.parent) for row in spec["rows"]]
    keys = [record["key"] for _, record in rendered]
    if len(set(keys)) != len(keys):
        raise SheetError("row keys must be unique")
    (output / "rows").mkdir(parents=True)
    rows = []
    for image, record in rendered:
        path = output / "rows" / (record["key"] + ".png")
        image.save(path, format="PNG", optimize=False)
        rows.append({**record, "image": "rows/" + path.name, "image_sha256": sha256_file(path)})
    sheet = {"schema": SHEET_SCHEMA, "spec_sha256": sha256_file(spec_path), "scale": SCALE, "cells": list(CELLS),
             "note": "local review material under .asset-cache; never committed or published", "rows": rows}
    (output / "sheet.json").write_text(json.dumps(sheet, indent=1, sort_keys=True) + "\n")
    return sheet


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("spec", type=Path)
    parser.add_argument("--output", type=Path, required=True, help="new directory under .asset-cache")
    args = parser.parse_args(argv)
    try:
        sheet = build(args.spec, args.output)
    except (SheetError, ValueError, OSError) as error:
        print("FAIL: " + str(error), file=sys.stderr)
        return 2
    print(f"wrote {len(sheet['rows'])} rows to {args.output}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
