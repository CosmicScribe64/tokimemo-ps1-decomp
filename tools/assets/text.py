"""Read linked talk tables without executing scripts (T-28001).

The game renderer consumes full-width Shift-JIS pairs with single-byte ASCII
commands. Unicode uses Python's strict shift_jis mapping, never a CP932 fallback.
Every token keeps its source bytes; Unicode is a view, not a rebuild format.
"""
from __future__ import annotations

import hashlib
import struct
from pathlib import PurePosixPath

NAME_CODES = {
    "P": "player_form", "p": "player_family", "m": "player_given",
    "F": "player_full", "G": "girl_name", "Y": "yoshio_name",
    "S": "context_s", "O": "context_o", "W": "context_w",
    "A": "context_a", "M": "context_m",
}
COMMAND_NAMES = (
    "player", "girl", "girl_reply", "choice_three", "player_parenthesis",
    "girl_reply_b", "player_parenthesis_b", "inline", "choice_two", "effect",
    "yoshio", "shiori", "girl_auto", "girl_continue", "player_parenthesis_nowait",
    "girl_clear", "plain_auto", "continue_auto", "girl_continue_auto", "choice_two_parenthesis",
)
VOICE_COMMANDS = frozenset((1, 2, 5, 7, 10, 11, 12, 13, 15, 18))
CONDITIONAL_COMMANDS = frozenset((0, 1, 10, 11, 12, 13))
MAX_LINES = 4096
MAX_STRING = 65536


class TextFormatError(ValueError):
    """The supplied layout does not describe a bounded linked talk script."""


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def source_path(path: str) -> str:
    """Validate a portable relative provenance path, never an output filename."""
    p = PurePosixPath(path)
    if not path or p.is_absolute() or ".." in p.parts or "\\" in path or str(p) != path:
        raise TextFormatError("source path must be a normalized relative POSIX path")
    return path


def _glyph(raw: bytes) -> dict:
    try:
        text = raw.decode("shift_jis", errors="strict")
        if len(text) != 1 or text.encode("shift_jis", errors="strict") != raw:
            raise UnicodeError("non-roundtripping glyph")
    except UnicodeError:
        return {"kind": "unknown", "reason": "invalid_or_nonroundtripping_shift_jis"}
    result = {"kind": "glyph", "text": text}
    try:
        other = raw.decode("cp932", errors="strict")
        if other != text:
            result["cp932_alternative"] = other
    except UnicodeError:
        pass
    return result


