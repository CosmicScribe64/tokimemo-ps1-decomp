"""Speech layout of the English dialogue (T-39506, owner decisions of 2026-11-01).

* Speaker labels start at one left margin (x -0x80); the opening mark (a quote, or a parenthesis for a
  thought) follows the name after 6 px; every row of the speech hangs under the first character after
  the mark (src/english/script.c, ``english_speaker_draw``, ``english_gnsx``/``english_sndi``). The text
  column therefore depends on the speaker. ``speech_window`` gives the window a record is wrapped in: the
  message window from that column, for the widest speaker the record can have (an inline label is
  measured; a girl's line is measured with the longest girl label, the player's with seven wide
  letters).
* Matched quotes: the Japanese opens 「/『 after the name and never closes it. The runtime draws a
  closing straight quote at the end of each speech (``english_speech_close`` in src/english/script.c):
  at the end of each reply variant, and at the end of a speech record unless the next record of its
  group continues the same speech (commands 0xD, 0x12, 0x11 draw on the next row of the same box). A
  page that already ends with a quote, or is a skipped variant, gets none. Drawing it at run time keeps
  the files at their size (the DATE talk files have no room for a byte per speech). The wrapper measures
  each page with that quote (``close_speech`` before ``wrap``, ``unclose`` after). Thoughts (commands
  4, 6, 0xE and inline thought labels) are closed by the runtime with ")" already. The runtime's
  opening mark is the straight quote (the translation of the original 「 literal), so the closing one
  is straight too.
"""
from __future__ import annotations

import dataclasses
import re

from .english import LABEL_END, LABEL_SPEECH, LABEL_THOUGHT, OPTION_END
from .windows import Window

LABEL_LEFT = -0x80
LABEL_GAP = 6
QUOTE = b'"'
OPEN_PAREN = b"("
CLOSING = (b'"', b"\xa1", b"\xa3")
EMPTY_OPTION = b"\x0c"

SPEECH = frozenset((0x0, 0x1, 0x2, 0x5, 0xA, 0xB, 0xC, 0xF))
CONTINUATION = frozenset((0xD, 0x11, 0x12))
THOUGHT = frozenset((0x4, 0x6, 0xE))
PLAYER = frozenset((0x0, 0x4, 0x6, 0xE))
WIDEST = {"player": b"MMMMMMM", "girl": b"Tatebayashi", 0xA: b"Yoshio", 0xB: b"Fujisaki"}
ID = re.compile(r"^(.*):g(\d+):l(\d+)$")


def command_of(record) -> int | None:
    return ((((record.unit or {}).get("context") or {}).get("command") or {}).get("value"))


def next_commands(records) -> dict:
    """{record id: command of the next record in the same group} for talk script records."""
    by_key = {}
    for record in records:
        m = ID.match(record.id)
        if m:
            by_key[(m.group(1), int(m.group(2)), int(m.group(3)))] = command_of(record)
    out = {}
    for record in records:
        m = ID.match(record.id)
        if m:
            out[record.id] = by_key.get((m.group(1), int(m.group(2)), int(m.group(3)) + 1))
    return out


def _label(payload: bytes) -> tuple[bytes, bytes] | None:
    """(label, mark) of an inline label at the start of the payload."""
    if not payload or payload[0] not in (LABEL_SPEECH, LABEL_THOUGHT):
        return None
    end = payload.find(bytes([LABEL_END]))
    label = payload[1:end] if end >= 0 else b""
    return label, OPEN_PAREN if payload[0] == LABEL_THOUGHT else QUOTE


def speech_window(window: Window, command, payload: bytes, metrics) -> Window:
    """The window from the text column of the record's widest speaker (see the module docstring)."""
    if window.name == "telephone_narration":
        label = max(WIDEST["player"], WIDEST["girl"], key=metrics.width)
        mark = QUOTE
    elif window.name != "message_speech":
        return window
    elif _label(payload) is not None:
        label, mark = _label(payload)
    elif command in PLAYER:
        label, mark = WIDEST["player"], OPEN_PAREN if command in THOUGHT else QUOTE
    elif command in WIDEST:
        label, mark = WIDEST[command], QUOTE
    else:
        label, mark = WIDEST["girl"], QUOTE
    left = LABEL_LEFT + metrics.width(label) + LABEL_GAP + metrics.width(mark)
    return dataclasses.replace(window, left=left) if left > window.left else window


def _close(page: bytes) -> bytes:
    if not page or page.startswith(EMPTY_OPTION):
        return page
    body = page.rstrip(EMPTY_OPTION)
    tail = page[len(body):]
    if body.rstrip(b" ").endswith(CLOSING) or not body.strip(b" "):
        return page
    return body.rstrip(b" ") + QUOTE + tail


def separate_aside(payload: bytes) -> bytes:
    """Keep a trailing parenthetical thought on its own row after a completed sentence."""
    body = payload.rstrip()
    if not body.endswith(b")"):
        return payload
    depth = 0
    for pos in range(len(body) - 1, -1, -1):
        if body[pos] == ord(")"):
            depth += 1
        elif body[pos] == ord("("):
            depth -= 1
            if depth == 0:
                before = body[:pos].rstrip(b" \n")
                if before.endswith((b".", b"!", b"?", b"\xa5")) and pos and body[pos - 1] in b" \n":
                    return before + b"\n" + body[pos:]
                break
    return payload


def close_speech(payload: bytes, command, next_command) -> tuple[bytes, int]:
    """The payload with matched quotes, and how many were added."""
    label = _label(payload)
    speech = (command in SPEECH or command in CONTINUATION or (command == 0x7 and label is not None
                                                                and label[1] == QUOTE))
    if not speech or next_command in CONTINUATION:
        return payload, 0
    if command in (0x2, 0x5):
        pages = payload.split(bytes([OPTION_END]))
        closed = [_close(page) for page in pages]
        return bytes([OPTION_END]).join(closed), sum(a != b for a, b in zip(pages, closed))
    closed = _close(payload)
    return closed, int(closed != payload)


def unclose(payload: bytes, before: bytes, command) -> bytes:
    """Removes the quotes ``close_speech`` added to ``before`` (the runtime draws them). Rows may have
    been wrapped in between, which turns spaces into row ends and changes no byte count."""
    split = (lambda p: p.split(bytes([OPTION_END]))) if command in (0x2, 0x5) else (lambda p: [p])
    out = []
    for page, original in zip(split(payload), split(before)):
        body = page.rstrip(EMPTY_OPTION)
        if len(page) == len(original) + 1 and body.endswith(QUOTE):
            page = body[:-1] + page[len(body):]
        out.append(page)
    return bytes([OPTION_END]).join(out)
