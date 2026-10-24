"""Synthetic tests for a safe, browseable media preview gallery."""
import hashlib
import unittest
from unittest.mock import patch

from assets.media_gallery import GalleryError, build_gallery, _verify_artifacts
from assets.visual_text import _stable_region_id


def sha(value):
    return hashlib.sha256(value.encode("ascii")).hexdigest()


def fixture(catalog=False):
    asset_id = "asset-gallery-demo"
    variants = [
        {"png": "base.png", "stp_png": "base.stp.png", "frame_index": 0,
         "variant": "base", "width": 4, "height": 2, "palette_index": 0},
        {"png": "hud.png", "stp_png": "hud.stp.png", "frame_index": 1,
         "variant": "hud-43", "width": 4, "height": 2, "palette_index": 1},
        {"png": "diagnostic.png", "frame_index": 2, "variant": "source-index",
         "preview_kind": "source_index_diagnostic", "width": 4, "height": 2,
         "palette_status": "unknown"},
        {"png": "source-page.png", "frame_index": 3, "variant": "source-page-0",
         "preview_kind": "source_texture_page", "width": 4, "height": 2},
    ]
    outputs = []
    for variant in variants:
        outputs.append({"path": f".asset-cache/media/run/{variant['png']}", "size": 12,
                        "sha256": sha(variant["png"])})
        if variant.get("stp_png"):
            outputs.append({"path": f".asset-cache/media/run/{variant['stp_png']}", "size": 8,
                            "sha256": sha(variant["stp_png"])})
    original = {"path": ".asset-cache/media/run/original.bin", "size": 20,
                "sha256": sha("original")}
    asset = {
        "id": asset_id, "format": "tokimemo-main-ui", "status": "exported",
        "source": {"path": "invented/UI.NBN", "sha256": sha("source"),
                   "location": {"path": "invented/UI.NBN"}},
        "source_profile": {"source_role": "main_screen_ui_pack", "priority": 0,
                           "screen_context": ["synthetic context"]},
        "export": {"status": "exported", "images": variants},
        "outputs": outputs, "original": original,
    }
    unsupported = {"id": "asset-bg-demo", "status": "deferred", "format": "bs",
                   "source": {"path": "invented/scene.BS", "sha256": None,
                              "location": {"path": "invented/scene.BS"}},
                   "reason": "synthetic MDEC source not decoded"}
    media = {"schema": "tokimemo.media.v1", "input": {"source_sha256": sha("disc")},
             "assets": [asset, unsupported]}
    visual = None
    if catalog:
        visual_regions = []
        for variant in variants:
            frame = variant["frame_index"]
            rect = {"x": 0, "y": 0, "width": variant["width"], "height": variant["height"]}
            visual_regions.append({
                "region_id": _stable_region_id(asset_id, frame, variant.get("palette_index"), rect,
                                               variant.get("variant")),
                "parent_asset_id": asset_id,
                "frame": frame,
                "png": {"path": f".asset-cache/media/run/{variant['png']}",
                        "sha256": sha(variant["png"])},
                "state": "confirmed_japanese_text" if frame == 0 else "unreviewed",
                "confidence": 1.0 if frame == 0 else None,
                "replacement_work": ["string_rendering"] if frame == 0 else ["undecided"],
                "screen_context": {"status": "candidate", "contexts": ["synthetic context"],
                                   "source_role": "main_screen_ui_pack", "priority": 0},
            })
        visual = {"schema": "tokimemo.visual-text.v1", "regions": visual_regions}
    return media, visual


class GalleryTests(unittest.TestCase):
    def test_color_gallery_hides_candidates_diagnostics_and_pages_by_default(self):
        media, _ = fixture()
        page = build_gallery(media)
        self.assertIn("base.png", page)
        self.assertIn("hud.png", page)
        self.assertIn('class="card base"', page)
        self.assertIn('class="card candidate"', page)
        self.assertIn('class="card diagnostic"', page)
        self.assertIn('class="card source-page"', page)
        self.assertIn("Show candidate palette/frame variants", page)
        self.assertIn("Show source texture pages", page)
        self.assertIn("Show numeric palette diagnostics", page)
        self.assertIn("PALETTE UNRESOLVED — numeric index diagnostic, not finished art", page)
        self.assertIn('alt="Index diagnostic (grayscale)', page)
        self.assertIn(">Index diagnostic PNG</a>", page)
        self.assertIn('style="width:16px;max-width:100%;height:auto"', page)
        self.assertIn(".card.candidate,.card.diagnostic,.card.source-page{display:none}", page)
        self.assertIn(".debug-link{display:none}", page)
        self.assertIn("../run/base.png", page)
        self.assertIn("../run/base.stp.png", page)
        self.assertIn("Source coverage incomplete.", page)
        self.assertIn("MDEC background records are present but not decoded.", page)
        self.assertIn("Portrait coverage is unresolved.", page)
        self.assertIn("source original", page)

    def test_visual_review_joins_by_id_and_png_hash(self):
        media, visual = fixture(catalog=True)
        page = build_gallery(media, visual)
        self.assertIn("confirmed_japanese_text", page)
        self.assertIn("string_rendering", page)
        visual["regions"][0]["png"]["sha256"] = sha("stale")
        with self.assertRaisesRegex(GalleryError, "review is stale"):
            build_gallery(media, visual)

    def test_html_escapes_source_and_profile_values(self):
        media, _ = fixture()
        asset = media["assets"][0]
        asset["source"]["path"] = 'invented/<img src=x onerror="bad">.NBN'
        asset["source_profile"]["screen_context"] = ['<script>alert("x")</script>']
        page = build_gallery(media)
        self.assertIn("&lt;img src=x onerror=&quot;bad&quot;&gt;.NBN", page)
        self.assertIn("&lt;script&gt;alert(&quot;x&quot;)&lt;/script&gt;", page)
        self.assertNotIn('<script>alert("x")</script>', page)

    def test_output_path_must_stay_inside_cache(self):
        media, _ = fixture()
        with self.assertRaisesRegex(ValueError, "below .asset-cache"):
            build_gallery(media, output_path="/tmp/escaped.html")

    def test_artifact_verification_checks_hash_and_cache_path(self):
        media, _ = fixture()
        with patch("assets.media_gallery.output_root", side_effect=lambda p: type("P", (), {
                "is_file": lambda self: True,
                "read_bytes": lambda self: b"invented bytes",
        })()):
            with self.assertRaisesRegex(GalleryError, "hash"):
                _verify_artifacts(media, None)

    def test_stale_visual_catalog_region_missing_required_artifact_fails(self):
        media, visual = fixture(catalog=True)
        del visual["regions"][0]["png"]["sha256"]
        with self.assertRaises(GalleryError):
            build_gallery(media, visual)

    def test_tim_palette_variant_uses_global_dimensions_and_local_origin(self):
        media, _ = fixture()
        image_asset = media["assets"][0]
        image_asset["format"] = "psx-tim"
        image_asset["export"]["metadata"] = {
            "width": 20, "height": 16,
            "image_block": {"x": 27, "y": 31, "width_words": 10, "height": 16},
        }
        variant = {"png": "tim.png", "palette_index": 0}
        image_asset["export"]["images"] = [variant]
        image_asset["outputs"] = [{"path": ".asset-cache/media/run/tim.png", "size": 1,
                                   "sha256": sha("tim.png")}]
        page = build_gallery(media)
        self.assertIn("0, 0 · 20 × 16", page)
        self.assertIn('style="width:80px;max-width:100%;height:auto"', page)


if __name__ == "__main__":
    unittest.main()
