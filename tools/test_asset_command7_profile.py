"""Invented boundary fixtures for the finite ordinary command-7 background profile."""
import copy
import hashlib
import struct
import unittest
from unittest import mock

from tools.assets import bs_cohort_profile as profile
from tools.port.school_day_media import _validate_command7_route


def rows():
    a = profile.COMMAND7_ASSETS[0]
    return [{"kind": "file", "path": a["path"], "size": a["size"], "sha256": a["sha256"],
             "location": {"lba": a["lba"], "extent_bytes": a["size"], "record_lba": a["record_lba"],
                          "record_offset": a["record_offset"], "sectors": [{}] * a["file_sectors"]}}]


def validate(rows_, assets=None, words=None):
    return profile._validate_asset_inventory(rows_, assets or profile.COMMAND7_ASSETS,
        words or profile.COMMAND7_TABLE_WORDS, "girl_index", (7,))


class Command7ProfileTests(unittest.TestCase):
    def test_exact_selector_seven_and_iso_row(self):
        result = validate(rows())
        self.assertEqual(result[0][0]["girl_index"], 7)
        self.assertEqual(result[0][0]["lba"], profile.COMMAND7_TABLE_WORDS[7])
        self.assertEqual(len(profile.COMMAND7_TABLE_RAW), 32)

    def test_substitution_and_forged_extent_are_rejected(self):
        for field, bad in [("path", "invented-substitute.BS"), ("size", 1), ("sha256", "0" * 64)]:
            with self.subTest(field=field):
                altered = rows(); altered[0][field] = bad
                with self.assertRaises(ValueError): validate(altered)
        for field in ("lba", "record_lba", "record_offset", "extent_bytes"):
            with self.subTest(field=field):
                altered = rows(); altered[0]["location"][field] += 1
                with self.assertRaises(ValueError): validate(altered)
        altered = rows(); altered[0]["location"]["sectors"].pop()
        with self.assertRaises(ValueError): validate(altered)

    def test_selector_or_executable_remap_is_rejected(self):
        assets = [dict(profile.COMMAND7_ASSETS[0], girl_index=6)]
        with self.assertRaisesRegex(ValueError, "selector indices"): validate(rows(), assets)
        words = list(profile.COMMAND7_TABLE_WORDS); words[7] += 1
        with self.assertRaisesRegex(ValueError, "table order"): validate(rows(), words=words)

    def test_rehashed_executable_with_changed_table_or_offset_is_rejected(self):
        data = bytearray(profile.EXE_TABLE_OFFSET + 32)
        data[profile.EXE_TABLE_OFFSET:] = profile.COMMAND7_TABLE_RAW
        for change in (False, True):
            fake = bytearray(data)
            if change: fake[-1] ^= 1
            with mock.patch.object(profile, "EXE_SHA256", hashlib.sha256(fake).hexdigest()):
                if change:
                    with self.assertRaisesRegex(ValueError, "table bytes"):
                        profile._validate_executable_bytes(fake, profile.EXE_TABLE_OFFSET, profile.COMMAND7_TABLE_RAW, profile.COMMAND7_TABLE_WORDS)
                else:
                    profile._validate_executable_bytes(fake, profile.EXE_TABLE_OFFSET, profile.COMMAND7_TABLE_RAW, profile.COMMAND7_TABLE_WORDS)
                    with self.assertRaisesRegex(ValueError, "table bytes"):
                        profile._validate_executable_bytes(fake, profile.EXE_TABLE_OFFSET + 4, profile.COMMAND7_TABLE_RAW, profile.COMMAND7_TABLE_WORDS)

    def test_raw_iso_both_endian_copies_and_offset_are_checked(self):
        expected = {"path": "invented.BS", "record_lba": 12, "record_offset": 40, "record_length": 60,
                    "identifier": "INVENTED.BS;1", "lba": 99, "size": 1234}
        payload = bytearray(2048); record = bytearray(60)
        record[0] = 60; struct.pack_into('<I', record, 2, 99); struct.pack_into('>I', record, 6, 99)
        struct.pack_into('<I', record, 10, 1234); struct.pack_into('>I', record, 14, 1234)
        name = b'INVENTED.BS;1'; record[32] = len(name); record[33:33 + len(name)] = name
        payload[40:100] = record
        class Source:
            def iter_raw_sectors(self, entry, track_hash):
                return iter([{"form": 1, "subheader_valid": True, "payload_size": 2048, "payload": bytes(payload)}])
        profile._validate_iso_directory_record(Source(), expected)
        for offset in (2, 6, 10, 14, 33):
            payload[40 + offset] ^= 1
            with self.subTest(offset=offset), self.assertRaises(ValueError):
                profile._validate_iso_directory_record(Source(), expected)
            payload[40 + offset] ^= 1
        with self.assertRaises(ValueError):
            profile._validate_iso_directory_record(Source(), dict(expected, record_offset=41))

    def test_native_first_event_and_sequence_are_exact(self):
        route = {"command": 7, "actual_last_command": 7, "first_day_event": [0, 7, 0, 15],
                 "sequence_slot": 0, "sd_set_id": 16, "actors": [96]}
        _validate_command7_route(route)
        for key, value in [("first_day_event", [0, 7, 1, 15]), ("first_day_event", [False, 7, 0, 15]),
                           ("sequence_slot", 1), ("sequence_slot", False), ("sd_set_id", 3),
                           ("actual_last_command", 0), ("actors", [97])]:
            with self.subTest(key=key, value=value), self.assertRaises(ValueError):
                _validate_command7_route(dict(route, **{key: value}))


if __name__ == '__main__': unittest.main()
