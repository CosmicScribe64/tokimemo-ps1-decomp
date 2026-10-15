#!/usr/bin/env python3
"""Synthetic Ninja/real-compiler dependency regressions (Docker, no game data).

Run: python3 -m unittest discover -s tools -p test_build_deps.py -v
Only unrelated pipeline stages are replaced; configure.py emits the actual C and
header-check edges and cc.py uses the installed IDO/GCC preprocessors and tools.
"""
import importlib.util
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile
import unittest
from types import SimpleNamespace
from unittest.mock import patch

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import cc


class NinjaDependencies(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        spec = importlib.util.spec_from_file_location("dependency_configure", ROOT / "configure.py")
        cls.configure = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(cls.configure)

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.previous = Path.cwd()
        os.chdir(self.root)
        self.addCleanup(os.chdir, self.previous)
        for name in ("cc.py", "trailing_pad.py", "srcscan.py", "frame_pass.py", "cvt_pass.py", "check_headers.py", "sync_protos.py"):
            self.write("tools/" + name, (ROOT / "tools" / name).read_text())
        # Layout validation is independent of dependencies and requires a game layout.
        self.write("tools/object_layout.py", "")
        for name in ("asmproc_prelude.inc", "macro.inc", "gte_macros.inc", "labels.inc", "include_asm.h"):
            self.write("include/" + name, (ROOT / "include" / name).read_text())
        self.write("include/shared.h", "#define SHARED 1\n")
        self.write("include/nested.h", "#define NESTED 2\n")
        self.write("include/disabled.h", "#define UNUSED 0\n")
        self.write("include/common.h", '#include "shared.h"\n#include "include_asm.h"\n')
        self.write("include/ovl/ALPHA.h", '#include "nested.h"\n#define LOCAL 3\n')
        self.write("include/ovl/BETA.h", "#define BETA 4\n")
        self.sources = ["src/main/a.c", "src/main/b.c", "src/ovl/ALPHA/a.c", "src/ovl/BETA/b.c"]
        self.write(self.sources[0], '#include "common.h"\n#if 0\n#include "disabled.h"\n#endif\nint main_a(void) { return SHARED; }\n')
        self.write(self.sources[1], '#include "common.h"\nint main_b(void) { return SHARED; }\n')
        self.write(self.sources[2], '#include "common.h"\n#define SELECTED "ovl/ALPHA.h"\n#include SELECTED\nint alpha(void) { return SHARED + NESTED + LOCAL; }\nINCLUDE_ASM("asm/fixture", asm_function);\nINCLUDE_RODATA("asm/fixture", asm_data);\n')
        self.write(self.sources[3], '#include "common.h"\n#include "ovl/BETA.h"\nint beta(void) { return SHARED + BETA; }\n')
        self.write("asm/fixture/asm_function.s", ".section .text\nglabel asm_function\n/* 0 00000000 00000000 */ nop\n/* 4 00000004 0800E003 */ jr $ra\n/* 8 00000008 00000000 */ nop\nendlabel asm_function\n")
        self.write("asm/fixture/asm_data.s", ".section .rodata\nglabel asm_data\n.word 123\n")
        self.write("asm/nonmatchings/main/a/main_a.s", "endlabel main_a\n")
        for path in ("config/SLPM_86.053.yaml", "config/overlays.txt", "config/reloc_addrs.txt", "config/symbol_addrs.txt", "config/SLPM_86.053.sha1", "config/symbol_addrs_obin.txt", "config/symbol_addrs_sdk.txt", "config/symbol_addrs_types.txt", "config/overlays/ALPHA.yaml", "config/overlays/BETA.yaml", "config/overlays/ALPHA.sha1", "config/overlays/BETA.sha1", "config/migrate_globals.txt", "tools/prepare_disc.py", "tools/identify_version.py", "tools/extract_disc.py", "config/versions.txt", "tools/check_symbols.py", "tools/syms_to_ld.py", "tools/data_pieces.py", "tools/migrate_globals.py", "tools/aggregate_audit.py", "tools/asm.py", "configure.py"):
            self.write(path, "")
        self.generate()

    def write(self, name, text):
        path = Path(name)
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)

    def generate(self, gcc=False):
        configure = self.configure
        writer = configure.ninja_syntax.Writer

        class FixtureWriter(writer):
            def rule(self, name, command, **kwargs):
                if name in ("split", "osplit", "disc", "symbols", "globals", "sha1", "osha1", "symsld"):
                    command = "mkdir -p build/ovl && touch $out"
                elif name in ("ld", "old"):
                    command = "cat $in > $out"
                elif name == "configure":
                    command = "touch build.ninja"
                elif name == "objcopy":
                    command = "cp $in $out"
                super().rule(name, command=command, **kwargs)

        def files(name):
            source = self.sources[2 if name == "ALPHA" else 3]
            return [SimpleNamespace(src=Path(source), obj="build/ovl/%s/%s.o" % (name, source[:-2]), name=name + "/a")]

        toolchains = {s: ["ido", "5.3"] for s in self.sources[:2]}
        if gcc:
            toolchains[self.sources[1]] = ["gcc", "2.8.1-psx", "2.79"]
        with patch.object(configure.ninja_syntax, "Writer", FixtureWriter), patch.object(configure, "C_FILES", toolchains), patch.object(configure, "ASM_FILES", []), patch.object(configure, "HEADER_FILES", sorted(str(p) for p in Path("include").rglob("*.h"))), patch.object(configure, "read_overlays", return_value=[("ALPHA", "0", "0"), ("BETA", "0", "0")]), patch.object(configure, "overlay_data_files", return_value=[]), patch.object(configure, "symbol_files", return_value=[]), patch.object(configure.prepare_disc, "find_candidates", return_value=([], [])), patch.object(configure.srcscan, "unit_c_files", side_effect=files):
            configure.main()

    def ninja(self, success=True):
        proc = subprocess.run(["ninja", "-j4"], text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        if success:
            self.assertEqual(proc.returncode, 0, proc.stdout)
        else:
            self.assertNotEqual(proc.returncode, 0, proc.stdout)
        return proc.stdout

    def compiled(self, output):
        return set(re.findall(r"CC (src/\S+)", output))

    def edit(self, name, text="\n"):
        with Path(name).open("a") as f:
            f.write(text)

    def test_fresh_noop_source_shared_and_overlay_header(self):
        self.assertEqual(self.compiled(self.ninja()), set(self.sources))
        self.assertIn("no work to do", self.ninja())
        self.edit(self.sources[0])
        out = self.ninja()
        self.assertEqual(self.compiled(out), {self.sources[0]})
        self.assertIn("CHECK HEADERS", out)
        self.assertNotIn("LD build/ovl/", out)
        self.edit("include/shared.h")
        self.assertEqual(self.compiled(self.ninja()), set(self.sources))
        self.edit("include/ovl/ALPHA.h")
        out = self.ninja()
        self.assertEqual(self.compiled(out), {self.sources[2]})
        self.assertIn("LD build/ovl/ALPHA.elf", out)
        self.assertNotIn("LD build/ovl/BETA.elf", out)
        self.assertNotIn("LD build/SLPM", out)
        self.edit("include/nested.h")
        self.assertEqual(self.compiled(self.ninja()), {self.sources[2]})
        self.edit("include/disabled.h")
        out = self.ninja()
        self.assertEqual(self.compiled(out), set())
        self.assertIn("CHECK HEADERS", out)
        self.assertIn("no work to do", self.ninja())

    def test_validation_failure_blocks_even_when_consumers_are_clean(self):
        self.ninja()
        self.edit("include/disabled.h", "extern int conflicting;\nextern float conflicting;\n")
        out = self.ninja(success=False)
        self.assertIn("CHECK HEADERS", out)
        self.assertFalse(self.compiled(out), out)
        self.assertNotIn("LD build/", out)
        # An independently dirty source also must wait for the failed checker.
        self.edit(self.sources[1])
        self.assertFalse(self.compiled(self.ninja(success=False)))

    def test_missing_active_header_and_new_conditional_include(self):
        self.ninja()
        Path("include/nested.h").unlink()
        self.ninja(success=False)
        self.write("include/nested.h", "#define NESTED 9\n")
        self.assertEqual(self.compiled(self.ninja()), {self.sources[2]})
        self.write("include/new.h", "#define NEW 8\n")
        self.edit(self.sources[0], '#include "new.h"\n')
        self.generate()
        self.assertEqual(self.compiled(self.ninja()), {self.sources[0]})
        self.edit("include/new.h")
        self.assertEqual(self.compiled(self.ninja()), {self.sources[0]})

    def test_asm_rodata_and_trailing_pad_dependencies(self):
        self.ninja()
        for name, source in (("asm/fixture/asm_function.s", self.sources[2]), ("asm/fixture/asm_data.s", self.sources[2]), ("asm/nonmatchings/main/a/main_a.s", self.sources[0])):
            self.edit(name)
            self.assertEqual(self.compiled(self.ninja()), {source})

    def test_gcc_tracks_actual_nested_and_conditional_headers(self):
        self.write(self.sources[1], '#include "common.h"\n#include "ovl/ALPHA.h"\n#if 0\n#include "disabled.h"\n#endif\nint main_b(void) { return SHARED + NESTED; }\n')
        self.generate(gcc=True)
        self.ninja()
        self.assertIn("no work to do", self.ninja())
        self.edit("include/nested.h")
        self.assertEqual(self.compiled(self.ninja()), {self.sources[1], self.sources[2]})
        self.edit("include/disabled.h")
        self.assertFalse(self.compiled(self.ninja()))
        Path("include/nested.h").unlink()
        self.ninja(success=False)
        self.write("include/nested.h", "#define NESTED 8\n")
        self.assertEqual(self.compiled(self.ninja()), {self.sources[1], self.sources[2]})

    def test_ido_tracks_source_local_headers(self):
        self.write("include/shadow.h", "#define PICK 1\n")
        self.write("src/main/shadow.h", "#define PICK 2\n")
        self.write(self.sources[0], '#include "common.h"\n#include "shadow.h"\nint main_a(void) { return PICK; }\n')
        self.generate()
        self.ninja()
        # IDO uses the logical source directory from asm-processor line markers
        # when resolving quoted includes; discover what that invocation reads.
        self.edit("src/main/shadow.h")
        self.assertEqual(self.compiled(self.ninja()), {self.sources[0]})
        self.edit("include/shadow.h")
        self.assertFalse(self.compiled(self.ninja()))

    def test_sjis_copy_does_not_leave_temporary_dependencies(self):
        self.edit(self.sources[0], 'char *message = "日本";\n')
        self.ninja()
        self.assertIn("no work to do", self.ninja())
        self.edit("include/shared.h")
        self.assertEqual(self.compiled(self.ninja()), set(self.sources))

    def test_header_paths_with_make_special_characters(self):
        header = "src/main/special #$.h"
        self.write(header, "#define SPECIAL 7\n")
        self.edit(self.sources[0], '#include "special #$.h"\n')
        self.generate()
        self.ninja()
        self.assertIn("no work to do", self.ninja())
        self.edit(header)
        self.assertEqual(self.compiled(self.ninja()), {self.sources[0]})


if __name__ == "__main__":
    unittest.main()
