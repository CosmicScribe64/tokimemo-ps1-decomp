#!/bin/sh
# T-39302: PS1 renderer tests, Docker only.
#   1. C++ unit tests of the GPU model (UBSan + ASan) in the compiler image.
#   2. Ps1Renderer + the built GDExtension under headless Godot (tokimemo-port), with the library
#      mounted read-only at res://bin/.
# usage: tools/port/run_render_tests.sh <library.so>   (or set RENDER_SO)
#   Build one first: tools/port/native_render/build.sh --output .asset-cache/m2-renderer/<new dir>
#   and pass <new dir>/project/bin/liboriginal_render.linux.template_release.arm64.so.
set -eu
cd "$(dirname "$0")/../.."
ROOT=$PWD
SO=${1:-${RENDER_SO:-}}
[ -n "$SO" ] || { echo "usage: $0 <liboriginal_render...so> (see the header of this script)"; exit 2; }
[ -f "$SO" ] || { echo "missing $SO"; exit 2; }
case $SO in /*) ABS_SO=$SO ;; *) ABS_SO=$ROOT/$SO ;; esac
bash tools/port/native_render/build.sh --test
MOUNT=$ROOT/port/godot/bin/liboriginal_render.linux.template_release.arm64.so
# Docker leaves an empty mount-point file in the source tree; the library itself is never installed there.
cleanup() { [ -s "$MOUNT" ] || rm -f "$MOUNT"; }
trap cleanup EXIT INT TERM
status=0
docker run --rm --cpus=4 -v "$ROOT/port/godot:/workspace" \
	-v "$ABS_SO:/workspace/bin/liboriginal_render.linux.template_release.arm64.so:ro" \
	tokimemo-port godot --headless --path /workspace --script res://tests/render/run_ps1_renderer_tests.gd || status=$?
exit $status
