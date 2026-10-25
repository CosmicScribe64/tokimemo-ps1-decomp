"""Original-instruction-backed DATE section hints, separate from editable batches.

T-28007. Named C initializer/table symbols provide source context. They do not
prove a runtime speaker, route execution, or voice ownership. Existing handoff
columns, IDs, source bytes, and translation targets are never modified.
"""
from __future__ import annotations

import argparse
import csv
import hashlib
import json
from pathlib import Path
import re

from .text import parse_script, source_path
from .text_handoff import digest, json_text, safe_destination


PROFILE_SHA256 = "d383d619f43dec759be52b33edddf2c02cbb59128a5a6287d200254f7e33fa55"
ROOT_BASES = (0x8015BDC0, 0x8015BE2C, 0x8015BE98)
WORD = re.compile(r"/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/")


class DateContextError(ValueError):
    """A source, native-instruction or script identity proof failed."""


def require(condition, message):
    if not condition:
        raise DateContextError(message)


def safe_file(root, relative):
    source_path(relative)
    path = (root / relative).resolve(strict=True)
    require(path.is_relative_to(root) and path.is_file(), "evidence path escapes selected root")
    return path


def replay_initializer(words):
    """Evaluate only straight-line LUI/ORI/SW plus final JR RA and its delay."""
    require(len(words) >= 2 and words[-2] == 0x03E00008, "initializer return/delay shape changed")
    registers = [None] * 32
    registers[0] = 0
    stores = {}
    for index, word in enumerate(words):
        opcode, rs, rt, immediate = word >> 26, word >> 21 & 31, word >> 16 & 31, word & 65535
        if word == 0x03E00008:
            require(index == len(words) - 2, "initializer returns before its final delay slot")
        elif opcode == 15:
            require(rs == 0 and rt != 0, "invalid LUI")
            registers[rt] = immediate << 16
        elif opcode == 13:
            require(registers[rs] is not None and rt != 0, "ORI uses an unknown register")
            registers[rt] = registers[rs] | immediate
        elif opcode == 43:
            require(registers[rs] is not None and registers[rt] is not None, "SW uses an unknown register")
            signed = immediate - 65536 if immediate & 32768 else immediate
            address = (registers[rs] + signed) & 0xFFFFFFFF
            require(address % 4 == 0 and address not in stores, "duplicate or unaligned initializer store")
            stores[address] = {"value": registers[rt], "instruction_index": index}
        else:
            raise DateContextError("initializer contains an unsupported instruction or control transfer")
    return stores


def source_assignments(source, function, symbols):
    matches = re.findall(r"\bvoid\s+" + re.escape(function) + r"\(void\)\s*\{([^{}]*)\}", source)
    require(len(matches) == 1, "initializer source definition missing or ambiguous")
    body = re.sub(r"/\*.*?\*/", "", matches[0], flags=re.S)
    pattern = re.compile(r"\s*([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;")
    assignments = {}
    position = 0
    while body[position:].strip():
        match = pattern.match(body, position)
        require(match is not None, "initializer source contains unmodelled statements")
        name, value = match[1], int(match[2], 16)
        require(name in symbols and symbols[name] not in assignments, "source symbol missing or duplicated")
        assignments[symbols[name]] = {"symbol": name, "value": value}
        position = match.end()
    return assignments


