#!/usr/bin/env python3
"""Tests for tools/viewsweep.py (T-20810): the lattice of a synthetic preprocessed unit, the search phases with a
fake compiler (order, budget cut-off, early stop, the serial retry that makes a failed probe UNTESTED instead of
"no gain"), the scoring and proof plumbing with mocks, the recipe, and the CLI. No compiler and no game data.

Run: tools/docker.sh python3 -m unittest -q tools/test_viewsweep.py   (from the repo root)
"""
import contextlib
import io
import json
import os
import sys
import tempfile
import threading
import time
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import protoverride as pv  # noqa: E402
import viewsweep as vs  # noqa: E402

UNIT = """typedef int s32;
typedef unsigned char u8;
void func_8013E97C(s32 a, s32 b, s32 c);
s32 func_8013E7C0(s32 a);
u8 func_8013E111(u8 a);
extern s32 D_800E0000;
void helper(s32 x);
INCLUDE_ASM("asm/nonmatchings/X", asm_callee);

void target(void) {
    char *s = "};( f(";
    func_8013E97C(1, 2, 3);
    D_800E0000 = func_8013E7C0(4);
    if (D_800E0000) func_8013E111(5);
    helper(2);
    asm_callee();
    (void)func_8013E111(6);
}

void helper(s32 x) {
    D_800E0000 = x;
}
"""


def E(symbol, decl, kind="callee"):
    return vs.Edit(symbol, decl, kind)


class Analysis(unittest.TestCase):
    def test_mask_blanks_literals_and_keeps_the_length(self):
        text = 'a("};( f(", \'(\'); b();'
        out = vs.mask(text)
        self.assertEqual(len(out), len(text))
        self.assertNotIn("(\"};", out)
        self.assertEqual(out.count("("), 2)          # a( and b(; the parentheses in the literals are blanked

    def test_calls_are_found_in_source_order_without_keywords_or_variables(self):
        body = "void g(void) { s32 fp; if (a) { f1(1); } while (f2()) f3(); return f1(); for (;;) f4(sizeof(x)); fp(3); }"
        self.assertEqual(vs.body_calls(body, "g"), ["f1", "f2", "f3", "f4"])

    def test_a_call_in_a_string_is_no_call(self):
        self.assertEqual(vs.body_calls('void g(void) { puts("f1(2)"); }', "g"), ["puts"])

    def test_ignored_results(self):
        body = """void g(void) {
            a();
            x = b();
            if (c()) d();
            else e();
            (void)f();
            y = (s32)h();
            for (i = 0; i < 3; i = k()) { m(); }
            while (n) n2();
            switch (p) { case 1: q(); break; }
            r(); t = r();
        }"""
        callees = vs.body_calls(body, "g")
        self.assertEqual(vs.ignored_results(body, callees),
                         {"a", "d", "e", "f", "m", "n2", "q"})

    def test_a_value_return_is_found(self):
        self.assertTrue(vs.has_value_return("{ return 1; }"))
        self.assertFalse(vs.has_value_return("{ return; }"))
        self.assertFalse(vs.has_value_return('{ puts("return 1;"); }'))


