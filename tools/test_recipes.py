"""Unit tests for tools/recipes.py (synthetic C only, no game data, no compiler; the scorer is mocked).

The bodies have the shape of the functions the recipes closed (TACO func_80140ADC, main func_80055A38,
main func_8006EB64, main func_800726F0) with invented names and values.

Run: python3 tools/test_recipes.py   (from the repo root; no Docker needed)
"""
import io
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import autotweak as A  # noqa: E402
import recipes as R  # noqa: E402

R.STRICT = True

TYPES = R.Types(
    names={"D_8015F000": ("u8", 0, []), "D_8015F001": ("u8", 0, []), "D_8015F002": ("s16", 0, []),
           "D_8015F004": ("s32", 0, []), "D_8015F010": ("u8", 0, ["[8]"]), "D_8015F020": ("s32", 1, [])},
    fields={"r0": ("u8", 0, []), "g0": ("u8", 0, []), "b0": ("u8", 0, []), "x0": ("s16", 0, []),
            "y0": ("s16", 0, []), "w": ("s32", 0, [])})


def setUpModule():
    R.set_types(TYPES)


def tearDownModule():
    R.set_types(None)


def fn(*lines, head="void func_80140000(void) {"):
    return head + "\n" + "\n".join(("    " + l if l else l) for l in lines) + "\n}"


def of(text, kind, prefix=""):
    """The variants of `text` of one kind whose label starts with `kind + prefix`."""
    return [v for v in A.variants(text, kinds={kind}) if v.label.startswith(kind + prefix)]


def one(text, kind, label):
    """The one variant of `kind` whose label is exactly `kind + label`."""
    got = [v for v in A.variants(text, kinds={kind}) if v.label == kind + label]
    assert len(got) == 1, [v.label for v in A.variants(text, kinds={kind})]
    return got[0]


def body(variant):
    """Lines of a variant's function body."""
    return variant.text.split("\n")[1:-1]


UV = ["poly->u0 = side << 5;", "poly->v0 = 0x60;", "poly->u1 = (side << 5) + 0x20;", "poly->v1 = 0x60;",
      "poly->u2 = side << 5;", "poly->v2 = 0x60 + 0x20;", "poly->u3 = (side << 5) + 0x20;",
      "poly->v3 = 0x60 + 0x20;"]


def uv_func(*extra):
    return fn("POLY_FT4 *poly;", "s16 side;", "", "poly = func_800490F0(0x28, D_8011ECA0);",
              "side = (rec->unk12 >> 4) & 1;", *UV, *extra, "poly->x0 = 5;")


# ---------------------------------------------------------------------------------------------
# gstate
# ---------------------------------------------------------------------------------------------

STATE_FUNC = fn(
    "s32 idx;", "s32 v;", "",
    "idx = D_800E6280.unk_110A;",
    "if (D_800E6280.unk_110A == 0xE) {",
    "    D_800E6280.unk_F5F = 3;",
    "}",
    "switch (idx) {",
    "case 0:",
    "    v = D_800E6280.unk_018[D_8011ECA0];",
    "    break;",
    "case 1:",
    "case 2:",
    "    v = D_800E6280.unk_01C + D_800E6280.unk_020;",
    "    break;",
    "default:",
    "    v = 0;",
    "    break;",
    "}",
    "func_80140100(&D_800E6280, v);",
    "D_8015F004 = v;")


class GState(unittest.TestCase):
    def test_all_uses_through_a_pointer_declared_first(self):
        v = one(STATE_FUNC, "gstate", " all")
        lines = body(v)
        self.assertEqual(lines[0], "    GameState *g = &D_800E6280;")
        self.assertEqual(lines[1:3], ["    s32 idx;", "    s32 v;"])
        self.assertNotIn("D_800E6280.", v.text)
        self.assertIn("    idx = g->unk_110A;", lines)
        self.assertIn("    if (g->unk_110A == 0xE) {", lines)
        self.assertIn("        g->unk_F5F = 3;", lines)
        self.assertIn("        v = g->unk_018[D_8011ECA0];", lines)
        self.assertIn("        v = g->unk_01C + g->unk_020;", lines)
        self.assertIn("    func_80140100(&D_800E6280, v);", lines, "an address of the aggregate stays")
        self.assertFalse(v.fake)
        self.assertNotIn("FAKE", v.text)

    def test_declared_last_among_the_locals(self):
        v = one(STATE_FUNC, "gstate", " all last")
        self.assertEqual(body(v)[:3], ["    s32 idx;", "    s32 v;", "    GameState *g = &D_800E6280;"])

    def test_first_statements_and_one_arm(self):
        labels = [v.label for v in of(STATE_FUNC, "gstate")]
        self.assertEqual(labels[:2], ["gstate all", "gstate all last"])
        first1 = one(STATE_FUNC, "gstate", " first1")
        self.assertEqual(first1.text.count("g->"), 1)
        self.assertIn("    idx = g->unk_110A;", first1.text)
        self.assertIn("if (D_800E6280.unk_110A == 0xE) {", first1.text)
        arm = [v for v in of(STATE_FUNC, "gstate") if v.label.startswith("gstate arm")]
        self.assertEqual(len(arm), 2, "the arms that read the aggregate: case 0 and cases 1/2")
        two = [v for v in arm if v.text.count("g->") == 2][0]
        self.assertIn("        v = g->unk_01C + g->unk_020;", two.text)
        self.assertIn("        v = D_800E6280.unk_018[D_8011ECA0];", two.text, "the other arms keep the direct read")

    def test_no_variant_when_g_exists_or_no_use(self):
        self.assertEqual(of(fn("s32 g;", "g = D_800E6280.unk_110A;"), "gstate"), [])
        self.assertEqual(of(fn("GameState *g = &D_800E6280;", "g->unk_110A = D_800E6280.unk_F5F;"), "gstate"), [])
        self.assertEqual(of(fn("s32 v;", "v = 1;"), "gstate"), [])
        self.assertEqual(of(fn("func_80140100(&D_800E6280);"), "gstate"), [])
        self.assertEqual(of(fn("s32 v;", "v = D_800E6280.unk_110A;", head="void f(s32 g) {"), "gstate"), [])
        # a member called g is not the name g
        self.assertTrue(of(fn("s32 v;", "v = D_800E6280.unk_110A + rec->g;"), "gstate"))

    def test_function_without_locals(self):
        v = of(fn("D_8015F004 = D_800E6280.unk_110A;"), "gstate")[0]
        self.assertEqual(body(v)[:2], ["    GameState *g = &D_800E6280;", "    D_8015F004 = g->unk_110A;"])

    def test_count_is_bounded(self):
        many = fn("s32 v;", *("v = D_800E6280.unk_%03X + %d;" % (k, k) for k in range(60)))
        self.assertLessEqual(len(of(many, "gstate")), R.MAX_VARIANTS["gstate"])

    def test_a_match_is_found_by_the_climb(self):
        want = "GameState *g = &D_800E6280;"
        r = A.climb(STATE_FUNC, lambda t: 0 if want in t and "D_800E6280.unk_110A" not in t else 1,
                    budget=60, order=False, kinds={"gstate"})
        self.assertTrue(r.matched)
        self.assertEqual(r.path, ["gstate all"])


