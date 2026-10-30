#!/bin/sh
# T-39022 (T-39018 WP-05): opening per-iteration state/RNG trace differential (Docker only).
# usage: tools/port/run_opening_trace_differential.sh <new-out-dir> [pad]
#   <new-out-dir> must be inside this checkout (normally under .asset-cache/);
#   pad is 2 (START at vsync 20, fcb-start20) or 3 (vsync 33), as in opening_frame_timing.json.
# Steps:
#   1. native: tools/emu/run.sh (tokimemo-emu) from name-shiori-confirm-v1.sstate, START at the
#      pad's vsync, --sample-every 1 with the dumps of config/port/opening-trace-projection.json
#      and an exec breakpoint on rand (0x800AE0D0) recording every hit, in two 3600-vsync parts
#      (the second starts from the first one's final save state);
#   2. port: tests/opening/run_opening_trace_emitter.gd under Xvfb (tokimemo-port-gui-test), the
#      timed opening route with the same pad;
#   3. tools/port/opening_trace.py (tokimemo-recovery) projects both sides and reports the first
#      divergent iteration through compare_trace.py.
# Inputs come from the user's own disc and stay under .asset-cache (override with NATIVE_STATE,
# OPENING_TIMING_BUNDLE, OPENING_TIMING_SO, OPENING_TIMING_BRIDGE). Exit status is step 3's:
# 0 equal, 1 divergence reported, 2 invalid input. SKIP_NATIVE=1 or SKIP_PORT=1 reuse a step's
# existing output in <new-out-dir> (for reruns of the comparison only).
set -eu
cd "$(dirname "$0")/../.."
ROOT=$PWD; OUT=$1; PAD=${2:-2}
case $PAD in 2) START=20 ;; 3) START=33 ;; *) echo "pad must be 2 or 3"; exit 2 ;; esac
case $OUT in /*) echo "use a path inside the checkout"; exit 2 ;; esac
STATE=${NATIVE_STATE:-.asset-cache/emu/newgame-golden-v1/inputs/name-shiori-confirm-v1.sstate}
[ -f "$STATE" ] || { echo "missing $STATE"; exit 2; }
NAME=tokimemo-opening-trace-$(basename "$OUT" | tr -c 'A-Za-z0-9_.\n-' '-')
if [ "${SKIP_NATIVE:-0}" != 1 ]; then
	[ ! -e "$OUT/native" ] || { echo "$OUT/native exists"; exit 2; }
	mkdir -p "$OUT"
	# emu.py keeps at most 4096 samples per run: two consecutive parts, joined by frame_count.
	DUMPS="--dump 800E7374:32 --dump 8014D180:12 --dump 801220D0:20 --dump 80120650:72"
	tools/emu/run.sh break 0x800AE0D0 --hits 400000 --no-stop --frames 3600 --sample-every 1 $DUMPS \
		--state "$STATE" --input "$START:START/2" --save-state "$OUT/native/part1.sstate" --out "$OUT/native/part1" --timeout 7200
	tools/emu/run.sh break 0x800AE0D0 --hits 400000 --no-stop --frames 3600 --sample-every 1 $DUMPS \
		--state "$OUT/native/part1.sstate" --out "$OUT/native/part2" --timeout 7200
fi
if [ "${SKIP_PORT:-0}" != 1 ]; then
	[ ! -e "$OUT/port" ] || { echo "$OUT/port exists"; exit 2; }
	SO=${OPENING_TIMING_SO:-.asset-cache/port/native-audio-shiori-v2/project/bin/liboriginal_audio.linux.template_release.arm64.so}
	BRIDGE=${OPENING_TIMING_BRIDGE:-.asset-cache/port/opening-hold-bank-bridge-v1/candidate_bgm_bridge.gd}
	BUNDLE=${OPENING_TIMING_BUNDLE:-.asset-cache/port/opening-line17-runtime-v3}
	for f in "$SO" "$BRIDGE" "$BUNDLE/manifest.json"; do [ -e "$f" ] || { echo "missing $f"; exit 2; }; done
	mkdir -p "$OUT/port/content-install/.staging"
	cp -R "$BUNDLE" "$OUT/port/content-install/.staging/prepared-original"
	cp "$BRIDGE" "$OUT/port/candidate_bgm_bridge.gd"
	shasum -a 256 "$SO" "$BRIDGE" "$BUNDLE/manifest.json" config/port/opening-trace-projection.json \
		port/godot/tests/opening/run_opening_trace_emitter.gd port/godot/tests/opening/run_opening_frame_timing_route.gd \
		port/godot/services/first_arrival_media_bridge.gd port/godot/flow/prologue_arrival.gd > "$OUT/port/hashes.txt"
	docker run --rm --name "$NAME-port" --cpus=4 -e REI_DIALOGUE=1 -e REI_TARGET_STAGE=12 -e TIMED_ENTRY_PAD="$PAD" \
		-v "$ROOT:/work" -v "$ROOT/$OUT/port:/out" \
		-v "$ROOT/$SO:/work/port/godot/bin/liboriginal_audio.linux.template_release.arm64.so:ro" \
		-v "$ROOT/$OUT/port/candidate_bgm_bridge.gd:/work/port/godot/services/first_arrival_bgm_bridge.gd:ro" \
		tokimemo-port-gui-test sh -ec '
		Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp -ac >/out/xvfb.log 2>&1 &
		export DISPLAY=:99 GODOT_SILENCE_ROOT_WARNING=1; sleep 1
		pulseaudio -D --exit-idle-time=-1 --system=false 2>/dev/null || true
		pactl load-module module-null-sink sink_name=virtual >/dev/null 2>&1 || true
		timeout 900 godot --path /work/port/godot --audio-driver PulseAudio --rendering-method gl_compatibility --script res://tests/opening/run_opening_trace_emitter.gd > /out/godot.log 2>&1 || true
		grep -E "opening frame timing|^PASS|^FAIL|SCRIPT ERROR" /out/godot.log | head -10'
fi
exec docker run --rm --name "$NAME-compare" -e PYTHONDONTWRITEBYTECODE=1 -v "$ROOT:/work" -w /work tokimemo-recovery \
	python3 tools/port/opening_trace.py --config config/port/opening-trace-projection.json \
	--native-dir "$OUT/native/part1" --native-dir "$OUT/native/part2" --port-trace "$OUT/port/opening-trace.port.jsonl" --out "$OUT/compare"
