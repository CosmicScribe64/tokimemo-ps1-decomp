#!/bin/sh
# Build and invoke the pinned Godot toolchain. Run from the repository root.
set -eu

IMAGE=${GODOT_PORT_IMAGE:-tokimemo-port}
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
PROJECT="$ROOT/port/godot"

if [ "${1:-test}" != build ] && ! docker image inspect "$IMAGE" >/dev/null 2>&1; then
  echo "Godot image '$IMAGE' is unavailable. Start Docker, then run: sh tools/port/run.sh build" >&2
  exit 1
fi

case "${1:-test}" in
  build)
    docker build --platform linux/arm64 -t "$IMAGE" -f "$ROOT/tools/port/Dockerfile" "$ROOT/tools/port"
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
  *) echo "usage: $0 [build|test|render-smoke]" >&2; exit 2 ;;
esac
