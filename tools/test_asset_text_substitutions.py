"""Synthetic contract-parser tests; no original text or assets are included."""
import tempfile
import unittest
from pathlib import Path

from tools.assets.text_substitutions import (
    SubstitutionProofError,
    _marker_symbol_pairs,
    _safe_file,
)


class SymbolicSubstitutionTests(unittest.TestCase):
    def test_marker_branches_capture_exact_buffer_mapping(self):
        source = "\n".join(
            f"if (line[out_index] == '{marker}') {{\n"
            f"    if (strlen({symbol}) > 0) {{}}\n"
            "}"
            for marker, symbol in (("S", "D_800CA23C"), ("O", "D_800CA248"),
                                   ("W", "D_800CA250"), ("A", "D_800CA25C"),
                                   ("M", "D_800CA280"))
        )
        self.assertEqual(_marker_symbol_pairs(source), [
            ("S", "D_800CA23C"), ("O", "D_800CA248"), ("W", "D_800CA250"),
            ("A", "D_800CA25C"), ("M", "D_800CA280")])

    def test_next_branch_terminates_previous_branch(self):
        source = ("if (line[out_index] == 'S') { strlen(D_800CA23C); } "
                  "else if (line[out_index] == 'O') { strlen(D_800CA248); }")
        self.assertEqual(_marker_symbol_pairs(source), [("S", "D_800CA23C"),
                                                        ("O", "D_800CA248")])

    def test_ambiguous_or_missing_symbol_is_rejected(self):
        for source in (
            "if (line[out_index] == 'S') { strlen(D_800CA23C); strlen(D_800CA248); }",
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


if __name__ == "__main__":
    unittest.main()
