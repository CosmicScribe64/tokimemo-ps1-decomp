"""Source-pinned runtime script patch contracts and unresolved writes (T-28001).

This ledger never changes game data. A protected byte slot is not a translation
boundary: changing its position or width requires changing the runtime patch.
Generic AST results are candidate writes, not proof of their ultimate target.
"""
from __future__ import annotations

import hashlib
import re
import struct
import subprocess
from pathlib import Path

from .text_source import REPOSITORY, _lex_c_tokens, _mask_c_comments, _mask_c_literals, _read_original, _string_bytes
from .text import tokenize_text

SOURCE_PINS = {
    "src/ovl/DATE/80149310.c": "d15bba6533f174b6be21f14af5d1c804c2e410738095e379d25d4d0bed8d476a",
    "src/ovl/DATE2/80137360.c": "31353b53f7c439fb31bef46e1d9b5df7ffbab45de359d510273652dd6f5dd7dd",
    "src/ovl/SHUGAKU/801378B0.c": "7535ca51fa77410ea4f6be8b1c01bc6a110d8c12cd86a9fe9782a690766720e1",
    "src/ovl/SHUGAKU/80136380.c": "75915b36e53189a482d1d894f9ddfbc6fa6c19c3849d4bdf82c26496fd974b80",
    "src/ovl/SHOUGATU/801330C0.c": "d18cbcb57fee183767219fb7c9e2e80e3ba53d4a08e3c2bc04e23dce247926fd",
    "src/ovl/SHOUGATU/80132550.c": "0ab133711eea89c078acde9f4e07fa06c72567df76d3cb998d734d9d27c0d21e",
    "src/ovl/SHOUGATU/80132000.c": "cfc9c16f59ad19735be9b29ec7b0aa026eb8ffd98c83122aebc66c6f4b252a92",
}
MUTATION_SPECS = [
    {"id": "date-birthday-initial-glyph", "source": "src/ovl/DATE/80149310.c",
     "function": "func_80149B78", "array": "buf", "write_lines": [247, 248], "length": 2,
     "script": "CDROM/SERIFU/G_BIRTH.BIN", "initializer_source": "src/ovl/DATE/80149310.c",
     "initializer": "func_80149310", "indices": list(range(11)) + [12], "group": 14, "line": 0,
     "conditions": ["D_80122CDC == 0", "root selected by func_801496C4 current_girl switch"],
     "selection": "explicit_switch_roots_with_conditional_nozomi_alternative",
     "evidence": ["src/ovl/DATE/80149310.c:72-112,122-124,150-211,241-249"],
     "pins": ["src/ovl/DATE/80149310.c"]},
    {"id": "date2-birthday-initial-glyph", "source": "src/ovl/DATE2/80137360.c",
     "function": "func_801378A4", "array": "buf", "write_lines": [124, 125], "length": 2,
     "script": "CDROM/SERIFU/G_BIRTH.BIN", "initializer_source": "src/ovl/DATE2/80137360.c",
     "initializer": "func_80137360", "indices": [1], "group": 14, "line": 0,
     "conditions": ["D_80122CDC == 0", "talk_text_table selected as D_8013A774 by func_801376F0"],
     "selection": "explicit_fixed_root", "evidence": ["src/ovl/DATE2/80137360.c:7-47,67-84,118-126"],
     "pins": ["src/ovl/DATE2/80137360.c"]},
    {"id": "school-trip-initial-line-shortening", "source": "src/ovl/SHUGAKU/801378B0.c",
     "function": "func_8013799C", "array": "buf", "write_lines": [32, 33, 34], "length": 3,
     "script": "CDROM/SERIFU/SHUGAKU.BIN", "initializer_source": "src/ovl/SHUGAKU/80136380.c",
     "initializer": "func_80136380", "indices": [11], "group": 2, "line": 0,
     "conditions": ["game_state.player_flags.b[1] & 0xF", "game_state.substep == 0"],
     "selection": "explicit_fixed_root", "evidence": ["src/ovl/SHUGAKU/801378B0.c:25-35,136-141",
                                                         "src/ovl/SHUGAKU/80136380.c:4-43"],
     "pins": ["src/ovl/SHUGAKU/801378B0.c", "src/ovl/SHUGAKU/80136380.c"]},
    {"id": "new-year-selected-line-and-command", "source": "src/ovl/SHOUGATU/801330C0.c",
     "function": "func_80133B44", "array": "sp40", "write_lines": [291, 292, 293, 294], "length": 3,
     "script": "CDROM/SERIFU/SHOGATU.BIN", "initializer_source": "src/ovl/SHOUGATU/80132000.c",
     "initializer": "func_80132000", "runtime_root": "D_80143AF4", "group": 0, "line": 0,
     "command_value": 7,
     "conditions": ["game_state.current_girl != GIRL_SHIORI", "game_state.substep == 0"],
     "selection": "source_assignment_union_requires_runtime_selector_proof",
     "evidence": ["src/ovl/SHOUGATU/801330C0.c:275-295", "src/ovl/SHOUGATU/80132550.c:133-202"],
     "pins": ["src/ovl/SHOUGATU/801330C0.c", "src/ovl/SHOUGATU/80132550.c", "src/ovl/SHOUGATU/80132000.c"]},
]
NAME_BUFFERS = {"name_code_player_form", "name_code_family", "name_code_given", "name_code_girl_short",
                "girl_name_buf", "D_800CA23C", "D_800CA248", "D_800CA250", "D_800CA25C", "D_800CA280"}
