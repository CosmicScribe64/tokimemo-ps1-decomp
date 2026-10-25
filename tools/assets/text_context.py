#!/usr/bin/env python3
"""Attach source-proven pointer-table consumers to verified text extents.

This module follows generated `.word` label edges and C symbol uses. Byte
equality is never used to infer a reference; every chain carries link-unit,
address, and source-site evidence.
"""
from __future__ import annotations

import hashlib
import json
import re
from pathlib import Path

from .text_source import (
    REPOSITORY,
    _call_containing_argument,
    _enclosing_function,
    _identifier_is_value_operand,
    _function_definitions,
    _function_spans,
    _lex_c_tokens,
    _mask_c_comments,
    _mask_c_literals,
    _read_original,
    source_link_unit,
)
from .text_renderers import string_argument_role

ADDR = re.compile(r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})(?:\s+([0-9A-Fa-f]{8}))?\s*\*/")
DLABEL = re.compile(r"^\s*dlabel\s+([A-Za-z_][A-Za-z_0-9]*)\b")
WORD = re.compile(r"^\s*\.word\s+(.+?)\s*$")
SYMBOL = re.compile(r"^[A-Za-z_][A-Za-z_0-9]*$")
class ContextError(ValueError):
    """The requested source context cannot be established safely."""


def _unit_from_binary(path: str) -> str:
    return source_link_unit(REPOSITORY, path) or "unknown"


def _unit_from_asm(path: Path, repo: Path) -> str:
    relative = path.relative_to(repo).as_posix()
    return source_link_unit(repo, relative) or "unknown"


def _role(call: str | None, function: str | None,
          argument_index: int | None = None, unit: str | None = None) -> tuple[str | None, str | None]:
    role = string_argument_role(call, argument_index, unit)
    if role in {"format", "diagnostic"}:
        return "debug_or_format", "known_format_or_diagnostic_consumer"
    if role == "display":
        return "display", "known_display_consumer"
    return None, None


def _role_fields(use: dict, ast_gated: bool) -> dict:
    candidate, basis = _role(use.get("call"), use.get("function"),
                             use.get("call_argument_index"), use.get("unit"))
    if ast_gated and use.get("ast_proof"):
        return {"category": candidate, "category_basis": basis,
                "category_candidate": None, "category_candidate_basis": None}
    return {"category": None, "category_basis": None,
            "category_candidate": candidate,
            "category_candidate_basis": "known call/index candidate; AST value-flow proof not supplied" if candidate else None}


def _literal_index(expression: str) -> int | None:
    value = re.sub(r"\b(?:[uUlL]+)$", "", expression.strip())
    if not re.fullmatch(r"(?:0[xX][0-9A-Fa-f]+|0[bB][01]+|[0-9]+|0[0-7]+)", value):
        return None
    try:
        if len(value) > 1 and value.startswith("0") and value[1] not in "xXbB":
            return int(value, 8)
        return int(value, 0)
    except ValueError:
        return None


def _selectors_for_symbol(expression: str, symbol: str) -> list[int | None]:
    match = re.search(r"\b" + re.escape(symbol) + r"\b((?:\s*\[[^\]]*\])*)", expression)
    if not match:
        return []
    return [_literal_index(item) for item in re.findall(r"\[([^\]]*)\]", match.group(1))]


def _source_files(repo: Path, area: str, suffix: str = "*.s"):
    root = repo / area
    return sorted(root.rglob(suffix)) if root.is_dir() else []


def _main_bss_regions(repo: Path) -> list[dict]:
    """Read noload BSS bounds from the executable config, never from symbol spelling."""
    repo = repo.resolve(strict=True)
    path = repo / "config" / "SLPM_86.053.yaml"
    try:
        resolved = path.resolve(strict=True)
        if not resolved.is_relative_to(repo) or not resolved.is_file():
            return []
        path = resolved
        raw = path.read_bytes()
        text = raw.decode("utf-8", errors="strict")
    except (OSError, UnicodeDecodeError):
        return []
    if text.encode("utf-8") != raw or "\ufffd" in text:
        return []
    size_matches = list(re.finditer(r"(?m)^\s*bss_size:\s*(0x[0-9A-Fa-f]+|[0-9]+)\s*$", text))
    segment_matches = list(re.finditer(
        r"(?m)^\s*-\s*\{\s*start:\s*(0x[0-9A-Fa-f]+|[0-9]+),\s*type:\s*bss,"
        r"\s*name:\s*([A-Za-z_][A-Za-z_0-9]*),\s*vram:\s*(0x[0-9A-Fa-f]+|[0-9]+)\s*\}", text))
    noload = re.search(r"(?m)^\s*ld_bss_is_noload:\s*(True|true)\s*$", text) is not None
    if not noload or len(size_matches) != 1 or len(segment_matches) != 1:
        return []
    size_match, segment = size_matches[0], segment_matches[0]
    start = int(segment.group(3), 0)
    size = int(size_match.group(1), 0)
    return [{"unit": "main", "name": segment.group(2), "noload": True,
             "vram_start": start, "vram_end_exclusive": start + size,
             "file_offset_start": int(segment.group(1), 0), "size": size,
             "config_path": path.relative_to(repo).as_posix(),
             "config_sha256": hashlib.sha256(raw).hexdigest(),
             "config_line": text.count("\n", 0, segment.start()) + 1,
             "size_config_line": text.count("\n", 0, size_match.start()) + 1}]