def verify_context(source_repository, input_root):
    repository, inputs = Path(source_repository).resolve(strict=True), Path(input_root).resolve(strict=True)
    profile_bytes = Path(__file__).with_name("text_date_context_profile.json").read_bytes()
    require(hashlib.sha256(profile_bytes).hexdigest() == PROFILE_SHA256, "DATE context descriptor changed")
    profile = json.loads(profile_bytes)
    source_bytes = {}
    for relative, expected in profile["source_pins"].items():
        raw = safe_file(repository, relative).read_bytes()
        require(hashlib.sha256(raw).hexdigest() == expected, "source/header/config/assembly pin mismatch")
        source_bytes[relative] = raw
    image = safe_file(inputs, profile["binary"]).read_bytes()
    require(hashlib.sha256(image).hexdigest() == profile["binary_sha256"], "DATE original image mismatch")
    symbols = {}
    for name, value in re.findall(r"^([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", source_bytes["config/symbols_DATE.txt"].decode(), re.M):
        require(name not in symbols, "duplicate DATE symbol definition")
        symbols[name] = int(value, 16)
    functions, total_words = [], 0
    for definition in profile["functions"]:
        name = definition["name"]
        assembly = source_bytes[definition["assembly"]].decode("utf-8", errors="strict")
        require(re.findall(r"^glabel (\w+)$", assembly, re.M) == [name], "native initializer entry label mismatch")
        words, pcs = [], []
        for match in WORD.finditer(assembly):
            offset, pc, raw = int(match[1], 16), int(match[2], 16), bytes.fromhex(match[3])
            require(offset == pc - 0x80132000 and image[offset:offset + 4] == raw, "native initializer original word mismatch")
            require(not pcs or pc == pcs[-1] + 4, "native initializer word gap")
            words.append(int.from_bytes(raw, "little"))
            pcs.append(pc)
        stores = replay_initializer(words)
        assignments = source_assignments(source_bytes[definition["source"]].decode("utf-8"), name, symbols)
        require({address: item["value"] for address, item in stores.items()} ==
                {address: item["value"] for address, item in assignments.items()}, "native stores differ from named C assignments")
        roots = {}
        for index in range(27):
            addresses = [base + index * 4 for base in ROOT_BASES]
            if not any(address in assignments for address in addresses):
                continue
            require(all(address in assignments for address in addresses), "incomplete initialized DATE triad")
            triple = tuple(assignments[address]["value"] for address in addresses)
            require(triple not in roots, "duplicate initialized DATE triad")
            roots[triple] = {"symbols": [assignments[address]["symbol"] for address in addresses],
                             "store_pcs": [pcs[stores[address]["instruction_index"]] for address in addresses],
                             "global_addresses": addresses}
        require(len(roots) * 3 == len(assignments), "initializer has stores outside reviewed triad arrays")
        functions.append({"definition": definition, "roots": roots, "words": len(words)})
        total_words += len(words)
    sections, parsed_records, used_functions = [], {}, set()
    for script in profile["scripts"]:
        original = safe_file(inputs, script["path"]).read_bytes()
        require(hashlib.sha256(original).hexdigest() == script["sha256"], "DATE script original input mismatch")
        triples = {(section["text_table"], section["command_table"], section["voice_table"]) for section in script["sections"]}
        candidates = [function for function in functions if set(function["roots"]) == triples]
        require(len(candidates) == 1, "script root set does not bind uniquely to one native initializer")
        function = candidates[0]
        name = function["definition"]["name"]
        require(name not in used_functions, "initializer reused for another DATE bank")
        used_functions.add(name)
        parsed = parse_script(original, script["path"], script)
        for record in parsed["records"]:
            if record.get("text") is not None:
                require(record["id"] not in parsed_records, "duplicate parsed DATE record ID")
                parsed_records[record["id"]] = record
        for section in script["sections"]:
            triple = tuple(section[key] for key in ("text_table", "command_table", "voice_table"))
            native = function["roots"][triple]
            text_symbol = native["symbols"][0]
            hint = text_symbol.removeprefix("date_").removesuffix("_text_table").removesuffix("_text").replace("_", " ")
            sections.append({"source_path": script["path"], "section": section["id"],
                             "source_subject_hint": name.removeprefix("date_fill_sections_"),
                             "source_section_hint": hint, "source_initializer": name,
                             "source_file": function["definition"]["source"],
                             "text_root_symbol": text_symbol, "command_root_symbol": native["symbols"][1],
                             "voice_root_symbol": native["symbols"][2], "native_store_pcs": native["store_pcs"],
                             "root_values": list(triple), "global_addresses": native["global_addresses"],
                             "binding_basis": "complete original script-root set equals named native initializer stores",
                             "runtime_speaker_proven": False, "loader_selection_proven": False,
                             "runtime_execution_proven": False})
    require(len(functions) == len(used_functions) == 11 and len(sections) == 293, "DATE context cohort count mismatch")
    for relative, expected in profile["source_pins"].items():
        require(digest(safe_file(repository, relative)) == expected, "source evidence changed during verification")
    return {"schema": "tokimemo.date-section-context.v1", "source_pins_verified": len(source_bytes),
            "native_instruction_words": total_words, "native_initializers": len(functions),
            "original_script_banks": len(profile["scripts"]), "sections": sections,
            "runtime_speaker_proven": False, "loader_selection_proven": False,
            "runtime_execution_proven": False}, parsed_records


