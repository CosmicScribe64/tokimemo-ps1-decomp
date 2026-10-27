"""Focused tests for the source-bound first-arrival media exporter."""
from __future__ import annotations

import importlib.util
import os
from pathlib import Path
import struct
import tempfile
import unittest
from unittest import mock


MODULE = Path(__file__).with_name("prologue_arrival_media.py")
SPEC = importlib.util.spec_from_file_location("prologue_arrival_media", MODULE)
media = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(media)


class PrologueArrivalMediaTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source_root = media._source_root()
        cls.track = cls.source_root / "disc" / "track1.bin"

    def test_original_track_exports_320_by_160_pixels_and_raw_actor(self):
        manifest, files = media.build_pack(self.track, source_root=self.source_root)
        background = manifest["background"]
        self.assertEqual(background["status"], "source_context_mdec_preview")
        self.assertEqual(background["source"]["lba"], 0x494B)
        self.assertEqual(background["geometry"], [320, 160])
        self.assertIn(background["geometry_proof"]["source_context_status"],
                      ("proved_single_shape", "proved_single_shape_with_unresolved_uses"))
        self.assertTrue(background["geometry_proof"]["unresolved_source_uses_present"])
        self.assertEqual(background["hardware_pixel_parity_verified"], False)
        png = files["background/arrival-stage0.background.png"]
        self.assertEqual(png[:8], b"\x89PNG\r\n\x1a\n")
        self.assertEqual(struct.unpack_from(">II", png, 16), (320, 160))

        actor = manifest["actor_96"]
        self.assertEqual(actor["status"], "source_profile_static_frames_first_visible_frame_proven_scene_composition_unresolved")
        self.assertEqual(actor["source"]["path"], "CDROM/PROLODIR/O_01.NBN")
        self.assertEqual(actor["source"]["lba"], 0xABAB)
        self.assertEqual(actor["native_read_sectors"], 13)
        self.assertEqual(actor["frame_table_file_offset"], 0xAC)
        self.assertEqual(actor["sequence_table_file_offset"], 0xC0)
        self.assertEqual(actor["tpage_table_file_offset"], 0xA8)
        self.assertTrue(actor["frame_pixels_exported"])
        self.assertEqual(len(files["actor/O_01.NBN"]), media.ACTOR_SIZE)
        self.assertEqual(actor["frame_profile"]["texture_page_offset"], 0xD0)
        self.assertEqual(actor["frame_profile"]["texture_page_dimensions"], [256, 128])
        self.assertEqual(actor["frame_profile"]["palette_clut_vram"], [0, 0x1F0])
        self.assertEqual([row["duration_ticks"] for row in actor["frame_profile"]["sequence_rows"]],
                         [3, 3, 3, 1, 1])
        runtime = actor["frame_profile"]["actor_stage_state"]
        self.assertEqual(actor["frame_profile"]["actor_record_alias_proof"]["actor_pool_index"], 96)
        self.assertFalse(runtime["stage0_visible"])
        self.assertEqual(runtime["stage0_draw_flags"], 0x24)
        self.assertTrue(runtime["post_hold_visible"])
        self.assertEqual(runtime["post_hold_draw_flags"], 0xA4)
        self.assertEqual(runtime["initial_frame_index"], 0)
        self.assertEqual(actor["frame_profile"]["runtime_frame_selection"],
                         "first visible actor frame is source sequence step 0/frame 0; all five linked static frames are exported")
        self.assertEqual(len(actor["frame_profile"]["rendered_images"]), 5)
        frame = actor["frame_profile"]["rendered_images"][3]
        self.assertEqual((frame["width"], frame["height"], frame["parts"]), (32, 33, 2))
        frame_png = files["actor/actor96.frame-03.png"]
        self.assertEqual(frame_png[:8], b"\x89PNG\r\n\x1a\n")
        self.assertEqual(struct.unpack_from(">II", frame_png, 16), (32, 33))
        self.assertTrue(manifest["capabilities"]["actor96_frame_pixels"])
        self.assertFalse(manifest["capabilities"]["complete_arrival_frame"])

    def test_source_pin_mismatch_fails_before_asset_decode(self):
        altered = dict(media.SOURCE_PINS)
        altered["src/main/background_images.c"] = "0" * 64
        with mock.patch.object(media, "SOURCE_PINS", altered):
            with self.assertRaises(media.ArrivalMediaError):
                media.build_pack(self.track, source_root=self.source_root)

    def test_actor_instruction_proof_rejects_changed_overlay_word(self):
        overlay = bytearray(0x30000)
        with self.assertRaises(media.ArrivalMediaError):
            media._overlay_actor_instructions(bytes(overlay))

    def test_export_rejects_path_outside_candidate_cache_before_read(self):
        with tempfile.TemporaryDirectory(prefix="arrival-media-outside-") as temp:
            destination = Path(temp) / "pack"
            with self.assertRaises(media.ArrivalMediaError):
                media.export(self.track, destination, source_root=self.source_root)
            self.assertFalse(destination.exists())

    def test_non_track_directory_is_rejected(self):
        extracted = self.source_root / "disc" / "files"
        with self.assertRaises(media.ArrivalMediaError):
            media.build_pack(extracted, source_root=self.source_root)


if __name__ == "__main__":
    unittest.main()
