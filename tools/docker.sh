#!/usr/bin/env bash
# Usage: tools/docker.sh <command...>   (no args = interactive bash)
# Builds image `tokimemo-decomp` (linux/amd64) if missing, runs with repo at /work.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IMG="${IMG:-tokimemo-decomp}"   # override to test Dockerfile changes without clobbering the shared image
export DOCKER_DEFAULT_PLATFORM=linux/amd64
if ! docker image inspect "$IMG" >/dev/null 2>&1; then
  docker build --platform linux/amd64 -t "$IMG" "$ROOT/tools"
fi
TTY=(); [ -t 0 ] && TTY=(-it)
exec docker run --rm --platform linux/amd64 ${TTY[@]+"${TTY[@]}"} \
  -u "$(id -u):$(id -g)" -e HOME=/tmp -v "$ROOT":/work -w /work "$IMG" "${@:-bash}"
