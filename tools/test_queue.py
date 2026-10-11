#!/usr/bin/env python3
"""Tests for tools/queue.py with synthetic splat-style asm (no game data).

Run: tools/docker.sh python3 tools/test_queue.py
"""
import importlib.util
import io
import tempfile
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parent
spec = importlib.util.spec_from_file_location("work_queue", HERE / "queue.py")
wq = importlib.util.module_from_spec(spec)
import sys
sys.path.insert(0, str(HERE))
spec.loader.exec_module(wq)


def asm(name, body):
    """Splat-style .s text. body: list of strings, each an instruction or a `.Lxxxxxxxx:` label."""
    out = ["nonmatching %s, 0x0" % name, "", "glabel %s" % name]
    addr = 0x80010000
    for line in body:
        if line.startswith("."):
            out.append("  " + line)
            continue
        out.append("    /* %X %08X 00000000 */  %s" % (addr - 0x80000000, addr, line))
        addr += 4
    out.append("endlabel " + name)
    return "\n".join(out) + "\n"


def at(n):
    return ".L%08X:" % (0x80010000 + 4 * n)


def lbl(n):
    return ".L%08X" % (0x80010000 + 4 * n)


# the T-0018 shape: v1 loaded in three blocks, compare chain, increments after the call
PROMOTED = [
    "lui        $v1, %hi(D_800E738D)",
    "lbu        $v1, %lo(D_800E738D)($v1)",
    "addiu      $sp, $sp, -0x28",
    "bnez       $v1, " + lbl(15),
    " sw         $ra, 0x14($sp)",
    "jal        func_80044E8C",
    " nop",
    "lui        $v1, %hi(D_800E738D)",
    "lbu        $v1, %lo(D_800E738D)($v1)",
    "lui        $at, %hi(D_800E738D)",
    "addiu      $t6, $v1, 0x1",
    "b          " + lbl(15),
    " sb         $t6, %lo(D_800E738D)($at)",
    "nop",
    "nop",
    at(15),
    "addiu      $at, $zero, 0x1",
    "bne        $v1, $at, " + lbl(19),
    " nop",
    "jr         $ra",
    " nop",
]


# unsigned global kept in $v1 and reloaded after a call, one compare only: not a switch (R)
RELOAD = [
    "lui        $v1, %hi(D_800E738D)",
    "lbu        $v1, %lo(D_800E738D)($v1)",
    "addiu      $sp, $sp, -0x28",
    "bnez       $v1, " + lbl(15),
    " sw         $ra, 0x14($sp)",
    "jal        func_80044E8C",
    " nop",
    "lui        $v1, %hi(D_800E738D)",
    "lbu        $v1, %lo(D_800E738D)($v1)",
    "lui        $at, %hi(D_800E738D)",
    "addiu      $t6, $v1, 0x1",
    "b          " + lbl(15),
    " sb         $t6, %lo(D_800E738D)($at)",
    "nop",
    "nop",
    at(15),
    "jr         $ra",
    " nop",
]


# a call before the first load: cvt_pass.py does not promote the global there (T-5010)
CALL_FIRST = ["jal        func_80010000", " nop"]


