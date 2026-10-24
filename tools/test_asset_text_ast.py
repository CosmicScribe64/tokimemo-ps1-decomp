"""Unit tests for source-only text-call AST facts (no original data)."""
import hashlib
import tempfile
import unittest
from pathlib import Path

from tools.assets.text_ast import inventory_text_consumers, source_input_fingerprint


class TextAstTests(unittest.TestCase):
    def test_source_input_fingerprint_invalidates_for_source_and_header_edits(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "src/ovl/TEST/unit.c"
            header = root / "include/test.h"
            source.parent.mkdir(parents=True)
            header.parent.mkdir(parents=True)
            header.write_text("extern char *g_text;\n", encoding="utf-8")
            source.write_text('#include "test.h"\nvoid sndi(char *);\nvoid f(void) { sndi(g_text); }\n',
                              encoding="utf-8")
            before = source_input_fingerprint(root)
            result = inventory_text_consumers(root, {"TEST": {"g_text"}}, ["src/ovl/TEST/unit.c"])
            self.assertTrue(result["stable_source_inputs"])
            self.assertEqual(result["source_inputs_sha256"], before)
            self.assertEqual(result["source_inputs_sha256_after"], before)
            self.assertEqual(result["files"][0]["source_sha256"],
                             hashlib.sha256(source.read_bytes()).hexdigest())
            self.assertIsInstance(result["files"][0]["calls"][0]["column"], int)

            header.write_text("/* changed header */\nextern char *g_text;\n", encoding="utf-8")
            after_header = source_input_fingerprint(root)
            self.assertNotEqual(before, after_header)
            source.write_text(source.read_text(encoding="utf-8") + "/* changed source */\n", encoding="utf-8")
            after_source = source_input_fingerprint(root)
            self.assertNotEqual(after_header, after_source)
            changed = inventory_text_consumers(root, {"TEST": {"g_text"}}, ["src/ovl/TEST/unit.c"])
            self.assertTrue(changed["stable_source_inputs"])
            self.assertEqual(changed["source_inputs_sha256"], after_source)

    def test_array_decay_and_pointer_index_facts(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "src/ovl/TEST/unit.c"
            source.parent.mkdir(parents=True)
            source.write_text(
                "typedef int s32;\n"
                "typedef unsigned short u16;\n"
                "extern char *g_table[4][8];\n"
                "extern char *g_pointer;\n"
                "extern char *g_pointer_table[4];\n"
                "extern int g_scalar_table[4][4];\n"
                "extern s32 global_index;\n"
                "typedef struct State { char *name; } State;\n"
                "extern State *g_state_pointer;\n"
                "void sndisp(char *);\n"
                "void set_kanji_string(s32, s32, s32, char *, s32);\n"
                "void f(s32 group, s32 line) {\n"
                "  sndisp(g_state_pointer->name);\n"
                "  sndisp(g_table[\n"
                "    global_index\n"
                "  ][\n"
                "    line\n"
                "  ]);\n"
                "  set_kanji_string(0, 0, 0, g_pointer_table[line], 0);\n"
                "  sndisp(g_pointer);\n"
                "}\n"
                "void shadowed(s32 line) { char *g_pointer;\n"
                "  set_kanji_string(0, 0, 0, g_pointer[line], 0);\n"
                "}\n"
                "void unresolved(s32 line) {\n"
                "  sndisp(g_pointer + line);\n"
                "  sndisp(global_index ? g_pointer : g_pointer_table[line]);\n"
                "  sndisp((s32)g_pointer);\n"
                "  sndisp((u16)g_pointer);\n"
                "  sndisp((s32)(g_pointer + line));\n"
                "  sndisp(g_pointer_table[(s32)g_pointer]);\n"
                "  sndisp((char *)g_scalar_table[0][0]);\n"
                "  sndisp((float)g_pointer);\n"
                "  sndisp((s32)(u16)g_pointer);\n"
                "  sndisp((char *)g_pointer);\n"
                "}\n", encoding="utf-8")
            result = inventory_text_consumers(
                root, {"TEST": {"g_table", "g_pointer", "g_pointer_table", "g_scalar_table", "global_index",
                                "g_state_pointer"}},
                ["src/ovl/TEST/unit.c"])
            file = result["files"][0]
            self.assertEqual(file["status"], "parsed")
            sndisp_calls = [call for call in file["calls"] if call["callee"] == "sndisp"]
            by_call = {call["callee"]: call for call in file["calls"] if call["callee"] != "sndisp"}
            table_call = next(call for call in sndisp_calls if "g_table" in call["candidate_symbols"])
            self.assertEqual(table_call["argument_role"], "display")
            table = table_call["symbol_facts"][0]
            self.assertEqual(table["global_type"]["kind"], "array")
            self.assertEqual(table["dereference_count"], 2)
            self.assertEqual(table["global_value_load_words"], 0)
            self.assertEqual(table["pointer_value_flows_to_argument"], True)
            self.assertEqual(table_call["selectors"][0]["selector_order"], "root_to_leaf")
            self.assertEqual([selector["expression"] for selector in
                              table_call["selectors"][0]["selectors"]],
                             ["global_index", "line"])
            self.assertEqual(table_call["candidate_symbols"], ["g_table"])
            self.assertEqual(table_call["unresolved_candidate_symbols"], ["global_index"])
            pointer_call = next(call for call in sndisp_calls if call["candidate_symbols"] == ["g_pointer"])
            pointer = pointer_call["symbol_facts"][0]
            self.assertEqual(pointer["global_type"]["kind"], "pointer")
            self.assertEqual(pointer["dereference_count"], 0)
            self.assertEqual(pointer["global_value_load_words"], 1)
            self.assertEqual(pointer["pointer_value_flows_to_argument"], True)
            self.assertEqual(pointer_call["candidate_symbols"], ["g_pointer"])
            struct_pointer = next(call for call in sndisp_calls
                                  if call["argument_expression"] == "g_state_pointer->name")
            struct_fact = struct_pointer["symbol_facts"][0]
            self.assertEqual(struct_fact["global_type"]["kind"], "pointer")
            self.assertEqual(struct_fact["dereference_count"], 1)
            self.assertEqual(struct_fact["global_value_load_words"], 1)
            self.assertEqual(struct_pointer["candidate_symbols"], ["g_state_pointer"])
            pointer_table = by_call["set_kanji_string"]["symbol_facts"][0]
            self.assertEqual(pointer_table["global_type"]["kind"], "array")
            self.assertEqual(pointer_table["dereference_count"], 1)
            self.assertEqual(pointer_table["global_value_load_words"], 0)
            self.assertEqual(pointer_table["pointer_value_flows_to_argument"], True)
            self.assertEqual(sum(call["callee"] == "set_kanji_string" for call in file["calls"]), 1)
            unresolved = [call for call in sndisp_calls if call["source_function"] == "unresolved"]
            arithmetic = next(call for call in unresolved if "+" in call["argument_expression"])
            self.assertEqual(arithmetic["candidate_symbols"], [])
            self.assertEqual(arithmetic["unresolved_candidate_symbols"], ["g_pointer"])
            conditional = next(call for call in unresolved if "?" in call["argument_expression"])
            self.assertEqual(conditional["candidate_symbols"], ["g_pointer", "g_pointer_table"])
            self.assertEqual(conditional["unresolved_candidate_symbols"], ["global_index"])
            ptr_cast = next(call for call in unresolved if "s32" in call["argument_expression"]
                            and "g_pointer" in call["argument_expression"]
                            and "+" not in call["argument_expression"]
                            and "g_pointer_table" not in call["argument_expression"])
            self.assertEqual(ptr_cast["candidate_symbols"], ["g_pointer"])
            self.assertEqual(ptr_cast["argument_expression_type"]["kind"],
                             "pointer_compatible_scalar_argument")
            self.assertEqual(ptr_cast["argument_expression_type"]["bit_width"], 32)
            self.assertTrue(ptr_cast["argument_expression_type"]["source_type_is_not_claimed_as_c_pointer"])
            narrow_cast = next(call for call in unresolved if "u16" in call["argument_expression"]
                               and "g_pointer" in call["argument_expression"])
            self.assertEqual(narrow_cast["candidate_symbols"], [])
            self.assertEqual(narrow_cast["unresolved_candidate_symbols"], ["g_pointer"])
            pointer_index = next(call for call in unresolved
                                 if "g_pointer_table" in call["argument_expression"]
                                 and "s32" in call["argument_expression"])
            self.assertEqual(pointer_index["candidate_symbols"], ["g_pointer_table"])
            self.assertEqual(pointer_index["unresolved_candidate_symbols"], ["g_pointer"])
            scalar_cast = next(call for call in unresolved if "g_scalar_table" in call["argument_expression"])
            self.assertEqual(scalar_cast["argument_expression_type"]["kind"], "cast_argument")
            self.assertEqual(scalar_cast["candidate_symbols"], [])
            self.assertEqual(scalar_cast["unresolved_candidate_symbols"], ["g_scalar_table"])
            self.assertFalse(scalar_cast["symbol_facts"][0]["pointer_value_flows_to_argument"])
            float_cast = next(call for call in unresolved if "float" in call["argument_expression"])
            self.assertEqual(float_cast["argument_expression_type"]["kind"], "cast_argument")
            self.assertEqual(float_cast["candidate_symbols"], [])
            self.assertEqual(float_cast["unresolved_candidate_symbols"], ["g_pointer"])
            narrowed_widened = next(call for call in unresolved if "u16" in call["argument_expression"]
                                    and "s32" in call["argument_expression"])
            self.assertEqual(narrowed_widened["argument_expression_type"]["kind"], "cast_argument")
            self.assertEqual(narrowed_widened["candidate_symbols"], [])
            self.assertEqual(narrowed_widened["unresolved_candidate_symbols"], ["g_pointer"])
            pointer_cast = next(call for call in unresolved if "(char *) g_pointer" in call["argument_expression"])
            self.assertEqual(pointer_cast["argument_expression_type"]["kind"], "cast_argument")
            self.assertEqual(pointer_cast["candidate_symbols"], [])
            self.assertEqual(pointer_cast["unresolved_candidate_symbols"], ["g_pointer"])

    def test_string_literals_are_redacted_from_call_facts(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "src/ovl/TEST/unit.c"
            source.parent.mkdir(parents=True)
            source.write_text(
                "void sndi(char *); extern char *g_text;\n"
                "void f(void) { sndi(g_text); sndi(\"private text\"); }\n",
                encoding="utf-8")
            result = inventory_text_consumers(
                root, {"TEST": {"g_text"}}, ["src/ovl/TEST/unit.c"])
            serialized = str(result)
            self.assertNotIn("private text", serialized)
            self.assertEqual(len(result["files"][0]["calls"]), 1)


if __name__ == "__main__":
    unittest.main()
