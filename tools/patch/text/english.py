"""English runtime encoder for the text writers (T-39507).

``EnglishEncoder`` turns the segments of an English record into the bytes the English build
draws (wiki/english-text-runtime.md):

* text: ASCII 0x20-0x7E as itself; the font's extra glyphs at 0xA0-0xA9 (curly quotes, em dash,
  ellipsis, heart, note, e-acute, star); typographic characters the translators used are first
  normalised to one of those or to ASCII (``NORMALISE``). A character the game's Shift-JIS font
  has (kana, kanji, full-width punctuation that has no English form) is kept as its 2-byte code,
  which the runtime draws at the fixed advance, and counted in ``kept_shift_jis``. Anything else
  is written as ``?`` and counted in ``unmapped``.
* controls: the script control codes move off the letters and the space, which are text in
  English: row end 0x20 to 0x0A, option end ``q`` to 0x0B, empty option ``n`` to 0x0C, and the
  name and value codes ``P p m F Y G S O W A M`` to 0x10-0x1A. The fixed renderer's odd byte
  before the NUL (``terminal_byte_before_nul``) only padded the 2-byte loader and is dropped.
  An inline speaker label marker (``>``, ``-``, ``+``: 0x1C; ``<``, a thought: 0x1D) starts a label
  of any length that ``end_inline_label`` ends with 0x1F where the translation's ``": "`` is
  (T-39506, include/english/script.h). Every other control (printf directives, storage bytes,
  opaque slots) is kept byte for byte: none of them collides with English text.

The counters are per encoder; ``take_record_notes`` returns and clears the characters seen since
the last call, so a caller that encodes one record at a time can report them per record.
"""
from __future__ import annotations

import unicodedata
from collections import Counter

from .segments import Control

LINE_END = 0x0A
OPTION_END = 0x0B
EMPTY_OPTION = 0x0C
# Name and value codes in the order of wiki/english-text-runtime.md (0x10 + index).
NAME_CODE_LETTERS = "PpmFYGSOWAM"
NAME_CODES = {letter: 0x10 + index for index, letter in enumerate(NAME_CODE_LETTERS)}
NAME_CODE_BYTES = frozenset(NAME_CODES.values())
LABEL_SPEECH = 0x1C   # inline label `>`, `-`, `+`: the label, then speech after a quote mark
LABEL_THOUGHT = 0x1D  # inline label `<`: the label, then a thought in parentheses
LABEL_END = 0x1F
LABEL_MARKERS = bytes([LABEL_SPEECH, LABEL_THOUGHT])

# Extra glyphs of translations/en/font/latin12.txt.
EXTRAS = {
    "“": 0xA0,  # left double quotation mark
    "”": 0xA1,  # right double quotation mark
    "‘": 0xA2,  # left single quotation mark
    "’": 0xA3,  # right single quotation mark
    "—": 0xA4,  # em dash
    "…": 0xA5,  # horizontal ellipsis
    "♥": 0xA6,  # heart
    "♪": 0xA7,  # musical note
    "é": 0xA8,  # e with acute
    "★": 0xA9,  # star
}

# Typographic variants folded onto an extra glyph or onto ASCII before encoding.
NORMALISE = {
    "―": "—", "⸺": "——",  # horizontal bar, two-em dash
    "–": "-", "‒": "-", "‐": "-", "‑": "-", "−": "-",
    "⋯": "…", "‥": "..",
    "♡": "♥", "❤": "♥", "❣": "♥", "❥": "♥",
    "♫": "♪", "♬": "♪", "♩": "♪",
    "☆": "★", "✩": "★", "✪": "★", "⭐": "★",
    "«": '"', "»": '"', "„": '"', "″": '"', "‚": ",", "′": "'",
    " ": " ", "　": " ", " ": " ", " ": " ", " ": " ",
    "～": "~", "〜": "~", "×": "x", "•": "*", "·": "*",
    "→": "->", "←": "<-",
    "É": "E",
}

# Characters the game's 2-byte font draws but that have an English form are folded instead.
_FULL_WIDTH_ASCII = range(0xFF01, 0xFF5F)


def _fold(char: str) -> str | None:
    """ASCII or extra-glyph spelling of ``char``, or None when it has none."""
    if char in NORMALISE:
        return NORMALISE[char]
    if ord(char) in _FULL_WIDTH_ASCII:
        return unicodedata.normalize("NFKC", char)
    decomposed = unicodedata.normalize("NFKD", char)
    base = "".join(c for c in decomposed if not unicodedata.combining(c))
    if base != char and base and all(" " <= c <= "~" for c in base):
        return base
    return None


def _shift_jis_pair(char: str) -> bytes | None:
    try:
        raw = char.encode("shift_jis")
    except UnicodeEncodeError:
        return None
    return raw if len(raw) == 2 else None


