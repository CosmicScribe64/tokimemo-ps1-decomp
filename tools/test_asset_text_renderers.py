"""Read/write/numeric operand distinctions for the shared consumer roles."""
import unittest
from tools.assets.text_renderers import string_argument_role


class RendererRoleTests(unittest.TestCase):
    def test_display_coordinates_and_formatter_payload_are_not_format_inputs(self):
        self.assertEqual(string_argument_role("sndi", 0), "display")
        self.assertIsNone(string_argument_role("sndi", 1))
        self.assertEqual(string_argument_role("sprintf", 1), "format")
        self.assertIsNone(string_argument_role("sprintf", 2))
        self.assertEqual(string_argument_role("set_kanji_string", 3), "display")
        self.assertIsNone(string_argument_role("set_kanji_string", 0))

    def test_write_destinations_and_unknown_names_never_prove_text_reading(self):
        for call in ("strcpy", "strncpy", "strcat", "strncat"):
            self.assertIsNone(string_argument_role(call, 0))
            self.assertEqual(string_argument_role(call, 1), "c_string_operand")
        self.assertIsNone(string_argument_role("get_g_name", 0))
        self.assertIsNone(string_argument_role("get_p_name", 0))
        self.assertIsNone(string_argument_role("get_menu_text", 0))
        self.assertIsNone(string_argument_role("load_string_by_id", 0))


if __name__ == "__main__":
    unittest.main()
