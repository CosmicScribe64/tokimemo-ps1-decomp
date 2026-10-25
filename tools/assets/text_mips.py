"""Conservative static MIPS call-argument evidence for verified text (T-28001).

This is constant propagation over original R3000 instruction bytes, not game
execution. It follows no branches and reads no mutable memory. Calls are
potential static call sites; runtime reachability and mutations remain unknown.
"""
from __future__ import annotations

import hashlib
import re
from pathlib import Path

from .text_source import REPOSITORY, _read_original, source_link_unit
from .text_renderers import (DISPLAY_ARGUMENT_INDEX, FORMAT_ARGUMENT_INDEX, DIAGNOSTIC_ARGUMENT_INDEX,
                             read_argument_specs)

INSTRUCTION = re.compile(r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*([^\s]+)")
LABEL = re.compile(r"^\s*(glabel|jlabel|dlabel|endlabel|enddlabel)\s+(\S+)|^\s*([.$A-Za-z_][\w.$]*):")
SYMBOL = re.compile(r"^\s*([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;")
ROLES = {name: (argument, "display", "set_kanji_string" if name in ("gnsx", "message_window_set_title") else name)
         for name, argument in DISPLAY_ARGUMENT_INDEX.items()}
ROLES.update({name: (argument, "debug_or_format", name) for name, argument in FORMAT_ARGUMENT_INDEX.items()})
ROLES.update({name: (argument, "debug_or_format", "ascii_literal")
              for name, argument in DIAGNOSTIC_ARGUMENT_INDEX.items()})
CLASSIFICATION_BASIS = "potential static text-argument call site from verified original MIPS instructions"


def _text_read_specs(call, unit=None):
    for spec in read_argument_specs(call, unit):
        if spec["role"] not in {"display", "format", "diagnostic"}:
            continue
        renderer = "ascii_literal" if spec["role"] == "diagnostic" else spec["renderer"]
        yield {**spec, "category": "display" if spec["role"] == "display" else "debug_or_format",
               "renderer": renderer,
               "renderer_status": spec.get("renderer_status", "source_verified_call"),
               "read_role": "format_string" if spec["role"] == "format" else
                            "diagnostic_literal" if spec["role"] == "diagnostic" else "display_string"}


def text_argument_spec(call, argument_index, unit=None):
    return next((spec for spec in _text_read_specs(call, unit) if spec["index"] == argument_index), None)


def role_for_argument(call, argument_index, unit=None):
    """Return the verified read slot's legacy role tuple in its caller scope."""
    spec = text_argument_spec(call, argument_index, unit)
    return (spec["index"], spec["category"], spec["renderer"]) if spec is not None else None


def _read_guards(conditions, state):
    facts, evidence = [], {}
    for condition in conditions:
        index = condition["argument_index"]
        value = state[4 + index] if 0 <= index <= 3 else None
        actual = value[0] if value is not None else None
        result = None if actual is None else (
            actual == condition["value"] if condition["operator"] == "==" else actual != condition["value"])
        facts.append({**condition, "constant_argument_value": actual,
                      "status": "unknown" if result is None else "proven_true" if result else "proven_false"})
        if value is not None:
            evidence.update({item[0]: item for item in value[1]})
    return not any(item["status"] == "proven_false" for item in facts), facts, evidence


def _unit(path, repo):
    relative = path.relative_to(repo).as_posix()
    return source_link_unit(repo, relative) or "unknown"


def _record_unit(path):
    return source_link_unit(REPOSITORY, path)


def _signed(value, bits):
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def _reset():
    state = [None] * 32
    state[0] = (0, ())
    return state


def _assign(state, register, value, evidence, operands=()):
    if not register:
        return
    if value is None:
        state[register] = None
        return
    chain = {item[0]: item for operand in operands if operand is not None for item in operand[1]}
    chain[evidence[0]] = evidence
    state[register] = (value & 0xFFFFFFFF, tuple(chain[key] for key in sorted(chain)))


def _ordinary(word, state, evidence):
    """Model only known GPR effects. Unknown effects discard all constants."""
    op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
    imm, shift, funct = word & 0xFFFF, (word >> 6) & 31, word & 63
    a, b = state[rs], state[rt]
    if op == 15:
        _assign(state, rt, imm << 16, evidence)
    elif op in (9, 12, 13, 14):
        value = None
        if a is not None:
            value = (a[0] + _signed(imm, 16) if op == 9 else a[0] & imm if op == 12 else
                     a[0] | imm if op == 13 else a[0] ^ imm)
        _assign(state, rt, value, evidence, (a,))
    elif op == 0 and funct in (0, 2, 3, 4, 6, 7, 0x21, 0x23, 0x24, 0x25, 0x26, 0x27):
        value = None
        if funct in (0, 2, 3) and b is not None:
            value = b[0] << shift if funct == 0 else b[0] >> shift if funct == 2 else _signed(b[0], 32) >> shift
        elif a is not None and b is not None:
            amount = a[0] & 31
            value = (b[0] << amount if funct == 4 else b[0] >> amount if funct == 6 else
                     _signed(b[0], 32) >> amount if funct == 7 else a[0] + b[0] if funct == 0x21 else
                     a[0] - b[0] if funct == 0x23 else a[0] & b[0] if funct == 0x24 else
                     a[0] | b[0] if funct == 0x25 else a[0] ^ b[0] if funct == 0x26 else ~(a[0] | b[0]))
        _assign(state, rd, value, evidence, (b,) if funct in (0, 2, 3) else (a, b))
    elif op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26):
        state[rt] = None  # No reads from globals, stack, or other runtime memory.
    elif op in (0x28, 0x29, 0x2A, 0x2B, 0x2E):
        pass  # Stores do not change GPRs; memory contents are never propagated.
    elif op in (8, 10, 11):
        state[rt] = None
    elif op == 0 and funct in (0x10, 0x12, 0x20, 0x22, 0x2A, 0x2B):
        state[rd] = None
    elif op == 0 and funct in (0x11, 0x13, 0x18, 0x19, 0x1A, 0x1B):
        pass  # Only HI/LO change; mfhi/mflo above always produce unknown values.
    else:
        state[:] = _reset()
    state[0] = (0, ())


