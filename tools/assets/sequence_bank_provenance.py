"""Verify native bank context and join validated SEQ metadata conditionally.

The bundled profile is hash-pinned and every source/native/ISO fact is
rechecked before graph construction. Requests are co-load candidates, never
live bank assignments.
"""
import collections
import hashlib
import json
import re
import struct
from pathlib import Path

from tools.assets import input as asset_input
from tools.assets import manifest

ROOT = Path(manifest.REPOSITORY).resolve()
PROFILE_PATH = Path(__file__).with_name("sequence_bank_profile.json")
PROFILE_SHA256 = "955648930342f82b2e0d8a90d23b6fcec9ee47dfb109471ef35bcf9baa608bd1"
EXPECTED_CACHE_CONDITION = "If cached d equals requested d, f/e/d reads are all skipped; records below are successful cache-miss/read contracts, not proof of current live buffers."
EXPECTED_ZERO_ARGUMENT_CONDITION = "Zero arguments retain/skip resources and adjust completion counters; they do not establish new source provenance."
EXPECTED_PROGRAM_CONTRACT = {
    "bank_field": "signed halfword score+0x4A, shared by the sequence",
    "initial_channel_pan": 64,
    "initial_channel_volume": 127,
    "native_initial_program_by_channel": list(range(16)),
    "note_consumer": "NoteOn reads current per-channel program and score VAB ID before SpuVmKeyOn/Off",
    "program_change_field": "score+0x2C+channel",
}
EXPECTED_LOAD_SETS = [
    {"assign_commands": [34, 33, 47], "load_set": 0, "music_vab_slot": 1,
     "music_vb_ram": 2149351424, "music_vh_ram": 2147557376,
     "sequence_pack_base": 2147598336, "sequence_pack_set": 0,
     "sound_effect_slot": 0, "sound_effect_vb_ram": 2149285888,
     "sound_effect_vh_ram": 2147549184},
    {"assign_commands": [35, 36, 46], "load_set": "nonzero", "music_vab_slot": 2,
     "music_vb_ram": 2149285888, "music_vh_ram": 2147573760,
     "sequence_pack_base": 2147674112, "sequence_pack_set": 1,
     "sound_effect_slot": 3, "sound_effect_vb_ram": 2149449728,
     "sound_effect_vh_ram": 2147590144},
]
EXE_PATH = ROOT / "disc/files/SLPM_86.053"
ISO_PATH = ROOT / "disc/track1.bin"
ASM_WORD = re.compile(r"/\* ([0-9A-Fa-f]+) ([0-9A-Fa-f]+) ([0-9A-Fa-f]{8}) \*/")
CALL = re.compile(r"\baudio_bank_load_start\(([^;\n]+)\);")
FUNCTION = re.compile(r"^[\w][\w \t*]*?\b([A-Za-z_]\w*)\([^;\n]*\)\s*\{", re.M)


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def check(condition, message):
    if not condition:
        raise ValueError(message)


def load_profile():
    raw = PROFILE_PATH.read_bytes()
    check(sha256(raw) == PROFILE_SHA256, "trusted bank profile digest mismatch")
    profile = json.loads(raw)
    check(profile.get("schema") == "tokimemo.sequence-bank-trusted-profile.v1",
          "unsupported trusted bank profile")
    return profile


def _exe_path(binary):
    return EXE_PATH if binary == "main" else ROOT / (
        "disc/files/CDROM/EXEDIR/" + binary + ".EXN")


