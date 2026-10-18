#!/usr/bin/env python3
"""Unit tests for tools/funcdiff.py (T-3300): synthetic objects only, no game data.

Run: tools/docker.sh python3 tools/test_funcdiff.py   (the integration tests need binutils)
"""
import contextlib
import io
import os
import shutil
import stat
import struct
import subprocess
import sys
import tempfile
import threading
import time
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import funcdiff  # noqa: E402
import funcloc  # noqa: E402

HAVE_BINUTILS = bool(shutil.which("mips-linux-gnu-as") and shutil.which("mips-linux-gnu-objdump"))

OBJDUMP = """
Disassembly of section .text:

00000000 <LoadImage>:
   0:\t27bdffe8 \taddiu\tsp,sp,-24
   4:\t10000003 \tb\t14 <L80133A00>
   8:\t00000000 \tnop

0000000c <L80133A00>:
   c:\t03e00008 \tjr\tra
  10:\t00000000 \tnop

00000014 <Lfoo>:
  14:\t3c19801d \tlui\tt9,0x801d
  18:\t872863c8 \tlh\tt0,25544(t9)

0000001c <func_80100000>:
  1c:\t3c190000 \tlui\tt9,0x0
\t\t\t1c: R_MIPS_HI16\tD_801D63C8
  20:\t87280000 \tlh\tt0,0(t9)
\t\t\t20: R_MIPS_LO16\tD_801D63C8
  24:\t0c000000 \tjal\t0 <func_80100000>
\t\t\t24: R_MIPS_26\tfunc_80102000
  28:\t3c010000 \tlui\tat,0x0
\t\t\t28: R_MIPS_HI16\tbg_read_sub2
"""


@contextlib.contextmanager
def in_repo_dir():
    """A temporary working directory that looks like a repository root (has config/)."""
    cwd, tmp = os.getcwd(), tempfile.mkdtemp()
    os.mkdir(os.path.join(tmp, "config"))
    os.chdir(tmp)
    try:
        yield tmp
    finally:
        os.chdir(cwd)
        shutil.rmtree(tmp, True)


class Parse(unittest.TestCase):
    def test_function_names_starting_with_L(self):
        funcs = funcdiff.split_functions(OBJDUMP)
        self.assertEqual(list(funcs), ["LoadImage", "Lfoo", "func_80100000"])
        # the label L80133A00 stays inside LoadImage
        self.assertEqual(len(funcs["LoadImage"]), 5)

    def test_normalised_text_ignores_addresses(self):
        with mock.patch.object(funcdiff, "disassemble", return_value=OBJDUMP):
            got = funcdiff.functions("x.o", ["LoadImage", "nope"])
        self.assertIsNone(got["nope"])
        self.assertEqual(got["LoadImage"][1], "10000003 b <L80133A00>")


class Resolve(unittest.TestCase):
    def keys(self, name):
        funcs = funcdiff.split_functions(OBJDUMP)
        return [k for k, _d in funcdiff.resolve(funcdiff.parse_insns(funcs[name]), names=self.names)]

    names = {}

    def test_hi_lo_pair_equals_absolute_address(self):
        got = self.keys("func_80100000")
        self.assertEqual(got[0], "3c19801d")
        self.assertEqual(got[1], "872863c8")
        self.assertEqual(got[0:2], self.keys("Lfoo"))

    def test_jal_resolved_and_unknown_symbol_kept(self):
        got = self.keys("func_80100000")
        self.assertEqual(got[2], "0c040800")        # jal 0x80102000: (0x80102000 >> 2) & 0x3ffffff
        self.assertEqual(got[3], "3c010000 R_MIPS_HI16 bg_read_sub2")

    def test_renamed_symbol_resolves_through_the_rename_table(self):
        # bg_read_sub2 is func_8007ED84 in config/obin_renames.txt (T-5030)
        self.names = {"bg_read_sub2": 0x8007ED84}
        self.assertEqual(self.keys("func_80100000")[3], "3c018008")

    def test_renames_are_read_from_the_config_files(self):
        with in_repo_dir() as tmp:
            with open(os.path.join(tmp, "config", "obin_renames.txt"), "w") as f:
                f.write("# old new\nfunc_8007ED84 bg_read_sub2\n")
            with open(os.path.join(tmp, "config", "symbol_addrs_main.txt"), "w") as f:
                f.write("my_global = 0x800E6280; // type:u8\n")
            funcdiff._names.clear()
            names = funcdiff.renamed_addresses(".")
            funcdiff._names.clear()
        self.assertEqual(names, {"bg_read_sub2": 0x8007ED84, "my_global": 0x800E6280})

    def test_negative_low_half_carries_into_hi(self):
        insns = [[0x3c190000, "lui t9,0x0", [("R_MIPS_HI16", "D_801D8010", 0)]],
                 [0x87280000, "lh t0,0(t9)", [("R_MIPS_LO16", "D_801D8010", 0)]]]
        self.assertEqual([k for k, _ in funcdiff.resolve(insns)], ["3c19801e", "87288010"])


