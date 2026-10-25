"""Synthetic-only tests for verified raw XA/STR decoding orchestration."""
import hashlib
import json
import re
import shutil
import struct
import subprocess
import tempfile
import unittest
import wave
from pathlib import Path
from unittest import mock

from tools.assets.manifest import CACHE, REPOSITORY
from tools.assets.media_stream_decode import (
    RAW_SCHEMA,
    StreamDecodeError,
    _actual_sector,
    decode_staged_stream,
    inspect_staged_stream,
)


def _sector(file_number, channel, kind, *, coding=0, frame_number=0, form=2,
            bitstream_version=1, bitstream_quant=1, declared_mdec_words=50):
    raw = bytearray(2352)
    raw[:12] = b"\x00" + b"\xff" * 10 + b"\x00"
    raw[15] = 2
    type_bits = {"audio": 0x04, "video": 0x02, "data": 0x08}[kind]
    submode = (0x20 if form == 2 else 0) | type_bits
    sub = bytes((file_number, channel, submode, coding))
    raw[16:20] = sub
    raw[20:24] = sub
    payload_size = 2324 if form == 2 else 2048
    raw[24:24 + payload_size] = bytes([file_number & 0xff]) * payload_size
    if kind == "video":
        struct.pack_into("<HH", raw, 0x18, 0x8001, 2)
        struct.pack_into("<HH", raw, 0x1C, 0, 1)
        struct.pack_into("<I", raw, 0x20, frame_number)
        struct.pack_into("<I", raw, 0x24, 100)
        struct.pack_into("<HH", raw, 0x28, 320, 240)
        struct.pack_into("<H", raw, 0x38, declared_mdec_words)
        struct.pack_into("<H", raw, 0x3A, 0x3800)
        struct.pack_into("<H", raw, 0x3C, bitstream_quant)
        struct.pack_into("<H", raw, 0x3E, bitstream_version)
    return bytes(raw)


def _fixture(sectors):
    input_root = CACHE / "input"
    input_root.mkdir(parents=True, exist_ok=True)
    folder = Path(tempfile.mkdtemp(prefix="test-stream-decode-", dir=input_root))
    raw_path = folder / "synthetic.raw2352"
    manifest_path = folder / "synthetic.json"
    raw_bytes = b"".join(sectors)
    raw_path.write_bytes(raw_bytes)
    payload = b"".join(sector[24:2348 if sector[18] & 0x20 else 2072] for sector in sectors)
    rows = []
    for i, sector in enumerate(sectors):
        if sector[16:20] == sector[20:24]:
            rows.append(_actual_sector(sector, i, 1000))
        else:
            rows.append({
                "sector_index": i, "lba": 1000 + i, "raw_size": 2352,
                "raw_sha256": hashlib.sha256(sector).hexdigest(),
                "subheader_primary": sector[16:20].hex(),
                "subheader_secondary": sector[20:24].hex(),
                "metadata_status": "opaque_invalid_duplicate_subheader",
                "subheader_valid": False, "form": None, "form_name": None,
                "payload_size": None, "payload_sha256": None,
                "file_number": None, "channel": None, "submode": None,
                "coding": None, "edc_validation": "not_performed",
            })
    manifest = {
        "schema": RAW_SCHEMA,
        "status": "staged",
        "source": {
            "path": "CDROM/SAMPLE.XA",
            "input_sha256": "a" * 64,
            "format": "mode2-2352",
            "container_source": "/original/disc.bin",
            "baseline_asset_id": "asset-fixture",
            "lba": 1000,
            "declared_iso_size_bytes": len(sectors) * 2048,
            "sector_count": len(sectors),
            "size_semantics": "2048-byte ISO address units; not Form2 payload byte length",
        },
        "raw_stream": {
            "path": raw_path.relative_to(REPOSITORY).as_posix(),
            "size": len(raw_bytes),
            "sha256": hashlib.sha256(raw_bytes).hexdigest(),
            "concatenation": "original 2352-byte sectors in ascending LBA order",
        },
        "payload_stream": {
            "size": len(payload),
            "sha256": hashlib.sha256(payload).hexdigest(),
            "concatenation": "each sector's Mode2 payload in ascending LBA order",
        },
        "edc_validation": "not_performed",
        "sectors": rows,
    }
    manifest_path.write_text(json.dumps(manifest))
    return folder, manifest_path