def tokenize_text(raw: bytes, command: int) -> dict:
    """Tokenize one non-NUL string using talk_render/talk_speak_part rules.

    Offsets are relative to the original string. Unknown bytes keep the game's
    two-byte stepping; we do not resynchronize into apparent ASCII commands.
    Name substitutions stay symbolic because their values depend on game state.
    """
    if b"\0" in raw:
        raise TextFormatError("tokenizer input must exclude its NUL terminator")
    # This source view includes literal ASCII controls. It is distinct from
    # the game's renderer, which does not accept every byte the codec accepts.
    try:
        source_text = raw.decode("shift_jis", errors="strict")
        if source_text.encode("shift_jis", errors="strict") != raw:
            raise UnicodeError("source bytes do not round-trip")
    except UnicodeError:
        source_text = None
    tokens = []
    end = len(raw)
    conditional = command in CONDITIONAL_COMMANDS and raw.endswith(b"nn")
    if conditional:
        end -= 2
    pos = 0
    inline_label = None
    if command == 7 and raw[:1] in (b">", b"-", b"+", b"<"):
        label_size = {ord(">"): 4, ord("-"): 6, ord("+"): 2, ord("<"): 4}[raw[0]]
        if len(raw) < 1 + label_size:
            raise TextFormatError("truncated inline speaker label")
        label_glyphs = [_glyph(raw[i:i + 2]) for i in range(1, 1 + label_size, 2)]
        inline_label = {"kind": "inline_label", "offset": 0,
                        "raw_hex": raw[:1 + label_size].hex(),
                        "marker": chr(raw[0]), "label_raw_hex": raw[1:1 + label_size].hex(),
                        "label_text": "".join(t["text"] for t in label_glyphs)
                                      if all(t["kind"] == "glyph" for t in label_glyphs) else None,
                        "unknown_label_glyphs": sum(t["kind"] == "unknown" for t in label_glyphs)}
        tokens.append(inline_label)
        pos = 1 + label_size
    variant_start = True
    while pos < end:
        start = pos
        byte = raw[pos]
        char = chr(byte)
        if command in (2, 5) and variant_start and byte == ord("n"):
            # Its reply scanner still steps over n as a two-byte unit. Consuming
            # just n would reinterpret a following trail q as a variant boundary.
            token = {"kind": "reply_skip_marker"}
            pos = min(pos + 2, end)
        elif char in NAME_CODES:
            token = {"kind": "substitution", "code": char, "name": NAME_CODES[char]}
            pos += 1
        elif byte in (ord(" "), ord("q")):
            token = {"kind": "separator", "code": char,
                     "role": "reply_variant" if command in (2, 5) and byte == ord("q") else
                             "choice_option" if command in (3, 8, 19) else "row"}
            pos += 1
        else:
            chunk = raw[pos:min(pos + 2, end)]
            # The glyph system rejects single-byte ASCII and half-width kana.
            token = _glyph(chunk) if len(chunk) == 2 and byte >= 0x81 else {
                "kind": "unknown", "reason": "unsupported_renderer_bytes"}
            pos += len(chunk)
        token.update(offset=start, raw_hex=raw[start:pos].hex())
        tokens.append(token)
        variant_start = token["kind"] == "separator" and token["role"] == "reply_variant"
    if conditional:
        tokens.append({"kind": "conditional_choice_zero", "offset": end, "raw_hex": "6e6e"})
    display = []
    for token in tokens:
        kind = token["kind"]
        if kind == "glyph":
            display.append(token["text"])
        elif kind == "substitution":
            display.append("{" + token["name"] + "}")
        elif kind == "separator":
            display.append("\n")
        elif kind == "unknown":
            display.append("{raw:" + token["raw_hex"] + "}")
    unknown_count = sum(t["kind"] == "unknown" for t in tokens)
    if inline_label is not None:
        unknown_count += inline_label["unknown_label_glyphs"]
    result = {"source_decoded_text": source_text,
              "source_decode_status": "roundtrip_verified" if source_text is not None else "unsupported_encoding_bytes",
              "decoded_text": "".join(display) if not unknown_count else None,
              "annotated_text": "".join(display), "tokens": tokens,
              "unknown_token_count": unknown_count,
              "original_copy_limits": {"speak_buffer": 512, "render_buffer": 512,
                                       "row_buffer": 64, "automatic_word_wrap": False,
                                       "runtime_substitutions_can_expand_rows": True,
                                       "source_exceeds_speak_buffer": len(raw) >= 512}}
    if command in (2, 5):
        result["reply_variant_count"] = 1 + sum(t["kind"] == "separator" and
                                                  t["role"] == "reply_variant" for t in tokens)
    if command in (3, 8, 19):
        result["choice"] = {"option_count": (1 + sum(t["kind"] == "separator" for t in tokens)) if raw else 0,
                            "expected_count": 3 if command == 3 else 2,
                            "order": "runtime_shuffle_unless_choice_255" if command == 3 else "fixed"}
    if inline_label is not None:
        result["speaker_label"] = inline_label
    return result


class _Reader:
    def __init__(self, data: bytes, load_address: int):
        self.data = data
        self.base = load_address
        self.used = bytearray(len(data))

    def read(self, offset: int, size: int) -> bytes:
        if offset < 0 or size < 0 or offset + size > len(self.data):
            raise TextFormatError(f"source extent outside input: offset={offset:#x}, length={size}")
        self.used[offset:offset + size] = b"\x01" * size
        return self.data[offset:offset + size]

    def pointer(self, offset: int) -> tuple[int, int]:
        word = struct.unpack("<I", self.read(offset, 4))[0]
        target = word - self.base
        if not 0 <= target < len(self.data):
            raise TextFormatError(f"pointer outside input at {offset:#x}: {word:#x}")
        return word, target

    def cstring(self, offset: int) -> bytes:
        end = self.data.find(b"\0", offset, min(len(self.data), offset + MAX_STRING))
        if end < 0:
            raise TextFormatError(f"unterminated text at {offset:#x}")
        return self.read(offset, end + 1 - offset)[:-1]

    def unclaimed(self) -> list[dict]:
        ranges = []
        pos = 0
        while pos < len(self.used):
            if self.used[pos]:
                pos += 1
                continue
            start = pos
            while pos < len(self.used) and not self.used[pos]:
                pos += 1
            ranges.append({"offset": start, "length": pos - start,
                           "sha256": sha256(self.data[start:pos]), "status": "uninterpreted"})
        return ranges


