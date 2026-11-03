"""Which line breaks of a dialogue page carry meaning (T-39506).

The translators kept the Japanese row breaks, so English pages arrive with short rows that only made
sense in Japanese ("(What'll I do / this / week?)"). Each break is judged on the Japanese source of the
same record (its segments, with the row controls):

* forced: the Japanese row before the break was full or nearly full for the window (at least
  ``capacity - FULL_MARGIN`` glyph cells of ``ORIGINAL_ROW_CELLS``); the break only wrapped the line.
* intentional: the Japanese row ended clearly short and at a natural boundary: after 。！？ (or ! ?),
  after a closing 」』）, after an ellipsis …/‥, or before an opening 「『（.
* any other short row: treated as forced (the text continues).

Intentional breaks are mapped to English sentence ends or opening quotes and parentheses. Equal row
counts allow a break to keep its position only when that English boundary has the expected kind.
Otherwise, use the boundary of the same kind and rank (the n-th sentence end or opener). When no such
boundary exists, drop the break and log it as ambiguous. Every other break becomes a space and the
page is wrapped again by ``wrap.wrap``. Pages (option ends) never change, and windows without
``reflow`` (choices) keep their rows.

Category rule: records whose container is listed in ``KEEP_ALL_BREAKS`` (songs, letters, poems, lists)
keep every break. None of the windowed containers is one today: the song lines (opening, ending,
karaoke) are C strings and table entries without a window, which the wrapper does not touch.
"""
from __future__ import annotations

import re
from dataclasses import dataclass, field

from .english import LINE_END, OPTION_END
from .segments import Control, Text

ORIGINAL_ROW_CELLS = {"message_speech": 14, "message_narration": 18, "telephone_narration": 14,
                      "ending_player": 15, "ending_heroine": 18}
FULL_MARGIN = 2
NAME_CODE_CELLS = 3  # a Japanese name substitution is about three glyphs
KEEP_ALL_BREAKS: tuple = ()

ROW_KINDS = {("separator", "row"), ("row_separator", "row_separator")}
SENTENCE_END_JP = "。！？!?"
CLOSERS_JP = "」』）)"
ELLIPSIS_JP = "…‥"
OPENERS_JP = "「『（("
EN_SENTENCE_END = re.compile(
    rb"(?<!\bDr)(?<!\bMr)(?<!\bMrs)(?<!\bMs)(?<!\bProf)(?<!\bSt)(?<!\bJr)(?<!\bSr)"
    rb"[.!?\xa1\xa5](?:[\")'\xa1\xa3]*)(?= )", re.IGNORECASE)
EN_OPENER = re.compile(rb"(?<= )[(\"\xa0\xa2]")


@dataclass
class SourceBreak:
    cells: int          # glyph cells of the Japanese row before the break
    capacity: int
    kind: str           # forced | intentional | short
    boundary: str = ""  # sentence | opener (for intentional breaks)
    rank: int = 0       # sentence ends or openers in the page before the break

    @property
    def fill(self) -> int:
        return round(100 * self.cells / self.capacity)


@dataclass
class ReflowNotes:
    breaks_seen: int = 0
    forced: int = 0
    kept_rule: int = 0
    kept_category: int = 0
    ambiguous: int = 0
    kept: list = field(default_factory=list)  # (reason, fill %)


def _is_page_end(segment) -> bool:
    return isinstance(segment, Control) and segment.kind == "separator" \
        and segment.name in ("choice_option", "reply_variant") and segment.raw != b" "


def _is_row_end(segment) -> bool:
    return isinstance(segment, Control) and ((segment.kind, segment.name) in ROW_KINDS
                                             or (segment.kind == "separator" and segment.name == "choice_option"
                                                 and segment.raw == b" "))


def source_pages(segments: list) -> list[list[str]]:
    """Japanese rows per page; a name code counts as NAME_CODE_CELLS glyphs (written as U+3000)."""
    pages, rows, row = [], [], []
    for segment in segments:
        if _is_page_end(segment):
            rows.append("".join(row))
            pages.append(rows)
            rows, row = [], []
        elif _is_row_end(segment):
            rows.append("".join(row))
            row = []
        elif isinstance(segment, Text):
            row.append(segment.text)
        elif isinstance(segment, Control) and segment.kind in ("substitution", "conditional_substitution"):
            row.append("　" * NAME_CODE_CELLS)
    rows.append("".join(row))
    pages.append(rows)
    return pages


