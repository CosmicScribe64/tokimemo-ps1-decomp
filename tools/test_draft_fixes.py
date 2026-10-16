#!/usr/bin/env python3
"""Tests for tools/draft_fixes.py and its hooks in tools/m2c.py and tools/m2c_args.py (T-16510).

The C pairs are real m2c output and the hand fix from wiki/matching-notes.md and the tickets named in
each test (T-12701, T-15300, T-15400, T-15600, T-16210); the asm is synthetic.

Run: tools/docker.sh python3 tools/test_draft_fixes.py   (from the repo root)
"""
import json
import os
import pathlib
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import draft_fixes as df  # noqa: E402
import m2c  # noqa: E402
import m2c_args  # noqa: E402


def asm(*items, name="f"):
    """A function .s: strings starting with `.L` or `jlabel` are label lines, the rest instructions."""
    out, n = ["nonmatching %s, 0x40" % name, "", "glabel %s" % name], 0
    for item in items:
        if item.startswith(".L") or item.startswith("jlabel"):
            out.append("  " + item)
        else:
            out.append("    /* %X %08X 00000000 */  %s" % (n * 4, 0x80100000 + n * 4, item))
            n += 1
    out.append("endlabel %s" % name)
    return "\n".join(out) + "\n"


# T-12701, BUNKAKEN func_80141088: m2c's output and the direct stores of the matched function.
BUNKAKEN_BEFORE = """void func_80141088(void) {
    s16 var_v0;
    s16 var_v0_2;
    u32 temp_v0_2;

    switch (D_800E738A) {
    case 10:
        temp_v0_2 = func_8008667C(2);
        switch (temp_v0_2) {
        case 0:
        case 1:
            var_v0 = 5;
block_33:
            D_800CA148 = var_v0;
            break;
        case 2:
            var_v0 = 6;
            goto block_33;
        case 3:
            var_v0 = 7;
            goto block_33;
        case 4:
            var_v0 = 8;
            goto block_33;
        }
        var_v0_2 = D_800CA148;
        if (D_80122EA4 == 2) {
            var_v0_2 = 8;
        }
        D_800CA148 = (var_v0_2 + (D_800E62BE * 8)) - 0x2F8;
        break;
    }
}
"""
BUNKAKEN_AFTER = """void func_80141088(void) {
    s16 var_v0_2;
    u32 temp_v0_2;

    switch (D_800E738A) {
    case 10:
        temp_v0_2 = func_8008667C(2);
        switch (temp_v0_2) {
        case 0:
        case 1:
            D_800CA148 = 5;
            break;
        case 2:
            D_800CA148 = 6;
            break;
        case 3:
            D_800CA148 = 7;
            break;
        case 4:
            D_800CA148 = 8;
            break;
        }
        var_v0_2 = D_800CA148;
        if (D_80122EA4 == 2) {
            var_v0_2 = 8;
        }
        D_800CA148 = (var_v0_2 + (D_800E62BE * 8)) - 0x2F8;
        break;
    }
}
"""


