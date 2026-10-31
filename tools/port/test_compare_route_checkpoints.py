"""T0 tests of tools/port/compare_route_checkpoints.py: pairing and its negatives on synthetic runs.

Run (Docker): python3 -I -m unittest discover -s tools/port -p 'test_compare_route_checkpoints.py'
CompareTests replace the state projection (compare_newgame_golden.compare) with a stub that compares
one field; RealProjectionTests run the unpatched projection on tools/port/fixtures/route_compare_state.json
(synthetic numbers in the decoder's and the port's shapes, no game data).
"""
import copy
import json
import struct
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import compare_route_checkpoints as cmp  # noqa: E402

FIXTURE = Path(__file__).resolve().parent / "fixtures" / "route_compare_state.json"


def stub_compare(port, golden, ram, tag=""):
    rows = [{"group": "state", "field": "day_count", "port": port["rule_state"]["day_count"],
             "golden": golden["game_state"]["fields"]["day_count"]}]
    rows[0]["match"] = rows[0]["port"] == rows[0]["golden"]
    return {"rows": rows, "mismatches": [r for r in rows if not r["match"]]}


class Runs:
    def __init__(self, base: Path):
        self.port = base / "port"
        self.native = base / "native"
        (self.port / "states").mkdir(parents=True)
        (self.port / "checkpoints").mkdir()
        self.native.mkdir()
        self.port_cps, self.native_cps = [], []

    def add(self, cid, port_day=None, native_day=None, port_fc=100, native_fc=7570):
        if native_day is not None:
            (self.native / ("%s.decoded.json" % cid)).write_text(json.dumps({"game_state": {"fields": {"day_count": native_day}}}))
            (self.native / ("%s.ram.bin" % cid)).write_bytes(b"\0" * 16)
            self.native_cps.append({"id": cid, "frame_count": native_fc})
        if port_day is not None:
            (self.port / "states" / ("%s.json" % cid)).write_text(json.dumps({"state": {}, "rule_state": {"day_count": port_day}}))
            self.port_cps.append({"id": cid, "frame_count": port_fc})

    def write(self, port_unsampled=(), native_unsampled=()):
        (self.port / "route.json").write_text(json.dumps({"side": "port", "script": "s", "reason": "app_failure", "stop": "x",
                                                          "checkpoints": self.port_cps, "unsampled": list(port_unsampled)}))
        (self.native / "route.json").write_text(json.dumps({"side": "native", "script": "s", "reason": "frames", "stop": "y",
                                                            "segment_at_stop": "w003-week", "checkpoints": self.native_cps,
                                                            "unsampled": list(native_unsampled)}))


