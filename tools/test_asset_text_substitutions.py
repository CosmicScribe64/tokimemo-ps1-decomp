"""Synthetic contract-parser tests; no original text or assets are included."""
import tempfile
import unittest
from pathlib import Path

from tools.assets.text_substitutions import (
    SubstitutionProofError,
    _marker_symbol_pairs,
    _safe_file,
    _zero_initialized_bytes,
)


class SymbolicSubstitutionTests(unittest.TestCase):
    def test_marker_branches_capture_exact_buffer_mapping(self):
        source = "\n".join(
            f"if (line[out_index] == '{marker}') {{\n"
            f"    if (strlen({symbol}) > 0) {{}}\n"
            "}"
            for marker, symbol in (("S", "name_code_s"), ("O", "name_code_o"),
                                   ("W", "name_code_w"), ("A", "name_code_a"),
                                   ("M", "name_code_m"))
        )
        self.assertEqual(_marker_symbol_pairs(source), [
            ("S", "name_code_s"), ("O", "name_code_o"), ("W", "name_code_w"),
            ("A", "name_code_a"), ("M", "name_code_m")])

    def test_next_branch_terminates_previous_branch(self):
        source = ("if (line[out_index] == 'S') { strlen(name_code_s); } "
                  "else if (line[out_index] == 'O') { strlen(name_code_o); }")
        self.assertEqual(_marker_symbol_pairs(source), [("S", "name_code_s"),
                                                        ("O", "name_code_o")])

    def test_ambiguous_or_missing_symbol_is_rejected(self):
        for source in (
            "if (line[out_index] == 'S') { strlen(name_code_s); strlen(name_code_o); }",
            "if (line[out_index] == 'S') { strlen(other_buffer); }",
        ):
            with self.subTest(source=source), self.assertRaises(SubstitutionProofError):
                _marker_symbol_pairs(source)

    def test_evidence_symlink_cannot_escape_selected_root(self):
        with tempfile.TemporaryDirectory() as temp, tempfile.TemporaryDirectory() as external:
            root = Path(temp).resolve()
            outside = Path(external) / "source.c"
            outside.write_text("fixture", encoding="utf-8")
            (root / "source.c").symlink_to(outside)
            with self.assertRaises(SubstitutionProofError):
                _safe_file(root, "source.c")

    def test_initialized_zero_buffer_requires_exact_label_extent(self):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / "buffer.s"
            path.write_text("dlabel name_code_s\n.byte 0, 0, 0, 0\nenddlabel name_code_s\n",
                            encoding="utf-8")
            self.assertEqual(_zero_initialized_bytes(path, "name_code_s", 4), b"\0" * 4)
            with self.assertRaises(SubstitutionProofError):
                _zero_initialized_bytes(path, "name_code_s", 3)
            path.write_text("dlabel name_code_s\n.space 4\nenddlabel name_code_s\n",
                            encoding="utf-8")
            with self.assertRaises(SubstitutionProofError):
                _zero_initialized_bytes(path, "name_code_s", 4)


if __name__ == "__main__":
    unittest.main()
