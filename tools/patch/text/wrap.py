"""Build-time word wrap of English payloads in pixels (T-39507).

``FontMetrics`` reads ``build/en/font_metrics.json`` (tools/english_font.py): the advance of each
single-byte glyph, the advance of a 2-byte Shift-JIS character and the row height. ``wrap``
takes an encoded payload (tools/patch/text/english.py) and the window it is drawn in
(tools/patch/text/windows.py) and returns the payload with row ends where a row is wider than
the window:

* Reflow (windows with ``reflow``, the dialogue windows; T-39506): the translators kept the
  Japanese row breaks, which leave short rows in English. The build decides each break from the
  Japanese source (tools/patch/text/reflow.py) and calls ``wrap`` with ``joined``. Without a source,
  each page (the rows up to an option end, so every reply variant is its own page) is joined into
  one paragraph and a row break stays only where the English shows it is deliberate (``keep_break``): before
  a row that opens a parenthesis or a quotation (a new thought or a quoted line), after a row that
  closes one, around an empty row, and after a row that ends a sentence when the next row starts
  with a dash or an ellipsis (a pause that the line break shows). Everywhere else the break becomes
  a space. Page breaks (records, option ends) never change.
* Rows are split at the row end 0x0A and the option end 0x0B. In a reflow window a row too wide
  for the window is set in the fewest rows that fit, with the most even fill (least squared
  slack) and no lone short word on its last row (``_balanced_row``). Elsewhere a row that fits is
  kept and a row that does not fit is broken at the last space that fits, repeatedly (the space
  becomes the row end). A word wider than the window stays whole and is reported
  (``overflow_rows``).
* Choice windows (``wrap`` false) are never broken: each option is one selectable row.
* Lines: a window that scrolls (the message window) takes any number of rows, as the original
  does, up to its ``max_rows`` when the renderer has one. A window that does not scroll reports
  ``too_many_lines`` when a block (the rows up to an option end, or every row of a choice) has
  more rows than the window. A page break would need a new script record or a runtime control
  code, so it is not inserted.
* Width counts the name and value codes at their expanded English width (``NAME_CODE_PX``) and
  skips an inline speaker label (marker byte 0x1C or 0x1D, then the label up to 0x1F), which the
  renderer draws left of the text.
"""
from __future__ import annotations

import json
from dataclasses import dataclass, field
from pathlib import Path

from .english import LABEL_END, LABEL_MARKERS, LINE_END, NAME_CODE_BYTES, OPTION_END
from .windows import NAME_CODE_PX, Window

SPACE = 0x20
OPENERS = (b"(", b"\"", b"\xa0", b"\xa2", b"'")
CLOSERS = (b")", b"\"", b"\xa1", b"\xa3", b"'")
SENTENCE_ENDS = (b".", b"!", b"?", b"\xa5", b")", b"\"", b"\xa1")
PAUSES = (b"-", b"\xa4", b"\xa5", b"...")


@dataclass(frozen=True)
class FontMetrics:
    advance: dict
    sjis_advance: int
    line_height: int

    @classmethod
    def load(cls, path: str | Path) -> "FontMetrics":
        data = json.loads(Path(path).read_text(encoding="utf-8"))
        return cls({int(code, 16): px for code, px in data["advance"].items()},
                   data["sjis_advance"], data["line_height"])

    def width(self, row: bytes) -> int:
        """Pen advance of a row in pixels."""
        total, pos = 0, 0
        while pos < len(row):
            byte = row[pos]
            if byte in NAME_CODE_BYTES:
                px = NAME_CODE_PX[byte]
                total += self.width(px) if isinstance(px, bytes) else px
            elif byte < 0x20:
                pass
            elif 0x81 <= byte <= 0x9F or 0xE0 <= byte <= 0xFC:
                total += self.sjis_advance
                pos += 1
            else:
                total += self.advance.get(byte, self.sjis_advance)
            pos += 1
        return total


@dataclass
class WrapResult:
    payload: bytes
    rows: int = 0
    breaks_added: int = 0
    overflow_rows: list = field(default_factory=list)
    too_many_lines: bool = False
    long_rows: int = 0


def label_end(payload: bytes) -> int:
    """Offset where the text of the first row starts (after an inline speaker label)."""
    if not payload or payload[0] not in LABEL_MARKERS:
        return 0
    end = payload.find(bytes([LABEL_END]))
    return end + 1 if end >= 0 else 1


def keep_break(before: bytes, after: bytes) -> bool:
    """True when the row break between two rows of a page is deliberate (see the module docstring)."""
    before, after = before.rstrip(b" "), after.lstrip(b" ")
    if not before or not after:
        return True
    if after.startswith(OPENERS) or before.endswith(CLOSERS):
        return True
    return before.endswith(SENTENCE_ENDS) and after.startswith(PAUSES)


def reflow(payload: bytes, skip: int = 0) -> bytes:
    """Joins the rows of each page with spaces where the break is not deliberate."""
    out = bytearray(payload[:skip])
    pages = payload[skip:].split(bytes([OPTION_END]))
    for index, page in enumerate(pages):
        rows = page.split(bytes([LINE_END]))
        text = bytearray(rows[0])
        for previous, row in zip(rows, rows[1:]):
            if keep_break(previous, row):
                text.append(LINE_END)
                text.extend(row)
            else:
                text = bytearray(bytes(text).rstrip(b" "))
                text.append(SPACE)
                text.extend(row.lstrip(b" "))
        if index:
            out.append(OPTION_END)
        out.extend(text)
    return bytes(out)


