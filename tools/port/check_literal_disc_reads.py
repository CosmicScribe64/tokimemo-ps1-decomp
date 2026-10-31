#!/usr/bin/env python3
"""T-39303: check every literal cd_read_sectors / cd_read_sectors_nowait call in src/ against a disc pack.

A call `cd_read_sectors(count, destination, lba)` with a literal count and a literal LBA
must lie in packed extents that are contiguous (one file, or adjacent files with no gap and
no excluded extent), the rule of port/godot/services/disc_pack.gd. Calls whose count or LBA
is not a literal (dec_bg_cd_read, bg_read_sub2, table-driven reads) are only counted: they
are NOT covered by this check.

Run in Docker (repository mounted at /work, pack read-only):
  python3 tools/port/check_literal_disc_reads.py --pack .asset-cache/<pack dir> [--src src]
Exit status 1 when a literal read is not covered.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import sys

CALL = re.compile(r"\bcd_read_sectors(?:_nowait)?\s*\(\s*([^,()]+?)\s*,\s*([^,()]+?)\s*,\s*([^,()]+?)\s*\)")
LITERAL = re.compile(r"^(0[xX][0-9A-Fa-f]+|\d+)[uUlL]*$")


def literal(text: str) -> int | None:
    match = LITERAL.match(text.strip())
    return int(match.group(1), 0) if match else None


def classify(files: list[dict], excluded: list[dict], lba: int, count: int) -> tuple[str, str]:
    """Return ("single"|"span"|"fail", detail) for a read of `count` sectors at `lba`."""
    end = lba + count
    cursor, touched = lba, []
    while cursor < end:
        owner = next((f for f in files if f["lba"] <= cursor < f["lba"] + f["sectors"]), None)
        if owner is None:
            hit = next((e for e in excluded if e["lba"] <= cursor < e["lba"] + e["sectors"]), None)
            return "fail", ("LBA 0x%X is in excluded %s" % (cursor, hit["path"]) if hit
                            else "LBA 0x%X is outside every file" % cursor)
        touched.append(owner["path"])
        cursor = owner["lba"] + owner["sectors"]
    return ("single" if len(touched) == 1 else "span"), ", ".join(touched)


def scan(src: Path, files: list[dict], excluded: list[dict]) -> dict:
    result = {"literal": 0, "single": 0, "span": [], "fail": [], "computed": 0}
    for path in sorted(src.rglob("*.c")):
        text = path.read_text(encoding="latin1")
        for match in CALL.finditer(text):
            line = text.count("\n", 0, match.start()) + 1
            count, lba = literal(match.group(1)), literal(match.group(3))
            if count is None or lba is None:
                result["computed"] += 1
                continue
            result["literal"] += 1
            kind, detail = classify(files, excluded, lba, count)
            place = "%s:%d (count 0x%X, LBA 0x%X)" % (path, line, count, lba)
            if kind == "single":
                result["single"] += 1
            else:
                result[kind].append(place + " " + detail)
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--pack", type=Path, required=True)
    parser.add_argument("--src", type=Path, default=Path("src"))
    args = parser.parse_args()
    manifest = json.loads((args.pack / "disc_pack.json").read_text())
    result = scan(args.src, sorted(manifest["files"], key=lambda f: f["lba"]), manifest["excluded"])
    print("literal reads: %d in one file, %d across adjacent files, %d not covered; %d computed (not checked)" % (
        result["single"], len(result["span"]), len(result["fail"]), result["computed"]))
    for line in result["span"]:
        print("span: " + line)
    for line in result["fail"]:
        print("FAIL: " + line)
    return 1 if result["fail"] else 0


if __name__ == "__main__":
    sys.exit(main())
