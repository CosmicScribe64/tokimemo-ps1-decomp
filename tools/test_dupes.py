#!/usr/bin/env python3
"""Unit tests for tools/dupes.py on synthetic asm and C (no game data).

Run: tools/docker.sh python3 tools/test_dupes.py
"""
import os
import shutil
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dupes  # noqa: E402


def insn(off, vram, word, text):
    return '    /* %X %08X %s */  %s\n' % (off, vram, word, text)


def func_asm(name, sym_a, sym_b, callee, regs=('t6', 'at')):
    """lui/lw/lui/sw + jal; the hex words use fixed immediates (relocs are masked)."""
    return (
        'nonmatching %s, 0x1C\n\nglabel %s\n' % (name, name)
        + insn(0, 0x80132000, '16800E3C', 'lui        $%s, %%hi(%s)' % (regs[0], sym_a))
        + insn(4, 0x80132004, 'A48ACE8D', 'lw         $t6, %%lo(%s)($t6)' % sym_a)
        + insn(8, 0x80132008, '0000C011', 'beqz       $t6, .L80132020')
        + insn(0xC, 0x8013200C, '9834050C', 'jal        %s' % callee)
        + insn(0x10, 0x80132010, '00000000', 'nop')
        + insn(0x14, 0x80132014, '1580013C', 'lui        $at, %%hi(%s)' % sym_b)
        + '.L80132020:\n'
        + insn(0x18, 0x80132018, '0800E003', 'jr         $ra')
        + insn(0x1C, 0x8013201C, '905E20AC', '   sw        $zero, %%lo(%s)($at)' % sym_b)
        + 'endlabel %s\n' % name)


class Repo:
    def __init__(self):
        self.root = tempfile.mkdtemp()

    def write(self, rel, text):
        p = os.path.join(self.root, rel)
        os.makedirs(os.path.dirname(p), exist_ok=True)
        with open(p, 'w') as fh:
            fh.write(text)

    def read(self, rel):
        with open(os.path.join(self.root, rel)) as fh:
            return fh.read()

    def close(self):
        shutil.rmtree(self.root)


class FingerprintTests(unittest.TestCase):
    def test_reloc_symbols_do_not_change_key(self):
        k1, r1, b1 = dupes.parse_asm(func_asm('f', 'D_80150000', 'D_80150004', 'func_80140000'))
        k2, r2, b2 = dupes.parse_asm(func_asm('g', 'D_80151110', 'D_80151114', 'func_80142222'))
        self.assertEqual(k1, k2)
        self.assertIsNone(b1)
        self.assertEqual([r[1] for r in r1], ['D_80150000', 'D_80150000', 'func_80140000', 'D_80150004', 'D_80150004'])
        self.assertEqual([r[1] for r in r2][0], 'D_80151110')

    def test_register_change_changes_key(self):
        k1, _, _ = dupes.parse_asm(func_asm('f', 'D_80150000', 'D_80150004', 'func_80140000'))
        text = func_asm('f', 'D_80150000', 'D_80150004', 'func_80140000').replace('16800E3C', '16800F3C')
        k2, _, _ = dupes.parse_asm(text)
        self.assertNotEqual(k1, k2)

    def test_literal_address_is_not_masked(self):
        a = func_asm('f', 'D_80150000', 'D_80150004', 'func_80140000')
        lit = a.replace('lui        $t6, %hi(D_80150000)', 'lui        $t6, (0x801E8A20 >> 16)')
        self.assertNotEqual(dupes.parse_asm(a)[0], dupes.parse_asm(lit)[0])

    def test_local_branch_label_keeps_offset(self):
        a = func_asm('f', 'D_80150000', 'D_80150004', 'func_80140000')
        b = a.replace('0000C011', '0100C011')
        self.assertNotEqual(dupes.parse_asm(a)[0], dupes.parse_asm(b)[0])

    def test_jump_table_and_data_are_excluded(self):
        a = func_asm('f', 'jtbl_80150000', 'D_80150004', 'func_80140000')
        self.assertIsNotNone(dupes.parse_asm(a)[2])
        b = func_asm('f', 'D_80150000', 'D_80150004', 'func_80140000') + '    .word 0x1234\n'
        self.assertIsNotNone(dupes.parse_asm(b)[2])


def make_repo(src_c, tgt_c, tgt_hdr='', src_hdr=''):
    r = Repo()
    r.write('asm/ovl/AAA/matchings/AAA/fsrc.s', func_asm('fsrc', 'D_80150000', 'D_80150004', 'func_80140000'))
    r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s', func_asm('ftgt', 'D_80151110', 'D_80151114', 'func_80142222'))
    r.write('src/ovl/AAA.c', '#include "common.h"\n#include "ovl/AAA.h"\n\n' + src_c)
    r.write('src/ovl/BBB.c', '#include "common.h"\n#include "ovl/BBB.h"\n\n' + tgt_c)
    r.write('include/ovl/AAA.h', '#ifndef A\n#define A\n#include "common.h"\n#include "game.h"\n'
            'extern s32 D_80150000;\nextern s32 D_80150004;\nvoid func_80140000(void);\n' + src_hdr + '#endif\n')
    r.write('include/ovl/BBB.h', '#ifndef B\n#define B\n#include "common.h"\n#include "game.h"\n' + tgt_hdr + '#endif\n')
    r.write('include/game.h', '#ifndef G\n#define G\n#endif\n')
    r.write('include/common.h', '')
    return r


