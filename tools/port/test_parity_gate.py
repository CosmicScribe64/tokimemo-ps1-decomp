"""T-39230: Parity gate checks for port evidence (synthetic records only).

Run: tools/port/run_playtest_unit_tests.sh tools.port.test_parity_gate
"""
import contextlib
import io
import json
from pathlib import Path
import tempfile
import unittest
import unittest.mock

from tools.port import parity_gate as gate


PIN = gate.load_pin()


def identity(**overrides):
    return dict({"schema": gate.IDENTITY_SCHEMA, "play_mode": "parity", "forced_by": "TOKIMEMO_PARITY",
                 "invariance_run": False, "violations": [], "gate": "passed", "error": "",
                 "registry_sha256": PIN["registry_sha256"], "locked_values_sha256": PIN["locked_values_sha256"]}, **overrides)


def settings(**overrides):
    return dict({"source": "ModernSettings.effective_settings", "language": "ja", "aspect": "original", "filter": "integer",
                 "play_mode": "parity", "parity_violations": [], "invariance_run": False,
                 "parity_registry_sha256": PIN["registry_sha256"],
                 "parity_locked_values_sha256": PIN["locked_values_sha256"]}, **overrides)


class IdentityTest(unittest.TestCase):
    def test_clean_parity_is_accepted(self):
        self.assertEqual(gate.check_identity(identity()), "parity")

    def test_everything_else_is_refused(self):
        cases = {
            "not a record": [],
            "other schema": identity(schema="tokimemo.other.v1"),
            "gate failed": identity(gate="failed", error="Parity mode is not active"),
            "QA route, not evidence": identity(gate="not_evidence"),
            "no gate": {k: v for k, v in identity().items() if k != "gate"},
            "modern": identity(play_mode="modern"),
            "unknown": identity(play_mode="unknown"),
            "unclean": identity(violations=["accessibility.text_speed"]),
            "violations missing": {k: v for k, v in identity().items() if k != "violations"},
            "invariance without allowance": identity(play_mode="modern", invariance_run=True),
            "invariance flag not boolean": identity(play_mode="modern", invariance_run="1"),
        }
        for name, record in cases.items():
            with self.subTest(name), self.assertRaises(gate.ParityError):
                gate.check_identity(record)

    def test_modern_invariance_needs_both_the_run_flag_and_the_allowance(self):
        self.assertEqual(gate.check_identity(identity(play_mode="modern", invariance_run=True), True), "modern")
        with self.assertRaises(gate.ParityError):
            gate.check_identity(identity(play_mode="modern", invariance_run=False), True)
        with self.assertRaises(gate.ParityError):
            gate.check_identity(identity(play_mode="unknown", invariance_run=True), True)

    def test_registry_digests_must_match_the_pin(self):
        stale = "0" * 64
        for record in (identity(registry_sha256=stale), identity(locked_values_sha256=stale),
                       {k: v for k, v in identity().items() if k != "registry_sha256"},
                       identity(play_mode="modern", invariance_run=True, locked_values_sha256="")):
            with self.subTest(record=record), self.assertRaises(gate.ParityError) as raised:
                gate.check_identity(record, True)
            self.assertIn("rerun under the current registry", str(raised.exception))
        with self.assertRaises(gate.ParityError):
            gate.check_receipt_settings(settings(parity_registry_sha256=stale))
        with self.assertRaises(gate.ParityError):
            gate.check_receipt_settings({k: v for k, v in settings().items() if k != "parity_locked_values_sha256"})
        # An explicit pin is honoured (a moved registry accepts its own evidence only).
        moved = dict(PIN, registry_sha256=stale)
        self.assertEqual(gate.check_identity(identity(registry_sha256=stale), pin=moved), "parity")

    def test_pin_file_is_well_formed(self):
        self.assertEqual(PIN["schema"], gate.PIN_SCHEMA)
        self.assertEqual(len(PIN["registry_sha256"]), 64)
        self.assertEqual(len(PIN["locked_values_sha256"]), 64)
        with tempfile.TemporaryDirectory() as tmp:
            bad = Path(tmp) / "pin.json"
            bad.write_text(json.dumps({"schema": "other"}))
            with self.assertRaises(gate.ParityError):
                gate.load_pin(bad)


class ReceiptTest(unittest.TestCase):
    def test_receipt_settings(self):
        self.assertEqual(gate.check_receipt_settings(settings()), "parity")
        for name, record in {"legacy": {"source": "x", "language": "ja", "aspect": "original", "filter": "integer"},
                             "modern": settings(play_mode="modern"), "unclean": settings(parity_violations=["audio.mono"]),
                             "no violations field": {k: v for k, v in settings().items() if k != "parity_violations"},
                             "invariance not allowed": settings(play_mode="modern", invariance_run=True), "not a dict": None}.items():
            with self.subTest(name), self.assertRaises(gate.ParityError):
                gate.check_receipt_settings(record)
        self.assertEqual(gate.check_receipt_settings(settings(play_mode="modern", invariance_run=True), True), "modern")

    def test_receipt_sets_name_the_offender_and_refuse_mixed_modes(self):
        good = {"settings": settings()}
        with self.assertRaises(gate.ParityError) as raised:
            gate.check_receipts({"stage8-1": good, "stage8-2": {"settings": settings(play_mode="modern")}})
        self.assertIn("stage8-2", str(raised.exception))
        mixed = {"a": good, "b": {"settings": settings(play_mode="modern", invariance_run=True)}}
        with self.assertRaises(gate.ParityError):
            gate.check_receipts(mixed, True)
        self.assertEqual(gate.check_receipts({"a": good, "b": good}), "parity")


class CliTest(unittest.TestCase):
    def run_cli(self, *argv):
        out, err = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
            code = gate.main(list(argv))
        return code, out.getvalue() + err.getvalue()

    def test_cli_exit_codes(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "parity-mode.json").write_text(json.dumps(identity()))
            self.assertEqual(self.run_cli(str(root))[0], 0)
            self.assertEqual(self.run_cli(str(root / "parity-mode.json"))[0], 0)
            (root / "m.json").write_text(json.dumps(identity(play_mode="modern", invariance_run=True)))
            code, text = self.run_cli(str(root / "m.json"))
            self.assertEqual(code, 1)
            self.assertIn("refused", text)
            # No option or environment variable lets the CLI accept a modern run.
            with self.assertRaises(SystemExit):
                self.run_cli(str(root / "m.json"), "--modern-invariance")
            with unittest.mock.patch.dict("os.environ", {"MODERN_INVARIANCE": "1"}):
                self.assertEqual(self.run_cli(str(root / "m.json"))[0], 1)
            self.assertEqual(self.run_cli(str(root / "missing"))[0], 1)
            (root / "bad.json").write_text("{")
            self.assertEqual(self.run_cli(str(root / "bad.json"))[0], 1)


if __name__ == "__main__":
    unittest.main()
