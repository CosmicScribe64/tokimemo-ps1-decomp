"""Build the source-keyed runtime text layer pack from approved English records.

T-39016. Run in Docker. Inputs are read only:

- translations/en/returned-v3.jsonl and translations/en/supplemental.jsonl
  (the English records; ids follow the producers in tools/assets/text_*.py),
- translations/en/graphics/inventory_decisions.json (its `text_approval`
  block must cover exactly the record count read here),
- the original extracted files (`disc/files`), used to bind every container
  path to its SHA-256 and to re-verify every physical record id,
- the original executable font, read through the canonical
  tools/assets/text_font.py reader (load_string/text_glyph_expand evidence).

Physical ids have the form `<container>:<offset:08x>:<sha12>`, where sha12 is
the first 12 hex digits of SHA-256 over the source bytes from the offset
through the first NUL, the NUL-terminated scheme of tools/assets/text_binary.py.
The builder re-reads those bytes from the original container and rejects any
mismatch as `source_bytes_mismatch`. That includes ids that
tools/assets/text_source.py derived from an assembler extent without a NUL or
from a NUL-appended alias: they do not hash the same way and are rejected,
not matched. Every other id is a logical record id; it is bound to its
container hash when the id names an original container, and is otherwise keyed
by id alone.

English characters become the original font's full-width Latin and
punctuation code units. A record is representable only when every character
maps to a code unit that the original font resolves to a non-blank bitmap
(the full-width space 0x8140 is the one blank glyph accepted). Protected
markers are restored to their recorded source bytes. Each record is checked
in this order, and the first failing check names its rejection reason:

- no English text (`no_translation`), an ASCII/format literal
  (`ascii_format_literal_not_glyph_text`), or a review status that says the
  renderer's tokenization of the source is unknown
  (`renderer_tokenization_unavailable`), unless RENDERER_PROVEN cites the
  original renderer for that exact id (the remaining checks still apply);
- for physical ids, the container and source bytes (`container_missing`,
  `source_extent_outside_container`, `source_bytes_mismatch`);
- markers without source bytes or that disagree with the record's tokens, a
  character without an original glyph, or markers only (`segments_for`);
- a printf directive such as `%s` outside a protected marker
  (`format_directive_untokenized`): the glyph run would draw it literally;
- for physical ids, the source walked as Shift-JIS units: every single-byte
  unit must lie inside a protected token's recorded bytes
  (`source_single_byte_control_untokenized`), and every token's bytes must
  occur in the source (`protected_token_not_in_source`). The original
  renderer gives single bytes their own meaning (sndisp and sndi,
  src/main/menu_speaker_girl.c: ' ' breaks the line, 'P' and 'G' expand
  names), so a glyph run cannot replace them.

Three mappings are deliberate substitutions, because JIS X 0208 has no
straight quotes and no en dash (CP932 places the full-width straight quotes
only in the NEC extension, 0xFA56/0xFA57): ASCII '"' becomes the right double quote
0x8168 (opening and closing alike), ASCII "'" becomes the right single quote
0x8166, and the en dash becomes the minus sign 0x817C, which is also CP932's
full-width hyphen-minus. `coverage.json` counts each substitution in the
representable records. Nothing else is substituted or guessed.

The output directory must not exist. It receives `text_layer.json` (the
pack) and `coverage.json` (per-reason counts, no game text).
"""
from __future__ import annotations

import argparse
import collections
import hashlib
import json
from pathlib import Path
import re
import sys


def _repository_root() -> Path:
    for parent in Path(__file__).resolve().parents:
        if (parent / "tools" / "assets" / "text_font.py").is_file():
            return parent
    raise RuntimeError("cannot locate canonical tools/assets/text_font.py")


ROOT = _repository_root()
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.assets.text_font import FONT_PROFILE, font_metadata, font_reference  # noqa: E402

