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
    def test_source_state_overrides_stale_folders_and_deduplicates(self):
        r = Repo()
        self.addCleanup(r.close)
        r.write('src/main/80041000.c',
                'void f(void) { }\n#ifdef NON_MATCHING\nvoid g(void) { }\n#else\n'
                'INCLUDE_ASM("asm/nonmatchings/main/80041000", g);\n#endif\n')
        for folder in ('matchings', 'nonmatchings'):
            for name in ('f', 'g', 'old'):
                r.write('asm/%s/main/80041000/%s.s' % (folder, name),
                        func_asm(name, 'D_80150000', 'D_80150004', 'callee'))
        funcs = dupes.load_funcs(r.root)
        self.assertEqual({f.name: f.matched for f in funcs}, {'f': True, 'g': False})
        self.assertEqual(len(funcs), 2)
        self.assertIn('/matchings/', next(f.path for f in funcs if f.name == 'f'))
        self.assertIn('/nonmatchings/', next(f.path for f in funcs if f.name == 'g'))

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

    def test_jump_table_without_its_data_and_plain_data_are_excluded(self):
        a = func_asm('f', 'jtbl_80150000', 'D_80150004', 'func_80140000')
        self.assertIsNotNone(dupes.parse_asm(a)[2])
        b = func_asm('f', 'D_80150000', 'D_80150004', 'func_80140000') + '    .word 0x1234\n'
        self.assertIsNotNone(dupes.parse_asm(b)[2])


def le(word):
    return word.to_bytes(4, 'little').hex().upper()


def jtbl_func_asm(name, base, jtbl, cases=(0x20, 0x24, 0x28, 0x20), slti=4, tail=True):
    """A four-case switch at `base`: sltiu/beqz guard, %hi/%lo(jtbl) load, jr, a `j .L` to the end.
    tail: append the table as rodata_pieces.py does (.late_rodata after its mark). Returns (asm, table)."""
    def lab(off):
        return '.L%08X' % (base + off)
    out = 'nonmatching %s, 0x30\n\nglabel %s\n' % (name, name)
    rows = [('%s' % le(0x2C810000 | slti), 'sltiu      $at, $a0, %d' % slti),
            ('06002010', 'beqz       $at, %s' % lab(0x24)),
            ('80700400', 'sll        $t6, $a0, 2'),
            ('1580013C', 'lui        $at, %%hi(%s)' % jtbl),
            ('21082E00', 'addu       $at, $at, $t6'),
            ('A48A2E8C', 'lw         $t6, %%lo(%s)($at)' % jtbl),
            ('0800C001', 'jr         $t6'),
            ('00000000', ' nop'),
            (le(0x08000000 | ((base + 0x28) >> 2) & 0x03FFFFFF), 'j          %s' % lab(0x28)),
            ('00000000', ' nop'),
            ('0800E003', 'jr         $ra'),
            ('00000000', ' nop')]
    for k, (word, text) in enumerate(rows):
        if k * 4 in (0x20, 0x24, 0x28):
            out += '%s:\n' % lab(k * 4)
        out += insn(k * 4, base + k * 4, word, text)
    out += 'endlabel %s\n' % name
    table = 'dlabel %s\n' % jtbl + ''.join(
        '    /* %X %08X %s */ .word %s\n' % (k * 4, 0x80150000 + k * 4, le(base + c), lab(c))
        for k, c in enumerate(cases)) + 'enddlabel %s\n' % jtbl
    if tail:
        out += '\n%s\n.section .late_rodata\n\n%s' % (dupes.RODATA_MARK + ' (rodata_pieces.py)', table)
    return out, table


