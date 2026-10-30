#!/bin/sh
# T-39060 visual QA screens (Docker only; X11 window captures, never a parity gate).
# usage: tools/port/run_visual_qa.sh <new-out-dir> <bundle-with-language-en>
# Runs tests/qa/run_visual_qa_screens.gd under Xvfb (1920x1200) in
# tokimemo-port-gui-test: the Welcome step, every settings page before and
# inside a game scene, and the English-mode name entry (Latin typing), at
# 1280x720, 1920x1080 and 960x720 in original and wide aspect. Captures land
# in <out>/qa. The bundle is copied, never modified. The opening dialogue and
# command-select captures come from the route:
#   ROUTE_OBSERVER=modern VISUAL_QA=1 ROUTE_SCREEN=1920x1200x24 \
#     ROUTE_PREFS='{"language.bundle":"en"}' sh tools/port/run_installed_route.sh <out> ...
set -eu
cd "$(dirname "$0")/../.."
ROOT=$PWD
[ $# -eq 2 ] || { sed -n '2,12p' "$0"; exit 2; }
OUT=$1; BUNDLE=$2
[ ! -e "$OUT" ] || { echo "output exists: $OUT" >&2; exit 2; }
[ -f "$BUNDLE/manifest.json" ] && [ -f "$BUNDLE/language/en/english_bundle.json" ] || { echo "bundle needs manifest.json and language/en: $BUNDLE" >&2; exit 2; }
mkdir -p "$OUT/content-install/.staging"
cp -R "$BUNDLE" "$OUT/content-install/.staging/prepared-original"
case $OUT in /*) ABS_OUT=$OUT ;; *) ABS_OUT=$ROOT/$OUT ;; esac
exec docker run --rm --name "tokimemo-visual-qa-$(basename "$OUT" | tr -c 'A-Za-z0-9_.\n-' '-')" --cpus=4 \
	-e GODOT_SILENCE_ROOT_WARNING=1 -v "$ROOT:/work" -v "$ABS_OUT:/out" "${GODOT_GUI_TEST_IMAGE:-tokimemo-port-gui-test}" sh -ec '
	Xvfb :99 -screen 0 1920x1200x24 -nolisten tcp -ac >/out/xvfb.log 2>&1 &
	export DISPLAY=:99; sleep 1
	status=0
	timeout 900 godot --path /work/port/godot --rendering-method gl_compatibility --audio-driver Dummy --script res://tests/qa/run_visual_qa_screens.gd > /out/godot.log 2>&1 || status=$?
	grep -E "^PASS|^FAIL|SCRIPT ERROR|QA:" /out/godot.log | head -20
	exit $status'
