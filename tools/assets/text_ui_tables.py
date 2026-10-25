"""Original fixed UI fields with source-proven dimensions and display readers.

The profiles preserve every declared/bounded slot, including empty slots and
bytes after the first NUL. Source capacities describe the original layout;
they impose no limit on a future translated representation. Uninterpreted
inter-table bytes remain explicit and are never discovered as NUL strings.
"""
from __future__ import annotations

import hashlib
import math
import re
from pathlib import Path

from .text import TextFormatError, source_path
from .text_fixed import tokenize_fixed_text
from .text_tables import parse_dictionary


def verify_ui_source_proof(data: bytes, profile: dict, repository, files_root) -> dict:
    """Check frozen C/header/ASM pins and original instruction/data-label bytes."""
    from .text_binary import _spans
    repo = Path(repository).resolve(strict=True)
    inputs = Path(files_root).resolve(strict=True)
    if not profile.get("source_pins") or not profile.get("code_proofs"):
        raise TextFormatError("UI source verification requires pinned source and original code evidence")
    if hashlib.sha256(data).hexdigest() != profile["sha256"]:
        raise TextFormatError("UI original table input differs from pinned source proof")
    sources = {}
    for relative, expected in profile["source_pins"].items():
        source_path(relative)
        path = (repo / relative).resolve(strict=True)
        if not path.is_relative_to(repo) or not path.is_file():
            raise TextFormatError("UI source proof path escapes selected source repository")
        raw = path.read_bytes()
        if hashlib.sha256(raw).hexdigest() != expected:
            raise TextFormatError("UI source/header/assembly differs from pinned proof")
        sources[relative] = path
    code_checks = []
    pattern = re.compile(r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*([^\s]+)")
    for proof in profile.get("code_proofs", []):
        relative = source_path(proof["source_path"])
        path = (inputs / relative).resolve(strict=True)
        if not path.is_relative_to(inputs) or not path.is_file():
            raise TextFormatError("UI original code proof escapes selected input root")
        original = path.read_bytes()
        if hashlib.sha256(original).hexdigest() != proof["source_sha256"]:
            raise TextFormatError("UI original code image differs from pinned proof")
        count = 0
        previous = None
        for line in sources[proof["assembly_path"]].read_text(encoding="utf-8").splitlines():
            match = pattern.search(line)
            if match is None or match[3].startswith("."):
                continue
            address, raw = int(match[1], 16), bytes.fromhex(match[2])
            offset = address - proof["load_address"] + proof["header_size"]
            if (offset < proof["header_size"] or original[offset:offset + 4] != raw or
                    previous is not None and address != previous + 4):
                raise TextFormatError("UI display/loader instruction disagrees with original image")
            previous = address
            count += 1
        if not count:
            raise TextFormatError("UI source code proof has no verified instruction words")
        code_checks.append({"assembly_path": proof["assembly_path"], "source_path": relative,
                            "source_sha256": proof["source_sha256"], "instruction_words_verified": count})
    data_checks = []
    for table in profile["tables"]:
        if "assembly_label" not in table:
            continue
        binary = (data, profile["load_address"], profile["header_size"], profile["sha256"], profile["path"])
        matches = [span for span in _spans(sources[table["assembly_path"]], repo, binary)
                   if span["label"] == table["assembly_label"] and span["address"] == table["address"]]
        if len(matches) != 1 or len(matches[0]["raw"]) != table["assembly_size"]:
            raise TextFormatError("UI initialized data-label bytes do not match bounded table extent")
        data_checks.append({"table": table["id"], "label": table["assembly_label"],
                            "address": table["address"], "initialized_bytes_verified": len(matches[0]["raw"])})
    return {"status": "verified_original_source_code_and_table_proof", "code": code_checks,
            "data_labels": data_checks, "source_pins_verified": len(sources)}


def parse_ui_tables(data: bytes, profile: dict) -> dict:
    """Read explicit original UI slots and keep their physical/context provenance."""
    digest = hashlib.sha256(data).hexdigest()
    if profile.get("sha256") and digest != profile["sha256"]:
        raise TextFormatError("UI table input differs from its source-proven profile")
    if "size" in profile and len(data) != profile["size"]:
        raise TextFormatError("UI table input size differs from its source-proven profile")
    tables = {}
    for table in profile["tables"]:
        if table["id"] in tables or table.get("kind") != "fixed_strings":
            raise TextFormatError("UI profile requires unique fixed-string tables")
        count, stride = table["count"], table["stride"]
        if (not isinstance(count, int) or isinstance(count, bool) or not 0 < count <= 4096 or
                not isinstance(stride, int) or isinstance(stride, bool) or not 0 < stride <= 65536):
            raise TextFormatError("UI table slot count or stride outside bounded limits")
        dimensions = table.get("dimensions", [count])
        if (not isinstance(dimensions, list) or not dimensions or
                any(not isinstance(size, int) or isinstance(size, bool) or size <= 0 for size in dimensions) or
                math.prod(dimensions) != count or len(table.get("context_axes", [])) != len(dimensions)):
            raise TextFormatError("UI table dimensions do not match its source slots/context")
        tables[table["id"]] = table
    parsed = parse_dictionary(data, profile)
    for record in parsed["records"]:
        table = tables[record["table"]]
        dimensions = table.get("dimensions", [table["count"]])
        remainder = record["index"]
        coordinates = []
        for axis in range(len(dimensions)):
            size = math.prod(dimensions[axis + 1:])
            coordinate, remainder = divmod(remainder, size)
            coordinates.append(coordinate)
        record["context"] = dict(zip(table["context_axes"], coordinates))
        if "profile_index" in record["context"]:
            index = record["context"]["profile_index"]
            record["context"]["girl_state_index"] = 12 if index == 11 else index
        raw = bytes.fromhex(record["text"]["raw_hex"])
        record["text"] = tokenize_fixed_text(raw, "set_kanji_string")
        source_text = record["text"]["source_decoded_text"]
        record["source_slot_status"] = "empty" if not raw else "whitespace_only" if source_text.isspace() else "source_text"
        record["original_layout"] = {"slot_stride": table["stride"],
                                     "capacity_is_translation_limit": False}
    extents = []
    for table in profile["tables"]:
        offset = table["address"] - profile["load_address"] + profile["header_size"]
        declared = table["count"] * table["stride"]
        size = table.get("assembly_size", declared)
        if not isinstance(size, int) or size < declared or offset < 0 or offset + size > len(data):
            raise TextFormatError("UI table trailing source extent outside input")
        extents.append({"table": table["id"], "offset": offset, "declared_size": declared,
                        "assembly_extent_size": size,
                        "trailing_assembly_raw_hex": data[offset + declared:offset + size].hex()})
    preserved = []
    for span in profile.get("preserved_inter_table_ranges", []):
        offset, size = span["offset"], span["length"]
        if offset < 0 or size < 0 or offset + size > len(data):
            raise TextFormatError("UI uninterpreted source range outside input")
        preserved.append({**span, "raw_hex": data[offset:offset + size].hex()})
    unsupported = {"unknown", "renderer_rejected_pair", "unmapped_numeric_glyph", "not_rendered_after_rejected_pair"}
    parsed.update(schema="tokimemo.ui-tables.v1", table_extents=extents,
                  preserved_inter_table_ranges=preserved)
    parsed["coverage"].update(
        nonempty_slots=sum(record["length"] > 0 for record in parsed["records"]),
        empty_slots=sum(record["length"] == 0 for record in parsed["records"]),
        whitespace_only_slots=sum(record["source_slot_status"] == "whitespace_only" for record in parsed["records"]),
        renderer_anomaly_records=[record["id"] for record in parsed["records"]
                                  if any(token["kind"] in unsupported for token in record["text"]["tokens"])],
        full_slot_bytes_preserved=all(len(bytes.fromhex(record["slot_raw_hex"])) == record["stride"]
                                     for record in parsed["records"]))
    return parsed

UI_TABLE_PROFILES = [{'id': 'main-profile-display-labels',
  'path': 'SLPM_86.053',
  'sha256': 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae',
  'load_address': 0x80041000,
  'header_size': 2048,
  'tables': [{'id': 'profile_display_lower',
              'address': 0x800b5c2c,
              'count': 13,
              'kind': 'fixed_strings',
              'stride': 17,
              'role': 'display',
              'context_axes': ['kind'],
              'assembly_size': 224,
              'assembly_label': 'D_800B5C2C',
              'assembly_path': 'asm/data/data_800B5950.data.s'},
             {'id': 'profile_display_upper',
              'address': 0x800b5d1c,
              'count': 13,
              'kind': 'fixed_strings',
              'stride': 17,
              'role': 'display',
              'context_axes': ['kind'],
              'assembly_size': 224,
              'assembly_label': 'D_800B5D1C',
              'assembly_path': 'asm/data/data_800B5950.data.s'}],
  'evidence': ['src/main/800674B0.c:948-1170',
               'include/main_api.h:844-846',
               'asm/data/data_800B5950.data.s:D_800B5C2C,D_800B5D0C,D_800B5D1C,D_800B5DFC'],
  'source_pins': {'src/main/800674B0.c': '48b49218c257c41443ebbaa0617e2f2ccaa332245473b140b7ff2a0aad196f65',
                  'include/main_api.h': 'befee8984f3ed5e43d3cb9f34302e42f5bb1e133f12e943ce539a30a3c393926',
                  'asm/data/data_800B5950.data.s': 'dd0757de5b9482a5107423ab00b4397d3e5127e5324b3e95a1f07ebaa2f80bd9',
                  'asm/matchings/main/800674B0/func_8006A2CC.s': '1bed8b44752642e338c24c6dd97e91e13a960cc818745641d6cdb595e7a6ed2e'},
  'code_proofs': [{'assembly_path': 'asm/matchings/main/800674B0/func_8006A2CC.s',
                   'source_path': 'SLPM_86.053',
                   'source_sha256': 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae',
                   'load_address': 0x80041000,
                   'header_size': 2048}]},
 {'id': 'joho-profile-display-fields',
  'path': 'CDROM/FUKUTEST/JOHO.NBN',
  'sha256': 'a67acfd5c81f369ba92434f73aa45f17348d44ac4ee8a737af08c09df30989e2',
  'size': 77824,
  'load_address': 0x801a0000,
  'header_size': 0,
  'iso_lba': 0x9d5f,
  'load_sectors': 38,
  'tables': [{'id': 'club_labels',
              'address': 0x801b2200,
              'count': 13,
              'kind': 'fixed_strings',
              'stride': 12,
              'role': 'display',
              'context_axes': ['club_index']},
             {'id': 'blood_type_labels',
              'address': 0x801b229c,
              'count': 4,
              'kind': 'fixed_strings',
              'stride': 8,
              'role': 'display',
              'context_axes': ['blood_type_index']},
             {'id': 'zodiac_labels',
              'address': 0x801b22bc,
              'count': 12,
              'kind': 'fixed_strings',
              'stride': 10,
              'role': 'display',
              'context_axes': ['zodiac_index']},
             {'id': 'measurement_labels',
              'address': 0x801b2334,
              'count': 36,
              'kind': 'fixed_strings',
              'stride': 18,
              'role': 'display',
              'dimensions': [12, 3],
              'context_axes': ['profile_index', 'year_index']},
             {'id': 'profile_notes',
              'address': 0x801b25f4,
              'count': 72,
              'kind': 'fixed_strings',
              'stride': 32,
              'role': 'display',
              'dimensions': [12, 3, 2],
              'context_axes': ['profile_index', 'year_index', 'line_index']}],
  'preserved_inter_table_ranges': [{'offset': 75196,
                                    'length': 56,
                                    'classification': 'uninterpreted_original_bytes'}],
  'evidence': ['src/ovl/TEL/80132000.c:909-924',
               'src/ovl/TEL/8013CCD0.c:89-206',
               'ISO9660 CDROM/FUKUTEST/JOHO.NBN extent 0x9D5F,38 sectors'],
  'source_pins': {'src/ovl/TEL/80132000.c': '450a9a25809648d33fb033bc4d35ccd1d9258f4ca1530daacc104328613d057f',
                  'src/ovl/TEL/8013CCD0.c': '24f01390319ee48f34c0fc55e8865a612d710133fd9938c27c7c8558587a9e07',
                  'include/ovl/TEL.h': '11348703e647f570f34ea5af525369189823ac6dab82302b2683b09efebbfeb6',
                  'asm/ovl/TEL/matchings/TEL/80132000/func_801345C4.s': '7a3fc5e2324c17db2478c5cfdcfc9dfcc782a5f0620b4f561733264ee51ebfda',
                  'asm/ovl/TEL/matchings/TEL/8013CCD0/func_8013D03C.s': '619be2ef983b68c698385aa14a31dab6a914dd274cb3cb35114bbf98dcf67c70'},
  'index_notes': ['profile_index0..10 names ordinary girls; profile_index11 uses girl state12',
                  'year_index0 belowday365,1 fromday365,2 fromday730',
                  'original fixed slot sizes are provenance, not translation layout limits'],
  'code_proofs': [{'assembly_path': 'asm/ovl/TEL/matchings/TEL/80132000/func_801345C4.s',
                   'source_path': 'CDROM/EXEDIR/TEL.EXN',
                   'source_sha256': '25a0379724a5e364e83e5196af5e59bd3c40eccb139c0bb0e80557138fd7e0e7',
                   'load_address': 0x80132000,
                   'header_size': 0},
                  {'assembly_path': 'asm/ovl/TEL/matchings/TEL/8013CCD0/func_8013D03C.s',
                   'source_path': 'CDROM/EXEDIR/TEL.EXN',
                   'source_sha256': '25a0379724a5e364e83e5196af5e59bd3c40eccb139c0bb0e80557138fd7e0e7',
                   'load_address': 0x80132000,
                   'header_size': 0}]}]
