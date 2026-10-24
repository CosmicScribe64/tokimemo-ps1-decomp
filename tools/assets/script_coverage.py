"""Numeric coverage audit for source-profiled linked script containers.

This module summarizes bytes claimed by explicit talk-table profiles and
reports heuristic Shift-JIS-looking strings in unclaimed ranges as leads only.
The heuristic is not a completeness proof; only source-backed table roots and
their validated links are treated as dialogue records. No text bytes or decoded
strings are emitted.
"""
from __future__ import annotations

import argparse
import json
import re
from pathlib import Path

from .script_profiles import PROFILES
from .text import parse_script

_DATE_FILLERS = {
    "FU": "shiori", "KI": "mio", "HI": "yuina", "KA": "ayako",
    "NI": "saki", "KO": "yukari", "KY": "nozomi", "KG": "mira",
    "AS": "yuko", "MI": "megumi", "SA": "yumi",
}
_EVENT_CONSUMER_SOURCES = {
    "FU": "8014F0F0.c", "KI": "80154260.c", "HI": "80150010.c",
    "KA": "80151460.c", "NI": "801586A0.c", "KO": "801552B0.c",
    "KY": "80156600.c", "KG": "80152FC0.c", "AS": "8014CC40.c",
    "MI": "80157880.c", "SA": "801594B0.c", "OT": "8015A190.c",
}
_EVENT_INITIALIZER_INDEXES = {
    "AS": set(range(0, 6)), "FU": set(range(6, 14)),
    "HI": set(range(14, 21)), "OT": set(range(21, 24)) | set(range(74, 79)),
    "KA": set(range(24, 30)), "KG": set(range(30, 37)),
    "KI": set(range(37, 42)), "KO": set(range(42, 49)),
    "KY": set(range(49, 54)), "MI": set(range(54, 61)),
    "NI": set(range(61, 67)), "SA": set(range(67, 74)),
}


def _assignments(source: str) -> dict[str, int]:
    return {name: int(value, 16) for name, value in re.findall(
        r"(D_8015\w+)\s*=\s*(0x[0-9A-Fa-f]+);", source
    )}


def _function_body(repo_root: Path, evidence: str) -> tuple[str, str]:
    """Read a cited C initializer function and return (path, balanced body)."""
    source_ref, function = evidence.split("::", 1)
    source = (repo_root / source_ref).read_text(encoding="utf-8")
    match = re.search(r"\b" + re.escape(function) + r"\s*\([^;]*?\)\s*\{", source)
    if not match:
        raise ValueError("missing source initializer: " + evidence)
    start = match.end()
    depth = 1
    index = start
    while index < len(source) and depth:
        if source[index] == "{":
            depth += 1
        elif source[index] == "}":
            depth -= 1
        index += 1
    if depth:
        raise ValueError("unterminated source initializer: " + evidence)
    return source_ref, source[start:index - 1]


def _audit_simple_source_profiles(repo_root: Path, profiles: list[dict],
                                  selected: list[dict]) -> dict:
    """Check source initializer address inventories against configured roots."""
    output = []
    missing_total = extra_total = 0
    for profile in selected:
        refs = [ref for ref in profile.get("evidence", [])
                if "::func_" in ref]
        if profile["id"] == "omimai-p-birth":
            refs = ["src/ovl/OMIMAI/80132AD0.c::func_80132AD0"]
        if not refs:
            output.append({"id": profile["id"], "source_initializer_count": 0,
                           "status": "no_func_evidence"})
            continue
        source_ref, body = _function_body(repo_root, refs[0])
        constants = {int(value, 16) for value in re.findall(
            r"=\s*(0x80[0-9A-Fa-f]{6})\s*;", body)}
        actual = {address for s in profile["sections"]
                  for address in (s["text_table"], s["command_table"], s["voice_table"])}
        missing = len(constants - actual)
        extra = len(actual - constants)
        missing_total += missing
        extra_total += extra
        output.append({"id": profile["id"], "source": source_ref,
                       "source_initializer_address_count": len(constants),
                       "profile_address_count": len(actual),
                       "missing_profile_roots": missing,
                       "profile_roots_not_in_source": extra})
    return {"profile_count": len(selected), "missing_profile_roots": missing_total,
            "profile_roots_not_in_source": extra_total, "profiles": output}


