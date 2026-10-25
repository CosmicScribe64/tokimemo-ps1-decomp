"""Synthetic contract tests for the T-29011 SERIFU adapter."""
import importlib.util
import pathlib
import tempfile
import unittest
from unittest.mock import patch

MODULE = pathlib.Path(__file__).resolve().parents[4] / "tools" / "port" / "dialogue_content.py"
SPEC = importlib.util.spec_from_file_location("dialogue_content", MODULE)
dialogue_content = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(dialogue_content)

PATH = "CDROM/SERIFU/SERIFU.BIN"
HASH = "5d6e1014c69769f18f7877ad801f04f872a9829c05d4c8509602adbccd5f22ee"
SECTION = "table-0"
PROFILE = "serifu-serifu"


def _record(line, command, text=None):
    item = {"id": f"talk:{PATH}:{SECTION}:g0000:l{line:04d}",
            "section": SECTION, "group": 0, "line": line,
            "command": {"value": command, "name": "end" if command == 255 else "choice_two",
                        "offset": 0x20 + line}}
    if text is not None:
        item["text"] = {"raw_hex": text, "offset": 0x80 + line,
                        "tokens": [{"kind": "glyph", "raw_hex": "8140"},
                                   {"kind": "separator", "role": "choice_option",
                                    "code": "q", "raw_hex": "71"},
                                   {"kind": "glyph", "raw_hex": "8140"}],
                        "unknown_token_count": 0}
    return item


def _catalog(records):
    return {"schema": "tokimemo.text.catalog.v1", "files": [{
        "path": PATH, "status": "exported",
        "script": {"schema": "tokimemo.text.v1",
                   "profile_id": PROFILE,
                   "source": {"path": PATH, "sha256": HASH, "size": 65536},
                   "records": records}}]}


class DialogueContentTests(unittest.TestCase):
    def test_preserves_stable_id_raw_bytes_controls_and_voice_pairs(self):
        row = _record(0, 8, "8140718140")
        row["voice"] = {"speaker": "runtime_current_girl", "pairs": [
            {"variant": 0, "clip_start": 0x1234, "clip_end": 0x5678, "raw_hex": "3412000078560000"}]}
        result = dialogue_content.project_catalog(
            _catalog([row, _record(1, 255)]), PATH, SECTION)
        actual = result["groups"][0]["records"][0]
        self.assertEqual(actual["id"], row["id"])
        self.assertEqual(actual["text"]["raw_hex"], "8140718140")
        self.assertEqual(actual["text"]["tokens"][1]["role"], "choice_option")
        self.assertEqual(actual["voice"]["pairs"][0]["clip_start"], 0x1234)
        self.assertFalse(result["semantic_coverage_complete"])

    def test_requires_one_final_terminator(self):
        with self.assertRaisesRegex(dialogue_content.DialogueContentError, "final 0xFF"):
            dialogue_content.project_catalog(
                _catalog([_record(0, 255), _record(1, 255)]), PATH, SECTION)

    def test_rejects_noncontiguous_line_cursor(self):
        with self.assertRaisesRegex(dialogue_content.DialogueContentError, "contiguous"):
            dialogue_content.project_catalog(
                _catalog([_record(0, 8, "8140718140"), _record(2, 255)]), PATH, SECTION)

    def test_rejects_unsupported_catalog_and_path_escape(self):
        with self.assertRaisesRegex(dialogue_content.DialogueContentError, "catalog.v1"):
            dialogue_content.project_catalog({}, PATH, SECTION)
        with self.assertRaisesRegex(dialogue_content.DialogueContentError, "normalized"):
            dialogue_content.project_catalog(_catalog([]), "CDROM/SERIFU/../x.BIN", SECTION)

    def test_rejects_embedded_nul_or_oversized_payload(self):
        with self.assertRaisesRegex(dialogue_content.DialogueContentError, "embedded terminator"):
            dialogue_content.project_catalog(
                _catalog([_record(0, 8, "810000"), _record(1, 255)]), PATH, SECTION)
        with self.assertRaisesRegex(dialogue_content.DialogueContentError, "exceeds bounds"):
            dialogue_content.project_catalog(
                _catalog([_record(0, 8, "81" * (dialogue_content.MAX_RAW_BYTES + 1)),
                          _record(1, 255)]), PATH, SECTION)

    def test_rejects_duplicate_or_unbound_stable_ids(self):
        records = [_record(0, 8, "8140718140"), _record(1, 255)]
        records[1]["id"] = records[0]["id"]
        with self.assertRaisesRegex(dialogue_content.DialogueContentError, "duplicated"):
            dialogue_content.project_catalog(_catalog(records), PATH, SECTION)

    def test_rejects_untrusted_profile_hash_and_section(self):
        catalog = _catalog([_record(0, 255)])
        catalog["files"][0]["script"]["source"]["sha256"] = "a" * 64
        with self.assertRaisesRegex(dialogue_content.DialogueContentError, "checksum"):
            dialogue_content.project_catalog(catalog, PATH, SECTION)
        with self.assertRaisesRegex(dialogue_content.DialogueContentError, "canonical source profile"):
            dialogue_content.project_catalog(_catalog([_record(0, 255)]), PATH, "made-up-section")

    def test_rejects_output_traversal_before_publication(self):
        with tempfile.TemporaryDirectory() as root:
            escaped = pathlib.Path(root) / ".asset-cache" / "port" / ".." / "escaped" / "dialogue.json"
            with self.assertRaisesRegex(dialogue_content.DialogueContentError, "parent traversal"):
                dialogue_content.write_pack(pathlib.Path("unused"), PATH, SECTION,
                                            pathlib.Path("unused-input"), escaped)
            self.assertFalse(pathlib.Path(root, ".asset-cache", "escaped", "dialogue.json").exists())

    def test_rejects_physical_output_through_symlinked_cache_root(self):
        with tempfile.TemporaryDirectory() as directory:
            root = pathlib.Path(directory) / "repo"
            outside = pathlib.Path(directory) / "outside"
            (root / ".asset-cache").mkdir(parents=True)
            outside.mkdir()
            (root / ".asset-cache" / "port").symlink_to(outside, target_is_directory=True)
            physical_output = outside / "dialogue.json"
            with patch.object(dialogue_content, "_repository_root", return_value=root):
                with self.assertRaisesRegex(dialogue_content.DialogueContentError, "symlinks"):
                    dialogue_content.write_pack(pathlib.Path("unused"), PATH, SECTION,
                                                pathlib.Path("unused-input"), physical_output)
            self.assertFalse(physical_output.exists())


if __name__ == "__main__":
    unittest.main()
