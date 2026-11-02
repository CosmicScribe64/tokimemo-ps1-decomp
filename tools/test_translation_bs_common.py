"""T-39400 / T-39412: shared BS draft helpers (citations, macroblocks, previews, measures, environment). No game data."""
from pathlib import Path
import tempfile
import unittest

import numpy as np
from PIL import Image

import translation_bs_common as common

SOURCE = """#include "common.h"

void scene_load(void) {
    dec_bg_cd_read(0x4020, 1);
    dec_bg_decode_to_vram(1);
}

s32 scene_other(s32 arg0) {
    dec_bg_decode_to_vram(0);
    return arg0;
}
"""
CITES = [('src/a.c', 'scene_load', 'dec_bg_cd_read(0x4020, 1)', 4), ('src/a.c', 'scene_load', 'dec_bg_decode_to_vram(1)', 5)]


class CitationTests(unittest.TestCase):
    def check(self, text, cites=CITES, checks=()):
        with tempfile.TemporaryDirectory() as root:
            (Path(root) / 'src').mkdir()
            (Path(root) / 'src/a.c').write_text(text)
            return common.check_citations(cites, checks, root)

    def test_function_body_runs_from_the_definition_to_its_closing_brace(self):
        body = common.function_body(SOURCE, 'scene_load')
        self.assertTrue(body.startswith('void scene_load(void) {'))
        self.assertTrue(body.endswith('}'))
        self.assertNotIn('scene_other', body)
        self.assertIn('return arg0', common.function_body(SOURCE, 'scene_other'))
        self.assertIsNone(common.function_body(SOURCE, 'scene'))

    def test_a_line_shift_does_not_break_a_citation(self):
        self.assertEqual(self.check(SOURCE), [])
        self.assertEqual(self.check('/* a new comment */\n\n\n' + SOURCE), [])

    def test_a_call_moved_to_another_function_or_a_lost_function_is_reported(self):
        moved = SOURCE.replace('    dec_bg_decode_to_vram(1);\n', '').replace('dec_bg_decode_to_vram(0);', 'dec_bg_decode_to_vram(1);')
        self.assertEqual(len(self.check(moved)), 1)
        self.assertEqual(len(self.check(SOURCE.replace('scene_load', 'scene_init'))), 2)
        self.assertEqual(len(self.check(SOURCE, cites=[], checks=[('src/a.c', 'cd_read_sectors(9')])), 1)

    def test_manifest_records_keep_the_line_for_humans(self):
        record = common.cite_record(CITES[0])
        self.assertEqual((record['function'], record['line']), ('scene_load', 4))


class HelperTests(unittest.TestCase):
    def test_macroblocks_of_a_rectangle(self):
        indices, bounds = common.macroblocks([30, 10, 5, 8], 160)
        self.assertEqual(indices, [10, 11, 20, 21])
        self.assertEqual(bounds, [16, 0, 32, 32])

    def test_rgb8_uses_the_replicate_rule(self):
        self.assertEqual(int(common.rgb8(np.array([31]))[0]), 255)
        self.assertEqual(int(common.rgb8(np.array([16]))[0]), 132)

    def test_source_equal_pixels_counts_written_pixels_only(self):
        source = np.full((4, 5, 3), 12)
        english = source.copy()
        english[0, 0] = 3
        rgb = Image.fromarray(common.rgb8(english), 'RGB')
        self.assertEqual(common.source_equal_pixels(rgb, source), 19)
        rgba = np.zeros((4, 5, 4), np.uint8)
        rgba[..., :3] = common.rgb8(english)
        rgba[0, 0, 3] = rgba[0, 1, 3] = 255
        self.assertEqual(common.source_equal_pixels(Image.fromarray(rgba, 'RGBA'), source), 1)

    def test_zoom_pair_scale_limits(self):
        picture = np.zeros((40, 60, 3), int)
        self.assertEqual(common.zoom_pair(picture, picture, [10, 10, 4, 4], margin=0).width, 4 * 10)
        self.assertEqual(common.zoom_pair(picture, picture, [10, 10, 4, 4], margin=0, max_scale=12).width, 4 * 12)
        self.assertEqual(common.zoom_pair(picture, picture, [0, 0, 60, 4], margin=0, limit=300).width, 60 * 5)

    def test_environment_mismatch_names_both_versions(self):
        self.assertIsNone(common.environment_mismatch({'environment': common.environment()}))
        message = common.environment_mismatch({'environment': {'pillow': '0.0', 'numpy': '0.0'}})
        self.assertIn("'pillow': '0.0'", message)
        self.assertIn(common.environment()['numpy'], message)
        self.assertIsNotNone(common.environment_mismatch({}))


if __name__ == '__main__':
    unittest.main()
