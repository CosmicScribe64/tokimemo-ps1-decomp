#!/bin/sh
# T-39007: save -> quit -> relaunch -> Continue, recovery and clear-save
# persistence. Every phase is a separate Godot process in its own container;
# phases share only USER_DATA and PROOF on the host. The read-only phase
# mounts USER_DATA read-only to produce a real write failure.
set -eu

if [ "$#" -ne 3 ]; then
  echo "usage: $0 NEW_OUTPUT_DIRECTORY CONTENT_INSTALL_ROOT MEDIA_PACK" >&2
  exit 2
fi
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd -P)
PROJECT=${TOKIMEMO_PLAYABLE_PROJECT:-"$ROOT/port/godot"}
IMAGE=${GODOT_PORT_IMAGE:-tokimemo-port}
if [ -e "$1" ]; then echo "output directory must be new: $1" >&2; exit 2; fi
mkdir -p "$1/userdata" "$1/proof"
OUTPUT=$(realpath "$1")
CONTENT=$(realpath "$2")
MEDIA=$(realpath "$3")
test -f "$MEDIA/manifest.json"
# Identity of the reviewed captured development inputs (app/playable_boot.gd).
BOUNDARY_SHA=cddc10f6e6ff93ff9f667fb2c8ed6d3e1ab42d09ec6c0ee0e7f2317750b03ce5
# Rule-table identity: the one constant in services/gameplay_content_identity.gd.
TABLES_SHA=$(sed -n 's/^const RULE_TABLES_SHA := "\([0-9a-f]\{64\}\)"$/\1/p' "$ROOT/port/godot/services/gameplay_content_identity.gd")
[ "${#TABLES_SHA}" -eq 64 ] || { echo "rule-table identity not found in gameplay_content_identity.gd" >&2; exit 2; }

# run_phase PHASE rw|ro headless|x11
run_phase() {
  phase=$1; mode=$2; display=$3
  echo "== phase $phase"
  docker run --rm --name "tokimemo-save-relaunch-$phase-$$" \
    -v "$PROJECT:/workspace" \
    -v "$CONTENT:/content:ro" -v "$MEDIA:/media:ro" \
    -v "$OUTPUT/userdata:/userdata:$mode" -e XDG_DATA_HOME=/userdata \
    -v "$OUTPUT/proof:/proof" -e T39007_PROOF=/proof -e T39007_PHASE="$phase" -e DISPLAY_MODE="$display" \
    -e TOKIMEMO_MEDIA_PACK=/media -e TOKIMEMO_BOUNDARY_SHA="$BOUNDARY_SHA" -e TOKIMEMO_TABLES_SHA="$TABLES_SHA" \
    "$IMAGE" sh -ec '
      script=res://tests/services/run_save_relaunch.gd
      if [ "$DISPLAY_MODE" = x11 ]; then
        Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp -ac >/tmp/xvfb.log 2>&1 &
        sleep 1
        DISPLAY=:99 LIBGL_ALWAYS_SOFTWARE=1 GALLIUM_DRIVER=llvmpipe timeout 120s godot --rendering-method gl_compatibility \
          --display-driver x11 --audio-driver Dummy --path /workspace --script "$script"
      else
        timeout 120s godot --headless --path /workspace --script "$script"
      fi' 2>&1 | grep -E '^(PASS|FAIL)' || true
  grep -q '"failures": \[\]' "$OUTPUT/proof/phase-$phase.json"
}

run_phase write rw headless
run_phase continue rw headless
run_phase corrupt rw headless
run_phase recover rw headless
run_phase keyboard rw x11
run_phase readonly ro headless
run_phase retry rw headless
echo "PASS: all save relaunch phases"
