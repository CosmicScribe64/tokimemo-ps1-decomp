"""Invented linked-script fixtures for T-28001; run in Docker with unittest."""
import copy
import json
import struct
import tempfile
import unittest
from unittest.mock import patch
from pathlib import Path

from tools.assets.text import TextFormatError, parse_script, tokenize_text
from tools.assets.text_export import OUTPUT_ROOT, export_text

BASE = 0x80100000


def fixture(command=1, raw=None):
    """One group, one line and FF; every text byte is invented in this test."""
    raw = "テスト".encode("shift_jis") if raw is None else raw
    data = bytearray(512)
    for off, target in ((0, 32), (4, 48), (8, 64), (32, 96), (64, 256)):
        struct.pack_into("<I", data, off, BASE + target)
    data[48:50] = bytes((command, 255))
    data[96:96 + len(raw) + 1] = raw + b"\0"
    struct.pack_into("<IIII", data, 256, 0x100, 0x200, 0x300, 0x400)
    profile = {"id": "synthetic", "path": "CDROM/SERIFU/INVENTED.BIN", "load_address": BASE,
               "sections": [{"id": "main", "text_table": BASE, "command_table": BASE + 4,
                             "voice_table": BASE + 8, "group_count": 1}]}
    return bytes(data), profile


class TokenTests(unittest.TestCase):
    def test_full_width_and_controls_roundtrip(self):
        raw = "テ".encode("shift_jis") + b"Pq" + "ト".encode("shift_jis") + b" pGmFSYOWAM"
        result = tokenize_text(raw, 1)
        self.assertEqual(bytes.fromhex("".join(t["raw_hex"] for t in result["tokens"])), raw)
        self.assertEqual(result["unknown_token_count"], 0)
        self.assertIn("{player_form}", result["decoded_text"])
        self.assertEqual([t["kind"] for t in result["tokens"]][1:3], ["substitution", "separator"])

    def test_trail_control_byte_is_not_a_command(self):
        # 0x8371 is a full-width katakana; its trail byte happens to be ASCII q.
        tokens = tokenize_text(bytes.fromhex("8371"), 2)["tokens"]
        self.assertEqual(len(tokens), 1)
        self.assertEqual(tokens[0]["kind"], "glyph")

    def test_cp932_ambiguity_is_explicit(self):
        token = tokenize_text(bytes.fromhex("8160"), 0)["tokens"][0]
        self.assertEqual(token["text"], "\u301c")
        self.assertEqual(token["cp932_alternative"], "\uff5e")

    def test_cp932_extension_stays_unknown(self):
        token = tokenize_text(bytes.fromhex("fa40"), 0)["tokens"][0]
        self.assertEqual(token["kind"], "unknown")
        self.assertEqual(token["raw_hex"], "fa40")

    def test_two_single_byte_kana_are_not_a_full_width_glyph(self):
        token = tokenize_text(bytes.fromhex("a1a2"), 0)["tokens"][0]
        self.assertEqual(token["kind"], "unknown")

    def test_unknown_pair_does_not_resync(self):
        tokens = tokenize_text(b"zP\xff", 0)["tokens"]
        self.assertEqual([t["raw_hex"] for t in tokens], ["7a50", "ff"])
        self.assertTrue(all(t["kind"] == "unknown" for t in tokens))

    def test_source_unicode_and_renderer_view_are_distinct(self):
        raw = "テ".encode("shift_jis") + b"\t" + "ト".encode("shift_jis")
        result = tokenize_text(raw, 1)
        self.assertEqual(result["source_decoded_text"].encode("shift_jis"), raw)
        self.assertIsNone(result["decoded_text"])
        self.assertIn("{raw:", result["annotated_text"])
        self.assertNotIn("\ufffd", str(result))

    def test_conditional_reply_and_choices(self):
        raw = "テ".encode("shift_jis")
        self.assertEqual(tokenize_text(raw + b"nn", 0)["tokens"][-1]["kind"], "conditional_choice_zero")
        reply = tokenize_text(raw + b"qn", 2)
        self.assertEqual(reply["reply_variant_count"], 2)
        self.assertEqual(reply["tokens"][-1]["kind"], "reply_skip_marker")
        self.assertEqual(tokenize_text(raw + b"q" + raw, 8)["choice"]["option_count"], 2)
        self.assertEqual(tokenize_text(raw + b" " + raw + b"q" + raw, 3)["choice"]["option_count"], 3)

    def test_inline_speaker_is_not_body_control(self):
        raw = b">" + "テト".encode("shift_jis") + b"P"
        parsed = tokenize_text(raw, 7)
        self.assertEqual(parsed["tokens"][0]["kind"], "inline_label")
        self.assertEqual(parsed["tokens"][1]["name"], "player_form")
        self.assertEqual(bytes.fromhex("".join(t["raw_hex"] for t in parsed["tokens"])), raw)
        with self.assertRaises(TextFormatError):
            tokenize_text(b">", 7)

    def test_skipped_reply_uses_engine_two_byte_stepping(self):
        glyph = "テ".encode("shift_jis")
        self.assertEqual(tokenize_text(b"nnq" + glyph, 2)["reply_variant_count"], 2)
        self.assertEqual(tokenize_text(b"nq" + glyph + b"q" + glyph, 2)["reply_variant_count"], 2)
        self.assertEqual(tokenize_text(b"nnq" + glyph, 2)["tokens"][0]["raw_hex"], "6e6e")


