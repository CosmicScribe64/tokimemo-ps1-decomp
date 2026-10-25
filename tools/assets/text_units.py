"""Stream compact translation-work projections from a validated text catalog.

The module preserves source identity and evidence while leaving every target
field empty. It is deliberately a projection: renderer limits and conditional
source views remain evidence, never translation limits or generated text.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import io
import json
import os
from pathlib import Path
import tempfile
from typing import Iterable, Iterator


REPOSITORY = Path(__file__).resolve().parents[2]
OUTPUT_ROOT = REPOSITORY / ".asset-cache" / "text"
DICTIONARY_ROLES = {
    "parameter_names": "parameter_label", "place_names": "place", "club_names": "name",
    "girl_full_names": "name", "girl_short_names": "name", "hiragana_grid": "character_grid",
    "katakana_grid": "character_grid", "reading_labels": "reading_label",
    "preset_family_names": "name", "preset_given_names": "name", "preset_nicknames": "name",
    "gift_names": "item_name",
}
FIELDS = (
    "id", "kind", "parent_id", "source_path", "source_sha256", "source_offset",
    "raw_hex", "raw_sha256", "source_text", "target_text", "tokens", "context",
    "readiness",
)


class UnitExportError(ValueError):
    """Invalid catalog, projection, or output location."""


def _json_bytes(value: object) -> bytes:
    return (json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(",", ":")) + "\n").encode("utf-8")


def _canonical_hash(value: object) -> str:
    digest = hashlib.sha256()
    encoder = json.JSONEncoder(ensure_ascii=False, sort_keys=True, separators=(",", ":"))
    for chunk in encoder.iterencode(value):
        digest.update(chunk.encode("utf-8"))
    return digest.hexdigest()


def _strict_text(text_view: dict | None) -> tuple[str | None, str]:
    if not isinstance(text_view, dict):
        return None, "missing_source_view"
    status = text_view.get("source_decode_status", "missing_source_view")
    text = text_view.get("source_decoded_text")
    if status not in ("ok", "roundtrip_verified", "strict_roundtrip") or not isinstance(text, str) or "\ufffd" in text:
        return None, str(status)
    return text, "ok"


def _raw_hash(raw_hex: str | None) -> str | None:
    if raw_hex is None:
        return None
    try:
        raw = bytes.fromhex(raw_hex)
    except (TypeError, ValueError) as exc:
        raise UnitExportError("catalog contains malformed raw_hex") from exc
    return hashlib.sha256(raw).hexdigest()


def _has_embedded_nul(raw_hex: str | None, terminal_nul_excluded: bool = False) -> bool:
    if not raw_hex:
        return False
    try:
        raw = bytes.fromhex(raw_hex)
    except (TypeError, ValueError) as exc:
        raise UnitExportError("catalog contains malformed raw_hex") from exc
    if terminal_nul_excluded and raw.endswith(b"\0"):
        raw = raw[:-1]
    return b"\0" in raw


def _source_payload(record: dict) -> bytes:
    """Return bytes before a terminator only when the catalog marks it as excluded."""
    try:
        raw = bytes.fromhex(record.get("raw_hex", ""))
    except (TypeError, ValueError) as exc:
        raise UnitExportError("catalog contains malformed raw_hex") from exc
    if record.get("source_terminal_nul_excluded") is True and raw.endswith(b"\0"):
        return raw[:-1]
    return raw


def _proven_function_extent(site: dict, records_by_id: dict) -> dict | None:
    if site.get("function_reference_status") != "unique_same_function_label_and_exact_bytes":
        return None
    candidates = site.get("function_binary_candidates", [])
    if len(candidates) != 1:
        return None
    record = records_by_id.get(candidates[0])
    if not record or not str(record.get("extent_status", "")).startswith("verified"):
        return None
    try:
        literal = bytes.fromhex(site.get("raw_hex", ""))
    except (TypeError, ValueError) as exc:
        raise UnitExportError("catalog contains malformed C-literal raw_hex") from exc
    return record if literal == _source_payload(record) else None


def _renderer_views_by_extent(catalog: dict, records_by_id: dict) -> dict[str, list[dict]]:
    views: dict[str, list[dict]] = {}
    for dictionary in catalog.get("dictionaries", []):
        for item in dictionary.get("records", []):
            text_view = item.get("text") or {}
            for record_id in item.get("source_inventory_record_ids", []):
                record = records_by_id.get(record_id)
                provenance = (record or {}).get("source") or {}
                if (record and item.get("source_path") == provenance.get("path") and
                        item.get("source_sha256") == provenance.get("sha256") and
                        item.get("offset") == provenance.get("offset") and
                        bytes.fromhex(text_view.get("raw_hex", "")) == _source_payload(record)):
                    views.setdefault(record_id, []).append({
                        "renderer": text_view.get("renderer"), "tokens": text_view.get("tokens", []),
                        "source": "typed_dictionary_link", "table": item.get("table"),
                    })
    for record in (catalog.get("source_inventory") or {}).get("records", []):
        for initial in record.get("binary_initial_renderer_views", []):
            view = initial.get("view") or {}
            if isinstance(view, dict):
                views.setdefault(record.get("id"), []).append({
                    "renderer": initial.get("renderer"), "tokens": view.get("tokens", []),
                    "source": "verified_binary_initial_view", "call_pc": initial.get("call_pc"),
                    "source_leaf_id": initial.get("source_leaf_id"),
                    "initial_view_only": record.get("initial_view_only", True),
                    "runtime_read_length_proven": initial.get("runtime_read_length_proven", False),
                    "selection_certainty": initial.get("selection_certainty"),
                })
    for site in (catalog.get("source_inventory") or {}).get("c_literal_sites", []):
        extent = _proven_function_extent(site, records_by_id)
        view = site.get("renderer_view")
        if extent and isinstance(view, dict):
            views.setdefault(extent["id"], []).append({
                "renderer": view.get("renderer", view.get("consumer")),
                "tokens": view.get("tokens", []), "source": "unique_function_label_and_exact_bytes",
                "callee_read_conditions": site.get("renderer_read_conditions", []),
                "call": site.get("source_site", {}).get("call"),
            })
    return views


def _view_tokens(views: list[dict]) -> tuple[list, str | None]:
    if not views:
        return [], "renderer_tokenization_unavailable"
    signatures = {json.dumps([view.get("renderer"), view.get("tokens", [])],
                             sort_keys=True, separators=(",", ":")) for view in views}
    if len(signatures) != 1:
        return [], "renderer_view_ambiguous"
    return views[0].get("tokens", []), None


def _tokenization_blockers(tokens: list) -> list[str]:
    unsupported = {"unknown", "unknown_source_bytes", "unknown_format", "unknown_renderer_bytes",
                   "renderer_rejected_pair", "unmapped_numeric_glyph", "not_rendered_after_rejected_pair"}
    if any(isinstance(token, dict) and token.get("kind") in unsupported for token in tokens):
        return ["renderer_tokens_contain_unsupported_or_unmapped_units"]
    return []


def _inline_label_segments(text_view: dict) -> list[dict]:
    label = text_view.get("speaker_label")
    if not isinstance(label, dict) or label.get("kind") != "inline_label":
        return []
    marker = label.get("marker")
    label_text = label.get("label_text")
    return [
        {"kind": "protected_marker", "source_text": marker, "raw_hex": (label.get("raw_hex") or "")[:2]},
        {"kind": "translatable_inline_label", "source_text": label_text,
         "raw_hex": label.get("label_raw_hex"), "font_references": label.get("label_font_references", []),
         "status": "source_label_text" if isinstance(label_text, str) else "unknown_label_glyphs"},
    ]


def _is_zero_source_padding(record: dict) -> bool:
    raw_hex = record.get("raw_hex", "")
    try:
        raw = bytes.fromhex(raw_hex) if raw_hex else b""
    except (TypeError, ValueError) as exc:
        raise UnitExportError("catalog contains malformed raw_hex") from exc
    if record.get("raw_length", len(raw)) == 0:
        return True
    return bool(raw) and not any(raw) and record.get("context_category") == "empty_extent_no_translatable_bytes"


def _ready(blockers: list[str], *, eligible: bool = True) -> dict:
    unique = sorted(set(blockers))
    return {"status": "ready_for_review" if eligible and not unique else "flagged",
            "translation_ready": bool(eligible and not unique), "blockers": unique}


def _row(*, identifier, kind, parent_id=None, source_path=None, source_sha256=None,
         source_offset=None, raw_hex=None, source_text=None, tokens=None, context=None,
         blockers=(), eligible=True) -> dict:
    blockers = list(blockers)
    if identifier is None:
        identity_material = {"kind": kind, "path": source_path, "sha256": source_sha256,
                             "offset": source_offset, "parent_id": parent_id, "context": context}
        identifier = "unresolved-locator:" + hashlib.sha256(_json_bytes(identity_material)).hexdigest()[:24]
        blockers.append("stable_source_identity_unresolved")
    return {
        "id": identifier,
        "kind": kind,
        "parent_id": parent_id,
        "source_path": source_path,
        "source_sha256": source_sha256,
        "source_offset": source_offset,
        "raw_hex": raw_hex,
        "raw_sha256": _raw_hash(raw_hex),
        "source_text": source_text,
        "target_text": None,
        "tokens": tokens if tokens is not None else [],
        "context": context if context is not None else {},
        "readiness": _ready(list(blockers), eligible=eligible),
    }


def _iter_units(catalog: dict) -> Iterator[dict]:
    """Yield one deterministic compact row at a time from an exported catalog."""
    if not isinstance(catalog, dict) or catalog.get("schema") != "tokimemo.text.catalog.v1":
        raise UnitExportError("expected a tokimemo.text.catalog.v1 catalog")

    for file_entry in catalog.get("files", []):
        script = file_entry.get("script")
        if not isinstance(script, dict):
            continue
        validation = file_entry.get("validation") or {}
        file_blockers = []
        if file_entry.get("status") not in ("exported", "exported_table"):
            file_blockers.append("container_not_exported")
        if validation.get("structural_checks_pass") is not True:
            file_blockers.append("script_validation_not_passed")
        if validation.get("strict_source_decode_failures", 0):
            file_blockers.append("container_has_decode_failures")
        anomaly_groups = validation.get("renderer_anomalies", [])
        for record in script.get("records", []):
            text_view = record.get("text")
            if text_view is None:
                continue
            source_text, decode_status = _strict_text(text_view)
            blockers = list(file_blockers)
            if anomaly_groups:
                blockers.append("container_has_renderer_anomalies")
            if decode_status != "ok":
                blockers.append("source_decode_" + decode_status)
            elif source_text == "":
                blockers.append("empty_source_slot")
            if _has_embedded_nul(text_view.get("raw_hex")):
                blockers.append("embedded_nul_extent")
            if record.get("unsupported"):
                blockers.append("unsupported_script_semantics")
            if text_view and text_view.get("unknown_token_count", 0):
                blockers.append("unknown_control_or_glyph_tokens")
            blockers.extend(_tokenization_blockers((text_view or {}).get("tokens", [])))
            context = {
                "profile_id": script.get("profile_id"),
                "section": record.get("section"), "group": record.get("group"),
                "line": record.get("line"), "command": record.get("command"),
                "speaker": record.get("speaker"),
                "original_copy_limits": text_view.get("original_copy_limits") if text_view else None,
                "translation_segments": _inline_label_segments(text_view),
                "renderer_anomaly_present": bool(anomaly_groups),
                "renderer_limits_are_provenance_only": True,
            }
            yield _row(
                identifier=record.get("id"), kind="script_record", source_path=record.get("source_path"),
                source_sha256=record.get("source_sha256"),
                source_offset=text_view.get("offset"),
                raw_hex=text_view.get("raw_hex"),
                source_text=source_text,
                tokens=text_view.get("tokens", []), context=context,
                blockers=blockers,
            )

    for dictionary in catalog.get("dictionaries", []):
        profile = dictionary.get("profile") or {}
        table_roles = {table.get("id"): table.get("role") or DICTIONARY_ROLES.get(table.get("id"))
                       for table in profile.get("tables", [])}
        for item in dictionary.get("records", []):
            text_view = item.get("text")
            source_text, decode_status = _strict_text(text_view)
            blockers = []
            if decode_status != "ok":
                blockers.append("source_decode_" + decode_status)
            if source_text == "":
                blockers.append("empty_dictionary_slot")
            if text_view and _has_embedded_nul(text_view.get("raw_hex")):
                blockers.append("embedded_nul_extent")
            if text_view and text_view.get("unknown_token_count", 0):
                blockers.append("unknown_control_or_glyph_tokens")
            blockers.extend(_tokenization_blockers((text_view or {}).get("tokens", [])))
            contexts = item.get("typed_dictionary_contexts", [])
            role = table_roles.get(item.get("table"))
            if not role and not contexts:
                blockers.append("dictionary_role_unclassified")
            yield _row(
                identifier=item.get("id"), kind="dictionary_slot", source_path=item.get("source_path"),
                source_sha256=item.get("source_sha256"), source_offset=item.get("offset"),
                raw_hex=(text_view.get("raw_hex") if text_view else
                         item.get("content_raw_hex", item.get("slot_raw_hex"))), source_text=source_text,
                tokens=text_view.get("tokens", []) if text_view else [],
                context={"table": item.get("table"), "index": item.get("index"),
                         "renderer": (text_view or {}).get("renderer"),
                         "renderer_unsupported": (text_view or {}).get("unsupported"),
                         "original_slot_capacity_is_translation_limit": False,
                         "source_table_context": item.get("context", {}),
                         "source_slot_status": item.get("source_slot_status"),
                         "string_extent_status": item.get("string_extent_status"),
                         "content_raw_hex": item.get("content_raw_hex"),
                         "table_role": role, "typed_contexts": contexts,
                         "source_context": item.get("context"),
                         "slot_offset": item.get("slot_offset"),
                         "slot_raw_hex": item.get("slot_raw_hex"),
                         "empty_slot": item.get("length") == 0},
                blockers=blockers,
            )

    inventory = catalog.get("source_inventory") or {}
    source_records = inventory.get("records", [])
    records_by_id = {record.get("id"): record for record in source_records if record.get("id")}
    renderer_views = _renderer_views_by_extent(catalog, records_by_id)
    for record in source_records:
        raw_hex = record.get("raw_hex")
        raw_length = record.get("raw_length", 0)
        if _is_zero_source_padding(record):
            continue
        source_text = record.get("source_decoded_text")
        decode_status = record.get("source_decode_status", "missing_source_view")
        blockers = []
        if decode_status != "ok" or not isinstance(source_text, str) or "\ufffd" in source_text:
            source_text = None
            blockers.append("source_decode_" + str(decode_status))
        if _has_embedded_nul(raw_hex, record.get("source_terminal_nul_excluded") is True):
            blockers.append("embedded_nul_extent")
        reference_status = str(record.get("reference_status", ""))
        if "ambiguous" in reference_status or "unconfirmed" in reference_status:
            blockers.append("consumer_reference_unresolved_or_ambiguous")
        if record.get("context_category") in (None, "unknown", "unclassified"):
            if record.get("category_basis") != "source-backed typed dictionary table":
                blockers.append("consumer_context_unclassified")
        proven_basis = record.get("context_category_basis") == "unique source-proven consumer role"
        typed_dictionary_basis = record.get("category_basis") == "source-backed typed dictionary table"
        if not (proven_basis or typed_dictionary_basis):
            blockers.append("consumer_context_not_source_proven")
        if record.get("source_roundtrip") is not True:
            blockers.append("source_roundtrip_unverified")
        if not str(record.get("extent_status", "")).startswith("verified"):
            blockers.append("source_extent_unverified")
        provenance = record.get("source") or {}
        identifier = record.get("id")
        if not identifier:
            blockers.append("stable_source_identity_unresolved")
            identifier = "unresolved-source-extent:" + hashlib.sha256(_json_bytes({
                "path": provenance.get("path"), "offset": provenance.get("offset"),
                "raw_sha256": _raw_hash(raw_hex),
            })).hexdigest()[:24]
        tokens, renderer_blocker = _view_tokens(renderer_views.get(record.get("id"), []))
        if renderer_blocker:
            blockers.append(renderer_blocker)
        if any(view.get("callee_read_conditions") for view in renderer_views.get(record.get("id"), [])):
            blockers.append("conditional_callee_text_read")
        blockers.extend(_tokenization_blockers(tokens))
        if any(view.get("runtime_read_length_proven") is False and
               view.get("source") == "verified_binary_initial_view"
               for view in renderer_views.get(record.get("id"), [])):
            blockers.append("runtime_read_length_unproven")
        yield _row(
            identifier=identifier, kind="source_extent", source_path=provenance.get("path"),
            source_sha256=provenance.get("sha256"), source_offset=provenance.get("offset"),
            raw_hex=raw_hex, source_text=source_text, tokens=tokens,
            context={"source_record_id": record.get("id"),
                     "identity_basis": "verified_source_extent_id" if record.get("id") else "unresolved_extent_locator",
                     "stable_id_annotation": record.get("stable_id_status"),
                     "category": record.get("category"), "category_basis": record.get("category_basis"),
                     "context_category": record.get("context_category"),
                     "context_category_basis": record.get("context_category_basis"),
                     "extent_status": record.get("extent_status"),
                     "reference_status": record.get("reference_status"),
                     "source_terminal_nul_excluded": record.get("source_terminal_nul_excluded"),
                     "initial_view_only": record.get("initial_view_only"),
                     "runtime_read_length_proven": record.get("runtime_read_length_proven"),
                     "terminator": record.get("terminator"),
                     "source_contexts": record.get("source_contexts", []),
                     "stage_bank_contexts": record.get("stage_bank_contexts", []),
                     "mips_contexts": record.get("mips_contexts", []),
                     "renderer_views": renderer_views.get(record.get("id"), [])},
            blockers=blockers,
        )

    for site in inventory.get("c_literal_sites", []):
        raw_hex = site.get("raw_hex")
        raw_length = site.get("raw_length", 0)
        if raw_length == 0:
            continue
        source_text = site.get("source_decoded_text")
        blockers = []
        if site.get("source_decode_status") != "ok" or not isinstance(source_text, str) or "\ufffd" in source_text:
            source_text = None
            blockers.append("source_decode_unverified")
        original_extent = _proven_function_extent(site, records_by_id)
        if original_extent is None:
            blockers.append("no_verified_original_extent")
        if _has_embedded_nul(raw_hex, site.get("source_terminal_nul_excluded") is True):
            blockers.append("embedded_nul_extent")
        if site.get("category") in (None, "unknown", "unclassified"):
            blockers.append("consumer_context_unclassified")
        if site.get("renderer_view", {}).get("unknown_token_count", 0):
            blockers.append("unknown_control_or_glyph_tokens")
        blockers.extend(_tokenization_blockers((site.get("renderer_view") or {}).get("tokens", [])))
        if (site.get("source_site", {}).get("call") and site.get("category") in ("display", "debug_or_format")
                and not site.get("renderer_view")):
            blockers.append("renderer_view_missing_or_unresolved")
        if site.get("source_site", {}).get("call") and site.get("renderer_view_status") not in (None, "tokenized"):
            blockers.append("renderer_view_anomaly_or_unsupported_call")
        if site.get("renderer_read_conditions"):
            blockers.append("conditional_callee_text_read")
        if site.get("stable_id_status") != "verified_matching_function_entry_and_local_ordinal":
            blockers.append("stable_source_identity_unresolved")
        source_site = site.get("source_site", {})
        if not source_site.get("call") and site.get("category") in (None, "unknown", "unclassified"):
            blockers.append("consumer_context_unclassified")
        if "candidate" in str(site.get("category_basis", "")) or "ambiguous" in str(site.get("category_basis", "")):
            blockers.append("semantic_category_is_candidate_or_ambiguous")
        original_source = (original_extent or {}).get("source") or {}
        identifier = site.get("stable_id")
        if identifier is None:
            identifier = "unresolved-c-locator:" + str(site.get("source_locator_id") or site.get("id") or "missing-locator")
        yield _row(
            identifier=identifier, kind="source_literal",
            parent_id=(original_extent or {}).get("id"),
            source_path=original_source.get("path", source_site.get("path")),
            source_sha256=original_source.get("sha256"), source_offset=original_source.get("offset"),
            raw_hex=raw_hex, source_text=source_text,
            tokens=(site.get("renderer_view") or {}).get("tokens", []),
            context={"source_locator_id": site.get("source_locator_id"),
                     "link_unit": site.get("link_unit"), "category": site.get("category"),
                     "category_basis": site.get("category_basis"), "call": source_site.get("call"),
                     "call_argument_index": source_site.get("call_argument_index"),
                     "source_byte_offset": (source_site.get("byte_offset")
                                             if source_site.get("byte_offset_basis") == "utf8_bytes" else None),
                     "source_character_offset": source_site.get("character_offset", source_site.get("byte_offset")),
                     "source_offset_basis": source_site.get("byte_offset_basis", "legacy_character_index"),
                     "source_line": source_site.get("line"),
                     "source_column": source_site.get("column"),
                     "source_function": source_site.get("function"),
                     "original_extent_id": (original_extent or {}).get("id"),
                     "identity_status": site.get("stable_id_status"),
                     "renderer_view_status": site.get("renderer_view_status"),
                     "renderer_read_conditions": site.get("renderer_read_conditions", []),
                     "binary_status": site.get("binary_status"),
                     "binary_candidate_status": site.get("binary_candidate_status")},
            blockers=blockers,
        )

    for bank in catalog.get("stage_text_banks", []):
        if bank.get("status") != "exported":
            continue
        for record in bank.get("records", []):
            view = record.get("text")
            source_text, decode_status = _strict_text(view)
            blockers = []
            if decode_status != "ok":
                blockers.append("source_decode_" + decode_status)
            if source_text == "" or record.get("source_slot_status") == "empty":
                blockers.append("empty_source_slot")
            if record.get("source_slot_status") == "whitespace_only":
                blockers.append("whitespace_only_source_slot")
            if bank.get("issues"):
                blockers.append("stage_bank_has_unresolved_issues")
            blockers.extend(_tokenization_blockers((view or {}).get("tokens", [])))
            yield _row(
                identifier=record.get("id"), kind="stage_text_record",
                parent_id=record.get("source_physical_record_id"),
                source_path=record.get("source_path"), source_sha256=record.get("source_sha256"),
                source_offset=record.get("offset"), raw_hex=(view or {}).get("raw_hex"),
                source_text=source_text, tokens=(view or {}).get("tokens", []),
                context={"table": record.get("table"), "index": record.get("index"),
                         "source_slot_status": record.get("source_slot_status"),
                         "stage_context": record.get("context", {}),
                         "full_offset": record.get("full_offset"), "full_raw_hex": record.get("full_raw_hex"),
                         "full_tokens": record.get("full_tokens", []),
                         "terminator_offset": record.get("terminator_offset"),
                         "source_inventory_record_ids": record.get("source_inventory_record_ids", []),
                         "runtime_reachability_proven": False,
                         "renderer_limits_are_provenance_only": True},
                blockers=blockers)

    for rule in (catalog.get("runtime_text_mutations") or {}).get("rules", []):
        for target in rule.get("targets", []):
            view = target.get("conditional_runtime_view")
            if not isinstance(view, dict):
                continue
            text_view = view.get("text")
            source_text, decode_status = _strict_text(text_view)
            blockers = ["conditional_runtime_variant"]
            if decode_status != "ok":
                blockers.append("source_decode_" + decode_status)
            if rule.get("selection_status") == "source_assignment_union_requires_runtime_selector_proof":
                blockers.append("conditional_route_not_proven")
            if view.get("conditions"):
                blockers.append("runtime_condition_requires_context")
            if source_text == "":
                blockers.append("empty_conditional_variant")
            if text_view:
                blockers.extend(_tokenization_blockers(text_view.get("tokens", [])))
            parent_id = view.get("source_record_id") or target.get("record_id")
            identifier = None
            if rule.get("id") and parent_id is not None:
                identifier = "runtime:" + str(rule["id"]) + ":" + str(parent_id) + ":" + str(target.get("relative_byte_offset", 0))
            else:
                blockers.append("stable_variant_identity_unresolved")
                identifier = "unresolved-runtime-locator:" + hashlib.sha256(_json_bytes({
                    "rule": rule.get("id"), "path": target.get("source_path"),
                    "sha256": target.get("source_sha256"), "offset": target.get("offset"),
                    "relative_byte_offset": target.get("relative_byte_offset"),
                })).hexdigest()[:24]
            yield _row(
                identifier=identifier, kind="conditional_runtime_variant", parent_id=parent_id,
                source_path=target.get("source_path"), source_sha256=target.get("source_sha256"),
                source_offset=target.get("offset"), raw_hex=text_view.get("raw_hex") if text_view else None,
                source_text=source_text, tokens=text_view.get("tokens", []) if text_view else [],
                context={"rule_id": rule.get("id"), "conditions": view.get("conditions", []),
                         "selection_status": rule.get("selection_status"),
                         "derived_view_status": view.get("status"),
                         "source_storage_unchanged": view.get("source_storage_unchanged"),
                         "ignored_storage_after_nul_hex": view.get("ignored_storage_after_nul_hex"),
                         "original_copy_limits": text_view.get("original_copy_limits") if text_view else None,
                         "renderer_limits_are_provenance_only": True},
                blockers=blockers,
            )


def iter_units(catalog: dict) -> Iterator[dict]:
    """Yield rows while enforcing non-null, unique projection identities."""
    seen = set()
    for row in _iter_units(catalog):
        identifier = row.get("id")
        if not isinstance(identifier, str) or not identifier:
            raise UnitExportError("projection row has no identity")
        if identifier in seen:
            raise UnitExportError("projection row identities are not unique")
        seen.add(identifier)
        yield row


def _safe_output_dir(path: str | Path, repository: str | Path) -> Path:
    repo = Path(repository).absolute()
    allowed = repo / ".asset-cache" / "text"
    out = Path(path).absolute()
    if not out.is_relative_to(allowed):
        raise UnitExportError("output must be under .asset-cache/text")
    for p in (repo / ".asset-cache", allowed, *out.parents):
        if p.exists() and p.is_symlink():
            raise UnitExportError("output path contains a symlink")
    if not out.resolve().is_relative_to(allowed.resolve()):
        raise UnitExportError("output path escapes .asset-cache/text")
    return out


def _write_jsonl(path: Path, rows: Iterable[dict]) -> tuple[int, str, dict]:
    counts: dict[str, int] = {}
    digest = hashlib.sha256()
    count = 0
    with path.open("wb") as stream:
        for row in rows:
            data = _json_bytes(row)
            stream.write(data)
            digest.update(data)
            count += 1
            counts[row["kind"]] = counts.get(row["kind"], 0) + 1
    return count, digest.hexdigest(), counts


def _write_tsv(path: Path, rows: Iterable[dict]) -> tuple[int, str, dict]:
    counts: dict[str, int] = {}
    digest = hashlib.sha256()
    count = 0
    with path.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        header = list(FIELDS)
        writer.writerow(header)
        digest.update(("\t".join(header) + "\n").encode("utf-8"))
        for row in rows:
            values = []
            for field in FIELDS:
                value = row.get(field)
                if field == "source_text":
                    value = json.dumps(value, ensure_ascii=False)
                elif isinstance(value, (dict, list)) or value is None:
                    value = json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(",", ":"))
                else:
                    value = str(value)
                values.append(value)
            # CSV quoting protects structured columns; source_text is already a JSON string.
            buffer = io.StringIO(newline="")
            csv.writer(buffer, delimiter="\t", lineterminator="\n").writerow(values)
            data = buffer.getvalue().encode("utf-8")
            stream.write(data.decode("utf-8"))
            digest.update(data)
            count += 1
            counts[row["kind"]] = counts.get(row["kind"], 0) + 1
    return count, digest.hexdigest(), counts


def export_units(catalog: dict | str | Path, output_dir: str | Path | None = None,
                 *, repository: str | Path = REPOSITORY, formats: tuple[str, ...] = ("jsonl", "tsv")) -> dict:
    """Write deterministic JSONL/TSV projections and a numeric manifest."""
    source_sha256 = None
    if isinstance(catalog, (str, Path)):
        raw = Path(catalog).absolute().read_bytes()
        source_sha256 = hashlib.sha256(raw).hexdigest()
        catalog = json.loads(raw)
    elif isinstance(catalog, dict):
        source_sha256 = _canonical_hash(catalog)
    if not formats or any(fmt not in ("jsonl", "tsv") for fmt in formats):
        raise UnitExportError("formats must contain jsonl and/or tsv")
    if output_dir is None:
        output_dir = Path(repository) / ".asset-cache" / "text" / "units"
    output = _safe_output_dir(output_dir, repository)
    output.mkdir(parents=True, exist_ok=True)
    if output.is_symlink():
        raise UnitExportError("output directory cannot be a symlink")
    files = {}
    counts = {}
    row_count = None
    for fmt in formats:
        target = output / ("units." + fmt)
        if target.is_symlink():
            raise UnitExportError("projection file cannot be a symlink")
        fd, tmp_name = tempfile.mkstemp(prefix=".units-", dir=output)
        os.close(fd)
        tmp = Path(tmp_name)
        try:
            rows = iter_units(catalog)
            count, digest, by_kind = (_write_jsonl(tmp, rows) if fmt == "jsonl" else _write_tsv(tmp, rows))
            tmp.replace(target)
        finally:
            tmp.unlink(missing_ok=True)
        files[fmt] = {"path": target.relative_to(Path(repository).absolute()).as_posix(),
                      "rows": count, "sha256": digest}
        counts = by_kind
        row_count = count
    source_inventory = catalog.get("source_inventory") or {}
    skipped_zero_extents = sum(_is_zero_source_padding(record) for record in source_inventory.get("records", []))
    skipped_zero_c_literals = sum(site.get("raw_length", 0) == 0 for site in source_inventory.get("c_literal_sites", []))
    skipped_script_terminators = sum(
        record.get("text") is None
        for entry in catalog.get("files", [])
        for record in (entry.get("script") or {}).get("records", [])
    )
    manifest = {"schema": "tokimemo.text.units.v1", "catalog_sha256": source_sha256,
                "row_count": row_count, "rows_by_kind": dict(sorted(counts.items())),
                "skipped_zero_source_extents": skipped_zero_extents,
                "skipped_zero_c_literals": skipped_zero_c_literals,
                "skipped_script_terminators": skipped_script_terminators,
                "target_text_policy": "null; no translation generated or imported",
                "files": files}
    manifest_path = output / "units-manifest.json"
    if manifest_path.is_symlink():
        raise UnitExportError("manifest cannot be a symlink")
    fd, tmp_name = tempfile.mkstemp(prefix=".units-manifest-", dir=output)
    os.close(fd)
    tmp_path = Path(tmp_name)
    try:
        tmp_path.write_bytes(_json_bytes(manifest))
        tmp_path.replace(manifest_path)
    finally:
        tmp_path.unlink(missing_ok=True)
    return manifest


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("catalog", type=Path)
    parser.add_argument("--output-dir", type=Path, default=OUTPUT_ROOT / "units")
    parser.add_argument("--repository", type=Path, default=REPOSITORY)
    parser.add_argument("--format", choices=("jsonl", "tsv", "both"), default="both")
    args = parser.parse_args(argv)
    formats = ("jsonl", "tsv") if args.format == "both" else (args.format,)
    manifest = export_units(args.catalog, args.output_dir, repository=args.repository, formats=formats)
    print(json.dumps({"row_count": manifest["row_count"], "rows_by_kind": manifest["rows_by_kind"],
                      "skipped_zero_source_extents": manifest["skipped_zero_source_extents"],
                      "skipped_zero_c_literals": manifest["skipped_zero_c_literals"],
                      "skipped_script_terminators": manifest["skipped_script_terminators"]}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
