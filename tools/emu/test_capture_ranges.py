"""Reject incomplete memory evidence before launching the emulator (T-39008)."""

import argparse
import unittest
import tempfile
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

from tools.emu.emu import find_iso, parse_dump, prepare_emulator_cue, validate_sampling


class CaptureRanges(unittest.TestCase):
    def test_sampling_is_bounded_and_requires_explicit_ranges(self):
        validate_sampling(1, 300, [(0x800e7374,32)], "boot")
        validate_sampling(0, 100000, [], "gdb")
        for every, frames, dumps, command in (
            (-1,300,[],"boot"), (1,300,[],"boot"),
            (1,300,[(0,32)],"gdb"), (1,4097,[(0,32)],"boot"),
            (1,100,[(0,0x200000)],"boot"), (301,300,[(0,32)],"boot"),
        ):
            with self.subTest(every=every, frames=frames, command=command):
                with self.assertRaises(ValueError):
                    validate_sampling(every,frames,dumps,command)

    def test_compatibility_cue_preserves_original_and_track_indices(self):
        with tempfile.TemporaryDirectory() as root:
            source = Path(root) / "source.cue"
            track = Path(root) / "track 1.bin"
            track.write_bytes(b"fixture")
            original = b'FILE "track 1.bin" BINARY\r\n TRACK 01 MODE2/2352\r\n FLAGS DCP\r\n INDEX 01 00:00:00\r\n'
            source.write_bytes(original)
            output = Path(root) / "output"
            output.mkdir()
            generated = Path(prepare_emulator_cue(str(source), str(output)))
            self.assertEqual(source.read_bytes(), original)
            self.assertEqual(generated.read_text(), 'FILE "%s" BINARY\n TRACK 01 MODE2/2352\n INDEX 01 00:00:00\n' % track)
            self.assertTrue((output / "cue-provenance.json").is_file())
            with self.assertRaises(FileExistsError):
                prepare_emulator_cue(str(source), str(output))

    def test_restored_tracks_ignore_stale_original_cue(self):
        with tempfile.TemporaryDirectory() as root:
            disc = Path(root) / "disc"
            disc.mkdir()
            (disc / "Original.cue").write_text('FILE "old track.bin" BINARY\n')
            (disc / "disc.cue").write_text('FILE "track1.bin" BINARY\n')
            (disc / "track1.bin").write_bytes(b"fixture")
            with patch("tools.emu.emu.ROOT", root):
                self.assertEqual(find_iso(SimpleNamespace(iso=None)), str(disc / "disc.cue"))
                (disc / "another.cue").write_text('FILE "track1.bin" BINARY\n')
                with self.assertRaises(SystemExit):
                    find_iso(SimpleNamespace(iso=None))

    def test_source_checkpoint_ranges(self):
        for spec, expected in (
            ("800E7374:32", (0x800E7374, 32)),
            ("80120650:2176", (0x80120650, 2176)),
            ("a01fffff:1", (0xA01FFFFF, 1)),
            ("1f800000:1024", (0x1F800000, 1024)),
            ("bfc00000:0x80000", (0xBFC00000, 0x80000)),
        ):
            with self.subTest(spec=spec):
                self.assertEqual(parse_dump(spec), expected)

    def test_unreadable_or_cross_region_ranges(self):
        for spec in (
            "-1:1", "100000000:1", "80000000:0", "80000000:-1",
            "801fffff:2", "1f8003ff:2", "bfc7ffff:2", "1f801000:4",
            "80000000:0x200001", "garbage:4", "80000000:bad",
        ):
            with self.subTest(spec=spec):
                with self.assertRaises(argparse.ArgumentTypeError):
                    parse_dump(spec)


if __name__ == "__main__":
    unittest.main()