class HeaderProblem(unittest.TestCase):
    def setUp(self):
        funcdiff._headers.clear()
        self.cwd = os.getcwd()
        self.tmp = tempfile.mkdtemp()
        os.chdir(self.tmp)
        self.addCleanup(shutil.rmtree, self.tmp, True)
        self.addCleanup(os.chdir, self.cwd)
        self.addCleanup(funcdiff._headers.clear)

    def test_no_include_dir_means_nothing_to_check(self):
        self.assertIsNone(funcdiff.header_problem(False))

    def test_failing_ninja_target_is_returned(self):
        with open("build.ninja", "w") as f:
            f.write("x")
        r = mock.Mock(returncode=1, stdout="FAILED: build/headers.ok\nconflict D_1\n")
        with mock.patch.object(funcdiff.shutil, "which", return_value="/bin/ninja"), \
                mock.patch.object(funcdiff.subprocess, "run", return_value=r) as run:
            problem = funcdiff.header_problem(True)
        self.assertEqual(run.call_args[0][0], ["ninja", "build/headers.ok"])
        self.assertIn("conflict D_1", problem)

    def test_without_a_build_check_headers_runs_directly(self):
        os.mkdir("include")
        with open("include/a.h", "w") as f:
            f.write("extern int x;\nextern char x;\n")
        self.assertIn("conflict x", funcdiff.header_problem(False))


class Fresh(unittest.TestCase):
    def setUp(self):
        funcdiff._checked.clear()
        self.cwd = os.getcwd()
        self.tmp = tempfile.mkdtemp()
        os.chdir(self.tmp)
        self.addCleanup(shutil.rmtree, self.tmp, True)
        self.addCleanup(os.chdir, self.cwd)

    def touch(self, path, mtime):
        with open(path, "w") as f:
            f.write("x")
        os.utime(path, (mtime, mtime))

    def test_missing_object(self):
        self.touch("a.c", 100)
        self.assertIn("does not exist", funcdiff.check_fresh("a.o", "a.c", build=False))

    def test_stale_object(self):
        self.touch("a.c", 200)
        self.touch("a.o", 100)
        self.assertIn("older than a.c", funcdiff.check_fresh("a.o", "a.c", build=False))

    def test_fresh_object(self):
        self.touch("a.c", 100)
        self.touch("a.o", 200)
        self.assertIsNone(funcdiff.check_fresh("a.o", "a.c", build=False))

    def test_freshness_is_not_cached_after_source_changes(self):
        self.touch("a.c", 100)
        self.touch("a.o", 200)
        self.assertIsNone(funcdiff.check_fresh("a.o", "a.c", build=False))
        self.touch("a.c", 300)
        self.assertIn("older than", funcdiff.check_fresh("a.o", "a.c", build=False))

    def fake_ninja(self, exit_code):
        os.mkdir("bin")
        path = os.path.join("bin", "ninja")
        with open(path, "w") as f:
            f.write("#!/bin/sh\necho 'FAILED: a.o'; echo 'a.c:3: error: boom'; exit %d\n" % exit_code)
        os.chmod(path, os.stat(path).st_mode | stat.S_IEXEC)
        self.touch("build.ninja", 1)
        return mock.patch.dict(os.environ, {"PATH": os.path.abspath("bin") + os.pathsep + os.environ["PATH"]})

    def test_failed_build_is_an_error_even_if_an_old_object_exists(self):
        self.touch("a.c", 100)
        self.touch("a.o", 200)          # newer than the source: only the ninja exit code tells
        with self.fake_ninja(1):
            problem = funcdiff.check_fresh("a.o", "a.c")
        self.assertIn("FAILED", problem)
        self.assertIn("boom", problem)

    def test_successful_build_passes(self):
        self.touch("a.c", 100)
        self.touch("a.o", 200)
        with self.fake_ninja(0):
            self.assertIsNone(funcdiff.check_fresh("a.o", "a.c"))

    def test_main_reports_the_failed_build_and_exits_1(self):
        self.touch("a.c", 100)
        self.touch("a.o", 200)
        os.mkdir("config")
        cf = mock.Mock(obj="a.o", src="a.c")
        out = io.StringIO()
        with self.fake_ninja(1), mock.patch.object(funcdiff, "locate", return_value=cf), \
                mock.patch.object(funcdiff.shutil, "which", side_effect=lambda n: "/bin/" + n), \
                contextlib.redirect_stdout(out):
            rc = funcdiff.main(["func_1"])
        self.assertEqual(rc, 1)
        self.assertIn("func_1: ERROR the build of a.o FAILED", out.getvalue())
        self.assertNotIn("MATCH", out.getvalue())


    def test_header_newer_than_object_is_stale(self):
        self.touch("a.c", 100)
        self.touch("a.h", 300)
        self.touch("a.o", 200)
        self.assertIn("older than a.h", funcdiff.check_fresh("a.o", "a.c", build=False, deps=["a.h"]))
        funcdiff._checked.clear()
        self.touch("a.o", 400)
        self.assertIsNone(funcdiff.check_fresh("a.o", "a.c", build=False, deps=["a.h"]))

    def test_no_build_and_built_never_match_a_stale_object(self):
        # a failed build leaves the old object; the edited source is newer than it
        self.touch("a.c", 200)
        self.touch("a.o", 100)
        os.mkdir("config")
        cf = mock.Mock(obj="a.o", src="a.c")
        for argv in (["--no-build", "func_1"], ["--built", "a.o", "func_1"]):
            funcdiff._checked.clear()
            out = io.StringIO()
            with mock.patch.object(funcdiff, "locate", return_value=cf), \
                    mock.patch.object(funcdiff, "cfile_of_object", return_value=cf), \
                    mock.patch.object(funcdiff.shutil, "which", side_effect=lambda n: "/bin/" + n), \
                    contextlib.redirect_stdout(out):
                rc = funcdiff.main(argv)
            self.assertEqual(rc, 1, argv)
            self.assertIn("ERROR a.o is older than a.c", out.getvalue())
            self.assertNotIn("MATCH", out.getvalue())


