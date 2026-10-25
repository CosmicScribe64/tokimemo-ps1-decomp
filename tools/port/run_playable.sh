#!/bin/sh
# T-29008: explicit external-input launcher. No originals are mounted in res://.
set -eu

if [ "$#" -lt 5 ] || [ "$#" -gt 6 ]; then
  echo "usage: $0 BOUNDARY_JSON BOUNDARY_SHA256 TABLES_JSON TABLES_SHA256 MEDIA_PACK [OUTPUT_DIRECTORY]" >&2
  echo "Set TOKIMEMO_PLAYABLE_SMOKE=1 for the finite scripted keyboard smoke." >&2
  exit 2
fi

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
PROJECT=${TOKIMEMO_PLAYABLE_PROJECT:-"$ROOT/port/godot"}
IMAGE=${GODOT_PORT_IMAGE:-tokimemo-port-foundation-candidate}
BOUNDARY=$(realpath "$1")
TABLES=$(realpath "$3")
MEDIA=$(realpath "$5")
test -f "$BOUNDARY"
test -f "$TABLES"
test -f "$MEDIA/manifest.json"
test -f "$PROJECT/app/playable.tscn"
OUTPUT=${6:-"$ROOT/.asset-cache/port-captures"}
mkdir -p "$OUTPUT"
OUTPUT=$(realpath "$OUTPUT")

# Read-only inputs, writable ignored captures, Xvfb + llvmpipe real rendering.
# The container is visible through its captures. Interactive launchers may expose
# this X display using their own existing desktop transport.
docker run --rm --cpus=2 \
  -v "$PROJECT:/workspace:ro" \
  -v "$BOUNDARY:/inputs/boundary.json:ro" \
  -v "$TABLES:/inputs/tables.json:ro" \
  -v "$MEDIA:/inputs/media:ro" \
  -v "$OUTPUT:/out" \
  -e TOKIMEMO_BOUNDARY=/inputs/boundary.json -e TOKIMEMO_BOUNDARY_SHA="$2" \
  -e TOKIMEMO_TABLES=/inputs/tables.json -e TOKIMEMO_TABLES_SHA="$4" \
  -e TOKIMEMO_MEDIA_PACK=/inputs/media -e TOKIMEMO_PLAYABLE_OUTPUT=/out \
  -e TOKIMEMO_PLAYABLE_SMOKE="${TOKIMEMO_PLAYABLE_SMOKE:-0}" \
  -e TOKIMEMO_PLAYABLE_RECORD="${TOKIMEMO_PLAYABLE_RECORD:-0}" \
  -e TOKIMEMO_PLAYABLE_SMOKE_COMMAND="${TOKIMEMO_PLAYABLE_SMOKE_COMMAND:-0}" \
  -e TOKIMEMO_PLAYABLE_FIRST_RESULT_ONLY="${TOKIMEMO_PLAYABLE_FIRST_RESULT_ONLY:-0}" \
  "$IMAGE" sh -ec '
    Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp -ac >/tmp/xvfb.log 2>&1 &
    xvfb_pid=$!; trap "kill $xvfb_pid 2>/dev/null || true" EXIT
    sleep 1
    # Four activities plus per-day PNG captures need a larger finite wall-clock
    # budget than the earlier first-result slice. Logic still has its own bound.
    if [ "$TOKIMEMO_PLAYABLE_SMOKE" = 1 ]; then runner="timeout 90s"; else runner=""; fi
    if [ "$TOKIMEMO_PLAYABLE_RECORD" = 1 ]; then capture_clock="--fixed-fps 60"; else capture_clock=""; fi
    DISPLAY=:99 LIBGL_ALWAYS_SOFTWARE=1 GALLIUM_DRIVER=llvmpipe \
      $runner godot $capture_clock --max-fps 60 --rendering-method gl_compatibility --display-driver x11 \
        --audio-driver Dummy --path /workspace res://app/playable.tscn
  '
