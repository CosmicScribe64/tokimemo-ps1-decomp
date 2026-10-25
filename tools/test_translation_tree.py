#!/usr/bin/env python3
"""Unit tests for translation_tree.py converter, validator, and builder."""

import hashlib
import json
import os
import shutil
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from translation_tree import (
    CJK_PATTERN,
    build_tree,
    parse_markdown_table,
    route_id,
    sha256_file,
    split_tree,
    validate_tree,
)

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CANONICAL_TREE_DIR = os.path.join(REPO_ROOT, "translations", "en")


class TestTranslationTree(unittest.TestCase):
    """Test suite for contributor translation tree."""

    def test_cjk_detection(self):
        """Test CJK detection regex against Japanese and English text."""
        # Japanese strings
        self.assertTrue(bool(CJK_PATTERN.search("ときめきメモリアル")))
        self.assertTrue(bool(CJK_PATTERN.search("藤崎詩織")))
        self.assertTrue(bool(CJK_PATTERN.search("テスト")))
        self.assertTrue(bool(CJK_PATTERN.search("期末試験結果発表")))
        self.assertTrue(bool(CJK_PATTERN.search("「こんにちは」")))
        self.assertTrue(bool(CJK_PATTERN.search("1995年4月23日")))

        # English strings with numbers and tokens
        self.assertFalse(bool(CJK_PATTERN.search("Hello world!")))
        self.assertFalse(bool(CJK_PATTERN.search("And so, the[[T001:row]]festival was over.")))
        self.assertFalse(bool(CJK_PATTERN.search("[[T001:sub:player_family]]")))
        self.assertFalse(bool(CJK_PATTERN.search("Q&A Corner | Term Exam Results (112x12 box)")))
        self.assertFalse(bool(CJK_PATTERN.search("SLPM_86.053:00072ad0:49803ec53984")))

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

    def test_markdown_table_parsing(self):
        """Test parsing of table cells including empty slots and backtick whitespace."""
        with tempfile.NamedTemporaryFile("w", encoding="utf-8", suffix=".md", delete=False) as f:
            f.write("# Test\n\n")
            f.write("| ID | Status | Translation | Notes |\n")
            f.write("| :--- | :--- | :--- | :--- |\n")
            f.write("| `main:g0001:l0000` | ready | Standard text | Some note |\n")
            f.write("| `main:g0001:l0001` | empty | _(empty slot)_ | Empty note |\n")
            f.write("| `main:g0001:l0002` | ready | `   ` | Three spaces |\n")
            f.write("| `main:g0001:l0003` | ready | Text with \\| pipe | Note with \\| pipe |\n")
            tmp_path = f.name

        try:
            table = parse_markdown_table(tmp_path)
            self.assertEqual(len(table), 4)

            # Standard
            st, target, note = table["main:g0001:l0000"]
            self.assertEqual(st, "ready")
            self.assertEqual(target, "Standard text")
            self.assertEqual(note, "Some note")

            # Empty slot
            st, target, note = table["main:g0001:l0001"]
            self.assertEqual(target, None)

            # Whitespace
            st, target, note = table["main:g0001:l0002"]
            self.assertEqual(target, "   ")

            # Escaped pipe
            st, target, note = table["main:g0001:l0003"]
            self.assertEqual(target, "Text with | pipe")
            self.assertEqual(note, "Note with | pipe")
        finally:
            os.remove(tmp_path)

    def test_lossless_roundtrip(self):
        """Verify full 55,403 + 508 lossless byte-for-byte roundtrip directly from baseline."""
        tree_dir = CANONICAL_TREE_DIR
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

            # Assert exact canonical hashes
            expected_ret = "28c2c8fbf6f31b8a56f8415328c94fc3aebed39dbe96e114096193602c4dd533"
            expected_sup = "49bbeaf94ce5cca88634fb17fb62e3b6a88b4032f86d769dc54ad4285e91e58c"

            self.assertEqual(ret_sha, expected_ret)
            self.assertEqual(sup_sha, expected_sup)

    def test_edit_propagation(self):
        """Verify human edits in markdown propagate cleanly to built output."""
        base_tree_dir = CANONICAL_TREE_DIR

        with tempfile.TemporaryDirectory() as tmp_dir:
            copied_tree = os.path.join(tmp_dir, "en")
            shutil.copytree(base_tree_dir, copied_tree)

            as_file = os.path.join(copied_tree, "dialogue/bunka/as_s6.md")
            with open(as_file, "r", encoding="utf-8") as f:
                content = f.read()

            self.assertIn("I came to visit.", content)
            edited_content = content.replace(
                "I came to visit.",
                "I came to visit the festival!",
            ).replace(
                "| `main:g0001:l0000` | ready | I came to visit the festival! |  |",
                "| `main:g0001:l0000` | ready | I came to visit the festival! | Contributor updated note |",
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
                        found = True
                        break
            self.assertTrue(found, "Edited row should be present in built JSONL")

    def test_validation(self):
        """Verify that validate_tree passes on canonical tree."""
        self.assertTrue(validate_tree(CANONICAL_TREE_DIR))

    def test_graphics_catalog(self):
        """Verify graphics catalog exists and has zero Japanese text."""
        cat_md = os.path.join(CANONICAL_TREE_DIR, "graphics/image_text_catalog.md")
        cat_jsonl = os.path.join(CANONICAL_TREE_DIR, "graphics/image_text_catalog.jsonl")

        self.assertTrue(os.path.exists(cat_md))
        self.assertTrue(os.path.exists(cat_jsonl))

        with open(cat_md, "r", encoding="utf-8") as f:
            md_text = f.read()
        self.assertFalse(bool(CJK_PATTERN.search(md_text)))

        with open(cat_jsonl, "r", encoding="utf-8") as f:
            jsonl_text = f.read()
        self.assertFalse(bool(CJK_PATTERN.search(jsonl_text)))

    def test_no_meta_dir(self):
        """Verify that duplicate _meta/ directory is removed."""
        meta_dir = os.path.join(CANONICAL_TREE_DIR, "_meta")
        self.assertFalse(os.path.exists(meta_dir), "_meta directory should not exist")


if __name__ == "__main__":
    unittest.main()
