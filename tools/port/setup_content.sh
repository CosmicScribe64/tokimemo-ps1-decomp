#!/usr/bin/env bash
# T-39002: container-only fresh-clone content setup. Original archive stays read-only.
# Also builds the English bundle (Phase 4 WP-C) from the committed translations/en.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
if [ "$#" -lt 1 ] || [ ! -f "$1" ]; then
  echo 'Usage: tools/port/setup_content.sh /absolute/path/to/game.7z [--output .asset-cache/port/NEW_DIRECTORY] [--english-output .asset-cache/port/NEW_DIRECTORY | --no-english]' >&2
  exit 2
fi
ARCHIVE="$(cd "$(dirname "$1")" && pwd)/$(basename "$1")"
shift
docker info >/dev/null 2>&1 || { echo 'Start Docker Desktop and retry.' >&2; exit 2; }
docker build -t tokimemo-content -f "$ROOT/tools/port/Dockerfile.content" "$ROOT/tools/port"
exec docker run --rm -v "$ROOT:/work" -v "$ARCHIVE:/original.7z:ro" tokimemo-content python3 tools/port/setup_content.py /original.7z "$@"