def _verify_original_code(profile):
    check(profile["load_set_contracts"] == EXPECTED_LOAD_SETS,
          "trusted load-set/VAB-slot/SEQ-pack contract mismatch")
    check(profile["requested_sector_counts"] ==
          {"b_VH": 3, "c_VB_set0": 32, "c_VB_set1": 53,
           "d_VH": 6, "e_VB": 80, "f_SEQ": 37},
          "trusted requested-read contract mismatch")
    check(profile["music_cache_condition"] == EXPECTED_CACHE_CONDITION,
          "trusted cache condition mismatch")
    check(profile["zero_argument_condition"] == EXPECTED_ZERO_ARGUMENT_CONDITION,
          "trusted zero-argument condition mismatch")
    check(profile["program_contract"] == EXPECTED_PROGRAM_CONTRACT,
          "trusted native program/VAB metadata mismatch")
    check(len(profile["native_sites"]) == 21 and
          len(profile["original_tables"]) == 6 and
          len(profile["verified_function_assembly"]) == 113 and
          len(profile["verified_sdk_functions"]) == 8,
          "trusted native proof profile is incomplete")
    exe = EXE_PATH.read_bytes()
    check(sha256(exe) == profile["original_exe_sha256"], "original EXE hash mismatch")
    check(len(profile["source_pins"]) == 91, "trusted source pin set is incomplete")
    for relative, expected in profile["source_pins"].items():
        path = (ROOT / relative).resolve()
        try:
            path.relative_to(ROOT)
        except ValueError as exc:
            raise ValueError("source pin escapes repository") from exc
        check(path.is_file() and sha256(path.read_bytes()) == expected,
              "current source pin mismatch: " + relative)

    binaries = {"main": exe}
    native_words = 0
    for asm_rel, row in profile["verified_function_assembly"].items():
        asm_path = ROOT / asm_rel
        asm_raw = asm_path.read_bytes()
        check(sha256(asm_raw) == row["assembly_sha256"], "assembly source pin mismatch: " + asm_rel)
        binary = row["binary"]
        if binary not in binaries:
            binaries[binary] = _exe_path(binary).read_bytes()
        native = binaries[binary]
        check(sha256(native) == row["binary_sha256"], "native binary pin mismatch: " + binary)
        words = ASM_WORD.findall(asm_raw.decode("utf-8"))
        check(len(words) == row["words"], "assembly word count mismatch: " + asm_rel)
        for offset_hex, _vma, word_hex in words:
            offset = int(offset_hex, 16)
            check(native[offset:offset + 4] == bytes.fromhex(word_hex),
                  "assembly differs from original binary: " + asm_rel)
        native_words += len(words)
    check(native_words == 24373, "native caller/shared word count mismatch")

    asm = (ROOT / "asm/libsnd.s").read_text(encoding="utf-8")
    sdk_words = 0
    for name, row in profile["verified_sdk_functions"].items():
        start = asm.find("glabel " + name + "\n")
        end = asm.find("endlabel " + name, start)
        check(start >= 0 and end > start, "missing SDK function body: " + name)
        words = ASM_WORD.findall(asm[start:end])
        check(len(words) == row["words"], "SDK word count mismatch: " + name)
        check(int(words[0][0], 16) == row["file_offset"] and
              int(words[0][1], 16) == row["vma"], "SDK address mismatch: " + name)
        for offset_hex, _vma, word_hex in words:
            offset = int(offset_hex, 16)
            check(exe[offset:offset + 4] == bytes.fromhex(word_hex),
                  "SDK instruction differs from original EXE: " + name)
        sdk_words += len(words)
    check(sdk_words == 839, "SDK word count mismatch")

    for site in profile["native_sites"]:
        offset = site["exe_file_offset"]
        raw = bytes.fromhex(site["original_bytes_hex"])
        check(exe[offset:offset + len(raw)] == raw, "native contract site differs from EXE")

    load_base = struct.unpack_from("<I", exe, 0x18)[0]
    for name, row in profile["original_tables"].items():
        offset = row["vma"] - load_base + 0x800
        raw = exe[offset:offset + len(row["words"]) * 4]
        check(offset == row["original_exe_offset"] and sha256(raw) == row["extent_sha256"],
              "native table extent mismatch: " + name)
        check(list(struct.unpack("<%dI" % len(row["words"]), raw)) == row["words"],
              "native table values mismatch: " + name)
    return exe, native_words, sdk_words


def _call_identity(row):
    """Stable, source-relative identity independent of C-file traversal order."""
    return (row["source"], row["line"], row["function"], tuple(row["arguments"]),
            tuple(row["literal_arguments"]) if "literal_arguments" in row else None)