def _table_graph(repo: Path, records: list[dict], files_root: Path) -> tuple[dict, list[dict], set[str], dict, list[dict]]:
    asm_paths = _source_files(repo, "asm/data") + _source_files(repo, "asm/ovl")
    definitions: dict[tuple[str, str], dict] = {}
    definition_sites: dict[tuple[str, str], list[dict]] = {}
    pending_rows = []
    blocked = []
    runtime_targets = []
    bss_regions = _main_bss_regions(repo)
    overlay_bases = {}
    overlays_file = repo / "config" / "overlays.txt"
    if overlays_file.is_file():
        for line in overlays_file.read_text(encoding="utf-8").splitlines():
            parts = line.split()
            if len(parts) >= 2 and not parts[0].startswith("#"):
                overlay_bases[parts[0]] = int(parts[1], 0)
    binary_cache = {}
    binary_hash_cache = {}

    def original_for(unit: str):
        if unit not in binary_cache:
            if unit == "main":
                path, base, bias = files_root / "SLPM_86.053", 0x80041000, 0x800
            elif unit in overlay_bases:
                path = files_root / "CDROM" / "EXEDIR" / f"{unit}.EXN"
                base, bias = overlay_bases[unit], 0
            else:
                binary_cache[unit] = (None, None, None)
                return binary_cache[unit]
            data, error = _read_original(path, files_root)
            binary_cache[unit] = (data, base, bias)
            if error:
                blocked.append({"unit": unit, "path": path.relative_to(files_root).as_posix(),
                                "status": "original_input_rejected", "reason": error})
        return binary_cache[unit]

    record_labels: dict[tuple[str, str], list[dict]] = {}
    verified_record_ids = set()
    for record in records:
        source = record.get("source", {})
        unit = _unit_from_binary(source.get("path", ""))
        binary, base, bias = original_for(unit)
        raw_hex = record.get("raw_hex")
        try:
            raw_text = bytes.fromhex(raw_hex) if isinstance(raw_hex, str) else b""
        except ValueError:
            raw_text = b""
        if binary is not None and unit not in binary_hash_cache:
            binary_hash_cache[unit] = hashlib.sha256(binary).hexdigest()
        offset = source.get("offset")
        address = source.get("address")
        valid = (
            binary is not None and isinstance(offset, int) and isinstance(address, int) and
            offset == address - base + bias and source.get("sha256") == binary_hash_cache.get(unit) and
            source.get("file_size") == len(binary) and source.get("length") == len(raw_text) and
            record.get("raw_length") == len(raw_text) and record.get("extent_status", "").startswith("verified") and
            0 <= offset <= len(binary) - len(raw_text) and binary[offset:offset + len(raw_text)] == raw_text
        )
        if not valid:
            blocked.append({"id": record.get("id"), "status": "target_text_extent_unverified",
                            "reason": "source address, hash, bounds, extent status, or original bytes do not agree"})
            continue
        verified_record_ids.add(record["id"])
        for site in record.get("assembly_sites", []):
            for label in site.get("labels", []):
                record_labels.setdefault((unit, label), []).append(record)
    for path in sorted(set(asm_paths)):
        unit = _unit_from_asm(path, repo)
        try:
            raw = path.read_bytes()
            source = raw.decode("utf-8", errors="strict")
        except (OSError, UnicodeDecodeError) as exc:
            blocked.append({"path": path.relative_to(repo).as_posix(), "status": "assembly_source_unreadable",
                            "reason": type(exc).__name__})
            continue
        if source.encode("utf-8") != raw or "\ufffd" in source:
            blocked.append({"path": path.relative_to(repo).as_posix(), "status": "assembly_utf8_roundtrip_failed"})
            continue
        source_sha256 = hashlib.sha256(raw).hexdigest()
        current = None
        current_definition = None
        for line_no, line in enumerate(source.splitlines(), 1):
            label = DLABEL.match(line)
            if label:
                current = label.group(1)
                addr = ADDR.search(line)
                current_definition = {"unit": unit, "label": current,
                                      "path": path.relative_to(repo).as_posix(),
                                      "line": line_no,
                                      "address": int(addr.group(1), 16) if addr else None,
                                      "directive_sites": [], "source_sha256": source_sha256}
                key = (unit, current)
                definition_sites.setdefault(key, []).append(current_definition)
                definitions.setdefault(key, current_definition)
                continue
            if current is None:
                continue
            code_line = re.sub(r"/\*.*?\*/", "", line).split("#", 1)[0].strip()
            directive_match = re.match(r"(\.[A-Za-z_][A-Za-z_0-9]*)\b(.*)$", code_line)
            if not directive_match:
                continue
            comment = ADDR.search(line)
            directive_name = directive_match.group(1)
            directive_address = int(comment.group(1), 16) if comment else None
            current_definition["directive_sites"].append({"directive": directive_name,
                                                          "path": path.relative_to(repo).as_posix(),
                                                          "line": line_no, "address": directive_address})
            # Alignment can move the label, so a following byte directive is
            # only an exact label address when no alignment directive intervened.
            if current_definition.get("address") is None and directive_address is not None and \
                    directive_name not in {".align", ".balign", ".p2align"}:
                previous = current_definition["directive_sites"][:-1]
                if not any(site["directive"] in {".align", ".balign", ".p2align"} for site in previous):
                    current_definition["address"] = directive_address
            directive = WORD.match(code_line)
            if not directive or comment is None:
                continue
            address = directive_address
            encoded_bytes = bytes.fromhex(comment.group(2)) if comment.group(2) else None
            encoded_word = int.from_bytes(encoded_bytes, "little") if encoded_bytes else None
            expression = directive.group(1).split("/*", 1)[0].split("#", 1)[0].strip()
            target = expression if SYMBOL.fullmatch(expression) else None
            if target is not None:
                pending_rows.append({"unit": unit, "table": current, "target": target,
                                     "path": path.relative_to(repo).as_posix(), "line": line_no,
                                     "address": address, "encoded_word": encoded_word,
                                     "assembly_sha256": source_sha256})

    known_symbols = set(definitions) | set(record_labels)
    for row in pending_rows:
        table_key = (row["unit"], row["table"])
        table_definition = next((definition for definition in definition_sites.get(table_key, [])
                                 if definition["path"] == row["path"]), definitions.get(table_key))
        if table_definition is not None and table_definition["address"] is None:
            table_definition["address"] = row["address"]

    def resolve_definition(key):
        candidates = definition_sites.get(key, [])
        if not candidates:
            return None, "missing_data_label_definition"
        addresses = [item.get("address") for item in candidates if isinstance(item.get("address"), int)]
        if not addresses:
            return None, "data_label_address_unavailable"
        if len(set(addresses)) != 1:
            return None, "ambiguous_data_label_addresses"
        # Split data files can repeat the same dlabel without an address
        # comment. A unique anchored definition supplies the candidate VMA;
        # each incoming original pointer word must still prove that VMA below.
        anchored = [item for item in candidates if item.get("address") == addresses[0]]
        return min(anchored, key=lambda item: (item["path"], item["line"])), None
    exact_record_labels: dict[tuple[str, str], list[dict]] = {}
    for key, candidates in record_labels.items():
        definition, error = resolve_definition(key)
        if error or definition is None:
            continue
        exact_record_labels[key] = [record for record in candidates
                                    if record.get("source", {}).get("address") == definition["address"]]
    edges: dict[tuple[str, str], list[dict]] = {}
    for row in pending_rows:
        local_key = (row["unit"], row["target"])
        main_key = ("main", row["target"])
        target_key = local_key if local_key in known_symbols else \
            main_key if row["unit"] != "main" and main_key in known_symbols else None
        if target_key is None:
            continue
        target_def, definition_error = resolve_definition(target_key)
        all_target_records = record_labels.get(target_key, [])
        target_records = exact_record_labels.get(target_key, [])
        if (target_def is None and definition_error == "data_label_address_unavailable") or target_def is not None:
            runtime_address = target_def.get("address") if target_def is not None else row.get("encoded_word")
            bss_region = next((region for region in bss_regions
                               if target_key[0] == region["unit"] and
                               isinstance(runtime_address, int) and
                               region["vram_start"] <= runtime_address < region["vram_end_exclusive"]), None)
            source_binary, source_base, source_bias = original_for(row["unit"])
            source_offset = row["address"] - source_base + source_bias \
                if source_binary is not None and isinstance(row.get("address"), int) else -1
            source_word = runtime_address.to_bytes(4, "little") if isinstance(runtime_address, int) else b""
            pointer_verified = source_binary is not None and len(source_word) == 4 and \
                0 <= source_offset <= len(source_binary) - 4 and \
                source_binary[source_offset:source_offset + 4] == source_word
            anchored_pointer_matches = target_def is None or row.get("encoded_word") == runtime_address
            if bss_region is not None and pointer_verified and anchored_pointer_matches:
                target_binary_path = "SLPM_86.053"
                if target_key[0] not in binary_hash_cache:
                    target_binary, _, _ = original_for(target_key[0])
                    if target_binary is not None:
                        binary_hash_cache[target_key[0]] = hashlib.sha256(target_binary).hexdigest()
                bss_evidence = dict(bss_region)
                pointer_evidence = {
                    "unit": row["unit"], "assembly_path": row["path"], "assembly_line": row["line"],
                    "assembly_sha256": row["assembly_sha256"], "address": row["address"],
                    "binary_path": "SLPM_86.053" if row["unit"] == "main" else
                                   f"CDROM/EXEDIR/{row['unit']}.EXN",
                    "binary_offset": source_offset, "source_word_hex": source_word.hex(),
                    "binary_sha256": binary_hash_cache.get(row["unit"]),
                    "bytes_verified": True,
                }
                definition_evidence = [{"path": definition["path"], "line": definition["line"],
                                        "address": definition.get("address"),
                                        "source_sha256": definition["source_sha256"]}
                                       for definition in definition_sites.get(target_key, [])]
                runtime_entry = {
                    "unit": target_key[0], "label": target_key[1], "address": runtime_address,
                    "status": "noload_bss_runtime_storage", "static_bytes_available": False,
                    "binary_path": target_binary_path,
                    "bss_region": bss_evidence,
                    "definition_sites": definition_evidence,
                    "pointer_evidence": pointer_evidence,
                }
                runtime_targets.append(runtime_entry)
                table_definition, _ = resolve_definition((row["unit"], row["table"]))
                table_address = table_definition.get("address") if table_definition else None
                slot_index = None
                if table_address is not None and row["address"] >= table_address and \
                        (row["address"] - table_address) % 4 == 0:
                    slot_index = (row["address"] - table_address) // 4
                edges.setdefault((row["unit"], row["table"]), []).append({
                    **row, "target_address": runtime_address, "target_data_label_evidence": runtime_entry,
                    "target_unit": target_key[0], "slot_index": slot_index,
                    "source_offset": source_offset, "source_word_hex": source_word.hex(),
                    "target_kind": "runtime_non_static"})
                continue
        if definition_error and target_def is None and not target_records:
            blocked.append({"path": row["path"], "line": row["line"], "unit": row["unit"],
                            "target_unit": target_key[0], "target": target_key[1],
                            "status": definition_error})
            continue
        target_address = target_def.get("address") if target_def else None
        if target_address is None:
            blocked.append({"path": row["path"], "line": row["line"], "unit": row["unit"],
                            "target_unit": target_key[0], "target": target_key[1],
                            "status": "target_data_label_address_unavailable"})
            continue
        # Source-inventory label associations can include a label that precedes
        # a string extent. Only a record beginning at the label VMA is a text
        # target; an interior/containing association stays diagnostic evidence.
        non_exact_records = [record for record in all_target_records if record not in target_records]
        if non_exact_records:
            blocked.append({"path": row["path"], "line": row["line"], "unit": row["unit"],
                            "target_unit": target_key[0], "target": target_key[1],
                            "target_address": target_address,
                            "record_ids": sorted(record["id"] for record in non_exact_records),
                            "status": "containing_label_record_is_not_exact_start"})
        if row["encoded_word"] is None:
            blocked.append({"path": row["path"], "line": row["line"], "unit": row["unit"],
                            "status": "pointer_word_bytes_unavailable"})
            continue
        if target_address is None or row["encoded_word"] != target_address:
            blocked.append({"path": row["path"], "line": row["line"], "unit": row["unit"],
                            "target_unit": target_key[0], "target": target_key[1],
                            "status": "pointer_target_address_mismatch"})
            continue
        binary, base, bias = original_for(row["unit"])
        if binary is None:
            blocked.append({"path": row["path"], "line": row["line"], "unit": row["unit"],
                            "status": "pointer_source_binary_missing"})
            continue
        offset = row["address"] - base + bias
        expected_bytes = row["encoded_word"].to_bytes(4, "little")
        if offset < 0 or offset + 4 > len(binary) or binary[offset:offset + 4] != expected_bytes:
            blocked.append({"path": row["path"], "line": row["line"], "unit": row["unit"],
                            "address": row["address"], "status": "pointer_word_binary_mismatch"})
            continue
        target_binary, target_base, target_bias = original_for(target_key[0])
        target_offset = target_address - target_base + target_bias if target_binary is not None else -1
        if target_binary is None or not 0 <= target_offset < len(target_binary):
            blocked.append({"path": row["path"], "line": row["line"], "unit": row["unit"],
                            "target_unit": target_key[0], "target": target_key[1],
                            "target_address": target_address, "status": "target_data_label_out_of_bounds"})
            continue
        target_binary_path = "SLPM_86.053" if target_key[0] == "main" else \
            f"CDROM/EXEDIR/{target_key[0]}.EXN"
        if target_key[0] not in binary_hash_cache:
            binary_hash_cache[target_key[0]] = hashlib.sha256(target_binary).hexdigest()
        target_label_evidence = {
            "unit": target_key[0], "label": target_key[1], "address": target_address,
            "binary_path": target_binary_path, "binary_offset": target_offset,
            "binary_file_size": len(target_binary),
            "binary_sha256": binary_hash_cache[target_key[0]],
        }
        label_definitions = definition_sites.get(target_key, [])
        target_label_evidence["definition_evidence"] = {
            "anchored_definition_count": sum(isinstance(item.get("address"), int)
                                             for item in label_definitions),
            "unanchored_duplicate_definition_count": sum(item.get("address") is None
                                                          for item in label_definitions),
            "all_anchored_definitions_agree": len({item.get("address") for item in label_definitions
                                                     if isinstance(item.get("address"), int)}) == 1,
        }
        if non_exact_records:
            target_label_evidence["non_exact_containing_text_record_ids"] = sorted(
                record["id"] for record in non_exact_records)
        if target_def:
            target_label_evidence.update({"assembly_path": target_def["path"],
                                          "assembly_line": target_def["line"],
                                          "assembly_sha256": target_def["source_sha256"],
                                          "directive_sites": target_def["directive_sites"]})
        elif target_records:
            target_label_evidence["verified_text_record_ids"] = sorted(record["id"] for record in target_records)
        table_definition, _ = resolve_definition((row["unit"], row["table"]))
        table_address = table_definition.get("address") if table_definition else None
        slot_index = None
        if table_address is not None and row["address"] >= table_address and (row["address"] - table_address) % 4 == 0:
            slot_index = (row["address"] - table_address) // 4
        edges.setdefault((row["unit"], row["table"]), []).append({
            **row, "target_address": target_address,
            "target_data_label_evidence": target_label_evidence,
            "target_unit": target_key[0],
            "slot_index": slot_index,
            "source_offset": offset, "source_word_hex": expected_bytes.hex(),
            "target_kind": "text_extent" if target_records else "pointer_table"})
    return edges, blocked, verified_record_ids, exact_record_labels, runtime_targets


