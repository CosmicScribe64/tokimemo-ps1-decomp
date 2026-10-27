#!/usr/bin/env python3
"""Developer export of six original BGM source files; no PCM/cache fallback."""
from __future__ import annotations
import argparse
from pathlib import Path
import sys
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.assets.input import open_input
from tools.port.original_first_arrival_sources import SOURCES, export_bytes
from tools.port.prologue_hud_content import _physical_path


def build(input_path: Path, output: Path) -> dict:
    original = _physical_path(input_path, "original input", must_exist=True)
    target = _physical_path(output, "output", must_exist=False)
    if target == original or original in target.parents or target in original.parents:
        raise ValueError("BGM output overlaps original input")
    if target.name != "first-arrival" or target.parent.name != "audio":
        raise ValueError("BGM output must be the final audio/first-arrival role")
    if target.exists():
        raise ValueError("BGM output must be fresh")
    repository = ROOT.resolve()
    staging = repository / ".asset-cache" / "port"
    if (target == repository or repository in target.parents) and not (target == staging or staging in target.parents):
        raise ValueError("BGM output inside the repository must remain in ignored .asset-cache/port staging")
    for protected in (ROOT / "disc", ROOT / "raw", ROOT / ".git"):
        physical = protected.resolve()
        if target == physical or physical in target.parents or target in physical.parents:
            raise ValueError("BGM output overlaps protected source data")
    inputs = {}
    with open_input(str(original)) as source:
        entries = source.entries()
        for identity, _, path, size, _ in SOURCES:
            matches = [entry for entry in entries if entry.get("path") == path and entry.get("kind") == "file" and entry.get("size") == size]
            if len(matches) != 1:
                raise ValueError("original BGM source identity is missing or ambiguous: " + identity)
            inputs[identity] = source.read(matches[0])
    return export_bytes(inputs, target)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    try:
        result = build(args.input, args.output)
    except (OSError, ValueError) as exc:
        print("BGM export rejected: " + str(exc), file=sys.stderr)
        raise SystemExit(1)
    print("BGM source role exported: %d files" % result["files"])
