#!/usr/bin/env python3
"""Unit tests for tools/siblings.py on synthetic asm and C (no game data).

Run: tools/docker.sh python3 tools/test_siblings.py
"""
import contextlib
import io
import json
import os
import random
import struct
import sys
import types
import unittest
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dupes  # noqa: E402
import siblings  # noqa: E402
from test_dupes import Repo, insn  # noqa: E402

# registers by number, for the listing text
REGS = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra']
T0, T1, S0, RA = 8, 9, 16, 31


def r(n):
    return '$' + REGS[n]


def itype(op, rs, rt, imm):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def rtype(funct, rs=0, rt=0, rd=0, sa=0):
    return (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | funct


def lw(rt, off, rs):
    return itype(0x23, rs, rt, off), 'lw         %s, 0x%X(%s)' % (r(rt), off, r(rs))


def sw(rt, off, rs):
    return itype(0x2B, rs, rt, off), 'sw         %s, 0x%X(%s)' % (r(rt), off, r(rs))


def addiu(rt, rs, imm):
    return itype(9, rs, rt, imm), 'addiu      %s, %s, 0x%X' % (r(rt), r(rs), imm)


def ori(rt, rs, imm):
    return itype(0x0D, rs, rt, imm), 'ori        %s, %s, 0x%X' % (r(rt), r(rs), imm)


def lui(rt, imm):
    return itype(0x0F, 0, rt, imm), 'lui        %s, 0x%X' % (r(rt), imm)


def addu(rd, rs, rt):
    return rtype(0x21, rs, rt, rd), 'addu       %s, %s, %s' % (r(rd), r(rs), r(rt))


def subu(rd, rs, rt):
    return rtype(0x23, rs, rt, rd), 'subu       %s, %s, %s' % (r(rd), r(rs), r(rt))


def and_(rd, rs, rt):
    return rtype(0x24, rs, rt, rd), 'and        %s, %s, %s' % (r(rd), r(rs), r(rt))


def or_(rd, rs, rt):
    return rtype(0x25, rs, rt, rd), 'or         %s, %s, %s' % (r(rd), r(rs), r(rt))


def sll(rd, rt, sa):
    return rtype(0, 0, rt, rd, sa), 'sll        %s, %s, %d' % (r(rd), r(rt), sa)


def nop():
    return 0, 'nop'


def jr_ra():
    return rtype(8, RA), 'jr         $ra'


def jal(target, name):
    return (3 << 26) | (target >> 2 & 0x3FFFFFF), 'jal        %s' % name


def beq(rs, rt, off, label):
    return itype(4, rs, rt, off), 'beq        %s, %s, %s' % (r(rs), r(rt), label)


# opcodes used to damage a function: none of them occurs in BASE_MENU
FRESH = [itype(op, T0, T1, 4) for op in (0x0A, 0x0B, 0x0C, 0x0E, 0x20, 0x21, 0x22, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2E)]
FRESH_TEXT = ['slti', 'sltiu', 'andi', 'xori', 'lb', 'lh', 'lwl', 'lbu', 'lhu', 'lwr', 'sb', 'sh', 'swl', 'swr']


def base_body(n=40, regs=(T0, T1), imm=0x10):
    """n instructions drawn from ten different operations in a fixed pseudo-random order (a regular
    cycle would let difflib align a function with a shifted copy of itself)."""
    a, b = regs
    menu = [lambda: lw(a, imm, b), lambda: addiu(a, a, imm), lambda: sw(a, imm + 4, b), lambda: addu(a, a, b),
            lambda: ori(b, b, imm), lambda: lui(a, imm), lambda: sll(a, b, 2), lambda: subu(b, a, b),
            lambda: and_(a, a, b), lambda: or_(b, a, b)]
    rng = random.Random(1234)
    return [menu[rng.randrange(10)]() for _ in range(n)]


def damage(body, start, count):
    """The body with `count` instructions from `start` replaced by operations that occur nowhere else."""
    out = list(body)
    for k in range(count):
        out[start + k] = (FRESH[k], '%-10s %s, 0x4(%s)' % (FRESH_TEXT[k], r(T0), r(T1)))
    return out


def func_text(name, body, tail=True):
    """Splat asm text of a function: the body, then jr ra and a nop."""
    rows = list(body) + ([jr_ra(), nop()] if tail else [])
    out = 'nonmatching %s, 0x%X\n\nglabel %s\n' % (name, 4 * len(rows), name)
    for i, (word, text) in enumerate(rows):
        out += insn(4 * i, 0x80132000 + 4 * i, struct.pack('<I', word).hex().upper(), text)
    return out + 'endlabel %s\n' % name


def put_func(repo, unit, name, body, matched, cname=None):
    """Write the asm of a function of overlay `unit` (C file src/ovl/<unit>.c, created or extended)."""
    folder = 'matchings' if matched else 'nonmatchings'
    repo.write('asm/ovl/%s/%s/%s/%s.s' % (unit, folder, unit, name), func_text(name, body))
    path = 'src/ovl/%s.c' % unit
    try:
        text = repo.read(path)
    except OSError:
        text = '#include "common.h"\n\n'
    if matched:
        text += cname or ('void %s(void) {\n    /* body of %s */\n    D_80150000 = 1;\n}\n\n' % (name, name))
    else:
        text += 'INCLUDE_ASM("asm/ovl/%s/nonmatchings/%s", %s);\n\n' % (unit, unit, name)
    repo.write(path, text)


def args(**kw):
    base = dict(min_ratio=0.70, cluster_ratio=0.90, top=3, ngram=4, candidates=100, probe=64, notes_top=2, max_lines=300)
    base.update(kw)
    return types.SimpleNamespace(**base)


def run_report(repo, **kw):
    fns, _hits = siblings.load(repo.root)
    matched = [f for f in fns if f.matched]
    targets = sorted((f for f in fns if not f.matched), key=lambda f: f.key)
    a = args(**kw)
    report, sibs = siblings.build_report(targets, matched, a)
    return report, sibs, targets, matched, a


class TokenTests(unittest.TestCase):
    def test_registers_and_constants_do_not_change_the_token(self):
        self.assertEqual(siblings.op_token(addu(2, 3, 4)[0]), siblings.op_token(addu(16, 17, 18)[0]))
        self.assertEqual(siblings.op_token(lw(8, 0x10, 4)[0]), siblings.op_token(lw(9, 0x7FF8, 16)[0]))
        self.assertEqual(siblings.op_token(sll(8, 9, 2)[0]), siblings.op_token(sll(10, 11, 7)[0]))
        self.assertEqual(siblings.op_token(jal(0x80012000, 'a')[0]), siblings.op_token(jal(0x8004C250, 'b')[0]))
        self.assertEqual(siblings.op_token(beq(1, 2, 5, 'x')[0]), siblings.op_token(beq(3, 4, 0x20, 'y')[0]))

    def test_operations_differ(self):
        words = [lw(8, 0, 4), sw(8, 0, 4), addiu(8, 8, 1), ori(8, 8, 1), addu(8, 8, 9), subu(8, 8, 9), nop(),
                 sll(8, 9, 2), jr_ra(), jal(0x80012000, 'a'), beq(1, 2, 3, 'x'), (itype(1, 4, 0, 5), 'bltz'),
                 (itype(1, 4, 1, 5), 'bgez')]
        tokens = [siblings.op_token(w) for w, _t in words]
        self.assertEqual(len(set(tokens)), len(words))
        self.assertTrue(all(0 <= t < siblings.RADIX for t in tokens))

    def test_nop_is_not_a_shift(self):
        self.assertNotEqual(siblings.op_token(0), siblings.op_token(sll(8, 8, 0)[0]))

    def test_gte_command_and_coprocessor_moves(self):
        rtps = (0x12 << 26) | (1 << 25) | 0x01
        nclip = (0x12 << 26) | (1 << 25) | 0x06
        mfc2 = (0x12 << 26) | (0 << 21) | (8 << 16) | (12 << 11)
        mtc2 = (0x12 << 26) | (4 << 21) | (8 << 16) | (12 << 11)
        toks = [siblings.op_token(w) for w in (rtps, nclip, mfc2, mtc2)]
        self.assertEqual(len(set(toks)), 4)
        self.assertTrue(all(0 <= t < siblings.RADIX for t in toks))

    def test_reg_token_keeps_registers_and_erases_constants(self):
        self.assertEqual(siblings.reg_token(lw(8, 0x10, 4)[0]), siblings.reg_token(lw(8, 0x44, 4)[0]))
        self.assertNotEqual(siblings.reg_token(lw(8, 0x10, 4)[0]), siblings.reg_token(lw(9, 0x10, 4)[0]))
        self.assertEqual(siblings.reg_token(sll(8, 9, 2)[0]), siblings.reg_token(sll(8, 9, 5)[0]))
        self.assertNotEqual(siblings.reg_token(sll(8, 9, 2)[0]), siblings.reg_token(sll(8, 10, 2)[0]))
        self.assertEqual(siblings.reg_token(jal(0x80012000, 'a')[0]), siblings.reg_token(jal(0x80013000, 'b')[0]))

    def test_clone_key_follows_registers_not_constants(self):
        a = [w for w, _t in base_body(12, (T0, T1), 0x10)]
        b = [w for w, _t in base_body(12, (T0, T1), 0x44)]
        c = [w for w, _t in base_body(12, (S0, T1), 0x10)]
        self.assertEqual(siblings.clone_key(a), siblings.clone_key(b))
        self.assertNotEqual(siblings.clone_key(a), siblings.clone_key(c))


class ReadWordsTests(unittest.TestCase):
    def test_words_text_and_rodata_cut(self):
        text = func_text('f', [lw(8, 4, 4), addu(9, 8, 8)])
        text += dupes.RODATA_MARK + '\n    /* 0 80140000 00000000 */  .word 0x0\n    /* 4 80140004 2300088D */  lw $t0, 0x4($t0)\n'
        rows = siblings.read_words(text)
        self.assertEqual([m for _w, m, _o in rows], ['lw', 'addu', 'jr', 'nop'])
        self.assertEqual(rows[0][0], lw(8, 4, 4)[0])
        self.assertEqual(rows[0][2], '$t0, 0x4($a0)')

    def test_data_directive_in_code_is_skipped(self):
        text = func_text('f', [lw(8, 4, 4)]) + '    /* 20 80132020 12345678 */  .word 0x78563412\n'
        self.assertEqual(len(siblings.read_words(text)), 3)


def dp_lcs(a, b):
    row = [0] * (len(b) + 1)
    for x in a:
        prev = 0
        for j, y in enumerate(b, 1):
            cur = row[j]
            row[j] = prev + 1 if x == y else max(row[j], row[j - 1])
            prev = cur
    return row[-1]


class LcsTests(unittest.TestCase):
    def test_bit_parallel_lcs_equals_dynamic_programming(self):
        rng = random.Random(5)
        for _ in range(60):
            a = [rng.randrange(6) for _ in range(rng.randrange(1, 60))]
            b = [rng.randrange(6) for _ in range(rng.randrange(1, 60))]
            masks, full = siblings.lcs_masks(b)
            self.assertEqual(siblings.lcs_length(a, masks, full), dp_lcs(a, b), (a, b))

    def test_lcs_bounds_the_difflib_ratio(self):
        rng = random.Random(9)
        import difflib
        for _ in range(30):
            a = [rng.randrange(5) for _ in range(rng.randrange(5, 50))]
            b = [rng.randrange(5) for _ in range(rng.randrange(5, 50))]
            masks, full = siblings.lcs_masks(b)
            sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
            self.assertLessEqual(sm.ratio(), 2.0 * siblings.lcs_length(a, masks, full) / (len(a) + len(b)) + 1e-12)

    def test_ratio_to_uses_difflib_not_the_lcs(self):
        """A sequence made of the same tokens in another order has a high LCS, a low difflib ratio."""
        repo = Repo()
        self.addCleanup(repo.close)
        body = base_body(40)
        shuffled = list(body)
        random.Random(3).shuffle(shuffled)
        put_func(repo, 'AAA', 'm_shuffled', shuffled, True)
        fns, _ = siblings.load(repo.root)
        pool = siblings.Pool(fns, 4)
        toks = [siblings.op_token(w) for w, _t in body] + [siblings.op_token(jr_ra()[0]), siblings.op_token(0)]
        masks, full = siblings.lcs_masks(toks)
        self.assertGreater(2.0 * siblings.lcs_length(pool.fns[0].tokens, masks, full) / (2 * len(toks)), 0.5)
        self.assertEqual(siblings.ratio_to(toks, pool, [0], 0.9), [])


class GramTests(unittest.TestCase):
    def test_grams_count_overlapping_windows(self):
        c = siblings.grams_of([1, 2, 3, 1, 2, 3, 1], 3)
        self.assertEqual(sum(c.values()), 5)
        self.assertEqual(max(c.values()), 2)     # 1,2,3 twice

    def test_short_sequence_is_one_gram_and_does_not_collide(self):
        self.assertEqual(sum(siblings.grams_of([5, 6], 4).values()), 1)
        self.assertNotEqual(list(siblings.grams_of([5, 6], 4)), list(siblings.grams_of([6, 5], 4)))

    def test_pool_probes_the_shared_rare_grams(self):
        rare = base_body(40)
        other = [(itype(0x0A + i % 4, T0, T1, 0), 'x') for i in range(40)]
        repo = Repo()
        self.addCleanup(repo.close)
        put_func(repo, 'AAA', 'f_rare', rare, True)
        put_func(repo, 'AAA', 'f_other', other, True)
        fns, _ = siblings.load(repo.root)
        pool = siblings.Pool(fns, 4)
        tokens = [siblings.op_token(w) for w, _t in damage(rare, 10, 2)]
        cand = pool.candidates(siblings.grams_of(tokens, 4), 5, 8)
        self.assertEqual([pool.fns[i].name for i in cand][:1], ['f_rare'])


class SimilarityTests(unittest.TestCase):
    def setUp(self):
        self.repo = Repo()
        self.addCleanup(self.repo.close)

    def test_tiers_follow_the_ratio(self):
        base = base_body(40)
        put_func(self.repo, 'AAA', 'm_base', base, True)
        put_func(self.repo, 'BBB', 't_high', damage(base, 20, 2), False)       # 40 of 42 matching: 0.95
        put_func(self.repo, 'BBB', 't_mid', damage(base, 10, 6), False)        # 36 of 42: 0.86
        put_func(self.repo, 'BBB', 't_low', damage(base, 10, 10), False)       # 32 of 42: 0.76
        put_func(self.repo, 'BBB', 't_none', [(itype(0x0A + i % 4, T0, T1, 0), 'x') for i in range(40)], False)
        report, sibs, targets, _m, _a = run_report(self.repo)
        rows = report['targets']
        self.assertEqual(rows['BBB/t_high']['tier'], '>=0.90')
        self.assertEqual(rows['BBB/t_mid']['tier'], '0.80-0.90')
        self.assertEqual(rows['BBB/t_low']['tier'], '0.70-0.80')
        self.assertEqual(rows['BBB/t_none']['tier'], '<0.70')
        self.assertEqual(rows['BBB/t_none']['siblings'], [])
        self.assertAlmostEqual(rows['BBB/t_high']['siblings'][0]['ratio'], 40 / 42.0, places=3)
        self.assertAlmostEqual(rows['BBB/t_mid']['siblings'][0]['ratio'], 36 / 42.0, places=3)
        self.assertAlmostEqual(rows['BBB/t_low']['siblings'][0]['ratio'], 32 / 42.0, places=3)
        for k in ('>=0.90', '0.80-0.90', '0.70-0.80', '<0.70'):
            self.assertEqual(report['tiers'][k]['functions'], 1, k)
        self.assertEqual(report['tiers']['>=0.90']['bytes'], 4 * 42)
        self.assertEqual(report['counts'], {'matched': 1, 'targets': 4, 'target_bytes': 4 * 42 * 4, 'guarded': 0})

    def test_sibling_record_fields(self):
        base = base_body(40)
        put_func(self.repo, 'AAA', 'm_base', base, True)
        put_func(self.repo, 'BBB', 't_high', damage(base, 20, 2), False)
        report, *_ = run_report(self.repo)
        s = report['targets']['BBB/t_high']['siblings'][0]
        self.assertEqual((s['unit'], s['name'], s['bytes'], s['file']), ('AAA', 'm_base', 168, 'src/ovl/AAA.c'))
        self.assertFalse(s['clone'])
        t = report['targets']['BBB/t_high']
        self.assertEqual((t['bytes'], t['file'], t['guarded']), (168, 'src/ovl/BBB.c', False))

    def test_top_three_ordered_by_ratio_then_own_unit_then_size(self):
        base = base_body(40)
        put_func(self.repo, 'AAA', 'm_exact', base, True)
        put_func(self.repo, 'AAA', 'm_one', damage(base, 5, 1), True)
        put_func(self.repo, 'BBB', 'm_same_unit_tie', base, True)           # ties m_exact; the target's unit wins
        put_func(self.repo, 'CCC', 'm_two', damage(base, 5, 2), True)
        put_func(self.repo, 'BBB', 't', base, False)
        report, *_ = run_report(self.repo)
        names = [s['unit'] + '/' + s['name'] for s in report['targets']['BBB/t']['siblings']]
        self.assertEqual(names, ['BBB/m_same_unit_tie', 'AAA/m_exact', 'AAA/m_one'])
        self.assertEqual([s['ratio'] for s in report['targets']['BBB/t']['siblings']][:2], [1.0, 1.0])

    def test_clone_flag_needs_equal_registers(self):
        put_func(self.repo, 'AAA', 'm_base', base_body(40, (T0, T1), 0x10), True)
        put_func(self.repo, 'BBB', 't_const', base_body(40, (T0, T1), 0x44), False)       # only constants differ
        put_func(self.repo, 'BBB', 't_regs', base_body(40, (S0, T1), 0x10), False)         # other registers
        report, *_ = run_report(self.repo)
        self.assertTrue(report['targets']['BBB/t_const']['siblings'][0]['clone'])
        self.assertEqual(report['targets']['BBB/t_regs']['siblings'][0]['ratio'], 1.0)
        self.assertFalse(report['targets']['BBB/t_regs']['siblings'][0]['clone'])
        self.assertEqual(report['tiers']['>=0.90']['clones'], 1)

    def test_guarded_draft_is_a_target(self):
        base = base_body(40)
        put_func(self.repo, 'AAA', 'm_base', base, True)
        put_func(self.repo, 'BBB', 't_guard', damage(base, 20, 2), False)
        text = self.repo.read('src/ovl/BBB.c')
        self.repo.write('src/ovl/BBB.c', '#ifdef NON_MATCHING\nvoid t_guard(void) {\n}\n#else\n' + text.split('\n', 2)[2] + '#endif\n')
        report, *_ = run_report(self.repo)
        self.assertTrue(report['targets']['BBB/t_guard']['guarded'])
        self.assertEqual(report['counts']['guarded'], 1)

    def test_a_target_is_not_its_own_sibling_and_unmatched_are_not_siblings(self):
        base = base_body(40)
        put_func(self.repo, 'AAA', 't1', base, False)
        put_func(self.repo, 'BBB', 't2', base, False)
        report, *_ = run_report(self.repo)
        self.assertEqual(report['targets']['AAA/t1']['siblings'], [])
        self.assertEqual(report['targets']['BBB/t2']['siblings'], [])

    def test_index_agrees_with_brute_force(self):
        base = base_body(40)
        for k in range(6):
            put_func(self.repo, 'AAA', 'm%d' % k, damage(base, 4 + 3 * k, 1 + k % 3), True)
        put_func(self.repo, 'BBB', 't', damage(base, 12, 3), False)
        _report, _sibs, targets, matched, a = run_report(self.repo)
        pool = siblings.Pool(matched, 4)
        t = targets[0]
        fast = siblings.find_siblings(t, siblings.grams_of(t.tokens, 4), pool, a)
        slow = siblings.brute_siblings(t, pool, a)
        self.assertEqual([(round(x, 6), f.key) for x, f in fast], [(round(x, 6), f.key) for x, f in slow])
        out = io.StringIO()
        bad, have = siblings.check_recall(targets, matched, types.SimpleNamespace(**dict(vars(a), check_recall=5, recall_max_insns=400)),
                                          log=lambda m: out.write(m + '\n'))
        self.assertEqual((bad, have), (0, 1))
        self.assertIn('recall check: 1 targets', out.getvalue())

    def test_min_ratio_filters_siblings(self):
        base = base_body(40)
        put_func(self.repo, 'AAA', 'm_base', base, True)
        put_func(self.repo, 'BBB', 't_low', damage(base, 10, 10), False)
        report, *_ = run_report(self.repo, min_ratio=0.9)
        self.assertEqual(report['targets']['BBB/t_low']['siblings'], [])
        report, *_ = run_report(self.repo, min_ratio=0.6)
        self.assertEqual(len(report['targets']['BBB/t_low']['siblings']), 1)


class ClusterTests(unittest.TestCase):
    def test_unmatched_pairs_cluster_and_carry_their_best_matched_sibling(self):
        repo = Repo()
        self.addCleanup(repo.close)
        base = base_body(40)
        other = [(itype(0x0A + i % 4, T0, T1, 0), 'x') for i in range(40)]
        put_func(repo, 'AAA', 'm_base', base, True)
        put_func(repo, 'BBB', 'x1', damage(base, 5, 1), False)                      # matched sibling
        put_func(repo, 'BBB', 'x2', damage(base, 25, 1), False)                      # chained to x1
        put_func(repo, 'CCC', 'y1', other, False)                                    # no matched sibling
        put_func(repo, 'CCC', 'y2', damage(other, 21, 1), False)
        put_func(repo, 'CCC', 'alone', list(reversed(base_body(30))), False)
        report, *_ = run_report(repo)
        cl = {tuple(m['name'] for m in c['members']): c for c in report['clusters']}
        self.assertEqual(sorted(cl), [('x1', 'x2'), ('y1', 'y2')])
        self.assertIsNotNone(cl[('x1', 'x2')]['best_matched'])
        self.assertEqual(cl[('x1', 'x2')]['best_matched']['name'], 'm_base')
        self.assertIsNone(cl[('y1', 'y2')]['best_matched'])
        self.assertEqual(cl[('y1', 'y2')]['bytes'], 2 * 42 * 4)
        self.assertGreaterEqual(cl[('y1', 'y2')]['min_ratio'], 0.9)
        out = []
        siblings.summary(report, out=out.append)
        text = '\n'.join(out)
        self.assertIn('unmatched clusters at ratio >= 0.90: 2', text)
        self.assertIn('1 of them have no matched sibling', text)
        self.assertIn('opens 1 functions, 168 bytes', text)


class LoadTests(unittest.TestCase):
    def setUp(self):
        self.repo = Repo()
        self.addCleanup(self.repo.close)
        put_func(self.repo, 'AAA', 'm', base_body(20), True)
        put_func(self.repo, 'AAA', 'u', base_body(24), False)

    def test_load_state_and_paths(self):
        fns, hits = siblings.load(self.repo.root)
        by = {f.name: f for f in fns}
        self.assertEqual(hits, 0)
        self.assertTrue(by['m'].matched)
        self.assertFalse(by['u'].matched)
        self.assertEqual((by['m'].n, by['m'].bytes, by['m'].key, by['m'].cfile), (22, 88, 'AAA/m', 'src/ovl/AAA.c'))

    def test_stale_asm_of_a_removed_function_is_ignored(self):
        self.repo.write('asm/ovl/AAA/nonmatchings/AAA/gone.s', func_text('gone', base_body(10)))
        fns, _ = siblings.load(self.repo.root)
        self.assertEqual(sorted(f.name for f in fns), ['m', 'u'])

    def test_cache_hits_and_invalidates_on_change(self):
        os.makedirs(os.path.join(self.repo.root, 'build'))
        cache = siblings.TokenCache(self.repo.root)
        self.assertTrue(cache.enabled)
        _fns, hits = siblings.load(self.repo.root, cache=cache)
        self.assertEqual(hits, 0)
        self.assertTrue(os.path.exists(os.path.join(self.repo.root, siblings.CACHE)))
        with mock.patch.object(siblings, 'read_words', side_effect=AssertionError('parsed again')):
            fns, hits = siblings.load(self.repo.root, cache=siblings.TokenCache(self.repo.root))
        self.assertEqual((hits, len(fns)), (2, 2))
        self.repo.write('asm/ovl/AAA/nonmatchings/AAA/u.s', func_text('u', base_body(30)))
        fns, hits = siblings.load(self.repo.root, cache=siblings.TokenCache(self.repo.root))
        self.assertEqual(hits, 1)
        self.assertEqual({f.name: f.n for f in fns}['u'], 32)

    def test_cache_is_off_without_build_dir_or_with_a_bad_file(self):
        self.assertFalse(siblings.TokenCache(self.repo.root).enabled)
        os.makedirs(os.path.join(self.repo.root, 'build'))
        self.repo.write(siblings.CACHE, '{not json')
        c = siblings.TokenCache(self.repo.root)
        self.assertTrue(c.enabled)
        self.assertEqual(c.files, {})
        self.repo.write(siblings.CACHE, json.dumps({'v': 0, 'files': {'x': [1, 2, [3], 'k']}}))
        self.assertEqual(siblings.TokenCache(self.repo.root).files, {})


class ListingTests(unittest.TestCase):
    def rows(self, body):
        return [(4 * i, text) for i, (_w, text) in enumerate(body)]

    def test_markers_and_counts(self):
        a = [lw(T0, 0x10, T1), addiu(T0, T0, 0x10), sw(T0, 0x14, T1), addu(T0, T0, T1), or_(T1, T0, T1)]
        b = list(a)
        b[1] = addiu(T0, T0, 0x99)             # constant only
        b[3] = addu(T1, T1, T0)                # registers
        lines, st = siblings.side_by_side(self.rows(a), self.rows(b), [siblings.op_token(w) for w, _t in a],
                                          [siblings.op_token(w) for w, _t in b])
        self.assertEqual(len(lines), 5)
        self.assertEqual((st['same'], st['constants'], st['registers']), (3, 1, 1))
        self.assertTrue(lines[0].startswith('0000 '))
        marks = [ln[5 + 40 + 1] for ln in lines]
        self.assertEqual(marks, ['=', '~', '=', '!', '='])

    def test_unaligned_rows_are_blank_marked_and_counted(self):
        a = base_body(10)
        b = a[:5] + [nop(), nop()] + a[5:]     # the sibling has two extra instructions
        lines, st = siblings.side_by_side(self.rows(a), self.rows(b), [siblings.op_token(w) for w, _t in a],
                                          [siblings.op_token(w) for w, _t in b])
        self.assertEqual(len(lines), 12)
        self.assertEqual((st['target_only'], st['sibling_only']), (0, 2))
        self.assertTrue(any(ln.startswith(' ' * 45) for ln in lines))

    def test_erase_constants(self):
        self.assertEqual(siblings.erase_constants('lw         $t0, %lo(D_80150000)($t0)'),
                         siblings.erase_constants('lw         $t0, %lo(D_80151234)($t0)'))
        self.assertEqual(siblings.erase_constants('addiu      $a0, $zero, -0x10'), 'addiu      $a0, $zero, N')
        self.assertNotEqual(siblings.erase_constants('addu $t0, $t1, $t2'), siblings.erase_constants('addu $t0, $t1, $t3'))
        self.assertEqual(siblings.erase_constants('jal        func_8004C250'), 'jal        S')


class PackageTests(unittest.TestCase):
    def setUp(self):
        self.repo = Repo()
        self.addCleanup(self.repo.close)
        base = base_body(40)
        put_func(self.repo, 'AAA', 'm_base', base, True,
                 cname='void m_base(void) {\n    D_80150000 = 1;\n    func_80012000();\n}\n\n')
        put_func(self.repo, 'BBB', 't_high', damage(base, 20, 2), False)
        put_func(self.repo, 'CCC', 't_high', damage(base, 21, 2), False)         # same name, other overlay
        put_func(self.repo, 'CCC', 't_none', [(itype(0x0A + i % 4, T0, T1, 0), 'x') for i in range(40)], False)
        self.report, self.sibs, self.targets, self.matched, self.args = run_report(self.repo)

    def test_read_entries_from_string_list_and_queue_json(self):
        self.assertEqual(siblings.read_entries('a, b/c ,,file:x'), ['a', 'b/c', 'file:x'])
        self.repo.write('list.txt', '# package 7\nBBB/t_high  # first\n\nm_x\n')
        self.assertEqual(siblings.read_entries(os.path.join(self.repo.root, 'list.txt')), ['BBB/t_high', 'm_x'])
        plan = [{'agent': 1, 'files': ['TACO'], 'functions': [{'file': '80041000', 'name': 'f1', 'size': 4},
                                                              {'file': 'TACO/80132000', 'name': 'f2', 'size': 8}]}]
        self.repo.write('plan.json', json.dumps(plan))
        self.assertEqual(siblings.read_entries(os.path.join(self.repo.root, 'plan.json')), ['main/f1', 'TACO/f2'])

    def test_resolve_entries(self):
        sel, bad = siblings.resolve_entries(['BBB/t_high', 't_high', 'file:src/ovl/CCC.c', 'file:CCC', 'nope', 'AAA/m_base'], self.targets)
        self.assertEqual([t.key for t in sel], ['BBB/t_high', 'CCC/t_high', 'CCC/t_none'])
        self.assertEqual(bad, ['nope', 'AAA/m_base'])        # a matched function is not a target

    def test_resolve_file_entries_in_the_per_object_layout(self):
        """The real tree has one C file per object: src/ovl/<NAME>/<address>.c, src/main/<address>.c."""
        ovl = types.SimpleNamespace(key='TACO/f', name='f', cfile='src/ovl/TACO/80134930.c')
        main = types.SimpleNamespace(key='main/g', name='g', cfile='src/main/80041000.c')
        for entry in ('file:src/ovl/TACO/80134930.c', 'file:TACO/80134930', 'file:80134930'):
            self.assertEqual(siblings.resolve_entries([entry], [ovl, main]), ([ovl], []), entry)
        for entry in ('file:src/main/80041000.c', 'file:80041000'):
            self.assertEqual(siblings.resolve_entries([entry], [ovl, main]), ([main], []), entry)
        self.assertEqual(siblings.resolve_entries(['file:src/ovl/TACO.c'], [ovl, main]), ([], ['file:src/ovl/TACO.c']))

    def test_note_has_c_listing_and_summary(self):
        t = next(f for f in self.targets if f.key == 'BBB/t_high')
        text = siblings.note_text(self.repo.root, t, self.sibs[self.targets.index(t)], self.args)
        self.assertIn('# Siblings of BBB/t_high (168 bytes, src/ovl/BBB.c)', text)
        self.assertIn('| 1 | `AAA/m_base` | 0.9', text)
        self.assertIn('```c\nvoid m_base(void) {\n    D_80150000 = 1;\n    func_80012000();\n}\n```', text)
        self.assertIn('Asm, target (left, `t_high`) against sibling (right, `m_base`)', text)
        self.assertIn('identical (`=`)', text)
        self.assertIn('lw         $t0, 0x10($t1)', text)

    def test_note_truncates_long_listings(self):
        t = next(f for f in self.targets if f.key == 'BBB/t_high')
        text = siblings.note_text(self.repo.root, t, self.sibs[self.targets.index(t)], args(max_lines=10))
        self.assertIn('more rows (full asm:', text)

    def test_note_without_sibling(self):
        t = next(f for f in self.targets if f.key == 'CCC/t_none')
        text = siblings.note_text(self.repo.root, t, [], self.args)
        self.assertIn('No matched function reaches ratio 0.70', text)

    def test_write_notes_files_and_index(self):
        out = os.path.join(self.repo.root, 'notes')
        paths = siblings.write_notes(self.repo.root, self.targets, self.sibs, out, self.args)
        self.assertEqual(sorted(os.path.basename(p) for p in paths),
                         ['BBB__t_high.md', 'CCC__t_high.md', 'CCC__t_none.md'])
        index = self.repo.read('notes/INDEX.md')
        self.assertIn('[BBB/t_high](BBB__t_high.md)', index)
        self.assertIn('`AAA/m_base`', index)

    def test_c_excerpt_missing_cases(self):
        f = self.matched[0]
        path, first, text = siblings.c_excerpt(self.repo.root, f, 100)
        self.assertEqual((path, first), ('src/ovl/AAA.c', 3))
        self.assertTrue(text.startswith('void m_base'))
        self.repo.write('src/ovl/AAA.c', '')
        self.assertIn('not found', siblings.c_excerpt(self.repo.root, f, 100)[2])
        os.remove(os.path.join(self.repo.root, 'src/ovl/AAA.c'))
        self.assertEqual(siblings.c_excerpt(self.repo.root, f, 100)[2], '(C file not found)')


class CliTests(unittest.TestCase):
    def setUp(self):
        self.repo = Repo()
        self.addCleanup(self.repo.close)
        base = base_body(40)
        put_func(self.repo, 'AAA', 'm_base', base, True)
        put_func(self.repo, 'BBB', 't_high', damage(base, 20, 2), False)
        put_func(self.repo, 'TT', 't_tt', damage(base, 8, 2), False)

    def run_main(self, *argv):
        out, err = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            code = siblings.main(['--root', self.repo.root, '--no-cache'] + list(argv))
        return code, out.getvalue(), err.getvalue()

    def test_default_run_writes_json_and_prints_the_summary(self):
        os.makedirs(os.path.join(self.repo.root, 'build'))
        code, out, _err = self.run_main()
        self.assertEqual(code, 0)
        data = json.loads(self.repo.read(siblings.JSON_OUT))
        self.assertEqual(sorted(data['targets']), ['BBB/t_high', 'TT/t_tt'])
        self.assertIn('siblings: 2 targets', out)
        self.assertIn('>=0.90', out)

    def test_skip_unit_and_unit_filter_targets_only(self):
        code, out, _ = self.run_main('--json', '-', '--skip-unit', 'TT')
        data = json.loads(out)
        self.assertEqual(sorted(data['targets']), ['BBB/t_high'])
        self.assertEqual(data['counts']['matched'], 1)
        code, out, _ = self.run_main('--json', '-', '--unit', 'TT')
        self.assertEqual(sorted(json.loads(out)['targets']), ['TT/t_tt'])

    def test_package_writes_notes_for_listed_targets_only(self):
        code, out, _ = self.run_main('--package', 'BBB/t_high,zzz', '--out-dir', 'notes')
        self.assertEqual(code, 0)
        self.assertEqual(sorted(os.listdir(os.path.join(self.repo.root, 'notes'))), ['BBB__t_high.md', 'INDEX.md'])
        self.assertFalse(os.path.exists(os.path.join(self.repo.root, siblings.JSON_OUT)))
        self.assertIn('wrote 1 notes', out)

    def test_package_with_nothing_resolved_fails(self):
        code, _out, err = self.run_main('--package', 'zzz')
        self.assertEqual(code, 1)
        self.assertIn("no unmatched function for 'zzz'", err)

    def test_package_ignores_unit_filters(self):
        code, _out, _ = self.run_main('--package', 'TT/t_tt', '--skip-unit', 'TT', '--out-dir', 'notes')
        self.assertEqual(code, 0)
        self.assertIn('TT__t_tt.md', os.listdir(os.path.join(self.repo.root, 'notes')))

    def test_check_recall_exit_code(self):
        code, out, _ = self.run_main('--check-recall', '3')
        self.assertEqual(code, 0)
        self.assertIn('recall check:', out)

    def test_verbose_lists_every_target(self):
        _code, out, _ = self.run_main('--json', os.path.join(self.repo.root, 'out.json'), '-v')
        self.assertIn('BBB/t_high', out)
        self.assertIn('AAA/m_base', out)


if __name__ == '__main__':
    unittest.main()