def _control(word):
    op, funct = word >> 26, word & 63
    if op == 3 or op == 0 and funct == 9 and (word >> 11) & 31:
        return "call"
    if (op in (1, 2, 4, 5, 6, 7, 0x14, 0x15, 0x16, 0x17) or op == 0 and funct in (8, 9) or
            op in (0x10, 0x11, 0x12, 0x13) and (word >> 21) & 31 == 8):
        return "branch"
    return None


def enrich_mips_context(inventory: dict, repository=REPOSITORY, files_root=None) -> dict:
    """Attach potential text-call evidence using exact argument-register constants.

    Every target extent and contributing instruction is rechecked against the
    selected original binary. Overlay calls see their own unit and shared main
    data only. No byte searches, memory-load values, or branch paths are guessed.
    """
    repo = Path(repository).resolve(strict=True)
    inputs = Path(files_root or repo / "disc" / "files").resolve(strict=True)
    bases = {}
    for line in (repo / "config" / "overlays.txt").read_text().splitlines():
        fields = line.split()
        if len(fields) >= 2 and not fields[0].startswith("#"):
            bases[fields[0]] = int(fields[1], 0)
    binaries, blocked = {}, []

    def original(unit):
        if unit not in binaries:
            path = "SLPM_86.053" if unit == "main" else f"CDROM/EXEDIR/{unit}.EXN"
            data, error = _read_original(inputs / path, inputs)
            base = 0x80041000 if unit == "main" else bases.get(unit)
            binaries[unit] = (data, base, 0x800 if unit == "main" else 0,
                              hashlib.sha256(data).hexdigest() if data is not None else None, path)
            if error:
                blocked.append({"source_path": path, "status": "input_rejected", "reason": error})
        return binaries[unit]

    by_address = {}
    verified = set()
    for record in inventory["records"]:
        record["mips_contexts"] = []
        if record.get("category_basis") == CLASSIFICATION_BASIS:
            record["category"] = "unknown"
            record["category_basis"] = "static MIPS context requires revalidation"
        source = record.get("source", {})
        unit = _record_unit(source.get("path", ""))
        if unit is None:
            blocked.append({"record_id": record.get("id"), "status": "unknown_text_unit"})
            continue
        data, base, bias, digest, _ = original(unit)
        try:
            raw = bytes.fromhex(record["raw_hex"])
        except (KeyError, ValueError, TypeError):
            raw = None
        offset, address = source.get("offset"), source.get("address")
        valid = (data is not None and base is not None and raw is not None and
                 isinstance(offset, int) and isinstance(address, int) and offset == address - base + bias and
                 0 <= offset <= len(data) - len(raw) and data[offset:offset + len(raw)] == raw and
                 source.get("sha256") == digest and source.get("file_size") == len(data) and
                 source.get("length") == len(raw) and record.get("raw_length") == len(raw) and
                 record.get("extent_status", "").startswith("verified"))
        if not valid:
            blocked.append({"record_id": record.get("id"), "status": "text_extent_unverified"})
            continue
        verified.add(record["id"])
        by_address.setdefault((unit, address), []).append(record)

    streams = []
    verified_pcs = set()
    join_pcs = set()
    entries = {}
    for path in sorted((repo / "asm").rglob("*.s")):
        relative = path.relative_to(repo).as_posix()
        if "/data/" in relative or relative.startswith("asm/data/"):
            continue
        unit = _unit(path, repo)
        data, base, bias, digest, binary_path = original(unit)
        if data is None or base is None:
            continue
        events = []
        name = None
        pending_entry = None
        in_text = True
        try:
            source_text = path.read_text(encoding="utf-8")
        except (OSError, UnicodeError) as exc:
            blocked.append({"assembly_path": relative, "status": "assembly_source_unreadable",
                            "reason": type(exc).__name__})
            continue
        if "\ufffd" in source_text:
            blocked.append({"assembly_path": relative, "status": "assembly_source_replacement_character"})
            continue
        for line_no, line in enumerate(source_text.splitlines(), 1):
            if line.strip().startswith(".section"):
                in_text = ".text" in line
                events.append(("reset", None))
            elif line.strip() in (".text", ".data", ".rodata", ".bss"):
                in_text = line.strip() == ".text"
                events.append(("reset", None))
            label = LABEL.match(line)
            if label:
                if label.group(1) == "glabel":
                    name = label.group(2)
                    pending_entry = (name, line_no)
                elif label.group(1) not in ("jlabel",):
                    pending_entry = None
                events.append(("reset", None))
            match = INSTRUCTION.search(line)
            if match is None or not in_text or match.group(3).startswith("."):
                continue
            pc = int(match.group(1), 16)
            raw_word = bytes.fromhex(match.group(2))
            offset = pc - base + bias
            if offset < 0 or offset + 4 > len(data) or data[offset:offset + 4] != raw_word:
                blocked.append({"assembly_path": relative, "line": line_no, "pc": pc,
                                "status": "instruction_bytes_disagree"})
                events.append(("reset", None))
                pending_entry = None
                continue
            verified_pcs.add((unit, pc))
            word = int.from_bytes(raw_word, "little")
            if _control(word) == "branch":
                if word >> 26 == 2:
                    join_pcs.add((unit, ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)))
                elif word >> 26 != 0:
                    join_pcs.add((unit, (pc + 4 + (_signed(word & 0xFFFF, 16) << 2)) & 0xFFFFFFFF))
            if pending_entry:
                if any(_text_read_specs(pending_entry[0], unit)):
                    entries[unit, pc] = {"name": pending_entry[0],
                                        "evidence": {"kind": "verified_glabel_entry", "unit": unit,
                                                     "assembly_path": relative, "assembly_line": pending_entry[1],
                                                     "pc": pc, "offset": offset, "raw_hex": raw_word.hex(),
                                                     "source_path": binary_path, "source_sha256": digest}}
                pending_entry = None
            events.append(("instruction", (pc, int.from_bytes(raw_word, "little"), line_no,
                                           offset, raw_word.hex(), name)))
        if events:
            streams.append((relative, unit, digest, binary_path, events))
    for path in sorted((repo / "config").rglob("*.txt")):
        for line_no, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
            match = SYMBOL.match(line)
            if match and any(_text_read_specs(match.group(1), "main")):
                address = int(match.group(2), 16)
                if ("main", address) in verified_pcs:
                    data, base, bias, digest, source_path = original("main")
                    offset = address - base + bias
                    entries["main", address] = {
                        "name": match.group(1), "evidence": {
                            "kind": "configured_function_symbol_at_verified_pc", "unit": "main",
                            "path": path.relative_to(repo).as_posix(), "line": line_no,
                            "pc": address, "offset": offset, "raw_hex": data[offset:offset + 4].hex(),
                            "source_path": source_path, "source_sha256": digest}}

    contexts = {}
    interior_references = {}
    unresolved_targets = {}
    calls_seen = set()
    for asm_path, unit, digest, binary_path, events in streams:
        state = _reset()
        pending = None
        previous_pc = None
        for kind, instruction in events:
            if kind == "reset":
                state, pending, previous_pc = _reset(), None, None
                continue
            pc, word, line, offset, raw_word, caller = instruction
            if (unit, pc) in join_pcs:
                state, pending = _reset(), None
            if previous_pc is not None and pc != previous_pc + 4:
                state, pending = _reset(), None
            previous_pc = pc
            evidence = (pc, offset, raw_word, line)
            if pending is not None:
                if _control(word) is not None:
                    state, pending = _reset(), None
                    continue
                _ordinary(word, state, evidence)
                if pending["kind"] == "call":
                    target = pending["target"]
                    resolved_callee = entries.get((unit, target), entries.get(("main", target)))
                    if resolved_callee is not None:
                        callee = resolved_callee["name"]
                        calls_seen.add((unit, pending["pc"], callee))
                        for spec in _text_read_specs(callee, unit):
                            arg, category, renderer = spec["index"], spec["category"], spec["renderer"]
                            if not 0 <= arg <= 3:
                                continue
                            allowed, guard_facts, guard_evidence = _read_guards(spec["conditions"], state)
                            value = state[4 + arg]
                            if value is None or not allowed:
                                continue
                            records = by_address.get((unit, value[0]), []) or by_address.get(("main", value[0]), [])
                            proof = {item[0]: item for item in value[1]}
                            proof.update(guard_evidence)
                            proof.update({item[0]: item for item in pending["target_evidence"]})
                            proof[pending["evidence"][0]] = pending["evidence"]
                            proof[pc] = evidence
                            context = {"status": "potential_static_callsite", "callee": callee,
                                           "callee_address": target, "argument_index": arg,
                                           "callee_evidence": resolved_callee["evidence"],
                                           "argument_register": "a" + str(arg), "argument_value": value[0],
                                           "category": category, "renderer": renderer,
                                           "renderer_status": spec["renderer_status"],
                                           "callee_read_conditions": guard_facts,
                                           "read_role": spec["read_role"],
                                           "caller": caller, "caller_unit": unit, "call_pc": pending["pc"],
                                           "delay_slot_pc": pc, "assembly_path": asm_path,
                                           "source_path": binary_path, "source_sha256": digest,
                                           "read_length_proven": False,
                                           "instruction_evidence": [{"pc": item[0], "offset": item[1],
                                                                     "raw_hex": item[2], "assembly_line": item[3]}
                                                                    for item in sorted(proof.values())],
                                       "proof": "verified_original_instructions_constant_text_argument"}
                            for record in records:
                                key = (record["id"], unit, pending["pc"], callee, arg)
                                if key not in contexts:
                                    contexts[key] = context
                                    record["mips_contexts"].append(context)
                            if not records:
                                matches = []
                                for record in inventory["records"]:
                                    if record["id"] not in verified:
                                        continue
                                    source = record["source"]
                                    if _record_unit(source["path"]) not in (unit, "main"):
                                        continue
                                    if source["address"] < value[0] < source["address"] + record["raw_length"]:
                                        matches.append(record)
                                for record in matches:
                                    key = (record["id"], unit, pending["pc"], callee, arg, value[0])
                                    item = {**context, "status": "verified_extent_interior_pointer",
                                            "record_id": record["id"],
                                            "relative_byte_offset": value[0] - record["source"]["address"],
                                            "category_not_promoted": True, "substring_bytes_not_decoded": True}
                                    interior_references[key] = item
                                if not matches:
                                    key = (unit, pending["pc"], callee, arg, value[0])
                                    unresolved_targets[key] = {**context,
                                                              "status": "constant_text_argument_outside_exported_extents"}
                    # Callee-saved s0-s7, sp and fp alone survive an unknown call.
                    for register in list(range(1, 16)) + [24, 25, 26, 27, 28, 31]:
                        state[register] = None
                else:
                    state = _reset()
                pending = None
                continue
            control = _control(word)
            if control == "call":
                if word >> 26 == 3:
                    target = ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
                    return_register = 31
                    target_evidence = ()
                else:
                    source = state[(word >> 21) & 31]
                    target = source[0] if source is not None else None
                    target_evidence = source[1] if source is not None else ()
                    return_register = (word >> 11) & 31
                _assign(state, return_register, pc + 8, evidence)
                pending = {"kind": "call", "target": target, "pc": pc, "evidence": evidence,
                           "target_evidence": target_evidence}
            elif control == "branch":
                state = _reset()
                pending = {"kind": "branch"}
            else:
                _ordinary(word, state, evidence)
    promoted = 0
    for record in inventory["records"]:
        roles = {context["category"] for context in record.get("mips_contexts", [])}
        if len(roles) == 1 and record.get("category") == "unknown":
            record["category"] = next(iter(roles))
            record["category_basis"] = CLASSIFICATION_BASIS
            promoted += 1
    inventory["mips_context"] = {"schema": "tokimemo.text.mips-context.v1",
                                 "verified_text_records": len(verified), "verified_instruction_pcs": len(verified_pcs),
                                 "known_text_calls": len(calls_seen), "contexts": len(contexts),
                                 "records_with_context": sum(bool(r.get("mips_contexts")) for r in inventory["records"]),
                                 "records_promoted_from_unknown": promoted, "blocked": blocked,
                                 "interior_references": list(interior_references.values()),
                                 "unresolved_constant_targets": list(unresolved_targets.values()),
                                 "limits": ["potential_static_callsite_not_runtime_reachability",
                                            "no_memory_load_constant_propagation", "no_branch_path_following",
                                            "runtime_string_mutations_not_modeled", "only_exact_extent_starts_promote_category",
                                            "o32_callee_saved_s0_s7_sp_fp_assumed"]}
    return inventory
