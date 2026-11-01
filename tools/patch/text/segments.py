"""Text records as structured segments, and the pluggable string encoders (T-39503).

A record's payload (its bytes without the terminator) is a list of segments:

* ``Text(text)``: translatable characters.
* ``Control(kind, name, raw)``: a control code kept byte-for-byte (row break,
  choice separator, name substitution, inline-label marker, format directive,
  storage byte...). Its ``kind`` and ``name`` are the ones of the translation
  markers ``[[Tnnn:name]]`` (``translations/en/TAGS.md``).

The split is the one the translator handoff made (``tools.assets.text_handoff.template``),
so the markers of ``translations/en/returned-v3.jsonl`` resolve to the same bytes.

An encoder turns segments into bytes. ``ShiftJisEncoder`` is the original encoding:
it rebuilds every original payload exactly. ``ProvisionalAsciiEncoder`` is the size
model for English (one byte per ASCII character, control codes unchanged) until the
runtime encoding of T-39502 exists; any object with ``name``, ``encode_text(str)``
and ``encode_control(Control)`` can take its place.
"""
from __future__ import annotations

import json
import re
from dataclasses import dataclass

from tools.assets.text_handoff import template

MARKER = re.compile(r"\[\[T([0-9]{3}):([A-Za-z0-9_-]+)\]\]")


class SegmentError(ValueError):
    """A record or a translation cannot be split into text and control segments."""


@dataclass(frozen=True)
class Text:
    text: str


@dataclass(frozen=True)
class Control:
    kind: str
    name: str
    raw: bytes


def _split(copy_template: str, references: dict[str, bytes], kinds: dict[str, str]) -> list:
    segments, cursor, used = [], 0, []
    for match in MARKER.finditer(copy_template):
        if match.start() > cursor:
            segments.append(Text(copy_template[cursor:match.start()]))
        marker = match.group(0)
        if marker not in references:
            raise SegmentError(f"marker {marker} has no source bytes")
        segments.append(Control(kinds[marker], match.group(2), references[marker]))
        used.append(marker)
        cursor = match.end()
    if cursor < len(copy_template):
        segments.append(Text(copy_template[cursor:]))
    return segments, used


def source_segments(unit: dict) -> list:
    """Split a unit's original payload (terminator excluded) into segments.

    A unit whose bytes the handoff could not project (undecodable or empty slots)
    becomes one opaque control, so it is still rebuilt exactly but cannot be translated.
    """
    raw = bytes.fromhex(unit.get("raw_hex") or "")
    copy_template, references, blocker = template(unit)
    if blocker is not None:
        payload = raw[:-1] if raw.endswith(b"\0") and unit.get("kind") == "source_extent" else raw
        return [Control("opaque", "source_bytes", payload)] if payload else []
    refs = {ref["marker"]: bytes.fromhex(ref["raw_hex"]) for ref in references}
    kinds = {ref["marker"]: ref["kind"] for ref in references}
    segments, _ = _split(copy_template, refs, kinds)
    return segments


def protected_references(tokens) -> tuple[dict[str, bytes], dict[str, str]]:
    """Marker -> raw bytes and kind from a translation record's ``protected_tokens``."""
    if isinstance(tokens, str):
        tokens = json.loads(tokens)
    refs, kinds = {}, {}
    for token in tokens:
        if "raw_hex" not in token:
            raise SegmentError(f"protected token {token.get('marker')} carries no source bytes")
        refs[token["marker"]] = bytes.fromhex(token["raw_hex"])
        kinds[token["marker"]] = token["kind"]
    return refs, kinds


def target_segments(target_text: str, references: dict[str, bytes], kinds: dict[str, str],
                    *, strict: bool = True) -> list:
    """Split a translation into segments; every source marker must appear exactly once."""
    segments, used = _split(target_text, references, kinds)
    if strict:
        if sorted(used) != sorted(references):
            missing = sorted(set(references) - set(used))
            extra = sorted(marker for marker in set(used) if used.count(marker) > 1)
            raise SegmentError(f"markers differ from the source: missing {missing}, repeated {extra}")
    return segments


def references_from_segments(segments: list, copy_markers: list[str]) -> tuple[dict, dict]:
    """Rebuild marker references from source segments when a record stores markers only.

    ``copy_markers`` are the markers in source order (``[[T001:...]]`` numbering).
    """
    controls = [segment for segment in segments if isinstance(segment, Control)]
    if len(controls) != len(copy_markers):
        raise SegmentError("source controls and markers differ in number")
    refs = {marker: control.raw for marker, control in zip(copy_markers, controls)}
    kinds = {marker: control.kind for marker, control in zip(copy_markers, controls)}
    return refs, kinds


class ShiftJisEncoder:
    """The original encoding: Shift-JIS text, control codes byte-for-byte."""

    name = "shift_jis"

    def encode_text(self, text: str) -> bytes:
        return text.encode("shift_jis", errors="strict")

    def encode_control(self, control: Control) -> bytes:
        return control.raw


# Typographic characters translators used, folded to ASCII for the provisional size model.
ASCII_FOLDS = {
    "‘": "'", "’": "'", "“": '"', "”": '"', "…": "...",
    "–": "-", "—": "--", " ": " ", "é": "e", "è": "e",
    "à": "a", "ü": "u", "ö": "o", "ä": "a", "ï": "i",
    "ê": "e", "ç": "c", "ô": "o", "â": "a", "•": "*",
    "×": "x", "→": "->", "←": "<-", "♥": "<3", "♪": "~",
    "〜": "~", "～": "~", "　": " ",
}


class ProvisionalAsciiEncoder:
    """Size model for English: one byte per ASCII character, controls unchanged.

    Characters outside 0x20-0x7E are folded through ``ASCII_FOLDS``; anything left
    is written as ``?`` and counted in ``unmapped`` so the report can list it.
    This is not the runtime encoding: in talk scripts 0x20 and the letters
    ``q n P p m F G Y S O W A M`` are control codes, so T-39502 must move either
    the controls or the letters before English can be displayed.
    """

    name = "provisional_ascii"

    def __init__(self):
        self.unmapped: dict[str, int] = {}

    def encode_text(self, text: str) -> bytes:
        out = bytearray()
        for char in text:
            if " " <= char <= "~":
                out.append(ord(char))
                continue
            folded = ASCII_FOLDS.get(char)
            if folded is None:
                self.unmapped[char] = self.unmapped.get(char, 0) + 1
                folded = "?"
            out.extend(folded.encode("ascii"))
        return bytes(out)

    def encode_control(self, control: Control) -> bytes:
        return control.raw


def encode(segments: list, encoder) -> bytes:
    """Encode a payload (no terminator) with any encoder."""
    out = bytearray()
    for segment in segments:
        if isinstance(segment, Text):
            out.extend(encoder.encode_text(segment.text))
        elif isinstance(segment, Control):
            out.extend(encoder.encode_control(segment))
        else:
            raise SegmentError(f"not a segment: {segment!r}")
    return bytes(out)