class JumpTableTests(unittest.TestCase):
    """T-11150: twins with jump tables and local labels."""

    def parse(self, text, tables=None):
        info = {}
        key, relocs, bad = dupes.parse_asm(text, info=info, tables=tables)
        return key, relocs, bad, info

    def test_twins_at_different_addresses_compare_equal(self):
        a, _ = jtbl_func_asm('f', 0x80132000, 'jtbl_80150000')
        b, _ = jtbl_func_asm('g', 0x80134560, 'jtbl_80151230')
        ka, ra, bada, ia = self.parse(a)
        kb, rb, badb, ib = self.parse(b)
        self.assertIsNone(bada)
        self.assertIsNone(badb)
        self.assertEqual(ka, kb)
        self.assertEqual(ra, [])                       # tables and labels are not C symbols
        self.assertEqual(ia['jtbls'], ['jtbl_80150000'])
        self.assertEqual(ia['jtbl_own'], ['jtbl_80150000'])

    def test_different_table_contents_differ(self):
        a, _ = jtbl_func_asm('f', 0x80132000, 'jtbl_80150000')
        b, _ = jtbl_func_asm('g', 0x80134560, 'jtbl_80151230', cases=(0x24, 0x20, 0x28, 0x20))
        self.assertNotEqual(self.parse(a)[0], self.parse(b)[0])
        c, _ = jtbl_func_asm('g', 0x80134560, 'jtbl_80151230', cases=(0x20, 0x24, 0x28))
        self.assertNotEqual(self.parse(a)[0], self.parse(c)[0])

    def test_table_from_the_unit_data_files(self):
        a, _ = jtbl_func_asm('f', 0x80132000, 'jtbl_80150000')
        b, table = jtbl_func_asm('g', 0x80134560, 'jtbl_80151230', tail=False)
        self.assertEqual(self.parse(b)[2], 'jump table data not found')
        kb, _, badb, ib = self.parse(b, dupes.jtbl_blocks(table))
        self.assertIsNone(badb)
        self.assertEqual(kb, self.parse(a)[0])
        self.assertEqual(ib['jtbl_own'], [])

    def test_table_padding_is_not_part_of_the_table(self):
        _, table = jtbl_func_asm('f', 0x80132000, 'jtbl_80150000')
        padded = table.replace('enddlabel', '    .word 0x00000000\nenddlabel')
        self.assertEqual(dupes.jtbl_blocks(padded), dupes.jtbl_blocks(table))
        self.assertEqual(len(dupes.jtbl_blocks(table)['jtbl_80150000']), 4)

    def test_table_target_outside_the_function_is_refused(self):
        a, _ = jtbl_func_asm('f', 0x80132000, 'jtbl_80150000', cases=(0x20, 0x24, 0x40, 0x20))
        self.assertEqual(self.parse(a)[2], 'jump table targets outside the function')

    def test_local_labels_compare_by_relative_offset(self):
        a, _ = jtbl_func_asm('f', 0x80132000, 'jtbl_80150000')
        b, _ = jtbl_func_asm('g', 0x80134560, 'jtbl_80151230')
        # the `j .L` words differ (absolute targets) and still compare equal: checked above; a jump
        # to another relative offset with the same word bits does not
        moved = b.replace('j          .L80134588', 'j          .L80134584')
        self.assertNotEqual(self.parse(a)[0], self.parse(moved)[0])
        self.assertIsNone(self.parse(moved)[2])

    def test_string_rodata_after_the_mark_is_part_of_the_shape(self):
        a, _ = jtbl_func_asm('f', 0x80132000, 'jtbl_80150000')
        b = a + '.section .rodata\n\ndlabel D_80150100\n    .asciz "x"\nenddlabel D_80150100\n'
        kb, _, badb, ib = self.parse(b)
        self.assertIsNone(badb)                        # T-11160: no longer "data in function"
        self.assertEqual(ib['datas'], ['D_80150100'])
        self.assertNotEqual(kb, self.parse(a)[0])


def data_func_asm(name, base, sym, data='    .asciz "abc"\n    .balign 4\n', tail=True, callee='func_80140010'):
    """`callee(&sym)`: lui/addiu of a string, jal, return. tail: append the string block as
    rodata_pieces.py does (.rodata after its mark). Returns (asm, block)."""
    out = 'nonmatching %s, 0x18\n\nglabel %s\n' % (name, name)
    rows = [('1580043C', 'lui        $a0, %%hi(%s)' % sym),
            ('A48A8424', 'addiu      $a0, $a0, %%lo(%s)' % sym),
            ('9834050C', 'jal        %s' % callee),
            ('00000000', ' nop'),
            ('0800E003', 'jr         $ra'),
            ('00000000', ' nop')]
    for k, (word, text) in enumerate(rows):
        out += insn(k * 4, base + k * 4, word, text)
    out += 'endlabel %s\n' % name
    block = 'dlabel %s\n' % sym + ''.join('    /* 0 80150100 */ %s\n' % l.strip() for l in data.splitlines()) \
        + 'enddlabel %s\n' % sym
    if tail:
        out += '\n%s\n.section .rodata\n\n%s' % (dupes.RODATA_MARK + ' (rodata_pieces.py)', block)
    return out, block