class Lattice(unittest.TestCase):
    def decls(self, edits):
        return [e.decl for e in edits]

    def test_own_edits_of_a_function_without_parameters(self):
        self.assertEqual(self.decls(vs.own_edits(UNIT, "target")), ["s32 target();", "s32 target(void);"])

    def test_own_edits_keep_the_parameters_of_the_definition(self):
        self.assertEqual(self.decls(vs.own_edits(UNIT, "helper")), ["s32 helper(s32 x);"])

    def test_an_s32_function_only_changes_the_spelling_of_its_empty_list(self):
        text = "typedef int s32;\ns32 f(void) { }\n"
        self.assertEqual(self.decls(vs.own_edits(text, "f")), ["s32 f();"])
        text = "typedef int s32;\nf() { }\n"
        self.assertEqual(self.decls(vs.own_edits(text, "f")), ["s32 f(void);"])

    def test_the_definition_not_the_header_is_the_reference(self):
        text = "typedef int s32;\ns32 f(s32 a);\nvoid f(s32 a) { }\n"          # a header that disagrees with the definition
        self.assertEqual(self.decls(vs.own_edits(text, "f")), ["s32 f(s32 a);"])
        text = "typedef int s32;\nvoid f(void);\ns32 f(void) { }\n"
        self.assertEqual(self.decls(vs.own_edits(text, "f")), ["s32 f();"])
        text = "typedef int s32;\nvoid f(a)\ns32 a;\n{\n}\n"
        self.assertEqual(self.decls(vs.own_edits(text, "f")), ["s32 f();"])

    def test_nothing_to_try_for_a_function_the_unit_does_not_define(self):
        self.assertEqual(vs.own_edits(UNIT, "func_8013E7C0"), [])

    def test_callee_edits_of_a_void_callee_with_parameters(self):
        self.assertEqual(self.decls(vs.callee_edits(UNIT, "func_8013E97C", ignored=True)),
                         ["s32 func_8013E97C();", "s32 func_8013E97C(s32 a, s32 b, s32 c);", "void func_8013E97C();"])

    def test_the_void_view_needs_an_ignored_result_or_a_void_callee(self):
        self.assertEqual(self.decls(vs.callee_edits(UNIT, "func_8013E7C0", ignored=False)), ["s32 func_8013E7C0();"])
        self.assertEqual(self.decls(vs.callee_edits(UNIT, "func_8013E7C0", ignored=True)),
                         ["s32 func_8013E7C0();", "void func_8013E7C0();"])

    def test_a_typed_result_gets_the_s32_views(self):
        self.assertEqual(self.decls(vs.callee_edits(UNIT, "func_8013E111", ignored=False)),
                         ["s32 func_8013E111();", "s32 func_8013E111(u8 a);"])

    def test_an_undeclared_callee_is_implicit_int_already(self):
        text = "void g(void) { h(1); }\n"
        self.assertEqual(self.decls(vs.callee_edits(text, "h", ignored=False)), [])
        self.assertEqual(self.decls(vs.callee_edits(text, "h", ignored=True)), ["void h();"])

    def test_a_callee_defined_in_the_file_keeps_a_value_return(self):
        text = "typedef int s32;\ns32 c(s32 a);\nvoid g(void) { c(1); }\ns32 c(s32 a) { return a; }\n"
        self.assertEqual(self.decls(vs.callee_edits(text, "c", ignored=True)), ["s32 c();"])       # no void: it returns a value
        text = "typedef int s32;\nvoid c(s32 a);\nvoid g(void) { c(1); }\nvoid c(s32 a) { return; }\n"
        self.assertEqual(self.decls(vs.callee_edits(text, "c", ignored=True)), ["s32 c();", "s32 c(s32 a);", "void c();"])

    def test_the_plan_lists_the_callees_that_have_edits(self):
        body = UNIT[UNIT.index("void target"):UNIT.index("void helper(s32 x) {")]
        callees = vs.body_calls(body, "target")
        self.assertEqual(callees, ["func_8013E97C", "func_8013E7C0", "func_8013E111", "helper", "asm_callee"])
        ignored = vs.ignored_results(body, callees)
        self.assertEqual(ignored, {"func_8013E97C", "func_8013E111", "helper", "asm_callee"})     # not func_8013E7C0: assigned
        own, per = vs.build_plan(UNIT, "target", callees, ignored)
        self.assertEqual(len(own), 2)
        self.assertEqual(list(per), ["func_8013E97C", "func_8013E7C0", "func_8013E111", "helper", "asm_callee"])

    def test_edits_apply_in_memory_to_the_definition_and_the_callee(self):
        out = vs.apply_edits(UNIT, [E("target", "s32 target();", "own"), E("helper", "s32 helper(s32 x);"),
                                    E("func_8013E97C", "void func_8013E97C();")])
        self.assertIn("s32 target() {", out)
        self.assertIn("s32 helper(s32 x) {", out)
        self.assertNotIn("void func_8013E97C(s32 a, s32 b, s32 c);", out)
        self.assertIn("void func_8013E97C();", out)
        self.assertIn("s32 helper(s32 x);", out)

    def test_variant_labels_and_keys_are_canonical(self):
        a, b = E("b", "s32 b();"), E("a", "s32 a();")
        self.assertEqual(vs.variant_key((a, b)), vs.variant_key((b, a)))
        self.assertEqual(vs.variant_label(()), "as is")
        self.assertEqual(vs.variant_label(vs.variant_key((a, b))), "s32 a(); s32 b();")


class Ordering(unittest.TestCase):
    OWN = [E("f", "s32 f();", "own")]
    PER = {"c1": [E("c1", "s32 c1();"), E("c1", "void c1();")], "c2": [E("c2", "s32 c2();"), E("c2", "void c2();")]}

    def order(self, hints):
        return [e.decl for e in vs.order_singles(self.OWN, self.PER, hints)]

    def test_without_hints_the_order_is_own_then_callees_in_source_order(self):
        self.assertEqual(self.order(None), ["s32 f();", "s32 c1();", "void c1();", "s32 c2();", "void c2();"])

    def test_an_exit_value_in_v0_and_a_read_result_come_first(self):
        got = self.order(vs.Hints(own=False, reads={"c1": False, "c2": True}))
        self.assertEqual(got[0], "s32 c2();")
        self.assertEqual(got[-1], "void c2();")      # the original reads c2's result: void contradicts it
        got = self.order(vs.Hints(own=True, reads={"c1": False, "c2": True}))
        self.assertEqual(got[:2], ["s32 f();", "s32 c2();"])

    def test_the_hints_come_from_the_asm(self):
        asm = """glabel f
    /* 0 80100000 0000A0AC */  jal c2
    /* 4 80100004 00000000 */   nop
    /* 8 80100008 00004210 */  addu $t0, $v0, $zero
    /* C 8010000C 00000000 */  jal c1
    /* 10 80100010 00000000 */   nop
    /* 14 80100014 00000000 */  jr $ra
    /* 18 80100018 00000000 */   or $v0, $a0, $zero
endlabel f
"""
        hints, order = vs.asm_hints(asm)
        self.assertEqual(order, ["c2", "c1"])
        self.assertEqual(hints.reads, {"c2": True, "c1": False})
        self.assertTrue(hints.own)


