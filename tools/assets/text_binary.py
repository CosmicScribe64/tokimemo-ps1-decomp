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

from .text_fixed import tokenize_fixed_text
from .text_mips import ROLES, _record_unit, _unit, enrich_mips_context
from .text_source import ADDR_COMMENT, REPOSITORY, _decoded_view, _read_original, _string_bytes

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
    active = None
    for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        label = _LABEL.match(line)
        if label:
            if active is not None:
                yield active
            active = None if label[1] == "enddlabel" else {
                "label": label[2], "assembly_path": path.relative_to(repo).as_posix(),
                "assembly_line": number, "address": None, "raw": bytearray(),
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


def enrich_binary_text(inventory: dict, repository=REPOSITORY, files_root=None) -> dict:
    """Add bounded initial C-string views for unresolved static text arguments.

    Call evidence is always regenerated from original instructions. Supplied
    MIPS metadata is never trusted as a source of argument values. Unresolved
    entries are retained with resolution metadata.
    """
    repo = Path(repository).resolve(strict=True)
    inputs = Path(files_root or repo / "disc/files").resolve(strict=True)
    enrich_mips_context(inventory, repo, inputs)
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
        role = ROLES.get(call.get("callee"))
        unit = call.get("caller_unit")
        binary = original(unit) if isinstance(unit, str) else None
        if (role is None or call.get("argument_index") != role[0] or binary is None or
                call.get("proof") != "verified_original_instructions_constant_text_argument" or
                call.get("source_sha256") != binary[3] or call.get("source_path") != binary[4]):
            return False
        evidence = call.get("instruction_evidence", [])
        if not evidence or call.get("call_pc") not in {item.get("pc") for item in evidence}:
            return False
        for item in evidence:
            try:
                raw, offset = bytes.fromhex(item["raw_hex"]), item["offset"]
                if (len(raw) != 4 or offset != item["pc"] - binary[1] + binary[2] or
                        offset < binary[2] or binary[0][offset:offset + 4] != raw):
                    return False
            except (KeyError, TypeError, ValueError):
                return False
        callee = call.get("callee_evidence", {})
        callee_unit = _record_unit(callee.get("source_path", ""))
        target = original(callee_unit) if callee_unit in (unit, "main") else None
        try:
            raw = bytes.fromhex(callee["raw_hex"])
            offset = callee["offset"]
            return (target is not None and callee.get("source_sha256") == target[3] and len(raw) == 4 and
                    callee.get("pc") == call.get("callee_address") and
                    offset == callee["pc"] - target[1] + target[2] and offset >= target[2] and
                    target[0][offset:offset + 4] == raw)
        except (KeyError, TypeError, ValueError):
            return False

    calls = inventory.get("mips_context", {}).get("unresolved_constant_targets", [])
    units = {unit for call in calls for unit in (call.get("caller_unit"), "main") if isinstance(unit, str)}
    for path in sorted((repo / "asm").rglob("*.s")):
        relative = path.relative_to(repo).as_posix()
        if "/data/" not in relative and not relative.startswith("asm/data/"):
            continue
        unit = _unit(path, repo)
        binary = original(unit) if unit in units else None
        if binary is None:
            continue
        for span in _spans(path, repo, binary):
            if span["address"] is not None:
                spans.setdefault((unit, span["address"]), []).append(span)
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
        data, base, bias, digest, source_path = original(unit)
        offset = address - base + bias
        key = (source_path, address, raw.hex())
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
            record = {"id": f"{source_path}:{offset:08x}:{hashlib.sha256(raw).hexdigest()[:12]}",
                      "category": ROLES[call["callee"]][1],
                      "category_basis": "verified data-label initial string with potential static text argument",
                      "raw_hex": raw.hex(), "raw_length": len(raw),
                      "extent_status": "verified_initialized_data_label_string",
                      "source": {"path": source_path, "address": address, "offset": offset,
                                 "length": len(raw), "end_offset": offset + len(raw),
                                 "file_size": len(data), "sha256": digest},
                      **_decoded_view(raw, True), "assembly_sites": [], "mips_contexts": [],
                      "initial_view_only": True, "runtime_read_length_proven": False,
                      "terminator": {"kind": "nul", "relative_offset": end, "included_in_raw": True}}
            inventory["records"].append(record)
            existing[key] = record
            added += 1
        site = {key: value for key, value in span.items() if key not in ("raw", "stopped")}
        site["verified_initialized_prefix_length"] = len(raw_span)
        if site not in record.setdefault("assembly_sites", []):
            record["assembly_sites"].append(site)
        context = {**call, "status": "potential_static_callsite"}
        if context not in record.setdefault("mips_contexts", []):
            record["mips_contexts"].append(context)
        renderer = ROLES[call["callee"]][2]
        views = record.setdefault("binary_initial_renderer_views", [])
        view = {"renderer": renderer, "call_pc": call["call_pc"],
                "view": tokenize_fixed_text(raw[:-1], renderer), "runtime_read_length_proven": False}
        if view not in views:
            views.append(view)
        resolution.update(status="recovered_verified_label_initial_string", record_id=record["id"])
    inventory["binary_text_supplement"] = {
        "records_added": added, "calls_considered": len(calls), "resolutions": resolutions,
        "blocked": blocked, "complete": False,
        "limitations": ["Potential static argument evidence does not prove execution or runtime read length.",
                        "Only contiguous supported initialized directives at exact data-label starts are read.",
                        "Initially zero storage remains a runtime-buffer candidate; no empty translation is inferred."]}
    return inventory