class Host(unittest.TestCase):
    def test_no_objdump_says_docker_without_a_nameerror(self):
        with mock.patch.object(funcdiff.shutil, "which", return_value=None):
            with self.assertRaises(SystemExit) as cm:
                funcdiff.main(["func_1"])
        self.assertIn("tools/docker.sh", str(cm.exception))

    def test_unknown_function_message(self):
        out = io.StringIO()
        with in_repo_dir(), mock.patch.object(funcdiff, "locate", side_effect=funcloc.LocateError(
                "func_1: no INCLUDE_ASM or C definition of it")), \
                mock.patch.object(funcdiff.shutil, "which", return_value="/bin/x"), \
                contextlib.redirect_stdout(out):
            rc = funcdiff.main(["func_1"])
        self.assertEqual(rc, 1)
        self.assertIn("no INCLUDE_ASM or C definition", out.getvalue())

    def test_ambiguous_name_is_an_error_listing_the_candidates(self):
        err = funcloc.AmbiguousError("func_1: held by 2 C files: A (src/ovl/A.c), B (src/ovl/B.c); pick one")
        out = io.StringIO()
        with in_repo_dir(), mock.patch.object(funcdiff, "locate", side_effect=err), \
                mock.patch.object(funcdiff.shutil, "which", return_value="/bin/x"), \
                contextlib.redirect_stdout(out):
            rc = funcdiff.main(["func_1"])
        self.assertEqual(rc, 1)
        self.assertIn("A (src/ovl/A.c), B (src/ovl/B.c)", out.getvalue())

    def test_unit_option_and_prefix_reach_locate(self):
        calls = []
        err = funcloc.LocateError("nope")
        with in_repo_dir(), mock.patch.object(funcdiff, "locate", side_effect=lambda n, u=None: calls.append((n, u)) or (_ for _ in ()).throw(err)), \
                mock.patch.object(funcdiff.shutil, "which", return_value="/bin/x"), \
                contextlib.redirect_stdout(io.StringIO()):
            funcdiff.main(["--unit", "TT", "func_1"])
            funcdiff.main(["B:func_2"])
        self.assertEqual(calls, [("func_1", "TT"), ("func_2", "B")])


