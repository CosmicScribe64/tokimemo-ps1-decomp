"""Unit tests for tools/bulk_m2c.py (synthetic C only, no game data; m2c, compiler, scorer and
ninja are mocked).

Run: python3 tools/test_bulk_m2c.py   (from the repo root; no Docker needed)
"""
import json
import os
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import autotweak  # noqa: E402
import bulk_m2c as B  # noqa: E402

SRC = """#include "common.h"
#include "ovl/AA.h"

INCLUDE_ASM("asm/ovl/AA/nonmatchings/80132000", func_80132000);

INCLUDE_ASM("asm/ovl/AA/nonmatchings/80132000", func_80132100);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0001 someone else's draft */
void func_80132200(void) {
}
#else
INCLUDE_ASM("asm/ovl/AA/nonmatchings/80132000", func_80132200);
#endif
"""
HEADER = """#ifndef OVL_AA_H
#define OVL_AA_H
#include "game.h"
#ifdef SOMETHING
void unrelated(void);
#endif
void func_80132000(void);
#endif
"""
MAIN_API = "#ifndef MAIN_API_H\n#define MAIN_API_H\n#endif /* MAIN_API_H */\n"


def body(name, stmt):
    return "void %s(void) {\n    %s\n}\n" % (name, stmt)


class FakeScorer:
    def __init__(self, backend, draft, text):
        self.backend, self.draft, self.text = backend, draft, text

    def score(self, text):
        self.backend.scored.append(text)
        return self.backend.scores.get(self.draft.func, lambda t: 5)(text)

    def verify(self, text):
        return self.backend.verify_ok


class FakeBackend:
    """m2c output per function; the 'compiler' needs every name listed in `needs` declared in the
    C file above the guard; scores come from per-function callables."""

    def __init__(self, root, drafts, scores=None, needs=None, verify_ok=True, headers_ok=True):
        self.root, self.drafts, self.scores = root, drafts, scores or {}
        self.needs, self.verify_ok, self.headers_ok = needs or {}, verify_ok, headers_ok
        self.scored, self.seen_files, self.builds = [], [], []

    def m2c(self, root, unit, func):
        d = self.drafts[func]
        if isinstance(d, Exception):
            raise d
        return d

    def scorer(self, root, draft):
        text = B.read(os.path.join(root, draft.src))
        self.seen_files.append(text)
        assert "#ifdef NON_MATCHING\n" + B.NOTE in text, "the draft is guarded while it is compiled"
        for name in self.needs.get(draft.func, ()):
            if not any(B.decl_name(l) == name for l in text.splitlines() if l.startswith(("extern", "s32"))):
                raise RuntimeError("invalid base: base.c does not build\ncfe: Error: /w/build/base.c, line 7: "
                                   "'%s' undefined, reported only once for this function" % name)
        if self.scores.get(draft.func) == "broken":
            raise RuntimeError("cfe: Error: /w/base.c, line 3: syntax error")
        return FakeScorer(self, draft, text)

    def check_headers(self, root):
        return self.headers_ok, "" if self.headers_ok else "redeclaration of D_80140000"

    def build_unit(self, root, unit):
        self.builds.append(unit)
        return True, ""


class NoClimb:
    def __init__(self, result=None):
        self.calls, self.result = [], result

    def __call__(self, text, score, verify, budget=0):
        self.calls.append((text, budget))
        if self.result:
            return self.result(text, score, verify)
        return autotweak.Result(score(text), score(text), text, [], 0)