# ---------------------------------------------------------------------------------------------
# group
# ---------------------------------------------------------------------------------------------

LOOP_FUNC = fn("s32 n;", "s32 b;", "u8 *p;", "u8 *q;", "s32 r;", "",
               "n = 0;", "b = 0;", "p = D_8011A000;", "r = D_8011A100;", "q = D_8011A000;",
               "for (; b < 2; b++) {", "    p[b] = 1;", "    q[b] = 2;", "    r += 4;", "}",
               "func_80140100(n, r);")


class Group(unittest.TestCase):
    def test_eight_stores_on_one_line(self):
        t = uv_func()
        v = one(t, "group", " join L6-13")
        lines = body(v)
        self.assertIn("    poly->u0 = side << 5; poly->v0 = 0x60; poly->u1 = (side << 5) + 0x20; poly->v1 = 0x60; "
                      "poly->u2 = side << 5; poly->v2 = 0x60 + 0x20; poly->u3 = (side << 5) + 0x20; "
                      "poly->v3 = 0x60 + 0x20;", lines)
        self.assertEqual(len(lines), len(t.split("\n")) - 2 - 7)

    def test_comma_form_is_one_expression_statement(self):
        t = uv_func()
        v = one(t, "group", " join, L6-13")
        self.assertIn("    poly->u0 = side << 5, poly->v0 = 0x60, poly->u1 = (side << 5) + 0x20, poly->v1 = 0x60, "
                      "poly->u2 = side << 5, poly->v2 = 0x60 + 0x20, poly->u3 = (side << 5) + 0x20, "
                      "poly->v3 = 0x60 + 0x20;", body(v))
        f = A.Func(v.text)       # still one function that the other edits can parse
        self.assertEqual(f.name, "func_80140000")

    def test_sizes_three_to_eight_only(self):
        t = uv_func()
        sizes = {int(x.label.rsplit("-", 1)[1]) - int(x.label.rsplit("-", 1)[0].rsplit("L", 1)[1]) + 1
                 for x in of(t, "group", " join") if "chunks" not in x.label}
        self.assertEqual(sizes, {3, 4, 5, 6, 7, 8})

    def test_dependent_statements_are_not_grouped(self):
        t = fn("s32 a;", "s32 b;", "s32 c;", "s32 d;", "a = D_8015F004 + 1;", "b = a + 1;", "c = b + 1;", "d = c + 1;")
        self.assertEqual(of(t, "group", " join"), [])
        t2 = fn("s32 a;", "s32 b;", "s32 c;", "a = 1;", "b = a + 1;", "c = 3;")
        self.assertEqual(of(t2, "group", " join"), [], "b reads a: no three independent statements")
        t3 = fn("D_8015F004 = 1;", "func_80140100(1);", "func_80140200(2);", "D_8015F000 = 2;")
        self.assertEqual(of(t3, "group", " join"), [], "two calls do not trade places")

    def test_a_call_moves_past_locals_only(self):
        t = fn("s32 a;", "s32 b;", "a = 1;", "b = 2;", "func_80140100(3);", "D_8015F004 = 4;")
        got = [v.label for v in of(t, "group", " join")]
        self.assertEqual(got, ["group join L3-5", "group join, L3-5"], "the memory store stays out of the window")

    def test_a_line_comment_stops_the_join(self):
        t = fn("D_8015F000 = 1; // one", "D_8015F001 = 2;", "D_8015F002 = 3;", "D_8015F004 = 4;")
        got = [v.label for v in of(t, "group", " join")]
        self.assertEqual(got, ["group join L2-4", "group join, L2-4"], "no window holds the // line but the last")
        # block comments are fine
        t = fn("D_8015F000 = 1; /* one */", "D_8015F001 = 2;", "D_8015F002 = 3;")
        v = one(t, "group", " join L1-3")
        self.assertIn("D_8015F000 = 1; /* one */ D_8015F001 = 2; D_8015F002 = 3;", v.text)

    def test_initial_assignments_join_the_loop_head(self):
        v = one(LOOP_FUNC, "group", " head L12 (5)")
        self.assertIn("    n = 0; b = 0; p = D_8011A000; r = D_8011A100; q = D_8011A000; for (; b < 2; b++) {",
                      body(v))
        self.assertIn("    q = D_8011A000; for (; b < 2; b++) {", body(one(LOOP_FUNC, "group", " head L12 (1)")))
        counts = sorted(int(x.label[-2]) for x in of(LOOP_FUNC, "group", " head L"))
        self.assertEqual(counts, [1, 2, 3, 4, 5])

    def test_do_and_while_heads(self):
        t = fn("s32 i;", "s32 n;", "i = 0;", "n = 0;", "do {", "    i++;", "} while (i < 4);",
               "while (n < 3) {", "    n++;", "}")
        self.assertIn("    i = 0; n = 0; do {", body(one(t, "group", " head L5 (2)")))
        t2 = fn("s32 i;", "i = 0;", "while (i < 4) {", "    i++;", "}")
        self.assertIn("    i = 0; while (i < 4) {", body(one(t2, "group", " head L3 (1)")))
        # the tail of a do-while is not a head
        self.assertEqual(of(fn("s32 i;", "do {", "    i++;", "} while (i < 4);"), "group", " head"), [])

    def test_reverse_splits(self):
        joined = fn("s32 a;", "s32 b;", "s32 c;", "s32 d;", "a = 1; b = 2; c = 3; d = 4;")
        got = {v.label: v for v in of(joined, "group", " split")}
        self.assertEqual(sorted(got), ["group split L5 @1", "group split L5 @2", "group split L5 @3"])
        self.assertEqual(body(got["group split L5 @1"])[-2:], ["    a = 1;", "    b = 2; c = 3; d = 4;"])
        self.assertEqual(body(got["group split L5 @2"])[-2:], ["    a = 1; b = 2;", "    c = 3; d = 4;"])
        comma = fn("poly->r0 = 1, poly->g0 = 2, poly->b0 = 3;")
        semi = one(comma, "group", " semi L1")
        self.assertEqual(body(semi), ["    poly->r0 = 1; poly->g0 = 2; poly->b0 = 3;"])
        split = one(comma, "group", " split L1")
        self.assertEqual(body(split), ["    poly->r0 = 1;", "    poly->g0 = 2;", "    poly->b0 = 3;"])
        head = of(LOOP_FUNC.replace("    n = 0;\n    b = 0;\n    p = D_8011A000;\n    r = D_8011A100;\n    q = D_8011A000;\n"
                                    "    for", "    n = 0; b = 0; p = D_8011A000; for"), "group", " unhead")
        self.assertEqual([v.label for v in head], ["group unhead L7", "group unhead1 L7"])
        self.assertIn("    n = 0;\n    b = 0;\n    p = D_8011A000;\n    for (; b < 2; b++) {", "\n".join(body(head[0])))
        self.assertIn("    n = 0;\n    b = 0; p = D_8011A000; for (; b < 2; b++) {", "\n".join(body(head[1])))

    def test_function_calls_keep_their_commas(self):
        self.assertEqual(of(fn("func_80140100(1, 2, 3);"), "group", " semi"), [])
        self.assertEqual(of(fn("D_8015F004 = c ? 1 : 2, D_8015F000 = 3;"), "group"), [])

    def test_count_is_bounded_on_a_long_run(self):
        stores = ["poly->f%02d = %d;" % (k, k) for k in range(40)]
        t = fn("POLY_FT4 *poly;", *stores)
        self.assertLessEqual(len(of(t, "group")), R.MAX_VARIANTS["group"])
        self.assertGreater(len(of(t, "group")), 10)

    def test_chunks_of_a_long_run(self):
        t = fn("POLY_G3 *poly;", "poly->r0 = a;", "poly->g0 = b;", "poly->b0 = c;", "poly->r1 = d;",
               "poly->g1 = e;", "poly->b1 = f;")
        v = one(t, "group", " chunks3 L2-7")
        self.assertEqual(body(v)[1:], ["    poly->r0 = a; poly->g0 = b; poly->b0 = c;",
                                       "    poly->r1 = d; poly->g1 = e; poly->b1 = f;"])
        v = one(t, "group", " chunks3, L2-7")
        self.assertEqual(body(v)[1:], ["    poly->r0 = a, poly->g0 = b, poly->b0 = c;",
                                       "    poly->r1 = d, poly->g1 = e, poly->b1 = f;"])

    def test_a_match_is_found_by_the_climb(self):
        want = "n = 0; b = 0; p = D_8011A000; r = D_8011A100; q = D_8011A000; for"
        r = A.climb(LOOP_FUNC, lambda t: 0 if want in t else 1, budget=60, order=False, kinds={"group"})
        self.assertTrue(r.matched)
        self.assertEqual(r.path, ["group head L12 (5)"])


