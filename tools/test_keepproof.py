#!/usr/bin/env python3
"""Tests for tools/keepproof.py on a synthetic tree and a fake toolchain (no game data, no compiler).

Run: tools/docker.sh python3 tools/test_keepproof.py
"""
import contextlib
import io
import os
import tempfile
import unittest
from pathlib import Path

import aggregate_audit as aa
import keepproof as kp
import migrate_globals as mg
import srcscan
import type_recovery as tr

HEADER = """#ifndef MAIN_API_H
#define MAIN_API_H
typedef union W {
    /* 0x00 */ s32 w;
    /* 0x00 */ u8 b[4];
} W; /* size 0x04 */
typedef struct State {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 pad[3];
    /* 0x04 */ W unk_04;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
} State; /* size 0x0C */
extern State D_80100000;
%s#endif
"""
CONFIG = "aggregate D_80100000 State include/main_api.h\n"
FUNC_ASM = ["3C0E8010", "8DCE0004", "25CF0001", "3C018010", "AC2F0004", "03E00008", "00000000"]
# the unit as committed: f is plain C and reads the counter through the separate symbol
SRC = """#include "main_api.h"
void f(s32 x) {
    if (D_80100004 == x) {
        g();
    }
    D_80100004 += 1;
    D_80100008 = 2;
}
"""


class FakeToolchain:
    """compile() keeps the text; the 'original' matches iff the counter is written as a separate symbol."""

    def __init__(self, root):
        self.root, self.compiles = root, []
        self.rows_without = 7

    def locate(self, func, scope):
        return srcscan.CFile("main", "main/80010000", Path("src/main/80010000.c"), Path("asm/nonmatchings/main/80010000"),
                             Path("asm/matchings/main/80010000"), "build/src/main/80010000.o", False, 0x80010000)

    def asm_path(self, c, func):
        p = os.path.join(self.root, "asm/matchings/main/80010000/%s.s" % func)
        return p if os.path.exists(p) else None

    def compile(self, c, text, work, non_matching=False):
        self.compiles.append(text)
        Path(work).mkdir(parents=True, exist_ok=True)
        p = Path(work) / "fake.o"
        p.write_text(text)
        return str(p)

    def rows(self, c, obj, func):
        return 0 if "D_80100004 += 1" in Path(obj).read_text() else self.rows_without

    def exact(self, c, obj, func, work, build=True):
        ok = self.rows(c, obj, func) == 0
        return ok, "linked function bytes %s target (28 bytes)" % ("equal" if ok else "differ from")


