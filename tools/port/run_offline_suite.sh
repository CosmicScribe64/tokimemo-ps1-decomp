#!/bin/sh
# T-39028: original-free port suite (T0). Original-free; not fidelity evidence.
#
#   tools/port/run_offline_suite.sh            run in Docker (tokimemo-recovery, repo read-only)
#   tools/port/run_offline_suite.sh --inside   run with the current python3 (used inside that
#                                              container and by .github/workflows/port-offline.yml)
#
# Steps, all without game data:
#   1. Python unittests for the original-free tools/port and tools/emu modules listed below;
#   2. JSON Schema validation of the committed checkpoint catalog, input scripts and
#      receipts that have a schema (a pair whose files are not in the tree yet is reported
#      UNAVAILABLE, never passed);
#   3. translation-pack and translated-artwork lint over translations/en
#      (tools/port/lint_translation_assets.py).
# Godot headless parse and the synthetic GDScript suites are not part of this job until the
# user approves building or pulling a Godot image in CI (wiki/ci.md).
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd -P)

if [ "${1:-}" != "--inside" ]; then
  IMAGE=${TOKIMEMO_OFFLINE_IMAGE:-tokimemo-recovery}
  exec docker run --rm --name "tokimemo-port-offline-$$" -e PYTHONDONTWRITEBYTECODE=1 \
    -v "$ROOT:/repo:ro" -w /repo --entrypoint /bin/sh "$IMAGE" tools/port/run_offline_suite.sh --inside
fi

cd "$ROOT"
export PYTHONDONTWRITEBYTECODE=1
echo "port-offline: original-free; not fidelity evidence ($(python3 --version 2>&1))"

# Original-free unittest modules. Excluded, because they need original data, a
# TOKIMEMO_* source root or modules from the tokimemo-content image:
# test_content, test_first_arrival_content, test_font_content, test_prologue_arrival_media,
# test_prologue_hud_content, test_prologue_runtime_content, test_release_catalog. The
# tools/port/progress suite is excluded until its 6 pre-existing errors are triaged.
MODULES="
tools.port.test_build_installed_runtime
tools.port.test_build_text_pack
tools.port.test_club_command_content
tools.port.test_compare_capture_pair
tools.port.test_contact_sheet
tools.port.test_differential
tools.port.test_english_lint
tools.port.test_extract_decomp_tables
tools.port.test_first_arrival_bgm_content
tools.port.test_first_arrival_board_content
tools.port.test_frame_pacing_report
tools.port.test_holiday_media
tools.port.test_holiday_scene_content
tools.port.test_hud_content
tools.port.test_lint_translation_assets
tools.port.test_media_cli
tools.port.test_native_memory_projection
tools.port.test_opening_voice_content
tools.port.test_preflight
tools.port.test_prepare_desktop
tools.port.test_readiness
tools.port.test_rei_stage_capture_pairs
tools.port.test_rgb555_diagnostic
tools.port.test_school_day_media
tools.port.test_school_day_media_fresh
tools.port.test_seasonal_command_content
tools.port.test_seasonal_content_integrity
tools.port.test_spring_command_content
tools.port.test_verify_tile_diff
tools.port.test_weekday_56_content
tools.emu.test_capture_ranges
tools.emu.test_emu
tools.emu.test_screenshot_evidence
"
status=0
echo "== unittests"
# shellcheck disable=SC2086
python3 -m unittest $MODULES >/tmp/port-offline-unittest.log 2>&1 || status=1
grep -E "^(Ran|OK|FAILED)" /tmp/port-offline-unittest.log || true
if [ "$status" -ne 0 ]; then grep -E "^(FAIL|ERROR):" /tmp/port-offline-unittest.log | head -40; fi

echo "== schema validation"
# schema|instance-glob pairs; the catalog and input-script schemas come from WP-01/WP-06.
for pair in \
  "tools/port/schemas/playtest-checkpoints.v1.schema.json|config/port/playtest-checkpoints.json" \
  "tools/port/schemas/input-script.v1.schema.json|config/port/input-scripts/*.json" \
  "tools/port/schemas/checkpoint-receipt.v2.schema.json|tools/port/tests/fixtures/checkpoint_receipt/*.json"; do
  schema=${pair%%|*}; pattern=${pair#*|}
  # shellcheck disable=SC2086
  set -- $pattern
  if [ ! -f "$schema" ] || [ ! -f "$1" ]; then
    echo "UNAVAILABLE $schema ($pattern not in the tree yet)"
    continue
  fi
  python3 tools/port/schema_check.py "$schema" "$@" || status=1
done
python3 tools/port/schema_check.py tools/port/schemas/visual-review.v1.schema.json /dev/stdin <<'EOF' || status=1
{"schema": "tokimemo.visual-review.v1", "queue_id": "offline-suite", "created_utc": "2026-10-29T00:00:00Z", "items": [], "deferred": []}
EOF

echo "== translation pack and artwork lint"
python3 tools/port/lint_translation_assets.py || status=1

if [ "$status" -eq 0 ]; then echo "PASS: port-offline suite (original-free; not fidelity evidence)"; else echo "FAIL: port-offline suite"; fi
exit "$status"
