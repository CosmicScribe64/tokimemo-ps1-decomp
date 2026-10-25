"""Invented fixed-slot UI data; no original game phrases or translations."""
import copy
import hashlib
from pathlib import Path
import tempfile
import unittest

from tools.assets.text import TextFormatError
from tools.assets.text_ui_tables import UI_TABLE_PROFILES, parse_ui_tables, verify_ui_source_proof


class UiTableTests(unittest.TestCase):
    def fixture(self):
        slots = [b"\x82\x60\0\x99\x98\x97\x96\x95", bytes(8),
                 b"\x82\x61\0" + bytes(5), b"\x82\x62\0" + bytes(5)]
        data = b"".join(slots) + b"\x12\x34\x56\x78"
        return data, {"id": "invented-ui", "path": "CDROM/INVENTED.NBN", "header_size": 0,
                      "load_address": 0x801A0000, "tables": [{"id": "labels", "address": 0x801A0000,
                          "kind": "fixed_strings", "count": 4, "stride": 8, "dimensions": [2, 2],
                          "context_axes": ["profile_index", "year_index"], "role": "display", "assembly_size": 34}],
                      "preserved_inter_table_ranges": [{"offset": 34, "length": 2,
                                                          "classification": "uninterpreted_original_bytes"}]}

    def test_slots_padding_empty_context_and_trailing_bytes_are_preserved(self):
        data, profile = self.fixture()
        parsed = parse_ui_tables(data, profile)
        self.assertEqual(parsed["coverage"]["records"], 4)
        self.assertEqual(parsed["coverage"]["empty_slots"], 1)
        self.assertTrue(parsed["coverage"]["full_slot_bytes_preserved"])
        self.assertEqual(parsed["records"][0]["padding_raw_hex"], "9998979695")
        self.assertEqual(parsed["records"][3]["context"], {"profile_index": 1, "year_index": 1, "girl_state_index": 1})
        self.assertFalse(parsed["records"][0]["original_layout"]["capacity_is_translation_limit"])
        self.assertEqual(parsed["table_extents"][0]["trailing_assembly_raw_hex"], "1234")
        self.assertEqual(parsed["preserved_inter_table_ranges"][0]["raw_hex"], "5678")

    def test_real_profiles_have_source_bounded_dimensions_and_original_pins(self):
        self.assertEqual(len(UI_TABLE_PROFILES), 2)
        main, joho = UI_TABLE_PROFILES
        self.assertEqual([(table["count"], table["stride"]) for table in main["tables"]], [(13, 17), (13, 17)])
        self.assertEqual([(table["count"], table["stride"]) for table in joho["tables"]],
                         [(13, 12), (4, 8), (12, 10), (36, 18), (72, 32)])
        self.assertEqual(joho["load_sectors"] * 2048, joho["size"])
        self.assertEqual(joho["iso_lba"], 0x9D5F)
        self.assertTrue(all(len(profile["sha256"]) == 64 and profile["source_pins"] for profile in UI_TABLE_PROFILES))

    def test_profile_index_eleven_maps_to_girl_state_twelve(self):
        data, profile = self.fixture()
        profile["tables"][0].update(count=12, dimensions=[12], context_axes=["profile_index"])
        profile["tables"][0].pop("assembly_size")
        profile.pop("preserved_inter_table_ranges")
        parsed = parse_ui_tables(bytes(12 * 8), profile)
        self.assertEqual(parsed["records"][11]["context"]["girl_state_index"], 12)

    def test_bad_hash_dimensions_and_unterminated_slots_are_rejected(self):
        data, profile = self.fixture()
        for change in ("hash", "dimensions", "terminator", "extent"):
            with self.subTest(change=change):
                modified = copy.deepcopy(profile)
                raw = data
                if change == "hash":
                    modified["sha256"] = "0" * 64
                elif change == "dimensions":
                    modified["tables"][0]["dimensions"] = [2, 3]
                elif change == "terminator":
                    raw = b"ABCDEFGH" + data[8:]
                else:
                    modified["tables"][0]["assembly_size"] = len(data) + 1
                with self.assertRaises(TextFormatError):
                    parse_ui_tables(raw, modified)

    def test_fixed_renderer_anomalies_are_reported_without_source_repair(self):
        data, profile = self.fixture()
        data = b"AB\0" + data[3:]
        profile["sha256"] = hashlib.sha256(data).hexdigest()
        parsed = parse_ui_tables(data, profile)
        self.assertEqual(parsed["records"][0]["text"]["source_decoded_text"], "AB")
        self.assertTrue(parsed["coverage"]["renderer_anomaly_records"])
        self.assertEqual(parsed["records"][0]["slot_raw_hex"], data[:8].hex())

    def test_source_pins_and_anchored_original_code_data_are_rechecked(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            sources, inputs = root / "sources", root / "inputs"
            data, profile = self.fixture()
            data_path = "asm/data/invented.data.s"
            code_path = "asm/matchings/main/invented.s"
            texts = {data_path: "dlabel slots\n/* 0 801A0000 */ .byte " +
                     ",".join(str(byte) for byte in data[:34]) + "\nenddlabel slots\n",
                     code_path: "glabel f\n/* 0 80041000 00000000 */ nop\n"
                     "/* 4 80041004 00000000 */ nop\nendlabel f\n"}
            for path, text in texts.items():
                target = sources / path
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_text(text)
            inputs.mkdir()
            (inputs / "SLPM_86.053").write_bytes(bytes(8))
            profile.update(sha256=hashlib.sha256(data).hexdigest(), source_pins={
                path: hashlib.sha256((sources / path).read_bytes()).hexdigest() for path in texts},
                code_proofs=[{"assembly_path": code_path, "source_path": "SLPM_86.053",
                              "source_sha256": hashlib.sha256(bytes(8)).hexdigest(),
                              "load_address": 0x80041000, "header_size": 0}])
            profile["tables"][0].update(assembly_path=data_path, assembly_label="slots")
            proof = verify_ui_source_proof(data, profile, sources, inputs)
            self.assertEqual(proof["code"][0]["instruction_words_verified"], 2)
            self.assertEqual(proof["data_labels"][0]["initialized_bytes_verified"], 34)
            (inputs / "SLPM_86.053").write_bytes(b"\x01" + bytes(7))
            with self.assertRaises(TextFormatError):
                verify_ui_source_proof(data, profile, sources, inputs)
            profile["code_proofs"][0]["source_sha256"] = hashlib.sha256(b"\x01" + bytes(7)).hexdigest()
            with self.assertRaises(TextFormatError):
                verify_ui_source_proof(data, profile, sources, inputs)
            (inputs / "SLPM_86.053").write_bytes(bytes(8))
            profile["code_proofs"][0]["source_sha256"] = hashlib.sha256(bytes(8)).hexdigest()
            path = sources / data_path
            path.write_text(path.read_text().replace(".byte 130,", ".byte 131,"))
            profile["source_pins"][data_path] = hashlib.sha256(path.read_bytes()).hexdigest()
            with self.assertRaises(TextFormatError):
                verify_ui_source_proof(data, profile, sources, inputs)

    def test_whitespace_only_slot_is_preserved_and_distinct_from_empty(self):
        data, profile = self.fixture()
        data = b"\x81\x40\0" + data[3:]
        parsed = parse_ui_tables(data, profile)
        self.assertEqual(parsed["records"][0]["source_slot_status"], "whitespace_only")
        self.assertEqual(parsed["records"][1]["source_slot_status"], "empty")
        self.assertEqual(parsed["coverage"]["whitespace_only_slots"], 1)

    def test_malformed_shift_jis_slot_is_rejected_as_source_encoding_error(self):
        data, profile = self.fixture()
        data = b"\x82\0" + data[2:]
        with self.assertRaisesRegex(TextFormatError, "unsupported source encoding"):
            parse_ui_tables(data, profile)


if __name__ == "__main__":
    unittest.main()
