#!/usr/bin/env python3
"""Synthetic tests for tools/regdiff.py (T-10400): decoding, alignment, the allocator-trace parsers, register
webs, range matching and the hints. No compiler, no game data.

Run: tools/docker.sh python3 tools/test_regdiff.py
"""
import unittest

import regdiff as rd

R = rd.REG_NUM


def rtype(rs, rt, rdst, sh, fn):
    return (R[rs] << 21) | (R[rt] << 16) | (R[rdst] << 11) | (sh << 6) | fn


def itype(op, rs, rt, imm):
    return (op << 26) | (R[rs] << 21) | (R[rt] << 16) | (imm & 0xFFFF)


def addu(d, s, t):
    return rtype(s, t, d, 0, 0x21)


def addiu(t, s, imm):
    return itype(9, s, t, imm)


def lui(t, imm):
    return itype(15, "zero", t, imm)


def lw(t, off, base):
    return itype(0x23, base, t, off)


def sw(t, off, base):
    return itype(0x2B, base, t, off)


def jal():
    return (3 << 26)


def jr_ra():
    return rtype("ra", "zero", "zero", 0, 8)


def nop():
    return 0


def beq(s, t, off):
    return itype(4, s, t, off)


def key(word):
    return "%08x" % word


def insns(words):
    return [rd.Decoded(w, "i%d" % k) for k, w in enumerate(words)]


class DecodeTest(unittest.TestCase):
    def test_roles_of_common_instructions(self):
        d = rd.Decoded(addu("v0", "a0", "a1"))
        self.assertEqual((d.defs, sorted(d.uses)), ([R["v0"]], [R["a0"], R["a1"]]))
        d = rd.Decoded(lw("t0", 4, "sp"))
        self.assertEqual((d.defs, d.uses), ([R["t0"]], [R["sp"]]))
        d = rd.Decoded(sw("t0", 4, "sp"))
        self.assertEqual((d.defs, sorted(d.uses)), ([], sorted([R["t0"], R["sp"]])))
        self.assertEqual(rd.Decoded(jal()).kind, "call")
        self.assertEqual(rd.Decoded(jr_ra()).kind, "ret")
        d = rd.Decoded(beq("a0", "zero", 3))
        self.assertEqual((d.kind, d.target), ("branch", 3))
        self.assertEqual(rd.Decoded(lui("t1", 0x8012)).defs, [R["t1"]])

    def test_zero_register_is_neither_def_nor_use(self):
        d = rd.Decoded(addiu("v0", "zero", 5))
        self.assertEqual((d.defs, d.uses), ([R["v0"]], []))

    def test_shape_clears_registers_only(self):
        a, b = rd.Decoded(addu("v0", "a0", "a1")), rd.Decoded(addu("v1", "t0", "t1"))
        self.assertEqual(a.shape, b.shape)
        self.assertNotEqual(a.shape, rd.Decoded(rtype("a0", "a1", "v0", 0, 0x23)).shape)   # subu

    def test_shape_key_clears_the_immediate_of_i_type_only(self):
        a, b = key(addiu("t1", "t1", 0x439C)), key(addiu("t2", "t2", 0x4394))
        self.assertEqual(rd.key_shape(a), rd.key_shape(b))
        self.assertNotEqual(rd.key_shape(key(itype(4, "a0", "a1", 3))), rd.key_shape(key(itype(4, "a0", "a1", 4))))
        self.assertNotEqual(rd.key_shape(key(addiu("t1", "t1", 1))), rd.key_shape(key(lw("t1", 1, "t1"))))