def _verify_calls(profile, source_root=None):
    actual = []
    source_root = Path(source_root) if source_root is not None else ROOT / "src"
    source_base = source_root.parent
    for path in source_root.rglob("*.c"):
        text = path.read_text(encoding="utf-8")
        funcs = [(m.start(), m.group(1)) for m in FUNCTION.finditer(text)]
        for match in CALL.finditer(text):
            args = [part.strip() for part in match.group(1).split(",")]
            check(len(args) == 6, "loader call does not have six arguments")
            parents = [name for offset, name in funcs if offset < match.start()]
            row = {"source": path.relative_to(source_base).as_posix(),
                   "line": text.count("\n", 0, match.start()) + 1,
                   "function": parents[-1] if parents else None,
                   "arguments": args}
            try:
                row["literal_arguments"] = [int(arg, 0) for arg in args]
            except ValueError:
                pass
            actual.append(row)
    fields = ("source", "line", "function", "arguments", "literal_arguments")
    expected = [{key: row[key] for key in fields if key in row} for row in profile["calls"]]
    # Counters preserve duplicate multiplicity. Rebuild the result in the
    # profile's stable call-index order only after comparing the full catalog.
    remaining = collections.Counter(_call_identity(row) for row in expected)
    observed = collections.Counter(_call_identity(row) for row in actual)
    check(len(actual) == 133 and observed == remaining,
          "source loader call multiset is incomplete, duplicated, or changed")
    by_identity = collections.defaultdict(collections.deque)
    for row in actual:
        by_identity[_call_identity(row)].append(row)
    ordered = []
    for row in expected:
        key = _call_identity(row)
        check(by_identity[key], "source call identity multiplicity mismatch")
        ordered.append(by_identity[key].popleft())
    check(not any(by_identity.values()), "unmatched source call identities remain")
    return ordered


def _verify_iso(profile):
    with asset_input.open_input(ISO_PATH) as source:
        description = source.describe()
        check(description["source_sha256"] == profile["input"]["source_sha256"],
              "original ISO track hash mismatch")
        inventory = source.entries()
        by_lba = {e["location"].get("lba"): e for e in inventory
                  if e.get("kind") == "file" and isinstance(e.get("location"), dict)}
        checked = {}
        join_count = 0
        for rows in (profile["literal_bindings"], profile["conditional_table_bindings"]):
            for row in rows:
                for role in ("d_vh", "e_vb", "f_seq"):
                    join_count += 1
                    expected = row[role]
                    record = by_lba.get(expected["lba"])
                    check(record is not None, "missing original ISO record at LBA %s" % expected["lba"])
                    location = record.get("location", {})
                    actual = {"path": record["path"], "lba": location.get("lba"),
                              "size": record["size"], "sha256": record["sha256"],
                              "record_lba": location.get("record_lba"),
                              "record_offset": location.get("record_offset")}
                    check(actual == expected, "original ISO record mismatch at LBA %s" % expected["lba"])
                    if expected["lba"] not in checked:
                        data = source.read(record)
                        check(sha256(data) == record["sha256"], "original ISO payload hash mismatch")
                        checked[expected["lba"]] = actual
        check(join_count == 159 and len(checked) == 139,
              "original ISO file-record closure is incomplete")
        seq_records = {}
        for entry in inventory:
            if entry.get("kind") != "file" or not entry["path"].startswith("CDROM/VAB_SEQ/") or not entry["path"].endswith(".SEQ"):
                continue
            raw = source.read(entry)
            check(sha256(raw) == entry["sha256"], "original SEQ ISO payload hash mismatch")
            location = entry["location"]
            seq_records[entry["path"]] = {
                "path": entry["path"], "size": entry["size"], "sha256": entry["sha256"],
                "lba": location["lba"], "record_lba": location.get("record_lba"),
                "record_offset": location.get("record_offset"),
            }
        seq_paths = sorted(seq_records)
        check(len(seq_paths) == 63, "original SEQ inventory count changed")
    return description, seq_paths, checked, seq_records


def verify_authority():
    """Verify source pins, original instructions/tables, caller contexts and ISO records."""
    profile = load_profile()
    exe, native_words, sdk_words = _verify_original_code(profile)
    calls = _verify_calls(profile)
    input_description, seq_paths, iso_files, seq_records = _verify_iso(profile)
    authority = {"profile": profile, "exe": exe, "calls": calls, "seq_paths": seq_paths,
                 "iso_files": iso_files, "iso_file_record_joins": 159,
                 "original_seq_records": seq_records, "input": input_description,
                 "native_words": native_words, "sdk_words": sdk_words}
    contexts = validate_context_catalog(authority)
    authority["contexts"] = contexts
    return authority


