"""Invented-data literal background-call provenance tests (Docker only)."""
import unittest
from assets.media_background_context import build_background_contexts


def inventory():
    return {"schema": "tokimemo.media.v1", "input": {"source_sha256": "a" * 64},
            "assets": [{"id": "invented", "source": {"path": "PICTURE.BS", "size": 4096,
                        "sha256": "b" * 64, "location": {"lba": 100}}},
                       {"id": "other", "source": {"path": "OTHER.BS", "size": 2048,
                        "sha256": "c" * 64, "location": {"lba": 200}}}]}


BODY = "int dec_bg_decode_to_vram(int layer) { if(layer==0) {func_800536AC(0,0,320,88);} else if(layer==1) {func_800536AC(0,0,320,160);} }\n"


class BackgroundContextTests(unittest.TestCase):
    def test_literal_calls_keep_locations_and_ignore_comments_and_strings(self):
        source = BODY + ('/* dec_bg_cd_read(200, 1); */\n'
                         'char *note = "dec_bg_cd_read(200, 1)";\n'
                         'void use(void) { dec_bg_cd_read(101u, 0); dec_bg_decode_to_vram(0); }\n')
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        self.assertEqual(report["counts"]["literal_calls"], 1)
        stream = report["streams"][0]
        self.assertEqual(stream["source_offset"], 2048)
        self.assertEqual(stream["shape_candidates"], [{"width": 320, "height": 88}])
        self.assertEqual(stream["contexts"][0]["line"], 4)
        self.assertEqual(report["unresolved_bs_sources"][0]["parent_asset_id"], "other")
        self.assertFalse(report["source_coverage_complete"])

    def test_multiple_shapes_and_unmapped_calls_do_not_choose_a_shape(self):
        source = BODY + 'void use(void) { dec_bg_cd_read(100, 0); dec_bg_decode_to_vram(0); dec_bg_cd_read(100,1); dec_bg_decode_to_vram(1); dec_bg_cd_read(99,1); }'
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        self.assertEqual(report["streams"][0]["context_status"], "conflicting_shapes")
        self.assertEqual(report["counts"]["unmapped_calls"], 1)

    def test_missing_body_proof_and_computed_calls_stay_unresolved(self):
        report = build_background_contexts(inventory(), {"src/invented.c":
            'void use(void) { dec_bg_cd_read(100,1); dec_bg_cd_read(table[n],1); }'})
        self.assertFalse(report["geometry_body_proved"])
        self.assertEqual(report["streams"][0]["context_status"], "shape_unresolved")
        self.assertEqual(report["counts"]["literal_calls"], 1)

    def test_direct_decoder_overrides_reader_layer_default(self):
        source = BODY + 'void use(void) { dec_bg_cd_read(100,1); func_800536AC(0,0,384,160); }'
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        self.assertEqual(report["streams"][0]["shape_candidates"], [{"width": 384, "height": 160}])
        self.assertEqual(report["streams"][0]["contexts"][0]["geometry_binding"], "direct_decoder_in_same_function")

    def test_layer_branch_mapping_is_read_from_body(self):
        body = BODY.replace('320,88', '320,999').replace('320,160', '320,88').replace('320,999', '320,160')
        source = body + 'void use(void) { dec_bg_cd_read(100,0); dec_bg_decode_to_vram(0); }'
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        self.assertEqual(report["streams"][0]["shape_candidates"], [{"width": 320, "height": 160}])

    def test_raw_read_requires_matching_setup_buffer(self):
        source = ('void use(void) { cd_read_sectors(10,4096,100); cd_read_sectors(1,8192,200); '
                  'func_80053650(0,0,4096); func_800536AC(0,0,320,240); }')
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        self.assertEqual(report["counts"]["literal_calls"], 1)
        stream = report["streams"][0]
        self.assertEqual(stream["shape_candidates"], [{"width": 320, "height": 240}])
        self.assertTrue(stream["contexts"][0]["physical_read_exceeds_logical_file"])

    def test_earlier_rectangle_cannot_bind_later_raw_read(self):
        source = ('void use(void) { cd_read_sectors(10,4096,100); '
                  'func_80053650(0,0,4096); func_800536AC(0,0,384,160); '
                  'cd_read_sectors(10,4096,200); }')
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        streams = {stream["parent_asset_id"]: stream for stream in report["streams"]}
        self.assertEqual(streams["invented"]["shape_candidates"], [{"width": 384, "height": 160}])
        self.assertEqual(streams["other"]["context_status"], "shape_unresolved")
        self.assertEqual(streams["other"]["contexts"][0]["decoder_evidence"], [])

    def test_reader_geometry_stops_at_next_read_and_keeps_unknown_uses(self):
        source = BODY + ('void use(void) { dec_bg_cd_read(100,0); dec_bg_cd_read(200,1); '
                         'dec_bg_decode_to_vram(1); dec_bg_cd_read(100,0); dec_bg_decode_to_vram(0); }')
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        streams = {stream["parent_asset_id"]: stream for stream in report["streams"]}
        self.assertEqual(streams["invented"]["context_status"], "proved_single_shape_with_unresolved_uses")
        self.assertEqual(streams["invented"]["contexts"][0]["shape_candidates"], [])
        self.assertEqual(streams["other"]["shape_candidates"], [{"width": 320, "height": 160}])

    def test_reader_geometry_stops_at_raw_read_before_decoder(self):
        source = BODY + ('void use(void) { dec_bg_cd_read(100,0); '
                         'cd_read_sectors(1,0x80162000,200); dec_bg_decode_to_vram(0); }')
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        streams = {stream["parent_asset_id"]: stream for stream in report["streams"]}
        self.assertEqual(streams["invented"]["context_status"], "shape_unresolved")
        self.assertEqual(streams["invented"]["contexts"][0]["shape_candidates"], [])

    def test_computed_wrapped_read_stops_previous_literal_binding(self):
        source = BODY + ('void use(void) { dec_bg_cd_read(100,0); '
                         'dec_bg_cd_read(next_lba(),0); dec_bg_decode_to_vram(0); }')
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        stream = report["streams"][0]
        self.assertEqual(stream["context_status"], "shape_unresolved")
        self.assertEqual(stream["contexts"][0]["shape_candidates"], [])

    def test_computed_raw_lba_stops_previous_literal_binding(self):
        source = BODY + ('void use(void) { dec_bg_cd_read(100,0); '
                         'cd_read_sectors_nowait(10,0x80162000,next_lba); dec_bg_decode_to_vram(0); }')
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        stream = report["streams"][0]
        self.assertEqual(stream["context_status"], "shape_unresolved")
        self.assertEqual(stream["contexts"][0]["shape_candidates"], [])

    def test_sector_count_requires_reader_body_evidence(self):
        source = BODY + 'void use(void) { dec_bg_cd_read(100,0); dec_bg_decode_to_vram(0); }'
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        self.assertIsNone(report["streams"][0]["contexts"][0]["physical_read_bytes"])
        source += '\nint dec_bg_cd_read(int lba, int layer) { cd_read_sectors_nowait(7,4096,lba); }'
        report = build_background_contexts(inventory(), {"src/invented.c": source})
        self.assertEqual(report["streams"][0]["contexts"][0]["physical_read_bytes"], 7 * 2048)

    def test_computed_read_count_or_unknown_buffer_stops_previous_binding(self):
        for replacement in ('cd_read_sectors(next_count(),4096,200)',
                            'cd_read_sectors(10,select_buffer(a,b),200)'):
            with self.subTest(replacement=replacement):
                source = ('void use(void) { cd_read_sectors(10,4096,100); ' + replacement +
                          '; func_80053650(0,0,4096); func_800536AC(0,0,320,160); }')
                report = build_background_contexts(inventory(), {"src/invented.c": source})
                self.assertEqual(report["streams"][0]["context_status"], "shape_unresolved")

    def test_malformed_inventory_is_rejected(self):
        for value in (None, {"schema": "tokimemo.media.v1", "assets": [None]},
                      {"schema": "tokimemo.media.v1", "assets": [{"id": "bad", "source": {"path": "BAD.BS", "location": None}}]}):
            with self.subTest(value=value), self.assertRaises(ValueError):
                build_background_contexts(value, {})


if __name__ == "__main__":
    unittest.main()
