"""Lossless pair storage for English player names (T-39506).

The C runtime and build writer share immutable assignments in name_pairs.inc.
Only unused English single-byte glyph slots are assigned. Shift-JIS pairs are
copied atomically, including trail bytes that equal an assigned pair code.
"""
from pathlib import Path
import re

TABLE = Path(__file__).resolve().parents[3] / "include/english/name_pairs.inc"


def load_pairs(path=TABLE):
    pairs = {}
    for line in path.read_text().splitlines():
        if not line.strip() or line.startswith("/*"):
            continue
        match = re.fullmatch(r'ENGLISH_NAME_PAIR\((0x[0-9A-F]{2}), "([a-z]{2})"\)', line)
        if not match:
            raise ValueError("Invalid persistent name-pair assignment: " + line)
        code, pair = int(match[1], 16), match[2].encode("ascii")
        if code != 0xB0 + len(pairs) or pair in pairs.values():
            raise ValueError("Name-pair assignments must be unique and consecutive")
        pairs[code] = pair
    if len(pairs) != 48:
        raise ValueError("Persistent name-pair table must cover 0xB0..0xDF")
    return pairs


PAIRS = load_pairs()
CODES = {pair: code for code, pair in PAIRS.items()}


def char_bytes(data, at):
    code = data[at]
    return 2 if code >= 0x80 and (code < 0xA0 or code >= 0xE0) and at + 1 < len(data) else 1


def pack(data: bytes) -> bytes:
    """Encode pairs without changing case, punctuation, existing tokens or kana."""
    out = bytearray()
    at = 0
    while at < len(data):
        size = char_bytes(data, at)
        if size == 2:
            out.extend(data[at:at + 2])
        elif data[at:at + 2] in CODES:
            out.append(CODES[data[at:at + 2]])
            size = 2
        else:
            out.append(data[at])
        at += size
    return bytes(out)


def unpack(data: bytes) -> bytes:
    """Decode for editing/display; never interpret a Shift-JIS trail as a token."""
    out = bytearray()
    at = 0
    while at < len(data):
        size = char_bytes(data, at)
        if size == 2:
            out.extend(data[at:at + 2])
        else:
            out.extend(PAIRS.get(data[at], data[at:at + 1]))
        at += size
    return bytes(out)