@unittest.skipUnless(HAVE_BINUTILS, "needs mips-linux-gnu binutils (Docker)")
class Objects(unittest.TestCase):
    """Real objects: the original-side object has relocations, the C-side object absolute words."""

    ORIGINAL = """.set noat
.set noreorder
.section .text
.globl func_80100000
func_80100000:
    lui $t9, %hi(D_801D63C8)
    lh $t0, %lo(D_801D63C8)($t9)
    jr $ra
    nop
.globl LoadImage
LoadImage:
    jr $ra
    nop
"""
    BUILT = """.set noat
.set noreorder
.section .text
.globl func_80100000
func_80100000:
    lui $t9, 0x801D
    lh $t0, 0x63C8($t9)
    jr $ra
    nop
.globl LoadImage
LoadImage:
    jr $ra
    nop
"""

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.tmp, True)
        self.exp = self.assemble("exp", self.ORIGINAL)
        self.got = self.assemble("got", self.BUILT)

    def assemble(self, name, text):
        s, o = os.path.join(self.tmp, name + ".s"), os.path.join(self.tmp, name + ".o")
        with open(s, "w") as f:
            f.write(text)
        subprocess.run(["mips-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32", "-o", o, s], check=True)
        return o

    def run_main(self, *argv):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rc = funcdiff.main(["--built", self.got, "--expected", self.exp] + list(argv))
        return rc, out.getvalue()

    def test_text_mode_reports_the_reloc_only_diff(self):
        rc, out = self.run_main("func_80100000")
        self.assertEqual(rc, 1)
        self.assertIn("R_MIPS_LO16 D_801D63C8", out)

    def test_resolve_mode_matches(self):
        rc, out = self.run_main("--resolve", "func_80100000")
        self.assertEqual(rc, 0, out)
        self.assertIn("MATCH (exact resolved instruction bytes)", out)
        self.assertIn("compared explicit object", out)

    STR_ORIGINAL = """.set noat
.set noreorder
.section .rodata
.word 0, 0, 0, 0
.globl D_80100010
D_80100010:
.asciz "hello"
.section .text
.globl func_80100100
func_80100100:
    lui $a1, %hi(D_80100010)
    addiu $a1, $a1, %lo(D_80100010)
    jr $ra
    nop
"""
    STR_BUILT = """.set noat
.set noreorder
.section .rodata
L1:
.asciz "{text}"
.section .text
.globl func_80100100
func_80100100:
    lui $a1, %hi(L1)
    addiu $a1, $a1, %lo(L1)
    jr $ra
    nop
"""

    def string_case(self, text):
        exp = self.assemble("sexp", self.STR_ORIGINAL)
        got = self.assemble("sgot", self.STR_BUILT.replace("{text}", text))
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rc = funcdiff.main(["--built", got, "--expected", exp, "--resolve", "func_80100100"])
        return rc, out.getvalue()

    def test_resolve_compares_string_relocations_by_content(self):
        rc, out = self.string_case("hello")
        self.assertEqual(rc, 1, out)
        self.assertIn("literal-byte proof", out)
        self.assertNotIn(": MATCH", out)

    def test_resolve_different_string_is_a_diff(self):
        rc, out = self.string_case("jello")
        self.assertEqual(rc, 1)
        self.assertIn("DIFF", out)
        self.assertIn("jello", out)

    def test_parse_object_info(self):
        info = funcdiff.parse_object_info(
            "00000000 l    d  .rodata\t00000020 .rodata\n00000010 g     O .rodata\t00000006 D_80100010\n"
            "00000000 g     F .text\t00000010 f\n",
            "Contents of section .rodata:\n 0000 00000000 00000000 00000000 00000000  ................\n"
            " 0010 68656c6c 6f00                          hello.          \n")
        self.assertEqual(info.symbols, {".rodata": 0, "D_80100010": 0x10})
        self.assertEqual(info.string_at("D_80100010", 0), b"hello")
        self.assertEqual(info.string_at(".rodata", 0x11), b"ello")
        self.assertEqual(info.string_at(".rodata", 0), b"")     # a valid empty literal
        self.assertIsNone(info.string_at("elsewhere", 0))

    def test_function_starting_with_L(self):
        rc, out = self.run_main("LoadImage")
        self.assertEqual(rc, 0, out)
        self.assertIn("LoadImage: MATCH", out)

    def test_instruction_swap_is_a_diff(self):
        self.got = self.assemble("swap", self.BUILT.replace(
            "    lui $t9, 0x801D\n    lh $t0, 0x63C8($t9)",
            "    lh $t0, 0x63C8($t9)\n    lui $t9, 0x801D"))
        rc, out = self.run_main("--resolve", "func_80100000")
        self.assertEqual(rc, 1, out)
        self.assertNotIn(": MATCH", out)

    def test_folded_zero_words_cannot_hide_a_length_difference(self):
        self.got = self.assemble("zeros", self.BUILT.replace(".globl LoadImage", ".word 0, 0, 0\n.globl LoadImage"))
        rc, out = self.run_main("--resolve", "func_80100000")
        self.assertEqual(rc, 1, out)
        self.assertNotIn(": MATCH", out)

    def test_guarded_auto_mode_compiles_c_and_requires_literal_proof(self):
        import funcdiff_verify as proof
        source = os.path.join(self.tmp, "draft.c")
        with open(source, "w") as f:
            f.write('#ifdef NON_MATCHING\nint func_80100000(void) { return 1; }\n#else\n'
                    'INCLUDE_ASM("asm", func_80100000);\n#endif\n')
        c = mock.Mock(src=source, obj=self.got, unit="main")
        out = io.StringIO()
        with mock.patch.object(funcdiff, "locate", return_value=c), \
                mock.patch.object(funcdiff, "check_fresh", return_value=None), \
                mock.patch.object(funcdiff, "header_problem", return_value=None), \
                mock.patch.object(proof, "compile_guarded", return_value=self.got) as compile_c, \
                mock.patch.object(proof, "exact_match", return_value=(False, "wrong linked bytes")), \
                contextlib.redirect_stdout(out):
            rc = funcdiff.main(["--expected", self.exp, "--resolve", "func_80100000"])
        self.assertEqual(rc, 1, out.getvalue())
        compile_c.assert_called_once()
        self.assertIn("compared NON_MATCHING C", out.getvalue())
        self.assertNotIn(": MATCH", out.getvalue())

    def test_local_symbol_cannot_hide_a_difference_after_the_prefix(self):
        template = '.set noreorder\n.text\n.globl f\nf:\nnop\ninner:\naddiu $v0,$zero,%d\njr $ra\nnop\n'
        self.exp = self.assemble("local-e", template % 1)
        self.got = self.assemble("local-g", template % 2)
        rc, out = self.run_main("--resolve", "f")
        self.assertEqual(rc, 1, out)
        self.assertNotIn(": MATCH", out)

    def test_address_named_own_data_cannot_match_without_layout(self):
        self.exp = self.assemble("owned", self.ORIGINAL +
                                 '.data\n.globl D_801D63C8\nD_801D63C8:\n.word 1\n')
        rc, out = self.run_main("--resolve", "func_80100000")
        self.assertEqual(rc, 1, out)
        self.assertIn("requires unit layout", out)
        self.assertNotIn(": MATCH", out)

    def test_absolute_elf_value_cannot_be_replaced_by_symbol_spelling(self):
        import trailing_pad
        elf = trailing_pad.Elf(Path(self.exp).read_bytes())
        table = elf.find('.symtab')
        for i, symbol in enumerate(trailing_pad.read_symbols(elf, table)):
            if symbol[0] == 'D_801D63C8':
                struct.pack_into('<I', elf.sections[table]['data'], i * 16 + 4, 0x80100000)
                struct.pack_into('<H', elf.sections[table]['data'], i * 16 + 14, 0xFFF1)
        Path(self.exp).write_bytes(elf.to_bytes())
        rc, out = self.run_main('--resolve', 'func_80100000')
        self.assertEqual(rc, 1, out)
        self.assertIn('ELF value differing', out)
        self.assertNotIn(': MATCH', out)

    def test_source_change_during_proof_cannot_match(self):
        import funcdiff_verify as proof
        source = Path(self.tmp) / 'draft.c'
        source.write_text('int func_80100000(void) { return 1; }\n')
        c = mock.Mock(src=source, obj=self.got, unit='main')
        def changing(*args):
            source.write_text('int func_80100000(void) { return 2; }\n')
            return True, 'equal'
        out = io.StringIO()
        with mock.patch.object(funcdiff, 'locate', return_value=c), \
                mock.patch.object(funcdiff, 'check_fresh', return_value=None), \
                mock.patch.object(funcdiff, 'header_problem', return_value=None), \
                mock.patch.object(proof, 'exact_match', side_effect=changing), \
                contextlib.redirect_stdout(out):
            rc = funcdiff.main(['--expected', self.exp, '--resolve', 'func_80100000'])
        self.assertEqual(rc, 1, out.getvalue())
        self.assertIn('changed during comparison', out.getvalue())
        self.assertNotIn(': MATCH', out.getvalue())


