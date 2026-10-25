"""Invented layouts and original-input SPEAKTST fidelity regression checks."""
import copy
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess
import sys
import unittest
from unittest.mock import patch

from tools.assets.text import TextFormatError
from tools.assets.text_speaktst import (
    FLAGS, PROFILE_SHA256, SPEAKTST_PROFILES, parse_speaktst_layout,
    reconstruct_speaktst_layout, validate_speaktst_layout,
)

BASE = 0x80180000
PATH = "CDROM/SPEAKTST/INVENTED.BIN"


def fixture():
    data = bytearray(256)
    for offset, target in ((0, 32), (4, 48), (8, 64), (32, 96), (36, 100),
                           (64, 128), (68, 152)):
        struct.pack_into("<I", data, offset, BASE + target)
    data[48:51] = bytes((1, 1, 255))
    data[96:99] = "テ".encode("shift_jis") + b"\0"
    data[100:102] = b"\xff\0"
    data[128:176] = bytes(range(48))
    data[200:202] = b"\x12\x34"
    profile = {"path": PATH, "size": len(data), "text_records": 2,
               "metadata_array_offset": 200,
               "profile": {"id": "invented-layout", "load_address": BASE,
                           "sections": [{"id": "candidate-only", "text_table": BASE,
                                         "command_table": BASE + 4, "voice_table": BASE + 8,
                                         "group_count": 1}]}}
    return seal(data, profile)


def seal(data, profile):
    data = bytearray(data)
    data[-4:-2] = (sum(data[:-4]) & 65535).to_bytes(2, "big")
    data = bytes(data)
    profile = copy.deepcopy(profile)
    profile["sha256"] = hashlib.sha256(data).hexdigest()
    profile["profile"]["sha256"] = profile["sha256"]
    return data, profile