def audit_source_roots(repo_root: Path, profiles: list[dict] | None = None) -> dict:
    """Compare configured profiles with source-initialized linked-table roots."""
    profiles = PROFILES if profiles is None else profiles
    date_source_paths = (repo_root / "src/ovl/DATE/80132000.c",
                         repo_root / "src/ovl/DATE/80141EE0.c")
    date_source = "\n".join(path.read_text(encoding="utf-8") for path in date_source_paths)
    date_profiles = {p["path"].rsplit("_", 1)[-1].split(".", 1)[0]: p
                     for p in profiles if "/DATE_" in p["path"]}
    date_results = []
    for suffix, function in _DATE_FILLERS.items():
        match = re.search(r"void date_fill_sections_" + function
                          + r"\(void\) \{(.*?)\n\}", date_source, re.S)
        if not match:
            raise ValueError("missing DATE section initializer: " + function)
        values = _assignments(match.group(1))
        expected = set()
        directly_consumed = set()
        for index in range(27):
            text_name = "D_8015%04X" % (0xBDC0 + 4 * index)
            command_name = "D_8015%04X" % (0xBE2C + 4 * index)
            voice_name = "D_8015%04X" % (0xBE98 + 4 * index)
            if all(name in values for name in (text_name, command_name, voice_name)):
                triple = (values[text_name], values[command_name], values[voice_name])
                expected.add(triple)
                if (re.search(r"(?:talk_text_table|D_800CA13C)\s*=\s*" + text_name + r"\b", date_source)
                        and re.search(r"(?:talk_command_table|D_800CA140)\s*=\s*" + command_name + r"\b", date_source)
                        and re.search(r"(?:talk_voice_table|D_800CA144)\s*=\s*" + voice_name + r"\b", date_source)):
                    directly_consumed.add(triple)
        profile = date_profiles.get(suffix)
        actual = {(s["text_table"], s["command_table"], s["voice_table"])
                  for s in profile["sections"]} if profile else set()
        date_results.append({
            "path": profile["path"] if profile else "CDROM/SERIFU/DATE_" + suffix + ".BIN",
            "initialized_triad_count": len(expected),
            "directly_consumed_triad_count": len(directly_consumed),
            "profile_triad_count": len(actual),
            "initialized_roots_missing_from_profile": len(expected - actual),
            "profile_roots_not_in_initializer": len(actual - expected),
            "initialized_roots_without_direct_consumer": len(expected - directly_consumed),
        })

    event_init = (repo_root / "src/ovl/DATE/8014B1F0.c").read_text(encoding="utf-8")
    event_values = _assignments(event_init)
    event_expected = set()
    for index in range(79):
        names = ("D_8015%04X" % (0xDE50 + 4 * index),
                 "D_8015%04X" % (0xDF8C + 4 * index),
                 "D_8015%04X" % (0xE0C8 + 4 * index))
        if all(name in event_values for name in names):
            event_expected.add(tuple(event_values[name] for name in names))
    event_profiles = [p for p in profiles if "/EVENT_" in p["path"]]
    event_actual = {(s["text_table"], s["command_table"], s["voice_table"])
                    for p in event_profiles for s in p["sections"]}
    event_index_by_triple = {triple: index for index in range(79)
                             for names in [("D_8015%04X" % (0xDE50 + 4 * index),
                                            "D_8015%04X" % (0xDF8C + 4 * index),
                                            "D_8015%04X" % (0xE0C8 + 4 * index))]
                             if all(name in event_values for name in names)
                             for triple in [tuple(event_values[name] for name in names)]}
    actual_by_suffix = {
        p["path"].rsplit("_", 1)[-1].split(".", 1)[0]: {
            (s["text_table"], s["command_table"], s["voice_table"])
            for s in p["sections"]
        }
        for p in event_profiles
    }
    event_distribution = []
    consumed_missing_correct_profile = 0
    for suffix, indexes in _EVENT_INITIALIZER_INDEXES.items():
        actual = actual_by_suffix.get(suffix, set())
        expected = {triple for triple, index in event_index_by_triple.items()
                    if index in indexes}
        event_distribution.append({
            "path": "CDROM/SERIFU/EVENT_" + suffix + ".BIN",
            "expected_initializer_roots": len(expected),
            "profile_roots": len(actual),
            "missing_expected_roots": len(expected - actual),
            "unexpected_profile_roots": len(actual - expected),
        })
        source_path = repo_root / "src/ovl/DATE" / _EVENT_CONSUMER_SOURCES[suffix]
        source = source_path.read_text(encoding="utf-8")
        names_in_source = re.findall(
            r"D_8015E208\s*=\s*(D_8015\w+);\s*D_8015E20C\s*=\s*(D_8015\w+);"
            r"\s*D_8015E210\s*=\s*(D_8015\w+);", source
        )
        source_roots = {tuple(event_values[name] for name in triple)
                        for triple in names_in_source if all(name in event_values for name in triple)}
        consumed_missing_correct_profile += len(source_roots - actual)
    event_source = "\n".join(path.read_text(encoding="utf-8")
                               for path in (repo_root / "src/ovl/DATE").glob("*.c"))
    consumed_names = re.findall(
        r"D_8015E208\s*=\s*(D_8015\w+);\s*D_8015E20C\s*=\s*(D_8015\w+);"
        r"\s*D_8015E210\s*=\s*(D_8015\w+);", event_source
    )
    consumed = {tuple(event_values[name] for name in triple)
                for triple in consumed_names if all(name in event_values for name in triple)}

    # BUNKA has per-container init functions, sometimes duplicated between the
    # culture-festival variants. Compare every initializer body to profile roots.
    bunka_sources = [repo_root / "src/ovl/BUNKAKEN/80132000.c",
                     repo_root / "src/ovl/BUNKASAI/80132000.c"]
    bunka_init_triples = []
    for path in bunka_sources:
        source = path.read_text(encoding="utf-8")
        for function in re.findall(r"\b(func_801[0-9A-Fa-f]+)\s*\([^;]*?\)\s*\{", source):
            _, body = _function_body(repo_root, str(path.relative_to(repo_root)) + "::" + function)
            values = [int(value, 16) for value in re.findall(
                r"=\s*(0x80[0-9A-Fa-f]{6})\s*;", body)]
            if len(values) == 3:
                bunka_init_triples.append(tuple(values))
    bunka_profiles = [p for p in profiles if "/BUNKA/" in p["path"]]
    bunka_profile_roots = {(s["text_table"], s["command_table"], s["voice_table"])
                           for p in bunka_profiles for s in p["sections"]}
    bunka_unique_source = set(bunka_init_triples)
    bunka_audit = {"initializer_function_count": len(bunka_init_triples),
                   "unique_initializer_triad_count": len(bunka_unique_source),
                   "profile_triad_count": len(bunka_profile_roots),
                   "duplicate_initializer_count": len(bunka_init_triples) - len(bunka_unique_source),
                   "source_roots_missing_from_profiles": len(bunka_unique_source - bunka_profile_roots),
                   "profile_roots_not_in_source": len(bunka_profile_roots - bunka_unique_source)}

    simple_profiles = [p for p in profiles
                       if "/DATE_" not in p["path"] and "/EVENT_" not in p["path"]
                       and "/BUNKA/" not in p["path"]]
    simple_audit = _audit_simple_source_profiles(repo_root, profiles, simple_profiles)
    return {
        "date_fillers": date_results,
        "date_initialized_triad_count": sum(item["initialized_triad_count"] for item in date_results),
        "date_directly_consumed_triad_count": sum(item["directly_consumed_triad_count"] for item in date_results),
        "event_initializer_triad_count": len(event_expected),
        "event_directly_consumed_triad_count": len(consumed),
        "event_profile_triad_count": len(event_actual),
        "event_initializer_roots_missing_from_profiles": len(event_expected - event_actual),
        "event_profile_roots_not_in_initializer": len(event_actual - event_expected),
        "event_consumed_roots_missing_from_profiles": len(consumed - event_actual),
        "event_consumed_roots_missing_from_correct_profile": consumed_missing_correct_profile,
        "event_initializer_roots_without_direct_consumer": len(event_expected - consumed),
        "event_distribution": event_distribution,
        "bunka": bunka_audit,
        "other_and_p_birth_literal_initializers": simple_audit,
    }