ROOT_SEEDS = {"talk_text_table", "talk_command_table", "D_800CA13C", "D_800CA140", "D_800CA2D0"}
WRITE_CALLS = {"strcpy", "strncpy", "strcat", "strncat", "sprintf", "snprintf", "vsprintf", "vsnprintf",
               "memcpy", "memmove", "memset", "get_p_name", "get_g_name"}
STRING_OUTPUT_CALLS = WRITE_CALLS - {"memcpy", "memmove", "memset"}


def _sha(data):
    return hashlib.sha256(data).hexdigest()


def _source_path(repo, relative):
    path = (repo / relative).resolve(strict=True)
    if not path.is_relative_to(repo) or not path.is_file():
        raise ValueError("mutation source is outside the selected repository")
    return path


def _function_body(text, name):
    tokens = list(_lex_c_tokens(text))
    code = _mask_c_literals(_mask_c_comments(text), tokens)
    match = re.search(r"\b" + re.escape(name) + r"\s*\([^;{}]*\)\s*\{", code)
    if match is None:
        raise ValueError("source function not found")
    start = code.find("{", match.start())
    depth = 1
    stop = start + 1
    while depth and stop < len(code):
        depth += (code[stop] == "{") - (code[stop] == "}")
        stop += 1
    if depth:
        raise ValueError("unbalanced source function")
    return code[start:stop], text.count("\n", 0, start) + 1


def _initializer(repo, spec):
    source = _source_path(repo, spec["initializer_source"])
    body, first_line = _function_body(source.read_text(), spec["initializer"])
    assignments = [(m.group(1), int(m.group(2), 16)) for m in
                   re.finditer(r"\b([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", body)]
    if not assignments or len(assignments) % 3:
        raise ValueError("initializer is not a bounded parallel table triad")
    count = len(assignments) // 3
    triads = [tuple(assignments[i + count * column][1] for column in range(3)) for i in range(count)]
    return assignments[:count], triads