def export_context(source_repository, input_root, units_path, output, repository):
    output = safe_destination(output, repository)
    source_hash = digest(units_path)
    report, parsed = verify_context(source_repository, input_root)
    contexts = {(row["source_path"], row["section"]): row for row in report["sections"]}
    affected_paths = {path for path, _ in contexts}
    rows, seen = [], set()
    with Path(units_path).open(encoding="utf-8") as stream:
        for line in stream:
            unit = json.loads(line)
            if unit["kind"] != "script_record" or unit["source_path"] not in affected_paths:
                continue
            identifier = unit["id"]
            require(identifier not in seen and identifier in parsed, "DATE unit ID duplicated or absent from original parse")
            seen.add(identifier)
            record = parsed[identifier]
            require(unit["target_text"] is None and unit["source_path"] == record["source_path"] and
                    unit["source_sha256"] == record["source_sha256"] and unit["source_offset"] == record["text"]["offset"] and
                    unit["raw_hex"] == record["text"]["raw_hex"] and unit["source_text"] == record["text"]["source_decoded_text"] and
                    all(unit["context"][key] == record[key] for key in ("section", "group", "line")),
                    "DATE unit original identity/source/group differs from fresh original parse")
            context = contexts.get((unit["source_path"], unit["context"]["section"]))
            require(context is not None, "DATE unit section lacks native context")
            rows.append({"id": identifier, "source_path": unit["source_path"], "section": unit["context"]["section"],
                         "group": unit["context"]["group"], "line": unit["context"]["line"],
                         "source_subject_hint": context["source_subject_hint"],
                         "source_section_hint": context["source_section_hint"],
                         "source_initializer": context["source_initializer"],
                         "text_root_symbol": context["text_root_symbol"],
                         "runtime_speaker_proven": False})
    require(seen == set(parsed), "trusted source units omit parsed DATE records")
    require(digest(units_path) == source_hash, "source units changed during context export")
    report["source_units_sha256"] = source_hash
    report["unit_contexts"] = len(rows)
    output.mkdir(parents=True)
    (output / "context-proof.json").write_text(json_text(report) + "\n", encoding="utf-8")
    with (output / "unit-context.tsv").open("x", encoding="utf-8-sig", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(rows[0]), delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)
    with (output / "section-context.tsv").open("x", encoding="utf-8-sig", newline="") as stream:
        fields = ("source_path", "section", "source_subject_hint", "source_section_hint", "source_initializer", "text_root_symbol")
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n", extrasaction="ignore")
        writer.writeheader()
        writer.writerows(report["sections"])
    (output / "README.md").write_text(
        "# DATE translation context\n\nUse section-context.tsv to look up a DATE file/section from your translation batch. "
        "unit-context.tsv also supports exact lookup by stable row ID. The names are source hints: "
        "they come from original-instruction-verified initializer and table symbols. They can help "
        "identify the source character grouping and place/scene, but do not prove the speaker of "
        "a particular line, runtime execution, or that a loader selected this bank. Keep command/speaker "
        "fields and unresolved flags from the original batch when interpreting a line.\n\n"
        "These are read-only reference files. Keep editing and returning the original TSV batches. "
        "Do not add these columns to a return file or replace its protected context column. "
        "The v3 source-unit digest and return contract remain unchanged; this sidecar adds no translation "
        "and changes no readiness status. Full native evidence is in context-proof.json.\n", encoding="utf-8")
    return {"sections": len(report["sections"]), "unit_contexts": len(rows),
            "native_instruction_words": report["native_instruction_words"], "source_units_sha256": source_hash}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source-repository", type=Path, required=True)
    parser.add_argument("--input-root", type=Path, required=True)
    parser.add_argument("--units", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--repository", type=Path, default=Path.cwd())
    args = parser.parse_args()
    try:
        print(json_text(export_context(args.source_repository, args.input_root, args.units, args.output, args.repository)))
    except (OSError, ValueError, KeyError, TypeError) as exc:
        parser.exit(1, "DATE context: " + str(exc) + "\n")


if __name__ == "__main__":
    main()