class Detector(unittest.TestCase):
    def test_switch_chain_on_unsigned_global_is_U_not_R_or_V(self):
        # T-5010: at the entry cvt_pass.py reproduces it; after a call it stays blocked (U1)
        f = wq.analyze(asm("f", PROMOTED))
        self.assertEqual(f.selector, "")
        self.assertFalse(f.reload)
        self.assertFalse(f.dispatch)
        f = wq.analyze(asm("f", CALL_FIRST + PROMOTED))
        self.assertEqual(f.selector, "v1")
        self.assertFalse(f.reload)
        self.assertFalse(f.dispatch)

    def test_reload_of_unsigned_global_in_two_blocks_is_R(self):
        f = wq.analyze(asm("f", CALL_FIRST + RELOAD))
        self.assertTrue(f.reload)
        self.assertEqual(f.selector, "")
        self.assertFalse(wq.analyze(asm("f", RELOAD)).reload)   # first load at the entry (T-5010)

    def test_old_rule_flags_both(self):
        f = wq.analyze(asm("f", PROMOTED), legacy=True)
        self.assertTrue(f.reload)
        self.assertTrue(f.dispatch)
        self.assertEqual(f.selector, "")

    def test_signed_and_word_globals_are_not_R(self):
        # T-1321: IDO keeps lw/lh/lb globals in a register by itself; the old rule over-reported
        for op in ("lw", "lh", "lb"):
            body = [l.replace("lbu", op) for l in RELOAD]
            self.assertFalse(wq.analyze(asm("f", body)).reload, op)
            self.assertTrue(wq.analyze(asm("f", body), legacy=True).reload, op)

    def test_v_shape_on_a_word_global_is_the_T_hint(self):
        # D++ compared in the same expression: old value in $v1, result in $v0 (unsigned compare)
        body = ["lui $v1, %hi(D_1)", "lw $v1, %lo(D_1)($v1)", "lui $at, %hi(D_1)", "sltiu $v0, $v1, 0x1",
                "addiu $v1, $v1, 0x1", "beqz $v0, " + lbl(7), " sw $v1, %lo(D_1)($at)", "nop", "jr $ra", " nop"]
        f = wq.analyze(asm("f", body))
        self.assertTrue(f.hint)
        self.assertFalse(f.dispatch)
        self.assertEqual(wq.Func("F", "f", "x", f).flags, "T")
        self.assertFalse(wq.Func("F", "f", "x", f).blocked)
        u = wq.analyze(asm("f", CALL_FIRST + [l.replace("lw ", "lbu ") for l in body]))
        self.assertTrue(u.dispatch)
        self.assertFalse(u.hint)
        self.assertFalse(wq.analyze(asm("f", [l.replace("lw ", "lbu ") for l in body])).dispatch)  # T-5010

    def test_jump_table_on_unsigned_global_is_U(self):
        body = ["lui $v0, %hi(D_1)", "lbu $v0, %lo(D_1)($v0)", "lui $at, %hi(jtbl_80010000)", "sltiu $at, $v0, 0x5",
                "beqz $at, " + lbl(9), " nop", "sll $t6, $v0, 2", "jr $t6", " nop", at(9), "jr $ra", " nop"]
        f = wq.analyze(asm("f", body))
        self.assertEqual(f.selector, "v0")
        self.assertEqual(wq.Func("F", "f", "x", f, island=True).flags, "JU0")

    def test_selector_must_start_the_function(self):
        body = ["nop"] * 12 + RELOAD[:0] + PROMOTED
        self.assertEqual(wq.analyze(asm("f", body)).selector, "")

    def test_flags_and_unknown(self):
        mk = lambda sel: wq.Func("F", "f", "x", wq.Facts(40, 0, False, False, False, False, False, False, sel))
        u1, u0 = mk("v1"), mk("v0")
        self.assertEqual((u1.flags, u0.flags), ("U1", "U0"))
        self.assertTrue(u1.blocked and u1.unknown and not u1.u0)
        self.assertTrue(u0.blocked and u0.unknown and u0.u0)
        self.assertFalse(func("F", "r", 8, flags="R").unknown)
        self.assertTrue(wq.allow_unknown(u0, "u0"))
        self.assertFalse(wq.allow_unknown(u1, "u0"))
        self.assertTrue(wq.allow_unknown(u1, "all"))
        self.assertFalse(wq.allow_unknown(u0, ""))

    def test_single_v1_use_is_not_V(self):
        # func_80042400 shape: v1 loaded, used once, v0 is the result
        body = ["lui $v1, %hi(D_1)", "lw $v1, %lo(D_1)($v1)", "lui $at, %hi(D_1)", "addiu $v0, $v1, 0x377",
                "jr $ra", " sw $v0, %lo(D_1)($at)"]
        f = wq.analyze(asm("f", body))
        self.assertFalse(f.dispatch)
        self.assertFalse(f.reload)

    def test_v0_load_is_not_flagged(self):
        body = ["lui $v0, %hi(D_1)", "lbu $v0, %lo(D_1)($v0)", "bnez $v0, " + lbl(7), " nop",
                "addiu $at, $zero, 1", "beq $v0, $at, " + lbl(7), " nop", at(7), "jr $ra", " nop"]
        f = wq.analyze(asm("f", body))
        self.assertFalse(f.dispatch)
        self.assertEqual(f.selector, "v0")   # but it is a U0 selector

    def test_v1_with_live_v0_is_not_V(self):
        # a call result in v0 is still read after the load: IDO has to use v1 (matched shape)
        body = ["jal func_8005742C", " nop", "lui $v1, %hi(D_1)", "lbu $v1, %lo(D_1)($v1)",
                "bne $v0, $v1, " + lbl(8), " nop", "beq $v1, $zero, " + lbl(8), " nop", at(8), "jr $ra", " nop"]
        f = wq.analyze(asm("f", body))
        self.assertFalse(f.dispatch)

    def test_array_index_load_is_not_a_global_load(self):
        body = ["sll $t6, $a0, 6", "lui $v1, %hi(D_1)", "addu $v1, $v1, $t6", "lbu $v1, %lo(D_1)($v1)",
                "bnez $v1, " + lbl(8), " nop", "beqz $v1, " + lbl(8), " nop", at(8), "jr $ra", " nop"]
        f = wq.analyze(asm("f", body))
        self.assertFalse(f.dispatch)
        self.assertFalse(f.reload)

    def test_argument_register_reload_is_not_R(self):
        body = ["lui $a0, %hi(D_1)", "lw $a0, %lo(D_1)($a0)", "jal f2", " nop",
                "lui $a0, %hi(D_1)", "lw $a0, %lo(D_1)($a0)", "jal f2", " nop", "jr $ra", " nop"]
        self.assertFalse(wq.analyze(asm("f", body)).reload)

    def test_reload_inside_loop_is_not_R(self):
        body = ["lui $v0, %hi(D_1)", at(1), "lbu $v0, %lo(D_1)($v0)", "jal g", " nop", "lui $v0, %hi(D_1)",
                "lbu $v0, %lo(D_1)($v0)", "bnez $v0, " + lbl(1), " nop", "jr $ra", " nop"]
        # the first load is outside the loop label? it is in a loop (label at 1 precedes both lbu)
        self.assertFalse(wq.analyze(asm("f", body)).reload)

    def test_different_registers_is_not_R(self):
        body = ["lui $t6, %hi(D_1)", "lw $t6, %lo(D_1)($t6)", "jal g", " nop",
                "lui $t7, %hi(D_1)", "lw $t7, %lo(D_1)($t7)", "jr $ra", " nop"]
        self.assertFalse(wq.analyze(asm("f", body)).reload)

    def test_jump_table_loop_calls_leaf(self):
        body = ["lui $at, %hi(jtbl_800AF7F0)", "lw $t8, %lo(jtbl_800AF7F0)($at)", at(2), "addiu $a0, $a0, -1",
                "bnez $a0, " + lbl(2), " nop", "jal g", " nop", "jr $ra", " nop"]
        f = wq.analyze(asm("f", body))
        self.assertTrue(f.jtbl)
        self.assertTrue(f.loop)
        self.assertEqual(f.calls, 1)
        self.assertEqual(f.size, 36)

    def test_forward_branch_is_no_loop(self):
        body = ["beqz $a0, " + lbl(4), " nop", "addiu $a0, $a0, 1", at(4), "jr $ra", " nop"]
        self.assertFalse(wq.analyze(asm("f", body)).loop)

    def test_string_reference(self):
        body = ["lui $a0, %hi(D_800AF340)", "addiu $a0, $a0, %lo(D_800AF340)", "jr $ra", " nop"]
        self.assertTrue(wq.analyze(asm("f", body), {"D_800AF340"}).strings)
        self.assertFalse(wq.analyze(asm("f", body), set()).strings)

    def test_trailing_single_nop(self):
        body = ["jr $ra", " nop", "nop"]
        self.assertTrue(wq.analyze(asm("f", ["addiu $v0, $zero, 1"] + body)).pad)
        self.assertFalse(wq.analyze(asm("f", ["addiu $v0, $zero, 1", "jr $ra", " nop"])).pad)


    def test_leading_nop_is_a_pad(self):
        """T-9030: DATE and ENDING func_80132000 start with a single nop."""
        f = wq.analyze(asm("f", ["nop", "jr $ra", " nop"]))
        self.assertTrue(f.pad)
        self.assertTrue(wq.Func("DATE", "func_80132000", "x.s", f).blocked)
        self.assertIn("P", wq.Func("DATE", "func_80132000", "x.s", f).flags)
        self.assertEqual(wq.p_match(wq.Func("DATE", "func_80132000", "x.s", f)), 0.0)
        self.assertFalse(wq.analyze(asm("f", ["jr $ra", " nop"])).pad)


