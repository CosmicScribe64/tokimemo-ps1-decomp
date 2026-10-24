"""Synthetic tests for the reviewable visual Japanese-text localization catalog."""
import copy
import hashlib
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from assets.visual_text import CatalogError, build_catalog, main, _verify_catalog_files


def sha(label):
    return hashlib.sha256(label.encode("ascii")).hexdigest()


def media_manifest(output_root=".asset-cache/media/run-a"):
    parent = "asset-parent-demo"
    source = {"path": "invented/example.TIM", "size": 64, "kind": "file",
              "location": {"lba": 7, "extent_bytes": 64}, "sha256": sha("source")}
    metadata = {
        "format": "psx-tim", "flags": 9, "bits_per_pixel": 4,
        "width": 8, "height": 4,
        "image_block": {"offset": 40, "size": 44, "x": 12, "y": 19,
                        "width_words": 2, "height": 4},
        "clut_block": {"offset": 8, "size": 32, "x": 4, "y": 3,
                       "width_words": 16, "height": 1},
        "palette_count": 1, "alpha_policy": "invented alpha metadata",
        "stp_policy": "invented STP metadata", "round_trip_verified": False,
    }
    images = []
    outputs = []
    for index in range(2):
        png = f"{parent}.palette-{index:03d}.png"
        stp = f"{parent}.palette-{index:03d}.stp.png"
        images.append({"palette_index": index, "png": png, "stp_png": stp,
                       "palette_words": [index], "clut_vram": {"x": 4, "y": 3 + index}})
        outputs.extend([
            {"path": f"{output_root}/assets/{parent}/{png}", "size": 12, "sha256": sha(png)},
            {"path": f"{output_root}/assets/{parent}/{stp}", "size": 8, "sha256": sha(stp)},
        ])
    image_record = {
        "id": parent, "source": source, "status": "exported", "format": "psx-tim",
        "inspection": {"candidates": [metadata]},
        "export": {"status": "exported", "images": images}, "outputs": outputs,
    }
    unsupported = {
        "id": "asset-unknown-demo",
        "source": {"path": "invented/container.NBN", "size": 99, "kind": "file",
                   "location": {"path": "invented/container.NBN"}, "sha256": None},
        "status": "unsupported", "format": "nbn", "reason": "synthetic unsupported container",
    }
    return {"schema": "tokimemo.media.v1", "input": {"source_sha256": sha("media source")},
            "assets": [image_record, unsupported]}


