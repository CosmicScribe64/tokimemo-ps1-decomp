#!/bin/sh
# T-39301: run the function-differential tests of the engine modules against emulator captures (Docker).
# The captures are game data under <data root>/.asset-cache/m2-engine (tools/emu/func_capture/run.sh, pack.sh);
# they are mounted read-only and never copied into the repository.
#   tools/port/run_engine_diff.sh --file game_mode            one module (config/port/engine-diff/game_mode.json)
#   tools/port/run_engine_diff.sh --all                       every spec
#   tools/port/run_engine_diff.sh --file cd_queue --function cd_read_sectors
# TOKIMEMO_DATA_ROOT: checkout that holds .asset-cache (default: the main checkout of this repo).
# Container name m2diff-$$ (kill with docker kill m2diff-<pid>), --cpus=4.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
DATA=${TOKIMEMO_DATA_ROOT:-$(CDPATH= cd -- "$(git -C "$ROOT" rev-parse --git-common-dir)/.." && pwd)}
docker run --rm --name "m2diff-$$" --cpus=4 \
  -v "$ROOT/port/godot:/workspace" \
  -v "$ROOT/config/port/engine-diff:/specs:ro" \
  -v "$DATA/.asset-cache/m2-engine:/captures:ro" \
  "${GODOT_PORT_IMAGE:-tokimemo-port}" \
  godot --headless --path /workspace --script res://tests/engine/engine_diff_runner.gd -- \
    --root /captures --specs /specs "$@"
