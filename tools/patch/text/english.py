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
  Every other control (inline label markers, printf directives, storage bytes, opaque slots)
  is kept byte for byte: none of them collides with English text.

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
