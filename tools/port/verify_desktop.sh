#!/bin/sh
# Check an exported Linux arm64 player in Docker with a fresh per-user data directory.
#
#   sh tools/port/verify_desktop.sh .asset-cache/port/BUILD .asset-cache/port/NEW_RUN [CONTENT_BUNDLE]
#
# Thin wrapper: the checks live in tools/port/desktop_verify/verify_player.py and run
# in the tokimemo-port-gui-test image (sh tools/port/run.sh build-gui-test).
# Without CONTENT_BUNDLE: setup routing, native extension load and the BGM bridge's
# native-component gate, plus a negative run without the library.
# With CONTENT_BUNDLE (from tools/port/setup_content.sh): also publication, New Game
# entry routing and the opening under Xvfb/PulseAudio with recorded audible output.
set -eu
usage() { echo "usage: $0 .asset-cache/port/BUILD .asset-cache/port/NEW_RUN [CONTENT_BUNDLE]" >&2; exit 2; }
[ "$#" -eq 2 ] || [ "$#" -eq 3 ] || usage
BUILD=$1
RUN=$2
BUNDLE=${3:-}
for path in "$BUILD" "$RUN"; do
  case "$path" in .asset-cache/port/*) ;; *) usage ;; esac
  case "$path" in *..*) echo "paths cannot contain '..'" >&2; exit 2 ;; esac
done
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
GUI_IMAGE=${TOKIMEMO_GUI_TEST_IMAGE:-tokimemo-port-gui-test}
docker info >/dev/null 2>&1 || { echo "Docker is not running. Start Docker Desktop and retry." >&2; exit 1; }
docker image inspect "$GUI_IMAGE" >/dev/null 2>&1 || {
  echo "GUI test image '$GUI_IMAGE' is unavailable. Build it with: sh tools/port/run.sh build-gui-test" >&2
  echo "(after sh tools/port/run.sh build)" >&2; exit 1; }
[ -x "$ROOT/$BUILD/player/tokimemo-port.arm64" ] && [ -f "$ROOT/$BUILD/project/project.godot" ] || {
  echo "No Linux arm64 player in $BUILD. Run: sh tools/port/build_desktop.sh linux-arm64 player $BUILD" >&2; exit 2; }
[ ! -e "$ROOT/$RUN" ] || { echo "Run directory $RUN exists; choose a new one." >&2; exit 2; }
set --
if [ -n "$BUNDLE" ]; then
  [ -f "$BUNDLE/manifest.json" ] || {
    echo "Content bundle '$BUNDLE' has no manifest.json; create one with tools/port/setup_content.sh." >&2; exit 2; }
  BUNDLE=$(CDPATH= cd -- "$BUNDLE" && pwd)
  set -- -v "$BUNDLE:/bundle:ro"
fi
mkdir -p "$ROOT/$RUN"
BUNDLE_ARG=
[ -z "$BUNDLE" ] || BUNDLE_ARG="--bundle /bundle"
# shellcheck disable=SC2086
docker run --rm --name "tokimemo-desktop-verify-$(basename "$RUN")" --network none --cpus=2 \
  -v "$ROOT/$BUILD:/build:ro" -v "$ROOT/tools/port/desktop_verify:/verify:ro" -v "$ROOT/$RUN:/run-out" \
  "$@" "$GUI_IMAGE" python3 -I /verify/verify_player.py --build /build --out /run-out $BUNDLE_ARG
