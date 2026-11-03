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
from .break_review import review as review_breaks
from .reviewed_targets import REPAIRS, reviewed_target
from .english import EnglishEncoder, end_inline_label, untokenized_name_codes
from .segments import Control, Text, encode
from .anomaly_repairs import repair_target
from .translations import target_of
from .windows import window_for
from .wrap import FontMetrics, label_end, wrap
from . import reflow as source_reflow
from . import speech
from . import names

# T-39531: tour_saki_script_begin binds NI_S6W and calls talk_names_set(4).
# Other containers retain conservative widths until their speaker binding is proven.
FIXED_GIRL_LABELS = {"CDROM/BUNKA/NI_S6W.BIN": b"Nijino"}

# T-39540: this literal wraps at runtime; keep its button sequence together.
REVIEWED_LITERAL_ROWS = {
    "SLPM_86.053:00075e0c:48a6d7d45052": (
        b"While reading the magazine, enter the Konami Code... U U D D L R L R X O)",
        b"While reading the magazine, enter the Konami Code...\nU U D D L R L R X O)"),
}

# T-39532: retain the invitation's date phrase on one row. Exact matching leaves
# revised translations alone; normal wrapping still checks both rows for fit.
REVIEWED_ROWS = {
    ("CDROM/SERIFU/SHUGAKU.BIN", b"Wanna go out again the day after tomorrow?"):
        b"Wanna go out again\nthe day after tomorrow?",
}

# T-39537: reviewed generated breaks; keep wording and reply boundaries intact.
REVIEWED_BREAK_ROWS = {
    "talk:CDROM/SERIFU/EVENT_FU.BIN:initializer-13:g0000:l0013": (
        b"Great...\nI've got a cake ready. Won't you come...\nto my room?",
        b"Great...\nI've got a cake ready.\nWon't you come...\nto my room?"),
    "talk:CDROM/BUNKA/BUN_PC.BIN:bun-pc:g0059:l0000": (
        b"It blew up... Is the performer okay?",
        b"It blew up...\nIs the performer okay?"),
    "talk:CDROM/BUNKA/FU_S6O.BIN:main:g0051:l0000": (
        b"It blew up... Is the performer okay?",
        b"It blew up...\nIs the performer okay?"),
    "talk:CDROM/BUNKA/HI_S6O.BIN:main:g0012:l0000": (
        b"It blew up... Is the performer okay?",
        b"It blew up...\nIs the performer okay?"),
    "talk:CDROM/SERIFU/WHITE.BIN:table-3:g0001:l0001": (
        b"Oh?\nYou have someone to repay? What a surprise.\nWell, do your best.",
        b"Oh?\nYou have someone to repay?\nWhat a surprise.\nWell, do your best."),
    "talk:CDROM/SERIFU/DATE_KA.BIN:section-01:g0001:l0000": (
        b"A stroll down a path like this is so refreshing.",
        b"A stroll down a path\nlike this is so refreshing."),
    "talk:CDROM/SERIFU/GASSHUKU.BIN:table-4:g0023:l0006": (
        b"T-that'd be nice, but \x15, let's become No. 1 first.",
        b"T-that'd be nice, but\n\x15, let's become\nNo. 1 first."),
}

# T-39535: keep the first reply's phrase boundaries in these park records.
PARK_REPLY_IDS = frozenset(
    f"talk:CDROM/SERIFU/DATE_KG.BIN:section-11:g{group:04d}:l0000"
    for group in (25, 41, 50, 57))
PARK_REPLY_ROWS = {
    b"Because you were entranced by me?\nThen that's only natural.\nOhohoho.":
        b"Because you were\nentranced by me?\nThen that's only natural.\nOhohoho.",
    b"I'm not sure why, but that feels good.\nPraise me more.\nOhohoho.":
        b"I'm not sure why,\nbut that feels good.\nPraise me more.\nOhohoho.",
}

