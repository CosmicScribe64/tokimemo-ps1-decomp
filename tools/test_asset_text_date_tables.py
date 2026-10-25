import unittest

from tools.assets.text import TextFormatError
from tools.assets.text_date_tables import DATE_TABLE_PROFILE, parse_date_tables


class DateTableTests(unittest.TestCase):
    def synthetic(self):
        base = 0x80132000
        stride = 33
        slots = [b"\x82\xa0\0" + bytes(30), bytes(stride),
                 b"\x82\xa2\0" + bytes(30), b"\x82\xa4\0" + bytes(30),
                 bytes(stride), b"\x82\xa6\0" + bytes(30),
                 b"\x82\xa8\0" + bytes(30), bytes(stride),
                 b"\x82\xaa\0" + bytes(30), b"\x82\xac\0" + bytes(30)]
        data = b"".join(slots)
        profile = {
            "id": "fixture", "path": "CDROM/EXEDIR/DATE.EXN",
            "load_address": base, "header_size": 0,
            "tables": [
                {"id": "place_names", "address": base, "count": 2,
                 "kind": "fixed_strings", "stride": stride, "valid_count": 1},
                {"id": "gift_names", "address": base + 2 * stride, "count": 8,
                 "kind": "fixed_strings", "stride": stride, "dimensions": [2, 2, 2]},
            ],
        }
        return data, profile

    def test_preserves_empty_slots_padding_and_context(self):
        data, profile = self.synthetic()
        parsed = parse_date_tables(data, profile)
        places = [row for row in parsed["records"] if row["table"] == "place_names"]
        gifts = [row for row in parsed["records"] if row["table"] == "gift_names"]
        self.assertEqual(len(places), 2)
        self.assertEqual(places[0]["context"], {
            "place_index": 0, "valid_place_id": True, "slot_status": "valid_place"})
        self.assertEqual(places[1]["context"]["slot_status"], "reserved")
        self.assertEqual(places[1]["slot_raw_hex"], "00" * 33)
        self.assertEqual(places[0]["padding_raw_hex"], "00" * 30)
        self.assertEqual(gifts[7]["context"], {
            "girl_index": 1, "year_index": 1, "answer_index": 1,
            "source_coordinates": [1, 1, 1]})
        self.assertTrue(parsed["coverage"]["raw_slots_preserved"])
        self.assertEqual(parsed["coverage"]["empty_slots"], 3)

    def test_rejects_source_hash_mismatch(self):
        data, profile = self.synthetic()
        profile["sha256"] = "0" * 64
        with self.assertRaises(TextFormatError):
            parse_date_tables(data, profile)

    def test_real_profile_carries_source_dimensions_and_pins(self):
        self.assertEqual([(table["count"], table["stride"]) for table in DATE_TABLE_PROFILE["tables"]],
                         [(22, 33), (99, 33)])
        self.assertEqual(DATE_TABLE_PROFILE["tables"][0]["valid_count"], 21)
        self.assertEqual(DATE_TABLE_PROFILE["tables"][1]["dimensions"], [11, 3, 3])
        self.assertEqual([table["assembly_size"] for table in DATE_TABLE_PROFILE["tables"]], [728, 3272])
        self.assertIn("src/ovl/date/girl_birthday.c", DATE_TABLE_PROFILE["source_pins"])
        self.assertIn("include/ovl/date.h", DATE_TABLE_PROFILE["source_pins"])
        self.assertEqual(len(DATE_TABLE_PROFILE["source_checks"]), 6)

    def test_real_trailing_bytes_have_neutral_extent_metadata(self):
        data = bytes(2 * 33 + 8 * 33)
        base = 0x80132000
        profile = {"id": "fixture", "path": "fixture.bin", "load_address": base,
                   "header_size": 0,
                   "tables": [{"id": "place_names", "address": base, "count": 2,
                               "kind": "fixed_strings", "stride": 33, "valid_count": 2,
                               "assembly_size": 68},
                              {"id": "gift_names", "address": base + 66, "count": 8,
                               "kind": "fixed_strings", "stride": 33, "dimensions": [2, 2, 2]}]}
        parsed = parse_date_tables(data, profile)
        self.assertEqual(parsed["table_extents"][0]["trailing_assembly_bytes_size"], 2)
        self.assertEqual(parsed["table_extents"][0]["trailing_assembly_bytes_raw_hex"], "0000")


if __name__ == "__main__":
    unittest.main()
