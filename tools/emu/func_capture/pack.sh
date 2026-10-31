#!/usr/bin/env bash
# T-39301: pack <name> = gzip the RAM images of .asset-cache/m2-engine/<name> and write its index.json (Docker).
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../../.." && pwd)
DATA=${TOKIMEMO_DATA_ROOT:-$(cd "$(git -C "$REPO" rev-parse --git-common-dir)/.." && pwd)}
docker run --rm --name "m2pack-$1" -u "$(id -u):$(id -g)" -v "$HERE:/t:ro" -v "$DATA/.asset-cache/m2-engine:/c" \
  tokimemo-recovery python3 -I /t/pack.py "/c/$1"
