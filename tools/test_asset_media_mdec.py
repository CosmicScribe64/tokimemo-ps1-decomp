"""Invented-data tests for the source-bound background publication command."""
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

from assets import manifest
from assets.media_mdec_export import export_mdec_source
from tools.test_asset_image_mdec import flat
from tools.test_asset_images import png_pixels


class Source:
    def __init__(self, path):
        self.path = str(path)
        self.data = flat()
    def entries(self):
        return [{"path": "PICTURE.BS", "size": len(self.data), "kind": "file",
                 "sha256": manifest.digest(self.data), "location": {"lba": 100}}]
    def describe(self):
        return {"source_sha256": "a" * 64, "read_only": True}
    def read(self, entry):
        return self.data


def contexts(source):
    return {"schema": "tokimemo.background-context.v1", "streams": [{
        "source": source.entries()[0], "source_offset": 0, "context_status": "proved_single_shape",
        "shape_candidates": [{"width": 320, "height": 88}], "contexts": [
            {"width": 320, "height": 88, "file": "src/invented.c", "line": 3,
             "function": "scene", "symbol": "dec_bg_cd_read", "decoder_evidence": [
                {"file": "src/invented.c", "line": 6, "function": "scene"}]}]}]}


class MdecPublicationTests(unittest.TestCase):
    def test_publish_is_deterministic_and_preserves_active_padded_and_mask_artifacts(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / "input.bin")
            with patch.object(manifest, "REPOSITORY", root), patch.object(manifest, "CACHE", root / ".asset-cache"):
                output = root / ".asset-cache" / "backgrounds"
                report = export_mdec_source(source, output, contexts(source))
                first = (output / "manifest.json").read_bytes()
                record = report["assets"][0]
                self.assertEqual(record["status"], "exported")
                self.assertEqual(len(record["outputs"]), 4)
                self.assertEqual((root / record["original"]["path"]).read_bytes(), source.data)
                self.assertEqual(len(record["export"]["files"]), 4)
                output_files = {Path(item["path"]).name: root / item["path"]
                                for item in record["outputs"]}
                self.assertEqual(set(output_files), set(record["export"]["files"]))

                active_name = record["export"]["images"][0]["png"]
                active_mask_name = record["export"]["images"][0]["stp_png"]
                padded = record["export"]["metadata"]["padded_decode"]
                self.assertEqual(record["export"]["images"][0]["width"], 320)
                self.assertEqual(record["export"]["images"][0]["height"], 88)
                self.assertEqual(padded["width"], 320)
                self.assertEqual(padded["height"], 96)
                self.assertEqual(padded["active_crop"], [0, 0, 320, 88])
                self.assertEqual(padded["padded_rows_not_uploaded_by_caller"], 8)

                padded_name = padded["png"]
                padded_mask_name = padded["stp_png"]
                dimensions = {}
                for name in record["export"]["files"]:
                    data = output_files[name].read_bytes()
                    self.assertEqual(data[:8], b"\x89PNG\r\n\x1a\n")
                    dimensions[name] = (int.from_bytes(data[16:20], "big"),
                                        int.from_bytes(data[20:24], "big"))
                self.assertEqual(dimensions[active_name], (320, 88))
                self.assertEqual(dimensions[padded_name], (320, 96))
                self.assertEqual(dimensions[active_mask_name], (320, 88))
                self.assertEqual(dimensions[padded_mask_name], (320, 96))

                active_pixels = png_pixels(output_files[active_name])
                padded_pixels = png_pixels(output_files[padded_name])
                active_mask = png_pixels(output_files[active_mask_name])
                padded_mask = png_pixels(output_files[padded_mask_name])
                active_stride = 320 * 4 + 1
                padded_stride = 320 * 4 + 1
                mask_active_stride = 320 + 1
                mask_padded_stride = 320 + 1
                for row in range(88):
                    self.assertEqual(active_pixels[row * active_stride:(row + 1) * active_stride],
                                     padded_pixels[row * padded_stride:(row + 1) * padded_stride])
                    self.assertEqual(active_mask[row * mask_active_stride:(row + 1) * mask_active_stride],
                                     padded_mask[row * mask_padded_stride:(row + 1) * mask_padded_stride])
                self.assertTrue(all(active_mask[row * mask_active_stride + 1:(row + 1) * mask_active_stride]
                                    == bytes([255]) * 320 for row in range(88)))

                export_mdec_source(source, output, contexts(source))
                self.assertEqual(first, (output / "manifest.json").read_bytes())

    def test_missing_geometry_retains_coefficient_census(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            with patch.object(manifest, "REPOSITORY", root), patch.object(manifest, "CACHE", root / ".asset-cache"):
                report = export_mdec_source(Source(root / "input.bin"), root / ".asset-cache" / "run",
                                            {"schema": "tokimemo.background-context.v1", "streams": []})
                record = report["assets"][0]
                self.assertEqual(record["status"], "unsupported")
                self.assertEqual(record["inspection"]["metadata"]["macroblock_count"], 120)
                self.assertNotIn("outputs", record)

    def test_context_hash_mismatch_is_rejected_before_output(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / "input.bin")
            proof = contexts(source)
            proof["streams"][0]["source"]["sha256"] = "f" * 64
            with patch.object(manifest, "REPOSITORY", root), patch.object(manifest, "CACHE", root / ".asset-cache"):
                with self.assertRaises(ValueError):
                    export_mdec_source(source, root / ".asset-cache" / "run", proof)

    def test_context_lba_mismatch_is_rejected(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / "input.bin")
            proof = contexts(source)
            proof["streams"][0]["source"]["location"]["lba"] = 101
            with patch.object(manifest, "REPOSITORY", root), patch.object(manifest, "CACHE", root / ".asset-cache"):
                with self.assertRaises(ValueError):
                    export_mdec_source(source, root / ".asset-cache" / "run", proof)

    def test_oversized_background_is_not_read_for_decode(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / "input.bin")
            entry = source.entries()[0]
            entry["size"] = 1024 * 1024 + 2
            source.entries = lambda: [entry]
            source.read = lambda entry: self.fail("oversized source was read for decoding")
            with patch.object(manifest, "REPOSITORY", root), patch.object(manifest, "CACHE", root / ".asset-cache"):
                report = export_mdec_source(source, root / ".asset-cache" / "run",
                                            {"schema": "tokimemo.background-context.v1", "streams": []})
                self.assertEqual(report["assets"][0]["status"], "unsupported")

    def test_oversized_background_clears_baseline_export_claims(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / "input.bin")
            with patch.object(manifest, "REPOSITORY", root), patch.object(manifest, "CACHE", root / ".asset-cache"):
                baseline = export_mdec_source(source, root / ".asset-cache" / "baseline", contexts(source))
                self.assertEqual(baseline["assets"][0]["status"], "exported")
                entry = source.entries()[0]
                entry["size"] = 1024 * 1024 + 1
                source.entries = lambda: [entry]
                source.read = lambda entry: self.fail("oversized source was read for decoding")
                report = export_mdec_source(source, root / ".asset-cache" / "oversized",
                                            {"schema": "tokimemo.background-context.v1", "streams": []}, baseline)
                record = report["assets"][0]
                self.assertEqual(record["status"], "unsupported")
                self.assertNotIn("outputs", record)
                self.assertNotIn("export", record)
                self.assertNotIn("original", record)

    def test_baseline_rejects_unsafe_asset_id(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            source = Source(root / "input.bin")
            baseline = {"schema": manifest.SCHEMA, "assets": [{
                "id": "../../outside", "source": {"path": "PICTURE.BS", "sha256": source.entries()[0]["sha256"]}}]}
            with patch.object(manifest, "REPOSITORY", root), patch.object(manifest, "CACHE", root / ".asset-cache"):
                with self.assertRaisesRegex(ValueError, "safe asset id"):
                    export_mdec_source(source, root / ".asset-cache" / "run", contexts(source), baseline)


if __name__ == "__main__":
    unittest.main()
