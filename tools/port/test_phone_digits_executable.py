"""Holiday phone book digit rows: the port table against girl_phone_digits.

girl_phone_digits (D_800B3778, u8[13][7]) is read from the pinned SLPM_86.053
and compared with GIRL_PHONE_DIGITS in port/godot/rules/calendar_phone_screens.gd.
The decoder tests run without game data:
  python3 -m unittest tools.port.test_phone_digits_executable
The executable check is a separate integration invocation, named explicitly,
with TOKIMEMO_EXE set to the mounted pinned executable. A missing variable or
file fails it; it never skips (as actual_red_receipts in wiki/port-testing.md):
  docker run --rm --name <name> -v "$PWD":/workspace \
    -v <main checkout>/disc/files/SLPM_86.053:/exe/SLPM_86.053:ro \
    -e TOKIMEMO_EXE=/exe/SLPM_86.053 -w /workspace tokimemo-content \
    python3 -m unittest \
    tools.port.test_phone_digits_executable.ExecutableTests.executable_rows_match_port_table
"""
import hashlib
import os
import re
import struct
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
RULES = ROOT / "port/godot/rules/calendar_phone_screens.gd"
EXE_SHA256 = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
LOAD_ADDRESS = 0x80041000
HEADER_SIZE = 0x800
GIRL_PHONE_DIGITS = 0x800B3778
ROWS, DIGITS = 13, 7


class PhoneDigitsError(ValueError):
    pass


def decode_rows(exe: bytes) -> list:
    """The 13 rows of 7 digits at D_800B3778, refusing a non-PS-X EXE or a bad digit."""
    if exe[:8] != b"PS-X EXE":
        raise PhoneDigitsError("not a PS-X EXE")
    load, size = struct.unpack_from("<II", exe, 0x18)
    if load != LOAD_ADDRESS:
        raise PhoneDigitsError("load address %#x differs from %#x" % (load, LOAD_ADDRESS))
    start = GIRL_PHONE_DIGITS - LOAD_ADDRESS
    if start + ROWS * DIGITS > size or HEADER_SIZE + start + ROWS * DIGITS > len(exe):
        raise PhoneDigitsError("girl_phone_digits lies outside the text segment")
    raw = exe[HEADER_SIZE + start:HEADER_SIZE + start + ROWS * DIGITS]
    if any(b > 2 for b in raw):
        raise PhoneDigitsError("a phone digit is outside 0..2")
    return [list(raw[r * DIGITS:(r + 1) * DIGITS]) for r in range(ROWS)]


def port_rows(source: str) -> list:
    """GIRL_PHONE_DIGITS rows parsed from the GDScript constant."""
    m = re.search(r"const GIRL_PHONE_DIGITS: Array\[Array\] = \[(.*?)\n\]", source, re.S)
    if not m:
        raise PhoneDigitsError("GIRL_PHONE_DIGITS constant not found")
    return [[int(d) for d in row.split(",")] for row in re.findall(r"\[([0-9, ]+)\]", m.group(1))]


def _exe(rows) -> bytes:
    size = (GIRL_PHONE_DIGITS - LOAD_ADDRESS) + ROWS * DIGITS
    exe = bytearray(HEADER_SIZE + size)
    exe[:8] = b"PS-X EXE"
    struct.pack_into("<II", exe, 0x18, LOAD_ADDRESS, size)
    base = HEADER_SIZE + GIRL_PHONE_DIGITS - LOAD_ADDRESS
    exe[base:base + ROWS * DIGITS] = bytes(d for row in rows for d in row)
    return bytes(exe)


class DecoderTests(unittest.TestCase):
    def test_rows_round_trip(self):
        rows = [[(r + d) % 3 for d in range(DIGITS)] for r in range(ROWS)]
        self.assertEqual(decode_rows(_exe(rows)), rows)

    def test_bad_inputs_are_refused(self):
        rows = [[0] * DIGITS for _ in range(ROWS)]
        bad_digit = [row[:] for row in rows]
        bad_digit[12][6] = 3
        cases = {"header": b"NOT EXE!" + _exe(rows)[8:], "digit": _exe(bad_digit),
                 "truncated": _exe(rows)[:-1]}
        for name, exe in cases.items():
            with self.subTest(name=name), self.assertRaises(PhoneDigitsError):
                decode_rows(exe)

    def test_port_table_parses_as_13_rows(self):
        rows = port_rows(RULES.read_text(encoding="utf-8"))
        self.assertEqual(len(rows), ROWS)
        self.assertTrue(all(len(r) == DIGITS for r in rows))


class ExecutableTests(unittest.TestCase):
    """Integration check, not collected by default discovery (no test_ prefix)."""

    def executable_rows_match_port_table(self):
        name = os.environ.get("TOKIMEMO_EXE")
        self.assertTrue(name, "TOKIMEMO_EXE must name the mounted pinned SLPM_86.053")
        path = Path(name)
        self.assertTrue(path.is_file(), "pinned executable %s is not mounted" % path)
        exe = path.read_bytes()
        self.assertEqual(hashlib.sha256(exe).hexdigest(), EXE_SHA256)
        rows = decode_rows(exe)
        self.assertEqual(port_rows(RULES.read_text(encoding="utf-8")), rows)
        # Row 11 is read by holiday_tel_exit (current_girl 11), row 12 only by the
        # dial shortcut; both hold the rival's number.
        self.assertEqual(rows[11], [0, 2, 1, 0, 2, 1, 0])
        self.assertEqual(rows[12], rows[11])


if __name__ == "__main__":
    unittest.main()
