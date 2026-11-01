"""Turn the English records into target segments for the writers (T-39503).

``returned-v3.jsonl`` rows carry ``target_text`` and ``protected_tokens`` with the raw
bytes of every marker. ``supplemental.jsonl`` rows carry ``translation`` and markers
only; their bytes come from the source string, in marker order.
"""
from __future__ import annotations

import json
from pathlib import Path

from .segments import (MARKER, Control, SegmentError, Text, protected_references, references_from_segments,
                       target_segments)


def load_jsonl(path: str | Path) -> list[dict]:
    with open(path, encoding="utf-8") as stream:
        return [json.loads(line) for line in stream]


def _drop_wrapped_source(segments: list) -> tuple[list, str | None]:
    """Remove Japanese that the handoff wrapped whole as an ``unknown_renderer_bytes`` marker.

    The fixed-slot tables of the overlays (``overlay-table:`` ids) have no glyph view, so
    their whole Japanese text became one protected marker and translators kept it next to
    the English. Writing it would put the Japanese back; it is text, not a control.
    """
    kept = [s for s in segments if not (isinstance(s, Control) and s.kind == "unknown_renderer_bytes"
                                        and _is_sjis_text(s.raw))]
    if len(kept) == len(segments) or not any(isinstance(s, Text) and s.text for s in kept):
        return segments, None
    return kept, "wrapped_source_text_dropped"


def _is_sjis_text(raw: bytes) -> bool:
    try:
        return raw.decode("shift_jis", errors="strict").encode("shift_jis") == raw and raw[:1] >= b"\x81"
    except UnicodeError:
        return False


def target_of(row: dict, source_segments: list) -> tuple[list | None, str | None]:
    """(segments, problem). ``None`` segments mean: keep the original payload."""
    if "target_text" in row:
        text = row["target_text"]
        if not text:
            return None, "empty_target"
        try:
            refs, kinds = protected_references(row["protected_tokens"])
            return _drop_wrapped_source(target_segments(text, refs, kinds))
        except SegmentError as exc:
            return None, f"marker_error: {exc}"
    text = row.get("translation") or ""
    if not text:
        return None, "empty_target"
    tokens = row.get("protected_tokens") or "[]"
    tokens = json.loads(tokens) if isinstance(tokens, str) else tokens
    markers = [token["marker"] for token in tokens]
    try:
        refs, kinds = references_from_segments(source_segments, markers)
        return target_segments(text, refs, kinds), None
    except SegmentError as exc:
        # Strings stored without markers are plain text; markers left in text are an error.
        if not markers and not MARKER.search(text):
            return target_segments(text, {}, {}), "source_controls_without_markers"
        return None, f"marker_error: {exc}"


def load_targets(returned: str | Path, supplemental: str | Path) -> dict[str, dict]:
    """Every English row by id (both files)."""
    rows = {}
    for row in load_jsonl(returned) + load_jsonl(supplemental):
        if row["id"] in rows:
            raise SegmentError(f"duplicate translation id {row['id']}")
        rows[row["id"]] = row
    return rows
