"""Canonical, lossless layout-only inventory for the twelve SPEAKTST banks.

All exported records remain flagged and unlinked. This module does not infer
voice, speaker, character, runtime installation, or execution semantics.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath

from .text import TextFormatError
from .text_speaktst import (
    PROFILE_SHA256,
    SPEAKTST_PROFILES,
    parse_speaktst_layout,
    reconstruct_speaktst_layout,
    validate_speaktst_layout,
)

SCHEMA = "tokimemo.speaktst.layout-inventory.v1"
TOTAL_BYTES = 1_253_376
TOTAL_RECORDS = 16_702
TOTAL_CONTROLS = 12
PROFILE_COUNT = 12
UNKNOWN_TOKENS = 4
OUTPUT_RELATIVE = Path(".asset-cache/text/speaktst-layout")


def _require(ok, message):
    if not ok:
        raise TextFormatError(message)


def _sha(data):
    return hashlib.sha256(data).hexdigest()


def _json_bytes(value):
    return (json.dumps(value, sort_keys=True, ensure_ascii=False, allow_nan=False,
                       indent=2) + "\n").encode("utf-8")


def _safe_relative(value):
    _require(isinstance(value, str) and "\\" not in value, "unsafe relative path")
    path = PurePosixPath(value)
    _require(value and not path.is_absolute() and str(path) == value and
             all(part not in ("", ".", "..") for part in path.parts), "unsafe relative path")
    return path


def _absolute_without_symlinks(value, label):
    raw = Path(value)
    _require(".." not in raw.parts, "parent traversal in " + label)
    path = raw if raw.is_absolute() else Path.cwd() / raw
    path = Path(os.path.normpath(str(path)))
    current = Path(path.anchor)
    for part in path.parts[1:]:
        current = current / part
        _require(not current.is_symlink(), "symlink in " + label)
    return path


def _profiles():
    paths = tuple(SPEAKTST_PROFILES)
    _require(len(paths) == PROFILE_COUNT and len(set(paths)) == PROFILE_COUNT,
             "unexpected shipped profile count or duplicate path")
    for value in paths:
        _safe_relative(value)
    return paths


def _implementation_pins():
    asset_dir = Path(__file__).resolve().parent
    repo_root = asset_dir.parents[1]
    files = ("tools/assets/text_speaktst_inventory.py", "tools/assets/text_speaktst.py",
             "tools/assets/text_speaktst_profiles.json", "tools/assets/text.py")
    result = {}
    for relative in files:
        path = repo_root / relative
        _require(path.is_file() and not path.is_symlink(), "missing trusted shipped module: " + relative)
        result[relative] = _sha(path.read_bytes())
    return {"files": result, "python": "stdlib-only"}


def _input_file(root, relative):
    rel = _safe_relative(relative)
    path = root
    for part in rel.parts:
        path = path / part
        _require(not path.is_symlink(), "symlink in original input path")
    _require(path.is_file(), "missing original input: " + relative)
    return path


def collect_speaktst_layouts(input_root):
    """Parse every shipped profile from an extracted-input root, in stable order."""
    root = _absolute_without_symlinks(input_root, "SPEAKTST input root")
    _require(root.is_dir(), "invalid SPEAKTST input root")
    banks = []
    for relative in _profiles():
        data = _input_file(root, relative).read_bytes()
        bank = parse_speaktst_layout(data, relative)
        banks.append(bank)
    _check_collection(banks)
    return banks


def _check_collection(banks):
    _require(isinstance(banks, list) and len(banks) == PROFILE_COUNT, "unexpected SPEAKTST bank count")
    _require([bank.get("source", {}).get("path") for bank in banks] == list(_profiles()),
             "SPEAKTST bank order/path set differs from shipped profiles")
    _require(sum(len(bank.get("records", [])) for bank in banks) == TOTAL_RECORDS,
             "SPEAKTST record cardinality mismatch")
    _require(sum(len(bank.get("controls", [])) for bank in banks) == TOTAL_CONTROLS,
             "SPEAKTST control cardinality mismatch")
    _require(sum(bank.get("source", {}).get("size", -1) for bank in banks) == TOTAL_BYTES,
             "SPEAKTST source byte cardinality mismatch")
    for bank in banks:
        _require(bank.get("status") == "layout_only_inventory", "invalid SPEAKTST bank status")
        _require(bank.get("coverage", {}).get("semantic_coverage_complete") is False and
                 bank.get("coverage", {}).get("runtime_execution_proven") is False and
                 bank.get("coverage", {}).get("runtime_file_installation_proven") is False,
                 "unsupported SPEAKTST claim")
        for row in bank["records"]:
            _require(row.get("target_text") is None and row.get("runtime_usage_proven") is False and
                     row.get("readiness", {}).get("translation_ready") is False and
                     row.get("readiness", {}).get("status") == "flagged", "SPEAKTST record is not flagged")
        for row in bank["controls"]:
            _require(row.get("runtime_usage_proven") is False,
                     "SPEAKTST control runtime use is not proved")
    unknown = sum(token.get("kind") == "unknown" for bank in banks
                  for row in bank["records"] for token in row.get("tokens", []))
    _require(unknown == UNKNOWN_TOKENS, "SPEAKTST unknown-token cardinality mismatch")


def _file_map(banks):
    outputs = {}
    for bank in banks:
        path = _safe_relative(bank["source"]["path"])
        name = path.name + ".json"
        _require(name not in outputs, "duplicate SPEAKTST output basename")
        outputs[name] = _json_bytes(bank)
    manifest = {
        "schema": SCHEMA,
        "status": "layout_only_inventory",
        "implementation_pins": _implementation_pins(),
        "profile_manifest_sha256": PROFILE_SHA256,
        "banks": [{"path": bank["source"]["path"], "sha256": bank["source"]["sha256"],
                   "size": bank["source"]["size"], "load_address": bank["source"]["load_address"],
                   "file": bank["source"]["path"].split("/")[-1] + ".json",
                   "text_records": len(bank["records"]), "controls": len(bank["controls"]),
                   "opaque_ranges": len(bank["opaque_ranges"])} for bank in banks],
        "text_records": TOTAL_RECORDS,
        "control_records": TOTAL_CONTROLS,
        "source_bytes": TOTAL_BYTES,
        "unknown_tokens": UNKNOWN_TOKENS,
        "all_records_flagged": True,
        "all_targets_null": True,
        "voice_semantics_proven": False,
        "character_semantics_proven": False,
        "runtime_installation_proven": False,
        "runtime_execution_proven": False,
        "semantic_coverage_complete": False,
    }
    outputs["manifest.json"] = _json_bytes(manifest)
    return outputs


def _output_path(repository, output=None):
    repo = _absolute_without_symlinks(repository, "repository root")
    _require(repo.is_dir(), "invalid repository root")
    output_root = repo / OUTPUT_RELATIVE.parent
    target = (_absolute_without_symlinks(output, "inventory output") if output is not None
              else repo / OUTPUT_RELATIVE)
    _require(target != output_root and output_root in target.parents,
             "output must be a descendant of repository .asset-cache/text")
    relative_target = target.relative_to(output_root)
    _require(all(part not in ("", ".", "..") for part in relative_target.parts),
             "unsafe inventory output path")
    current = repo
    _require(not repo.is_symlink(), "repository root symlink rejected")
    relative = target.relative_to(repo)
    for part in relative.parts:
        current = current / part
        _require(not current.is_symlink(), "symlink in inventory output path")
        if current.exists():
            _require(current.is_dir(), "inventory output path component is not a directory")
    return target


def _validation_result(outputs):
    return {"status": "validated_layout_only_inventory", "banks": PROFILE_COUNT,
            "text_records": TOTAL_RECORDS, "control_records": TOTAL_CONTROLS,
            "source_bytes_reconstructed": TOTAL_BYTES, "all_fields_canonical": True,
            "all_targets_null": True, "all_records_flagged": True,
            "semantic_coverage_complete": False, "runtime_installation_proven": False,
            "runtime_execution_proven": False,
            "inventory_sha256": _sha(outputs["manifest.json"])}


def _preflight(directory, outputs, canonical_names=None, validation_payload=None):
    canonical_names = set(outputs) if canonical_names is None else set(canonical_names)
    allowed = canonical_names | {"validation.json"}
    if directory.exists():
        _require(directory.is_dir() and not directory.is_symlink(), "unsafe inventory output directory")
        present = set()
        for item in directory.iterdir():
            _require(item.name in allowed, "unexpected file in inventory output")
            _require(not item.is_symlink() and item.is_file() and item.stat().st_nlink == 1,
                     "unsafe file in inventory output")
            present.add(item.name)
        for name, payload in outputs.items():
            item = directory / name
            if item.exists():
                _require(item.read_bytes() == payload, "conflicting existing inventory file: " + name)
        receipt = directory / "validation.json"
        if receipt.exists() and validation_payload is not None and "validation.json" not in outputs:
            _require(receipt.read_bytes() == validation_payload,
                     "conflicting existing validation receipt")
    else:
        _require(not directory.is_symlink(), "symlink inventory output rejected")


def _write_exclusive(directory, outputs, canonical_names=None, validation_payload=None):
    directory.mkdir(parents=True, exist_ok=True)
    _preflight(directory, outputs, canonical_names, validation_payload)
    for name, payload in outputs.items():
        path = directory / name
        if path.exists():
            continue
        try:
            fd = os.open(path, os.O_WRONLY | os.O_CREAT | os.O_EXCL, 0o600)
        except FileExistsError as error:
            raise TextFormatError("inventory output appeared during write") from error
        with os.fdopen(fd, "wb") as stream:
            stream.write(payload)


def export_inventory(input_root, repository, output=None):
    """Preflight and exclusively write canonical banks; matching reruns are safe."""
    directory = _output_path(repository, output)
    banks = collect_speaktst_layouts(input_root)
    outputs = _file_map(banks)
    validation_payload = _json_bytes(_validation_result(outputs))
    _preflight(directory, outputs, validation_payload=validation_payload)
    _write_exclusive(directory, outputs, validation_payload=validation_payload)
    return {"status": "exported_layout_only_inventory", "banks": PROFILE_COUNT,
            "text_records": TOTAL_RECORDS, "control_records": TOTAL_CONTROLS,
            "source_bytes": TOTAL_BYTES, "output": str(directory)}


def validate_inventory(input_root, repository, output=None):
    """Regenerate and compare all fields, then validate/reconstruct stored banks."""
    directory = _output_path(repository, output)
    _require(directory.is_dir() and not directory.is_symlink(), "missing inventory directory")
    banks = collect_speaktst_layouts(input_root)
    expected = _file_map(banks)
    _preflight(directory, expected)
    present = set(item.name for item in directory.iterdir())
    _require(present in (set(expected), set(expected) | {"validation.json"}),
             "inventory output set is incomplete")
    persisted = {}
    for name, canonical in expected.items():
        raw = (directory / name).read_bytes()
        _require(raw == canonical, "persisted canonical file differs: " + name)
        persisted[name] = json.loads(raw)
    root = Path(input_root).absolute()
    for bank in banks:
        path = bank["source"]["path"]
        original = _input_file(root, path).read_bytes()
        name = path.split("/")[-1] + ".json"
        persisted_bank = persisted[name]
        validate_speaktst_layout(original, path, persisted_bank)
        _require(reconstruct_speaktst_layout(persisted_bank) == original,
                 "persisted source spans do not reconstruct original")
    result = _validation_result(expected)
    _write_exclusive(directory, {"validation.json": _json_bytes(result)}, expected)
    return result


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("operation", choices=("export", "validate"))
    parser.add_argument("--input-root", type=Path, required=True)
    parser.add_argument("--repository", type=Path, default=Path.cwd())
    parser.add_argument("--output", type=Path)
    args = parser.parse_args(argv)
    result = (export_inventory(args.input_root, args.repository, args.output)
              if args.operation == "export" else
              validate_inventory(args.input_root, args.repository, args.output))
    print(json.dumps({key: value for key, value in result.items() if key != "output"},
                     sort_keys=True))


if __name__ == "__main__":
    main()