def _c_symbol_uses(repo: Path, symbols: set[tuple[str, str]]) -> tuple[list[dict], list[dict]]:
    uses, blocked = [], []
    files = _source_files(repo, "src/main", "*.c") + _source_files(repo, "src/ovl", "*.c")
    for path in sorted(files):
        relative = path.relative_to(repo).as_posix()
        unit = source_link_unit(repo, relative) or "unknown"
        try:
            raw = path.read_bytes()
            text = raw.decode("utf-8", errors="strict")
        except (OSError, UnicodeDecodeError) as exc:
            blocked.append({"path": relative, "status": "C_source_unreadable", "reason": type(exc).__name__})
            continue
        if text.encode("utf-8") != raw or "\ufffd" in text:
            blocked.append({"path": relative, "status": "C_source_utf8_roundtrip_failed"})
            continue
        tokens = list(_lex_c_tokens(text))
        errors = [t for t in tokens if t["kind"] == "lex_error"]
        if errors:
            blocked.extend({"path": relative, "line": text.count("\n", 0, t["start"]) + 1,
                            "status": "C_lexer_error", "reason": t.get("error")} for t in errors)
        tokens = [t for t in tokens if t["kind"] != "lex_error"]
        for token in tokens:
            if token["kind"] == "other" and text[token["start"]:token["end"]] in ("{", "}"):
                token["punct"] = text[token["start"]:token["end"]]
        masked = _mask_c_comments(text)
        code = _mask_c_literals(masked, tokens)
        spans = _function_spans(_function_definitions(masked), tokens)
        for token in tokens:
            if token["kind"] != "other":
                continue
            token_text = text[token["start"]:token["end"]]
            for match in re.finditer(r"[A-Za-z_][A-Za-z_0-9]*", token_text):
                symbol = match.group()
                position = token["start"] + match.start()
                line_start = text.rfind("\n", 0, position) + 1
                line_end = text.find("\n", position)
                source_line = text[line_start:] if line_end < 0 else text[line_start:line_end]
                if re.search(r"\bINCLUDE_(?:RODATA|ASM)\s*\(", source_line):
                    continue
                if re.match(r"\s*#\s*include\b", source_line):
                    continue
                declaration = re.search(
                    r"\b(?:const\s+)?(?:u8|s8|u16|s16|u32|s32|char|int|short|long|void|struct\s+\w+)\s+\*?\s*" +
                    re.escape(symbol) + r"\b", source_line)
                if declaration:
                    continue
                symbol_unit = unit if (unit, symbol) in symbols else \
                    "main" if unit != "main" and ("main", symbol) in symbols else None
                if symbol_unit is None:
                    continue
                call_info = _call_containing_argument(code, position)
                call = call_info["call"] if call_info else None
                argument_index = call_info["argument_index"] if call_info else None
                argument_expression = call_info["argument_expression"] if call_info else None
                if not _identifier_is_value_operand(argument_expression, symbol):
                    continue
                function = _enclosing_function(spans, position)
                call_start = call_info.get("call_start") if call_info else None
                call_line = text.count("\n", 0, call_start) + 1 if call_start is not None else None
                call_column = call_start - (text.rfind("\n", 0, call_start) + 1) + 1 \
                    if call_start is not None else None
                if call is None and function is None:
                    continue
                declaration = re.search(
                    r"\b(?:const\s+)?(?:u8|s8|u16|s16|u32|s32|char|int|short|long|void|struct\s+\w+)\s+\*?\s*" +
                    re.escape(symbol) + r"\b", source_line)
                if declaration:
                    continue
                category, category_basis = _role(call, function, argument_index, unit)
                uses.append({"unit": unit, "symbol_unit": symbol_unit, "symbol": symbol, "path": relative,
                             "line": text.count("\n", 0, position) + 1,
                             "call_line": call_line, "call_column": call_column,
                             "function": function, "call": call, "call_argument_index": argument_index,
                             "call_argument_expression": argument_expression,
                             "selectors": _selectors_for_symbol(argument_expression or "", symbol),
                             "category": category, "category_basis": category_basis,
                             "direct_call_argument": call is not None})
    return uses, blocked