# ---------------------------------------------------------------------------------------------
# chain
# ---------------------------------------------------------------------------------------------

class Chain(unittest.TestCase):
    RGB = fn("POLY_FT4 *poly;", "poly->r0 = 0x80;", "poly->g0 = 0x80;", "poly->b0 = 0x80;", "poly->x0 = 5;")

    def test_equal_stores_become_a_chain(self):
        v = one(self.RGB, "chain", " join L2-4")
        self.assertEqual(body(v)[1:], ["    poly->r0 = poly->g0 = poly->b0 = 0x80;", "    poly->x0 = 5;"])

    def test_target_orders(self):
        labels = [v.label for v in of(self.RGB, "chain", " join")]
        self.assertEqual(len(labels), 6 + 2 + 2, "all 6 orders of three, 2 each for the pairs")
        self.assertEqual(labels.count("chain join L2-4"), 1)
        order = one(self.RGB, "chain", " join L2-4 210")
        self.assertIn("    poly->b0 = poly->g0 = poly->r0 = 0x80;", order.text)
        four = fn("a->w = 1;", "b->w = 1;", "c->w = 1;", "d->w = 1;")
        self.assertEqual(len([v for v in of(four, "chain", " join L1-4")]), 4, "identity, reversed, two rotations")

    def test_value_may_be_a_read_with_distinct_roots(self):
        t = fn("D_8015F000 = D_8015F004 + 1;", "D_8015F001 = D_8015F004 + 1;", "D_8015F010[0] = D_8015F004 + 1;")
        self.assertTrue(of(t, "chain", " join L1-3"), "stores to A and B do not touch D_8015F004")
        self.assertIn("D_8015F000 = D_8015F001 = D_8015F010[0] = D_8015F004 + 1;", one(t, "chain", " join L1-3").text)

    def test_nothing_when_an_earlier_store_changes_the_value(self):
        t = fn("D_8015F004 = D_8015F004 + 1;", "D_8015F000 = D_8015F004 + 1;", "D_8015F001 = D_8015F004 + 1;")
        got = [v.label for v in of(t, "chain", " join")]
        self.assertNotIn("chain join L1-3", got)
        self.assertNotIn("chain join L1-2", got, "the store to D_8015F004 comes first")
        self.assertIn("chain join L2-3", got)

    def test_pointer_stores_alias_a_read_through_the_same_pointer(self):
        t = fn("p->a = p[0];", "p->b = p[0];")
        self.assertEqual(of(t, "chain"), [])
        t = fn("p->a = p->a;", "p->b = p->a;")
        self.assertEqual(of(t, "chain"), [], "the first store writes what the value reads")
        t = fn("p->a = p->c;", "p->b = p->c;")
        self.assertTrue(of(t, "chain", " join"), "another field of the same object")
        t = fn("poly->r0 = D_8015F004;", "poly->g0 = D_8015F004;", "poly->b0 = D_8015F004;")
        self.assertTrue(of(t, "chain", " join L1-3"), "names decide, as in autotweak.independent")
        t = fn("D_8015F010[i] = D_8015F010[j];", "D_8015F010[k] = D_8015F010[j];")
        self.assertEqual(of(t, "chain"), [], "the same array: an index is not told apart")

    def test_known_different_widths_are_left_alone(self):
        t = fn("D_8015F000 = 1;", "D_8015F002 = 1;")          # u8 and s16
        self.assertEqual(of(t, "chain"), [])
        t = fn("D_8015F000 = 1;", "D_8015F001 = 1;")          # u8 and u8
        self.assertTrue(of(t, "chain", " join"))
        t = fn("s16 i;", "s32 j;", "i = 0;", "j = 0;")
        self.assertEqual(of(t, "chain"), [])
        t = fn("rec->unk8 = 1;", "rec->unkA = 1;")            # fields the headers do not know
        self.assertTrue(of(t, "chain", " join"), "an unknown width does not block")
        t = fn("poly->r0 = 1;", "poly->x0 = 1;")              # u8 and s16 fields
        self.assertEqual(of(t, "chain"), [])

    def test_through_an_array_element_and_a_cast(self):
        t = fn("D_8015F010[0] = 1;", "D_8015F010[1] = 1;", "D_8015F010[2] = 1;")
        self.assertTrue(of(t, "chain", " join L1-3"))
        t = fn("*(u8 *)&D_8015F004 = 1;", "D_8015F001 = 1;")      # unknown shape, same width of u8
        self.assertEqual(R.TYPES.width(A.Func(t), A.tokenize("*(u8 *)&D_8015F004")), 1)

    def test_nothing_for_effects_or_different_values(self):
        for lines in (["a = f();", "b = f();"], ["a = i++;", "b = i++;"], ["a = 1;", "b = 2;"],
                      ["a = 1;", "x = y;", "b = 1;"], ["a += 1;", "b += 1;"], ["a = (b = 1);", "c = (b = 1);"]):
            self.assertEqual(of(fn(*lines), "chain"), [], lines)
        self.assertEqual(of(fn("a = 1;", "a = 1;"), "chain"), [], "the same target twice")
        self.assertEqual(of(fn("a[f()] = 1;", "b[g()] = 1;"), "chain"), [])

    def test_the_reverse_splits_a_chain(self):
        t = fn("D_8015F000 = D_8015F001 = 0x80;", "D_8015F004 = 1;")
        rtl = one(t, "chain", " split L1 rtl")
        ltr = one(t, "chain", " split L1 ltr")
        self.assertEqual(body(rtl), ["    D_8015F001 = 0x80;", "    D_8015F000 = 0x80;", "    D_8015F004 = 1;"])
        self.assertEqual(body(ltr), ["    D_8015F000 = 0x80;", "    D_8015F001 = 0x80;", "    D_8015F004 = 1;"])
        three = fn("s32 c;", "poly->r0 = poly->g0 = poly->b0 = c;")
        self.assertEqual(body(one(three, "chain", " split L2 rtl"))[1:],
                         ["    poly->b0 = c;", "    poly->g0 = c;", "    poly->r0 = c;"])
        # widths that differ: the chain stays
        self.assertEqual(of(fn("D_8015F000 = D_8015F002 = 1;"), "chain"), [])
        self.assertEqual(of(fn("a = b = f();"), "chain"), [])

    def test_a_match_is_found_by_the_climb(self):
        want = "poly->b0 = poly->g0 = poly->r0 = 0x80;"
        r = A.climb(self.RGB, lambda t: 0 if want in t else 1, budget=60, order=False, kinds={"chain"})
        self.assertTrue(r.matched)
        self.assertEqual(r.path, ["chain join L2-4 210"])

    def test_count_is_bounded(self):
        stores = ["D_8015F010[%d] = 1;" % k for k in range(30)]
        self.assertLessEqual(len(of(fn(*stores), "chain")), R.MAX_VARIANTS["chain"])


