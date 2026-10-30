"""Synthetic contract tests for the T-39016 text layer pack builder (no game data)."""
import hashlib
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest

MODULE = Path(__file__).with_name("build_text_pack.py")
SPEC = importlib.util.spec_from_file_location("build_text_pack", MODULE)
builder = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(builder)


class StubFont:
    """Supports every mapped code except one deliberately missing glyph."""

    metadata = {"glyph_bank_sha256": "0" * 64}

    def __init__(self, missing=()):
        self.missing = set(missing)

    def supported(self, code):
        return code not in self.missing


def _sha12(raw):
    return hashlib.sha256(raw).hexdigest()[:12]


class BuildTextPackTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="text-layer-test-")
        self.root = Path(self.temp.name)
        self.files = self.root / "files"
        (self.files / "CDROM/EXEDIR").mkdir(parents=True)
        self.executable = b"\0" * 16 + b"\x82\xa0\x82\xa2\0" + b"\0" * 11
        (self.files / "SLPM_86.053").write_bytes(self.executable)
        (self.files / "CDROM/EXEDIR/X.EXN").write_bytes(b"synthetic overlay")
        self.physical = f"SLPM_86.053:00000010:{_sha12(self.executable[16:21])}"
        self.returned = [
            {"id": self.physical, "target_text": "Hi.", "protected_tokens": [], "review_status": "ready_for_review"},
            {"id": "talk:CDROM/EXEDIR/X.EXN:main:g0000:l0000", "target_text": "Two[[T001:row]]rows",
             "protected_tokens": [{"marker": "[[T001:row]]", "raw_hex": "20", "kind": "separator"}],
             "review_status": "ready_for_review"},
            {"id": "SLPM_86.053:00000010:000000000000", "target_text": "Stale", "protected_tokens": []},
            {"id": "c:UNIT:80010000:1", "target_text": "%d pts", "protected_tokens": [],
             "translator_note": builder.ASCII_LITERAL_NOTE},
            {"id": "c:UNIT:80010000:2", "target_text": "Café", "protected_tokens": []},
            {"id": "c:UNIT:80010000:3", "target_text": None, "protected_tokens": []},
            {"id": "c:UNIT:80010000:4", "target_text": "Lost[[T001:row]]", "protected_tokens": []},
        ]
        self.supplemental = [
            {"id": "supplemental:CDROM/EXEDIR/X.EXN:0000", "translation": "A[[T001:row]]B",
             "protected_tokens": "[{\"marker\": \"[[T001:row]]\", \"kind\": \"control\"}]"},
            {"id": "supplemental:CDROM/EXEDIR/X.EXN:0010", "translation": "Plain"},
        ]
        self.decisions = {"text_approval": {
            "approver": "user", "date": "2026-10-29", "instruction": "approved", "source": "chat",
            "scope": "every record in translations/en/returned-v3.jsonl and translations/en/supplemental.jsonl (9 records at this date)"}}

    def tearDown(self):
        self.temp.cleanup()

    def _write(self):
        paths = []
        for name, rows in (("returned.jsonl", self.returned), ("supplemental.jsonl", self.supplemental)):
            path = self.root / name
            path.write_text("".join(json.dumps(row) + "\n" for row in rows), encoding="utf-8")
            paths.append(path)
        decisions = self.root / "decisions.json"
        decisions.write_text(json.dumps(self.decisions), encoding="utf-8")
        return paths[0], paths[1], decisions

    def test_builds_source_keyed_entries_with_explicit_rejections(self):
        pack, coverage = builder.build(*self._write(), self.files, font=StubFont())
        entry = pack["entries"][self.physical]
        self.assertEqual(entry["offset"], 16)
        self.assertEqual(entry["raw_sha256"], hashlib.sha256(self.executable[16:21]).hexdigest())
        self.assertEqual(entry["segments"], [["g", "826782898144"]])
        rows = pack["entries"]["talk:CDROM/EXEDIR/X.EXN:main:g0000:l0000"]
        self.assertEqual([segment[0] for segment in rows["segments"]], ["g", "t", "g"])
        self.assertEqual(rows["segments"][1], ["t", "20", "separator"])
        self.assertEqual(rows["container"], "CDROM/EXEDIR/X.EXN")
        self.assertIn("SLPM_86.053", pack["containers"])
        self.assertEqual(coverage["records"], 9)
        self.assertEqual(coverage["representable"], 3)
        self.assertEqual(coverage["rejected"], {
            "ascii_format_literal_not_glyph_text": 1, "no_translation": 1,
            "protected_token_bytes_unavailable": 1, "protected_token_mismatch": 1,
            "source_bytes_mismatch": 1,
            "unsupported_character": 1})
        self.assertEqual(coverage["unsupported_characters"], {"U+00E9": 1})
        self.assertNotIn("supplemental:CDROM/EXEDIR/X.EXN:0000", pack["entries"])
        self.assertIn("supplemental:CDROM/EXEDIR/X.EXN:0010", pack["entries"])

    def _physical_rows(self, sources):
        """Lay `sources` (bytes with NUL) out in a fresh executable; return ids."""
        data = bytearray(b"\0" * 16)
        ids = []
        for raw in sources:
            ids.append(f"SLPM_86.053:{len(data):08x}:{_sha12(raw)}")
            data += raw
        (self.files / "SLPM_86.053").write_bytes(bytes(data))
        return ids

    def _count(self, total):
        self.decisions["text_approval"]["scope"] = self.decisions["text_approval"]["scope"].replace(
            "9 records", f"{total} records")

    def test_single_byte_renderer_controls_need_protected_tokens(self):
        # Even-phase ASCII has its own meaning in sndisp/sndi: ' ' breaks the
        # line, 'P' expands the player name, and '%s' is a format argument.
        name, line_break, directive, covered, stray, flagged = self._physical_rows([
            b"\x89\xb4\x82\xcdP\x81\x42\0",
            b" \x93\xfc\0",
            b"%s\x81\x63\0",
            b"\x82\xa0 \x82\xa2\0",
            b"\x82\xa0\x82\xa2\0",
            b"\x82\xa0\x82\xa4\0",
        ])
        token = {"marker": "[[T001:row]]", "raw_hex": "20", "kind": "row_separator"}
        self.returned = [
            {"id": name, "target_text": "I'm P.", "protected_tokens": []},
            {"id": line_break, "target_text": "Line", "protected_tokens": []},
            {"id": directive, "target_text": "Uh, %s...", "protected_tokens": []},
            {"id": covered, "target_text": "A[[T001:row]]B", "protected_tokens": [token]},
            {"id": stray, "target_text": "A[[T001:row]]B", "protected_tokens": [token]},
            {"id": flagged, "target_text": "Fine", "protected_tokens": [],
             "review_status": "unresolved: renderer_tokenization_unavailable"},
            {"id": "c:UNIT:80010000:5", "target_text": "Uh, %-2d...", "protected_tokens": []},
        ]
        self.supplemental = []
        self._count(7)
        pack, coverage = builder.build(*self._write(), self.files, font=StubFont())
        self.assertEqual(list(pack["entries"]), [covered])
        self.assertEqual(pack["entries"][covered]["segments"],
                         [["g", "8260"], ["t", "20", "row_separator"], ["g", "8261"]])
        self.assertNotIn("raw", pack["entries"][covered])
        self.assertEqual(coverage["rejected"], {
            "format_directive_untokenized": 2, "protected_token_not_in_source": 1,
            "renderer_tokenization_unavailable": 1, "source_single_byte_control_untokenized": 2})

    def test_cited_renderer_clears_only_the_tokenization_flag(self):
        clean, controlled = self._physical_rows([b"\x81\x69\0", b"P\x81\x69\0"])
        flag = "unresolved: renderer_tokenization_unavailable"
        self.returned = [
            {"id": clean, "target_text": "(", "protected_tokens": [], "review_status": flag},
            {"id": controlled, "target_text": "P(", "protected_tokens": [], "review_status": flag},
        ]
        self.supplemental = []
        self._count(2)
        saved = dict(builder.RENDERER_PROVEN)
        try:
            builder.RENDERER_PROVEN.clear()
            builder.RENDERER_PROVEN.update({clean: "cited", controlled: "cited"})
            pack, coverage = builder.build(*self._write(), self.files, font=StubFont())
        finally:
            builder.RENDERER_PROVEN.clear()
            builder.RENDERER_PROVEN.update(saved)
        self.assertEqual(list(pack["entries"]), [clean])
        self.assertEqual(pack["entries"][clean]["context_status"], flag)
        self.assertEqual(coverage["renderer_proven_by_citation"], {clean: "cited"})
        self.assertEqual(coverage["rejected"], {"source_single_byte_control_untokenized": 1})

    def test_source_control_walk_follows_shift_jis_units(self):
        reason = builder.source_control_reason
        self.assertIsNone(reason(b"\x82\x50\x82\x47\0", []))  # trail bytes 'P'/'G' are not controls
        self.assertEqual(reason(b"\xb1\0", []), "source_single_byte_control_untokenized")  # half-width kana
        self.assertEqual(reason(b"\x82\0", []), "source_single_byte_control_untokenized")  # truncated lead
        self.assertIsNone(reason(b"P\x82\xa0 \0", [{"raw_hex": "50"}, {"raw_hex": "20"}]))
        self.assertEqual(reason(b" \x82\xa0 \0", [{"raw_hex": "20"}]), "source_single_byte_control_untokenized")
        # A recorded token offset binds the token to that position only.
        self.assertIsNone(reason(b"\x82\xa0 \0", [{"raw_hex": "20", "offset": 2}]))
        self.assertEqual(reason(b" \x82\xa0\0", [{"raw_hex": "20", "offset": 2}]),
                         "source_single_byte_control_untokenized")
        self.assertEqual(reason(b"\x82\xa0\0", [{"raw_hex": "20", "offset": 2}]), "protected_token_not_in_source")

    def test_deliberate_substitutions_are_counted(self):
        self.returned[0]["target_text"] = "\"Hi.\""
        _, coverage = builder.build(*self._write(), self.files, font=StubFont())
        self.assertEqual(coverage["substituted_characters"], {"U+0022->U+201D": 2})

    def test_glyph_missing_from_original_font_is_rejected(self):
        _, coverage = builder.build(*self._write(), self.files, font=StubFont(missing={0x8144}))
        self.assertEqual(coverage["rejected"]["unsupported_character"], 2)

    def test_approval_must_cover_the_records_read(self):
        self.decisions["text_approval"]["scope"] = self.decisions["text_approval"]["scope"].replace("9 records", "8 records")
        with self.assertRaises(builder.PackError):
            builder.build(*self._write(), self.files, font=StubFont())
        del self.decisions["text_approval"]
        with self.assertRaises(builder.PackError):
            builder.build(*self._write(), self.files, font=StubFont())

    def test_duplicate_ids_and_missing_containers(self):
        self.returned.append(dict(self.returned[0]))
        self.decisions["text_approval"]["scope"] = self.decisions["text_approval"]["scope"].replace("9 records", "10 records")
        with self.assertRaises(builder.PackError):
            builder.build(*self._write(), self.files, font=StubFont())
        self.returned.pop()
        self.returned[1]["id"] = "talk:CDROM/EXEDIR/MISSING.EXN:main:g0000:l0000"
        self.decisions["text_approval"]["scope"] = self.decisions["text_approval"]["scope"].replace("10 records", "9 records")
        _, coverage = builder.build(*self._write(), self.files, font=StubFont())
        self.assertEqual(coverage["rejected"]["container_missing"], 1)

    def test_original_executable_is_required_without_test_font(self):
        with self.assertRaises(builder.PackError):
            builder.build(*self._write(), self.files)

    def test_code_mapping_uses_full_width_original_units(self):
        self.assertEqual(builder.code_for("A"), 0x8260)
        self.assertEqual(builder.code_for("z"), 0x829A)
        self.assertEqual(builder.code_for("0"), 0x824F)
        self.assertEqual(builder.code_for(" "), 0x8140)
        self.assertIsNone(builder.code_for("é"))

    def test_cli_refuses_existing_output(self):
        returned, supplemental, decisions = self._write()
        status = builder.main(["--returned", str(returned), "--supplemental", str(supplemental),
                               "--decisions", str(decisions), "--files", str(self.files),
                               "--output", str(self.root)])
        self.assertEqual(status, 2)


if __name__ == "__main__":
    unittest.main()