def func(file, name, size, calls=0, flags=""):
    f = wq.Facts(size, calls, "L" in flags, "J" in flags, "S" in flags, "P" in flags, "R" in flags, "V" in flags)
    return wq.Func(file, name, "x.s", f)


class Ranking(unittest.TestCase):
    def test_unblocked_leaf_first_then_size(self):
        a = func("F", "a", 100)
        b = func("F", "b", 8, calls=1)
        c = func("F", "c", 4, flags="V")
        d = func("F", "d", 12)
        self.assertEqual([f.name for f in sorted([a, b, c, d], key=wq.rank_key)], ["d", "a", "b", "c"])

    def test_select_by_file(self):
        fs = [func("80041000", "a", 4), func("TEL", "b", 4), func("TT", "c", 4)]
        self.assertEqual([f.name for f in wq.select(fs, ["tel", "800410"])], ["a", "b"])
        self.assertEqual(len(wq.select(fs, [])), 3)

    def test_blocker_flags(self):
        self.assertTrue(func("F", "a", 4, flags="R").blocked)
        self.assertFalse(func("F", "a", 4, flags="L").blocked)


class Data(unittest.TestCase):
    def test_cases_table(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "c.md"
            p.write_text("text\n\n| file | function | category | symptom |\n|---|---|---|---|\n"
                         "| TEL | `func_8013A40C` | promo | chain |\n| 8005A0B0 | `GetWorkBase` | regorder | x |\n")
            self.assertEqual(wq.read_cases(p), [("TEL", "func_8013A40C", "promo", "chain"),
                                                ("8005A0B0", "GetWorkBase", "regorder", "x")])
            self.assertEqual(wq.read_cases(Path(d) / "none.md"), [])

    def test_dupes_groups(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "dupes.txt"
            p.write_text("func_80010000 func_80020000 func_80030000\nfunc_80040000\n")
            self.assertEqual(wq.read_dupes(p), {"func_80020000": "func_80010000", "func_80030000": "func_80010000"})

    def test_table_output(self):
        out = io.StringIO()
        wq.print_table([func("TEL", "func_1", 8, flags="LR")], {"func_1": "func_0"}, out)
        self.assertIn("LR", out.getvalue())
        self.assertIn("=func_0", out.getvalue())


class Project(unittest.TestCase):
    """load() over a tiny synthetic tree."""

    def test_load_remaining_and_matched(self):
        with tempfile.TemporaryDirectory() as d:
            r = Path(d)
            (r / "src/main").mkdir(parents=True)
            (r / "src/ovl").mkdir(parents=True)
            (r / "config").mkdir()
            (r / "config/overlays.txt").write_text("# none\n")
            (r / "config/SLPM_86.053.yaml").write_text(
                "segments:\n  - name: main\n    type: code\n    start: 0x800\n    vram: 0x80041000\n"
                "    subsegments:\n      - { start: 0x800, type: c, name: main/80041000 }\n")
            (r / "src/main/80041000.c").write_text(
                'INCLUDE_ASM("asm/nonmatchings/main/80041000", func_80041000);\n'
                "void func_80041010(void) {\n}\n")
            nm = r / "asm/nonmatchings/main/80041000"
            nm.mkdir(parents=True)
            (nm / "func_80041000.s").write_text(asm("func_80041000", ["jr $ra", " nop"]))
            mt = r / "asm/matchings/main/80041000"
            mt.mkdir(parents=True)
            (mt / "func_80041010.s").write_text(asm("func_80041010", ["jr $ra", " nop"]))
            remaining, matched = wq.load(r)
            self.assertEqual([(f.file, f.name, f.facts.size) for f in remaining], [("80041000", "func_80041000", 8)])
            self.assertEqual([f.name for f in matched], ["func_80041010"])

    def test_jump_table_in_an_orphan_chunk_is_flag_O(self):
        with tempfile.TemporaryDirectory() as d:
            r = Path(d)
            (r / "src/main").mkdir(parents=True)
            (r / "config/objects").mkdir(parents=True)
            (r / "config/overlays.txt").write_text("# none\n")
            (r / "config/SLPM_86.053.yaml").write_text(
                "segments:\n  - name: main\n    type: code\n    start: 0x800\n    vram: 0x80041000\n"
                "    subsegments:\n      - { start: 0x800, type: c, name: main/80041000 }\n")
            (r / "config/objects/main.txt").write_text("orphan 8015F730 8015FF70 second_chunk_of_801511E0\n")
            (r / "src/main/80041000.c").write_text(
                'INCLUDE_ASM("asm/nonmatchings/main/80041000", func_a);\n'
                'INCLUDE_ASM("asm/nonmatchings/main/80041000", func_b);\n')
            nm = r / "asm/nonmatchings/main/80041000"
            nm.mkdir(parents=True)
            body = ["lui $at, %hi(jtbl_{0})", "lw $t6, %lo(jtbl_{0})($at)", "jr $ra", " nop"]
            (nm / "func_a.s").write_text(asm("func_a", [x.format("8015F800") for x in body]))
            (nm / "func_b.s").write_text(asm("func_b", [x.format("8015E044") for x in body]))
            remaining, _m = wq.load(r)
            a, b = remaining
            self.assertEqual(a.facts.tables, (0x8015F800,))
            self.assertIn("O", a.flags)
            self.assertTrue(a.blocked)
            self.assertEqual(wq.p_match(a), 0.0)
            self.assertNotIn("O", b.flags)

    def test_orphan_flag_survives_the_cache_round_trip_and_blocks_with_an_island(self):
        f = wq.Func("F", "a", "x", wq.Facts(8, 0, False, True, False, False, False, False, tables=(0x80001000,)), True, "main", True)
        g = wq.func_from_json(__import__("json").loads(__import__("json").dumps(wq.func_to_json(f))))
        self.assertEqual(g, f)
        self.assertTrue(g.blocked and not g.unknown)


def sel(file, name, size, selector, island=False):
    return wq.Func(file, name, "x.s", wq.Facts(size, 0, False, False, False, False, False, False, selector), island)


class Bytes(unittest.TestCase):
    """T-3340: byte-weighted ranking."""

    def test_match_chance_by_flags(self):
        self.assertEqual(wq.p_match(func("F", "a", 8, flags="P")), 0.0)
        self.assertEqual(wq.p_match(func("F", "a", 8, flags="J")), 0.0)          # no island: cannot be built
        isl = wq.Func("F", "a", "x", wq.Facts(8, 0, False, True, False, False, False, False), True)
        self.assertAlmostEqual(wq.p_match(isl), 0.85 * 0.8)
        self.assertGreater(wq.p_match(func("F", "a", 8)), wq.p_match(func("F", "a", 8, calls=3)))
        self.assertGreater(wq.p_match(func("F", "a", 8)), wq.p_match(func("F", "a", 8, flags="L")))
        self.assertLess(wq.p_match(func("F", "a", 8, flags="R")), 0.3 * wq.p_match(func("F", "a", 8)))
        self.assertGreater(wq.p_match(sel("F", "a", 8, "v0")), 5 * wq.p_match(sel("F", "a", 8, "v1")))

    def test_size_weighs_against_effort(self):
        # tiny leaves are cheap but small; the middle sizes give the most bytes per effort; huge ones cost more
        scores = {n: wq.score_funcs([func("F", "f", n)])[0].score for n in (8, 120, 400, 4000)}
        self.assertGreater(scores[120], scores[8])
        self.assertGreater(scores[120], scores[400])
        self.assertGreater(scores[400], scores[4000])

    def test_flags_cost(self):
        a = wq.score_funcs([func("F", "a", 120)])[0].score
        for flags in ("R", "V"):
            self.assertLess(wq.score_funcs([func("F", "b", 120, flags=flags)])[0].score, a / 3)

    def groups(self, matched_member=False):
        fs = [func("TT/1", "func_a", 100), func("TT/1", "func_b", 120), func("TEL", "func_c", 100),
              func("TEL", "func_d", 100, flags="R")]
        for f in fs:
            self.assertEqual(f.unit_name, f.file.split("/")[0])
        members = [("TT", "func_a", False, 100, 1), ("TT", "func_b", False, 120, 2), ("TEL", "func_c", False, 100, 1),
                   ("TEL", "func_d", False, 100, 1)]
        if matched_member:
            members.append(("TT", "func_m", True, 100, 1))
        return fs, wq.group_index([members], fs)

    def test_group_without_matched_member_lists_one_representative(self):
        fs, gi = self.groups()
        reps = [n for (u, n), g in gi.items() if g.rep]
        self.assertEqual(len(reps), 1)
        self.assertIn(reps[0], ("func_a", "func_c"))           # unblocked, smallest
        rep = next(g for g in gi.values() if g.rep)
        self.assertEqual(rep.unmatched, 4)
        # two byte-identical twins (exact 1) at 0.9 and one near duplicate at 0.6, the blocked twin included
        self.assertAlmostEqual(rep.credit, 0.9 + 0.9 + 0.6)
        items = {sc.func.name: sc for sc in wq.score_funcs(fs, gi)}
        self.assertEqual(items["func_b"].score, 0.0)
        self.assertEqual(items["func_b"].dup, "=rep")
        top = max(items.values(), key=lambda sc: sc.score)
        self.assertEqual(top.dup, "rep+3")
        alone = wq.score_funcs([func("TT/1", "func_a", 100)])[0]
        self.assertGreater(top.score, 2 * alone.score)

    def test_group_with_a_matched_member_is_applied_by_the_copy_tools(self):
        fs, gi = self.groups(matched_member=True)
        for sc in wq.score_funcs(fs, gi):
            self.assertEqual(sc.score, 0.0)
            self.assertEqual(sc.dup, "apply:func_m")

    def test_groups_file_roundtrip(self):
        with tempfile.TemporaryDirectory() as d:
            g = [[("TT", "func_a", False, 100, 7), ("TEL", "func_c", True, 100, 7)]]
            wq.save_groups(g, Path(d) / "g.json")
            self.assertEqual(wq.load_groups_file(Path(d) / "g.json"), g)

    def test_plan_lists_are_balanced_and_never_share_a_file(self):
        fs = [func("A", "a%d" % i, 40 + 4 * i) for i in range(6)] + [func("B", "b%d" % i, 100) for i in range(2)] + \
             [func("C", "c%d" % i, 60) for i in range(3)] + [func("D", "d0", 80), func("E", "e0", 20)]
        items = wq.score_funcs(fs)
        lists = wq.plan_lists(items, 3, lambda sc: sc.eff)
        self.assertEqual(len(lists), 3)
        files = [{sc.func.file for sc in l} for l in lists]
        self.assertEqual(sum(len(f) for f in files), len(set().union(*files)))      # disjoint
        self.assertEqual(sum(len(l) for l in lists), len(fs))
        loads = [sum(sc.eff for sc in l) for l in lists]
        self.assertLess(max(loads) / min(loads), 1.5)
        # deterministic
        again = wq.plan_lists(wq.score_funcs(fs), 3, lambda sc: sc.eff)
        self.assertEqual([[sc.func.name for sc in l] for l in again], [[sc.func.name for sc in l] for l in lists])

    def test_plan_lists_by_unit(self):
        fs = [func("TT/1", "a", 40), func("TT/2", "b", 40), func("TEL", "c", 40)]
        lists = wq.plan_lists(wq.score_funcs(fs), 2, lambda sc: sc.eff, lambda sc: wq.unit_of(sc.func))
        units = [{wq.unit_of(sc.func) for sc in l} for l in lists]
        self.assertTrue(units[0].isdisjoint(units[1]))

    def test_more_lists_than_files_leaves_some_empty(self):
        lists = wq.plan_lists(wq.score_funcs([func("A", "a", 40), func("A", "b", 40)]), 3, lambda sc: sc.eff)
        self.assertEqual(sorted(len(l) for l in lists), [0, 0, 2])
        out = io.StringIO()
        wq.print_plan(lists, out)
        self.assertIn("no work left", out.getvalue())

    def test_plan_json(self):
        import json
        lists = wq.plan_lists(wq.score_funcs([func("A", "a", 40), func("B", "b", 80)]), 2, lambda sc: sc.eff)
        data = json.loads(wq.plan_json(lists))
        self.assertEqual(sorted(x["files"][0] for x in data), ["A", "B"])
        self.assertEqual(data[0]["functions"][0]["size"] in (40, 80), True)


class Calibration(unittest.TestCase):
    def test_rows_found_in_either_set_and_family_split(self):
        rem = [sel("TT/1", "func_p", 40, "v1"), sel("TT/1", "func_q", 40, "")]
        mat = [sel("TEL", "func_m", 40, "v0"), sel("TEL", "func_n", 40, ""), sel("TT/2", "func_x", 40, "")]
        cases = [("TT", "func_p", "promo", "compare chain on a global"), ("TT", "func_q", "promo", "`D = N; f(N)` constants"),
                 ("TEL", "func_m", "promo", "post-increment compare"), ("TT", "func_gone", "promo", "compare chain"),
                 ("TT", "func_x", "regorder", "other")]
        c = wq.calibration(rem, mat, cases)
        self.assertEqual([f.name for f in c["pos"]], ["func_p", "func_q", "func_m"])
        self.assertEqual([f.name for f in c["fam"]], ["func_p", "func_m"])
        self.assertEqual([f.name for f in c["neg"]], ["func_n"])        # func_m and func_x are table rows
        self.assertEqual(c["missing"], [("TT", "func_gone")])
        out = io.StringIO()
        res = wq.calibrate(rem, mat, cases, out)
        self.assertEqual(res["U1 ($v1 selector)"], (1, 1, 0))
        self.assertEqual(res["U0 ($v0 selector)"], (1, 1, 0))
        self.assertIn("family", out.getvalue())


class Cli(unittest.TestCase):
    def tree(self, d):
        r = Path(d)
        (r / "src/main").mkdir(parents=True)
        (r / "config").mkdir()
        (r / "config/overlays.txt").write_text("# none\n")
        (r / "config/SLPM_86.053.yaml").write_text(
            "segments:\n  - name: main\n    type: code\n    start: 0x800\n    vram: 0x80041000\n"
            "    subsegments:\n      - { start: 0x800, type: c, name: main/80041000 }\n")
        names = ["func_80041000", "func_80041010", "func_80041020"]
        (r / "src/main/80041000.c").write_text("".join(
            'INCLUDE_ASM("asm/nonmatchings/main/80041000", %s);\n' % n for n in names))
        nm = r / "asm/nonmatchings/main/80041000"
        nm.mkdir(parents=True)
        for n, extra in zip(names, ([], ["addiu $v0, $zero, 1"], ["addiu $v0, $zero, 1", "addiu $v0, $v0, 1"])):
            (nm / (n + ".s")).write_text(asm(n, extra + ["jr $ra", " nop"]))
        return r

    def run_cli(self, *args):
        import contextlib
        buf = io.StringIO()
        with contextlib.redirect_stdout(buf), contextlib.redirect_stderr(io.StringIO()):
            rc = wq.main(list(args))
        return rc, buf.getvalue()

    @unittest.skipUnless(importlib.util.find_spec("yaml"), "needs PyYAML (runs in Docker)")
    def test_bytes_next_and_plan(self):
        with tempfile.TemporaryDirectory() as d:
            r = self.tree(d)
            rc, out = self.run_cli("--root", str(r), "--by", "bytes", "--no-groups", "--next", "2")
            self.assertEqual(rc, 0)
            self.assertIn("score", out)
            self.assertEqual(len(out.strip().splitlines()), 3)
            rc, out = self.run_cli("--root", str(r), "--by", "bytes", "--no-groups", "--plan", "3", "--agents", "2")
            self.assertEqual(rc, 0)
            self.assertIn("plan: 3 functions", out)
            self.assertIn("agent 2", out)
            rc, out = self.run_cli("--root", str(r), "--plan", "3", "--agents", "2", "--no-groups", "--json")
            self.assertEqual(rc, 0)
            self.assertEqual(out.count('"name"'), 3)


@unittest.skipUnless(importlib.util.find_spec("yaml"), "needs PyYAML (runs in Docker)")
class CacheTest(unittest.TestCase):
    """T-5030: queue.py over many files ran for minutes and Docker cut the connection."""

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.r = Cli().tree(self.tmp.name)
        (self.r / "build").mkdir()

    def run_cli(self, *args):
        import contextlib
        out, err = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            rc = wq.main(list(args) + ["--root", str(self.r)])
        return rc, out.getvalue(), err.getvalue()

    def test_second_run_reads_no_asm_and_gives_the_same_table(self):
        rc, first, err = self.run_cli("--by", "bytes", "--no-groups", "--next", "5")
        self.assertIn("reading the asm", err)
        self.assertTrue((self.r / wq.CACHE).exists())
        real = wq.analyze
        wq.analyze = lambda *a, **k: self.fail("asm was analysed again")
        try:
            rc, second, err = self.run_cli("--by", "bytes", "--no-groups", "--next", "5")
        finally:
            wq.analyze = real
        self.assertEqual((rc, second), (0, first))
        self.assertEqual(err, "")

    def test_edited_asm_invalidates_the_cache(self):
        self.run_cli("--summary")
        p = self.r / "asm/nonmatchings/main/80041000/func_80041000.s"
        p.write_text(asm("func_80041000", ["addiu $v0, $zero, 7"] * 6 + ["jr $ra", " nop"]))
        rc, out, err = self.run_cli("--by", "bytes", "--no-groups", "--next", "5")
        self.assertIn("reading the asm", err)
        self.assertIn("32", out)    # the new size, not the cached one

    def test_no_cache_flag_and_missing_build_dir(self):
        self.run_cli("--no-cache", "--summary")
        self.assertFalse((self.r / wq.CACHE).exists())
        import shutil
        shutil.rmtree(self.r / "build")
        rc, _out, _err = self.run_cli("--summary")
        self.assertEqual(rc, 0)
        self.assertFalse((self.r / wq.CACHE).exists())

    def test_groups_are_cached_too(self):
        calls = []
        real = wq.compute_groups
        wq.compute_groups = lambda root: calls.append(1) or [[("main", "func_80041000", False, 8, "x"),
                                                                ("main", "func_80041010", False, 12, "x")]]
        try:
            rc, a, _e = self.run_cli("--by", "bytes", "--next", "3")
            rc, b, _e = self.run_cli("--by", "bytes", "--next", "3")
        finally:
            wq.compute_groups = real
        self.assertEqual(calls, [1])
        self.assertEqual(a, b)
        self.assertIn("rep+1", a)

    def test_files_limit_what_is_read_without_groups(self):
        seen = []
        real = wq.load
        wq.load = lambda root, string_syms=None, legacy=False, files=None: seen.append(files) or real(
            root, string_syms, legacy, files)
        try:
            rc, out, _e = self.run_cli("--files", "80041000", "--next", "2", "--no-cache")
            self.run_cli("--files", "80041000", "--by", "bytes", "--no-groups", "--next", "2", "--no-cache")
        finally:
            wq.load = real
        self.assertEqual(seen, [["80041000"], []])
        self.assertIn("func_80041000", out)

    def test_many_files_filter_matches_prefixes(self):
        names = ",".join(["80041000"] + ["NOPE%d" % i for i in range(30)])
        rc, out, _e = self.run_cli("--files", names, "--next", "10")
        self.assertEqual(rc, 0)
        self.assertEqual(out.count("func_8004"), 3)
        rc, out, _e = self.run_cli("--files", "ZZZ", "--next", "10")
        self.assertEqual(rc, 0)

    def test_json_roundtrip_of_a_func(self):
        f = func_obj = wq.Func("TEL", "func_1", "p.s", wq.Facts(8, 1, True, False, True, False, False, False, "v0", True),
                               True, "TEL")
        self.assertEqual(wq.func_from_json(__import__("json").loads(__import__("json").dumps(wq.func_to_json(f)))), func_obj)


if __name__ == "__main__":
    unittest.main()