class GotoSharedStore(unittest.TestCase):
    def test_t12701_bunkaken_switch(self):
        result = df.analyze(BUNKAKEN_BEFORE)
        self.assertEqual(result.body, BUNKAKEN_AFTER)
        self.assertEqual([(f.kind, f.fixed) for f in result.findings], [("goto-shared-store", True)])

    def test_report_only_leaves_the_body(self):
        result = df.analyze(BUNKAKEN_BEFORE, rewrite=False)
        self.assertEqual(result.body, BUNKAKEN_BEFORE)
        self.assertEqual([(f.kind, f.fixed) for f in result.findings], [("goto-shared-store", False)])

    def test_select_skips_a_label(self):
        text = BUNKAKEN_BEFORE
        self.assertEqual(df.analyze(text, select=lambda kind, n: False).body, text)

    def test_value_that_calls_moves_into_the_only_store(self):
        before = """void f(void) {
    s32 var_t0;

    switch (D_8015EDE4) {
    case 1:
        var_t0 = g(1) - 1;
block_15:
        D_8015EDE8 = var_t0;
        break;
    case 2:
        var_t0 = g(2) - 1;
        goto block_15;
    }
}
"""
        after = """void f(void) {

    switch (D_8015EDE4) {
    case 1:
        D_8015EDE8 = g(1) - 1;
        break;
    case 2:
        D_8015EDE8 = g(2) - 1;
        break;
    }
}
"""
        self.assertEqual(df.analyze(before).body, after)

    def test_function_pointer_temporary_and_a_pointer_that_stays(self):
        before = """void f(void) {
    s32 (**temp_v0)(s32, u8);
    s32 (*var_v1)(s32, u8);

    temp_v0 = &D_80100000[D_800B1AF6];
    switch (D_801243B0) {
    case 1:
        var_v1 = a;
block_23:
        *temp_v0 = var_v1;
        break;
    case 2:
        var_v1 = b;
        goto block_23;
    }
}
"""
        after = """void f(void) {
    s32 (**temp_v0)(s32, u8);

    temp_v0 = &D_80100000[D_800B1AF6];
    switch (D_801243B0) {
    case 1:
        *temp_v0 = a;
        break;
    case 2:
        *temp_v0 = b;
        break;
    }
}
"""
        self.assertEqual(df.analyze(before).body, after)

    def test_assignment_moves_past_an_unrelated_store(self):
        before = """void f(void) {
    s32 var_t9;

    switch (D_1) {
    case 1:
        var_t9 = D_8015D050;
        D_8015D070 = D_8015D01C;
block_23:
        D_8015D074 = var_t9;
        break;
    case 2:
        var_t9 = D_8015D068;
        D_8015D070 = D_8015D034;
        goto block_23;
    }
}
"""
        after = df.analyze(before).body
        self.assertIn("        D_8015D070 = D_8015D034;\n        D_8015D074 = D_8015D068;\n        break;", after)
        self.assertNotIn("var_t9", after)

    def test_blocked_cases_stay_gotos(self):
        cases = {
            "tail-not-store": "        func_a();\n        break;",
            "tail-not-straight-line": "        if (x) {\n        }\n        break;",
            "loop-label": None,
        }
        for why, tail in cases.items():
            if tail is None:
                text = "void f(void) {\nloop_1:\n    D_1 += 1;\n    if (D_1 < 5) {\n        goto loop_1;\n    }\n}\n"
            else:
                text = ("void f(void) {\n    switch (D_0) {\n    case 1:\nblock_1:\n%s\n    case 2:\n        goto block_1;\n    }\n}\n"
                        % tail)
            result = df.analyze(text)
            self.assertEqual(result.body, text, why)
            self.assertIn(why, [f.detail.split(": ")[1] for f in result.findings if f.kind.endswith("-blocked")])

    def test_break_inside_an_inner_loop_is_not_moved(self):
        text = """void f(void) {
    s32 var_v0;

    switch (D_0) {
    case 1:
        var_v0 = 1;
block_1:
        D_1 = var_v0;
        break;
    case 2:
        while (D_2) {
            var_v0 = 2;
            goto block_1;
        }
        break;
    }
}
"""
        result = df.analyze(text)
        self.assertEqual(result.body, text)
        self.assertEqual(result.findings[0].detail, "block_1: break-leaves-another-construct")

    def test_temporary_read_elsewhere_blocks(self):
        text = BUNKAKEN_BEFORE.replace("var_v0_2 = D_800CA148;", "var_v0_2 = var_v0;")
        result = df.analyze(text)
        self.assertEqual(result.body, text)
        self.assertEqual(result.findings[0].detail, "block_33: temporary-used-elsewhere")

    def test_literal_too_wide_for_the_temporary_blocks(self):
        text = BUNKAKEN_BEFORE.replace("var_v0 = 7;", "var_v0 = 0x12345;")
        self.assertEqual(df.analyze(text).body, text)

    def test_value_read_after_another_store_blocks(self):
        before = """void f(void) {
    s32 var_v0;

    switch (D_0) {
    case 1:
        var_v0 = 1;
block_1:
        D_1 = 3;
        *p = var_v0;
        break;
    case 2:
        var_v0 = D_5;
        goto block_1;
    }
}
"""
        result = df.analyze(before)
        self.assertEqual(result.body, before)
        self.assertIn("value-read-after-store", result.findings[0].detail)


