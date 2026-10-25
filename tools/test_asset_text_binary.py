"""Invented initialized-data fixtures; no original game text is included."""
import tempfile
import unittest
import hashlib
from unittest.mock import patch
from pathlib import Path

from tools.assets.text_binary import enrich_binary_text
from tools.assets.text_mips import enrich_mips_context
from tools import test_asset_text_mips as mips


class BinaryTextTests(unittest.TestCase):
    def test_conditional_call_annotations_are_revalidated(self):
        from tools.assets.text_binary import _enrich_binary_text_from_reports
        from tools.assets.text_context import enrich_source_context
        for tamper in (None, "unresolved_guard", "guards", "guard_value", "duplicate_pc", "missing_delay",
                       "after_delay", "category", "renderer", "read_role"):
            with self.subTest(tamper=tamper), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                words = [mips.lui(6, mips.TEXT >> 16), mips.ori(6, 6, mips.TEXT & 0xFFFF),
                         mips.ori(7, 0, 3), mips.jal(mips.CALLEE), 0]
                if tamper == "unresolved_guard":
                    words.pop(2)
                inventory = self.fixture(root, callee="x_taku_string_set", words=words)
                enrich_source_context(inventory, root, root / "disc/files", use_ast=True)
                call, = inventory["mips_context"]["unresolved_constant_targets"]
                if tamper == "guards":
                    call["callee_read_conditions"] = []
                elif tamper == "guard_value":
                    call["callee_read_conditions"][0]["constant_argument_value"] = 4
                elif tamper == "duplicate_pc":
                    call["instruction_evidence"].append(dict(call["instruction_evidence"][0]))
                elif tamper == "missing_delay":
                    call["instruction_evidence"] = [item for item in call["instruction_evidence"]
                                                    if item["pc"] != call["delay_slot_pc"]]
                elif tamper == "after_delay":
                    item = dict(call["instruction_evidence"][-1])
                    item.update(pc=item["pc"] + 4, offset=item["offset"] + 4)
                    call["instruction_evidence"].append(item)
                elif tamper not in (None, "unresolved_guard"):
                    call[tamper] = "invented"
                _enrich_binary_text_from_reports(inventory, root, root / "disc/files")
                if tamper in (None, "unresolved_guard"):
                    self.assertEqual(len(inventory["records"]), 1)
                    view = inventory["records"][0]["binary_initial_renderer_views"][0]
                    self.assertEqual(len(view["callee_read_conditions"]), 2)
                    if tamper == "unresolved_guard":
                        self.assertEqual(view["callee_read_conditions"][0]["status"], "unknown")
                    continue
                self.assertEqual(inventory["records"], [])
                self.assertEqual(inventory["binary_text_supplement"]["resolutions"][0]["status"],
                                 "call_evidence_requires_revalidation")

    def test_known_false_guard_cannot_be_hidden_as_unknown(self):
        from tools.assets.text_binary import _enrich_binary_text_from_reports
        from tools.assets.text_context import enrich_source_context
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            words = [mips.lui(6, mips.TEXT >> 16), mips.ori(6, 6, mips.TEXT & 0xFFFF),
                     mips.ori(7, 0, 3), mips.jal(mips.CALLEE), 0]
            inventory = self.fixture(root, callee="x_taku_string_set", words=words)
            enrich_source_context(inventory, root, root / "disc/files", use_ast=True)
            call, = inventory["mips_context"]["unresolved_constant_targets"]
            path = root / "disc/files/SLPM_86.053"
            data = bytearray(path.read_bytes())
            replacement = mips.ori(7, 0, 4).to_bytes(4, "little")
            data[0x808:0x80C] = replacement
            path.write_bytes(data)
            digest = hashlib.sha256(data).hexdigest()
            call["source_sha256"] = digest
            call["callee_evidence"]["source_sha256"] = digest
            for item in call["instruction_evidence"]:
                if item["pc"] == mips.BASE + 8:
                    item["raw_hex"] = replacement.hex()
            call["callee_read_conditions"][0].update(status="unknown", constant_argument_value=None)
            _enrich_binary_text_from_reports(inventory, root, root / "disc/files")
            self.assertEqual(inventory["records"], [])
            self.assertTrue(inventory["binary_text_supplement"]["blocked"])

    def test_callee_name_must_match_its_original_symbol_declaration(self):
        from tools.assets.text_binary import _enrich_binary_text_from_reports
        from tools.assets.text_context import enrich_source_context
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root)
            enrich_source_context(inventory, root, root / "disc/files", use_ast=True)
            call, = inventory["mips_context"]["unresolved_constant_targets"]
            call.update(callee="sndi", category="display", renderer="sndi", read_role="display_string")
            _enrich_binary_text_from_reports(inventory, root, root / "disc/files")
            self.assertEqual(inventory["records"], [])
            self.assertTrue(inventory["binary_text_supplement"]["blocked"])

    def test_unmodeled_renderer_stays_explicit_in_initial_view(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, callee="talk_render", words=[
                mips.lui(7, mips.TEXT >> 16), mips.ori(7, 7, mips.TEXT & 0xFFFF), mips.jal(mips.CALLEE), 0])
            enrich_binary_text(inventory, root, root / "disc/files")
            view = inventory["records"][0]["binary_initial_renderer_views"][0]
            self.assertIsNone(view["renderer"])
            self.assertEqual(view["renderer_status"], "unimplemented_fixed_renderer")
            self.assertEqual(view["view"]["unsupported"], "renderer_semantics_not_verified")

    def test_external_assembly_symlink_is_rejected_before_span_read(self):
        from tools.assets.text_binary import _enrich_binary_text_from_reports
        from tools.assets.text_context import enrich_source_context
        with tempfile.TemporaryDirectory() as temp, tempfile.TemporaryDirectory() as external:
            root = Path(temp)
            inventory = self.fixture(root)
            enrich_source_context(inventory, root, root / "disc/files", use_ast=True)
            original = root / "asm/data/invented.data.s"
            outside = Path(external) / "outside.s"
            outside.write_bytes(original.read_bytes())
            original.unlink()
            original.symlink_to(outside)
            with patch("tools.assets.text_binary._spans", side_effect=AssertionError("must not read external source")):
                _enrich_binary_text_from_reports(inventory, root, root / "disc/files")
            self.assertEqual(inventory["records"], [])
            self.assertEqual(inventory["binary_text_supplement"]["blocked"], [{
                "source_path": "asm/data/invented.data.s", "status": "assembly_span_rejected",
                "reason": "ValueError"}])

    def fixture(self, root, payload=b"AB %d\0", directives=None, *, callee="printf", words=None):
        fixture = mips.MipsContextTests()
        if words is None:
            words = [mips.lui(4, mips.TEXT >> 16), mips.ori(4, 4, mips.TEXT & 0xFFFF), mips.jal(mips.CALLEE), 0]
        inventory = fixture.fixture(root, words, callee=callee)
        (root / "src" / "main").mkdir(parents=True)
        (root / "src" / "ovl").mkdir()
        (root / "include").mkdir()
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

    def test_ast_pointer_leaf_recovers_numeric_directive_string_and_label_site(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root)
            (root / "src/main/use.c").write_text(
                "extern const char **ROOT[];\n"
                "void render(void) { sndi(ROOT[0][0], 0, 0); }\n", encoding="utf-8")
            data_path = root / "asm/data/invented.data.s"
            data_path.write_text(
                f"dlabel ROOT\n    /* 10 80041010 18100480 */ .word D_TABLE\n"
                f"dlabel D_TABLE\n    /* 18 80041018 30100480 */ .word D_NUMERIC\n"
                f"dlabel D_NUMERIC\n    /* 30 80041030 00414200 */ .byte 65,66,0\n")
            binary_path = root / "disc/files/SLPM_86.053"
            binary = bytearray(binary_path.read_bytes())
            binary[0x810:0x814] = (0x80041018).to_bytes(4, "little")
            binary[0x818:0x81C] = (0x80041030).to_bytes(4, "little")
            binary[0x830:0x833] = b"AB\0"
            binary_path.write_bytes(binary)

            result = enrich_binary_text(inventory, root, root / "disc/files")
            matches = [record for record in result["records"]
                       if record["source"].get("address") == 0x80041030]
            self.assertEqual(len(matches), 1)
            record = matches[0]
            self.assertEqual(record["raw_hex"], b"AB\0".hex())
            self.assertEqual(record["category"], "display")
            self.assertFalse(record["runtime_read_length_proven"])
            self.assertEqual(record["source_contexts"][0]["selection_certainty"],
                             "statically_selected_pointer_path")
            self.assertEqual(record["assembly_sites"][0]["labels"], ["D_NUMERIC"])
            self.assertTrue(record["binary_initial_renderer_views"])
            self.assertEqual(result["binary_text_supplement"]["pointer_leaf_records_added"], 1)

            unchanged = enrich_binary_text(result, root, root / "disc/files")
            replay_record = next(item for item in unchanged["records"]
                                 if item["source"].get("address") == 0x80041030)
            self.assertEqual(replay_record["category"], "display")
            self.assertTrue(replay_record["binary_initial_renderer_views"])
            self.assertTrue(all(not view["runtime_read_length_proven"]
                                for view in replay_record["binary_initial_renderer_views"]))
            self.assertEqual(unchanged["binary_text_supplement"]["pointer_leaf_records_added"], 0)
            record["assembly_sites"].clear()
            replay = enrich_binary_text(result, root, root / "disc/files")
            replay_record = next(record for record in replay["records"]
                                 if record["source"].get("address") == 0x80041030)
            self.assertEqual(replay_record["raw_hex"], b"AB\0".hex())
            self.assertEqual(replay_record["assembly_sites"][0]["labels"], ["D_NUMERIC"])

    def test_public_api_rederives_tampered_leaf_metadata(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root)
            (root / "src/main/use.c").write_text(
                "extern const char **ROOT[];\nvoid render(void) { sndi(ROOT[0][0], 0, 0); }\n",
                encoding="utf-8")
            (root / "asm/data/invented.data.s").write_text(
                "dlabel ROOT\n    /* 10 80041010 18100480 */ .word D_TABLE\n"
                "dlabel D_TABLE\n    /* 18 80041018 30100480 */ .word D_NUMERIC\n"
                "dlabel D_NUMERIC\n    /* 30 80041030 00414200 */ .byte 65,66,0\n")
            binary_path = root / "disc/files/SLPM_86.053"
            binary = bytearray(binary_path.read_bytes())
            binary[0x810:0x814] = (0x80041018).to_bytes(4, "little")
            binary[0x818:0x81C] = (0x80041030).to_bytes(4, "little")
            binary[0x830:0x833] = b"AB\0"
            binary_path.write_bytes(binary)
            result = enrich_binary_text(inventory, root, root / "disc/files")
            result["unresolved_data_label_leaves"][0]["terminal_data_label"]["address"] = 0x80042000
            replay = enrich_binary_text(result, root, root / "disc/files")
            recovered = [record for record in replay["records"]
                         if record["source"].get("address") == 0x80041030]
            self.assertEqual(len(recovered), 1)
            self.assertEqual(recovered[0]["raw_hex"], b"AB\0".hex())

    def test_pointer_slot_proof_counts_leading_null_words(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, b"AB\0")
            (root / "src/main/use.c").write_text(
                "extern const char **ROOT[];\nvoid render(void) { sndi(ROOT[1][1], 0, 0); }\n",
                encoding="utf-8")
            (root / "asm/data/invented.data.s").write_text(
                "dlabel ROOT\n"
                "    /* 10 80041010 00000000 */ .word 0\n"
                "    /* 14 80041014 18100480 */ .word D_TABLE\n"
                "dlabel D_TABLE\n"
                "    /* 18 80041018 00000000 */ .word 0\n"
                "    /* 1C 8004101C 30100480 */ .word D_NUMERIC\n"
                "dlabel D_NUMERIC\n"
                "    /* 30 80041030 00414200 */ .byte 65,66,0\n")
            binary_path = root / "disc/files/SLPM_86.053"
            binary = bytearray(binary_path.read_bytes())
            binary[0x814:0x818] = (0x80041018).to_bytes(4, "little")
            binary[0x81C:0x820] = (0x80041030).to_bytes(4, "little")
            binary[0x830:0x833] = b"AB\0"
            binary_path.write_bytes(binary)

            result = enrich_binary_text(inventory, root, root / "disc/files")
            self.assertEqual(result["binary_text_supplement"]["pointer_leaf_records_added"], 1)
            self.assertEqual(result["records"][-1]["raw_hex"], b"AB\0".hex())

    def test_public_rerun_revokes_stale_leaf_semantics_but_keeps_extent(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            inventory = self.fixture(root, b"AB\0")
            source = root / "src/main/use.c"
            source.write_text("extern const char **ROOT[];\nvoid render(void) { sndi(ROOT[0][0], 0, 0); }\n",
                              encoding="utf-8")
            (root / "asm/data/invented.data.s").write_text(
                "dlabel ROOT\n    /* 10 80041010 18100480 */ .word D_TABLE\n"
                "dlabel D_TABLE\n    /* 18 80041018 30100480 */ .word D_NUMERIC\n"
                "dlabel D_NUMERIC\n    /* 30 80041030 00414200 */ .byte 65,66,0\n")
            binary_path = root / "disc/files/SLPM_86.053"
            binary = bytearray(binary_path.read_bytes())
            binary[0x810:0x814] = (0x80041018).to_bytes(4, "little")
            binary[0x818:0x81C] = (0x80041030).to_bytes(4, "little")
            binary[0x830:0x833] = b"AB\0"
            binary_path.write_bytes(binary)

            result = enrich_binary_text(inventory, root, root / "disc/files")
            recovered = next(record for record in result["records"]
                             if record["source"].get("address") == 0x80041030)
            record_id = recovered["id"]
            self.assertEqual(recovered["category"], "display")
            self.assertTrue(recovered["binary_initial_renderer_views"])

            source.write_text("extern const char **ROOT[];\nvoid render(void) { (void)ROOT[0][0]; }\n",
                              encoding="utf-8")
            refreshed = enrich_binary_text(result, root, root / "disc/files")
            retained = next(record for record in refreshed["records"] if record["id"] == record_id)
            self.assertEqual(retained["raw_hex"], b"AB\0".hex())
            self.assertEqual(retained["category"], "unknown")
            self.assertFalse(retained.get("source_contexts"))
            self.assertFalse(retained.get("binary_initial_renderer_views"))


if __name__ == "__main__":
    unittest.main()