def judge(rows: list[str], capacity: int) -> list[SourceBreak]:
    out, sentences, openers = [], 0, 0
    for before, after in zip(rows, rows[1:]):
        sentences += sum(before.count(c) for c in SENTENCE_END_JP)
        openers += sum(before.count(c) for c in OPENERS_JP)
        cells = len(before)
        tail, head = before.rstrip(), after.lstrip()
        if cells >= capacity - FULL_MARGIN:
            out.append(SourceBreak(cells, capacity, "forced"))
        elif tail and (tail[-1] in SENTENCE_END_JP + CLOSERS_JP or tail[-1] in ELLIPSIS_JP):
            out.append(SourceBreak(cells, capacity, "intentional", "sentence", sentences))
        elif head and head[0] in OPENERS_JP:
            out.append(SourceBreak(cells, capacity, "intentional", "opener", openers + 1))
        else:
            out.append(SourceBreak(cells, capacity, "short"))
    return out


def _nth(pattern, text: bytes, n: int) -> int | None:
    """Offset of the space that follows (sentence) or precedes (opener) the n-th match, or None."""
    if n <= 0:
        return None
    matches = list(pattern.finditer(text))
    if len(matches) < n:
        return None
    m = matches[n - 1]
    return m.end() if pattern is EN_SENTENCE_END else m.start() - 1


def reflow_page(page: bytes, source: list[SourceBreak] | None, notes: ReflowNotes, keep_all: bool) -> bytes:
    rows = page.split(bytes([LINE_END]))
    english_breaks = len(rows) - 1
    notes.breaks_seen += english_breaks
    if keep_all:
        notes.kept_category += english_breaks
        notes.kept += [("category", None)] * english_breaks
        return page
    if source is None:
        notes.ambiguous += english_breaks
        return b" ".join(r.strip(b" ") for r in rows)
    text = bytearray(b" ".join(r.strip(b" ") for r in rows))
    starts, at = [], 0
    for r in rows[:-1]:  # offset of the space that replaced each English break
        at += len(r.strip(b" "))
        starts.append(at)
        at += 1
    kept_at = set()
    for k, brk in enumerate(source):
        if brk.kind != "intentional":
            continue
        pattern = EN_SENTENCE_END if brk.boundary == "sentence" else EN_OPENER
        boundaries = {m.end() if pattern is EN_SENTENCE_END else m.start() - 1
                      for m in pattern.finditer(text)}
        if len(source) == english_breaks and starts[k] in boundaries:
            where = starts[k]
        else:
            where = _nth(pattern, bytes(text), brk.rank)
        if where is None or where >= len(text) or text[where] != 0x20 \
                or not text[:where].strip() or not text[where + 1:].strip():
            notes.ambiguous += 1
            continue
        kept_at.add(where)
        notes.kept_rule += 1
        notes.kept.append(("after %s" % brk.boundary, brk.fill))
    notes.forced += sum(1 for b in source if b.kind != "intentional")
    for where in kept_at:
        text[where] = LINE_END
    return bytes(text)


def reflow(payload: bytes, skip: int, segments: list | None, window_name: str, container_path: str) -> tuple[bytes, ReflowNotes]:
    """The payload with only the meaningful row breaks (see the module docstring), and the notes."""
    notes = ReflowNotes()
    capacity = ORIGINAL_ROW_CELLS.get(window_name, 14)
    pages = payload[skip:].split(bytes([OPTION_END]))
    source = [judge(rows, capacity) for rows in source_pages(segments)] if segments is not None else None
    if source is not None and len(source) != len(pages):
        source = None
    keep_all = container_path in KEEP_ALL_BREAKS
    out = [reflow_page(page, source[i] if source else None, notes, keep_all) for i, page in enumerate(pages)]
    return payload[:skip] + bytes([OPTION_END]).join(out), notes
