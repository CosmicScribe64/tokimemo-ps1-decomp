"""English payloads of every record: encode, then wrap for the record's window (T-39507).

``english_payloads(containers, targets, metrics)`` encodes the English target of every owner and
source-only record with ``EnglishEncoder`` and wraps it with ``wrap.wrap`` when ``window_for``
knows its window. It returns the payloads by container path (the input of ``Image.write`` and of
``c_literals.rewrite_sources``) and one note per record for the build report: the window and why,
rows, breaks added, rows still too wide, characters kept as Shift-JIS or not mappable, and the
reason when the original payload is kept.

SPEAKTST files are out of scope for the first release (T-39500) and get no payloads.
"""
from __future__ import annotations

from collections import Counter
from dataclasses import dataclass, field, replace

from .containers import Container
from .english import EnglishEncoder, end_inline_label, untokenized_name_codes
from .segments import Control, encode
from .translations import target_of
from .windows import window_for
from .wrap import FontMetrics, label_end, wrap
from . import reflow as source_reflow
from . import speech

SKIPPED_KINDS = ("speaktst", "reference_only")
# Tables that must stay Japanese: the kana pages and the kanji reading labels of the name entry, where a
# player still picks kana (T-39506). The translators gave romaji there.
KEPT_TABLES = ("dictionary:CDROM/EXEDIR/NAME_ENT.EXN:hiragana_grid:",
               "dictionary:CDROM/EXEDIR/NAME_ENT.EXN:katakana_grid:",
               "dictionary:CDROM/EXEDIR/NAME_ENT.EXN:reading_labels:")
# Data that the handoff read as text: ETC album_background_lba_cat_00 .. _15 (0x8014E0C4-0x8014EA1C) are
# tables of CD sector numbers whose bytes decode as half-width katakana. Writing the "translations" there
# would load the wrong album backgrounds (T-39506).
NOT_TEXT_RANGES = {"CDROM/EXEDIR/ETC.EXN": ((0x1C0C4, 0x1CA1C),)}


# Name entry presets become the player's name, which holds at most 7 letters (11 for the nickname) in the
# English build (T-39506); a longer English preset keeps the original kana one and is reported.
PRESET_LIMITS = {"dictionary:CDROM/EXEDIR/NAME_ENT.EXN:preset_family_names:": 7,
                 "dictionary:CDROM/EXEDIR/NAME_ENT.EXN:preset_given_names:": 7,
                 "dictionary:CDROM/EXEDIR/NAME_ENT.EXN:preset_nicknames:": 11}


def _preset_too_long(record_id: str, payload: bytes) -> bool:
    for prefix, limit in PRESET_LIMITS.items():
        if record_id.startswith(prefix):
            return len(payload) > limit
    return False


def _not_text(record) -> bool:
    ranges = NOT_TEXT_RANGES.get(record.path or "", ())
    return record.offset is not None and any(start <= record.offset < end for start, end in ranges)


@dataclass
class EnglishText:
    payloads: dict = field(default_factory=dict)
    notes: dict = field(default_factory=dict)
    encoder: EnglishEncoder | None = None

    def counts(self) -> Counter:
        return Counter(note["status"] for note in self.notes.values())


