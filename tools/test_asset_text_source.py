"""Invented fixtures for source-backed text inventory (T-28001)."""
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

from tools.assets.text_source import (_asm_groups, _c_literals, _call_containing,
                                     _call_containing_argument, _decoded_view,
                                     _identifier_is_value_operand,
                                     _literal_extent_candidates, _string_bytes, inventory_source_text)


class LiteralTests(unittest.TestCase):
    def test_inventory_json_is_hash_seed_independent(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "src" / "main").mkdir(parents=True)
            (root / "src" / "ovl").mkdir()
            (root / "asm" / "data" / "main").mkdir(parents=True)
            (root / "asm" / "matchings" / "main" / "80041000").mkdir(parents=True)
            (root / "asm" / "ovl").mkdir(parents=True)
            (root / "config").mkdir()
            (root / "config" / "overlays.txt").write_text("")
            files = root / "disc" / "files"
            files.mkdir(parents=True)
            exe = bytearray(0x1000)
            exe[0x810:0x812] = b"A\0"
            exe[0x814:0x816] = b"B\0"
            (files / "SLPM_86.053").write_bytes(exe)
            (root / "src" / "main" / "sample.c").write_text(
                "void f(void) { sndisp(D_A, 0, 0); sndisp(D_B, 0, 0); }\n")
            (root / "asm" / "data" / "main" / "strings.s").write_text(
                'dlabel D_A\n    /* 10 80041010 */ .asciz "A"\n'
                'dlabel D_B\n    /* 14 80041014 */ .asciz "B"\n')
            (root / "asm" / "matchings" / "main" / "80041000" / "refs.s").write_text(
                "glabel f\n    lui $a0, %hi(D_B); lui $a1, %hi(D_A)\nendlabel f\n")
            script = ("import json,sys; from tools.assets.text_source import inventory_source_text; "
                      "print(json.dumps(inventory_source_text(sys.argv[1]),sort_keys=True, "
                      "ensure_ascii=False, separators=(',',':')))" )
            project = Path(__file__).resolve().parents[1]
            outputs = []
            for seed in ("1", "913"):
                env = os.environ.copy()
                env["PYTHONHASHSEED"] = seed
                outputs.append(subprocess.run([sys.executable, "-c", script, str(root)],
                                              cwd=project, env=env, check=True,
                                              stdout=subprocess.PIPE, text=True).stdout)
            self.assertEqual(outputs[0], outputs[1])

    def test_cached_call_argument_spans_preserve_nested_delimiters(self):
        code = "sink(inner(a, b), table[(i, j)], (Pair){ .left=x, .right=y });"
        positions = [code.index("a, b"), code.index("table"), code.index(".right")]
        infos = [_call_containing_argument(code, position) for position in positions]
        self.assertEqual([(item["call"], item["argument_index"]) for item in infos],
                         [("inner", 0), ("sink", 1), ("sink", 2)])

    def test_identifier_in_array_subscript_index_is_not_a_text_value(self):
        self.assertFalse(_identifier_is_value_operand("other[D_LABEL]", "D_LABEL"))
        self.assertTrue(_identifier_is_value_operand("D_LABEL", "D_LABEL"))
        self.assertTrue(_identifier_is_value_operand("table[D_INDEX]", "table"))

    def test_strict_unicode_view_preserves_embedded_nul_and_roundtrips(self):
        view = _decoded_view(bytes.fromhex("8365410042"), False)
        self.assertEqual(view["source_decoded_text"], "テA\0B")
        self.assertEqual(view["source_decode_status"], "ok")
        self.assertTrue(view["source_roundtrip"])
        terminated = _decoded_view(bytes.fromhex("414200"), True)
        self.assertEqual(terminated["source_decoded_text"], "AB")
        self.assertTrue(terminated["source_terminal_nul_excluded"])
        invalid = _decoded_view(bytes.fromhex("82"), False)
        self.assertIsNone(invalid["source_decoded_text"])
        self.assertEqual(invalid["source_decode_status"], "invalid_shift_jis")
        self.assertFalse(invalid["source_roundtrip"])

    def test_array_capacity_controls_implicit_terminator_candidates(self):
        site = {"raw_hex": b"AB".hex(), "source_site": {"array_length": 2}}
        self.assertEqual(_literal_extent_candidates(site), (b"AB",))
        site["source_site"]["array_length"] = 3
        self.assertEqual(_literal_extent_candidates(site), (b"AB\0",))

    def test_nonstring_directive_breaks_assembly_extent_and_address_gap(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            data = root / "data"
            data.mkdir()
            (data / "sample.s").write_text(
                'dlabel D_FIRST\n'
                '    /* 0 80041010 */ .ascii "A"\n'
                '    /* 1 80041011 00000041 */ .byte 0x41\n'
                '    .ascii "A"\n')
            groups = list(_asm_groups(root))
            self.assertEqual(len(groups), 2)
            self.assertEqual([bytes(group["raw"]) for group in groups], [b"A", b"A"])
            self.assertEqual([group["address"] for group in groups], [0x80041010, None])

    def _ascii_inventory(self, root: Path, trailing: bytes):
        (root / "src" / "main").mkdir(parents=True)
        (root / "src" / "ovl").mkdir()
        (root / "asm" / "data" / "main").mkdir(parents=True)
        (root / "asm" / "ovl").mkdir(parents=True)
        (root / "config").mkdir()
        (root / "config" / "overlays.txt").write_text("")
        files = root / "disc" / "files"
        files.mkdir(parents=True)
        exe = bytearray(0x1000)
        exe[0x810:0x812] = b"AB"
        exe[0x812:0x812 + len(trailing)] = trailing
        (files / "SLPM_86.053").write_bytes(exe)
        (root / "src" / "main" / "sample.c").write_text('void f(void) { sndisp("AB", 0, 0); }\n')
        (root / "asm" / "data" / "main" / "80041000.rodata.s").write_text(
            'dlabel D_80041010\n    /* 10 80041010 */ .ascii "AB"\n')
        return inventory_source_text(root)

    def test_ascii_prefix_requires_original_nul_for_c_string(self):
        with tempfile.TemporaryDirectory() as temp:
            inventory = self._ascii_inventory(Path(temp), b"\0")
            site = inventory["c_literal_sites"][0]
            self.assertEqual(len(site["binary_candidates"]), 1)
            record = next(r for r in inventory["records"] if r["id"] == site["binary_candidates"][0])
            self.assertEqual(record["raw_hex"], b"AB\0".hex())
            self.assertEqual(record["source"]["length"], 3)
            self.assertEqual(record["extent_status"], "verified_by_assembly_prefix_and_original_terminal_nul")

    def test_ascii_prefix_without_original_nul_is_not_a_c_string_candidate(self):
        with tempfile.TemporaryDirectory() as temp:
            inventory = self._ascii_inventory(Path(temp), b"X")
            site = inventory["c_literal_sites"][0]
            self.assertEqual(site["binary_candidates"], [])
            self.assertEqual(site["binary_status"], "unmapped_literal_no_exact_assembler_match")

    def test_zero_only_binary_extent_has_no_translatable_content(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "src" / "main").mkdir(parents=True)
            (root / "src" / "ovl").mkdir()
            (root / "asm" / "data" / "main").mkdir(parents=True)
            (root / "asm" / "ovl").mkdir(parents=True)
            (root / "config").mkdir()
            (root / "config" / "overlays.txt").write_text("")
            files = root / "disc" / "files"
            files.mkdir(parents=True)
            binary = bytearray(0x1000)
            binary[0x810] = 0
            (files / "SLPM_86.053").write_bytes(binary)
            (root / "asm" / "data" / "main" / "empty.s").write_text(
                'dlabel D_EMPTY\n    /* 10 80041010 */ .asciz ""\n')
            inventory = inventory_source_text(root)
            self.assertEqual(inventory["coverage"]["empty_extent_no_translatable_bytes"], 1)
            self.assertEqual(inventory["coverage"]["semantic_unknown_nonempty_extents"], 0)
            self.assertEqual(inventory["records"][0]["category"], "empty_extent_no_translatable_bytes")

    def test_shift_jis_and_byte_escapes(self):
        self.assertEqual(_string_bytes('"テ"'), bytes.fromhex("8365"))
        self.assertEqual(_string_bytes('"\\203\\x65"'), bytes.fromhex("8365"))

    def test_inventory_verifies_binary_and_keeps_duplicate_candidates(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            files = repo / "disc" / "files"
            (repo / "src" / "main").mkdir(parents=True)
            (repo / "src" / "ovl").mkdir(parents=True)
            asm = repo / "asm" / "data" / "main"
            asm.mkdir(parents=True)
            (repo / "asm" / "ovl").mkdir(parents=True)
            (repo / "config").mkdir()
            (repo / "config" / "overlays.txt").write_text("TEST 0x80132000 0x10\n")
            (repo / "src" / "main" / "sample.c").write_text(
                'void f(void) {\n    sndisp("HI", 0, 0);\n    sndisp("HI", 0, 0);\n    sndisp("", 0, 0);\n}\n')
            exe = bytearray(0x1000)
            exe[0x800 + 0x10:0x800 + 0x13] = b"HI\0"
            exe[0x800 + 0x20:0x800 + 0x23] = b"HI\0"
            exe[0x800 + 0x30:0x800 + 0x33] = b"HI\0"
            files.mkdir(parents=True)
            (files / "SLPM_86.053").write_bytes(exe)
            (asm / "80041000.rodata.s").write_text(
                'dlabel D_80041010\n    /* 10 80041010 */ .asciz "HI"\n'
                'enddlabel D_80041010\ndlabel D_80041020\n'
                '    /* 20 80041020 */ .asciz "HI"\nenddlabel D_80041020\n')
            aggregate = repo / "asm" / "data" / "80041000.aggregate.s"
            aggregate.write_text('dlabel D_80041010\n    /* 10 80041010 */ .asciz "HI"\n'
                                 'dlabel D_80041030\n    /* 30 80041030 */ .asciz "HI"\n')
            inv = inventory_source_text(repo)
            self.assertEqual(inv["coverage"]["verified_extents"], 3)
            self.assertTrue(inv["coverage"]["assembly_source_tree_stable_during_scan"])
            self.assertTrue(inv["coverage"]["assembly_source_tree_sha256"])
            empty = next(site for site in inv["c_literal_sites"] if site["raw_length"] == 0)
            self.assertEqual(empty["binary_status"], "empty_constant_no_translatable_bytes")
            self.assertEqual(empty["function_reference_status"], "empty_constant_no_translatable_bytes")
            self.assertEqual(inv["coverage"]["unmapped_C_literals"], 0)
            self.assertEqual(len(inv["records"]), 3)
            self.assertEqual(len(inv["records"][0]["assembly_sites"]), 2)
            self.assertEqual(inv["records"][0]["reference_status"], "ambiguous_byte_candidates")
            self.assertEqual(inv["records"][0]["category"], "unknown")
            self.assertIn("do not prove", inv["records"][0]["category_basis"])
            self.assertTrue(all(r["source"]["path"] == "SLPM_86.053" for r in inv["records"]))
            self.assertIn("D_80041010", inv["records"][0]["assembly_sites"][0]["labels"])
            self.assertEqual(inv["records"][1]["assembly_sites"][0]["labels"], ["D_80041020"])
            self.assertTrue(inv["records"][0]["terminator"]["nul_terminated"])
            self.assertEqual(inv["records"][0]["source"]["end_offset_exclusive"],
                             inv["records"][0]["source"]["offset"] + 3)

    def test_lexer_concatenates_and_excludes_non_game_literals(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "src" / "main").mkdir(parents=True)
            (root / "src" / "ovl").mkdir()
            (root / "src" / "main" / "lex.c").write_text(
                '#include "header.h"\n'
                'INCLUDE_RODATA("asm/data/main/x", symbol);\n'
                "char marker = 'q';\n"
                'void f(void) { sndisp("A" /* join */ "B", 0, 0); }\n')
            records, blocked, excluded = _c_literals(root / "src")
            self.assertFalse(blocked)
            self.assertEqual(len(records), 1)
            self.assertEqual(records[0]["raw_hex"], b"AB".hex())
            self.assertEqual(records[0]["source_site"]["literal_count"], 2)
            self.assertEqual(len(excluded), 3)
            self.assertTrue(all(item["status"] in ("build_only_path_literal", "character_constant_excluded")
                                for item in excluded))

    def test_array_literal_inherits_unique_consumer_call_context(self):
        self.assertEqual(_call_containing("void f(void) { sndisp(buffer, 0, 0); }", 22), "sndisp")
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "src" / "main").mkdir(parents=True)
            (root / "src" / "ovl").mkdir()
            (root / "src" / "main" / "array.c").write_text(
                'void f(void) {\n    u8 buffer[3] = "AB";\n    sndisp(buffer, 0, 0);\n}\n')
            records, blocked, _ = _c_literals(root / "src")
            self.assertFalse(blocked)
            self.assertEqual(records[0]["source_site"]["array_symbol"], "buffer")
            self.assertEqual(records[0]["source_site"]["function"], "f")
            self.assertEqual(records[0]["source_site"]["array_consumer_calls"], ["sndisp"])
            self.assertEqual(records[0]["source_site"]["call"], "sndisp")
            self.assertEqual(records[0]["source_site"]["call_argument_index"], 0)
            self.assertEqual(records[0]["source_site"]["call_basis"], "unique_array_consumer")
            self.assertEqual(records[0]["category"], "display")

    def test_known_calls_only_classify_the_expected_text_argument(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "src" / "main").mkdir(parents=True)
            (root / "src" / "ovl").mkdir()
            (root / "src" / "main" / "args.c").write_text(
                'void f(void) { sprintf(buf, "%s", "payload%d"); sndi(0, "numeric", 0); }\n')
            records, blocked, _ = _c_literals(root / "src")
            self.assertFalse(blocked)
            by_arg = {record["source_site"]["call_argument_index"]: record
                      for record in records if record["source_site"]["call"] == "sprintf"}
            self.assertEqual(by_arg[1]["category"], "debug_or_format")
            self.assertEqual(by_arg[2]["category"], "unknown")
            numeric = next(record for record in records if record["source_site"]["call"] == "sndi")
            self.assertEqual(numeric["source_site"]["call_argument_index"], 1)
            self.assertEqual(numeric["category"], "unknown")

    def test_overlay_unit_does_not_borrow_main_literal_candidates(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            files = repo / "disc" / "files"
            (repo / "src" / "main").mkdir(parents=True)
            (repo / "src" / "ovl" / "TEST").mkdir(parents=True)
            (repo / "asm" / "data" / "main").mkdir(parents=True)
            (repo / "asm" / "ovl" / "TEST" / "data").mkdir(parents=True)
            (repo / "config").mkdir()
            (repo / "config" / "overlays.txt").write_text("TEST 0x80132000 0x10\n")
            (repo / "src" / "main" / "m.c").write_text('void f(void) { sndisp("HI", 0, 0); }')
            (repo / "src" / "ovl" / "TEST" / "o.c").write_text('void g(void) { sndisp("HI", 0, 0); }')
            exe = bytearray(0x1000)
            exe[0x810:0x813] = b"HI\0"
            files.mkdir(parents=True)
            (files / "SLPM_86.053").write_bytes(exe)
            overlay = bytearray(0x40)
            overlay[0x10:0x13] = b"HI\0"
            (files / "CDROM" / "EXEDIR").mkdir(parents=True)
            (files / "CDROM" / "EXEDIR" / "TEST.EXN").write_bytes(overlay)
            (repo / "asm" / "data" / "main" / "a.s").write_text(
                'dlabel D_80041010\n    /* 10 80041010 */ .asciz "HI"\n')
            (repo / "asm" / "ovl" / "TEST" / "data" / "a.s").write_text(
                'dlabel D_80132010\n    /* 10 80132010 */ .asciz "HI"\n')
            inv = inventory_source_text(repo)
            by_path = {r["source"]["path"]: r for r in inv["records"]}
            self.assertEqual(len(by_path["SLPM_86.053"]["reference_sites"]), 1)
            self.assertEqual(len(by_path["CDROM/EXEDIR/TEST.EXN"]["reference_sites"]), 1)
            self.assertEqual(by_path["CDROM/EXEDIR/TEST.EXN"]["reference_sites"][0]["source_site"]["path"],
                             "src/ovl/TEST/o.c")

    def test_assembler_symbol_references_are_scoped_to_overlay_unit(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            files = repo / "disc" / "files"
            (repo / "src" / "main").mkdir(parents=True)
            (repo / "src" / "ovl").mkdir()
            (repo / "src" / "ovl" / "A").mkdir(parents=True)
            (repo / "asm" / "data" / "main").mkdir(parents=True)
            (repo / "asm" / "ovl").mkdir(parents=True)
            (repo / "config").mkdir()
            (repo / "config" / "overlays.txt").write_text("A 0x80132000 0x40\nB 0x80132000 0x40\n")
            main_binary = bytearray(0x1000)
            main_binary[0x810:0x812] = b"M\0"
            files.mkdir(parents=True)
            (files / "SLPM_86.053").write_bytes(main_binary)
            (repo / "asm" / "data" / "main" / "main.s").write_text(
                'dlabel D_MAIN\n    /* 10 80041010 */ .asciz "M"\n')
            (repo / "src" / "ovl" / "A" / "use.c").write_text(
                'void use_main(void) { sndisp(D_MAIN, 0, 0); }\n')
            (files / "CDROM" / "EXEDIR").mkdir(parents=True)
            for unit, byte in (("A", b"A"), ("B", b"B")):
                binary = bytearray(0x40)
                binary[0x10:0x12] = byte + b"\0"
                (files / "CDROM" / "EXEDIR" / f"{unit}.EXN").write_bytes(binary)
                data = repo / "asm" / "ovl" / unit / "data" / unit
                data.mkdir(parents=True)
                (data / "strings.s").write_text(
                    f'dlabel D_80132010\n    /* 10 80132010 */ .asciz "{byte.decode()}"\n')
                refs = repo / "asm" / "ovl" / unit / "matchings" / unit / "obj"
                refs.mkdir(parents=True)
                (refs / "use.s").write_text(
                    "glabel use\n    lui $a0, %hi(D_80132010)\nendlabel use\n")
                if unit == "A":
                    (refs / "use.s").write_text(
                        "glabel use\n    lui $a0, %hi(D_80132010)\n"
                        "    lui $a1, %hi(D_MAIN)\nendlabel use\n")
            inventory = inventory_source_text(repo)
            records = {record["source"]["path"]: record for record in inventory["records"]}
            self.assertEqual(set(records), {"SLPM_86.053", "CDROM/EXEDIR/A.EXN", "CDROM/EXEDIR/B.EXN"})
            for unit in ("A", "B"):
                record = records[f"CDROM/EXEDIR/{unit}.EXN"]
                refs = [ref for ref in record["reference_sites"]
                        if ref.get("kind") == "assembler_symbol_reference"]
                self.assertEqual(len(refs), 1)
                self.assertIn(f"asm/ovl/{unit}/", refs[0]["path"])
            main_refs = [ref for ref in records["SLPM_86.053"]["reference_sites"]
                         if ref.get("kind") == "assembler_symbol_reference" and
                         ref.get("label") == "D_MAIN"]
            self.assertEqual(len(main_refs), 1)
            self.assertIn("asm/ovl/A/", main_refs[0]["path"])
            main_c_refs = [ref for ref in records["SLPM_86.053"]["reference_sites"]
                           if ref.get("kind") == "C_symbol_reference" and ref.get("label") == "D_MAIN"]
            self.assertEqual(len(main_c_refs), 1)
            self.assertIsNone(main_c_refs[0]["semantic_category"])
            self.assertEqual(main_c_refs[0]["semantic_category_candidate"], "display")

    def test_matching_function_proves_data_label_reference_and_code_extent(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            files = repo / "disc" / "files"
            (repo / "src" / "main").mkdir(parents=True)
            (repo / "src" / "ovl").mkdir()
            (repo / "asm" / "data" / "main").mkdir(parents=True)
            match_dir = repo / "asm" / "matchings" / "main" / "80041000"
            match_dir.mkdir(parents=True)
            (repo / "asm" / "ovl").mkdir(parents=True)
            (repo / "config").mkdir()
            (repo / "config" / "overlays.txt").write_text("")
            (repo / "src" / "main" / "sample.c").write_text(
                'void f(void) { sndisp("HI", 0, 0); }\n')
            exe = bytearray(0x1000)
            exe[0x810:0x813] = b"HI\0"
            exe[0x820:0x824] = bytes.fromhex("00000000")
            files.mkdir(parents=True)
            (files / "SLPM_86.053").write_bytes(exe)
            (repo / "asm" / "data" / "main" / "80041000.rodata.s").write_text(
                'dlabel D_80041010\n    /* 10 80041010 00004948 */ .word 0x00004948\n')
            (match_dir / "f.s").write_text(
                'nonmatching f, 0x4\nglabel f\n'
                '    /* 1020 80041020 00000000 */ lui $at, %hi(D_80041010)\n'
                'endlabel f\n')
            inv = inventory_source_text(repo)
            site = inv["c_literal_sites"][0]
            self.assertEqual(site["function_reference_status"], "unique_same_function_label_and_exact_bytes")
            self.assertEqual(site["function_code_extent"]["status"], "verified_instruction_bytes")
            self.assertTrue(site["function_code_extent"]["sha256"])
            self.assertEqual(site["stable_id"], "c:main:80041020:1")
            old_locator = site["source_locator_id"]
            record = next(r for r in inv["records"] if r["raw_hex"] == bytes.fromhex("484900").hex())
            self.assertTrue(any(ref.get("kind") == "matching_function_data_reference" and
                                ref["confirmed_reference"] for ref in record["reference_sites"]))
            (repo / "src" / "main" / "sample.c").rename(repo / "src" / "main" / "renamed.c")
            renamed = inventory_source_text(repo)
            renamed_site = renamed["c_literal_sites"][0]
            self.assertEqual(renamed_site["stable_id"], site["stable_id"])
            self.assertNotEqual(renamed_site["source_locator_id"], old_locator)

    def test_invalid_utf8_and_unencodable_literals_are_blocked(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "src" / "main").mkdir(parents=True)
            (root / "src" / "ovl").mkdir()
            (root / "src" / "main" / "bad.c").write_bytes(b'void f(){ const char *s = "\xff"; }')
            (root / "src" / "main" / "bad_literal.c").write_text('void f(){ const char *s = "🚀"; }')
            _, blocked, _ = _c_literals(root / "src")
            self.assertEqual(len(blocked), 2)
            self.assertEqual({item["status"] for item in blocked}, {"invalid_utf8", "literal_decode_failed"})

    def test_replacement_character_is_blocked(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / "src" / "main").mkdir(parents=True)
            (root / "src" / "ovl").mkdir()
            (root / "src" / "main" / "replacement.c").write_text('void f(){ const char *s = "�"; }')
            _, blocked, _ = _c_literals(root / "src")
            self.assertEqual(blocked[0]["status"], "utf8_roundtrip_failed")

    def test_missing_original_is_blocked_without_guessing(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            (repo / "src" / "main").mkdir(parents=True)
            (repo / "src" / "ovl").mkdir()
            (repo / "asm" / "data" / "main").mkdir(parents=True)
            (repo / "asm" / "ovl").mkdir(parents=True)
            (repo / "config").mkdir()
            (repo / "config" / "overlays.txt").write_text("")
            (repo / "asm" / "data" / "main" / "80041000.rodata.s").write_text(
                'dlabel D_80041010\n    /* 10 80041010 */ .asciz "HI"\n')
            inv = inventory_source_text(repo)
            self.assertEqual(inv["coverage"]["blocked_extents"], 1)
            self.assertEqual(inv["blocked"][0]["status"], "source_binary_missing")

    def test_original_symlink_escape_is_rejected_before_read(self):
        with tempfile.TemporaryDirectory() as temp, tempfile.TemporaryDirectory() as outside:
            repo = Path(temp)
            (repo / "src" / "main").mkdir(parents=True)
            (repo / "src" / "ovl").mkdir()
            (repo / "asm" / "data" / "main").mkdir(parents=True)
            (repo / "asm" / "ovl").mkdir(parents=True)
            (repo / "config").mkdir()
            (repo / "config" / "overlays.txt").write_text("")
            files = repo / "disc" / "files"
            files.mkdir(parents=True)
            external = Path(outside) / "foreign.bin"
            external.write_bytes(b"invented")
            (files / "SLPM_86.053").symlink_to(external)
            inv = inventory_source_text(repo)
            self.assertFalse(inv["sources"]["main_binary_available"])
            self.assertIn("source_input_escapes_root", [item["status"] for item in inv["blocked"]])


if __name__ == "__main__":
    unittest.main()