@unittest.skipUnless(HAVE_BINUTILS, "needs binutils")
class TableAndOffsetDiffs(unittest.TestCase):
    """T-9220: jump-table functions and GameState offsets."""

    HEAD = ".set noat\n.set noreorder\n"

    def setUp(self):
        self.tmp = tempfile.mkdtemp()
        self.addCleanup(shutil.rmtree, self.tmp, True)

    def assemble(self, name, text):
        s, o = os.path.join(self.tmp, name + ".s"), os.path.join(self.tmp, name + ".o")
        with open(s, "w") as f:
            f.write(self.HEAD + text)
        subprocess.run(["mips-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32", "-o", o, s], check=True)
        return o

    def run_main(self, exp, got, *argv):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rc = funcdiff.main(["--built", got, "--expected", exp] + list(argv))
        return rc, out.getvalue()

    GLOBL = ".globl jtbl_80100100\n"
    TABLE = """.section .rodata
%s:
.word f
.word f
.section .text
.globl f
f:
    lui $at, %%hi(%s)
    sll $t6, $a0, 2
    addu $at, $at, $t6
    lw $t6, %%lo(%s)($at)
    jr $t6
    nop
"""

    def test_jump_table_function_matches_without_manual_resolve(self):
        exp = self.assemble("e", self.GLOBL + self.TABLE % ("jtbl_80100100", "jtbl_80100100", "jtbl_80100100"))
        got = self.assemble("g", self.TABLE % ("L1", "L1", "L1"))
        rc, out = self.run_main(exp, got, "f")
        self.assertEqual(rc, 1, out)
        self.assertNotIn(": MATCH", out)
        self.assertIn("literal-byte proof", out)

    def test_jump_table_function_with_another_instruction_is_still_a_diff(self):
        exp = self.assemble("e", self.GLOBL + self.TABLE % ("jtbl_80100100", "jtbl_80100100", "jtbl_80100100"))
        got = self.assemble("g", (self.TABLE % ("L1", "L1", "L1")).replace("sll $t6, $a0, 2", "sll $t6, $a1, 2"))
        rc, out = self.run_main(exp, got, "f")
        self.assertEqual(rc, 1)
        self.assertIn("REAL DIFFERENCE", out)

    def test_internal_label_headers_are_naming_only(self):
        exp = self.assemble("e", ".text\n.globl f, .L80100100\nf:\nnop\n.L80100100:\njr $ra\nnop\n")
        got = self.assemble("g", ".text\n.globl f\nf:\nnop\njr $ra\nnop\n")
        rc, out = self.run_main(exp, got, "f", "--resolve")
        self.assertEqual(rc, 0, out)

    def test_resolve_real_diff_always_has_a_verdict(self):
        exp = self.assemble("e", self.FIELD % ("D_800E738D", "D_800E738D"))
        got = self.assemble("g", self.FIELD % ("D_800E738E", "D_800E738E"))
        rc, out = self.run_main(exp, got, "f", "--resolve")
        self.assertEqual(rc, 1)
        self.assertIn("resolved: REAL DIFFERENCE", out)
        self.assertTrue(out.splitlines()[-1].strip().startswith("resolved:"), out)

    def test_missing_resolved_comparison_has_an_explicit_verdict(self):
        exp = self.assemble("e", self.FIELD % ("D_800E738D", "D_800E738D"))
        got = self.assemble("g", self.FIELD % ("D_800E738E", "D_800E738E"))
        with mock.patch.object(funcdiff, "resolved_verdict", return_value=None):
            rc, out = self.run_main(exp, got, "f")
        self.assertEqual(rc, 1)
        self.assertIn("resolved: UNAVAILABLE", out)

    def test_external_original_strings_and_empty_literals_resolve(self):
        # Converted .s files have external string symbols instead of their rodata.
        original = funcdiff.ObjInfo(original=[(0x80144A40, b"\0\0\0\0hello\0")])
        built = funcdiff.ObjInfo(b"\0\0\0\0hello\0")
        for addr, off in (("D_80144A40", 0), ("D_80144A44", 4)):
            want = [[0x3c040000, "lui", [("R_MIPS_HI16", addr, 0)]],
                    [0x24840000, "addiu", [("R_MIPS_LO16", addr, 0)]]]
            got = [[0x3c040000, "lui", [("R_MIPS_HI16", ".rodata", 0)]],
                   [0x24840000 + off, "addiu", [("R_MIPS_LO16", ".rodata", 0)]]]
            self.assertEqual(funcdiff.keys(funcdiff.resolve(want, original)),
                             funcdiff.keys(funcdiff.resolve(got, built)))
        self.assertIsNone(original.string_at("D_80144A50", 0))

    def test_original_string_fallback_is_scoped_to_one_overlay(self):
        with in_repo_dir():
            os.makedirs("config/overlays", exist_ok=True)
            for unit, literal in (("A", b"first"), ("B", b"other")):
                with open(unit + ".bin", "wb") as f:
                    f.write(bytes(16) + literal + bytes(16 - len(literal)))
                with open("config/overlays/%s.yaml" % unit, "w") as f:
                    f.write("options: {target_path: %s.bin}\nsegments:\n"
                            "  - start: 0\n    vram: 0x80132000\n    type: code\n"
                            "    subsegments: [[0, c, %s], [16, .rodata, %s], [32, data, data]]\n"
                            % (unit, unit, unit))
            a = funcdiff.ObjInfo(original=funcdiff.original_rodata(mock.Mock(unit="A")))
            b = funcdiff.ObjInfo(original=funcdiff.original_rodata(mock.Mock(unit="B")))
            self.assertEqual(a.string_at("D_80132010", 0), b"first")
            self.assertEqual(b.string_at("D_80132010", 0), b"other")

    FIELD = """.section .text
.globl f
f:
    lui $t9, %%hi(%s)
    lbu $t0, %%lo(%s)($t9)
    jr $ra
    nop
"""

    def test_naming_only_difference_is_labelled_as_such(self):
        exp = self.assemble("e", self.FIELD % ("D_800E738D", "D_800E738D"))
        got = self.assemble("g", self.FIELD % ("D_800E6280 + 0x110D", "D_800E6280 + 0x110D"))
        rc, out = self.run_main(exp, got, "f")
        self.assertEqual(rc, 1)
        self.assertIn("differ only in symbol naming", out)
        self.assertEqual(self.run_main(exp, got, "--resolve", "f")[0], 0)

    def test_wrong_gamestate_offset_is_a_real_diff(self):
        exp = self.assemble("e", self.FIELD % ("D_800E738D", "D_800E738D"))
        got = self.assemble("g", self.FIELD % ("D_800E6280 + 0x110C", "D_800E6280 + 0x110C"))
        rc, out = self.run_main(exp, got, "f")
        self.assertEqual(rc, 1)
        self.assertIn("REAL DIFFERENCE", out)
        self.assertNotIn("differ only in symbol naming", out)
        self.assertEqual(self.run_main(exp, got, "--resolve", "f")[0], 1)

    def test_other_relocation_types_keep_their_addend(self):
        a = funcdiff.resolve([[0x10, "x", [("R_MIPS_32", "D_800E6280", 0x10)]]])
        b = funcdiff.resolve([[0x10, "x", [("R_MIPS_32", "D_800E6280", 0x14)]]])
        self.assertNotEqual(a[0][0], b[0][0])