@mock.patch.object(cmp.golden, "compare", stub_compare)
class CompareTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.runs = Runs(Path(self.tmp.name))

    def tearDown(self):
        self.tmp.cleanup()

    def test_match_and_first_not_reached(self):
        self.runs.add("select-001", 0, 0, 100, 7570)
        self.runs.add("enter-30-001", None, 0)
        self.runs.write()
        report = cmp.compare_runs(self.runs.port, self.runs.native)
        self.assertEqual(report["paired"], 1)
        self.assertTrue(report["all_paired_match"])
        self.assertEqual(report["first_not_reached_by_port"], "enter-30-001")
        # One pair: the offset is reported, drift is not measurable.
        self.assertEqual(report["checkpoints"][0]["clock"]["offset"], 7470)
        self.assertIsNone(report["checkpoints"][0]["clock"]["offset_drift"])
        self.assertIsNone(report["clock"]["max_abs_drift"])

    def test_differing_field_is_reported_first(self):
        self.runs.add("select-001", 0, 0, 100, 7570)
        self.runs.add("select-002", 6, 5, 671, 8141)
        self.runs.write()
        report = cmp.compare_runs(self.runs.port, self.runs.native)
        row = report["checkpoints"][1]
        self.assertEqual(row["status"], "differs")
        self.assertEqual(row["first_difference"]["field"], "day_count")
        self.assertEqual(row["clock"]["offset_drift"], 0)
        self.assertFalse(report["all_paired_match"])
        self.assertEqual(cmp.main([str(self.runs.port), str(self.runs.native)]), 1)

    def test_clock_drift(self):
        self.runs.add("select-001", 0, 0, 100, 7570)
        self.runs.add("select-002", 5, 5, 670, 8141)
        self.runs.write()
        self.assertEqual(cmp.compare_runs(self.runs.port, self.runs.native)["checkpoints"][1]["clock"]["offset_drift"], 1)

    def test_unsampled_same_interval_matches(self):
        self.runs.add("select-001", 0, 0, 100, 7570)
        self.runs.add("select-002", 5, 5, 670, 8140)
        self.runs.write(port_unsampled=[{"frame_count": 700, "button": "CIRCLE"}],
                        native_unsampled=[{"frame_count": 8172, "button": "CIRCLE"}])
        report = cmp.compare_runs(self.runs.port, self.runs.native)
        self.assertEqual(report["unsampled"]["matched"], 1)
        self.assertEqual(report["unsampled"]["port_only_unsampled"], [])
        self.assertEqual(cmp.main([str(self.runs.port), str(self.runs.native)]), 0)

    def test_port_only_unsampled_fails(self):
        self.runs.add("select-001", 0, 0, 100, 7570)
        self.runs.add("select-002", 5, 5, 670, 8140)
        # The native press missed in the first interval; the port's in the second one: not the same miss.
        self.runs.write(port_unsampled=[{"frame_count": 700, "button": "CIRCLE"}],
                        native_unsampled=[{"frame_count": 7600, "button": "CIRCLE"}])
        report = cmp.compare_runs(self.runs.port, self.runs.native)
        self.assertEqual(report["unsampled"]["port_only_unsampled"],
                         [{"interval": "select-002", "button": "CIRCLE", "port_frame_count": 700}])
        self.assertEqual(cmp.main([str(self.runs.port), str(self.runs.native)]), 1)

    def test_unsampled_past_native_reach_is_uncompared(self):
        self.runs.add("select-001", 0, 0, 100, 7570)
        self.runs.add("select-002", 5, None, 670)
        self.runs.write(port_unsampled=[{"frame_count": 700, "button": "CIRCLE"}])
        report = cmp.compare_runs(self.runs.port, self.runs.native)
        self.assertEqual(report["unsampled"]["port_uncompared"][0]["interval"], "select-002")
        self.assertEqual(report["unsampled"]["port_only_unsampled"], [])

    def test_port_only_checkpoint_fails(self):
        self.runs.add("select-001", 0, 0)
        self.runs.add("select-009", 0, None)
        self.runs.write()
        report = cmp.compare_runs(self.runs.port, self.runs.native)
        self.assertEqual(report["port_only_checkpoints"], ["select-009"])
        self.assertEqual(cmp.main([str(self.runs.port), str(self.runs.native)]), 1)

    def test_truncated_route_json_is_unusable(self):
        self.runs.add("select-001", 0, 0)
        self.runs.write()
        text = (self.runs.native / "route.json").read_text()
        (self.runs.native / "route.json").write_text(text[: len(text) // 2])
        with self.assertRaises(cmp.InputError):
            cmp.compare_runs(self.runs.port, self.runs.native)
        self.assertEqual(cmp.main([str(self.runs.port), str(self.runs.native)]), 2)

    def test_missing_route_json_is_unusable(self):
        self.runs.add("select-001", 0, 0)
        self.runs.write()
        (self.runs.port / "route.json").unlink()
        self.assertEqual(cmp.main([str(self.runs.port), str(self.runs.native)]), 2)

    def test_listed_checkpoint_without_files_is_unusable(self):
        self.runs.add("select-001", 0, 0)
        self.runs.write()
        (self.runs.native / "select-001.ram.bin").unlink()
        with self.assertRaises(cmp.InputError):
            cmp.compare_runs(self.runs.port, self.runs.native)
        (self.runs.native / "select-001.ram.bin").write_bytes(b"\0")
        (self.runs.port / "states" / "select-001.json").unlink()
        with self.assertRaises(cmp.InputError):
            cmp.compare_runs(self.runs.port, self.runs.native)

    def test_wrong_side_refused(self):
        self.runs.add("select-001", 0, 0)
        self.runs.write()
        with self.assertRaises(cmp.InputError):
            cmp.compare_runs(self.runs.native, self.runs.port)


class RealProjectionTests(unittest.TestCase):
    """The unpatched compare_newgame_golden.compare, called as compare_runs calls it."""

    def setUp(self):
        self.fixture = json.loads(FIXTURE.read_text())
        self.ram = bytearray(0x8510)
        struct.pack_into("<I", self.ram, cmp.golden.RAND_WORD, self.fixture["rand_word"])

    def run_compare(self, port):
        return cmp.golden.compare(port, self.fixture["decoded"], bytes(self.ram), cmp.ROUTE_TAG)

    def test_state_rows_and_match(self):
        result = self.run_compare(self.fixture["port"])
        state_rows = [r for r in result["rows"] if r["group"] == "state"]
        self.assertEqual(len(state_rows), 124)  # the count route checkpoints report (16 girls x 5 + 44)
        self.assertEqual(result["mismatches"], [])
        # The route tag is a7: the icon rows are counted and carry no boot-state note.
        icon = next(r for r in state_rows if r["field"] == "icon_enabled[0..14]")
        self.assertNotIn("note", icon)
        self.assertTrue(all(r["match"] for r in result["rows"] if r["group"] == "clock" and r["field"] == "bios_rand_state"))

    def test_seeded_difference_reported_first(self):
        port = copy.deepcopy(self.fixture["port"])
        port["rule_state"]["girls"][2]["flags"] ^= 0x40
        port["rule_state"]["scene_work"] = 0
        result = self.run_compare(port)
        self.assertEqual([m["field"] for m in result["mismatches"]], ["girls[2].flags", "scene_work"])

    def test_compare_runs_uses_real_projection(self):
        with tempfile.TemporaryDirectory() as tmp:
            runs = Runs(Path(tmp))
            port = copy.deepcopy(self.fixture["port"])
            port["rule_state"]["day_count"] = 6
            (runs.port / "states" / "select-001.json").write_text(json.dumps(port))
            (runs.native / "select-001.decoded.json").write_text(json.dumps(self.fixture["decoded"]))
            (runs.native / "select-001.ram.bin").write_bytes(bytes(self.ram))
            runs.port_cps.append({"id": "select-001", "frame_count": 100})
            runs.native_cps.append({"id": "select-001", "frame_count": 7570})
            runs.write()
            row = cmp.compare_runs(runs.port, runs.native)["checkpoints"][0]
            self.assertEqual((row["state_fields"], row["state_matching"]), (124, 123))
            self.assertEqual(row["first_difference"]["field"], "day_count")


if __name__ == "__main__":
    unittest.main()