class Repo(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = os.path.join(self.tmp.name, "repo")
        self.out = os.path.join(self.tmp.name, "out")
        self.src = os.path.join(self.root, "src", "ovl", "AA", "80132000.c")
        os.makedirs(os.path.dirname(self.src))
        os.makedirs(os.path.join(self.root, "include", "ovl"))
        self.write(self.src, SRC)
        self.write(os.path.join(self.root, "include", "ovl", "AA.h"), HEADER)
        self.write(os.path.join(self.root, "include", "main_api.h"), MAIN_API)
        self.targets = [B.Target("AA", "src/ovl/AA/80132000.c", "func_80132000", 0x40),
                        B.Target("AA", "src/ovl/AA/80132000.c", "func_80132100", 0x20)]

    def tearDown(self):
        self.tmp.cleanup()

    def write(self, path, text):
        with open(path, "w") as fh:
            fh.write(text)

    def read(self, path=None):
        with open(path or self.src) as fh:
            return fh.read()

    def run_main(self, backend, extra=(), climb=None, targets=None):
        lines = []
        rc = B.main(["--root", self.root, "--out", self.out, "--exclude", ""] + list(extra), backend=backend,
                    targets=self.targets if targets is None else targets, climb=climb or NoClimb(),
                    emit=lines.append)
        return rc, lines


class Cleanup(unittest.TestCase):
    def test_unknown_types_comments_and_decls(self):
        decls = ("extern ? D_80140000;\n? func_80140100(s32);                /* extern */\n"
                 "static ? D_80140200;  /* unable to generate initializer */\n"
                 "static s32 D_80140300[2] = { 1, 2 };\nextern s32 D_80149999;\n"
                 "struct _m2c_stack_f {\n    s32 x;\n};\n")
        text = ("void f(? *arg0) {\n    ? var_v0; // m2c note\n    var_v0 = (? *) D_80140000;\n"
                "    var_v0 = arg0 ? 1 : (s32) \"a // b\";\n    func_80140100(D_80140200);\n}\n")
        out, known, problem = B.cleanup(decls, text)
        self.assertIsNone(problem)
        self.assertIn("void f(s32 *arg0) {", out)
        self.assertIn("s32 var_v0; /* m2c note */", out)
        self.assertIn("(s32 *) D_80140000", out)
        self.assertIn("arg0 ? 1 : (s32) \"a // b\"", out)
        self.assertEqual(known, {"D_80140000": "extern s32 D_80140000;", "func_80140100": "s32 func_80140100();",
                                 "D_80140200": "extern s32 D_80140200;"})

    def test_m2c_placeholder_is_refused(self):
        _b, _k, problem = B.cleanup("", "void f(void) {\n    M2C_ERROR(/* unknown instruction */);\n}\n")
        self.assertEqual(problem, "m2c-error: M2C_ERROR")

    def test_inline_temps(self):
        text = ("void f(void) {\n    s32 temp_v0;\n    s32 temp_v1;\n\n    temp_v0 = D_80140000 + 4;\n"
                "    if (temp_v0 != 0) {\n        temp_v1 = func_80140100();\n        D_80140000 = temp_v1;\n"
                "    }\n}\n")
        out = B.inline_temps(text)
        self.assertIn("if ((D_80140000 + 4) != 0) {", out)
        self.assertIn("    s32 temp_v1;\n\n    if (", out)
        self.assertNotIn("temp_v0", out)
        self.assertIn("temp_v1 = func_80140100();", out, "a call is not inlined")

    def test_inline_temps_needs_single_use_on_next_line(self):
        text = "void f(void) {\n    s32 temp_a0;\n\n    temp_a0 = D_X;\n    g(temp_a0);\n    h(temp_a0);\n}\n"
        self.assertEqual(B.inline_temps(text), text)

    def test_cheap_fix(self):
        known = {"D_80140000": "extern u8 D_80140000;"}
        msg = "cfe: Error: base.c, line 7: 'D_80140000' undefined\ncfe: Error: base.c, line 9: 'D_80140004' undefined"
        self.assertEqual(B.cheap_fix(msg, [], known), ["extern u8 D_80140000;", "extern s32 D_80140004;"])
        msg = "cfe: Error: base.c, line 2: redeclaration of 'D_80140000'; previous declaration at line 3"
        self.assertEqual(B.cheap_fix(msg, ["extern u8 D_80140000;"], known), [])
        self.assertIsNone(B.cheap_fix("cfe: Error: base.c, line 3: syntax error", [], known))
        self.assertEqual(B.first_error("x\ncfe: Error: /work/build/p/base.c, line 3: syntax error"),
                         "Error: base.c, line 3: syntax error")


class Placement(unittest.TestCase):
    def test_insert_and_drop(self):
        new = B.insert_draft(SRC, "func_80132100", body("func_80132100", "D = 1;"), ["extern s32 D;"])
        self.assertIn(B.DECL_START + "\nextern s32 D;\n" + B.DECL_END + "\n#ifdef NON_MATCHING\n" + B.NOTE, new)
        self.assertIn('#else\nINCLUDE_ASM("asm/ovl/AA/nonmatchings/80132000", func_80132100);\n#endif\n', new)
        self.assertEqual([g.func for g in autotweak.guards(new)], ["func_80132100", "func_80132200"])
        self.assertNotIn(B.DECL_START, B.drop_decl_block(new))
        self.assertIsNone(B.insert_draft(SRC, "func_80132200", "x", []), "already a draft")

    def test_include_asm_funcs_skip_guarded(self):
        self.assertEqual(B.include_asm_funcs(SRC), ["func_80132000", "func_80132100"])

    def test_header_home_and_insert(self):
        self.assertEqual(B.header_home("D_800E0000", "AA"), "include/main_api.h")
        self.assertEqual(B.header_home("func_80140000", "AA"), "include/ovl/AA.h")
        self.assertIsNone(B.header_home("func_80140000", "main"))
        self.assertIsNone(B.header_home("some_name", "AA"))
        new = B.insert_in_header(HEADER, ["extern s32 D_80140000;"])
        self.assertTrue(new.endswith("extern s32 D_80140000;\n\n#endif\n"), new)
        self.assertIn("#ifdef SOMETHING\nvoid unrelated(void);\n#endif\n", new)

    def test_select(self):
        ts = [B.Target("AA", "a.c", "f1", 0x200), B.Target("BB", "b.c", "f2", 0x10), B.Target("AA", "a.c", "f3", 0x20)]
        self.assertEqual([t.func for t in B.select(ts, 600)], ["f2", "f3", "f1"])
        self.assertEqual([t.func for t in B.select(ts, 0x200)], ["f2", "f3"])
        self.assertEqual([t.func for t in B.select(ts, 600, {"BB:b.c:f2"}, limit=1)], ["f3"])
        self.assertEqual([len(g) for g in B.group_by_file(ts)], [2, 1])


class Pipeline(Repo):
    def test_near_miss_restores_include_asm_and_saves_draft(self):
        be = FakeBackend(self.root, {"func_80132000": ("", body("func_80132000", "D_X = 1;")),
                                     "func_80132100": ("", body("func_80132100", "D_X = 2;"))},
                         scores={"func_80132000": lambda t: 50, "func_80132100": lambda t: 70})
        climb = NoClimb()
        rc, lines = self.run_main(be, climb=climb)
        self.assertEqual(rc, 0)
        self.assertEqual(self.read(), SRC)
        self.assertEqual(climb.calls, [], "scores at or above --keep-score are not climbed")
        self.assertIn("near AA:func_80132000 0x40 50", lines)
        near = self.read(os.path.join(self.out, "near.txt")).splitlines()
        self.assertEqual([l.split("\t")[:3] for l in near], [["50", "0x40", "AA:func_80132000"],
                                                              ["70", "0x20", "AA:func_80132100"]])
        draft = self.read(os.path.join(self.out, "drafts", "AA", "func_80132000.c"))
        self.assertIn("score 50", draft)
        self.assertIn("D_X = 1;", draft)
        self.assertFalse(os.listdir(os.path.join(self.out, "inflight")))

    def test_score_zero_match_writes_plain_c(self):
        be = FakeBackend(self.root, {"func_80132100": ("", body("func_80132100", "D_X = 2;"))},
                         scores={"func_80132100": lambda t: 0})
        rc, lines = self.run_main(be, targets=self.targets[1:])
        text = self.read()
        self.assertIn("void func_80132100(void) {\n    D_X = 2;\n}\n", text)
        self.assertNotIn("func_80132100);", text)
        self.assertNotIn(B.NOTE, text)
        self.assertIn("func_80132200);", text, "the other draft stays")
        self.assertIn("MATCH AA:func_80132100 0x20", lines)
        self.assertIn("MATCH 1 (0x20 bytes)  near 0  fail 0  (1 processed)", lines)

    def test_failed_verification_is_a_near_miss(self):
        be = FakeBackend(self.root, {"func_80132100": ("", body("func_80132100", "D_X = 2;"))},
                         scores={"func_80132100": lambda t: 0}, verify_ok=False)
        self.run_main(be, targets=self.targets[1:])
        self.assertEqual(self.read(), SRC)
        row = B.load_state(os.path.join(self.out, "state.jsonl"))["AA:src/ovl/AA/80132000.c:func_80132100"]
        self.assertEqual((row["status"], row["score"]), ("near", 0))

    def test_threshold_routes_to_climb(self):
        be = FakeBackend(self.root, {"func_80132000": ("", body("func_80132000", "D_X = 1;")),
                                     "func_80132100": ("", body("func_80132100", "D_X = 2;"))},
                         scores={"func_80132000": lambda t: 0 if "GOOD" in t else 12,
                                 "func_80132100": lambda t: 3})

        def solve(text, score, verify):
            if "func_80132000" in text:
                good = text.replace("D_X = 1;", "D_X = 1; /* GOOD */")
                return autotweak.Result(12, score(good), good, ["swap"], 3, matched=verify(good))
            return autotweak.Result(3, 2, text.replace("2;", "2 ;"), ["lines"], 4)
        climb = NoClimb(solve)
        rc, lines = self.run_main(be, ["--keep-score", "20", "--time", "7"], climb=climb)
        self.assertEqual(len(climb.calls), 2)
        self.assertEqual(climb.calls[0][1], 7)
        text = self.read()
        self.assertIn("D_X = 1; /* GOOD */", text)
        self.assertIn("func_80132100);", text)
        self.assertIn("near AA:func_80132100 0x20 2", lines)
        self.assertIn("2 ;", self.read(os.path.join(self.out, "drafts", "AA", "func_80132100.c")))

    def test_order_stage_flag(self):
        seen = []

        def climb(text, score, verify, budget=0, **kw):
            seen.append(kw)
            return autotweak.Result(score(text), score(text), text, [], 0)
        for extra, want in (([], {}), (["--no-order"], {"order": False})):
            be = FakeBackend(self.root, {"func_80132100": ("", body("func_80132100", "D_X = 2;"))},
                             scores={"func_80132100": lambda t: 3})
            seen.clear()
            self.run_main(be, extra, climb=climb, targets=self.targets[1:])
            self.assertEqual(seen, [want], extra)
            os.unlink(os.path.join(self.out, "state.jsonl"))
        self.assertTrue(B.parse_args(["--no-order"]).no_order)
        self.assertFalse(B.parse_args([]).no_order)

    def test_regdiff_advisor_passed_to_climb(self):
        seen, made = [], []

        def climb(text, score, verify, budget=0, **kw):
            seen.append(sorted(kw))
            return autotweak.Result(score(text), score(text), text, [], 0)

        def advisor(root, draft):
            made.append((draft.func, "#ifdef NON_MATCHING" in self.read()))
            return lambda text: autotweak.Hint(["tie"])
        for extra, want, uopt in (([], ["advise"], True), (["--no-regdiff"], [], True), ([], [], False)):
            be = FakeBackend(self.root, {"func_80132100": ("", body("func_80132100", "D_X = 2;"))},
                             scores={"func_80132100": lambda t: 3})
            seen.clear()
            with mock.patch.object(autotweak, "make_advisor", advisor if uopt else lambda root, draft: None):
                self.run_main(be, extra, climb=climb, targets=self.targets[1:])
            self.assertEqual(seen, [want], extra)
            os.unlink(os.path.join(self.out, "state.jsonl"))
        self.assertEqual(made, [("func_80132100", True)], "made once, with the guarded draft in the file")
        self.assertTrue(B.parse_args(["--no-regdiff"]).no_regdiff)

    def test_inlined_variant_is_kept_when_it_scores_better(self):
        src = "void func_80132100(void) {\n    s32 temp_v0;\n\n    temp_v0 = D_X + 1;\n    g(temp_v0);\n}\n"
        be = FakeBackend(self.root, {"func_80132100": ("", src)},
                         scores={"func_80132100": lambda t: 4 if "temp_v0" in t else 0})
        self.run_main(be, targets=self.targets[1:])
        self.assertIn("void func_80132100(void) {\n    g(D_X + 1);\n}\n", self.read())

    def test_compile_fix_and_declarations_move_to_headers(self):
        decls = "extern ? D_80140000;\nextern ? D_800E0000;\n"
        be = FakeBackend(self.root, {"func_80132100": (decls, body("func_80132100", "D_80140000 = D_800E0000 + D_80140004;"))},
                         scores={"func_80132100": lambda t: 0}, needs={"func_80132100": ["D_80140004"]})
        self.run_main(be, targets=self.targets[1:])
        self.assertIn(B.DECL_START + "\nextern s32 D_800E0000;\nextern s32 D_80140000;\n" + B.DECL_END, be.seen_files[0])
        self.assertIn("extern s32 D_80140004;", be.seen_files[1])
        text = self.read()
        self.assertNotIn(B.DECL_START, text)
        self.assertIn("D_80140000 = D_800E0000 + D_80140004;", text)
        ovl = self.read(os.path.join(self.root, "include", "ovl", "AA.h"))
        self.assertIn("extern s32 D_80140000;\nextern s32 D_80140004;\n", ovl)
        self.assertIn("extern s32 D_800E0000;", self.read(os.path.join(self.root, "include", "main_api.h")))
        self.assertEqual(be.builds, ["AA"])

    def test_header_check_failure_reverts_everything(self):
        be = FakeBackend(self.root, {"func_80132100": ("extern ? D_80140000;\n", body("func_80132100", "D_80140000 = 1;"))},
                         scores={"func_80132100": lambda t: 0}, headers_ok=False)
        rc, lines = self.run_main(be, targets=self.targets[1:])
        self.assertEqual(self.read(), SRC)
        self.assertEqual(self.read(os.path.join(self.root, "include", "ovl", "AA.h")), HEADER)
        self.assertTrue(any(l.startswith("fail AA:func_80132100 0x20 headers:") for l in lines), lines)
        self.assertTrue(any(l.strip().startswith("fail headers") for l in lines), lines)

    def test_compile_and_m2c_failures(self):
        be = FakeBackend(self.root, {"func_80132000": RuntimeError("m2c failed:\nTraceback\nDecompFailure: bad"),
                                     "func_80132100": ("", body("func_80132100", "x"))},
                         scores={"func_80132100": "broken"})
        rc, lines = self.run_main(be)
        self.assertEqual(self.read(), SRC)
        self.assertIn("fail AA:func_80132100 0x20 compile: Error: base.c, line 3: syntax error", lines)
        self.assertTrue(any(l.startswith("fail AA:func_80132000 0x40 m2c:") for l in lines), lines)
        self.assertIn("  fail compile      1", lines)
        self.assertIn("  fail m2c          1", lines)

    def test_resume_skips_processed_targets(self):
        be = FakeBackend(self.root, {"func_80132000": ("", body("func_80132000", "D_X = 1;")),
                                     "func_80132100": ("", body("func_80132100", "D_X = 2;"))})
        rc, lines = self.run_main(be, ["--limit", "1"])
        self.assertEqual(lines[0], "1 targets (0 already processed)")
        self.assertTrue(any("func_80132100" in l for l in lines[1:2]), "smallest first")
        rc, lines = self.run_main(be)
        self.assertEqual(lines[0], "1 targets (1 already processed)")
        rc, lines = self.run_main(be)
        self.assertEqual(lines[0], "0 targets (2 already processed)")
        self.assertEqual(len(B.load_state(os.path.join(self.out, "state.jsonl"))), 2)

    def test_recover_restores_a_killed_draft(self):
        os.makedirs(os.path.join(self.out, "inflight"))
        with open(os.path.join(self.out, "inflight", "AA__func_80132100.json"), "w") as fh:
            json.dump({"src": "src/ovl/AA/80132000.c", "text": SRC}, fh)
        self.write(self.src, B.insert_draft(SRC, "func_80132100", body("func_80132100", "x;"), []))
        lines = []
        self.assertEqual(B.recover(self.root, self.out, lines.append), 1)
        self.assertEqual(self.read(), SRC)
        self.assertFalse(os.listdir(os.path.join(self.out, "inflight")))

    def test_out_inside_repo_is_refused(self):
        lines = []
        rc = B.main(["--root", self.root, "--out", os.path.join(self.root, "build", "x")],
                    backend=FakeBackend(self.root, {}), targets=[], emit=lines.append)
        self.assertEqual(rc, 2)
        self.assertIn("outside the checkout", lines[0])

    def test_unit_and_exclude_filters(self):
        be = FakeBackend(self.root, {})
        rc, lines = self.run_main(be, ["--exclude", "AA"])
        self.assertEqual(lines[0], "0 targets (0 already processed)")
        rc, lines = self.run_main(be, ["--unit", "BB"])
        self.assertEqual(lines[0], "0 targets (0 already processed)")


if __name__ == "__main__":
    unittest.main()