SCHEMA = "tokimemo.text-layer-pack.v1"
COVERAGE_SCHEMA = "tokimemo.text-layer-coverage.v1"
EXECUTABLE = "SLPM_86.053"
MARKER = re.compile(r"\[\[T[^\[\]\r\n]*\]\]")
CONTAINER = re.compile(r"^(SLPM_86\.053|CDROM/[A-Z0-9_]+(?:/[A-Z0-9_]+)*\.[A-Z0-9]+)$")
PHYSICAL = re.compile(r"^(SLPM_86\.053|CDROM/[A-Z0-9_/]+\.[A-Z0-9]+):([0-9a-f]{8}):([0-9a-f]{12})$")
ASCII_LITERAL_NOTE = "ASCII / format literal preserved."
TOKENIZATION_UNAVAILABLE = "renderer_tokenization_unavailable"
# Records whose review status lacks a renderer view, but whose renderer is
# proven from the decompiled source. Each value is the citation.
RENDERER_PROVEN = {
    # D_800AFB74, the full-width "(" that sndi prepends to row 0 for speaker
    # style 0x1F: strcpy(decorated_text, "（") in sndi,
    # src/main/menu_speaker_girl.c. Drawn by the command HUD narration
    # (tools/port/hud_content.py, presentation/command_hud_presenter.gd).
    "SLPM_86.053:0006f374:b8bad341dc9f": "src/main/menu_speaker_girl.c sndi, speaker style 0x1F row-0 decoration",
}
PRINTF = re.compile(r"%[-0-9.]*[sdxuc]")
# Characters drawn with a glyph that is not their own (see the module docstring).
SUBSTITUTIONS = {"\"": "U+201D", "'": "U+2019", "\u2013": "U+2212"}
SPACE = 0x8140
# Full-width code units of the original font for printable ASCII and the
# typographic marks that appear in the English records. Every entry is
# re-checked against the original font bitmaps before it is used.
PUNCTUATION = {
    " ": 0x8140, "!": 0x8149, "\"": 0x8168, "#": 0x8194, "$": 0x8190, "%": 0x8193,
    "&": 0x8195, "'": 0x8166, "(": 0x8169, ")": 0x816A, "*": 0x8196, "+": 0x817B,
    ",": 0x8143, "-": 0x817C, ".": 0x8144, "/": 0x815E, ":": 0x8146, ";": 0x8147,
    "<": 0x8183, "=": 0x8181, ">": 0x8184, "?": 0x8148, "@": 0x8197, "[": 0x816D,
    "\\": 0x815F, "]": 0x816E, "^": 0x814F, "_": 0x8151, "`": 0x814D, "{": 0x816F,
    "|": 0x8162, "}": 0x8170, "~": 0x8160, "‘": 0x8165, "’": 0x8166,
    "“": 0x8167, "”": 0x8168, "–": 0x817C, "—": 0x815C,
    "…": 0x8163,
}


class PackError(ValueError):
    """An input breaks the approval, source or font contract."""


def _sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def code_for(character: str) -> int | None:
    """Return the original full-width code unit for one English character."""
    value = ord(character)
    if 0x41 <= value <= 0x5A:
        return 0x8260 + value - 0x41
    if 0x61 <= value <= 0x7A:
        return 0x8281 + value - 0x61
    if 0x30 <= value <= 0x39:
        return 0x824F + value - 0x30
    return PUNCTUATION.get(character)


class FontCheck:
    """Original-font support for code units, through the canonical reader."""

    def __init__(self, executable: bytes):
        self.metadata = font_metadata(executable, FONT_PROFILE)
        start = self.metadata["glyph_offset"]
        self.bank = executable[start:start + self.metadata["glyph_count"] * 18]
        self._cache: dict[int, bool] = {}

    def supported(self, code: int) -> bool:
        if code not in self._cache:
            found = font_reference(code.to_bytes(2, "big"), self.metadata)
            if found["status"] != "glyph":
                self._cache[code] = False
            else:
                index = found["glyph_index"]
                blank = not any(self.bank[index * 18:index * 18 + 18])
                self._cache[code] = code == SPACE or not blank
        return self._cache[code]


