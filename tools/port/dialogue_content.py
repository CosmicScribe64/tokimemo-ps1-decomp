"""Project verified SERIFU reader output into an external dialogue pack (T-29011).

Consumes tokimemo.text.catalog.v1 from tools/assets/text.py. Source bytes,
stable IDs, command values, token records and voice extents are retained.
Output belongs in ignored .asset-cache/port/ and must never be committed.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
from pathlib import PurePosixPath
import re
import sys
import tempfile

SCHEMA = "tokimemo.dialogue-content.v1"
SUPPORTED_COMMANDS = set(range(20)) | {0xFF}
MAX_GROUPS = 4096
MAX_RECORDS = 65536
MAX_RAW_BYTES = 65536
VOICE_COMMANDS = frozenset((1, 2, 5, 7, 10, 11, 12, 13, 15, 18))


class DialogueContentError(ValueError):
    """Malformed catalog or unsupported dialogue table."""


def _fail(message: str) -> None:
    raise DialogueContentError(message)


def _sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _repository_root() -> Path:
    """Find the canonical checkout from tools/port or an isolated candidate copy."""
    for parent in Path(__file__).resolve().parents:
        if (parent / "tools" / "assets" / "script_profiles.py").is_file():
            return parent
    _fail("cannot locate repository root containing trusted script profiles")


def _trusted_profile(source_path: str, section_id: str) -> tuple[dict, dict]:
    """Resolve a unique source layout from the existing reviewed text profiles."""
    root = _repository_root()
    if str(root) not in sys.path:
        sys.path.insert(0, str(root))
    from tools.assets.script_profiles import PROFILES
    profiles = [item for item in PROFILES if item.get("path") == source_path]
    if len(profiles) != 1:
        _fail("source path does not have one canonical linked-table profile")
    profile = profiles[0]
    sections = [item for item in profile.get("sections", []) if item.get("id") == section_id]
    if len(sections) != 1:
        _fail("section is not declared by the canonical source profile")
    return profile, sections[0]


def _normalize_digest_value(value):
    """Normalize JSON numbers to decimal strings for matching Python/Godot hashes."""
    if isinstance(value, bool) or value is None or isinstance(value, str):
        return value
    if isinstance(value, int):
        return str(value)
    if isinstance(value, float):
        if not value.is_integer():
            _fail("content digest cannot contain fractional numbers")
        return str(int(value))
    if isinstance(value, list):
        return [_normalize_digest_value(item) for item in value]
    if isinstance(value, dict):
        if any(not isinstance(key, str) for key in value):
            _fail("content digest object contains a non-string key")
        return {key: _normalize_digest_value(value[key]) for key in sorted(value)}
    _fail("content digest contains an unsupported JSON value")


def content_digest(component: dict) -> str:
    """Hash the canonical component with JSON numbers represented as strings."""
    material = {key: value for key, value in component.items() if key != "content_sha256"}
    canonical = json.dumps(_normalize_digest_value(material), sort_keys=True,
                           ensure_ascii=False, separators=(",", ":")).encode("utf-8")
    return _sha256(canonical)


def verify_against_original(catalog: dict, source_path: str, section_id: str,
                            input_path: Path) -> dict:
    """Re-read the original file and return current canonical reader output."""
    root = _repository_root()
    if str(root) not in sys.path:
        sys.path.insert(0, str(root))
    from tools.assets.input import open_input
    from tools.assets.text import parse_script
    profile, _ = _trusted_profile(source_path, section_id)
    with open_input(str(input_path)) as source:
        entries = [entry for entry in source.entries() if entry.get("path") == source_path]
        if len(entries) != 1:
            _fail("original input does not contain exactly one selected source file")
        data = source.read(entries[0])
    expected = parse_script(data, source_path, profile)
    matches = [item for item in catalog.get("files", [])
               if isinstance(item, dict) and item.get("path") == source_path]
    if len(matches) != 1 or matches[0].get("status") != "exported":
        _fail("catalog does not contain one exported original source")
    catalog_script = matches[0].get("script")
    if (not isinstance(catalog_script, dict) or
            catalog_script.get("source", {}).get("path") != source_path or
            catalog_script.get("source", {}).get("sha256") != profile.get("sha256")):
        _fail("catalog source identity differs from the canonical original-input profile")
    return expected


def _reject_symlink_path(path: Path) -> None:
    """Reject symlinks in existing output path segments, including the cache root."""
    current = Path(path.anchor)
    for part in path.parts[1:]:
        current = current / part
        if current.is_symlink():
            _fail("output path cannot contain symlinks")


def project_catalog(catalog: dict, source_path: str, section_id: str) -> dict:
    """Build a source-bound route without decoding or rewriting text."""
    if not isinstance(catalog, dict) or catalog.get("schema") != "tokimemo.text.catalog.v1":
        _fail("expected tokimemo.text.catalog.v1 from the shipped text reader")
    if (not isinstance(source_path, str) or not source_path.startswith("CDROM/SERIFU/")
            or "\\" in source_path or PurePosixPath(source_path).as_posix() != source_path
            or ".." in source_path.split("/")):
        _fail("route source must be a normalized CDROM/SERIFU path")
    if not isinstance(section_id, str) or not section_id or len(section_id) > 128:
        _fail("section ID must be a nonempty bounded string")
    profile, trusted_section = _trusted_profile(source_path, section_id)
    files = catalog.get("files")
    if not isinstance(files, list) or any(not isinstance(item, dict) for item in files):
        _fail("catalog file list is missing or malformed")
    matches = [f for f in files if f.get("path") == source_path]
    if len(matches) != 1 or matches[0].get("status") != "exported":
        _fail("selected SERIFU source is missing, duplicated, or not source-profile exported")
    script = matches[0].get("script")
    if not isinstance(script, dict) or script.get("schema") != "tokimemo.text.v1":
        _fail("selected source lacks tokimemo.text.v1 structured reader output")
    source = script.get("source", {})
    if not isinstance(source, dict) or source.get("path") != source_path:
        _fail("structured source path does not match the selected catalog path")
    source_hash = source.get("sha256")
    source_size = source.get("size")
    if (source.get("path") != source_path or source_hash != profile.get("sha256")
            or type(source_size) is not int or source_size <= 0):
        _fail("source path or checksum does not match the canonical linked-table profile")
    if script.get("profile_id") != profile.get("id"):
        _fail("reader profile ID does not match the canonical linked-table profile")
    all_records = script.get("records")
    if not isinstance(all_records, list):
        _fail("structured reader records are missing or malformed")
    if any(not isinstance(record, dict) for record in all_records):
        _fail("structured reader contains a malformed record")
    selected = [r for r in all_records if r.get("section") == section_id]
    if not selected or len(selected) > MAX_RECORDS:
        _fail("selected section is empty or exceeds the bounded record count")
    selected.sort(key=lambda r: (r.get("group", -1), r.get("line", -1)))
    groups: dict[int, list[dict]] = {}
    seen_ids: set[str] = set()
    for record in selected:
        group, line = record.get("group"), record.get("line")
        command_row = record.get("command")
        if not isinstance(command_row, dict):
            _fail("structured record command metadata is missing or malformed")
        command = command_row.get("value")
        if type(group) is not int or not 0 <= group < MAX_GROUPS or type(line) is not int or line < 0:
            _fail("table has an invalid group or line index")
        if type(command) is not int or command not in SUPPORTED_COMMANDS:
            _fail("table has a command value outside the source command set")
        if command != 0xFF and not isinstance(record.get("text"), dict):
            _fail("non-terminator record is missing its structured text")
        ident = record.get("id")
        expected_id = f"talk:{source_path}:{section_id}:g{group:04d}:l{line:04d}"
        if not isinstance(ident, str) or ident != expected_id or ident in seen_ids:
            _fail("record ID is missing, duplicated, or not bound to its source cursor")
        seen_ids.add(ident)
        command_offset = command_row.get("offset")
        if type(command_offset) is not int or not 0 <= command_offset < source_size:
            _fail("command locator is outside the canonical source extent")
        row = {"id": ident, "line": line, "command": command,
               "command_name": command_row.get("name"),
               "source_locator": {"path": source_path, "source_sha256": source_hash,
                                  "text_offset": (record.get("text") or {}).get("offset"),
                                  "command_offset": command_row.get("offset")}}
        if command != 0xFF:
            text = record["text"]
            try:
                raw = bytes.fromhex(text.get("raw_hex", ""))
            except (TypeError, ValueError):
                _fail("reader output contains malformed raw text bytes")
            if len(raw) > MAX_RAW_BYTES or b"\0" in raw:
                _fail("text payload exceeds bounds or includes an embedded terminator")
            text_offset = text.get("offset")
            if type(text_offset) is not int or text_offset < 0 or text_offset + len(raw) > source_size:
                _fail("text locator is outside the canonical source extent")
            row["text"] = {"raw_hex": raw.hex(), "tokens": text.get("tokens", []),
                           "unknown_token_count": text.get("unknown_token_count", 0),
                           "inline_label": text.get("speaker_label")}
            voice = record.get("voice")
            if command in VOICE_COMMANDS and not isinstance(voice, dict):
                _fail("voice-bearing command is missing its canonical voice extent")
            if voice is not None:
                if not isinstance(voice, dict) or not isinstance(voice.get("pairs"), list):
                    _fail("voice extent structure is malformed")
                row["voice"] = {"speaker": voice.get("speaker"), "pairs": voice.get("pairs", [])}
        elif record.get("voice") is not None:
            _fail("group terminator cannot carry a voice extent")
        groups.setdefault(group, []).append(row)
    output_groups = []
    expected_group_count = trusted_section.get("group_count")
    for group_index in sorted(groups):
        records = groups[group_index]
        if [r["line"] for r in records] != list(range(len(records))):
            _fail("group line indices are not contiguous from zero")
        terminators = [i for i, r in enumerate(records) if r["command"] == 0xFF]
        if terminators != [len(records) - 1]:
            _fail("each group must have one final 0xFF terminator")
        output_groups.append({"index": group_index, "records": records})
    indices = [g["index"] for g in output_groups]
    if type(expected_group_count) is not int or indices != list(range(expected_group_count)):
        _fail("selected groups do not match the canonical profile group count")
    result = {"schema": SCHEMA, "profile": "serifu-native-v1",
              "source": {"path": source_path, "sha256": source_hash,
                         "size_bytes": source_size,
                         "reader_schema": "tokimemo.text.v1"},
              "profile_id": profile["id"], "section_id": section_id,
              "groups": output_groups,
              "voice_timing": "preserved_extents_runtime_mapping_unproved",
              "text_encoding": "original_bytes_with_reader_token_view",
              "semantic_coverage_complete": False}
    result["content_sha256"] = content_digest(result)
    return result


def write_pack(catalog_path: Path, source_path: str, section_id: str,
               input_path: Path, output_path: Path) -> None:
    """Write an immutable dialogue component below an ignored pack directory."""
    repo = _repository_root()
    raw_output = os.fspath(output_path)
    if "\\" in raw_output or any(part == ".." for part in raw_output.split("/")):
        _fail("output path cannot contain parent traversal")
    output_path = Path(os.path.abspath(output_path))
    lexical_allowed = repo / ".asset-cache" / "port"
    _reject_symlink_path(lexical_allowed)
    allowed = lexical_allowed.resolve()
    resolved_output = output_path.resolve(strict=False)
    if allowed not in resolved_output.parents or output_path.exists():
        _fail("output must be a new immutable child of .asset-cache/port/")
    _reject_symlink_path(resolved_output)
    if resolved_output != output_path:
        _fail("output path resolves through a symlink or noncanonical segment")
    catalog = json.loads(catalog_path.read_text(encoding="utf-8"))
    fresh_script = verify_against_original(catalog, source_path, section_id, input_path)
    fresh_catalog = dict(catalog)
    fresh_files = [dict(item) if isinstance(item, dict) else item
                   for item in catalog.get("files", [])]
    selected = [index for index, item in enumerate(fresh_files)
                if isinstance(item, dict) and item.get("path") == source_path]
    if len(selected) != 1:
        _fail("catalog selected source became ambiguous during fresh validation")
    fresh_files[selected[0]]["script"] = fresh_script
    fresh_catalog["files"] = fresh_files
    result = project_catalog(fresh_catalog, source_path, section_id)
    resolved_output.parent.mkdir(parents=True, exist_ok=True)
    data = (json.dumps(result, sort_keys=True, ensure_ascii=False, indent=2) + "\n").encode("utf-8")
    with tempfile.NamedTemporaryFile(dir=resolved_output.parent, prefix=".dialogue-", delete=False) as handle:
        temp = Path(handle.name)
        handle.write(data)
    try:
        os.link(temp, resolved_output)
    finally:
        temp.unlink(missing_ok=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("catalog", type=Path)
    parser.add_argument("source_path")
    parser.add_argument("section_id")
    parser.add_argument("--input", required=True, type=Path,
                        help="read-only original directory, MODE2 track, or supported archive")
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    write_pack(args.catalog, args.source_path, args.section_id, args.input, args.output)
    print("wrote immutable dialogue component: " + str(args.output))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