class FakeScore:
    """A compiler stand-in: rows by variant label; `fail` labels raise (every time, or `flaky`: once)."""

    def __init__(self, rows, default=50, fail=(), flaky=(), proto_error=(), delay=0.0):
        self.rows, self.default, self.fail, self.flaky, self.proto_error = rows, default, set(fail), set(flaky), set(proto_error)
        self.delay = delay
        self.calls, self.lock = [], threading.Lock()
        self.running = self.peak = 0
        self.running_at_call = {}

    def __call__(self, edits):
        label = vs.variant_label(edits)
        with self.lock:
            self.calls.append(label)
            self.running += 1
            self.peak = max(self.peak, self.running)
            self.running_at_call.setdefault(label, []).append(self.running)
            first = self.calls.count(label) == 1
        try:
            time.sleep(self.delay)
            if label in self.proto_error:
                raise pv.ProtoError("cannot apply")
            if label in self.fail or (label in self.flaky and first):
                raise ValueError("cc.py failed:\nbuild.ninja: No such file or directory")
            return self.rows.get(label, self.default)
        finally:
            with self.lock:
                self.running -= 1


OWN1, OWN2 = E("f", "s32 f();", "own"), E("f", "s32 f(void);", "own")
C1S, C1V = E("c1", "s32 c1();"), E("c1", "void c1();")
C2S = E("c2", "s32 c2();")
C3S = E("c3", "s32 c3();")
PER = {"c1": [C1S, C1V], "c2": [C2S], "c3": [C3S]}


def sweep(rows, jobs=1, budget=150, per=PER, own=(OWN1, OWN2), **kw):
    fake = FakeScore(rows, **kw)
    s = vs.Sweep(fake, list(own), dict(per), None, budget, jobs, log=lambda m: None)
    return s, fake, s.run()


def L(*edits):
    """The label the fake compiler sees for a variant."""
    return vs.variant_label(vs.variant_key(edits))


class Phases(unittest.TestCase):
    def test_baseline_then_singles_in_order(self):
        _s, fake, result = sweep({"as is": 20})
        self.assertEqual(fake.calls, ["as is", "s32 f();", "s32 f(void);", "s32 c1();", "void c1();", "s32 c2();", "s32 c3();"])
        self.assertEqual((result["status"], result["baseline"], result["best_rows"], result["best"]),
                         ("no-gain", 20, 20, []))

    def test_an_improving_own_edit_is_crossed_with_every_callee_edit(self):
        rows = {"as is": 20, "s32 f();": 12, L(OWN1, C2S): 4}
        _s, fake, result = sweep(rows)
        crossed = [L(OWN1, e) for e in (C1S, C1V, C2S, C3S)]
        self.assertEqual([c for c in fake.calls if c in crossed], crossed)
        self.assertNotIn(L(OWN2, C1S), fake.calls)                       # only the best own edit is crossed
        self.assertEqual((result["status"], result["best_rows"], result["best"]), ("improved", 4, ["s32 c2();", "s32 f();"]))

    def test_without_an_improving_own_edit_nothing_is_crossed(self):
        _s, fake, _r = sweep({"as is": 20, "s32 c1();": 15})
        self.assertNotIn(L(OWN1, C1S), fake.calls)
        self.assertNotIn(L(OWN2, C1S), fake.calls)

    def test_pairs_among_the_five_best_improvers_skip_the_same_function(self):
        rows = {"as is": 30, "s32 c1();": 20, "void c1();": 21, "s32 c2();": 22, "s32 c3();": 23, "s32 f();": 24}
        _s, fake, _r = sweep(rows)
        self.assertNotIn(L(C1S, C1V), fake.calls)                         # one function, two views: never a pair
        for pair in ((C1S, C2S), (C2S, C3S), (C1V, C3S), (C1S, C3S)):
            self.assertIn(L(*pair), fake.calls)

    def test_only_the_five_best_improvers_are_paired(self):
        edits = {i: E("c%d" % i, "s32 c%d();" % i) for i in range(1, 8)}
        per = {"c%d" % i: [edits[i]] for i in edits}
        rows = {"as is": 100}
        rows.update({e.decl: 100 - i for i, e in edits.items()})          # c7 best ... c1 worst
        _s, fake, _r = sweep(rows, per=per, own=())
        self.assertIn(L(edits[7], edits[3]), fake.calls)
        self.assertNotIn(L(edits[7], edits[2]), fake.calls)
        self.assertNotIn(L(edits[1], edits[2]), fake.calls)

    def test_the_best_variant_is_extended_by_the_other_improvers_while_it_improves(self):
        rows = {"as is": 30, "s32 c1();": 20, "s32 c2();": 22, "s32 c3();": 24, L(C1S, C2S): 14, L(C1S, C2S, C3S): 9}
        _s, fake, result = sweep(rows, own=(), per={"c1": [C1S], "c2": [C2S], "c3": [C3S]})
        self.assertEqual(result["best_rows"], 9)
        self.assertEqual(len(result["best"]), 3)

    def test_the_search_stops_at_the_first_zero(self):
        _s, fake, result = sweep({"as is": 20, "s32 f();": 0})
        self.assertEqual(result["status"], "match")
        self.assertEqual(fake.calls, ["as is", "s32 f();"])
        self.assertEqual(result["best"], ["s32 f();"])

    def test_a_zero_in_a_parallel_batch_stops_the_remaining_probes(self):
        _s, fake, result = sweep({"as is": 20, "s32 f();": 0}, jobs=2, delay=0.05)
        self.assertEqual(result["status"], "match")
        self.assertLessEqual(len(fake.calls), 4)          # the baseline, the two probes in flight, nothing after the zero

    def test_a_baseline_of_zero_is_reported_and_nothing_else_is_compiled(self):
        _s, fake, result = sweep({"as is": 0})
        self.assertEqual((result["status"], fake.calls), ("matches-as-is", ["as is"]))

    def test_the_budget_includes_the_baseline_and_cuts_a_batch(self):
        _s, fake, result = sweep({"as is": 20}, budget=4)
        self.assertEqual(fake.calls, ["as is", "s32 f();", "s32 f(void);", "s32 c1();"])
        self.assertEqual(result["compiled"], 4)
        self.assertTrue(result["budget_cut"])

    def test_a_budget_that_covers_everything_is_not_cut(self):
        _s, fake, result = sweep({"as is": 20}, budget=150)
        self.assertFalse(result["budget_cut"])
        self.assertEqual(result["compiled"], 7)

    def test_the_budget_also_ends_the_later_phases(self):
        rows = {"as is": 30, "s32 f();": 10, "s32 c1();": 20, "s32 c2();": 21}
        _s, fake, result = sweep(rows, budget=9)
        self.assertEqual(len(fake.calls), 9)
        self.assertTrue(result["budget_cut"])

    def test_no_variant_is_compiled_twice(self):
        rows = {"as is": 30, "s32 f();": 20, "s32 c1();": 20}
        _s, fake, _r = sweep(rows)
        self.assertEqual(len(fake.calls), len(set(fake.calls)))

    def test_parallel_probes_are_limited_to_jobs(self):
        _s, fake, result = sweep({"as is": 20}, jobs=2, delay=0.02)
        self.assertEqual(fake.peak, 2)
        self.assertEqual(result["compiled"], len(fake.calls))