def _read_jsonl(name: str, data: bytes) -> list[dict]:
    rows = []
    # Split on LF only: str.splitlines would also split on U+2028 inside text.
    for number, line in enumerate(data.decode("utf-8").split("\n"), 1):
        if not line.strip():
            continue
        value = json.loads(line)
        if not isinstance(value, dict) or not isinstance(value.get("id"), str):
            raise PackError(f"{name}:{number}: record without id")
        rows.append(value)
    return rows


def approval(decisions: dict, total: int) -> dict:
    """Return the text approval block when it covers exactly `total` records."""
    block = decisions.get("text_approval")
    if not isinstance(block, dict):
        raise PackError("inventory decisions have no text_approval block")
    scope = block.get("scope")
    for key in ("approver", "date", "instruction", "scope", "source"):
        if not isinstance(block.get(key), str) or not block[key]:
            raise PackError("text approval lacks " + key)
    if "returned-v3.jsonl" not in scope or "supplemental.jsonl" not in scope:
        raise PackError("text approval scope does not name both record files")
    counted = re.search(r"\(([\d,]+) records", scope)
    if counted is None or int(counted[1].replace(",", "")) != total:
        raise PackError(f"text approval covers a different record count than the {total} read")
    return {key: block[key] for key in ("approver", "date", "instruction", "scope", "source")}


def container_of(record_id: str) -> str | None:
    """Return the first id field naming an original container, if any."""
    for field in record_id.split(":"):
        if CONTAINER.match(field):
            return field
    return None


def segments_for(text: str, tokens: list[dict] | None, font: FontCheck):
    """Split English text into glyph runs and restored source token bytes.

    Returns (segments, reason, unsupported_characters)."""
    markers = MARKER.findall(text)
    by_marker: dict[str, dict] = {}
    if markers:
        if tokens is None:
            return None, "protected_token_bytes_unavailable", []
        for token in tokens:
            marker = token.get("marker")
            raw = token.get("raw_hex")
            if not isinstance(marker, str) or not isinstance(raw, str) or not raw or len(raw) % 2:
                return None, "protected_token_bytes_unavailable", []
            by_marker[marker] = token
        if sorted(markers) != sorted(by_marker) or len(set(markers)) != len(markers):
            return None, "protected_token_mismatch", []
    elif tokens:
        return None, "protected_token_mismatch", []
    segments: list[list[str]] = []
    unsupported: list[str] = []
    cursor = 0
    for match in list(MARKER.finditer(text)) + [None]:
        piece = text[cursor:match.start() if match else len(text)]
        codes = bytearray()
        for character in piece:
            code = code_for(character)
            if code is None or not font.supported(code):
                unsupported.append(character)
                continue
            codes += code.to_bytes(2, "big")
        if codes:
            segments.append(["g", codes.hex()])
        if match:
            token = by_marker[match.group()]
            segments.append(["t", token["raw_hex"].lower(), str(token.get("kind", ""))])
            cursor = match.end()
    if unsupported:
        return None, "unsupported_character", unsupported
    if not any(kind == "g" for kind, *_ in segments):
        return None, "no_glyph_text", []
    return segments, None, []


def verify_physical(record_id: str, files: Path, cache: dict) -> dict:
    """Re-read a physical id's bytes from the original container."""
    match = PHYSICAL.match(record_id)
    path, offset, prefix = match[1], int(match[2], 16), match[3]
    if path not in cache:
        target = files / path
        cache[path] = target.read_bytes() if target.is_file() else None
    data = cache[path]
    if data is None:
        return {"error": "container_missing"}
    end = data.find(b"\0", offset)
    if offset >= len(data) or end < 0:
        return {"error": "source_extent_outside_container"}
    raw = data[offset:end + 1]
    digest = _sha256(raw)
    if digest[:12] != prefix:
        return {"error": "source_bytes_mismatch"}
    return {"container": path, "offset": offset, "raw_sha256": digest, "raw_length": len(raw), "raw": raw}


