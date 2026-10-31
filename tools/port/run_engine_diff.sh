#!/bin/sh
# T-39301: run the function-differential tests of the engine modules against emulator captures (Docker).
# The captures are game data under <data root>/.asset-cache/m2-engine (tools/emu/func_capture/run.sh, pack.sh);
# they are mounted read-only and never copied into the repository.
#   tools/port/run_engine_diff.sh --file game_mode            one module (config/port/engine-diff/game_mode.json)
#   tools/port/run_engine_diff.sh --all                       every spec
#   tools/port/run_engine_diff.sh --file cd_queue --function cd_read_sectors
# TOKIMEMO_DATA_ROOT: checkout that holds .asset-cache (default: the main checkout of this repo).
# Container name m2diff-$$ (kill with docker kill m2diff-<pid>), --cpus=4.
# T-39314: the SDK layer hands drawing to the generic renderer (a GDExtension, not installed under port/godot/bin).
# RENDER_SO names the library to mount read-only at res://bin/ (default: the candidate build under
# <data root>/.asset-cache/m2-renderer/build-v3, tools/port/native_render/build.sh); without a library the
# renderer-bound functions refuse and their hits fail. Pin the library (RENDER_SO and its hash) before this
# run is used as an acceptance gate: the default is only a development candidate.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
DATA=${TOKIMEMO_DATA_ROOT:-$(CDPATH= cd -- "$(git -C "$ROOT" rev-parse --git-common-dir)/.." && pwd)}
LIB=liboriginal_render.linux.template_release.arm64.so
SO=${RENDER_SO:-$DATA/.asset-cache/m2-renderer/build-v3/project/bin/$LIB}
if [ -f "$SO" ]; then
  # Docker leaves an empty mount-point file in the source tree; the library itself is never installed there.
  trap '[ -s "$ROOT/port/godot/bin/$LIB" ] || rm -f "$ROOT/port/godot/bin/$LIB"' EXIT INT TERM
  docker run --rm --name "${CONTAINER_PREFIX:-m2diff}-$$" --cpus=4 \
    -v "$ROOT/port/godot:/workspace" \
    -v "$ROOT/config/port/engine-diff:/specs:ro" \
    -v "$DATA/.asset-cache/m2-engine:/captures:ro" \
    -v "$SO:/workspace/bin/$LIB:ro" \
    "${GODOT_PORT_IMAGE:-tokimemo-port}" \
    godot --headless --path /workspace --script res://tests/engine/engine_diff_runner.gd -- \
      --root /captures --specs /specs "$@"
else
  docker run --rm --name "${CONTAINER_PREFIX:-m2diff}-$$" --cpus=4 \
    -v "$ROOT/port/godot:/workspace" \
    -v "$ROOT/config/port/engine-diff:/specs:ro" \
    -v "$DATA/.asset-cache/m2-engine:/captures:ro" \
    "${GODOT_PORT_IMAGE:-tokimemo-port}" \
    godot --headless --path /workspace --script res://tests/engine/engine_diff_runner.gd -- \
      --root /captures --specs /specs "$@"
fi