def _replacement(catalog, repo, spec):
    matches = [site for site in catalog.get("source_inventory", {}).get("c_literal_sites", [])
               if site["source_site"].get("path") == spec["source"] and
               site["source_site"].get("function") == spec["function"] and
               site["source_site"].get("array_symbol") == spec["array"]]
    if len(matches) != 1:
        raise ValueError("replacement requires one source-inventory literal site")
    site = matches[0]
    raw = bytes.fromhex(site["raw_hex"])
    capacity = site["source_site"].get("array_length")
    if capacity != len(raw) + 1 or spec["length"] > capacity:
        raise ValueError("replacement literal capacity does not establish requested bytes")
    text = _source_path(repo, spec["source"]).read_text()
    actual = []
    for token in _lex_c_tokens(text):
        if token["kind"] != "string" or text.count("\n", 0, token["start"]) + 1 != site["source_site"]["line"]:
            continue
        prefix = text[max(0, token["start"] - 120):token["start"]]
        if re.search(r"\b" + re.escape(spec["array"]) + r"\s*\[\s*" + str(capacity) + r"\s*\]\s*=\s*$", prefix):
            actual.append(_string_bytes(token["literal"]))
    if actual != [raw]:
        raise ValueError("replacement source literal does not agree with source-inventory bytes")
    return {"c_literal_site_id": site["id"], "source_path": spec["source"],
            "source_line": site["source_site"]["line"], "array_symbol": spec["array"],
            "array_capacity": capacity, "raw_hex": (raw + b"\0")[:spec["length"]].hex(),
            "includes_terminal_nul": spec["length"] == capacity}


def _target(record, section, data, base, length, command_value=None):
    text = record["text"]
    raw = bytes.fromhex(text["raw_hex"])
    offset = text["offset"]
    if (record.get("source_sha256") != _sha(data) or text.get("length") != len(raw) or
            data[offset:offset + len(raw) + 1] != raw + b"\0" or not 0 <= offset <= len(data) - length):
        raise ValueError("target text extent or patch bytes disagree with original input")
    group_slot = section["text_table"] - base + record["group"] * 4
    if not 0 <= group_slot <= len(data) - 4:
        raise ValueError("target group slot outside input")
    group_pointer = struct.unpack_from("<I", data, group_slot)[0]
    line_slot = group_pointer - base + record["line"] * 4
    if not 0 <= line_slot <= len(data) - 4:
        raise ValueError("target line slot outside input")
    pointer = struct.unpack_from("<I", data, line_slot)[0]
    if pointer != base + offset or line_slot != text["pointer_offset"] or text["pointer_value"] != pointer:
        raise ValueError("original linked tables do not resolve to target record")
    fits = length <= len(raw) + 1
    result = {"record_id": record["id"], "section": section["id"], "group": record["group"], "line": record["line"],
              "source_path": record["source_path"], "source_sha256": record["source_sha256"],
              "offset": offset, "relative_byte_offset": 0, "length": length,
              "original_raw_hex": data[offset:offset + length].hex(),
              "write_fits_exported_extent": fits, "exported_text_capacity": len(raw) + 1,
              "pointer_evidence": [{"offset": group_slot, "value": group_pointer}, {"offset": line_slot, "value": pointer}],
              "translation_requires_patch_aware_relocation": True}
    if command_value is not None:
        command = record["command"]
        command_group_slot = section["command_table"] - base + record["group"] * 4
        if not 0 <= command_group_slot <= len(data) - 4:
            raise ValueError("command group slot outside input")
        command_pointer = struct.unpack_from("<I", data, command_group_slot)[0]
        command_offset = command_pointer - base + record["line"]
        if (not 0 <= command_offset < len(data) or command_offset != command["offset"] or
                data[command_offset] != command["value"]):
            raise ValueError("command pointer or byte differs from original input")
        result["command_patch"] = {"offset": command_offset, "original_value": command["value"], "replacement_value": command_value,
                                   "pointer_evidence": {"offset": command_group_slot, "value": command_pointer}}
    return result


def _conditional_view(record, replacement, conditions, command_value=None):
    """Derive one bounded runtime C-string view without modifying its source."""
    original = bytes.fromhex(record["text"]["raw_hex"]) + b"\0"
    patch = bytes.fromhex(replacement["raw_hex"])
    if len(patch) > len(original):
        return {"status": "target_capacity_unproved"}
    storage = patch + original[len(patch):]
    terminator = storage.find(b"\0")
    if terminator < 0:
        return {"status": "original_terminator_overwritten", "storage_raw_hex": storage.hex()}
    raw = storage[:terminator]
    command = record["command"]["value"] if command_value is None else command_value
    return {"status": "conditional_derived_view", "source_record_id": record["id"],
            "conditions": list(conditions), "command_value": command,
            "source_storage_unchanged": True, "storage_raw_hex": storage.hex(),
            "ignored_storage_after_nul_hex": storage[terminator + 1:].hex(),
            "text": {"raw_hex": raw.hex(), "length": len(raw), "terminator_hex": "00",
                     **tokenize_text(raw, command)}}


