#!/usr/bin/env python3
"""Synthetic checks for the T-9800 trace repair and evaluation gate.

Docker only. Optional UOPT_TRACE_BINARY and UOPT_RECOMP_SOURCE enable native
integration tests. TMPDIR must point into the external experiment directory.
No game or reconstruction source is embedded in these tests.
"""

import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

import uopt_eval
import uopt_link_eval
import uopt_patch


class EvaluationTests(unittest.TestCase):
    def test_register_bits_are_not_masked(self):
        self.assertFalse(uopt_eval.compare([("24030001", "li v1,1")],
                                           [("24020001", "li v0,1")])["match"])

    def test_only_display_names_are_ignored(self):
        self.assertTrue(uopt_eval.compare([("resolved word", "old symbol")],
                                          [("resolved word", "new symbol")])["match"])

    def test_missing_function_is_not_an_empty_match(self):
        self.assertFalse(uopt_eval.compare(None, None)["match"])
        self.assertFalse(uopt_eval.compare([], None)["match"])

    def test_source_and_anchor_checks_fail_closed(self):
        with self.assertRaises(ValueError):
            uopt_patch.patch("unrelated source", "zero_threshold")
        with self.assertRaises(ValueError):
            uopt_patch.replace_exact("anchor anchor", "anchor", "new")

    def test_case_bounds_keep_delay_slot_but_ignore_object_alignment(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            results = []
            for name, slot, padding in (("short", "nop", 0), ("aligned", "nop", 16),
                                         ("changed", "addiu $v0,$zero,1", 0)):
                src, obj = root / (name + ".s"), root / (name + ".o")
                src.write_text(".set noreorder\n.text\n.globl f\n.ent f\nf:\n"
                               "jr $ra\n" + slot + "\n.end f\n" +
                               (".space " + str(padding) + "\n" if padding else ""))
                subprocess.run(["mips-linux-gnu-as", "-EL", "-march=r3000", "-o",
                                str(obj), str(src)], check=True)
                results.append(uopt_eval.case_function(str(obj), "f", "synthetic.c"))
            self.assertEqual(len(results[0]), 2)
            self.assertTrue(uopt_eval.compare(results[0], results[1])["match"])
            self.assertFalse(uopt_eval.compare(results[0], results[2])["match"])

    def test_original_and_c_sizes_handle_explicit_trailing_word(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            original_asm = root / "original.s"
            original_asm.write_text("endlabel f\n    /* 8 80100008 00000000 */ nop\n")
            rows = []
            for original in (False, True):
                src, obj = root / (str(original) + ".s"), root / (str(original) + ".o")
                text = ".set noreorder\n.text\n.globl f\n.type f,@function\nf:\njr $ra\nnop\n"
                text += "nop\n.size f,.-f\n" if original else ".size f,.-f\nnop\n"
                src.write_text(text)
                subprocess.run(["mips-linux-gnu-as", "-EL", "-march=r3000", "-o",
                                str(obj), str(src)], check=True)
                with patch.object(uopt_eval.trailing_pad, "find_asm", return_value=original_asm), \
                        patch.object(uopt_eval, "original_ranges", return_value=[]):
                    rows.append(uopt_eval.case_function(str(obj), "f", "synthetic.c", original, "test"))
            self.assertEqual(len(rows[0]), 3)
            self.assertTrue(uopt_eval.compare(*rows)["match"])

    def test_linked_byte_gate_checks_address_and_keeps_input_unchanged(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            src, obj = root / "f.s", root / "f.o"
            src.write_text(".set noreorder\n.text\n.globl f\n.ent f\nf:\njr $ra\nnop\n.end f\n")
            subprocess.run(["mips-linux-gnu-as", "-EL", "-march=r3000", "-o",
                            str(obj), str(src)], check=True)
            images = []
            for address in (0x80100000, 0x80100100):
                elf = root / (str(address) + ".elf")
                subprocess.run(["mips-linux-gnu-ld", "-EL", "-Ttext=" + hex(address),
                                "-e", "f", "-o", str(elf), str(obj)], check=True)
                before = elf.read_bytes()
                images.append(uopt_link_eval.image_functions(elf, ".text", elf.with_suffix(".raw")))
                self.assertEqual(before, elf.read_bytes())
            self.assertEqual(images[0][2]["f"], (0x80100000, 8))
            self.assertTrue(uopt_link_eval.literal_match(images[0], images[0], "f", 0))
            self.assertFalse(uopt_link_eval.literal_match(images[0], images[1], "f", 0))
            changed = (images[0][0], b"\xff" + images[0][1][1:], images[0][2])
            self.assertFalse(uopt_link_eval.literal_match(changed, images[0], "f", 0))

    @unittest.skipUnless(os.environ.get("UOPT_TRACE_BINARY"), "set UOPT_TRACE_BINARY for integration")
    def test_real_trace_and_codegen(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            src = root / "synthetic.c"
            src.write_text("extern int d; int f(void) { return d+d; }\n")
            binary = os.environ["UOPT_TRACE_BINARY"]
            objects = []
            for name, uopt, trace in (("stock", "/opt/ido/5.3/uopt", False),
                                      ("trace", binary, True)):
                obj = root / (name + ".o")
                cmd = [sys.executable, str(Path(uopt_eval.__file__)), "compile", str(src),
                       str(obj), "--uopt", uopt] + (["--trace"] if trace else [])
                result = subprocess.run(cmd, capture_output=True, text=True)
                self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
                objects.append(uopt_eval.funcdiff.resolved_functions(str(obj), ["f"])["f"])
            self.assertTrue(uopt_eval.compare(*objects)["match"])
            listing = (root / "trace.o.trace").read_text()
            self.assertRegex(listing, r"assigned \(unconstrained\)\s+2")
            self.assertIn("global coloring of f", listing)

    @unittest.skipUnless(os.environ.get("UOPT_RECOMP_SOURCE"), "set UOPT_RECOMP_SOURCE for guest ABI check")
    def test_ecvt_guest_abi(self):
        text = Path(os.environ["UOPT_RECOMP_SOURCE"]).read_text()
        match = re.search(r"uint32_t wrapper_ecvt\([^\n]+\{.*?\n\}", text, re.S)
        self.assertIsNotNone(match)
        # Exercise the actual patched wrapper, including byte-swapped guest
        # strings, sign/decimal outputs, reuse and growth of its static buffer.
        fixture = r'''
#define _GNU_SOURCE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#define MEM_U8(a) mem[(a) ^ 3]
#define MEM_S32(a) (*(int32_t *)(mem + (a)))
static int allocations;
uint32_t wrapper_realloc(uint8_t *mem, uint32_t old, uint32_t size) {
    (void)mem; (void)old; assert(size < 128); allocations++; return 128;
}
'''
        fixture += match[0]
        fixture += r'''
static void check(uint8_t *mem, double number, int digits,
                  const char *wanted, int point, int sign) {
    uint32_t result = wrapper_ecvt(mem, number, digits, 16, 20);
    assert(result == 128);
    assert(MEM_S32(16) == point && MEM_S32(20) == sign);
    for (unsigned i = 0; i <= strlen(wanted); i++) assert(MEM_U8(result+i) == wanted[i]);
}
int main(void) {
    uint32_t storage[128] = {0};
    uint8_t *mem = (uint8_t *)storage;
    check(mem, 12.5, 4, "1250", 2, 0);
    check(mem, -0.03125, 4, "3125", -1, 1);
    assert(allocations == 1);
    check(mem, 1.25, 8, "12500000", 1, 0);
    assert(allocations == 2);
    check(mem, 0.0, 4, "0000", 1, 0);
    assert(allocations == 2);
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            src, binary = Path(tmp) / "ecvt.c", Path(tmp) / "ecvt"
            src.write_text(fixture)
            subprocess.run(["gcc", "-std=c11", "-Wall", "-Wextra", "-Werror",
                            str(src), "-o", str(binary)], check=True)
            subprocess.run([str(binary)], check=True)


if __name__ == "__main__":
    unittest.main()