def _ast_fact_index(ast_consumers: dict) -> dict[tuple[str, int, str, int], list[dict]]:
    facts = {}
    for source in ast_consumers.get("files", []):
        path = source.get("path")
        if not isinstance(path, str):
            continue
        for call in source.get("calls", []):
            key = (path, call.get("line"), call.get("callee"), call.get("argument_index"))
            facts.setdefault(key, []).append(call)
    return facts


def _apply_ast_facts(uses: list[dict], ast_consumers: dict) -> tuple[list[dict], int]:
    """Keep only identifier flows AST proved as text-pointer values at known read args."""
    index = _ast_fact_index(ast_consumers)
    accepted, rejected = [], 0
    for use in uses:
        if use.get("call") is None or use.get("call_argument_index") is None:
            rejected += 1
            continue
        key = (use["path"], use.get("call_line"), use["call"], use["call_argument_index"])
        matches = [fact for fact in index.get(key, [])
                   if use["symbol"] in fact.get("candidate_symbols", []) and
                   fact.get("column") == use.get("call_column") and
                   fact.get("argument_role") == string_argument_role(
                       use.get("call"), use.get("call_argument_index"), use.get("unit"))]
        if len(matches) != 1:
            rejected += 1
            continue
        fact = matches[0]
        value_facts = [item for item in fact.get("symbol_facts", [])
                       if item.get("symbol") == use["symbol"] and
                       item.get("pointer_value_flows_to_argument")]
        if len(value_facts) != 1:
            rejected += 1
            continue
        symbol_fact = value_facts[0]
        if symbol_fact.get("occurrence_role") == "member_base":
            rejected += 1
            continue
        selector_groups = [group for group in fact.get("selectors", [])
                           if use["symbol"] in group.get("base_symbols", []) and
                           group.get("expression_use") == "argument_value"]
        if len(selector_groups) > 1:
            rejected += 1
            continue
        selector_indices = []
        if selector_groups:
            group = selector_groups[0]
            selector_items = group.get("selectors", [])
            if group.get("dereference_count") != len(selector_items):
                rejected += 1
                continue
            selector_indices = [item.get("constant") for item in selector_items]
        if symbol_fact.get("dereference_count", 0) != len(selector_indices):
            rejected += 1
            continue
        load_words = symbol_fact.get("global_value_load_words", 0)
        use["ast_global_value_load_words"] = load_words
        use["ast_address_of"] = bool(symbol_fact.get("address_of"))
        use["ast_occurrence_role"] = symbol_fact.get("occurrence_role")
        if use["ast_address_of"]:
            use["ast_unresolved_reason"] = "address_of_requires_interior_address_resolution"
            use["selectors"] = []
        else:
            # Scalar pointer globals require a proven load of their stored word
            # before any source-level array selectors can be applied.
            use["selectors"] = [0] * load_words + selector_indices
            if load_words and not selector_indices:
                use["ast_unresolved_reason"] = "global_pointer_word_load_without_explicit_index"
        use["ast_argument_role"] = fact.get("argument_role")
        use["callee_read_conditions"] = fact.get("callee_read_conditions", [])
        use["callee_read_condition_status"] = fact.get("callee_read_condition_status")
        use["ast_value_flow_status"] = fact.get("value_flow_status")
        use["ast_argument_expression_type"] = fact.get("argument_expression_type")
        use["ast_proof"] = "typed_pointer_value_flow_to_known_read_argument"
        accepted.append(use)
    return accepted, rejected