def enrich_script_mutations(catalog, repository=REPOSITORY, files_root=None, *, specs=None, source_pins=None,
                            scan_candidates=True, candidate_files=None):
    """Attach validated patch slots and a separate unresolved source-write ledger."""
    repo = Path(repository).resolve(strict=True)
    inputs = Path(files_root or repo / "disc/files").resolve(strict=True)
    specs = MUTATION_SPECS if specs is None else specs
    pins = SOURCE_PINS if source_pins is None else source_pins
    rules, blocked = [], []
    for spec in specs:
        rule = {"id": spec["id"], "conditions": spec["conditions"], "selection_status": spec["selection"],
                "source": {"path": spec["source"], "function": spec["function"], "write_lines": spec["write_lines"]},
                "evidence": spec.get("evidence", []), "targets": []}
        rules.append(rule)
        try:
            for path in spec["pins"]:
                if path not in pins or _sha(_source_path(repo, path).read_bytes()) != pins[path]:
                    raise ValueError("source differs from pinned mutation proof")
            rule["source_pins"] = {path: pins[path] for path in spec["pins"]}
            roots, triads = _initializer(repo, spec)
            rule["replacement"] = _replacement(catalog, repo, spec)
            indices = spec.get("indices")
            if indices is None:
                indices = set()
                references = []
                root_map = {symbol: i for i, (symbol, _) in enumerate(roots)}
                for path in sorted((repo / Path(spec["source"]).parent).glob("*.c")):
                    source = path.read_text()
                    code = _mask_c_literals(_mask_c_comments(source), list(_lex_c_tokens(source)))
                    for match in re.finditer(r"\b" + re.escape(spec["runtime_root"]) + r"\s*=\s*([A-Za-z_]\w*)\s*;", code):
                        if match.group(1) in root_map:
                            index = root_map[match.group(1)]
                            indices.add(index)
                            references.append({"path": path.relative_to(repo).as_posix(),
                                               "line": source.count("\n", 0, match.start()) + 1, "table_index": index,
                                               "source_sha256": _sha(path.read_bytes())})
                indices = sorted(indices)
                rule["root_assignment_candidates"] = references
                blocked.append({"rule_id": spec["id"], "status": "runtime_root_selection_not_fully_proved"})
            files = [item for item in catalog["files"] if item["path"] == spec["script"] and "script" in item]
            if len(files) != 1:
                raise ValueError("required linked script is absent or ambiguous")
            parsed = files[0]["script"]
            data, error = _read_original(inputs / spec["script"], inputs)
            if error or data is None or _sha(data) != parsed["source"]["sha256"]:
                raise ValueError("script original input missing, unsafe, or checksum differs")
            layout = parsed["layout"]
            for index in indices:
                expected = triads[index]
                sections = [section for section in layout["sections"]
                            if tuple(section[key] for key in ("text_table", "command_table", "voice_table")) == expected]
                if len(sections) != 1:
                    raise ValueError("initializer root does not identify one exported section")
                section = sections[0]
                records = [record for record in parsed["records"] if record["section"] == section["id"] and
                           record["group"] == spec["group"] and record["line"] == spec["line"] and "text" in record]
                if len(records) != 1:
                    raise ValueError("patch target is not one exported text record")
                if records[0].get("source_path") != spec["script"]:
                    raise ValueError("patch target source path differs from the selected script")
                target = _target(records[0], section, data, layout["load_address"], spec["length"], spec.get("command_value"))
                target["initializer_table_index"] = index
                if spec["selection"] != "source_assignment_union_requires_runtime_selector_proof":
                    target["conditional_runtime_view"] = _conditional_view(records[0], rule["replacement"],
                                                                             spec["conditions"], spec.get("command_value"))
                    if target["conditional_runtime_view"]["status"] == "original_terminator_overwritten":
                        blocked.append({"rule_id": spec["id"], "record_id": target["record_id"],
                                        "status": "runtime_terminator_outside_exported_extent"})
                rule["targets"].append(target)
                if not target["write_fits_exported_extent"]:
                    blocked.append({"rule_id": spec["id"], "record_id": target["record_id"], "status": "target_allocation_capacity_unproved"})
            rule["status"] = "source_pinned_slots_validated"
        except (OSError, ValueError, KeyError, TypeError, IndexError) as exc:
            rule["status"] = "unresolved_mutation"
            blocked.append({"rule_id": spec["id"], "status": "mutation_proof_failed", "reason": str(exc)})
    candidate_report = inventory_mutation_candidates(repo, catalog, candidate_files) if scan_candidates else {
        "status": "not_requested", "candidates": [], "blocked": []}
    known_sites = {(spec["source"], line) for spec in specs for line in spec["write_lines"]}
    for candidate in candidate_report["candidates"]:
        candidate["known_patch_rule_ids"] = [spec["id"] for spec in specs
                                             if candidate["source_path"] == spec["source"] and candidate["line"] in spec["write_lines"]]
    unresolved = [candidate["id"] for candidate in candidate_report["candidates"]
                  if (candidate["source_path"], candidate["line"]) not in known_sites and
                  candidate["kind"] not in ("script_root_binding", "source_literal_initializer")]
    catalog["runtime_text_mutations"] = {"schema": "tokimemo.text.runtime-mutations.v1", "rules": rules,
                                         "source_write_inventory": candidate_report, "blocked": blocked + candidate_report["blocked"],
                                         "unresolved_candidate_ids": unresolved,
                                         "completeness": {"complete": False, "unresolved_writes_block_completion": True},
                                         "coverage": {"rules": len(rules), "target_views": sum(len(rule["targets"]) for rule in rules),
                                                      "unresolved_candidate_writes": len(unresolved)},
                                         "limits": ["no_original_bytes_modified", "conditions_not_executed",
                                                    "generic_write_candidates_require_alias_and_state_resolution",
                                                    "assembly_only_mutations_not_enumerated"]}
    return catalog