SRC_C = ('void fsrc(void) {\n    if (D_80150000 != 0) {\n        func_80140000();\n    }\n'
         '    D_80150004 = 0;\n}\n')
TGT_C = 'INCLUDE_ASM("asm/ovl/BBB/nonmatchings/BBB", ftgt);\n'


class PlanTests(unittest.TestCase):
    def setUp(self):
        self.r = make_repo(SRC_C, TGT_C)
        self.cache = {'aliases': dupes.load_aliases(self.r.root)}

    def tearDown(self):
        self.r.close()

    def plan(self):
        funcs = dupes.load_funcs(self.r.root)
        plans, skipped, stats = dupes.plan_all(self.r.root, funcs, self.cache)
        return funcs, plans, skipped, stats

    def test_group_found_and_symbols_translated(self):
        funcs, plans, skipped, stats = self.plan()
        self.assertEqual(stats['groups'], 1)
        self.assertEqual(len(plans), 1)
        t = plans[0]['text']
        self.assertIn('void ftgt(void)', t)
        self.assertIn('D_80151110', t)
        self.assertIn('func_80142222();', t)
        self.assertNotIn('D_80150000', t)
        decls = [d[1] for d in plans[0]['decls']]
        self.assertIn('extern s32 D_80151110;', decls)
        self.assertIn('void func_80142222(void);', decls)
        self.assertTrue(all(d[0] == 'include/ovl/BBB.h' for d in plans[0]['decls']))

    def test_apply_replaces_include_asm_and_rolls_back(self):
        funcs, plans, skipped, stats = self.plan()
        saved = dupes.apply_plan(self.r.root, plans[0])
        c = self.r.read('src/ovl/BBB.c')
        self.assertNotIn('INCLUDE_ASM', c)
        self.assertIn('void ftgt(void) {', c)
        self.assertIn('extern s32 D_80151114;', self.r.read('include/ovl/BBB.h'))
        dupes.restore(self.r.root, saved)
        self.assertIn('INCLUDE_ASM', self.r.read('src/ovl/BBB.c'))
        self.assertNotIn('D_80151114', self.r.read('include/ovl/BBB.h'))

    def test_shared_main_symbol_stays_and_is_declared_once(self):
        # callee is the same main-exe function in both; it keeps its name
        self.r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s',
                     func_asm('ftgt', 'D_80151110', 'D_80151114', 'func_80140000'))
        self.r.write('config/symbol_addrs.txt', 'func_80140000 = 0x80140000;\n')
        self.cache['aliases'] = dupes.load_aliases(self.r.root)
        # 0x80140000 is above the overlay floor: it is overlay-space, identity by name
        funcs, plans, skipped, stats = self.plan()
        self.assertEqual(len(plans), 1)
        self.assertIn('func_80140000();', plans[0]['text'])

    def test_string_literal_is_skipped(self):
        r = make_repo(SRC_C.replace('D_80150004 = 0;', 'puts("x");'), TGT_C)
        try:
            funcs = dupes.load_funcs(r.root)
            plans, skipped, stats = dupes.plan_all(r.root, funcs, {'aliases': dupes.load_aliases(r.root)})
            self.assertEqual(plans, [])
            self.assertEqual(len(skipped), 1)
        finally:
            r.close()

    def test_conflicting_target_declaration_is_skipped(self):
        r = make_repo(SRC_C, TGT_C, tgt_hdr='extern u8 D_80151110;\n')
        try:
            funcs = dupes.load_funcs(r.root)
            plans, skipped, stats = dupes.plan_all(r.root, funcs, {'aliases': dupes.load_aliases(r.root)})
            self.assertEqual(plans, [])
            self.assertIn('declares D_80151110 differently', skipped[0][1][0])
            plans, skipped, stats = dupes.plan_all(r.root, funcs, {'aliases': dupes.load_aliases(r.root), 'lenient': True})
            self.assertEqual(len(plans), 1)
        finally:
            r.close()

    def test_non_bijective_mapping_is_skipped(self):
        # target uses one symbol where the source uses two
        r = make_repo(SRC_C, TGT_C)
        try:
            r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s',
                    func_asm('ftgt', 'D_80151110', 'D_80151110', 'func_80142222'))
            funcs = dupes.load_funcs(r.root)
            plans, skipped, stats = dupes.plan_all(r.root, funcs, {'aliases': dupes.load_aliases(r.root)})
            self.assertEqual(plans, [])
            self.assertIn('one-to-one', skipped[0][1][0])
        finally:
            r.close()

    def test_alias_names_resolve_to_one_symbol(self):
        al = dupes.load_aliases(self.r.root)
        self.assertEqual(dupes.sym_key('func_80042000', al), ('a', 0x80042000))
        self.assertEqual(dupes.sym_key('D_80150000', al), ('n', 'D_80150000'))

    def test_multi_declarator_line(self):
        self.assertEqual(dupes.find_decl(['extern s32 A, B, C;\n'], 'B'), 'extern s32 B;')
        self.assertEqual(dupes.find_decl(['void f(s32 a);\n'], 'f'), 'void f(s32 a);')


if __name__ == '__main__':
    unittest.main()
