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

    def test_main_exe_symbol_is_declared_in_main_api(self):
        # T-3340: a main-exe symbol the target needs goes to include/main_api.h, overlay symbols to the overlay header
        r = make_repo(SRC_C.replace('func_80140000', 'func_80041234'), TGT_C, src_hdr='void func_80041234(void);\n')
        try:
            r.write('asm/ovl/AAA/matchings/AAA/fsrc.s', func_asm('fsrc', 'D_80150000', 'D_80150004', 'func_80041234'))
            r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s', func_asm('ftgt', 'D_80151110', 'D_80151114', 'func_80042222'))
            r.write('include/main_api.h', '#ifndef M\n#define M\n#endif\n')
            funcs = dupes.load_funcs(r.root)
            plans, skipped, stats = dupes.plan_all(r.root, funcs, {'aliases': dupes.load_aliases(r.root)})
            self.assertEqual(len(plans), 1)
            dest = {d[1]: d[0] for d in plans[0]['decls']}
            self.assertEqual(dest['void func_80042222(void);'], 'include/main_api.h')
            self.assertEqual(dest['extern s32 D_80151110;'], 'include/ovl/BBB.h')
            dupes.apply_plan(r.root, plans[0])
            self.assertIn('void func_80042222(void);', r.read('include/main_api.h'))
            self.assertNotIn('func_80042222', r.read('include/ovl/BBB.h'))
        finally:
            r.close()

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


