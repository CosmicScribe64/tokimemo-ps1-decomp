"""Unit tests for tools/autotweak.py (synthetic C only, no game data; the scorer is mocked).

Run: python3 tools/test_autotweak.py   (from the repo root; no Docker needed)
"""
import io
import os
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import autotweak as A  # noqa: E402


def labels(text, prefix):
    return [v for v in A.variants(text) if v.label.startswith(prefix)]


def texts(text, prefix):
    return [v.text for v in labels(text, prefix)]


FUNC = """void func_80132000(void) {
    s32 i;
    u16 k;

    i = D_80140000 + 1;
    k = i * 4;
    func_80132100(i, k);
}"""


class Transforms(unittest.TestCase):
    def test_byteview_on_global_selector(self):
        f = "void f(void) {\n    switch (D_X) {\n    case 1:\n        break;\n    }\n}"
        out = labels(f, "byteview")
        self.assertEqual(len(out), 1)
        self.assertIn("switch (*(u8 *)&D_X) { /* FAKE: scalar byte view of the switch selector. T-11170 */",
                      out[0].text)
        self.assertTrue(out[0].fake)

    def test_byteview_skips_locals(self):
        f = "void f(void) {\n    s32 v;\n    v = 1;\n    switch (v) {\n    }\n}"
        self.assertEqual(labels(f, "byteview"), [])

    def test_swap_commutative(self):
        f = "void f(void) {\n    x = a + b;\n    if (a == 1) {\n    }\n    if (p->q != r[2]) {\n    }\n}"
        got = texts(f, "swap")
        self.assertIn(f.replace("a + b", "b + a"), got)
        self.assertIn(f.replace("a == 1", "1 == a"), got)
        self.assertIn(f.replace("p->q != r[2]", "r[2] != p->q"), got)

    def test_swap_respects_precedence(self):
        for body in ("x = -a + b;", "x = c * a + b;", "x = c - a + b;", "x = a + b * c;",
                     "if (*(u8 *)&D_X != 0) {\n    }", "x = !a == b;"):
            f = "void f(void) {\n    %s\n}" % body
            for t in texts(f, "swap"):
                self.assertNotIn(t, (f.replace("a + b", "b + a"), f.replace("D_X != 0", "0 != D_X"),
                                     f.replace("a == b", "b == a")), body)

    def test_swap_cast_operand(self):
        f = "void f(void) {\n    x = (s32)a + b;\n}"
        self.assertIn(f.replace("(s32)a + b", "b + (s32)a"), texts(f, "swap"))

    def test_join_and_split(self):
        f = "void f(void) {\n    a = 1;\n    b = 2;\n}"
        self.assertEqual(texts(f, "join"), ["void f(void) {\n    a = 1; b = 2;\n}"])
        g = "void f(void) {\n    a = 1; b = 2;\n}"
        self.assertEqual(texts(g, "split"), [f])

    def test_join_skips_control_lines(self):
        f = "void f(void) {\n    if (x)\n        a = 1;\n    b = 2;\n}"
        self.assertEqual(texts(f, "join"), [])
        g = "void f(void) {\n    for (i = 0; i < 3; i++) a = 1;\n}"
        self.assertEqual(texts(g, "split"), [])

    def test_compound_both_ways(self):
        f = "void f(void) {\n    x = x + k;\n}"
        self.assertEqual(texts(f, "compound"), ["void f(void) {\n    x += k;\n}"])
        g = "void f(void) {\n    p->n += k;\n    m -= a + b;\n}"
        got = texts(g, "expand")
        self.assertIn("void f(void) {\n    p->n = p->n + k;\n    m -= a + b;\n}", got)
        self.assertIn("void f(void) {\n    p->n += k;\n    m = m - (a + b);\n}", got)

    def test_compound_needs_tighter_rest(self):
        f = "void f(void) {\n    x = x - a + b;\n}"
        self.assertEqual(texts(f, "compound"), [])

    def test_emptytest(self):
        out = labels(FUNC, "emptytest i")
        self.assertTrue(out and all(v.fake for v in out))
        first = out[0].text.split("\n")
        self.assertEqual(first[4], "    if (i) {} /* FAKE: empty test splits the live range of i. T-11170 */")
        self.assertEqual(first[5], "    i = D_80140000 + 1;")

    def test_emptytest_not_into_braceless_if_or_before_else(self):
        f = ("void f(void) {\n    s32 v;\n    if (a)\n        v = 1;\n    if (b) {\n        c = 0;\n    }\n"
             "    else {\n        v = 2;\n    }\n}")
        for v in labels(f, "emptytest"):
            lines = v.text.split("\n")
            n = next(i for i, l in enumerate(lines) if "FAKE" in l)
            self.assertFalse(lines[n - 1].rstrip().endswith(")"), v.label)
            self.assertFalse(lines[n + 1].strip().startswith("else"), v.label)

    def test_pad(self):
        out = labels(FUNC, "pad")
        self.assertEqual([v.label for v in out], ["pad first", "pad last"])
        self.assertEqual(out[0].text.split("\n")[1],
                         "    s32 pad; /* FAKE: unused local reserves a stack word (frame home). T-11170 */")
        self.assertEqual(out[1].text.split("\n")[3].split(";")[0], "    s32 pad")

    def test_declswap(self):
        self.assertEqual(texts(FUNC, "declswap"), [FUNC.replace("    s32 i;\n    u16 k;", "    u16 k;\n    s32 i;")])

    def test_shift_both_ways(self):
        self.assertIn(FUNC.replace("i * 4", "(i << 2)"), texts(FUNC, "mul>shift"))
        f = "void f(void) {\n    x = (col << 4) - 0x80;\n    y = b + c << 2;\n}"
        self.assertEqual(texts(f, "shift>mul"), [f.replace("(col << 4)", "((col * 16))")])

    def test_shift_skips_casts_and_division(self):
        f = "void f(void) {\n    x = (s16)a * 4;\n    y = b / c * 8;\n}"
        # the cast stays on its operand; (b / c) * 8 is not b / (c << 3)
        self.assertEqual(texts(f, "mul>shift"), [f.replace("(s16)a * 4", "((s16)a << 2)")])

    def test_temp_inline(self):
        f = "void f(void) {\n    s32 t;\n    t = g(1) + 2;\n    h(t);\n}"
        self.assertIn("void f(void) {\n    h((g(1) + 2));\n}", texts(f, "inline"))

    def test_temp_introduce(self):
        f = "void f(void) {\n    s32 i;\n    switch (D_X.f) {\n    }\n}"
        got = texts(f, "temp")
        self.assertIn("void f(void) {\n    s32 i;\n    u8 temp;\n    temp = D_X.f;\n    switch (temp) {\n    }\n}", got)
        g = "void f(void) {\n    p->a = g(2);\n}"
        self.assertIn("void f(void) {\n    s32 temp;\n    temp = g(2);\n    p->a = temp;\n}", texts(g, "temp"))

    def test_width(self):
        got = texts(FUNC, "width")
        self.assertIn(FUNC.replace("u16 k;", "s16 k;"), got)
        self.assertIn(FUNC.replace("s32 i;", "u32 i;"), got)
        f = "s32 f(arg0)\nu8 arg0;\n{\n    return arg0;\n}"
        self.assertEqual(texts(f, "width"), [f.replace("u8 arg0", "s8 arg0")])

    def test_comments_and_strings_are_not_edited(self):
        f = 'void f(void) {\n    /* a + b */\n    g("a + b", x + y);\n}'
        for t in texts(f, "swap"):
            self.assertIn("/* a + b */", t)
            self.assertIn('"a + b"', t)
        self.assertIn(f.replace("x + y", "y + x"), texts(f, "swap"))

    def test_variants_are_deduplicated(self):
        out = A.variants(FUNC)
        self.assertEqual(len(out), len({v.text for v in out}))
        self.assertNotIn(FUNC, [v.text for v in out])