def parse_script(data: bytes, path: str, profile: dict) -> dict:
    """Return JSON-compatible records for explicit linked-table profile sections.

    Each section supplies absolute text/command/voice table addresses and an
    explicit group_count. No byte-scanning discovery or guessed pointer bases.
    """
    path = source_path(path)
    digest = sha256(data)
    if profile.get("sha256") and profile["sha256"] != digest:
        raise TextFormatError("input checksum differs from the verified layout")
    base = profile["load_address"]
    if not isinstance(base, int) or not 0 <= base <= 0xFFFFFFFF:
        raise TextFormatError("load_address must be an unsigned 32-bit integer")
    if not isinstance(profile["sections"], list) or not profile["sections"]:
        raise TextFormatError("profile requires at least one section")
    reader = _Reader(data, base)
    records = []
    section_ids = set()
    for section in profile["sections"]:
        section_id = section["id"]
        if not isinstance(section_id, str) or not section_id or section_id in section_ids:
            raise TextFormatError("sections require unique nonempty string IDs")
        section_ids.add(section_id)
        count = section["group_count"]
        if not isinstance(count, int) or not 0 < count <= MAX_LINES:
            raise TextFormatError("group_count must be in 1..4096")
        tables = [section[key] - base for key in ("text_table", "command_table", "voice_table")]
        for table in tables:
            reader.read(table, count * 4)
        for group in range(count):
            group_slots = [table + group * 4 for table in tables]
            group_targets = [reader.pointer(slot)[1] for slot in group_slots]
            text_table, command_table, voice_table = group_targets
            for line in range(MAX_LINES):
                command_offset = command_table + line
                command = reader.read(command_offset, 1)[0]
                record = {
                    "id": f"talk:{path}:{section_id}:g{group:04d}:l{line:04d}",
                    "source_path": path, "source_sha256": digest, "section": section_id,
                    "group": group, "line": line,
                    "group_pointer_offsets": dict(zip(("text", "command", "voice"), group_slots)),
                    "command": {"value": command, "offset": command_offset,
                                "raw_hex": f"{command:02x}",
                                "name": "end" if command == 255 else COMMAND_NAMES[command]
                                        if command < len(COMMAND_NAMES) else "unsupported"},
                }
                if command == 255:
                    records.append(record)
                    break
                text_slot = text_table + line * 4
                text_pointer, text_offset = reader.pointer(text_slot)
                raw = reader.cstring(text_offset)
                record["text"] = {"offset": text_offset, "length": len(raw),
                                  "pointer_offset": text_slot, "pointer_value": text_pointer,
                                  "raw_hex": raw.hex(), "terminator_hex": "00",
                                  **tokenize_text(raw, command)}
                speaker = ("player_family" if command in (0, 4, 6, 14) else
                           "current_girl_short_name" if command in (1, 2, 5, 12, 15) else
                           "yoshio" if command == 10 else "shiori" if command == 11 else
                           "inline_or_effect" if command == 7 else
                           "blank_continuation" if command in (13, 18) else "none")
                record["speaker"] = {"label": speaker, "runtime_values_preserved_symbolically": True}
                if command >= len(COMMAND_NAMES):
                    record["unsupported"] = ["unknown_command_semantics_and_voice_extent"]
                if command in VOICE_COMMANDS:
                    voice_slot = voice_table + line * 4
                    voice_pointer, voice_offset = reader.pointer(voice_slot)
                    variant_count = record["text"].get("reply_variant_count", 1)
                    pairs = []
                    for variant in range(variant_count):
                        off = voice_offset + variant * 8
                        pair = reader.read(off, 8)
                        start, end = struct.unpack("<II", pair)
                        pairs.append({"variant": variant, "offset": off, "raw_hex": pair.hex(),
                                      "clip_start": start, "clip_end": end})
                    record["voice"] = {"pointer_offset": voice_slot, "pointer_value": voice_pointer,
                                       "pairs": pairs, "speaker": "yoshio" if command == 10 else
                                       "shiori" if command == 11 else "runtime_current_girl"}
                records.append(record)
            else:
                raise TextFormatError(f"group {group} has no FF terminator within {MAX_LINES} lines")
    return {"schema": "tokimemo.text.v1", "source": {"path": path, "sha256": digest,
            "size": len(data), "load_address": base}, "profile_id": profile["id"],
            "encoding": {"view": "shift_jis", "errors": "strict_with_raw_unknown_tokens",
                         "cp932_fallback": False, "unicode_is_rebuild_format": False},
            "layout": profile, "records": records, "unclaimed_ranges": reader.unclaimed(),
            "coverage": {"records": len(records), "text_records": sum("text" in r for r in records),
                         "unknown_tokens": sum(r.get("text", {}).get("unknown_token_count", 0) for r in records),
                         "source_decode_failures": sum(r.get("text", {}).get("source_decode_status") == "unsupported_encoding_bytes" for r in records),
                         "unsupported_commands": sum("unsupported" in r for r in records),
                         "claimed_bytes": sum(reader.used), "container_bytes": len(data)}}
