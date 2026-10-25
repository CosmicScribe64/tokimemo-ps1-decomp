"""Recover initialized data-label strings rooted in verified MIPS text arguments.

This supplements string directives, never searches a binary for possible text.
Only an exact data-label start with a contiguous, byte-verified initialized
prefix and a terminator inside that prefix is eligible. Calls remain potential
static contexts; writable buffers and runtime read lengths are not inferred.
"""
from __future__ import annotations

import hashlib
import re
from pathlib import Path

from .text import source_path
from .text_fixed import tokenize_fixed_text
from .text_mips import (_record_unit, _unit, _reset, _ordinary, _control, enrich_mips_context,
                        INSTRUCTION, LABEL, SYMBOL, role_for_argument, text_argument_spec)
from .text_renderers import string_argument_role
from .text_source import (ADDR_COMMENT, REPOSITORY, _decoded_view, _read_original,
                          _string_bytes, source_link_unit)

_LABEL = re.compile(r"^\s*(dlabel|enddlabel)\s+(\S+)\s*$")
_DIRECTIVE = re.compile(r"^\s*\.([A-Za-z0-9_]+)\s*(.*?)\s*$")
_STRING = re.compile(r'"(?:\\.|[^"\\])*"')


def _emitted(name, arguments, cursor):
    """Return exact directive bytes, or None for an unsupported initializer."""
    if name in ("align", "p2align", "balign"):
        try:
            fields = [int(item.strip(), 0) for item in arguments.split(",")]
            if name != "balign" and not 0 <= fields[0] <= 16:
                return None
            alignment = fields[0] if name == "balign" else 1 << fields[0]
            fill = fields[1] if len(fields) > 1 else 0
            if cursor is None or alignment <= 0 or alignment > 65536 or not 0 <= fill <= 255:
                return None
            size = (-cursor) % alignment
            if len(fields) > 2 and size > fields[2]:
                size = 0
            return bytes([fill]) * size
        except (ValueError, IndexError, OverflowError):
            return None
    if name in ("ascii", "asciz", "string"):
        literals = list(_STRING.finditer(arguments))
        remainder = _STRING.sub("", arguments).replace(",", "").strip()
        if not literals or remainder:
            return None
        try:
            return b"".join(_string_bytes(item.group()) + (b"\0" if name != "ascii" else b"")
                            for item in literals)
        except (ValueError, UnicodeError):
            return None
    width = {"byte": 1, "short": 2, "half": 2, "word": 4, "long": 4}.get(name)
    if width is None:
        return None
    try:
        values = [int(item.strip(), 0) for item in arguments.split(",")]
        if any(not -(1 << (width * 8 - 1)) <= value < (1 << (width * 8)) for value in values):
            return None
        return b"".join((value % (1 << (width * 8))).to_bytes(width, "little") for value in values)
    except ValueError:
        return None


def _spans(path, repo, binary):
    """Yield only contiguous verified prefixes belonging to explicit labels."""
    data, base, bias, digest, source_path = binary
    source_bytes = path.read_bytes()
    source = source_bytes.decode("utf-8", errors="strict")
    if source.encode("utf-8") != source_bytes or "\ufffd" in source:
        raise ValueError("assembly source does not round-trip as strict UTF-8")
    assembly_sha256 = hashlib.sha256(source_bytes).hexdigest()
    active = None
    for number, line in enumerate(source.splitlines(), 1):
        label = _LABEL.match(line)
        if label:
            if active is not None:
                yield active
            active = None if label[1] == "enddlabel" else {
                "label": label[2], "assembly_path": path.relative_to(repo).as_posix(),
                "assembly_line": number, "assembly_sha256": assembly_sha256,
                "address": None, "raw": bytearray(),
                "directives": [], "stopped": False, "source_path": source_path,
                "source_sha256": digest}
            continue
        if active is None or active["stopped"]:
            continue
        comment = ADDR_COMMENT.search(line)
        # Quoted strings may contain comment-looking bytes or '#'.
        body = re.sub(r'"(?:\\.|[^"\\])*"|/\*.*?\*/|#.*$',
                      lambda match: match.group() if match.group().startswith('"') else "", line).strip()
        if not body:
            continue
        directive = _DIRECTIVE.match(body)
        if not directive:
            active["stopped"] = True
            continue
        cursor = None if active["address"] is None else active["address"] + len(active["raw"])
        address = int(comment[1], 16) if comment else cursor
        if cursor is not None and address != cursor:
            active["stopped"] = True
            continue
        raw = _emitted(directive[1], directive[2], address)
        if raw is None or address is None:
            active["stopped"] = True
            continue
        offset = address - base + bias
        if offset < bias or offset + len(raw) > len(data) or data[offset:offset + len(raw)] != raw:
            active["stopped"] = True
            continue
        if active["address"] is None:
            active["address"] = address
        active["raw"].extend(raw)
        active["directives"].append({"line": number, "address": address, "offset": offset,
                                      "length": len(raw), "directive": directive[1]})
    if active is not None:
        yield active


