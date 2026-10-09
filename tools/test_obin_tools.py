#!/usr/bin/env python3
"""Unit tests for tools/obin_syms.py and tools/obin_map.py with synthetic input (no game data).

Run (in Docker): python3 tools/test_obin_tools.py
"""
import os
import struct
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import obin_map  # noqa: E402
import obin_syms  # noqa: E402


def make_ecoff(syms):
    """Build a minimal ECOFF: file header, optional header, no sections, mdebug with externals."""
    strs = b""
    ext = b""
    for name, addr in syms:
        ext += struct.pack("<hhiII", 0, 0, len(strs), addr, 1 | (5 << 6))
        strs += name.encode() + b"\0"
    symptr = 20 + 0x38
    hdr = struct.pack("<HHIIIHH", 0x162, 0, 0, symptr, 0x60, 0x38, 0)
    hdr += b"\0" * 0x38
    ss_off = symptr + 0x60
    ext_off = ss_off + len(strs)
    h = [0] * 23
    h[obin_syms.HDRR_NAMES.index("issExtMax")] = len(strs)
    h[obin_syms.HDRR_NAMES.index("cbSsExtOffset")] = ss_off
    h[obin_syms.HDRR_NAMES.index("iextMax")] = len(syms)
    h[obin_syms.HDRR_NAMES.index("cbExtOffset")] = ext_off
    return hdr + struct.pack("<hh23i", 0x7009, 0x312, *h) + strs + ext


class ParseTest(unittest.TestCase):
    def test_parse_and_sizes(self):
        secs, syms = obin_syms.parse(make_ecoff([("b", 0x80000020), ("a", 0x80000000)]))
        self.assertEqual([s.name for s in syms], ["a", "b"])
        self.assertEqual(syms[0].size, 0x20)
        self.assertIsNone(syms[1].size)

    def test_rejects_other_files(self):
        with self.assertRaises(ValueError):
            obin_syms.parse(b"\0" * 0x100)


E = obin_map.Entry


class MapTest(unittest.TestCase):
    def test_align_shifted_sequence(self):
        o = [8, 16, 24, 32, 40]
        f = [4, 8, 16, 24, 32, 40]
        self.assertEqual(obin_map.align(o, f), [(0, 1), (1, 2), (2, 3), (3, 4), (4, 5)])

    def test_high_needs_diverse_run(self):
        O = [E(i * 16, 16, "n%d" % i) for i in range(6)]
        F = [E(i * 16, 16, "func_%d" % i) for i in range(6)]
        res = {i: i for i in range(6)}
        self.assertNotIn("high", obin_map.classify(res, O, F).values())   # one repeated size
        sizes = [4, 8, 12, 16]
        O = [E(0, z, "a%d" % z) for z in sizes]
        F = [E(0, z, "f%d" % z) for z in sizes]
        self.assertEqual(set(obin_map.classify({i: i for i in range(4)}, O, F).values()), {"high"})

    def test_lis_chain(self):
        self.assertEqual(obin_map.lis_chain([(0, 5), (1, 1), (2, 2), (3, 3)]), [(1, 1), (2, 2), (3, 3)])

    def test_map_region_uses_anchors(self):
        sizes = [4, 8, 12, 16, 20, 24]
        O = [E(0, z, "o%d" % i) for i, z in enumerate(sizes)]
        F = [E(0, 99, "f_extra")] + [E(0, z, "f%d" % i) for i, z in enumerate(sizes)]
        res = obin_map.map_region(O, F, [(0, 1)])
        self.assertEqual(res, {i: i + 1 for i in range(6)})

    def test_map_region_anchor_splits_segments(self):
        # Equal sizes on both sides of the anchor must not be paired across it.
        O = [E(0, 8, "a"), E(0, 50, "anchor"), E(0, 8, "b")]
        F = [E(0, 8, "x"), E(0, 50, "anchor"), E(0, 8, "y")]
        self.assertEqual(obin_map.map_region(O, F, [(1, 1)]), {0: 0, 1: 1, 2: 2})

    def test_write_outputs_only_high_placeholders(self):
        import tempfile
        O = [E(0, 4, "keep"), E(0, 8, "skip")]
        F = [E(0x80041000, 4, "func_80041000"), E(0x80041004, 8, "func_80041004")]
        res = {0: 0, 1: 1}
        with tempfile.TemporaryDirectory() as d:
            sp, rp = os.path.join(d, "s"), os.path.join(d, "r")
            n = obin_map.write_outputs(sp, rp, [("t", res, {0: "high", 1: "med"}, O, F, "type:func")], set())
            self.assertEqual(n, 1)
            self.assertIn("keep = 0x80041000; // type:func size:0x4", open(sp).read())
            self.assertNotIn("skip", open(sp).read())


if __name__ == "__main__":
    unittest.main()