SRC = """#include "common.h"

void func_80132000(void) {
    x = a + b;
    switch (D_X) {
    }
}

/* NON_MATCHING: T-0001, register swap */
#ifdef NON_MATCHING
/* FAKE: keep me. T-0001 */
void func_80132100(void) {
    s32 i;
    i = D_80140000 + 1;
    func_80132200(i * 4);
}
#else
INCLUDE_RODATA("asm/ovl/AA/nonmatchings/AA/80132000", D_80132300);
INCLUDE_ASM("asm/ovl/AA/nonmatchings/AA/80132000", func_80132100);
#endif

INCLUDE_ASM("asm/ovl/AA/nonmatchings/AA/80132000", func_80132400);
"""


class Repo(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = self.tmp.name
        self.path = os.path.join(self.root, "src", "ovl", "AA", "80132000.c")
        os.makedirs(os.path.dirname(self.path))
        with open(self.path, "w") as fh:
            fh.write(SRC)

    def tearDown(self):
        self.tmp.cleanup()

    def read(self):
        with open(self.path) as fh:
            return fh.read()


class Drafts(Repo):
    def test_find_drafts(self):
        ds = A.find_drafts(self.root)
        self.assertEqual([(d.unit, d.func, d.src) for d in ds], [("AA", "func_80132100", "src/ovl/AA/80132000.c")])
        self.assertTrue(ds[0].text.startswith("void func_80132100(void) {"))
        self.assertTrue(ds[0].text.endswith("}"))
        self.assertEqual(A.find_drafts(self.root, "BB:func_80132100"), [])
        self.assertEqual(A.find_drafts(self.root, exclude={"AA"}), [])

    def test_scoping_stays_inside_the_function(self):
        d = A.find_drafts(self.root)[0]
        for v in A.variants(d.text):
            new = A.rewrite_guard(SRC, d.func, v.text)
            self.assertTrue(new.startswith(SRC[:SRC.index("/* NON_MATCHING")]), v.label)
            self.assertTrue(new.endswith(SRC[SRC.index("\nINCLUDE_ASM(\"asm/ovl/AA/nonmatchings/AA/80132000\", func_80132400"):]),
                            v.label)

    def test_rewrite_guard(self):
        d = A.find_drafts(self.root)[0]
        new = A.rewrite_guard(SRC, d.func, d.text.replace("i * 4", "(i << 2)"))
        self.assertNotIn("NON_MATCHING", new)
        self.assertNotIn("#else", new)
        self.assertNotIn("#endif", new)
        self.assertNotIn("INCLUDE_ASM(\"asm/ovl/AA/nonmatchings/AA/80132000\", func_80132100)", new)
        self.assertIn("/* FAKE: keep me. T-0001 */\n"
                      "INCLUDE_RODATA(\"asm/ovl/AA/nonmatchings/AA/80132000\", D_80132300);\n"
                      "void func_80132100(void) {", new)
        self.assertIn("func_80132200((i << 2));\n}\n\nINCLUDE_ASM", new)
        self.assertIsNone(A.rewrite_guard(SRC, "func_80132000", d.text))


class FakeScorer:
    """Score = number of wanted substrings missing from the text."""

    def __init__(self, wanted, verify_ok=True):
        self.wanted, self.verify_ok, self.calls = wanted, verify_ok, 0

    def score(self, text):
        self.calls += 1
        return sum(w not in text for w in self.wanted)

    def verify(self, text):
        return self.verify_ok


class Search(unittest.TestCase):
    def test_single_edit_match(self):
        s = FakeScorer(["(i << 2)"])
        r = A.climb(FUNC, s.score, s.verify, budget=60)
        self.assertTrue(r.matched)
        self.assertEqual((r.base, r.best), (1, 0))
        self.assertIn("(i << 2)", r.text)

    def test_depth_two_combination(self):
        # each edit alone does not improve, only the pair does
        s = FakeScorer([])
        s.score = lambda text: 0 if ("(i << 2)" in text and "u16 k;\n    s32 i;" in text) else 1
        r = A.climb(FUNC, s.score, lambda t: True, budget=60)
        self.assertTrue(r.matched)
        self.assertEqual(len(r.path), 2)

    def test_failed_verification_is_not_a_match(self):
        s = FakeScorer(["(i << 2)"], verify_ok=False)
        r = A.climb(FUNC, s.score, s.verify, budget=60)
        self.assertFalse(r.matched)
        self.assertEqual(r.best, 1)

    def test_unverified_zero_draft_is_not_a_match(self):
        r = A.climb(FUNC, lambda t: 0, lambda t: False, budget=60)
        self.assertFalse(r.matched)
        self.assertTrue(r.unverified)
        self.assertEqual((r.base, r.best, r.text), (0, 0, FUNC))

    def test_unverified_zero_draft_keeps_searching(self):
        r = A.climb(FUNC, lambda t: 0, lambda t: "(i << 2)" in t, budget=60)
        self.assertTrue(r.matched)
        self.assertFalse(r.unverified)
        self.assertIn("(i << 2)", r.text)

    def test_time_budget(self):
        ticks = iter(range(1000))
        s = FakeScorer(["never there"])
        r = A.climb(FUNC, s.score, s.verify, budget=5, clock=lambda: next(ticks))
        self.assertLessEqual(r.tried, 5)

    def test_draft_that_does_not_compile(self):
        r = A.climb(FUNC, lambda t: None)
        self.assertEqual(r.error, "draft does not compile")


def order_func(body, decls="    s32 a;\n    s32 b;\n    s32 v;\n    s32 *p;\n"):
    return "void f(Q *q) {\n%s\n%s\n}" % (decls, "\n".join("    " + l for l in body))


def pair_independent(first, second, decls=None):
    f = A.Func(order_func([first, second]) if decls is None else order_func([first, second], decls))
    i = f.stmt_line
    return A.independent(A.access(f, i), A.access(f, i + 1))


STORES = ["q->x = 0;", "q->y = a;", "q->h = 4;", "q->w = 8;"]


class Order(unittest.TestCase):
    def test_swap_allowed(self):
        for first, second in (("q->x = 0;", "q->y = 1;"), ("a = 1;", "b = 2;"), ("a = g();", "b = 2;"),
                              ("D_X = 1;", "a = 2;"), ("q->x = a;", "q->y = a;"), ("a = q->x;", "b = q->y;"),
                              ("g(a);", "b = a + 1;")):
            self.assertTrue(pair_independent(first, second), (first, second))
            self.assertTrue(pair_independent(second, first), (second, first))

    def test_swap_forbidden(self):
        for first, second in (("a = 1;", "b = a;"),           # the second reads what the first writes
                              ("b = a;", "a = 1;"),           # the second writes what the first reads
                              ("a = 1;", "a = 2;"),           # both write a
                              ("a += 1;", "b = a;"),
                              ("a++;", "b = a;"),
                              ("q->x = 0;", "a = q->y;"),     # store through q, then a read through q
                              ("q->x = 0;", "q->x = 1;"),     # the same store twice
                              ("D_X = 1;", "a = D_X;"),       # global store, then its read
                              ("*p = 1;", "a = *p;"),
                              ("p = q->next;", "a = p[1];"),
                              ("g();", "h();"),               # two calls
                              ("g();", "D_X = 1;"),           # a call and memory
                              ("a = g();", "q->x = 1;"),
                              ("a = 1;", "g(&a);")):          # address-taken local
            self.assertFalse(pair_independent(first, second), (first, second))

    def test_order_edit(self):
        f = order_func(["a = 1;", "b = 2;", "if (a) {", "    v = 3;", "}", "v = a;"])
        got = texts(f, "order")
        self.assertEqual(got, [f.replace("a = 1;\n    b = 2;", "b = 2;\n    a = 1;")])

    def test_order_edit_in_climb(self):
        f = order_func(STORES)
        want = ("q->y = a;\n    q->x = 0;", "q->w = 8;\n    q->h = 4;")
        r = A.climb(f, lambda t: sum(w not in t for w in want), budget=60, order=False)
        self.assertTrue(r.matched)
        self.assertEqual([l.split()[0] for l in r.path], ["order", "order"])

    def test_runs(self):
        f = A.Func(order_func(["a = 1;", "b = 2;", "v = a;", "q->x = 0;", "if (v) {", "    a = 2;",
                               "    b = 3;", "}"]))
        s = f.stmt_line
        self.assertEqual(A.order_runs(f), [(s, s + 1), (s + 2, s + 3), (s + 5, s + 6)])

    def test_runs_are_capped_at_six(self):
        f = A.Func(order_func(["q->f%d = %d;" % (k, k) for k in range(8)]))
        s = f.stmt_line
        self.assertEqual(A.order_runs(f), [(s, s + 5), (s + 6, s + 7)])

    def test_store_runs_first_and_cap(self):
        f = A.Func(order_func(["a = 1;", "b = 2;", "v = 3;", "g(a);", "q->x = 0;", "q->y = 1;", "g(a);",
                               "D_X = a + 1;", "b = D_X == 2;"]))
        s = f.stmt_line
        runs, sites = A.pick_order(f)
        self.assertEqual(runs, [(s + 4, s + 5), (s, s + 2)])
        self.assertEqual([op for *_, op in sites], ["+", "=="])
        runs, sites = A.pick_order(f, max_exhaustive=12)    # 2! * 3! = 12: no room for a swap
        self.assertEqual((len(runs), sites), (2, []))
        runs, sites = A.pick_order(f, max_exhaustive=4)     # the second run does not fit
        self.assertEqual(runs, [(s + 4, s + 5)])
        self.assertEqual(len(sites), 1)
        self.assertEqual(A.pick_order(A.Func(order_func(["a = 1;", "b = a;"]))), ([], []))

    def test_markers_inserted_and_removed(self):
        import order_search
        text = order_func(STORES + ["a = D_X + 1;"])
        f = A.Func(text)
        runs, sites = A.pick_order(f)
        marked = A.mark_order(f, runs, sites)
        self.assertIn("    /* ORDER */\n    q->x = 0;", marked)
        # a = D_X + 1 writes a, which q->y = a reads: it stays outside the block
        self.assertIn("    q->w = 8;\n    /* END ORDER */\n    a = ORDER_SWAP(D_X, +, 1);\n}", marked)
        t = order_search.Template(marked)
        self.assertEqual(t.render(t.identity()), text)
        for state in t.all_states():
            out = t.render(state)
            self.assertNotIn("ORDER", out)
        self.assertNotIn("ORDER", text)

    def test_order_stage_calls_order_search(self):
        import order_search
        text = order_func(STORES)
        want = "q->w = 8;\n    q->h = 4;\n    q->y = a;\n    q->x = 0;"
        scored, verified = [], []

        def score(t):
            scored.append(t)
            return 0 if want in t else 1

        real = order_search.search
        with mock.patch.object(order_search, "search", side_effect=real) as spy:
            got = A.order_stage(text, score, lambda t: verified.append(t) or True, deadline=float("inf"))
        spy.assert_called_once()
        self.assertEqual(spy.call_args.kwargs["max_exhaustive"], 2000)
        self.assertEqual(got[0], 0)
        self.assertIn(want, got[1])
        self.assertTrue(got[2].startswith("order[block 1 order 4,3,2,1"))
        self.assertEqual(verified, [got[1]])
        self.assertFalse(any("ORDER" in t for t in scored))

    def test_order_stage_unverified_zero_and_deadline(self):
        text = order_func(STORES)
        got = A.order_stage(text, lambda t: 0 if t != text else 2, lambda t: False, deadline=float("inf"))
        self.assertIsNone(got)
        ticks = iter(range(100))
        calls = []
        got = A.order_stage(text, lambda t: calls.append(t) or 1, lambda t: True, deadline=3,
                            clock=lambda: next(ticks))
        self.assertLessEqual(len(calls), 3)

    def test_climb_runs_order_stage_when_stalled(self):
        text = order_func(STORES)
        want = "q->w = 8;\n    q->h = 4;\n    q->y = a;\n    q->x = 0;"   # 6 adjacent swaps: beyond depth 2
        score = lambda t: 0 if want in t else 1
        self.assertFalse(A.climb(text, score, budget=60, order=False).matched)
        r = A.climb(text, score, budget=60)
        self.assertTrue(r.matched)
        self.assertTrue(r.path[-1].startswith("order["))
        self.assertIn(want, r.text)
        r = A.climb(text, score, lambda t: False, budget=60)
        self.assertFalse(r.matched)
        self.assertEqual(r.best, 1)


# Synthetic regdiff reports in the format of tools/regdiff.py report() (T-11220); not real code.
TIE_REPORT = """regdiff func_80132000 [AA]  draft: build/autotweak/AA/func_80132000/regdiff.c
  12 draft / 12 original instructions
  2 instruction(s) differ in registers only
  register map (draft -> original): $v0->$v1 x2, $v1->$v0 x2

FIRST DIVERGENCE at draft insn 3
  draft    addu v0,a0,a1
  original addu v1,a0,a1
  draft puts value computed by `addu v0,a0,a1` in $v0, the original in $v1 (result of `addu v0,a0,a1`).
  draft range: bit 5, local i (lines 14,15)
    priority 2 = 1 use(s) + 1 def(s) over 1 unit(s), loop weight 1, denominator 1; constrained, allocated 1st
  the range that holds $v1 where the original puts the other one in the draft: bit 7, local k (lines 15)
    priority 2 = 1 use(s) + 1 def(s), constrained, allocated 2nd, cheapest colour cost 0, got colour 2 ($v1)
  hint: tie. Equal priority 2; bit 5 is coloured before bit 7, so it takes the lower colour.
  [lookup 0.1s, traced compile 1.5s, analysis 0.1s]"""


def report(*body):
    return "regdiff f [AA]  draft: x.c\n  4 draft / 4 original instructions\n" + "\n".join(body) + \
        "\n  [lookup 0.1s, traced compile 1.5s, analysis 0.1s]"


class RegdiffHints(unittest.TestCase):
    def test_tie_names_and_lines(self):
        h = A.parse_regdiff(TIE_REPORT, first_line=11)
        self.assertEqual(h.classes, ["tie"])
        self.assertEqual(h.names, ["i", "k"])
        self.assertEqual(h.lines, [3, 4])
        self.assertEqual(h.summary(), "tie i,k L3,4")
        self.assertEqual(h.kinds[:2], ["order", "inline"])

    def test_classes(self):
        div = "FIRST DIVERGENCE at draft insn 1"
        cases = [
            ("priority", "hint: bit 3 (priority 4) is coloured before bit 4 (priority 2) and takes the lower colour."),
            ("priority", "hint: bit 3 (priority 4) outranks bit 4 (priority 2). The original needs bit 4 first."),
            ("tie", "hint: equal priorities (2): the lower bit position is coloured first."),
            ("extra-range", "hint: colours are tried lowest first, so an interfering range holds the colour(s)"),
            ("missing-range", "hint: the draft took the lower colour 1 ($v0); in the original an interfering"),
            ("ugen", "$t7 is not a uopt colour: ugen (or the assembler for a macro) picked it"),
            ("ugen", "hint: the original's $t6 is not a uopt colour (ugen temporary): an expression shape"),
        ]
        for want, line in cases:
            self.assertEqual(A.parse_regdiff(report(div, "  " + line)).classes, [want], line)

    def test_secondary_classes_and_global_names(self):
        h = A.parse_regdiff(report(
            "FIRST DIVERGENCE at draft insn 1",
            "  draft range: bit 2, global D_X/D_Y (lines 3)",
            "  hint: bit 2 (priority 4) outranks bit 9 (priority 2). The original needs bit 9 first.",
            "  not allocated in the draft: bit 4, local M12 (net zero)",
            "  the original passes this value to the call at its insn 6 in $a0: uopt precolours"))
        self.assertEqual(h.classes, ["priority", "argument", "unallocated"])
        self.assertEqual(h.names, ["D_X", "D_Y"])       # M12 is regdiff's name for an unnamed local
        self.assertEqual(A.parse_regdiff(report("  draft range: local v (lines 9)")).names, [],
                         "names count only inside the FIRST DIVERGENCE block")

    def test_structure_frame_operand_match_error(self):
        frame = report("  STRUCTURE differs in 1 place(s); first at draft insn 0 / original insn 0:",
                       "    - draft    addiu sp,sp,-24", "    + original addiu sp,sp,-32")
        self.assertEqual(A.parse_regdiff(frame).classes, ["frame"])
        shape = report("  STRUCTURE differs in 1 place(s); first at draft insn 2 / original insn 2:",
                       "    - draft    sll v0,a0,2", "    + original addu v0,a0,a0")
        self.assertEqual(A.parse_regdiff(shape).classes, ["structure"])
        swap = report("  operand order: draft `addu v0,a0,a1` against original `addu v0,a1,a0` (same values)")
        self.assertEqual(A.parse_regdiff(swap).classes, ["operand"])
        self.assertEqual(A.parse_regdiff(report("  MATCH: identical instructions, no divergence.")).classes,
                         ["match"])
        err = A.parse_regdiff("regdiff: the draft does not compile:\nerror")
        self.assertTrue(err.error)
        self.assertEqual(err.summary(), "error")

    def test_table_names_real_edit_kinds(self):
        known = {"byteview", "swap", "join", "split", "compound", "expand", "emptytest", "pad", "declswap",
                 "shift", "inline", "temp", "width", "order"} | set(A.typefix.KINDS)  # T-11240
        produced = {A.edit_kind(v.label) for v in A.variants(FUNC)}
        self.assertTrue(produced <= known, produced - known)
        for cls, kinds in A.HINT_EDITS.items():
            self.assertTrue(set(kinds) <= known, cls)
        for cls, _pat in A.HINT_PATTERNS + A.SECONDARY_PATTERNS:
            self.assertIn(cls, A.HINT_EDITS)
        self.assertEqual(A.edit_kind("swap== L4"), "swap")
        self.assertEqual(A.edit_kind("mul>shift L5"), "shift")
        self.assertEqual(A.edit_kind("order[0:1,0]"), "order")


class RegdiffOrdering(unittest.TestCase):
    def test_without_hint_order_is_unchanged(self):
        vs = A.variants(FUNC)
        self.assertEqual(A.rank_variants(vs, None, FUNC), vs)
        self.assertEqual(A.rank_variants(vs, A.Hint(error="boom"), FUNC), vs)

    def test_kind_and_location_first_nothing_dropped(self):
        vs = A.variants(FUNC)
        hint = A.Hint(["ugen"], names=["k"])
        got = A.rank_variants(vs, hint, FUNC)
        self.assertEqual(sorted(v.label for v in got), sorted(v.label for v in vs))
        self.assertIn(A.edit_kind(got[0].label), A.HINT_EDITS["ugen"])
        self.assertIn("k", A.changed_lines(FUNC, got[0].text)[1])
        groups = []
        for v in got:
            kind = A.edit_kind(v.label) in hint.kinds
            touch = "k" in A.changed_lines(FUNC, v.text)[1]
            groups.append((0 if touch else 1) if kind else (2 if touch else 3))
        self.assertEqual(groups, sorted(groups))

    def test_line_hint(self):
        # line 5 of FUNC is `k = i * 4;`: an edit there comes first for a structure hint
        got = A.rank_variants(A.variants(FUNC), A.Hint(["structure"], lines=[5]), FUNC)
        touching = [v.label for v in got if 5 in A.changed_lines(FUNC, v.text)[0]]
        self.assertTrue(touching)
        self.assertIn(got[0].label, touching)

    def test_climb_scores_suggested_edit_first_and_asks_on_improvement(self):
        asked, scored, unguided = [], [], []

        def advise(text):
            asked.append(text)
            return A.Hint(["ugen"], names=["k"])

        def score(text):
            scored.append(text)
            return 0 if "s32 k;" in text else 1
        r = A.climb(FUNC, score, budget=60, advise=advise)
        self.assertTrue(r.matched)
        self.assertEqual(asked, [FUNC])
        self.assertIn("width k u16->s32", r.path)
        A.climb(FUNC, lambda t: unguided.append(t) or (0 if "s32 k;" in t else 1), budget=60)
        self.assertLess(len(scored), len(unguided))

    def test_climb_reports_hint_of_the_final_text(self):
        hints = {}

        def advise(text):
            hints[text] = A.Hint(["tie"], names=["i"])
            return hints[text]
        r = A.climb(FUNC, lambda t: 1 if "(i << 2)" in t else 2, budget=60, advise=advise, order=False)
        self.assertEqual(r.best, 1)
        self.assertIn(r.text, hints)
        self.assertEqual(r.hint.summary(), "tie i")

    def test_unguided_climb_is_unchanged_by_empty_hint(self):
        a, b = [], []
        A.climb(FUNC, lambda t: a.append(t) or 1, budget=60, order=False)
        A.climb(FUNC, lambda t: b.append(t) or 1, budget=60, order=False, advise=lambda t: A.Hint())
        self.assertEqual(a, b)


class FakeRegdiff:
    DEFAULT_UOPT = "/nonexistent/trace"

    def __init__(self):
        self.calls = []

    def run(self, args, out=print):
        with open(args.src) as fh:
            text = fh.read()
        self.calls.append((args, text))
        line = text[:text.index("i = D_80140000")].count("\n") + 1
        for l in TIE_REPORT.replace("(lines 14,15)", "(lines %d)" % line).split("\n"):
            out(l)
        return 1


class RegdiffAdvisor(Repo):
    def test_no_traced_uopt_means_no_advisor(self):
        with mock.patch.dict(os.environ, {"REGDIFF_UOPT": os.path.join(self.root, "missing")}):
            self.assertIsNone(A.regdiff_uopt())
            self.assertIsNone(A.make_advisor(self.root, A.find_drafts(self.root)[0], regdiff=FakeRegdiff()))

    def test_advisor_runs_regdiff_on_a_copy(self):
        d = A.find_drafts(self.root)[0]
        rd = FakeRegdiff()
        advise = A.make_advisor(self.root, d, uopt="/scratch/trace", regdiff=rd)
        variant = d.text.replace("i * 4", "(i << 2)")
        h = advise(variant)
        self.assertEqual(h.classes, ["tie"])
        self.assertEqual(h.lines[0], 2, "file line -> line index in the function text")
        args, text = rd.calls[0]
        self.assertEqual((args.func, args.unit, args.uopt), ("func_80132100", "AA", "/scratch/trace"))
        self.assertIn("(i << 2)", text)
        self.assertNotIn("NON_MATCHING", text)
        self.assertTrue(args.src.startswith(os.path.join(os.path.abspath(self.root), "build", "autotweak")))
        self.assertEqual(self.read(), SRC, "src/ is not touched")
        advise(variant)
        self.assertEqual(len(rd.calls), 1, "cached per text")

    def test_advisor_survives_regdiff_errors(self):
        class Broken(FakeRegdiff):
            def run(self, args, out=print):
                raise OSError("no objdump")
        advise = A.make_advisor(self.root, A.find_drafts(self.root)[0], uopt="/x", regdiff=Broken())
        h = advise("void func_80132100(void) {\n}")
        self.assertIn("no objdump", h.error)
        self.assertEqual([v.label for v in A.rank_variants(A.variants(FUNC), h, FUNC)],
                         [v.label for v in A.variants(FUNC)])


class Main(Repo):
    def run_main(self, argv, wanted):
        out = io.StringIO()
        scorer = FakeScorer(wanted)
        rc = A.main(argv + ["--root", self.root], make_scorer=lambda root, d: scorer,
                    out=lambda line: print(line, file=out))
        return rc, out.getvalue()

    def test_match_rewrites_source(self):
        rc, out = self.run_main(["AA:func_80132100", "--time", "60"], ["if (i) {}"])
        self.assertEqual(rc, 0)
        self.assertTrue(out.startswith("MATCH AA:func_80132100  1 -> 0"), out)
        self.assertIn("FAKE", out)
        text = self.read()
        self.assertNotIn("#ifdef NON_MATCHING", text)
        self.assertIn("if (i) {} /* FAKE: empty test splits the live range of i. T-11170 */", text)

    def test_dry_run_and_no_match(self):
        rc, out = self.run_main(["--all", "--dry-run", "--time", "60"], ["(i << 2)"])
        self.assertIn("(dry run)", out)
        self.assertEqual(self.read(), SRC)
        rc, out = self.run_main(["func_80132100", "--time", "1"], ["nothing like this"])
        self.assertEqual(out.split()[0], "AA:func_80132100")
        self.assertEqual(self.read(), SRC)

    def test_unverified_zero_is_reported(self):
        out = io.StringIO()
        scorer = FakeScorer([], verify_ok=False)
        A.main(["func_80132100", "--time", "1", "--root", self.root], make_scorer=lambda root, d: scorer,
               out=lambda line: print(line, file=out))
        self.assertIn("0 -> 0", out.getvalue())
        self.assertIn("(score 0 failed verification)", out.getvalue())
        self.assertNotIn("MATCH", out.getvalue())
        self.assertEqual(self.read(), SRC)

    def test_list(self):
        rc, out = self.run_main(["--list", "--all"], [])
        self.assertRegex(out, r"^AA:func_80132100  src/ovl/AA/80132000.c  \d+ edits\n$")

    def test_regdiff_hint_in_output_and_flag(self):
        made = []

        def fake_advisor(root, draft):
            made.append(draft.func)
            return lambda text: A.Hint(["tie"], names=["i"], lines=[2])
        with mock.patch.object(A, "make_advisor", fake_advisor):
            rc, out = self.run_main(["func_80132100", "--time", "1"], ["nothing like this"])
            self.assertIn("regdiff: tie i L2", out)
            self.assertEqual(made, ["func_80132100"])
            rc, out = self.run_main(["func_80132100", "--time", "1", "--no-regdiff"], ["nothing like this"])
            self.assertNotIn("regdiff:", out)
            self.assertEqual(made, ["func_80132100"])
        self.assertTrue(A.parse_args(["f", "--no-regdiff"]).no_regdiff)
        self.assertFalse(A.parse_args(["f"]).no_regdiff)

    def test_without_traced_uopt_output_is_unguided(self):
        with mock.patch.object(A, "regdiff_uopt", lambda: None):
            rc, out = self.run_main(["func_80132100", "--time", "1"], ["nothing like this"])
        self.assertNotIn("regdiff:", out)


if __name__ == "__main__":
    unittest.main()