class ExpectedObject(unittest.TestCase):
    """T-7030: the original-side object is built from the original .s files, never copied by hand."""

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.old = os.getcwd()
        os.chdir(self.tmp.name)
        self.addCleanup(self.tmp.cleanup)
        self.addCleanup(os.chdir, self.old)
        import srcscan
        self.m = "asm/ovl/AAA/matchings/AAA/80132000"
        self.n = "asm/ovl/AAA/nonmatchings/AAA/80132000"
        os.makedirs(self.m)
        os.makedirs(self.n)
        self.asm("%s/func_80132040.s" % self.n, "80132040")
        self.asm("%s/early_name.s" % self.m, "80132000")          # renamed: ordered by its address
        self.c = srcscan.CFile("AAA", "AAA/80132000", __import__("pathlib").Path("src/ovl/AAA/80132000.c"),
                               __import__("pathlib").Path(self.n), __import__("pathlib").Path(self.m),
                               "build/ovl/AAA/src/ovl/AAA/80132000.o", False, 0x80132000)
        self.compiled = []

    def asm(self, path, vram, body="jr $ra"):
        with open(path, "w") as f:
            f.write("nonmatching x, 0x8\n\nglabel x\n    /* 0 %s 03E00008 */  %s\n" % (vram, body))

    def compile(self, stub, obj):
        self.compiled.append(obj)
        with open(obj, "w") as f:
            f.write("obj")
        return None

    def test_stub_lists_every_function_in_address_order_from_both_folders(self):
        funcs = funcdiff.asm_functions(self.c)
        self.assertEqual([f[2] for f in funcs], ["early_name", "func_80132040"])
        text = funcdiff.expected_stub(funcs)
        self.assertIn('INCLUDE_ASM("%s", early_name);' % self.m, text)
        self.assertLess(text.index("early_name"), text.index("func_80132040"))

    def test_stale_copy_of_a_function_that_changed_state_is_ignored(self):
        self.asm("%s/func_80132040.s" % self.m, "80132040", "nop")     # stale: now INCLUDE_ASM again
        self.asm("%s/early_name.s" % self.n, "80132000", "nop")        # stale: now defined in C
        os.makedirs("src/ovl/AAA", exist_ok=True)
        with open("src/ovl/AAA/80132000.c", "w") as f:
            f.write('void early_name() {\n}\nINCLUDE_ASM("%s", func_80132040);\n' % self.n)
        stale = []
        funcs = funcdiff.asm_functions(self.c, stale)
        self.assertEqual(funcs, [(0x80132000, self.m, "early_name"), (0x80132040, self.n, "func_80132040")])
        self.assertEqual(sorted(os.path.basename(x) for x in stale), ["early_name.s", "func_80132040.s"])
        text = funcdiff.expected_stub(funcs)
        self.assertEqual(text.count("early_name"), 1)
        self.assertEqual(text.count("func_80132040"), 1)

    def test_stale_copy_without_a_readable_c_file_is_decided_by_age(self):
        self.asm("%s/func_80132040.s" % self.m, "80132040", "nop")
        os.utime("%s/func_80132040.s" % self.m, (1, 1))
        funcs = funcdiff.asm_functions(self.c)
        self.assertEqual(sum(1 for f in funcs if f[2] == "func_80132040"), 1)
        self.assertIn(self.n, [f[1] for f in funcs if f[2] == "func_80132040"])

    def test_created_when_missing_and_reused_while_the_original_is_unchanged(self):
        obj, problem = funcdiff.expected_object(self.c, compile_fn=self.compile)
        self.assertEqual((obj, problem), ("expected/build/ovl/AAA/src/ovl/AAA/80132000.o", None))
        funcdiff.expected_object(self.c, compile_fn=self.compile)
        self.assertEqual(len(self.compiled), 1)

    def test_refreshed_when_the_original_asm_changes_or_is_forced(self):
        funcdiff.expected_object(self.c, compile_fn=self.compile)
        self.asm("%s/func_80132040.s" % self.n, "80132040", "nop")
        funcdiff.expected_object(self.c, compile_fn=self.compile)
        self.assertEqual(len(self.compiled), 2)
        funcdiff.expected_object(self.c, refresh=True, compile_fn=self.compile)
        self.assertEqual(len(self.compiled), 3)

    def test_editing_the_c_source_never_changes_the_reference(self):
        os.makedirs("src/ovl/AAA", exist_ok=True)
        with open("src/ovl/AAA/80132000.c", "w") as f:
            f.write("void func_80132040(void) {}\n")
        funcdiff.expected_object(self.c, compile_fn=self.compile)
        with open("src/ovl/AAA/80132000.c", "w") as f:
            f.write("void func_80132040(void) { int x; }\n")
        funcdiff.expected_object(self.c, compile_fn=self.compile)
        self.assertEqual(len(self.compiled), 1)

    def test_a_failed_compile_is_a_problem_and_leaves_no_current_object(self):
        obj, problem = funcdiff.expected_object(self.c, compile_fn=lambda s, o: "boom")
        self.assertIsNone(obj)
        self.assertEqual(problem, "boom")
        obj, problem = funcdiff.expected_object(self.c, compile_fn=self.compile)
        self.assertIsNone(problem)
        self.assertEqual(len(self.compiled), 1)

    def test_concurrent_readers_publish_one_complete_reference(self):
        barrier = threading.Barrier(2)
        def slow_compile(source, obj):
            time.sleep(0.05)
            return self.compile(source, obj)
        def compare(_):
            barrier.wait()
            obj, problem = funcdiff.expected_object(self.c, compile_fn=slow_compile)
            self.assertIsNone(problem)
            return Path(obj).read_bytes()
        results, errors = [None, None], []
        def worker(i):
            try:
                results[i] = compare(i)
            except BaseException as error:
                errors.append(error)
        workers = [threading.Thread(target=worker, args=(i,)) for i in range(2)]
        for worker in workers:
            worker.start()
        for worker in workers:
            worker.join(timeout=10)
            self.assertFalse(worker.is_alive())
        self.assertEqual(errors, [])
        self.assertEqual(results[0], results[1])
        self.assertEqual(len(self.compiled), 1)

    def test_no_asm_is_an_error_not_an_empty_reference(self):
        import shutil as sh
        sh.rmtree("asm")
        obj, problem = funcdiff.expected_object(self.c, compile_fn=self.compile)
        self.assertIsNone(obj)
        self.assertIn("asm/ is missing", problem)


