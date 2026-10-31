#!/bin/sh
# T-39314: DrawOTag through the engine's SDK layer against the emulator's VRAM (Docker). Needs the renderer
# GDExtension and the DrawOTag captures of T-39302 under <data root>/.asset-cache/m2-renderer (both outside Git).
#   CONTAINER_PREFIX=sdk sh tools/port/run_sdk_render_check.sh [--limit N]
# RENDER_SO: the library to mount (default: <data root>/.asset-cache/m2-renderer/build-v3/project/bin/...).
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
DATA=${TOKIMEMO_DATA_ROOT:-$(CDPATH= cd -- "$(git -C "$ROOT" rev-parse --git-common-dir)/.." && pwd)}
LIB=liboriginal_render.linux.template_release.arm64.so
SO=${RENDER_SO:-$DATA/.asset-cache/m2-renderer/build-v3/project/bin/$LIB}
[ -f "$SO" ] || { echo "missing renderer library $SO"; exit 2; }
trap '[ -s "$ROOT/port/godot/bin/$LIB" ] || rm -f "$ROOT/port/godot/bin/$LIB"' EXIT INT TERM
docker run --rm --name "${CONTAINER_PREFIX:-m2render}-$$" --cpus=4 \
  -v "$ROOT/port/godot:/workspace" \
  -v "$DATA/.asset-cache/m2-renderer:/captures:ro" \
  -v "$SO:/workspace/bin/$LIB:ro" \
  "${GODOT_PORT_IMAGE:-tokimemo-port}" \
  godot --headless --path /workspace --script res://tests/engine/sdk_render_check.gd -- --root /captures "$@"
