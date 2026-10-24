"""Invented non-talk renderer fixtures for Docker unittest (T-28001)."""
import unittest

from tools.assets.text import TextFormatError
from tools.assets.text_fixed import tokenize_fixed_text


class FixedTextTests(unittest.TestCase):
    def check_lossless(self, result, raw):
        self.assertEqual(bytes.fromhex("".join(t["raw_hex"] for t in result["tokens"])), raw)
        self.assertNotIn("\ufffd", str(result))

    def test_direct_loader_has_no_talk_name_commands(self):
        raw = b"PG" + "テ".encode("shift_jis")
        result = tokenize_fixed_text(raw, "load_string")
        self.assertEqual(result["tokens"][0]["kind"], "renderer_rejected_pair")
        self.assertEqual(result["tokens"][1]["kind"], "not_rendered_after_rejected_pair")
        self.check_lossless(result, raw)

    def test_terminal_space_is_not_a_row_separator(self):
        raw = "テ".encode("shift_jis") + b" "
        result = tokenize_fixed_text(raw, "set_kanji_string")
        self.assertEqual(result["tokens"][-1]["kind"], "terminal_byte_before_nul")
        self.assertTrue(result["entry_active"])
        self.check_lossless(result, raw)

    def test_one_byte_entry_is_inactive(self):
        result = tokenize_fixed_text(b"P", "set_kanji_string")
        self.assertFalse(result["entry_active"])

    def test_sndi_only_replaces_first_even_p(self):
        raw = b"PP" + "テ".encode("shift_jis") + b"P"
        result = tokenize_fixed_text(raw, "sndi", {"y": 0, "col": 0})
        subs = [t for t in result["tokens"] if t["kind"] == "substitution"]
        self.assertEqual([t["offset"] for t in subs], [0])
        self.assertEqual(result["style"]["prefix"], "girl_quote_open")
        self.check_lossless(result, raw)

    def test_sndi_does_not_expand_g_in_ordinary_color(self):
        result = tokenize_fixed_text(b"G", "sndi", {"y": 1, "col": 0})
        self.assertFalse(any("substitution" in t["kind"] for t in result["tokens"]))

    def test_sndi_g_parity_is_recomputed_after_p_expansion(self):
        raw = b"PG"
        context = {"y": 0, "col": 15, "bindings": {"player_form": "テ".encode("shift_jis")}}
        result = tokenize_fixed_text(raw, "sndi", context)
        self.assertEqual([t.get("code") for t in result["tokens"]], ["P", "G"])
        self.assertTrue(all(t["kind"] == "substitution" for t in result["tokens"]))
        unknown = tokenize_fixed_text(raw, "sndi", {"y": 0, "col": 15})
        self.assertEqual(unknown["tokens"][1]["kind"], "conditional_substitution")
        self.check_lossless(unknown, raw)

    def test_sndisp_counter_resets_for_p_g_and_spaces(self):
        raw = b"P " + "テ".encode("shift_jis") + b" G "
        result = tokenize_fixed_text(raw, "sndisp", {"y": 0, "col": 15})
        separators = [t["offset"] for t in result["tokens"] if t["kind"] == "row_separator"]
        self.assertEqual(separators, [1, 4, 6])
        self.assertEqual(len(result["rows"]), 4)
        self.assertEqual(result["rows"][-1]["length"], 0)
        self.check_lossless(result, raw)

    def test_sndisp_odd_space_is_not_separator(self):
        raw = b"x " + "テ".encode("shift_jis")
        result = tokenize_fixed_text(raw, "sndisp")
        self.assertEqual(len(result["rows"]), 1)
        self.check_lossless(result, raw)

    def test_fixed_style_and_copy_limits_are_explicit(self):
        result = tokenize_fixed_text(b"x" * 64, "sndi", {"y": 0, "col": 0x1F})
        self.assertEqual(result["style"]["prefix"], "parenthesis_open")
        self.assertFalse(result["copy_terminated"])
        wrapped = tokenize_fixed_text(b" " * 6, "sndisp")
        self.assertTrue(wrapped["split_capacity_exceeded"])

    def test_printf_directives_are_lossless_and_not_prose(self):
        raw = b"value=%-+#08.3ld name=%.*s %%\n"
        result = tokenize_fixed_text(raw, "sprintf")
        directives = [t for t in result["tokens"] if t["kind"] == "format_directive"]
        self.assertEqual([t["conversion"] for t in directives], ["d", "s", "%"])
        self.assertEqual([t["argument_count"] for t in directives], [1, 2, 0])
        self.assertFalse(any(t["translatable"] for t in directives))
        self.check_lossless(result, raw)

    def test_printf_unknown_and_modern_formats_stay_explicit(self):
        raw = b"%Q %2$*3$.4lld end%"
        result = tokenize_fixed_text(raw, "printf")
        self.assertTrue(any(t["kind"] == "unknown_format" for t in result["tokens"]))
        modern = [t for t in result["tokens"] if t["kind"] == "format_directive" and t["length"] == "ll"][0]
        self.assertEqual(modern["runtime_support"], "requires_libc_verification")
        self.check_lossless(result, raw)

    def test_strict_source_and_unknown_renderer_never_replace_bytes(self):
        raw = b"\xff\x09"
        result = tokenize_fixed_text(raw, "unverified")
        self.assertIsNone(result["source_decoded_text"])
        self.assertIn("unsupported", result)
        self.check_lossless(result, raw)
        with self.assertRaises(TextFormatError):
            tokenize_fixed_text(b"x\0", "sndi")


if __name__ == "__main__":
    unittest.main()
