"""Invented runtime text patch contracts and opaque write candidates (T-28001)."""
import hashlib
import tempfile
import unittest
from pathlib import Path

from tools.assets.script_mutations import enrich_script_mutations, inventory_mutation_candidates
from tools.assets.text import parse_script
from tools.test_asset_text import BASE, fixture


class MutationTests(unittest.TestCase):
    def fixture(self, root, raw=b"ABCD"):
        data, profile = fixture(raw=raw)
        source_path = "src/ovl/TEST/unit.c"
        source = ("typedef unsigned char u8;\nint text, command, voice, active, flag;\n"
                  f"void init(void) {{ text=0x{BASE:X}; command=0x{BASE + 4:X}; voice=0x{BASE + 8:X}; }}\n"
                  "void select_root(void) { active = text; }\n"
                  "void patch(void) {\n  u8 buf[3] = \"XY\";\n  if (flag == 0) {\n"
                  "    ((u8 ***)active)[0][0][0] = buf[0];\n"
                  "    ((u8 ***)active)[0][0][1] = buf[1];\n  }\n}\n")
        path = root / source_path
        path.parent.mkdir(parents=True)
        path.write_text(source)
        (root / "src/main").mkdir(parents=True)
        inputs = root / "disc/files"
        original = inputs / profile["path"]
        original.parent.mkdir(parents=True)
        original.write_bytes(data)
        catalog = {"files": [{"path": profile["path"], "script": parse_script(data, profile["path"], profile)}],
                   "source_inventory": {"c_literal_sites": [{"id": "c:invented:6:1", "raw_hex": "5859", "raw_length": 2,
                       "source_site": {"path": source_path, "line": 6, "function": "patch", "array_symbol": "buf", "array_length": 3}}]}}
        spec = {"id": "invented-patch", "source": source_path, "function": "patch", "array": "buf", "length": 2,
                "write_lines": [8, 9], "script": profile["path"], "initializer_source": source_path,
                "initializer": "init", "indices": [0], "group": 0, "line": 0,
                "conditions": ["flag == 0"], "selection": "explicit_fixed_root", "pins": [source_path]}
        pins = {source_path: hashlib.sha256(source.encode()).hexdigest()}
        return catalog, spec, pins, inputs

    def run_patch(self, root, catalog, spec, pins, inputs):
        return enrich_script_mutations(catalog, root, inputs, specs=[spec], source_pins=pins,
                                       scan_candidates=False)["runtime_text_mutations"]

    def test_source_literal_and_pointer_tables_prove_slot_without_writing(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root)
            original = inputs / spec["script"]
            before = original.read_bytes()
            report = self.run_patch(root, catalog, spec, pins, inputs)
            self.assertEqual(report["blocked"], [])
            rule = report["rules"][0]
            self.assertEqual(rule["replacement"]["raw_hex"], "5859")
            self.assertEqual(rule["replacement"]["c_literal_site_id"], "c:invented:6:1")
            target = rule["targets"][0]
            self.assertEqual(target["offset"], 96)
            self.assertEqual(target["original_raw_hex"], "4142")
            self.assertTrue(target["write_fits_exported_extent"])
            self.assertEqual(len(target["pointer_evidence"]), 2)
            self.assertEqual(target["conditional_runtime_view"]["text"]["raw_hex"], "58594344")
            self.assertEqual(catalog["files"][0]["script"]["records"][0]["text"]["raw_hex"], "41424344")
            self.assertEqual(original.read_bytes(), before)

    def test_terminating_and_command_patches_are_explicit(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root)
            spec.update(length=3, command_value=7)
            rule = self.run_patch(root, catalog, spec, pins, inputs)["rules"][0]
            self.assertTrue(rule["replacement"]["includes_terminal_nul"])
            self.assertEqual(rule["replacement"]["raw_hex"], "585900")
            self.assertEqual(rule["targets"][0]["command_patch"]["replacement_value"], 7)
            view = rule["targets"][0]["conditional_runtime_view"]
            self.assertEqual(view["text"]["raw_hex"], "5859")
            self.assertEqual(view["ignored_storage_after_nul_hex"], "4400")

    def test_changed_source_or_forged_replacement_is_blocked(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root)
            pins[spec["source"]] = "0" * 64
            report = self.run_patch(root, catalog, spec, pins, inputs)
            self.assertEqual(report["rules"][0]["status"], "unresolved_mutation")
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root)
            catalog["source_inventory"]["c_literal_sites"][0]["raw_hex"] = "5a5a"
            report = self.run_patch(root, catalog, spec, pins, inputs)
            self.assertEqual(report["rules"][0]["targets"], [])

    def test_changed_original_and_wrong_target_group_are_blocked(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root)
            original = inputs / spec["script"]
            data = bytearray(original.read_bytes())
            data[400] = 1
            original.write_bytes(data)
            self.assertTrue(self.run_patch(root, catalog, spec, pins, inputs)["blocked"])
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root)
            spec["group"] = 99
            self.assertTrue(self.run_patch(root, catalog, spec, pins, inputs)["blocked"])

    def test_storage_after_early_nul_is_not_assumed_allocated(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root, raw=b"")
            report = self.run_patch(root, catalog, spec, pins, inputs)
            self.assertIn("target_allocation_capacity_unproved", [x["status"] for x in report["blocked"]])

    def test_command_patch_requires_actual_command_table_pointer(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root)
            spec["command_value"] = 7
            catalog["files"][0]["script"]["records"][0]["command"].update(offset=49, value=255)
            report = self.run_patch(root, catalog, spec, pins, inputs)
            self.assertEqual(report["rules"][0]["targets"], [])
            self.assertTrue(report["blocked"])

    def test_root_assignment_union_stays_unresolved(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root)
            spec.pop("indices")
            spec.update(runtime_root="active", selection="source_assignment_union_requires_runtime_selector_proof")
            report = self.run_patch(root, catalog, spec, pins, inputs)
            self.assertEqual(len(report["rules"][0]["targets"]), 1)
            self.assertIn("runtime_root_selection_not_fully_proved", [x["status"] for x in report["blocked"]])
            self.assertNotIn("conditional_runtime_view", report["rules"][0]["targets"][0])

    def test_patch_that_removes_known_terminator_does_not_guess_next_string(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root, raw=b"A")
            report = self.run_patch(root, catalog, spec, pins, inputs)
            self.assertIn("runtime_terminator_outside_exported_extent", [x["status"] for x in report["blocked"]])
            self.assertEqual(report["rules"][0]["targets"][0]["conditional_runtime_view"]["status"], "original_terminator_overwritten")

    def test_ast_candidates_preserve_operators_conditions_and_redact_literals(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            path = root / "src/main/unit.c"
            path.parent.mkdir(parents=True)
            path.write_text("typedef unsigned char u8;\nextern u8 *talk_text_table;\n"
                            "extern u8 name_code_family[16];\nextern int flag;\n"
                            "void sndi(u8 *, int, int);\nvoid strcpy(u8 *, u8 *);\n"
                            "void f(void) { u8 out[16]; u8 *p=(u8 *)talk_text_table;\n"
                            "if(flag) { p[0] |= 1; name_code_family[0] = 2; strcpy(out, \"INVENTED\"); }\n"
                            "sndi(out,0,0); }\n")
            report = inventory_mutation_candidates(root, {"files": []}, files=["src/main/unit.c"])
            self.assertEqual(report["blocked"], [])
            self.assertTrue(any(row["operator"] == "|=" for row in report["candidates"]))
            self.assertTrue(any(row["callee"] == "strcpy" for row in report["candidates"]))
            self.assertTrue(any(row["syntactic_guard_context"] == ["flag"] for row in report["candidates"]))
            self.assertNotIn("INVENTED", str(report))

    def candidate_fixture(self, source):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            path = root / "src/main/unit.c"
            path.parent.mkdir(parents=True)
            path.write_text(source)
            report = inventory_mutation_candidates(root, {"files": []}, files=["src/main/unit.c"])
            self.assertEqual(report["blocked"], [])
            return report

    def test_logical_overlay_folder_uses_its_configured_display_role(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            relative = "src/ovl/rpg_battle/unit.c"
            path = root / relative
            path.parent.mkdir(parents=True)
            path.write_text("typedef unsigned char u8;\n"
                            "void rpg_label_create(int, int, u8 *, int);\n"
                            "void f(void) { u8 text[8]; text[0] = 1; rpg_label_create(0,0,text,0); }\n")
            config = root / "config/overlays/RPG_BAT.yaml"
            config.parent.mkdir(parents=True)
            config.write_text("options:\n  basename: RPG_BAT\n  src_path: src/ovl\n"
                              "segments:\n  - [0x0, c, rpg_battle/unit]\n")
            report = inventory_mutation_candidates(root, {"files": []}, files=[relative])
            self.assertEqual(report["blocked"], [])
            self.assertEqual(len(report["candidates"]), 1)
            self.assertEqual(report["candidates"][0]["operator"], "=")
            config.unlink()
            wrong = config.with_name("SHOUGATU.yaml")
            wrong.write_text("options:\n  basename: SHOUGATU\n  src_path: src/ovl\n"
                             "segments:\n  - [0x0, c, rpg_battle/unit]\n")
            report = inventory_mutation_candidates(root, {"files": []}, files=[relative])
            self.assertEqual(report["blocked"], [])
            self.assertEqual(report["candidates"], [])

    def test_array_selectors_and_member_tokens_do_not_seed_aliases(self):
        report = self.candidate_fixture(
            "typedef unsigned char u8; extern u8 **table; extern u8 *other;\n"
            "extern int i,j,flag,text; struct Box {u8 *text;}; extern struct Box box;\n"
            "void sndi(u8*,int,int);\nvoid f(void) {\n"
            "u8 *p=table[i]; u8 *q=flag?table[i]:table[j];\n"
            "i++; j++; flag++; text++; other[i]=3; p[0]=1; q[0]=2; box.text[0]=4;\n"
            "sndi(table[i],0,0); sndi(box.text,0,0); }\n")
        destinations = {row["destination_expression"] for row in report["candidates"]}
        self.assertFalse(destinations & {"i", "j", "flag", "text", "other[i]"})
        self.assertTrue({"p[0]", "q[0]", "box.text[0]"} <= destinations)

    def test_opaque_helper_returns_and_all_copy_calls_remain_unresolved(self):
        report = self.candidate_fixture(
            "typedef unsigned char u8; extern u8 **table; extern int i; extern u8 *out;\n"
            "void sndi(u8*,int,int); u8 *opaque(u8*); u8 *numeric_helper(int);\n"
            "void strcpy(u8*,u8*);\nvoid f(void) {\n"
            "u8 *p=table[i]; u8 *q=opaque(p); q[0]=1; i++;\n"
            "sndi(table[i],0,0); sndi(numeric_helper(i),0,0); strcpy(out,p); }\n")
        self.assertTrue(any(row["destination_expression"] == "q[0]" for row in report["candidates"]))
        self.assertFalse(any(row["destination_expression"] == "i" for row in report["candidates"]))
        self.assertTrue(any(row["callee"] == "strcpy" for row in report["candidates"]))
        opaque = report["opaque_value_dependencies"]
        self.assertEqual({row["role"] for row in opaque}, {"initializer_value", "display_argument"})
        self.assertTrue(any(row["destination_expression"] == "q" for row in opaque))
        self.assertTrue(all(row["status"] == "opaque_return_value_not_proven_alias" for row in opaque))

    def test_comma_value_uses_final_operand_and_subscript_calls_are_still_visited(self):
        report = self.candidate_fixture(
            "typedef unsigned char u8; extern u8 *text_buffer; extern int i;\n"
            "void sndi(u8*,int,int); void strcpy(u8*,u8*);\nvoid f(void) {\n"
            "u8 *q=(i,text_buffer); i++; q[0]=1;\n"
            "sndi((i,text_buffer),0,0); text_buffer[(strcpy(text_buffer,text_buffer),i)]=2; }\n")
        self.assertFalse(any(row["destination_expression"] == "i" for row in report["candidates"]))
        self.assertTrue(any(row["destination_expression"] == "q[0]" for row in report["candidates"]))
        self.assertEqual(sum(row["callee"] == "strcpy" for row in report["candidates"]), 1)

    def test_opaque_write_base_retained_without_aliasing_its_selector(self):
        report = self.candidate_fixture(
            "typedef unsigned char u8; extern u8 *text_buffer,*other; extern int i;\n"
            "u8 *opaque(u8*); int selector(u8*); void sndi(u8*,int,int);\n"
            "void f(void) { sndi(text_buffer,0,0); opaque(text_buffer)[0]=1;\n"
            "opaque(text_buffer)[1]++; other[selector(text_buffer)]=2; }\n")
        destinations = {row["destination_expression"] for row in report["candidates"]}
        self.assertTrue({"opaque(text_buffer)[0]", "opaque(text_buffer)[1]"} <= destinations)
        self.assertNotIn("other[selector(text_buffer)]", destinations)
        opaque = report["opaque_value_dependencies"]
        self.assertEqual(sum(row["role"] == "write_destination" for row in opaque), 3)

    def test_text_derived_selectors_are_explicit_unresolved_address_dependencies(self):
        report = self.candidate_fixture(
            "typedef unsigned char u8; extern u8 *text_ptr,*other; extern int i,j;\n"
            "void sndi(u8*,int,int); void f(void) {\n"
            "i=(int)text_ptr; sndi(text_ptr,0,0); other[(int)text_ptr]=1;\n"
            "other[i]=2; other[j]=3; other[i]++; }\n")
        destinations = {row["destination_expression"] for row in report["candidates"]}
        self.assertFalse(destinations & {"other[(int) text_ptr]", "other[i]", "other[j]"})
        rows = report["unmodeled_address_dependencies"]
        self.assertEqual(len(rows), 3)
        self.assertEqual({tuple(row["text_derived_selector_symbols"]) for row in rows}, {("text_ptr",), ("i",)})
        self.assertTrue(all(row["status"] == "unmodeled_address_dependency_not_proven_text_write" for row in rows))

    def test_opaque_and_address_counts_are_exposed_in_completeness(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            path = root / "src/main/unit.c"
            path.parent.mkdir(parents=True)
            path.write_text("typedef unsigned char u8; extern u8 *text_ptr,*other;\n"
                            "u8 *opaque(u8*); void sndi(u8*,int,int);\n"
                            "void f(void) { sndi(text_ptr,0,0); sndi(opaque(text_ptr),0,0);\n"
                            "other[(int)text_ptr]=1; }\n")
            catalog = {"files": []}
            result = enrich_script_mutations(catalog, root, root, specs=[], source_pins={},
                                             candidate_files=["src/main/unit.c"])["runtime_text_mutations"]
            self.assertEqual(result["coverage"]["opaque_value_dependencies"], 1)
            self.assertEqual(result["coverage"]["unmodeled_address_dependencies"], 1)
            self.assertEqual(result["completeness"]["unresolved_opaque_dependencies"], 1)
            self.assertEqual(result["completeness"]["unresolved_address_dependencies"], 1)
            self.assertFalse(result["completeness"]["complete"])

    def selector_fixture(self, root, spec):
        base = 0x80041000
        code = bytes.fromhex("0800e00300000000")
        binary = bytearray(0x820)
        binary[0x800:0x808] = code
        binary[0x810:0x814] = base.to_bytes(4, "little")
        original = root / "disc/files/SLPM_86.053"
        original.write_bytes(binary)
        asm_path = "asm/matchings/main/minimal.s"
        table_path = "asm/data/minimal.data.s"
        for path, text in ((asm_path, "glabel minimal\n/* 0 80041000 0800E003 */ jr $ra\n"
                          "/* 4 80041004 00000000 */ nop\nendlabel minimal\n"),
                           (table_path, "dlabel table\n/* 10 80041010 00100480 */ .word minimal\nenddlabel table\n")):
            target = root / path
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text(text)
        spec["selection"] = "source_proved_conditional_stage2_route"
        spec["selector_contract"] = {
            "indices": [0], "source_pins": {}, "binary_pins": {"SLPM_86.053": hashlib.sha256(binary).hexdigest()},
            "units": {"SLPM_86.053": {"load_address": base, "file_bias": 0x800}},
            "assembly_pins": {asm_path: hashlib.sha256((root / asm_path).read_bytes()).hexdigest()},
            "dispatch_source": "SLPM_86.053", "data_assembly": {
                "path": table_path, "sha256": hashlib.sha256((root / table_path).read_bytes()).hexdigest(),
                "label": "table", "address": base + 16, "word_count": 1,
                "raw_sha256": hashlib.sha256(base.to_bytes(4, "little")).hexdigest(), "required_slots": {0: base}},
            "assumptions": ["invented conditional route"], "excluded_roots": {1: "not on invented route"},
            "root_conditions": {0: ["invented selector == 0"]}}

    def test_conditional_selector_contract_includes_original_word_proofs_and_conditions(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            catalog, spec, pins, inputs = self.fixture(root)
            self.selector_fixture(root, spec)
            result = self.run_patch(root, catalog, spec, pins, inputs)
            self.assertEqual(result["blocked"], [])
            rule = result["rules"][0]
            self.assertFalse(rule["selector_route"]["runtime_reachability_proven"])
            self.assertEqual(rule["selector_route"]["dispatch"]["words"], [0x80041000])
            self.assertEqual(len(rule["selector_route"]["function_instruction_evidence"][0]["instructions"]), 2)
            self.assertIn("invented conditional route", rule["targets"][0]["conditional_runtime_view"]["conditions"])
            self.assertIn("invented selector == 0", rule["targets"][0]["conditional_runtime_view"]["conditions"])

    def test_selector_contract_rejects_changed_roots_code_dispatch_and_source(self):
        for problem in ("roots", "binary", "instruction", "dispatch", "dispatch_digest", "dispatch_bytes", "source"):
            with self.subTest(problem=problem), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                catalog, spec, pins, inputs = self.fixture(root)
                self.selector_fixture(root, spec)
                contract = spec["selector_contract"]
                if problem == "roots":
                    spec["indices"] = [1]
                elif problem == "binary":
                    contract["binary_pins"]["SLPM_86.053"] = "0" * 64
                elif problem == "instruction":
                    path = root / next(iter(contract["assembly_pins"]))
                    path.write_text(path.read_text().replace("0800E003", "00000000"))
                    contract["assembly_pins"][path.relative_to(root).as_posix()] = hashlib.sha256(path.read_bytes()).hexdigest()
                elif problem == "dispatch":
                    contract["data_assembly"]["required_slots"] = {0: 0}
                elif problem == "dispatch_digest":
                    contract["data_assembly"]["raw_sha256"] = "0" * 64
                elif problem == "dispatch_bytes":
                    path = inputs / "SLPM_86.053"
                    data = bytearray(path.read_bytes())
                    data[0x810] ^= 1
                    path.write_bytes(data)
                    contract["binary_pins"]["SLPM_86.053"] = hashlib.sha256(data).hexdigest()
                else:
                    contract["source_pins"][spec["source"]] = "0" * 64
                result = self.run_patch(root, catalog, spec, pins, inputs)
                self.assertTrue(result["blocked"])
                self.assertEqual(result["rules"][0]["targets"], [])


if __name__ == "__main__":
    unittest.main()
