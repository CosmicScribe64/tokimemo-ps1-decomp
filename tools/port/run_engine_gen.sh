#!/bin/sh
# T-39312: regenerate (or --check) the engine modules that are translated from the C by tools/port/engine_gen/cgen.py.
#   tools/port/run_engine_gen.sh                    write port/godot/engine/{hud_panels,scene_display,scene_library}.gd
#   tools/port/run_engine_gen.sh --check            fail if a committed module differs from the generated one
#   tools/port/run_engine_gen.sh --report-dir /work/build/engine-gen     also write <unit>.json (order, callees) for the two tools below
#   tools/port/run_engine_gen.sh mkspec --report-dir /work/build/engine-gen --captures-root /captures <unit> cap1,cap2   write config/port/engine-diff/<unit>.json
#   tools/port/run_engine_gen.sh capture_cmd --report-dir /work/build/engine-gen <name> <state> <frames> <hits> ALL   print the run.sh command of a capture
# The rodata strings of the C files come from asm/data/main/<unit>.rodata.s of the user's build (asm/ is not committed):
# TOKIMEMO_DATA_ROOT is the checkout that holds asm/ (default: the main checkout of this repo).
# Docker only (image tokimemo-engine-gen from tools/Dockerfile.engine_gen), container m2gen-$$, --cpus=4.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
DATA=${TOKIMEMO_DATA_ROOT:-$(CDPATH= cd -- "$(git -C "$ROOT" rev-parse --git-common-dir)/.." && pwd)}
IMG=tokimemo-engine-gen
docker image inspect "$IMG" >/dev/null 2>&1 || docker build -q -t "$IMG" -f "$ROOT/tools/Dockerfile.engine_gen" "$ROOT/tools" >&2
TOOL=cgen.py
TOOL_ARGS="--repo /work --asm /asm"
case "${1:-}" in
  mkspec|capture_cmd) TOOL=$1.py; TOOL_ARGS=; shift ;;
esac
docker run --rm --name "m2gen-$$" --cpus=4 -u "$(id -u):$(id -g)" -e HOME=/tmp \
  -v "$ROOT":/work -v "$DATA/asm":/asm:ro -v "$DATA/.asset-cache/m2-engine":/captures:ro -w /work "$IMG" \
  python3 -I tools/port/engine_gen/$TOOL $TOOL_ARGS "$@"