def _candidate_strings(data: bytes, offset: int, length: int) -> list[dict]:
    """Locate likely unclaimed Shift-JIS C strings without exposing contents."""
    end = offset + length
    candidates = []
    start = offset
    while start < end:
        nul = data.find(b"\0", start, end)
        if nul < 0:
            break
        raw = data[start:nul]
        # Talk strings are NUL-terminated. Reject controls and require multiple
        # multibyte characters to keep random binary matches less noisy.
        if 4 <= len(raw) <= 4096 and all(byte >= 0x20 and byte != 0x7F for byte in raw):
            try:
                decoded = raw.decode("shift_jis", errors="strict")
            except UnicodeDecodeError:
                decoded = ""
            if sum(ord(char) > 0x7F for char in decoded) >= 2:
                candidates.append({"offset": start, "length": len(raw)})
        start = nul + 1
    return candidates


def audit_profile(data: bytes, profile: dict) -> dict:
    """Summarize parser-reachable strings and unclaimed bytes for one profile."""
    parsed = parse_script(data, profile["path"], profile)
    refs = {
        (record["text"]["offset"], record["text"]["length"])
        for record in parsed["records"] if "text" in record
    }
    candidates = []
    unclaimed_zero = unclaimed_ff = unclaimed_other = 0
    for region in parsed["unclaimed_ranges"]:
        start, size = region["offset"], region["length"]
        raw = data[start:start + size]
        unclaimed_zero += raw.count(0)
        unclaimed_ff += raw.count(0xFF)
        unclaimed_other += sum(byte not in (0, 0xFF) for byte in raw)
        candidates.extend(_candidate_strings(data, start, size))
    return {
        "id": profile["id"],
        "path": profile["path"],
        "sha256": parsed["source"]["sha256"],
        "size": len(data),
        "section_count": len(profile["sections"]),
        "group_count": sum(section["group_count"] for section in profile["sections"]),
        "record_count": len(parsed["records"]),
        "text_record_count": parsed["coverage"]["text_records"],
        "unique_text_target_count": len(refs),
        "unique_text_target_bytes": sum(size for _, size in refs),
        "claimed_bytes": parsed["coverage"]["claimed_bytes"],
        "unclaimed_bytes": len(data) - parsed["coverage"]["claimed_bytes"],
        "unclaimed_zero_bytes": unclaimed_zero,
        "unclaimed_ff_bytes": unclaimed_ff,
        "unclaimed_other_bytes": unclaimed_other,
        "unclaimed_candidate_strings": candidates,
        "candidate_method": "diagnostic_only_strict_shift_jis_at_least_two_multibyte_chars",
    }