class SpeaktstTests(unittest.TestCase):
    def setUp(self):
        self.data, self.profile = fixture()
        self.bank = parse_speaktst_layout(self.data, PATH, self.profile)

    def reject_bank(self, bank):
        with self.assertRaises(TextFormatError):
            validate_speaktst_layout(self.data, PATH, bank, self.profile)

    def test_layout_roundtrip_and_opaque_semantics(self):
        self.assertEqual(reconstruct_speaktst_layout(json.loads(json.dumps(self.bank))), self.data)
        summary = validate_speaktst_layout(self.data, PATH, self.bank, self.profile)
        self.assertEqual((summary["text_records"], summary["control_records"], summary["unknown_tokens"]), (2, 1, 1))
        self.assertFalse(summary["semantic_coverage_complete"])
        self.assertEqual(self.bank["status"], "layout_only_inventory")
        self.assertEqual(self.bank["source"]["load_address"], BASE)
        for row in self.bank["records"]:
            self.assertFalse(row["runtime_usage_proven"])
            self.assertFalse(row["readiness"]["translation_ready"])
            self.assertIsNone(row["target_text"])
            self.assertNotIn("voice", row)
            self.assertNotIn("speaker", row)
            self.assertEqual(row["spans"]["record24"]["length"], 24)
            self.assertEqual(row["spans"]["metadata_byte"]["length"], 1)
            self.assertEqual(row["readiness"]["blockers"][:len(FLAGS)], list(FLAGS))
        self.assertNotIn("source_text", self.bank["controls"][0])

    def test_strict_unknown_source_and_source_token_spans(self):
        row = self.bank["records"][1]
        self.assertIsNone(row["source_text"])
        self.assertEqual(row["tokens"][0]["kind"], "unknown")
        self.assertEqual(row["tokens"][0]["source_offset"], 100)
        self.assertNotIn("\ufffd", json.dumps(self.bank, ensure_ascii=False))

    def test_identity_and_checksum_rejection(self):
        for value in (b"", self.data[:-1], bytes([self.data[0] ^ 1]) + self.data[1:]):
            with self.subTest(length=len(value)), self.assertRaises(TextFormatError):
                parse_speaktst_layout(value, PATH, self.profile)
        changed = bytearray(self.data)
        changed[-4] ^= 1
        profile = copy.deepcopy(self.profile)
        profile["sha256"] = profile["profile"]["sha256"] = hashlib.sha256(changed).hexdigest()
        with self.assertRaises(TextFormatError):
            parse_speaktst_layout(bytes(changed), PATH, profile)

    def test_pointer_stride_count_metadata_and_ff_rejection(self):
        variants = []
        changed = bytearray(self.data)
        struct.pack_into("<I", changed, 68, BASE + 151)
        variants.append(seal(changed, self.profile))
        changed = bytearray(self.data)
        struct.pack_into("<I", changed, 32, BASE + 300)
        variants.append(seal(changed, self.profile))
        for offset, value in ((48, 255), (50, 1)):
            changed = bytearray(self.data)
            changed[offset] = value
            variants.append(seal(changed, self.profile))
        for key, value in (("text_records", 1), ("metadata_array_offset", 255),
                           ("metadata_array_offset", -1), ("text_records", True)):
            profile = copy.deepcopy(self.profile)
            profile[key] = value
            variants.append((self.data, profile))
        for data, profile in variants:
            with self.subTest(), self.assertRaises(TextFormatError):
                parse_speaktst_layout(data, PATH, profile)

    def test_dropped_duplicate_and_extra_controls_opaque_reject(self):
        for key in ("controls", "opaque_ranges"):
            for mode in ("drop", "duplicate", "extra"):
                bank = copy.deepcopy(self.bank)
                if mode == "drop":
                    bank[key].pop()
                else:
                    row = copy.deepcopy(bank[key][0])
                    if mode == "extra":
                        row["unexpected"] = True
                    bank[key].append(row)
                with self.subTest(key=key, mode=mode):
                    self.reject_bank(bank)

    def test_all_record_fields_and_claims_rechecked(self):
        changes = (("id", "forged"), ("source_text", "invented"), ("raw_hex", "00"),
                   ("raw_sha256", "0" * 64), ("runtime_usage_proven", True),
                   ("target_text", "invented"), ("group", 9), ("line", 9))
        for key, value in changes:
            bank = copy.deepcopy(self.bank)
            bank["records"][0][key] = value
            with self.subTest(key=key):
                self.reject_bank(bank)
        for key in ("translation_ready", "status", "blockers"):
            bank = copy.deepcopy(self.bank)
            bank["records"][0]["readiness"][key] = True
            self.reject_bank(bank)
        bank = copy.deepcopy(self.bank)
        bank["coverage"]["runtime_execution_proven"] = True
        self.reject_bank(bank)
        bank = copy.deepcopy(self.bank)
        bank["controls"][0]["runtime_usage_proven"] = True
        self.reject_bank(bank)

    def test_token_source_offset_and_span_rejection(self):
        for key, value in (("offset", -1), ("offset", True), ("source_offset", 97),
                           ("raw_hex", "00"), ("kind", "glyph")):
            bank = copy.deepcopy(self.bank)
            bank["records"][1]["tokens"][0][key] = value
            with self.subTest(key=key):
                self.reject_bank(bank)

    def test_reconstruction_bounds_types_hash_gaps_conflicts_sources(self):
        for key, value in (("offset", -1), ("offset", True), ("offset", 999),
                           ("length", -1), ("length", 999), ("raw_hex", "XX"),
                           ("sha256", "0" * 64)):
            bank = copy.deepcopy(self.bank)
            bank["opaque_ranges"][0]["span"][key] = value
            with self.subTest(key=key), self.assertRaises(TextFormatError):
                reconstruct_speaktst_layout(bank)
        bank = copy.deepcopy(self.bank)
        bank["opaque_ranges"] = []
        with self.assertRaises(TextFormatError):
            reconstruct_speaktst_layout(bank)
        bank = copy.deepcopy(self.bank)
        span = bank["records"][0]["spans"]["record24"]
        span["raw_hex"] = "ff" * span["length"]
        span["sha256"] = hashlib.sha256(bytes.fromhex(span["raw_hex"])).hexdigest()
        with self.assertRaises(TextFormatError):
            reconstruct_speaktst_layout(bank)
        bank = copy.deepcopy(self.bank)
        bank["controls"][0]["source_sha256"] = "0" * 64
        with self.assertRaises(TextFormatError):
            reconstruct_speaktst_layout(bank)

    def test_profile_registry_immutable_and_pin_rechecked(self):
        with self.assertRaises(TypeError):
            SPEAKTST_PROFILES["new"] = self.profile
        first = next(iter(SPEAKTST_PROFILES.values()))
        with self.assertRaises(TypeError):
            first["profile"]["sections"][0]["text_table"] = 0
        with patch("tools.assets.text_speaktst.Path.read_bytes", return_value=b"[]"):
            with self.assertRaises(TextFormatError):
                parse_speaktst_layout(self.data, PATH, self.profile)
        shipped = Path(__file__).parent / "assets/text_speaktst_profiles.json"
        self.assertEqual(hashlib.sha256(shipped.read_bytes()).hexdigest(), PROFILE_SHA256)

    def test_reconstruction_rejects_detached_text_and_impossible_source_size(self):
        bank = copy.deepcopy(self.bank)
        bank["source"]["size"] = 10 ** 15
        with self.assertRaises(TextFormatError):
            reconstruct_speaktst_layout(bank)
        bank = copy.deepcopy(self.bank)
        row = bank["records"][0]
        row["raw_hex"] = "4142"
        row["raw_sha256"] = hashlib.sha256(b"AB").hexdigest()
        row["tokens"] = [{"offset": 0, "source_offset": row["source_offset"], "raw_hex": "4142"}]
        with self.assertRaises(TextFormatError):
            reconstruct_speaktst_layout(bank)

    def test_paths_input_types_and_canonical_override_reject(self):
        for path in ("../invented", "/invented", "a//b", "a\\b", ""):
            with self.subTest(path=path), self.assertRaises(TextFormatError):
                parse_speaktst_layout(self.data, path, self.profile)
        for value in (bytearray(self.data), None, "invented"):
            with self.assertRaises(TextFormatError):
                parse_speaktst_layout(value, PATH, self.profile)
        with self.assertRaises(TextFormatError):
            parse_speaktst_layout(self.data, next(iter(SPEAKTST_PROFILES)), self.profile)

    def test_optimized_rejection(self):
        code = """import copy, hashlib
from tools.test_asset_text_speaktst import fixture, PATH
from tools.assets.text_speaktst import parse_speaktst_layout, validate_speaktst_layout, reconstruct_speaktst_layout
from tools.assets.text import TextFormatError
data, profile = fixture()
bank = parse_speaktst_layout(data, PATH, profile)
bank['records'][0]['runtime_usage_proven'] = True
bad_data = bytearray(data)
bad_data[-4] ^= 1
bad_profile = copy.deepcopy(profile)
bad_profile['sha256'] = bad_profile['profile']['sha256'] = hashlib.sha256(bad_data).hexdigest()
bad_span = parse_speaktst_layout(data, PATH, profile)
bad_span['opaque_ranges'][0]['span']['offset'] = -1
for operation in (lambda: validate_speaktst_layout(data, PATH, bank, profile),
                  lambda: parse_speaktst_layout(bytes(bad_data), PATH, bad_profile),
                  lambda: reconstruct_speaktst_layout(bad_span)):
    try:
        operation()
    except TextFormatError:
        pass
    else:
        raise SystemExit(2)
"""
        result = subprocess.run([sys.executable, "-O", "-c", code], cwd=Path(__file__).parents[1],
                                env=os.environ.copy(), capture_output=True, check=False)
        self.assertEqual(result.returncode, 0, result.stderr.decode())

    def test_all_twelve_original_inputs(self):
        original = Path("/work/disc/files")
        total_records = total_controls = total_bytes = total_unknown = 0
        for path in SPEAKTST_PROFILES:
            data = (original / path).read_bytes()
            bank = parse_speaktst_layout(data, path)
            summary = validate_speaktst_layout(data, path, bank)
            total_records += summary["text_records"]
            total_controls += summary["control_records"]
            total_bytes += summary["reconstructed_bytes"]
            total_unknown += summary["unknown_tokens"]
        self.assertEqual((total_records, total_controls, total_bytes, total_unknown),
                         (16702, 12, 1253376, 4))


if __name__ == "__main__":
    unittest.main()
