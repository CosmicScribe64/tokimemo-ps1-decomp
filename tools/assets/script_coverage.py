"""Numeric coverage audit for source-profiled linked script containers.

This module summarizes bytes claimed by explicit talk-table profiles and
reports heuristic Shift-JIS-looking strings in unclaimed ranges as leads only.
The heuristic is not a completeness proof; only source-backed table roots and
their validated links are treated as dialogue records. No text bytes or decoded
strings are emitted.
"""
from __future__ import annotations

import argparse
import hashlib
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

_C_NONCODE = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S)
_DATE_ROOT_GLOBALS = {
    base + 4 * index
    for bases, count in [((0x8015BDC0, 0x8015BE2C, 0x8015BE98), 27),
                         ((0x8015DE50, 0x8015DF8C, 0x8015E0C8), 79)]
    for base in bases for index in range(count)
} | {0x8015E208, 0x8015E20C, 0x8015E210}


def _verify_profile_source_pins(repo_root: Path, profiles: list[dict]) -> None:
    """Enforce reviewed hardcoded pins independently of snapshot self-lists."""
    expected = {}
    for profile in profiles:
        for relative, digest in profile.get('source_pins', {}).items():
            if relative in expected and expected[relative] != digest:
                raise ValueError('conflicting profile source pins')
            expected[relative] = digest
    root = repo_root.resolve(strict=True)
    for relative, digest in expected.items():
        path = Path(relative)
        if path.is_absolute() or '..' in path.parts:
            raise ValueError('profile source pin path escapes repository')
        try:
            target = (root / path).resolve(strict=True)
        except OSError as exc:
            raise ValueError('profile source pin file missing') from exc
        if not target.is_relative_to(root) or not target.is_file():
            raise ValueError('profile source pin path escapes repository')
        if hashlib.sha256(target.read_bytes()).hexdigest() != digest:
            raise ValueError('profile source pin mismatch')


def _current_source_path(repo_root: Path, relative: str) -> Path:
    """Resolve an old object identity only through the explicit rename ledger."""
    ledger = repo_root / 'config/renames/overlay-files.txt'
    if not ledger.is_file():
        return repo_root / relative
    parts = relative.split('/')
    if len(parts) != 4 or parts[:2] != ['src', 'ovl']:
        return repo_root / relative
    unit, filename = parts[2], parts[3]
    directory = unit
    for line in (repo_root / 'config/renames/t27074-paths.txt').read_text().splitlines():
        fields = line.split()
        if fields[:2] == ['dir', unit] and fields[2] == directory:
            directory = fields[3]
    stem = filename.removesuffix('.c')
    for line in ledger.read_text().splitlines():
        fields = line.split()
        if fields[:3] == ['file', unit, stem]:
            stem = fields[3]
    return repo_root / 'src/ovl' / directory / (stem + '.c')


def _read_source(repo_root: Path, relative: str) -> tuple[str, str]:
    path = _current_source_path(repo_root, relative)
    actual = path.relative_to(repo_root).as_posix()
    raw = path.read_bytes()
    manifest = repo_root / 'snapshot-manifest.json'
    if manifest.is_file():
        pins = json.loads(manifest.read_text())['files']
        if pins.get(actual) != hashlib.sha256(raw).hexdigest():
            raise ValueError('source-root audit current source pin mismatch')
    source = raw.decode('utf-8', errors='strict')
    source = _C_NONCODE.sub(lambda m: ''.join('\n' if c == '\n' else ' ' for c in m[0]), source)
    # Exact configured data-symbol addresses preserve the audit's address-keyed
    # assignment/consumer checks. Function names and constants are untouched.
    if relative.startswith('src/ovl/'):
        unit = relative.split('/')[2]
        symbols = repo_root / ('config/symbols_' + unit + '.txt')
        if symbols.is_file():
            mapping = {}
            for line in symbols.read_text().splitlines():
                match = re.fullmatch(r'(\w+)\s*=\s*0x([0-9A-Fa-f]+);(.*)', line)
                if (unit == 'DATE' and match and 'type:func' not in match[3]
                        and int(match[2], 16) in _DATE_ROOT_GLOBALS):
                    mapping[match[1]] = 'D_%08X' % int(match[2], 16)
            shared_symbols = repo_root / 'config/symbol_addrs.txt'
            if shared_symbols.is_file():
                for line in shared_symbols.read_text().splitlines():
                    match = re.fullmatch(r'(\w+)\s*=\s*0x([0-9A-Fa-f]+);(.*)', line)
                    if match and int(match[2], 16) in (0x800CA13C, 0x800CA140, 0x800CA144):
                        mapping[match[1]] = 'D_%08X' % int(match[2], 16)
            source = re.sub(r'\b[A-Za-z_]\w*\b', lambda m: mapping.get(m[0], m[0]), source)
    return actual, source