def inventory_mutation_candidates(repository, catalog, files=None):
    """Inventory C string-writing calls and writes through possible text aliases.

    Numeric address matches and lexical aliases select candidates only; they
    never prove that an assignment modifies a loaded script or language string.
    """
    from pycparser import c_ast, c_parser
    from .text_ast import _GENERATOR, _ids, _redacted_expr
    from .text_renderers import DISPLAY_ARGUMENT_INDEX
    repo = Path(repository).resolve(strict=True)
    paths = sorted((repo / "src/main").rglob("*.c")) + sorted((repo / "src/ovl").rglob("*.c")) if files is None else [repo / path for path in files]
    root_values = {section[key] for file in catalog.get("files", []) for section in file.get("script", {}).get("layout", {}).get("sections", [])
                   for key in ("text_table", "command_table") if key in section}
    roots_by_unit = {}
    source_inputs = []
    source_read_blocked = []
    for path in paths:
        try:
            if not path.resolve(strict=True).is_relative_to(repo):
                raise ValueError("source escapes repository")
            source = path.read_text(encoding="utf-8")
            if "\ufffd" in source:
                raise UnicodeError("replacement character in source")
        except (OSError, ValueError, UnicodeError) as exc:
            source_read_blocked.append({"source_path": path.as_posix(), "status": "mutation_source_unreadable",
                                        "error_type": type(exc).__name__})
            continue
        code = _mask_c_literals(_mask_c_comments(source), list(_lex_c_tokens(source)))
        rel_parts = path.relative_to(repo).parts
        unit = "main" if rel_parts[:2] == ("src", "main") else rel_parts[2]
        roots = roots_by_unit.setdefault(unit, set(ROOT_SEEDS))
        source_inputs.append((path, source, code, unit))
        for match in re.finditer(r"\b([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", code):
            if int(match.group(2), 16) in root_values:
                roots.add(match.group(1))
    while True:
        before = sum(len(roots) for roots in roots_by_unit.values())
        for _, _, code, unit in source_inputs:
            roots = roots_by_unit[unit]
            for match in re.finditer(r"\b([A-Za-z_]\w*)\s*=\s*([A-Za-z_]\w*)\s*;", code):
                if match.group(2) in roots:
                    roots.add(match.group(1))
        if sum(len(roots) for roots in roots_by_unit.values()) == before:
            break
    candidates, blocked, parsed_files, skipped_files = [], list(source_read_blocked), [], []
    generator = _GENERATOR()
    for path, source, code, unit in source_inputs:
        roots = roots_by_unit[unit]
        rel = path.relative_to(repo).as_posix()
        if files is None and not any(re.search(r"\b" + re.escape(name) + r"\b", code) for name in WRITE_CALLS | roots | NAME_BUFFERS | set(DISPLAY_ARGUMENT_INDEX)):
            skipped_files.append(rel)
            continue
        try:
            preprocessed = subprocess.run(["gcc", "-E", "-std=gnu89", "-DM2CTX", "-Iinclude", rel], cwd=repo,
                                          check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True).stdout
            tree = c_parser.CParser().parse(preprocessed, filename=rel)
        except Exception as exc:
            blocked.append({"source_path": rel, "status": "mutation_source_ast_unavailable", "error_type": type(exc).__name__})
            continue
        parsed_files.append(rel)
        current = {"function": None, "locals": set(), "conditions": []}

        def coordinate(node):
            if node.coord is None:
                return None
            location = Path(node.coord.file)
            if location.is_absolute():
                try:
                    location = location.relative_to(repo)
                except ValueError:
                    return None
            return node.coord.line if location.as_posix() == rel else None

        def emit(node, kind, target, value, call=None):
            line = coordinate(node)
            if line is None:
                return
            candidates.append({"id": f"mutation-candidate:{rel}:{line}:{len(candidates)}", "source_path": rel,
                               "source_sha256": _sha(source.encode()), "line": line, "function": current["function"],
                               "kind": kind, "callee": call, "destination_expression": _redacted_expr(target, generator),
                               "operator": getattr(node, "op", None),
                               "value_expression": _redacted_expr(value, generator),
                               "destination_symbols": sorted(_ids(target)),
                               "syntactic_guard_context": list(current["conditions"]),
                               "status": "candidate_not_proven_runtime_target"})

        class Visitor(c_ast.NodeVisitor):
            def visit_FuncDef(self, node):
                old = dict(current)
                current.update(function=node.decl.name, locals=set(), conditions=[])
                class Reads(c_ast.NodeVisitor):
                    def visit_FuncCall(self, call):
                        name = call.name.name if isinstance(call.name, c_ast.ID) else None
                        args = call.args.exprs if call.args else []
                        if name in DISPLAY_ARGUMENT_INDEX and DISPLAY_ARGUMENT_INDEX[name] < len(args):
                            current["locals"].update(_ids(args[DISPLAY_ARGUMENT_INDEX[name]]))
                        if name in STRING_OUTPUT_CALLS and args:
                            current["locals"].update(_ids(args[0]))
                        self.generic_visit(call)
                Reads().visit(node.body)
                alias_rows = []
                class Aliases(c_ast.NodeVisitor):
                    def visit_Assignment(self, assignment):
                        if isinstance(assignment.lvalue, c_ast.ID):
                            alias_rows.append((assignment.lvalue.name, _ids(assignment.rvalue)))
                        self.generic_visit(assignment)
                    def visit_Decl(self, declaration):
                        if declaration.name and declaration.init is not None:
                            alias_rows.append((declaration.name, _ids(declaration.init)))
                        self.generic_visit(declaration)
                Aliases().visit(node.body)
                while True:
                    before = len(current["locals"])
                    for name, identifiers in alias_rows:
                        if identifiers & (roots | NAME_BUFFERS | current["locals"]):
                            current["locals"].add(name)
                    if len(current["locals"]) == before:
                        break
                self.visit(node.body)
                current.update(old)
            def visit_If(self, node):
                self.visit(node.cond)
                condition = _redacted_expr(node.cond, generator)
                current["conditions"].append(condition)
                self.visit(node.iftrue)
                current["conditions"].pop()
                if node.iffalse:
                    current["conditions"].append("!(" + condition + ")")
                    self.visit(node.iffalse)
                    current["conditions"].pop()
            def visit_For(self, node):
                if node.init:
                    self.visit(node.init)
                if node.cond:
                    self.visit(node.cond)
                current["conditions"].append("for_condition:" + (_redacted_expr(node.cond, generator) if node.cond else "true"))
                self.visit(node.stmt)
                if node.next:
                    self.visit(node.next)
                current["conditions"].pop()
            def visit_While(self, node):
                self.visit(node.cond)
                current["conditions"].append("while_condition:" + _redacted_expr(node.cond, generator))
                self.visit(node.stmt)
                current["conditions"].pop()
            def visit_DoWhile(self, node):
                current["conditions"].append("do_while_post_test:" + _redacted_expr(node.cond, generator))
                self.visit(node.stmt)
                current["conditions"].pop()
                self.visit(node.cond)
            def visit_Switch(self, node):
                self.visit(node.cond)
                current["conditions"].append("switch:" + _redacted_expr(node.cond, generator))
                self.visit(node.stmt)
                current["conditions"].pop()
            def visit_Case(self, node):
                current["conditions"].append("case:" + _redacted_expr(node.expr, generator))
                for statement in node.stmts or []:
                    self.visit(statement)
                current["conditions"].pop()
            def visit_Default(self, node):
                current["conditions"].append("default_case:fallthrough_possible")
                for statement in node.stmts or []:
                    self.visit(statement)
                current["conditions"].pop()
            def visit_Assignment(self, node):
                identifiers = _ids(node.lvalue)
                if identifiers & (roots | NAME_BUFFERS | current["locals"]):
                    kind = "script_root_binding" if isinstance(node.lvalue, c_ast.ID) and node.lvalue.name in roots else "text_or_buffer_write"
                    emit(node, kind, node.lvalue, node.rvalue)
                self.generic_visit(node)
            def visit_UnaryOp(self, node):
                if node.op in ("++", "--", "p++", "p--") and _ids(node.expr) & (roots | NAME_BUFFERS | current["locals"]):
                    emit(node, "text_or_buffer_write", node.expr, node.expr)
                self.generic_visit(node)
            def visit_Decl(self, node):
                if current["function"] and node.init is not None and node.name in current["locals"]:
                    kind = "source_literal_initializer" if isinstance(node.init, c_ast.Constant) and node.init.type == "string" else "text_or_buffer_write"
                    emit(node, kind, c_ast.ID(node.name), node.init)
                self.generic_visit(node)
            def visit_FuncCall(self, node):
                name = node.name.name if isinstance(node.name, c_ast.ID) else None
                args = node.args.exprs if node.args else []
                if name in WRITE_CALLS and args:
                    emit(node, "string_or_byte_copy", args[0], node.args, name)
                self.generic_visit(node)
        Visitor().visit(tree)
    return {"schema": "tokimemo.text.mutation-candidates.v1", "source_files_enumerated": len(source_inputs),
            "parsed_files": parsed_files, "files_skipped_by_candidate_prefilter": skipped_files,
            "candidates": candidates, "blocked": blocked,
            "root_seed_count": sum(len(roots) for roots in roots_by_unit.values()), "limits": ["candidate_alias_closure_not_pointer_proof",
                                                       "all_string_and_byte_copy_calls_are_candidates_not_necessarily_text",
                                                       "opaque_helpers_and_unseeded_byte_writes_require_further_analysis",
                                                       "syntactic_guards_are_not_resolved_path_predicates",
                                                       "case_fallthrough_and_label_goto_edges_not_resolved",
                                                       "control_conditions_recorded_without_execution"]}