class Untested(unittest.TestCase):
    def test_a_probe_that_fails_once_is_retried_serially_and_counted(self):
        s, fake, result = sweep({"as is": 20, "s32 c1();": 7}, jobs=2, flaky=["s32 c1();"])
        self.assertEqual(fake.calls.count("s32 c1();"), 2)
        self.assertEqual(fake.running_at_call["s32 c1();"][-1], 1)        # alone: the retry runs after the batch
        self.assertEqual(result["untested"], [])
        self.assertEqual(result["best_rows"], 7)
        self.assertEqual(result["compiled"], len(fake.calls))

    def test_a_probe_that_fails_twice_is_untested_not_no_gain(self):
        _s, fake, result = sweep({"as is": 20}, fail=["s32 c2();"])
        self.assertEqual(fake.calls.count("s32 c2();"), 2)
        self.assertEqual([label for label, _e in result["untested"]], ["s32 c2();"])
        self.assertIn("build.ninja", result["untested"][0][1])
        self.assertTrue(result["untested"][0][1].startswith("ValueError: "))
        listed = {tuple(v["edits"]): v for v in result["variants"]}
        self.assertIsNone(listed[("s32 c2();",)]["rows"])
        self.assertEqual(result["status"], "no-gain")                      # the others still scored; this one is open
        self.assertEqual(result["baseline"], 20)

    def test_an_untested_variant_never_wins_and_is_not_paired(self):
        rows = {"as is": 20, "s32 c1();": 10, "s32 c3();": 12}
        _s, fake, result = sweep(rows, fail=["s32 c2();"])
        self.assertFalse(any("c2()" in c and " ; " in c for c in fake.calls))
        self.assertEqual(result["best"], ["s32 c1();", "s32 c3();"] if result["best_rows"] < 10 else ["s32 c1();"])

    def test_an_edit_the_file_cannot_take_is_reported_without_a_retry(self):
        _s, fake, result = sweep({"as is": 20}, proto_error=["s32 f(void);"])
        self.assertEqual(fake.calls.count("s32 f(void);"), 1)
        self.assertTrue(result["untested"][0][1].startswith("ProtoError"))

    def test_a_baseline_that_does_not_compile_still_sweeps_the_variants(self):
        _s, fake, result = sweep({"s32 f();": 40, "s32 c1();": 30}, fail=["as is"])
        self.assertIsNone(result["baseline"])
        self.assertIn("build.ninja", result["baseline_error"])
        self.assertEqual(result["status"], "improved")
        self.assertEqual(result["best_rows"], 30)

    def test_nothing_that_compiles_is_a_failure(self):
        s = vs.Sweep(FakeScore({}, fail=["as is", "s32 c1();"]), [], {"c1": [C1S]}, None, 10, 1, log=lambda m: None)
        result = s.run()
        self.assertEqual(result["status"], "failed")
        self.assertIsNone(result["best_rows"])


class Errors(unittest.TestCase):
    def test_the_compiler_error_line_is_the_summary(self):
        err = ValueError("python3 failed:\ncfe: Error: build/viewsweep/RPG_BAT_f/ab/guarded.o.sjis.c, line 3359: redeclaration of 'f'\n"
                         " void f(void) {\n -----^\ncc.py: stage failed (55): python3 /opt/asm-processor/build.py")
        self.assertEqual(vs.error_summary(err), "cfe: Error: unit, line 3359: redeclaration of 'f'")

    def test_without_a_compiler_error_it_is_the_last_line(self):
        self.assertEqual(vs.error_summary(ValueError("ninja failed:\nbuild.ninja: No such file")), "build.ninja: No such file")
        self.assertEqual(vs.error_summary(OSError()), "OSError")
        self.assertEqual(len(vs.error_summary(ValueError("x" * 500))), 160)