class Strings(unittest.TestCase):
    def test_t15400_pasokon_label(self):
        before = 'func_8006612C((s32) "パ\\x83\\\\コン占い");\n'
        self.assertEqual(df.analyze(before).body, 'func_8006612C((s32) "パソコン占い");\n')

    def test_second_byte_backslash_pairs(self):
        for before, after in (('"結果発\\x95\\\\ "', '"結果発表 "'), ('"カー\\x83\\\\ル速度"', '"カーソル速度"'),
                              ('"\\x95\\\\示しない"', '"表示しない"')):
            self.assertEqual(df.analyze("f(%s);\n" % before).body, "f(%s);\n" % after)

    def test_octal_bytes_become_text(self):
        self.assertEqual(df.analyze('f("\\203K\\203`\\203\\203");\n').body, 'f("ガチャ");\n')

    def test_ascii_escapes_survive(self):
        self.assertEqual(df.analyze('f("a\\n\\x83\\\\b\\"c\\\\");\n').body, 'f("a\\nソb\\"c\\\\");\n')

    def test_hex_below_0x80_becomes_octal(self):
        self.assertEqual(df.analyze('f("\\x1bZ");\n').body, 'f("\\033Z");\n')

    def test_comments_plain_text_and_escaped_backslash_are_left(self):
        text = '/* "\\x83\\\\" */ f("\\\\x83", "plain \\n");\n'
        self.assertEqual(df.analyze(text).body, text)

    def test_broken_pair_is_reported_and_left(self):
        text = 'f("\\x83\\x20");\n'
        result = df.analyze(text)
        self.assertEqual(result.body, text)
        self.assertEqual([f.kind for f in result.findings], ["sjis-escape-blocked"])

    def test_text_round_trips_through_the_build_encoding(self):
        import cc
        fixed = df.analyze('f("カー\\x83\\\\ル");\n').body
        octal = "".join("\\%03o" % b for b in "カーソル".encode("shift_jis"))
        self.assertEqual(cc.sjis_literals(fixed), 'f("%s");\n' % octal)


# SHOUGATU func_80137144 (T-15600): the compare reads the old value, the add is in place.
POSTINC_ASM = asm("lui $v1, %hi(D_80100000)", "lw $v1, %lo(D_80100000)($v1)", "lui $at, %hi(D_80100000)",
                  "sltiu $v0, $v1, 0x10", "xori $v0, $v0, 0x1", "addiu $v1, $v1, 0x1", "beqz $v0, .L80100040",
                  "sw $v1, %lo(D_80100000)($at)", "jr $ra", "nop")
POSTINC_BEFORE = """void f(void) {
    if (g()) {
    } else {
        D_80100000 += 1;
        if ((u32) D_80100000 >= 0x10U) {
            h(2);
        }
    }
}
"""
POSTINC_AFTER = POSTINC_BEFORE.replace("        D_80100000 += 1;\n        if ((u32) D_80100000 >=",
                                       "        if ((u32) D_80100000++ >=")


