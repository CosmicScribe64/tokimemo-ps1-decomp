#!/usr/bin/env python3
"""Tests for the --proto override of tools/regdiff.py and tools/funcdiff.py (T-16530): parsing, the rewrite of a
preprocessed unit, the keep-it hint, the plumbing in both tools, and the clearer message for a missing traced uopt.
Synthetic text only; no compiler and no game data.

Run: tools/docker.sh python3 -m pytest -q tools/test_protoverride.py   (or python3 tools/test_protoverride.py)
"""
import argparse
import contextlib
import io
import os
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import funcdiff  # noqa: E402
import funcdiff_verify as proof  # noqa: E402
import protoverride as pv  # noqa: E402
import regdiff as rd  # noqa: E402

UNIT = """typedef int s32;
typedef struct { s32 a; s32 b; } Pair;
void func_8013E97C(s32 a, s32 b, s32 c);
s32 D_800E0000[2] = { 1, 2 };
extern s32 func_8013E7C0(s32 a);

void old_style(a, b)
s32 a;
s32 b;
{
    func_8013E97C(a, b, 0);
}

void caller(void) {
    char *s = "};{";
    char c = '{';
    func_8013E97C(1, 2, 3);
    func_8013E7C0(4);
}
"""


def squash(text):
    return " ".join(text.split())


class Parse(unittest.TestCase):
    def test_a_prototype_is_normalised_and_closed(self):
        self.assertEqual(pv.parse(["void   func_8013E97C(s32 a,\n s32 b)"]),
                         [pv.Proto("func_8013E97C", "void func_8013E97C(s32 a, s32 b);")])

    def test_implicit_int_and_k_and_r_forms(self):
        self.assertEqual(pv.parse(["func_1();"])[0].name, "func_1")
        self.assertEqual(pv.parse(["s32 func_2();"])[0].decl, "s32 func_2();")
        self.assertEqual(pv.parse(["extern const char *func_3(void);"])[0].name, "func_3")

    def test_several_declarations_in_one_argument_and_repeated_options(self):
        got = pv.parse(["void f(void); s32 g();", "u8 h(u8 x);"])
        self.assertEqual([p.name for p in got], ["f", "g", "h"])

    def test_only_function_declarations_are_accepted(self):
        for bad in ("", "  ;  ", "extern s32 D_800E7384;", "s32 f(void) { return 1; }", "typedef void F(void);",
                    "void (*f)(void);", "s32 a, f(void);", "s32 f(void) = 0;", "f"):
            with self.assertRaises(pv.ProtoError, msg=bad):
                pv.parse([bad])

    def test_a_function_declared_twice_is_refused(self):
        with self.assertRaisesRegex(pv.ProtoError, "declared twice"):
            pv.parse(["void f(void);", "s32 f();"])

    def test_no_option_is_no_override(self):
        self.assertEqual(pv.parse(None), [])
        self.assertEqual(pv.parse([]), [])


