"""T-39021 (T-39018 WP-04): every suite result transition on synthetic images.

No game data: images are generated here, the catalog is a minimal synthetic
copy of the committed schema shape, and pair manifests follow the
test_compare_capture_pair fixture. Run: tools/port/run_playtest_unit_tests.sh
"""
import contextlib
import copy
import hashlib
import io
import json
from pathlib import Path
import tempfile
import unittest

from PIL import Image

from tools.port import run_differential_suite as suite

W, H = 32, 24


def zero(code):
    return code << 3


def replicated(code):
    return (code << 3) | (code >> 2)


def base_image(expand=zero):
    image = Image.new("RGB", (W, H))
    for y in range(H):
        for x in range(W):
            image.putpixel((x, y), (expand((x * 3) % 32), expand(y % 32), expand((x + y) % 32)))
    return image


def with_residual(image, points, expand=zero):
    image = image.copy()
    for x, y in points:
        r, g, b = image.getpixel((x, y))
        image.putpixel((x, y), (expand(((r >> 3) + 1) % 32), g, b))
    return image


class Fixture:
    def __init__(self, root):
        self.root = root
        self.catalog = {
            "schema": "tokimemo.playtest-checkpoints.v1", "policy": {}, "reach": {},
            "defects": [{"id": "T-39030", "ticket": "T-39030", "class": "port_defect", "summary": "synthetic defect"}],
            "checkpoints": []}
        self.frames = {}

    def entry(self, key, expected, kind="frame", **extra):
        self.catalog["checkpoints"].append(dict({"key": key, "group": "synthetic", "kind": kind,
                                                 "port_observer_key": key, "expected": expected}, **extra))

    def save(self, name, image):
        path = self.root / name
        image.save(path)
        return {"path": name, "sha256": hashlib.sha256(path.read_bytes()).hexdigest()}

    def frame(self, key, native, port):
        self.frames[key] = {"native": self.save(key + "-n.png", native), "port": self.save(key + "-p.png", port)}

    def run(self, out_name="out", keys=None, **kwargs):
        captures = dict({"schema": suite.CAPTURES_SCHEMA, "frames": self.frames}, **kwargs)
        return suite.compare(self.catalog, captures, self.root, self.root / out_name, keys=keys)


def known(residual, bbox, exact=200):
    return {"status": "known_diff", "defect_id": "T-39030", "residual_domain": "rgb555",
            "encodings": {"native": "zero_extended", "port": "source_proven_mixed"},
            "exclusions": ["emulator_x0_column"], "residual_pixels": residual, "bbox": bbox,
            "exact_rgba_pixels": exact, "exact_bbox": [0, 0, W - 1, H - 1]}


class ClassifyTests(unittest.TestCase):
    def measured(self, exact, residual, bbox):
        return {"exact_rgba8": {"pixels": exact}, "rgb555_residual": {"pixels": residual, "bbox": bbox}}

    def test_known_diff_transitions(self):
        pin = known(3, [2, 2, 10, 10])
        cases = [((0, 0, None, "equal"), "promote"),
                 ((0, 0, None, "different"), "known_diff_clean_except_exclusions"),
                 ((50, 3, [2, 2, 10, 10], "different"), "known_diff_stable"),
                 ((50, 2, [3, 3, 9, 9], "different"), "known_diff_improved"),
                 ((50, 0, None, "different"), "known_diff_improved"),
                 ((50, 3, [3, 3, 9, 9], "different"), "known_diff_changed"),
                 ((50, 4, [2, 2, 10, 10], "different"), "known_diff_regressed"),
                 ((50, 1, [0, 0, 0, 0], "different"), "known_diff_bbox_moved")]
        for (exact, residual, bbox, status), result in cases:
            with self.subTest(result=result, bbox=bbox, status=status):
                self.assertEqual(suite.known_diff_result(pin, self.measured(exact, residual, bbox), status), result)

    def test_bbox_moving_past_each_pinned_edge(self):
        pin = known(3, [2, 2, 10, 10])
        for edge, bbox in (("left", [1, 2, 10, 10]), ("top", [2, 1, 10, 10]),
                           ("right", [2, 2, 11, 10]), ("bottom", [2, 2, 10, 11])):
            with self.subTest(edge=edge):
                self.assertEqual(suite.known_diff_result(pin, self.measured(50, 3, bbox), "different"),
                                 "known_diff_bbox_moved")
                self.assertEqual(suite.known_diff_result(pin, self.measured(50, 2, bbox), "different"),
                                 "known_diff_bbox_moved")
        self.assertFalse(suite.within([2, 2, 10, 11], [2, 2, 10, 10]))
        self.assertTrue(suite.within([2, 2, 10, 10], [2, 2, 10, 10]))


class SuiteTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.fx = Fixture(self.root)

    def results(self, report):
        return {row["key"]: row["result"] for row in report["results"]}

    def test_every_frame_result_transition(self):
        native = base_image()
        fx = self.fx
        fx.entry("exact-ok", {"status": "exact"})
        fx.frame("exact-ok", native, native)
        fx.entry("exact-bad", {"status": "exact"})
        fx.frame("exact-bad", native, with_residual(native, [(5, 5)]))
        fx.entry("exact-colour", {"status": "exact"})  # colour expansion alone fails the exact gate
        fx.frame("exact-colour", native, base_image(replicated))
        port = with_residual(base_image(replicated), [(4, 4), (8, 6)], replicated)
        stable_pin = known(2, [4, 4, 8, 6])
        for key, pin in (("kd-stable", stable_pin), ("kd-improved", known(3, [2, 2, 9, 9])),
                         ("kd-regressed", known(1, [4, 4, 8, 6])), ("kd-moved", known(5, [5, 4, 8, 6]))):
            fx.entry(key, pin)
            fx.frame(key, native, port)
        fx.entry("kd-promote", stable_pin)
        fx.frame("kd-promote", native, native)
        fx.entry("kd-changed", known(2, [3, 3, 9, 9]))
        fx.frame("kd-changed", native, with_residual(native, [(4, 4), (5, 4)]))
        fx.entry("kd-edge-only", stable_pin)  # only the excluded x=0 column differs: never a promotion
        edge_only = native.copy()
        for y in range(H):
            edge_only.putpixel((0, y), (0, 0, 0))
        fx.frame("kd-edge-only", native, edge_only)
        fx.entry("kd-rejected", stable_pin)
        bad = native.copy()
        bad.putpixel((3, 3), (13, 0, 0))
        fx.frame("kd-rejected", native, bad)
        fx.entry("kd-excluded-edge", stable_pin)  # an x=0 change is counted under the exclusion, not the residual
        fx.frame("kd-excluded-edge", native, with_residual(port, [(0, 7)], replicated))
        fx.entry("unsync", {"status": "unsynchronizable", "reason": "synthetic: no single-vsync window exists"})
        fx.frame("unsync", native, with_residual(native, [(1, 1)]))
        fx.entry("absent", {"status": "exact"})
        fx.entry("state", {"status": "exact"}, kind="state")
        report = fx.run()
        self.assertEqual(self.results(report), {
            "exact-ok": "exact_pass", "exact-bad": "exact_fail", "exact-colour": "exact_fail",
            "kd-stable": "known_diff_stable", "kd-improved": "known_diff_improved",
            "kd-regressed": "known_diff_regressed", "kd-moved": "known_diff_bbox_moved",
            "kd-promote": "promote", "kd-changed": "known_diff_changed",
            "kd-edge-only": "known_diff_clean_except_exclusions", "kd-rejected": "known_diff_rejected", "kd-excluded-edge": "known_diff_stable",
            "unsync": "unsynchronizable_listed", "absent": "missing_capture", "state": "not_implemented"})
        self.assertEqual(report["status"], "failed")
        self.assertEqual(report["promote"], ["kd-promote"])
        self.assertEqual(report["repin_advised"], ["kd-improved", "kd-edge-only"])
        self.assertIn("kd-changed", report["failed"])
        self.assertNotIn("kd-edge-only", report["failed"])
        rows = {row["key"]: row for row in report["results"]}
        edge = rows["kd-edge-only"]
        self.assertEqual(edge["exact_rgba"]["status"], "different")
        self.assertEqual(edge["rgb555"]["exact_rgba_pixels"], 0)
        self.assertGreater(edge["rgb555"]["domain_exclusions"][0]["exact_changed"], 0)
        self.assertIn("not a promotion", edge["note"])
        self.assertEqual(rows["kd-excluded-edge"]["rgb555"]["domain_exclusions"][0]["rgb555_changed"], 1)
        self.assertIn("never compared", rows["unsync"]["note"])
        self.assertNotIn("rgb555", rows["unsync"])
        self.assertTrue((self.root / "out" / "suite-report.json").is_file())
        self.assertTrue((self.root / "out" / "rgb555-kd-stable.json").is_file())

    def test_passing_subset_and_selection(self):
        native = base_image()
        self.fx.entry("a", {"status": "exact"})
        self.fx.frame("a", native, native)
        self.fx.entry("b", {"status": "exact"})
        report = self.fx.run(keys={"a"})
        self.assertEqual(report["status"], "passed")
        self.assertEqual(report["counts"], {"exact_pass": 1})
        with self.assertRaises(suite.Invalid):
            self.fx.run(out_name="out2", keys={"zzz"})

    def test_exact_with_pair_manifest_is_blocked_by_the_gate(self):
        native = Image.new("RGBA", (320, 240), (16, 32, 48, 255))
        self.fx.entry("gated", {"status": "exact"})
        self.fx.frame("gated", native, native)
        pins = self.fx.frames["gated"]
        record = dict(frame=3, cycles=1, pc=1, mode=21, phase="vsync", state_load_phase="restored", path="gated-n.raw",
                      width=320, height=240, image_status="ok", png="gated-n.png", png_sha256=pins["native"]["sha256"],
                      dumps=[dict(addr=0x800E7388, length=1, status="ok", data="15")])
        (self.root / "native.json").write_text(json.dumps({"shots": [record]}))
        (self.root / "port.json").write_text(json.dumps({"mode": 21, "frame": 9, "sha256": pins["port"]["sha256"]}))
        pin = lambda name: {"path": name, "sha256": hashlib.sha256((self.root / name).read_bytes()).hexdigest()}
        pair = {"schema": "tokimemo.capture-pair.v1", "scope": "synthetic",
                "native": {"record": pin("native.json"), "selector": ["shots", 0], "image": pins["native"]},
                "port": {"record": pin("port.json"), "selector": [], "image": pins["port"], "image_sha_path": ["sha256"],
                         "clock_paths": {"frame": ["frame"]}},
                "native_map": {"schema": "tokimemo.native-memory-map.v1",
                               "fields": [{"name": "mode", "address": "0x800E7388", "type": "u8", "length": 1}]},
                "mappings": [{"name": "mode", "native_field": "mode", "port_path": ["mode"], "meaning": "mode byte", "compatibility": "equivalent"}],
                "checkpoint": {"name": "synthetic", "expected": {"mode": 21}},
                "synchronization": {"status": "unverified", "basis": "synthetic"}}
        (self.root / "pair.json").write_text(json.dumps(pair))
        self.fx.frames["gated"]["pair_manifest"] = pin("pair.json")
        rows = {r["key"]: r for r in self.fx.run()["results"]}
        self.assertEqual(rows["gated"]["result"], "exact_blocked")
        self.assertIn("checkpoint synchronization remains unverified", rows["gated"]["capture_pair_gate"]["blockers"])

    def test_frame_timing_results(self):
        fixture = json.loads((suite.ROOT / "port/godot/tests/fixtures/opening_frame_timing.json").read_text())
        rows = fixture["pads"]["2"]["substeps"]
        frame = 100
        transitions = {}
        for key, count in rows:
            transitions[key] = frame
            frame += count
        transitions["12/0/0"] = frame
        transitions[rows[1][0]] += 1  # substep 0 one longer, substep 1 one shorter
        (self.root / "transitions.json").write_text(json.dumps({"transitions": transitions}))
        pin = {"path": "transitions.json", "sha256": hashlib.sha256((self.root / "transitions.json").read_bytes()).hexdigest()}
        for key, _ in rows[:3]:
            self.fx.entry("t-" + key.replace("/", "-"), {"status": "exact"}, kind="frame_timing",
                          timing_reference={"substep": key})
        report = self.fx.run(timing={"pad": "2", "port_transitions": pin})
        got = [row["result"] for row in report["results"]]
        self.assertEqual(got, ["exact_fail", "exact_fail", "exact_pass"])
        self.assertEqual(report["results"][0]["port_iterations"], rows[0][1] + 1)

    def test_changed_capture_and_bad_input_are_invalid(self):
        native = base_image()
        self.fx.entry("a", {"status": "exact"})
        self.fx.frame("a", native, native)
        native.putpixel((1, 1), (8, 8, 8))
        native.save(self.root / "a-n.png")
        with self.assertRaises(suite.Invalid):
            self.fx.run()
        with self.assertRaises(suite.Invalid):
            suite.compare(self.fx.catalog, {"schema": "other"}, self.root, self.root / "x")

    def test_output_guard(self):
        repo = Path("/repo")
        self.assertEqual(suite.guarded_output("/repo/.asset-cache/run1", repo), Path("/repo/.asset-cache/run1"))
        with self.assertRaises(suite.Invalid):
            suite.guarded_output("/repo/build/run1", repo)
        with self.assertRaises(suite.Invalid):
            suite.guarded_output("/repo/config/port", repo)
        with self.assertRaises(suite.Invalid):
            suite.guarded_output(self.root, repo)  # exists

    def test_plan_is_docker_only_and_inside_repo(self):
        catalog = json.loads((suite.ROOT / "config/port/playtest-checkpoints.json").read_text())
        entries = suite.select(catalog, groups=["rei-stage8-10"])
        root = suite.ROOT
        steps = suite.plan(entries, root / ".asset-cache/diffsuite-x", root / ".asset-cache/s.sstate",
                           root / ".asset-cache/bundle", 2600, [323, 335])
        names = [s["name"] for s in steps["steps"]]
        self.assertEqual(names, ["native", "port", "pairs"])
        native = steps["steps"][0]["argv"]
        self.assertEqual(native[:2], ["tools/emu/run.sh", "boot"])
        self.assertIn("800B5BC8:12", native)
        self.assertEqual(steps["steps"][2]["argv"][:3], ["docker", "run", "--rm"])
        with self.assertRaises(suite.Invalid):
            suite.plan(entries, Path("/elsewhere/out"), root / "s", root / "b", 10, [])
        with self.assertRaises(suite.Invalid):
            suite.plan(suite.select(catalog, groups=["opening-frame-timing"]), root / ".asset-cache/y", root / "s", root / "b", 10, [])

    def test_captures_from_pairs_maps_observer_keys(self):
        catalog = copy.deepcopy(self.fx.catalog)
        catalog["checkpoints"].append({"key": "rei-stage8-1", "kind": "frame", "port_observer_key": "stage8-1",
                                       "group": "g", "expected": {"status": "exact"}})
        (self.root / "pairs").mkdir()
        base_image().save(self.root / "n.png")
        digest = hashlib.sha256((self.root / "n.png").read_bytes()).hexdigest()
        pair = {"native": {"image": {"path": "../n.png", "sha256": digest}}, "port": {"image": {"path": "../n.png", "sha256": digest}}}
        (self.root / "pairs" / "pair-stage8-1.json").write_text(json.dumps(pair))
        (self.root / "pairs" / "pair-stage8-1-report.json").write_text("{}")
        frames = suite.captures_from_pairs(self.root / "pairs", self.root / "captures.json", catalog)
        self.assertEqual(list(frames), ["rei-stage8-1"])
        (self.root / "pairs" / "pair-stage99-1.json").write_text(json.dumps(pair))
        with self.assertRaises(suite.Invalid):
            suite.captures_from_pairs(self.root / "pairs", self.root / "captures2.json", catalog)

    def test_cli_exit_codes(self):
        with contextlib.redirect_stderr(io.StringIO()):
            self.assertEqual(suite.main(["compare", "--captures", str(self.root / "none.json"),
                                         "--out", str(self.root / "o")]), 2)


if __name__ == "__main__":
    unittest.main()
