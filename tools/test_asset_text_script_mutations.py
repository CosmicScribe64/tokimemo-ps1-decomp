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


if __name__ == "__main__":
    unittest.main()