class PostIncrement(unittest.TestCase):
    def test_in_place_increment_after_the_compare_is_old_value(self):
        self.assertEqual(m2c_args.post_increment_compares(POSTINC_ASM), {"D_80100000": {"old"}})
        self.assertEqual(m2c_args.post_increment_old(POSTINC_ASM), {"D_80100000"})

    def test_t15600_rewrite(self):
        result = df.analyze(POSTINC_BEFORE, POSTINC_ASM)
        self.assertEqual(result.body, POSTINC_AFTER)
        self.assertEqual([(f.kind, f.fixed) for f in result.findings if f.kind.startswith("postinc")],
                         [("postinc-compare", True)])

    def test_report_only_and_no_asm(self):
        self.assertEqual(df.analyze(POSTINC_BEFORE, POSTINC_ASM, rewrite=False).body, POSTINC_BEFORE)
        self.assertEqual(df.analyze(POSTINC_BEFORE).body, POSTINC_BEFORE)

    def test_new_value_compare_is_not_rewritten(self):
        new_asm = asm("lui $v0, %hi(D_80100000)", "lw $v0, %lo(D_80100000)($v0)", "lui $at, %hi(D_80100000)",
                      "addiu $v0, $v0, 0x1", "sw $v0, %lo(D_80100000)($at)", "sltiu $v1, $v0, 0x10", "jr $ra", "nop")
        self.assertEqual(m2c_args.post_increment_compares(new_asm), {"D_80100000": {"new"}})
        self.assertEqual(df.analyze(POSTINC_BEFORE, new_asm).body, POSTINC_BEFORE)

    def test_wrong_post_increment_is_taken_back(self):
        new_asm = asm("lui $v0, %hi(D_80100000)", "lw $v0, %lo(D_80100000)($v0)", "lui $at, %hi(D_80100000)",
                      "addiu $v0, $v0, 0x1", "sw $v0, %lo(D_80100000)($at)", "sltiu $v1, $v0, 0x10", "jr $ra", "nop")
        wrong = POSTINC_AFTER
        result = df.analyze(wrong, new_asm)
        self.assertEqual(result.body, POSTINC_BEFORE)
        self.assertEqual([f.kind for f in result.findings if f.fixed], ["postinc-wrong"])

    def test_separate_register_still_old(self):
        other = asm("lw $v1, %lo(D_80100000)($v1)", "addiu $v0, $v1, 0x1", "sw $v0, %lo(D_80100000)($at)",
                    "sltiu $a0, $v1, 0x10", "jr $ra", "nop")
        self.assertEqual(m2c_args.post_increment_compares(other), {"D_80100000": {"old"}})

    def test_m2c_postprocess_hook(self):
        raw = "extern u32 D_80100000;\n\n" + POSTINC_BEFORE
        logged = []
        fixed = m2c.fix_defects(raw, "f", POSTINC_ASM, logged.append)
        self.assertEqual(fixed, "extern u32 D_80100000;\n\n" + POSTINC_AFTER)
        self.assertEqual(len(logged), 1)
        self.assertIn("postinc-compare", logged[0])
        self.assertEqual(m2c.fix_defects("no function here\n", "f", POSTINC_ASM, logged.append), "no function here\n")