class Apply(unittest.TestCase):
    def test_the_declaration_is_replaced_in_place(self):
        text, notes = pv.apply(UNIT, pv.parse(["s32 func_8013E97C();"]))
        self.assertIn("s32 func_8013E97C();\ns32 D_800E0000[2]", text)
        self.assertNotIn("void func_8013E97C(s32 a, s32 b, s32 c);", text)
        self.assertEqual(text.count("func_8013E97C();"), 1)
        self.assertEqual(len(notes), 1)
        self.assertIn("replaces void func_8013E97C(s32 a, s32 b, s32 c);", notes[0])
        # everything else is byte for byte what it was
        self.assertEqual(text.replace("s32 func_8013E97C();\n", "void func_8013E97C(s32 a, s32 b, s32 c);\n"), UNIT)

    def test_two_functions_at_once(self):
        text, notes = pv.apply(UNIT, pv.parse(["void func_8013E97C();", "void func_8013E7C0(s32 a, s32 b);"]))
        self.assertIn("void func_8013E97C();", text)
        self.assertIn("void func_8013E7C0(s32 a, s32 b);", text)
        self.assertNotIn("extern s32 func_8013E7C0", text)
        self.assertEqual(len(notes), 2)

    def test_every_declaration_of_the_function_goes(self):
        text = "void f(s32 a);\nstatic int x;\nint f(s32 a);\nvoid g(void) { f(1); }\n"
        out, notes = pv.apply(text, pv.parse(["s32 f();"]))
        self.assertEqual(out.count("f("), 2)           # the declaration and the call
        self.assertTrue(out.startswith("s32 f();\nstatic int x;"))
        self.assertIn("replaces void f(s32 a); int f(s32 a);", notes[0])

    def test_without_a_declaration_it_goes_before_the_first_function_that_calls(self):
        text = "typedef int s32;\nvoid a(void) { }\nvoid b(void) { f(1); }\nvoid c(void) { f(2); }\n"
        out, notes = pv.apply(text, pv.parse(["s32 f(s32 x);"]))
        self.assertEqual(out, "typedef int s32;\nvoid a(void) { }\ns32 f(s32 x);\nvoid b(void) { f(1); }\n"
                              "void c(void) { f(2); }\n")
        self.assertIn("no declaration in the unit", notes[0])
        self.assertIn("line 3", notes[0])

    def test_the_declaration_never_lands_inside_a_k_and_r_definition(self):
        text = "typedef int s32;\nvoid g(a, b)\ns32 a;\ns32 b;\n{\n    f(a, b);\n}\n"
        out, _ = pv.apply(text, pv.parse(["void f();"]))
        self.assertEqual(out, "typedef int s32;\nvoid f();\nvoid g(a, b)\ns32 a;\ns32 b;\n{\n    f(a, b);\n}\n")

    def test_a_use_before_the_declaration_moves_the_declaration_up(self):
        text = "void g(void) { f(1); }\nvoid f(s32 a);\n"
        out, _ = pv.apply(text, pv.parse(["s32 f();"]))
        self.assertEqual(out, "s32 f();\nvoid g(void) { f(1); }\n\n")

    def test_literals_with_braces_and_semicolons_do_not_confuse_the_scan(self):
        out, _ = pv.apply(UNIT, pv.parse(["s32 func_8013E7C0();"]))
        self.assertIn('char *s = "};{";', out)
        self.assertIn("char c = '{';", out)
        self.assertIn("s32 func_8013E7C0();", out)
        self.assertEqual(out.count("func_8013E7C0"), 2)

    def test_a_longer_name_is_not_the_function(self):
        text = "void func_10(void);\nvoid func_1(void);\nvoid g(void) { func_10(); func_1(); }\n"
        out, _ = pv.apply(text, pv.parse(["s32 func_1();"]))
        self.assertIn("void func_10(void);", out)
        self.assertNotIn("void func_1(void);", out)

    def test_pragma_lines_belong_to_no_statement(self):
        text = "#pragma foo\nvoid f(void);\n#pragma bar\nvoid g(void) { f(); }\n"
        out, _ = pv.apply(text, pv.parse(["s32 f();"]))
        self.assertEqual(out, "#pragma foo\ns32 f();\n#pragma bar\nvoid g(void) { f(); }\n")

    def test_a_function_the_file_defines_is_refused(self):
        for name in ("caller", "old_style"):
            with self.assertRaisesRegex(pv.ProtoError, "is defined in this file"):
                pv.apply(UNIT, pv.parse(["void %s();" % name]))

    def test_a_name_that_is_not_in_the_unit_is_refused(self):
        with self.assertRaisesRegex(pv.ProtoError, "does not occur"):
            pv.apply(UNIT, pv.parse(["void func_8013E97D();"]))

    def test_a_shared_declaration_statement_is_refused(self):
        text = "void f(void), g(void);\nvoid h(void) { f(); }\n"
        with self.assertRaisesRegex(pv.ProtoError, "shares a statement"):
            pv.apply(text, pv.parse(["s32 f();"]))
        text = "s32 x, f(void);\nvoid h(void) { f(); }\n"
        with self.assertRaisesRegex(pv.ProtoError, "shares a statement"):
            pv.apply(text, pv.parse(["s32 f();"]))

    def test_data_with_the_function_name_in_an_initializer_is_left_alone(self):
        text = "void f(void);\nvoid (*tbl[1])(void) = { f };\n"
        out, _ = pv.apply(text, pv.parse(["s32 f();"]))
        self.assertEqual(out, "s32 f();\nvoid (*tbl[1])(void) = { f };\n")

    def test_the_same_declaration_changes_nothing_but_whitespace(self):
        out, _ = pv.apply(UNIT, pv.parse(["void func_8013E97C(s32 a, s32 b, s32 c);"]))
        self.assertEqual(out, UNIT)


