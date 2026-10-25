"""T-29002: public synthetic tests for trace diagnostics and evidence boundaries.

Run in Docker: python3 -m unittest discover -s port/godot/tests/core -p 'test_*.py'.
"""

import copy
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[4]


def module(name):
    spec = importlib.util.spec_from_file_location(name, ROOT / "tools/port" / f"{name}.py")
    loaded = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(loaded)
    return loaded


TRACE = module("compare_trace")
CAPTURE = module("capture_route")


class TraceTests(unittest.TestCase):
    def setUp(self):
        self.rows = [{"iteration": 0, "phase": "mode", "source": "synthetic-test",
                      "state": {"parameter": 123456, "seed": 4294967295},
                      "events": [{"service": "synthetic-loader", "ready": True}]}]

    def test_identical_projection(self):
        self.assertIsNone(TRACE.compare(self.rows, copy.deepcopy(self.rows)))

    def test_first_nested_mutation_includes_source_and_phase(self):
        changed = copy.deepcopy(self.rows)
        changed[0]["state"]["parameter"] += 1
        mismatch = TRACE.compare(self.rows, changed)
        self.assertEqual(mismatch, {"record": 0, "iteration": 0, "phase": "mode",
                                   "source": "synthetic-test", "field": "$.state.parameter",
                                   "expected": 123456, "actual": 123457})

    def test_event_order_and_type_are_not_erased(self):
        changed = copy.deepcopy(self.rows)
        changed[0]["events"][0]["ready"] = 1
        self.assertEqual(TRACE.compare(self.rows, changed)["field"], "$.events[0].ready")

    def test_truncated_trace_is_a_divergence(self):
        self.assertEqual(TRACE.compare(self.rows, [])["field"], "$.trace.length")

    def test_float_and_empty_trace_cannot_claim_parity(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "trace.jsonl"
            path.write_text("")
            with self.assertRaisesRegex(ValueError, "empty traces"):
                TRACE.load_trace(path)
            self.rows[0]["state"]["parameter"] = 1.5
            path.write_text(json.dumps(self.rows[0]) + "\n")
            with self.assertRaisesRegex(ValueError, "exact integers"):
                TRACE.load_trace(path)

    def test_backwards_iteration_is_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            path = Path(folder) / "trace.jsonl"
            later = copy.deepcopy(self.rows[0])
            later["iteration"] = 1
            path.write_text(json.dumps(later) + "\n" + json.dumps(self.rows[0]) + "\n")
            with self.assertRaisesRegex(ValueError, "monotonic integer"):
                TRACE.load_trace(path)


class CaptureTests(unittest.TestCase):
    def test_evidence_rejects_public_and_escaped_destinations(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            with self.assertRaisesRegex(ValueError, "evidence output"):
                CAPTURE.evidence_directory(root, root / "port/godot/original")
            target = CAPTURE.evidence_directory(root, root / "build/emu/synthetic")
            self.assertTrue(target.is_dir())
            with self.assertRaises(FileExistsError):
                CAPTURE.evidence_directory(root, target)
            (root / "build/emu/link").symlink_to(root / "port")
            with self.assertRaisesRegex(ValueError, "evidence output"):
                CAPTURE.evidence_directory(root, root / "build/emu/link/escaped")

    def test_probe_workload_is_bounded_and_separate_from_gameplay(self):
        words, stop = CAPTURE.bios_program([0, 1, 0xFFFFFFFF, 0x80000000], 8)
        self.assertLess(len(words) * 4, 0x4000)
        self.assertEqual(stop, 0x801D0000 + (len(words) - 2) * 4)
        self.assertEqual(words[-1], 0)
        self.assertTrue(all(0 <= word <= 0xFFFFFFFF for word in words))


if __name__ == "__main__":
    unittest.main()
