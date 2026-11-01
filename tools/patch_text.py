#!/usr/bin/env python3
"""Text-container writers for the English patch: round-trip, coverage and size reports (T-39503).

Docker only:
  tools/docker.sh python3 -m tools.patch_text all
  TOKIMEMO_SCRATCH=<dir with track1.bin> tools/docker.sh python3 -m tools.patch_text lba-map /scratch/track1.bin

Inputs: ``disc/files`` (original files), ``.asset-cache/text/units/units.jsonl`` (from
``tools.assets.text_export --include-source`` then ``tools.assets.text_units``) and the two
English JSONL files. Reports go to ``.asset-cache/patch-text/`` (ignored); they hold ids,
offsets and counts, never game text. Exit status 1 when a round-trip differs or a record
id is unmapped.

Commands:
  roundtrip  rebuild every container and every C literal from the original records
  coverage   map every English record id to one location; scan for text without records
  sizes      write every container with the provisional English encoding and report sizes
  all        the three above
  lba-map    read the ISO directory of the original data track (for the read-site check)
"""
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

from tools.patch.text import c_literals, coverage, sizes
from tools.patch.text.containers import build_containers, identity_check, load_units
from tools.patch.text.segments import ProvisionalAsciiEncoder
from tools.patch.text.translations import load_jsonl, load_targets

REPOSITORY = Path(__file__).resolve().parents[1]
OUT = REPOSITORY / ".asset-cache" / "patch-text"


def _write(name: str, value) -> Path:
    OUT.mkdir(parents=True, exist_ok=True)
    path = OUT / name
    path.write_text(json.dumps(value, indent=1, sort_keys=True) + "\n", encoding="utf-8")
    return path


def _inputs(args):
    units = load_units(args.units)
    supplemental = load_jsonl(args.supplemental)
    return units, supplemental, build_containers(args.disc, units, supplemental)


def cmd_roundtrip(args, units, supplemental, containers) -> bool:
    rows = [identity_check(c) for c in containers.values() if c.image is not None]
    sites = c_literals.locate_sites(args.repository, units)
    literal = c_literals.identity_check(args.repository, sites)
    bad = [r for r in rows if r["status"] != "identical"]
    by_kind = {}
    for row in rows:
        item = by_kind.setdefault(row["kind"], {"files": 0, "identical": 0, "slots": 0, "pointers": 0})
        item["files"] += 1
        item["identical"] += row["status"] == "identical"
        item["slots"] += row["slots"]
        item["pointers"] += row["pointers"]
    _write("roundtrip.json", {"containers": rows, "by_kind": by_kind, "c_literals": literal})
    print(json.dumps({"by_kind": by_kind, "c_literals": literal, "different": [r["path"] for r in bad]}))
    return not bad and literal["status"] == "identical"


def cmd_coverage(args, units, supplemental, containers) -> bool:
    mapping = coverage.coverage_map(containers)
    ids = [row["id"] for row in load_jsonl(args.returned)] + [row["id"] for row in supplemental]
    check = coverage.check_ids(mapping, ids)
    gaps = coverage.text_gaps(containers, args.disc, args.repository)
    magazine = coverage.magazine_tables(containers)
    _write("coverage-map.json", mapping)
    _write("coverage.json", {"ids": check, "gaps": gaps, "magazine_tables": magazine})
    print(json.dumps({"ids": {k: (len(v) if isinstance(v, list) else v) for k, v in check.items()},
                      "gap_containers": {p: g["count"] for p, g in gaps["containers"].items()},
                      "gap_other_files": {p: g["count"] for p, g in gaps["other_files"].items()},
                      "magazine_slots_without_record": sum(not r["records"] for r in magazine)}))
    return not check["unmapped"] and not check["duplicate_ids"]


def cmd_sizes(args, units, supplemental, containers) -> bool:
    targets = load_targets(args.returned, args.supplemental)
    encoder = ProvisionalAsciiEncoder()
    payloads, problems = sizes.english_payloads(containers, targets, encoder)
    lba = json.loads(Path(args.lba_map).read_text()) if Path(args.lba_map).is_file() else None
    report = sizes.size_report(containers, payloads, lba_map=lba,
                               read_sites=sizes.scan_read_sites(Path(args.repository) / "src"),
                               repository=args.repository)
    report["encoder"] = encoder.name
    report["unmapped_characters"] = len(encoder.unmapped)
    report["record_problems"] = {k: {"count": len(v), "ids": v[:20]} for k, v in problems.items()}
    _write("sizes.json", report)
    summary = {k: v for k, v in report["summary"].items() if k not in ("c_literal_growth_by_unit",
                                                                       "pointer_rewrites_by_file")}
    print(json.dumps({"summary": summary, "lba_map": lba is not None,
                      "problems": {k: v["count"] for k, v in report["record_problems"].items()}}))
    return True


def cmd_lba_map(args) -> bool:
    from tools.assets.input import open_input
    with open_input(args.image) as source:
        entries = source.entries()
    table = {e["path"]: {"lba": e["location"]["lba"], "size": e["size"]}
             for e in entries if "lba" in e.get("location", {})}
    print(_write("lba-map.json", table), len(table))
    return True


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("command", choices=("roundtrip", "coverage", "sizes", "all", "lba-map"))
    parser.add_argument("image", nargs="?", help="lba-map: original data track (MODE2/2352)")
    parser.add_argument("--disc", default=str(REPOSITORY / "disc" / "files"))
    parser.add_argument("--units", default=str(REPOSITORY / ".asset-cache" / "text" / "units" / "units.jsonl"))
    parser.add_argument("--returned", default=str(REPOSITORY / "translations" / "en" / "returned-v3.jsonl"))
    parser.add_argument("--supplemental", default=str(REPOSITORY / "translations" / "en" / "supplemental.jsonl"))
    parser.add_argument("--lba-map", default=str(OUT / "lba-map.json"))
    parser.add_argument("--repository", default=str(REPOSITORY))
    args = parser.parse_args(argv)
    if args.command == "lba-map":
        if not args.image:
            parser.error("lba-map needs the data track path")
        return 0 if cmd_lba_map(args) else 1
    inputs = _inputs(args)
    commands = {"roundtrip": [cmd_roundtrip], "coverage": [cmd_coverage], "sizes": [cmd_sizes],
                "all": [cmd_roundtrip, cmd_coverage, cmd_sizes]}[args.command]
    ok = all([command(args, *inputs) for command in commands])
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
