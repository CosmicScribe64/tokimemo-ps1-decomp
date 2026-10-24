#!/usr/bin/env bash
# Usage: tools/emu/run.sh <emu.py subcommand and options>   (T-20500, wiki/emulator.md)
#        tools/emu/run.sh shell                              (bash in the image)
# Builds the emulator image (tools/emu/Dockerfile) if missing or stale, then runs
# tools/emu/emu.py in it with the repo at /work. Containers get EMU_CPUS CPUs (default 2)
# so other jobs on the host keep their share. `gdb` publishes the GDB server port on localhost.
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
IMG="${EMU_IMG:-tokimemo-emu}"
CPUS="${EMU_CPUS:-2}"
WANT="$(shasum -a 256 "$ROOT/tools/emu/Dockerfile" 2>/dev/null || sha256sum "$ROOT/tools/emu/Dockerfile")"
WANT="${WANT%% *}"
HAVE="$(docker image inspect -f '{{ index .Config.Labels "dockerfile.sha256" }}' "$IMG" 2>/dev/null || true)"
if [ "$HAVE" != "$WANT" ]; then
  nice -n 19 docker build --label "dockerfile.sha256=$WANT" -t "$IMG" "$ROOT/tools/emu" >&2
fi
mkdir -p "$ROOT/build/emu"
PORTS=()
if [ "${1:-}" = gdb ]; then
  PORT=3333
  for ((i = 1; i <= $#; i++)); do
    if [ "${!i}" = --port ]; then j=$((i + 1)); PORT="${!j}"; fi
  done
  PORTS=(-p "127.0.0.1:$PORT:$PORT")
fi
TTY=(); [ -t 0 ] && TTY=(-it)
if [ "${1:-}" = shell ]; then
  shift
  set -- bash "$@"
else
  set -- python3 tools/emu/emu.py "$@"
fi
exec docker run --rm --cpus="$CPUS" ${TTY[@]+"${TTY[@]}"} ${PORTS[@]+"${PORTS[@]}"} \
  -u "$(id -u):$(id -g)" -e HOME=/tmp -v "$ROOT":/work -w /work "$IMG" "$@"
