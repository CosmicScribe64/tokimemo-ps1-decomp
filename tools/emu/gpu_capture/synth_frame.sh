#!/usr/bin/env bash
# T-39302: the synthetic ordering table (synth_ot.py) drawn by the emulator over the real VRAM of a save
# state at DrawOTag call <k>, then drawn by the renderer from the same RAM bytes, compared pixel for pixel.
# usage: [SYNTH_GROUPS=polygons,lines,...] synth_frame.sh <name> <state (relative to the data root)> <k>
# Output: <data root>/.asset-cache/m2-renderer/<name>/{synth.json,a/...}. Same requirements as capture_frame.sh.
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
SRC=$(cd "$HERE/../../port/native_render" && pwd)
DATA=${TOKIMEMO_DATA_ROOT:?set TOKIMEMO_DATA_ROOT}
NAME=$1; STATE=$2; K=$3
OUT=$DATA/.asset-cache/m2-renderer/$NAME
export TOKIMEMO_DATA_ROOT
mkdir -p "$OUT"
py() { docker run --rm -u "$(id -u):$(id -g)" -v "$HERE:/t:ro" -v "$DATA/.asset-cache/m2-renderer:/c" tokimemo-recovery python3 -I "$@"; }
py /t/synth_ot.py "/c/$NAME/synth.bin" "/c/$NAME/inject.lua" 8014E000 "${SYNTH_GROUPS:-polygons,semi,textured,lines,rects,sprites,flips,window_wrap,clip,mask,transfers}"
"$HERE/run.sh" "$OUT/a" "$STATE" "picks={$K}, frames=$((K + 400))" "$OUT/inject.lua" >/dev/null
py /t/extract.py "/c/$NAME/a" >/dev/null
TAG=$(printf 'call%04d' "$K")
docker run --rm --cpus=4 -v "$SRC:/src:ro" -v "$DATA/.asset-cache/m2-renderer:/m" tokimemo-content sh -c \
  "mkdir -p /tmp/b && g++ -std=c++17 -O2 -I/src /src/compare_capture.cpp -o /tmp/b/cmp && cd /m/$NAME/a && /tmp/b/cmp --ram $TAG.ram.bin --before $TAG.vram_before.bin --after $TAG.vram_after.bin \$(cat $TAG.cmp.args) --max-report 24" > "$OUT/synth.json"
cut -c1-600 "$OUT/synth.json"