class EnglishEncoder:
    """The English runtime encoding (wiki/english-text-runtime.md)."""

    name = "english"
    allows_high_bytes = True

    def __init__(self):
        self.unmapped: Counter = Counter()
        self.kept_shift_jis: Counter = Counter()
        self.folded: Counter = Counter()
        self.dropped_controls: Counter = Counter()
        self._record_unmapped: Counter = Counter()
        self._record_kept: Counter = Counter()

    def encode_text(self, text: str) -> bytes:
        out = bytearray()
        for char in text:
            out.extend(self._char(char))
        return bytes(out)

    def _char(self, char: str) -> bytes:
        if " " <= char <= "~":
            return char.encode("ascii")
        if char in EXTRAS:
            return bytes([EXTRAS[char]])
        folded = _fold(char)
        if folded is not None:
            self.folded[char] += 1
            return self.encode_text(folded) if folded != char else b"?"
        pair = _shift_jis_pair(char)
        if pair is not None:
            self.kept_shift_jis[char] += 1
            self._record_kept[char] += 1
            return pair
        self.unmapped[char] += 1
        self._record_unmapped[char] += 1
        return b"?"

    def encode_control(self, control: Control) -> bytes:
        kind, name, raw = control.kind, control.name, control.raw
        if kind == "separator":
            if name == "row" or (name == "choice_option" and raw == b" "):
                return bytes([LINE_END])
            if name in ("choice_option", "reply_variant"):
                return bytes([OPTION_END])
        if kind == "row_separator":
            return bytes([LINE_END])
        if kind in ("substitution", "conditional_substitution"):
            letter = raw.decode("latin-1")
            if letter in NAME_CODES:
                return bytes([NAME_CODES[letter]])
        if kind in ("conditional_choice_zero", "reply_skip_marker"):
            return raw.replace(b"n", bytes([EMPTY_OPTION]))
        if kind == "inline_label":
            return bytes([LABEL_THOUGHT if raw == b"<" else LABEL_SPEECH])
        if kind == "terminal_byte_before_nul":
            self.dropped_controls[kind] += 1
            return b""
        return raw

    def take_record_notes(self) -> dict:
        """Unmapped and kept Shift-JIS characters since the last call (then cleared)."""
        notes = {}
        if self._record_unmapped:
            notes["unmapped"] = dict(self._record_unmapped)
        if self._record_kept:
            notes["kept_shift_jis"] = dict(self._record_kept)
        self._record_unmapped, self._record_kept = Counter(), Counter()
        return notes


def end_inline_label(payload: bytes) -> bytes:
    """Ends the inline speaker label at the start of a payload with LABEL_END.

    The translators wrote the label as text before ``": "`` (``Teacher: Ah, ...``); the ``": "``
    becomes LABEL_END. Without a colon in the first row the label is empty."""
    if not payload or payload[0] not in LABEL_MARKERS:
        return payload
    stop = len(payload)
    for end_byte in (LINE_END, OPTION_END):
        at = payload.find(bytes([end_byte]))
        if at >= 0:
            stop = min(stop, at)
    colon = payload.find(b": ", 1, stop)
    if colon >= 0:
        return payload[:colon] + bytes([LABEL_END]) + payload[colon + 2:]
    colon = payload.find(b":", 1, stop)
    if colon >= 0:
        return payload[:colon] + bytes([LABEL_END]) + payload[colon + 1:].lstrip(b" ")
    return payload[:1] + bytes([LABEL_END]) + payload[1:]


def untokenized_name_codes(payload: bytes, source_raw: bytes) -> bytes:
    """Name codes in a string the handoff could not tokenise (T-39506).

    Strings drawn by ``sndisp`` without a renderer view (the prologue lines of NAME_ENT) kept their
    name letters as text, and the translators wrote them as words: "My name's P." Each ``P`` or
    ``G`` that the original holds as a one-byte code (Shift-JIS stepping) becomes that code in the
    English payload, matched to a standalone capital letter, at most as many times as the original
    has it."""
    counts = {ord("P"): 0, ord("G"): 0}
    pos = 0
    while pos < len(source_raw):
        byte = source_raw[pos]
        if 0x81 <= byte <= 0x9F or 0xE0 <= byte <= 0xFC:
            pos += 2
            continue
        if byte in counts:
            counts[byte] += 1
        pos += 1
    out = bytearray(payload)
    for letter, count in counts.items():
        code = NAME_CODES[chr(letter)]
        for at in range(len(out)):
            if count == 0:
                break
            before = out[at - 1] if at else 0x20
            after = out[at + 1] if at + 1 < len(out) else 0x20
            if out[at] == letter and not chr(before).isalpha() and not chr(after).isalpha():
                out[at] = code
                count -= 1
    return bytes(out)