class KeepIt(unittest.TestCase):
    def test_guards_are_found_in_the_headers(self):
        with tempfile.TemporaryDirectory() as tmp:
            (Path(tmp) / "ovl").mkdir()
            (Path(tmp) / "ovl" / "RPG_BAT.h").write_text(
                "#ifndef RPG_BAT_OVERRIDE_func_8013E97C\nvoid func_8013E97C(s32 a);\n#endif\n"
                "#ifndef RPG_BAT_OVERRIDE_func_8013E97CX\nvoid func_8013E97CX(void);\n#endif\n")
            (Path(tmp) / "main_api.h").write_text("#ifndef MAIN_API_OVERRIDE_func_80053418\n#endif\n")
            self.assertEqual(pv.override_macros("func_8013E97C", tmp), ["RPG_BAT_OVERRIDE_func_8013E97C"])
            lines = pv.commit_hint(pv.parse(["s32 func_8013E97C();", "void func_80053418();", "void nothing();"]), tmp)
        self.assertIn("#define RPG_BAT_OVERRIDE_func_8013E97C", lines[0])
        self.assertIn("'s32 func_8013E97C();'", lines[0])
        self.assertIn("MAIN_API_OVERRIDE_func_80053418", lines[1])
        self.assertIn("no *_OVERRIDE_nothing guard", lines[2])


class FindUopt(unittest.TestCase):
    def find(self, explicit=None, env=None, present=(), scratch_mounted=True, executable=True):
        return rd.find_uopt(explicit, env or {}, exists=lambda p: p in present, isdir=lambda p: scratch_mounted,
                            access=lambda p, mode: executable)

    def test_the_default_location_is_used_when_it_exists(self):
        path, problem = self.find(present={rd.DEFAULT_UOPT})
        self.assertEqual((path, problem), (rd.DEFAULT_UOPT, None))

    def test_an_explicit_path_wins_and_is_never_replaced_by_the_default(self):
        path, problem = self.find("/x/uopt", {"REGDIFF_UOPT": "/y/uopt"}, present={"/x/uopt", rd.DEFAULT_UOPT})
        self.assertEqual(path, "/x/uopt")
        path, problem = self.find("/x/uopt", present={rd.DEFAULT_UOPT})
        self.assertIsNone(path)
        self.assertIn("/x/uopt  [--uopt]", problem)
        self.assertNotIn(rd.DEFAULT_UOPT, problem)

    def test_the_environment_variable_is_second(self):
        path, _ = self.find(env={"REGDIFF_UOPT": "/y/uopt"}, present={"/y/uopt"})
        self.assertEqual(path, "/y/uopt")
        _, problem = self.find(env={"REGDIFF_UOPT": "/y/uopt"})
        self.assertIn("/y/uopt  [$REGDIFF_UOPT]", problem)

    def test_a_container_with_tokimemo_scratch_set_also_looks_there(self):
        wanted = "/data/scratch/" + rd.UOPT_REL
        path, _ = self.find(env={"TOKIMEMO_SCRATCH": "/data/scratch"}, present={wanted})
        self.assertEqual(path, wanted)

    def test_missing_binary_says_where_it_looked_and_how_to_provide_it(self):
        path, problem = self.find()
        self.assertIsNone(path)
        self.assertIn("looked at: " + rd.DEFAULT_UOPT, problem)
        self.assertIn("r9-uopt/variants/trace", problem)
        self.assertIn("wiki/uopt-register-allocation.md", problem)
        self.assertIn("--uopt PATH", problem)
        self.assertIn("REGDIFF_UOPT", problem)
        self.assertNotIn("not mounted", problem)

    def test_an_unmounted_scratch_directory_is_named(self):
        _, problem = self.find(scratch_mounted=False)
        self.assertIn("/scratch is not mounted", problem)
        self.assertIn("TOKIMEMO_SCRATCH=<dir> tools/docker.sh", problem)

    def test_a_file_that_cannot_run_is_not_taken_for_the_binary(self):
        path, problem = self.find(present={rd.DEFAULT_UOPT}, executable=False)
        self.assertIsNone(path)
        self.assertIn("exists but is not executable", problem)


