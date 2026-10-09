#!/usr/bin/env bash
# Usage: tools/docker.sh <command...>   (no args = interactive bash)
# Builds image `tokimemo-decomp` (linux/amd64) if missing or if tools/Dockerfile changed
# since it was built, then runs with the repo at /work.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IMG="${IMG:-tokimemo-decomp}"   # override to test Dockerfile changes without clobbering the shared image
export DOCKER_DEFAULT_PLATFORM=linux/amd64
# The image carries the Dockerfile's hash as a label; a mismatch means a stale image.
WANT="$(shasum -a 256 "$ROOT/tools/Dockerfile" 2>/dev/null || sha256sum "$ROOT/tools/Dockerfile")"
WANT="${WANT%% *}"
HAVE="$(docker image inspect -f '{{ index .Config.Labels "dockerfile.sha256" }}' "$IMG" 2>/dev/null || true)"
if [ "$HAVE" != "$WANT" ]; then
  docker build --platform linux/amd64 --label "dockerfile.sha256=$WANT" -t "$IMG" "$ROOT/tools" >&2
fi
TTY=(); [ -t 0 ] && TTY=(-it)
exec docker run --rm --platform linux/amd64 ${TTY[@]+"${TTY[@]}"} \
  -u "$(id -u):$(id -g)" -e HOME=/tmp -v "$ROOT":/work -w /work "$IMG" "${@:-bash}"
