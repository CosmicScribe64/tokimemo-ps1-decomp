#!/usr/bin/env bash
# Publish only explicitly named capture files through the existing project image.
set -euo pipefail
if (( $# < 1 )); then
  echo "usage: $0 CAPTURE.png|CAPTURE.mp4|CAPTURE.webm [...]" >&2
  exit 2
fi
ROOT=$(cd -- "$(dirname -- "$0")/../../.." && pwd)
TOOL_DIR="$ROOT/tools/port/progress"
IMAGE=${PORT_PROGRESS_IMAGE:-tokimemo-decomp-arm64}
MEDIA_DIR=$("$TOOL_DIR/cache_root.sh" "$ROOT")
mkdir -p "$MEDIA_DIR"
MEDIA_DIR=$("$TOOL_DIR/cache_root.sh" "$ROOT")
docker_args=()
container_args=()
idx=0
for source in "$@"; do
  [[ -f "$source" && ! -L "$source" ]] || { echo "capture must be an explicit regular non-symlink file: $source" >&2; exit 2; }
  case "$source" in
    *.png|*.PNG) suffix=png ;;
    *.mp4|*.MP4) suffix=mp4 ;;
    *.webm|*.WEBM) suffix=webm ;;
    *) echo "unsupported capture extension: $source" >&2; exit 2 ;;
  esac
  idx=$((idx + 1))
  source_dir=$(cd -- "$(dirname -- "$source")" && pwd)
  source_path="$source_dir/$(basename -- "$source")"
  docker_args+=( -v "$source_path:/incoming/capture-$idx.$suffix:ro" )
  container_args+=( "/incoming/capture-$idx.$suffix" )
done
exec docker run --rm --network none --cpus=2 --entrypoint python3 \
  -v "$TOOL_DIR:/app:ro" -v "$MEDIA_DIR:/media" "${docker_args[@]}" \
  "$IMAGE" /app/publish.py --media-root /media "${container_args[@]}"
