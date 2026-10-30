#!/bin/sh
# Build and invoke the pinned Godot toolchain. Run from the repository root.
#   build          build the Godot 4.7.2 engine/export-template image (tokimemo-port)
#   build-content  build the Python/media/compiler image (tokimemo-content) used for
#                  content setup, desktop staging and the native audio build
#   build-gui-test build the X11/PulseAudio test image (tokimemo-port-gui-test) on top
#                  of tokimemo-port, for real-input and audio-output checks
set -eu

IMAGE=${GODOT_PORT_IMAGE:-tokimemo-port}
CONTENT_IMAGE=${TOKIMEMO_CONTENT_IMAGE:-tokimemo-content}
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
PROJECT="$ROOT/port/godot"

docker info >/dev/null 2>&1 || { echo "Docker is not running. Start Docker Desktop and retry." >&2; exit 1; }
case "${1:-test}" in build|build-content) ;; *) if ! docker image inspect "$IMAGE" >/dev/null 2>&1; then
  echo "Godot image '$IMAGE' is unavailable. Build it with: sh tools/port/run.sh build" >&2
  exit 1
fi ;;
esac

case "${1:-test}" in
  build)
    docker build --platform linux/arm64 -t "$IMAGE" -f "$ROOT/tools/port/Dockerfile" "$ROOT/tools/port"
    ;;
  build-gui-test)
    docker image inspect "$IMAGE" >/dev/null 2>&1 || {
      echo "Build the engine image first: sh tools/port/run.sh build" >&2; exit 1; }
    docker build --platform linux/arm64 -t "${TOKIMEMO_GUI_TEST_IMAGE:-tokimemo-port-gui-test}" \
      --build-arg BASE="$IMAGE" -f "$ROOT/tools/port/Dockerfile.gui-test" "$ROOT/tools/port"
    ;;
  build-content)
    docker build --platform linux/arm64 -t "$CONTENT_IMAGE" -f "$ROOT/tools/port/Dockerfile.content" "$ROOT/tools/port"
    ;;
  test)
    docker run --rm --cpus=2 -v "$PROJECT:/workspace:ro" "$IMAGE" \
      sh -ec 'status=0; timeout 45s godot --headless \
        --rendering-method gl_compatibility --path /workspace \
        --script res://tests/run_tests.gd >/tmp/port-tests.log 2>&1 || status=$?; \
        cat /tmp/port-tests.log; \
        if [ "$status" -ne 0 ]; then exit "$status"; fi; \
        if grep -q "SCRIPT ERROR:" /tmp/port-tests.log; then \
          echo "FAIL: GDScript runtime error during tests" >&2; exit 1; fi; \
        grep -Eq "^PASS: [0-9]+ test suite\(s\) ran$" /tmp/port-tests.log'
    ;;
  render-smoke)
    mkdir -p "$ROOT/.asset-cache/port-smoke"
    rm -f -- "$ROOT/.asset-cache/port-smoke/render-smoke.png"
    docker run --rm --cpus=2 -e GODOT_SMOKE_CAPTURE=/out/render-smoke.png \
      -v "$PROJECT:/workspace:ro" -v "$ROOT/.asset-cache/port-smoke:/out" "$IMAGE" \
      sh -ec 'Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp -ac >/tmp/xvfb.log 2>&1 & \
        xvfb_pid=$!; trap "kill $xvfb_pid 2>/dev/null || true" EXIT; sleep 1; \
        DISPLAY=:99 LIBGL_ALWAYS_SOFTWARE=1 GALLIUM_DRIVER=llvmpipe \
        timeout 45s godot --rendering-method gl_compatibility --display-driver x11 \
          --audio-driver Dummy --path /workspace --quit-after 4'
    test -s "$ROOT/.asset-cache/port-smoke/render-smoke.png"
    ;;
  *) echo "usage: $0 [build|build-content|build-gui-test|test|render-smoke]" >&2; exit 2 ;;
esac