def audit_all(data_root: Path, profiles: list[dict] | None = None,
              repo_root: Path | None = None) -> dict:
    """Audit configured profiles against an extracted ``disc/files`` root."""
    profiles = PROFILES if profiles is None else profiles
    results = []
    for profile in profiles:
        data = (data_root / profile["path"]).read_bytes()
        results.append(audit_profile(data, profile))
    return {
        "schema": "tokimemo.script_coverage.v1",
        "scope": "explicit_source_profiled_linked_tables",
        "candidate_strings_are_completeness_proof": False,
        "profile_count": len(results),
        "section_count": sum(item["section_count"] for item in results),
        "record_count": sum(item["record_count"] for item in results),
        "text_record_count": sum(item["text_record_count"] for item in results),
        "unclaimed_candidate_count": sum(len(item["unclaimed_candidate_strings"]) for item in results),
        "profiles": results,
        "source_roots": audit_source_roots(repo_root or data_root.parent.parent, profiles),
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path("disc/files"),
                        help="extracted disc root (default: disc/files)")
    parser.add_argument("--output", type=Path,
                        help="write numeric report JSON (normally .asset-cache/text/)")
    args = parser.parse_args()
    report = audit_all(args.root)
    rendered = json.dumps(report, ensure_ascii=False, indent=2) + "\n"
    if args.output:
        output_root = Path(__file__).resolve().parents[2] / ".asset-cache" / "text"
        target = args.output.resolve()
        if not target.is_relative_to(output_root.resolve()):
            raise SystemExit("coverage reports may only be written under .asset-cache/text/")
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(rendered, encoding="utf-8")
    else:
        print("profiles=%d sections=%d records=%d text_records=%d candidates=%d" % (
            report["profile_count"], report["section_count"], report["record_count"],
            report["text_record_count"], report["unclaimed_candidate_count"],
        ))


if __name__ == "__main__":
    main()