class AlignTest(unittest.TestCase):
    def keys(self, words):
        return [key(w) for w in words]

    def test_register_only_difference(self):
        draft = self.keys([lui("t1", 0x8012), addu("v0", "t1", "a0"), jr_ra(), nop()])
        want = self.keys([lui("t2", 0x8012), addu("v0", "t2", "a0"), jr_ra(), nop()])
        pairs, structural, left = rd.align(draft, want)
        self.assertEqual((structural, left), ([], []))
        self.assertEqual([(p.draft_index, p.target_index) for p in pairs], [(0, 0), (1, 1)])
        self.assertEqual(rd.register_map(pairs), {(R["t1"], R["t2"]): 2})

    def test_first_divergence_prefers_the_definition(self):
        draft = self.keys([addu("v0", "t1", "a0")])
        want = self.keys([addu("v1", "t2", "a0")])
        pairs, _s, _l = rd.align(draft, want)
        p, op = rd.first_divergence(pairs)
        self.assertEqual(op[1], "def")
        self.assertEqual((op[2], op[3]), (R["v0"], R["v1"]))

    def test_swapped_constants_are_paired_by_value(self):
        # draft: t1 = ...439c, t2 = ...4394; original: t1 = ...4394, t2 = ...439c
        draft = self.keys([lui("t1", 0x8012), lui("t2", 0x8012), addiu("t1", "t1", 0x439C), addiu("t2", "t2", 0x4394)])
        want = self.keys([lui("t1", 0x8012), lui("t2", 0x8012), addiu("t1", "t1", 0x4394), addiu("t2", "t2", 0x439C)])
        pairs, structural, left = rd.align(draft, want)
        self.assertEqual((structural, left), ([], []))
        self.assertEqual([(p.draft_index, p.target_index) for p in pairs], [(2, 3), (3, 2)])
        self.assertEqual(rd.register_map(pairs), {(R["t1"], R["t2"]): 2, (R["t2"], R["t1"]): 2})

    def test_a_constant_without_a_partner_is_reported(self):
        draft = self.keys([addiu("t1", "t1", 1)])
        want = self.keys([addiu("t1", "t1", 2)])
        pairs, structural, left = rd.align(draft, want)
        self.assertEqual((pairs, structural), ([], []))
        self.assertEqual([(p.draft_index, p.target_index) for p in left], [(0, 0)])

    def test_other_instructions_are_structure(self):
        draft = self.keys([addu("v0", "a0", "a1"), nop()])
        want = self.keys([addu("v0", "a0", "a1"), addiu("v0", "v0", 1), nop()])
        pairs, structural, _l = rd.align(draft, want)
        self.assertEqual(pairs, [])
        self.assertEqual(structural[0][0], "insert")

    def test_a_folded_run_of_zero_words_is_a_nop(self):
        self.assertEqual(rd.key_word("..."), 0)
        pairs, structural, left = rd.align([key(addu("v0", "a0", "a1")), "...", key(jr_ra())],
                                           [key(addu("v0", "a0", "a1")), "...", key(jr_ra())])
        self.assertEqual((pairs, structural, left), ([], [], []))

    def test_trailing_padding_is_dropped(self):
        e = [("8c000000", "x"), ("00000000", "nop"), ("...", "...")]
        self.assertEqual(rd.strip_padding(e), [("8c000000", "x")])


