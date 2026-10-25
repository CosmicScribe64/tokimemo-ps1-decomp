"""Export a bounded set of original school-day rule tables to ignored JSON."""
import argparse
import hashlib
import json
import os
import re
import shutil
import struct
import tempfile
import sys
from pathlib import Path

ROOT = next(p for p in Path(__file__).resolve().parents if (p / "tools/assets/input.py").is_file())
sys.path.insert(0, str(ROOT))
from tools.assets.input import open_input

SCHEMA = "tokimemo.port-rule-tables.v1"
SCHEMA_VERSION = 1
RELEASE = "Tokimeki Memorial - Forever with You (Japan), PlayStation the Best"
EXE_ENTRY = "SLPM_86.053"
EXE_SHA256 = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
TRACK_SHA256 = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
EXPECTED_LOAD_ADDRESS = 0x80041000
EXPECTED_TEXT_SIZE = 0xA2800
HEADER_SIZE = 0x800

# Shapes/types follow their cited C declarations/specifications; sizes are derived only from them.
TABLES = {
    "command_deltas": {"symbol": "school_day_command_deltas", "vaddr": 0x800B68E0, "type": "s8", "shape": [8, 9], "evidence": [("include/main/school_day.h", "extern s8 school_day_command_deltas[8][9]"), ("config/symbol_addrs.txt", "school_day_command_deltas = 0x800B68E0;")], "config_symbol": "school_day_command_deltas"},
    "birthday_offsets": {"symbol": "birthday_day_table", "vaddr": 0x800B670A, "type": "u16", "shape": [13], "evidence": [("include/main/hud.h", "extern u16 birthday_day_table[]"), ("wiki/spec/m-hud.md", "birthday_day_table")], "config_symbol": "birthday_day_table"},
    "sine_quarter": {"symbol": "D_800DD6C0", "vaddr": 0x800DD6C0, "type": "s16", "shape": [1025], "evidence": [("include/libgte.h", "s32 rsin()"), ("asm/libgte_8009FA5C.s", "slti       $v0, $a0, 0x801"), ("asm/libgte_8009FA5C.s", "$a0, 0x401"), ("asm/data/data_800CA380.data.s", "dlabel D_800DD6C0") ]},
    "tokimeki_weights": {"symbol": "girl_tokimeki_weight", "vaddr": 0x800B4100, "type": "u8", "shape": [13], "evidence": [("include/main/girl.h", "extern u8 girl_tokimeki_weight[13]"), ("config/symbol_addrs.txt", "girl_tokimeki_weight = 0x800B4100;")], "config_symbol": "girl_tokimeki_weight"},
    "bonus_param_masks": {"symbol": "girl_bonus_param_mask", "vaddr": 0x800B4110, "type": "u8", "shape": [13], "evidence": [("include/main/girl.h", "extern u8 girl_bonus_param_mask[13]"), ("config/symbol_addrs.txt", "girl_bonus_param_mask = 0x800B4110;")], "config_symbol": "girl_bonus_param_mask"},
    "ladder_kinds": {"symbol": "girl_param_ladder_kind", "vaddr": 0x800B4120, "type": "u8", "shape": [13], "evidence": [("include/main/girl.h", "extern u8 girl_param_ladder_kind[13]"), ("config/symbol_addrs.txt", "girl_param_ladder_kind = 0x800B4120;")], "config_symbol": "girl_param_ladder_kind"},
    "param_bonus": {"symbol": "girl_param_bonus", "vaddr": 0x800B4130, "type": "s8", "shape": [13, 8], "evidence": [("include/main/girl.h", "extern s8 girl_param_bonus[13][8]"), ("config/symbol_addrs.txt", "girl_param_bonus = 0x800B4130;")], "config_symbol": "girl_param_bonus"},
    "ladder_bases": {"symbol": "girl_param_ladder_base", "vaddr": 0x800B4198, "type": "u8", "shape": [7], "evidence": [("include/main/girl.h", "extern u8 girl_param_ladder_base[7]"), ("config/symbol_addrs.txt", "girl_param_ladder_base = 0x800B4198;")], "config_symbol": "girl_param_ladder_base"},
    "affection_weights": {"symbol": "girl_affection_weight", "vaddr": 0x800B41A0, "type": "u8", "shape": [13], "evidence": [("include/main/girl.h", "extern u8 girl_affection_weight[]"), ("wiki/spec/m-menutext.md", "girl_affection_weight  83"), ("config/symbol_addrs.txt", "girl_affection_weight = 0x800B41A0;")], "config_symbol": "girl_affection_weight"},
    "closeness_grid": {"symbol": "girl_closeness_grid", "vaddr": 0x800B41B0, "type": "u8", "shape": [20, 20], "evidence": [("include/main/girl.h", "extern u8 girl_closeness_grid[20][20]"), ("config/symbol_addrs_hypotheses.txt", "girl_closeness_grid = 0x800B41B0;")]},
    "command0_rank_threshold": {"symbol": "schedule_command_rank_bar_liberal_arts_athletics", "vaddr": 0x800B6748, "type": "s32", "shape": [1], "evidence": [("include/main/schedule.h", "extern s32 schedule_command_rank_bar_liberal_arts_athletics"), ("config/symbol_addrs.txt", "schedule_command_rank_bar_liberal_arts_athletics = 0x800B6748;")], "config_symbol": "schedule_command_rank_bar_liberal_arts_athletics"},
    "weekday_background_lbas": {"symbol": "D_800B6760", "vaddr": 0x800B6760, "type": "s32", "shape": [8], "evidence": [("include/main/school_day.h", "extern s32 D_800B6760[]"), ("wiki/spec/m-weekday.md", "week_day_bg_sectors") ]},
    "weekday_sd_lbas": {"symbol": "D_800B67AC", "vaddr": 0x800B67AC, "type": "s32", "shape": [25], "evidence": [("include/main/school_day.h", "extern s32 D_800B67AC[]"), ("wiki/spec/m-weekday.md", "week_day_sd_set_sectors") ]},
    "sd_select_non_summer": {"symbol": "D_800B68D0", "vaddr": 0x800B68D0, "type": "u8", "shape": [16], "evidence": [("include/main/school_day.h", "extern u8 D_800B68D0[16]"), ("wiki/spec/m-weekday.md", "week_day_sd_set_other") ]},
    "sd_select_summer": {"symbol": "D_800B68C0", "vaddr": 0x800B68C0, "type": "u8", "shape": [16], "evidence": [("include/main/school_day.h", "extern u8 D_800B68C0[16]"), ("wiki/spec/m-weekday.md", "week_day_sd_set_summer") ]},
}
FORMATS = {"s8": (1, "b", -128, 127), "u8": (1, "B", 0, 255), "s16": (2, "h", -32768, 32767), "u16": (2, "H", 0, 65535), "s32": (4, "i", -2147483648, 2147483647)}


