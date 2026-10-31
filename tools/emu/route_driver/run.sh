#!/usr/bin/env bash
# Route driver, emulator side: plays an expanded input-script v2 on the original game in the
# tokimemo-emu image (PCSX-Redux interpreter, OpenBIOS) with tools/emu/route_driver/player.lua,
# then converts screenshots (PNG) and decodes checkpoint RAM (decoded.json) in tokimemo-recovery.
#
# usage: tools/emu/route_driver/run.sh <out-dir (new, under the repo root)> <script.json> [vsync-limit]
# Environment:
#   ROUTE_START_STATE  save state loaded on vsync 1 (default: newgame-golden-v1 set A stage20-entry,
#                      so the player observes the switch into the first 0x34/1/0 exactly as the port does)
#   ROUTE_MEMCARD_DIR  dir with memcard1.mcd/memcard2.mcd copied into the run (default: newgame-golden-v1/inputs)
#   ROUTE_TRACE=0      no per-boundary route-trace.jsonl
#   ROUTE_DECODER      decode.py of the newgame golden (default .asset-cache/emu/newgame-golden-v1/tools/decode.py)
#   TOKIMEMO_ROOT      repository root holding disc/ and .asset-cache/ (default: this checkout; a worktree
#                      passes the main checkout)
# Inputs are copied into the run; existing caches and states are never written.
set -euo pipefail
HERE=$(cd "$(dirname "$0")" && pwd)
CODE=$(cd "$HERE/../../.." && pwd)
ROOT=${TOKIMEMO_ROOT:-$CODE}
[ $# -ge 2 ] || { sed -n '2,17p' "$0"; exit 2; }
OUT=$1; SCRIPT=$2; FRAMES=${3:-2000000}
case "$OUT" in /*) ;; *) OUT="$ROOT/$OUT" ;; esac
case "$OUT" in "$ROOT"/*) ;; *) echo "out dir must be under $ROOT" >&2; exit 2 ;; esac
[ -e "$OUT" ] && { echo "refusing to reuse $OUT" >&2; exit 2; }
G=$ROOT/.asset-cache/emu/newgame-golden-v1
START=${ROUTE_START_STATE:-$G/states/setA/stage20-entry.sstate}
CARDS=${ROUTE_MEMCARD_DIR:-$G/inputs}
DECODER=${ROUTE_DECODER:-$G/tools/decode.py}
[ -f "$START" ] && [ -f "$CARDS/memcard1.mcd" ] && [ -f "$SCRIPT" ] || { echo "missing start state, cards or script" >&2; exit 2; }
mkdir -p "$OUT/inputs"
cp "$START" "$OUT/inputs/start.sstate"
cp "$CARDS/memcard1.mcd" "$OUT/memcard1.mcd"; cp "$CARDS/memcard2.mcd" "$OUT/memcard2.mcd"
cp "$SCRIPT" "$OUT/inputs/script.json"
sha() { set -- $(shasum -a 256 "$1" 2>/dev/null || sha256sum "$1"); echo "$1"; }
{
	printf 'start %s %s\nscript %s %s\nplayer %s tools/emu/route_driver/player.lua\nhead %s\n' "$(sha "$START")" "$START" \
		"$(sha "$SCRIPT")" "$SCRIPT" "$(sha "$HERE/player.lua")" "$(git -C "$CODE" rev-parse HEAD)"
	printf 'emu-image %s\n' "$(docker image inspect --format '{{.Id}}' tokimemo-emu)"
} >"$OUT/provenance.txt"
REL=${OUT#"$ROOT"/}
W=/work/$REL
docker run --rm --name "tokimemo-routelua-$(basename "$OUT")" -u "$(id -u):$(id -g)" -v "$CODE":/code:ro -v "$OUT":/out tokimemo-recovery \
	python3 -I /code/tools/emu/route_script.py lua /out/inputs/script.json --out /out/inputs/script.lua
cat > "$OUT/emulator.cue" <<CUE
FILE "/work/disc/track1.bin" BINARY
  TRACK 01 MODE2/2352
    INDEX 01 00:00:00
FILE "/work/disc/track2.bin" BINARY
  TRACK 02 AUDIO
    INDEX 00 00:00:00
    INDEX 01 00:02:00
CUE
TRACE=true; [ "${ROUTE_TRACE:-1}" = 0 ] && TRACE=false
{ echo "JOB = { out = '$W', load_state = '$W/inputs/start.sstate', frames = $FRAMES, trace = $TRACE }"
  cat "$OUT/inputs/script.lua" "$HERE/player.lua"; } > "$OUT/job.lua"
NAME="tokimemo-routeemu-$(basename "$OUT")"
docker run --rm --name "$NAME" --cpus=2 -u "$(id -u):$(id -g)" -e HOME=/tmp \
	-e SDL_AUDIO_DRIVER=dummy -e SDL_AUDIODRIVER=dummy -e SDL_VIDEO_DRIVER=dummy \
	-v "$ROOT":/work -w "$W" tokimemo-emu \
	timeout 36000 nice -n 19 /opt/redux/pcsx-redux -no-ui -stdout -lua_stdout -run -interpreter -debugger \
	-bios /opt/redux/openbios.bin -iso "$W/emulator.cue" -memcard1 "$W/memcard1.mcd" -memcard2 "$W/memcard2.mcd" \
	-dofile "$W/job.lua" > "$OUT/emulator.log" 2>&1 || echo "emulator exit $?"
[ -f "$OUT/route.json" ] || { echo "no route.json; see $OUT/emulator.log" >&2; exit 1; }
# Screenshots to PNG and checkpoint RAM decoded with the source field names (read-only on the repo).
docker run --rm --name "tokimemo-routepost-$(basename "$OUT")" -u "$(id -u):$(id -g)" -v "$CODE":/code:ro -v "$OUT":/out \
	-v "$(dirname "$DECODER")":/decoder:ro tokimemo-recovery \
	python3 -I /code/tools/emu/route_driver/postprocess.py /out --decoder "/decoder/$(basename "$DECODER")" --repo /code \
	|| echo "postprocess exit $?"
python3 -I -c "import json,sys; r=json.load(open(sys.argv[1])); print('ROUTE native reason=%s stop=%s segment=%s/%s checkpoints=%d passed=%s' % (r['reason'], r['stop'], r['segment_at_stop'], r['segments_total'], len(r['checkpoints']), r['passed']))" "$OUT/route.json" 2>/dev/null || cat "$OUT/route.json" | head -c 400
