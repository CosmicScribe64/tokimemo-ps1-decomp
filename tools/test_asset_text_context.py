"""Invented fixtures for source-derived pointer-table context."""
import hashlib
import tempfile
import unittest
from pathlib import Path

from tools.assets.text_context import (ContextError, _apply_ast_facts, _c_symbol_uses, _role,
                                       _table_graph, enrich_source_context)


class SourceContextTests(unittest.TestCase):
    def test_ast_gate_requires_typed_pointer_flow_and_uses_ast_selectors(self):
        uses = [{"path": "src/main/use.c", "line": 44, "call_line": 42,
                 "call_column": 9, "call": "sndi", "call_argument_index": 0,
                 "symbol": "D_TEXT", "selectors": [0, 99]}]
        ast = {"files": [{"path": "src/main/use.c", "calls": [{
            "line": 42, "column": 9, "callee": "sndi", "argument_index": 0,
            "argument_role": "display", "candidate_symbols": ["D_TEXT"],
            "value_flow_status": "source_pointer_candidate",
            "symbol_facts": [{"symbol": "D_TEXT", "pointer_value_flows_to_argument": True,
                              "dereference_count": 2}],
            "selectors": [{"base_symbols": ["D_TEXT"], "expression_use": "argument_value",
                           "dereference_count": 2,
                           "selectors": [{"constant": 0}, {"constant": 1}]}],
        }]}]}
        accepted, rejected = _apply_ast_facts(uses, ast)
        self.assertEqual(rejected, 0)
        self.assertEqual(accepted[0]["selectors"], [0, 1])
        self.assertEqual(accepted[0]["ast_proof"], "typed_pointer_value_flow_to_known_read_argument")
        uses[0]["symbol"] = "D_INDEX"
        accepted, rejected = _apply_ast_facts(uses, ast)
        self.assertEqual((accepted, rejected), ([], 1))

    def test_unary_dereference_without_selector_metadata_is_rejected(self):
        use = {"path": "src/main/use.c", "line": 10, "call_line": 10,
               "call_column": 4, "call": "sndi", "call_argument_index": 0,
               "symbol": "D_TEXT"}
        ast = {"files": [{"path": "src/main/use.c", "calls": [{
            "line": 10, "column": 4, "callee": "sndi", "argument_index": 0,
            "argument_role": "display", "candidate_symbols": ["D_TEXT"],
            "symbol_facts": [{"symbol": "D_TEXT", "pointer_value_flows_to_argument": True,
                              "dereference_count": 1, "occurrence_role": "pointer_base"}],
            "selectors": [],
        }]}]}
        accepted, rejected = _apply_ast_facts([use], ast)
        self.assertEqual((accepted, rejected), ([], 1))

    def test_repeated_symbol_index_occurrence_cannot_supply_value_selectors(self):
        use = {"path": "src/main/use.c", "line": 12, "call_line": 12,
               "call_column": 4, "call": "sndi", "call_argument_index": 0,
               "symbol": "D_TEXT"}
        ast = {"files": [{"path": "src/main/use.c", "calls": [{
            "line": 12, "column": 4, "callee": "sndi", "argument_index": 0,
            "argument_role": "display", "candidate_symbols": ["D_TEXT"],
            "symbol_facts": [
                {"symbol": "D_TEXT", "pointer_value_flows_to_argument": True,
                 "dereference_count": 1, "occurrence_role": "pointer_base"},
                {"symbol": "D_TEXT", "pointer_value_flows_to_argument": False,
                 "dereference_count": 0, "occurrence_role": "index_operand"},
            ],
            "selectors": [
                {"base_symbols": ["D_TEXT"], "expression_use": "selector_expression",
                 "dereference_count": 1, "selectors": [{"constant": 99}]},
                {"base_symbols": ["D_TEXT"], "expression_use": "argument_value",
                 "dereference_count": 1, "selectors": [{"constant": 0}]},
            ],
        }]}]}
        accepted, rejected = _apply_ast_facts([use], ast)
        self.assertEqual(rejected, 0)
        self.assertEqual(accepted[0]["selectors"], [0])

    def test_roles_require_known_read_arguments(self):
        self.assertEqual(_role("get_g_name", "render", 0), (None, None))
        self.assertEqual(_role("sprintf", "render", 0), (None, None))
        self.assertEqual(_role("sprintf", "render", 1)[0], "debug_or_format")
        self.assertEqual(_role("set_kanji_string", "render", 3)[0], "display")
        self.assertEqual(_role("set_string", "render", 0), (None, None))

    def _repo(self, root: Path):
        (root / "src" / "main").mkdir(parents=True)
        (root / "src" / "ovl").mkdir()
        (root / "asm" / "data" / "main").mkdir(parents=True)
        (root / "asm" / "ovl").mkdir(parents=True)
        (root / "config").mkdir()
        (root / "config" / "overlays.txt").write_text("")
        files = root / "disc" / "files"
        files.mkdir(parents=True)
        (files / "SLPM_86.053").write_bytes(bytes(0x2000))

    def _put_main_word(self, root: Path, address: int, target: int):
        path = root / "disc" / "files" / "SLPM_86.053"
        data = bytearray(path.read_bytes())
        offset = address - 0x80041000 + 0x800
        data[offset:offset + 4] = target.to_bytes(4, "little")
        path.write_bytes(data)

    def _text_record(self, root: Path, record_id: str, binary_path: str,
                     address: int, raw: bytes, label: str):
        files = root / "disc" / "files"
        path = files / binary_path
        if binary_path == "SLPM_86.053":
            offset = address - 0x80041000 + 0x800
        else:
            unit = Path(binary_path).stem
            bases = {line.split()[0]: int(line.split()[1], 0)
                     for line in (root / "config" / "overlays.txt").read_text().splitlines() if line.strip()}
            offset = address - bases[unit]
        data = bytearray(path.read_bytes())
        data[offset:offset + len(raw)] = raw
        path.write_bytes(data)
        return {"id": record_id, "category": "unknown", "raw_hex": raw.hex(),
                "raw_length": len(raw), "extent_status": "verified",
                "source": {"path": binary_path, "offset": offset, "address": address,
                           "length": len(raw), "end_offset_exclusive": offset + len(raw),
                           "file_size": len(data), "sha256": hashlib.sha256(data).hexdigest()},
                "assembly_sites": [{"labels": [label]}]}

    def test_multilevel_word_tables_reach_only_same_unit_text_and_consumer(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            (repo / "src" / "main" / "use.c").write_text(
                "extern const char *outer_table[];\n"
                "void render(void) { sndi(outer_table[0][0], 0, 0); "
                "sndi(0, outer_table[0][1], 0); "
                "sndi(outer_table[0][1], 0, 0); }\n")
            (repo / "asm" / "data" / "main" / "tables.s").write_text(
                "dlabel outer_table\n"
                "    /* 10 80041010 14100480 */ .word inner_table\n"
                "dlabel inner_table\n"
                "    /* 14 80041014 00200480 */ .word D_TEXT\n"
                "    /* 18 80041018 20200480 */ .word D_OTHER\n")
            self._put_main_word(repo, 0x80041010, 0x80041014)
            self._put_main_word(repo, 0x80041014, 0x80042000)
            self._put_main_word(repo, 0x80041018, 0x80042020)
            inventory = {"records": [
                self._text_record(repo, "main-text", "SLPM_86.053", 0x80042000, b"TEXT\0", "D_TEXT"),
                self._text_record(repo, "other-text", "SLPM_86.053", 0x80042020, b"OTHER\0", "D_OTHER"),
            ]}
            binary = (repo / "disc" / "files" / "SLPM_86.053").read_bytes()
            for record in inventory["records"]:
                record["source"]["sha256"] = hashlib.sha256(binary).hexdigest()
                record["source"]["file_size"] = len(binary)
            edges, _, _ = _table_graph(repo, inventory["records"], repo / "disc" / "files")
            table_uses, _ = _c_symbol_uses(repo, set(edges) | {("main", "inner_table")})
            self.assertEqual([use["selectors"] for use in table_uses if use["symbol"] == "outer_table"],
                             [[0, 0], [0, 1], [0, 1]])
            result = enrich_source_context(inventory, repo, repo / "disc" / "files")
            self.assertGreater(result["source_context"]["pointer_table_edges"], 0)
            self.assertGreater(result["source_context"]["C_symbol_uses"], 0, repr(result["source_context"]))
            records = {record["id"]: record for record in result["records"]}
            record = records["main-text"]
            self.assertIsNone(record["context_category"], repr(record["source_contexts"]))
            self.assertNotEqual(record["category"], "display")
            context = next(c for c in record["source_contexts"] if c["source_site"]["call"])
            self.assertEqual(context["source_site"]["call"], "sndi")
            self.assertEqual(context["source_site"]["call_argument_index"], 0)
            self.assertEqual(context["source_site"]["function"], "render")
            self.assertEqual([edge["table"] for edge in context["evidence_chain"]],
                             ["outer_table", "inner_table"])
            self.assertEqual(context["proof"], "same_link_unit_word_pointer_targets_verified_text_label")
            self.assertFalse(any(c["source_site"]["call_argument_index"] == 1
                                 for c in record["source_contexts"]))
            other_contexts = records["other-text"]["source_contexts"]
            self.assertTrue(any(c["category_candidate"] == "display" and c["source_site"]["call_argument_index"] == 0
                                for c in other_contexts))
            self.assertTrue(any(c["category"] is None and c["source_site"]["call_argument_index"] == 1
                                for c in other_contexts))

    def test_global_pointer_load_selects_only_first_word_before_explicit_index(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            (repo / "src" / "main" / "use.c").write_text(
                "extern const char **D_PTR;\nvoid render(void) { sndi(D_PTR[0], 0, 0); }\n")
            (repo / "asm" / "data" / "main" / "tables.s").write_text(
                "dlabel D_PTR\n"
                "    /* 10 80041010 18100480 */ .word D_TABLE\n"
                "    /* 14 80041014 1c100480 */ .word D_OTHER_TABLE\n"
                "dlabel D_TABLE\n"
                "    /* 18 80041018 00200480 */ .word D_TEXT\n"
                "dlabel D_OTHER_TABLE\n"
                "    /* 1c 8004101C 20200480 */ .word D_OTHER_TEXT\n")
            for address, target in ((0x80041010, 0x80041018),
                                    (0x80041014, 0x8004101C),
                                    (0x80041018, 0x80042000),
                                    (0x8004101C, 0x80042020)):
                self._put_main_word(repo, address, target)
            records = [
                self._text_record(repo, "first", "SLPM_86.053", 0x80042000, b"FIRST\0", "D_TEXT"),
                self._text_record(repo, "second", "SLPM_86.053", 0x80042020, b"SECOND\0", "D_OTHER_TEXT"),
            ]
            binary = (repo / "disc" / "files" / "SLPM_86.053").read_bytes()
            for record in records:
                record["source"]["sha256"] = hashlib.sha256(binary).hexdigest()
                record["source"]["file_size"] = len(binary)
            result = enrich_source_context({"records": records}, repo,
                                           repo / "disc" / "files", use_ast=True)
            by_id = {record["id"]: record for record in result["records"]}
            self.assertTrue(result["source_context"]["AST_value_flow_gated"])
            self.assertEqual(by_id["first"]["context_category"], "display")
            self.assertEqual(by_id["second"]["source_contexts"], [])

    def test_revalidation_revokes_context_owned_display_category(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            source = repo / "src" / "main" / "use.c"
            source.write_text("extern const char D_TEXT[];\n"
                              "void render(void) { sndisp(D_TEXT, 0, 0); }\n")
            (repo / "asm" / "data" / "main" / "text.s").write_text(
                'dlabel D_TEXT\n    /* 1800 80042000 */ .asciz "TEXT"\n')
            record = self._text_record(repo, "text", "SLPM_86.053", 0x80042000,
                                       b"TEXT\0", "D_TEXT")
            result = enrich_source_context({"records": [record]}, repo,
                                          repo / "disc" / "files", use_ast=True)
            self.assertEqual(result["records"][0]["category"], "display")
            self.assertEqual(result["records"][0]["category_basis"],
                             "unique source-proven consumer role")
            source.write_text("extern const char D_TEXT[];\nextern const char **other;\n"
                              "void render(void) { sndisp(other[(int)D_TEXT], 0, 0); }\n")
            result = enrich_source_context(result, repo, repo / "disc" / "files", use_ast=True)
            refreshed = result["records"][0]
            self.assertEqual(refreshed["category"], "unknown")
            self.assertEqual(refreshed["category_basis"],
                             "context-owned category reset before revalidation")
            self.assertIsNone(refreshed["context_category"])

    def test_same_vma_symbol_aliases_do_not_cross_overlay_units(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            (repo / "src" / "ovl" / "A").mkdir(parents=True)
            (repo / "src" / "ovl" / "B").mkdir(parents=True)
            (repo / "src" / "ovl" / "A" / "use.c").write_text(
                "extern const char *table[];\nvoid f(void) { sndisp(table[0], 0, 0); }\n")
            (repo / "config" / "overlays.txt").write_text("A 0x80132000 0x40\nB 0x80132000 0x40\n")
            files = repo / "disc" / "files" / "CDROM" / "EXEDIR"
            files.mkdir(parents=True)
            for unit, byte in (("A", "A"), ("B", "B")):
                binary = bytearray(0x40)
                binary[0x10:0x14] = (0x80132020).to_bytes(4, "little")
                (files / f"{unit}.EXN").write_bytes(binary)
                (repo / "asm" / "ovl" / unit / "data" / unit).mkdir(parents=True)
                (repo / "asm" / "ovl" / unit / "data" / unit / "table.s").write_text(
                    "dlabel table\n    /* 10 80132010 20201380 */ .word D_TEXT\n")
            inventory = {"records": [
                self._text_record(repo, "A-text", "CDROM/EXEDIR/A.EXN", 0x80132020, b"A\0", "D_TEXT"),
                self._text_record(repo, "B-text", "CDROM/EXEDIR/B.EXN", 0x80132020, b"B\0", "D_TEXT"),
            ]}
            result = enrich_source_context(inventory, repo, repo / "disc" / "files")
            by_id = {record["id"]: record for record in result["records"]}
            display_contexts = [c for c in by_id["A-text"]["source_contexts"]
                                if c["category_candidate"] == "display"]
            self.assertEqual(len(display_contexts), 1)
            self.assertEqual(by_id["B-text"]["source_contexts"], [])

    def test_mismatched_encoded_pointer_is_not_a_reference(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            (repo / "src" / "main" / "use.c").write_text(
                "extern const char *table[];\nvoid f(void) { sndi(table[0], 0, 0); }\n")
            (repo / "asm" / "data" / "main" / "table.s").write_text(
                "dlabel table\n    /* 10 80041010 00200480 */ .word D_TEXT\n")
            self._put_main_word(repo, 0x80041010, 0x80042000)
            inventory = {"records": [self._text_record(repo, "text", "SLPM_86.053",
                                                       0x80042004, b"T\0", "D_TEXT")]}
            result = enrich_source_context(inventory, repo, repo / "disc" / "files")
            self.assertEqual(result["records"][0]["source_contexts"], [])
            self.assertEqual(result["source_context"]["records_without_proven_consumer_context"], 1)

    def test_text_target_before_selector_path_is_exhausted_is_not_context(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            (repo / "src" / "main" / "use.c").write_text(
                "extern const char **table;\n"
                "void render(void) { sndi(table[0][1], 0, 0); }\n")
            (repo / "asm" / "data" / "main" / "table.s").write_text(
                "dlabel table\n    /* 10 80041010 00200480 */ .word D_TEXT\n")
            self._put_main_word(repo, 0x80041010, 0x80042000)
            record = self._text_record(repo, "text", "SLPM_86.053", 0x80042000,
                                       b"TEXT\0", "D_TEXT")
            result = enrich_source_context({"records": [record]}, repo, repo / "disc" / "files")
            self.assertEqual(result["records"][0]["source_contexts"], [])
            self.assertIsNone(result["records"][0]["context_category"])

    def test_inline_array_row_does_not_map_to_base_label_address(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            (repo / "src" / "main" / "use.c").write_text(
                "extern char rows[2][8];\n"
                "void render(void) { sndi(rows[1], 0, 0); }\n")
            record = self._text_record(repo, "row-zero", "SLPM_86.053", 0x80042000,
                                       b"ROWZERO\0", "rows")
            result = enrich_source_context({"records": [record]}, repo, repo / "disc" / "files")
            self.assertEqual(result["records"][0]["source_contexts"], [])
            self.assertIsNone(result["records"][0]["context_category"])

    def test_direct_C_text_label_call_gets_consumer_context(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            (repo / "src" / "main" / "use.c").write_text(
                "void render(void) { sndisp(D_TEXT, 0, 0); sndisp(0, D_TEXT, 0); }\n")
            inventory = {"records": [self._text_record(repo, "direct-text", "SLPM_86.053",
                                                       0x80042000, b"TEXT\0", "D_TEXT")]}
            result = enrich_source_context(inventory, repo, repo / "disc" / "files")
            record = result["records"][0]
            self.assertIsNone(record["context_category"])
            self.assertEqual(record["source_contexts"][0]["category_candidate"], "display")
            self.assertEqual(record["source_contexts"][0]["source_site"]["call"], "sndisp")
            self.assertIn("path", record["source_contexts"][0]["evidence_chain"][0])
            self.assertTrue(any(context["category"] is None and
                                context["source_site"]["call_argument_index"] == 1
                                for context in record["source_contexts"]))

    def test_fresh_ast_gate_classifies_exact_label_but_blocks_interior_pointer(self):
        from tools.assets.text_ast import source_input_fingerprint
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            code = "void render(void) { sndisp(D_TEXT, 0, 0); sndisp(&D_INTERIOR[1], 0, 0); }\n"
            (repo / "src" / "main" / "use.c").write_text(code)
            records = [
                self._text_record(repo, "direct", "SLPM_86.053", 0x80042000, b"TEXT\0", "D_TEXT"),
                self._text_record(repo, "interior", "SLPM_86.053", 0x80042020, b"TEXT\0", "D_INTERIOR"),
            ]
            binary = (repo / "disc" / "files" / "SLPM_86.053").read_bytes()
            for record in records:
                record["source"]["sha256"] = hashlib.sha256(binary).hexdigest()
                record["source"]["file_size"] = len(binary)
            facts = []
            for symbol, call_offset, interior in (
                    ("D_TEXT", code.index("sndisp"), False),
                    ("D_INTERIOR", code.index("sndisp", code.index("sndisp") + 1), True)):
                facts.append({"source_path": "src/main/use.c", "line": 1,
                              "column": call_offset + 1, "callee": "sndisp", "argument_index": 0,
                              "argument_role": "display", "candidate_symbols": [symbol],
                              "value_flow_status": "source_pointer_candidate",
                              "symbol_facts": [{"symbol": symbol, "pointer_value_flows_to_argument": True,
                                                "occurrence_role": "value", "address_of": interior,
                                                "dereference_count": 1 if interior else 0,
                                                "global_value_load_words": 0}],
                              "selectors": ([{"base_symbols": [symbol], "dereference_count": 1,
                                              "selectors": [{"constant": 1}]}] if interior else [])})
            fingerprint = source_input_fingerprint(repo)
            ast = {"stable_source_inputs": True, "source_inputs_sha256": fingerprint,
                   "source_inputs_sha256_after": fingerprint,
                   "files": [{"path": "src/main/use.c", "calls": facts}]}
            result = enrich_source_context({"records": records}, repo, repo / "disc" / "files",
                                           ast_consumers=ast)
            by_id = {record["id"]: record for record in result["records"]}
            self.assertEqual(by_id["direct"]["context_category"], "display")
            self.assertEqual(by_id["interior"]["source_contexts"], [])

    def test_stale_ast_fingerprint_is_rejected(self):
        from tools.assets.text_ast import source_input_fingerprint
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            source = repo / "src" / "main" / "use.c"
            source.write_text("void f(void) {}\n")
            fingerprint = source_input_fingerprint(repo)
            source.write_text("void f(void) { int changed; }\n")
            stale = {"stable_source_inputs": True, "source_inputs_sha256": fingerprint,
                     "source_inputs_sha256_after": fingerprint, "files": []}
            with self.assertRaises(ContextError):
                enrich_source_context({"records": []}, repo, repo / "disc" / "files",
                                     ast_consumers=stale)

    def test_stale_target_record_hash_is_blocked(self):
        with tempfile.TemporaryDirectory() as temp:
            repo = Path(temp)
            self._repo(repo)
            (repo / "src" / "main" / "use.c").write_text(
                "void render(void) { sndisp(table[0], 0, 0); }\n")
            (repo / "asm" / "data" / "main" / "table.s").write_text(
                "dlabel table\n    /* 10 80041010 00200480 */ .word D_TEXT\n")
            self._put_main_word(repo, 0x80041010, 0x80042000)
            record = self._text_record(repo, "text", "SLPM_86.053", 0x80042000, b"TEXT\0", "D_TEXT")
            record["source"]["sha256"] = "0" * 64
            result = enrich_source_context({"records": [record]}, repo, repo / "disc" / "files")
            self.assertEqual(result["records"][0]["source_contexts"], [])
            self.assertEqual(result["source_context"]["unverified_record_ids"], ["text"])
            self.assertIn("target_text_extent_unverified",
                          {entry["status"] for entry in result["source_context"]["blocked"]})


if __name__ == "__main__":
    unittest.main()