class TableExportError(ValueError):
    pass


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def product(shape):
    result = 1
    for dimension in shape:
        if type(dimension) is not int or dimension <= 0:
            raise TableExportError("table shape dimensions must be positive integers")
        result *= dimension
    return result


def parse_psx_exe(exe_bytes, exe_sha256=EXE_SHA256):
    if not isinstance(exe_bytes, bytes) or len(exe_bytes) < HEADER_SIZE:
        raise TableExportError("PS-X EXE header is truncated")
    if exe_bytes[:8] != b"PS-X EXE":
        raise TableExportError("original executable does not have the PS-X EXE signature")
    actual_hash = sha256(exe_bytes)
    if actual_hash != exe_sha256:
        raise TableExportError("executable SHA-256 is not the verified PlayStation the Best release")
    load_address, text_size = struct.unpack_from("<II", exe_bytes, 0x18)
    pc, gp = struct.unpack_from("<II", exe_bytes, 0x10)
    if load_address != EXPECTED_LOAD_ADDRESS or text_size != EXPECTED_TEXT_SIZE:
        raise TableExportError("PS-X EXE loaded address or text extent differs from verified release")
    if HEADER_SIZE + text_size != len(exe_bytes):
        raise TableExportError("PS-X EXE file size does not equal header plus loaded text extent")
    return {"magic": "PS-X EXE", "initial_pc": pc, "initial_gp": gp,
            "load_address": load_address, "text_size_bytes": text_size,
            "payload_file_offset": HEADER_SIZE, "loaded_vaddr_start": load_address,
            "loaded_vaddr_end_exclusive": load_address + text_size}