def validate_context_catalog(authority):
    """Recompute exact literal groups and all table rows; reject context drift."""
    profile, calls = authority["profile"], authority["calls"]
    by_lba = {row["lba"]: row for row in authority["iso_files"].values()}
    grouped = collections.defaultdict(lambda: {"calls": [], "sets": set()})
    for index, call in enumerate(calls):
        args = call.get("literal_arguments")
        if args and args[5]:
            grouped[tuple(args[3:])]["calls"].append(index)
            grouped[tuple(args[3:])]["sets"].add(args[0])
    literal = []
    for (vh, vb, seq), context in sorted(grouped.items()):
        literal.append({"d_vh": by_lba[vh], "e_vb": by_lba[vb], "f_seq": by_lba[seq],
                        "call_indices": context["calls"], "load_sets": sorted(context["sets"])})
    fields = ("d_vh", "e_vb", "f_seq", "call_indices", "load_sets")
    expected = [{key: row[key] for key in fields} for row in profile["literal_bindings"]]
    check(len(literal) == 33 and literal == expected,
          "literal contexts missing, duplicated, swapped, or mismatched")

    names = ("D_800B5BE8", "D_800B5BF8", "D_800B5BD8",
             "D_800B3688", "D_800B36C8", "D_800B3708")
    load_base = struct.unpack_from("<I", authority["exe"], 0x18)[0]
    rows = []
    for group, tables, count in (("season", names[:3], 4), ("indexed_16_row", names[3:], 16)):
        values = []
        for name in tables:
            table = profile["original_tables"][name]
            offset = table["vma"] - load_base + 0x800
            values.append(struct.unpack_from("<%dI" % count, authority["exe"], offset))
        for index, triple in enumerate(zip(*values)):
            files = [by_lba[value] for value in triple]
            rows.append({"group": group, "index": index, "d_vh": files[0], "e_vb": files[1],
                         "f_seq": files[2],
                         "reachability": "conditional on actual source selector; not all rows asserted reachable"})
    check(len(rows) == 20 and rows == profile["conditional_table_bindings"],
          "conditional table rows missing, duplicated, swapped, or mismatched")
    return literal, rows


def validated_sequence_packages(sequence_manifest_path):
    """Validate the actual selected manifest and every package against originals."""
    path = Path(sequence_manifest_path)
    if not path.is_absolute():
        path = ROOT / path
    path = path.resolve()
    try:
        path.relative_to(manifest.CACHE.resolve())
    except ValueError as exc:
        raise ValueError("sequence manifest must be beneath .asset-cache") from exc
    check(path.name == "manifest.json", "sequence manifest must be named manifest.json")
    out_dir = manifest.output_root(path.parent)
    from tools.assets.sequence_music_validate import validate_directory
    validate_directory(ROOT / "disc/files/CDROM/VAB_SEQ", out_dir)
    catalog = json.loads(path.read_text(encoding="utf-8"))
    packages = {}
    for row in catalog["files"]:
        package_path = manifest.output_root(ROOT / row["output"]["path"])
        raw = package_path.read_bytes()
        check(len(raw) == row["output"]["size"] and sha256(raw) == row["output"]["sha256"],
              "validated package output descriptor mismatch")
        source = row["source"]["path"].removeprefix("disc/files/")
        check(source not in packages, "duplicate source package row")
        packages[source] = json.loads(raw)
    return catalog, packages, sha256(path.read_bytes())


def validate_sequence_iso_identity(authority, catalog, packages):
    """Require each validated wrapper to equal its original ISO SEQ identity."""
    expected = authority["original_seq_records"]
    rows = catalog.get("files")
    check(isinstance(rows, list) and len(rows) == 63,
          "selected SEQ manifest does not contain the complete original source set")
    seen = set()
    for row in rows:
        source = row.get("source", {})
        repo_path = source.get("path")
        check(isinstance(repo_path, str) and repo_path.startswith("disc/files/"),
              "selected SEQ source path is not the original repository path")
        iso_path = repo_path.removeprefix("disc/files/")
        original = expected.get(iso_path)
        check(original is not None and iso_path not in seen,
              "selected SEQ source path is missing, extra, or duplicated")
        seen.add(iso_path)
        check(source.get("size") == original["size"] and
              source.get("sha256") == original["sha256"],
              "selected SEQ bytes differ from original ISO path/size/hash: " + iso_path)
        package_record = packages.get(iso_path)
        check(package_record is not None, "validated SEQ package missing for original path: " + iso_path)
        package = package_record.get("package", {})
        check(package.get("source_relative_path") == repo_path and
              package.get("source_sha256") == original["sha256"] and
              package.get("source_size") == original["size"],
              "SEQ package metadata differs from original ISO identity: " + iso_path)
        for entry in package_record.get("entries", []):
            identity = entry.get("identity", {})
            check(identity.get("source_relative_path") == repo_path and
                  identity.get("source_sha256") == original["sha256"] and
                  identity.get("source_size") == original["size"],
                  "physical sequence identity differs from original ISO: " + iso_path)
    check(seen == set(expected) and set(packages) == set(expected),
          "selected SEQ path set differs from all original ISO SEQ records")


