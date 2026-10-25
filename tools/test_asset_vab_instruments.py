"""Synthetic tests for the isolated VAB program/tone inventory candidate."""

from __future__ import annotations

import hashlib
from pathlib import Path
import struct
import sys
ROOT = next(parent for parent in Path(__file__).resolve().parents if (parent / "tools/assets/audio.py").is_file())
sys.path.insert(0, str(ROOT / "tools"))
import assets
import unittest

assets.__path__.insert(0, str(Path(__file__).resolve().parent / "assets"))
from assets.vab_instruments import VabInventoryError, inventory_vab


def _fixture() -> tuple[bytes, bytes]:
    programs = 1
    sizes_offset = 0x820 + programs * 0x200
    header = bytearray(sizes_offset + 0x200)
    header[:4] = b"pBAV"
    struct.pack_into("<III", header, 4, 7, 0x1234, len(header))
    struct.pack_into("<HHH", header, 0x12, 1, 1, 1)
    header[0x20:0x30] = bytes((1, 127, 4, 3, 64, 0, 0x34, 0x12, 0, 0, 0, 0, 0, 0, 0, 0))
    tone_offset = 0x820
    tone = bytearray(32)
    tone[:14] = bytes((1, 2, 100, 64, 60, 0, 0, 127, 0, 0, 0, 0, 2, 2))
    struct.pack_into("<HHHH", tone, 16, 0x8123, 0x4567, 0, 1)
    header[tone_offset:tone_offset + 32] = tone
    struct.pack_into("<H", header, sizes_offset + 2, 6)  # VAG(1): 48 bytes.
    body = bytes((0, 4)) + bytes(14) + bytes((0, 1)) + bytes(14) + bytes(16)
    struct.pack_into("<I", header, 0x0C, len(header) + len(body))
    return bytes(header), body


class VabInstrumentInventoryTests(unittest.TestCase):
    def test_sparse_last_program_uses_first_packed_tone_block(self) -> None:
        header, body = _fixture()
        changed = bytearray(header)
        changed[0x20 + 127 * 16:0x30 + 127 * 16] = changed[0x20:0x30]
        changed[0x20:0x30] = bytes(16)
        struct.pack_into("<H", changed, 0x820 + 20, 127)
        result = inventory_vab(bytes(changed), body)
        self.assertEqual([row["index"] for row in result["programs"]], [127])
        self.assertEqual(result["programs"][0]["tones"][0]["offset"], 0x820)

    def test_more_active_slots_than_blocks_rejected(self) -> None:
        header, body = _fixture()
        changed = bytearray(header)
        changed[0x20 + 127 * 16] = 1
        with self.assertRaisesRegex(VabInventoryError, "active program slots"):
            inventory_vab(bytes(changed), body)

    def test_bank_local_tone_to_sample_and_loop_metadata(self) -> None:
        header, body = _fixture()
        result = inventory_vab(header, body, header_path="synthetic.VH", body_path="synthetic.VB")
        self.assertEqual(result["bank"]["effective_tone_count"], 1)
        self.assertEqual(result["header_declared_file_size_bytes"], len(header) + len(body))
        self.assertEqual(result["paired_extent_bytes"], len(header) + len(body))
        tone = result["programs"][0]["tones"][0]
        self.assertEqual(tone["vag_index"], 1)
        self.assertEqual(tone["sample_offset"], 0)
        self.assertEqual(tone["sample_size_bytes"], 48)
        self.assertEqual(tone["native_sample_rate_hz"], None)
        self.assertEqual(tone["sample_loop_metadata"]["markers"][0]["kind"], "loop_start")
        self.assertEqual(tone["sample_loop_metadata"]["markers"][1]["kind"], "end")
        self.assertEqual(tone["sample_loop_metadata"]["source_block_flags"], [4, 1, 0])
        self.assertEqual(tone["sample_loop_metadata"]["trailing_source_bytes"], 16)

    def test_malformed_tone_owner_rejected(self) -> None:
        header, body = _fixture()
        changed = bytearray(header)
        struct.pack_into("<H", changed, 0x820 + 20, 2)
        with self.assertRaisesRegex(VabInventoryError, "owner field"):
            inventory_vab(bytes(changed), body)

    def test_tone_sample_outside_bank_rejected(self) -> None:
        header, body = _fixture()
        changed = bytearray(header)
        struct.pack_into("<H", changed, 0x820 + 22, 2)
        with self.assertRaisesRegex(VabInventoryError, "invalid VAG index"):
            inventory_vab(bytes(changed), body)

    def test_short_body_and_bad_alignment_rejected(self) -> None:
        header, body = _fixture()
        with self.assertRaisesRegex(VabInventoryError, "paired VB is short"):
            inventory_vab(header, body[:-1])
        with self.assertRaisesRegex(VabInventoryError, "does not match paired VH/VB extent"):
            inventory_vab(header, body + b"\0")
        changed = bytearray(header)
        size_offset = 0x820 + 0x200
        struct.pack_into("<H", changed, size_offset + 2, 1)  # VAG(1): 8 bytes.
        with self.assertRaisesRegex(VabInventoryError, "not aligned"):
            inventory_vab(bytes(changed), body)

    def test_declared_fsize_under_and_over_pair_are_rejected(self) -> None:
        header, body = _fixture()
        pair_size = len(header) + len(body)
        for declared_size in (pair_size - 1, pair_size + 1):
            with self.subTest(declared_size=declared_size):
                changed = bytearray(header)
                struct.pack_into("<I", changed, 0x0C, declared_size)
                with self.assertRaisesRegex(VabInventoryError, "does not match paired VH/VB extent"):
                    inventory_vab(bytes(changed), body)

    def test_valid_declared_pair_trailer_is_reported_and_hash_bound(self) -> None:
        header, body = _fixture()
        trailer = b"TAIL"
        changed = bytearray(header)
        struct.pack_into("<I", changed, 0x0C, len(header) + len(body) + len(trailer))
        result = inventory_vab(bytes(changed), body + trailer)
        self.assertEqual(result["body_trailing_bytes"], len(trailer))
        self.assertEqual(result["body_trailing_sha256"], hashlib.sha256(trailer).hexdigest())

    def test_output_is_deterministic_and_hash_bound(self) -> None:
        header, body = _fixture()
        one = inventory_vab(header, body)
        two = inventory_vab(header, body)
        self.assertEqual(one, two)
        self.assertEqual(one["header_sha256"], hashlib.sha256(header).hexdigest())
        self.assertEqual(one["body_sha256"], hashlib.sha256(body).hexdigest())


if __name__ == "__main__":
    unittest.main()