def _read_descriptor(exe_bytes, header, descriptor):
    if descriptor["type"] not in FORMATS:
        raise TableExportError("unsupported numeric table type")
    width, fmt, low, high = FORMATS[descriptor["type"]]
    count = product(descriptor["shape"])
    size = count * width
    vaddr = descriptor["vaddr"]
    if type(vaddr) is not int or vaddr % width:
        raise TableExportError("table address is unaligned for its declared type")
    load_address = header["load_address"]
    if vaddr < load_address or vaddr + size > header["loaded_vaddr_end_exclusive"]:
        raise TableExportError("table extent is outside the verified loaded executable")
    file_offset = header["payload_file_offset"] + vaddr - load_address
    if file_offset < HEADER_SIZE or file_offset + size > len(exe_bytes):
        raise TableExportError("table extent is outside the executable file")
    raw = exe_bytes[file_offset:file_offset + size]
    if len(raw) != size:
        raise TableExportError("table read is short")
    values = list(struct.unpack("<" + fmt * count, raw))
    if any(value < low or value > high for value in values):
        raise TableExportError("table value is outside the declared numeric type")

    def nested(offset, dimensions):
        if len(dimensions) == 1:
            end = offset + dimensions[0]
            return values[offset:end], end
        rows = []
        for _ in range(dimensions[0]):
            row, offset = nested(offset, dimensions[1:])
            rows.append(row)
        return rows, offset

    shaped, end = nested(0, descriptor["shape"])
    if end != count:
        raise TableExportError("table shape does not consume its exact value count")
    return shaped, raw, file_offset, size


def _reader_executable(input_path):
    with open_input(input_path) as source:
        entries = source.entries()
        candidates = [entry for entry in entries if entry.get("kind") == "file"
                      and str(entry.get("path", "")).split("/")[-1] == EXE_ENTRY]
        if len(candidates) != 1:
            raise TableExportError("original input must contain exactly one SLPM_86.053 executable")
        entry = candidates[0]
        executable = source.read(entry)
        digest = sha256(executable)
        if digest != EXE_SHA256:
            raise TableExportError("SLPM_86.053 does not match the verified original release SHA-256")
        input_info = source.describe()
        if input_info["format"] == "mode2-2352" and input_info["source_sha256"] != TRACK_SHA256:
            raise TableExportError("MODE2 track SHA-256 is not the verified original release input")
        location = entry.get("location", {})
        if input_info["format"] in ("mode2-2352", "zip-mode2-2352"):
            location = {key: location[key] for key in ("lba", "extent_bytes", "record_lba", "record_offset") if key in location}
        else:
            location = {key: location[key] for key in ("path",) if key in location}
        entry = dict(entry)
        entry["location"] = location
        return executable, input_info, entry