class WebTest(unittest.TestCase):
    def test_lui_addiu_is_one_web_and_a_call_splits_nothing_it_does_not_clobber(self):
        words = [lui("s0", 0x8012), addiu("s0", "s0", 0x4394), lw("v0", 0, "s0"), jal(), nop(),
                 addu("v0", "s0", "v0"), jr_ra(), nop()]
        webs = rd.build_webs(insns(words))
        s0 = [w for w in webs if w.reg == R["s0"]]
        self.assertEqual(len(s0), 1)
        self.assertEqual((s0[0].defs, s0[0].uses), ([0, 1], [2, 5]))
        self.assertEqual(rd.web_constant(s0[0], insns(words)), 0x80124394)

    def test_a_call_clobbers_caller_saved_registers_and_a_spill_joins_the_pieces(self):
        words = [addu("t1", "a0", "a1"), sw("t1", 16, "sp"), jal(), nop(), lw("t1", 16, "sp"),
                 addu("v0", "t1", "t1"), jr_ra(), nop()]
        webs = [w for w in rd.build_webs(insns(words)) if w.reg == R["t1"]]
        self.assertEqual(len(webs), 1)
        self.assertEqual((webs[0].defs, webs[0].uses, webs[0].spill), ([0], [5], 2))

    def test_without_a_spill_the_value_after_a_call_is_a_new_web(self):
        words = [addu("t1", "a0", "a1"), jal(), nop(), addu("t1", "a0", "a1"), addu("v0", "t1", "t1"), jr_ra(), nop()]
        webs = sorted((w for w in rd.build_webs(insns(words)) if w.reg == R["t1"]), key=lambda w: w.first)
        self.assertEqual([(w.defs, w.uses) for w in webs], [([0], []), ([3], [4])])

    def test_the_delay_slots_of_a_return_and_a_call_define_the_value_they_pass(self):
        words = [jal(), addiu("a0", "zero", 1), addiu("v0", "zero", 7), jr_ra(), addiu("v0", "v0", 2)]
        webs = rd.build_webs(insns(words))
        self.assertEqual([(w.defs, w.uses) for w in webs if w.reg == R["a0"] and w.defs], [([1], [])])
        v0 = [w for w in webs if w.reg == R["v0"]]
        self.assertEqual(sorted(i for w in v0 for i in w.defs), [2, 4])
        self.assertEqual(len(v0), 1)        # `addiu v0,v0,2` updates the value in place

    def test_branch_joins_definitions_of_both_arms(self):
        # if (a0) v1 = 1; else v1 = 2; use v1
        words = [beq("a0", "zero", 4), nop(), addiu("v1", "zero", 1), beq("zero", "zero", 2), nop(),
                 addiu("v1", "zero", 2), addu("v0", "v1", "v1"), jr_ra(), nop()]
        v1 = [w for w in rd.build_webs(insns(words)) if w.reg == R["v1"]]
        self.assertEqual(len(v1), 1)
        self.assertEqual((v1[0].defs, v1[0].uses), ([2, 5], [6]))

    def test_web_at_finds_the_web_of_an_operand(self):
        words = [addu("t1", "a0", "a1"), addu("v0", "t1", "t1"), jr_ra(), nop()]
        ins = insns(words)
        webs = rd.build_webs(ins)
        self.assertEqual(rd.web_at(webs, 0, R["t1"], "def").defs, [0])
        self.assertEqual(rd.web_at(webs, 1, R["t1"], "use").defs, [0])


TRACE = """R9PROC f
R9POOL caller=1..13 saved=14..22 forbidden=00000000
R9SAVE bit=0 lr=0 blocks=2 priority=1.5 state=1
R9UNIT bb=0 freq=1 loads=1 stores=1 load=0 save=0 dead=0
R9UNIT bb=1 freq=1 loads=1 stores=0 load=0 save=0 dead=1
R9SAVE bit=2 lr=2 blocks=1 priority=1 state=1
R9UNIT bb=0 freq=10 loads=1 stores=0 load=0 save=0 dead=1
R9SAVE bit=7 lr=7 blocks=2 priority=0 state=2
R9UNIT bb=0 freq=1 loads=1 stores=0 load=1 save=0 dead=0
R9COST bit=0 lr=0 cost=0 reg=2
R9COST bit=2 lr=2 cost=0 reg=1
R9COST bit=7 lr=7 cost=2 reg=3
R9SAVE bit=7 lr=7 blocks=1 priority=0 state=2
R9UNIT bb=0 freq=1 loads=0 stores=1 load=0 save=1 dead=0
R9SAVE bit=7 lr=28 blocks=1 priority=0 state=2
R9UNIT bb=1 freq=1 loads=1 stores=0 load=1 save=0 dead=1
"""

LISTING = """ * *    0.102 SECONDS IN REMOVAL OF REDUNDANT STORES OF f
 index      bit kind    op      l      r
{1024|0}      0 isvar   M    4    -4vreg
\"\"\"           1
{ 493|0}      2 isvar   P    4     0vreg
\"\"\"           3
{1136|0}      4 isop    umpy{ 493|0} {   4|0}
{ 237|0}      5 isop    uadd{1136|0} {1024|0}
{ 938|0}      7 isvar   S    5     0
 * *    0.102 SECONDS IN CODE MOTION OF f
 * *    0.071 SECONDS IN reg alloc preparation OF f
   7:    7 not colored (-ve save)
   2:    2 assigned (constrained)   1
   0:    0 assigned (unconstrained)   2
live range   7:    7 split out   28
   7:   28 not colored (-ve save)
 * *    0.116 SECONDS IN global coloring of f
"""


