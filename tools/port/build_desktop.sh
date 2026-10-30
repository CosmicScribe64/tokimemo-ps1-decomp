#!/bin/sh
# Export an unsigned desktop development player with the pinned Docker images.
# Targets:
#   linux-arm64  player with the reviewed native original-audio library
#   macos        code-only universal app; no native audio backend (rejected at runtime)
# linux-x86_64 is refused: no reviewed x86_64 native audio library exists.
# Original game data is never staged; players install it at first launch.
set -eu
usage() {
  echo "usage: $0 linux-arm64|macos player|week|new-game .asset-cache/port/NEW_BUILD_DIRECTORY" >&2
  exit 2
}
[ "$#" -eq 3 ] || usage
TARGET=$1
SCENE=$2
OUTPUT=$3
case "$TARGET" in
  linux-arm64|macos) ;;
  linux-x86_64)
    echo "Linux x86_64 is not packaged: port/godot/original_audio.gdextension declares only the reviewed Linux arm64 library," >&2
    echo "tools/port/native_audio/build.py builds arm64 only, and the BGM bridge accepts only Linux arm64." >&2
    echo "Build, review and pin an x86_64 library before adding this target." >&2
    exit 3 ;;
  *) usage ;;
esac
case "$SCENE" in player|week|new-game) ;; *) usage ;; esac
case "$OUTPUT" in .asset-cache/port/*) ;; *) echo "output must be an ignored port-cache path (.asset-cache/port/...)" >&2; exit 2 ;; esac
case "$OUTPUT" in *..*) echo "output cannot contain '..'" >&2; exit 2 ;; esac
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
IMAGE=${GODOT_PORT_IMAGE:-tokimemo-port}
CONTENT_IMAGE=${TOKIMEMO_CONTENT_IMAGE:-tokimemo-content}
docker info >/dev/null 2>&1 || { echo "Docker is not running. Start Docker Desktop and retry." >&2; exit 1; }
if ! docker image inspect "$IMAGE" >/dev/null 2>&1; then
  echo "Godot image '$IMAGE' is unavailable. Build it with: sh tools/port/run.sh build" >&2
  echo "(or set GODOT_PORT_IMAGE to an existing image built from tools/port/Dockerfile)" >&2
  exit 1
fi
if ! docker image inspect "$CONTENT_IMAGE" >/dev/null 2>&1; then
  echo "Staging image '$CONTENT_IMAGE' is unavailable. Build it with: sh tools/port/run.sh build-content" >&2
  echo "(no game archive is needed; or set TOKIMEMO_CONTENT_IMAGE to an existing image built from tools/port/Dockerfile.content)" >&2
  exit 1
fi
case "$TARGET" in
  linux-arm64) PRESET="Linux arm64"; MODE=--export-release; ARTIFACT=player/tokimemo-port.arm64 ;;
  macos) PRESET="macOS Development"; MODE=--export-debug; ARTIFACT=player.zip ;;
esac
NAME=$(basename "$OUTPUT")
docker run --rm --name "tokimemo-desktop-stage-$NAME" --network none -v "$ROOT:/repo" -w /repo \
  --entrypoint python3 "$CONTENT_IMAGE" \
  tools/port/prepare_desktop.py --repository /repo \
  --destination "/repo/$OUTPUT/project" --scene "$SCENE" --target "$TARGET"
docker run --rm --name "tokimemo-desktop-export-$NAME" --network none --cpus=2 \
  -e PRESET="$PRESET" -e MODE="$MODE" -e ARTIFACT="$ARTIFACT" -e REPRO="${ARTIFACT%%/*}" \
  -v "$ROOT/$OUTPUT:/output" "$IMAGE" sh -ec '
    mkdir -p "$(dirname "/output/$ARTIFACT")"
    timeout 120s godot --headless --editor --path /output/project --import > /output/import.log 2>&1
    if grep -q "ERROR:" /output/import.log; then cat /output/import.log; exit 1; fi
    timeout 120s godot --headless --path /output/project "$MODE" "$PRESET" "/output/$ARTIFACT" > /output/export.log 2>&1
    if grep -q "ERROR:" /output/export.log; then cat /output/export.log; exit 1; fi
    test -s "/output/$ARTIFACT"
    # Hash the export; the macOS zip stores export-time entry dates, so hash its entries.
    contents() {
      if [ "${1##*.}" = zip ]; then rm -rf /tmp/unzipped; mkdir /tmp/unzipped; unzip -q "$1" -d /tmp/unzipped; set -- /tmp/unzipped; fi
      (cd "$1" && find . -type f -print0 | sort -z | xargs -0 sha256sum)
    }
    cd /output
    find player player.zip -type f 2>/dev/null | sort | xargs sha256sum > /output/player.sha256
    contents "/output/$REPRO" > /output/player-contents.sha256
    # Reproducibility: a second export of the same staged project must hash the same.
    mkdir -p "$(dirname "/tmp/repeat/$ARTIFACT")"
    timeout 120s godot --headless --path /output/project "$MODE" "$PRESET" "/tmp/repeat/$ARTIFACT" > /output/export-repeat.log 2>&1
    contents "/tmp/repeat/$REPRO" > /output/player-repeat-contents.sha256
    if ! cmp -s /output/player-contents.sha256 /output/player-repeat-contents.sha256; then
      echo "Export is not reproducible: a second export of the same staged project differs" >&2
      diff /output/player-contents.sha256 /output/player-repeat-contents.sha256 >&2 || true
      exit 1
    fi
  '
echo "Desktop export ($TARGET): $ROOT/$OUTPUT/$ARTIFACT"
echo "Unsigned development build; original content must be installed separately at first launch."
if [ "$TARGET" = macos ]; then
  echo "macOS build is code-only: no native audio library is packaged and original BGM is rejected at runtime."
fi
