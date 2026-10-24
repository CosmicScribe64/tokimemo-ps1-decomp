#!/usr/bin/env python3
"""Attach source-proven pointer-table consumers to verified text extents.

This module follows generated `.word` label edges and C symbol uses. Byte
equality is never used to infer a reference; every chain carries link-unit,
address, and source-site evidence.
"""
from __future__ import annotations

import hashlib
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
)
from .text_renderers import string_argument_role

ADDR = re.compile(r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})(?:\s+([0-9A-Fa-f]{8}))?\s*\*/")
DLABEL = re.compile(r"^\s*dlabel\s+([A-Za-z_][A-Za-z_0-9]*)\b")
WORD = re.compile(r"^\s*\.word\s+(.+?)\s*$")
SYMBOL = re.compile(r"^[A-Za-z_][A-Za-z_0-9]*$")
class ContextError(ValueError):
    """The requested source context cannot be established safely."""


def _unit_from_binary(path: str) -> str:
    if path == "SLPM_86.053":
        return "main"
    match = re.fullmatch(r"CDROM/EXEDIR/([A-Z0-9_]+)\.EXN", path)
    return match.group(1) if match else "unknown"


def _unit_from_asm(path: Path, repo: Path) -> str:
    relative = path.relative_to(repo).as_posix()
    if relative.startswith("asm/data/ovl/"):
        return relative.split("/", 3)[3]
    if relative.startswith("asm/data/") or relative.startswith("asm/matchings/main/"):
        return "main"
    match = re.match(r"asm/ovl/([A-Z0-9_]+)/", relative)
    return match.group(1) if match else "unknown"


def _role(call: str | None, function: str | None,
          argument_index: int | None = None) -> tuple[str | None, str | None]:
    role = string_argument_role(call, argument_index)
    if role in {"format", "diagnostic"}:
        return "debug_or_format", "known_format_or_diagnostic_consumer"
    if role == "display":
        return "display", "known_display_consumer"
    return None, None


def _role_fields(use: dict, ast_gated: bool) -> dict:
    candidate, basis = _role(use.get("call"), use.get("function"),
                             use.get("call_argument_index"))
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


def _table_graph(repo: Path, records: list[dict], files_root: Path) -> tuple[dict, list[dict], set[str]]:
    asm_paths = _source_files(repo, "asm/data") + _source_files(repo, "asm/ovl")
    definitions: dict[tuple[str, str], dict] = {}
    pending_rows = []
    blocked = []
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
        current = None
        for line_no, line in enumerate(source.splitlines(), 1):
            label = DLABEL.match(line)
            if label:
                current = label.group(1)
                addr = ADDR.search(line)
                definitions[(unit, current)] = {"unit": unit, "label": current,
                                                "path": path.relative_to(repo).as_posix(),
                                                "line": line_no,
                                                "address": int(addr.group(1), 16) if addr else None}
                continue
            if current is None:
                continue
            directive = WORD.match(re.sub(r"/\*.*?\*/", "", line).strip())
            if not directive:
                continue
            comment = ADDR.search(line)
            if not comment:
                continue
            address = int(comment.group(1), 16)
            encoded_bytes = bytes.fromhex(comment.group(2)) if comment.group(2) else None
            encoded_word = int.from_bytes(encoded_bytes, "little") if encoded_bytes else None
            expression = directive.group(1).split("/*", 1)[0].split("#", 1)[0].strip()
            target = expression if SYMBOL.fullmatch(expression) else None
            if target is not None:
                pending_rows.append({"unit": unit, "table": current, "target": target,
                                     "path": path.relative_to(repo).as_posix(), "line": line_no,
                                     "address": address, "encoded_word": encoded_word})

    known_symbols = set(definitions) | set(record_labels)
    for row in pending_rows:
        table_definition = definitions.get((row["unit"], row["table"]))
        if table_definition is not None and table_definition["address"] is None:
            table_definition["address"] = row["address"]
    edges: dict[tuple[str, str], list[dict]] = {}
    for row in pending_rows:
        local_key = (row["unit"], row["target"])
        main_key = ("main", row["target"])
        target_key = local_key if local_key in known_symbols else \
            main_key if row["unit"] != "main" and main_key in known_symbols else None
        if target_key is None or row["encoded_word"] is None:
            continue
        target_def = definitions.get(target_key)
        target_records = record_labels.get(target_key, [])
        target_address = target_def.get("address") if target_def else None
        if target_address is None and target_records:
            target_address = target_records[0]["source"].get("address")
        if target_address is None or row["encoded_word"] != target_address:
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
        table_address = definitions.get((row["unit"], row["table"]), {}).get("address")
        slot_index = None
        if table_address is not None and row["address"] >= table_address and (row["address"] - table_address) % 4 == 0:
            slot_index = (row["address"] - table_address) // 4
        edges.setdefault((row["unit"], row["table"]), []).append({
            **row, "target_address": target_address,
            "target_unit": target_key[0],
            "slot_index": slot_index,
            "source_offset": offset, "source_word_hex": expected_bytes.hex(),
            "target_kind": "text_extent" if target_records else "pointer_table"})
    return edges, blocked, verified_record_ids