class WaveFourTests(unittest.TestCase):
    """T-7030: the reasons dupes.py and neardupes.py planned 5 and 15 copies of 200 and none built."""

    def repo(self, src_c, tgt_c, src_syms=('D_800E7388', 'D_800E738C'), tgt_syms=('D_800E7388', 'D_800E738C'),
             src_hdr='', tgt_hdr='', callee=('func_80140000', 'func_80140000'), main_api=None, src_head=''):
        r = make_repo(src_c, tgt_c, tgt_hdr, src_hdr)
        self.addCleanup(r.close)
        r.write('asm/ovl/AAA/matchings/AAA/fsrc.s', func_asm('fsrc', src_syms[0], src_syms[1], callee[0]))
        r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s', func_asm('ftgt', tgt_syms[0], tgt_syms[1], callee[1]))
        r.write('config/overlays.txt', 'AAA 0x80132000 0x100\nBBB 0x80132000 0x100\n')
        r.write('include/main_api.h', main_api or ('#ifndef M\n#define M\nextern s32 D_800E7388;\nextern s32 D_800E738C;\n'
                                                  'void func_80140000(void);\n#endif\n'))
        r.write('include/game.h', '#ifndef G\n#define G\n#include "main_api.h"\n#endif\n')
        r.write('include/ovl/AAA.h', '#ifndef A\n#define A\n#include "common.h"\n#include "game.h"\n' + src_hdr + '#endif\n')
        r.write('include/ovl/BBB.h', '#ifndef B\n#define B\n#include "common.h"\n#include "game.h"\n' + tgt_hdr + '#endif\n')
        if src_head:
            r.write('src/ovl/AAA.c', src_head + '\n#include "common.h"\n#include "ovl/AAA.h"\n\n' + src_c)
        return r

    def plan(self, r, **cache):
        funcs = dupes.load_funcs(r.root)
        c = {'aliases': dupes.load_aliases(r.root)}
        c.update(cache)
        plans, skipped, _stats = dupes.plan_all(r.root, funcs, c)
        return plans, skipped

    def test_override_of_the_source_is_copied_to_the_target(self):
        """DATE func_8014F370: the selector is `u32` through MAIN_API_OVERRIDE_; without it the target
        compiled the `s32` view of main_api.h and the switch used $v0 instead of $v1."""
        src = ('void fsrc(void) {\n    if (D_800E7388 != 0) {\n        func_80140000();\n    }\n'
               '    D_800E738C = 0;\n}\n')
        r = self.repo(src, TGT_C, src_hdr='', src_head='#define MAIN_API_OVERRIDE_D_800E7388 /* switched as u32 */')
        r.write('src/ovl/AAA.c', '#define MAIN_API_OVERRIDE_D_800E7388 /* switched as u32 */\n#include "common.h"\n'
                '#include "ovl/AAA.h"\n\nextern u32 D_800E7388;\n\n' + src)
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        local = plans[0]['local']
        self.assertIn(('define', '#define MAIN_API_OVERRIDE_D_800E7388 /* switched as u32 */'), local)
        self.assertIn(('block', 'extern u32 D_800E7388;'), local)
        dupes.apply_plan(r.root, plans[0])
        t = r.read('src/ovl/BBB.c')
        self.assertLess(t.index('MAIN_API_OVERRIDE_D_800E7388'), t.index('#include "common.h"'))
        self.assertLess(t.index('#include "ovl/BBB.h"'), t.index('extern u32 D_800E7388;'))
        self.assertLess(t.index('extern u32 D_800E7388;'), t.index('void ftgt'))

    def test_file_local_struct_typedef_moves_with_the_body(self):
        src = ('typedef struct {\n    void (*f[3])();\n} FnTbl3; /* size 0xC */\nextern FnTbl3 D_800E738C;\n\n'
               'void fsrc(void) {\n    FnTbl3 tbl;\n\n    tbl = D_800E738C;\n    tbl.f[0]();\n'
               '    func_80140000();\n    D_800E7388 = 0;\n}\n')
        r = self.repo(src, TGT_C)
        r.write('src/ovl/AAA.c', '#include "common.h"\n#include "ovl/AAA.h"\n\n' + src)
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        dupes.apply_plan(r.root, plans[0])
        t = r.read('src/ovl/BBB.c')
        self.assertIn('} FnTbl3; /* size 0xC */', t)
        self.assertLess(t.index('FnTbl3;'), t.index('void ftgt'))
        self.assertIn('FnTbl3 tbl;', t)

    def test_address_literal_follows_the_relocation(self):
        """`*(s16 *)0x801F0D14` is `%lo(D_801F0D14)`: the target's address replaces it."""
        src = 'void fsrc(void) {\n    if (*(s16 *)0x801F0D14 != 0) {\n        func_80140000();\n    }\n    D_800E738C = 0;\n}\n'
        r = self.repo(src, TGT_C)
        r.write('asm/ovl/AAA/matchings/AAA/fsrc.s', func_asm('fsrc', 'D_801F0D14', 'D_800E738C', 'func_80140000'))
        r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s', func_asm('ftgt', 'D_801EB6E0', 'D_800E738C', 'func_80140000'))
        r.write('include/main_api.h', '#ifndef M\n#define M\nextern s32 D_800E738C;\nvoid func_80140000(void);\n#endif\n')
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertIn('0x801EB6E0', plans[0]['text'])
        self.assertNotIn('0x801F0D14', plans[0]['text'])

    def test_target_without_a_prototype_gets_one_so_an_earlier_call_is_not_implicit_int(self):
        """TEL func_801361CC: the dispatcher above calls it, the definition is `void`, the implicit `int`
        declaration of the call clashed with it (cfe: redeclaration)."""
        r = self.repo(SRC_C.replace('D_80150000', 'D_800E7388').replace('D_80150004', 'D_800E738C'), TGT_C)
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertIn(('include/ovl/BBB.h', 'void ftgt(void);'), plans[0]['decls'])

    def test_callee_defined_below_in_the_target_file_gets_a_prototype_in_the_header(self):
        """GEKO func_8013B9F0: the copy calls a function the .c defines further down; without a prototype
        the call is an implicit `int f()` and the later `void f(void)` is a redeclaration."""
        src = ('void helper(void) {\n}\n\n' + SRC_C.replace('D_80150000', 'D_800E7388').replace('D_80150004', 'D_800E738C')
               .replace('func_80140000();', 'helper();'))
        r = self.repo(src, TGT_C + '\nvoid helper2(void) {\n}\n', callee=('helper', 'helper2'),
                      main_api='#ifndef M\n#define M\nextern s32 D_800E7388;\nextern s32 D_800E738C;\n#endif\n',
                      src_hdr='void helper(void);\n')
        r.write('asm/ovl/AAA/matchings/AAA/fsrc.s', func_asm('fsrc', 'D_800E7388', 'D_800E738C', 'helper'))
        r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s', func_asm('ftgt', 'D_800E7388', 'D_800E738C', 'helper2'))
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertIn(('include/ovl/BBB.h', 'void helper2(void);'), plans[0]['decls'])

    def test_void_prototype_of_the_target_learns_the_return_type_of_the_matched_twin(self):
        """main uwasa0: the callers said void, the twin is non-void (`or v0,v1,zero` in the delay slot)."""
        r = self.repo(SRC_C.replace('D_80150000', 'D_800E7388').replace('D_80150004', 'D_800E738C')
                      .replace('void fsrc', 's32 fsrc'), TGT_C, tgt_hdr='void ftgt(void);\n')
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertTrue(plans[0]['text'].startswith('s32 ftgt(void) {'))
        self.assertEqual(plans[0]['retype'], [('include/ovl/BBB.h', 'ftgt', 'void', 's32')])
        dupes.apply_plan(r.root, plans[0])
        self.assertIn('s32 ftgt(void);', r.read('include/ovl/BBB.h'))
        self.assertNotIn('void ftgt', r.read('include/ovl/BBB.h'))

    def test_declaration_with_a_type_only_the_c_files_define_stays_in_the_c_file(self):
        """GYOZI func_8013B3B4: `extern FnTbl20 D_...;` cannot go to the header, FnTbl20 is a typedef of the .c."""
        src = ('typedef struct {\n    void (*f[2])();\n} FnTbl2;\nextern FnTbl2 D_80150000;\n\n'
               'void fsrc(void) {\n    FnTbl2 t;\n\n    t = D_80150000;\n    t.f[0]();\n    func_80140000();\n    D_800E738C = 0;\n}\n')
        r = self.repo(src, 'typedef struct {\n    void (*f[2])();\n} FnTbl2;\n\n' + TGT_C,
                      src_syms=('D_80150000', 'D_800E738C'), tgt_syms=('D_80151110', 'D_800E738C'))
        r.write('src/ovl/AAA.c', '#include "common.h"\n#include "ovl/AAA.h"\n\n' + src)
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertEqual([d for d in plans[0]['decls'] if 'FnTbl2' in d[1]], [])
        self.assertIn(('late', 'extern FnTbl2 D_80151110;'), plans[0]['local'])
        dupes.apply_plan(r.root, plans[0])
        t = r.read('src/ovl/BBB.c')
        self.assertLess(t.index('} FnTbl2;'), t.index('extern FnTbl2 D_80151110;'))
        self.assertLess(t.index('extern FnTbl2 D_80151110;'), t.index('void ftgt'))

    def test_return_type_follows_the_target_prototype(self):
        r = self.repo(SRC_C.replace('D_80150000', 'D_800E7388').replace('D_80150004', 'D_800E738C'), TGT_C,
                      tgt_hdr='s32 ftgt(void);\n')
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertTrue(plans[0]['text'].startswith('s32 ftgt(void) {'), plans[0]['text'])

    def test_callee_defined_in_the_target_file_with_another_return_type_keeps_the_targets_signature(self):
        src = ('s32 helper(void) {\n}\n\n' + SRC_C.replace('D_80150000', 'D_800E7388').replace('D_80150004', 'D_800E738C')
               .replace('func_80140000();', 'helper();'))
        r = self.repo(src, 'void helper2(void) {\n}\n\n' + TGT_C, callee=('helper', 'helper2'),
                      main_api='#ifndef M\n#define M\nextern s32 D_800E7388;\nextern s32 D_800E738C;\n#endif\n',
                      src_hdr='s32 helper(void);\n')
        r.write('asm/ovl/AAA/matchings/AAA/fsrc.s', func_asm('fsrc', 'D_800E7388', 'D_800E738C', 'helper'))
        r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s', func_asm('ftgt', 'D_800E7388', 'D_800E738C', 'helper2'))
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        # the header gets the target's own signature (void), never the source twin's s32
        self.assertEqual([d for d in plans[0]['decls'] if 'ftgt' not in d[1]], [('include/ovl/BBB.h', 'void helper2(void);')])

    def test_result_used_against_a_void_target_prototype_is_a_conflict(self):
        """GYOZI func_8013FD9C: `if (f(...) == 1)` against the target's `void f()` does not compile."""
        src = ('void fsrc(void) {\n    if (func_80140000() == 1) {\n        D_800E738C = 0;\n    }\n    D_800E7388 = 0;\n}\n')
        r = self.repo(src, TGT_C, tgt_hdr='void func_80140000();\n', src_hdr='s32 func_80140000(void);\n',
                      main_api='#ifndef M\n#define M\nextern s32 D_800E7388;\nextern s32 D_800E738C;\n#endif\n')
        plans, skipped = self.plan(r)
        self.assertEqual(plans, [])
        self.assertIn('declares func_80140000 differently', skipped[0][1][0])
        self.assertFalse(dupes.value_used('    f(1);\n    g(2);\n', 'f'))
        self.assertTrue(dupes.value_used('    return f(1);\n', 'f'))
        self.assertTrue(dupes.value_used('    x = f(1);\n', 'f'))

    def test_two_game_state_fields_against_plain_globals_are_skipped(self):
        r = self.gs_repo(('D_800E6284', 'D_800E6288'), ('D_800F647A', 'D_800F647B'),
                         '    D_800E6280.unk_04 = D_800E6280.unk_08;\n')
        plans, skipped = self.plan(r)
        self.assertEqual(plans, [])
        self.assertIn('2 fields of the aggregate map to plain globals', skipped[0][1][0])

    def test_kr_and_void_prototypes_are_the_same_view(self):
        r = self.repo(SRC_C.replace('D_80150000', 'D_800E7388').replace('D_80150004', 'D_800E738C'), TGT_C,
                      tgt_hdr='void func_80140000();\n')
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)

    def test_changed_data_type_is_still_a_conflict(self):
        r = self.repo(SRC_C.replace('D_80150000', 'D_800E7388').replace('D_80150004', 'D_800E738C'), TGT_C,
                      tgt_hdr='extern u8 D_800E7388;\n', src_hdr='extern s32 D_800E7388;\n',
                      main_api='#ifndef M\n#define M\nextern s32 D_800E738C;\nvoid func_80140000(void);\n#endif\n')
        plans, skipped = self.plan(r)
        self.assertEqual(plans, [])
        self.assertIn('declares D_800E7388 differently', skipped[0][1][0])

    GS = ('#ifndef M\n#define M\ntypedef struct GameState {\n    /* 0x00 */ s32 unk_00;\n    /* 0x04 */ u8 unk_04;\n'
          '    /* 0x05 */ u8 pad_05[3];\n    /* 0x08 */ u8 unk_08;\n    /* 0x09 */ u8 pad_09[3];\n    /* 0x0C */ u8 unk_0C[4];\n} GameState; /* size 0x10 */\nextern GameState D_800E6280;\n'
          'extern u8 D_800F647A;\nvoid func_80140000(void);\n#endif\n')

    def gs_repo(self, src_syms, tgt_syms, body, tgt_main=None):
        r = self.repo('void fsrc(void) {\n' + body + '    func_80140000();\n}\n', TGT_C, src_syms=src_syms, tgt_syms=tgt_syms,
                      main_api=tgt_main or self.GS)
        r.write('config/migrate_globals.txt', 'aggregate D_800E6280 GameState include/main_api.h\n')
        return r

    def test_game_state_field_in_the_source_is_kept_for_the_same_address(self):
        r = self.gs_repo(('D_800E6284', 'D_800E6284'), ('D_800E6284', 'D_800E6284'),
                         '    D_800E6280.unk_04 = 1;\n    D_800E6280.unk_04 += 2;\n')
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertIn('D_800E6280.unk_04 = 1;', plans[0]['text'])
        self.assertEqual([d for d in plans[0]['decls'] if 'ftgt' not in d[1]], [])

    def test_game_state_field_maps_to_the_target_field(self):
        r = self.gs_repo(('D_800E6284', 'D_800E6288'), ('D_800E6288', 'D_800E6284'),
                         '    D_800E6280.unk_04 = D_800E6280.unk_08;\n')
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertIn('D_800E6280.unk_08 = D_800E6280.unk_04;', plans[0]['text'])

    def test_game_state_field_and_a_plain_global_of_the_target(self):
        r = self.gs_repo(('D_800E6284', 'D_800E6288'), ('D_800F647A', 'D_800E6288'),
                         '    D_800E6280.unk_04 = D_800E6280.unk_08;\n')
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertIn('D_800F647A = D_800E6280.unk_08;', plans[0]['text'])
        self.assertEqual([d for d in plans[0]['decls'] if 'ftgt' not in d[1]], [])      # D_800F647A is declared (main_api.h)

    def test_struct_array_element_against_a_plain_global_is_skipped(self):
        r = self.gs_repo(('D_800E628C', 'D_800E628C'), ('D_800F647A', 'D_800F647A'), '    D_800E6280.unk_0C[0] = 1;\n')
        plans, skipped = self.plan(r)
        self.assertEqual(plans, [])
        self.assertIn('struct array', skipped[0][1][0])

    def test_game_state_field_without_a_counterpart_is_a_clear_reason(self):
        r = self.gs_repo(('D_800E6284', 'D_800E6284'), ('D_800E6284', 'D_800E6284'), '    D_800E6280.unk_08 = 1;\n')
        plans, skipped = self.plan(r)
        self.assertEqual(plans, [])
        self.assertIn('D_800E6280.unk_08 (0x800E6288) which is not in the relocation sequence', skipped[0][1][0])

    def test_plain_source_global_and_a_target_game_state_field(self):
        r = self.repo('void fsrc(void) {\n    D_800F647A = 1;\n    func_80140000();\n    D_800E738C = 0;\n}\n', TGT_C,
                      src_syms=('D_800F647A', 'D_800E738C'), tgt_syms=('D_800E6284', 'D_800E738C'), main_api=self.GS.replace(
                          'void func_80140000(void);', 'extern s32 D_800E738C;\nvoid func_80140000(void);'))
        r.write('config/migrate_globals.txt', 'aggregate D_800E6280 GameState include/main_api.h\n')
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertIn('D_800E6280.unk_04 = 1;', plans[0]['text'])