class DataTests(unittest.TestCase):
    """T-11160: twins with their own rodata."""

    def parse(self, text, rodata=None):
        info = {}
        key, relocs, bad = dupes.parse_asm(text, info=info, rodata=rodata)
        return key, relocs, bad, info

    def test_twins_with_equal_strings_compare_equal(self):
        a, _ = data_func_asm('f', 0x80132000, 'D_80150100')
        b, _ = data_func_asm('g', 0x80134560, 'D_80151230', callee='func_80142222')
        ka, ra, bada, ia = self.parse(a)
        kb, rb, badb, ib = self.parse(b)
        self.assertIsNone(bada)
        self.assertEqual(ka, kb)
        self.assertEqual([r[1] for r in ra], ['func_80140010'])     # own data is not a C symbol
        self.assertEqual(ia['datas'], ['D_80150100'])
        self.assertEqual(ia['data_own'], ['D_80150100'])
        self.assertFalse(ia['data_ptr'])

    def test_different_data_differs(self):
        a, _ = data_func_asm('f', 0x80132000, 'D_80150100')
        b, _ = data_func_asm('g', 0x80134560, 'D_80151230', data='    .asciz "abd"\n    .balign 4\n')
        self.assertNotEqual(self.parse(a)[0], self.parse(b)[0])
        c, _ = data_func_asm('g', 0x80134560, 'D_80151230', data='    .asciz "abc"\n')
        self.assertNotEqual(self.parse(a)[0], self.parse(c)[0])

    def test_data_from_the_island_file_and_shift_jis(self):
        a, _ = data_func_asm('f', 0x80132000, 'D_80150100', data='    .asciz "あ"\n')
        b, block = data_func_asm('g', 0x80134560, 'D_80151230', data='    .asciz "あ"\n', tail=False)
        isl = dupes.data_blocks('.section .rodata\n\n' + block)[0]
        kb, rb, badb, ib = self.parse(b, isl)
        self.assertIsNone(badb)
        self.assertEqual(kb, self.parse(a)[0])
        self.assertEqual(ib['data_own'], [])
        self.assertEqual(isl['D_80151230'], ['.asciz "\\202\\240"'])   # escaped as asm.py does
        # without the island the symbol is an ordinary relocation
        self.assertEqual([r[1] for r in self.parse(b)[1]], ['D_80151230', 'D_80151230', 'func_80140010'])

    def test_words_naming_labels_and_symbols(self):
        a, _ = data_func_asm('f', 0x80132000, 'D_80150100', data='    .word .L80132010\n')
        b, _ = data_func_asm('g', 0x80134560, 'D_80151230', data='    .word .L80134570\n')
        c, _ = data_func_asm('g', 0x80134560, 'D_80151230', data='    .word .L80134574\n')
        self.assertEqual(self.parse(a)[0], self.parse(b)[0])
        self.assertNotEqual(self.parse(a)[0], self.parse(c)[0])
        self.assertFalse(self.parse(a)[3]['data_ptr'])
        d, _ = data_func_asm('g', 0x80134560, 'D_80151230', data='    .word D_80150000\n')
        self.assertTrue(self.parse(d)[3]['data_ptr'])

    def test_unparsed_lines(self):
        a, _ = data_func_asm('f', 0x80132000, 'D_80150100', data='    .quad 1\n')
        self.assertEqual(self.parse(a)[2], 'unparsed rodata in function')
        b, _ = data_func_asm('f', 0x80132000, 'D_80150100')
        b = b.replace('endlabel f', '    .word 0x1234\nendlabel f')
        self.assertEqual(self.parse(b)[2], 'data in function text')

    def test_island_path(self):
        self.assertEqual(dupes.island_path('main', '80041000'), 'asm/data/main/80041000.rodata.s')
        self.assertEqual(dupes.island_path('BBB', 'BBB/80132000'), 'asm/ovl/BBB/data/BBB/80132000.rodata.s')


