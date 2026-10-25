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

from .script_profiles import PROFILES, NON_TEXT_PROFILES, RESOURCE_PROFILES
from .script_profiles_master import verify_boundaries
from .text import TextFormatError, parse_script, sha256, source_path
from .text_font import attach_font_references, font_metadata, verify_font_source_proof
from .script_validation import checked_cd_trailer, validate_script
from .text_tables import (DICTIONARY_PROFILES, NAME_KAN_PROFILE, parse_character_lists, parse_dictionary,
                          verify_table_source_proof)
from .text_date_tables import DATE_TABLE_PROFILE, parse_date_tables, verify_date_source_proof
from .text_ui_tables import UI_TABLE_PROFILES, parse_ui_tables, verify_ui_source_proof
from .text_overlay_tables import (OVERLAY_TABLE_PROFILES, parse_overlay_fixed_tables,
                                  verify_profile_source_pins)
from .text_bunka_sd import BUNKA_SD_PROFILE, parse_bunka_sd, _source_files as _bunka_source_files
from .text_fixed import tokenize_fixed_text
from .text_renderers import read_argument_specs

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
    counts = {}
    for site in inventory["c_literal_sites"]:
        call = site["source_site"].get("call")
        specs = [spec for spec in read_argument_specs(call, site.get("link_unit"))
                 if spec["role"] in {"display", "format", "diagnostic"}]
        if not specs:
            continue
        matches = [spec for spec in specs if spec["index"] == site["source_site"].get("call_argument_index")]
        if len(matches) != 1:
            site["renderer_view_status"] = "not_a_proven_text_or_format_argument"
            continue
        spec = matches[0]
        renderer = spec["renderer"]
        site["renderer_status"] = spec.get("renderer_status", "source_verified_call")
        site["renderer_read_conditions"] = list(spec["conditions"])
        if renderer is None:
            site["renderer_view_status"] = "source_read_renderer_not_yet_modeled"
            continue
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
    roles = {"parameter_names": "parameter_label", "place_names": "place", "gift_names": "item_name", "club_names": "name",
             "girl_full_names": "name", "girl_short_names": "name", "hiragana_grid": "character_grid",
             "katakana_grid": "character_grid", "reading_labels": "reading_label",
             "preset_family_names": "name", "preset_given_names": "name", "preset_nicknames": "name"}
    links = 0
    for dictionary in catalog.get("dictionaries", []):
        table_roles = {table["id"]: table.get("role") or roles.get(table["id"])
                       for table in dictionary["profile"].get("tables", [])}
        for item in dictionary.get("records", []):
            if item.get("text") is None:
                continue
            body = bytes.fromhex(item["text"]["raw_hex"])
            matches = []
            for record in by_location.get((item["source_path"], item["offset"]), []):
                raw = bytes.fromhex(record["raw_hex"])
                if record["source"]["sha256"] != item["source_sha256"] or raw not in (body, body + b"\0"):
                    continue
                matches.append(record["id"])
                record.setdefault("typed_dictionary_contexts", []).append({
                    "record_id": item["id"], "table": item["table"], "index": item["index"],
                    "index_context": item.get("context", {}),
                    "source_evidence": dictionary["profile"]["evidence"],
                    "proof": "source_typed_table_same_file_hash_address_and_exact_string_bytes"})
                role = table_roles.get(item["table"]) or roles.get(item["table"])
                if record["category"] == "unknown" and role is not None:
                    record["category"] = role
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


def _attach_stage_storage_links(catalog: dict) -> None:
    inventory = catalog.get("source_inventory")
    if inventory is None:
        return
    locations = {}
    for record in inventory["records"]:
        if "stage_bank_contexts" in record:
            record["stage_bank_contexts"] = []
        if record.get("category_basis") == "source-backed stage bank storage and body boundary":
            if record.get("context_category_basis") == "unique source-proven consumer role" and record.get("context_category"):
                record["category"] = record["context_category"]
                record["category_basis"] = record["context_category_basis"]
            else:
                record["category"] = "unknown"
                record["category_basis"] = "stage bank attribution reset before fresh linking"
        source = record["source"]
        locations.setdefault((source["path"], source["sha256"], source["offset"]), []).append(record)
    links = 0
    for bank in catalog.get("stage_text_banks", []):
        for item in bank.get("records", []):
            item["source_inventory_record_ids"] = []
        if bank.get("status") != "exported":
            continue
        for item in bank["records"]:
            full = bytes.fromhex(item["full_raw_hex"])
            body = bytes.fromhex(item["text"]["raw_hex"])
            linked = []
            for offset, forms, kind in ((item["offset"], (body, body + b"\0"), "body"),
                                        (item["full_offset"], (full, full[:-1]), "protected_prefix_storage")):
                key = (item["source_path"], item["source_sha256"], offset)
                for record in locations.get(key, []):
                    if record["id"] in linked or bytes.fromhex(record["raw_hex"]) not in forms:
                        continue
                    record.setdefault("stage_bank_contexts", []).append({
                        "record_id": item["id"], "kind": kind,
                        "body_relative_offset": item["offset"] - offset,
                        "proof": "source_proved_stage_table_same_file_hash_address_and_exact_bytes",
                        "runtime_reachability_proven": False})
                    if record["category"] == "unknown":
                        record["category"] = "display" if kind == "body" else "structured_stage_storage"
                        record["category_basis"] = "source-backed stage bank storage and body boundary"
                    linked.append(record["id"])
                    links += 1
            item["source_inventory_record_ids"] = sorted(linked)
    inventory["stage_bank_storage_links"] = links


