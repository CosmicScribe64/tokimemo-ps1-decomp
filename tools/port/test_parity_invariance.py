"""T-39230: the modern-vs-parity invariance comparator on synthetic run directories.

Run: tools/port/run_playtest_unit_tests.sh tools.port.test_parity_invariance
"""
import contextlib
import copy
import io
import json
from pathlib import Path
import tempfile
import unittest

from tools.port import parity_invariance as inv
from tools.port import parity_gate

PIN = parity_gate.load_pin()

EXCLUDED = [{"id": "accessibility.text_speed", "class": "game_affecting", "invariant": "holds the presentation"},
            {"id": "feature.quick_save", "class": "game_affecting", "invariant": "writes the quick slot"},
            {"id": "profile.mode", "class": "presentation_only", "invariant": "the switch itself"}]


def frames(count=6, image_change=None, state_change=None):
    result = []
    for i in range(count):
        record = {"schema": inv.FRAME_SCHEMA, "frame": 100 + i, "phase": "entry" if i < 3 else "opening",
                  "state": "s%02d" % i, "image": "i%02d" % i}
        if image_change == i:
            record["image"] = "changed"
        if state_change == i:
            record["state"] = "changed"
        result.append(record)
    return result


def write_run(root, name, profile, mode, records, changes=None, script="a" * 64, registry="b" * 64, invariance=None):
    directory = root / name
    directory.mkdir()
    (directory / "parity-mode.json").write_text(json.dumps({
        "schema": "tokimemo.parity-mode-identity.v1", "play_mode": mode, "violations": [], "gate": "passed",
        "invariance_run": mode == "modern" if invariance is None else invariance,
        "registry_sha256": PIN["registry_sha256"], "locked_values_sha256": PIN["locked_values_sha256"]}))
    (directory / "invariance-profile.json").write_text(json.dumps({
        "schema": inv.PROFILE_SCHEMA, "profile": profile, "changes": changes if changes is not None else ({} if profile == "parity" else {"profile.mode": "modern", "display.filter": "linear"}),
        "excluded": EXCLUDED, "registry_sha256": registry, "input_script_sha256": script}))
    (directory / "invariance-frames.jsonl").write_text("".join(json.dumps(r) + "\n" for r in records))
    return directory


class InvarianceTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.parity = write_run(self.root, "parity", "parity", "parity", frames())

    def tearDown(self):
        self.tmp.cleanup()

    def test_equal_runs_are_invariant(self):
        modern = write_run(self.root, "modern", "modern", "modern", frames())
        state_only = write_run(self.root, "state", "modern_state_only", "modern", frames(image_change=4))
        report = inv.compare(self.parity, {"modern": modern, "modern_state_only": state_only})
        self.assertEqual(report["status"], "invariant")
        self.assertEqual(report["runs"]["modern_state_only"]["compared"], ["frame", "phase", "state"])
        self.assertIn("accessibility.text_speed", [row["id"] for row in report["excluded"]])

    def test_divergences_name_the_frame_and_field(self):
        cases = {"image": frames(image_change=2), "state": frames(state_change=5), "length": frames(4)}
        for field, records in cases.items():
            with self.subTest(field):
                modern = write_run(self.root, "m-" + field, "modern", "modern", records)
                report = inv.compare(self.parity, {"modern": modern})
                self.assertEqual(report["status"], "diverged")
                self.assertEqual(report["runs"]["modern"]["first_divergence"]["field"], field)
        state_only = write_run(self.root, "s-state", "modern_state_only", "modern", frames(state_change=1))
        self.assertEqual(inv.compare(self.parity, {"modern_state_only": state_only})["runs"]["modern_state_only"]["first_divergence"]["field"], "state")
        # Per-part hashes name the component that differs.
        with_parts = frames()
        other_parts = frames(state_change=0)
        with_parts[0]["parts"] = {"rng": "1", "state": "2"}
        other_parts[0]["parts"] = {"rng": "9", "state": "2"}
        base = write_run(self.root, "p-parts", "parity", "parity", with_parts)
        modern = write_run(self.root, "m-parts", "modern", "modern", other_parts)
        divergence = inv.compare(base, {"modern": modern})["runs"]["modern"]["first_divergence"]
        self.assertEqual(divergence["parts"], ["rng"])
        drawn = write_run(self.root, "m-images", "modern", "modern", frames(image_change=3))
        self.assertEqual(inv.compare(self.parity, {"modern": drawn})["runs"]["modern"]["image_mismatches"], [3])
        renumbered = copy.deepcopy(frames())
        renumbered[0]["frame"] = 99
        modern = write_run(self.root, "m-frame", "modern", "modern", renumbered)
        self.assertEqual(inv.compare(self.parity, {"modern": modern})["runs"]["modern"]["first_divergence"]["field"], "frame")

    def test_parity_control_run(self):
        modern = write_run(self.root, "modern", "modern", "modern", frames())
        same = write_run(self.root, "repeat", "parity", "parity", frames())
        report = inv.compare(self.parity, {"modern": modern}, same)
        self.assertEqual((report["status"], report["control"]["status"]), ("invariant", "reproducible"))
        # A parity run that differs from itself makes every modern verdict inconclusive.
        drift = write_run(self.root, "repeat-drift", "parity", "parity", frames(state_change=0))
        report = inv.compare(self.parity, {"modern": modern}, drift)
        self.assertEqual(report["status"], "baseline_nondeterministic")
        self.assertEqual(report["control"]["first_divergence"]["field"], "state")
        with self.assertRaises(inv.Invalid):
            inv.compare(self.parity, {"modern": modern}, write_run(self.root, "repeat-m", "parity", "modern", frames()))
        with self.assertRaises(inv.Invalid):
            inv.compare(self.parity, {"modern": modern}, write_run(self.root, "repeat-s", "parity", "parity", frames(), script="e" * 64))

    def test_invalid_inputs(self):
        good = frames()
        cases = {
            "modern run not marked invariance": lambda: write_run(self.root, "a", "modern", "modern", good, invariance=False),
            "modern profile run in parity": lambda: write_run(self.root, "b", "modern", "parity", good),
            "other input script": lambda: write_run(self.root, "c", "modern", "modern", good, script="c" * 64),
            "other registry": lambda: write_run(self.root, "d", "modern", "modern", good, registry="d" * 64),
            "excluded entry enabled": lambda: write_run(self.root, "e", "modern", "modern", good,
                                                        changes={"profile.mode": "modern", "accessibility.text_speed": "fast"}),
            "profile name mismatch": lambda: write_run(self.root, "f", "modern_state_only", "modern", good),
            "no frames": lambda: write_run(self.root, "g", "modern", "modern", []),
        }
        for name, make in cases.items():
            with self.subTest(name), self.assertRaises(inv.Invalid):
                inv.compare(self.parity, {"modern": make()})
        bad_parity = write_run(self.root, "p2", "parity", "modern", good)
        with self.assertRaises(inv.Invalid):
            inv.compare(bad_parity, {"modern": write_run(self.root, "h", "modern", "modern", good)})
        with self.assertRaises(inv.Invalid):
            inv.compare(self.parity, {})
        (self.root / "i").mkdir()
        with self.assertRaises(inv.Invalid):
            inv.compare(self.parity, {"modern": self.root / "i"})

    def test_cli_exit_codes(self):
        modern = write_run(self.root, "modern", "modern", "modern", frames())
        drift = write_run(self.root, "drift", "modern", "modern", frames(image_change=0))
        with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
            self.assertEqual(inv.main(["--parity", str(self.parity), "--modern", str(modern), "--out", str(self.root / "r1.json")]), 0)
            self.assertEqual(inv.main(["--parity", str(self.parity), "--modern", str(drift), "--out", str(self.root / "r2.json")]), 1)
            self.assertEqual(inv.main(["--parity", str(self.parity), "--modern", str(modern), "--out", str(self.root / "r1.json")]), 2)
        self.assertEqual(json.loads((self.root / "r2.json").read_text())["status"], "diverged")


if __name__ == "__main__":
    unittest.main()