def source_control_reason(raw: bytes, tokens: list[dict]) -> str | None:
    """Check a physical source's single-byte units against the record's tokens.

    `raw` is walked as Shift-JIS units up to its first NUL: a byte in
    0x81-0x9F or 0xE0-0xFC starts a two-byte unit, any other byte is a
    single-byte unit. Each token's recorded bytes cover one occurrence that
    starts at a unit boundary; when the token records its source `offset`,
    only that position counts. Returns a rejection reason or None."""
    remaining = []
    for token in tokens:
        offset = token.get("offset")
        try:
            value = bytes.fromhex(token["raw_hex"])
        except (KeyError, TypeError, ValueError):
            return "protected_token_bytes_unavailable"
        if not value or (offset is not None and (not isinstance(offset, int) or isinstance(offset, bool))):
            return "protected_token_bytes_unavailable"
        remaining.append((offset, value))
    index = 0
    while index < len(raw) and raw[index] != 0:
        match = max((item for item in remaining if item[0] in (None, index) and raw.startswith(item[1], index)),
                    key=lambda item: (item[0] is not None, len(item[1])), default=None)
        if match is not None:
            remaining.remove(match)
            index += len(match[1])
            continue
        lead = raw[index]
        if (0x81 <= lead <= 0x9F or 0xE0 <= lead <= 0xFC) and index + 1 < len(raw) and raw[index + 1] != 0:
            index += 2
            continue
        return "source_single_byte_control_untokenized"
    return "protected_token_not_in_source" if remaining else None