def _source_functions(repo_root: Path, relative: str) -> list[str]:
    """Use exact current ASM glabels belonging to this source object."""
    path = _current_source_path(repo_root, relative)
    unit = relative.split('/')[2]
    paths = list((repo_root / 'asm/ovl' / unit / 'matchings').glob('*/' + path.stem + '/*.s'))
    if paths:
        names = []
        for assembly in paths:
            label = re.search(r'^glabel\s+(\w+)\s*$', assembly.read_text(), re.M)
            if label:
                names.append(label[1])
        return names
    _, source = _read_source(repo_root, relative)
    return re.findall(r'\b(func_801[0-9A-Fa-f]+)\s*\([^;]*?\)\s*\{', source)


def _assignments(source: str) -> dict[str, int]:
    return {name: int(value, 16) for name, value in re.findall(
        r"(D_8015\w+)\s*=\s*(0x[0-9A-Fa-f]+);", source
    )}


def _function_body(repo_root: Path, evidence: str) -> tuple[str, str]:
    """Read a cited C initializer function and return (path, balanced body)."""
    source_ref, function = evidence.split("::", 1)
    original_ref = source_ref
    source_ref, source = _read_source(repo_root, source_ref)
    old = re.fullmatch(r'func_([0-9A-Fa-f]{8})', function)
    if old and not re.search(r'\b' + re.escape(function) + r'\s*\(', source):
        path = _current_source_path(repo_root, original_ref)
        unit = original_ref.split('/')[2]
        matches = []
        for assembly in (repo_root / 'asm/ovl' / unit / 'matchings').glob('*/' + path.stem + '/*.s'):
            text = assembly.read_text()
            label = re.search(r'^glabel\s+(\w+)\s*$', text, re.M)
            word = re.search(r'/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s*\*/', text)
            if label and word and int(word[1], 16) == int(old[1], 16):
                matches.append(label[1])
        if len(matches) != 1:
            raise ValueError('initializer does not have one exact current ASM entry')
        function = matches[0]
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
                if "::" in ref]
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
    _verify_profile_source_pins(repo_root, profiles)
    date_source_paths = (repo_root / "src/ovl/DATE/80132000.c",
                         repo_root / "src/ovl/DATE/80141EE0.c")
    date_source = "\n".join(_read_source(repo_root, str(path.relative_to(repo_root)))[1]
                            for path in date_source_paths)
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

    _, event_init = _read_source(repo_root, "src/ovl/DATE/8014B1F0.c")
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
        _, source = _read_source(repo_root, str(source_path.relative_to(repo_root)))
        names_in_source = re.findall(
            r"D_8015E208\s*=\s*(D_8015\w+);\s*D_8015E20C\s*=\s*(D_8015\w+);"
            r"\s*D_8015E210\s*=\s*(D_8015\w+);", source
        )
        source_roots = {tuple(event_values[name] for name in triple)
                        for triple in names_in_source if all(name in event_values for name in triple)}
        consumed_missing_correct_profile += len(source_roots - actual)
    event_directory = _current_source_path(repo_root, 'src/ovl/DATE/80132000.c').parent
    event_source = "\n".join(_read_source(repo_root, 'src/ovl/DATE/' + path.name)[1]
                               for path in event_directory.glob("*.c"))
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
        for function in _source_functions(repo_root, str(path.relative_to(repo_root))):
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
                       and "/BUNKA/" not in p["path"] and "/MASTER/" not in p["path"]]
    simple_audit = _audit_simple_source_profiles(repo_root, profiles, simple_profiles)
    master_audit = _audit_simple_source_profiles(
        repo_root, profiles, [p for p in profiles if "/MASTER/" in p["path"]])
    return {
        "master_literal_initializers": master_audit,
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
