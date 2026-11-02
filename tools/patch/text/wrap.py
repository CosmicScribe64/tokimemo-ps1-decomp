"""Build-time word wrap of English payloads in pixels (T-39507).

``FontMetrics`` reads ``build/en/font_metrics.json`` (tools/english_font.py): the advance of each
single-byte glyph, the advance of a 2-byte Shift-JIS character and the row height. ``wrap``
takes an encoded payload (tools/patch/text/english.py) and the window it is drawn in
(tools/patch/text/windows.py) and returns the payload with row ends where a row is wider than
the window:

* Rows are split at the row end 0x0A and the option end 0x0B. A row that fits is kept as it is,
  so a translator's explicit line breaks stay; a row that does not fit is broken at the last
  space that fits, repeatedly (the space becomes the row end). A word wider than the window
  stays whole and is reported (``overflow_rows``).
* Choice windows (``wrap`` false) are never broken: each option is one selectable row.
* Lines: a window that scrolls (the message window) takes any number of rows, as the original
  does, up to its ``max_rows`` when the renderer has one. A window that does not scroll reports
  ``too_many_lines`` when a block (the rows up to an option end, or every row of a choice) has
  more rows than the window. A page break would need a new script record or a runtime control
  code, so it is not inserted.
* Width counts the name and value codes at their expanded English width (``NAME_CODE_PX``) and
  skips an inline speaker label (marker byte, then the label up to ``": "``), which the
  renderer draws left of the text.
"""
from __future__ import annotations

import json
from dataclasses import dataclass, field
from pathlib import Path

from .english import LINE_END, NAME_CODE_BYTES, OPTION_END
from .windows import NAME_CODE_PX, YOSHIO, Window

LABEL_MARKERS = b">-+<"
SPACE = 0x20


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
                total += self.width(YOSHIO) if px is None else px
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
    stop = len(payload)
    for end_byte in (LINE_END, OPTION_END):
        at = payload.find(bytes([end_byte]))
        if at >= 0:
            stop = min(stop, at)
    colon = payload.find(b": ", 1, stop)
    return colon + 2 if colon >= 0 else 1


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


def wrap(payload: bytes, window: Window, metrics: FontMetrics) -> WrapResult:
    """Wrap ``payload`` for ``window``; see the module docstring."""
    result = WrapResult(payload)
    out = bytearray()
    skip = label_end(payload)
    block_rows, rows_total, max_block = 0, 0, 0
    for index, (row, separator) in enumerate(_split_rows(payload)):
        lead = skip if index == 0 else 0
        if window.wrap:
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
