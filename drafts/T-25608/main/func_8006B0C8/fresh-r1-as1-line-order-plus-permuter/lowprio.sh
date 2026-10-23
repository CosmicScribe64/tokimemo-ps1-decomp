#!/bin/bash
# T-25608: low-priority permuter run (1 CPU, low CPU shares, nice 19) on build/permute/func_8006B0C8.
# usage (worktree root): drafts/T-25608/main/func_8006B0C8/fresh-r1-as1-line-order-plus-permuter/lowprio.sh SECONDS
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../../../../.." && pwd)"
exec docker run --rm --platform linux/arm64 --cpus 1 --cpu-shares 2 --name "f-b0c8-as1-perm" \
  -u "$(id -u):$(id -g)" -e HOME=/tmp -e PYTHONUNBUFFERED=1 -v "$ROOT":/work -w /work tokimemo-decomp-arm64 \
  nice -n 19 python3 tools/permute.py run build/permute/func_8006B0C8 --unit main -j 1 --time "${1:-12600}"
