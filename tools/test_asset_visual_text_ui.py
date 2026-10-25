"""Synthetic tests for source-backed MAINROM context and review todo metadata."""
import hashlib
import unittest

from assets.visual_text_ui import build_main_ui_context
from assets.visual_text import _stable_region_id


def sha(label):
    return hashlib.sha256(label.encode("ascii")).hexdigest()


def fixture():
    asset_id = "asset-main-ui-synthetic"
    variants = []
    outputs = []

    def add(name, frame, variant="base", **extra):
        image = {"png": name + ".png", "frame_index": frame, "variant": variant,
                 "width": 24, "height": 12, **extra}
        variants.append(image)
        outputs.append({"path": ".asset-cache/media/synthetic/" + image["png"],
                        "sha256": sha(image["png"]), "size": 8})
        return image

    add("digit", 0x44, parts=[{"part_index": 0, "palette_exe_address": 0x800C6020,
                               "palette_exe_offset": 0x1234, "clut_vram": [0, 0x1E0],
                               "source_palette_words": [0, 1]}])
    add("digit-43", 0x44, "hud-43")
    add("plate-51", 2, "icon-51")
    for offset in (0x2000, 0x6000, 0xC000, 0x10000, 0x14000):
        add("diag-%x" % offset, None, "source-index-sheet-%05x" % offset,
            preview_kind="source_index_diagnostic", source_sheet_offset=offset,
            source_sheet_size=0x2000, palette_status="unknown")
    metadata = {
        "source_path_profile": "CDROM/FUKUTEST/MAINROM.NBN",
        "source_size": 0x1A800,
        "source_lba": 0x9D85,
        "source_sha256": sha("MAINROM"), "paired_exe_sha256": sha("EXE"), "paired_exe_size": 500,
        "frame_table_referenced_sheet_offsets": [0, 0x4000, 0x8000, 0xA000,
                                                 0xE000, 0x12000, 0x16000, 0x18000],
        "unmapped_sheet_extents": [
            {"offset": offset, "size": 0x2000, "sha256": sha("sheet-%x" % offset),
             "status": "unreviewed; no frame-table part references this sheet"}
            for offset in (0x2000, 0x6000, 0xC000, 0x10000, 0x14000)],
        "sequences": [{"sequence_index": 0,
                       "steps": [{"frame_index": 0x44, "duration": 4}]}],
    }
    media = {"schema": "tokimemo.media.v1", "assets": [{
        "id": asset_id, "format": "tokimemo-main-ui", "status": "exported",
        "source": {"path": "CDROM/FUKUTEST/MAINROM.NBN", "size": 0x1A800,
                   "sha256": sha("MAINROM"), "location": {"path": "CDROM/FUKUTEST/MAINROM.NBN"}},
        "paired_source": {"path": "SLPM_86.053", "size": 500,
                          "sha256": sha("EXE"), "location": {"path": "SLPM_86.053"}},
        "export": {"status": "exported", "metadata": metadata, "images": variants},
        "outputs": outputs,
    }]}
    return media


class MainUiContextTests(unittest.TestCase):
    def test_unmapped_sheets_stay_unresolved_and_review_todo_is_complete(self):
        report = build_main_ui_context(fixture())
        sheets = {item["offset"]: item for item in report["unmapped_sheets"]}
        self.assertEqual(set(sheets), {0x2000, 0x6000, 0xC000, 0x10000, 0x14000})
        self.assertEqual(sheets[0x6000]["immediate_boot_upload_tpage"], 0x1B)
        self.assertEqual(sheets[0x10000]["immediate_boot_upload_tpage"], 0x3B)
        self.assertIsNone(sheets[0x2000]["immediate_boot_upload_tpage"])
        self.assertEqual(sheets[0x2000]["clut"], "unknown")
        self.assertEqual(sheets[0x2000]["direct_quad_use"], "not established")
        diagnostic = next(item for item in report["images"] if item["variant"] == "source-index-sheet-02000")
        self.assertEqual(diagnostic["source_sheet_offsets"], [0x2000])
        self.assertEqual(sheets[0x2000]["diagnostic_region_id"], diagnostic["region_id"])
        self.assertEqual(report["source"]["asset_id"], "asset-main-ui-synthetic")
        self.assertFalse(report["completeness"]["corpus_complete"])
        self.assertFalse(report["completeness"]["visual_text_review_complete"])
        self.assertEqual(report["review_counts"], {"unreviewed": len(report["images"]), "classified": 0})
        self.assertEqual(len(report["local_review_todo"]), len(report["images"]))
        self.assertTrue(all(entry["review_state"] == "unreviewed"
                            for entry in report["local_review_todo"]))

    def test_actor_context_and_palette_binding_are_source_evidence_only(self):
        report = build_main_ui_context(fixture())
        digit = next(item for item in report["images"] if item["variant"] == "base")
        self.assertEqual(digit["usage"]["status"], "source-backed")
        self.assertIn({"actors": list(range(32, 59)),
                       "role": "parameter-panel decimal digit; value selects use"},
                      digit["usage"]["direct_actor_contexts"])
        self.assertEqual(digit["palette_binding"]["status"], "source-default-per-part")
        self.assertTrue(digit["palette_binding"]["runtime_palette_override_possible"])
        self.assertEqual(digit["palette_binding"]["parts"][0]["palette_exe_address"], 0x800C6020)
        candidate = next(item for item in report["images"] if item["variant"] == "hud-43")
        self.assertEqual(candidate["palette_binding"]["palette_exe_address"], 0x800C6280)
        self.assertEqual(candidate["palette_binding"]["part_palette_data"], [])
        self.assertIn("candidate", candidate["palette_binding"]["applicability"])
        self.assertEqual(candidate["review"]["state"], "unreviewed")
        self.assertEqual(digit["usage"]["sequence_references"][0]["actor_slot"], 80)
        self.assertEqual(candidate["region_id"], _stable_region_id(
            "asset-main-ui-synthetic", 0x44, None,
            {"x": 0, "y": 0, "width": 24, "height": 12}, "hud-43"))

    def test_requires_exact_profile_and_complete_unmapped_extent_inventory(self):
        media = fixture()
        del media["assets"][0]["export"]["metadata"]["unmapped_sheet_extents"][0]
        with self.assertRaisesRegex(ValueError, "lacks expected unmapped sheet extent"):
            build_main_ui_context(media)

    def test_requires_bound_source_and_paired_executable_hashes(self):
        media = fixture()
        media["assets"][0]["paired_source"]["sha256"] = sha("wrong executable")
        with self.assertRaisesRegex(ValueError, "paired executable source hash"):
            build_main_ui_context(media)
        media = fixture()
        media["assets"][0]["source"]["sha256"] = sha("different source")
        with self.assertRaisesRegex(ValueError, "source hash differs"):
            build_main_ui_context(media)

    def test_rejects_missing_diagnostic_and_cache_escape_paths(self):
        media = fixture()
        media["assets"][0]["export"]["images"].pop()
        media["assets"][0]["outputs"].pop()
        with self.assertRaisesRegex(ValueError, "lacks diagnostic"):
            build_main_ui_context(media)
        media = fixture()
        media["assets"][0]["outputs"][0]["path"] = ".asset-cache/media/../outside.png"
        with self.assertRaisesRegex(ValueError, "under .asset-cache/media"):
            build_main_ui_context(media)


if __name__ == "__main__":
    unittest.main()
