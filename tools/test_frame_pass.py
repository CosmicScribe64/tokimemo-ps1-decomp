#!/usr/bin/env python3
"""Unit tests for tools/frame_pass.py using synthetic binasm streams.

No game data and no compiler are involved: each test builds a procedure out
of 16-byte binasm records (the record layout is documented in frame_pass.py)
and checks the rewritten immediates.

Run (in Docker): python3 tools/test_frame_pass.py
"""
import os
import struct
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import frame_pass as fp  # noqa: E402

SP, RA, S0, S1, A0, T8, T9, V0 = 29, 31, 16, 17, 4, 24, 25, 2
Z = fp.Z


def raw(instr, w0=0, w2=0, w3=0, low=0):
    return fp.Rec([w0, (instr << 16) | low, w2, w3])


def code(op, form, r1, r2=fp.REG_NONE, imm=0, r3=0):
    w2 = (r1 << 25) | (r2 << 18) | (form << 14)
    if form == fp.F_FRRR:
        w2 |= r3 << 7
    return fp.Rec([0, (fp.I_IOCODE << 16) | (op << 1), w2, imm & 0xFFFFFFFF])


def mem(op, reg, off, base=SP):
    return code(op, fp.F_FROB, reg, base, off)


def alloc(op, size):
    return code(op, fp.F_FRI, SP, imm=size)


def frame(size):
    return raw(fp.I_IFRAME, w2=size, w3=(SP << 25) | (31 << 18))


def mask(bits, off):
    return raw(fp.I_IMASK, w2=bits, w3=off & 0xFFFFFFFF)


def proc(*body):
    return [raw(fp.I_IENT), raw(fp.I_ILABEL)] + list(body) + [raw(fp.I_IEND)]


def run(recs):
    data = b"".join(struct.pack(">4I", *r.w) for r in recs)
    out = fp.transform(data)
    return [fp.Rec(struct.unpack(">4I", out[i:i + 16])) for i in range(0, len(out), 16)]


def imms(recs):
    return [r.imm for r in recs if r.instr == fp.I_IOCODE]


class NonLeaf(unittest.TestCase):
    def test_hole_after_saves(self):
        # frame 40: args 16, ra save slot 20, local at 28, incoming home at 40
        recs = proc(
            alloc(Z["zsubu"], 40), mem(Z["zsw"], RA, 20), mask(1 << RA, -20),
            frame(40), mem(Z["zsw"], A0, 28), mem(Z["zsw"], A0, 40),
            code(Z["zaddiu"], fp.F_FRRI, A0, SP, 28), mem(Z["zlw"], RA, 20),
            alloc(Z["zaddu"], 40))
        out = run(recs)
        self.assertEqual(imms(out), [56, 20, 44, 56, 44, 20, 56])
        self.assertEqual(out[5].w[2], 56)                      # .frame
        self.assertEqual(out[4].w[3] & 0xFFFFFFFF, (-36) & 0xFFFFFFFF)  # .mask

    def test_save_block_end_uses_mask_offset(self):
        # s0 at 16, ra at 20 (block end 24), locals from 24: only >=24 moves
        recs = proc(
            alloc(Z["zsubu"], 32), mem(Z["zsw"], RA, 20), mem(Z["zsw"], S0, 16),
            mask((1 << RA) | (1 << S0), -12), frame(32),
            mem(Z["zsw"], V0, 16 + 8), mem(Z["zsw"], V0, 12), mem(Z["zlw"], RA, 20),
            alloc(Z["zaddu"], 32))
        out = run(recs)
        self.assertEqual(imms(out), [48, 20, 16, 40, 12, 20, 48])

    def test_argument_build_area_untouched(self):
        recs = proc(
            alloc(Z["zsubu"], 48), mem(Z["zsw"], RA, 28), mem(Z["zsw"], V0, 16),
            mem(Z["zsw"], V0, 20), mask(1 << RA, -20), frame(48),
            mem(Z["zlw"], RA, 28), alloc(Z["zaddu"], 48))
        self.assertEqual(imms(run(recs)), [64, 28, 16, 20, 28, 64])


