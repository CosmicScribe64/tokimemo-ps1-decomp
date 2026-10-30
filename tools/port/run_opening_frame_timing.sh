#!/bin/sh
# T-39002 whole-opening frame timing route (Docker only).
# usage: tools/port/run_opening_frame_timing.sh <new-out-dir> <pad>
# Runs tests/opening/run_opening_frame_timing_route.gd with REI_TARGET_STAGE=12 and
# TIMED_ENTRY_PAD=<pad> (2 or 3, see tests/fixtures/opening_frame_timing.json) against a
# prepared original bundle, native audio library and BGM bridge. Those inputs are built
# from the user's own disc and stay under .asset-cache (never committed); override them
# with OPENING_TIMING_BUNDLE, OPENING_TIMING_SO and OPENING_TIMING_BRIDGE.
# Exit status is the route's: 0 only if every substep matches the fixture.
set -eu
cd "$(dirname "$0")/../.."
ROOT=$PWD; OUT=$1; PAD=$2
[ ! -e "$OUT" ] || { echo "$OUT exists"; exit 2; }
SO=${OPENING_TIMING_SO:-.asset-cache/port/native-audio-core-v1/project/bin/liboriginal_audio.linux.template_release.arm64.so}
BRIDGE=${OPENING_TIMING_BRIDGE:-.asset-cache/port/native-core-bridge-v1/candidate_bgm_bridge.gd}
BUNDLE=${OPENING_TIMING_BUNDLE:-.asset-cache/port/installed-runtime-v14}
for f in "$SO" "$BRIDGE" "$BUNDLE/manifest.json"; do [ -e "$f" ] || { echo "missing $f"; exit 2; }; done
mkdir -p "$OUT/content-install/.staging"
cp -R "$BUNDLE" "$OUT/content-install/.staging/prepared-original"
cp "$BRIDGE" "$OUT/candidate_bgm_bridge.gd"
case $OUT in /*) ABS_OUT=$OUT ;; *) ABS_OUT=$ROOT/$OUT ;; esac
NAME=tokimemo-opening-frame-timing-$(basename "$OUT" | tr -c 'A-Za-z0-9_.\n-' '-')
shasum -a 256 "$SO" "$BRIDGE" "$BUNDLE/manifest.json" port/godot/tests/opening/run_opening_frame_timing_route.gd port/godot/tests/fixtures/opening_frame_timing.json port/godot/services/prologue_media_queue.gd port/godot/services/opening_audio_rules.gd port/godot/services/opening_audio_bridge.gd port/godot/app/first_arrival_boot.gd port/godot/flow/prologue_arrival.gd port/godot/rules/prologue_text_work.gd > "$OUT/hashes.txt"
exec docker run --rm --name "$NAME" --cpus=4 -e REI_DIALOGUE=1 -e REI_TARGET_STAGE=12 -e TIMED_ENTRY_PAD="$PAD" \
	-v "$ROOT:/work" -v "$ABS_OUT:/out" \
	-v "$ROOT/$SO:/work/port/godot/bin/liboriginal_audio.linux.template_release.arm64.so:ro" \
	-v "$ABS_OUT/candidate_bgm_bridge.gd:/work/port/godot/services/first_arrival_bgm_bridge.gd:ro" \
	tokimemo-port-gui-test sh -ec '
	Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp -ac >/out/xvfb.log 2>&1 &
	export DISPLAY=:99 GODOT_SILENCE_ROOT_WARNING=1; sleep 1
	pulseaudio -D --exit-idle-time=-1 --system=false 2>/dev/null || true
	pactl load-module module-null-sink sink_name=virtual >/dev/null 2>&1 || true
	status=0
	timeout 900 godot --path /work/port/godot --audio-driver PulseAudio --rendering-method gl_compatibility --script res://tests/opening/run_opening_frame_timing_route.gd > /out/godot.log 2>&1 || status=$?
	grep -E "opening frame timing|^PASS|^FAIL|ERROR|SCRIPT ERROR" /out/godot.log | head -20
	exit $status'