class TraceTest(unittest.TestCase):
    def parsed(self):
        procs = rd.parse_stderr(TRACE)
        rd.parse_listing(LISTING, procs)
        return procs["f"]

    def test_ranges_units_and_allocation_order(self):
        p = self.parsed()
        r0 = p.ranges[0]
        self.assertEqual((r0.bit, r0.priority, r0.loads, r0.stores, r0.blocks), (0, 1.5, 2, 1, 2))
        self.assertEqual(r0.denominator, 2)
        self.assertEqual(p.sequence, [0, 2, 7])
        self.assertEqual([p.ranges[lr].order for lr in (0, 2, 7)], [0, 1, 2])
        self.assertEqual(p.ranges[2].units[0]["freq"], 10)

    def test_final_colours_modes_and_splits(self):
        p = self.parsed()
        self.assertEqual((p.ranges[2].colour, p.ranges[2].mode), (1, "constrained"))
        self.assertEqual((p.ranges[0].colour, p.ranges[0].mode), (2, "unconstrained"))
        finals = {r.lr: r for r in rd.final_ranges(p)}
        self.assertEqual(finals[28].split_from, 7)
        self.assertEqual(finals[28].reason, "-ve save")
        self.assertIsNone(finals[7].colour)
        self.assertEqual(finals[7].units[0]["stores"], 1)      # the recomputed piece, not the first record

    def test_expression_table(self):
        p = self.parsed()
        self.assertEqual(p.table[0]["cls"], "M")
        self.assertEqual((p.table[0]["l"], p.table[0]["r"]), (4, -4))
        self.assertEqual(p.table[4]["op"], "umpy")
        self.assertEqual(p.table[4]["ops"], [(493, 0), (4, 0)])
        self.assertEqual(p.max_table_bit, 7)

    def test_colour_numbers_name_registers(self):
        self.assertEqual(rd.COLOUR_REG[1], R["v0"])
        self.assertEqual(rd.COLOUR_REG[3], R["a0"])
        self.assertEqual(rd.COLOUR_REG[7], R["t0"])
        self.assertEqual(rd.COLOUR_REG[14], R["s0"])
        self.assertEqual(rd.REG_COLOUR[R["t5"]], 12)

    def test_denominator_compresses_long_ranges(self):
        r = rd.Range(1, 1)
        for blocks, want in ((1, 1), (2, 2), (3, 2), (6, 3), (10, 4)):
            r.blocks = blocks
            self.assertEqual(r.denominator, want)

    def test_expression_text_uses_variable_names(self):
        p = self.parsed()
        names = {("M", 4, -4): ("count", [10]), ("P", 4, 0): ("n", [9])}
        self.assertEqual(rd.render(p, names, 237), "add(mpy(n, #4), count)")


class NamesTest(unittest.TestCase):
    SRC = """s32 f(n)
s32 n;
{
    s32 count;
    s16 *p;

    count = n * 4;
    p = D_80123456;
    return count + *p;
}
"""

    def test_declared_names(self):
        params, locals_ = rd.declared_locals(self.SRC)
        self.assertEqual(params, ["n"])
        self.assertEqual(locals_, ["count", "p"])

    def test_variables_are_named_by_the_lines_that_mention_them(self):
        proc = rd.ProcTrace("f")
        lines = self.SRC.split("\n")
        refs = {("M", 4, -4): [7, 9], ("P", 4, 0): [7], ("S", 5, 0): [8]}
        names = rd.name_variables(proc, refs, lines, 1)
        self.assertEqual(names[("M", 4, -4)][0], "count")
        self.assertEqual(names[("P", 4, 0)][0], "n")
        self.assertEqual(names[("S", 5, 0)][0], "D_80123456")


def make_range(bit, priority, order, colour, loads=1, stores=0, mode="constrained", blocks=2, span=(0, 1), freq=1):
    r = rd.Range(bit, bit)
    r.priority, r.order, r.colour, r.mode, r.blocks = priority, order, colour, mode, blocks
    r.units = [{"bb": span[0], "freq": freq, "loads": loads, "stores": stores},
               {"bb": span[1], "freq": freq, "loads": 0, "stores": 0}]
    r.cost = 0
    return r


