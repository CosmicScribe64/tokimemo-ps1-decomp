"""Synthetic unit tests for differential evidence; no native trace is authentic."""
import json
from contextlib import redirect_stdout, redirect_stderr
from io import StringIO
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import differential


class DifferentialTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.evidence = self.root / "evidence"
        self.evidence.mkdir()
        self.port = self.root / "port"
        self.port.mkdir()
        (self.port / "runtime.gd").write_text("# test runtime\n")
        self.original = self.root / "SLPM.BIN"
        self.original.write_bytes(b"synthetic original executable")
        self.initial = self.evidence / "initial.json"
        self.initial.write_text('{"seed":1}\n')
        self.inputs = self.evidence / "inputs.jsonl"
        self.inputs.write_text('{"frame":0,"buttons":[]}\n')
        self.projection = self.evidence / "projection.json"
        self.projection.write_text(json.dumps({
            "schema": "tokimemo.trace-projection.v1",
            "required_state_keys": ["projected_state", "input", "rng", "services"],
            "required_projected_state_fields": ["mode", "day"],
        }, sort_keys=True) + "\n")
        self.projection_sha = differential.digest_file(self.projection)["sha256"]
        self.native_script = self.evidence / "native_capture.py"
        self.native_script.write_text("# synthetic native producer\n")
        self.port_script = self.evidence / "port_capture.py"
        self.port_script.write_text("# synthetic actual app producer\n")
        self.native_metadata = self.evidence / "native_run.json"
        self.port_metadata = self.evidence / "port_run.json"
        common_pins = {
            "schema": "tokimemo.differential-run.v1",
            "projection_schema_sha256": self.projection_sha,
            "initial_state_sha256": differential.digest_file(self.initial)["sha256"],
            "input_log_sha256": differential.digest_file(self.inputs)["sha256"],
        }
        self.native_metadata.write_text(json.dumps({
            **common_pins, "role": "native_emulator", "producer_id": "synthetic-native-test",
            "producer_script_sha256": differential.digest_file(self.native_script)["sha256"],
            "original_executable_sha256": differential.digest_file(self.original)["sha256"],
        }, sort_keys=True) + "\n")
        self.port_metadata.write_text(json.dumps({
            **common_pins, "role": "actual_app", "producer_id": "synthetic-port-test",
            "producer_script_sha256": differential.digest_file(self.port_script)["sha256"],
            "port_tree_sha256": differential.tree_identity(self.port),
        }, sort_keys=True) + "\n")
        self.native_trace = self.evidence / "native.jsonl"
        self.port_trace = self.evidence / "port.jsonl"
        self.rows = [self.row(0, 1), self.row(1, 2)]
        self.write_rows(self.native_trace, self.rows)
        self.write_rows(self.port_trace, self.rows)

    def tearDown(self):
        self.temp.cleanup()

    @staticmethod
    def row(iteration, rng):
        return {
            "iteration": iteration, "phase": "school_day", "source": "actual_loop",
            "state": {
                "projected_state": {"mode": 1, "day": iteration},
                "input": {"held": 0, "pressed": 0, "released": 0},
                "rng": {"state": rng, "draw_count": iteration}, "services": [],
            },
        }

    @staticmethod
    def write_rows(path, rows):
        path.write_text("".join(json.dumps(row, sort_keys=True) + "\n" for row in rows))

    def run_compare(self):
        return differential.compare_runs(
            evidence_root=self.evidence, native_trace=self.native_trace, port_trace=self.port_trace,
            projection_schema=self.projection, expected_projection_sha256=self.projection_sha,
            original_executable=self.original, port_root=self.port, initial_state=self.initial,
            input_log=self.inputs, native_metadata=self.native_metadata, port_metadata=self.port_metadata,
            native_producer_id="synthetic-native-test",
            native_producer_script=self.native_script, port_producer_id="synthetic-port-test",
            port_producer_script=self.port_script,
        )

    def test_equal_ordered_traces_emit_pinned_pass_report(self):
        report = self.run_compare()
        self.assertTrue(report["passed"])
        self.assertEqual(report["records"], 2)
        self.assertIsNone(report["first_divergence"])
        self.assertEqual(report["native_trace"]["origin"], "native_emulator")
        self.assertEqual(report["port_trace"]["origin"], "actual_app")
        differential.verify_report(report, self.evidence)

    def test_state_rng_order_and_length_divergences_fail_with_location(self):
        event_reordered = [self.row(0, 1), self.row(1, 2)]
        event_reordered[0]["state"]["services"] = [{"service": "save"}, {"service": "audio"}]
        variants = [
            ([self.row(0, 1), self.row(1, 9)], 1),
            (event_reordered, 0),
            ([self.row(0, 1)], 1),
        ]
        for rows, record_index in variants:
            with self.subTest(rows=rows):
                self.write_rows(self.port_trace, rows)
                report = self.run_compare()
                self.assertFalse(report["passed"])
                self.assertIsNotNone(report["first_divergence"])
                self.assertEqual(report["first_divergence"]["record"], record_index)

    def test_missing_empty_truncated_duplicate_float_and_nan_are_rejected(self):
        cases = ["", '{"iteration":0',
                 '{"iteration":0,"iteration":0,"phase":"p","source":"s","state":{}}\n',
                 json.dumps(self.row(0, 1)).replace('"day": 0', '"day": 0.5') + "\n",
                 json.dumps(self.row(0, 1)).replace('"day": 0', '"day": NaN') + "\n"]
        for value in cases:
            with self.subTest(value=value):
                self.port_trace.write_text(value)
                with self.assertRaises((ValueError, json.JSONDecodeError)):
                    self.run_compare()

    def test_missing_required_projection_or_float_is_rejected(self):
        row = self.row(0, 1)
        del row["state"]["rng"]
        self.write_rows(self.port_trace, [row])
        with self.assertRaisesRegex(ValueError, "missing projected fields"):
            self.run_compare()
        self.write_rows(self.port_trace, self.rows)
        self.projection.write_text('{"schema":"tokimemo.trace-projection.v1","required_state_keys":["projected_state","input","rng","services"],"required_projected_state_fields":["mode","day"],"tolerance":0.01}\n')
        with self.assertRaisesRegex(ValueError, "projection schema pin"):
            self.run_compare()

    def test_input_bitmasks_must_be_uint32(self):
        for value in (-1, 0x1_0000_0000, True, [1]):
            row = self.row(0, 1)
            row["state"]["input"]["held"] = value
            self.write_rows(self.port_trace, [row])
            with self.subTest(value=value), self.assertRaisesRegex(ValueError, "uint32 integers"):
                self.run_compare()

    def test_same_file_same_producer_and_different_initial_input_are_rejected(self):
        with self.assertRaisesRegex(ValueError, "distinct files"):
            differential.compare_runs(
                evidence_root=self.evidence, native_trace=self.native_trace, port_trace=self.native_trace,
                projection_schema=self.projection, expected_projection_sha256=self.projection_sha,
                original_executable=self.original, port_root=self.port, initial_state=self.initial,
                input_log=self.inputs, native_metadata=self.native_metadata, port_metadata=self.port_metadata,
                native_producer_id="x", native_producer_script=self.native_script,
                port_producer_id="y", port_producer_script=self.port_script)
        with self.assertRaisesRegex(ValueError, "distinct nonempty IDs"):
            differential.compare_runs(
                evidence_root=self.evidence, native_trace=self.native_trace, port_trace=self.port_trace,
                projection_schema=self.projection, expected_projection_sha256=self.projection_sha,
                original_executable=self.original, port_root=self.port, initial_state=self.initial,
                input_log=self.inputs, native_metadata=self.native_metadata, port_metadata=self.port_metadata,
                native_producer_id="same", native_producer_script=self.native_script,
                port_producer_id="same", port_producer_script=self.port_script)

    def test_run_metadata_must_bind_same_inputs_and_actual_origins(self):
        metadata = json.loads(self.port_metadata.read_text())
        metadata["input_log_sha256"] = "0" * 64
        self.port_metadata.write_text(json.dumps(metadata, sort_keys=True) + "\n")
        with self.assertRaisesRegex(ValueError, "do not share projection/initial/input"):
            self.run_compare()

    def test_forged_pass_report_on_mismatching_traces_is_rejected(self):
        changed = [self.row(0, 77), self.row(1, 2)]
        self.write_rows(self.port_trace, changed)
        report = self.run_compare()
        self.assertFalse(report["passed"])
        report["passed"] = True
        report["first_divergence"] = None
        with self.assertRaisesRegex(ValueError, "does not match trace comparison"):
            differential.verify_report(report, self.evidence)

    def test_forged_equal_report_with_empty_projection_is_rejected_after_rehash(self):
        report = self.run_compare()
        empty = self.row(0, 1)
        empty["state"]["projected_state"] = {}
        self.write_rows(self.native_trace, [empty])
        self.write_rows(self.port_trace, [empty])
        for name, path in (("native_trace", self.native_trace), ("port_trace", self.port_trace)):
            identity = differential.digest_file(path)
            report[name].update(identity)
            report[name]["path"] = path.name
        report["records"] = 1
        report["passed"] = True
        report["first_divergence"] = None
        with self.assertRaisesRegex(ValueError, "projected_state must be a nonempty"):
            differential.verify_report(report, self.evidence)

    def test_metadata_pins_and_evidence_paths_are_bound(self):
        report = self.run_compare()
        self.initial.write_text('{"seed":2}\n')
        with self.assertRaisesRegex(ValueError, "initial_state identity mismatch"):
            differential.verify_report(report, self.evidence)

    def test_result_output_is_exclusive_and_cannot_overwrite_input(self):
        original_bytes = self.original.read_bytes()
        with self.assertRaisesRegex(ValueError, "new file"):
            differential.write_new_result(self.original, {"passed": True})
        self.assertEqual(self.original.read_bytes(), original_bytes)
        result = self.evidence / "result.json"
        differential.write_new_result(result, {"passed": True})
        with self.assertRaisesRegex(ValueError, "new file"):
            differential.write_new_result(result, {"passed": False})

    def test_cli_writes_strict_pass_and_nonzero_divergence_reports(self):
        def invoke(output):
            argv = [
                "--evidence-root", str(self.evidence), "--native-trace", str(self.native_trace),
                "--port-trace", str(self.port_trace), "--projection-schema", str(self.projection),
                "--expected-projection-sha256", self.projection_sha,
                "--original-executable", str(self.original), "--port-root", str(self.port),
                "--initial-state", str(self.initial), "--input-log", str(self.inputs),
                "--native-metadata", str(self.native_metadata), "--port-metadata", str(self.port_metadata),
                "--native-producer-id", "synthetic-native-test", "--native-producer-script", str(self.native_script),
                "--port-producer-id", "synthetic-port-test", "--port-producer-script", str(self.port_script),
                "--output", str(output),
            ]
            with redirect_stdout(StringIO()), redirect_stderr(StringIO()):
                return differential.main(argv)
        passed_path = self.evidence / "pass-result.json"
        self.assertEqual(invoke(passed_path), 0)
        passed = json.loads(passed_path.read_text())
        self.assertEqual(passed["schema"], "tokimemo.differential-result.v1")
        self.assertTrue(passed["passed"])
        self.write_rows(self.port_trace, [self.row(0, 999)])
        failed_path = self.evidence / "failed-result.json"
        self.assertEqual(invoke(failed_path), 1)
        failed = json.loads(failed_path.read_text())
        self.assertFalse(failed["passed"])
        self.assertEqual(failed["first_divergence"]["record"], 0)

    def test_tooling_copy_refuses_symlink_or_changed_existing_file(self):
        target = self.evidence / "linked.py"
        target.symlink_to(self.original)
        with self.assertRaisesRegex(ValueError, "symbolic-link"):
            differential.write_artifact_once(target, b"safe")
        other = self.evidence / "different.py"
        other.write_bytes(b"changed")
        with self.assertRaisesRegex(ValueError, "differs"):
            differential.write_artifact_once(other, b"expected")

    def test_symlink_tooling_directory_and_output_preflight_preserve_original(self):
        saved = self.original.read_bytes()
        destination = self.root / "tooling-target"
        destination.mkdir()
        (self.evidence / "tooling").symlink_to(destination, target_is_directory=True)
        with self.assertRaisesRegex(ValueError, "tooling evidence directory is symbolic"):
            self.run_compare()
        with self.assertRaisesRegex(ValueError, "new file"):
            differential.preflight_new_output(self.original)
        self.assertEqual(self.original.read_bytes(), saved)

    def test_symlink_evidence_root_and_ancestor_rejected_before_any_writes(self):
        alias = self.root / "evidence-alias"
        alias.symlink_to(self.evidence, target_is_directory=True)
        before = sorted(path.name for path in self.evidence.iterdir())
        with self.assertRaisesRegex(ValueError, "symbolic-link path component"):
            differential.compare_runs(
                evidence_root=alias, native_trace=self.native_trace, port_trace=self.port_trace,
                projection_schema=self.projection, expected_projection_sha256=self.projection_sha,
                original_executable=self.original, port_root=self.port, initial_state=self.initial,
                input_log=self.inputs, native_metadata=self.native_metadata, port_metadata=self.port_metadata,
                native_producer_id="synthetic-native-test", native_producer_script=self.native_script,
                port_producer_id="synthetic-port-test", port_producer_script=self.port_script)
        self.assertEqual(sorted(path.name for path in self.evidence.iterdir()), before)


if __name__ == "__main__":
    unittest.main()
