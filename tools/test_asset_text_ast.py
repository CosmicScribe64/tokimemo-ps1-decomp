"""Unit tests for source-only text-call AST facts (no original data)."""
import hashlib
import tempfile
import unittest
from pathlib import Path

from tools.assets.text_ast import inventory_text_consumers, source_input_fingerprint


class TextAstTests(unittest.TestCase):
    def test_scoped_renderer_aliases_and_callee_guard_facts(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            main = root / "src/main/ui.c"
            rpg = root / "src/ovl/RPG_BAT/ui.c"
            other = root / "src/ovl/OTHER/ui.c"
            for path in (main, rpg, other):
                path.parent.mkdir(parents=True, exist_ok=True)
            main.write_text(
                "typedef unsigned char u8; typedef int s32;\n"
                "extern u8 *title; extern u8 *first; extern u8 *second; extern u8 *third;\n"
                "extern s32 dynamic_count;\n"
                "void message_window_set_title_hidden(u8 *); void x_taku_string_set(s32,s32,s32,s32);\n"
                "void f(void) { message_window_set_title_hidden(title); x_taku_string_set(first,second,third,3);\n"
                "x_taku_string_set(first,second,third,2);\n"
                "x_taku_string_set(first,second,third,dynamic_count); }\n",
                encoding="utf-8")
            rpg.write_text(
                "typedef unsigned char u8; typedef int s32; extern u8 *rpg_text;\n"
                "void rpg_label_create(s32,s32,s32);\n"
                "void f(void) { rpg_label_create(1,2,(s32)rpg_text); }\n",
                encoding="utf-8")
            other.write_text(
                "typedef unsigned char u8; typedef int s32; extern u8 *other_text;\n"
                "void rpg_label_create(s32,s32,s32);\n"
                "void f(void) { rpg_label_create(1,2,(s32)other_text); }\n",
                encoding="utf-8")
            candidates = {
                "main": {"title", "first", "second", "third", "dynamic_count"},
                "RPG_BAT": {"rpg_text"},
                "OTHER": {"other_text"},
            }
            result = inventory_text_consumers(
                root, candidates,
                ["src/main/ui.c", "src/ovl/RPG_BAT/ui.c", "src/ovl/OTHER/ui.c"])
            main_calls, rpg_calls, other_calls = [entry["calls"] for entry in result["files"]]
            wrapper = next(call for call in main_calls if call["callee"] == "message_window_set_title_hidden")
            self.assertEqual(wrapper["source_unit"], "main")
            self.assertEqual(wrapper["renderer"], "set_kanji_string")
            self.assertEqual(wrapper["candidate_symbols"], ["title"])
            conditional_calls = [call for call in main_calls
                                 if call["callee"] == "x_taku_string_set"
                                 and call["argument_index"] == 2]
            self.assertEqual(len(conditional_calls), 2)
            constant_true = next(call for call in conditional_calls
                                 if call["callee_read_condition_status"] == "conditional"
                                 and call["callee_read_conditions"][0]["status"] == "proven_true")
            self.assertEqual(constant_true["candidate_symbols"], ["third"])
            self.assertEqual(constant_true["callee_read_conditions"][1]["status"], "unknown")
            dynamic = next(call for call in conditional_calls
                           if call["callee_read_conditions"][0]["status"] == "unknown")
            self.assertEqual(dynamic["candidate_symbols"], ["third"])
            self.assertEqual(dynamic["callee_read_condition_status"], "conditional")
            self.assertFalse(any(call["callee"] == "x_taku_string_set" and
                                 call["argument_index"] == 2 and
                                 call["callee_read_condition_status"] == "proven_false"
                                 for call in main_calls))
            self.assertEqual(len(rpg_calls), 1)
            self.assertEqual(rpg_calls[0]["source_unit"], "RPG_BAT")
            self.assertEqual(rpg_calls[0]["argument_role"], "display")
            self.assertEqual(rpg_calls[0]["candidate_symbols"], ["rpg_text"])
            self.assertEqual(other_calls, [])

    def test_renderer_alias_roles_are_unit_scoped_in_ast_inventory(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            rpg = root / "src/ovl/rpg_battle/unit.c"
            other = root / "src/ovl/other_topic/unit.c"
            rpg.parent.mkdir(parents=True)
            other.parent.mkdir(parents=True)
            config_root = root / "config" / "overlays"
            config_root.mkdir(parents=True)
            for overlay, source_root in (("RPG_BAT", "rpg_battle"), ("OTHER", "other_topic")):
                (config_root / f"{overlay}.yaml").write_text(
                    f"options:\n  basename: {overlay}\n  src_path: src/ovl\n"
                    "segments:\n  - name: overlay\n    subsegments:\n"
                    f"      - [0x0, c, {source_root}/unit]\n",
                    encoding="utf-8")
            source = (
                "typedef unsigned char u8; typedef int s32; extern u8 *text;\n"
                "void rpg_label_create(s32,s32,s32); void get_p_name(s32);\n"
                "void f(void) { rpg_label_create(0,0,(s32)text); get_p_name((s32)text); }\n")
            rpg.write_text(source, encoding="utf-8")
            other.write_text(source, encoding="utf-8")
            result = inventory_text_consumers(
                root, {"RPG_BAT": {"text"}, "OTHER": {"text"}},
                ["src/ovl/rpg_battle/unit.c", "src/ovl/other_topic/unit.c"])
            rpg_calls, other_calls = [file["calls"] for file in result["files"]]
            self.assertEqual(len(rpg_calls), 1)
            self.assertEqual(rpg_calls[0]["callee"], "rpg_label_create")
            self.assertEqual(rpg_calls[0]["source_unit"], "RPG_BAT")
            self.assertEqual(rpg_calls[0]["candidate_symbols"], ["text"])
            self.assertEqual(other_calls, [])

    def test_local_function_pointer_does_not_inherit_global_renderer_role(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source = root / "src/main/local_shadow.c"
            source.parent.mkdir(parents=True)
            source.write_text(
                "typedef unsigned char u8; extern u8 *text;\n"
                "void local(void) { void (*message_window_set_title_hidden)(u8 *);\n"
                "message_window_set_title_hidden(text); }\n",
                encoding="utf-8")
            result = inventory_text_consumers(
                root, {"main": {"text"}}, ["src/main/local_shadow.c"])
            self.assertEqual(result["files"][0]["calls"], [])

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
