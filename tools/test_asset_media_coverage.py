"""Invented manifests for source-vs-preview coverage accounting."""
import copy
import unittest

from tools.assets.media_coverage import CoverageError, _compact_location, build_media_coverage


def media_fixture():
    return {
        "schema": "tokimemo.media.v1",
        "input": {"format": "directory", "source_sha256": "0" * 64},
        "assets": [
            {"id": "main", "source": {"path": "CDROM/FUKUTEST/MAINROM.NBN", "size": 128,
                                          "sha256": "1" * 64, "location": {"lba": 3}},
             "format": "tokimemo-main-ui", "status": "exported",
             "export": {"format": "tokimemo-main-ui", "status": "exported",
                        "images": [{"frame_index": 1, "variant": "base", "parts": [{"u": 0}]}],
                        "metadata": {"source_size": 128, "sheet_bytes": 16,
                                     "frame_table_referenced_sheet_offsets": [0, 16],
                                     "unmapped_sheet_extents": [{"offset": 32, "size": 16}],
                                     "trailing_extent": {"offset": 120, "size": 8},
                                     "coverage_policy": "only selected regions"}}},
            {"id": "portrait", "source": {"path": "CDROM/BUSTUP/TEST.BIN", "size": 20,
                                              "sha256": "2" * 64},
             "format": "tokimemo-portrait", "status": "exported",
             "export": {"format": "tokimemo-portrait", "images": [
                 {"variant": "source-page-0", "preview_kind": "source_texture_page",
                  "source_sheet_offset": 4, "source_sheet_size": 8,
                  "source_sheet_sha256": "3" * 64, "parts": []},
                 {"variant": "static-frame", "preview_kind": "static_frame_composite",
                  "parts": [{"part_index": 0}]},
             ], "metadata": {"source_size": 20,
                              "unrepresented_extents": [{"offset": 12, "size": 8, "status": "trailer"}]}}},
            {"id": "ambiguous-bin", "source": {"path": "CDROM/OTHER/THING.BIN", "size": 8},
             "format": "bin", "status": "unsupported", "reason": "game-specific container"},
        ],
    }


