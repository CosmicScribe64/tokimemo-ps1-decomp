#!/usr/bin/env python3
"""Unit tests for tools/port/verify_tile_diff.py."""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from PIL import Image

from tools.port.verify_tile_diff import compare_16x16_tiles, normalize_320x240


class VerifyTileDiffTests(unittest.TestCase):
    def test_identical_images_have_zero_mae_and_zero_hotspots(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            p1 = Path(tmp) / "a.png"
            p2 = Path(tmp) / "b.png"
            img = Image.new("RGB", (320, 240), (40, 200, 160))
            img.save(p1)
            img.save(p2)
            report = compare_16x16_tiles(p1, p2, hotspot_threshold=5.0)
            self.assertEqual(report["overall_mae"], 0.0)
            self.assertEqual(report["hotspot_count"], 0)
            self.assertEqual(report["tile_count"], 300)

    def test_single_16x16_tile_anomaly_is_detected_even_when_overall_mae_is_low(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            p1 = Path(tmp) / "emu.png"
            p2 = Path(tmp) / "gd.png"
            emu = Image.new("RGB", (320, 240), (24, 128, 0))
            gd = emu.copy()
            # Corrupt a single 16x16 tile at (80, 128)
            for y in range(128, 144):
                for x in range(80, 96):
                    gd.putpixel((x, y), (255, 255, 255))
            emu.save(p1)
            gd.save(p2)
            report = compare_16x16_tiles(p1, p2, hotspot_threshold=20.0)
            # Whole-screen MAE is < 1.0, but the 16x16 tile pass catches the hotspot!
            self.assertLess(report["overall_mae"], 1.0)
            self.assertEqual(report["hotspot_count"], 1)
            self.assertEqual(report["top_hotspots"][0]["x"], 80)
            self.assertEqual(report["top_hotspots"][0]["y"], 128)
            self.assertGreater(report["top_hotspots"][0]["mae"], 200.0)

    def test_normalize_960x720_downsamples_cleanly(self) -> None:
        big = Image.new("RGB", (960, 720), (128, 64, 32))
        norm = normalize_320x240(big)
        self.assertEqual(norm.size, (320, 240))
        self.assertEqual(norm.getpixel((160, 120)), (128, 64, 32))


if __name__ == "__main__":
    unittest.main()