class RegdiffPlumbing(unittest.TestCase):
    def args(self, **kw):
        base = dict(func="func_1", unit=None, src="f.c", uopt="/u/uopt", scratch="/tmp/x", proto=[])
        base.update(kw)
        return SimpleNamespace(**base)

    def run_regdiff(self, args, **patches):
        messages = []
        c = SimpleNamespace(src=Path("f.c"), unit="TT")
        with mock.patch.object(rd, "find_uopt", return_value=patches.get("uopt", ("/u/uopt", None))), \
                mock.patch("funcloc.locate", return_value=c), \
                mock.patch("funcdiff.expected_object", return_value=("expected.o", None)), \
                mock.patch.object(rd, "compile_draft", side_effect=ValueError("stop here")) as compile_draft:
            rc = rd.run(args, out=messages.append)
        return rc, messages, compile_draft

    def test_the_parsed_prototypes_reach_the_compile(self):
        rc, messages, compile_draft = self.run_regdiff(self.args(proto=["s32 func_2();", "void func_3(s32 a);"]))
        self.assertEqual(rc, 2)
        protos = compile_draft.call_args.kwargs["protos"]
        self.assertEqual([p.decl for p in protos], ["s32 func_2();", "void func_3(s32 a);"])

    def test_no_option_passes_no_prototypes(self):
        _, _, compile_draft = self.run_regdiff(self.args())
        self.assertEqual(compile_draft.call_args.kwargs["protos"], [])

    def test_an_arguments_object_without_the_option_still_works(self):
        args = self.args()
        del args.proto
        _, _, compile_draft = self.run_regdiff(args)
        self.assertEqual(compile_draft.call_args.kwargs["protos"], [])

    def test_a_malformed_option_is_reported_before_anything_is_compiled(self):
        rc, messages, compile_draft = self.run_regdiff(self.args(proto=["extern s32 D_1;"]))
        self.assertEqual(rc, 2)
        self.assertIn("expected a function declaration", messages[-1])
        compile_draft.assert_not_called()

    def test_a_missing_uopt_is_the_clear_message_and_status_2(self):
        rc, messages, compile_draft = self.run_regdiff(self.args(), uopt=(None, "regdiff: the instrumented (traced) uopt was not found."))
        self.assertEqual(rc, 2)
        self.assertEqual(messages, ["regdiff: the instrumented (traced) uopt was not found."])
        compile_draft.assert_not_called()

    def test_compile_draft_writes_the_overridden_unit_and_keeps_the_notes(self):
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / "f.c"
            src.write_text("x")
            with mock.patch.object(proof, "preprocess_draft", return_value=UNIT), \
                    mock.patch.object(rd.subprocess, "run", return_value=SimpleNamespace(returncode=1)):
                comp = rd.compile_draft(tmp, "caller", str(src), "/u/uopt", os.path.join(tmp, "w"),
                                        protos=pv.parse(["s32 func_8013E97C();"]))
            written = Path(comp.base).read_text()
        self.assertIn("s32 func_8013E97C();", written)
        self.assertNotIn("void func_8013E97C(s32 a, s32 b, s32 c);", written)
        self.assertEqual(written, comp.base_text)
        self.assertEqual(len(comp.proto_notes), 1)
        self.assertEqual([p.name for p in comp.protos], ["func_8013E97C"])

    def test_compile_draft_without_prototypes_writes_the_unit_unchanged(self):
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / "f.c"
            src.write_text("x")
            with mock.patch.object(proof, "preprocess_draft", return_value=UNIT), \
                    mock.patch.object(rd.subprocess, "run", return_value=SimpleNamespace(returncode=1)):
                comp = rd.compile_draft(tmp, "caller", str(src), "/u/uopt", os.path.join(tmp, "w"))
        self.assertEqual(comp.base_text, UNIT)
        self.assertEqual((comp.protos, comp.proto_notes), ([], []))

    def test_a_prototype_for_a_defined_function_is_an_error_report(self):
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / "f.c"
            src.write_text("x")
            with mock.patch.object(proof, "preprocess_draft", return_value=UNIT):
                with self.assertRaisesRegex(ValueError, "is defined in this file"):
                    rd.compile_draft(tmp, "caller", str(src), "/u/uopt", os.path.join(tmp, "w"),
                                     protos=pv.parse(["void caller();"]))

    def test_option_is_on_the_command_line_and_repeatable(self):
        seen = []
        with mock.patch.object(rd.shutil, "which", return_value="/bin/x"), \
                mock.patch.object(rd, "run", side_effect=lambda a: seen.append(a) or 0):
            rd.main(["func_1", "--proto", "void a();", "--proto", "s32 b(s32 x);"])
        self.assertEqual(seen[0].proto, ["void a();", "s32 b(s32 x);"])

    def report(self, protos):
        entries = [("k%d" % i, "k%d  ; i%d" % (i, i)) for i in range(2)]
        comp = SimpleNamespace(base_text="", log="", listing="", ucode=b"", protos=protos,
                               proto_notes=["proto override f: s32 f();  (replaces void f(s32 a);)"] if protos else [])
        lines = []
        with mock.patch.object(pv, "override_macros", return_value=["RPG_BAT_OVERRIDE_f"]):
            rc = rd.report(SimpleNamespace(src="f.c", verbose=False), "g", SimpleNamespace(unit="TT"), "f.c", comp,
                           entries, entries, (0, 0, 0), lines.append)
        return rc, "\n".join(lines)

    def test_a_match_with_an_override_says_so_and_how_to_keep_it(self):
        rc, text = self.report(pv.parse(["s32 f();"]))
        self.assertEqual(rc, 0)
        self.assertIn("proto override f: s32 f();", text)
        self.assertIn("with the prototype override above", text)
        self.assertIn("#define RPG_BAT_OVERRIDE_f", text)

    def test_a_plain_match_has_no_override_text(self):
        _, text = self.report([])
        self.assertIn("MATCH", text)
        self.assertNotIn("override", text)