def build_graph(authority, catalog, packages, catalog_sha):
    """Create a provenance graph from reverified source/native/ISO facts."""
    profile = authority["profile"]
    validate_sequence_iso_identity(authority, catalog, packages)
    literal_rows, table_rows = authority["contexts"]
    candidates, requests = collections.defaultdict(list), []

    def add(kind, row, index, sets, selector):
        seq, vh, vb = row["f_seq"], row["d_vh"], row["e_vb"]
        load_contracts = []
        if sets is not None:
            for value in sets:
                key = 0 if value == 0 else "nonzero"
                matches = [r for r in profile["load_set_contracts"] if r["load_set"] == key]
                check(len(matches) == 1, "unknown literal load-set contract")
                load_contracts.extend(matches)
        cid = "%s:%d:%d:%d:%d" % (kind, index, vh["lba"], vb["lba"], seq["lba"])
        item = {"candidate_id": cid, "candidate_kind": kind, "selector": selector,
                "music_vh": vh, "music_vb": vb, "sequence": seq, "load_sets": sets,
                "load_set_argument_status": "literal_caller_arguments" if sets is not None else "dynamic_or_selector_value_unresolved",
                "load_set_contracts": load_contracts, "call_indices": row.get("call_indices", []),
                "claim": "conditional_co_load_request_only",
                "conditions": ["caller and selector are reached with these arguments",
                               profile["music_cache_condition"],
                               "fixed-size CD reads complete with the intended bytes",
                               profile["vab_open_return_contract"],
                               "successful transfer establishes the destination slot/bank epoch",
                               profile["seq_open_contract"]]}
        if kind == "literal_triple":
            item["literal_call_contexts"] = [
                {key: authority["calls"][i][key] for key in
                 ("source", "line", "function", "arguments", "literal_arguments")
                 if key in authority["calls"][i]} for i in row["call_indices"]]
        requests.append(item)
        candidates[seq["path"]].append(cid)

    for i, row in enumerate(literal_rows):
        add("literal_triple", row, i, row["load_sets"], {"kind": "literal_call_group"})
    for row in table_rows:
        add("conditional_table_row", row, row["index"], None,
            {"kind": "source_selector_required", "table_group": row["group"],
             "row_index": row["index"], "source_reachability": row["reachability"]})

    check(len(packages) == 63 and set(packages) == set(authority["seq_paths"]),
          "validated package paths differ from original SEQ inventory")
    sequences = []
    for path in authority["seq_paths"]:
        package_record = packages[path]
        package = package_record["package"]
        for entry in package_record["entries"]:
            ident = entry["identity"]
            programs, cc0, cc1, tempos = [], [], [], 0
            for event in entry.get("event_parse", {}).get("events", []):
                kind = event.get("kind")
                if kind == 0xC0:
                    programs.append({"tick": event["tick"], "channel": event["channel"],
                                     "program_index": event["data"][0], "source_offset": event["source_offset"]})
                elif kind == 0xB0 and event["data"][0] == 0:
                    cc0.append({"tick": event["tick"], "channel": event["channel"],
                                "value": event["data"][1], "source_offset": event["source_offset"]})
                elif kind == 0xB0 and event["data"][0] == 1:
                    cc1.append({"tick": event["tick"], "channel": event["channel"],
                                "value": event["data"][1], "source_offset": event["source_offset"]})
                elif kind == "meta_tempo":
                    tempos += 1
            sequences.append({"sequence_asset_id": ident["asset_id"], "source_path": path,
                              "source_sha256": ident["source_sha256"],
                              "source_size": ident["source_size"],
                              "original_iso_record": authority["original_seq_records"][path],
                              "physical_offset": ident["physical_offset"],
                              "runtime_slots": ident["runtime_slots"],
                              "source_event_status": ident["event_status"],
                              "conditional_candidate_ids": candidates.get(path, []),
                              "provenance_status": "conditional_requests_present" if candidates.get(path) else "not_bound_by_census",
                              "bank_playback_assignment": None,
                              "native_program_state": {
                                  "status": "constructor_initialization_proven",
                                  "initial_program_index_by_channel": list(range(16)),
                                  "program_changes": programs, "cc0_events_in_source_stream": cc0,
                                  "cc0_zero_is_current_corpus_observation": not cc0,
                                  "cc1_modulation_events_in_source_stream": cc1,
                                  "tempo_event_count": tempos,
                                  "interpretation": "logical VAB program indexes; no General MIDI mapping or playback equivalence"}})
    check(len(sequences) == 177, "physical sequence count changed")
    check(sum(len(s["runtime_slots"]) for s in sequences) == 1008, "runtime slot count changed")
    check(sum(len(s["native_program_state"]["program_changes"]) for s in sequences) == 1393,
          "program event count changed")
    check(sum(len(s["native_program_state"]["cc0_events_in_source_stream"]) for s in sequences) == 0,
          "current-source CC0 count changed")
    check(sum(s["native_program_state"]["tempo_event_count"] for s in sequences) == 235,
          "tempo event count changed")
    check(sum(len(s["native_program_state"]["cc1_modulation_events_in_source_stream"]) for s in sequences) == 42,
          "CC1 event count changed")
    return {
        "schema": "tokimemo.conditional-sequence-bank-provenance.v2",
        "classification": "static conditional co-loading provenance; not runtime bank binding",
        "source_proof": {"trusted_profile_sha256": PROFILE_SHA256,
                         "approved_static_proof_sha256": profile["approved_static_proof_sha256"],
                         "original_exe_sha256": profile["original_exe_sha256"],
                         "original_track_sha256": profile["input"]["source_sha256"],
                         "source_pins": profile["source_pins"],
                         "checked_native_words": authority["native_words"],
                         "checked_sdk_words": authority["sdk_words"],
                         "checked_iso_file_record_joins": authority["iso_file_record_joins"],
                         "unique_iso_files_checked": len(authority["iso_files"])},
        "sequence_catalog_sha256": catalog_sha,
        "load_contract": {"arguments": profile["load_arguments"],
                          "load_sets": profile["load_set_contracts"],
                          "music_cache_condition": profile["music_cache_condition"],
                          "zero_argument_condition": profile["zero_argument_condition"],
                          "requested_sector_counts": profile["requested_sector_counts"],
                          "vab_open_return_contract": profile["vab_open_return_contract"],
                          "seq_open_contract": profile["seq_open_contract"],
                          "conditional_provenance_graph": profile["conditional_provenance_graph"],
                          "required_runtime_trace": profile["required_trace"],
                          "limitations": profile["limitations"]},
        "coverage": {"audio_bank_load_start_calls": 133, "literal_calls": 114,
                     "dynamic_calls": 19, "literal_nonzero_seq_triples": 33,
                     "conditional_table_rows": 20, "conditional_requests": len(requests),
                     "original_iso_file_record_joins": authority["iso_file_record_joins"],
                     "seq_paths_with_conditional_candidates": len(candidates),
                     "original_seq_paths": len(authority["seq_paths"]),
                     "physical_sequences": len(sequences), "runtime_slots": 1008,
                     "program_change_events": 1393, "tempo_events": 235,
                     "cc1_events": 42, "cc0_events": 0},
        "requests": requests, "sequences": sequences,
        "unresolved_seq_paths": sorted(set(authority["seq_paths"]) - set(candidates)),
        "non_claims": ["co-loading requests do not prove sequence playback with that bank",
                       "UNDOKAI2 co-loading with KARI_VO/NININ_VO does not establish actual playback binding",
                       "same-stem filenames are not binding authority",
                       "program indexes are not General MIDI instrument identities",
                       "zero CC0 is a current source-stream observation, not a runtime invariant",
                       "no WAV/SF2 rendering or hardware playback parity is claimed"]}
