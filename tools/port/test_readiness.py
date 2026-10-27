"""Readiness contract regressions, including actual failed-app evidence; Docker."""
import copy
from datetime import datetime, timezone
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

from tools.port import readiness as gate


class ReadinessTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        for directory in ("runtime", "content", "evidence", "receipts", "reviews"):
            (self.root / directory).mkdir()
        (self.root / "runtime/main.gd").write_text("extends Node\n")
        (self.root / "content/pack.json").write_text('{"installed":true}')
        (self.root / "evidence/frame.png").write_bytes(b"invented-contract-fixture-not-app-evidence")
        (self.root / "evidence/observer.gd").write_text("extends SceneTree\n")
        self.now = datetime(2026, 10, 26, 22, tzinfo=timezone.utc)
        self.policy = {
            "schema": "tokimemo.readiness-policy.v1", "scope_id": "contract-test",
            "author_id": "author", "coverage_status": "audited", "max_age_seconds": 3600,
            "reviewer_ids": ["reviewer", "second-reviewer", "producer"],
            "execution": {"engine_id": "godot-4.7.2", "platform_id": "linux-arm64", "container_image_sha256": "a" * 64},
            "content_files": [{"id": "pack", "path": "pack.json", **gate.file_identity(self.root / "content/pack.json")}],
            "scenarios": [{"id": "phone", "start_kind": "captured-boundary", "checkpoints": [
                {"id": "telephone", "raw_check_ids": ["telephone"], "evidence": [{"kind": "viewport", "allowed_origins": ["actual_app"]}]}]}]}
        source_files = {"main.gd": gate.file_identity(self.root / "runtime/main.gd")["sha256"]}
        self.producer = {"schema": "tokimemo.actual-app-producer.v1", "scenario": "phone", "start_kind": "captured-boundary",
                         "harness_gameplay_writes": [], "harness_service_completion_injection": False,
                         "image_producer": "actual root viewport get_texture().get_image(); no cached presenter image or rescale",
                         "production_source_files": source_files, "runtime_tree_sha256": gate.canonical_sha(source_files),
                         "observer_sha256": gate.file_identity(self.root / "evidence/observer.gd")["sha256"]}
        self.route = {"schema": "tokimemo.actual-app-route.v1", "scenario": "phone", "start_kind": "captured-boundary",
                      "test_kind": "production_input", "passed": True, "checks": [{"id": "telephone", "outcome": "pass"}],
                      "unattempted": [], "failures": [], "not_covered": [{"id": "other-scenario", "outcome": "skip", "reason": "outside this bounded run"}]}
        self.receipt = {"schema": "tokimemo.readiness-receipt.v1", "id": "run-one", "scenario_id": "phone",
                        "start_kind": "captured-boundary", "producer_id": "producer", "status": "pass", "synthetic": False,
                        "started_at": "2026-10-26T21:30:00Z", "finished_at": "2026-10-26T21:31:00Z",
                        "runtime_tree_sha256": gate.runtime_identity(self.root / "runtime")["sha256"],
                        "content_inputs_sha256": gate.canonical_sha(self.policy["content_files"]),
                        "execution": {**self.policy["execution"], "producer_script_artifact_id": "observer"},
                        "checks": [{"id": "telephone-check", "checkpoint_id": "telephone", "status": "pass", "evidence_ids": ["frame"], "raw_output_id": "route", "raw_check_id": "telephone"}],
                        "evidence": [{"id": "frame", "checkpoint_id": "telephone", "kind": "viewport", "origin": "actual_app",
                                      "path": "frame.png", "synthetic": False, **gate.file_identity(self.root / "evidence/frame.png")}],
                        "raw_outputs": []}
        self.review = {"schema": "tokimemo.readiness-review.v1", "id": "receipt-review", "reviewer_id": "reviewer",
                       "runtime_tree_sha256": self.receipt["runtime_tree_sha256"], "decision": "accepted",
                       "semantic_authenticity_accepted": True, "unresolved_findings": []}
        self.policy_review = {"schema": "tokimemo.readiness-policy-review.v1", "id": "policy-review", "reviewer_id": "reviewer",
                              "decision": "accepted", "coverage_audit_accepted": True, "unresolved_findings": []}
        self.freeze_policy()
        self.freeze_receipt()

    def write(self, path, record):
        (self.root / path).write_text(json.dumps(record, sort_keys=True, indent=2))

    def freeze_policy(self):
        self.write("policy.json", self.policy)
        self.expected_policy = gate.file_identity(self.root / "policy.json")["sha256"]
        self.receipt["policy_sha256"] = self.expected_policy
        self.review["policy_sha256"] = self.expected_policy
        self.policy_review["policy_sha256"] = self.expected_policy
        self.write("reviews/policy.json", self.policy_review)

    def freeze_receipt(self):
        self.write("evidence/producer.data", self.producer)
        producer_sha = gate.file_identity(self.root / "evidence/producer.data")["sha256"]
        self.route["producer_sha256"] = producer_sha
        self.write("evidence/route.data", self.route)
        self.artifacts = {"schema": "tokimemo.actual-app-artifacts.v1", "start_kind": self.producer["start_kind"],
                          "test_kind": "production_input", "outcome": "pass", "runtime_exit_code": 0, "import_exit_code": 0,
                          "runtime_tree_sha256": self.producer["runtime_tree_sha256"],
                          "producer": {"path": "producer.data", "sha256": producer_sha},
                          "route": {"path": "route.data", "sha256": gate.file_identity(self.root / "evidence/route.data")["sha256"]}, "checks": []}
        self.write("evidence/artifacts.data", self.artifacts)
        self.receipt["raw_outputs"] = [
            {"id": "producer", "path": "producer.data", "record_type": "producer_json", **gate.file_identity(self.root / "evidence/producer.data")},
            {"id": "route", "path": "route.data", "record_type": "producer_json", **gate.file_identity(self.root / "evidence/route.data")},
            {"id": "artifacts", "path": "artifacts.data", "record_type": "producer_json", **gate.file_identity(self.root / "evidence/artifacts.data")},
            {"id": "observer", "path": "observer.gd", "record_type": "producer_script", **gate.file_identity(self.root / "evidence/observer.gd")}]
        self.write("receipts/run.json", self.receipt)
        self.review["receipt_sha256"] = gate.file_identity(self.root / "receipts/run.json")["sha256"]
        self.write("reviews/receipt.json", self.review)

    def run_gate(self):
        return gate.validate(self.root / "policy.json", self.expected_policy, self.root / "runtime", self.root / "content",
                             self.root / "evidence", self.root / "receipts", self.root / "reviews", now=self.now)

    def assert_blocked(self, fragment):
        result = self.run_gate()
        self.assertFalse(result["independently_reviewed"], result)
        self.assertFalse(result["full_port_complete"])
        self.assertIn(fragment, "\n".join(result["errors"]), result)
        self.assertIn("missing_scenarios", result)
        return result

    def test_bounded_reviewed_contract_never_claims_complete_port(self):
        result = self.run_gate()
        self.assertTrue(result["independently_reviewed"], result)
        self.assertEqual(result["missing_scenarios"], [])
        self.assertFalse(result["full_port_complete"])
        self.assertEqual(len(result["out_of_scope_declarations"]), 1)

    def test_two_independent_reviews_and_conflicting_review(self):
        second = {**self.review, "id": "second-review", "reviewer_id": "second-reviewer"}
        self.write("reviews/second.json", second)
        self.assertTrue(self.run_gate()["independently_reviewed"])
        second["decision"] = "rejected"
        self.write("reviews/second.json", second)
        self.assert_blocked("semantic review is not accepted")

    def test_policy_cannot_be_weakened_without_external_pin(self):
        self.policy["scenarios"] = []
        self.write("policy.json", self.policy)
        self.assert_blocked("externally pinned policy")

    def test_provisional_and_missing_content_remain_blocked(self):
        self.policy["coverage_status"] = "provisional"
        self.freeze_policy()
        self.freeze_receipt()
        self.assert_blocked("provisional")
        (self.root / "content/pack.json").unlink()
        self.policy["content_files"] = []
        self.freeze_policy()
        self.receipt["content_inputs_sha256"] = gate.canonical_sha([])
        self.freeze_receipt()
        self.assert_blocked("content inputs not pinned")

    def test_changed_or_added_runtime_invalidates_receipt(self):
        (self.root / "runtime/new.gd").write_text("new runtime code")
        self.assert_blocked("runtime is stale")

    def test_changed_or_unlisted_content_blocks(self):
        (self.root / "content/unlisted.bin").write_bytes(b"new input")
        self.assert_blocked("entire isolated content tree")
        (self.root / "content/unlisted.bin").unlink()
        (self.root / "content/pack.json").write_text("changed content")
        self.assert_blocked("current content identity mismatch")

    def test_stale_or_modified_artifact_blocks(self):
        (self.root / "evidence/frame.png").write_bytes(b"modified")
        self.assert_blocked("artifact hash/size mismatch")

    def test_old_receipt_blocks(self):
        self.receipt["started_at"] = "2026-10-25T00:00:00Z"
        self.freeze_receipt()
        self.assert_blocked("stale or has invalid timestamps")

    def test_duplicate_ids_and_omitted_checkpoint(self):
        self.receipt["checks"].append(copy.deepcopy(self.receipt["checks"][0]))
        self.freeze_receipt()
        self.assert_blocked("duplicate check")
        self.receipt["checks"] = [{"id": "wrong", "checkpoint_id": "other", "status": "pass"}]
        self.freeze_receipt()
        self.assert_blocked("required checkpoint omitted")
        self.route["checks"].append(copy.deepcopy(self.route["checks"][0]))
        self.receipt["checks"] = [{"id": "telephone-check", "checkpoint_id": "telephone", "status": "pass", "evidence_ids": ["frame"], "raw_output_id": "route", "raw_check_id": "telephone"}]
        self.freeze_receipt()
        self.assert_blocked("duplicate raw check ID")

    def test_failed_extra_and_unknown_outcomes_always_block(self):
        for outcome in ("fail", "skip", "unsupported", "success", None):
            with self.subTest(outcome=outcome):
                self.receipt["checks"] = [{"id": "telephone-check", "checkpoint_id": "telephone", "status": "pass"},
                                          {"id": "extra", "checkpoint_id": "other", "status": outcome}]
                self.freeze_receipt()
                self.assert_blocked("check outcome")

    def test_synthetic_raw_cannot_be_relabelled_as_actual(self):
        self.producer["synthetic"] = True
        self.freeze_receipt()  # Even newly hash-bound wrapper/review cannot hide raw synthesis.
        self.assert_blocked("synthetic producer")

    def test_raw_start_kind_cannot_be_relabelled_new_game(self):
        self.policy["scenarios"][0]["start_kind"] = "normal-new-game"
        self.receipt["start_kind"] = "normal-new-game"
        self.freeze_policy()
        self.freeze_receipt()
        self.assert_blocked("raw producer start kind was relabeled")

    def test_raw_nested_failed_check_cannot_be_overridden(self):
        self.route["checks"].append({"id": "save-load", "outcome": "fail"})
        self.freeze_receipt()
        self.assert_blocked("raw check not pass: save-load")

    def test_unknown_raw_schema_or_changed_suffix_cannot_hide_failure(self):
        self.route["schema"] = "invented.success.v1"
        self.freeze_receipt()  # Raw records already use .data, never suffix-based parsing.
        self.assert_blocked("unsupported raw producer schema")

    def test_self_review_and_missing_review_block(self):
        self.review["reviewer_id"] = "producer"
        self.freeze_receipt()
        self.assert_blocked("self-review")
        (self.root / "reviews/receipt.json").unlink()
        self.assert_blocked("independent receipt review is required")

    def test_missing_raw_role_cannot_bypass_provenance(self):
        self.receipt["raw_outputs"] = [r for r in self.receipt["raw_outputs"] if r["id"] != "producer"]
        self.write("receipts/run.json", self.receipt)
        self.review["receipt_sha256"] = gate.file_identity(self.root / "receipts/run.json")["sha256"]
        self.write("reviews/receipt.json", self.review)
        self.assert_blocked("complete producer/route/artifacts raw schema trio")

    def test_wrapper_cannot_invent_checkpoint_from_unrelated_raw_pass(self):
        self.route["checks"] = [{"id": "unrelated-route", "outcome": "pass"}]
        self.freeze_receipt()
        self.assert_blocked("policy-required raw check is missing")

    def test_execution_identity_and_producer_script_are_bound(self):
        self.receipt["execution"]["platform_id"] = "other-platform"
        self.freeze_receipt()
        self.assert_blocked("execution identity mismatch")
        self.receipt["execution"] = {**self.policy["execution"], "producer_script_artifact_id": "missing"}
        self.freeze_receipt()
        self.assert_blocked("producer script artifact is missing")

    def test_path_traversal_and_symlink_rejected(self):
        self.receipt["evidence"][0]["path"] = "../content/pack.json"
        self.freeze_receipt()
        self.assert_blocked("path traversal")
        self.receipt["evidence"][0]["path"] = "frame.png"
        (self.root / "runtime/alias.gd").symlink_to(self.root / "runtime/main.gd")
        self.freeze_receipt()
        self.assert_blocked("symbolic links")

    def actual_red_receipts_never_pass(self):
        """Explicit integration invocation only; offline unit discovery needs no originals."""
        directory = os.environ.get("ACTUAL_RED_RECEIPTS")
        if not directory:
            self.fail("ACTUAL_RED_RECEIPTS is required for the original failed-app regression")
        root = Path(directory)
        route = gate.read_json(root / "proof/route.json")
        errors = gate.raw_failures(route, "actual-red", scenario_id="week-sunday-yoshio", start_kind="captured-boundary")
        self.assertTrue(any("raw check not pass: initial-save" in e for e in errors), errors)
        self.assertTrue(any("raw check not pass: Yoshio enters original native mode" in e for e in errors), errors)
        self.assertTrue(any("unattempted" in e for e in errors), errors)
        artifacts = gate.read_json(root / "proof/artifacts.json")
        self.assertTrue(gate.raw_failures(artifacts, "actual-red", scenario_id="week-sunday-yoshio", start_kind="captured-boundary"))

    def test_cli_structural_failure_is_json_nonzero(self):
        (self.root / "policy.json").write_text('{"schema":"bad","schema":"duplicate"}')
        args = [sys.executable, "-m", "tools.port.readiness", "--expected-policy-sha256", self.expected_policy]
        for name in ("policy", "runtime-root", "content-root", "evidence-root", "receipt-dir", "review-dir"):
            path = {"policy": "policy.json", "runtime-root": "runtime", "content-root": "content", "evidence-root": "evidence", "receipt-dir": "receipts", "review-dir": "reviews"}[name]
            args.extend(["--" + name, str(self.root / path)])
        process = subprocess.run(args, capture_output=True, text=True, timeout=10)
        self.assertNotEqual(process.returncode, 0)
        result = json.loads(process.stdout)
        self.assertIn("missing_scenarios", result)
        self.assertFalse(result["full_port_complete"])

    def add_differential(self):
        from tools.port import differential
        evidence_root = self.root / "evidence"
        self.write("evidence/projection.json", {"schema": "tokimemo.trace-projection.v1",
                   "required_state_keys": ["projected_state", "input", "rng", "services"], "required_projected_state_fields": ["mode"]})
        self.write("evidence/initial.json", {"seed": 1})
        (evidence_root / "inputs.jsonl").write_text('{"frame":0,"buttons":[]}\n')
        for name in ("native_capture.py", "port_capture.py"):
            (evidence_root / name).write_text("# invented unit-contract producer " + name + "\n")
        common = {"schema": "tokimemo.differential-run.v1",
                  "projection_schema_sha256": gate.file_identity(evidence_root / "projection.json")["sha256"],
                  "initial_state_sha256": gate.file_identity(evidence_root / "initial.json")["sha256"],
                  "input_log_sha256": gate.file_identity(evidence_root / "inputs.jsonl")["sha256"]}
        self.write("evidence/native_run.json", {**common, "role": "native_emulator", "producer_id": "native-test",
                   "producer_script_sha256": gate.file_identity(evidence_root / "native_capture.py")["sha256"],
                   "original_executable_sha256": self.policy["content_files"][0]["sha256"]})
        self.write("evidence/port_run.json", {**common, "role": "actual_app", "producer_id": "port-test",
                   "producer_script_sha256": gate.file_identity(evidence_root / "port_capture.py")["sha256"],
                   "port_tree_sha256": self.receipt["runtime_tree_sha256"]})
        row = {"iteration": 0, "phase": "school_day", "source": "actual_loop", "state": {
            "projected_state": {"mode": 1}, "input": {"held": 0, "pressed": 0, "released": 0}, "rng": {"state": 1, "draw_count": 0}, "services": []}}
        for name in ("native.jsonl", "port.jsonl"):
            (evidence_root / name).write_text(json.dumps(row) + "\n")
        report = differential.compare_runs(evidence_root=evidence_root,
            native_trace=evidence_root / "native.jsonl", port_trace=evidence_root / "port.jsonl",
            projection_schema=evidence_root / "projection.json", expected_projection_sha256=common["projection_schema_sha256"],
            original_executable=self.root / "content/pack.json", port_root=self.root / "runtime",
            initial_state=evidence_root / "initial.json", input_log=evidence_root / "inputs.jsonl",
            native_metadata=evidence_root / "native_run.json", port_metadata=evidence_root / "port_run.json",
            native_producer_id="native-test", native_producer_script=evidence_root / "native_capture.py",
            port_producer_id="port-test", port_producer_script=evidence_root / "port_capture.py")
        self.write("evidence/differential.data", report)
        self.policy["differential"] = {"original_executable_content_id": "pack", "approved_projection_sha256": [common["projection_schema_sha256"]]}
        self.policy["scenarios"][0]["checkpoints"][0]["evidence"].extend([
            {"kind": "reference_comparison", "allowed_origins": ["native_emulator"], "initial_state_sha256": common["initial_state_sha256"], "input_log_sha256": common["input_log_sha256"], "port_producer_id": "port-test"},
            {"kind": "state_trace", "allowed_origins": ["actual_app"]},
            {"kind": "input_log", "allowed_origins": ["actual_app"]}])
        for key, kind, name in (("initial", "state_trace", "initial.json"), ("inputs", "input_log", "inputs.jsonl")):
            self.receipt["evidence"].append({"id": key, "checkpoint_id": "telephone", "kind": kind, "origin": "actual_app", "path": name, "synthetic": False, **gate.file_identity(evidence_root / name)})
        self.receipt["evidence"].append({"id": "comparison", "checkpoint_id": "telephone", "kind": "reference_comparison",
            "origin": "native_emulator", "path": "differential.data", "synthetic": False,
            "initial_state_evidence_id": "initial", "input_log_evidence_id": "inputs", **gate.file_identity(evidence_root / "differential.data")})
        self.receipt["checks"][0]["evidence_ids"].extend(["initial", "inputs", "comparison"])
        self.freeze_policy()
        self.freeze_receipt()
        return report

    def update_differential(self, report):
        self.write("evidence/differential.data", report)
        self.receipt["evidence"][-1].update(gate.file_identity(self.root / "evidence/differential.data"))
        self.freeze_receipt()

    def test_differential_report_is_replayed_and_bound_to_current_inputs(self):
        report = self.add_differential()
        self.assertTrue(self.run_gate()["independently_reviewed"], self.run_gate())
        report["pins"]["port_tree_sha256"] = "0" * 64
        self.update_differential(report)
        self.assert_blocked("differential port runtime is stale")

    def test_differential_original_and_projection_cannot_be_relabelled(self):
        report = self.add_differential()
        original = copy.deepcopy(report)
        report["pins"]["original_executable_sha256"] = "0" * 64
        self.update_differential(report)
        self.assert_blocked("original executable identity mismatch")
        report = original
        report["pins"]["projection_schema_sha256"] = "0" * 64
        self.update_differential(report)
        self.assert_blocked("projection is not externally approved")

    def test_forged_differential_pass_cannot_hide_raw_divergence(self):
        report = self.add_differential()
        path = self.root / "evidence/port.jsonl"
        row = json.loads(path.read_text())
        row["state"]["rng"]["state"] = 99
        path.write_text(json.dumps(row) + "\n")
        report["port_trace"].update(gate.file_identity(path))
        report["passed"] = True
        report["first_divergence"] = None
        self.update_differential(report)
        self.assert_blocked("differential")

    def test_direct_cli_replays_reference_comparison(self):
        self.add_differential()
        timestamp = datetime.now(timezone.utc).isoformat()
        self.receipt["started_at"] = timestamp
        self.receipt["finished_at"] = timestamp
        self.freeze_receipt()
        args = [sys.executable, gate.__file__, "--expected-policy-sha256", self.expected_policy]
        for name, path in (("policy", "policy.json"), ("runtime-root", "runtime"), ("content-root", "content"),
                           ("evidence-root", "evidence"), ("receipt-dir", "receipts"), ("review-dir", "reviews")):
            args.extend(["--" + name, str(self.root / path)])
        process = subprocess.run(args, capture_output=True, text=True, timeout=10, cwd=self.root)
        self.assertEqual(process.returncode, 0, process.stdout + process.stderr)
        result = json.loads(process.stdout)
        self.assertTrue(result["independently_reviewed"])
        self.assertFalse(result["full_port_complete"])

    def test_unrelated_differential_input_state_or_producer_cannot_satisfy_checkpoint(self):
        original = self.add_differential()
        for field in ("initial_state_sha256", "input_log_sha256", "producer_id"):
            with self.subTest(field=field):
                report = copy.deepcopy(original)
                if field == "producer_id":
                    report["port_trace"]["producer_id"] = "unrelated-run"
                else:
                    report["pins"][field] = "0" * 64
                self.update_differential(report)
                self.assert_blocked("differential checkpoint")

    def test_differential_must_link_actual_input_state_artifacts(self):
        self.add_differential()
        self.receipt["evidence"][-1]["input_log_evidence_id"] = "frame"
        self.freeze_receipt()
        self.assert_blocked("actual input/state evidence binding mismatch")


if __name__ == "__main__":
    unittest.main()