class FuncdiffPlumbing(unittest.TestCase):
    """funcdiff.main with the compile and the comparison mocked: what matters is which compile is chosen."""

    def run_main(self, argv, guarded=False, exact=(True, "linked bytes equal")):
        with tempfile.TemporaryDirectory() as tmp:
            exp = Path(tmp) / "exp.o"
            exp.write_bytes(b"x")
            got = Path(tmp) / "got.o"
            got.write_bytes(b"y")
            src = Path(tmp) / "a.c"
            src.write_text("x")
            c = SimpleNamespace(src=src, obj=str(got), unit="main")
            out = io.StringIO()
            os.makedirs(os.path.join(tmp, "config"), exist_ok=True)
            cwd = os.getcwd()
            os.chdir(tmp)
            try:
                with mock.patch.object(funcdiff.shutil, "which", return_value="/bin/x"), \
                        mock.patch.object(funcdiff, "locate", return_value=c), \
                        mock.patch.object(funcdiff, "check_fresh", return_value=None) as fresh, \
                        mock.patch.object(funcdiff, "header_problem", return_value=None), \
                        mock.patch.object(funcdiff, "expected_object", return_value=(str(exp), None)), \
                        mock.patch.object(funcdiff, "functions", return_value={"func_1": ["addiu v0,zero,1"]}), \
                        mock.patch.object(proof, "compile_unit", return_value=(str(got), ["proto override func_2: s32 func_2();"])) as unit, \
                        mock.patch.object(proof, "compile_guarded", return_value=str(got)) as plain, \
                        mock.patch.object(proof, "exact_match", return_value=exact), \
                        mock.patch.object(proof, "guarded", return_value=guarded), \
                        mock.patch.object(pv, "override_macros", return_value=["RPG_BAT_OVERRIDE_func_2"]), \
                        contextlib.redirect_stdout(out):
                    rc = funcdiff.main(argv)
            finally:
                os.chdir(cwd)
        return rc, out.getvalue(), unit, plain, fresh

    def test_an_unguarded_file_is_compiled_as_the_production_unit(self):
        rc, out, unit, plain, fresh = self.run_main(["--proto", "s32 func_2();", "func_1"])
        self.assertEqual(rc, 0, out)
        unit.assert_called_once()
        self.assertFalse(unit.call_args.kwargs["non_matching"])
        self.assertEqual([p.name for p in unit.call_args.args[2]], ["func_2"])
        plain.assert_not_called()
        fresh.assert_not_called()
        self.assertIn("func_1: compared default build (full translation unit) with --proto s32 func_2();", out)
        self.assertIn("proto override func_2: s32 func_2();", out)
        self.assertIn("func_1: MATCH", out)
        self.assertIn("with the prototype override above", out)
        self.assertIn("#define RPG_BAT_OVERRIDE_func_2", out)

    def test_a_guarded_function_is_compiled_with_non_matching(self):
        rc, out, unit, _, _ = self.run_main(["--proto", "s32 func_2();", "func_1"], guarded=True)
        self.assertTrue(unit.call_args.kwargs["non_matching"])
        self.assertIn("compared NON_MATCHING C (full translation unit) with --proto", out)

    def test_version_default_never_selects_non_matching(self):
        _, _, unit, _, _ = self.run_main(["--proto", "s32 func_2();", "--version", "default", "func_1"], guarded=True)
        self.assertFalse(unit.call_args.kwargs["non_matching"])

    def test_version_c_forces_non_matching(self):
        _, _, unit, _, _ = self.run_main(["--proto", "s32 func_2();", "--version", "c", "func_1"])
        self.assertTrue(unit.call_args.kwargs["non_matching"])

    def test_without_the_option_nothing_changes(self):
        rc, out, unit, plain, fresh = self.run_main(["func_1"])
        unit.assert_not_called()
        fresh.assert_called_once()
        self.assertIn("func_1: compared default build\n", out)
        self.assertNotIn("override", out)

    def test_no_match_is_claimed_when_the_proof_fails(self):
        rc, out, _, _, _ = self.run_main(["--proto", "s32 func_2();", "func_1"], exact=(False, "linked bytes differ"))
        self.assertEqual(rc, 1)
        self.assertIn("func_1: DIFF", out)
        self.assertNotIn("MATCH", out)
        self.assertNotIn("RPG_BAT_OVERRIDE", out)

    def test_built_and_proto_do_not_mix(self):
        with mock.patch.object(funcdiff.shutil, "which", return_value="/bin/x"):
            with self.assertRaises(SystemExit) as cm:
                funcdiff.main(["--built", "x.o", "--proto", "s32 f();", "func_1"])
        self.assertIn("cannot be combined with --built", str(cm.exception))

    def test_a_malformed_proto_stops_before_any_work(self):
        with mock.patch.object(funcdiff.shutil, "which", return_value="/bin/x"):
            with self.assertRaises(SystemExit) as cm:
                funcdiff.main(["--built", "x.o", "--proto", "int x;", "func_1"])
        self.assertIn("expected a function declaration", str(cm.exception))

    def test_a_refused_override_is_an_error_line_and_status_1(self):
        def refuse(*a, **k):
            raise pv.ProtoError("func_2 is defined in this file; use --src")
        with tempfile.TemporaryDirectory() as tmp:
            os.makedirs(os.path.join(tmp, "config"))
            src = Path(tmp) / "a.c"
            src.write_text("x")
            c = SimpleNamespace(src=src, obj="a.o", unit="main")
            out = io.StringIO()
            cwd = os.getcwd()
            os.chdir(tmp)
            try:
                with mock.patch.object(funcdiff.shutil, "which", return_value="/bin/x"), \
                        mock.patch.object(funcdiff, "locate", return_value=c), \
                        mock.patch.object(proof, "guarded", return_value=False), \
                        mock.patch.object(proof, "compile_unit", side_effect=refuse), \
                        contextlib.redirect_stdout(out):
                    rc = funcdiff.main(["--proto", "void func_2();", "func_1"])
            finally:
                os.chdir(cwd)
        self.assertEqual(rc, 1)
        self.assertIn("func_1: ERROR func_2 is defined in this file", out.getvalue())