def _source_assembly_path(repo: Path, relative: str) -> Path:
    source_path(relative)
    path = Path(relative)
    if path.is_absolute() or ".." in path.parts or path.suffix != ".s":
        raise ValueError("assembly evidence path is not a normalized source path")
    resolved = (repo / path).resolve(strict=True)
    if not resolved.is_relative_to(repo) or not resolved.is_file():
        raise ValueError("assembly evidence path escapes the selected repository")
    return resolved


def _enrich_binary_text_from_reports(inventory: dict, repository, files_root=None) -> dict:
    """Consume fresh MIPS and AST/context reports after rechecking binary proofs."""
    repo = Path(repository).resolve(strict=True)
    inputs = Path(files_root or repo / "disc/files").resolve(strict=True)
    context = inventory.get("source_context", {})
    ast_report = inventory.get("ast_consumers", {})
    from .text_ast import source_input_fingerprint
    fingerprint = source_input_fingerprint(repo)
    if (context.get("AST_value_flow_gated") is not True or
            ast_report.get("stable_source_inputs") is not True or
            ast_report.get("source_inputs_sha256") != fingerprint or
            ast_report.get("source_inputs_sha256_after") != fingerprint):
        raise ValueError("binary supplement requires fresh AST-gated source context")
    leaves = inventory.get("unresolved_data_label_leaves", [])
    if leaves != context.get("unresolved_data_label_leaves", []):
        raise ValueError("binary supplement leaf ledger differs from its source-context report")
    bases = {}
    for line in (repo / "config/overlays.txt").read_text().splitlines():
        fields = line.split()
        if len(fields) >= 2 and not fields[0].startswith("#"):
            bases[fields[0]] = int(fields[1], 0)
    binaries, spans, blocked, resolutions = {}, {}, [], []

    def original(unit):
        if unit not in binaries:
            path = "SLPM_86.053" if unit == "main" else f"CDROM/EXEDIR/{unit}.EXN"
            data, error = _read_original(inputs / path, inputs)
            base = 0x80041000 if unit == "main" else bases.get(unit)
            binaries[unit] = None if data is None or base is None else (
                data, base, 0x800 if unit == "main" else 0, hashlib.sha256(data).hexdigest(), path)
            if error:
                blocked.append({"source_path": path, "status": "input_rejected", "reason": error})
        return binaries[unit]

    def valid_proof(call):
        unit = call.get("caller_unit")
        spec = text_argument_spec(call.get("callee"), call.get("argument_index"), unit)
        binary = original(unit) if isinstance(unit, str) else None
        if (spec is None or binary is None or
                call.get("proof") != "verified_original_instructions_constant_text_argument" or
                call.get("source_sha256") != binary[3] or call.get("source_path") != binary[4]):
            return False
        if call.get("argument_register") != "a" + str(spec["index"]):
            return False
        if any(call.get(key) != spec[key] for key in ("category", "renderer", "renderer_status", "read_role")):
            return False
        conditions = call.get("callee_read_conditions", [])
        if len(conditions) != len(spec["conditions"]):
            return False
        if any({key: actual.get(key) for key in expected} != expected
               for actual, expected in zip(conditions, spec["conditions"])):
            return False
        evidence = call.get("instruction_evidence", [])
        if not evidence:
            return False
        try:
            ordered = sorted(evidence, key=lambda item: item["pc"])
            pcs = [item["pc"] for item in ordered]
            delay_slot = call["call_pc"] + 4
            if (len(pcs) != len(set(pcs)) or call["call_pc"] not in pcs or delay_slot not in pcs or
                    call.get("delay_slot_pc") != delay_slot or any(pc > delay_slot for pc in pcs)):
                return False
        except (KeyError, TypeError, ValueError):
            return False
        state = _reset()
        for item in ordered:
            try:
                raw, offset = bytes.fromhex(item["raw_hex"]), item["offset"]
                if (len(raw) != 4 or offset != item["pc"] - binary[1] + binary[2] or
                        offset < binary[2] or binary[0][offset:offset + 4] != raw):
                    return False
                if item["pc"] == call["call_pc"]:
                    word = int.from_bytes(raw, "little")
                    if _control(word) != "call":
                        return False
                    if word >> 26 == 3:
                        target_address = ((item["pc"] + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
                        return_register = 31
                    else:
                        target_value = state[(word >> 21) & 31]
                        if target_value is None:
                            return False
                        target_address = target_value[0]
                        return_register = (word >> 11) & 31
                    if target_address != call.get("callee_address"):
                        return False
                    if return_register:
                        state[return_register] = (item["pc"] + 8, ((item["pc"], offset, item["raw_hex"], item.get("assembly_line")),))
                else:
                    _ordinary(int.from_bytes(raw, "little"), state,
                              (item["pc"], offset, item["raw_hex"], item.get("assembly_line")))
            except (KeyError, TypeError, ValueError):
                return False
        if (not 0 <= spec["index"] <= 3 or state[4 + spec["index"]] is None or
                state[4 + spec["index"]][0] != call.get("argument_value")):
            return False
        for condition in conditions:
            index = condition["argument_index"]
            value = condition.get("constant_argument_value")
            status = condition.get("status")
            if status == "unknown" and value is None:
                if not 0 <= index <= 3 or state[4 + index] is not None:
                    return False
                continue
            if status != "proven_true" or not 0 <= index <= 3 or state[4 + index] is None or state[4 + index][0] != value:
                return False
            if condition["operator"] == "==" and value != condition["value"] or condition["operator"] == "!=" and value == condition["value"]:
                return False
        callee = call.get("callee_evidence", {})
        callee_unit = _record_unit(callee.get("source_path", ""))
        target = original(callee_unit) if callee_unit in (unit, "main") else None
        try:
            if callee.get("kind") == "verified_glabel_entry":
                source = _source_assembly_path(repo, callee["assembly_path"])
                lines = source.read_text(encoding="utf-8").splitlines()
                line = callee["assembly_line"]
                declaration = LABEL.match(lines[line - 1]) if isinstance(line, int) and 1 <= line <= len(lines) else None
                if declaration is None or declaration.group(1) != "glabel" or declaration.group(2) != call["callee"]:
                    return False
                first = None
                for following in lines[line:]:
                    label = LABEL.match(following)
                    if label and label.group(1) == "glabel":
                        break
                    instruction = INSTRUCTION.search(following)
                    if instruction and not instruction.group(3).startswith("."):
                        first = instruction
                        break
                if first is None or int(first.group(1), 16) != callee["pc"] or bytes.fromhex(first.group(2)) != bytes.fromhex(callee["raw_hex"]):
                    return False
            elif callee.get("kind") == "configured_function_symbol_at_verified_pc":
                relative = source_path(callee["path"])
                path = (repo / relative).resolve(strict=True)
                if not path.is_relative_to(repo) or path.suffix != ".txt" or not path.is_file():
                    return False
                lines = path.read_text(encoding="utf-8").splitlines()
                line = callee["line"]
                declaration = SYMBOL.match(lines[line - 1]) if isinstance(line, int) and 1 <= line <= len(lines) else None
                if declaration is None or declaration.group(1) != call["callee"] or int(declaration.group(2), 0) != callee["pc"]:
                    return False
            else:
                return False
            raw = bytes.fromhex(callee["raw_hex"])
            offset = callee["offset"]
            return (target is not None and callee.get("source_sha256") == target[3] and len(raw) == 4 and
                    callee.get("pc") == call.get("callee_address") and
                    offset == callee["pc"] - target[1] + target[2] and offset >= target[2] and
                    target[0][offset:offset + 4] == raw)
        except (KeyError, TypeError, ValueError, OSError, IndexError):
            return False

    def validate_leaf(leaf):
        """Recheck a complete AST word path and exact initialized data label."""
        try:
            site = leaf["source_site"]
            c_path = source_path(site["path"])
            caller_unit = source_link_unit(repo, c_path)
            role = role_for_argument(site["call"], site.get("call_argument_index"), caller_unit)
            spec = text_argument_spec(site["call"], site.get("call_argument_index"), caller_unit)
            selectors = leaf["selectors"]
            chain = leaf["evidence_chain"]
            terminal = leaf["terminal_data_label"]
            if (role is None or leaf.get("status") != "unclassified_data_label_reached_by_typed_text_path" or
                    leaf.get("category") is not None or leaf.get("category_candidate") != role[1] or
                    site.get("call_argument_index") != role[0] or
                    leaf.get("source_proof") != "typed_pointer_value_flow_to_known_read_argument" or
                    leaf.get("source_argument_role") != string_argument_role(
                        site["call"], site["call_argument_index"], caller_unit) or
                    len(chain) != len(selectors) or not chain or len(chain) > 32 or
                    chain[0].get("table") != leaf.get("source_symbol")):
                return None, "leaf_source_proof_incomplete"
            conditions = site.get("callee_read_conditions", [])
            if (len(conditions) != len(spec["conditions"]) or
                    any({key: actual.get(key) for key in expected} != expected
                        for actual, expected in zip(conditions, spec["conditions"])) or
                    any(condition.get("status") == "proven_false" for condition in conditions)):
                return None, "leaf_read_guards_require_revalidation"
            if any(index is not None and (not isinstance(index, int) or isinstance(index, bool) or index < 0)
                   for index in selectors):
                return None, "leaf_selector_unresolved_or_invalid"
            certainty = "possible_dynamic_index" if None in selectors else "statically_selected_pointer_path"
            if leaf.get("selection_certainty") != certainty:
                return None, "leaf_selection_certainty_mismatch"
            if caller_unit is None:
                return None, "leaf_source_unit_unavailable"
            previous = None
            checked_edges = []
            for index, edge in enumerate(chain):
                edge_path = _source_assembly_path(repo, edge["path"])
                edge_unit = _unit(edge_path, repo)
                if index == 0 and edge_unit not in (caller_unit, "main"):
                    return None, "leaf_root_unit_mismatch"
                if index and (previous["target"] != edge["table"] or
                              previous["target_unit"] != edge_unit):
                    return None, "leaf_pointer_chain_disconnected"
                if (edge.get("requested_index") != selectors[index] or
                        not isinstance(edge.get("slot_index"), int) or edge["slot_index"] < 0 or
                        selectors[index] is not None and edge["slot_index"] != selectors[index]):
                    return None, "leaf_pointer_slot_mismatch"
                source_bytes = edge_path.read_bytes()
                assembly_hash = hashlib.sha256(source_bytes).hexdigest()
                if assembly_hash != edge.get("assembly_sha256"):
                    return None, "leaf_pointer_assembly_changed"
                source_text = source_bytes.decode("utf-8", errors="strict")
                if source_text.encode("utf-8") != source_bytes or "\ufffd" in source_text:
                    return None, "leaf_pointer_assembly_encoding_invalid"
                lines = source_text.splitlines()
                line_number = edge["line"]
                if not isinstance(line_number, int) or not 1 <= line_number <= len(lines):
                    return None, "leaf_pointer_line_out_of_bounds"
                line = lines[line_number - 1]
                current_table = None
                for prior_line in lines[:line_number]:
                    prior_label = re.fullmatch(r"\s*dlabel\s+([A-Za-z_][A-Za-z_0-9]*)\s*", prior_line)
                    if prior_label:
                        current_table = prior_label.group(1)
                    elif re.fullmatch(r"\s*enddlabel\s+\S+\s*", prior_line):
                        current_table = None
                if current_table != edge.get("table"):
                    return None, "leaf_pointer_table_label_mismatch"
                body = re.sub(r"/\*.*?\*/", "", line).split("#", 1)[0].strip()
                word = re.fullmatch(r"\.word\s+([A-Za-z_][A-Za-z_0-9]*)", body)
                comment = ADDR_COMMENT.search(line)
                encoded_comment = re.fullmatch(
                    r"/\*\s*[0-9A-Fa-f]+\s+[0-9A-Fa-f]{8}\s+([0-9A-Fa-f]{8})\s*\*/",
                    comment.group(0), re.IGNORECASE) if comment else None
                raw_word = bytes.fromhex(edge["source_word_hex"])
                if (word is None or word.group(1) != edge.get("target") or comment is None or
                        int(comment.group(1), 16) != edge.get("address") or len(raw_word) != 4 or
                        encoded_comment is None or bytes.fromhex(encoded_comment.group(1)) != raw_word or
                        int.from_bytes(raw_word, "little") != edge.get("target_address")):
                    return None, "leaf_pointer_source_row_mismatch"
                table_start = max(index for index, prior_line in enumerate(lines[:line_number], 1)
                                  if re.fullmatch(r"\s*dlabel\s+" + re.escape(edge["table"]) +
                                                  r"\s*", prior_line))
                first_word_address = None
                for following_line in lines[table_start:]:
                    if re.fullmatch(r"\s*(?:dlabel|enddlabel)\s+\S+\s*", following_line):
                        break
                    follow_body = re.sub(r"/\*.*?\*/", "", following_line).split("#", 1)[0].strip()
                    follow_comment = ADDR_COMMENT.search(following_line)
                    if re.fullmatch(r"\.word\s+.+", follow_body) and follow_comment:
                        first_word_address = int(follow_comment.group(1), 16)
                        break
                if (first_word_address is None or edge["address"] < first_word_address or
                        (edge["address"] - first_word_address) % 4 or
                        (edge["address"] - first_word_address) // 4 != edge["slot_index"]):
                    return None, "leaf_pointer_slot_address_mismatch"
                source_binary = original(edge_unit)
                if source_binary is None:
                    return None, "leaf_pointer_source_binary_missing"
                offset = edge["address"] - source_binary[1] + source_binary[2]
                if (offset != edge.get("source_offset") or offset < source_binary[2] or
                        offset + 4 > len(source_binary[0]) or
                        source_binary[0][offset:offset + 4] != raw_word):
                    return None, "leaf_pointer_binary_word_mismatch"
                target_unit = edge.get("target_unit")
                if target_unit not in (edge_unit, "main"):
                    return None, "leaf_pointer_target_unit_mismatch"
                target_binary = original(target_unit)
                if target_binary is None:
                    return None, "leaf_target_binary_missing"
                target_offset = edge["target_address"] - target_binary[1] + target_binary[2]
                if target_offset < target_binary[2] or target_offset >= len(target_binary[0]):
                    return None, "leaf_pointer_target_out_of_bounds"
                previous = edge
                checked_edges.append(edge)
            if (terminal.get("unit") != previous.get("target_unit") or
                    terminal.get("label") != previous.get("target") or
                    terminal.get("address") != previous.get("target_address")):
                return None, "leaf_terminal_label_disconnected"
            terminal_path = _source_assembly_path(repo, terminal["assembly_path"])
            terminal_bytes = terminal_path.read_bytes()
            if hashlib.sha256(terminal_bytes).hexdigest() != terminal.get("assembly_sha256"):
                return None, "leaf_terminal_assembly_changed"
            terminal_text = terminal_bytes.decode("utf-8", errors="strict")
            terminal_lines = terminal_text.splitlines()
            label_line = terminal.get("assembly_line")
            if (not isinstance(label_line, int) or not 1 <= label_line <= len(terminal_lines) or
                    not re.fullmatch(r"\s*dlabel\s+" + re.escape(terminal["label"]) + r"\s*",
                                     terminal_lines[label_line - 1])):
                return None, "leaf_terminal_label_line_mismatch"
            target_binary = original(terminal["unit"])
            if target_binary is None:
                return None, "leaf_terminal_binary_missing"
            target_offset = terminal["address"] - target_binary[1] + target_binary[2]
            if (terminal.get("binary_path") != target_binary[4] or
                    terminal.get("binary_offset") != target_offset or
                    terminal.get("binary_file_size") != len(target_binary[0]) or
                    terminal.get("binary_sha256") != target_binary[3] or
                    target_offset < target_binary[2] or target_offset >= len(target_binary[0])):
                return None, "leaf_terminal_binary_evidence_mismatch"
            candidates = [span for span in spans.get((terminal["unit"], terminal["address"]), [])
                          if span.get("label") == terminal["label"] and
                          span.get("assembly_path") == terminal["assembly_path"] and
                          span.get("assembly_line") == terminal["assembly_line"] and
                          span.get("assembly_sha256") == terminal["assembly_sha256"]]
            if len(candidates) != 1:
                return None, "leaf_terminal_span_ambiguous_or_missing"
            return (candidates[0], role), None
        except (KeyError, TypeError, ValueError, OSError, UnicodeError, IndexError):
            return None, "leaf_evidence_malformed_or_unreadable"

    # Revoke semantics owned by the previous leaf pass before applying fresh
    # evidence. Keep the byte extent and stable record identity as history.
    for record in inventory.get("records", []):
        record["source_contexts"] = [item for item in record.get("source_contexts", [])
                                     if not item.get("source_leaf_id")]
        record["binary_initial_renderer_views"] = [
            item for item in record.get("binary_initial_renderer_views", [])
            if not item.get("source_leaf_id") and not item.get("source_context_owned")]
        if record.get("category_basis") in {
                "verified initialized label reached by typed C pointer path",
                "unique typed C consumer through verified pointer path",
                "multiple typed C consumer roles through verified pointer paths"}:
            record["category"] = "unknown"
            record["category_basis"] = "leaf-derived semantic attribution reset before fresh validation"

    calls = inventory.get("mips_context", {}).get("unresolved_constant_targets", [])
    units = {unit for call in calls for unit in (call.get("caller_unit"), "main") if isinstance(unit, str)}
    units.update(leaf.get("terminal_data_label", {}).get("unit") for leaf in leaves
                 if isinstance(leaf.get("terminal_data_label", {}).get("unit"), str))
    for leaf in leaves:
        for edge in leaf.get("evidence_chain", []):
            try:
                edge_path = _source_assembly_path(repo, edge["path"])
                units.add(_unit(edge_path, repo))
            except (KeyError, ValueError, OSError):
                continue
    for path in sorted((repo / "asm").rglob("*.s")):
        relative = path.relative_to(repo).as_posix()
        if "/data/" not in relative and not relative.startswith("asm/data/"):
            continue
        try:
            path = _source_assembly_path(repo, relative)
        except (OSError, ValueError) as exc:
            blocked.append({"source_path": relative, "status": "assembly_span_rejected",
                            "reason": type(exc).__name__})
            continue
        unit = _unit(path, repo)
        binary = original(unit) if unit in units else None
        if binary is None:
            continue
        try:
            parsed_spans = _spans(path, repo, binary)
            for span in parsed_spans:
                if span["address"] is not None:
                    spans.setdefault((unit, span["address"]), []).append(span)
        except (OSError, UnicodeError, ValueError) as exc:
            blocked.append({"source_path": relative, "status": "assembly_span_rejected",
                            "reason": type(exc).__name__})
    existing = {(record["source"]["path"], record["source"]["address"], record["raw_hex"]): record
                for record in inventory["records"]}
    added = 0
    for call in calls:
        resolution = {key: call.get(key) for key in ("caller_unit", "call_pc", "callee", "argument_value")}
        resolutions.append(resolution)
        if not valid_proof(call):
            resolution["status"] = "call_evidence_requires_revalidation"
            blocked.append(resolution.copy())
            continue
        address = call.get("argument_value")
        matches = spans.get((call["caller_unit"], address), []) or spans.get(("main", address), [])
        if not matches:
            resolution["status"] = "no_verified_exact_data_label"
            continue
        if len({bytes(span["raw"]) for span in matches}) != 1:
            resolution["status"] = "ambiguous_label_spans"
            continue
        span = matches[0]
        raw_span = bytes(span["raw"])
        if not raw_span or raw_span[0] == 0:
            resolution["status"] = "initially_zero_runtime_buffer"
            continue
        end = raw_span.find(b"\0")
        if end < 0:
            resolution["status"] = "no_terminator_in_verified_label_prefix"
            continue
        raw = raw_span[:end + 1]
        unit = _record_unit(span["source_path"])
        data, base, bias, digest, source_binary_path = original(unit)
        offset = address - base + bias
        key = (source_binary_path, address, raw.hex())
        record = existing.get(key)
        if record is not None and (record["source"].get("sha256") != digest or
                                   record["source"].get("file_size") != len(data) or
                                   record["source"].get("offset") != offset or
                                   record.get("raw_length") != len(raw) or
                                   not record.get("extent_status", "").startswith("verified")):
            resolution["status"] = "existing_extent_requires_revalidation"
            blocked.append(resolution.copy())
            continue
        if record is None:
            record = {"id": f"{source_binary_path}:{offset:08x}:{hashlib.sha256(raw).hexdigest()[:12]}",
                      "category": role_for_argument(call["callee"], call["argument_index"], call["caller_unit"])[1],
                      "category_basis": "verified data-label initial string with potential static text argument",
                      "raw_hex": raw.hex(), "raw_length": len(raw),
                      "extent_status": "verified_initialized_data_label_string",
                      "source": {"path": source_binary_path, "address": address, "offset": offset,
                                 "length": len(raw), "end_offset": offset + len(raw),
                                 "file_size": len(data), "sha256": digest},
                      **_decoded_view(raw, True), "assembly_sites": [], "mips_contexts": [],
                      "initial_view_only": True, "runtime_read_length_proven": False,
                      "terminator": {"kind": "nul", "relative_offset": end, "included_in_raw": True}}
            inventory["records"].append(record)
            existing[key] = record
            added += 1
        site = {key: value for key, value in span.items() if key not in ("raw", "stopped")}
        site["labels"] = [span["label"]]
        site["verified_initialized_prefix_length"] = len(raw_span)
        if site not in record.setdefault("assembly_sites", []):
            record["assembly_sites"].append(site)
        context = {**call, "status": "potential_static_callsite"}
        if context not in record.setdefault("mips_contexts", []):
            record["mips_contexts"].append(context)
        spec = text_argument_spec(call["callee"], call["argument_index"], call["caller_unit"])
        renderer = spec["renderer"]
        views = record.setdefault("binary_initial_renderer_views", [])
        view = {"renderer": renderer, "renderer_status": spec["renderer_status"],
                "callee_read_conditions": call.get("callee_read_conditions", []), "call_pc": call["call_pc"],
                "view": tokenize_fixed_text(raw[:-1], renderer), "runtime_read_length_proven": False}
        if view not in views:
            views.append(view)
        resolution.update(status="recovered_verified_label_initial_string", record_id=record["id"])
    leaf_resolutions = []
    leaf_added = 0
    for leaf in leaves:
        resolution = {"leaf_id": leaf.get("id"),
                      "terminal_data_label": leaf.get("terminal_data_label", {}).get("label"),
                      "selection_certainty": leaf.get("selection_certainty")}
        leaf_resolutions.append(resolution)
        call_name = leaf.get("source_site", {}).get("call")
        site = leaf.get("source_site", {})
        caller_unit = source_link_unit(repo, site.get("path", ""))
        if role_for_argument(call_name, site.get("call_argument_index"), caller_unit) is None:
            resolution["status"] = "consumer_role_not_supported"
            continue
        validated, error = validate_leaf(leaf)
        if error:
            resolution["status"] = error
            blocked.append({**resolution, "status": error})
            continue
        span, role = validated
        spec = text_argument_spec(call_name, site.get("call_argument_index"), caller_unit)
        raw_span = bytes(span["raw"])
        if not raw_span or raw_span[0] == 0:
            resolution["status"] = "initially_zero_runtime_buffer"
            continue
        end = raw_span.find(b"\0")
        if end < 0:
            resolution["status"] = "no_terminator_in_verified_label_prefix"
            continue
        raw = raw_span[:end + 1]
        terminal = leaf["terminal_data_label"]
        unit = terminal["unit"]
        data, base, bias, digest, source_binary_path = original(unit)
        offset = terminal["address"] - base + bias
        key = (source_binary_path, terminal["address"], raw.hex())
        record = existing.get(key)
        if record is not None and (record["source"].get("sha256") != digest or
                                   record["source"].get("file_size") != len(data) or
                                   record["source"].get("offset") != offset or
                                   record.get("raw_length") != len(raw) or
                                   bytes.fromhex(record.get("raw_hex", "")) != raw or
                                   not record.get("extent_status", "").startswith("verified")):
            resolution["status"] = "existing_extent_requires_revalidation"
            blocked.append(resolution.copy())
            continue
        if record is None:
            record = {"id": f"{source_binary_path}:{offset:08x}:{hashlib.sha256(raw).hexdigest()[:12]}",
                      "category": role[1],
                      "category_basis": "verified initialized label reached by typed C text-pointer path",
                      "raw_hex": raw.hex(), "raw_length": len(raw),
                      "extent_status": "verified_initialized_data_label_string",
                      "source": {"path": source_binary_path, "address": terminal["address"],
                                 "offset": offset, "length": len(raw), "end_offset": offset + len(raw),
                                 "file_size": len(data), "sha256": digest},
                      **_decoded_view(raw, True), "assembly_sites": [],
                      "initial_view_only": True, "runtime_read_length_proven": False,
                      "terminator": {"kind": "nul", "relative_offset": end, "included_in_raw": True}}
            inventory["records"].append(record)
            existing[key] = record
            added += 1
            leaf_added += 1
        site = {key: value for key, value in span.items() if key not in ("raw", "stopped")}
        site["labels"] = [terminal["label"]]
        site["verified_initialized_prefix_length"] = len(raw_span)
        if site not in record.setdefault("assembly_sites", []):
            record["assembly_sites"].append(site)
        source_context = {
            "source_site": leaf["source_site"], "table_symbol": leaf["source_symbol"],
            "category": role[1], "category_basis": "known typed C read argument through verified pointer path",
            "category_candidate": None, "category_candidate_basis": None,
            "evidence_chain": leaf["evidence_chain"],
            "proof": "fresh_AST_pointer_word_path_reaches_verified_initialized_data_label",
            "selection_certainty": leaf["selection_certainty"],
            "source_leaf_id": leaf["id"], "source_proof": leaf["source_proof"]}
        contexts = record.setdefault("source_contexts", [])
        if source_context not in contexts:
            contexts.append(source_context)
        roles = {item.get("category") for item in contexts if item.get("category")}
        if len(roles) == 1 and record.get("category") in ("unknown", role[1]):
            record["category"] = next(iter(roles))
            record["category_basis"] = "unique typed C consumer through verified pointer path"
        elif len(roles) > 1 and record.get("category_basis", "").startswith("unique typed C consumer"):
            record["category"] = "unknown"
            record["category_basis"] = "multiple typed C consumer roles through verified pointer paths"
        view = {"renderer": role[2], "renderer_status": spec["renderer_status"],
                "callee_read_conditions": leaf["source_site"].get("callee_read_conditions", []),
                "source_leaf_id": leaf["id"],
                "view": tokenize_fixed_text(raw[:-1], role[2]), "runtime_read_length_proven": False,
                "selection_certainty": leaf["selection_certainty"]}
        views = record.setdefault("binary_initial_renderer_views", [])
        if view not in views:
            views.append(view)
        resolution.update(status="recovered_verified_label_initial_string", record_id=record["id"])
    # A recovered extent becomes a normal verified text label on the next
    # source-context pass. Rebuild its views from those fresh contexts even
    # when it no longer appears in the unresolved-leaf ledger.
    for record in inventory["records"]:
        if (record.get("initial_view_only") is not True or
                record.get("extent_status") != "verified_initialized_data_label_string"):
            continue
        source = record["source"]
        binary = original(_record_unit(source["path"]))
        raw = bytes.fromhex(record["raw_hex"])
        offset = source["offset"]
        if (binary is None or source["sha256"] != binary[3] or
                source["file_size"] != len(binary[0]) or
                source["address"] - binary[1] + binary[2] != offset or
                not binary[2] <= offset <= len(binary[0]) - len(raw) or
                record["raw_length"] != len(raw) or source["length"] != len(raw) or
                binary[0][offset:offset + len(raw)] != raw or
                not raw.endswith(b"\0") or b"\0" in raw[:-1]):
            continue
        fresh_contexts = [item for item in record.get("source_contexts", [])
                          if not item.get("source_leaf_id") and item.get("category") and
                          item.get("proof") in {
                              "same_link_unit_word_pointer_targets_verified_text_label",
                              "same_link_unit_or_proven_main_global_C_symbol_reference"}]
        roles = {item["category"] for item in fresh_contexts}
        if (len(roles) == 1 and record.get("category") == "unknown" and
                record.get("context_category") in roles and
                record.get("context_category_basis") == "unique source-proven consumer role"):
            record["category"] = record["context_category"]
            record["category_basis"] = record["context_category_basis"]
        for context in fresh_contexts:
            call = context["source_site"].get("call")
            caller_unit = source_link_unit(repo, context["source_site"].get("path", ""))
            role = role_for_argument(call, context["source_site"].get("call_argument_index"), caller_unit)
            if role is None or role[1] != context["category"]:
                continue
            spec = text_argument_spec(call, context["source_site"].get("call_argument_index"), caller_unit)
            view = {"renderer": role[2], "renderer_status": spec["renderer_status"],
                    "callee_read_conditions": context["source_site"].get("callee_read_conditions", []),
                    "source_context_owned": True,
                    "source_site": context["source_site"],
                    "selection_certainty": context["selection_certainty"],
                    "view": tokenize_fixed_text(raw[:-1], role[2]),
                    "runtime_read_length_proven": False}
            views = record.setdefault("binary_initial_renderer_views", [])
            if view not in views:
                views.append(view)
    inventory["binary_text_supplement"] = {
        "records_added": added, "calls_considered": len(calls), "resolutions": resolutions,
        "pointer_leaves_considered": len(leaves), "pointer_leaf_records_added": leaf_added,
        "pointer_leaf_resolutions": leaf_resolutions,
        "blocked": blocked, "complete": False,
        "limitations": ["Potential static argument evidence does not prove execution or runtime read length.",
                        "Only contiguous supported initialized directives at exact data-label starts are read.",
                        "Initially zero storage remains a runtime-buffer candidate; no empty translation is inferred."]}
    return inventory


def enrich_binary_text(inventory: dict, repository=REPOSITORY, files_root=None) -> dict:
    """Regenerate source context and MIPS proofs, then recover bounded initial views."""
    repo = Path(repository).resolve(strict=True)
    inputs = Path(files_root or repo / "disc/files").resolve(strict=True)
    from .text_context import enrich_source_context
    enrich_source_context(inventory, repo, inputs, use_ast=True)
    enrich_mips_context(inventory, repo, inputs)
    return _enrich_binary_text_from_reports(inventory, repo, inputs)