class Targets(unittest.TestCase):
    def test_a_targets_file(self):
        text = "# comment\n\nRPG_BAT:func_8013E54C\nmain:func_80144094 drafts/T-17016/main/func_80144094/body.c  # x\n"
        self.assertEqual(vs.parse_targets(text), [("RPG_BAT:func_8013E54C", None),
                                                  ("main:func_80144094", "drafts/T-17016/main/func_80144094/body.c")])
        with self.assertRaises(ValueError):
            vs.parse_targets("a b c\n")

    def test_archived_bodies_are_found_by_their_folder_names(self):
        with tempfile.TemporaryDirectory() as tmp:
            for rel in ("main/f1/body.c", "TACO/f2/body.c", "TACO/f3/NOTES.txt"):
                path = Path(tmp, rel)
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text("x")
            got = vs.draft_bodies([tmp])
        self.assertEqual([s for s, _p in got], ["TACO:f2", "main:f1"])

    def test_a_body_replaces_the_include_asm_line_as_a_draft(self):
        text = 'void a(void) {}\nINCLUDE_ASM("asm/nonmatchings/X", f);\nINCLUDE_ASM("asm/nonmatchings/X", g);\n'
        out = vs.insert_body(text, "f", "void f(void) {\n}\n")
        self.assertEqual(out, 'void a(void) {}\n#ifdef NON_MATCHING\nvoid f(void) {\n}\n#else\n'
                              'INCLUDE_ASM("asm/nonmatchings/X", f);\n#endif\nINCLUDE_ASM("asm/nonmatchings/X", g);\n')
        self.assertIsNone(vs.insert_body(text, "missing", "x"))

    def test_make_target_refuses_a_body_for_a_function_that_is_c_already(self):
        c = SimpleNamespace(unit="main", src=Path("src/main/a.c"))
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp, "a.c")
            src.write_text("void f(void) {\n}\n")
            body = Path(tmp, "b.c")
            body.write_text("void f(void) {}\n")
            c = SimpleNamespace(unit="main", src=src)
            with mock.patch("funcloc.locate", return_value=c):
                with self.assertRaisesRegex(ValueError, "is C or guarded"):
                    vs.make_target("main:f", body=str(body), work=Path(tmp, "w"))

    def test_make_target_writes_the_scratch_copy_of_a_restore(self):
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp, "a.c")
            src.write_text('void other(void) {\n}\nINCLUDE_ASM("asm/nonmatchings/a", f);\n')
            body = Path(tmp, "b.c")
            body.write_text("void f(void) {\n    other();\n}\n")
            c = SimpleNamespace(unit="main", src=src)
            with mock.patch("funcloc.locate", return_value=c):
                t = vs.make_target("main:f", body=str(body), work=Path(tmp, "w"))
            self.assertTrue(t.non_matching)
            self.assertEqual(t.scope, "main:f")
            self.assertIn("#ifdef NON_MATCHING\nvoid f(void) {", Path(t.source).read_text())
            self.assertEqual(src.read_text().count("NON_MATCHING"), 0)                  # the tree is untouched

    def test_a_function_without_a_c_body_needs_a_draft(self):
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp, "a.c")
            src.write_text('INCLUDE_ASM("asm/nonmatchings/a", f);\n')
            with mock.patch("funcloc.locate", return_value=SimpleNamespace(unit="main", src=src)):
                with self.assertRaisesRegex(ValueError, "no C body"):
                    vs.make_target("main:f")


class Scoring(unittest.TestCase):
    def make(self, tmp, got_keys):
        c = SimpleNamespace(unit="main", src=Path("src/main/a.c"), obj="build/a.o")
        target = vs.Target("main:target", "target", "main", c, Path("src/main/a.c"), "", True, None, "guarded draft")
        funcdiff = mock.Mock()
        funcdiff.expected_object.return_value = ("expected.o", None)
        funcdiff.resolved_functions.side_effect = lambda obj, names, c=None: {names[0]: ["a", "b", "c"] if obj == "expected.o" else got_keys}
        funcdiff.keys.side_effect = lambda rows: list(rows)
        funcdiff.diff_lines.side_effect = lambda want, got: ["-x", "+y"] * 3
        proof = mock.Mock()
        proof.compile_unit.side_effect = lambda c, directory, non_matching, edit, source: ((directory / "guarded.o").as_posix(), [])
        with mock.patch.dict(sys.modules, {"funcdiff": funcdiff, "funcdiff_verify": proof}):
            scorer = vs.Scorer(target, Path(tmp))
        return scorer, proof

    def test_rows_are_the_diff_lines_and_zero_keeps_the_object(self):
        with tempfile.TemporaryDirectory() as tmp:
            scorer, proof = self.make(tmp, ["a", "x", "c"])
            self.assertEqual(scorer(()), 6)
            self.assertEqual(scorer.objects, {})
            self.assertFalse(any(Path(tmp).rglob("guarded.o")))
            scorer, proof = self.make(tmp, ["a", "b", "c"])
            self.assertEqual(scorer((E("c1", "s32 c1();"),)), 0)
            self.assertEqual(list(scorer.objects), [(E("c1", "s32 c1();"),)])

    def test_the_edit_is_applied_to_the_preprocessed_text_and_baseline_has_none(self):
        with tempfile.TemporaryDirectory() as tmp:
            scorer, proof = self.make(tmp, ["a", "x", "c"])
            scorer(())
            self.assertIsNone(proof.compile_unit.call_args.kwargs["edit"])
            scorer((E("target", "s32 target();", "own"),))
            edit = proof.compile_unit.call_args.kwargs["edit"]
            self.assertIn("s32 target() {", edit(UNIT))
            self.assertTrue(proof.compile_unit.call_args.kwargs["non_matching"])

    def test_a_function_missing_from_the_object_is_an_error_not_a_score(self):
        with tempfile.TemporaryDirectory() as tmp:
            scorer, _ = self.make(tmp, None)
            with self.assertRaisesRegex(ValueError, "missing from the compiled object"):
                scorer(())