class ReturnValue(unittest.TestCase):
    def test_copy_in_a_branch_delay_slot_reaches_the_return(self):
        a = df.parse_asm(asm("lbu $v1, 0($a0)", "beqz $v1, .L80100014", "or $v0, $v1, $zero", "jal g", "nop",
                             ".L80100014:", "jr $ra", "nop"))
        self.assertEqual(df.exit_v0_defs(a), [(2, "copy", True)])

    def test_zero_on_an_early_exit(self):
        a = df.parse_asm(asm("bnez $a0, .L80100010", "nop", "jr $ra", "move $v0, $zero", ".L80100010:", "jal g",
                             "nop", "jr $ra", "nop"))
        self.assertEqual([k for _, k, _ in df.exit_v0_defs(a)], ["zero"])

    def test_read_value_call_result_and_plain_void_are_not_reported(self):
        for items in (("or $v0, $a0, $zero", "sw $v0, 0($a1)", "jr $ra", "nop"),
                      ("jal g", "nop", "jr $ra", "nop"),
                      ("sw $zero, 0($a1)", "jr $ra", "nop"),
                      ("or $v0, $a0, $zero", "jal g", "nop", "jr $ra", "nop")):
            self.assertEqual(df.exit_v0_defs(df.parse_asm(asm(*items))), [], items)

    def test_jump_table_exits(self):
        a = df.parse_asm(asm("sltiu $at, $a0, 2", "beqz $at, .L80100024", "nop", "jr $t0", "nop", "jlabel .L80100014",
                             "or $v0, $a1, $zero", "j .L80100024", "nop", ".L80100024:", "jr $ra", "nop"))
        self.assertEqual([(i, k) for i, k, _ in df.exit_v0_defs(a)], [(5, "copy")])

    def test_s32_without_return_report(self):
        a = asm("lbu $v1, 0($a0)", "beqz $v1, .L80100014", "or $v0, $v1, $zero", "jal g", "nop", ".L80100014:",
                "jr $ra", "nop")
        void = "void f(void) {\n    g();\n}\n"
        found = [f for f in df.analyze(void, a).findings if f.kind == "s32-no-return"]
        self.assertEqual(len(found), 1)
        self.assertIn("copy", found[0].detail)
        self.assertEqual(df.analyze(void.replace("void", "s32"), a).findings, [])
        self.assertEqual(df.analyze("s32 f(void) {\n    return g();\n}\n", a).findings, [])

    def test_callers_reading_v0(self):
        reading = df.parse_asm(asm("jal g", "nop", "sw $v0, 0($a1)", "jr $ra", "nop"))
        self.assertEqual(df.call_sites(reading), [("g", True)])
        for items in (("jal g", "or $a0, $v0, $zero", "jr $ra", "nop"),            # the slot runs before the call
                      ("jal g", "nop", "addiu $v0, $zero, 1", "sw $v0, 0($a1)"),     # overwritten first
                      ("jal g", "nop", "jal h", "nop", "sw $v0, 0($a1)"),            # another call in between
                      ("jal g", "nop", ".L80100010:", "sw $v0, 0($a1)")):            # join point: unknown
            self.assertEqual(df.call_sites(df.parse_asm(asm(*items)))[0], ("g", False), items)
        branch = df.parse_asm(asm("jal g", "nop", "beqz $v0, .L80100010", "nop", "jr $ra", "nop", ".L80100010:", "jr $ra", "nop"))
        self.assertEqual(df.call_sites(branch), [("g", True)])

    def test_void_callers_report_and_index(self):
        with tempfile.TemporaryDirectory() as root:
            def put(path, text):
                full = os.path.join(root, path)
                os.makedirs(os.path.dirname(full), exist_ok=True)
                pathlib.Path(full).write_text(text)
            put("asm/ovl/AAA/nonmatchings/AAA/80132000/func_80132000.s", asm("jr $ra", "nop", name="func_80132000"))
            put("asm/ovl/AAA/nonmatchings/AAA/80132000/func_80132100.s",
                asm("jal func_80132000", "nop", "sw $v0, 0($a1)", "jr $ra", "nop", name="func_80132100"))
            put("asm/ovl/BBB/nonmatchings/BBB/80132000/func_80132000.s", asm("jr $ra", "nop", name="func_80132000"))
            put("asm/ovl/BBB/nonmatchings/BBB/80132000/func_80132100.s",
                asm("jal func_80132000", "nop", "jr $ra", "nop", name="func_80132100"))
            put("asm/nonmatchings/main/80010000/func_80010000.s", asm("jr $ra", "nop", name="func_80010000"))
            put("asm/ovl/AAA/nonmatchings/AAA/80132000/func_80132200.s",
                asm("jal func_80010000", "nop", "or $a0, $v0, $zero", "jr $ra", "nop", name="func_80132200"))
            put("asm/data/not_a_function.s", "jal func_80132000\n")
            files = df.asm_files(root)
            self.assertEqual(sorted(files), [("AAA", "func_80132000"), ("AAA", "func_80132100"), ("AAA", "func_80132200"),
                                             ("BBB", "func_80132000"), ("BBB", "func_80132100"), ("main", "func_80010000")])
            index = df.build_callers(files)
            self.assertEqual(index[("AAA", "func_80132000")], [("AAA:func_80132100", True)])
            self.assertEqual(index[("BBB", "func_80132000")], [("BBB:func_80132100", False)])
            self.assertEqual(index[("main", "func_80010000")], [("AAA:func_80132200", True)])
            void = "void f(void) {\n}\n"
            found = df.analyze(void, callers=index, scope="AAA:func_80132000").findings
            self.assertEqual([f.kind for f in found], ["void-callers-read-v0"])
            self.assertIn("1 of 1", found[0].detail)
            self.assertEqual(df.analyze(void, callers=index, scope="BBB:func_80132000").findings, [])
            self.assertEqual(df.analyze("s32 f(void) {\n}\n", callers=index, scope="AAA:func_80132000").findings, [])