DATA_SRC_C = 'void fsrc(void) {\n    func_80140010("abc"); /* "x" */\n}\n'
DATA_HDR = 'void func_80140010(char *);\n'


class DataPlanTests(unittest.TestCase):
    def repo(self, tgt_tail=True, tgt_data='    .asciz "abc"\n    .balign 4\n', src_c=DATA_SRC_C):
        r = make_repo(src_c, TGT_C, src_hdr=DATA_HDR)
        self.addCleanup(r.close)
        src, sblock = data_func_asm('fsrc', 0x80132000, 'D_80150100', tail=False)
        r.write('asm/ovl/AAA/matchings/AAA/fsrc.s', src)
        r.write('asm/ovl/AAA/data/AAA.rodata.s', '.section .rodata\n\n' + sblock)
        tgt, tblock = data_func_asm('ftgt', 0x80134560, 'D_80151230', tail=tgt_tail, data=tgt_data,
                                    callee='func_80142222')
        r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s', tgt)
        r.write('asm/ovl/BBB/data/BBB.rodata.s', '.section .rodata\n\n' + tblock)
        return r

    def plans(self, r):
        funcs = dupes.load_funcs(r.root)
        return funcs, dupes.plan_all(r.root, funcs, {'aliases': dupes.load_aliases(r.root)})

    def test_string_in_the_function_file_is_applied_only_with_check(self):
        r = self.repo()
        funcs, (plans, skipped, stats) = self.plans(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertEqual(plans[0]['data'], 'island')
        self.assertIn('func_80142222("abc");', plans[0]['text'])
        self.assertIn('void ftgt(void)', plans[0]['text'])
        self.assertEqual(dupes.split_held(plans, False), ([], plans))
        self.assertEqual(dupes.split_held(plans, True), (plans, []))
        out = []
        dupes.data_report(dupes.group_funcs(funcs), plans, log=out.append)
        self.assertIn('data-in-function groups 1 (1 unmatched members, 24 bytes); fillable 1 functions, '
                      '24 bytes', out[0])
        self.assertTrue(out[1].startswith('NEEDS DATA CHECK BBB/ftgt <- fsrc: its rodata is in its own'))

    def test_data_outside_the_function_file_is_never_applied(self):
        r = self.repo(tgt_tail=False)
        funcs, (plans, skipped, stats) = self.plans(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertEqual(plans[0]['data'], 'outside')
        self.assertEqual(dupes.split_held(plans, True), ([], plans))

    def test_different_string_is_no_twin(self):
        r = self.repo(tgt_data='    .asciz "abd"\n    .balign 4\n')
        funcs, (plans, skipped, stats) = self.plans(r)
        self.assertEqual((plans, stats['groups']), ([], 0))

    def test_pointer_words_are_never_applied(self):
        r = self.repo(tgt_data='    .word D_80150000\n')
        src, sblock = data_func_asm('fsrc', 0x80132000, 'D_80150100', tail=False, data='    .word D_80150000\n')
        r.write('asm/ovl/AAA/matchings/AAA/fsrc.s', src)
        r.write('asm/ovl/AAA/data/AAA.rodata.s', '.section .rodata\n\n' + sblock)
        r.write('src/ovl/AAA.c', '#include "common.h"\n#include "ovl/AAA.h"\n\n'
                'void fsrc(void) {\n    func_80140010(0);\n}\n')
        funcs, (plans, skipped, stats) = self.plans(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertEqual(plans[0]['data'], 'pointers')
        self.assertEqual(dupes.split_held(plans, True), ([], plans))

    def test_source_string_without_own_data_is_still_refused(self):
        r = self.repo()
        r.write('asm/ovl/AAA/data/AAA.rodata.s', '')
        r.write('asm/ovl/BBB/data/BBB.rodata.s', '')
        r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s', data_func_asm(
            'ftgt', 0x80134560, 'D_80151230', tail=False, callee='func_80142222')[0])
        funcs, (plans, skipped, stats) = self.plans(r)
        self.assertEqual(plans, [])
        self.assertIn('string literal', skipped[0][1][0])

    def test_static_and_preprocessor_use_are_still_refused(self):
        r = self.repo(src_c='void fsrc(void) {\n    static char *s = "abc";\n    func_80140010(s);\n}\n')
        funcs, (plans, skipped, stats) = self.plans(r)
        self.assertEqual(plans, [])
        self.assertIn('static', skipped[0][1][0])


JT_SRC_C = 'void fsrc(s32 a) {\n    switch (a) {\n    case 0:\n        break;\n    }\n}\n'


class JumpTablePlanTests(unittest.TestCase):
    def repo(self, tgt_tail):
        r = make_repo(JT_SRC_C, TGT_C)
        self.addCleanup(r.close)
        r.write('asm/ovl/AAA/matchings/AAA/fsrc.s', jtbl_func_asm('fsrc', 0x80132000, 'jtbl_80150000')[0])
        tgt, table = jtbl_func_asm('ftgt', 0x80134560, 'jtbl_80151230', tail=tgt_tail)
        r.write('asm/ovl/BBB/nonmatchings/BBB/ftgt.s', tgt)
        r.write('asm/ovl/BBB/data/BBB.rodata.s', '.section .rodata\n\n' + table)
        return r

    def plans(self, r):
        funcs = dupes.load_funcs(r.root)
        return funcs, dupes.plan_all(r.root, funcs, {'aliases': dupes.load_aliases(r.root)})

    def test_table_in_the_function_file_is_applied_only_with_check(self):
        r = self.repo(True)
        funcs, (plans, skipped, stats) = self.plans(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertEqual(plans[0]['jtbl'], 'island')
        self.assertIn('void ftgt(s32 a)', plans[0]['text'])
        self.assertEqual(dupes.split_held(plans, False), ([], plans))
        self.assertEqual(dupes.split_held(plans, True), (plans, []))
        out = []
        dupes.jtbl_report(dupes.group_funcs(funcs), plans, log=out.append)
        self.assertIn('jump-table groups 1 (1 unmatched members); fillable 1 functions, 48 bytes', out[0])
        self.assertTrue(out[1].startswith('NEEDS JTBL CHECK BBB/ftgt <- fsrc'))

    def test_table_outside_the_c_object_is_never_applied(self):
        r = self.repo(False)
        funcs, (plans, skipped, stats) = self.plans(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertEqual(plans[0]['jtbl'], 'outside')
        self.assertEqual(dupes.split_held(plans, True), ([], plans))

    def test_plain_plans_are_not_held(self):
        r = make_repo(SRC_C, TGT_C)
        self.addCleanup(r.close)
        funcs, (plans, skipped, stats) = self.plans(r)
        self.assertIsNone(plans[0]['jtbl'])
        self.assertEqual(dupes.split_held(plans, False), (plans, []))


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


    def test_interior_byte_view_maps_effective_address_not_base(self):
        # Source fields are four bytes apart, target fields eight bytes apart:
        # copying the old +4 index would silently access target pad_09.
        r = self.gs_repo(('D_800F6476', 'D_800F647A'), ('D_800E6284', 'D_800E628C'),
                         '    D_800F6476 = 1;\n    (&D_800F6476)[4] += 5;\n',
                         tgt_main=self.GS.replace('extern u8 D_800F647A;',
                                                  'extern u8 D_800F6476;\nextern u8 D_800F647A;'))
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertIn('D_800E6280.unk_0C[0] += 5;', plans[0]['text'])
        self.assertNotIn('(&D_800E6280.unk_04)[4]', plans[0]['text'])



    def test_interior_cast_with_conflicting_source_view_is_skipped(self):
        r = self.gs_repo(('D_800F6476', 'D_800F647A'), ('D_800E6284', 'D_800E628C'),
                         '    D_800F6476 = 1;\n    ((u8 *)&D_800F6476)[4] += 5;\n',
                         tgt_main=self.GS.replace('extern u8 D_800F647A;',
                                                  'extern s32 D_800F6476;\nextern s32 D_800F647A;'))
        plans, skipped = self.plan(r)
        self.assertEqual(plans, [])
        self.assertIn('different access type', skipped[0][1][0])

    def test_near_copier_uses_same_interior_view_mapping(self):
        import neardupes
        r = self.gs_repo(('D_800E6284', 'D_800E6288'), ('D_800E6288', 'D_800E628C'),
                         '    D_800E6280.unk_04 = 1;\n'
                         '    ((u8 *)&D_800E6280.unk_04)[4] += 5;\n')
        funcs = dupes.load_funcs(r.root, near=True)
        src = next(f for f in funcs if f.matched)
        tgt = next(f for f in funcs if not f.matched)
        plan, why = neardupes.plan_near(r.root, src, tgt, {'aliases': dupes.load_aliases(r.root)})
        self.assertIsNotNone(plan, why)
        self.assertIn('D_800E6280.unk_0C[0] += 5;', plan['text'])

    def test_cast_interior_game_state_view_uses_access_type(self):
        r = self.gs_repo(('D_800E6284', 'D_800E6288'), ('D_800E6288', 'D_800E628C'),
                         '    D_800E6280.unk_04 = 1;\n'
                         '    *((u8 *)&D_800E6280.unk_04 + 4) += 5;\n')
        plans, skipped = self.plan(r)
        self.assertEqual(len(plans), 1, skipped)
        self.assertIn('D_800E6280.unk_0C[0] += 5;', plans[0]['text'])

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

    def test_exact_unit_and_address_case_insensitive(self):
        self.assertTrue(dupes.wanted_file('src/main/80041000.c', 'main/80041000'))
        self.assertTrue(dupes.wanted_file('src/main/80041000.c', '80041000'))
        self.assertFalse(dupes.wanted_file('src/main/80041000.c', '8004'))
        self.assertTrue(dupes.wanted_file('src/ovl/GEKO/80132000.c', 'geko/80132000'))
        self.assertFalse(dupes.wanted_file('src/ovl/GEKO/80132000.c', '80132000'))
        self.assertFalse(dupes.wanted_file('src/ovl/GEKO/80132000.c', 'TEL/80132000'))
        self.assertFalse(dupes.wanted_file('src/ovl/GEKO/80132000.c', 'GEKO/80132'))
        self.assertFalse(dupes.wanted_file('src/ovl/GEKO/80132000.c', 'GEKO'))
        self.assertTrue(dupes.wanted_file('src/ovl/TEL.c', 'tel'))
        self.assertFalse(dupes.wanted_file('src/ovl/DATE.c', 'TEL,80041000'))

    def test_restrict_files_filters_plans_and_skipped(self):
        class T:
            def __init__(self, path):
                self.path = path
        skipped = [(T('src/ovl/TEL.c'), ['r']), (T('src/ovl/DATE.c'), ['r'])]
        plans, sk = dupes.restrict_files(self.PLANS, skipped, 'TEL,main/80041000', lambda t: t.path)
        self.assertEqual([p['name'] for p in plans], ['a', 'b'])
        self.assertEqual(len(sk), 1)

    def test_apply_filters_before_writing(self):
        from unittest import mock
        plans = [dict(tgtfile='src/main/80132000.c'), dict(tgtfile='src/ovl/GEKO/80132000.c'),
                 dict(tgtfile='src/ovl/TEL/80132000.c')]
        with mock.patch.object(dupes, 'load_funcs', return_value=[]), \
                mock.patch.object(dupes, 'load_aliases', return_value={}), \
                mock.patch.object(dupes, 'plan_all', return_value=(plans, [], {'groups': 0, 'cand': 0})), \
                mock.patch.object(dupes, 'apply_all', return_value=[]) as apply:
            dupes.main(['--apply', '--files', '80132000'])
        self.assertEqual(apply.call_args.args[1], plans[:1])


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