def enrich_source_context(inventory: dict, repository=REPOSITORY, files_root=None,
                          ast_consumers: dict | None = None, use_ast: bool = False) -> dict:
    """Add verified source pointer-table and C consumer context to an inventory in place."""
    repo = Path(repository).resolve(strict=True)
    inputs = Path(files_root or (REPOSITORY / "disc" / "files")).resolve(strict=True)
    records = inventory.get("records")
    if not isinstance(records, list):
        raise ContextError("inventory records must be a list")
    edges, blocked_asm, verified_record_ids, exact_record_labels, runtime_targets = \
        _table_graph(repo, records, inputs)
    symbols = set(edges)
    symbols.update((row["target_unit"], row["target"]) for rows in edges.values() for row in rows
                   if row["target_kind"] == "pointer_table")
    symbols.update((_unit_from_binary(record["source"]["path"]), label)
                   for record in records if record["id"] in verified_record_ids
                   for site in record.get("assembly_sites", [])
                   for label in site.get("labels", []))
    uses, blocked_c = _c_symbol_uses(repo, symbols)
    ast_rejected_uses = 0
    if ast_consumers is None and use_ast:
        from .text_ast import inventory_text_consumers, source_input_fingerprint
        candidates_by_unit = {}
        main_symbols = {symbol for unit, symbol in symbols if unit == "main"}
        for unit, symbol in symbols:
            candidates_by_unit.setdefault(unit, set()).add(symbol)
        for unit in {unit for unit, _ in symbols if unit != "main"}:
            candidates_by_unit.setdefault(unit, set()).update(main_symbols)
        ast_consumers = inventory_text_consumers(repo, candidates_by_unit)
        inventory["ast_consumers"] = ast_consumers
    if ast_consumers is not None:
        from .text_ast import source_input_fingerprint
        current_fingerprint = source_input_fingerprint(repo)
        if (ast_consumers.get("stable_source_inputs") is not True or
                ast_consumers.get("source_inputs_sha256") != current_fingerprint or
                ast_consumers.get("source_inputs_sha256_after") != current_fingerprint):
            raise ContextError("AST consumer report is stale or source inputs changed during analysis")
        uses, ast_rejected_uses = _apply_ast_facts(uses, ast_consumers)
    for record in records:
        if record.get("category_basis") == "unique source-proven consumer role":
            record["category"] = "unknown"
            record["category_basis"] = "context-owned category reset before revalidation"
    contexts_by_record: dict[str, list[dict]] = {r["id"]: [] for r in records}
    records_by_label: dict[tuple[str, str], list[dict]] = {}
    for label_key, labeled_records in exact_record_labels.items():
        for record in labeled_records:
            records_by_label.setdefault(label_key, []).append(record)
    unresolved_leaves_by_id = {}
    typed_text_reached_edges = set()
    dynamically_reached_edges = set()
    for use in uses:
        root = (use["symbol_unit"], use["symbol"])
        argument_site = {k: use[k] for k in ("path", "line", "call_line", "call_column",
                                               "function", "call", "call_argument_index")}
        argument_site["callee_read_conditions"] = use.get("callee_read_conditions", [])
        argument_site["callee_read_condition_status"] = use.get("callee_read_condition_status")
        role_fields = _role_fields(use, ast_consumers is not None)
        if root in records_by_label and not use.get("ast_address_of") and \
                not use.get("ast_global_value_load_words") and \
                not use.get("selectors") and \
                use.get("ast_occurrence_role") != "member_base":
            for record in records_by_label[root]:
                contexts_by_record[record["id"]].append({
                    "source_site": argument_site,
                    "table_symbol": None, **role_fields,
                    "evidence_chain": [{"kind": "C_symbol_reference", "symbol": use["symbol"],
                                        "unit": use["unit"], "target_unit": use["symbol_unit"],
                                        "path": use["path"], "line": use["line"]}],
                    "proof": "same_link_unit_or_proven_main_global_C_symbol_reference",
                    "selection_certainty": "direct_symbol_argument" if role_fields["category"] else
                                           "candidate_direct_symbol_argument" if role_fields["category_candidate"] else
                                           "unclassified_call_argument"})
        selectors = use.get("selectors", [])
        if not selectors:
            continue
        todo = [(root, [])]
        visited = set()
        while todo:
            node, chain = todo.pop()
            state = (node, len(chain), tuple((edge.get("path"), edge.get("line"),
                                               edge.get("slot_index"), edge.get("target_unit"),
                                               edge.get("target")) for edge in chain))
            if state in visited:
                continue
            visited.add(state)
            depth = len(chain)
            if depth >= len(selectors):
                continue
            requested_index = selectors[depth]
            for edge in edges.get(node, []):
                if edge["slot_index"] is None:
                    continue
                if requested_index is not None and edge["slot_index"] != requested_index:
                    continue
                edge_key = (edge["unit"], edge["table"], edge["path"], edge["line"])
                if ast_consumers is not None and role_fields["category"] in {"display", "debug_or_format"}:
                    typed_text_reached_edges.add(edge_key)
                    if requested_index is None:
                        dynamically_reached_edges.add(edge_key)
                next_chain = chain + [{"table": edge["table"], "target": edge["target"],
                                       "path": edge["path"], "line": edge["line"],
                                       "address": edge["address"],
                                       "source_offset": edge["source_offset"],
                                       "source_word_hex": edge["source_word_hex"],
                                       "assembly_sha256": edge["assembly_sha256"],
                                       "slot_index": edge["slot_index"],
                                       "requested_index": requested_index,
                                       "target_unit": edge["target_unit"],
                                       "target_address": edge["target_address"]}]
                target_key = (edge["target_unit"], edge["target"])
                path_is_terminal = depth + 1 == len(selectors)
                target_records = records_by_label.get(target_key, [])
                selection_certainty = "possible_dynamic_index" if None in selectors else \
                    "statically_selected_pointer_path"
                if path_is_terminal and target_records:
                    for record in target_records:
                        contexts_by_record[record["id"]].append({
                            "source_site": argument_site,
                            "table_symbol": use["symbol"], **role_fields,
                            "evidence_chain": next_chain,
                            "proof": "same_link_unit_word_pointer_targets_verified_text_label",
                            "selection_certainty": selection_certainty})
                elif path_is_terminal and not target_records and ast_consumers is not None and \
                        edge["target_kind"] != "runtime_non_static" and \
                        role_fields["category"] in {"display", "debug_or_format"} and \
                        edge.get("target_data_label_evidence"):
                    leaf_basis = (use["path"], use.get("call_line"), use.get("call_column"),
                                  use.get("call_argument_index"), target_key,
                                  tuple((item.get("path"), item.get("line"), item.get("slot_index"),
                                         item.get("target_unit"), item.get("target")) for item in next_chain))
                    leaf_id = "data-leaf:" + hashlib.sha256(
                        json.dumps(leaf_basis, sort_keys=True, separators=(",", ":")).encode("utf-8")
                    ).hexdigest()[:20]
                    unresolved_leaves_by_id[leaf_id] = {
                        "id": leaf_id,
                        "status": "unclassified_data_label_reached_by_typed_text_path",
                        "category": None,
                        "category_candidate": role_fields["category"],
                        "category_candidate_basis": role_fields["category_basis"],
                        "source_site": argument_site,
                        "source_symbol": use["symbol"],
                        "source_argument_role": use.get("ast_argument_role"),
                        "source_argument_type": use.get("ast_argument_expression_type"),
                        "source_value_flow_status": use.get("ast_value_flow_status"),
                        "source_proof": use.get("ast_proof"),
                        "selectors": selectors,
                        "selection_certainty": selection_certainty,
                        "terminal_data_label": edge["target_data_label_evidence"],
                        "evidence_chain": next_chain,
                    }
                if edge["target_kind"] == "pointer_table" and not path_is_terminal:
                    todo.append((target_key, next_chain))

    category_counts = {}
    unresolved = []
    unclassified_nonempty = []
    for record in records:
        contexts = contexts_by_record.get(record["id"], [])
        # Stable dedup across aggregate/split assembly sources.
        unique = {}
        for context in contexts:
            key = (context["source_site"]["path"], context["source_site"]["line"],
                   context["source_site"].get("call_line", -1),
                   context["source_site"].get("call_column", -1),
                   context["source_site"].get("call_argument_index", -1),
                   context["table_symbol"] or "", tuple((e.get("path"), e.get("line"))
                                                   for e in context["evidence_chain"]))
            unique[key] = context
        contexts = sorted(unique.values(), key=lambda c: (c["source_site"]["path"],
                                                           c["source_site"]["line"],
                                                           c["source_site"].get("call_argument_index", -1),
                                                           c["table_symbol"] or ""))
        record["source_contexts"] = contexts
        if record.get("category") == "empty_extent_no_translatable_bytes":
            record["context_category"] = "empty_extent_no_translatable_bytes"
            record["context_category_basis"] = "zero-only verified source extent has no translatable bytes"
            category_counts[record["category"]] = category_counts.get(record["category"], 0) + 1
            continue
        categories = {c["category"] for c in contexts if c.get("category")}
        if len(categories) == 1:
            record["context_category"] = next(iter(categories))
            record["context_category_basis"] = "unique source-proven consumer role"
            if record.get("category") == "unknown":
                record["category"] = record["context_category"]
                record["category_basis"] = record["context_category_basis"]
        else:
            record["context_category"] = None
            record["context_category_basis"] = "no unique consumer role proved"
        category_counts[record["category"]] = category_counts.get(record["category"], 0) + 1
        if not contexts:
            unresolved.append(record["id"])
        if record.get("category") == "unknown" and any(bytes.fromhex(record.get("raw_hex", ""))):
            unclassified_nonempty.append(record["id"])

    unresolved_data_label_leaves = sorted(unresolved_leaves_by_id.values(), key=lambda item: item["id"])
    for runtime_target in runtime_targets:
        pointer = runtime_target["pointer_evidence"]
        edge_key = (pointer["unit"], None, pointer["assembly_path"], pointer["assembly_line"])
        matched = any(key[0] == edge_key[0] and key[2:] == edge_key[2:]
                      for key in typed_text_reached_edges)
        runtime_target["typed_text_reachability"] = "reached_by_typed_text_use" if matched else \
            "unreached_global_graph_scaffolding"
    inventory["unresolved_data_label_leaves"] = unresolved_data_label_leaves
    inventory["source_context"] = {
        "schema": "tokimemo.text.source-context.v1",
        "pointer_table_edges": sum(len(rows) for rows in edges.values()),
        "pointer_table_edges_reached_by_typed_text_uses": len(typed_text_reached_edges),
        "pointer_table_edges_reached_dynamically": len(dynamically_reached_edges),
        "pointer_table_edges_unreached_global_scaffolding":
            sum(len(rows) for rows in edges.values()) - len(typed_text_reached_edges),
        "runtime_non_static_target_count": len(runtime_targets),
        "runtime_non_static_targets": runtime_targets,
        "pointer_table_symbols": sorted([list(key) for key in edges]),
        "C_symbol_uses": len(uses),
        "AST_value_flow_gated": ast_consumers is not None,
        "AST_rejected_C_symbol_uses": ast_rejected_uses,
        "unresolved_data_label_leaf_count": len(unresolved_data_label_leaves),
        "unresolved_data_label_leaves": unresolved_data_label_leaves,
        "records_with_context": sum(bool(contexts_by_record.get(r["id"])) for r in records),
        "records_without_proven_consumer_context": len(unresolved),
        "unverified_record_ids": sorted(r["id"] for r in records if r["id"] not in verified_record_ids),
        "unclassified_nonempty_record_ids": unclassified_nonempty,
        "unclassified_nonempty_records": len(unclassified_nonempty),
        "category_counts_after_context": category_counts,
        "unresolved_record_ids": unresolved,
        "blocked": blocked_asm + blocked_c,
                         "proof_rules": ["same-link-unit symbols only",
                         "word comment VMA and little-endian target VMA agree",
                         "original binary bytes at the table VMA equal the emitted pointer word",
                         "target text extents require exact label-start address agreement",
                         "duplicate unanchored labels require one agreed anchored VMA and pointer-byte agreement",
                         "noload BSS targets use configured runtime bounds and carry no static text bytes",
                         "category requires a recognized direct C consumer call or typed name/system routine"],
    }
    return inventory
