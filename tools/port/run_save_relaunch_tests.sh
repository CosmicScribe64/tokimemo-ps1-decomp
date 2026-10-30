#!/bin/sh
# T-39007: save -> quit -> relaunch -> Continue, recovery and clear-save
# persistence. Every phase is a separate Godot process in its own container;
# phases share only USER_DATA and PROOF on the host. The read-only phase
# mounts USER_DATA read-only to produce a real write failure.
set -eu

# WP-10 (T-39007): --settings-matrix NEW_OUTPUT_DIRECTORY runs only the
# preferences matrix and settings-file recovery phases. They need no original
# content: their saves are invented codec fixtures. Each phase is a separate
# headless Godot process in its own container sharing only user data and proof.
if [ "${1:-}" = "--settings-matrix" ]; then
  if [ "$#" -ne 2 ]; then echo "usage: $0 --settings-matrix NEW_OUTPUT_DIRECTORY" >&2; exit 2; fi
  ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd -P)
  PROJECT=${TOKIMEMO_PLAYABLE_PROJECT:-"$ROOT/port/godot"}
  IMAGE=${GODOT_PORT_IMAGE:-tokimemo-port}
  if [ -e "$2" ]; then echo "output directory must be new: $2" >&2; exit 2; fi
  mkdir -p "$2/recovery-userdata" "$2/matrix-userdata" "$2/proof"
  OUTPUT=$(realpath "$2")
  # settings_phase SCRIPT USERDATA ENV... ; prints PASS/FAIL/XFAIL lines.
  settings_phase() {
    script=$1; userdata=$2; shift 2
    set -- "$@"
    envs=""
    for pair in "$@"; do envs="$envs -e $pair"; done
    echo "== $script $*"
    # shellcheck disable=SC2086
    docker run --rm --name "tokimemo-settings-matrix-$$-$(date +%s%N | tail -c 7)" \
      -v "$PROJECT:/workspace" -v "$OUTPUT/$userdata:/userdata" -e XDG_DATA_HOME=/userdata \
      -v "$OUTPUT/proof:/proof" -e SETTINGS_PROOF=/proof -e MATRIX_PROOF=/proof $envs \
      "$IMAGE" sh -ec "timeout 120s godot --headless --path /workspace --script res://$script" 2>&1 \
      | grep -E '^(PASS|FAIL|XFAIL)' || true
  }
  recovery() {
    settings_phase tests/services/run_settings_file_recovery.gd recovery-userdata "SETTINGS_PHASE=$1"
    grep -q '"failures": \[\]' "$OUTPUT/proof/settings-$1.json"
  }
  matrix() {
    settings_phase tests/app/run_save_relaunch_settings_matrix.gd matrix-userdata "MATRIX_PHASE=$1" "MATRIX_CELL=$2" "MATRIX_NEXT_CELL=$3"
    label=$1; if [ -n "$2" ]; then label="$1-$(echo "$2" | tr , -)"; fi
    grep -q '"failures": \[\]' "$OUTPUT/proof/matrix-$label.json"
  }
  for phase in seed corrupt missing newer newer-with-backup pending-kill pending-boot; do recovery "$phase"; done
  # Expected failures are ticketed product defects (strict: an XPASS is a failure
  # above). They keep the driver from reporting PASS.
  xfail_phases=""
  for proof in "$OUTPUT"/proof/settings-*.json; do
    grep -q '"xfail": \[\]' "$proof" || xfail_phases="$xfail_phases $(basename "$proof" .json)"
  done
  grep -q '"killed": true' "$OUTPUT/proof/settings-pending-kill.json"
  matrix write "" en,wide,linear
  matrix relaunch en,wide,linear ja,auto,nearest
  matrix relaunch ja,auto,nearest en,original,nearest
  matrix relaunch en,original,nearest ja,wide,integer
  matrix relaunch ja,wide,integer en,auto,linear
  matrix relaunch en,auto,linear ja,original,linear
  matrix relaunch ja,original,linear en,wide,nearest
  matrix relaunch en,wide,nearest ja,auto,integer
  matrix relaunch ja,auto,integer en,original,integer
  matrix continue-resave en,original,integer ja,original,integer
  matrix continue-resave ja,original,integer en,original,integer
  matrix clear-write en,original,integer en,wide,linear
  matrix clear-verify en,wide,linear ""
  if [ -n "$xfail_phases" ]; then
    echo "XFAIL: settings matrix and settings-file recovery phases; every other check passes; expected failures in:$xfail_phases (see XFAIL lines and tickets)"
    exit 3
  fi
  echo "PASS: settings matrix and settings-file recovery phases"
  exit 0
fi

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
