"""T-39018 WP-01: the committed checkpoint catalog is schema-valid and consistent.

Uses only the committed catalog, schema and fixture plus mutated copies of them
(no game data). jsonschema is required, not optional.
Run: tools/port/run_playtest_unit_tests.sh
"""
import copy
import json
import unittest

from tools.port import playtest_catalog as pc


class CatalogTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.catalog = pc.load()
        cls.by_key = {entry["key"]: entry for entry in cls.catalog["checkpoints"]}

    def errors(self, catalog):
        return pc.schema_errors(catalog) + pc.semantic_errors(catalog)

    def test_committed_catalog_is_valid(self):
        self.assertEqual(self.errors(self.catalog), [])

    def test_seed_groups_are_present(self):
        groups = {}
        for entry in self.catalog["checkpoints"]:
            groups.setdefault(entry["group"], []).append(entry)
        rei = groups["rei-stage8-10"]
        self.assertEqual(len(rei), 8 + 12 + 9)
        compared = [e for e in rei if e["expected"]["status"] != "unsynchronizable"]
        self.assertEqual(len(compared), 25)
        self.assertIn("first-school", self.by_key)
        self.assertEqual(len(groups["name-entry"]), 7)
        self.assertEqual(len(groups["newgame-golden"]), 8)
        self.assertEqual(len(groups["opening-frame-timing"]), 104)

    def test_t39008_unsynchronizable_entries(self):
        for key in ("rei-stage8-0", "rei-stage10-0", "rei-stage10-1", "rei-stage10-8"):
            expected = self.by_key[key]["expected"]
            self.assertEqual(expected["status"], "unsynchronizable", key)
            self.assertIn("T-39008", expected["reason"])
        self.assertIn("port_failure_frame", self.by_key["rei-stage10-8"]["expected"]["reason"])

    def test_every_known_diff_cites_a_defect(self):
        defects = {d["id"] for d in self.catalog["defects"]}
        known = [e for e in self.catalog["checkpoints"] if e["expected"]["status"] == "known_diff"]
        self.assertEqual(len(known), 26)
        for entry in known:
            self.assertIn(entry["expected"]["defect_id"], defects)
        self.assertEqual(self.by_key["rei-stage8-2"]["expected"]["defect_id"], "T-39030")
        self.assertEqual(self.by_key["rei-stage9-3"]["expected"]["defect_id"], "T-39031")

    def test_negative_duplicate_key(self):
        bad = copy.deepcopy(self.catalog)
        bad["checkpoints"].append(copy.deepcopy(bad["checkpoints"][0]))
        self.assertTrue(any("duplicate key" in e for e in self.errors(bad)))

    def test_negative_unknown_defect_reference(self):
        bad = copy.deepcopy(self.catalog)
        entry = next(e for e in bad["checkpoints"] if e["expected"]["status"] == "known_diff")
        entry["expected"]["defect_id"] = "T-99999"
        self.assertTrue(any("unknown defect T-99999" in e for e in self.errors(bad)))
        entry["expected"]["defect_id"] = "T-39030"
        entry["expected"]["related_defects"] = ["T-39008-ZZ9"]
        self.assertTrue(any("unknown defect" in e for e in self.errors(bad)))

    def test_negative_primary_defect_repeated_in_related(self):
        bad = copy.deepcopy(self.catalog)
        entry = next(e for e in bad["checkpoints"] if e["expected"]["status"] == "known_diff")
        entry["expected"]["related_defects"] = [entry["expected"]["defect_id"]]
        self.assertTrue(any("primary defect repeated in related_defects" in e
                            for e in pc.semantic_errors(bad)))

    def test_negative_known_diff_without_defect_or_residual(self):
        bad = copy.deepcopy(self.catalog)
        entry = next(e for e in bad["checkpoints"] if e["expected"]["status"] == "known_diff")
        del entry["expected"]["defect_id"]
        self.assertTrue(self.errors(bad))
        bad = copy.deepcopy(self.catalog)
        entry = next(e for e in bad["checkpoints"] if e["expected"]["status"] == "known_diff")
        entry["expected"]["residual_pixels"] = entry["expected"]["exact_rgba_pixels"] + 1
        self.assertTrue(any("exceeds" in e for e in self.errors(bad)))
        entry["expected"]["residual_pixels"] = 1
        entry["expected"]["bbox"] = [0, 0, 320, 10]
        self.assertTrue(any("outside the native frame" in e for e in self.errors(bad)))

    def test_negative_unsynchronizable_needs_reason_and_status_enum(self):
        bad = copy.deepcopy(self.catalog)
        bad["checkpoints"][0]["expected"] = {"status": "unsynchronizable"}
        self.assertTrue(self.errors(bad))
        bad["checkpoints"][0]["expected"] = {"status": "skipped"}
        self.assertTrue(self.errors(bad))

    def test_negative_modes_must_include_fidelity(self):
        bad = copy.deepcopy(self.catalog)
        bad["checkpoints"][0]["modes"] = ["E", "M"]
        self.assertTrue(self.errors(bad))
        bad["checkpoints"][0]["modes"] = ["F", "X"]
        self.assertTrue(self.errors(bad))

    def test_negative_reach_and_seed_provenance(self):
        bad = copy.deepcopy(self.catalog)
        bad["checkpoints"][0]["reach"]["native"] = "nowhere"
        self.assertTrue(any("unknown native reach" in e for e in self.errors(bad)))
        bad = copy.deepcopy(self.catalog)
        bad["checkpoints"][0]["reach"]["native"] = "port-installed-scripted-entry"
        self.assertTrue(any("not a native recipe" in e for e in self.errors(bad)))
        bad = copy.deepcopy(self.catalog)
        recipe = next(r for r in bad["reach"].values() if r["kind"] == "seed_state")
        del recipe["provenance"]
        self.assertTrue(self.errors(bad))
        bad = copy.deepcopy(self.catalog)
        recipe = next(r for r in bad["reach"].values() if r["kind"] == "seed_state")
        recipe["label"] = "verified"
        self.assertTrue(self.errors(bad))
        bad = copy.deepcopy(self.catalog)
        recipe = next(r for r in bad["reach"].values() if r["kind"] == "input_script")
        recipe["script"] = "config/port/input-scripts/missing_route.json"
        self.assertTrue(any("input script missing" in e for e in self.errors(bad)))

    def test_negative_timing_coverage(self):
        bad = copy.deepcopy(self.catalog)
        timing = [e for e in bad["checkpoints"] if e["kind"] == "frame_timing"]
        bad["checkpoints"].remove(timing[5])
        self.assertTrue(any("do not cover" in e for e in self.errors(bad)))
        bad = copy.deepcopy(self.catalog)
        timing = [e for e in bad["checkpoints"] if e["kind"] == "frame_timing"]
        timing[3]["native_predicate"]["substep"] += 1
        self.assertTrue(any("native predicate" in e for e in self.errors(bad)))

    def test_negative_embedded_pixels(self):
        bad = copy.deepcopy(self.catalog)
        bad["checkpoints"][0]["english_risks"].append("data:image/png;base64,AAAA")
        self.assertTrue(any("embeds" in e for e in self.errors(bad)))

    def test_cli_exit_codes(self):
        import tempfile
        from pathlib import Path
        with tempfile.TemporaryDirectory() as tmp:
            good = Path(tmp) / "good.json"
            good.write_text(json.dumps(self.catalog))
            bad = copy.deepcopy(self.catalog)
            bad["checkpoints"].append(copy.deepcopy(bad["checkpoints"][0]))
            broken = Path(tmp) / "bad.json"
            broken.write_text(json.dumps(bad))
            import contextlib, io
            with contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(pc.main([str(good)]), 0)
                self.assertEqual(pc.main([str(broken)]), 1)


if __name__ == "__main__":
    unittest.main()