class Proof(unittest.TestCase):
    def setup(self, tmp, results):
        c = SimpleNamespace(unit="main", src=Path("src/main/a.c"), obj="build/a.o")
        text = ("#ifdef NON_MATCHING\nvoid target(void) {\n}\n#else\nINCLUDE_ASM(\"asm/nonmatchings/a\", target);\n#endif\n"
                "#ifdef NON_MATCHING\nvoid other(void) {\n}\n#else\nINCLUDE_ASM(\"asm/nonmatchings/a\", other);\n#endif\n")
        target = vs.Target("main:target", "target", "main", c, Path("a.c"), text, True, None, "guarded draft")
        proof = mock.Mock()
        proof.exact_match.side_effect = list(results)
        scorer = SimpleNamespace(proof=proof, objects={(): ("o.o", Path(tmp))},
                                 compile=mock.Mock(return_value=("iso.o", Path(tmp))))
        return target, scorer, proof

    def test_an_equal_unit_is_enough(self):
        with tempfile.TemporaryDirectory() as tmp:
            target, scorer, proof = self.setup(tmp, [(True, "linked function bytes equal target (8 bytes)")])
            self.assertEqual(vs.prove(target, scorer, ()), (True, "linked function bytes equal target (8 bytes)"))
            scorer.compile.assert_not_called()

    def test_another_guarded_draft_moving_the_function_falls_back_to_the_isolated_unit(self):
        fake_autotweak = mock.Mock()
        fake_autotweak.guards.return_value = [SimpleNamespace(func="target", body=20, else_start=40)]
        fake_autotweak.rewrite_guard.return_value = "void target(void) {\n}\n"
        with tempfile.TemporaryDirectory() as tmp, mock.patch.dict(sys.modules, {"autotweak": fake_autotweak}):
            target, scorer, proof = self.setup(tmp, [(False, "linked function address or size differs from target"),
                                                     (True, "linked function bytes equal target (8 bytes)")])
            equal, reason = vs.prove(target, scorer, ())
            self.assertTrue(equal)
            self.assertIn("only this function as C", reason)
            self.assertEqual(scorer.compile.call_args.kwargs["non_matching"], False)
            self.assertEqual(Path(tmp, "isolated.c").read_text(), "void target(void) {\n}\n")

    def test_different_bytes_stay_different(self):
        fake_autotweak = mock.Mock()
        fake_autotweak.guards.return_value = [SimpleNamespace(func="target", body=20, else_start=40)]
        fake_autotweak.rewrite_guard.return_value = "x"
        with tempfile.TemporaryDirectory() as tmp, mock.patch.dict(sys.modules, {"autotweak": fake_autotweak}):
            target, scorer, proof = self.setup(tmp, [(False, "linked function bytes differ from target (8 bytes)")] * 2)
            self.assertFalse(vs.prove(target, scorer, ())[0])

    def test_a_missing_proof_is_a_failed_proof(self):
        with tempfile.TemporaryDirectory() as tmp:
            target, scorer, proof = self.setup(tmp, [])
            proof.exact_match.side_effect = ValueError("ninja failed:\nheaders")
            target = target._replace(text="void target(void) {\n}\n")
            fake_autotweak = mock.Mock()
            fake_autotweak.guards.return_value = []
            with mock.patch.dict(sys.modules, {"autotweak": fake_autotweak}):
                equal, reason = vs.prove(target, scorer, ())
            self.assertFalse(equal)
            self.assertIn("literal-byte proof unavailable: headers", reason)