class CompileUnit(unittest.TestCase):
    def test_the_production_variant_does_not_define_non_matching(self):
        seen = []
        with mock.patch.object(proof, "command", side_effect=lambda a: seen.append(a) or ""):
            proof.preprocess_draft("f.c", non_matching=False)
            proof.preprocess_draft("f.c")
        self.assertNotIn("-DNON_MATCHING", seen[0])
        self.assertIn("-DNON_MATCHING", seen[1])
        self.assertEqual(seen[0][-1], "f.c")

    def test_compile_unit_applies_the_override_before_the_compiler_sees_the_unit(self):
        import object_layout
        import trailing_pad
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            obj = tmp / "guarded.o"
            compiled = []

            def fake_command(args):
                if args[0] == "gcc":
                    return UNIT
                compiled.append(Path(args[2]).read_text())
                obj.write_bytes(b"obj")
                return ""
            c = SimpleNamespace(src=tmp / "f.c")
            with mock.patch.object(proof, "command", side_effect=fake_command), \
                    mock.patch.object(trailing_pad, "pad_functions"), \
                    mock.patch.object(object_layout, "trim", side_effect=lambda data, extent: data), \
                    mock.patch.object(object_layout, "source_extent", return_value=0):
                path, notes = proof.compile_unit(c, tmp, pv.parse(["s32 func_8013E97C();"]), non_matching=False)
        self.assertEqual(path, str(obj))
        self.assertEqual(len(notes), 1)
        self.assertIn("s32 func_8013E97C();", compiled[0])
        self.assertNotIn("void func_8013E97C(s32 a, s32 b, s32 c);", compiled[0])


if __name__ == "__main__":
    unittest.main()
