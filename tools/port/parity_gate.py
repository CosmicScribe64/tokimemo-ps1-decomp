#!/usr/bin/env python3
"""T-39230: Parity gate for evidence the port produces.

Differential, golden and playtest comparisons hold the port to the original
game, so the port side must have run in Parity mode
(port/godot/services/parity_mode.gd). The Godot routes write
``parity-mode.json`` (schema ``tokimemo.parity-mode-identity.v1``) and every
v2 checkpoint receipt carries the play mode in ``settings``. This module
checks both, including that the run used the feature registry and locked
values pinned in port/godot/services/parity-registry-pin.json.

Only tools/port/parity_invariance.py accepts an explicit modern-invariance
run (``allow_modern_invariance=True``), and it compares that run against a
Parity run, never against the original. The CLI and every comparison against
the original accept Parity runs only; there is no option or environment
variable that widens them.

Usage: python3 tools/port/parity_gate.py <parity-mode.json | run directory>.
       Exit 0 accepted, 1 refused (including a missing or unreadable file).
"""
import argparse
import json
from pathlib import Path
import sys

IDENTITY_SCHEMA = "tokimemo.parity-mode-identity.v1"
IDENTITY_FILE = "parity-mode.json"
PIN_SCHEMA = "tokimemo.parity-registry-pin.v1"
PIN_FILE = Path(__file__).resolve().parents[2] / "port/godot/services/parity-registry-pin.json"


class ParityError(ValueError):
    pass


def load_pin(path=PIN_FILE):
    """The reviewed registry and locked-value digests the evidence must name."""
    try:
        pin = json.loads(Path(path).read_text())
    except (OSError, ValueError) as error:
        raise ParityError("cannot read the registry pin %s: %s" % (path, error))
    if not isinstance(pin, dict) or pin.get("schema") != PIN_SCHEMA:
        raise ParityError("%s is not a %s record" % (path, PIN_SCHEMA))
    return pin


def check_digests(registry_sha256, locked_values_sha256, pin=None):
    """Raise ParityError unless the run used the pinned registry and locked values."""
    pin = load_pin() if pin is None else pin
    for name, value in (("registry_sha256", registry_sha256), ("locked_values_sha256", locked_values_sha256)):
        if value != pin.get(name):
            raise ParityError("run used another feature registry (%s %s, pinned %s); rerun under the current registry"
                              % (name, value, pin.get(name)))


def check_identity(identity, allow_modern_invariance=False, pin=None):
    """Raise ParityError unless the identity proves a clean Parity run (or, for
    parity_invariance.py only, a modern-invariance run) under the pinned
    registry. Returns the play mode."""
    if not isinstance(identity, dict) or identity.get("schema") != IDENTITY_SCHEMA:
        raise ParityError("not a %s record" % IDENTITY_SCHEMA)
    if identity.get("gate") == "not_evidence":
        raise ParityError("the port run is a QA route that applies modern preferences on purpose, not evidence")
    if identity.get("gate") != "passed":
        raise ParityError("the port's own parity gate did not pass: %s" % identity.get("error", "no gate result"))
    mode = identity.get("play_mode")
    if mode == "parity":
        if identity.get("violations") != []:
            raise ParityError("Parity mode is not clean: %s" % identity.get("violations"))
    elif mode == "modern" and identity.get("invariance_run") is True:
        if not allow_modern_invariance:
            raise ParityError("modern-invariance run offered where Parity is required (only parity_invariance.py accepts one)")
    else:
        raise ParityError("port did not run in Parity mode (play_mode %s)" % mode)
    check_digests(identity.get("registry_sha256"), identity.get("locked_values_sha256"), pin)
    return mode


def check_receipt_settings(settings, allow_modern_invariance=False, pin=None):
    """Same rule for the settings block of a v2 checkpoint receipt."""
    if not isinstance(settings, dict) or "play_mode" not in settings:
        raise ParityError("receipt settings carry no play_mode (written before Parity mode, or by an unknown producer); rerun under --parity")
    mode = settings["play_mode"]
    if mode == "parity":
        if settings.get("parity_violations") != []:
            raise ParityError("receipt made in an unclean Parity mode: %s" % settings.get("parity_violations"))
    elif not (mode == "modern" and settings.get("invariance_run") is True and allow_modern_invariance):
        raise ParityError("receipt was not made in Parity mode (play_mode %s)" % mode)
    check_digests(settings.get("parity_registry_sha256"), settings.get("parity_locked_values_sha256"), pin)
    return mode


def check_receipts(receipts, allow_modern_invariance=False, pin=None):
    """All receipts of a v2 set must agree on one accepted play mode."""
    pin = load_pin() if pin is None else pin
    modes = set()
    for key, receipt in sorted(receipts.items()):
        try:
            modes.add(check_receipt_settings(receipt.get("settings"), allow_modern_invariance, pin))
        except ParityError as error:
            raise ParityError("receipt %s: %s" % (key, error))
    if len(modes) > 1:
        raise ParityError("receipts mix play modes: %s" % sorted(modes))
    return modes.pop() if modes else None


def load_identity(path):
    path = Path(path)
    if path.is_dir():
        path = path / IDENTITY_FILE
    if not path.is_file():
        raise ParityError("missing %s (the port run did not pass through the Parity gate)" % path)
    try:
        return json.loads(path.read_text())
    except ValueError as error:
        raise ParityError("unreadable %s: %s" % (path, error))


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("identity", type=Path, help="parity-mode.json or the run directory holding it")
    args = parser.parse_args(argv)
    try:
        mode = check_identity(load_identity(args.identity))
    except ParityError as error:
        print(json.dumps({"status": "refused", "error": str(error)}), file=sys.stderr)
        return 1
    print(json.dumps({"status": "accepted", "play_mode": mode}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
