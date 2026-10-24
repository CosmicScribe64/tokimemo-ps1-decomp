#!/usr/bin/env python3
"""Export verified linked dialogue to ignored engine-independent JSON (T-28001).

Docker invocation:
  python3 -m tools.assets.text_export disc/files --output .asset-cache/text

API: export_text(input_dir, output_dir=None, profiles=None) returns a JSON-compatible
catalog. Explicit profiles may be supplied via --profiles (a JSON list matching
script_profiles.PROFILES). Invalid known layouts fail instead of falling back to
string scans; unprofiled SERIFU files appear in the unsupported inventory.
"""
from __future__ import annotations

import argparse
import json
from pathlib import Path

from .script_profiles import PROFILES, NON_TEXT_PROFILES
from .text import TextFormatError, parse_script, sha256, source_path
from .text_font import attach_font_references, font_metadata
from .script_validation import checked_cd_trailer, validate_script
from .text_tables import DICTIONARY_PROFILES, NAME_KAN_PROFILE, parse_character_lists, parse_dictionary
from .text_fixed import tokenize_fixed_text
from .text_renderers import DISPLAY_ARGUMENT_INDEX, FORMAT_ARGUMENT_INDEX

REPOSITORY = Path(__file__).resolve().parents[2]
OUTPUT_ROOT = REPOSITORY / ".asset-cache" / "text"


def _input_file(root: Path, path: str) -> Path:
    resolved = (root / path).resolve(strict=True)
    if not resolved.is_relative_to(root) or not resolved.is_file():
        raise TextFormatError("input source escapes extracted directory or is not a file")
    return resolved


def _numeric_catalog(data: bytes, profile: dict) -> dict:
    if sha256(data) != profile["sha256"]:
        raise TextFormatError("numeric data checksum differs from the verified layout")
    tables = []
    for table in profile["tables"]:
        start, length = table["offset"], table["length"]
        if start < 0 or length < 0 or start + length > len(data):
            raise TextFormatError("numeric table outside input")
        raw = data[start:start + length]
        tables.append({**table, "raw_hex": raw.hex(), "values": list(raw)})
    tail = profile["uninterpreted_tail"]
    trailer = checked_cd_trailer(data)
    return {"profile": profile, "source_sha256": sha256(data), "size": len(data), "tables": tables,
            "tail": {**tail, "raw_hex": data[tail["offset"]:tail["offset"] + tail["length"]].hex(),
                     "classification": "zero_padding_and_checked_cd_trailer"
                     if not any(data[tail["offset"]:-4]) and trailer.get("marker_present") else "uninterpreted"},
            "cd_trailer": trailer}


def _attach_source_renderer_views(inventory: dict) -> None:
    supported = {name: name for name in ("load_string", "set_kanji_string", "sndi", "sndisp",
                                         "printf", "fprintf", "sprintf", "snprintf", "vprintf", "vfprintf",
                                         "vsprintf", "vsnprintf")}
    supported.update(gnsx="set_kanji_string", message_window_set_title="set_kanji_string")
    counts = {}
    for site in inventory["c_literal_sites"]:
        call = site["source_site"].get("call")
        if call not in supported:
            continue
        expected_argument = FORMAT_ARGUMENT_INDEX.get(call) if call in FORMAT_ARGUMENT_INDEX else DISPLAY_ARGUMENT_INDEX[call]
        if site["source_site"].get("call_argument_index") != expected_argument:
            site["renderer_view_status"] = "not_a_proven_text_or_format_argument"
            continue
        renderer = supported[call]
        raw = bytes.fromhex(site["raw_hex"])
        if b"\0" in raw:
            site["renderer_view_status"] = "embedded_nul_requires_sized_consumer_evidence"
            continue
        site["renderer_view"] = tokenize_fixed_text(raw, renderer)
        site["renderer_view"]["consumer"] = call
        counts[renderer] = counts.get(renderer, 0) + 1
    inventory["renderer_view_counts"] = counts


def _attach_typed_dictionary_links(catalog: dict) -> None:
    inventory = catalog.get("source_inventory")
    if inventory is None:
        return
    by_location = {}
    for record in inventory["records"]:
        source = record["source"]
        by_location.setdefault((source["path"], source["offset"]), []).append(record)
    roles = {"parameter_names": "parameter_label", "place_names": "place", "club_names": "name",
             "girl_full_names": "name", "girl_short_names": "name", "hiragana_grid": "character_grid",
             "katakana_grid": "character_grid", "reading_labels": "reading_label",
             "preset_family_names": "name", "preset_given_names": "name", "preset_nicknames": "name"}
    links = 0
    for dictionary in catalog.get("dictionaries", []):
        for item in dictionary.get("records", []):
            body = bytes.fromhex(item["text"]["raw_hex"])
            matches = []
            for record in by_location.get((item["source_path"], item["offset"]), []):
                raw = bytes.fromhex(record["raw_hex"])
                if record["source"]["sha256"] != item["source_sha256"] or raw not in (body, body + b"\0"):
                    continue
                matches.append(record["id"])
                record.setdefault("typed_dictionary_contexts", []).append({
                    "record_id": item["id"], "table": item["table"], "index": item["index"],
                    "source_evidence": dictionary["profile"]["evidence"],
                    "proof": "source_typed_table_same_file_hash_address_and_exact_string_bytes"})
                if record["category"] == "unknown":
                    record["category"] = roles[item["table"]]
                    record["category_basis"] = "source-backed typed dictionary table"
                links += 1
            item["source_inventory_record_ids"] = sorted(matches)
    inventory["typed_dictionary_link_count"] = links