class VisualCatalogTests(unittest.TestCase):
    def test_catalog_has_one_region_per_palette_and_full_image_rect(self):
        catalog = build_catalog(media_manifest())
        self.assertEqual(catalog["schema"], "tokimemo.visual-text.v1")
        self.assertFalse(catalog["corpus_complete"])
        self.assertFalse(catalog["japanese_text_detection_complete"])
        self.assertEqual(len(catalog["regions"]), 2)
        first = catalog["regions"][0]
        self.assertEqual(first["region"], {"x": 0, "y": 0, "width": 8, "height": 4})
        self.assertEqual(first["source_placement"]["image_block"]["x"], 12)
        self.assertEqual(first["source_placement"]["image_block"]["y"], 19)
        self.assertEqual(first["frame"], None)
        self.assertEqual(first["frame_status"], "unknown")
        self.assertEqual(first["screen_context"]["status"], "unknown")
        self.assertEqual(first["screen_context"]["contexts"], [])
        self.assertEqual(first["stp"]["sha256"], sha("asset-parent-demo.palette-000.stp.png"))
        self.assertEqual(first["clut"]["palette_words"], [0])
        self.assertEqual(first["state"], "unreviewed")
        self.assertIsNone(first["confidence"])
        self.assertEqual(first["replacement_work"], ["undecided"])
        self.assertEqual(catalog["counts"]["probable_image_sources_unrepresented"], 1)
        self.assertIsNone(catalog["unrepresented_sources"][0]["source"]["sha256"])
        self.assertEqual(catalog["unrepresented_sources"][0]["source"]["input_sha256"], sha("media source"))

    def test_ids_do_not_depend_on_output_directory(self):
        first = build_catalog(media_manifest(".asset-cache/media/a"))
        second = build_catalog(media_manifest(".asset-cache/media/b"))
        self.assertEqual([r["region_id"] for r in first["regions"]],
                         [r["region_id"] for r in second["regions"]])

    def test_manual_confirmation_requires_current_png_hash(self):
        catalog = build_catalog(media_manifest())
        region = catalog["regions"][0]
        review = {"region_id": region["region_id"], "png_sha256": region["png"]["sha256"],
                  "state": "confirmed_japanese_text", "confidence": 1.0,
                  "evidence": {"method": "manual_visual", "reference": "synthetic review record"},
                  "replacement_work": ["string_rendering"]}
        reviewed = build_catalog(media_manifest(), [review])
        self.assertEqual(reviewed["regions"][0]["state"], "confirmed_japanese_text")
        self.assertEqual(reviewed["regions"][0]["replacement_work"], ["string_rendering"])
        stale = copy.deepcopy(review)
        stale["png_sha256"] = sha("stale")
        with self.assertRaisesRegex(CatalogError, "hash is stale"):
            build_catalog(media_manifest(), [stale])

    def test_ocr_cannot_confirm_and_unknown_fields_are_rejected(self):
        region = build_catalog(media_manifest())["regions"][0]
        review = {"region_id": region["region_id"], "png_sha256": region["png"]["sha256"],
                  "state": "confirmed_japanese_text", "confidence": 0.99,
                  "evidence": {"method": "ocr_candidate", "reference": "synthetic OCR"}}
        with self.assertRaisesRegex(CatalogError, "manual visual evidence"):
            build_catalog(media_manifest(), [review])
        review["state"] = "suspected"
        review["extra"] = "not allowed"
        with self.assertRaisesRegex(CatalogError, "unknown fields"):
            build_catalog(media_manifest(), [review])

    def test_stale_region_and_invalid_review_states_fail_closed(self):
        region = build_catalog(media_manifest())["regions"][0]
        review = {"region_id": "region-stale", "png_sha256": region["png"]["sha256"],
                  "state": "suspected", "confidence": 0.5}
        with self.assertRaisesRegex(CatalogError, "unknown or stale region"):
            build_catalog(media_manifest(), [review])
        review["region_id"] = region["region_id"]
        review["state"] = "translated"
        with self.assertRaisesRegex(CatalogError, "invalid review state"):
            build_catalog(media_manifest(), [review])

    def test_unreviewed_requires_null_confidence(self):
        region = build_catalog(media_manifest())["regions"][0]
        review = {"region_id": region["region_id"], "png_sha256": region["png"]["sha256"],
                  "state": "unreviewed", "confidence": 0.0}
        with self.assertRaisesRegex(CatalogError, "must be null"):
            build_catalog(media_manifest(), [review])

    def test_confirmed_other_text_requires_manual_visual_evidence(self):
        region = build_catalog(media_manifest())["regions"][0]
        review = {"region_id": region["region_id"], "png_sha256": region["png"]["sha256"],
                  "state": "confirmed_other_text", "confidence": 0.95,
                  "evidence": {"method": "manual_visual", "reference": "synthetic visual review"}}
        catalog = build_catalog(media_manifest(), [review])
        self.assertEqual(catalog["regions"][0]["state"], "confirmed_other_text")
        self.assertEqual(catalog["counts"]["reviews"]["confirmed_other_text"], 1)
        self.assertIn("confirmed_other_text", catalog["review_states"])
        review["evidence"] = {"method": "ocr_candidate", "reference": "synthetic OCR"}
        with self.assertRaisesRegex(CatalogError, "manual visual evidence"):
            build_catalog(media_manifest(), [review])

    def test_frame_variants_keep_dependencies_and_evidence_backed_context(self):
        manifest = media_manifest()
        parent = manifest["assets"][0]
        parent["format"] = "psx-main-ui"
        parent["source_dependencies"] = {"exe": {"path": "invented/paired.exe", "sha256": sha("exe")}}
        parent["inspection"]["candidates"] = []
        parent["export"]["metadata"] = {"format": "synthetic-frame-pack", "alpha_policy": "source metadata"}
        parent["export"]["images"] = [
            {"png": "frame-002.png", "frame_index": 2, "variant": "body", "width": 20,
             "height": 9, "parts": [{"source": "invented source part"}]},
            {"png": "frame-000.png", "frame_index": 0, "variant": "body", "width": 18,
             "height": 8, "parts": [{"source": "invented source part"}]},
        ]
        parent["outputs"] = [
            {"path": f".asset-cache/media/run/assets/{parent['id']}/frame-002.png",
             "size": 1, "sha256": sha("frame2")},
            {"path": f".asset-cache/media/run/assets/{parent['id']}/frame-000.png",
             "size": 1, "sha256": sha("frame0")},
        ]
        with patch("assets.visual_text._source_profile", return_value={
                "priority": 0, "source_role": "synthetic profile",
                "screen_context": ["synthetic context"], "evidence": ["synthetic source evidence"],
                "frame_roles": {0: "synthetic frame role"}}):
            catalog = build_catalog(manifest)
        frame_regions = catalog["regions"][:2]
        self.assertEqual([r["frame"] for r in frame_regions], [0, 2])
        self.assertEqual(frame_regions[0]["region"]["width"], 18)
        self.assertEqual(frame_regions[0]["region"]["height"], 8)
        self.assertEqual(frame_regions[0]["source"]["dependencies"], parent["source_dependencies"])
        self.assertEqual(frame_regions[0]["screen_context"]["status"], "candidate")
        self.assertEqual(frame_regions[0]["screen_context"]["contexts"], ["synthetic context"])
        self.assertEqual(frame_regions[0]["screen_context"]["frame_role"], "synthetic frame role")
        self.assertEqual(frame_regions[0]["source_profile"]["frame_roles"], {0: "synthetic frame role"})
        self.assertEqual(frame_regions[0]["variant_metadata"]["parts"], [{"source": "invented source part"}])

    def test_diagnostic_variant_keeps_unknown_palette_and_no_runtime_alpha(self):
        manifest = media_manifest()
        parent = manifest["assets"][0]
        parent["export"]["metadata"] = {"alpha_policy": "global alpha claim"}
        parent["export"]["images"] = [{
            "png": "diagnostic.png", "frame_index": 4, "variant": "diagnostic",
            "width": 6, "height": 3, "preview_kind": "source_index_diagnostic",
            "source_palette_words": [17, 34], "palette_status": "unknown",
            "source_clut_vram": {"x": 9, "y": 11},
            "alpha_policy": "opaque diagnostic preview; no runtime alpha semantics",
        }]
        parent["outputs"] = [{"path": ".asset-cache/media/diagnostic.png", "size": 1,
                              "sha256": sha("diagnostic") }]
        catalog = build_catalog(manifest)
        region = catalog["regions"][0]
        self.assertEqual(region["clut"]["palette_words"], [17, 34])
        self.assertEqual(region["clut"]["palette_status"], "unknown")
        self.assertEqual(region["clut"]["vram"], {"x": 9, "y": 11})
        self.assertEqual(region["alpha_handling"], "opaque diagnostic preview; no runtime alpha semantics")
        self.assertIsNone(region["stp"])
        self.assertEqual(region["stp_status"], "not_applicable")

    def test_cli_reads_json_and_uses_manifest_writer(self):
        with tempfile.TemporaryDirectory() as temporary:
            source = Path(temporary) / "media.json"
            source.write_text(json.dumps(media_manifest()), encoding="utf-8")
            with patch("assets.visual_text._verify_catalog_files", return_value=0), \
                    patch("assets.visual_text.write_manifest", return_value={
                        "path": ".asset-cache/media/catalog.json", "sha256": sha("catalog")}):
                result = main(["--media-manifest", str(source),
                               "--output", ".asset-cache/media/catalog.json"])
        self.assertEqual(result, 0)

    def test_cli_artifact_validation_hashes_png_and_stp(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            png = root / "image.png"
            stp = root / "image.stp.png"
            png.write_bytes(b"invented png")
            stp.write_bytes(b"invented mask")
            hashes = {"image.png": hashlib.sha256(png.read_bytes()).hexdigest(),
                      "image.stp.png": hashlib.sha256(stp.read_bytes()).hexdigest()}
            catalog = {"regions": [{
                "png": {"path": ".asset-cache/image.png", "sha256": hashes["image.png"]},
                "stp": {"path": ".asset-cache/image.stp.png", "sha256": hashes["image.stp.png"]},
            }]}
            def resolve(path):
                return root / Path(path).name
            with patch("assets.visual_text.output_root", side_effect=resolve):
                self.assertEqual(_verify_catalog_files(catalog), 2)
                catalog["regions"][0]["png"]["sha256"] = sha("stale")
                with self.assertRaisesRegex(CatalogError, "hash is stale"):
                    _verify_catalog_files(catalog)


if __name__ == "__main__":
    unittest.main()
