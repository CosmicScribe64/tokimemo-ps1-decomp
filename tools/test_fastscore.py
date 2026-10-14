"""Unit tests for tools/fastscore.py and the prefetch hooks of tools/autotweak.py (T-11250).

Synthetic C only; the preprocessor and the compile are faked. Run: python3 tools/test_fastscore.py
"""
import os
import sys
import threading
import time
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import autotweak as A  # noqa: E402
import fastscore as F  # noqa: E402
import permute  # noqa: E402
from test_autotweak import FUNC, FakeScorer  # noqa: E402

HEAD = """typedef int s32;
extern s32 D_80140000;
void func_80132100(s32 a, s32 b);
s32 other_one(s32 x) {
    return x + 1;
}
"""
TAIL = """
void other_two(void) {
    D_80140000 = 0;
}
static s32 other_three(s32 a, s32 b)
{
    return a * b;
}
"""
TARGET = "void func_80132000(void) {\n    s32 i;\n    i = D_80140000 + 1;\n    func_80132100(i, 4);\n}\n"


class FakePermute:
    """The permute functions the builder needs, with sh() = identity of the file content."""
    CPP = ["cpp"]

    def __init__(self):
        self.strips = 0

    strip_include_asm = staticmethod(permute.strip_include_asm)
    function_range = staticmethod(permute.function_range)
    signature_names = staticmethod(permute.signature_names)
    def_regex = staticmethod(permute.def_regex)

    def strip_functions(self, pre, func):
        self.strips += 1
        return permute.strip_functions(pre, func)

    @staticmethod
    def sh(cmd, cwd=None):
        with open(cmd[-1]) as fh:
            return fh.read()


class BaseBuilder(unittest.TestCase):
    def setUp(self):
        import tempfile
        self.tmp = tempfile.TemporaryDirectory()
        self.p = FakePermute()
        self.b = F.BaseBuilder(self.p, self.tmp.name, "func_80132000")

    def tearDown(self):
        self.tmp.cleanup()

    def legacy(self, text):
        return permute.strip_functions(text, "func_80132000")

    def test_equals_legacy_and_strips_once(self):
        for body in (TARGET, TARGET.replace("+ 1", "+ 2"), TARGET.replace("i = D_80140000 + 1", "i = 1 + D_80140000")):
            text = HEAD + body + TAIL
            self.assertEqual(self.b.build(text), self.legacy(text))
        self.assertEqual((self.b.misses, self.b.hits), (1, 2))
        self.assertEqual(self.p.strips, 1)

    def test_other_functions_are_stripped(self):
        out = self.b.build(HEAD + TARGET + TAIL)
        self.assertNotIn("return x + 1", out)
        self.assertNotIn("a * b", out)
        self.assertIn("i = D_80140000 + 1;", out)

    def test_declaration_change_outside_misses(self):
        a = HEAD + TARGET + TAIL
        b = a.replace("extern s32 D_80140000;", "extern s32 D_80140000, D_80140004;")
        self.assertEqual(self.b.build(a), self.legacy(a))
        self.assertEqual(self.b.build(b), self.legacy(b))
        self.assertEqual(self.b.misses, 2)
        self.assertEqual(self.b.build(a), self.legacy(a))      # both entries kept
        self.assertEqual(self.b.hits, 1)

    def test_cache_is_bounded(self):
        for n in range(F.BaseBuilder.CACHE_SIZE + 3):
            self.b.build(HEAD + "s32 g%d;\n" % n + TARGET + TAIL)
        self.assertEqual(len(self.b.cache), F.BaseBuilder.CACHE_SIZE)

    def test_definition_like_line_in_body_falls_back(self):
        # a body line that looks like the head of another definition: the legacy strip decides
        body = "void func_80132000(void) {\ns32 other_one(s32 x)\n{\n}\n}\n"
        text = HEAD + body + TAIL
        self.assertEqual(self.b.build(text), self.legacy(text))

    def test_missing_definition_exits(self):
        with self.assertRaises(SystemExit):
            self.b.build(HEAD + TAIL)

    def test_target_first_and_last(self):
        for text in (TARGET + TAIL, HEAD + TARGET):
            b = F.BaseBuilder(FakePermute(), self.tmp.name, "func_80132000")
            self.assertEqual(b.build(text), self.legacy(text))
            self.assertEqual(b.build(text.replace("+ 1", "+ 3")), self.legacy(text.replace("+ 1", "+ 3")))

    def test_threads_strip_once(self):
        out = []
        ts = [threading.Thread(target=lambda n=n: out.append(self.b.build(HEAD + TARGET.replace("4", str(n)) + TAIL)))
              for n in range(8)]
        for t in ts:
            t.start()
        for t in ts:
            t.join()
        self.assertEqual(len(out), 8)
        self.assertEqual(self.p.strips, 1)


class WorkersTest(unittest.TestCase):
    def test_results_and_errors(self):
        w = F.Workers(3)
        jobs = [w.submit(lambda x: x * 2, n) for n in range(10)]
        self.assertEqual([j.result() for j in jobs], [n * 2 for n in range(10)])

        def boom(_):
            raise ValueError("x")
        with self.assertRaises(ValueError):
            w.submit(boom, 0).result()
        w.close()

    def test_cancel_pending_keeps_running_jobs(self):
        w = F.Workers(1)
        gate = threading.Event()
        first = w.submit(lambda _: gate.wait(5) and "first", 0)
        rest = [w.submit(lambda x: x, n) for n in range(5)]
        time.sleep(0.05)
        self.assertEqual(w.cancel_pending(), 5)
        gate.set()
        self.assertEqual(first.result(), "first")
        self.assertEqual({j.state for j in rest}, {"cancelled"})
        w.close()

    def test_fifo_with_one_thread(self):
        w = F.Workers(1)
        order = []
        jobs = [w.submit(order.append, n) for n in range(6)]
        for j in jobs:
            j.result()
        self.assertEqual(order, list(range(6)))
        w.close()

    def test_default_jobs(self):
        self.assertEqual(F.default_jobs(10), 8)
        self.assertEqual(F.default_jobs(2), 1)
        self.assertEqual(F.default_jobs(1), 1)


