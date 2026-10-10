#!/usr/bin/env python3
"""Unit tests for tools/funcdiff.py (T-3300): synthetic objects only, no game data.

Run: tools/docker.sh python3 tools/test_funcdiff.py   (the integration tests need binutils)
"""
import contextlib
import io
import os
import shutil
import stat
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import funcdiff  # noqa: E402

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
        return [k for k, _d in funcdiff.resolve(funcdiff.parse_insns(funcs[name]))]

    def test_hi_lo_pair_equals_absolute_address(self):
        got = self.keys("func_80100000")
        self.assertEqual(got[0], "3c19801d")
        self.assertEqual(got[1], "872863c8")
        self.assertEqual(got[0:2], self.keys("Lfoo"))

    def test_jal_resolved_and_unknown_symbol_kept(self):
        got = self.keys("func_80100000")
        self.assertEqual(got[2], "0c040800")        # jal 0x80102000: (0x80102000 >> 2) & 0x3ffffff
        self.assertEqual(got[3], "3c010000 R_MIPS_HI16 bg_read_sub2")

    def test_negative_low_half_carries_into_hi(self):
        insns = [[0x3c190000, "lui t9,0x0", [("R_MIPS_HI16", "D_801D8010", 0)]],
                 [0x87280000, "lh t0,0(t9)", [("R_MIPS_LO16", "D_801D8010", 0)]]]
        self.assertEqual([k for k, _ in funcdiff.resolve(insns)], ["3c19801e", "87288010"])


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


class Host(unittest.TestCase):
    def test_no_objdump_says_docker_without_a_nameerror(self):
        with mock.patch.object(funcdiff.shutil, "which", return_value=None):
            with self.assertRaises(SystemExit) as cm:
                funcdiff.main(["func_1"])
        self.assertIn("tools/docker.sh", str(cm.exception))

    def test_unknown_function_message(self):
        out = io.StringIO()
        with in_repo_dir(), mock.patch.object(funcdiff, "locate", return_value=None), \
                mock.patch.object(funcdiff.shutil, "which", return_value="/bin/x"), \
                contextlib.redirect_stdout(out):
            rc = funcdiff.main(["func_1"])
        self.assertEqual(rc, 1)
        self.assertIn("not found in any C source", out.getvalue())


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
        self.assertEqual((rc, out.strip()), (0, "func_80100000: MATCH (relocations resolved)"))

    def test_function_starting_with_L(self):
        rc, out = self.run_main("LoadImage")
        self.assertEqual((rc, out.strip()), (0, "LoadImage: MATCH"))


if __name__ == "__main__":
    unittest.main()