class ExplainTest(unittest.TestCase):
    def setUp(self):
        # draft: t1 = 0x439c (bit 68, first), t2 = 0x4394 (bit 69); the original has them the other way round
        self.words = [lui("t1", 0x8012), lui("t2", 0x8012), addiu("t1", "t1", 0x439C), addiu("t2", "t2", 0x4394),
                      lw("v0", 0, "t1"), lw("v1", 0, "t2"), jr_ra(), nop()]
        self.want = [lui("t1", 0x8012), lui("t2", 0x8012), addiu("t1", "t1", 0x4394), addiu("t2", "t2", 0x439C),
                     lw("v0", 0, "t1"), lw("v1", 0, "t2"), jr_ra(), nop()]
        self.ins = insns(self.words)
        self.proc = rd.ProcTrace("f")
        self.proc.max_table_bit = 66

    def run_explain(self, ranges):
        pairs, _s, _l = rd.align([key(w) for w in self.words], [key(w) for w in self.want])
        webs = rd.build_webs(self.ins)
        return rd.explain(self.ins, pairs, webs, ranges, self.proc, {})

    def test_equal_priorities_blame_the_bit_order(self):
        ranges = [make_range(68, 2.5, 0, rd.REG_COLOUR[R["t1"]]), make_range(69, 2.5, 1, rd.REG_COLOUR[R["t2"]])]
        text = "\n".join(self.run_explain(ranges))
        self.assertIn("bit 68", text)
        self.assertIn("constant 0x8012439C", text)
        self.assertIn("tie", text)
        self.assertIn("bit 69", text)
        self.assertIn("order the code first uses them", text)      # bits past the expression table
        self.assertIn("hoisted temporaries in bit order: 68 0x8012439C -> $t1; 69 0x80124394 -> $t2", text)

    def test_a_higher_priority_range_is_named_with_both_priorities(self):
        ranges = [make_range(68, 7.5, 0, rd.REG_COLOUR[R["t1"]]), make_range(69, 2.5, 1, rd.REG_COLOUR[R["t2"]])]
        text = "\n".join(self.run_explain(ranges))
        self.assertIn("priority 7.5", text)
        self.assertIn("priority 2.5", text)
        self.assertIn("raise", text)

    def test_a_ugen_temporary_is_not_blamed_on_the_allocator(self):
        words = [addu("t7", "a0", "a1"), addu("v0", "t7", "a0"), jr_ra(), nop()]
        want = [addu("t8", "a0", "a1"), addu("v0", "t8", "a0"), jr_ra(), nop()]
        pairs, _s, _l = rd.align([key(w) for w in words], [key(w) for w in want])
        text = "\n".join(rd.explain(insns(words), pairs, rd.build_webs(insns(words)), [], self.proc, {}))
        self.assertIn("not a uopt colour", text)


class OperandOrderTest(unittest.TestCase):
    def test_exchanged_sources_of_a_commutative_operation_are_a_swap_not_a_register_difference(self):
        draft = [key(beq("v1", "v0", 3)), key(addu("t0", "a0", "a1")), key(nop())]
        want = [key(beq("v0", "v1", 3)), key(addu("t0", "a1", "a0")), key(nop())]
        pairs, structural, left = rd.align(draft, want)
        self.assertEqual((structural, left), ([], []))
        self.assertEqual([p.swap for p in pairs], [True, True])
        self.assertIsNone(rd.first_divergence(pairs))
        self.assertEqual(rd.register_map(pairs), {})

    def test_a_non_commutative_operation_is_not_a_swap(self):
        a, b = rd.Decoded(rtype("a0", "a1", "v0", 0, 0x23)), rd.Decoded(rtype("a1", "a0", "v0", 0, 0x23))   # subu
        self.assertFalse(rd.is_swap(a, b))
        a, b = rd.Decoded(addu("v0", "a0", "a1")), rd.Decoded(addu("v1", "a1", "a0"))      # other destination
        self.assertFalse(rd.is_swap(a, b))

    def test_a_swap_next_to_a_real_difference_leaves_the_real_one_first(self):
        draft = [key(beq("v1", "v0", 3)), key(addu("t1", "a0", "a1"))]
        want = [key(beq("v0", "v1", 3)), key(addu("t2", "a0", "a1"))]
        pairs, _s, _l = rd.align(draft, want)
        p, op = rd.first_divergence(pairs)
        self.assertEqual((p.draft_index, op[2], op[3]), (1, R["t1"], R["t2"]))


