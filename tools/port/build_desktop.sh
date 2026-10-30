#!/bin/sh
# Export a code-only macOS development player using the pinned Docker templates.
set -eu
if [ "$#" -ne 2 ]; then
  echo "usage: $0 week|new-game .asset-cache/port/NEW_BUILD_DIRECTORY" >&2
  exit 2
fi
SCENE=$1
OUTPUT=$2
case "$SCENE" in week|new-game) ;; *) echo "unknown scene" >&2; exit 2 ;; esac
case "$OUTPUT" in .asset-cache/port/*) ;; *) echo "output must be an ignored port-cache path" >&2; exit 2 ;; esac
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
IMAGE=${GODOT_PORT_IMAGE:-tokimemo-port}
CONTENT_IMAGE=${TOKIMEMO_CONTENT_IMAGE:-tokimemo-content}
if ! docker image inspect "$IMAGE" >/dev/null 2>&1; then
  echo "Godot image '$IMAGE' is unavailable. Start Docker, then run: sh tools/port/run.sh build" >&2
  exit 1
fi
if ! docker image inspect "$CONTENT_IMAGE" >/dev/null 2>&1; then
  echo "Content image '$CONTENT_IMAGE' is unavailable. Run tools/port/setup_content.sh with your original archive first." >&2
  exit 1
fi
docker run --rm --network none -v "$ROOT:/repo" -w /repo \
  --entrypoint python3 "$CONTENT_IMAGE" \
  tools/port/prepare_desktop.py --repository /repo \
  --destination "/repo/$OUTPUT/project" --scene "$SCENE"
docker run --rm --network none --cpus=2 \
  -v "$ROOT/$OUTPUT:/output" "$IMAGE" sh -ec '
    timeout 90s godot --headless --editor --path /output/project --import > /output/import.log 2>&1
    if grep -q "ERROR:" /output/import.log; then cat /output/import.log; exit 1; fi
    timeout 90s godot --headless --path /output/project --export-debug "macOS Development" /output/player.zip > /output/export.log 2>&1
    if grep -q "ERROR:" /output/export.log; then cat /output/export.log; exit 1; fi
    test -s /output/player.zip
    sha256sum /output/player.zip > /output/player.sha256
  '
echo "Desktop export: $ROOT/$OUTPUT/player.zip"
echo "Unsigned development build; original content must be supplied separately."
