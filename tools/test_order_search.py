#!/usr/bin/env python3
"""Synthetic tests for tools/order_search.py (T-11530): marker parsing, rendering, enumeration and the search.
No compiler, no game data.

Run: tools/docker.sh python3 tools/test_order_search.py
"""
import unittest

import order_search as os_

SRC = """void f(void) {
    a = 1;
    /* ORDER */
    q->x = 0;
    q->y = 0; q->h = 4;

    q->w = 8;
    /* END ORDER */
    v = ORDER_SWAP(*(s16 *)s, +, g(1, 2) / 64);
}
"""


class Parse(unittest.TestCase):
    def test_blocks_and_swaps(self):
        t = os_.Template(SRC)
        self.assertEqual(len(t.blocks), 1)
        self.assertEqual([u.strip() for u in t.blocks[0]], ["q->x = 0;", "q->y = 0; q->h = 4;", "q->w = 8;"])
        self.assertEqual(t.swaps, [("*(s16 *)s", "+", "g(1, 2) / 64")])
        self.assertEqual(t.size(), 6 * 2)

    def test_identity_render_removes_markers_and_keeps_order(self):
        t = os_.Template(SRC)
        out = t.render(t.identity())
        self.assertNotIn("ORDER", out)
        self.assertIn("    q->x = 0;\n    q->y = 0; q->h = 4;\n    q->w = 8;\n", out)
        self.assertIn("v = *(s16 *)s + g(1, 2) / 64;", out)
        self.assertTrue(out.startswith("void f(void) {\n    a = 1;\n"))

    def test_render_permutation_and_flip(self):
        t = os_.Template(SRC)
        out = t.render((((2, 0, 1),), (1,)))
        self.assertIn("    q->w = 8;\n    q->x = 0;\n    q->y = 0; q->h = 4;\n", out)
        self.assertIn("v = g(1, 2) / 64 + *(s16 *)s;", out)
        self.assertEqual(t.describe((((2, 0, 1),), (1,))), "block 1 order 3,1,2; swap 1 flipped")
        self.assertEqual(t.describe(t.identity()), "as written")

    def test_errors(self):
        for bad in ("x;\n", "/* ORDER */\na;\nb;\n", "/* END ORDER */\n", "/* ORDER */\na;\n/* END ORDER */\n",
                    "v = ORDER_SWAP(a, +);\n", "v = ORDER_SWAP(a, +, ORDER_SWAP(b, +, c));\n",
                    "v = ORDER_SWAP(a, +, b;\n"):
            with self.assertRaises(os_.MarkerError, msg=bad):
                os_.Template(bad)

    def test_split_args_respects_nesting(self):
        self.assertEqual(os_.split_args("f(a, b), +, x[1, 2]"), ["f(a, b)", "+", "x[1, 2]"])


class Enumerate(unittest.TestCase):
    def test_all_states_count_and_unique(self):
        t = os_.Template(SRC)
        states = list(t.all_states())
        self.assertEqual(len(states), 12)
        self.assertEqual(len(set(states)), 12)

    def test_neighbours_are_one_move_away(self):
        t = os_.Template(SRC)
        neigh = list(t.neighbours(t.identity()))
        # 3 units: insertions give 4 distinct orders (the other moves repeat them), plus one swap flip
        self.assertEqual(len(set(neigh)), 5)
        self.assertNotIn(t.identity(), neigh)


class Search(unittest.TestCase):
    def target_scorer(self, target):
        calls = []

        def score(state):
            calls.append(state)
            perm, bits = state[0][0], state[1]
            return sum(a != b for a, b in zip(perm, target[0])) + sum(a != b for a, b in zip(bits, target[1]))
        return score, calls

    def test_exhaustive_finds_target_and_stops_at_zero(self):
        t = os_.Template(SRC)
        score, calls = self.target_scorer(((1, 2, 0), (1,)))
        res = os_.search(t, score, jobs=1, max_exhaustive=100, progress=lambda m: None)
        self.assertEqual(res[0], (0, (((1, 2, 0),), (1,))))
        self.assertEqual(len(calls), len(set(calls)))

    def test_hill_climb_reaches_target(self):
        lines = "".join("    s%d = %d;\n" % (i, i) for i in range(7))
        t = os_.Template("/* ORDER */\n" + lines + "/* END ORDER */\n")
        target = (3, 0, 1, 2, 4, 5, 6)

        def score(state):
            return sum(a != b for a, b in zip(state[0][0], target))
        res = os_.search(t, score, jobs=2, max_exhaustive=10, budget=500, progress=lambda m: None)
        self.assertEqual(res[0][0], 0)
        self.assertEqual(res[0][1][0][0], target)

    def test_hill_climb_respects_budget(self):
        lines = "".join("    s%d = %d;\n" % (i, i) for i in range(8))
        t = os_.Template("/* ORDER */\n" + lines + "/* END ORDER */\n")
        calls = []

        def score(state):
            calls.append(state)
            return 5
        os_.search(t, score, jobs=1, max_exhaustive=10, budget=20, progress=lambda m: None)
        self.assertLessEqual(len(calls), 20)


if __name__ == "__main__":
    unittest.main()