class FilesFilterTests(unittest.TestCase):
    """T-9030: --files limits which C files --apply may edit (parallel batch agents)."""
    PLANS = [dict(tgtfile='src/main/80041000.c', name='a'), dict(tgtfile='src/ovl/TEL.c', name='b'),
             dict(tgtfile='src/ovl/DATE.c', name='c')]

    def test_empty_spec_keeps_everything(self):
        self.assertTrue(all(dupes.wanted_file(p['tgtfile'], None) for p in self.PLANS))

    def test_stems_and_prefixes_case_insensitive(self):
        self.assertTrue(dupes.wanted_file('src/main/80041000.c', '8004'))
        self.assertTrue(dupes.wanted_file('src/ovl/TEL.c', 'tel'))
        self.assertFalse(dupes.wanted_file('src/ovl/DATE.c', 'TEL,80041000'))

    def test_restrict_files_filters_plans_and_skipped(self):
        class T:
            def __init__(self, path):
                self.path = path
        skipped = [(T('x/TEL.c'), ['r']), (T('x/DATE.c'), ['r'])]
        plans, sk = dupes.restrict_files(self.PLANS, skipped, 'TEL,8004', lambda t: t.path)
        self.assertEqual([p['name'] for p in plans], ['a', 'b'])
        self.assertEqual(len(sk), 1)