# ---------------------------------------------------------------------------------------------
# inline_multi
# ---------------------------------------------------------------------------------------------

TEST_FUNC = fn("s32 v;", "s32 t;", "",
               "v = D_800E6280.unk_F5F;",
               "if (v == 3) {",
               "    D_8015F000 = 1;",
               "}",
               "if (v != 0) {",
               "    t = 2;",
               "}",
               "return t;")


def whole(text, name=" v"):
    """The inline_multi variants that drop the local (not the ones that read the source at one use)."""
    return [v for v in of(text, "inline_multi", name) if " use" not in v.label]


class InlineMulti(unittest.TestCase):
    def test_a_local_tested_twice_becomes_direct_reads(self):
        v = one(TEST_FUNC, "inline_multi", " v L4")
        self.assertEqual(body(v), ["    s32 t;", "", "    if (D_800E6280.unk_F5F == 3) {", "        D_8015F000 = 1;", "    }",
                                   "    if (D_800E6280.unk_F5F != 0) {", "        t = 2;", "    }", "    return t;"])

    def test_one_use_reads_the_source_and_the_local_stays(self):
        v1 = one(TEST_FUNC, "inline_multi", " v L4 use1")
        self.assertEqual(body(v1)[:5], ["    s32 v;", "    s32 t;", "", "    v = D_800E6280.unk_F5F;",
                                        "    if (D_800E6280.unk_F5F == 3) {"])
        self.assertIn("    if (v != 0) {", body(v1))
        v2 = one(TEST_FUNC, "inline_multi", " v L4 use2")
        self.assertIn("    if (v == 3) {", body(v2))
        self.assertIn("    if (D_800E6280.unk_F5F != 0) {", body(v2))
        # the climb finds the form that closed func_800726F0: one test on the local, the other direct
        want = "if (v == 3)"
        r = A.climb(TEST_FUNC, lambda t: 0 if want in t and "if (D_800E6280.unk_F5F != 0)" in t else 1, budget=60,
                    order=False, kinds={"inline_multi"})
        self.assertTrue(r.matched)
        self.assertEqual(r.path, ["inline_multi v L4 use2"])

    def test_a_source_that_is_not_a_postfix_expression_is_parenthesised(self):
        t = fn("s32 v;", "v = D_8015F004 + 1;", "D_8015F001 = v;", "D_8015F002 = v * 2;")
        v = one(t, "inline_multi", " v L2")
        self.assertEqual(body(v)[-2:], ["    D_8015F001 = (D_8015F004 + 1);", "    D_8015F002 = (D_8015F004 + 1) * 2;"])

    def test_a_call_or_aliasing_store_between_blocks_it(self):
        for mid in ("func_80140100();", "D_800E6280.unk_F5F = 0;", "D_800E6280.unk_F5F += 1;", "D_800E6280.unk_F5F++;",
                    "D_8015F004 = D_800E6280.unk_F5F;", "p->x = 1;"):
            t = fn("s32 v;", "v = D_800E6280.unk_F5F;", "D_8015F001 = v;", mid, "D_8015F002 = v;")
            if mid.startswith(("D_8015F004", "p->")):
                self.assertTrue(whole(t), mid)     # a store to another name is fine
            else:
                self.assertEqual(whole(t), [], mid)
                self.assertEqual([v.label for v in of(t, "inline_multi", " v")], ["inline_multi v L2 use1"],
                                 "the use before the call can still read the source")
        t = fn("s32 v;", "v = D_800E6280.unk_F5F;", "D_8015F001 = v;", "D_800E6280.unk_F60 = 0;", "D_8015F002 = v;")
        self.assertTrue(whole(t), "another field of the same global")
        # the call in the line of the last use runs after the argument is read
        t = fn("s32 v;", "v = D_800E6280.unk_F5F;", "D_8015F001 = v;", "func_80140100(v);")
        self.assertTrue(whole(t))

    def test_a_store_to_a_name_the_source_reads_blocks_it(self):
        t = fn("s32 v;", "s32 i;", "v = D_8015F010[i];", "D_8015F001 = v;", "i = i + 1;", "D_8015F002 = v;")
        self.assertEqual(whole(t), [])

    def test_a_loop_around_a_use_counts_whole(self):
        t = fn("s32 v;", "s32 i;", "v = D_800E6280.unk_F5F;", "D_8015F001 = v;",
               "for (i = 0; i < 4; i++) {", "    D_8015F002 = v;", "    D_800E6280.unk_F5F = i;", "}")
        self.assertEqual(whole(t), [], "the store later in the loop changes the next read")
        self.assertEqual([v.label for v in of(t, "inline_multi", " v")], ["inline_multi v L3 use1"])
        t = fn("s32 v;", "s32 i;", "v = D_800E6280.unk_F5F;", "D_8015F001 = v;",
               "for (i = 0; i < 4; i++) {", "    D_8015F002 = v;", "    D_8015F000 = i;", "}")
        self.assertTrue(whole(t))

    def test_guards(self):
        one_use = fn("s32 v;", "v = D_800E6280.unk_F5F;", "D_8015F001 = v;")
        self.assertEqual(of(one_use, "inline_multi", " v"), [], "one use: the inline edit of t_temp")
        twice = fn("s32 v;", "v = D_800E6280.unk_F5F;", "D_8015F001 = v;", "v = 2;", "D_8015F002 = v;")
        self.assertEqual(of(twice, "inline_multi", " v"), [], "assigned twice")
        addr = fn("s32 v;", "v = D_800E6280.unk_F5F;", "func_80140100(&v);", "D_8015F001 = v;", "D_8015F002 = v;")
        self.assertEqual(of(addr, "inline_multi", " v"), [])
        local_src = fn("s32 v;", "s32 w;", "v = w;", "D_8015F001 = v;", "D_8015F002 = v;")
        self.assertEqual(of(local_src, "inline_multi", " v"), [], "a copy of a local is no memory read")
        call_src = fn("s32 v;", "v = func_80140100();", "D_8015F001 = v;", "D_8015F002 = v;")
        self.assertEqual(of(call_src, "inline_multi", " v"), [])
        branch = fn("s32 v;", "if (D_8015F000) {", "    v = D_800E6280.unk_F5F;", "}", "D_8015F001 = v;", "D_8015F002 = v;")
        self.assertEqual(of(branch, "inline_multi", " v"), [], "a use that the assignment does not dominate")
        init = fn("s32 v = D_800E6280.unk_F5F;", "D_8015F001 = v;", "D_8015F002 = v;")
        self.assertEqual(of(init, "inline_multi", " v"), [])
        goto = fn("s32 v;", "v = D_800E6280.unk_F5F;", "D_8015F001 = v;", "D_8015F002 = v;", "goto end;", "end:", ";")
        self.assertEqual(of(goto, "inline_multi"), [])

    def test_the_reverse_introduces_a_local_for_repeated_reads(self):
        t = fn("s32 t;", "",
               "if (D_800E6280.unk_110A == 1) {", "    t = 1;", "} else if (D_800E6280.unk_110A == 2) {", "    t = 2;",
               "}", "D_8015F004 = t;")
        v = one(t, "inline_multi", " intro D_800E6280.unk_110A L3")
        self.assertEqual(body(v), ["    s32 t;", "    s32 temp;", "", "    temp = D_800E6280.unk_110A;", "    if (temp == 1) {",
                                   "        t = 1;", "    } else if (temp == 2) {", "        t = 2;", "    }", "    D_8015F004 = t;"])

    def test_the_local_gets_the_exact_type_too(self):
        t = fn("D_8015F004 = D_8015F001 + 1;", "D_8015F002 = D_8015F001 + 2;")
        vs = of(t, "inline_multi", " intro")
        self.assertEqual(len(vs), 2)
        self.assertIn("s32 temp;", vs[0].text)
        self.assertIn("u8 temp;", vs[1].text)
        t = fn("D_8015F004 = *D_8015F020;", "D_8015F002 = *D_8015F020;")
        self.assertIn("s32 *temp;", of(t, "inline_multi", " intro D_8015F020")[-1].text)

    def test_the_reverse_stops_at_a_store_or_call(self):
        t = fn("D_8015F004 = D_8015F001 + 1;", "D_8015F001 = 5;", "D_8015F002 = D_8015F001 + 2;")
        self.assertEqual(of(t, "inline_multi", " intro"), [], "the second read comes after a store")
        t = fn("D_8015F004 = D_8015F001 + 1;", "func_80140100();", "D_8015F002 = D_8015F001 + 2;")
        self.assertEqual(of(t, "inline_multi", " intro"), [])
        t = fn("D_8015F004 = D_8015F001 + 1;", "D_8015F000 = 5;", "D_8015F002 = D_8015F001 + 2;")
        self.assertTrue(of(t, "inline_multi", " intro"))
        # a read that is only an address or a store target is not a read
        t = fn("p = &D_8015F001;", "q = &D_8015F001;")
        self.assertEqual(of(t, "inline_multi", " intro"), [])
        t = fn("D_8015F001 = 1;", "D_8015F001 = 2;")
        self.assertEqual(of(t, "inline_multi", " intro"), [])

    def test_the_reverse_stops_at_a_loop_that_changes_the_value(self):
        t = fn("for (; D_8015F001 < 3;) {", "    D_8015F002 = D_8015F001;", "    D_8015F001 = D_8015F001 + 1;", "}")
        self.assertEqual(of(t, "inline_multi", " intro"), [], "the head reads the value again after the store")
        t = fn("for (; D_8015F001 < 3;) {", "    D_8015F002 = D_8015F001;", "    D_8015F000 = D_8015F000 + 1;", "}")
        self.assertTrue(of(t, "inline_multi", " intro"))

    def test_the_reverse_needs_the_first_read_to_dominate_the_others(self):
        t = fn("if (c) {", "    D_8015F004 = D_8015F001 + 1;", "}", "D_8015F002 = D_8015F001 + 2;")
        self.assertEqual(of(t, "inline_multi", " intro"), [])

    def test_a_match_is_found_by_the_climb(self):
        want = "if (D_800E6280.unk_F5F == 3)"
        r = A.climb(TEST_FUNC, lambda t: 0 if want in t and "v = " not in t else 1, budget=60, order=False,
                    kinds={"inline_multi"})
        self.assertTrue(r.matched)
        self.assertEqual(r.path, ["inline_multi v L4"])

    def test_count_is_bounded(self):
        reads = ["D_8015F004 = D_800E6280.unk_%03X + D_800E6280.unk_%03X;" % (k, k) for k in range(30)]
        self.assertLessEqual(len(of(fn(*reads), "inline_multi")), R.MAX_VARIANTS["inline_multi"])