def _attach_symbolic_buffer_bindings(catalog: dict) -> None:
    contract = catalog.get("symbolic_substitution_contract", {})
    bindings = {}
    if contract.get("status") == "verified_source_and_original_image_pins":
        buffers = {item["symbol"]: item for item in contract["buffers"]}
        for branch in contract["marker_branches"]:
            bindings[branch["marker"]] = buffers[branch["buffer_symbol"]]

    def visit(value):
        if isinstance(value, dict):
            if value.get("kind") == "substitution":
                value.pop("source_buffer_binding", None)
                if value.get("code") in bindings:
                    value["source_buffer_binding"] = {
                        **bindings[value["code"]], "runtime_contents": "unknown",
                        "runtime_read_length_proven": False, "capacity_is_translation_limit": False}
            for key, child in list(value.items()):
                if key != "source_buffer_binding":
                    visit(child)
        elif isinstance(value, list):
            for child in value:
                visit(child)
    visit(catalog)


def export_text(input_dir, output_dir=None, profiles=None, *, include_source=False, source_repository=None) -> dict:
    """Parse an extracted ISO root, optionally write catalog.json under the cache.

    No schema changes or engine packages are required by API consumers. The input
    directory corresponds to disc/files, so paths start with CDROM/.
    """
    root = Path(input_dir).resolve(strict=True)
    proof_repository = (REPOSITORY if source_repository is None else Path(source_repository).resolve(strict=True)) \
        if include_source or source_repository is not None else None
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
    resources = {p["path"]: p for p in RESOURCE_PROFILES} if profiles is None else {}
    character_tables = {NAME_KAN_PROFILE["path"]: NAME_KAN_PROFILE} if profiles is None else {}
    paths = set(by_path) | set(numeric) | set(character_tables) | set(resources)
    for directory in ("SERIFU", "BUNKA", "MASTER", "SPEAKTST"):
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
            parsed = parse_character_lists(data, character_tables[path])
            parsed["source_proof"] = (verify_table_source_proof(data, character_tables[path], proof_repository, root)
                                      if proof_repository is not None else
                                      {"status": "pinned_profile_source_files_not_rechecked"})
            files.append({"path": path, "status": "exported_table",
                          "character_lists": parsed})
            continue
        if path in numeric:
            files.append({"path": path, "status": "non_text", "reason": "source_typed_numeric_choice_results",
                          "numeric_data": _numeric_catalog(data, numeric[path])})
            continue
        if path in resources:
            profile = resources[path]
            if sha256(data) != profile["sha256"] or len(data) != profile["size"]:
                raise TextFormatError("actor resource differs from verified original input")
            base = profile["load_address"]
            locations = [(address, 4) for address in profile["actor_resource_roots"]]
            locations.append((profile["actor_halfword_address"], 2))
            if any(address < base or address + size > base + len(data) for address, size in locations):
                raise TextFormatError("actor resource address outside original input")
            files.append({"path": path, "size": len(data), "sha256": sha256(data),
                          "status": "source_resource", "resource": profile,
                          "reason": profile["status"], "whole_file_text_absence_proven": False})
            continue
        if path not in by_path:
            files.append({"path": path, "size": len(data), "sha256": sha256(data),
                          "status": "unsupported", "reason": "no_verified_linked_table_layout"})
            continue
        parsed = parse_script(data, path, by_path[path])
        validation = validate_script(data, path, by_path[path], parsed)
        if profiles is None and path.startswith("CDROM/MASTER/"):
            validation["source_family_boundaries"] = verify_boundaries(data, by_path[path])
        files.append({"path": path, "status": "exported", "script": parsed, "validation": validation})
    catalog = {"schema": "tokimemo.text.catalog.v1", "files": files,
               "coverage": {"exported_files": sum(f["status"] == "exported" for f in files),
                            "unsupported_files": sum(f["status"] == "unsupported" for f in files),
                            "missing_files": sum(f["status"] == "missing" for f in files)},
               "deferred": ["runtime_user_supplied_values", "translation_and_rebuild",
                            "bitmap_lettering_inventory_owned_by_media_lane"]}
    catalog["non_text_files"] = sum(f["status"] == "non_text" for f in files)
    catalog["character_table_files"] = sum(f["status"] == "exported_table" for f in files)
    catalog["source_resource_files"] = sum(f["status"] == "source_resource" for f in files)
    catalog["text_completeness"] = {"complete": False, "scope": "source_backed_catalog_in_progress",
                                     "reason": "source/reference classification and runtime rendering audit are required"}
    exe = root / "SLPM_86.053"
    if exe.is_file() and profiles is None:
        font_data = _input_file(root, "SLPM_86.053").read_bytes()
        catalog["font"] = font_metadata(font_data)
        catalog["font"]["source_proof"] = (verify_font_source_proof(font_data, proof_repository, root)
                                           if proof_repository is not None else
                                           {"status": "pinned_profile_source_files_not_rechecked"})
    if profiles is None:
        catalog["dictionaries"] = []
        for dictionary_profile in DICTIONARY_PROFILES:
            path = dictionary_profile["path"]
            if (root / path).is_file():
                data = _input_file(root, path).read_bytes()
                parsed = parse_dictionary(data, dictionary_profile)
                parsed["source_proof"] = (verify_table_source_proof(data, dictionary_profile, proof_repository, root)
                                           if proof_repository is not None else
                                           {"status": "pinned_profile_source_files_not_rechecked"})
                catalog["dictionaries"].append(parsed)
            else:
                catalog["dictionaries"].append({"profile": dictionary_profile, "status": "source_missing"})
        path = DATE_TABLE_PROFILE["path"]
        if (root / path).is_file():
            data = _input_file(root, path).read_bytes()
            parsed = parse_date_tables(data)
            parsed["source_proof"] = (verify_date_source_proof(data, proof_repository, root)
                                       if proof_repository is not None else
                                       {"status": "pinned_profile_source_files_not_rechecked"})
            catalog["dictionaries"].append(parsed)
        else:
            catalog["dictionaries"].append({"profile": DATE_TABLE_PROFILE, "status": "source_missing"})
        for profile in UI_TABLE_PROFILES:
            path = profile["path"]
            if not (root / path).is_file():
                catalog["dictionaries"].append({"profile": profile, "status": "source_missing"})
                continue
            data = _input_file(root, path).read_bytes()
            proof = (verify_ui_source_proof(data, profile, proof_repository, root)
                     if proof_repository is not None else
                     {"status": "pinned_profile_source_files_not_rechecked"})
            parsed = parse_ui_tables(data, profile)
            parsed["source_proof"] = proof
            catalog["dictionaries"].append(parsed)
        for profile in OVERLAY_TABLE_PROFILES:
            path = profile["path"]
            if (root / path).is_file():
                proof = (verify_profile_source_pins(proof_repository, profile)
                         if proof_repository is not None else
                         {"status": "pinned_profile_source_files_not_rechecked"})
                parsed = parse_overlay_fixed_tables(_input_file(root, path).read_bytes(), profile)
                parsed["source_proof"] = proof
                catalog["dictionaries"].append(parsed)
            else:
                catalog["dictionaries"].append({"profile": profile, "status": "source_missing"})
        catalog["stage_text_banks"] = []
        stage_path = BUNKA_SD_PROFILE["path"]
        if (root / stage_path).is_file():
            data = _input_file(root, stage_path).read_bytes()
            if sha256(data) != BUNKA_SD_PROFILE["sha256"] or len(data) != BUNKA_SD_PROFILE["size"]:
                raise TextFormatError("BUNKA_SD input differs from its source-proved bank layout")
            stage_repo = REPOSITORY if source_repository is None else Path(source_repository).resolve(strict=True)
            try:
                _bunka_source_files(stage_repo, BUNKA_SD_PROFILE)
            except (OSError, ValueError) as exc:
                catalog["stage_text_banks"].append({"profile": BUNKA_SD_PROFILE,
                    "status": "source_evidence_not_verified", "reason": type(exc).__name__})
            else:
                bank = parse_bunka_sd(data, stage_repo)
                bank["status"] = "exported"
                catalog["stage_text_banks"].append(bank)
        else:
            catalog["stage_text_banks"].append({"profile": BUNKA_SD_PROFILE, "status": "source_missing"})
        catalog["text_completeness"]["stage_bank_blockers"] = sum(
            bank.get("status") != "exported" or bool(bank.get("issues")) for bank in catalog["stage_text_banks"])
        if proof_repository is not None:
            catalog["text_completeness"]["fixed_table_source_proof_blockers"] = sum(
                not dictionary.get("source_proof", {}).get("status", "").startswith("verified_")
                for dictionary in catalog["dictionaries"])
        if "font" in catalog:
            attach_font_references(catalog, catalog["font"])
    if include_source:
        from .text_source import inventory_source_text
        from .text_context import enrich_source_context
        from .text_binary import _enrich_binary_text_from_reports
        from .text_mips import enrich_mips_context
        from .script_mutations import enrich_script_mutations
        from .text_renderer_proof import verify_renderer_source_proofs
        from .text_substitutions import verify_symbolic_substitution_contract
        source_repo = REPOSITORY if source_repository is None else Path(source_repository).resolve(strict=True)
        catalog["renderer_source_proofs"] = verify_renderer_source_proofs(source_repo, root)
        try:
            catalog["symbolic_substitution_contract"] = verify_symbolic_substitution_contract(source_repo, root)
        except (OSError, ValueError) as exc:
            catalog["symbolic_substitution_contract"] = {
                "status": "source_contract_not_verified", "reason": type(exc).__name__,
                "runtime_contents_unknown": True, "execution_claim": False}
        catalog["source_inventory"] = inventory_source_text(source_repo, root)
        enrich_source_context(catalog["source_inventory"], source_repo, root, use_ast=True)
        snapshot_manifest = source_repo / "snapshot-manifest.json"
        if snapshot_manifest.is_file():
            manifest_raw = snapshot_manifest.read_bytes()
            metadata = json.loads(manifest_raw)
            catalog["source_evidence_snapshot"] = {key: metadata[key] for key in
                                                    ("schema", "source_tree_sha256", "stable_copy_verified")}
            catalog["source_evidence_snapshot"]["manifest_sha256"] = sha256(manifest_raw)
            for key in ("revision", "build_source_digest", "matching_proof_sha256"):
                if key in metadata:
                    catalog["source_evidence_snapshot"][key] = metadata[key]
        _attach_source_renderer_views(catalog["source_inventory"])
        _attach_typed_dictionary_links(catalog)
        _attach_stage_storage_links(catalog)
        enrich_mips_context(catalog["source_inventory"], source_repo, root)
        # These reports were derived in this invocation; avoid a second AST pass.
        _enrich_binary_text_from_reports(catalog["source_inventory"], source_repo, root)
        enrich_script_mutations(catalog, source_repo, root)
        _attach_storage_overlap_metadata(catalog["source_inventory"])
        categories = {}
        for record in catalog["source_inventory"]["records"]:
            categories[record["category"]] = categories.get(record["category"], 0) + 1
        catalog["source_inventory"]["resolved_category_counts"] = categories
        supplement = catalog["source_inventory"]["binary_text_supplement"]
        writes = catalog["runtime_text_mutations"].get("source_write_inventory", {})
        catalog["source_inventory"]["final_inventory_counts"] = {
            "binary_extents": len(catalog["source_inventory"]["records"]),
            "supplemental_extents_added": supplement["records_added"],
            "records_with_mips_context": sum(bool(record.get("mips_contexts"))
                                             for record in catalog["source_inventory"]["records"])}
        catalog["text_completeness"].update(
            renderer_alias_contract_blockers=int(not catalog["renderer_source_proofs"]["verified"]),
            symbolic_substitution_contract_blockers=int(catalog["symbolic_substitution_contract"]["status"] !=
                                                        "verified_source_and_original_image_pins"),
            source_literal_byte_blockers=len(catalog["source_inventory"]["blocked"]),
            source_context_blockers=len(catalog["source_inventory"]["source_context"]["blocked"]),
            source_mips_blockers=len(catalog["source_inventory"]["mips_context"]["blocked"]),
            mips_targets_outside_initial_inventory=supplement["calls_considered"],
            unresolved_mips_constant_targets=sum(item["status"] != "recovered_verified_label_initial_string"
                                                  for item in supplement["resolutions"]),
            source_binary_supplement_blockers=len(supplement["blocked"]),
            unresolved_pointer_data_label_paths=sum(item["status"] != "recovered_verified_label_initial_string"
                                                     for item in supplement.get("pointer_leaf_resolutions", [])),
            runtime_mutation_blockers=len(catalog["runtime_text_mutations"]["blocked"]),
            unresolved_runtime_write_candidates=len(catalog["runtime_text_mutations"]["unresolved_candidate_ids"]),
            unresolved_opaque_value_dependencies=len(writes.get("opaque_value_dependencies", [])),
            unresolved_address_dependencies=len(writes.get("unmodeled_address_dependencies", [])),
            unresolved_semantic_record_count=categories.get("unknown", 0),
            unresolved_semantics_block_completion=True)
        _attach_symbolic_buffer_bindings(catalog)
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