def english_payloads(containers: dict[str, Container], targets: dict[str, dict], metrics: FontMetrics,
                     encoder: EnglishEncoder | None = None) -> EnglishText:
    encoder = encoder or EnglishEncoder()
    out = EnglishText(encoder=encoder)
    for path, container in sorted(containers.items()):
        if container.kind in SKIPPED_KINDS:
            continue
        following = speech.next_commands(container.records.values()) if container.kind == "talk_script" else {}
        for record in container.records.values():
            if record.role not in ("owner", "source_only"):
                continue
            note = {"path": path, "kind": record.kind, "container": container.kind}
            out.notes[record.id] = note
            row = targets.get(record.id)
            if _not_text(record):
                note["status"], note["reason"] = "kept_original", "data, not text (T-39506)"
                continue
            if record.id.startswith(KEPT_TABLES):
                note["status"], note["reason"] = "kept_original", "name entry kana page (T-39506)"
                continue
            if row is None:
                note["status"] = "kept_original"
                note["reason"] = "no_translation_row"
                continue
            if any(isinstance(s, Control) and s.kind == "opaque" for s in record.segments):
                note["status"], note["reason"] = "kept_original", "opaque_source_bytes"
                continue
            segments, problem = target_of(row, record.segments)
            if problem:
                note["problem"] = problem
            if segments is None:
                note["status"], note["reason"] = "kept_original", problem
                continue
            encoder.take_record_notes()
            payload = end_inline_label(encode(segments, encoder))
            if _preset_too_long(record.id, payload):
                note["status"], note["reason"] = "kept_original", "preset longer than the English name limit (T-39506)"
                continue
            note.update(encoder.take_record_notes())
            window, basis = window_for(container.kind, record)
            if window is not None and window.name == "telephone_narration" \
                    and not any(isinstance(s, Control) and s.kind == "substitution" for s in segments):
                payload = untokenized_name_codes(payload, bytes.fromhex((record.unit or {}).get("raw_hex") or ""))
            note["status"] = "placed"
            closed = 0
            if window is not None:
                window = speech.speech_window(window, speech.command_of(record), payload, metrics)
                joined = False
                if window.reflow:
                    rows_before = payload.count(b"\n") + 1
                    payload, flow = source_reflow.reflow(payload, label_end(payload), record.segments,
                                                         window.name, path)
                    joined = True
                    note["reflow"] = {"breaks": flow.breaks_seen, "forced": flow.forced, "kept_rule": flow.kept_rule,
                                      "kept_category": flow.kept_category, "ambiguous": flow.ambiguous,
                                      "kept": flow.kept, "rows_before": rows_before}
                if container.kind in ("talk_script", "talk_script_supplemental"):
                    # measured with the closing quote that the runtime draws (speech.py)
                    unclosed = payload
                    payload, closed = speech.close_speech(payload, speech.command_of(record), following.get(record.id))
                if window.name == "telephone_narration":
                    payload = speech.separate_aside(payload)
                    # sndisp appends the closing mark at runtime; reserve its advance.
                    window = replace(window, right=window.right - metrics.width(b'"'))
                result = wrap(payload, window, metrics, joined=joined)
                payload = speech.unclose(result.payload, unclosed, speech.command_of(record)) if closed else result.payload
                note.update(window=window.name, basis=basis, rows=result.rows,
                            breaks_added=result.breaks_added)
                if window.reflow:
                    note["reflow"]["rows_after"] = result.payload.count(b"\n") + 1
                    note["reflow"]["orphans"], note["reflow"]["pages_over_lines"] = \
                        _orphans_and_long_pages(result.payload, window, metrics)
                if result.long_rows:
                    note["rows_over_row_buffer"] = result.long_rows
                if result.overflow_rows or result.too_many_lines:
                    note["status"] = "unwrappable"
                    note["overflow_px"] = [px - window.width for px in result.overflow_rows]
                    note["too_many_lines"] = result.too_many_lines
            else:
                note["width_px"] = metrics.width(payload)
            out.payloads.setdefault(path, {})[record.id] = payload
    return out


def _orphans_and_long_pages(payload: bytes, window, metrics: FontMetrics) -> tuple[int, int]:
    """Pages whose last row is one word under a third of the window, and pages with more rows than
    the window shows (they scroll, as long pages do in the original)."""
    orphans = long_pages = 0
    for page in payload[label_end(payload):].split(b"\x0b"):
        rows = page.split(b"\n")
        if len(rows) > 1 and b" " not in rows[-1].strip() and metrics.width(rows[-1]) < window.width // 3:
            orphans += 1
        if len(rows) > window.lines:
            long_pages += 1
    return orphans, long_pages