class OccupantTest(unittest.TestCase):
    def test_occupants_are_the_values_alive_inside_an_interval(self):
        words = [addiu("v0", "zero", 1), addiu("v1", "zero", 2), addu("a0", "v0", "v1"), addu("a1", "v0", "a0"),
                 jr_ra(), nop()]
        webs = rd.build_webs(insns(words))
        v0 = rd.web_at(webs, 0, R["v0"], "def")
        self.assertEqual((v0.first, v0.last), (0, 3))
        self.assertEqual([w.defs for w in rd.occupants(webs, R["v0"], 1, 2)], [[0]])
        self.assertEqual(rd.occupants(webs, R["v0"], 3, 5), [])

    def test_scratch_sequence_lists_the_ugen_temporaries_in_order(self):
        words = [addu("t6", "a0", "a1"), addu("t7", "t6", "a0"), addu("t6", "t7", "t7"), addu("v0", "t6", "a0")]
        self.assertEqual(rd.scratch_sequence(insns(words)), ["t6", "t7", "t6"])

    def test_constants_by_value_and_the_table_of_differences(self):
        d = insns([lui("t1", 0x8012), addiu("t1", "t1", 0x4394), lui("t2", 0x8012), addiu("t2", "t2", 0x439C),
                   addu("v0", "t1", "t2"), addiu("t3", "zero", 4), addu("v1", "v0", "t3"), jr_ra(), nop()])
        o = insns([lui("t2", 0x8012), addiu("t2", "t2", 0x4394), lui("t1", 0x8012), addiu("t1", "t1", 0x439C),
                   addu("v0", "t2", "t1"), addiu("t3", "zero", 4), addu("v1", "v0", "t3"), jr_ra(), nop()])
        wd, wo = rd.build_webs(d), rd.build_webs(o)
        self.assertEqual(rd.constants_by_value(d, wd), {0x80124394: [R["t1"]], 0x8012439C: [R["t2"]], 4: [R["t3"]]})
        lines = rd.constant_table(d, wd, o, wo)
        self.assertEqual(len(lines), 1)
        self.assertIn("0x80124394: draft $t1, original $t2", lines[0])
        self.assertNotIn("0x4:", lines[0])

    def test_hoisted_sequence_orders_by_bit(self):
        d = insns([lui("t1", 0x8012), addiu("t1", "t1", 0x4394), addiu("t2", "zero", 4), addu("v0", "t1", "t2"),
                   jr_ra(), nop()])
        webs = rd.build_webs(d)
        a, b = make_range(70, 2.5, 1, rd.REG_COLOUR[R["t2"]]), make_range(69, 2.5, 0, rd.REG_COLOUR[R["t1"]])
        got = rd.assign_ranges([a, b], webs)
        webs_of = {id(r): w for w in webs for (r, _e) in [got.get(id(w), (None, 0))] if r is not None}
        self.assertEqual(rd.hoisted_sequence([a, b], webs_of, d), ["69 0x80124394 -> $t1", "70 0x4 -> $t2"])


class AssignTest(unittest.TestCase):
    def test_ranges_sharing_a_register_follow_program_order_and_counts(self):
        words = [addu("v0", "a0", "a1"), addu("v1", "v0", "v0"), addu("v0", "v1", "a1"), addu("v1", "v0", "v0"),
                 jr_ra(), nop()]
        webs = rd.build_webs(insns(words))
        v0 = rd.REG_COLOUR[R["v0"]]
        first = make_range(1, 3, 0, v0, loads=2, stores=1, span=(0, 0))
        second = make_range(2, 3, 1, v0, loads=2, stores=1, span=(1, 1))
        got = rd.assign_ranges([second, first], webs)
        by_def = {w.defs[0]: got[id(w)][0].bit for w in webs if w.reg == R["v0"] and w.defs}
        self.assertEqual(by_def, {0: 1, 2: 2})


