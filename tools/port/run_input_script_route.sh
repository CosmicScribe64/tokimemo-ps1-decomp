#!/bin/sh
# T-39023 (T-39018 WP-06): play an input-script v1 on the installed port under Xvfb (Docker only).
# usage: tools/port/run_input_script_route.sh <new-out-dir> <config/port/input-scripts/NAME.json> [target-stage]
# Runs tests/support/run_input_script_route.gd (REI_TARGET_STAGE, default 12, is the declared
# port stop) against a prepared original bundle, native audio library and BGM bridge built from
# the user's own disc under .asset-cache (never committed); override them with
# OPENING_TIMING_BUNDLE, OPENING_TIMING_SO and OPENING_TIMING_BRIDGE.
# Prints the script preflight (expected unsupported stops) before the long run.
set -eu
cd "$(dirname "$0")/../.."
ROOT=$PWD; OUT=$1; SCRIPT=$2; TARGET=${3:-12}
[ ! -e "$OUT" ] || { echo "$OUT exists"; exit 2; }
[ -f "$SCRIPT" ] || { echo "missing $SCRIPT"; exit 2; }
SO=${OPENING_TIMING_SO:-.asset-cache/port/native-audio-shiori-v2/project/bin/liboriginal_audio.linux.template_release.arm64.so}
BRIDGE=${OPENING_TIMING_BRIDGE:-.asset-cache/port/opening-hold-bank-bridge-v1/candidate_bgm_bridge.gd}
BUNDLE=${OPENING_TIMING_BUNDLE:-.asset-cache/port/opening-line17-runtime-v3}
for f in "$SO" "$BRIDGE" "$BUNDLE/manifest.json"; do [ -e "$f" ] || { echo "missing $f"; exit 2; }; done
mkdir -p "$OUT/content-install/.staging"
case $OUT in /*) ABS_OUT=$OUT ;; *) ABS_OUT=$ROOT/$OUT ;; esac
case $SO in /*) ABS_SO=$SO ;; *) ABS_SO=$ROOT/$SO ;; esac
NAME=tokimemo-input-script-$(basename "$OUT" | tr -c 'A-Za-z0-9_.\n-' '-')
docker run --rm --name "$NAME-preflight" -v "$ROOT:/work:ro" -w /work tokimemo-recovery \
	python3 tools/emu/input_script.py preflight "$SCRIPT" --side port > "$OUT/preflight.json"
cat "$OUT/preflight.json"
cp -R "$BUNDLE" "$OUT/content-install/.staging/prepared-original"
cp "$BRIDGE" "$OUT/candidate_bgm_bridge.gd"
cp "$SCRIPT" "$OUT/input-script.json"
shasum -a 256 "$SO" "$BRIDGE" "$BUNDLE/manifest.json" "$SCRIPT" port/godot/tests/support/run_input_script_route.gd \
	port/godot/tests/support/input_script_player.gd port/godot/tests/support/checkpoint_observer.gd \
	port/godot/tests/opening/run_installed_rei_banner.gd > "$OUT/hashes.txt"
exec docker run --rm --name "$NAME" --cpus=4 -e REI_DIALOGUE=1 -e REI_TARGET_STAGE="$TARGET" -e INPUT_SCRIPT=/out/input-script.json \
	-v "$ROOT:/work" -v "$ABS_OUT:/out" \
	-v "$ABS_SO:/work/port/godot/bin/liboriginal_audio.linux.template_release.arm64.so:ro" \
	-v "$ABS_OUT/candidate_bgm_bridge.gd:/work/port/godot/services/first_arrival_bgm_bridge.gd:ro" \
	tokimemo-port-gui-test sh -ec '
	Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp -ac >/out/xvfb.log 2>&1 &
	export DISPLAY=:99 GODOT_SILENCE_ROOT_WARNING=1; sleep 1
	pulseaudio -D --exit-idle-time=-1 --system=false 2>/dev/null || true
	pactl load-module module-null-sink sink_name=virtual >/dev/null 2>&1 || true
	status=0
	timeout 1200 godot --path /work/port/godot --audio-driver PulseAudio --rendering-method gl_compatibility --script res://tests/support/run_input_script_route.gd > /out/godot.log 2>&1 || status=$?
	grep -E "^PASS|^FAIL|ERROR|SCRIPT ERROR|input script" /out/godot.log | head -20
	exit $status'
