"""Invented pixel patterns for conservative visual-review grouping."""
import copy
import hashlib
import struct
import unittest
import zlib

from tools.assets.visual_text_equivalence import EquivalenceError, build_equivalence


def sha(data):
    return hashlib.sha256(data).hexdigest()


def png_rgba(width, height, pixels):
    def chunk(kind, payload):
        return (struct.pack(">I", len(payload)) + kind + payload
                + struct.pack(">I", zlib.crc32(kind + payload) & 0xffffffff))
    raw = b"".join(b"\0" + bytes(channel for pixel in pixels[row * width:(row + 1) * width]
                                   for channel in pixel)
                    for row in range(height))
    return (b"\x89PNG\r\n\x1a\n"
            + chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0))
            + chunk(b"IDAT", zlib.compress(raw)) + chunk(b"IEND", b""))


def fixture(images, *, parts=None):
    media = {"schema": "tokimemo.media.v1", "assets": [{
        "id": "asset-synthetic", "source": {"path": "invented/texture.bin", "sha256": "a" * 64},
        "outputs": [],
    }]}
    regions = []
    byte_map = {}
    for index, (png, clut_row) in enumerate(images):
        region_id = "region-%d" % index
        byte_map[region_id] = png
        output_path = ".asset-cache/media/synthetic/%d.png" % index
        media["assets"][0]["outputs"].append({"path": output_path, "sha256": sha(png)})
        regions.append({
            "region_id": region_id,
            "parent_asset_id": "asset-synthetic",
            "source": {"path": "invented/texture.bin", "sha256": "a" * 64,
                       "location": {"lba": 7}},
            "frame": 3, "palette_index": index,
            "region": {"x": 0, "y": 0, "width": 2, "height": 2},
            "png": {"path": output_path, "sha256": sha(png)},
            "clut": {"palette_status": "available", "palette_words": [index, index + 1],
                     "vram": [0, clut_row]},
            "source_placement": {
                "image_block": {"offset": 32, "size": 8},
                "frame_parts": parts if parts is not None else [
                    {"source_record_offset": 64, "source_sheet_offset": 128, "u": 8, "v": 16,
                     "x": 0, "y": 0, "width": 2, "height": 2, "flip_flags": 0}],
            },
            "original_metadata": {"image_block": {"offset": 32, "size": 8}},
            "variant_metadata": {"variant": "palette-%d" % index,
                                  "source_sheet_offset": 128},
            "screen_context": {"status": "candidate", "contexts": ["synthetic context"]},
            "state": "confirmed_no_text" if index == 0 else "unreviewed",
            "confidence": 1.0 if index == 0 else None,
            "alpha_handling": "synthetic alpha policy", "stp_handling": "synthetic STP policy",
            "stp_status": "not_applicable", "stp": None,
        })
    return media, {"schema": "tokimemo.visual-text.v1", "regions": regions}, byte_map