class Leaf(unittest.TestCase):
    def test_whole_frame_moves(self):
        # leaf, no ra: saves at 8 and 12 in a 16-byte frame move to 24 and 28
        recs = proc(
            alloc(Z["zsubu"], 16), mem(Z["zsw"], S1, 12), mem(Z["zsw"], S0, 8),
            mask((1 << S0) | (1 << S1), -4), frame(16),
            mem(Z["zlw"], S0, 8), mem(Z["zlw"], S1, 12), alloc(Z["zaddu"], 16))
        out = run(recs)
        self.assertEqual(imms(out), [32, 28, 24, 24, 28, 32])
        self.assertEqual(out[5].w[3] & 0xFFFFFFFF, (-4) & 0xFFFFFFFF)  # unchanged

    def test_leaf_that_saves_ra_keeps_saves(self):
        # $ra used as a register in a leaf: saves stay, hole after them
        recs = proc(
            alloc(Z["zsubu"], 24), mem(Z["zsw"], S0, 8), mem(Z["zsw"], RA, 12),
            mask((1 << S0) | (1 << RA), -12), frame(24),
            mem(Z["zsw"], V0, 16), mem(Z["zlw"], RA, 12), alloc(Z["zaddu"], 24))
        out = run(recs)
        self.assertEqual(imms(out), [40, 8, 12, 32, 12, 40])

    def test_frameless_is_untouched(self):
        recs = proc(frame(0), mem(Z["zsw"], A0, 0))
        src = b"".join(struct.pack(">4I", *r.w) for r in recs)
        self.assertEqual(fp.transform(src), src)


class IndexedLocals(unittest.TestCase):
    def leaf(self, *mid):
        return proc(alloc(Z["zsubu"], 32), mask(0, 0), frame(32), *mid,
                    alloc(Z["zaddu"], 32))

    def test_sp_plus_register_base(self):
        recs = self.leaf(
            code(Z["zaddu"], fp.F_FRRR, T8, SP, r3=T9),
            mem(Z["zlw"], V0, 92, base=T8), mem(Z["zsw"], V0, 96, base=T8))
        out = run(recs)
        self.assertEqual(imms(out)[2:4], [108, 112])

    def test_use_after_label_fails(self):
        recs = self.leaf(code(Z["zaddu"], fp.F_FRRR, T8, SP, r3=T9),
                         raw(fp.I_ILAB), mem(Z["zlw"], V0, 92, base=T8))
        with self.assertRaises(fp.PassError):
            run(recs)

    def test_redefinition_ends_tracking(self):
        recs = self.leaf(
            code(Z["zaddu"], fp.F_FRRR, T8, SP, r3=T9),
            code(Z["zlui"], fp.F_FRI, T8, imm=1),
            mem(Z["zlw"], V0, 92, base=T8))
        self.assertEqual(imms(run(recs))[3], 92)

    def test_storing_derived_register_fails(self):
        recs = self.leaf(code(Z["zaddu"], fp.F_FRRR, T8, SP, r3=T9),
                         mem(Z["zsw"], T8, 0))
        with self.assertRaises(fp.PassError):
            run(recs)

    def test_unfollowable_use_fails(self):
        recs = self.leaf(code(Z["zaddu"], fp.F_FRRR, T8, SP, r3=T9),
                         code(Z["zsll"], fp.F_FRRI, V0, T8, 2))
        with self.assertRaises(fp.PassError):
            run(recs)


class Failures(unittest.TestCase):
    def test_frame_pointer_frame(self):
        f = frame(32)
        f.w[3] = (30 << 25) | (31 << 18)
        with self.assertRaises(fp.PassError):
            run(proc(f))

    def test_unmatched_allocation(self):
        with self.assertRaises(fp.PassError):
            run(proc(alloc(Z["zsubu"], 16), mask(0, 0), frame(16)))

    def test_adjust_does_not_match_frame(self):
        with self.assertRaises(fp.PassError):
            run(proc(alloc(Z["zsubu"], 24), mask(0, 0), frame(16),
                     alloc(Z["zaddu"], 24)))

    def test_float_saves_rejected(self):
        recs = proc(alloc(Z["zsubu"], 16), mask(0, 0), raw(fp.I_IFMASK, w2=1),
                    frame(16), alloc(Z["zaddu"], 16))
        with self.assertRaises(fp.PassError):
            run(recs)

    def test_truncated_file(self):
        with self.assertRaises(fp.PassError):
            fp.transform(b"\0" * 20)

    def test_unknown_option_record(self):
        with self.assertRaises(fp.PassError):
            fp.parse(struct.pack(">4I", 0, (fp.I_IOPTION << 16) | (3 << 14), 0, 0))


