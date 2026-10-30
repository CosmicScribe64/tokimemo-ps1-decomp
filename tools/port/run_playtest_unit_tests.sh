#!/bin/sh
# T-39018: original-free unit tests of the playtest/differential harness (Docker only).
# usage: tools/port/run_playtest_unit_tests.sh [unittest module ...]
# Runs in tokimemo-content (Python 3 + Pillow). jsonschema is pinned in
# tools/port/requirements-test.txt and installed into the throwaway container's
# /tmp, never on the host. The repository is mounted read-only.
set -eu
cd "$(dirname "$0")/../.."
[ $# -gt 0 ] || set -- tools.port.test_rgb555_diagnostic tools.port.test_playtest_checkpoints \
	tools.port.test_run_differential_suite tools.port.test_opening_trace_differential tools.emu.test_input_script \
	tools.port.test_rei_stage_capture_pairs
exec docker run --rm --name "tokimemo-playtest-unit-$$" -e PYTHONDONTWRITEBYTECODE=1 -e PYTHONPATH=/tmp/deps \
	-v "$PWD:/work:ro" -w /work tokimemo-content sh -ec '
	/opt/splat/bin/pip install --quiet --disable-pip-version-check --target /tmp/deps -r tools/port/requirements-test.txt
	python3 -m unittest "$@"' unit "$@"