SKIPPED_KINDS = ("speaktst", "reference_only")
# Tables that must stay Japanese: the kana pages and the kanji reading labels of the name entry, where a
# player still picks kana (T-39506). The translators gave romaji there.
KEPT_TABLES = ("dictionary:CDROM/EXEDIR/NAME_ENT.EXN:hiragana_grid:",
               "dictionary:CDROM/EXEDIR/NAME_ENT.EXN:katakana_grid:",
               "dictionary:CDROM/EXEDIR/NAME_ENT.EXN:reading_labels:")
# Data that the handoff read as text: ETC album_background_lba_cat_00 .. _15 (0x8014E0C4-0x8014EA1C) are
# tables of CD sector numbers whose bytes decode as half-width katakana. Writing the "translations" there
# would load the wrong album backgrounds (T-39506).
NOT_TEXT_RANGES = {
    "CDROM/EXEDIR/ETC.EXN": ((0x1C0C4, 0x1CA1C),),
    # T-39547: TtSpriteSet.attr contains texture-page/UV bits, not text.
    "CDROM/EXEDIR/TT.EXN": (
        (0x1BC30, 0x1BC34), (0x1C100, 0x1C104), (0x1CD3C, 0x1CD40),
        (0x1E92C, 0x1E930), (0x20C28, 0x20C2C), (0x224FC, 0x22504),
        (0x239D8, 0x239DC), (0x24C78, 0x24C7C), (0x259C0, 0x259C4),
        (0x26A48, 0x26A4C), (0x27538, 0x2753C), (0x27B00, 0x27B04),
    ),
}


# Name storage keeps its original byte limits; reversible letter pairs preserve full presets.
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


def stage_prefixes(container: Container) -> dict[str, bytes]:
    """T-39544: command bytes belong to the physical row, before its visible text."""
    prefixes = {}
    for record in container.records.values():
        if record.kind != "stage_text_record":
            continue
        context = (record.unit or {}).get("context", {})
        stage = context.get("stage_context", {})
        skip = stage.get("body_relative_offset", 0)
        if not skip:
            continue
        prefix = bytes.fromhex(stage.get("protected_prefix_hex", ""))
        if len(prefix) != skip:
            raise ValueError("Missing stage command prefix: " + record.id)
        for rid in context.get("source_inventory_record_ids", []):
            owner = container.records[rid]
            if owner.owner:
                owner = container.records[owner.owner]
            if not owner.payload.startswith(prefix):
                raise ValueError("Stage prefix disagrees with physical source: " + rid)
            if owner.id in prefixes and prefixes[owner.id] != prefix:
                raise ValueError("Conflicting stage prefixes: " + owner.id)
            prefixes[owner.id] = prefix
    return prefixes


