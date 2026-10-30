"""T-39008: invalid GPU output must not become accepted image evidence."""
import struct
import tempfile
import unittest
import zlib
from pathlib import Path

from tools.emu.emu import prepare_shot


class ScreenshotEvidence(unittest.TestCase):
    def test_invalid_capture_retains_raw_and_state(self):
        for width, height, bpp, payload in (
            (0, 0, 16, b""), (1, 1, 8, b"x"),
            (1, 1, 16, b"x"), (1, 1, 24, b"xxxx"),
            (1025, 1, 16, b""), (1, -1, 16, b""),
        ):
            with self.subTest(width=width, height=height, bpp=bpp):
                with tempfile.TemporaryDirectory() as directory:
                    raw = Path(directory) / "shot.raw"
                    raw.write_bytes(payload)
                    shot = dict(path=str(raw), width=width, height=height,
                                bpp=bpp, frame=1, cycles=42, dumps=[{"data":"00"}])
                    prepare_shot(shot, directory)
                    self.assertEqual(shot["image_status"], "unavailable")
                    self.assertNotIn("png", shot)
                    self.assertEqual(shot["cycles"], 42)
                    self.assertEqual(shot["dumps"], [{"data":"00"}])
                    self.assertEqual(raw.read_bytes(), payload)
                    self.assertFalse(raw.with_suffix(".png").exists())

    def test_valid_formats_preserve_existing_color_conversion(self):
        for bpp, payload, rgb in [(16, b"\x1f\x00", b"\xf8\x00\x00"),
                                  (24, b"\x12\x34\x56", b"\x12\x34\x56")]:
            with tempfile.TemporaryDirectory() as directory:
                raw = Path(directory) / "shot.raw"
                raw.write_bytes(payload)
                shot = dict(path=str(raw), width=1, height=1, bpp=bpp)
                prepare_shot(shot, directory)
                self.assertEqual(shot["image_status"], "ok")
                self.assertEqual(len(shot["png_sha256"]), 64)
                png = (Path(directory) / shot["png"]).read_bytes()
                offset = 8
                while png[offset + 4:offset + 8] != b"IDAT":
                    offset += 12 + struct.unpack_from(">I", png, offset)[0]
                size = struct.unpack_from(">I", png, offset)[0]
                self.assertEqual(zlib.decompress(png[offset + 8:offset + 8 + size]), b"\0" + rgb)

    def test_missing_raw_is_unavailable(self):
        with tempfile.TemporaryDirectory() as directory:
            shot = dict(path="missing.raw", width=320, height=240, bpp=16)
            prepare_shot(shot, directory)
            self.assertEqual(shot["image_status"], "unavailable")


if __name__ == "__main__":
    unittest.main()