def _split_rows(payload: bytes) -> list[tuple[bytes, int | None]]:
    """[(row bytes, separator byte or None)] in order."""
    rows, start = [], 0
    for pos, byte in enumerate(payload):
        if byte in (LINE_END, OPTION_END):
            rows.append((payload[start:pos], byte))
            start = pos + 1
    rows.append((payload[start:], None))
    return rows


def _break_row(row: bytes, limit: int, metrics: FontMetrics, skip: int) -> tuple[list[bytes], list[int]]:
    """Greedy break of one row at spaces; (pieces, widths of pieces still too wide)."""
    pieces, too_wide, start = [], [], 0
    while True:
        rest = row[start:]
        lead = skip if start == 0 else 0
        if metrics.width(rest[lead:]) <= limit:
            pieces.append(rest)
            return pieces, too_wide
        cut = None
        for pos in range(len(rest) - 1, lead - 1, -1):
            if rest[pos] == SPACE and metrics.width(rest[lead:pos]) <= limit:
                cut = pos
                break
        if cut is None:
            first_space = rest.find(b" ", lead + 1)
            if first_space < 0:
                pieces.append(rest)
                too_wide.append(metrics.width(rest[lead:]))
                return pieces, too_wide
            cut = first_space
            too_wide.append(metrics.width(rest[lead:cut]))
        pieces.append(rest[:cut])
        start += cut + 1


def _balanced_row(row: bytes, limit: int, metrics: FontMetrics, skip: int) -> list[bytes] | None:
    """Fewest rows that fit, with the most even fill and no lone short word on the last row
    (T-39506); None when a word is wider than the window (the greedy break reports it)."""
    words = row.split(b" ")
    if len(words) < 2:
        return None
    space = metrics.advance.get(SPACE, 4)
    widths = [metrics.width(word[skip:] if i == 0 else word) for i, word in enumerate(words)]
    if max(widths) > limit:
        return None

    def span(i, j):  # width of words i..j-1 on one row
        return sum(widths[i:j]) + space * (j - i - 1)

    count = len(words)
    lines, used = 1, 0
    for i, w in enumerate(widths):  # greedy row count, the fewest that fit
        if used and used + space + w > limit:
            lines, used = lines + 1, w
        else:
            used = w if not used else used + space + w
    if lines == 1:
        return None
    best = {}

    def cost(i, k):
        """Least cost to set words i.. into exactly k rows."""
        if (i, k) in best:
            return best[(i, k)]
        if k == 1:
            w = span(i, count)
            result = (None if w > limit else
                      (limit * limit if count - i == 1 and w < limit // 3 else 0), count)
        else:
            result = None
            for j in range(i + 1, count - k + 2):
                w = span(i, j)
                if w > limit:
                    break
                rest = cost(j, k - 1)
                if rest[0] is None:
                    continue
                total = (limit - w) ** 2 + rest[0]
                if result is None or total < result[0]:
                    result = (total, j)
        best[(i, k)] = result if result is not None else (None, None)
        return best[(i, k)]

    if cost(0, lines)[0] is None:
        return None
    pieces, i, k = [], 0, lines
    while k > 0:
        j = cost(i, k)[1]
        pieces.append(b" ".join(words[i:j]))
        i, k = j, k - 1
    return pieces


def wrap(payload: bytes, window: Window, metrics: FontMetrics, joined: bool = False) -> WrapResult:
    """Wrap ``payload`` for ``window``; see the module docstring. ``joined``: the caller has already
    removed the row breaks that carry no meaning (tools/patch/text/reflow.py, from the Japanese
    source); otherwise a reflow window uses ``keep_break`` on the English alone."""
    skip = label_end(payload)
    if window.reflow and not joined:
        payload = reflow(payload, skip)
    result = WrapResult(payload)
    out = bytearray()
    block_rows, rows_total, max_block = 0, 0, 0
    for index, (row, separator) in enumerate(_split_rows(payload)):
        lead = skip if index == 0 else 0
        balanced = _balanced_row(row, window.width, metrics, lead) if window.wrap and window.reflow else None
        if balanced is not None:
            pieces, too_wide = balanced, []
        elif window.wrap:
            pieces, too_wide = _break_row(row, window.width, metrics, lead)
        else:
            width = metrics.width(row[lead:])
            pieces, too_wide = [row], ([width] if width > window.width else [])
        result.overflow_rows.extend(too_wide)
        result.breaks_added += len(pieces) - 1
        for piece in pieces:
            if window.row_bytes is not None and len(piece) > window.row_bytes:
                result.long_rows += 1
        out.extend(bytes([LINE_END]).join(pieces))
        if separator is not None:
            out.append(separator)
        block_rows += len(pieces)
        rows_total += len(pieces)
        if separator == OPTION_END or separator is None:
            max_block = max(max_block, block_rows)
            block_rows = 0
    result.payload = bytes(out)
    result.rows = rows_total if not window.wrap else max_block
    limit = window.max_rows if window.scroll else window.lines
    if limit is not None and result.rows > limit:
        result.too_many_lines = True
    return result