class Wave8Relocations(unittest.TestCase):
    def test_short_absolute_symbol_and_low_carry(self):
        self.assertEqual(funcdiff.sym_address("D_7FFFFF"), 0x7FFFFF)
        self.assertIsNone(funcdiff.sym_address("unknown"))
        self.assertIsNone(funcdiff.sym_address("my_dead"))
        reloc = [[0x3c010000, "lui", [("R_MIPS_HI16", "D_7FFFFF", 0)]],
                 [0x2421ffff, "addiu", [("R_MIPS_LO16", "D_7FFFFF", 0)]]]
        self.assertEqual(funcdiff.keys(funcdiff.resolve(reloc)), ["3c010080", "2421fffe"])
        # A symbol holding 0x7fffff with zero low addend produces the literal constant.
        reloc[1][0] = 0x24210000
        self.assertEqual(funcdiff.keys(funcdiff.resolve(reloc)), ["3c010080", "2421ffff"])

    def test_pc16_section_positions_and_absolute_target(self):
        ins = funcdiff.parse_insns(["  24: 1000ffff b", "  24: R_MIPS_PC16 .L80100010"])
        info = funcdiff.ObjInfo(text_symbols={".L80100010": 0x30})
        self.assertEqual(funcdiff.keys(funcdiff.resolve(ins, info)), ["10000002"])
        self.assertEqual(funcdiff.keys(funcdiff.resolve(ins, text_base=0x800fffe0)), ["10000002"])
        ins[0][2] = [("R_MIPS_PC16", ".L80100010", 4)]
        self.assertEqual(funcdiff.keys(funcdiff.resolve(ins, info)), ["10000003"])
        ins[0][2] = [("R_MIPS_PC16", "missing", 0)]
        self.assertIn("R_MIPS_PC16 missing", funcdiff.resolve(ins, info)[0][0])

    def test_scheduled_string_highs_do_not_inherit_first_low_identity(self):
        original = funcdiff.ObjInfo(original=[(0x80100000, b"first\0second\0")])
        built = funcdiff.ObjInfo(rodata=b"first\0second\0")
        # Two delay-slot highs precede both lows: first-LO matching gives both
        # built highs 'first', although one path uses 'second'. The linker high
        # is shared; each LO still checks its actual string contents.
        want = [[0x3c040000, "lui", [("R_MIPS_HI16", "D_80100000", 0)]],
                [0x3c040000, "lui", [("R_MIPS_HI16", "D_80100006", 0)]],
                [0x24840000, "addiu", [("R_MIPS_LO16", "D_80100000", 0)]],
                [0x24840000, "addiu", [("R_MIPS_LO16", "D_80100006", 0)]]]
        got = [[0x3c040000, "lui", [("R_MIPS_HI16", ".rodata", 0)]],
               [0x3c040000, "lui", [("R_MIPS_HI16", ".rodata", 0)]],
               [0x24840000, "addiu", [("R_MIPS_LO16", ".rodata", 0)]],
               [0x24840006, "addiu", [("R_MIPS_LO16", ".rodata", 0)]]]
        self.assertEqual(funcdiff.keys(funcdiff.resolve(want, original)),
                         funcdiff.keys(funcdiff.resolve(got, built)))
        got[3][0] = 0x24840000
        self.assertNotEqual(funcdiff.keys(funcdiff.resolve(want, original)),
                            funcdiff.keys(funcdiff.resolve(got, built)))