def _c_symbol_uses(repo: Path, symbols: set[tuple[str, str]]) -> tuple[list[dict], list[dict]]:
    uses, blocked = [], []
    files = _source_files(repo, "src/main", "*.c") + _source_files(repo, "src/ovl", "*.c")
    for path in sorted(files):
        relative = path.relative_to(repo).as_posix()
        unit = "main" if relative.startswith("src/main/") else relative.split("/")[2]
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
                category, category_basis = _role(call, function, argument_index)
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
                       use.get("call"), use.get("call_argument_index"))]
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
        use["ast_value_flow_status"] = fact.get("value_flow_status")
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
    edges, blocked_asm, verified_record_ids = _table_graph(repo, records, inputs)
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
    for record in records:
        if record["id"] not in verified_record_ids:
            continue
        unit = _unit_from_binary(record["source"]["path"])
        for asm_site in record.get("assembly_sites", []):
            for label in asm_site.get("labels", []):
                records_by_label.setdefault((unit, label), []).append(record)
    for use in uses:
        root = (use["symbol_unit"], use["symbol"])
        argument_site = {k: use[k] for k in ("path", "line", "call_line", "call_column",
                                               "function", "call", "call_argument_index")}
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
            if node in visited:
                continue
            visited.add(node)
            depth = len(chain)
            if depth >= len(selectors):
                continue
            requested_index = selectors[depth]
            for edge in edges.get(node, []):
                if edge["slot_index"] is None:
                    continue
                if requested_index is not None and edge["slot_index"] != requested_index:
                    continue
                next_chain = chain + [{"table": edge["table"], "target": edge["target"],
                                       "path": edge["path"], "line": edge["line"],
                                       "address": edge["address"],
                                       "source_offset": edge["source_offset"],
                                       "source_word_hex": edge["source_word_hex"],
                                       "slot_index": edge["slot_index"],
                                       "requested_index": requested_index,
                                       "target_unit": edge["target_unit"],
                                       "target_address": edge["target_address"]}]
                target_key = (edge["target_unit"], edge["target"])
                for record in (records_by_label.get(target_key, [])
                               if depth + 1 == len(selectors) else []):
                    contexts_by_record[record["id"]].append({
                        "source_site": argument_site,
                        "table_symbol": use["symbol"], **role_fields,
                        "evidence_chain": next_chain,
                        "proof": "same_link_unit_word_pointer_targets_verified_text_label",
                        "selection_certainty": "possible_dynamic_index" if None in selectors else
                                               "statically_selected_pointer_path"})
                if edge["target_kind"] == "pointer_table":
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

    inventory["source_context"] = {
        "schema": "tokimemo.text.source-context.v1",
        "pointer_table_edges": sum(len(rows) for rows in edges.values()),
        "pointer_table_symbols": sorted([list(key) for key in edges]),
        "C_symbol_uses": len(uses),
        "AST_value_flow_gated": ast_consumers is not None,
        "AST_rejected_C_symbol_uses": ast_rejected_uses,
        "records_with_context": sum(bool(contexts_by_record.get(r["id"])) for r in records),
        "records_without_proven_consumer_context": len(unresolved),
        "unverified_record_ids": sorted(r["id"] for r in records if r["id"] not in verified_record_ids),
        "unclassified_nonempty_record_ids": unclassified_nonempty,
        "unclassified_nonempty_records": len(unclassified_nonempty),
        "category_counts_after_context": category_counts,
        "unresolved_record_ids": unresolved,
        "blocked": blocked_asm + blocked_c,
                         "proof_rules": ["same-link-unit symbols only", "word comment VMA and little-endian target VMA agree",
                         "original binary bytes at the table VMA equal the emitted pointer word",
                         "target label resolves to a verified text extent or another pointer table",
                         "category requires a recognized direct C consumer call or typed name/system routine"],
    }
    return inventory