class Tree(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = self.tmp.name
        for d in ("include", "src/main", "config", "asm/matchings/main/80010000"):
            os.makedirs(os.path.join(self.root, d))
        self.header("")
        self.put("config/migrate_globals.txt", CONFIG)
        rows = ["    /* %X %08X %s */  nop" % (4 * i, 0x80010000 + 4 * i, w) for i, w in enumerate(FUNC_ASM)]
        self.put("asm/matchings/main/80010000/f.s", "\n".join(["glabel f"] + rows + ["endlabel f"]) + "\n")
        self.tc = FakeToolchain(self.root)

    def tearDown(self):
        self.tmp.cleanup()

    def put(self, rel, text):
        with open(os.path.join(self.root, rel), "w") as f:
            f.write(text)

    def header(self, decls):
        self.put("include/main_api.h", HEADER % decls)

    def aggs(self):
        return mg.load_config(self.root)[0]

    def prove(self, text, declare=None, symbol="D_80100004"):
        c = self.tc.locate("f", "main")
        return kp.prove(self.tc, self.root, self.aggs(), c, "f", symbol, text, declare)


class FoldTest(Tree):
    def test_fold_only_the_named_symbol(self):
        text = '#include "main_api.h"\nextern s32 D_80100004;\nextern s16 D_80100008;\n' \
               "void f(void) { D_80100004 += 1; D_80100008 = 2; }\n"
        out = kp.fold_symbol(text, self.aggs(), mg.Views(self.root), "D_80100004")
        self.assertIn("D_80100000.unk_04.w += 1; D_80100008 = 2;", out)
        self.assertNotIn("extern s32 D_80100004", out)
        self.assertIn("extern s16 D_80100008;", out)         # the other absorbed name stays as written

    def test_fold_uses_the_header_declaration(self):
        self.header("extern s32 D_80100004;\n")
        out = kp.fold_symbol(SRC, self.aggs(), mg.Views(self.root), "D_80100004")
        self.assertIn("D_80100000.unk_04.w += 1;", out)
        self.assertIn("D_80100000.unk_04.w == x", out)
        self.assertIn("D_80100008 = 2;", out)

    def test_fold_without_a_declaration_is_ambiguous_until_declared(self):
        views = mg.Views(self.root)
        with self.assertRaises(kp.FoldError) as e:
            kp.fold_symbol(SRC, self.aggs(), views, "D_80100004")
        self.assertIn("ambiguous", str(e.exception))
        text = kp.declare_symbol(SRC, views, "D_80100004", "s32")
        self.assertIn('#include "main_api.h"\nextern s32 D_80100004;\n', text)
        self.assertIn("D_80100000.unk_04.w += 1;", kp.fold_symbol(text, self.aggs(), views, "D_80100004"))

    def test_declare_symbol_is_a_noop_when_a_declaration_is_visible(self):
        self.header("extern s32 D_80100004;\n")
        views = mg.Views(self.root)
        self.assertEqual(kp.declare_symbol(SRC, views, "D_80100004", "s32"), SRC)
        self.assertEqual(kp.declare_symbol(SRC, views, "D_80100004", None), SRC)

    def test_fold_errors(self):
        views = mg.Views(self.root)
        with self.assertRaises(kp.FoldError):
            kp.fold_symbol(SRC, self.aggs(), views, "D_80200000")             # not inside the aggregate
        self.header("extern s32 D_80100004;\n")
        with self.assertRaises(kp.FoldError):
            kp.fold_symbol('#include "main_api.h"\nvoid g(void) {}\n', self.aggs(), mg.Views(self.root), "D_80100004")


class ProveTest(Tree):
    def test_necessary_and_sufficient(self):
        self.header("extern s32 D_80100004;\n")
        res = self.prove(SRC)
        self.assertTrue(res.ok, res.report())
        self.assertEqual((res.rows_with, res.rows_without), (0, 7))
        self.assertEqual(res.sha, mg.asm_sha1(mg.read(os.path.join(self.root, "asm/matchings/main/80010000/f.s"))))
        self.assertEqual(len(self.tc.compiles), 2)                           # symbol folded, symbol separate
        self.assertIn("D_80100000.unk_04.w += 1", self.tc.compiles[0])
        self.assertIn("D_80100004 += 1", self.tc.compiles[1])
        self.assertIn("PROVEN", res.report())
        line = res.keep_line("why")
        self.assertEqual(line, "keep src/main/80010000.c D_80100004 proof=f:7:%s why" % res.sha)

    def test_the_keep_line_passes_migrate_globals_check(self):
        self.header("extern s32 D_80100004;\n")
        self.put("src/main/80010000.c", SRC)
        res = self.prove(SRC)
        self.put("config/migrate_globals.txt", CONFIG + res.keep_line(kp.DEFAULT_REASON) + "\n")
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(mg.main(["--root", self.root, "--apply"]), 0)  # folds D_80100008, keeps D_80100004
        aggs, keep = mg.load_config(self.root)
        self.assertEqual(keep["src/main/80010000.c"].proofs["D_80100004"][0].rows, 7)
        msgs = [m for m in mg.check(self.root, aggs, keep) if "D_80100004" in m]
        self.assertEqual(msgs, [])

    def test_declare_is_used_when_no_header_declares_the_symbol(self):
        res = self.prove(SRC)
        self.assertFalse(res.ok)
        self.assertIn("cannot fold", res.problems[0])
        self.assertEqual(self.tc.compiles, [])
        res = self.prove(SRC, declare="s32")
        self.assertTrue(res.ok, res.report())

    def test_a_function_that_is_not_plain_c_is_refused_without_compiling(self):
        self.header("extern s32 D_80100004;\n")
        for src in ('#include "main_api.h"\nINCLUDE_ASM("asm/nonmatchings/main/80010000", f);\n',
                    "#ifdef NON_MATCHING\n" + SRC + '#else\nINCLUDE_ASM("asm/nonmatchings/main/80010000", f);\n#endif\n'):
            res = self.prove(src)
            self.assertFalse(res.ok)
            self.assertFalse(res.plain)
            self.assertIn("not plain C", res.problems[0])
        self.assertEqual(self.tc.compiles, [])

    def test_not_necessary_when_the_folded_source_matches_too(self):
        self.header("extern s32 D_80100004;\n")
        self.tc.rows_without = 0
        self.tc.rows = lambda c, obj, func: 0
        res = self.prove(SRC)
        self.assertFalse(res.necessary)
        self.assertIsNone(res.sufficient)                    # not run: the first compile ended the check
        self.assertEqual(len(self.tc.compiles), 1)
        self.assertFalse(res.ok)
        self.assertIsNone(res.keep_line())
        self.assertIn("NOT PROVEN", res.report())
        self.assertIn("necessity:    NO", res.report())

    def test_not_sufficient_when_the_bytes_differ(self):
        self.header("extern s32 D_80100004;\n")
        self.tc.exact = lambda c, obj, func, work, build=True: (False, "linked function bytes differ from target")
        self.tc.diff = ["-@row 15 sw $t0, 0x10($sp)", "+@row 15 sw $v0, 0x10($sp)"]
        res = self.prove(SRC)
        self.assertFalse(res.sufficient)
        self.assertTrue(res.necessary)
        self.assertFalse(res.ok)
        self.assertIn("    +@row 15 sw $v0, 0x10($sp)", res.report())

    def test_toolchain_failure_is_a_problem_not_a_proof(self):
        self.header("extern s32 D_80100004;\n")

        def boom(c, text, work, non_matching=False):
            raise ValueError("ido failed:\nstdin:3: error")
        self.tc.compile = boom
        res = self.prove(SRC)
        self.assertFalse(res.ok)
        self.assertIn("compile or score failed", res.problems[0])

    def test_missing_original_asm(self):
        self.header("extern s32 D_80100004;\n")
        os.remove(os.path.join(self.root, "asm/matchings/main/80010000/f.s"))
        res = self.prove(SRC)
        self.assertFalse(res.ok)
        self.assertIn("no original asm", res.problems[0])


class FromDraftTest(Tree):
    GUARDED = ("#include \"main_api.h\"\n#ifdef NON_MATCHING\nvoid f(s32 x) {\n"
               "    if (D_80100000.unk_04.w == x) {\n        g();\n    }\n"
               "    /* FAKE: volatile tail, T-1 */\n    *(volatile s32 *)&D_80100000.unk_04.w += 1;\n}\n#else\n"
               "INCLUDE_ASM(\"asm/nonmatchings/main/80010000\", f);\n#endif\n")

    def test_unguard(self):
        plain = kp.unguard(self.GUARDED, "f")
        self.assertNotIn("INCLUDE_ASM", plain)
        self.assertNotIn("NON_MATCHING", plain)
        self.assertTrue(mg.plain_c(plain, "f"))
        self.assertIsNone(kp.unguard("void f(void) {\n}\n", "f"))
        self.assertIsNone(kp.unguard(self.GUARDED, "g"))

    def test_from_draft_writes_the_tail_with_the_separate_symbol(self):
        text, ctype = kp.from_draft(self.GUARDED, self.aggs(), "f", "D_80100004")
        self.assertEqual(ctype, "s32")
        self.assertIn("D_80100000.unk_04.w == x", text)           # the chain still reads the field
        self.assertIn("    D_80100004 += 1;", text)
        self.assertNotIn("*(volatile", text)                      # the comment stays for the author to rewrite

    def test_from_draft_errors(self):
        with self.assertRaises(kp.FoldError):
            kp.from_draft("void f(void) {\n}\n", self.aggs(), "f", "D_80100004")
        with self.assertRaises(kp.FoldError):
            kp.from_draft(self.GUARDED, self.aggs(), "f", "D_80100008")

    def test_the_draft_proves_with_the_fake_toolchain(self):
        text, ctype = kp.from_draft(self.GUARDED, self.aggs(), "f", "D_80100004")
        res = self.prove(text, declare=ctype)
        self.assertTrue(res.ok, res.report())
        self.assertIn("D_80100000.unk_04.w += 1", self.tc.compiles[0])


class VerifyConfigTest(Tree):
    def line(self, sha, rows=7):
        return "keep src/main/80010000.c D_80100004 proof=f:%d:%s reason\n" % (rows, sha)

    def setup_keep(self, sha=None, rows=7):
        self.header("extern s32 D_80100004;\n")
        self.put("src/main/80010000.c", SRC)
        real = mg.asm_sha1(mg.read(os.path.join(self.root, "asm/matchings/main/80010000/f.s")))
        self.put("config/migrate_globals.txt", CONFIG + self.line(sha or real, rows))
        return real

    def verify(self):
        aggs, keep = mg.load_config(self.root)
        with contextlib.redirect_stdout(io.StringIO()):
            return kp.verify_config(self.root, aggs, keep, self.tc)

    def test_a_valid_line_reproduces(self):
        self.setup_keep()
        self.assertEqual(self.verify(), [])

    def test_a_changed_row_count_is_only_a_note(self):
        self.setup_keep(rows=242)
        self.assertEqual(self.verify(), [])

    def test_stale_sha_fails(self):
        self.setup_keep(sha="0123abcd")
        lines = self.verify()
        self.assertEqual(len(lines), 1)
        self.assertIn("is stale", lines[0])

    def test_a_proof_that_no_longer_holds_fails(self):
        self.setup_keep()
        self.tc.rows = lambda c, obj, func: 0                  # the folded source matches now: not necessary
        lines = self.verify()
        self.assertEqual(len(lines), 1)
        self.assertIn("not reproduced", lines[0])


# ------------------------------------------------------------------------------------------ census

def asm(*rows):
    return ["glabel f"] + ["    /* %X %08X 00000000 */  %s" % (4 * i, 0x80100000 + 4 * i, r)
                           for i, r in enumerate(rows)] + ["endlabel f"]


X = "D_80100004"
LOAD1 = ["lui $v0, %%hi(%s)" % X, "lw $v0, %%lo(%s)($v0)" % X]
RELOAD = ["lui $t1, %%hi(%s)" % X, "lw $t1, %%lo(%s)($t1)" % X, "addiu $t2, $t1, 0x1",
          "lui $at, %%hi(%s)" % X, "sw $t2, %%lo(%s)($at)" % X]
RANGES = aa.Ranges([("D_80100000", 0x80100000, 0x8010000C)])


def hits(*rows):
    return kp.shape_hits(asm(*rows), "main", tr.Symbols(), RANGES)


class ShapeTest(unittest.TestCase):
    def test_held_in_a_callee_saved_copy_across_a_call(self):
        h = hits(*LOAD1, "or $s0, $v0, $zero", "slti $at, $s0, 0x5", "jal g", " nop", "slti $at, $s0, 0x9", *RELOAD)
        self.assertEqual(len(h), 1)
        self.assertEqual((h[0].addr, h[0].uses, h[0].held), (0x80100004, 3, True))
        self.assertLess(h[0].load, h[0].reload)
        self.assertLess(h[0].reload, h[0].store)

    def test_chain_without_a_surviving_value_is_not_held(self):
        h = hits(*LOAD1, "slti $at, $v0, 0x5", "jal g", " nop", *RELOAD)
        self.assertEqual(len(h), 1)
        self.assertFalse(h[0].held)

    def test_held_in_a_spill(self):
        h = hits(*LOAD1, "sw $v0, 0x10($sp)", "jal g", " nop", "lw $t0, 0x10($sp)", "slti $at, $t0, 0x1", *RELOAD)
        self.assertEqual(len(h), 1)
        self.assertTrue(h[0].held)

    def test_one_load_is_not_the_shape(self):
        self.assertEqual(hits(*LOAD1, "slti $at, $v0, 0x5", "addiu $t2, $v0, 0x1", "lui $at, %%hi(%s)" % X,
                              "sw $t2, %%lo(%s)($at)" % X), [])

    def test_a_store_between_the_loads_changes_the_value(self):
        self.assertEqual(hits(*LOAD1, "slti $at, $v0, 0x5", "lui $at, %%hi(%s)" % X, "sw $zero, %%lo(%s)($at)" % X,
                              *RELOAD), [])

    def test_the_first_value_must_be_used(self):
        self.assertEqual(hits(*LOAD1, "jal g", " nop", *RELOAD), [])

    def test_the_reload_must_be_changed_before_the_store(self):
        self.assertEqual(hits(*LOAD1, "slti $at, $v0, 0x5", "lui $t1, %%hi(%s)" % X, "lw $t1, %%lo(%s)($t1)" % X,
                              "lui $at, %%hi(%s)" % X, "sw $t1, %%lo(%s)($at)" % X), [])

    def test_addresses_outside_the_aggregates_are_ignored(self):
        other = [r.replace(X, "D_80300000") for r in LOAD1 + RELOAD]
        self.assertEqual(hits(*other[:2], "slti $at, $v0, 0x5", *other[2:]), [])


class ClassifyTest(unittest.TestCase):
    AGGS = None

    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        os.makedirs(os.path.join(cls.tmp.name, "include"))
        with open(os.path.join(cls.tmp.name, "include/main_api.h"), "w") as f:
            f.write(HEADER % "")
        cls.agg = mg.Aggregate("D_80100000", "State", "include/main_api.h", cls.tmp.name)

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def tags(self, body, lead=""):
        return kp.classify_c("%svoid f(void) {\n%s\n}\n" % (lead, body), "f", [self.agg])

    def test_plain(self):
        self.assertEqual(self.tags("    D_80100000.unk_04.w += 1;"), set())

    def test_keep(self):
        self.assertEqual(self.tags("    D_80100004 += 1;"), {"keep"})

    def test_volatile_and_absolute(self):
        self.assertEqual(self.tags("    *(volatile s32 *)&D_80100000.unk_04.w += 1;"), {"volatile"})
        self.assertEqual(self.tags("    D_80100000.unk_04.w = *(s32 *)0x80100004 + 1;"), {"absolute"})
        self.assertEqual(self.tags("    *(volatile s32 *)0x80100004 += 1;"), {"absolute", "volatile"})

    def test_volatile_elsewhere_is_not_the_trick(self):
        self.assertEqual(self.tags("    *(volatile u32 *)0x1F801070 = 0;"), set())

    def test_pointer_local_is_informational(self):
        self.assertEqual(self.tags("    State *g = &D_80100000;\n    g->unk_04.w += 1;"), {"local"})

    def test_fake_comment_is_informational(self):
        self.assertEqual(self.tags("    D_80100000.unk_04.w += 1;", "/* FAKE: pad, T-1 */\n"), {"fake"})

    def test_missing_function(self):
        self.assertIsNone(kp.classify_c("void g(void) {}\n", "f", [self.agg]))

    def test_function_span_ignores_braces_in_comments_and_strings(self):
        text = 'void f(void) {\n    /* } */ s("}");\n    if (1) { g(); }\n}\nvoid h(void) {\n}\n'
        s, e = kp.function_span(text, "f")
        self.assertEqual(text[s:e], 'void f(void) {\n    /* } */ s("}");\n    if (1) { g(); }\n}')


class SummaryTest(unittest.TestCase):
    def info(self, name, matched, tags, held=False):
        f = kp.FuncInfo("TACO", name, matched, "src/ovl/TACO/a.c", 10)
        f.hits = [kp.Hit(0x800E7384, 1, 5, 7, 2, held)]
        f.tags = tags
        return f

    def test_false_positive_rate_counts_matched_functions_that_needed_no_trick(self):
        funcs = [self.info("a", True, set(), True), self.info("b", True, {"fake"}), self.info("c", True, {"volatile"}),
                 self.info("d", True, {"absolute", "fake"}, True), self.info("e", False, None, True)]
        n = kp.FuncInfo("TACO", "n", True, "src/ovl/TACO/a.c", 10)                    # no hit: not in the shape counts
        n.tags = {"volatile"}
        funcs.append(n)
        s = kp.summarize(funcs)
        self.assertEqual((s["hit_functions"], s["hit_matched"], s["hit_unmatched"]), (5, 4, 1))
        self.assertEqual((s["matched_classified"], s["matched_plain"], s["matched_trick"]), (4, 2, 2))
        self.assertAlmostEqual(s["false_positive_rate"], 0.5)
        self.assertEqual((s["held_matched"], s["held_matched_plain"], s["held_unmatched"]), (2, 1, 1))
        self.assertEqual(s["trick_counts"], {"volatile": 1, "absolute": 1})
        self.assertEqual(s["other_tags"], {"fake": 2})
        self.assertEqual((s["tree_matched_classified"], s["tree_matched_trick"]), (5, 3))          # + the one without a hit
        self.assertIn("whole tree: 5 matched functions", kp.census_text(funcs))
        text = kp.census_text(funcs)
        self.assertIn("false-positive rate 50.0%", text)
        self.assertIn("e", text)
        self.assertEqual(kp.census_json(funcs)["summary"]["hit_matched"], 4)

    def test_no_matched_function_gives_no_rate(self):
        s = kp.summarize([self.info("e", False, None)])
        self.assertIsNone(s["false_positive_rate"])
        self.assertIn("n/a", kp.census_text([self.info("e", False, None)]))


class RowsTest(Tree):
    def test_diff_rows(self):
        self.assertEqual(kp.diff_rows(["a", "b"], ["a", "b"], []), 0)
        self.assertEqual(kp.diff_rows(["a", "b"], ["a", "c"], ["-@row 1 b", "+@row 1 c"]), 1)
        self.assertEqual(kp.diff_rows(["a", "b", "x"], ["a", "c"], ["-b", "-x", "+c"]), 2)
        self.assertEqual(kp.diff_rows(["a"], ["b"], []), 1)

    def test_path_target_and_field_uses(self):
        agg = self.aggs()[0]
        self.assertEqual(kp.path_target(agg, ".unk_04.w")[0], 4)
        self.assertEqual(kp.path_target(agg, ".unk_04.b[2]")[0], 6)
        self.assertIsNone(kp.path_target(agg, ".unk_99"))
        self.assertIsNone(kp.path_target(agg, ".unk_08[1]"))
        body = "D_80100000.unk_04.w += 1; if (D_80100000.unk_04.w == 2) { D_80100000.unk_08 = 1; D_80100000.unk_04; }"
        uses = kp.field_uses(body, agg)
        self.assertEqual(sorted(uses), [("D_80100004", "s32"), ("D_80100008", "s16")])
        self.assertEqual(len(uses[("D_80100004", "s32")]), 2)

    def test_separate_variants_all_and_tail(self):
        agg = self.aggs()[0]
        body = "{ D_80100000.unk_04.w += 1; x = D_80100000.unk_04.w; D_80100000.unk_04.w += 1; }"
        spans = kp.field_uses(body, agg)[("D_80100004", "s32")]
        v = kp.separate_variants(body, "D_80100004", spans)
        self.assertEqual(v["all"], "{ D_80100004 += 1; x = D_80100004; D_80100004 += 1; }")
        self.assertEqual(v["tail"], "{ D_80100000.unk_04.w += 1; x = D_80100000.unk_04.w; D_80100004 += 1; }")

    def test_tail_takes_fake_casts_and_absolute_reads_of_the_last_statement(self):
        agg = self.aggs()[0]
        for last, want in (("*(volatile s32 *)&D_80100000.unk_04.w += 1;", "D_80100004 += 1;"),
                           ("D_80100000.unk_04.w = *(s32 *)0x80100004 + 1;", "D_80100004 = D_80100004 + 1;"),
                           ("*(s32 *)&D_80100000.unk_04.w = 2;", "D_80100004 = 2;")):
            body = "{ x = D_80100000.unk_04.w; %s }" % last
            uses = kp.field_uses(body, agg)
            self.assertEqual(list(uses), [("D_80100004", "s32")], last)
            self.assertEqual(kp.separate_variants(body, "D_80100004", uses[("D_80100004", "s32")])["tail"],
                             "{ x = D_80100000.unk_04.w; %s }" % want, last)

    def test_a_cast_of_another_type_is_not_swallowed(self):
        agg = self.aggs()[0]
        body = "{ *(u8 *)&D_80100000.unk_04.w = 1; }"
        (sym, spans), = kp.field_uses(body, agg).items()
        self.assertEqual(body[spans[0][0]:spans[0][1]], "D_80100000.unk_04.w")

    def test_with_lines_and_replace_spans(self):
        self.assertEqual(kp.with_lines('#include "a.h"\n#include "b.h"\nint x;\n', ["int y;"]),
                         '#include "a.h"\n#include "b.h"\nint y;\nint x;\n')
        self.assertEqual(kp.with_lines("int x;\n", ["int y;"]), "int y;\nint x;\n")
        self.assertEqual(kp.with_lines("int x;\n", []), "int x;\n")
        self.assertEqual(kp.replace_spans("abcdef", [(4, 5, "X"), (1, 3, "YY")]), "aYYdXf")

    def test_compiler_error_picks_the_error_line(self):
        e = ValueError("ido failed:\nfoo\ncfe: Error: x.c, line 3: undeclared name\npython3 build.py --x")
        self.assertEqual(kp.compiler_error(e), "cfe: Error: x.c, line 3: undeclared name")
        self.assertEqual(kp.compiler_error(ValueError("boom")), "boom")

    def test_run_rows_with_a_fake_toolchain(self):
        self.header("")
        src = ('#include "main_api.h"\n#ifdef NON_MATCHING\nvoid f(s32 x) {\n    if (D_80100000.unk_04.w == x) {\n'
               '        g();\n    }\n    D_80100000.unk_04.w += 1;\n}\n#else\n'
               'INCLUDE_ASM("asm/nonmatchings/main/80010000", f);\n#endif\n')
        self.put("src/main/80010000.c", src)
        old = os.getcwd()
        os.chdir(self.root)
        try:
            tc = self.tc
            # only the tail written as the separate symbol (the compare stays a field) matches
            tc.rows = lambda c, obj, func: (0 if "D_80100004 += 1" in Path(obj).read_text() and
                                            "D_80100000.unk_04.w == x" in Path(obj).read_text() else 9)
            out = []
            res = kp.run_rows(".", self.aggs(), [("main", "f", ())], 4, tc, out.append)
        finally:
            os.chdir(old)
        self.assertEqual(len(res), 1, "\n".join(out))
        self.assertEqual((res[0]["base"], res[0]["all"], res[0]["tail"]), (9, 9, 0))
        self.assertTrue(any("committed guarded draft" in line for line in out))


if __name__ == "__main__":
    unittest.main()
