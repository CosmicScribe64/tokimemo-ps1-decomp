#!/bin/sh
# T-39230: modern-vs-parity invariance on the installed route (Docker only).
# usage: tools/port/run_parity_invariance.sh <new-out-dir> <config/port/input-scripts/NAME.json> [target-stage]
# Plays the same input script four times through tools/port/run_input_script_route.sh with
# res://tests/support/run_parity_invariance_route.gd:
#   <out>/parity             TOKIMEMO_PARITY=1 (Parity gate)
#   <out>/parity_control     the same again: proves the route is reproducible run to run
#   <out>/modern             MODERN_INVARIANCE=1, every state_and_image registry entry on
#   <out>/modern_state_only  MODERN_INVARIANCE=1, plus the state_only entries (text drawing, language)
# then compares them with tools/port/parity_invariance.py into <out>/invariance-report.json.
# The route may stop at a declared port stop or a script failure; the comparison covers the
# frames each run reached and fails on any difference, including a different length.
# Inputs (bundle, native audio library, BGM bridge) are the same as run_input_script_route.sh.
# TOKIMEMO_NEW_GAME_SEED (default 19950404) is passed to every run. The installed entry
# boot ignores it today and seeds from the host clock (root hook in wiki/parity-mode.md),
# so the parity control diverges on the RNG and the report says baseline_nondeterministic.
set -eu
cd "$(dirname "$0")/../.."
OUT=$1; SCRIPT=$2; TARGET=${3:-12}
[ ! -e "$OUT" ] || { echo "$OUT exists"; exit 2; }
mkdir -p "$OUT"
for profile in parity parity_control modern modern_state_only; do
	case $profile in parity*) parity=1; invariance=0 ;; *) parity=0; invariance=1 ;; esac
	INPUT_SCRIPT_ROUTE=res://tests/support/run_parity_invariance_route.gd PARITY_INVARIANCE_PROFILE=${profile%_control} \
		TOKIMEMO_PARITY=$parity MODERN_INVARIANCE=$invariance TOKIMEMO_NEW_GAME_SEED=${TOKIMEMO_NEW_GAME_SEED:-19950404} \
		sh tools/port/run_input_script_route.sh "$OUT/$profile" "$SCRIPT" "$TARGET" > "$OUT/$profile.log" 2>&1 \
		|| echo "$profile route exited nonzero (see $OUT/$profile/godot.log); comparing the frames it reached"
done
case $OUT in /*) ABS_OUT=$OUT ;; *) ABS_OUT=$PWD/$OUT ;; esac
exec docker run --rm --name "tokimemo-parity-invariance-$(basename "$OUT" | tr -c 'A-Za-z0-9_.\n-' '-')" -e PYTHONDONTWRITEBYTECODE=1 \
	-v "$PWD:/work:ro" -v "$ABS_OUT:/out" -w /work tokimemo-recovery \
	python3 tools/port/parity_invariance.py --parity /out/parity --modern /out/modern --state-only /out/modern_state_only --control /out/parity_control \
	--out /out/invariance-report.json