class SearchScore(unittest.TestCase):
    def test_edit_costs_and_empty_functions(self):
        for want, got, cost in [([], [], 0), ([], ["a"], 1), (["a"], [], 1),
                                (["a"], ["b"], 2), (["a", "b"], ["a", "x", "b"], 1)]:
            with self.subTest(want=want, got=got):
                self.assertEqual(funcdiff.diff_score(want, got), cost)
                self.assertEqual(funcdiff.diff_score(got, want), cost)

    def test_extra_wrong_instruction_cannot_win_by_changing_diff_mode(self):
        want = list("abcdefghijkl")
        shifted = ["inserted"] + want[:-1]
        worse = shifted + ["another wrong instruction"]
        # Positional diagnostics retain useful evidence, but invert this ranking.
        self.assertGreater(len(funcdiff.diff_lines(want, shifted)),
                           len(funcdiff.diff_lines(want, worse)))
        self.assertEqual(funcdiff.diff_score(want, shifted), 2)
        self.assertEqual(funcdiff.diff_score(want, worse), 3)

    def test_display_text_is_not_part_of_the_score(self):
        self.assertEqual(funcdiff.diff_score([("key", "old label")], [("key", "new label")]), 0)
        self.assertGreater(funcdiff.diff_score([("key", "same")], [("other", "same")]), 0)

    def test_scheduling_swap_keeps_positional_diagnostics(self):
        want, got = ["a", "b"], ["b", "a"]
        self.assertEqual(funcdiff.diff_score(want, got), 2)
        self.assertEqual(len(funcdiff.diff_lines(want, got)), 4)
        self.assertTrue(all("@row" in row for row in funcdiff.diff_lines(want, got)))


if __name__ == "__main__":
    unittest.main()