def build(returned: Path, supplemental: Path, decisions_path: Path, files: Path,
          font: FontCheck | None = None) -> tuple[dict, dict]:
    """Build the pack; `font` is a test seam, the CLI always reads the original."""
    executable_path = files / EXECUTABLE
    if not executable_path.is_file():
        raise PackError("original executable is missing from the files root")
    executable = executable_path.read_bytes()
    if font is None:
        if _sha256(executable) != FONT_PROFILE["sha256"]:
            raise PackError("executable differs from the verified original")
        font = FontCheck(executable)
    # Each input is read once, so its recorded hash is the parsed content.
    inputs = {key: path.read_bytes() for key, path in
              (("returned", returned), ("supplemental", supplemental), ("decisions", decisions_path))}
    main_rows = _read_jsonl(returned.name, inputs["returned"])
    extra_rows = _read_jsonl(supplemental.name, inputs["supplemental"])
    decisions = json.loads(inputs["decisions"].decode("utf-8"))
    approved = approval(decisions, len(main_rows) + len(extra_rows))
    cache: dict[str, bytes | None] = {EXECUTABLE: executable}
    containers: dict[str, dict] = {}
    entries: dict[str, dict] = {}
    reasons: collections.Counter = collections.Counter()
    by_family: dict[str, collections.Counter] = collections.defaultdict(collections.Counter)
    characters: collections.Counter = collections.Counter()
    substituted: collections.Counter = collections.Counter()
    seen: set[str] = set()
    rows = [(row, "returned") for row in main_rows] + [(row, "supplemental") for row in extra_rows]
    for row, origin in rows:
        record_id = row["id"]
        if record_id in seen:
            raise PackError("duplicate record id " + record_id)
        seen.add(record_id)
        family = "physical" if PHYSICAL.match(record_id) else record_id.split(":", 1)[0]
        text = row.get("target_text") if origin == "returned" else row.get("translation")
        tokens = row.get("protected_tokens")
        if origin == "supplemental":
            # Supplemental markers carry kinds only, never source bytes.
            tokens = None if MARKER.search(text or "") else []
        reason = None
        location: dict = {}
        if not isinstance(text, str) or not text:
            reason = "no_translation"
        elif row.get("translator_note") == ASCII_LITERAL_NOTE:
            reason = "ascii_format_literal_not_glyph_text"
        elif TOKENIZATION_UNAVAILABLE in str(row.get("review_status", "")) and record_id not in RENDERER_PROVEN:
            reason = TOKENIZATION_UNAVAILABLE
        elif family == "physical":
            location = verify_physical(record_id, files, cache)
            reason = location.pop("error", None)
        else:
            path = container_of(record_id)
            if path is not None:
                if path not in cache:
                    target = files / path
                    cache[path] = target.read_bytes() if target.is_file() else None
                if cache[path] is None:
                    reason = "container_missing"
                else:
                    location = {"container": path}
        segments = None
        if reason is None:
            segments, reason, unsupported = segments_for(text, tokens, font)
            characters.update(unsupported)
        if reason is None and PRINTF.search(MARKER.sub("\0", text)):
            reason = "format_directive_untokenized"
        if reason is None and "raw" in location:
            reason = source_control_reason(location["raw"], tokens or [])
        if reason is not None:
            reasons[reason] += 1
            by_family[family][reason] += 1
            continue
        by_family[family]["representable"] += 1
        location.pop("raw", None)
        substituted.update(character for character in MARKER.sub("", text) if character in SUBSTITUTIONS)
        path = location.get("container")
        if path is not None and path not in containers:
            containers[path] = {"sha256": _sha256(cache[path]), "size": len(cache[path])}
        entry = {"text": text, "segments": segments, "context_status": str(row.get("review_status", ""))}
        entry.update(location)
        entries[record_id] = entry
    total = len(rows)
    pack = {
        "schema": SCHEMA, "language": "en",
        "approval": approved,
        "sources": {key + "_sha256": _sha256(data) for key, data in inputs.items()},
        "font": {"source_path": EXECUTABLE, "source_sha256": FONT_PROFILE["sha256"],
                 "glyph_bank_sha256": font.metadata["glyph_bank_sha256"]},
        "containers": dict(sorted(containers.items())),
        "entries": dict(sorted(entries.items())),
    }
    coverage = {
        "schema": COVERAGE_SCHEMA, "records": total, "representable": len(entries),
        "rejected": dict(sorted(reasons.items())),
        "by_family": {key: dict(sorted(value.items())) for key, value in sorted(by_family.items())},
        "unsupported_characters": {f"U+{ord(key):04X}": value for key, value in characters.most_common()},
        "substituted_characters": {f"U+{ord(key):04X}->{SUBSTITUTIONS[key]}": value
                                   for key, value in sorted(substituted.items())},
        "renderer_proven_by_citation": {key: value for key, value in sorted(RENDERER_PROVEN.items()) if key in entries},
        "physical_verified": sum(1 for entry in entries.values() if "offset" in entry),
        "container_bound": sum(1 for entry in entries.values() if "container" in entry),
        "not_covered": ["consumer draw-site context per record", "layout fit per screen"],
    }
    return pack, coverage


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--returned", type=Path, required=True)
    parser.add_argument("--supplemental", type=Path, required=True)
    parser.add_argument("--decisions", type=Path, required=True)
    parser.add_argument("--files", type=Path, required=True, help="original extracted disc files root")
    parser.add_argument("--output", type=Path, required=True, help="new directory")
    args = parser.parse_args(argv)
    if args.output.exists():
        print("output already exists; choose a new directory", file=sys.stderr)
        return 2
    try:
        pack, coverage = build(args.returned, args.supplemental, args.decisions, args.files)
    except (PackError, OSError, json.JSONDecodeError) as failure:
        print(f"text layer build failed: {failure}", file=sys.stderr)
        return 1
    args.output.mkdir(parents=True)
    pack_path = args.output / "text_layer.json"
    pack_path.write_text(json.dumps(pack, ensure_ascii=False, sort_keys=True, separators=(",", ":")) + "\n", encoding="utf-8")
    (args.output / "coverage.json").write_text(json.dumps(coverage, indent=1, sort_keys=True) + "\n", encoding="utf-8")
    print(f"records={coverage['records']} representable={coverage['representable']} "
          f"physical_verified={coverage['physical_verified']} pack_sha256={_sha256(pack_path.read_bytes())}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
