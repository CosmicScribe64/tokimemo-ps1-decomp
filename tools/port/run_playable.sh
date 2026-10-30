#!/bin/sh
# T-29008: explicit external-input launcher. No originals are mounted in res://.
set -eu

if [ "$#" -lt 5 ] || [ "$#" -gt 6 ]; then
  echo "usage: $0 BOUNDARY_JSON BOUNDARY_SHA256 TABLES_JSON TABLES_SHA256 MEDIA_PACK [OUTPUT_DIRECTORY]" >&2
  echo "Set TOKIMEMO_PLAYABLE_SMOKE=1 for the finite scripted keyboard smoke." >&2
  exit 2
fi

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd -P)
PROJECT=${TOKIMEMO_PLAYABLE_PROJECT:-"$ROOT/port/godot"}
IMAGE=${GODOT_PORT_IMAGE:-tokimemo-port}
BOUNDARY_SHA=$2
TABLES_SHA=$4
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
if [ -n "${TOKIMEMO_PLAYABLE_USER_DATA:-}" ]; then
  test -d "$TOKIMEMO_PLAYABLE_USER_DATA"
  USER_DATA=$(realpath "$TOKIMEMO_PLAYABLE_USER_DATA")
else
  USER_DATA="$(realpath "$ROOT/.asset-cache")/port-user-data"
  if [ -e "$USER_DATA" ]; then USER_DATA=$(realpath "$USER_DATA"); fi
fi
PROJECT=$(realpath "$PROJECT")
case "$USER_DATA" in
  "$PROJECT"|"$PROJECT"/*) echo "user data must be outside the Godot project" >&2; exit 2 ;;
  "$ROOT"|"$ROOT"/*)
    case "$USER_DATA" in
      "$ROOT/.asset-cache/"*) ;;
      *) echo "repository user data must be under ignored .asset-cache/" >&2; exit 2 ;;
    esac ;;
esac
mkdir -p "$USER_DATA"

set --
if [ -n "${TOKIMEMO_HUD_MANIFEST:-}" ]; then
  test -f "$TOKIMEMO_HUD_MANIFEST"
  HUD_DIR=$(realpath "$(dirname "$TOKIMEMO_HUD_MANIFEST")")
  HUD_FILE=$(basename "$TOKIMEMO_HUD_MANIFEST")
  set -- "$@" -v "$HUD_DIR:/inputs/hud:ro" \
    -e "TOKIMEMO_HUD_MANIFEST=/inputs/hud/$HUD_FILE" -e "TOKIMEMO_HUD_SHA=${TOKIMEMO_HUD_SHA:-}"
fi
if [ -n "${TOKIMEMO_FONT_ROOT:-}" ]; then
  test -d "$TOKIMEMO_FONT_ROOT"
  FONT_DIR=$(realpath "$TOKIMEMO_FONT_ROOT")
  set -- "$@" -v "$FONT_DIR:/runtime/.asset-cache/port/fonts/native-font-v2:ro" \
    -e TOKIMEMO_FONT_ROOT=/runtime/.asset-cache/port/fonts/native-font-v2
fi
if [ -n "${TOKIMEMO_HUD_CONTEXT:-}" ]; then
  test -f "$TOKIMEMO_HUD_CONTEXT"
  CTX_FILE=$(realpath "$TOKIMEMO_HUD_CONTEXT")
  set -- "$@" -v "$CTX_FILE:/inputs/hud-context.json:ro" \
    -e TOKIMEMO_HUD_CONTEXT=/inputs/hud-context.json -e "TOKIMEMO_HUD_CONTEXT_SHA=${TOKIMEMO_HUD_CONTEXT_SHA:-}"
fi
if [ -n "${TOKIMEMO_REQUIRE_HUD:-}" ]; then
  set -- "$@" -e "TOKIMEMO_REQUIRE_HUD=$TOKIMEMO_REQUIRE_HUD"
fi

# Read-only inputs, writable ignored captures, Xvfb + llvmpipe real rendering.
# The container is visible through its captures. Interactive launchers may expose
# this X display using their own existing desktop transport.
docker run --rm --cpus=2 \
  -v "$PROJECT:/workspace:ro" \
  -v "$BOUNDARY:/inputs/boundary.json:ro" \
  -v "$TABLES:/inputs/tables.json:ro" \
  -v "$MEDIA:/inputs/media:ro" \
  -v "$OUTPUT:/out" \
  -v "$USER_DATA:/userdata" -e XDG_DATA_HOME=/userdata \
  "$@" \
  -e TOKIMEMO_BOUNDARY=/inputs/boundary.json -e TOKIMEMO_BOUNDARY_SHA="$BOUNDARY_SHA" \
  -e TOKIMEMO_TABLES=/inputs/tables.json -e TOKIMEMO_TABLES_SHA="$TABLES_SHA" \
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