# ---------------------------------------------------------------------------------------------
# Types
# ---------------------------------------------------------------------------------------------

class TypeTables(unittest.TestCase):
    def test_field_types_from_headers(self):
        with tempfile.TemporaryDirectory() as root:
            os.makedirs(os.path.join(root, "include"))
            with open(os.path.join(root, "include", "a.h"), "w") as fh:
                fh.write("typedef struct P {\n    /* 0x00 */ u32 tag;\n    /* 0x04 */ u8 r0;\n"
                         "    /* 0x08 */ s16 x0, y0;\n    /* 0x0C */ u16 *list;\n    /* 0x10 */ s32 tbl[4];\n"
                         "    struct { s32 inner; } in;\n} P;\n"
                         "typedef struct Q {\n    s16 r0;\n    s32 only;\n} Q;\n"
                         "extern s32 D_8015F004;\n")
            got = R.field_types(root)
        self.assertEqual(got["tag"], ("u32", 0, ()))
        self.assertEqual(got["tbl"], ("s32", 0, ("[4]",)))
        self.assertEqual(got["list"], ("u16", 1, ()))
        self.assertEqual(got["only"], ("s32", 0, ()))
        self.assertNotIn("r0", got, "declared as u8 and s16: unknown")
        self.assertNotIn("D_8015F004", got)

    def test_widths(self):
        f = A.Func(fn("s16 h;", "u8 *p;", "s32 words[4];", "h = 1;"))

        def w(text):
            return R.TYPES.width(f, A.tokenize(text))
        self.assertEqual(w("h"), 2)
        self.assertEqual(w("p"), 4)
        self.assertEqual(w("*p"), 1)
        self.assertEqual(w("p[1]"), 1)
        self.assertEqual(w("words[2]"), 4)
        self.assertEqual(w("D_8015F002"), 2)
        self.assertEqual(w("D_8015F010[3]"), 1)
        self.assertEqual(w("rec->unknown"), None)
        self.assertEqual(w("poly->x0"), 2)
        self.assertEqual(w("(&D_8015F000)[1]"), 1)
        self.assertEqual(w("((s16 *)p)[0]"), 2)
        self.assertEqual(w("*(u16 *)&D_8015F004"), 2)


