import importlib.util
import shutil
import tempfile
import unittest
from pathlib import Path

MODULE_PATH = Path(__file__).with_name("spring_command_content.py")
spec = importlib.util.spec_from_file_location("spring_command_content", MODULE_PATH)
content = importlib.util.module_from_spec(spec)
spec.loader.exec_module(content)


class SpringCommandContentTests(unittest.TestCase):
    def setUp(self):
        # Deliberately synthetic: the public unit test checks selector math,
        # while the exporter separately pins and reads the actual disc tables.
        self.tables = {
            "weekday_background_lbas": [1000 + i for i in range(8)],
            "weekday_sd_lbas": [2000 + i for i in range(25)],
            "sd_select_non_summer": list(range(16)),
        }
        self.temp = Path(tempfile.mkdtemp(prefix="t29014-content-test-"))

    def tearDown(self):
        shutil.rmtree(self.temp)

    def test_synthetic_selector_table_for_commands_1_to_3(self):
        expected = {1: (1001, 3, 3, 2003), 2: (1002, 5, 5, 2005), 3: (1003, 7, 7, 2007)}
        for command, values in expected.items():
            route = content.resolve_april_route(self.tables, command, 0xF)
            self.assertEqual((route["background_lba"], route["sd_table_index"], route["sd_set_id"], route["sd_lba"]), values)
            self.assertEqual(route["season_domain"], "months 3–5 (non-summer table)")

    def test_conditional_sd_selector_math(self):
        self.assertEqual(content.resolve_april_route(self.tables, 1, 0)["sd_set_id"], 2)
        self.assertEqual(content.resolve_april_route(self.tables, 1, 0xF)["sd_set_id"], 3)
        self.assertEqual(content.resolve_april_route(self.tables, 3, 0)["sd_set_id"], 6)
        self.assertEqual(content.resolve_april_route(self.tables, 3, 5)["sd_set_id"], 8)
        self.assertEqual(content.resolve_april_route(self.tables, 3, 0xF)["sd_set_id"], 7)

    def test_malformed_table_shapes_are_rejected(self):
        for key, value in (("weekday_background_lbas", [1]), ("weekday_sd_lbas", [1]), ("sd_select_non_summer", [1])):
            table = dict(self.tables)
            table[key] = value
            with self.subTest(key=key), self.assertRaises(ValueError):
                content.resolve_april_route(table, 1, 0xF)

    def test_other_commands_and_malformed_values_stop(self):
        for command in (0, 4, 5, 6, 7, True):
            with self.subTest(command=command), self.assertRaises(ValueError):
                content.resolve_april_route(self.tables, command, 0xF)
        with self.assertRaises(ValueError):
            content.resolve_april_route(self.tables, 1, True)

    def test_safe_output_rejects_traversal_symlink_and_outside_cache(self):
        root = Path(tempfile.mkdtemp(prefix="t29014-output-root-"))
        cache = root / ".asset-cache/port"
        cache.mkdir(parents=True)
        outside = root / "outside"
        outside.mkdir()
        link = cache / "link"
        link.symlink_to(outside, target_is_directory=True)
        try:
            for output in (str(cache / "../escape"), str(link / "pack"), str(outside / "pack")):
                with self.subTest(output=output):
                    with self.assertRaises(ValueError):
                        content._safe_output(root, output)
        finally:
            link.unlink()
            shutil.rmtree(root)


if __name__ == "__main__":
    unittest.main()