class Archive(unittest.TestCase):
    def make(self, root):
        """An archive of two drafts: a goto pair and a clean one (no asm: only text detectors)."""
        for name, body, score in (("func_80132000", BUNKAKEN_BEFORE, 40), ("func_80132100", "void g(void) {\n}\n", 7)):
            folder = os.path.join(root, "AAA", name)
            os.makedirs(folder)
            pathlib.Path(folder, "body.c").write_text(body)
        rows = [dict(scope="AAA:func_80132000", selected_path=os.path.join(root, "AAA", "func_80132000"), score=40, src="s.c"),
                dict(scope="AAA:func_80132100", selected_path=os.path.join(root, "AAA", "func_80132100"), score=7, src="s.c")]
        pathlib.Path(root, "best-quality-catalog.jsonl").write_text("".join(json.dumps(r) + "\n" for r in rows))

    def test_survey_counts_and_hits(self):
        with tempfile.TemporaryDirectory() as root:
            self.make(root)
            summary, results = df.survey(root, callers=False, root=root)
            self.assertEqual(summary["bodies"], 2)
            self.assertEqual(summary["asm_missing"], 2)
            self.assertEqual(summary["detectors"], {"goto-shared-store": {"bodies": 1, "findings": 1}})
            hits = df.rewrite_hits(root, root)
            self.assertEqual([h[0]["scope"] for h in hits], ["AAA:func_80132000"])

    def test_best_rewrite_picks_the_lowest_score(self):
        context = "int x;\n" + BUNKAKEN_BEFORE
        scores = []

        def scorer(text, work):
            scores.append(text)
            return 100 if "goto" in text else 60

        base, score, chosen, body, text, tried = df.best_rewrite({}, BUNKAKEN_BEFORE, None, context, scorer,
                                                                 pathlib.Path(tempfile.mkdtemp()))
        self.assertEqual((base, score, chosen), (100, 60, [("goto-shared-store", 0)]))
        self.assertEqual(body, BUNKAKEN_AFTER)
        self.assertEqual(text, "int x;\n" + BUNKAKEN_AFTER)

    def test_best_rewrite_keeps_the_original_when_nothing_helps(self):
        base, score, chosen, body, _, _ = df.best_rewrite(
            {}, BUNKAKEN_BEFORE, None, BUNKAKEN_BEFORE, lambda text, work: 5, pathlib.Path(tempfile.mkdtemp()))
        self.assertEqual((base, score), (5, 5))

    def test_best_rewrite_chooses_a_subset(self):
        body = BUNKAKEN_BEFORE.replace("D_800CA148 = (var_v0_2", 'g("\\x95\\\\");\n        D_800CA148 = (var_v0_2')

        def scorer(text, work):     # only the string rewrite helps
            return 3 if "表" in text else 9

        _, score, chosen, new, _, _ = df.best_rewrite({}, body, None, body, scorer, pathlib.Path(tempfile.mkdtemp()))
        self.assertEqual(score, 3)
        self.assertIn("表", new)
        self.assertIn(("sjis-escape", 0), chosen)


if __name__ == "__main__":
    unittest.main()
