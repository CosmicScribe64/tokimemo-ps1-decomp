#!/bin/sh
# T-39002 whole-opening frame timing route (Docker only).
# usage: tools/port/run_opening_frame_timing.sh [--fast] <new-out-dir> <pad>
# Runs tests/opening/run_opening_frame_timing_route.gd with REI_TARGET_STAGE=12 and
# TIMED_ENTRY_PAD=<pad> (2 or 3, see tests/fixtures/opening_frame_timing.json) against a
# prepared original bundle, native audio library and BGM bridge. Those inputs are built
# from the user's own disc and stay under .asset-cache (never committed); override them
# with OPENING_TIMING_BUNDLE, OPENING_TIMING_SO and OPENING_TIMING_BRIDGE (relative to
# the repository or absolute).
# --fast: FAST route test mode (see tools/port/run_installed_route.sh and
# tests/fast_route/fast_route.gd): --fixed-fps 60 --max-fps 0 --disable-vsync, dummy
# rendering driver, offline sink bound over bgm_sink.gd, no PulseAudio. Both modes write
# frames.tsv, audio-blocks.tsv and route-speed.json for tools/port/compare_route_modes.py.
# Exit status is the route's: 0 only if every substep matches the fixture.
set -eu
cd "$(dirname "$0")/../.."
ROOT=$PWD
FAST=0
if [ "${1:-}" = "--fast" ]; then FAST=1; shift; fi
OUT=$1; PAD=$2
[ ! -e "$OUT" ] || { echo "$OUT exists"; exit 2; }
SO=${OPENING_TIMING_SO:-.asset-cache/port/native-audio-core-v1/project/bin/liboriginal_audio.linux.template_release.arm64.so}
BRIDGE=${OPENING_TIMING_BRIDGE:-.asset-cache/port/native-core-bridge-v1/candidate_bgm_bridge.gd}
BUNDLE=${OPENING_TIMING_BUNDLE:-.asset-cache/port/installed-runtime-v14}
for f in "$SO" "$BRIDGE" "$BUNDLE/manifest.json"; do [ -e "$f" ] || { echo "missing $f"; exit 2; }; done
mkdir -p "$OUT/content-install/.staging"
cp -R "$BUNDLE" "$OUT/content-install/.staging/prepared-original"
cp "$BRIDGE" "$OUT/candidate_bgm_bridge.gd"
case $OUT in /*) ABS_OUT=$OUT ;; *) ABS_OUT=$ROOT/$OUT ;; esac
case $SO in /*) ABS_SO=$SO ;; *) ABS_SO=$ROOT/$SO ;; esac
NAME=tokimemo-opening-frame-timing-$(basename "$OUT" | tr -c 'A-Za-z0-9_.\n-' '-')
SINK=port/godot/bgm_sink.gd
FAST_MOUNT=
if [ "$FAST" = 1 ]; then
	SINK=port/godot/tests/fast_route/offline_bgm_sink.gd
	FAST_MOUNT="-v $ROOT/$SINK:/work/port/godot/bgm_sink.gd:ro"
	GODOT_MODE_ARGS="--display-driver x11 --rendering-driver dummy --audio-driver Dummy --fixed-fps 60 --max-fps 0 --disable-vsync"
else
	GODOT_MODE_ARGS="--audio-driver PulseAudio --rendering-method gl_compatibility"
fi
shasum -a 256 "$SO" "$BRIDGE" "$BUNDLE/manifest.json" port/godot/tests/opening/run_opening_frame_timing_route.gd port/godot/tests/fixtures/opening_frame_timing.json port/godot/services/prologue_media_queue.gd port/godot/services/opening_audio_rules.gd port/godot/services/opening_audio_bridge.gd port/godot/app/first_arrival_boot.gd port/godot/flow/prologue_arrival.gd port/godot/rules/prologue_text_work.gd \
	"$SINK" port/godot/tests/route_receipts.gd port/godot/tests/fast_route/fast_route.gd port/godot/tests/run_installed_next_arrival_boundary.gd > "$OUT/hashes.txt"
printf 'mode %s\n' "$([ "$FAST" = 1 ] && echo fast || echo realtime)" >> "$OUT/hashes.txt"
exec docker run --rm --name "$NAME" --cpus=4 -e REI_DIALOGUE=1 -e REI_TARGET_STAGE=12 -e TIMED_ENTRY_PAD="$PAD" \
	-e TOKIMEMO_FAST_ROUTE="$FAST" -e GODOT_MODE_ARGS="$GODOT_MODE_ARGS" -e ROUTE_AUDIO_WAV="${ROUTE_AUDIO_WAV:-}" \
	-v "$ROOT:/work" -v "$ABS_OUT:/out" \
	-v "$ABS_SO:/work/port/godot/bin/liboriginal_audio.linux.template_release.arm64.so:ro" \
	-v "$ABS_OUT/candidate_bgm_bridge.gd:/work/port/godot/services/first_arrival_bgm_bridge.gd:ro" $FAST_MOUNT \
	tokimemo-port-gui-test sh -ec '
	Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp -ac >/out/xvfb.log 2>&1 &
	export DISPLAY=:99 GODOT_SILENCE_ROOT_WARNING=1; sleep 1
	if [ "$TOKIMEMO_FAST_ROUTE" != 1 ]; then
		pulseaudio -D --exit-idle-time=-1 --system=false 2>/dev/null || true
		pactl load-module module-null-sink sink_name=virtual >/dev/null 2>&1 || true
	fi
	status=0
	timeout 900 godot --path /work/port/godot $GODOT_MODE_ARGS --script res://tests/opening/run_opening_frame_timing_route.gd > /out/godot.log 2>&1 || status=$?
	grep -E "opening frame timing|^PASS|^FAIL|ERROR|SCRIPT ERROR" /out/godot.log | head -20
	exit $status'