class Prefetch(unittest.TestCase):
    """climb(prefetch=...) announces the texts it scores, in order, and changes nothing else."""

    def run_climb(self, score, **kw):
        asked, told = [], []

        def counting(text):
            asked.append(text)
            return score(text)
        r = A.climb(FUNC, counting, lambda t: True, budget=60, prefetch=told.extend if kw.pop("pf", True) else None, **kw)
        return r, asked, told

    def test_same_result_with_and_without(self):
        s = FakeScorer(["(i << 2)"])
        r0, asked0, _ = self.run_climb(s.score, pf=False)
        r1, asked1, told = self.run_climb(s.score)
        self.assertEqual((r0.best, r0.text, r0.path, r0.tried), (r1.best, r1.text, r1.path, r1.tried))
        self.assertTrue(set(asked1[1:]) <= set(told))   # [0] is the start text

    def test_pairs_are_announced_in_scoring_order(self):
        score = lambda text: 0 if ("(i << 2)" in text and "u16 k;\n    s32 i;" in text) else 1   # noqa: E731
        r0, asked0, _ = self.run_climb(score, pf=False)
        r1, asked1, told = self.run_climb(score)
        self.assertTrue(r1.matched)
        self.assertEqual(asked0, asked1)
        # the order the climb asks in is the order it was told (a subsequence of the announcements)
        it = iter(told)
        self.assertTrue(all(any(t == a for t in it) for a in asked1[1:]))

    def test_nothing_announced_without_hook(self):
        r, asked, told = self.run_climb(lambda t: 1, pf=False)
        self.assertEqual(told, [])


class ScorerCache(unittest.TestCase):
    """BuildScorer.score/prefetch bookkeeping with the compile replaced."""

    def make(self, jobs=2):
        sc = object.__new__(A.BuildScorer)
        sc._cache, sc._jobs, sc._builder, sc.exact, sc.jobs = {}, {}, object(), False, jobs
        sc._workers = F.Workers(jobs)
        sc.calls = []

        def fast(text):
            sc.calls.append(text)
            sc._cache[text] = len(text)
            return len(text)
        sc._score_fast = fast
        return sc

    def test_prefetched_then_cached(self):
        sc = self.make()
        sc.prefetch(["a", "bb", "ccc"])
        self.assertEqual([sc.score(t) for t in ("a", "bb", "ccc")], [1, 2, 3])
        self.assertEqual(sorted(sc.calls), ["a", "bb", "ccc"])
        self.assertEqual(sc.score("bb"), 2)
        self.assertEqual(len(sc.calls), 3)          # identical text: no second compile
        sc._workers.close()

    def test_cancelled_job_is_scored_directly(self):
        sc = self.make(1)
        gate = threading.Event()
        sc._score_fast_real = sc._score_fast
        sc._score_fast = lambda t: gate.wait(5) and sc._score_fast_real(t)
        sc.prefetch(["a", "bb", "ccc"])
        time.sleep(0.05)
        sc.prefetch(["ccc"])           # drops "bb" (still queued); "a" is running
        gate.set()
        self.assertEqual(sc.score("bb"), 2)
        self.assertEqual(sc.score("ccc"), 3)
        sc._workers.close()

    def test_exact_never_uses_the_cache(self):
        sc = self.make()
        sc.exact = True
        sc.score_exact = lambda t: -len(t)
        sc.prefetch(["a"])
        self.assertEqual(sc.score("a"), -1)
        sc._workers.close()


class Options(unittest.TestCase):
    def test_flags(self):
        a = A.parse_args(["f"])
        self.assertEqual((a.score_jobs, a.exact), (0, False))
        a = A.parse_args(["f", "--score-jobs", "4", "--exact"])
        self.assertEqual((a.score_jobs, a.exact), (4, True))

    def test_run_one_configures_the_scorer(self):
        seen = {}

        class S(FakeScorer):
            def configure(self, jobs=1, exact=False):
                seen["conf"] = (jobs, exact)

            def prefetch(self, texts):
                seen["prefetch"] = True

            def close(self):
                seen["closed"] = True
        d = A.Draft("src/main/a.c", "main", "func_80132000", FUNC)
        args = A.parse_args(["func_80132000", "--score-jobs", "3", "--no-regdiff", "--time", "5", "--dry-run"])
        A.run_one(".", d, args, make_scorer=lambda root, draft: S(["(i << 2)"]), out=lambda s: None)
        self.assertEqual(seen["conf"], (3, False))
        self.assertTrue(seen["prefetch"] and seen["closed"])
        seen.clear()
        args = A.parse_args(["func_80132000", "--exact", "--no-regdiff", "--time", "5", "--dry-run"])
        A.run_one(".", d, args, make_scorer=lambda root, draft: S(["(i << 2)"]), out=lambda s: None)
        self.assertEqual(seen["conf"], (1, True))
        self.assertNotIn("prefetch", seen)


if __name__ == "__main__":
    unittest.main()
