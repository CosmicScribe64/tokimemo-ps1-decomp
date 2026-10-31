#!/bin/sh
# T-39027 opening pacing receipt (Docker only).
# usage: tools/port/run_presentation_pacing.sh <new-out-dir> <pad> [native-frames.jsonl]
# Runs tests/opening/run_presentation_pacing.gd (the T-39002 frame timing route plus
# the observe-only pacing probe) under Xvfb in tokimemo-port-gui-test, then, when a
# native per-vsync frames.jsonl is given, writes <out>/pacing-report.json with
# tools/port/frame_pacing_report.py in tokimemo-recovery. Inputs are the same
# .asset-cache bundles as run_opening_frame_timing.sh (OPENING_TIMING_BUNDLE,
# OPENING_TIMING_SO, OPENING_TIMING_BRIDGE). Host metrics from this runner are
# Xvfb numbers: relative-only regression signals, never T4 pacing evidence.
# T-39230: the port side runs in Parity mode (TOKIMEMO_PARITY=1 by default) and the
# route fails fast without it. MODERN_INVARIANCE=1 is refused (see below).
set -eu
# T-39230: this runner's output is compared with the original, so a modern-invariance
# run is refused here; tools/port/run_parity_invariance.sh is the only invariance entry.
[ "${MODERN_INVARIANCE:-0}" = 0 ] || { echo "MODERN_INVARIANCE=1 is refused: this run is compared with the original; use tools/port/run_parity_invariance.sh"; exit 2; }
cd "$(dirname "$0")/../.."
ROOT=$PWD; OUT=$1; PAD=$2; NATIVE=${3:-}
[ ! -e "$OUT" ] || { echo "$OUT exists"; exit 2; }
SO=${OPENING_TIMING_SO:-.asset-cache/port/native-audio-shiori-v2/project/bin/liboriginal_audio.linux.template_release.arm64.so}
BRIDGE=${OPENING_TIMING_BRIDGE:-.asset-cache/port/opening-hold-bank-bridge-v1/candidate_bgm_bridge.gd}
BUNDLE=${OPENING_TIMING_BUNDLE:-.asset-cache/port/opening-line17-runtime-v3}
for f in "$SO" "$BRIDGE" "$BUNDLE/manifest.json"; do [ -e "$f" ] || { echo "missing $f"; exit 2; }; done
if [ -n "$NATIVE" ]; then [ -f "$NATIVE" ] || { echo "missing $NATIVE"; exit 2; }; fi
mkdir -p "$OUT/content-install/.staging"
cp -R "$BUNDLE" "$OUT/content-install/.staging/prepared-original"
cp "$BRIDGE" "$OUT/candidate_bgm_bridge.gd"
case $OUT in /*) ABS_OUT=$OUT ;; *) ABS_OUT=$ROOT/$OUT ;; esac
case $SO in /*) ABS_SO=$SO ;; *) ABS_SO=$ROOT/$SO ;; esac
NAME=tokimemo-presentation-pacing-$(basename "$OUT" | tr -c 'A-Za-z0-9_.\n-' '-')
shasum -a 256 "$SO" "$BRIDGE" "$BUNDLE/manifest.json" port/godot/tests/opening/run_presentation_pacing.gd \
	port/godot/tests/support/pacing_probe.gd port/godot/tests/opening/run_opening_frame_timing_route.gd \
	port/godot/tests/fixtures/opening_frame_timing.json > "$OUT/hashes.txt"
status=0
docker run --rm --name "$NAME" --cpus=4 -e TOKIMEMO_PARITY="${TOKIMEMO_PARITY:-1}" -e MODERN_INVARIANCE=0 -e REI_DIALOGUE=1 -e REI_TARGET_STAGE=12 -e TIMED_ENTRY_PAD="$PAD" \
	-e TOKIMEMO_HOST_DISPLAY=xvfb \
	-v "$ROOT:/work" -v "$ABS_OUT:/out" \
	-v "$ABS_SO:/work/port/godot/bin/liboriginal_audio.linux.template_release.arm64.so:ro" \
	-v "$ABS_OUT/candidate_bgm_bridge.gd:/work/port/godot/services/first_arrival_bgm_bridge.gd:ro" \
	tokimemo-port-gui-test sh -ec '
	Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp -ac >/out/xvfb.log 2>&1 &
	export DISPLAY=:99 GODOT_SILENCE_ROOT_WARNING=1; sleep 1
	pulseaudio -D --exit-idle-time=-1 --system=false 2>/dev/null || true
	pactl load-module module-null-sink sink_name=virtual >/dev/null 2>&1 || true
	status=0
	timeout 900 godot --path /work/port/godot --audio-driver PulseAudio --rendering-method gl_compatibility --script res://tests/opening/run_presentation_pacing.gd > /out/godot.log 2>&1 || status=$?
	grep -E "opening frame timing|^PASS|^FAIL|ERROR|SCRIPT ERROR" /out/godot.log | head -20
	exit $status' || status=$?
if [ -n "$NATIVE" ] && [ -f "$OUT/pacing-probe.json" ]; then
	case $NATIVE in /*) ABS_NATIVE=$NATIVE ;; *) ABS_NATIVE=$ROOT/$NATIVE ;; esac
	NATIVE_DIR=$(dirname "$ABS_NATIVE")
	# loops.jsonl next to frames.jsonl keys iterations at the original's loop boundary.
	LOOPS=
	if [ -f "$NATIVE_DIR/loops.jsonl" ]; then LOOPS="--native-loops /native/loops.jsonl"; fi
	# shellcheck disable=SC2086
	docker run --rm --name "$NAME-report" -e PYTHONDONTWRITEBYTECODE=1 \
		-v "$ROOT/tools:/repo/tools:ro" -v "$NATIVE_DIR:/native:ro" -v "$ABS_OUT:/out" -w /repo \
		--entrypoint python3 tokimemo-recovery -m tools.port.frame_pacing_report \
		--native "/native/$(basename "$ABS_NATIVE")" $LOOPS --until 12/0/0 \
		--port /out/pacing-probe.json --output /out/pacing-report.json || status=$?
fi
exit $status