# ---------------------------------------------------------------------------------------------
# Registration, --only-kinds, --list
# ---------------------------------------------------------------------------------------------

SRC = """#include "common.h"

#ifdef NON_MATCHING
void func_80132100(void) {
    s32 v;

    v = D_800E6280.unk_110A;
    if (v == 1) {
        D_8015F001 = 1;
    }
    if (v == 2) {
        D_8015F001 = 2;
    }
    D_800E6280.unk_F5F = 1;
    D_800E6280.unk_F60 = 1;
}
#else
INCLUDE_ASM("asm/ovl/AA/nonmatchings/AA/80132000", func_80132100);
#endif
"""


class FakeScorer:
    def __init__(self, wanted):
        self.wanted = wanted

    def score(self, text):
        return sum(w not in text for w in self.wanted)

    def verify(self, text):
        return True


class Main(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = self.tmp.name
        path = os.path.join(self.root, "src", "ovl", "AA", "80132000.c")
        os.makedirs(os.path.dirname(path))
        with open(path, "w") as fh:
            fh.write(SRC)
        self.path = path

    def tearDown(self):
        self.tmp.cleanup()

    def run_main(self, argv, wanted=()):
        out = io.StringIO()
        scorer = FakeScorer(list(wanted))
        rc = A.main(argv + ["--root", self.root, "--no-regdiff"], make_scorer=lambda root, d: scorer,
                    out=lambda line: print(line, file=out))
        return rc, out.getvalue()

    def test_registered(self):
        for name in R.KINDS:
            self.assertIn(name, A.KINDS)
        for tr in R.TRANSFORMS:
            self.assertIn(tr, A.TRANSFORMS)
        self.assertEqual(sum(t in A.TRANSFORMS for t in R.TRANSFORMS), 4)
        R.register(A)                      # twice is harmless
        self.assertEqual(sum(t in A.TRANSFORMS for t in R.TRANSFORMS), 4)
        self.assertEqual(len(A.TRANSFORMS), len(set(A.TRANSFORMS)))

    def test_hint_edits(self):
        h = A.HINT_EDITS
        self.assertIn("group", h["tie"])
        self.assertTrue({"group", "chain"} <= set(h["ugen"]))
        self.assertTrue({"gstate", "inline_multi"} <= set(h["priority"]))
        self.assertIn("gstate", h["structure"])
        self.assertIn("gstate", h["argument"])
        for cls in ("tie", "ugen", "priority", "structure", "argument"):
            self.assertEqual(len(h[cls]), len(set(h[cls])), cls)
            self.assertEqual(h[cls][:3], A.HINT_EDITS[cls][:3])
        # the old kinds keep their rank: the new ones come last
        self.assertEqual(h["tie"][:7], ("order", "inline", "temp", "join", "split", "swap", "declswap"))

    def test_regdiff_hint_ranks_the_new_kinds(self):
        vs = A.variants(STATE_FUNC)
        base = STATE_FUNC
        kinds = [A.edit_kind(v.label) for v in A.rank_variants(vs, A.Hint(["priority"]), base)]
        first_gstate = kinds.index("gstate")
        self.assertLess(first_gstate, kinds.index("swap"), "priority puts gstate before the operand swaps")
        runs = fn("D_8015F000 = 1;", "D_8015F001 = 2;", "D_8015F004 = 3;", "D_8015F002 = 4;")
        kinds = [A.edit_kind(v.label) for v in A.rank_variants(A.variants(runs), A.Hint(["tie"]), runs)]
        self.assertLess(kinds.index("order"), kinds.index("group"), "tie: order first, group after")
        self.assertLess(kinds.index("join"), kinds.index("group"))

    def test_labels_map_to_kinds(self):
        for label, kind in (("gstate all", "gstate"), ("group join, L3-9", "group"), ("group head L4 (2)", "group"),
                            ("chain join L2-4 210", "chain"), ("inline_multi intro D_X L3", "inline_multi")):
            self.assertEqual(A.edit_kind(label), kind)
        text = TEST_FUNC
        produced = {A.edit_kind(v.label) for v in A.variants(text)}
        self.assertTrue(produced <= set(A.KINDS), produced - set(A.KINDS))

    def test_only_kinds_filters_the_variants(self):
        every = {A.edit_kind(v.label) for v in A.variants(STATE_FUNC)}
        self.assertTrue({"gstate", "swap", "temp"} <= every, every)
        only = A.variants(STATE_FUNC, kinds={"gstate"})
        self.assertTrue(only)
        self.assertEqual({A.edit_kind(v.label) for v in only}, {"gstate"})
        both = {A.edit_kind(v.label) for v in A.variants(STATE_FUNC, kinds={"gstate", "temp"})}
        self.assertEqual(both, {"gstate", "temp"})
        self.assertEqual(A.variants(STATE_FUNC, kinds={"chain"}), [])

    def test_only_kinds_argument(self):
        self.assertEqual(A.parse_args(["f", "--only-kinds", "gstate,group,chain,inline_multi"]).only_kinds,
                         {"gstate", "group", "chain", "inline_multi"})
        self.assertIsNone(A.parse_args(["f"]).only_kinds)
        self.assertEqual(A.parse_args(["f", "--only-kinds", "order,swap"]).only_kinds, {"order", "swap"})
        with self.assertRaises(SystemExit):
            with unittest.mock.patch("sys.stderr", io.StringIO()):
                A.parse_args(["f", "--only-kinds", "gstate,nonsense"])

    def test_list_prints_counts_per_kind(self):
        rc, out = self.run_main(["--list", "--all"])
        self.assertEqual(rc, 0)
        self.assertRegex(out, r"^AA:func_80132100  src/ovl/AA/80132000.c  \d+ edits  .*gstate \d+")
        self.assertRegex(out, r"gstate \d+(, [a-z_]+ \d+)*\n$")
        total = int(out.split("edits")[0].split()[-1])
        counts = [int(c) for c in __import__("re").findall(r" (\d+)(?:,|\n)", out)]
        self.assertEqual(sum(counts), total)
        rc, out = self.run_main(["--list", "--all", "--only-kinds", "gstate,inline_multi"])
        kinds = [w.rstrip(",") for w in out.split("edits")[1].split()[::2]]
        self.assertEqual(set(kinds), {"gstate", "inline_multi"})
        rc, out = self.run_main(["--list", "--all", "--only-kinds", "chain"])
        self.assertRegex(out, r"  2 edits  chain 2\n$")
        rc, out = self.run_main(["--list", "--all", "--only-kinds", "pad"])
        self.assertRegex(out, r"  2 edits  pad 2\n$")
        rc, out = self.run_main(["--list", "--all", "--only-kinds", "cast"])
        self.assertRegex(out, r"  \d+ edits")

    def test_search_uses_only_the_kinds_asked_for(self):
        # joining the two stores is a `join` edit; with only gstate in play it is not tried
        want = ["g->unk_110A", "GameState *g"]
        rc, out = self.run_main(["func_80132100", "--time", "60", "--dry-run", "--only-kinds", "gstate"], want)
        self.assertTrue(out.startswith("MATCH AA:func_80132100  2 -> 0  [gstate all]"), out)
        rc, out = self.run_main(["func_80132100", "--time", "5", "--dry-run", "--only-kinds", "chain,group"], want)
        self.assertNotIn("MATCH", out)

    def test_the_order_stage_needs_order_among_the_kinds(self):
        calls = []
        real = A.order_stage

        def spy(*a, **k):
            calls.append(1)
            return real(*a, **k)
        with unittest.mock.patch.object(A, "order_stage", spy):
            self.run_main(["func_80132100", "--time", "5", "--dry-run", "--only-kinds", "gstate"], ["no such text"])
            self.assertEqual(calls, [])
            self.run_main(["func_80132100", "--time", "5", "--dry-run", "--only-kinds", "order,gstate"], ["no such text"])
            self.assertTrue(calls)


import unittest.mock  # noqa: E402

if __name__ == "__main__":
    unittest.main()
