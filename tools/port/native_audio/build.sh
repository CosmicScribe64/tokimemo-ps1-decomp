#!/usr/bin/env bash
# T-39002: build a Linux arm64 candidate without installing host dependencies.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
IMAGE="${TOKIMEMO_CONTENT_IMAGE:-tokimemo-content}"
docker info >/dev/null 2>&1 || { echo 'Start Docker Desktop and retry.' >&2; exit 2; }
docker image inspect "$IMAGE" >/dev/null 2>&1 || {
  echo "Missing compiler image $IMAGE. Build tools/port/Dockerfile.content, or set TOKIMEMO_CONTENT_IMAGE to an existing compatible compiler image." >&2
  exit 2
}
exec docker run --rm --platform linux/arm64 -v "$ROOT:/work" -w /work "$IMAGE" \
  python3 tools/port/native_audio/build.py "$@"