def validate_table_contract(bundle):
    if not isinstance(bundle, dict) or set(bundle) != {"schema", "schema_version", "tables", "provenance"}:
        raise TableExportError("bundle fields do not match rule-table v1")
    if bundle["schema"] != SCHEMA or type(bundle["schema_version"]) is not int or bundle["schema_version"] != SCHEMA_VERSION:
        raise TableExportError("unsupported rule-table schema/version")
    if not isinstance(bundle["tables"], dict) or set(bundle["tables"]) != set(TABLES):
        raise TableExportError("table names do not match bounded school-day contract")
    proof_root = bundle["provenance"]
    if not isinstance(proof_root, dict) or set(proof_root) != {"reader", "reader_source_sha256", "input", "release_executable", "tables", "exporter"}:
        raise TableExportError("provenance fields do not match rule-table v1")
    if proof_root.get("reader") != "tools.assets.input.open_input" or not re.fullmatch(r"[0-9a-f]{64}", str(proof_root.get("reader_source_sha256", ""))):
        raise TableExportError("original input reader provenance is missing")
    input_proof = proof_root.get("input")
    if not isinstance(input_proof, dict) or input_proof.get("entry_path", "").split("/")[-1] != EXE_ENTRY or not re.fullmatch(r"[0-9a-f]{64}", str(input_proof.get("source_sha256", ""))):
        raise TableExportError("original input provenance is invalid")
    exe_proof = proof_root.get("release_executable")
    if not isinstance(exe_proof, dict) or exe_proof.get("sha256") != EXE_SHA256 or exe_proof.get("release") != RELEASE:
        raise TableExportError("release executable identity is not the verified original")
    header = exe_proof.get("psx_exe_header")
    if (not isinstance(header, dict) or header.get("magic") != "PS-X EXE"
            or header.get("load_address") != EXPECTED_LOAD_ADDRESS
            or header.get("text_size_bytes") != EXPECTED_TEXT_SIZE
            or header.get("payload_file_offset") != HEADER_SIZE):
        raise TableExportError("PS-X EXE address mapping provenance is invalid")
    table_provenance = proof_root.get("tables")
    if not isinstance(table_provenance, dict) or set(table_provenance) != set(TABLES):
        raise TableExportError("per-table provenance is incomplete")
    for name, desc in TABLES.items():
        values = bundle["tables"][name]
        _validate_values(values, desc["shape"], desc["type"])
        proof = table_provenance.get(name)
        expected_size = product(desc["shape"]) * FORMATS[desc["type"]][0]
        expected_offset = HEADER_SIZE + desc["vaddr"] - EXPECTED_LOAD_ADDRESS
        if (not isinstance(proof, dict) or proof.get("symbol") != desc["symbol"]
                or proof.get("virtual_address") != f"0x{desc['vaddr']:08X}"
                or proof.get("file_offset") != expected_offset or proof.get("size_bytes") != expected_size
                or proof.get("element_type") != desc["type"] or proof.get("shape") != desc["shape"]):
            raise TableExportError(f"{name} table provenance does not match descriptor")
        width, fmt, _low, _high = FORMATS[desc["type"]]
        raw_values = []
        def flatten(value):
            for entry in value:
                if isinstance(entry, list):
                    flatten(entry)
                else:
                    raw_values.append(entry)
        flatten(values)
        raw = struct.pack("<" + fmt * len(raw_values), *raw_values)
        if len(raw) != expected_size or sha256(raw) != proof.get("raw_sha256"):
            raise TableExportError(f"{name} table values do not match their raw-byte digest")
        if not isinstance(proof.get("source_evidence"), list) or not proof["source_evidence"]:
            raise TableExportError(f"{name} source evidence is missing")
    exporter = proof_root.get("exporter")
    if not isinstance(exporter, dict) or exporter.get("name") != "tools/port/school_day_tables.py" or exporter.get("version") != 1 or not re.fullmatch(r"[0-9a-f]{64}", str(exporter.get("source_sha256", ""))):
        raise TableExportError("exporter provenance is invalid")
    return bundle


def _validate_values(values, shape, element_type, depth=0):
    if element_type not in FORMATS:
        raise TableExportError("unsupported numeric table type")
    width, _fmt, low, high = FORMATS[element_type]
    if depth < len(shape) - 1:
        if not isinstance(values, list) or len(values) != shape[depth]:
            raise TableExportError("nested table array shape does not match descriptor")
        for row in values:
            _validate_values(row, shape, element_type, depth + 1)
        return
    if not isinstance(values, list) or len(values) != shape[depth]:
        raise TableExportError("table array value count does not match descriptor")
    for value in values:
        if type(value) is not int or value < low or value > high:
            raise TableExportError("table value is not a valid integer of declared type")


def _assert_no_symlink_path(path):
    """Inspect every lexical path component without resolving a symlink first."""
    absolute = Path(path).absolute()
    current = Path(absolute.anchor)
    for part in absolute.parts[1:]:
        if part in ("", ".", ".."):
            raise TableExportError("export path contains a noncanonical component")
        current = current / part
        if current.is_symlink():
            raise TableExportError("export path contains a symlink: " + str(current))
    return absolute