def _png_header(width, height):
    chunk = b"IHDR" + struct.pack(">II", width, height) + b"\x08\x02\x00\x00\x00"
    import zlib
    return b"\x89PNG\r\n\x1a\n" + struct.pack(">I", len(chunk) - 4) + chunk + struct.pack(">I", zlib.crc32(chunk) & 0xffffffff)


class StreamDecodeTests(unittest.TestCase):
    def setUp(self):
        media = CACHE / "media"
        media.mkdir(parents=True, exist_ok=True)
        self.output = Path(tempfile.mkdtemp(prefix="test-stream-decode-", dir=media))
        self.folders = []
        self.demux_input_sizes = []
        self.codec_input_bytes = []

    def tearDown(self):
        for folder in self.folders:
            shutil.rmtree(folder, ignore_errors=True)
        shutil.rmtree(self.output, ignore_errors=True)

    def fixture(self, sectors):
        folder, manifest = _fixture(sectors)
        self.folders.append(folder)
        return manifest

    def fake_ffmpeg(self, args, **kwargs):
        if args[0] in ("ffmpeg", "ffprobe") and len(args) > 1 and args[1] == "-version":
            return subprocess.CompletedProcess(args, 0,
                stdout=args[0] + " version 4.4.2-0ubuntu0.22.04.1\n", stderr="")
        if args[0] == "ffprobe":
            path = args[-1]
            self.codec_input_bytes.append(Path(path).read_bytes())
            video = "video" in Path(path).name
            coding_match = re.search(r"-code-([0-9a-f]{2})-", Path(path).name)
            coding = int(coding_match.group(1), 16) if coding_match else 0
            channels = 2 if coding & 1 else 1
            sample_rate = 18900 if coding & 4 else 37800
            stream = ({"index": 0, "codec_name": "mdec", "codec_type": "video",
                       "width": 320, "height": 240, "time_base": "1/15"}
                      if video else {"index": 0, "codec_name": "adpcm_xa", "codec_type": "audio",
                                    "channels": channels, "sample_rate": str(sample_rate),
                                    "time_base": "1/37800"})
            return subprocess.CompletedProcess(args, 0,
                stdout=json.dumps({"streams": [stream], "format": {"format_name": "psxstr"}}), stderr="")
        if args[0] == "ffmpeg":
            if "-i" in args:
                self.demux_input_sizes.append(Path(args[args.index("-i") + 1]).stat().st_size)
            if "wav" in args:
                target = Path(args[-1])
                coding_match = re.search(r"-code-([0-9a-f]{2})-", target.name)
                coding = int(coding_match.group(1), 16) if coding_match else 0
                channels = 2 if coding & 1 else 1
                sample_rate = 18900 if coding & 4 else 37800
                source = Path(args[args.index("-i") + 1])
                sectors = source.stat().st_size // 2352
                frames = (2016 if channels == 2 else 4032) * sectors
                with wave.open(str(target), "wb") as out:
                    out.setnchannels(channels)
                    out.setsampwidth(2)
                    out.setframerate(sample_rate)
                    out.writeframes(b"\0\0" * channels * frames)
            else:
                pattern = Path(args[-1])
                frame_count = int(args[args.index("-frames:v") + 1])
                for index in range(1, frame_count + 1):
                    target = Path(str(pattern).replace("%06d", "%06d" % index))
                    target.write_bytes(_png_header(320, 240))
            return subprocess.CompletedProcess(args, 0, stdout="", stderr="")
        raise AssertionError(args)

    def test_same_channel_different_file_numbers_decode_separately(self):
        manifest = self.fixture([_sector(1, 0, "audio"), _sector(2, 0, "audio")])
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg) as run:
            result = decode_staged_stream(manifest, self.output, "synthetic-xa")
        self.assertEqual([s["file_number"] for s in result["streams"]], [1, 2])
        self.assertEqual(len(list((self.output / "synthetic-xa").glob("*.wav"))), 2)
        ffmpeg_inputs = [call.args[0] for call in run.call_args_list
                         if call.args[0][0] == "ffmpeg" and "-i" in call.args[0]]
        self.assertEqual(len(ffmpeg_inputs), 2)
        self.assertTrue(all("-f" in args and args[args.index("-f") + 1] == "psxstr" for args in ffmpeg_inputs))
        self.assertNotEqual(Path(ffmpeg_inputs[0][ffmpeg_inputs[0].index("-i") + 1]),
                            Path(ffmpeg_inputs[1][ffmpeg_inputs[1].index("-i") + 1]))

    def test_coding_change_creates_independent_stable_outputs(self):
        manifest = self.fixture([_sector(1, 0, "audio", coding=1),
                                 _sector(1, 0, "audio", coding=4)])
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg):
            result = decode_staged_stream(manifest, self.output, "coding-runs")
        self.assertEqual([s["source_coding"] for s in result["streams"]], [1, 4])
        self.assertEqual([s["source_coding_segment_index"] for s in result["streams"]], [0, 1])

    def test_audio_frame_limit_matches_validated_sector_count(self):
        manifest = self.fixture([_sector(1, 0, "audio") for _ in range(3)])
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg) as run:
            result = decode_staged_stream(manifest, self.output, "audio-frame-cap", media_kinds=("audio",))
        audio_command = next(call.args[0] for call in run.call_args_list
                             if call.args[0][0] == "ffmpeg" and "-map" in call.args[0])
        self.assertEqual(audio_command[audio_command.index("-frames:a") + 1], "3")
        self.assertEqual(result["streams"][0]["ffmpeg_audio_frame_limit"], 3)
        self.assertEqual(result["streams"][0]["sample_frames"], 3 * 4032)

    def test_partial_opaque_sector_is_quarantined_and_not_demuxed(self):
        sectors = [_sector(1, 0, "audio"), _sector(99, 29, "audio"), _sector(1, 0, "audio")]
        sectors[1] = sectors[1][:20] + bytes((sectors[1][20] ^ 1,)) + sectors[1][21:]
        manifest_path = self.fixture(sectors)
        value = json.loads(manifest_path.read_text())
        invalid_index = 1
        invalid = sectors[invalid_index]
        record = value["sectors"][invalid_index]
        value["sectors"][invalid_index] = {
            "sector_index": invalid_index, "lba": 1000 + invalid_index,
            "raw_size": 2352, "raw_sha256": hashlib.sha256(invalid).hexdigest(),
            "subheader_primary": invalid[16:20].hex(), "subheader_secondary": invalid[20:24].hex(),
            "metadata_status": "opaque_invalid_duplicate_subheader", "subheader_valid": False,
            "form": None, "form_name": None, "payload_size": None, "payload_sha256": None,
            "file_number": None, "channel": None, "submode": None, "coding": None,
            "edc_validation": "not_performed",
        }
        value["status"] = "partial"
        value["source"]["invalid_subheader_sector_count"] = 1
        valid_payload = b"".join(
            s[24:2348 if s[18] & 0x20 else 2072]
            for i, s in enumerate(sectors) if i != invalid_index)
        value["payload_stream"].update({"complete": False, "invalid_sector_count": 1,
                                        "sha256": None, "size": len(valid_payload)})
        value["raw_stream"]["sha256"] = hashlib.sha256(b"".join(sectors)).hexdigest()
        manifest_path.write_text(json.dumps(value))
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg) as run:
            result = decode_staged_stream(manifest_path, self.output, "partial-xa")
        self.assertEqual(result["status"], "partial")
        self.assertEqual(len(result["quarantined_sectors"]), 1)
        self.assertEqual([s["source_segment_index"] for s in result["streams"]], [0, 1])
        ffmpeg_inputs = [call.args[0] for call in run.call_args_list
                         if call.args[0][0] == "ffmpeg" and "-i" in call.args[0]]
        self.assertEqual(len(ffmpeg_inputs), 2)
        self.assertEqual(self.demux_input_sizes, [2352, 2352])

    def test_video_exports_png_without_claiming_timing(self):
        manifest = self.fixture([_sector(7, 3, "video", frame_number=42)])
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg):
            result = decode_staged_stream(manifest, self.output, "synthetic-str")
        stream = result["streams"][0]
        self.assertEqual(stream["frame_headers"][0]["source_frame_number"], 42)
        self.assertIn("not proven original playback timing", stream["timing"])
        self.assertEqual(stream["outputs"][0]["path"].endswith(".png"), True)
        adapter = stream["codec_input_adapter"]
        self.assertEqual(adapter["status"], "adapted-v1-to-v2")
        self.assertEqual(adapter["version_changes"][0]["raw_sector_byte_offset"], 0x3E)

    def test_v1_adapter_changes_only_temp_codec_input_and_binds_hashes(self):
        manifest = self.fixture([_sector(7, 3, "video", frame_number=42, form=1)])
        value = json.loads(manifest.read_text())
        raw_path = REPOSITORY / value["raw_stream"]["path"]
        original_source = raw_path.read_bytes()
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg):
            result = decode_staged_stream(manifest, self.output, "v1-adapter")
        derived = self.codec_input_bytes[0]
        adapter = result["streams"][0]["codec_input_adapter"]
        self.assertEqual(original_source, raw_path.read_bytes())
        self.assertEqual(len(derived), len(original_source))
        self.assertEqual(derived[0x3E:0x40], b"\x02\x00")
        expected = bytearray(original_source)
        expected[0x3E:0x40] = b"\x02\x00"
        self.assertEqual(derived, bytes(expected))
        self.assertEqual(adapter["source_input_sha256"], hashlib.sha256(original_source).hexdigest())
        self.assertEqual(adapter["derived_input_sha256"], hashlib.sha256(derived).hexdigest())
        self.assertEqual(adapter["version_changes"][0]["derived_input_byte_offset"], 0x3E)
        self.assertEqual(adapter["changed_byte_count"], 1)
        self.assertEqual(adapter["version_word_span_bytes"], 2)
        self.assertEqual(adapter["changed_input_byte_offsets"], [0x3E])
        self.assertEqual(adapter["version_changes"][0]["declared_mdec_bytes"], 200)
        self.assertEqual(adapter["source_raw_sector_mutation"],
                         "none; only the derived temporary FFmpeg input was changed")

    def test_unknown_bs_version_fails_before_ffprobe(self):
        manifest = self.fixture([_sector(7, 3, "video", frame_number=42, form=1,
                                         bitstream_version=4)])
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg) as run:
            with self.assertRaisesRegex(StreamDecodeError, "unsupported STR BS version"):
                decode_staged_stream(manifest, self.output, "unknown-bs-version")
        self.assertFalse(any(call.args[0][0] == "ffprobe" and call.args[0][1] == "-v"
                             for call in run.call_args_list))

    def test_invalid_bs_quant_fails_before_ffprobe(self):
        manifest = self.fixture([_sector(7, 3, "video", frame_number=42, form=1,
                                         bitstream_quant=64)])
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg) as run:
            with self.assertRaisesRegex(StreamDecodeError, "declaration/magic/quant"):
                decode_staged_stream(manifest, self.output, "invalid-bs-quant")
        self.assertFalse(any(call.args[0][0] == "ffprobe" and call.args[0][1] == "-v"
                             for call in run.call_args_list))

    def test_declared_mdec_words_are_not_compared_to_compressed_frame_size(self):
        manifest = self.fixture([_sector(7, 3, "video", frame_number=42, form=1,
                                         declared_mdec_words=101)])
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg):
            result = decode_staged_stream(manifest, self.output, "mdec-word-count")
        change = result["streams"][0]["codec_input_adapter"]["version_changes"][0]
        self.assertGreater(change["declared_mdec_words"], 100)
        self.assertEqual(change["declared_mdec_bytes"], 404)

    def test_bs_versions_two_and_three_pass_without_adapter_changes(self):
        sectors = [_sector(7, 3, "video", frame_number=1, form=1, bitstream_version=2),
                   _sector(7, 3, "video", frame_number=2, form=1, bitstream_version=3)]
        manifest = self.fixture(sectors)
        value = json.loads(manifest.read_text())
        raw_path = REPOSITORY / value["raw_stream"]["path"]
        original_source = raw_path.read_bytes()
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg):
            result = decode_staged_stream(manifest, self.output, "bs-v2-v3")
        self.assertEqual(self.codec_input_bytes[0], original_source)
        adapter = result["streams"][0]["codec_input_adapter"]
        self.assertEqual(adapter["status"], "not-required")
        self.assertEqual(adapter["validated_bs_versions"], [2, 3])

    def test_form1_str_video_and_form2_data_are_accounted_separately(self):
        manifest = self.fixture([
            _sector(7, 3, "video", frame_number=42, form=1),
            _sector(9, 0, "data"),
        ])
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg):
            result = decode_staged_stream(manifest, self.output, "mixed-sectors")
        self.assertEqual(len(result["streams"]), 1)
        self.assertEqual(result["streams"][0]["kind"], "video_frames")
        self.assertEqual(result["unrepresented_sector_counts"], {"Form2:type-08": 1})

    def test_audio_selection_records_unselected_video_coverage(self):
        manifest = self.fixture([
            _sector(7, 3, "video", frame_number=42, form=1),
            _sector(1, 0, "audio"),
        ])
        with mock.patch("tools.assets.media_stream_decode.subprocess.run", side_effect=self.fake_ffmpeg):
            result = decode_staged_stream(manifest, self.output, "audio-scope", media_kinds=("audio",))
        self.assertEqual(result["status"], "partial")
        self.assertEqual(result["media_selection"]["selected_kinds"], ["audio"])
        self.assertEqual(result["media_selection"]["source_stream_coverage"], "selected-kinds-only")
        self.assertEqual(result["media_selection"]["unselected_streams"][0]["kind"], "video")
        self.assertEqual(len(result["streams"]), 1)
        self.assertEqual(result["streams"][0]["kind"], "audio")

    def test_tampered_sector_hash_fails_before_ffmpeg(self):
        manifest = self.fixture([_sector(1, 0, "audio")])
        raw_path = REPOSITORY / json.loads(manifest.read_text())["raw_stream"]["path"]
        data = bytearray(raw_path.read_bytes())
        data[30] ^= 1
        raw_path.write_bytes(data)
        with mock.patch("tools.assets.media_stream_decode.subprocess.run") as run:
            with self.assertRaises(StreamDecodeError):
                decode_staged_stream(manifest, self.output, "tampered")
        run.assert_not_called()

    def test_duplicate_subheader_is_rejected_before_ffmpeg(self):
        folder = self.fixture([_sector(1, 0, "audio")])
        raw_path = folder.parent / "synthetic.raw2352"
        data = bytearray(raw_path.read_bytes())
        data[20] ^= 1
        raw_path.write_bytes(data)
        with mock.patch("tools.assets.media_stream_decode.subprocess.run") as run:
            with self.assertRaises(StreamDecodeError):
                inspect_staged_stream(folder)
        run.assert_not_called()

    def test_output_outside_media_cache_is_rejected(self):
        manifest = self.fixture([_sector(1, 0, "audio")])
        with self.assertRaises(StreamDecodeError):
            decode_staged_stream(manifest, Path(tempfile.gettempdir()), "escape")

    def test_unsupported_audio_coding_fails_closed(self):
        manifest = self.fixture([_sector(1, 0, "audio", coding=0x20)])
        with mock.patch("tools.assets.media_stream_decode.subprocess.run") as run:
            with self.assertRaises(StreamDecodeError):
                inspect_staged_stream(manifest)
        run.assert_not_called()


if __name__ == "__main__":
    unittest.main()