class VisualTextEquivalenceTests(unittest.TestCase):
    def test_bijective_rgba_partition_keeps_members_and_never_copies_review(self):
        first = png_rgba(2, 2, [(0, 0, 0, 0), (1, 2, 3, 255),
                               (1, 2, 3, 255), (4, 5, 6, 128)])
        second = png_rgba(2, 2, [(9, 8, 7, 0), (200, 100, 50, 255),
                                (200, 100, 50, 255), (20, 30, 40, 128)])
        media, catalog, pngs = fixture([(first, 0x1f0), (second, 0x1f1)])
        result = build_equivalence(media, catalog, pngs)
        group = result["groups"][0]
        self.assertEqual(group["status"], "png_preview_partition_proven_manual_adjudication_required")
        self.assertTrue(group["manual_review_may_be_shared"])
        self.assertTrue(group["preview_only_proof"])
        self.assertFalse(group["runtime_render_equivalence_proven"])
        self.assertFalse(group["automatic_review_promotion"])
        self.assertEqual([member["region_id"] for member in group["members"]], ["region-0", "region-1"])
        self.assertEqual([member["review_state"] for member in group["members"]],
                         ["confirmed_no_text", "unreviewed"])
        self.assertEqual([member["clut"]["vram"] for member in group["members"]],
                         [[0, 0x1f0], [0, 0x1f1]])
        self.assertEqual(group["pixel_partition_check"]["alpha_pixels"],
                         {"transparent": 1, "translucent": 1, "opaque": 2})

    def test_same_geometry_with_different_color_partition_is_only_candidate(self):
        first = png_rgba(2, 2, [(1, 1, 1, 255), (1, 1, 1, 255),
                               (2, 2, 2, 255), (2, 2, 2, 255)])
        second = png_rgba(2, 2, [(3, 3, 3, 255), (4, 4, 4, 255),
                                (4, 4, 4, 255), (4, 4, 4, 255)])
        media, catalog, pngs = fixture([(first, 0), (second, 1)])
        group = build_equivalence(media, catalog, pngs)["groups"][0]
        self.assertEqual(group["pixel_partition_check"]["status"], "different_color_boundary_partition")
        self.assertFalse(group["manual_review_may_be_shared"])

    def test_bijective_near_duplicate_palette_colors_raise_contrast_flag(self):
        first = png_rgba(2, 2, [(100, 100, 100, 255), (110, 110, 110, 255),
                               (100, 100, 100, 255), (110, 110, 110, 255)])
        second = png_rgba(2, 2, [(200, 200, 200, 255), (204, 204, 204, 255),
                                (200, 200, 200, 255), (204, 204, 204, 255)])
        media, catalog, pngs = fixture([(first, 0), (second, 1)])
        group = build_equivalence(media, catalog, pngs)["groups"][0]
        metric = group["pixel_partition_check"]["member_visibility_metrics"][1]
        self.assertLessEqual(metric["min_pairwise_used_rgb_euclidean_distance"], 8)
        self.assertEqual(metric["contrast_flag"], "near_duplicate_used_colors")
        self.assertIn("manual contrast check is required",
                      " ".join(group["members"][1]["visibility_concerns"]))

    def test_equal_colors_with_different_alpha_mask_is_only_candidate(self):
        first = png_rgba(2, 2, [(1, 1, 1, 0), (2, 2, 2, 255),
                               (2, 2, 2, 255), (1, 1, 1, 0)])
        second = png_rgba(2, 2, [(3, 3, 3, 255), (4, 4, 4, 255),
                                (4, 4, 4, 255), (3, 3, 3, 255)])
        media, catalog, pngs = fixture([(first, 0), (second, 1)])
        group = build_equivalence(media, catalog, pngs)["groups"][0]
        self.assertEqual(group["pixel_partition_check"]["status"], "different_alpha_mask")
        self.assertFalse(group["manual_review_may_be_shared"])

    def test_source_crop_orientation_layering_and_output_bounds_split_groups(self):
        image = png_rgba(2, 2, [(1, 1, 1, 255)] * 4)
        for altered in (
            [{"source_record_offset": 64, "source_sheet_offset": 128, "u": 8, "v": 16,
              "x": 0, "y": 0, "width": 2, "height": 2, "flip_flags": 0, "layer_index": 1}],
        ):
            media, catalog, pngs = fixture([(image, 0), (image, 1)])
            catalog["regions"][1]["source_placement"]["frame_parts"] = altered
            self.assertEqual(build_equivalence(media, catalog, pngs)["groups"], [])
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        catalog["regions"][1]["region"]["width"] = 1
        self.assertEqual(build_equivalence(media, catalog, pngs)["groups"], [])
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        catalog["regions"][1]["source_placement"]["frame_parts"][0]["source_record_offset"] = 90
        result = build_equivalence(media, catalog, pngs)
        self.assertEqual(len(result["groups"]), 1)
        self.assertEqual([member["source_placement"]["frame_parts"][0]["source_record_offset"]
                          for member in result["groups"][0]["members"]], [64, 90])

    def test_real_sheet_source_offset_spelling_and_tile_extents_are_bound(self):
        image = png_rgba(2, 2, [(1, 1, 1, 255)] * 4)
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        for region in catalog["regions"]:
            part = region["source_placement"]["frame_parts"][0]
            part["sheet_source_offset"] = part.pop("source_sheet_offset")
        self.assertEqual(len(build_equivalence(media, catalog, pngs)["groups"]), 1)
        catalog["regions"][1]["variant_metadata"].update(tile_source_offset=256, tile_source_size=8)
        self.assertEqual(build_equivalence(media, catalog, pngs)["groups"], [])

    def test_unconfirmed_palette_variant_scope_blocks_shared_adjudication(self):
        image = png_rgba(2, 2, [(1, 1, 1, 255)] * 4)
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        catalog["regions"][1]["variant_metadata"]["palette_variant_scope"] = (
            "candidate override; applicability unconfirmed")
        group = build_equivalence(media, catalog, pngs)["groups"][0]
        self.assertEqual(group["pixel_partition_check"]["status"], "color_partition_and_alpha_match")
        self.assertFalse(group["manual_review_may_be_shared"])
        self.assertIn("not a confirmed applicable palette", " ".join(group["shared_visual_evidence_reasons"]))

    def test_paired_geometry_dependency_hash_is_part_of_group_key(self):
        image = png_rgba(2, 2, [(1, 1, 1, 255)] * 4)
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        media["assets"][0]["source_dependencies"] = {
            "exe": {"path": "invented/exe", "sha256": "b" * 64}}
        for region in catalog["regions"]:
            region["source"]["dependencies"] = copy.deepcopy(media["assets"][0]["source_dependencies"])
        self.assertEqual(len(build_equivalence(media, catalog, pngs)["groups"]), 1)
        catalog["regions"][1]["source"]["dependencies"]["exe"]["sha256"] = "c" * 64
        with self.assertRaisesRegex(EquivalenceError, "dependencies differ"):
            build_equivalence(media, catalog, pngs)

    def test_missing_geometry_and_hash_mismatch_do_not_authorize_review_sharing(self):
        image = png_rgba(2, 2, [(1, 1, 1, 255)] * 4)
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        catalog["regions"][1]["source_placement"]["frame_parts"] = None
        catalog["regions"][1]["source_placement"]["image_block"] = None
        catalog["regions"][1]["variant_metadata"] = {"variant": "palette-1"}
        result = build_equivalence(media, catalog, pngs)
        self.assertEqual(result["counts"]["ungrouped_regions"], 1)
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        pngs["region-1"] = png_rgba(2, 2, [(9, 9, 9, 255)] * 4)
        result = build_equivalence(media, catalog, pngs)
        group = result["groups"][0]
        self.assertFalse(group["manual_review_may_be_shared"])
        self.assertEqual(group["pixel_partition_check"]["status"], "pixel_check_unavailable")
        self.assertEqual(result["artifact_validation"]["status"], "hash_verification_failed")

    def test_diagnostic_or_unknown_palette_and_different_stp_prevent_sharing(self):
        image = png_rgba(2, 2, [(1, 1, 1, 255)] * 4)
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        catalog["regions"][1]["clut"]["palette_status"] = "unknown"
        result = build_equivalence(media, catalog, pngs)
        self.assertFalse(result["groups"][0]["manual_review_may_be_shared"])
        catalog["regions"][1]["clut"]["palette_status"] = "available"
        catalog["regions"][1]["stp_status"] = "exported"
        catalog["regions"][1]["stp"] = {"path": ".asset-cache/media/x.stp.png", "sha256": "b" * 64}
        group = build_equivalence(media, catalog, pngs)["groups"][0]
        self.assertIn("STP masks differ", " ".join(group["shared_visual_evidence_reasons"]))
        self.assertFalse(group["manual_review_may_be_shared"])
        catalog["regions"][1]["stp"] = None
        group = build_equivalence(media, catalog, pngs)["groups"][0]
        self.assertIn("status and artifact reference disagree",
                      " ".join(group["shared_visual_evidence_reasons"]))

    def test_rejects_bad_manifest_schemas(self):
        image = png_rgba(2, 2, [(1, 1, 1, 255)] * 4)
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        with self.assertRaises(EquivalenceError):
            build_equivalence({}, catalog, pngs)
        with self.assertRaises(EquivalenceError):
            build_equivalence(media, {"schema": "bad", "regions": []}, pngs)

    def test_exact_png_alias_groups_cross_source_geometry_and_keeps_provenance(self):
        image = png_rgba(2, 2, [(1, 1, 1, 255), (2, 2, 2, 255),
                                (1, 1, 1, 255), (2, 2, 2, 255)])
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        path = ".asset-cache/media/synthetic/1.png"
        media["assets"].append({"id": "asset-second",
                                "source": {"path": "invented/other.bin", "sha256": "b" * 64},
                                "outputs": [{"path": path, "sha256": sha(image)}]})
        region = catalog["regions"][1]
        region["parent_asset_id"] = "asset-second"
        region["source"].update(path="invented/other.bin", sha256="b" * 64)
        result = build_equivalence(media, catalog, pngs, exact_aliases=True)
        self.assertEqual(len(result["exact_preview_alias_groups"]), 1)
        alias = result["exact_preview_alias_groups"][0]
        self.assertTrue(alias["manual_preview_text_adjudication_may_be_shared"])
        self.assertFalse(alias["runtime_render_equivalence_proven"])
        self.assertEqual({member["source"]["sha256"] for member in alias["members"]},
                         {"a" * 64, "b" * 64})
        self.assertEqual({tuple(member["clut"]["vram"]) for member in alias["members"]},
                         {(0, 0), (0, 1)})

    def test_exact_png_alias_includes_same_source_candidate_palette_variants(self):
        image = png_rgba(2, 2, [(1, 1, 1, 255), (2, 2, 2, 255),
                                (1, 1, 1, 255), (2, 2, 2, 255)])
        media, catalog, pngs = fixture([(image, 0), (image, 0)])
        catalog["regions"][1]["variant_metadata"]["palette_variant_scope"] = (
            "candidate override; runtime applicability unconfirmed")
        result = build_equivalence(media, catalog, pngs, exact_aliases=True)
        self.assertEqual(len(result["exact_preview_alias_groups"]), 1)
        alias = result["exact_preview_alias_groups"][0]
        self.assertTrue(alias["manual_preview_text_adjudication_may_be_shared"])
        self.assertFalse(alias["runtime_render_equivalence_proven"])
        self.assertEqual({member["source"]["sha256"] for member in alias["members"]}, {"a" * 64})
        self.assertEqual({member["palette_variant_scope"] for member in alias["members"]},
                         {None, "candidate override; runtime applicability unconfirmed"})
        self.assertEqual({member["region_id"] for member in alias["members"]},
                         {"region-0", "region-1"})

    def test_exact_png_alias_flags_conflicting_confirmed_states_and_hash_failures(self):
        image = png_rgba(2, 2, [(1, 1, 1, 255)] * 4)
        media, catalog, pngs = fixture([(image, 0), (image, 1)])
        media["assets"].append({"id": "asset-second",
                                "source": {"path": "invented/other.bin", "sha256": "b" * 64},
                                "outputs": [{"path": ".asset-cache/media/synthetic/1.png",
                                             "sha256": sha(image)}]})
        region = catalog["regions"][1]
        region["parent_asset_id"] = "asset-second"
        region["source"].update(path="invented/other.bin", sha256="b" * 64)
        region["state"] = "confirmed_japanese_text"
        region["confidence"] = 1.0
        result = build_equivalence(media, catalog, pngs, exact_aliases=True)
        alias = result["exact_preview_alias_groups"][0]
        self.assertTrue(alias["manual_review_state_conflict"])
        self.assertFalse(alias["manual_preview_text_adjudication_may_be_shared"])
        pngs["region-1"] = png_rgba(2, 2, [(3, 3, 3, 255)] * 4)
        with self.assertRaisesRegex(EquivalenceError, "hash mismatch"):
            build_equivalence(media, catalog, pngs, exact_aliases=True)


if __name__ == "__main__":
    unittest.main()