def english_payloads(containers: dict[str, Container], targets: dict[str, dict], metrics: FontMetrics,
                     encoder: EnglishEncoder | None = None) -> EnglishText:
    encoder = encoder or EnglishEncoder()
    out = EnglishText(encoder=encoder)
    for path, container in sorted(containers.items()):
        if container.kind in SKIPPED_KINDS:
            continue
        prefixes = stage_prefixes(container)
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
            reviewed_row = reviewed_target(record.id, record.payload, row)
            if reviewed_row is not row:
                note['translation_review'] = REPAIRS[record.id]['ticket']
                row = reviewed_row
            segments, problem = target_of(row, record.segments)
            if problem:
                note["problem"] = problem
            if segments is None:
                note["status"], note["reason"] = "kept_original", problem
                continue
            repaired = repair_target(record.id, record.payload, row.get('target_text', ''))
            if repaired is not None:
                segments = [Text(repaired)]
                note['source_repair'] = 'reviewed source anomaly (T-39511)'
            encoder.take_record_notes()
            payload = end_inline_label(encode(segments, encoder))
            literal_rows = REVIEWED_LITERAL_ROWS.get(record.id)
            if literal_rows is not None:
                if payload != literal_rows[0]:
                    raise ValueError("Reviewed menu literal changed: " + record.id)
                payload = literal_rows[1]
                note["reviewed_rows"] = "T-39540"
            if _preset_too_long(record.id, payload):
                payload = names.pack(payload)
                if _preset_too_long(record.id, payload):
                    raise ValueError("English preset exceeds packed name storage: " + record.id)
                note["name_pair_encoding"] = True
            note.update(encoder.take_record_notes())
            window, basis = window_for(container.kind, record)
            if window is not None and window.name == "telephone_narration" \
                    and not any(isinstance(s, Control) and s.kind == "substitution" for s in segments):
                payload = untokenized_name_codes(payload, bytes.fromhex((record.unit or {}).get("raw_hex") or ""))
            note["status"] = "placed"
            closed = 0
            if window is not None:
                window = speech.speech_window(window, speech.command_of(record), payload, metrics,
                                              girl_label=FIXED_GIRL_LABELS.get(path))
                joined = False
                if window.reflow:
                    rows_before = payload.count(b"\n") + 1
                    payload, flow = source_reflow.reflow(payload, label_end(payload), record.segments,
                                                         window.name, path)
                    joined = True
                    note["reflow"] = {"breaks": flow.breaks_seen, "forced": flow.forced, "kept_rule": flow.kept_rule,
                                      "kept_category": flow.kept_category, "ambiguous": flow.ambiguous,
                                      "kept": flow.kept, "rows_before": rows_before}
                    reviewed = REVIEWED_ROWS.get((path, payload))
                    if reviewed is not None:
                        payload = reviewed
                        note["reviewed_rows"] = "T-39532"
                    reviewed_break = REVIEWED_BREAK_ROWS.get(record.id)
                    if reviewed_break is not None and payload == reviewed_break[0]:
                        payload = reviewed_break[1]
                        note["reviewed_rows"] = "T-39537"
                    if record.id in PARK_REPLY_IDS:
                        first, separator, remaining = payload.partition(b"\x0b")
                        reviewed = PARK_REPLY_ROWS.get(first)
                        if reviewed is not None:
                            payload = reviewed + separator + remaining
                            note["reviewed_rows"] = "T-39535"
                if window.name in ("ending_player", "ending_heroine"):
                    payload = speech.close_ending_speech(payload, speech.command_of(record))
                elif container.kind in ("talk_script", "talk_script_supplemental"):
                    # measured with the closing quote that the runtime draws (speech.py)
                    unclosed = payload
                    payload, closed = speech.close_speech(payload, speech.command_of(record), following.get(record.id))
                if window.name == "telephone_narration":
                    payload = speech.separate_aside(payload)
                    # sndisp appends the closing mark at runtime; reserve its advance.
                    window = replace(window, right=window.right - metrics.width(b'"'))
                result = wrap(payload, window, metrics, joined=joined)
                if window.reflow:
                    note["break_review"] = review_breaks(
                        payload[label_end(payload):], result.payload[label_end(result.payload):],
                        metrics.for_window(window), window.width)
                payload = speech.unclose(result.payload, unclosed, speech.command_of(record)) if closed else result.payload
                note.update(window=window.name, basis=basis, rows=result.rows,
                            breaks_added=result.breaks_added)
                if window.reflow:
                    note["reflow"]["rows_after"] = result.payload.count(b"\n") + 1
                    note["reflow"]["orphans"], note["reflow"]["pages_over_lines"] = \
                        _orphans_and_long_pages(result.payload, window, metrics.for_window(window))
                if result.long_rows:
                    note["rows_over_row_buffer"] = result.long_rows
                if result.overflow_rows or result.too_many_lines:
                    note["status"] = "unwrappable"
                    note["overflow_px"] = [px - window.width for px in result.overflow_rows]
                    note["too_many_lines"] = result.too_many_lines
            else:
                note["width_px"] = metrics.width(payload)
            if record.id in prefixes:
                payload = prefixes[record.id] + payload
                note["protected_stage_prefix"] = prefixes[record.id].hex()
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
