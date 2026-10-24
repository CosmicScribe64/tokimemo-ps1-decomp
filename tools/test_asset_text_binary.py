"""Invented initialized-data fixtures; no original game text is included."""
import tempfile
import unittest
from pathlib import Path

from tools.assets.text_binary import enrich_binary_text
from tools.assets.text_mips import enrich_mips_context
from tools import test_asset_text_mips as mips


class BinaryTextTests(unittest.TestCase):
    def fixture(self, root, payload=b"AB %d\0", directives=None):
        fixture = mips.MipsContextTests()
        inventory = fixture.fixture(root, [mips.lui(4, mips.TEXT >> 16),
                                          mips.ori(4, 4, mips.TEXT & 0xFFFF), mips.jal(mips.CALLEE), 0])
        inventory["records"] = []
        binary_path = root / "disc/files/SLPM_86.053"
        binary = bytearray(binary_path.read_bytes())
        offset = mips.TEXT - mips.BASE + 0x800
        binary[offset:offset + len(payload)] = payload
        binary_path.write_bytes(binary)
        directory = root / "asm/data"
        directory.mkdir()
        directives = directives or [f"/* 1000 {mips.TEXT:08X} */ .byte " + ",".join(str(v) for v in payload)]
        (directory / "invented.data.s").write_text("\n".join(["dlabel invented", *directives, "enddlabel invented"]))
        return enrich_mips_context(inventory, root, root / "disc/files")

    def test_exact_label_numeric_bytes_and_format_view(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root)
            self.assertEqual(len(inventory["mips_context"]["unresolved_constant_targets"]), 1)
            enrich_binary_text(inventory, root, root / "disc/files")
            record = inventory["records"][0]
            self.assertEqual(record["raw_hex"], b"AB %d\0".hex())
            self.assertTrue(record["source_roundtrip"])
            self.assertEqual(record["source_decoded_text"], "AB %d")
            self.assertEqual(record["mips_contexts"][0]["argument_value"], mips.TEXT)
            self.assertFalse(record["runtime_read_length_proven"])
            tokens = record["binary_initial_renderer_views"][0]["view"]["tokens"]
            self.assertTrue(any(token["kind"] == "format_directive" for token in tokens))
            self.assertEqual(record["assembly_sites"][0]["label"], "invented")

    def test_mixed_widths_alignment_and_quoted_comment_bytes(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            payload = b"ABCD#/*x*/\0"
            directives = [f"/* 1000 {mips.TEXT:08X} */ .short 0x4241", ".align 1",
                          ".byte 67,68", '.asciz "#/*x*/"']
            inventory = self.fixture(root, payload, directives)
            enrich_binary_text(inventory, root, root / "disc/files")
            self.assertEqual(inventory["records"][0]["raw_hex"], payload.hex())

    def test_no_terminator_across_gap_or_next_label(self):
        for tail in ((f"/* 1004 {mips.TEXT + 4:08X} */ .byte 0",),
                     ("enddlabel invented", "dlabel next", ".byte 0"),
                     (".space 1", ".byte 0")):
            with self.subTest(tail=tail), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                inventory = self.fixture(root, b"ABXX\0", [f"/* 1000 {mips.TEXT:08X} */ .short 0x4241", *tail])
                enrich_binary_text(inventory, root, root / "disc/files")
                self.assertEqual(inventory["records"], [])

    def test_zero_initial_buffer_is_not_an_empty_translation(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, b"\0\0")
            enrich_binary_text(inventory, root, root / "disc/files")
            self.assertEqual(inventory["records"], [])
            self.assertEqual(inventory["binary_text_supplement"]["resolutions"][0]["status"],
                             "initially_zero_runtime_buffer")

    def test_stale_code_or_data_and_nonlabel_pointer_are_rejected(self):
        for problem in ("code", "data", "label"):
            with self.subTest(problem=problem), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                inventory = self.fixture(root)
                if problem == "code":
                    path = root / "disc/files/SLPM_86.053"
                    data = bytearray(path.read_bytes())
                    data[0x800] ^= 1
                    path.write_bytes(data)
                elif problem == "data":
                    path = root / "asm/data/invented.data.s"
                    path.write_text(path.read_text().replace(".byte 65", ".byte 66"))
                elif problem == "label":
                    path = root / "asm/data/invented.data.s"
                    path.write_text(path.read_text().replace(f"{mips.TEXT:08X}", f"{mips.TEXT + 1:08X}"))
                enrich_binary_text(inventory, root, root / "disc/files")
                self.assertEqual(inventory["records"], [])

    def test_foreign_overlay_label_cannot_supply_main_target(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root)
            foreign = root / "asm/ovl/FOREIGN/data"
            foreign.mkdir(parents=True)
            (root / "asm/data/invented.data.s").rename(foreign / "invented.data.s")
            enrich_binary_text(inventory, root, root / "disc/files")
            self.assertEqual(inventory["records"], [])

    def test_strict_decode_failure_preserves_bytes_without_replacement(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, b"\x81\0")
            enrich_binary_text(inventory, root, root / "disc/files")
            record = inventory["records"][0]
            self.assertIsNone(record["source_decoded_text"])
            self.assertEqual(record["raw_hex"], "8100")

    def test_actual_padding_is_verified_and_unknown_tail_does_not_expand_prefix(self):
        for directive, payload, expected in ((".align 2", b"AB\0\0", "414200"),
                                              (".space 100", b"AB\0", "414200")):
            with self.subTest(directive=directive), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                first = ".short 0x4241" if directive.startswith(".align") else ".byte 65,66,0"
                inventory = self.fixture(root, payload, [f"/* 1000 {mips.TEXT:08X} */ {first}", directive])
                enrich_binary_text(inventory, root, root / "disc/files")
                self.assertEqual(inventory["records"][0]["raw_hex"], expected)

    def test_supplied_argument_and_instruction_proofs_are_regenerated(self):
        for proof in ("instruction_evidence", "callee_evidence", "argument_value", "argument_index", None):
            with self.subTest(proof=proof), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                inventory = self.fixture(root)
                call = inventory["mips_context"]["unresolved_constant_targets"][0]
                if proof == "instruction_evidence":
                    call[proof][0]["raw_hex"] = "ffffffff"
                elif proof == "callee_evidence":
                    call[proof]["raw_hex"] = "ffffffff"
                elif proof == "argument_value":
                    call[proof] = mips.TEXT + 1
                elif proof == "argument_index":
                    call[proof] = 1
                enrich_binary_text(inventory, root, root / "disc/files")
                self.assertEqual(len(inventory["records"]), 1)
                context = inventory["records"][0]["mips_contexts"][0]
                self.assertEqual(context["argument_value"], mips.TEXT)
                self.assertEqual(context["argument_index"], 0)
                enrich_binary_text(inventory, root, root / "disc/files")
                self.assertEqual(len(inventory["records"]), 1)
                self.assertEqual(len(inventory["records"][0]["mips_contexts"]), 1)
                self.assertEqual(inventory["binary_text_supplement"]["records_added"], 0)


if __name__ == "__main__":
    unittest.main()