class Recipe(unittest.TestCase):
    def tree(self, tmp):
        Path(tmp, "include/ovl").mkdir(parents=True)
        Path(tmp, "include/ovl/X.h").write_text("#ifndef X_H\nvoid target(void);\nvoid func_8013E97C(s32 a, s32 b, s32 c);\n"
                                                "#ifndef X_OVERRIDE_func_8013E111\nu8 func_8013E111(u8 a);\n#endif\n#endif\n")
        Path(tmp, "src/ovl/X").mkdir(parents=True)
        Path(tmp, "src/ovl/X/8013.c").write_text("void target(void) {\n    func_8013E111(1);\n}\n")
        Path(tmp, "src/main").mkdir(parents=True)
        Path(tmp, "src/main/8004.c").write_text("void m(void) {\n    target();\n}\n")

    def test_the_recipe_names_definition_header_guard_and_callers(self):
        with tempfile.TemporaryDirectory() as tmp:
            self.tree(tmp)
            c = SimpleNamespace(unit="X", src=Path("src/ovl/X/8013.c"))
            target = vs.Target("X:target", "target", "X", c, c.src, "", True, None, "guarded draft")
            lines = vs.recipe(target, [E("func_8013E111", "s32 func_8013E111();"), E("target", "s32 target();", "own")],
                              UNIT.replace("void target(void) {", "void target(void) {"), tmp,
                              unit_of=lambda p: "X" if "/ovl/" in p else "main")
        text = "\n".join(lines)
        self.assertIn("definition in src/ovl/X/8013.c: change `void target(void)` to `s32 target()`", text)
        self.assertIn("so `$v0` stays live at the exit", text)
        self.assertIn("include/ovl/X.h:2: `void target(void)` becomes `s32 target()`, the type of the definition", text)
        self.assertIn("alternative to the guard: change include/ovl/X.h:5 `u8 func_8013E111(u8 a)` to `s32 func_8013E111()`", text)
        self.assertIn("#define X_OVERRIDE_func_8013E111", text)
        self.assertIn("uses of target (2, units X, main)", text)
        self.assertIn("every unit above (X, main) must stay sha1 OK", text)

    def test_a_header_that_has_the_type_already_needs_no_change(self):
        with tempfile.TemporaryDirectory() as tmp:
            self.tree(tmp)
            Path(tmp, "include/ovl/X.h").write_text("s32 target();\n")
            c = SimpleNamespace(unit="X", src=Path("src/ovl/X/8013.c"))
            target = vs.Target("X:target", "target", "X", c, c.src, "", True, None, "guarded draft")
            lines = vs.recipe(target, [E("target", "s32 target();", "own")], UNIT, tmp, unit_of=lambda p: "X")
        self.assertIn("include/ovl/X.h:1 already says `s32 target()`", "\n".join(lines))

    def test_a_restore_says_where_the_body_goes(self):
        with tempfile.TemporaryDirectory() as tmp:
            self.tree(tmp)
            c = SimpleNamespace(unit="X", src=Path("src/ovl/X/8013.c"))
            target = vs.Target("X:target", "target", "X", c, c.src, "", True, "drafts/T-1/X/target/body.c", "archived draft")
            lines = vs.recipe(target, [E("target", "s32 target();", "own")], UNIT, tmp, unit_of=lambda p: "X")
        self.assertIn("after putting the draft body drafts/T-1/X/target/body.c in place of the INCLUDE_ASM", lines[0])


class Cli(unittest.TestCase):
    def result(self, scope, status, base, best, edits=()):
        return dict(scope=scope, status=status, baseline=base, best_rows=best, best=list(edits), compiled=12,
                    untested=[], variants=[], seconds=1.5)

    def test_the_table_is_ranked_by_best_rows_then_gain(self):
        table = vs.format_table([self.result("a:x", "no-gain", 10, 10), self.result("b:y", "match", 30, 0, ["s32 y();"]),
                                 self.result("c:z", "improved", 90, 8, ["void c();"]),
                                 dict(self.result("d:w", "failed", None, None))])
        names = [line.split()[0] for line in table.splitlines()[2:]]
        self.assertEqual(names, ["b:y", "c:z", "a:x", "d:w"])
        self.assertIn("err", table.splitlines()[-1])

    def test_json_goes_to_one_file_per_target_and_resume_skips_them(self):
        with tempfile.TemporaryDirectory() as tmp:
            outdir = Path(tmp, "out")
            run = mock.Mock(side_effect=lambda t, j, b, log: self.result(t.scope, "no-gain", 5, 5))
            make = mock.Mock(side_effect=lambda spec, unit, body, src: SimpleNamespace(scope=spec))
            old = os.getcwd()
            os.chdir(tmp)
            Path(tmp, "config").mkdir()
            try:
                with mock.patch.object(vs, "make_target", make), mock.patch.object(vs, "sweep_target", run), \
                        contextlib.redirect_stdout(io.StringIO()) as out:
                    vs.main(["a:x", "b:y", "--json", str(outdir)])
                    self.assertEqual(sorted(p.name for p in outdir.iterdir()), ["a_x.json", "b_y.json"])
                    self.assertEqual(json.loads(Path(outdir, "a_x.json").read_text())["scope"], "a:x")
                    self.assertEqual(run.call_count, 2)
                    vs.main(["a:x", "b:y", "c:z", "--json", str(outdir), "--resume"])
                    self.assertEqual(run.call_count, 3)
                    self.assertIn("resumed", out.getvalue())
                    vs.main(["a:x", "--json", str(Path(tmp, "all.json"))])
                    self.assertEqual(len(json.loads(Path(tmp, "all.json").read_text())), 1)
            finally:
                os.chdir(old)

    def test_a_target_that_cannot_be_swept_is_an_error_row_not_a_crash(self):
        with tempfile.TemporaryDirectory() as tmp:
            Path(tmp, "config").mkdir()
            old = os.getcwd()
            os.chdir(tmp)
            try:
                with mock.patch.object(vs, "make_target", side_effect=ValueError("main:f has no C body (INCLUDE_ASM only)")), \
                        contextlib.redirect_stdout(io.StringIO()) as out:
                    self.assertEqual(vs.main(["main:f"]), 0)
            finally:
                os.chdir(old)
        self.assertIn("error", out.getvalue())
        self.assertIn("no C body", out.getvalue())

    def test_an_archived_draft_that_is_c_already_is_skipped(self):
        with tempfile.TemporaryDirectory() as tmp:
            Path(tmp, "config").mkdir()
            old = os.getcwd()
            os.chdir(tmp)
            try:
                with mock.patch.object(vs, "make_target", side_effect=ValueError("main:f is C or guarded in x.c already")), \
                        contextlib.redirect_stdout(io.StringIO()) as out:
                    vs.main(["--body", "b.c", "--scope", "main:f"])
            finally:
                os.chdir(old)
        self.assertIn("skipped", out.getvalue())
        self.assertNotIn("error", out.getvalue().split("viewsweep:")[0])

    def test_the_arguments_are_checked(self):
        with tempfile.TemporaryDirectory() as tmp:
            Path(tmp, "config").mkdir()
            old = os.getcwd()
            os.chdir(tmp)
            try:
                for argv in ([], ["--body", "b.c"], ["a:x", "b:y", "--src", "f.c"]):
                    with self.assertRaises(SystemExit, msg=argv), contextlib.redirect_stderr(io.StringIO()):
                        vs.main(argv)
            finally:
                os.chdir(old)