class MediaCoverageTests(unittest.TestCase):
    def test_source_counts_stay_separate_from_image_regions_and_review(self):
        catalog = {"schema": "tokimemo.visual-text.v1", "regions": [
            {"region_id": "region-main", "parent_asset_id": "main", "state": "confirmed_japanese_text"},
            {"region_id": "region-portrait", "parent_asset_id": "portrait", "state": "unreviewed"},
        ]}
        result = build_media_coverage(media_fixture(), visual_catalog=catalog)
        self.assertFalse(result["corpus_complete"])
        self.assertEqual(result["counts"]["source_files"], 3)
        self.assertEqual(result["counts"]["decoded_image_variants"], 3)
        self.assertEqual(result["counts"]["visual_regions"], 2)
        self.assertEqual(result["counts"]["visual_review_states"],
                         {"confirmed_japanese_text": 1, "unreviewed": 1})
        main = next(item for item in result["sources"] if item["asset_id"] == "main")
        self.assertEqual(main["coverage_status"], "decoded")
        self.assertEqual(main["source_evidence"]["source_role"], "main_screen_ui_pack")
        self.assertEqual(main["source_components"]["frame_part_region_count"], 1)
        self.assertEqual(len(main["source_components"]["unmapped_sheet_extents"]), 1)
        portrait = next(item for item in result["sources"] if item["asset_id"] == "portrait")
        self.assertEqual(len(portrait["source_components"]["texture_page_extents"]), 1)
        self.assertEqual(portrait["source_components"]["static_frame_part_count"], 1)

    def test_unknown_bin_suffix_is_not_claimed_as_graphics(self):
        result = build_media_coverage(media_fixture())
        item = next(item for item in result["sources"] if item["asset_id"] == "ambiguous-bin")
        self.assertEqual(item["source_classification"]["class"], "ambiguous_binary_source")
        self.assertIsNone(item["source_evidence"])
        self.assertEqual(item["coverage_status"], "unsupported")

    def test_context_decoded_bs_is_distinguished_from_unsupported_bs(self):
        manifest = media_fixture()
        manifest["assets"].extend([
            {"id": "mdec", "source": {"path": "CDROM/SCENE/FRAME.BS", "size": 320,
                                          "sha256": "4" * 64, "location": {"lba": 44}},
             "format": "sony-bs-mdec", "status": "exported",
             "export": {"format": "sony-bs-mdec", "status": "exported",
                         "images": [{"variant": "background", "preview_kind": "decoded_background",
                                     "width": 320, "height": 88, "parts": []}],
                         "metadata": {"source_size": 320, "source_sha256": "4" * 64,
                                      "bs_version": 2, "quantization": 4,
                                      "declared_mdec_bytes": 128, "decoded_runlevel_halfwords": 32,
                                      "block_count": 6, "macroblock_count": 1,
                                      "escape_codes": 2, "zero_level_escape_codes": 1,
                                      "end_marker_bit_offset": 400, "consumed_stream_bits": 410,
                                      "unused_bits_in_final_source_halfword": 6,
                                      "trailing_extent": {"offset": 64, "size": 256},
                                      "width": 320, "height": 88, "padded_height": 96,
                                      "geometry_context": {"width": 320, "height": 88,
                                                            "evidence": ["caller:dec_bg_cd_read"]},
                                      "hardware_pixel_parity_verified": False,
                                      "unrepresented_pixel_regions": [],
                                      "source_output_bit15": 1,
                                      "stp_policy": "constant one mask",
                                      "limitations": ["hardware parity unknown"]}},
             "inspection": {"metadata": {"coefficient_order": "source-scan-order"}}},
            {"id": "unsupported-bs", "source": {"path": "CDROM/SCENE/UNKNOWN.BS", "size": 10},
             "format": "bs", "status": "unsupported", "reason": "no background context"},
        ])
        result = build_media_coverage(manifest)
        decoded = next(item for item in result["sources"] if item["asset_id"] == "mdec")
        unsupported = next(item for item in result["sources"] if item["asset_id"] == "unsupported-bs")
        self.assertEqual(decoded["coverage_status"], "decoded")
        self.assertEqual(decoded["source_classification"]["class"], "decoded_mdec_background")
        self.assertEqual(decoded["image_variant_count"], 1)
        self.assertEqual(decoded["source_components"]["block_count"], 6)
        self.assertEqual(decoded["source_components"]["geometry_context"]["width"], 320)
        self.assertFalse(decoded["source_components"]["hardware_pixel_parity_verified"])
        self.assertEqual(decoded["source_components"]["source_output_bit15"], 1)
        self.assertEqual(decoded["source_components"]["stp_policy"], "constant one mask")
        self.assertEqual(decoded["source_components"]["unrepresented_pixel_regions"], [])
        self.assertEqual(decoded["source_components"]["limitations"], ["hardware parity unknown"])
        self.assertEqual(decoded["source_components"]["coefficient_order"], "source-scan-order")
        self.assertEqual(unsupported["coverage_status"], "unsupported")
        self.assertEqual(unsupported["source_classification"]["class"], "known_graphic_container_unsupported")

    def test_title_profile_metadata_is_retained_without_claiming_full_source_coverage(self):
        manifest = media_fixture()
        manifest["assets"].append({
            "id": "title", "source": {"path": "CDROM/FUKUTEST/TITLE.NBN", "size": 512},
            "format": "tokimemo-title-ui", "status": "exported",
            "export": {"format": "tokimemo-title-ui", "images": [
                {"png": "title.png", "variant": "base", "frame_index": 0}],
                "metadata": {"background_sheet_offsets": [0, 0x4000],
                             "actor_sheet_offsets": [0x8000],
                             "uninterpreted_frame_record_gaps": [{"offset": 12, "size": 4}],
                             "partially_represented_extents": [{"offset": 0x8000, "size": 0x4000}],
                             "unmapped_model_palette_uploads": [{"source_offset": 0x4C200}],
                             "source_sequence": [{"frame": 0}]}}})
        result = build_media_coverage(manifest)
        title = next(item for item in result["sources"] if item["asset_id"] == "title")
        components = title["source_components"]
        self.assertEqual(title["source_classification"]["class"], "decoded_graphic_container")
        self.assertEqual(components["background_sheet_offsets"], [0, 0x4000])
        self.assertEqual(components["actor_sheet_offsets"], [0x8000])
        self.assertEqual(components["uninterpreted_frame_record_gaps"], [{"offset": 12, "size": 4}])
        self.assertEqual(components["partially_represented_extents"], [{"offset": 0x8000, "size": 0x4000}])
        self.assertEqual(components["unmapped_model_palette_uploads"], [{"source_offset": 0x4C200}])
        self.assertEqual(components["source_sequence"], [{"frame": 0}])
        self.assertFalse(result["corpus_complete"])

    def test_new_original_ui_profiles_keep_part_and_opaque_extent_evidence(self):
        for fmt in ('tokimemo-filesel-ui', 'tokimemo-option-ui', 'tokimemo-exam-ui', 'tokimemo-name-ui'):
            with self.subTest(format=fmt):
                manifest = media_fixture()
                manifest['assets'].append({
                    'id': 'new-ui', 'source': {'path': 'INVENTED.NBN', 'size': 512},
                    'format': fmt, 'status': 'exported', 'export': {'images': [
                        {'frame_index': 0, 'variant': 'base', 'width': 8, 'height': 8,
                         'clut_vram': [0, 0x1F0], 'source_palette_words': [0, 31],
                         'parts': [{'source_sheet_offset': 16, 'u': 8, 'v': 16,
                                    'width': 8, 'height': 8, 'palette_source_offset': 64,
                                    'source_palette_words': [0, 0x3E0]}]}],
                        'metadata': {'texture_pages': [{'source_offset': 16}],
                                     'uninterpreted_extents': [{'offset': 128, 'size': 32}],
                                     'direct_texture_page_upload': {'source_offset': 256},
                                     'actor_runtime_palette_override': 0x10}}})
                result = build_media_coverage(manifest)
                source = next(s for s in result['sources'] if s['asset_id'] == 'new-ui')
                self.assertEqual(source['source_classification']['class'], 'decoded_graphic_container')
                components = source['source_components']
                self.assertEqual(components['texture_pages'], [{'source_offset': 16}])
                self.assertEqual(components['uninterpreted_extents'], [{'offset': 128, 'size': 32}])
                self.assertEqual(components['direct_texture_page_upload'], {'source_offset': 256})
                self.assertEqual(components['actor_runtime_palette_override'], 0x10)
                part = components['static_ui_frame_regions'][0]['parts'][0]
                self.assertEqual((part['source_sheet_offset'], part['u'], part['v']), (16, 8, 16))
                self.assertEqual(part['source_palette_words'], [0, 0x3E0])
                self.assertEqual(components['static_ui_frame_regions'][0]['image_palette']['clut_vram'], [0, 0x1F0])
                self.assertEqual(components['static_ui_frame_regions'][0]['image_palette']['source_palette_words'], [0, 31])
                self.assertFalse(result['corpus_complete'])

    def test_mdec_coefficient_census_without_geometry_is_not_classified_as_decoded(self):
        manifest = media_fixture()
        manifest["assets"].append({
            "id": "mdec-unknown-geometry",
            "source": {"path": "CDROM/SCENE/UNKNOWN.BS", "size": 512, "sha256": "5" * 64},
            "format": "sony-bs-mdec", "status": "unsupported",
            "reason": "dimensions require source-backed caller context",
            "inspection": {"status": "unsupported", "format": "sony-bs-mdec",
                            "metadata": {"block_count": 12, "macroblock_count": 2,
                                         "bs_version": 3, "declared_mdec_bytes": 512}},
        })
        result = build_media_coverage(manifest)
        item = next(source for source in result["sources"] if source["asset_id"] == "mdec-unknown-geometry")
        self.assertEqual(item["source_classification"]["class"], "known_mdec_background_geometry_unresolved")
        self.assertEqual(item["coverage_status"], "unsupported")
        self.assertEqual(item["image_variant_count"], 0)
        self.assertEqual(result["counts"]["decoded_image_variants"], 3)
        self.assertEqual(item["source_components"]["block_count"], 12)

    def test_profiled_format_without_exported_regions_is_not_classified_as_decoded(self):
        manifest = media_fixture()
        main = manifest["assets"][0]
        main["status"] = "unsupported"
        main["export"]["images"] = []
        result = build_media_coverage(manifest)
        item = next(source for source in result["sources"] if source["asset_id"] == "main")
        self.assertEqual(item["source_classification"]["class"], "known_graphic_container_unsupported")
        self.assertEqual(item["image_variant_count"], 0)

    def test_source_inventory_adds_streams_without_inventing_payload_hash(self):
        inventory = {"schema": "tokimemo.media.v1", "assets": [
            {"id": "stream", "format": "xa", "status": "unsupported",
             "unsupported": "Form2 stream", "source": {"path": "CDROM/STREAM/VOICE.XA",
                 "size": 2324, "location": {"lba": 90, "sectors": [{"lba": 90}]}}},
        ]}
        result = build_media_coverage(media_fixture(), source_inventory=inventory)
        stream = next(item for item in result["sources"] if item["path"].endswith("VOICE.XA"))
        self.assertEqual(stream["coverage_status"], "unsupported")
        self.assertIsNone(stream["sha256"])
        self.assertEqual(stream["sha256_status"], "not_available_for_source_record")
        self.assertEqual(stream["location"]["sector_runs"][0]["first_lba"], 90)
        self.assertEqual(stream["source_classification"]["class"], "audio_or_stream_source")

    def test_rejects_unknown_catalog_schema_and_duplicate_source_paths(self):
        with self.assertRaises(CoverageError):
            build_media_coverage(media_fixture(), visual_catalog={"schema": "bad", "regions": []})
        duplicate = media_fixture()
        duplicate["assets"].append(copy.deepcopy(duplicate["assets"][0]))
        with self.assertRaises(CoverageError):
            build_media_coverage(duplicate)

    def test_rejects_stale_inventory_provenance_and_preserves_original_lba(self):
        inventory = {"schema": "tokimemo.media.v1", "input": {"format": "mode2-2352"}, "assets": [
            {"id": "main", "status": "supported", "format": "nbn",
             "source": {"path": "CDROM/FUKUTEST/MAINROM.NBN", "size": 128,
                        "sha256": "1" * 64, "location": {"lba": 123, "extent_bytes": 128}}}
        ]}
        media = media_fixture()
        media["assets"][0]["source"]["location"] = {"path": "CDROM/FUKUTEST/MAINROM.NBN"}
        result = build_media_coverage(media, source_inventory=inventory)
        main = next(item for item in result["sources"] if item["path"].endswith("MAINROM.NBN"))
        self.assertEqual(main["location"], {"path": "CDROM/FUKUTEST/MAINROM.NBN"})
        self.assertEqual(main["original_input_location"], {"lba": 123, "extent_bytes": 128})
        self.assertEqual(result["source_inventory_input"]["format"], "mode2-2352")
        inventory["assets"][0]["source"]["sha256"] = "f" * 64
        with self.assertRaisesRegex(CoverageError, "hashes differ"):
            build_media_coverage(media, source_inventory=inventory)

    def test_rejects_duplicate_inventory_paths_and_invalid_visual_review_state(self):
        inventory = {"schema": "tokimemo.media.v1", "assets": [
            {"id": "a", "source": {"path": "same.bin"}},
            {"id": "b", "source": {"path": "same.bin"}},
        ]}
        with self.assertRaisesRegex(CoverageError, "duplicate source inventory path"):
            build_media_coverage(media_fixture(), source_inventory=inventory)
        catalog = {"schema": "tokimemo.visual-text.v1", "regions": [
            {"region_id": "bad", "parent_asset_id": "main", "state": "complete"},
        ]}
        with self.assertRaisesRegex(CoverageError, "unknown review state"):
            build_media_coverage(media_fixture(), visual_catalog=catalog)

    def test_sector_run_compression_preserves_contiguous_extent_details(self):
        compact = _compact_location({"lba": 30, "extent_bytes": 4097, "sectors": [
            {"lba": 30, "payload_bytes": 2048, "form2": False, "directory_subheader_fallback": False},
            {"lba": 31, "payload_bytes": 2048, "form2": False, "directory_subheader_fallback": False},
            {"lba": 32, "payload_bytes": 2048, "form2": False, "directory_subheader_fallback": False},
        ]})
        self.assertEqual(compact["extent_bytes"], 4097)
        self.assertEqual(compact["sector_record_count"], 3)
        self.assertEqual(compact["sector_runs"], [{
            "count": 3, "sector_metadata": {"payload_bytes": 2048, "form2": False,
                                             "directory_subheader_fallback": False},
            "address_kind": "lba", "first_lba": 30,
        }])


if __name__ == "__main__":
    unittest.main()
