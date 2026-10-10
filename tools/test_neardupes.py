#!/usr/bin/env python3
"""Unit tests for tools/neardupes.py on synthetic asm and C (no game data).

Run: tools/docker.sh python3 tools/test_neardupes.py
"""
import os
import struct
import sys
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dupes  # noqa: E402
import neardupes  # noqa: E402
from test_dupes import Repo, insn  # noqa: E402


def w(op, rs=0, rt=0, imm=0):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def hexw(word):
    return struct.pack('<I', word & 0xFFFFFFFF).hex().upper()


def func_asm(name, callee, imm_a, imm_b, hi=None, lo=None, frame=0x18, slot=4):
    """addiu $a0,$zero,imm_a ; jal callee ; ori $a1,$zero,imm_b [; lui/ori pair] ; jr ra."""
    out = 'nonmatching %s, 0x28\n\nglabel %s\n' % (name, name)
    out += insn(0, 0x80132000, hexw(w(9, 29, 29, -frame)), 'addiu      $sp, $sp, -0x%X' % frame)
    out += insn(4, 0x80132004, hexw(w(9, 0, 4, imm_a)), 'addiu      $a0, $zero, %d' % imm_a)
    out += insn(8, 0x80132008, '9834050C', 'jal        %s' % callee)
    out += insn(0xC, 0x8013200C, hexw(w(13, 0, 5, imm_b)), '   ori        $a1, $zero, 0x%X' % imm_b)
    h, l = (hi, lo) if hi is not None else (0, 0)
    out += insn(0x10, 0x80132010, hexw(w(15, 0, 8, h)), 'lui        $t0, 0x%X' % h)
    out += insn(0x14, 0x80132014, hexw(w(13, 8, 8, l)), 'ori        $t0, $t0, 0x%X' % l)
    out += insn(0x18, 0x80132018, hexw(w(0x2B, 29, 8, slot)), 'sw         $t0, 0x%X($sp)' % slot)
    out += insn(0x1C, 0x8013201C, hexw(w(9, 29, 29, frame)), 'addiu      $sp, $sp, 0x%X' % frame)
    out += insn(0x20, 0x80132020, '0800E003', 'jr         $ra')
    out += insn(0x24, 0x80132024, '00000000', ' nop')
    out += 'endlabel %s\n' % name
    return out


SRC_C = ('void fsrc(void) {\n    s32 v;\n    func_80140000(5, 0x30);\n    v = 0x80100000;\n}\n')
TGT_C = 'INCLUDE_ASM("asm/ovl/BBB/nonmatchings/BBB", ftgt);\n'


def make_repo(src_asm, tgt_asm, src_c=SRC_C):
    r = Repo()
    r.write('asm/ovl/AAA/matchings/AAA/fsrc.s', src_asm)
    r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s', tgt_asm)
    r.write('src/ovl/AAA.c', '#include "common.h"\n#include "ovl/AAA.h"\n\n' + src_c)
    r.write('src/ovl/BBB.c', '#include "common.h"\n#include "ovl/BBB.h"\n\n' + TGT_C)
    r.write('include/ovl/AAA.h', '#ifndef A\n#define A\n#include "common.h"\n#include "game.h"\n'
            'void func_80140000(s32, s32);\n#endif\n')
    r.write('include/ovl/BBB.h', '#ifndef B\n#define B\n#include "common.h"\n#include "game.h"\n#endif\n')
    r.write('include/game.h', '#ifndef G\n#define G\n#endif\n')
    r.write('include/common.h', '')
    return r


def plan(r):
    funcs = dupes.load_funcs(r.root, near=True)
    cache = {'aliases': dupes.load_aliases(r.root)}
    return funcs, neardupes.plan_all(r.root, funcs, cache)


SRC_ASM = func_asm('fsrc', 'func_80140000', 5, 0x30, 0x8010, 0)
TGT_ASM = func_asm('ftgt', 'func_80140000', 9, 0x2C, 0x8011, 0x40)


