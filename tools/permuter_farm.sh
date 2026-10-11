#!/usr/bin/env bash
# T-9810: one persistent, CPU-only Docker container per farm worker.
# Usage: SCRATCHPAD=/path/to/scratchpad tools/permuter_farm.sh collect|status|run [-j N] [options]
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SCRATCHPAD="${SCRATCHPAD:-/private/tmp/claude-501/-Users-lila-Documents-Projects-tokimemo-decomp/43264b98-bb59-4c1b-ad99-a2084d55fcf0/scratchpad}"
IMAGE="${IMG:-tokimemo-decomp-arm64}"
COMMAND="${1:-status}"
shift || true
ARGS=()
JOBS="${FARM_JOBS:-}"
while [ "$#" -gt 0 ]; do
  case "$1" in
    -j|--jobs) JOBS="$2"; shift 2 ;;
    --root|--scratch|--farm|--worker-id|--root=*|--scratch=*|--farm=*|--worker-id=*) echo "farm paths/worker IDs are managed by this launcher" >&2; exit 2 ;;
    *) ARGS+=("$1"); shift ;;
  esac
done
mkdir -p "$SCRATCHPAD/r9-farm/tmp"
MOUNTS=(-v "$ROOT":/work:ro -v "$SCRATCHPAD":/scratch -w /work)
ENV=(-e PYTHONDONTWRITEBYTECODE=1 -e TMPDIR=/scratch/r9-farm/tmp -e HOME=/scratch/r9-farm/tmp)
FARM_USER=(-u "$(id -u):$(id -g)")
COMMON=(--root /work --scratch /scratch --farm /scratch/r9-farm)
if [ "$COMMAND" != run ]; then
  exec docker run --rm "${FARM_USER[@]}" "${ENV[@]}" "${MOUNTS[@]}" "$IMAGE" \
    python3 tools/permuter_farm.py "$COMMAND" "${COMMON[@]}" "${ARGS[@]+"${ARGS[@]}"}"
fi
if [ -z "$JOBS" ]; then
  JOBS="$(docker run --rm "$IMAGE" python3 -c 'import os; print(max(1,(os.cpu_count() or 1)//2))')"
fi
case "$JOBS" in ''|*[!0-9]*|0) echo 'jobs must be positive' >&2; exit 2 ;; esac
if [ ! -f "$SCRATCHPAD/r9-farm/queue.json" ]; then
  echo 'collect the queue before starting the farm' >&2; exit 2
fi
for ((i=0; i<JOBS; i++)); do
  CID="$(docker run -d --rm --cpus 1 -e WORKER_ID="$i" "${FARM_USER[@]}" "${ENV[@]}" "${MOUNTS[@]}" "$IMAGE" \
    sh -c 'exec python3 tools/permuter_farm.py run "$@" >> "/scratch/r9-farm/worker-$WORKER_ID.log" 2>&1' \
    farm "${COMMON[@]}" --worker-id "$i" -j 1 "${ARGS[@]+"${ARGS[@]}"}"
  )"
  # Docker's hostname is a unique worker owner; the numeric slot protects its private build.
  printf '%s\t%s\n' "$i" "$CID" >> "$SCRATCHPAD/r9-farm/containers.tsv"
  printf 'worker %s: %s\n' "$i" "$CID"
done
