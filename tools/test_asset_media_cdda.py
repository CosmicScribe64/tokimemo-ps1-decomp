"""Synthetic-only tests for strict CDDA CUE/BINARY reading."""
import json
import shutil
import tempfile
import unittest
import wave
import zipfile
from pathlib import Path

from tools.assets.media_cdda import CddaError, SECTOR_BYTES, export_cdda, inspect_cdda


def _cue():
    return (b'FILE "track2.bin" BINARY\n'
            b'  TRACK 02 AUDIO\n'
            b'    INDEX 00 00:00:00\n'
            b'    INDEX 01 00:00:02\n')


class CddaTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.source = self.root / "source"
        self.source.mkdir()
        self.payload = bytearray(SECTOR_BYTES * 4)
        self.payload[2 * SECTOR_BYTES:2 * SECTOR_BYTES + 4] = b"\x12\x34\xfe\xdc"
        (self.source / "disc.cue").write_bytes(_cue())
        (self.source / "track2.bin").write_bytes(self.payload)

    def tearDown(self):
        self.tmp.cleanup()

    def test_inspection_bounds_index_and_hashes_file(self):
        meta = inspect_cdda(self.source)
        self.assertEqual(meta["index00_sector"], 0)
        self.assertEqual(meta["index01_sector"], 2)
        self.assertEqual(meta["end_sector_exclusive"], 4)
        self.assertEqual(meta["pregap_sectors"], 2)
        self.assertEqual(meta["program_sectors"], 2)
        self.assertEqual(meta["source"]["file_size"], 4 * SECTOR_BYTES)

    def test_export_preserves_binary_little_endian_samples_and_pregap(self):
        from tools.assets.manifest import CACHE
        CACHE.mkdir(parents=True, exist_ok=True)
        out = Path(tempfile.mkdtemp(prefix="test-cdda-", dir=CACHE))
        try:
            result = export_cdda(self.source, out, "synthetic-track")
            with wave.open(str(out / "synthetic-track.wav"), "rb") as wav:
                audio_format = (wav.getnchannels(), wav.getsampwidth(), wav.getframerate())
                self.assertEqual(audio_format, (2, 2, 44100))
                self.assertEqual(wav.getnframes(), 2 * 588)
                self.assertEqual(wav.readframes(1), b"\x12\x34\xfe\xdc")
            pregap = (out / "synthetic-track.pregap.bin").read_bytes()
            self.assertEqual(pregap, self.payload[:2 * SECTOR_BYTES])
            self.assertEqual(result["status"], "exported")
            self.assertEqual(result["pregap"]["size"], 2 * SECTOR_BYTES)
            self.assertEqual(result["raw_track_copy"]["size"], len(self.payload))
            self.assertEqual(result["source"]["file_sha256"], result["raw_track_copy"]["sha256"])
            json.loads((out / "synthetic-track.json").read_text())
        finally:
            shutil.rmtree(out, ignore_errors=True)

    def test_zip_input(self):
        archive = self.root / "disc.zip"
        with zipfile.ZipFile(archive, "w") as z:
            z.writestr("disc.cue", _cue())
            z.writestr("track2.bin", self.payload)
        self.assertEqual(inspect_cdda(archive)["program_sectors"], 2)

    def test_explicit_cue_path(self):
        self.assertEqual(inspect_cdda(self.source / "disc.cue")["program_sectors"], 2)

    def test_same_file_next_track_closes_selected_program_area(self):
        cue = (_cue() + b'  TRACK 03 AUDIO\n'
               b'    INDEX 00 00:00:03\n'
               b'    INDEX 01 00:00:04\n')
        (self.source / "disc.cue").write_bytes(cue)
        meta = inspect_cdda(self.source, track_number=2)
        self.assertEqual(meta["end_sector_exclusive"], 3)
        self.assertEqual(meta["program_sectors"], 1)

    def test_path_traversal_rejected(self):
        (self.source / "disc.cue").write_text(
            'FILE "../outside.bin" BINARY\n TRACK 02 AUDIO\n INDEX 01 00:00:00\n')
        with self.assertRaises(CddaError):
            inspect_cdda(self.source)

    def test_index_beyond_track_rejected(self):
        (self.source / "disc.cue").write_text(
            'FILE "track2.bin" BINARY\n TRACK 02 AUDIO\n INDEX 01 00:00:05\n')
        with self.assertRaises(CddaError):
            inspect_cdda(self.source)

    def test_duplicate_zip_member_rejected(self):
        archive = self.root / "dup.zip"
        with zipfile.ZipFile(archive, "w") as z:
            z.writestr("disc.cue", _cue())
            z.writestr("disc.cue", _cue())
        with self.assertRaises(CddaError):
            inspect_cdda(archive)

    def test_non_audio_selected_track_rejected(self):
        (self.source / "disc.cue").write_text(
            'FILE "track2.bin" BINARY\n TRACK 02 MODE2/2352\n INDEX 01 00:00:00\n')
        with self.assertRaises(CddaError):
            inspect_cdda(self.source)

    def test_pre_emphasis_rejected_and_dcp_preserved(self):
        cue = _cue().replace(b"TRACK 02 AUDIO\n", b"TRACK 02 AUDIO\n    FLAGS DCP\n")
        (self.source / "disc.cue").write_bytes(cue)
        self.assertEqual(inspect_cdda(self.source)["track_flags"], ["DCP"])
        (self.source / "disc.cue").write_bytes(cue.replace(b"FLAGS DCP", b"FLAGS PRE"))
        with self.assertRaises(CddaError):
            inspect_cdda(self.source)

    def test_extra_index_must_fit_selected_track(self):
        (self.source / "disc.cue").write_text(
            'FILE "track2.bin" BINARY\n TRACK 02 AUDIO\n'
            ' INDEX 01 00:00:02\n INDEX 02 00:00:05\n')
        with self.assertRaises(CddaError):
            inspect_cdda(self.source)

    def test_output_may_not_overlap_source_directory(self):
        from tools.assets.manifest import CACHE
        CACHE.mkdir(parents=True, exist_ok=True)
        source = Path(tempfile.mkdtemp(prefix="test-cdda-source-", dir=CACHE))
        (source / "disc.cue").write_bytes(_cue())
        (source / "track2.bin").write_bytes(self.payload)
        try:
            with self.assertRaises(CddaError):
                export_cdda(source, source / "exports", "synthetic-track")
        finally:
            shutil.rmtree(source, ignore_errors=True)

    def test_asset_id_rejected(self):
        from tools.assets.manifest import CACHE
        with self.assertRaises(CddaError):
            export_cdda(self.source, CACHE / "test-cdda", "../escape")


if __name__ == "__main__":
    unittest.main()