def _attach_storage_overlap_metadata(inventory: dict) -> None:
    active = []
    previous_source = None
    pairs = 0
    for record in sorted(inventory["records"], key=lambda r: (r["source"]["path"], r["source"]["sha256"],
                                                              r["source"]["offset"], r["id"])):
        source = record["source"]
        path, start = source["path"], source["offset"]
        identity = (path, source["sha256"])
        end = start + record["raw_length"]
        active = [] if identity != previous_source else [other for other in active
                   if other["source"]["offset"] + other["raw_length"] > start]
        previous_source = identity
        for other in active:
            shared_end = min(end, other["source"]["offset"] + other["raw_length"])
            if shared_end <= start:
                continue
            record.setdefault("storage_overlaps", []).append({"record_id": other["id"], "offset": start,
                                                              "length": shared_end - start})
            other.setdefault("storage_overlaps", []).append({"record_id": record["id"], "offset": start,
                                                             "length": shared_end - start})
            pairs += 1
        active.append(record)
    inventory["storage_overlap_pairs"] = pairs


def export_text(input_dir, output_dir=None, profiles=None, *, include_source=False, source_repository=None) -> dict:
    """Parse an extracted ISO root, optionally write catalog.json under the cache.

    No schema changes or engine packages are required by API consumers. The input
    directory corresponds to disc/files, so paths start with CDROM/.
    """
    root = Path(input_dir).resolve(strict=True)
    if not root.is_dir():
        raise TextFormatError("input must be an extracted directory")
    selected = list(PROFILES if profiles is None else profiles)
    by_path = {}
    for profile in selected:
        path = source_path(profile["path"])
        if path in by_path:
            raise TextFormatError("duplicate source path in profiles")
        by_path[path] = profile
    numeric = {p["path"]: p for p in NON_TEXT_PROFILES} if profiles is None else {}
    character_tables = {NAME_KAN_PROFILE["path"]: NAME_KAN_PROFILE} if profiles is None else {}
    paths = set(by_path) | set(numeric) | set(character_tables)
    for directory in ("SERIFU", "BUNKA"):
        folder = root / "CDROM" / directory
        if folder.is_dir():
            paths.update(p.relative_to(root).as_posix() for p in folder.rglob("*") if p.is_file())
    files = []
    for path in sorted(paths):
        src = root / path
        if not src.exists():
            files.append({"path": path, "status": "missing", "reason": "profile_input_not_present"})
            continue
        resolved = _input_file(root, path)
        data = resolved.read_bytes()
        if path in character_tables:
            files.append({"path": path, "status": "exported_table",
                          "character_lists": parse_character_lists(data, character_tables[path])})
            continue
        if path in numeric:
            files.append({"path": path, "status": "non_text", "reason": "source_typed_numeric_choice_results",
                          "numeric_data": _numeric_catalog(data, numeric[path])})
            continue
        if path not in by_path:
            files.append({"path": path, "size": len(data), "sha256": sha256(data),
                          "status": "unsupported", "reason": "no_verified_linked_table_layout"})
            continue
        parsed = parse_script(data, path, by_path[path])
        validation = validate_script(data, path, by_path[path], parsed)
        files.append({"path": path, "status": "exported", "script": parsed, "validation": validation})
    catalog = {"schema": "tokimemo.text.catalog.v1", "files": files,
               "coverage": {"exported_files": sum(f["status"] == "exported" for f in files),
                            "unsupported_files": sum(f["status"] == "unsupported" for f in files),
                            "missing_files": sum(f["status"] == "missing" for f in files)},
               "deferred": ["runtime_user_supplied_values", "translation_and_rebuild",
                            "bitmap_lettering_inventory_owned_by_media_lane"]}
    catalog["non_text_files"] = sum(f["status"] == "non_text" for f in files)
    catalog["character_table_files"] = sum(f["status"] == "exported_table" for f in files)
    catalog["text_completeness"] = {"complete": False, "scope": "source_backed_catalog_in_progress",
                                     "reason": "source/reference classification and runtime rendering audit are required"}
    exe = root / "SLPM_86.053"
    if exe.is_file() and profiles is None:
        catalog["font"] = font_metadata(_input_file(root, "SLPM_86.053").read_bytes())
    if profiles is None:
        catalog["dictionaries"] = []
        for dictionary_profile in DICTIONARY_PROFILES:
            path = dictionary_profile["path"]
            if (root / path).is_file():
                catalog["dictionaries"].append(parse_dictionary(_input_file(root, path).read_bytes(), dictionary_profile))
            else:
                catalog["dictionaries"].append({"profile": dictionary_profile, "status": "source_missing"})
        if "font" in catalog:
            attach_font_references(catalog, catalog["font"])
    if include_source:
        from .text_source import inventory_source_text
        from .text_context import enrich_source_context
        from .text_binary import enrich_binary_text
        from .script_mutations import enrich_script_mutations
        source_repo = REPOSITORY if source_repository is None else Path(source_repository).resolve(strict=True)
        catalog["source_inventory"] = inventory_source_text(source_repo, root)
        enrich_source_context(catalog["source_inventory"], source_repo, root, use_ast=True)
        snapshot_manifest = source_repo / "snapshot-manifest.json"
        if snapshot_manifest.is_file():
            metadata = json.loads(snapshot_manifest.read_text(encoding="utf-8"))
            catalog["source_evidence_snapshot"] = {key: metadata[key] for key in
                                                    ("schema", "source_tree_sha256", "stable_copy_verified")}
        _attach_source_renderer_views(catalog["source_inventory"])
        _attach_typed_dictionary_links(catalog)
        # The supplement derives fresh MIPS arguments itself before reading data.
        enrich_binary_text(catalog["source_inventory"], source_repo, root)
        enrich_script_mutations(catalog, source_repo, root)
        _attach_storage_overlap_metadata(catalog["source_inventory"])
        categories = {}
        for record in catalog["source_inventory"]["records"]:
            categories[record["category"]] = categories.get(record["category"], 0) + 1
        catalog["source_inventory"]["resolved_category_counts"] = categories
        supplement = catalog["source_inventory"]["binary_text_supplement"]
        catalog["source_inventory"]["final_inventory_counts"] = {
            "binary_extents": len(catalog["source_inventory"]["records"]),
            "supplemental_extents_added": supplement["records_added"],
            "records_with_mips_context": sum(bool(record.get("mips_contexts"))
                                             for record in catalog["source_inventory"]["records"])}
        catalog["text_completeness"].update(
            source_literal_byte_blockers=len(catalog["source_inventory"]["blocked"]),
            source_context_blockers=len(catalog["source_inventory"]["source_context"]["blocked"]),
            source_mips_blockers=len(catalog["source_inventory"]["mips_context"]["blocked"]),
            mips_targets_outside_initial_inventory=supplement["calls_considered"],
            unresolved_mips_constant_targets=sum(item["status"] != "recovered_verified_label_initial_string"
                                                  for item in supplement["resolutions"]),
            source_binary_supplement_blockers=len(supplement["blocked"]),
            runtime_mutation_blockers=len(catalog["runtime_text_mutations"]["blocked"]),
            unresolved_runtime_write_candidates=len(catalog["runtime_text_mutations"]["unresolved_candidate_ids"]),
            unresolved_semantic_record_count=categories.get("unknown", 0),
            unresolved_semantics_block_completion=True)
        if "font" in catalog:
            attach_font_references(catalog, catalog["font"])
    else:
        catalog["deferred"].append("source_inventory_not_requested")
    if output_dir is not None:
        output = Path(output_dir).resolve()
        # Keep the allowed root lexical: resolving a cache-directory symlink
        # must not make a destination outside the repository acceptable.
        allowed = OUTPUT_ROOT
        if not output.is_relative_to(allowed):
            raise TextFormatError("text exports must be under .asset-cache/text/")
        if output.is_relative_to(root) or root.is_relative_to(output):
            raise TextFormatError("output must be separate from the original input directory")
        output.mkdir(parents=True, exist_ok=True)
        target = output / "catalog.json"
        if target.is_symlink():
            raise TextFormatError("catalog output cannot be a symlink")
        target.write_text(json.dumps(catalog, ensure_ascii=False, sort_keys=True, separators=(",", ":")) + "\n",
                          encoding="utf-8")
    return catalog


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input_dir", type=Path)
    parser.add_argument("--output", type=Path, default=OUTPUT_ROOT)
    parser.add_argument("--profiles", type=Path, help="explicit JSON profile list")
    parser.add_argument("--include-source", action="store_true", help="include executable/overlay source-backed text inventory")
    parser.add_argument("--source-repository", type=Path, help="optional stable source-evidence snapshot root")
    args = parser.parse_args(argv)
    try:
        profiles = None if args.profiles is None else json.loads(args.profiles.read_text(encoding="utf-8"))
        catalog = export_text(args.input_dir, args.output, profiles, include_source=args.include_source,
                              source_repository=args.source_repository)
    except (OSError, ValueError, KeyError, TypeError) as exc:
        parser.exit(1, f"text export failed: {exc}\n")
    print(json.dumps(catalog["coverage"], sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