class EndToEnd(unittest.TestCase):
    """sweep_target with the compiler, the asm and the proof replaced: the plumbing from unit text to result."""

    ASM = """glabel target
    /* 0 80100000 00000000 */  jal func_8013E7C0
    /* 4 80100004 00000000 */   nop
    /* 8 80100008 00004210 */  addu $t0, $v0, $zero
    /* C 8010000C 00000000 */  jal func_8013E97C
    /* 10 80100010 00000000 */   nop
    /* 14 80100014 00000000 */  jr $ra
    /* 18 80100018 00000000 */   or $v0, $a0, $zero
endlabel target
"""

    def test_a_sweep_finds_the_match_and_prints_the_recipe(self):
        seen = []

        class FakeScorer:
            def __init__(self, target, *a, **k):
                self.root = Path(tempfile.mkdtemp())
                self.objects = {}

            def __call__(self, edits):
                seen.append(vs.variant_label(edits))
                decls = sorted(e.decl for e in edits)
                rows = 0 if decls == ["s32 target();", "void func_8013E97C();"] else 5 if decls == ["s32 target();"] else 9
                if rows == 0:
                    self.objects[vs.variant_key(edits)] = ("o.o", Path("."))
                return rows

        with tempfile.TemporaryDirectory() as tmp:
            asm = Path(tmp, "target.s")
            asm.write_text(self.ASM)
            c = SimpleNamespace(unit="X", src=Path("src/ovl/X/8013.c"))
            target = vs.Target("X:target", "target", "X", c, c.src, "", True, None, "guarded draft")
            proof = mock.Mock()
            proof.preprocess_draft.return_value = UNIT
            tp = mock.Mock()
            tp.find_asm.return_value = str(asm)
            with mock.patch.object(vs, "Scorer", FakeScorer), mock.patch.object(vs, "prove", return_value=(True, "equal")), \
                    mock.patch.object(vs, "recipe", return_value=["definition in ..."]), \
                    mock.patch.dict(sys.modules, {"funcdiff_verify": proof, "trailing_pad": tp}):
                result = vs.sweep_target(target, jobs=1, budget=60, log=lambda m: None)
        self.assertEqual(result["status"], "match")
        self.assertEqual(sorted(result["best"]), ["s32 target();", "void func_8013E97C();"])
        self.assertEqual(result["exact"], {"equal": True, "reason": "equal"})
        self.assertEqual(result["recipe"], ["definition in ..."])
        # the asm lists func_8013E7C0 and func_8013E97C only: the other callees of the body are not probed
        self.assertFalse(any("func_8013E111" in label or "helper" in label for label in seen))
        # the asm leaves a copy in $v0 at the exit: the own edit goes first
        self.assertEqual(seen[:2], ["as is", "s32 target();"])
        self.assertEqual(result["lattice"]["own"], ["s32 target();", "s32 target(void);"])

    def test_an_unproven_zero_is_not_a_match(self):
        class FakeScorer:
            def __init__(self, target, *a, **k):
                self.root = Path(tempfile.mkdtemp())
                self.objects = {}

            def __call__(self, edits):
                if edits:
                    self.objects[vs.variant_key(edits)] = ("o.o", Path("."))
                return 0 if edits else 3

        c = SimpleNamespace(unit="X", src=Path("src/ovl/X/8013.c"))
        target = vs.Target("X:target", "target", "X", c, c.src, "", True, None, "guarded draft")
        proof = mock.Mock()
        proof.preprocess_draft.return_value = UNIT
        tp = mock.Mock()
        tp.find_asm.return_value = None
        with mock.patch.object(vs, "Scorer", FakeScorer), mock.patch.object(vs, "prove", return_value=(False, "differs")), \
                mock.patch.dict(sys.modules, {"funcdiff_verify": proof, "trailing_pad": tp}):
            result = vs.sweep_target(target, jobs=1, budget=60, log=lambda m: None)
        self.assertEqual(result["status"], "match-unproven")
        self.assertEqual(result["recipe"], [])

    def test_a_target_without_a_definition_is_reported(self):
        class FakeScorer:
            def __init__(self, target, *a, **k):
                self.root = Path(tempfile.mkdtemp())

        c = SimpleNamespace(unit="X", src=Path("src/ovl/X/8013.c"))
        target = vs.Target("X:nothing", "nothing", "X", c, c.src, "", True, None, "guarded draft")
        proof = mock.Mock()
        proof.preprocess_draft.return_value = UNIT
        with mock.patch.object(vs, "Scorer", FakeScorer), mock.patch.dict(sys.modules, {"funcdiff_verify": proof}):
            result = vs.sweep_target(target, 1, 10, log=lambda m: None)
        self.assertEqual(result["status"], "no-definition")


if __name__ == "__main__":
    unittest.main()