class ScriptTests(unittest.TestCase):
    def test_offsets_hash_voice_end_and_unclaimed(self):
        data, profile = fixture()
        parsed = parse_script(data, profile["path"], profile)
        line, end = parsed["records"]
        self.assertEqual(line["text"]["offset"], 96)
        self.assertEqual(line["command"]["offset"], 48)
        self.assertEqual(line["voice"]["pairs"][0]["clip_start"], 256)
        self.assertEqual(end["command"]["name"], "end")
        self.assertNotIn("text", end)  # FF has no corresponding text pointer slot.
        self.assertEqual(parsed["coverage"]["text_records"], 1)
        self.assertTrue(parsed["unclaimed_ranges"])
        self.assertEqual(json.dumps(parsed, sort_keys=True), json.dumps(parse_script(data, profile["path"], profile), sort_keys=True))

    def test_reply_voice_variants(self):
        raw = "テ".encode("shift_jis") + b"q" + "ト".encode("shift_jis")
        data, profile = fixture(2, raw)
        pairs = parse_script(data, profile["path"], profile)["records"][0]["voice"]["pairs"]
        self.assertEqual(len(pairs), 2)
        self.assertEqual(pairs[1]["offset"], 264)
        self.assertEqual(pairs[1]["clip_end"], 0x400)

    def test_unknown_command_is_preserved(self):
        data, profile = fixture(240)
        line = parse_script(data, profile["path"], profile)["records"][0]
        self.assertEqual(line["command"]["raw_hex"], "f0")
        self.assertIn("unsupported", line)
        self.assertNotIn("voice", line)

    def test_bad_layout_and_checksum_fail(self):
        data, profile = fixture()
        bad = bytearray(data)
        struct.pack_into("<I", bad, 32, BASE - 1)
        with self.assertRaises(TextFormatError):
            parse_script(bytes(bad), profile["path"], profile)
        profile["sha256"] = "0" * 64
        with self.assertRaises(TextFormatError):
            parse_script(data, profile["path"], profile)

    def test_unterminated_text_and_command_fail(self):
        data, profile = fixture()
        bad = bytearray(data)
        bad[96:] = b"\x82" * (len(bad) - 96)
        with self.assertRaises(TextFormatError):
            parse_script(bytes(bad), profile["path"], profile)
        bad = bytearray(data)
        bad[49:] = b"\0" * (len(bad) - 49)
        with self.assertRaises(TextFormatError):
            parse_script(bytes(bad), profile["path"], profile)

    def test_duplicate_section_and_unsafe_source(self):
        data, profile = fixture()
        profile["sections"].append(copy.deepcopy(profile["sections"][0]))
        with self.assertRaises(TextFormatError):
            parse_script(data, profile["path"], profile)
        with self.assertRaises(TextFormatError):
            parse_script(data, "../escape", profile)

    def test_catalog_inventory_and_output_guard(self):
        data, profile = fixture()
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            file = root / profile["path"]
            file.parent.mkdir(parents=True)
            file.write_bytes(data)
            (file.parent / "UNKNOWN.BIN").write_bytes(b"invented")
            catalog = export_text(root, profiles=[profile])
            self.assertEqual(catalog["coverage"], {"exported_files": 1, "unsupported_files": 1, "missing_files": 0})
            with self.assertRaises(TextFormatError):
                export_text(root, root / "out", [profile])
            OUTPUT_ROOT.mkdir(parents=True, exist_ok=True)
            with tempfile.TemporaryDirectory(dir=OUTPUT_ROOT) as output:
                export_text(root, output, [profile])
                first = (Path(output) / "catalog.json").read_bytes()
                export_text(root, output, [profile])
                self.assertEqual(first, (Path(output) / "catalog.json").read_bytes())

    def test_input_symlink_escape_fails(self):
        data, profile = fixture()
        with tempfile.TemporaryDirectory() as tmp, tempfile.TemporaryDirectory() as outside:
            root = Path(tmp)
            target = Path(outside) / "external.bin"
            target.write_bytes(data)
            file = root / profile["path"]
            file.parent.mkdir(parents=True)
            file.symlink_to(target)
            with self.assertRaises(TextFormatError):
                export_text(root, profiles=[profile])

    def test_symlinked_cache_root_is_rejected(self):
        data, profile = fixture()
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp) / "input"
            file = root / profile["path"]
            file.parent.mkdir(parents=True)
            file.write_bytes(data)
            outside = Path(tmp) / "outside"
            outside.mkdir()
            cache = Path(tmp) / "cache"
            cache.symlink_to(outside, target_is_directory=True)
            with patch("tools.assets.text_export.OUTPUT_ROOT", cache / "text"):
                with self.assertRaises(TextFormatError):
                    export_text(root, cache / "text", [profile])
            self.assertFalse((outside / "text").exists())

    def test_font_executable_symlink_escape_fails_before_read(self):
        with tempfile.TemporaryDirectory() as tmp, tempfile.TemporaryDirectory() as outside:
            root = Path(tmp)
            external = Path(outside) / "external.exe"
            external.write_bytes(b"invented")
            (root / "SLPM_86.053").symlink_to(external)
            with self.assertRaisesRegex(TextFormatError, "escapes extracted directory"):
                export_text(root)


if __name__ == "__main__":
    unittest.main()
