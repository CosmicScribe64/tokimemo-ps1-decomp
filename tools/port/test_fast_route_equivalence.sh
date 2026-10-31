#!/bin/sh
# FAST vs real-time route equivalence test (Docker only).
# usage: tools/port/test_fast_route_equivalence.sh <new-out-root> [pad]
# Runs the opening frame-timing route (tools/port/run_opening_frame_timing.sh,
# TIMED_ENTRY_PAD=<pad>, default 2: scripted entry, unskipped opening through the
# Rei start and the logic-locked audio clock to 12/0/0, about 6800 logic frames)
# once in real time and once with --fast, then compares the receipts with
# tools/port/compare_route_modes.py: every published frame's tick,
# frame_count, substep and native image hash, the per-substep frame table,
# transitions, rei-proof logic fields and every native PNG must be identical,
# and the FAST run must show one native block per logic frame from the Rei
# start, no underrun and a race FIFO of at most one block. Native PCM is
# reported, not required: real-time pre-Rei audio follows the wall-clock
# device (see compare_route_modes.py); FAST PCM determinism is checked with
# --audio require-all on two FAST runs. Exits non-zero when either route fails
# or the two modes diverge.
# Inputs are the timing route's (OPENING_TIMING_BUNDLE/_SO/_BRIDGE).
set -eu
cd "$(dirname "$0")/../.."
ROOT=$PWD
OUT=$1; PAD=${2:-2}
[ ! -e "$OUT" ] || { echo "$OUT exists"; exit 2; }
mkdir -p "$OUT"
case $OUT in /*) ABS_OUT=$OUT ;; *) ABS_OUT=$ROOT/$OUT ;; esac
status=0
sh tools/port/run_opening_frame_timing.sh "$OUT/realtime" "$PAD" || status=$?
[ "$status" = 0 ] || { echo "real-time route failed ($status)"; exit 1; }
sh tools/port/run_opening_frame_timing.sh --fast "$OUT/fast" "$PAD" || status=$?
[ "$status" = 0 ] || { echo "fast route failed ($status)"; exit 1; }
exec docker run --rm --name "tokimemo-fast-route-compare-$(basename "$OUT" | tr -c 'A-Za-z0-9_.\n-' '-')" \
	-v "$ROOT:/work:ro" -v "$ABS_OUT:/out" -w /work tokimemo-port-gui-test \
	python3 -I tools/port/compare_route_modes.py /out/realtime /out/fast --audio report --json /out/compare.json
