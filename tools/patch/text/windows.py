"""Text windows of the English build: pixel width and line count per record context (T-39507).

One table (``WINDOWS``) holds every window whose geometry the code proves, and ``window_for``
picks the window a record is drawn in. Coordinates are the game's: x is relative to the screen
centre (the GPU draw offset), a row is 16 px. Sources:

* Message window: ``message_window_show`` (src/main/hud_panels.c) draws the frame from x -0x85
  to +0x85, 0x34 px high. ``talk_speak_part`` (src/main/scene_library.c) renders speech at
  x -0x48 (the speaker label and the opening bracket sit left of it) and narration at x -0x80.
  Rows are at y 0x30, 0x40 and 0x50; ``check_k_scroll`` scrolls every row up by 16 when the
  typewriter reaches a row at y 0x60 or below, so the window shows 3 rows and longer text
  scrolls (the original has single records of up to 9 rows). ``talk_render`` copies each row into a
  64-byte buffer.
* Right edge 0x7C: the widest original rows end there in every message-window context (speech
  rows of 14 glyphs at 14 px from -0x48, narration of 18 from -0x80, choices of 16 plus the
  closing bracket from -0x72).
* Choices: commands 3, 8 and 0x13 render through ``talk_render(-0x72, ..., label_enabled=1)``,
  which draws the opening bracket at x - 14 and appends the closing one to every row; each
  option is one row and the box has room for three. Options do not scroll and cannot wrap.
* Telephone narration: ``sndisp`` splits at the row separator and ``sndi``
  (src/main/menu_speaker_girl.c) draws rows at y 0x32, 0x42, 0x52 from x -0x48 (the first row
  from -0x56 behind an opening bracket), through 40-byte row buffers. Rows from the third on all
  go to y 0x52 and ``sndisp`` sets ``text_scroll_state``, so the box scrolls; the originals have
  up to 6 rows, and ``sndisp`` keeps at most 5 row ends (``line_break_offsets[5]``), so 6 rows
  is a hard limit.

A record outside these contexts (menu labels, tables, C strings) has no window proven yet:
``window_for`` returns None and the wrapper only measures it.
"""
from __future__ import annotations

from dataclasses import dataclass

ROW_PX = 16
MESSAGE_RIGHT = 0x7C
BRACKET_PX = 14  # one Shift-JIS glyph at the default advance (text_state.glyph_advance)


@dataclass(frozen=True)
class Window:
    name: str
    left: int
    right: int
    lines: int
    scroll: bool
    wrap: bool
    row_bytes: int | None
    source: str
    max_rows: int | None = None
    reflow: bool = False

    @property
    def width(self) -> int:
        return self.right - self.left


# Row buffers of the English parsers (src/english/script.c, T-39506): 160 bytes after name expansion.
ENGLISH_ROW_BYTES = 159

WINDOWS = {window.name: window for window in (
    Window("message_speech", -0x48, MESSAGE_RIGHT, 3, True, True, ENGLISH_ROW_BYTES,
           "talk_speak_part: speech rows at x -0x48; message_window_show; check_k_scroll", reflow=True),
    Window("message_narration", -0x80, MESSAGE_RIGHT, 3, True, True, ENGLISH_ROW_BYTES,
           "talk_speak_part: narration rows at x -0x80 (commands 7 without label, 9, 0x10)", reflow=True),
    Window("choice", -0x72, MESSAGE_RIGHT - BRACKET_PX, 3, False, False, ENGLISH_ROW_BYTES,
           "talk_speak_part commands 3, 8, 0x13: talk_render(-0x72, label_enabled) with brackets"),
    Window("telephone_narration", -0x48, MESSAGE_RIGHT, 3, True, True, ENGLISH_ROW_BYTES,
           "sndisp/sndi: rows at x -0x48 that scroll, at most 6 rows (english_sndisp)", max_rows=6,
           reflow=True),
)}

# Talk script commands (context.command.value) and the window their text goes to.
COMMAND_WINDOWS = {
    0x0: "message_speech", 0x1: "message_speech", 0x2: "message_speech", 0x4: "message_speech",
    0x5: "message_speech", 0x6: "message_speech", 0xA: "message_speech", 0xB: "message_speech",
    0xC: "message_speech", 0xD: "message_speech", 0xE: "message_speech", 0xF: "message_speech",
    0x11: "message_speech", 0x12: "message_speech",
    0x9: "message_narration", 0x10: "message_narration",
    0x3: "choice", 0x8: "choice", 0x13: "choice",
}
LABEL_COMMAND = 0x7  # inline label: speech when the record starts with a label, else narration

# Width of a name or value code once the runtime expands it (T-39506): player names hold at most
# 7 single-byte letters, 6 px is the advance of most lower-case letters; the player form adds an
# honorific ("-kun"), the girl name is a family name with "-san" (the longest is measured).
NAME_LETTERS = 7
NAME_PX = NAME_LETTERS * 6
NAME_CODE_PX = {
    0x10: b"Aaaaaaa-kun",       # P player form: name and honorific
    0x11: NAME_PX,              # p family name
    0x12: NAME_PX,              # m given name
    0x13: 2 * NAME_PX + 4,      # F full name, family + space + given
    0x14: b"Yoshio",            # Y Yoshio
    0x15: b"Tatebayashi-san",   # G girl name with honorific, the longest
    0x16: NAME_PX, 0x17: NAME_PX, 0x18: NAME_PX, 0x19: NAME_PX, 0x1A: NAME_PX,  # S O W A M
}
YOSHIO = b"Yoshio"


SNDISP_CALLS = frozenset(("sndisp", "sndi"))


def consumer_calls(unit: dict) -> set:
    """Functions the unit's proven consumers pass the string to (source and MIPS contexts)."""
    context = unit.get("context") or {}
    calls = set()
    for item in context.get("source_contexts") or []:
        site = item.get("source_site") or {}
        if site.get("call"):
            calls.add(site["call"])
    for item in context.get("mips_contexts") or []:
        if item.get("callee"):
            calls.add(item["callee"])
    return calls


def window_for(container_kind: str, record) -> tuple[Window | None, str]:
    """(window, basis) for a record; basis says why that window was chosen."""
    unit = record.unit or {}
    controls = [s for s in record.segments if getattr(s, "kind", None)]
    if any(c.kind == "row_separator" for c in controls):
        return WINDOWS["telephone_narration"], "row_separator: sndisp renderer"
    if consumer_calls(unit) & SNDISP_CALLS:
        return WINDOWS["telephone_narration"], "consumer sndisp/sndi (T-39506)"
    if container_kind == "talk_script" and record.kind == "script_record":
        command = (unit.get("context") or {}).get("command") or {}
        value = command.get("value")
        if value == LABEL_COMMAND:
            labelled = bool(record.segments) and getattr(record.segments[0], "kind", None) == "inline_label"
            return (WINDOWS["message_speech"], "command 7 with inline label") if labelled else \
                (WINDOWS["message_narration"], "command 7 without label")
        name = COMMAND_WINDOWS.get(value)
        if name:
            return WINDOWS[name], "command %s" % command.get("name", value)
        return WINDOWS["message_speech"], "command %r not in the table: narrowest message window" % value
    if container_kind == "talk_script_supplemental":
        return WINDOWS["message_speech"], "supplemental talk file: command unknown, narrowest message window"
    return None, "no window proven for this context"