class FingerprintTests(unittest.TestCase):
    def test_immediates_do_not_change_near_key(self):
        a = dupes.parse_asm(SRC_ASM, 'alu')[0]
        b = dupes.parse_asm(TGT_ASM, 'alu')[0]
        self.assertEqual(a, b)
        self.assertNotEqual(dupes.parse_asm(SRC_ASM)[0], dupes.parse_asm(TGT_ASM)[0])

    def test_frame_size_and_register_stay_in_shape(self):
        a = dupes.parse_asm(SRC_ASM, 'alu')[0]
        self.assertNotEqual(a, dupes.parse_asm(func_asm('f', 'func_80140000', 5, 0x30, 0x8010, 0, frame=0x20), 'alu')[0])
        self.assertNotEqual(a, dupes.parse_asm(SRC_ASM.replace(hexw(w(9, 0, 4, 5)), hexw(w(9, 0, 6, 5))), 'alu')[0])

    def test_memory_offset_masked_only_in_wide_mode(self):
        a = func_asm('f', 'func_80140000', 5, 0x30, 0x8010, 0)
        b = a.replace(hexw(w(0x2B, 29, 8, 4)), hexw(w(0x2B, 4, 8, 8)))      # sw $t0, 8($a0)
        c = a.replace(hexw(w(0x2B, 29, 8, 4)), hexw(w(0x2B, 4, 8, 12)))
        self.assertNotEqual(dupes.parse_asm(b, 'alu')[0], dupes.parse_asm(c, 'alu')[0])
        self.assertEqual(dupes.parse_asm(b, 'wide')[0], dupes.parse_asm(c, 'wide')[0])

    def test_exact_mode_unchanged(self):
        out = []
        dupes.parse_asm(SRC_ASM, imm_out=out)
        self.assertEqual(out, [])

    def test_constants_pair_lui_ori(self):
        out = []
        dupes.parse_asm(SRC_ASM, 'alu', out)
        vals = [(c.bits, c.value) for c in neardupes.consts_of(out)]
        self.assertEqual(vals, [(16, 5), (16, 0x30), (32, 0x80100000)])

    def test_addiu_pair_sign_extends(self):
        out = [(0, w(15, 0, 8, 0x8011)), (1, w(9, 8, 8, 0x8000))]
        self.assertEqual(neardupes.consts_of(out)[0].value, 0x80108000)


class LiteralTests(unittest.TestCase):
    def sub(self, text, sv, tv, bits=16, count=1):
        s = [neardupes.Const(neardupes.OP_ADDIU if bits == 16 else neardupes.OP_LUI, sv, bits, [i]) for i in range(count)]
        t = [neardupes.Const(neardupes.OP_ADDIU if bits == 16 else neardupes.OP_LUI, tv, bits, [i]) for i in range(count)]
        return neardupes.substitute(text, s, t)

    def test_decimal_and_hex_keep_style(self):
        self.assertEqual(self.sub('f(5, 0x30);', 5, 9)[0], 'f(9, 0x30);')
        self.assertEqual(self.sub('f(5, 0x30);', 0x30, 0x2C)[0], 'f(5, 0x2C);')

    def test_negative_and_subtraction(self):
        self.assertEqual(self.sub('x = y - 3;', 0xFFFD, 0xFFFB)[0], 'x = y - 5;')
        self.assertEqual(self.sub('x = -3;', 0xFFFD, 7)[0], 'x = 7;')
        self.assertEqual(self.sub('x = y + 3;', 3, 0xFFFB)[0], 'x = y + -5;')

    def test_subtraction_sign_flip_is_refused(self):
        text, why = self.sub('x = y - 3;', 0xFFFD, 4)
        self.assertIsNone(text)
        self.assertIn('sign flip', why)

    def test_ambiguity_and_missing_literal(self):
        text, why = self.sub('a[5] = 5;', 5, 9)
        self.assertIsNone(text)
        self.assertIn('2 C literals for 1', why)
        text, why = self.sub('f(1);', 5, 9)
        self.assertIsNone(text)
        self.assertIn('no C literal', why)

    def test_same_value_two_targets_is_refused(self):
        s = [neardupes.Const(neardupes.OP_ADDIU, 5, 16, [0]), neardupes.Const(neardupes.OP_ADDIU, 5, 16, [1])]
        t = [neardupes.Const(neardupes.OP_ADDIU, 6, 16, [0]), neardupes.Const(neardupes.OP_ADDIU, 7, 16, [1])]
        text, why = neardupes.substitute('f(5); g(5);', s, t)
        self.assertIsNone(text)
        self.assertIn('maps to different', why)

    def test_same_value_same_target_replaces_all(self):
        self.assertEqual(self.sub('f(5); g(5);', 5, 6, count=2)[0], 'f(6); g(6);')

    def test_identifiers_comments_and_floats_untouched(self):
        t = self.sub('/* 5 */ D_80150005 = 5; x = 1.5;', 5, 6)[0]
        self.assertEqual(t, '/* 5 */ D_80150005 = 6; x = 1.5;')

    def test_32bit_constant(self):
        self.assertEqual(self.sub('v = 0x80100000;', 0x80100000, 0x80110040, 32)[0], 'v = 0x80110040;')


