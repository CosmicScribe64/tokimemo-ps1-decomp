#!/usr/bin/env python3
"""Unit tests for tools/emu/emu.py on synthetic data (no game data, no emulator).

Run: tools/emu/run.sh shell -c "python3 tools/emu/test_emu.py"  (or tools/docker.sh python3 ...)
"""
import argparse
import os
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import emu  # noqa: E402


class FakeImage:
    """Main-exe code at 0x80010000: words given as {addr: word}."""

    def __init__(self, words):
        self.words = words

    def is_code(self, addr, overlay=None):
        return 0x80010000 <= addr < 0x80020000

    def word(self, addr, overlay=None):
        return self.words.get(addr, 0) if self.is_code(addr) else None


class FakeSyms:
    def __init__(self, starts):
        self.starts = sorted(starts)

    def func_start(self, addr, overlay=None):
        return max(s for s in self.starts if s <= addr), True


class UnwindTest(unittest.TestCase):
    def test_walks_prologues(self):
        # caller at 0x80010000: addiu sp,-0x20; sw ra,0x1C(sp); jal callee at +0x8 (returns to +0x10)
        # callee at 0x80010100: addiu sp,-0x18; sw ra,0x14(sp); break at +0x20
        words = {0x80010000: 0x27BDFFE0, 0x80010004: 0xAFBF001C,
                 0x80010100: 0x27BDFFE8, 0x80010104: 0xAFBF0014}
        sp = 0x801FFE00
        stack = bytearray(0x40)
        struct.pack_into("<I", stack, 0x14, 0x80010010)  # callee's saved ra
        struct.pack_into("<I", stack, 0x18 + 0x1C, 0x90000000)  # caller's saved ra: not code, stop
        hit = {"pc": 0x80010120, "regs": {"sp": sp, "ra": 0}, "stack": stack.hex()}
        frames = emu.unwind(hit, FakeImage(words), FakeSyms([0x80010000, 0x80010100]), None)
        self.assertEqual(frames, [0x80010120, 0x80010008])

    def test_leaf_at_entry_uses_ra(self):
        hit = {"pc": 0x80010100, "regs": {"sp": 0x801FFE00, "ra": 0x80010010}, "stack": ""}
        frames = emu.unwind(hit, FakeImage({}), FakeSyms([0x80010000, 0x80010100]), None)
        self.assertEqual(frames, [0x80010100, 0x80010008])


class JobTest(unittest.TestCase):
    def args(self, **kw):
        a = argparse.Namespace(input=None, mash=None, frames=100)
        a.__dict__.update(kw)
        return a

    def test_inputs(self):
        ev = emu.parse_inputs(self.args(input="10:START,20:UP+CROSS/3"))
        self.assertEqual(ev, [{"frame": 10, "buttons": ["START"], "hold": 6},
                              {"frame": 20, "buttons": ["UP", "CROSS"], "hold": 3}])

    def test_mash(self):
        ev = emu.parse_inputs(self.args(mash=["START:30:30:90"]))
        self.assertEqual([e["frame"] for e in ev], [30, 60, 90])

    def test_lua_value(self):
        self.assertEqual(emu.lua_value({"a": [1, True, None, "x\"y"]}), '{["a"] = {1, true, nil, "x\\"y"}}')


class PngTest(unittest.TestCase):
    def test_16bpp(self):
        with tempfile.TemporaryDirectory() as d:
            raw, png = os.path.join(d, "s.raw"), os.path.join(d, "s.png")
            open(raw, "wb").write(struct.pack("<HH", 0x001F, 0x7C00))  # red, blue
            emu.png_from_raw(raw, 2, 1, 16, png)
            data = open(png, "rb").read()
            self.assertTrue(data.startswith(b"\x89PNG"))
            self.assertFalse(os.path.exists(raw))


if __name__ == "__main__":
    unittest.main()
