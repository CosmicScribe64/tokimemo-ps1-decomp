#!/usr/bin/env python3
"""Synthetic ELF checks for configured subobject text spans (T-10200)."""
from pathlib import Path
import subprocess
import tempfile
import unittest

import object_layout as layout
from trailing_pad import Elf


def assemble(body):
    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp) / "test.o"
        subprocess.run(["mips-linux-gnu-as", "-EL", "-march=r3000", "-o", str(obj)],
                       input=(".set noreorder\n.text\n" + body).encode(), check=True)
        return obj.read_bytes()


FUNCTION = ".globl f\n.type f,@function\nf:\njr $ra\nnop\n.size f, .-f\n"


class LayoutTests(unittest.TestCase):
    def test_trim_tail_only_and_keep_relocations(self):
        before = assemble(FUNCTION + ".data\n.word f\n")
        after = layout.trim(before, 8)
        a, b = Elf(before), Elf(after)
        self.assertEqual(len(b.sections[b.find(".text")]["data"]), 8)
        for old, new in zip(a.sections, b.sections):
            if old["sname"] != ".text":
                self.assertEqual(old["data"], new["data"])
        self.assertEqual(layout.trim(after, 8), after)

    def test_preserve_original_padding(self):
        before = assemble(FUNCTION)
        after = Elf(layout.trim(before, 12))
        self.assertEqual(len(after.sections[after.find(".text")]["data"]), 12)

    def test_unchanged_matching_and_mismatching_sizes(self):
        before = assemble(FUNCTION)
        self.assertEqual(layout.trim(before, 16), before)
        self.assertEqual(layout.trim(before, 32), before)
        longer = assemble(FUNCTION + ".space 32\n")
        self.assertEqual(layout.trim(longer, 8), longer)

    def test_nonzero_tail_rejected(self):
        with self.assertRaisesRegex(ValueError, "nonzero"):
            layout.trim(assemble(FUNCTION + ".word 1\n"), 8)

    def test_symbol_in_tail_rejected(self):
        with self.assertRaisesRegex(ValueError, "symbol"):
            layout.trim(assemble(FUNCTION + ".globl tail\ntail:\n.space 4\n.size tail,4\n"), 8)

    def test_zero_size_label_at_trim_boundary_rejected(self):
        with self.assertRaisesRegex(ValueError, "symbol"):
            layout.trim(assemble(FUNCTION + ".globl tail\ntail:\n"), 8)

    def test_relocation_in_tail_rejected(self):
        with self.assertRaisesRegex(ValueError, "relocation"):
            layout.trim(assemble(FUNCTION + ".word external\n"), 8)

    def test_bad_extent_rejected(self):
        for extent in (0, -4, 7):
            with self.assertRaises(ValueError):
                layout.trim(assemble(FUNCTION), extent)

    def test_extent_stops_at_next_asm_subsegment(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "config").mkdir()
            (root / "config/SLPM_86.053.yaml").write_text(
                "segments:\n  - type: code\n    start: 0x800\n    vram: 0x80041000\n"
                "    subsegments:\n      - [0x800, c, main/80041000]\n"
                "      - [0x818, asm, library]\n      - [0x900, rodata, data]\n")
            self.assertEqual(layout.source_extent("src/main/80041000.c", root), 24)

    def test_extent_uses_word_aligned_overlay_boundaries(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "config/overlays").mkdir(parents=True)
            (root / "config/overlays/TEST.yaml").write_text(
                "segments:\n  - type: code\n    start: 0\n    vram: 0x80132000\n"
                "    subsegments:\n      - [0, c, TEST/80132000]\n"
                "      - [0x18, c, TEST/80132018]\n      - [0x30, .rodata, TEST/80132000]\n")
            self.assertEqual(layout.source_extent(root / "src/ovl/TEST/80132018.c", root), 24)

    def test_data_regions_only_for_several_islands(self):
        """T-25005: (section, first label, size) per island of the object, else nothing."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "config").mkdir()
            yaml_text = ("options:\n  base_path: ..\n  asm_path: asm\n"
                         "segments:\n  - type: code\n    start: 0x800\n    vram: 0x80041000\n"
                         "    subsegments:\n      - [0x800, c, main/80041000]\n"
                         "      - { start: 0x900, type: .data, name: main/80041000, dir: island_80041100,"
                         " linker_section: .data_80041100 }\n"
                         "      - [0x920, .data, main/80041000]\n  - [0x940]\n")
            (root / "config/SLPM_86.053.yaml").write_text(yaml_text)
            for sub, label in (("island_80041100/main", "D_80041100"), ("main", "D_80041120")):
                (root / "asm/data" / sub).mkdir(parents=True)
                (root / "asm/data" / sub / "80041000.data.s").write_text(
                    "dlabel %s\n    /* 900 80041100 */ .word 0x0\n" % label)
            self.assertEqual(layout.data_regions("src/main/80041000.c", root),
                             [(".data_80041100", "D_80041100", 0x20), (".data", "D_80041120", 0x20)])
            (root / "config/SLPM_86.053.yaml").write_text(
                yaml_text.replace("{ start: 0x900, type: .data, name: main/80041000, dir: island_80041100,"
                                  " linker_section: .data_80041100 }", "[0x900, data, d]"))
            self.assertEqual(layout.data_regions("src/main/80041000.c", root), [])


if __name__ == "__main__":
    unittest.main()