class Container(unittest.TestCase):
    def test_ascii_payload_skipped(self):
        head = struct.pack(">4I", 0, fp.I_IASCII << 16, 20, 0)
        data = head + b"x" * 32 + struct.pack(">4I", 0, 21 << 16, 0, 0)
        recs = fp.parse(data)
        self.assertEqual(len(recs), 2)
        self.assertEqual(fp.serialize(recs), data)

    def test_roundtrip_without_procedures(self):
        data = struct.pack(">4I", 0, 42 << 16, 3, 19)
        self.assertEqual(fp.transform(data), data)


SNIPPETS = {
    # non-leaf, one local: IDO gives addiu sp,-40 / ra at 20 / local at 28
    "nonleaf": ("extern int g(int *p); int f(int x) { int l[3]; l[0] = x; return g(l); }",
                ["addiu\tsp,sp,-56", "sw\tra,20(sp)", "sw\ta0,44(sp)", "addiu\ta0,sp,44",
                 "lw\tra,20(sp)", "addiu\tsp,sp,56"]),
    # leaf with a local array: IDO gives -16 / 0,4,8 ; the pass moves the frame up
    "leaf": ("int f(int a) { int l[4]; l[0] = a; l[1] = a + 1; l[2] = a * 3; return l[a & 3]; }",
             ["addiu\tsp,sp,-32", "sw\ta0,16(sp)", "addiu\tsp,sp,32"]),
    # variable index into a local array of a non-leaf: IDO uses addu v0,sp,t8;
    # lw v0,24(v0); the displacement must move with the array (IDO frame 56, local at 24)
    "indexed": ("extern void g(int *); int f(int n) { int l[8]; g(l); return l[n & 7]; }",
                ["addiu\tsp,sp,-72", "addiu\ta0,sp,40", "lw\tv0,40(v0)", "addiu\tsp,sp,72"]),
    # loop over a local array: uopt keeps the array address in s1 (addiu s1,sp,48
    # in IDO's frame of 80, T-0017 flags); the immediate moves with the array
    "hoisted": ("extern int g(int); int f(int n) { int l[8]; int i; for (i = 0; i < n; i++) l[i & 7] = g(i); return l[n & 7]; }",
                ["addiu\tsp,sp,-96", "addiu\ts1,sp,64", "addiu\tsp,sp,96"]),
}


@unittest.skipUnless(os.path.exists("/opt/ido/5.3/cc"), "IDO 5.3 not installed")
class IdoIntegration(unittest.TestCase):
    """Compile synthetic C with the real IDO through tools/cc.py's shim."""

    def compile(self, text):
        import shutil
        import subprocess
        import tempfile
        import cc
        tmp = tempfile.mkdtemp(prefix="fptest")
        lib = os.path.join(tmp, "lib")
        os.mkdir(lib)
        try:
            src = os.path.join(tmp, "t.c")
            with open(src, "w") as f:
                f.write(text + "\n")
            env = cc.ido_frame_env("5.3", lib)
            flags = [a for a in cc.IDO_CFLAGS if a != "-Iinclude"]
            subprocess.run(["/opt/ido/5.3/cc"] + flags + ["-o", os.path.join(tmp, "t.o"), src],
                           check=True, env=env)
            return subprocess.run(["mips-linux-gnu-objdump", "-d", os.path.join(tmp, "t.o")],
                                  check=True, stdout=subprocess.PIPE, text=True).stdout
        finally:
            shutil.rmtree(tmp)

    def test_snippets(self):
        for name, (text, expected) in SNIPPETS.items():
            dis = self.compile(text)
            for want in expected:
                self.assertIn(want, dis, "%s: %s missing" % (name, want))


if __name__ == "__main__":
    unittest.main()
