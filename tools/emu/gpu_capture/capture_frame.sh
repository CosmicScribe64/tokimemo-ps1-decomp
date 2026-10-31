#!/usr/bin/env bash
# T-39302: one frame, end to end. Captures DrawOTag call <k> (and call k-1) from a save state, then
#   1. compares the renderer with the emulator's VRAM after the call (real VRAM before), and
#   2. poisons both frame buffers of the state at the return of call k-1 with noise, lets the emulator
#      run the game on to the same DrawOTag over the noise, and compares again (coverage and blending
#      cannot hide behind unchanged pixels).
# usage: capture_frame.sh <name> <state (relative to the data root)> <k> [extra JOB fields, e.g. inputs={...}]
# Needs TOKIMEMO_DATA_ROOT (checkout with disc/ and .asset-cache) and the tokimemo-emu, tokimemo-recovery
# and tokimemo-content images. Output: <data root>/.asset-cache/m2-renderer/<name>/{real,noisy}.json
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
SRC=$(cd "$HERE/../../port/native_render" && pwd)
DATA=${TOKIMEMO_DATA_ROOT:?set TOKIMEMO_DATA_ROOT}
NAME=$1; STATE=$2; K=$3; EXTRA=${4:-}
OUT=$DATA/.asset-cache/m2-renderer/$NAME
REL=.asset-cache/m2-renderer/$NAME
FRAMES=$((K * 2 + 400))
export TOKIMEMO_DATA_ROOT
py() { docker run --rm -u "$(id -u):$(id -g)" -v "$HERE:/t:ro" -v "$DATA/.asset-cache/m2-renderer:/c" tokimemo-recovery python3 -I "$@"; }
cmp_tool() { # <dir> <tag> -> JSON on stdout
  docker run --rm --cpus=4 -v "$SRC:/src:ro" -v "$DATA/.asset-cache/m2-renderer:/m" tokimemo-content sh -c \
    "mkdir -p /tmp/b && g++ -std=c++17 -O2 -I/src /src/compare_capture.cpp -o /tmp/b/cmp && cd /m/$1 && /tmp/b/cmp --ram $2.ram.bin --before $2.vram_before.bin --after $2.vram_after.bin \$(cat $2.cmp.args) --env-addr 800DD5B8 --max-report 16"
}
mkdir -p "$DATA/.asset-cache/m2-renderer"
PICKS="$K"; [ "$K" -gt 1 ] && PICKS="$((K - 1)),$K"
"$HERE/run.sh" "$OUT/a" "$STATE" "picks={$PICKS}, frames=$FRAMES${EXTRA:+, $EXTRA}" >/dev/null
py /t/extract.py "/c/$NAME/a" >/dev/null
TAG=$(printf 'call%04d' "$K")
cmp_tool "$NAME/a" "$TAG" > "$OUT/real.json"
echo "real:  $(cut -c1-400 "$OUT/real.json")"
if [ "$K" -gt 1 ]; then
  PREV=$(printf 'call%04d' "$((K - 1))")
  py /t/poison.py "/c/$NAME/a/$PREV.after.sstate" "/c/$NAME/a/noisy.sstate" 0,0,319,479
  "$HERE/run.sh" "$OUT/b" "$REL/a/noisy.sstate" "picks={1}, frames=400" >/dev/null
  py /t/extract.py "/c/$NAME/b" >/dev/null
  cmp -s "$OUT/a/$TAG.ram.bin" "$OUT/b/call0001.ram.bin" && echo "ram: identical to the clean run" || echo "ram: DIFFERS from the clean run"
  cmp_tool "$NAME/b" call0001 > "$OUT/noisy.json"
  echo "noisy: $(cut -c1-400 "$OUT/noisy.json")"
fi
