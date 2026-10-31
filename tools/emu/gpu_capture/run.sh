#!/usr/bin/env bash
# T-39302: capture DrawOTag frames (RAM + VRAM before, VRAM after) from a save state, in the
# tokimemo-emu image (PCSX-Redux interpreter, OpenBIOS). Read-only observation; game data stays
# under the data root's .asset-cache and is never committed.
# usage: run.sh <out-dir (new, under <data-root>/.asset-cache/m2-renderer)> <state path (relative to the
#        data root)> '<JOB lua fields without out=/load_state=, e.g. picks={3,40}, frames=200>'
# <prelude.lua> (optional 4th argument): Lua prepended to the job, e.g. the INJECT table of synth_ot.py.
# TOKIMEMO_DATA_ROOT: checkout that holds disc/ and .asset-cache (default: this checkout).
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
DATA=${TOKIMEMO_DATA_ROOT:-$(cd "$HERE/../../.." && pwd)}
OUT="$1"; STATE="$2"; BODY="$3"; PRELUDE="${4:-}"
case "$OUT" in /*) ;; *) OUT="$DATA/$OUT" ;; esac
case "$OUT" in "$DATA/.asset-cache/m2-renderer"/*) ;; *) echo "out dir must be under $DATA/.asset-cache/m2-renderer" >&2; exit 2 ;; esac
[ -e "$OUT" ] && { echo "refusing to reuse $OUT" >&2; exit 2; }
[ -f "$DATA/$STATE" ] || { echo "missing state $DATA/$STATE" >&2; exit 2; }
mkdir -p "$OUT"
cat > "$OUT/emulator.cue" <<CUE
FILE "/disc/track1.bin" BINARY
  TRACK 01 MODE2/2352
    INDEX 01 00:00:00
FILE "/disc/track2.bin" BINARY
  TRACK 02 AUDIO
    INDEX 00 00:00:00
    INDEX 01 00:02:00
CUE
{ [ -z "$PRELUDE" ] || cat "$PRELUDE"; echo "JOB = { out = '/out', load_state = '/data/$STATE', $BODY }"; cat "$HERE/capture.lua"; } > "$OUT/job.lua"
docker run --rm --name "m2cap-$(basename "$OUT")" --cpus=4 -u "$(id -u):$(id -g)" -e HOME=/tmp \
  -e SDL_AUDIO_DRIVER=dummy -e SDL_AUDIODRIVER=dummy -e SDL_VIDEO_DRIVER=dummy \
  -v "$DATA/disc":/disc:ro -v "$DATA/.asset-cache":/data/.asset-cache:ro -v "$OUT":/out -w /out tokimemo-emu \
  timeout 1800 nice -n 19 /opt/redux/pcsx-redux -no-ui -stdout -lua_stdout -run -interpreter -debugger \
  -bios /opt/redux/openbios.bin -iso /out/emulator.cue -memcard1 /out/memcard1.mcd -memcard2 /out/memcard2.mcd \
  -dofile /out/job.lua > "$OUT/emulator.log" 2>&1 || echo "exit $?"
ls -la "$OUT"
