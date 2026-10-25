"""Read/write/numeric operand distinctions for the shared consumer roles."""
import unittest
from tools.assets.text_renderers import known_calls, read_argument_specs, string_argument_role


class RendererRoleTests(unittest.TestCase):
    def test_display_coordinates_and_formatter_payload_are_not_format_inputs(self):
        self.assertEqual(string_argument_role("sndi", 0), "display")
        self.assertIsNone(string_argument_role("sndi", 1))
        self.assertEqual(string_argument_role("sprintf", 1), "format")
        self.assertIsNone(string_argument_role("sprintf", 2))
        self.assertEqual(string_argument_role("set_kanji_string", 3), "display")
        self.assertIsNone(string_argument_role("set_kanji_string", 0))
        for call in ("gnsx", "message_window_set_title"):
            spec = read_argument_specs(call)[0]
            self.assertEqual(spec["renderer"], "set_kanji_string")
        for call in ("talk_render", "talk_speak_part"):
            spec = read_argument_specs(call)[0]
            self.assertIsNone(spec["renderer"])
            self.assertEqual(spec["renderer_status"], "unimplemented_fixed_renderer")

    def test_write_destinations_and_unknown_names_never_prove_text_reading(self):
        for call in ("strcpy", "strncpy", "strcat", "strncat"):
            self.assertIsNone(string_argument_role(call, 0))
            self.assertEqual(string_argument_role(call, 1), "c_string_operand")
        self.assertIsNone(string_argument_role("get_g_name", 0))
        self.assertIsNone(string_argument_role("get_p_name", 0))
        self.assertIsNone(string_argument_role("get_menu_text", 0))
        self.assertIsNone(string_argument_role("load_string_by_id", 0))

    def test_main_aliases_and_conditional_choice_argument(self):
        self.assertEqual(string_argument_role("message_window_set_title_hidden", 0), "display")
        self.assertIsNone(string_argument_role("message_window_set_title_hidden", 1))
        self.assertIsNone(string_argument_role("func_80066104", 0))
        self.assertEqual(string_argument_role("x_taku_string_set", 0, "main"), "display")
        self.assertEqual(string_argument_role("x_taku_string_set", 1, "main"), "display")
        self.assertEqual(string_argument_role("x_taku_string_set", 2, "main"), "display")
        self.assertIsNone(string_argument_role("x_taku_string_set", 3, "main"))
        third = next(spec for spec in read_argument_specs("x_taku_string_set", "main")
                     if spec["index"] == 2)
        self.assertEqual(third["renderer"], "set_kanji_string")
        self.assertEqual(third["conditions"], (
            {"argument_index": 3, "operator": "==", "value": 3},
            {"argument_index": 2, "operator": "!=", "value": 0},
        ))

    def test_overlay_numeric_alias_requires_exact_link_unit(self):
        self.assertIsNone(string_argument_role("rpg_label_create", 2))
        self.assertIsNone(string_argument_role("rpg_label_create", 2, "OTHER"))
        self.assertEqual(string_argument_role("rpg_label_create", 2, "RPG_BAT"), "display")
        self.assertIsNone(string_argument_role("rpg_label_create", 0, "RPG_BAT"))
        self.assertIsNone(string_argument_role("func_8013E934", 2, "RPG_BAT"))
        self.assertIn("rpg_label_create", known_calls("RPG_BAT"))
        self.assertNotIn("rpg_label_create", known_calls("OTHER"))


if __name__ == "__main__":
    unittest.main()