REPORT_TRACE = """R9PROC f
R9POOL caller=1..13 saved=14..22 forbidden=00000000
R9SAVE bit=68 lr=68 blocks=2 priority=2.5 state=1
R9UNIT bb=1 freq=10 loads=1 stores=0 load=1 save=0 dead=0
R9SAVE bit=69 lr=69 blocks=2 priority=2.5 state=1
R9UNIT bb=1 freq=10 loads=1 stores=0 load=1 save=0 dead=0
R9COST bit=68 lr=68 cost=0 reg=8
R9COST bit=69 lr=69 cost=0 reg=9
"""
REPORT_LISTING = """ * *    0.102 SECONDS IN CODE MOTION OF f
 * *    0.071 SECONDS IN reg alloc preparation OF f
  68:   68 assigned (constrained)   8
  69:   69 assigned (constrained)   9
 * *    0.116 SECONDS IN global coloring of f
"""


class ReportTest(unittest.TestCase):
    def entries(self, words):
        return [(key(w), "%s  ; i%d" % (key(w), k)) for k, w in enumerate(words)]

    def run_report(self, draft, want):
        import argparse
        import os
        import tempfile
        with tempfile.TemporaryDirectory() as tmp:
            src = os.path.join(tmp, "f.c")
            text = "void f(void) {\n}\n"
            with open(src, "w") as f:
                f.write(text)
            comp = rd.Compiled()
            comp.base_text, comp.log, comp.listing, comp.ucode = text, REPORT_TRACE, REPORT_LISTING, b""
            args = argparse.Namespace(src=src, verbose=False)
            unit = argparse.Namespace(unit="TT")
            lines = []
            rc = rd.report(args, "f", unit, src, comp, self.entries(draft), self.entries(want), (0, 0, 0), lines.append)
        return rc, "\n".join(lines)

    def test_identical_functions_report_no_divergence(self):
        words = [addu("v0", "a0", "a1"), jr_ra(), nop(), nop()]
        rc, text = self.run_report(words, words[:3])
        self.assertEqual(rc, 0)
        self.assertIn("MATCH: identical instructions, no divergence", text)

    def test_swapped_address_constants_are_explained(self):
        draft = [lui("t1", 0x8012), addiu("t1", "t1", 0x439C), lui("t2", 0x8012), addiu("t2", "t2", 0x4394),
                 lw("v0", 0, "t1"), lw("v1", 0, "t2"), jr_ra(), nop()]
        want = [lui("t1", 0x8012), addiu("t1", "t1", 0x4394), lui("t2", 0x8012), addiu("t2", "t2", 0x439C),
                lw("v0", 0, "t2"), lw("v1", 0, "t1"), jr_ra(), nop()]
        rc, text = self.run_report(draft, want)
        self.assertEqual(rc, 1)
        self.assertIn("FIRST DIVERGENCE at draft insn", text)
        self.assertIn("constant 0x8012439C", text)
        self.assertIn("tie. Equal priority 2.5", text)
        self.assertIn("constants kept in registers that differ (2)", text)

    def test_structure_is_reported_before_registers(self):
        draft = [addu("v0", "a0", "a1"), jr_ra(), nop()]
        want = [addu("v0", "a0", "a1"), addiu("v0", "v0", 1), jr_ra(), nop()]
        rc, text = self.run_report(draft, want)
        self.assertEqual(rc, 1)
        self.assertIn("STRUCTURE differs in 1 place(s)", text)
        self.assertIn("+ original", text)

    def test_only_an_operand_order_difference(self):
        draft = [beq("v1", "v0", 3), nop(), nop(), jr_ra(), nop()]
        want = [beq("v0", "v1", 3), nop(), nop(), jr_ra(), nop()]
        rc, text = self.run_report(draft, want)
        self.assertIn("operand order", text)
        self.assertIn("write the operands", text)
        self.assertNotIn("FIRST DIVERGENCE", text)


class DraftSourceTest(unittest.TestCase):
    def test_wrapper_template_formats(self):
        text = rd.WRAPPER % {"tools": "/work/tools"}
        compile(text, "wrapper", "exec")
        self.assertIn("REGDIFF_UCODE", text)


if __name__ == "__main__":
    unittest.main()
