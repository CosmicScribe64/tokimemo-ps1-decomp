#!/usr/bin/env python3
"""Unit tests for translation_tree.py converter, validator, and builder."""

import glob
import hashlib
import json
import os
import shutil
import sys
import tempfile
import unittest

TOOLS_DIR = os.path.dirname(os.path.abspath(__file__))
if TOOLS_DIR not in sys.path:
    sys.path.insert(0, TOOLS_DIR)

CANDIDATE_TREE_DIR = os.path.abspath(
    os.path.join(TOOLS_DIR, "..", "translations", "en")
)

from translation_tree import (
    ARCHIVE_TITLES,
    CHAR_NAMES,
    PURE_UNKNOWN_PATTERN,
    build_tree,
    choose_japanese_text,
    format_context_string,
    parse_markdown_table,
    route_id,
    sha256_file,
    split_tree,
    validate_tree,
)


class TestTranslationTree(unittest.TestCase):
    """Test suite for 5-column contributor translation tree."""

    def test_routing(self):
        """Test ID routing to clean directory structure with canonical names."""
        # Talk: Asahina Yuko
        fn, lid, title = route_id("talk:CDROM/BUNKA/AS_S6.BIN:main:g0000:l0000")
        self.assertEqual(fn, "dialogue/bunka/as_s6.md")
        self.assertEqual(lid, "main:g0000:l0000")
        self.assertIn("Asahina Yuko", title)

        # Speaktst: Fujisaki Shiori
        fn, lid, title = route_id("speaktst-layout:CDROM/SPEAKTST/TEST_FU.BIN:g0000:l0000:o00000000")
        self.assertEqual(fn, "test_dialogue/test_fu.md")
        self.assertEqual(lid, "g0000:l0000")
        self.assertIn("Fujisaki Shiori", title)

        # Speaktst: Himoo Yuina
        fn, lid, title = route_id("speaktst-layout:CDROM/SPEAKTST/TEST_HI.BIN:g0000:l0000:o00000000")
        self.assertEqual(fn, "test_dialogue/test_hi.md")
        self.assertEqual(lid, "g0000:l0000")
        self.assertIn("Himoo Yuina", title)

        # Speaktst: Koshiki Yukari
        fn, lid, title = route_id("speaktst-layout:CDROM/SPEAKTST/TEST_KO.BIN:g0000:l0000:o00000000")
        self.assertEqual(fn, "test_dialogue/test_ko.md")
        self.assertEqual(lid, "g0000:l0000")
        self.assertIn("Koshiki Yukari", title)

        # Bunka: Katagiri Ayako (KT)
        fn, lid, title = route_id("talk:CDROM/BUNKA/KT_S61.BIN:main:g0000:l0000")
        self.assertEqual(fn, "dialogue/bunka/kt_s61.md")
        self.assertIn("Katagiri Ayako", title)

        # Bunka: Kisaragi Mio (KS)
        fn, lid, title = route_id("talk:CDROM/BUNKA/KS_S61.BIN:main:g0000:l0000")
        self.assertEqual(fn, "dialogue/bunka/ks_s61.md")
        self.assertIn("Kisaragi Mio", title)

        # C strings
        fn, lid, title = route_id("c:BUNKAKEN:80149144:1")
        self.assertEqual(fn, "system/c_strings/bunkaken.md")
        self.assertEqual(lid, "80149144:1")

        # Exedir
        fn, lid, title = route_id("CDROM/EXEDIR/TEL.EXN:0000c430:ff82ae95a313")
        self.assertEqual(fn, "system/exedir/tel.md")
        self.assertEqual(lid, "0000c430:ff82ae95a313")

        # Main exe
        fn, lid, title = route_id("SLPM_86.053:00072ad0:49803ec53984")
        self.assertEqual(fn, "system/main_exe/slpm_86_053.md")
        self.assertEqual(lid, "00072ad0:49803ec53984")

        # Supplemental
        fn, lid, title = route_id("supplemental:CDROM/SHIAI/SIAI_TX.BIN:0000", is_supplemental=True)
        self.assertEqual(fn, "supplemental/siai_tx.md")
        self.assertEqual(lid, "0000")

    def test_japanese_selection(self):
        """Test Japanese display string selection rules."""
        # 1. Standard copy_template with tokens
        res = choose_japanese_text(
            japanese="こうして、 文化祭は、終了した。",
            copy_template="こうして、[[T001:row]]文化祭は、終了した。",
            target_text="And so, the[[T001:row]]festival was over.",
        )
        self.assertEqual(res, "こうして、[[T001:row]]文化祭は、終了した。")

        # 2. Pure unknown token with readable Japanese
        res = choose_japanese_text(
            japanese="文芸部",
            copy_template="[[T001:unknown_renderer_bytes]]",
            target_text="Literature Club[[T001:unknown_renderer_bytes]]",
        )
        self.assertEqual(res, "文芸部 (`[[T001:unknown_renderer_bytes]]`)")

        # 3. Empty copy_template fallback to japanese
        res = choose_japanese_text(
            japanese="近所の公園",
            copy_template="",
            target_text="Neighborhood Park",
        )
        self.assertEqual(res, "近所の公園")

        # 4. Empty slot
        res = choose_japanese_text(
            japanese="",
            copy_template="",
            target_text=None,
        )
        self.assertEqual(res, "_(empty slot)_")

    def test_context_formatting(self):
        """Test human-readable concise context formatting."""
        section_context = {
            ("CDROM/SERIFU/DATE_FU.BIN", "section-00"): "Neighborhood Park",
            ("CDROM/SERIFU/DATE_FU.BIN", "section-01"): "Central Park",
        }

        # Dialogue with location and monologue
        ctx1 = 'CDROM/SERIFU/DATE_FU.BIN; script_record; section="section-00"; group=0; line=1; speaker={"label":"inline_or_effect","runtime_values_preserved_symbolically":true}; command=inline; next_id=...'
        res1 = format_context_string("talk:CDROM/SERIFU/DATE_FU.BIN:section-00:g0000:l0001", ctx1, section_context)
        self.assertEqual(res1, "Neighborhood Park · Player (monologue)")

        # Dialogue with heroine line
        ctx2 = 'CDROM/SERIFU/DATE_FU.BIN; script_record; section="section-00"; group=0; line=0; speaker={"label":"current_girl_short_name","runtime_values_preserved_symbolically":true}; command=girl; next_id=...'
        res2 = format_context_string("talk:CDROM/SERIFU/DATE_FU.BIN:section-00:g0000:l0000", ctx2, section_context)
        self.assertEqual(res2, "Neighborhood Park · Shiori")

        # C string function
        ctx3 = 'CDROM/EXEDIR/NAME_ENT.EXN; source_literal; category="debug_or_format"; source_function="file_select_settings_restore"; source_line=601; call="printf"; ...'
        res3 = format_context_string("c:NAME_ENT:8013b77c:1", ctx3, section_context)
        self.assertEqual(res3, "file_select_settings_restore()")

        # Dictionary slot
        ctx4 = 'CDROM/EXEDIR/DATE.EXN; dictionary_slot; table="place_names"; index=0; ...'
        res4 = format_context_string("dictionary:CDROM/EXEDIR/DATE.EXN:place_names:00", ctx4, section_context)
        self.assertEqual(res4, "place_names[0]")

        # Supplemental commentary
        ctx5 = "CDROM/SHIAI/SIAI_TX.BIN; supplemental_talk; group=0; line=0; offset=0x0"
        res5 = format_context_string("supplemental:CDROM/SHIAI/SIAI_TX.BIN:0000", ctx5, section_context)
        self.assertEqual(res5, "Match commentary (line 0)")

    def test_markdown_table_parsing_5_col(self):
        """Test parsing 5-column Markdown tables."""
        with tempfile.NamedTemporaryFile("w", encoding="utf-8", suffix=".md", delete=False) as f:
            f.write("# Test\n\n")
            f.write("| ID | Context | Japanese | Translation | Notes |\n")
            f.write("| :--- | :--- | :--- | :--- | :--- |\n")
            f.write("| `main:g0001:l0000` | Central Park · Shiori | こんにちは | Hello there! | Some note |\n")
            f.write("| `main:g0001:l0001` | place_names[21] | _(empty slot)_ | _(empty slot)_ | Empty slot note |\n")
            f.write("| `main:g0001:l0002` | Misc | `   ` | `   ` | Three spaces |\n")
            f.write("| `main:g0001:l0003` | Dialog | A \\| B | Text with \\| pipe | Note with \\| pipe |\n")
            tmp_path = f.name

        try:
            table = parse_markdown_table(tmp_path)
            self.assertEqual(len(table), 4)

            # Standard
            target, note = table["main:g0001:l0000"]
            self.assertEqual(target, "Hello there!")
            self.assertEqual(note, "Some note")

            # Empty slot
            target, note = table["main:g0001:l0001"]
            self.assertIsNone(target)

            # Whitespace
            target, note = table["main:g0001:l0002"]
            self.assertEqual(target, "   ")

            # Escaped pipe
            target, note = table["main:g0001:l0003"]
            self.assertEqual(target, "Text with | pipe")
            self.assertEqual(note, "Note with | pipe")
        finally:
            os.remove(tmp_path)

    def test_lossless_roundtrip(self):
        """Verify full 55,403 + 508 lossless byte-for-byte roundtrip from 5-column tree."""
        tree_dir = CANDIDATE_TREE_DIR
        with tempfile.TemporaryDirectory() as tmp_dir:
            out_ret = os.path.join(tmp_dir, "returned-v3.jsonl")
            out_sup = os.path.join(tmp_dir, "supplemental.jsonl")

            ret_sha, sup_sha = build_tree(
                tree_dir=tree_dir,
                out_returned_path=out_ret,
                out_supplemental_path=out_sup,
                base_returned_path=os.path.join(tree_dir, "returned-v3.jsonl"),
                base_supplemental_path=os.path.join(tree_dir, "supplemental.jsonl"),
            )

            expected_ret = "28c2c8fbf6f31b8a56f8415328c94fc3aebed39dbe96e114096193602c4dd533"
            expected_sup = "49bbeaf94ce5cca88634fb17fb62e3b6a88b4032f86d769dc54ad4285e91e58c"

            self.assertEqual(ret_sha, expected_ret)
            self.assertEqual(sup_sha, expected_sup)

    def test_edit_propagation(self):
        """Verify human edits in 5-column markdown propagate cleanly to built output."""
        base_tree_dir = CANDIDATE_TREE_DIR

        with tempfile.TemporaryDirectory() as tmp_dir:
            copied_tree = os.path.join(tmp_dir, "en")
            shutil.copytree(base_tree_dir, copied_tree)

            as_file = os.path.join(copied_tree, "dialogue/bunka/as_s6.md")
            with open(as_file, "r", encoding="utf-8") as f:
                content = f.read()

            self.assertIn("I came to visit.", content)
            edited_content = content.replace(
                "| `main:g0001:l0000` | Main · Yuko | 遊びに来ちゃった。 | I came to visit. |  |",
                "| `main:g0001:l0000` | Main · Yuko | 遊びに来ちゃった。 | I came to visit the festival! | Contributor updated note |",
            )
            with open(as_file, "w", encoding="utf-8") as f:
                f.write(edited_content)

            out_ret = os.path.join(tmp_dir, "returned-v3.jsonl")
            out_sup = os.path.join(tmp_dir, "supplemental.jsonl")
            build_tree(
                tree_dir=copied_tree,
                out_returned_path=out_ret,
                out_supplemental_path=out_sup,
                base_returned_path=os.path.join(copied_tree, "returned-v3.jsonl"),
                base_supplemental_path=os.path.join(copied_tree, "supplemental.jsonl"),
            )

            found = False
            with open(out_ret, "r", encoding="utf-8") as f:
                for line in f:
                    d = json.loads(line)
                    if d["id"] == "talk:CDROM/BUNKA/AS_S6.BIN:main:g0001:l0000":
                        self.assertEqual(d["target_text"], "I came to visit the festival!")
                        self.assertEqual(d["translator_note"], "Contributor updated note")
                        # review_status preserved from baseline
                        self.assertEqual(d["review_status"], "ready_for_review")
                        found = True
                        break
            self.assertTrue(found, "Edited row should be present in built JSONL")

    def test_validation(self):
        """Verify that validate_tree passes on candidate tree."""
        tree_dir = CANDIDATE_TREE_DIR
        self.assertTrue(validate_tree(tree_dir))

    def test_image_catalog_japanese_preserved(self):
        """Verify image text catalog exists and retains Original Japanese."""
        cat_md = os.path.join(CANDIDATE_TREE_DIR, "graphics/image_text_catalog.md")
        cat_jsonl = os.path.join(CANDIDATE_TREE_DIR, "graphics/image_text_catalog.jsonl")

        self.assertTrue(os.path.exists(cat_md))
        self.assertTrue(os.path.exists(cat_jsonl))

        with open(cat_md, "r", encoding="utf-8") as f:
            md_text = f.read()
        self.assertIn("Original Japanese", md_text)
        self.assertIn("期末試験結果発表", md_text)

        with open(cat_jsonl, "r", encoding="utf-8") as f:
            first_line = json.loads(f.readline())
        self.assertIn("original_japanese", first_line)

    def test_tags_documentation(self):
        """Verify TAGS.md exists and documents all 28 tags."""
        tags_file = os.path.join(CANDIDATE_TREE_DIR, "TAGS.md")
        self.assertTrue(os.path.exists(tags_file))

        with open(tags_file, "r", encoding="utf-8") as f:
            text = f.read()

        all_28_tags = [
            "row", "choice_option", "girl_name", "reply_variant", "player_form",
            "unknown_renderer_bytes", "line_feed", "inline_label", "format_directive",
            "row_separator", "player_family", "player_full", "context_m", "context_a",
            "unknown", "terminal_byte_before_nul", "player_given", "conditional_choice_zero",
            "reply_skip_marker", "context_o", "tab", "carriage_return", "context_s",
            "yoshio_name", "context_w", "storage_byte_1b", "storage_byte_1a", "storage_byte_0c",
        ]
        for t in all_28_tags:
            self.assertIn(f"### {t}", text, f"TAGS.md should document {t}")

    def test_no_meta_dir(self):
        """Verify that duplicate _meta/ directory is removed."""
        meta_dir = ".claude/orchestrator/state/T-28009/japanese-columns-v1/candidate/translations/en/_meta"
        self.assertFalse(os.path.exists(meta_dir), "_meta directory should not exist")


if __name__ == "__main__":
    unittest.main()