class PlanTests(unittest.TestCase):
    def test_near_group_plan_substitutes_constants(self):
        r = make_repo(SRC_ASM, TGT_ASM)
        try:
            funcs, (plans, skipped, stats, nosrc) = plan(r)
            self.assertEqual(stats['groups'], 1)
            self.assertEqual(len(plans), 1, skipped)
            t = plans[0]['text']
            self.assertIn('void ftgt(void)', t)
            self.assertIn('func_80140000(9, 0x2C);', t)
            self.assertIn('v = 0x80110040;', t)
            self.assertEqual(plans[0]['ndiff'], 3)
            self.assertEqual(plans[0]['bytes'], 0x28)
        finally:
            r.close()

    def test_exact_twin_is_left_to_dupes(self):
        r = make_repo(SRC_ASM, SRC_ASM.replace('fsrc', 'ftgt'))
        try:
            funcs, (plans, skipped, stats, nosrc) = plan(r)
            self.assertEqual((plans, skipped), ([], []))
        finally:
            r.close()

    def test_group_without_matched_member_is_counted(self):
        r = make_repo(SRC_ASM, TGT_ASM)
        try:
            r.write('asm/ovl/AAA/nonmatchings/AAA/fsrc.s', r.read('asm/ovl/AAA/matchings/AAA/fsrc.s'))
            os.remove(os.path.join(r.root, 'asm/ovl/AAA/matchings/AAA/fsrc.s'))
            funcs, (plans, skipped, stats, nosrc) = plan(r)
            self.assertEqual((len(plans), stats['no_src'], stats['nosrc_members']), (0, 1, 2))
            self.assertEqual(stats['nosrc_bytes'], 2 * 0x28)
        finally:
            r.close()

    def test_unresolvable_constant_is_skipped_with_reason(self):
        r = make_repo(SRC_ASM, TGT_ASM, SRC_C.replace('func_80140000(5, 0x30)', 'func_80140000(1 + 4, 0x30)'))
        try:
            funcs, (plans, skipped, stats, nosrc) = plan(r)
            self.assertEqual(plans, [])
            self.assertIn('no C literal', skipped[0][1][0])
        finally:
            r.close()

    def test_apply_writes_substituted_c(self):
        r = make_repo(SRC_ASM, TGT_ASM)
        try:
            funcs, (plans, skipped, stats, nosrc) = plan(r)
            dupes.apply_plan(r.root, plans[0])
            c = r.read('src/ovl/BBB.c')
            self.assertNotIn('INCLUDE_ASM', c)
            self.assertIn('func_80140000(9, 0x2C);', c)
        finally:
            r.close()


if __name__ == '__main__':
    unittest.main()
