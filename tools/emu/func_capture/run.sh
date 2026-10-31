#!/usr/bin/env bash
# T-39301: capture N hits of C functions (arguments, full RAM at entry, v0/v1 and RAM diff at return,
# plus the effects of callees the port has not ported yet) from a save state, in the tokimemo-emu image
# (PCSX-Redux interpreter, OpenBIOS). Read-only observation of the original game; the output is game
# data and stays under <data root>/.asset-cache/m2-engine/<name>. Never commit it.
#
# usage: run.sh <name> <state (path relative to the data root)> --frames N
#          --fn NAME[=0xADDR][:HITS[:KEYADDR:PER_KEY[:FROM_VSYNC]]] ...   function(s) to capture (address from config/symbol_addrs*.txt)
#          [--callee NAME[=0xADDR]] ...                      unported callee to record (outermost call per hit)
#          [--callee-ptr NAME:0xADDR] ...                    unported callee whose code address is the u32 at ADDR
#          [--input FRAME:BUTTON[:HOLD]] ...                 pad press (vsyncs counted from the state load)
#          [--poke FRAME:0xADDR:HEXBYTES] ...                RAM write at the start of that vsync (a constructed state)
#          [--max-diff BYTES] [--cover] [--ptr-bytes N]
#   --cover records the executed instruction addresses of every hit (slow: a breakpoint per instruction of the function);
#   tools/emu/func_capture/coverage.sh <name> then reports unreached blocks. --ptr-bytes: bytes recorded behind a callee's
#   pointer argument into the stack (default 64).
#   HITS: hits to keep (default 8). KEYADDR/PER_KEY: keep at most PER_KEY hits per distinct byte value at KEYADDR
#   (for example --fn game_mode_dispatch:40:0x800E7388:2 keeps two hits per game mode). FROM_VSYNC ignores earlier hits.
# Then pack the result (gzip the RAM images, write index.json):  pack.sh <name>
# TOKIMEMO_DATA_ROOT: checkout that holds disc/ and .asset-cache (default: the main checkout of this repo).
# Docker: at most one container (--name m2fn-<name>, --cpus=4); kill it with `docker kill m2fn-<name>`.
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../../.." && pwd)
DATA=${TOKIMEMO_DATA_ROOT:-$(cd "$(git -C "$REPO" rev-parse --git-common-dir)/.." && pwd)}
NAME="$1"; STATE="$2"; shift 2
OUT="$DATA/.asset-cache/m2-engine/$NAME"
[ -e "$OUT" ] && { echo "refusing to reuse $OUT" >&2; exit 2; }
[ -f "$DATA/$STATE" ] || { echo "missing state $DATA/$STATE" >&2; exit 2; }

FRAMES=600; TARGETS=""; CALLEES=""; INPUTS=""; MAXDIFF=""; POKES=""; COVER=""; PTRB=""
addr_of() { # NAME -> 0xADDR from the symbol files
  local a
  a=$(grep -h "^$1 = 0x" "$REPO"/config/symbol_addrs*.txt | head -1 | sed 's/.*= \(0x[0-9A-Fa-f]*\).*/\1/')
  [ -n "$a" ] || { echo "no address for $1 in config/symbol_addrs*.txt (use NAME=0xADDR)" >&2; exit 2; }
  echo "$a"
}
while [ $# -gt 0 ]; do
  case "$1" in
    --frames) FRAMES=$2; shift 2 ;;
    --max-diff) MAXDIFF="max_diff = $2," ; shift 2 ;;
    --fn)
      IFS=: read -r head hits keyaddr keymax from <<<"$2"
      name=${head%%=*}; if [ "$name" = "$head" ]; then addr=$(addr_of "$name"); else addr=${head#*=}; fi
      size=$(grep "\"$name\":" "$REPO/port/godot/engine/_frame_sizes.gd" | tail -1 | sed 's/.*: \(0x[0-9A-F]*\),.*/\1/')
      TARGETS+="{ name = '$name', addr = $addr, size = ${size:-0}, max = ${hits:-8}${keyaddr:+, key = $keyaddr, key_max = ${keymax:-1}}${from:+, from = $from} },"
      mkdir -p "$OUT/$name"; shift 2 ;;
    --callee)
      head=$2; name=${head%%=*}; if [ "$name" = "$head" ]; then addr=$(addr_of "$name"); else addr=${head#*=}; fi
      CALLEES+="{ name = '$name', addr = $addr },"; shift 2 ;;
    --callee-ptr)
      IFS=: read -r name ptr <<<"$2"
      CALLEES+="{ name = '$name', ptr = $ptr },"; shift 2 ;;
    --input)
      IFS=: read -r f b h <<<"$2"; INPUTS+="{ $f, '$b', ${h:-2} },"; shift 2 ;;
    --cover) COVER="cover = true,"; shift ;;
    --ptr-bytes) PTRB="ptr_bytes = $2,"; shift 2 ;;
    --poke)
      IFS=: read -r f a b <<<"$2"; POKES+="{ $f, $a, '$b' },"; shift 2 ;;
    *) echo "unknown argument $1" >&2; exit 2 ;;
  esac
done
[ -n "$TARGETS" ] || { echo "give at least one --fn" >&2; exit 2; }
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
{ echo "JOB = { out = '/out', load_state = '/data/$STATE', frames = $FRAMES, ${MAXDIFF}${COVER}${PTRB}"
  echo "  targets = { $TARGETS }, callees = { $CALLEES }, inputs = { $INPUTS }, pokes = { $POKES } }"
  cat "$HERE/capture.lua"; } > "$OUT/job.lua"
docker run --rm --name "m2fn-$NAME" --cpus=4 -u "$(id -u):$(id -g)" -e HOME=/tmp \
  -e SDL_AUDIO_DRIVER=dummy -e SDL_AUDIODRIVER=dummy -e SDL_VIDEO_DRIVER=dummy \
  -v "$DATA/disc":/disc:ro -v "$DATA/.asset-cache":/data/.asset-cache:ro -v "$DATA/build/emu/states":/data/build/emu/states:ro -v "$OUT":/out -w /out tokimemo-emu \
  timeout 3000 nice -n 19 /opt/redux/pcsx-redux -no-ui -stdout -lua_stdout -run -interpreter -debugger \
  -bios /opt/redux/openbios.bin -iso /out/emulator.cue -memcard1 /out/memcard1.mcd -memcard2 /out/memcard2.mcd \
  -dofile /out/job.lua > "$OUT/emulator.log" 2>&1 || echo "exit $?"
cat "$OUT/meta.jsonl" 2>/dev/null | tail -3 || true
