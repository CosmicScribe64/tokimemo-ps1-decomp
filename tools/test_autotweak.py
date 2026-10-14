"""Unit tests for tools/autotweak.py (synthetic C only, no game data; the scorer is mocked).

Run: python3 tools/test_autotweak.py   (from the repo root; no Docker needed)
"""
import io
import os
import sys
import tempfile
import unittest

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

    def test_time_budget(self):
        ticks = iter(range(1000))
        s = FakeScorer(["never there"])
        r = A.climb(FUNC, s.score, s.verify, budget=5, clock=lambda: next(ticks))
        self.assertLessEqual(r.tried, 5)

    def test_draft_that_does_not_compile(self):
        r = A.climb(FUNC, lambda t: None)
        self.assertEqual(r.error, "draft does not compile")


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

    def test_list(self):
        rc, out = self.run_main(["--list", "--all"], [])
        self.assertRegex(out, r"^AA:func_80132100  src/ovl/AA/80132000.c  \d+ edits\n$")


if __name__ == "__main__":
    unittest.main()
