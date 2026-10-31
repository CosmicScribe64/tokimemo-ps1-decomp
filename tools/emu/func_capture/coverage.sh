#!/usr/bin/env bash
# T-39301: coverage.sh <capture name>... : unreached basic blocks per function of captures made with --cover (Docker).
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../../.." && pwd)
DATA=${TOKIMEMO_DATA_ROOT:-$(cd "$(git -C "$REPO" rev-parse --git-common-dir)/.." && pwd)}
ARGS=(); for n in "$@"; do ARGS+=("/c/$n"); done
docker run --rm --name "m2cov-$$" -u "$(id -u):$(id -g)" -v "$HERE:/t:ro" -v "$DATA/.asset-cache/m2-engine:/c:ro" -v "$DATA/asm:/asm:ro" \
  tokimemo-recovery python3 -I /t/coverage.py "${ARGS[@]}" --asm /asm
