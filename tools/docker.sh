#!/usr/bin/env bash
# Usage: tools/docker.sh <command...>   (no args = interactive bash)
# Builds the toolchain image if missing or if tools/Dockerfile changed since it was built,
# then runs with the repo at /work.
#
# Platform (T-3310): the image is built natively for the Docker host (linux/arm64 on Apple
# Silicon, linux/amd64 elsewhere). Override with TOKIMEMO_PLATFORM=linux/amd64 (or
# linux/arm64). The amd64 image is the only one with mkpsxiso and the old-gcc versions other
# than 2.7.2-psx and 2.8.1-psx (the arm64 image builds those two, T-9200); on Apple Silicon
# it runs emulated and is several times slower.
# Image name: `tokimemo-decomp` for amd64, `tokimemo-decomp-arm64` for arm64, so both
# can coexist; IMG overrides it.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
if [ -z "${TOKIMEMO_PLATFORM:-}" ]; then
  case "$(docker info --format '{{.Architecture}}' 2>/dev/null || uname -m)" in
    aarch64|arm64) TOKIMEMO_PLATFORM=linux/arm64 ;;
    *)             TOKIMEMO_PLATFORM=linux/amd64 ;;
  esac
fi
case "$TOKIMEMO_PLATFORM" in
  linux/amd64) DEFAULT_IMG=tokimemo-decomp ;;
  linux/arm64) DEFAULT_IMG=tokimemo-decomp-arm64 ;;
  *) echo "docker.sh: TOKIMEMO_PLATFORM must be linux/amd64 or linux/arm64, got '$TOKIMEMO_PLATFORM'" >&2; exit 2 ;;
esac
IMG="${IMG:-$DEFAULT_IMG}"   # override to test Dockerfile changes without clobbering the shared image
# The image carries the Dockerfile's hash as a label; a mismatch (or a different
# architecture under the same name) means a stale image.
WANT="$(shasum -a 256 "$ROOT/tools/Dockerfile" 2>/dev/null || sha256sum "$ROOT/tools/Dockerfile")"
WANT="${WANT%% *}"
HAVE="$(docker image inspect -f '{{ index .Config.Labels "dockerfile.sha256" }}/{{ .Os }}/{{ .Architecture }}' "$IMG" 2>/dev/null || true)"
if [ "$HAVE" != "$WANT/$TOKIMEMO_PLATFORM" ]; then
  docker build --platform "$TOKIMEMO_PLATFORM" --label "dockerfile.sha256=$WANT" -t "$IMG" "$ROOT/tools" >&2
fi
TTY=(); [ -t 0 ] && TTY=(-it)
exec docker run --rm --platform "$TOKIMEMO_PLATFORM" ${TTY[@]+"${TTY[@]}"} \
  -u "$(id -u):$(id -g)" -e HOME=/tmp -v "$ROOT":/work -w /work "$IMG" "${@:-bash}"
