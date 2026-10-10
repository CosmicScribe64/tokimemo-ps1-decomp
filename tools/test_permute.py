"""Unit tests for the setup logic of tools/permute.py (synthetic files only, no game data).

Run: tools/docker.sh python3 tools/test_permute.py   (from the repo root)
"""
import os
import shutil
import subprocess
import sys
import tempfile
import time
import unittest
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import funcloc  # noqa: E402
import permute  # noqa: E402


def touch(root, rel, text=""):
    path = os.path.join(root, rel)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w") as f:
        f.write(text)
    return path


def make_repo(root):
    """Synthetic splat configs: main + overlays A and B, which both load at 0x80132000."""
    touch(root, "config/overlays.txt", "A 0x80132000 0x100\nB 0x80132000 0x100\n")

    def yaml(subs, vram):
        return ("segments:\n  - {start: 0, vram: %d, type: code, subsegments: [%s]}\n"
                % (vram, ", ".join("[0, c, %s]" % x for x in subs)))
    touch(root, "config/SLPM_86.053.yaml", yaml(["main/80041000"], 0x80041000))
    touch(root, "config/overlays/A.yaml", yaml(["A"], 0x80132000))
    touch(root, "config/overlays/B.yaml", yaml(["B"], 0x80132000))


class FindSource(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = self.tmp.name
        make_repo(self.root)

    def test_finds_c_definition_not_include_asm_or_call(self):
        root = self.root
        touch(root, "src/main/80041000.c",
              'INCLUDE_ASM("asm/nonmatchings/main/80041000", func_1);\nvoid g(void) { func_1(); }\n')
        c = touch(root, "src/ovl/A.c", "#ifdef NON_MATCHING\nvoid func_1(s32 a) {\n}\n#endif\n")
        self.assertEqual(permute.find_source(root, "func_1"), c)
        self.assertIsNone(permute.find_source(root, "func_2"))

    def test_name_is_not_a_prefix_match(self):
        touch(self.root, "src/main/80041000.c", "void func_10(void) {\n}\n")
        self.assertIsNone(permute.find_source(self.root, "func_1"))

    def test_pointer_return(self):
        c = touch(self.root, "src/main/80041000.c", "u8 *func_1(u8 *a) {\n return a;\n}\n")
        self.assertEqual(permute.find_source(self.root, "func_1"), c)

    def test_colliding_overlay_names_need_a_unit(self):
        a = touch(self.root, "src/ovl/A.c", "void func_80132100(void) {\n}\n")
        b = touch(self.root, "src/ovl/B.c", "void func_80132100(s32 x) {\n}\n")
        with self.assertRaises(funcloc.AmbiguousError):
            permute.find_source(self.root, "func_80132100")
        self.assertEqual(permute.find_source(self.root, "func_80132100", "B"), b)
        self.assertEqual(permute.find_source(self.root, "func_80132100", "src/ovl/A.c"), a)

    def test_a_caller_in_another_overlay_is_not_the_source(self):
        touch(self.root, "src/ovl/A.c", "void g(void) {\n    func_80132100();\n}\n")
        b = touch(self.root, "src/ovl/B.c", "void func_80132100(void) {\n}\n")
        self.assertEqual(permute.find_source(self.root, "func_80132100"), b)

    def test_knr_definition(self):
        c = touch(self.root, "src/ovl/A.c", "void func_1(a, b)\ns32 a;\nu8 *b;\n{\n    D = a;\n}\n")
        self.assertEqual(permute.find_source(self.root, "func_1"), c)
        self.assertTrue(permute.def_regex("func_1").search("void func_1(a)\nregister u8 a;\n{\n}\n"))
        self.assertTrue(permute.def_regex("func_1").search("u8 *func_1()\n{\n}\n"))
        self.assertIsNone(permute.def_regex("func_1").search("void g(void) {\n    func_1(a)\n;\n}\n"))
        self.assertIsNone(permute.def_regex("func_1").search("void func_1(s32 a);\nint x;\n{\n"))


class FindAsm(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = self.tmp.name
        make_repo(self.root)

    def test_nonmatchings_overlay_and_missing(self):
        a = touch(self.root, "asm/ovl/A/nonmatchings/A/func_1.s")
        self.assertEqual(permute.find_asm(self.root, "func_1"), a)
        self.assertIsNone(permute.find_asm(self.root, "func_2"))

    def test_asm_of_the_units_own_source_not_another_overlays(self):
        a = touch(self.root, "asm/ovl/A/nonmatchings/A/func_80132100.s")
        b = touch(self.root, "asm/ovl/B/nonmatchings/B/func_80132100.s")
        src_b = touch(self.root, "src/ovl/B.c", "void func_80132100(void) {\n}\n")
        self.assertEqual(permute.find_asm(self.root, "func_80132100", src_b), b)
        self.assertEqual(permute.find_asm(self.root, "func_80132100", None, "A"), a)
        with self.assertRaises(SystemExit):
            permute.find_asm(self.root, "func_80132100")

    def test_setup_uses_source_and_asm_of_the_same_unit(self):
        touch(self.root, "src/ovl/A.c", "void func_80132100(void) {\n    a();\n}\n")
        touch(self.root, "src/ovl/B.c", "void func_80132100(void) {\n    b();\n}\n")
        touch(self.root, "asm/ovl/A/nonmatchings/A/func_80132100.s")
        b = touch(self.root, "asm/ovl/B/nonmatchings/B/func_80132100.s")
        seen = {}
        with mock.patch.object(permute, "build_base", side_effect=lambda r, src, f: seen.update(src=src) or "x"), \
                mock.patch.object(permute, "build_target", side_effect=lambda r, asm, o: seen.update(asm=asm)):
            permute.setup(self.root, "B:func_80132100", out=os.path.join(self.root, "out"))
        self.assertEqual(seen["asm"], b)
        self.assertTrue(seen["src"].endswith("B.c"))
        with self.assertRaises(SystemExit) as cm:
            permute.setup(self.root, "func_80132100", out=os.path.join(self.root, "out2"))
        self.assertIn("held by 2", str(cm.exception))


class Args(unittest.TestCase):
    def test_bare_name_and_explicit_all_take_the_function(self):
        for argv in (["func_1", "--time", "5"], ["all", "func_1", "--time", "5"]):
            a = permute.parse_args(argv)
            self.assertEqual((a.cmd, a.func, a.time), ("all", "func_1", 5), argv)

    def test_unit_option(self):
        a = permute.parse_args(["all", "func_1", "--unit", "TT"])
        self.assertEqual(a.unit, "TT")
        self.assertEqual(permute.parse_args(["setup", "func_1", "--unit", "TT"]).unit, "TT")

    def test_options_work_anywhere(self):
        """T-7030: --unit used to be accepted only after the subcommand."""
        for argv in (["--unit", "TT", "func_1", "--time", "5"],
                     ["--unit", "TT", "all", "func_1"],
                     ["all", "--unit", "TT", "func_1"],
                     ["all", "func_1", "--unit", "TT"],
                     ["func_1", "--unit", "TT"],
                     ["--time", "5", "--unit", "TT", "func_1"]):
            a = permute.parse_args(argv)
            self.assertEqual((a.cmd, a.func, a.unit), ("all", "func_1", "TT"), argv)
        a = permute.parse_args(["--unit", "TT", "setup", "func_1", "--out", "x"])
        self.assertEqual((a.cmd, a.unit, a.out), ("setup", "TT", "x"))
        a = permute.parse_args(["--time", "7", "run", "build/permute/x", "-j", "2"])
        self.assertEqual((a.cmd, a.time, a.j), ("run", 7, 2))

    def test_defaults_and_a_subcommand_option_never_reset_a_top_option(self):
        a = permute.parse_args(["func_1"])
        self.assertEqual((a.unit, a.time, a.no_verify, a.allow_decl_edits), (None, 60, False, False))
        a = permute.parse_args(["--unit", "TT", "all", "func_1", "--no-verify"])
        self.assertEqual((a.unit, a.no_verify), ("TT", True))

    def test_run_takes_a_directory(self):
        self.assertEqual(permute.parse_args(["run", "build/permute/x", "--time", "3"]).directory, "build/permute/x")


@unittest.skipUnless(os.path.isdir(permute.PERMUTER) and shutil.which("gcc"), "needs decomp-permuter and gcc (Docker)")
class KnrBase(unittest.TestCase):
    def test_build_base_accepts_a_knr_target(self):
        with tempfile.TemporaryDirectory() as root:
            src = touch(root, "a.c", "int D;\nvoid other(void) {\n    D = 2;\n}\n\n"
                                     "void func_1(a, b)\ns32 a;\ns32 b;\n{\n    D = a + b;\n}\n")
            base = permute.build_base(root, src, "func_1")
        self.assertIn("func_1", base)
        self.assertIn("D = a + b", base)
        self.assertNotIn("D = 2", base)   # the other function is stripped to a prototype


class Text(unittest.TestCase):
    def test_strip_include_asm(self):
        src = 'a;\nINCLUDE_ASM("asm/x", f1);\n  INCLUDE_ASM("asm/y", f2)\nb;\n'
        self.assertEqual(permute.strip_include_asm(src), "a;\nb;\n")

    def test_settings(self):
        self.assertEqual(permute.settings_toml("func_1", allow_decl_edits=True),
                         'func_name = "func_1"\ncompiler_type = "ido"\n')

    def test_settings_forbid_declaration_passes_by_default(self):
        t = permute.settings_toml("func_1")
        self.assertIn("[weight_overrides]", t)
        self.assertIn("perm_randomize_external_type = 0", t)
        self.assertIn("perm_randomize_function_type = 0", t)

    def test_compile_script_uses_project_pipeline(self):
        s = permute.compile_script("/repo")
        self.assertIn('cd "/repo"', s)
        self.assertIn('tools/cc.py', s)
        self.assertIn(" ido 5.3", s)

    def test_target_source_has_prelude_first(self):
        self.assertEqual(permute.target_source("glabel f\n"),
                         '.include "asmproc_prelude.inc"\nglabel f\n')


class Outputs(unittest.TestCase):
    def test_sorted_best_first_and_other_dirs_ignored(self):
        with tempfile.TemporaryDirectory() as d:
            for n in ("output-20-1", "output-0-2", "output-5-1", "base.c", "output-x"):
                os.makedirs(os.path.join(d, n))
            self.assertEqual([(s, n) for s, n, _ in permute.list_outputs(d)],
                             [(0, 2), (5, 1), (20, 1)])


class Running(unittest.TestCase):
    """T-7030: --time is a wall-clock limit for the whole process group, with progress lines."""

    def fake(self, body):
        d = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, d)
        path = os.path.join(d, "fake.py")
        with open(path, "w") as f:
            f.write(body)
        return d, [sys.executable, path]

    def test_a_permuter_that_ignores_sigint_and_leaves_a_worker_is_stopped_on_time(self):
        pidfile = tempfile.mktemp()
        d, cmd = self.fake(
            "import os, signal, subprocess, sys, time\n"
            "signal.signal(signal.SIGINT, signal.SIG_IGN)\n"
            "child = subprocess.Popen([sys.executable, '-c', 'import signal,time\\n"
            "signal.signal(signal.SIGINT, signal.SIG_IGN)\\ntime.sleep(600)'])\n"
            "open(%r, 'w').write(str(child.pid))\n"
            "time.sleep(600)\n" % pidfile)
        lines = []
        t0 = time.time()
        outs = permute.run(d, 2, 1, progress=lines.append, interval=1, cmd=cmd)
        took = time.time() - t0
        self.assertLess(took, 25)
        self.assertEqual(outs, [])
        self.assertTrue(any("time limit reached" in l for l in lines), lines)
        self.assertTrue(any("no output better than the base yet" in l for l in lines), lines)
        with open(pidfile) as f:
            pid = int(f.read())
        time.sleep(0.3)
        try:
            with open("/proc/%d/stat" % pid) as f:
                state = f.read().rsplit(")", 1)[1].split()[0]
        except OSError:
            state = "gone"
        self.assertIn(state, ("Z", "gone"))      # the worker died with the group (a zombie is dead)

    def test_progress_names_the_best_score_and_a_finished_permuter_returns_early(self):
        d, cmd = self.fake(
            "import os, sys, time\n"
            "d = %r\n"
            "os.makedirs(os.path.join(d, 'output-12-1'))\n"
            "time.sleep(2.5)\n" % "{DIR}")
        script = cmd[1]
        with open(script) as f:
            text = f.read().replace("{DIR}", d)
        with open(script, "w") as f:
            f.write(text)
        lines = []
        t0 = time.time()
        outs = permute.run(d, 30, 1, progress=lines.append, interval=1, cmd=cmd)
        self.assertLess(time.time() - t0, 10)
        self.assertEqual([(o[0], o[1]) for o in outs], [(12, 1)])
        self.assertTrue(any("best score 12" in l for l in lines), lines)


class Verification(unittest.TestCase):
    BASE = "extern s32 D_1;\nvoid func_1(void) {\n    D_1 = 2;\n}\n"

    def test_function_range_and_outside_edits(self):
        self.assertEqual(permute.function_range("a;\nvoid func_1(void) {\n  if (x) {\n  }\n}\nb;\n", "func_1")[0], 3)
        same = "extern s32 D_1;\nvoid func_1(void) {\n    D_1 = 3;\n}\n"
        self.assertEqual(permute.outside_edits(self.BASE, same, "func_1"), [])
        retyped = "extern u8 D_1;\nvoid func_1(void) {\n    D_1 = 2;\n}\n"
        self.assertEqual(permute.outside_edits(self.BASE, retyped, "func_1"), [("s32", "u8")])
        reformatted = "extern   s32\n D_1 ;\nvoid func_1(void)\n{ D_1 = 9 ; }\n"
        self.assertEqual(permute.outside_edits(self.BASE, reformatted, "func_1"), [])

    def test_replace_function_keeps_the_rest(self):
        text = '#include "a.h"\n\nvoid g(void) {\n}\n\nvoid func_1(void) {\n    old();\n}\n\nvoid h(void) {\n}\n'
        new = permute.replace_function(text, "func_1", "void func_1(void) {\n    new();\n}")
        self.assertIn("new();", new)
        self.assertNotIn("old();", new)
        self.assertIn("void g(void) {\n}", new)
        self.assertIn("void h(void) {\n}", new)
        self.assertIsNone(permute.replace_function(text, "func_9", "x"))

    def test_replace_function_replaces_the_non_matching_guard_as_a_whole(self):
        text = ('a;\n#ifdef NON_MATCHING\nvoid func_1(void) {\n    old();\n}\n#else\n'
                'INCLUDE_ASM("asm/x", func_1);\n#endif\nb;\n')
        new = permute.replace_function(text, "func_1", "void func_1(void) {\n    new();\n}\n")
        self.assertEqual(new, "a;\nvoid func_1(void) {\n    new();\n}\nb;\n")

    def test_replace_include_asm_line(self):
        text = 'a;\nINCLUDE_ASM("asm/x", func_1);\nb;\n'
        self.assertEqual(permute.replace_function(text, "func_1", "void func_1(void) {\n}"),
                         "a;\nvoid func_1(void) {\n}\nb;\n")

    def make(self, cand):
        root = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, root)
        src = touch(root, "src/ovl/TT/80132000.c", "void g(void) {\n}\n\nvoid func_1(void) {\n    old();\n}\n")
        d = os.path.join(root, "build/permute/func_1")
        touch(d, "base.c", self.BASE)
        touch(d, "output-0-1/source.c", cand)
        return root, src, os.path.join(d, "output-0-1")

    class Runner:
        def __init__(self, funcdiff_out, ninja_rc=0, seen=None):
            self.funcdiff_out, self.ninja_rc, self.seen = funcdiff_out, ninja_rc, seen if seen is not None else []

        def __call__(self, cmd, **kw):
            with open(os.path.join(kw["cwd"], "src/ovl/TT/80132000.c")) as f:
                self.seen.append((cmd[-1], f.read()))
            if cmd[0] == "ninja":
                return subprocess.CompletedProcess(cmd, self.ninja_rc, stdout="boom\n")
            return subprocess.CompletedProcess(cmd, 0 if "MATCH" in self.funcdiff_out and "DIFF" not in self.funcdiff_out else 1,
                                               stdout=self.funcdiff_out)

    def test_a_candidate_that_builds_and_matches_is_verified_and_the_file_is_restored(self):
        root, src, cand = self.make("extern s32 D_1;\nvoid func_1(void) {\n    D_1 = 2;\n}\n")
        with open(src) as f:
            before = f.read()
        seen = []
        ok, report = permute.verify_candidate(root, {"func": "func_1", "unit": "TT", "src": "src/ovl/TT/80132000.c"},
                                              cand, self.Runner("TT:func_1: MATCH (relocations resolved)\n", seen=seen))
        self.assertTrue(ok, report)
        self.assertIn("D_1 = 2;", seen[0][1])          # the build saw the candidate
        self.assertNotIn("old();", seen[0][1])
        with open(src) as f:
            self.assertEqual(f.read(), before)    # and the file is back
        self.assertEqual(seen[0][0], "TT:func_1")

    def test_funcdiff_difference_is_not_a_match_whatever_the_permuter_scored(self):
        root, src, cand = self.make("extern s32 D_1;\nvoid func_1(void) {\n    D_1 = 2;\n}\n")
        with open(src) as f:
            before = f.read()
        ok, report = permute.verify_candidate(
            root, {"func": "func_1", "unit": "TT", "src": "src/ovl/TT/80132000.c"}, cand,
            self.Runner("TT:func_1: DIFF (- expected, + built)\n  -lw v0\n  +lw v1\n"))
        self.assertFalse(ok)
        self.assertIn("DIFF", " ".join(report))
        with open(src) as f:
            self.assertEqual(f.read(), before)

    def test_unit_sha1_failure_is_not_a_match(self):
        root, src, cand = self.make("extern s32 D_1;\nvoid func_1(void) {\n    D_1 = 2;\n}\n")
        ok, report = permute.verify_candidate(
            root, {"func": "func_1", "unit": "TT", "src": "src/ovl/TT/80132000.c"}, cand,
            self.Runner("TT:func_1: MATCH\n", ninja_rc=1))
        self.assertFalse(ok)
        self.assertIn("FAILED", " ".join(report))

    def test_edited_extern_type_is_reported_as_the_cause(self):
        root, src, cand = self.make("extern u8 D_1;\nvoid func_1(void) {\n    D_1 = 2;\n}\n")
        ok, report = permute.verify_candidate(
            root, {"func": "func_1", "unit": "TT", "src": "src/ovl/TT/80132000.c"}, cand,
            self.Runner("TT:func_1: MATCH\n"))
        self.assertFalse(ok)
        self.assertIn("declarations outside the function were changed", report[0])
        self.assertIn("'s32' -> 'u8'", report[0])

    def test_one_line_statement_groups_are_reported_because_the_layout_matters(self):
        """main func_8004111C: the permuter's `do { a; b; c; d; } while (0);` line builds the original's
        store order; the same statements on lines of their own build the other one."""
        body = "void f(void) {\n  RECT rect;\n do { rect.x = 0; rect.y = 0; } while (0);\n  g(&rect);\n}\n"
        self.assertEqual(len(permute.layout_notes(body)), 1)
        self.assertEqual(permute.layout_notes("void f(void) {\n  int i;\n  for (i = 0; i < 3; i++) {\n    g(i);\n  }\n}\n"), [])
        root, src, cand = self.make("extern s32 D_1;\nvoid func_1(void) {\n do { D_1 = 2; D_1 = 3; } while (0);\n}\n")
        ok, report = permute.verify_candidate(
            root, {"func": "func_1", "unit": "TT", "src": "src/ovl/TT/80132000.c"}, cand,
            self.Runner("TT:func_1: MATCH\n"))
        self.assertTrue(ok, report)
        self.assertTrue(any(l.startswith("note: line") and "holds several statements" in l for l in report), report)

    def test_scratch_source_cannot_be_verified(self):
        ok, report = permute.verify_candidate("/", {"func": "f", "src": None}, "/x", None)
        self.assertFalse(ok)
        self.assertIn("scratch", report[0])


if __name__ == "__main__":
    unittest.main()
