#!/bin/sh
# Run, test or publish through the existing project Docker image.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../../.." && pwd)
TOOL_DIR="$ROOT/tools/port/progress"
IMAGE=${PORT_PROGRESS_IMAGE:-tokimemo-decomp-arm64}
MODE=${1:-serve}
if [ "$#" -gt 0 ]; then shift; fi
case "$MODE" in
  serve)
    MEDIA_DIR=$("$TOOL_DIR/cache_root.sh" "$ROOT")
    mkdir -p "$MEDIA_DIR"
    MEDIA_DIR=$("$TOOL_DIR/cache_root.sh" "$ROOT")
    exec docker run --rm --cpus=2 --name tokimemo-port-progress \
      -p 127.0.0.1:8765:8765 -v "$TOOL_DIR:/app:ro" -v "$MEDIA_DIR:/media:ro" \
      --entrypoint python3 "$IMAGE" /app/server.py --host 0.0.0.0 --port 8765 --media-root /media
    ;;
  publish)
    exec "$TOOL_DIR/publish.sh" "$@"
    ;;
  capture)
    "$TOOL_DIR/cache_root.sh" "$ROOT" >/dev/null
    "$ROOT/tools/port/run.sh" render-smoke
    exec "$TOOL_DIR/publish.sh" "$ROOT/.asset-cache/port-smoke/render-smoke.png"
    ;;
  watch)
    MEDIA_DIR=$("$TOOL_DIR/cache_root.sh" "$ROOT" watch)
    mkdir -p "$ROOT/.asset-cache/port-smoke" "$ROOT/.asset-cache/port-preview-output" \
      "$ROOT/.asset-cache/port-captures" "$MEDIA_DIR"
    MEDIA_DIR=$("$TOOL_DIR/cache_root.sh" "$ROOT" watch)
    exec docker run --detach --name tokimemo-port-progress-watch --network none --cpus=2 \
      -v "$TOOL_DIR:/app:ro" \
      -v "$ROOT/.asset-cache/port-smoke:/watch/port-smoke:ro" \
      -v "$ROOT/.asset-cache/port-preview-output:/watch/port-preview-output:ro" \
      -v "$ROOT/.asset-cache/port-captures:/watch/port-captures:ro" \
      -v "$MEDIA_DIR:/media" --entrypoint python3 "$IMAGE" \
      /app/watcher.py --capture-root /watch --media-root /media --interval 2
    ;;
  check-root)
    "$TOOL_DIR/cache_root.sh" "$ROOT" >/dev/null
    ;;
  test)
    exec docker run --rm --network none --cpus=2 --entrypoint python3 \
      -v "$TOOL_DIR:/app:ro" "$IMAGE" -m unittest discover -s /app/tests -v
    ;;
  *) echo "usage: $0 [serve|publish CAPTURE...|capture|watch|check-root|test]" >&2; exit 2 ;;
esac
