#!/bin/sh
# Memory-card codec, virtual card service and ending candidate gate suites, in the
# pinned Godot image. The emulator goldens are mounted read-only when present:
#   .asset-cache/emu/memcard-golden-v1        original-written card (slot create)
#   .asset-cache/emu/newgame-golden-v1        first command select RAM (set A)
#   .asset-cache/emu/endings-saves-golden-v1  notebook save (S1) and load (S2)
# Without them the suites run their source-rule checks and print SKIP lines.
# Run from the repository root (or set TOKIMEMO_ROOT to the checkout that holds
# .asset-cache).
set -eu

IMAGE=${GODOT_PORT_IMAGE:-tokimemo-port}
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
DATA=${TOKIMEMO_ROOT:-$ROOT}
CACHE="$DATA/.asset-cache/emu"

set -- -v "$ROOT/port/godot:/workspace:ro"
if [ -f "$CACHE/memcard-golden-v1/memcard1.mcd" ]; then
  set -- "$@" -v "$CACHE/memcard-golden-v1:/golden/card:ro" -e TOKIMEMO_MEMCARD_GOLDEN=/golden/card
fi
if [ -f "$CACHE/newgame-golden-v1/runs/setA/a7_command_select_ready.ram.bin" ]; then
  set -- "$@" -v "$CACHE/newgame-golden-v1/runs/setA:/golden/setA:ro" \
    -e TOKIMEMO_NEWGAME_RAM=/golden/setA/a7_command_select_ready.ram.bin
fi
if [ -f "$CACHE/endings-saves-golden-v1/s1-probe4/ram.bin" ]; then
  set -- "$@" -v "$CACHE/endings-saves-golden-v1:/golden/save:ro" -e TOKIMEMO_SAVE_GOLDEN=/golden/save
fi

status=0
for suite in tests/services/run_memory_card_image.gd tests/services/run_memory_card_service.gd \
    tests/overlays/run_ending_candidate_gate.gd; do
  name="tokimemo-memcard-tests-$$-$(basename "$suite" .gd | tr '_' '-')"
  echo "== $suite"
  docker run --rm --name "$name" --cpus=2 "$@" "$IMAGE" \
    timeout 120s godot --headless --path /workspace --script "res://$suite" || status=1
done
exit "$status"