def _check_export_path(output_path, repository_root=ROOT):
    """Guard repository ancestry, ignored cache ancestry, and destination before writes."""
    root = _assert_no_symlink_path(Path(repository_root))
    allowed_root = _assert_no_symlink_path(root / ".asset-cache" / "port")
    output = _assert_no_symlink_path(Path(output_path))
    try:
        relative = output.relative_to(allowed_root)
    except ValueError as exc:
        raise TableExportError("rule-table export output must be under ignored .asset-cache/port/") from exc
    if not relative.parts or any(part in ("", ".", "..") for part in relative.parts):
        raise TableExportError("rule-table output must name a safe pack directory")
    if output.exists() or output.is_symlink():
        raise TableExportError("refusing to overwrite an existing rule-table pack")
    return output, allowed_root


def build_bundle(input_path):
    exe, input_info, entry = _reader_executable(input_path)
    header = parse_psx_exe(exe)
    config_text = (ROOT / "config/symbol_addrs.txt").read_text(encoding="utf-8")
    tables, table_proof = {}, {}
    for name, desc in TABLES.items():
        if "config_symbol" in desc:
            match = re.search(r"^\s*" + re.escape(desc["config_symbol"]) + r"\s*=\s*(0x[0-9A-Fa-f]+)\s*;", config_text, re.M)
            if not match or int(match.group(1), 16) != desc["vaddr"]:
                raise TableExportError(f"{name} symbol address differs from canonical symbol map")
        evidence = []
        for relative, needle in desc["evidence"]:
            evidence_path = (ROOT / relative).resolve()
            try:
                evidence_path.relative_to(ROOT)
            except ValueError as exc:
                raise TableExportError("source evidence path escapes repository") from exc
            data = evidence_path.read_bytes()
            if needle.encode("utf-8") not in data:
                raise TableExportError(f"named source evidence missing for {name}: {relative}")
            evidence.append({"path": relative, "sha256": sha256(data), "reference": needle})
        values, raw, file_offset, size = _read_descriptor(exe, header, desc)
        tables[name] = values
        table_proof[name] = {"symbol": desc["symbol"], "virtual_address": f"0x{desc['vaddr']:08X}",
            "file_offset": file_offset, "size_bytes": size, "raw_sha256": sha256(raw),
            "element_type": desc["type"], "shape": desc["shape"], "source_evidence": evidence}
    bundle = {"schema": SCHEMA, "schema_version": SCHEMA_VERSION, "tables": tables,
        "provenance": {"reader": "tools.assets.input.open_input", "reader_source_sha256": sha256((ROOT / "tools/assets/input.py").read_bytes()), "input": {
            "format": input_info["format"], "source_sha256": input_info["source_sha256"],
            "source_path": Path(input_path).name, "entry_path": entry["path"], "entry_size_bytes": entry["size"],
            "location": entry.get("location", {})},
            "release_executable": {"release": RELEASE, "entry_path": EXE_ENTRY,
                "sha256": sha256(exe), "size_bytes": len(exe), "psx_exe_header": header},
            "tables": table_proof, "exporter": {"name": "tools/port/school_day_tables.py", "version": 1,
                "source_sha256": sha256(Path(__file__).read_bytes())}}}
    return validate_table_contract(bundle)


def export(input_path, output_path):
    output, _allowed_root = _check_export_path(output_path)
    bundle = build_bundle(input_path)
    _check_export_path(output, ROOT)
    output.parent.mkdir(parents=True, exist_ok=True)
    _assert_no_symlink_path(output.parent)
    if output.exists() or output.is_symlink():
        raise TableExportError("refusing to overwrite an existing rule-table pack")
    stage = Path(tempfile.mkdtemp(prefix=".school-day-tables-", dir=output.parent))
    try:
        (stage / "manifest.json").write_text(json.dumps(bundle, sort_keys=True, indent=2) + "\n", encoding="utf-8")
        _assert_no_symlink_path(output.parent)
        if output.exists() or output.is_symlink():
            raise TableExportError("destination appeared during export; refusing overwrite")
        os.rename(stage, output)
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise
    return output


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, help="Read-only original directory or MODE2/2352 track")
    parser.add_argument("--output", required=True, help="New ignored directory under .asset-cache/port")
    args = parser.parse_args()
    print(export(args.input, args.output) / "manifest.json")


if __name__ == "__main__":
    main()
