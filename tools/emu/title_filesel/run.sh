#!/usr/bin/env bash
# Title/file-select golden: boots the original from power-on in the tokimemo-emu image
# (PCSX-Redux interpreter, OpenBIOS) with capture.lua and a scripted pad.
# usage: run.sh <out-dir (new, under the repo root)> '<JOB lua fields without out=>'
# The output dir must not exist yet; nothing outside it is written.
set -euo pipefail
ROOT=${TOKIMEMO_ROOT:-$(cd "$(dirname "$0")/../../.." && pwd)}
OUT="$1"; BODY="$2"
case "$OUT" in /*) ;; *) OUT="$ROOT/$OUT" ;; esac
case "$OUT" in "$ROOT"/*) ;; *) echo "out dir must be under $ROOT" >&2; exit 2 ;; esac
[ -e "$OUT" ] && { echo "refusing to reuse $OUT" >&2; exit 2; }
mkdir -p "$OUT"
REL=${OUT#"$ROOT"/}
W=/work/$REL
HERE=$(cd "$(dirname "$0")" && pwd)
cat > "$OUT/emulator.cue" <<CUE
FILE "/work/disc/track1.bin" BINARY
  TRACK 01 MODE2/2352
    INDEX 01 00:00:00
FILE "/work/disc/track2.bin" BINARY
  TRACK 02 AUDIO
    INDEX 00 00:00:00
    INDEX 01 00:02:00
CUE
# Card paths are run-local and start absent; the job decides whether the slots count as inserted.
{ echo "JOB = { out = '$W', $BODY }"; cat "$HERE/capture.lua"; } > "$OUT/job.lua"
NAME="tfs-$(basename "$OUT")"
docker run --rm --name "$NAME" --cpus=2 -u "$(id -u):$(id -g)" -e HOME=/tmp \
  -e SDL_AUDIO_DRIVER=dummy -e SDL_AUDIODRIVER=dummy -e SDL_VIDEO_DRIVER=dummy \
  -v "$ROOT":/work -w "$W" tokimemo-emu \
  timeout 3600 nice -n 19 /opt/redux/pcsx-redux -no-ui -stdout -lua_stdout -run -interpreter -debugger \
  -bios /opt/redux/openbios.bin -iso "$W/emulator.cue" -memcard1 "$W/memcard1.mcd" -memcard2 "$W/memcard2.mcd" \
  -dofile "$W/job.lua" > "$OUT/emulator.log" 2>&1 || echo "exit $?"
ls "$OUT"
