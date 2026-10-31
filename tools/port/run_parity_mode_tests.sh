#!/bin/sh
# T-39230: Parity mode tests (Docker only, no game data).
# usage: tools/port/run_parity_mode_tests.sh
# Godot (tokimemo-port, headless): preferences and runtime refusals, the
# feature registry and Parity UI, the invariance harness, the checkpoint
# observer gate, the launch options (TOKIMEMO_PARITY=1, -- --parity, none,
# MODERN_INVARIANCE=1) and the settings suites the Game page touches.
# Python (tokimemo-content): parity_gate, parity_invariance and the gated
# differential suite and pair builder.
set -eu
cd "$(dirname "$0")/../.."
ROOT=$PWD
docker run --rm --name "tokimemo-parity-mode-godot-$$" --cpus=2 -v "$ROOT/port/godot:/workspace:ro" tokimemo-port sh -c '
status=0
run() { # run <label> <VAR=value list or -> <script> [godot user args...]
	label=$1; vars=$2; script=$3; shift 3
	[ "$vars" = - ] && vars=
	s=0; env $vars timeout 180s godot --headless --rendering-method gl_compatibility --path /workspace --script "$script" "$@" >/tmp/log 2>&1 || s=$?
	errors=$(grep -c "SCRIPT ERROR" /tmp/log || true)
	if [ "$s" -ne 0 ] || [ "$errors" -ne 0 ]; then echo "FAIL $label (exit $s, script errors $errors)"; grep -v "^Godot Engine" /tmp/log | head -40; status=1
	else echo "PASS $label: $(grep -v "^Godot Engine" /tmp/log | grep -v "^$" | tail -1)"; fi
}
for t in services/run_parity_mode services/run_feature_registry support/run_parity_invariance_tests \
	support/run_checkpoint_observer_tests support/run_input_script_player_tests services/run_settings_pages \
	services/run_settings_navigation services/run_settings_save_menu services/run_modern_settings app/run_first_launch_welcome; do
	run "$t" - "res://tests/$t.gd"
done
run "launch: no option" PARITY_LAUNCH_EXPECT=modern res://tests/services/run_parity_launch.gd
run "launch: TOKIMEMO_PARITY=1" "PARITY_LAUNCH_EXPECT=forced-env TOKIMEMO_PARITY=1" res://tests/services/run_parity_launch.gd
run "launch: -- --parity" PARITY_LAUNCH_EXPECT=forced-arg res://tests/services/run_parity_launch.gd -- --parity
run "launch: MODERN_INVARIANCE=1" "PARITY_LAUNCH_EXPECT=invariance MODERN_INVARIANCE=1" res://tests/services/run_parity_launch.gd
exit $status'
sh tools/port/run_playtest_unit_tests.sh tools.port.test_parity_gate tools.port.test_parity_invariance \
	tools.port.test_run_differential_suite tools.port.test_rei_stage_capture_pairs
