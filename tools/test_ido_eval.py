#!/usr/bin/env python3
"""Unit tests for the binasm `.option pic` rewrite of tools/ido_eval.py (T-20700).

Synthetic 16-byte binasm records only; no compiler, archive or game data is needed.

Run (in Docker): python3 tools/test_ido_eval.py
"""
import os
import struct
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import frame_pass as fp  # noqa: E402
import ido_eval  # noqa: E402


def option(kind, value):
    """An `.option` record: kind 1 is the optimisation level, kind 2 is pic."""
    return struct.pack(">4I", 0, (fp.I_IOPTION << 16) | (kind << 14), 0, value)


def other(instr, w3):
    return struct.pack(">4I", 0, instr << 16, 0, w3)


class SetPicOption(unittest.TestCase):
    def test_pic_record_only(self):
        data = option(2, 0) + option(1, 2) + other(fp.I_IENT, 1) + other(fp.I_IEND, 0)
        out = ido_eval.set_pic_option(data, 1)
        self.assertEqual(out[:16], option(2, 1))
        self.assertEqual(out[16:], data[16:])

    def test_same_value_is_unchanged(self):
        data = option(2, 1) + option(1, 2)
        self.assertEqual(ido_eval.set_pic_option(data, 1), data)

    def test_every_pic_record(self):
        data = option(2, 1) + other(fp.I_IENT, 1) + option(2, 1)
        out = ido_eval.set_pic_option(data, 0)
        self.assertEqual(out, option(2, 0) + other(fp.I_IENT, 1) + option(2, 0))

    def test_ascii_payload_kept(self):
        # an .ascii record carries its bytes after the record; they must not be parsed
        text = b"\x00\x2f\x80\x00" + b"\x00" * 12
        data = struct.pack(">4I", 0, fp.I_IASCII << 16, len(text), 0) + text + option(2, 0)
        out = ido_eval.set_pic_option(data, 1)
        self.assertEqual(out[:32], data[:32])
        self.assertEqual(out[32:], option(2, 1))

    def test_values_per_version(self):
        self.assertEqual(ido_eval.PIC_OPTION, {"5.3": 0, "4.1": 1})


if __name__ == "__main__":
    unittest.main()
