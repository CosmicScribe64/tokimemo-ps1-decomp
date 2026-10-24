import struct
import tempfile
import unittest
import wave
from pathlib import Path
from unittest.mock import patch

from tools.assets.audio import VAG_HEADER_SIZE, _decode_vag_blocks, export_audio, inspect_audio


def vag(data=b"\x00\x01" + b"\x00" * 14, rate=22050):
    header = b"VAGp" + struct.pack(">IIII", 0x20, 0, len(data), rate) + b"fixture".ljust(16, b"\0")
    return header.ljust(VAG_HEADER_SIZE, b"\0") + data


class AudioReaderTests(unittest.TestCase):
    def test_vag_inspection_preserves_rate_and_loop_flags(self):
        raw = bytearray(vag())
        raw[VAG_HEADER_SIZE + 1] = 0x05
        info = inspect_audio(bytes(raw), "fixture.vag")
        self.assertEqual(info["status"], "supported")
        self.assertEqual(info["metadata"]["sample_rate_hz"], 22050)
        self.assertEqual([x["kind"] for x in info["metadata"]["loop_markers"]], ["loop_start", "end"])

    def test_export_is_mono_16bit_wav_with_expected_frame_count(self):
        with tempfile.TemporaryDirectory() as tmp:
            result = export_audio(vag(), tmp, "sample")
            self.assertTrue(result["exportable"])
            self.assertEqual(result["status"], "exported")
            with wave.open(str(Path(tmp) / "sample.wav"), "rb") as wav:
                self.assertEqual((wav.getnchannels(), wav.getsampwidth(), wav.getframerate(), wav.getnframes()), (1, 2, 22050, 28))
            self.assertTrue((Path(tmp) / "sample.json").is_file())

    def test_zero_rate_does_not_guess(self):
        info = inspect_audio(vag(rate=0), "unknown-rate.vag")
        self.assertFalse(info["exportable"])
        self.assertIn("1 through 192000", info["limitations"][0])

    def test_game_sequence_and_xa_are_inventory_only(self):
        for filename, kind in (("music.seq", "game_sound_bank_or_sequence"), ("voice.XA", "xa_or_str_stream")):
            info = inspect_audio(b"invented fixture", filename)
            self.assertEqual(info["format"], kind)
            self.assertFalse(info["exportable"])

    def test_truncated_payload_rejected(self):
        info = inspect_audio(vag(data=b"\0" * 15), "short.vag")
        self.assertEqual(info["status"], "invalid")

    def test_vab_pair_export_requires_explicit_audition_rate(self):
        header = bytearray(0x20 + 0x800 + 0x200 + 0x200)
        header[:4] = b"pBAV"
        struct.pack_into("<III", header, 4, 7, 0, len(header) + 16)
        struct.pack_into("<HHH", header, 0x12, 1, 0, 1)
        sample_sizes_offset = 0x820 + 0x200
        struct.pack_into("<H", header, sample_sizes_offset + 2, 2)  # index 0 is reserved; sample 1 is 2 * 8 bytes
        info = inspect_audio(bytes(header), "bank.VH")
        self.assertEqual(info["format"], "sony_vab_header")
        self.assertTrue(info["metadata"]["samples"][0]["empty"])
        self.assertEqual(info["metadata"]["samples"][1], {"index": 1, "offset": 0, "size_bytes": 16, "empty": False})
        body = b"\x00\x01" + b"\x00" * 14
        self.assertFalse(export_audio(bytes(header), ".", "bank", body=body)["exportable"])
        with tempfile.TemporaryDirectory() as tmp:
            result = export_audio(bytes(header), tmp, "bank", body=body, sample_rate=11025)
            self.assertTrue(result["exportable"])
            self.assertEqual(result["status"], "exported")
            with wave.open(str(Path(tmp) / "bank.sample-001.wav"), "rb") as wav:
                self.assertEqual(wav.getframerate(), 11025)

    def test_unsafe_asset_id_rejected(self):
        with self.assertRaises(ValueError):
            export_audio(vag(), "/tmp", "../unsafe")

    def test_adpcm_end_block_is_included_and_trailing_bytes_are_reported(self):
        first = b"\x00\x01" + b"\x11" * 14
        following = b"\x00\x00" + b"\x77" * 14
        pcm, markers, info = _decode_vag_blocks(first + following)
        self.assertEqual(len(pcm), 28)
        self.assertEqual(markers[0]["kind"], "end")
        self.assertEqual(markers[0]["sample"], 28)
        self.assertEqual(info["source_block_flags"], [1, 0])
        self.assertEqual(info["decoded_block_count"], 1)
        self.assertEqual(info["source_block_count"], 2)
        self.assertEqual(info["trailing_source_bytes"], 16)
        self.assertEqual(info["termination_block"], 0)

    def test_export_refuses_to_overwrite_outputs(self):
        with tempfile.TemporaryDirectory() as tmp:
            export_audio(vag(), tmp, "sample")
            with self.assertRaises(FileExistsError):
                export_audio(vag(), tmp, "sample")

    def test_decoded_sample_cap_rejects_overrun_but_honors_early_end(self):
        plain = b"\x00\x00" + b"\x11" * 14
        end = b"\x00\x01" + b"\x11" * 14
        with patch("tools.assets.audio.MAX_DECODED_SAMPLES", 28):
            with self.assertRaisesRegex(ValueError, "28-sample limit"):
                _decode_vag_blocks(plain + plain)
            pcm, _, info = _decode_vag_blocks(end + plain)
        self.assertEqual(len(pcm), 28)
        self.assertEqual(info["decoded_block_count"], 1)
        self.assertEqual(info["source_block_count"], 2)

    def test_adpcm_predictor_and_history_numeric_vector(self):
        first = b"\x00\x00" + b"\x00" * 13 + b"\x10"
        second = b"\x10\x00" + b"\x00" * 14
        pcm, _, _ = _decode_vag_blocks(first + second)
        self.assertEqual(pcm[27], 4096)
        self.assertEqual(pcm[28:30], [3840, 3600])

    def test_vag_extreme_sample_rate_is_not_exportable(self):
        info = inspect_audio(vag(rate=192001), "extreme-rate.vag")
        self.assertEqual(info["status"], "unsupported")
        self.assertIn("1 through 192000", info["limitations"][0])


if __name__ == "__main__":
    unittest.main()