class ApplyAllTests(unittest.TestCase):
    def test_a_rejected_batch_restores_every_header(self):
        r = make_repo(SRC_C, TGT_C)
        self.addCleanup(r.close)
        r.write('include/main_api.h', '#ifndef M\n#define M\n#endif\n')
        funcs = dupes.load_funcs(r.root)
        plans, _s, _st = dupes.plan_all(r.root, funcs, {'aliases': dupes.load_aliases(r.root)})
        before = dupes.snapshot_include(r.root)
        calls = []
        old_build, old_sync = dupes.build_ok, dupes.sync_headers
        dupes.build_ok = lambda root, unit: (calls.append(unit) or False, 'boom')
        dupes.sync_headers = lambda root, ps, log=print, before=None: (r.write('include/ovl/NEW.h', 'x') or None)
        try:
            kept = dupes.apply_all(r.root, plans, True, False, log=lambda m: None)
        finally:
            dupes.build_ok, dupes.sync_headers = old_build, old_sync
        self.assertEqual(kept, [])
        self.assertEqual(dupes.snapshot_include(r.root), before)      # NEW.h is gone, edits undone
        self.assertIn('INCLUDE_ASM', r.read('src/ovl/BBB.c'))

    def test_add_local_puts_defines_first_and_blocks_after_the_includes(self):
        text = '#include "common.h"\n#include "ovl/X.h"\n\nvoid f(void) {\n}\n'
        out = dupes.add_local(text, [('define', '#define MAIN_API_OVERRIDE_D_1 /* r */'), ('block', 'extern u32 D_1;')])
        self.assertEqual(out, '#define MAIN_API_OVERRIDE_D_1 /* r */\n#include "common.h"\n#include "ovl/X.h"\n\n'
                              'extern u32 D_1;\n\nvoid f(void) {\n}\n')
        self.assertEqual(dupes.add_local(out, [('define', '#define MAIN_API_OVERRIDE_D_1 /* r */'),
                                               ('block', 'extern u32 D_1;')]), out)


if __name__ == '__main__':
    unittest.main()
