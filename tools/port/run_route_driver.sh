#!/bin/sh
# Route driver: one input script on the installed port route and on the original game, then a
# checkpoint-by-checkpoint comparison. Every step runs in Docker.
#
# usage: sh tools/port/run_route_driver.sh <new-out-dir> <script.json> [--port|--emu|--both]
#   <new-out-dir>  must not exist; gets <name>-port/ (installed route; the container is tokimemo-route-<name>-port), native/ (emulator) and compare.json
#   <script.json>  expanded input-script v2 (config/port/input-scripts/three_year_default.json;
#                  regenerate with tools/port/route_strategy.py)
#   --port         port only, compared with ROUTE_NATIVE when that is set
#   --emu          emulator only
#   --both         (default) both sides, then compare
# Environment:
#   TOKIMEMO_ROOT  checkout holding disc/ and .asset-cache/ (default: this checkout; from a worktree,
#                  the main checkout). Native output must be under it.
#   ROUTE_NATIVE   an existing native run to compare with instead of running the emulator
#                  (e.g. .asset-cache/emu/route-golden-three_year_default_continuous-v1)
#   ROUTE_BUNDLE / ROUTE_SO / ROUTE_BRIDGE / ROUTE_SO_SHA
#                  installed bundle, native audio candidate and bridge for the port route (defaults:
#                  the T-39002 r13 set: installed-runtime-v18, native-audio-core-v1, native-core-bridge-v1)
#   ROUTE_FRAMES   port: iterations past command select; ROUTE_VSYNCS: emulator vsync limit
# Exit: 0 when each side that ran passed (script complete or a pinned stop) and the paired checkpoints
# match; otherwise the first failing step's status.
set -eu
CODE=$(cd "$(dirname "$0")/../.." && pwd)
ROOT=${TOKIMEMO_ROOT:-$CODE}
[ $# -ge 2 ] || { sed -n '2,25p' "$0"; exit 2; }
OUT=$1; SCRIPT=$2; MODE=${3:---both}
case $OUT in /*) ;; *) OUT=$ROOT/$OUT ;; esac
case $SCRIPT in /*) ;; *) SCRIPT=$CODE/$SCRIPT ;; esac
[ ! -e "$OUT" ] || { echo "output exists: $OUT" >&2; exit 2; }
[ -f "$SCRIPT" ] || { echo "no script $SCRIPT" >&2; exit 2; }
mkdir -p "$OUT"
NAME=$(basename "$OUT")
py() { n=$1; shift; docker run --rm --name "tokimemo-routedrv-$NAME-$n" -u "$(id -u):$(id -g)" -v "$CODE":/code:ro -v "$ROOT/.asset-cache":/data:ro -v "$OUT":/out -w /code tokimemo-recovery python3 -I "$@"; }
inside() { case $1 in "$ROOT"/.asset-cache/*) echo "/data/${1#"$ROOT"/.asset-cache/}" ;; "$OUT"/*) echo "/out/${1#"$OUT"/}" ;; "$CODE"/*) echo "/code/${1#"$CODE"/}" ;; *) echo "$1" ;; esac; }
cp "$SCRIPT" "$OUT/script.json"
echo "== preflight (expected stops)"
docker run --rm --name "tokimemo-routedrv-$NAME-pf" -v "$CODE":/code:ro -v "$OUT":/out tokimemo-recovery python3 -I /code/tools/emu/route_script.py preflight /out/script.json | tee "$OUT/preflight.json" | sed -n '/expected_unsupported_stops/,$p'
STATUS=0
if [ "$MODE" != --emu ]; then
	echo "== port (installed route, about 25 minutes to command select)"
	BUNDLE=${ROUTE_BUNDLE:-$ROOT/.asset-cache/core-round3-2026-10-30/builder-cache/installed-runtime-v18}
	SO=${ROUTE_SO:-$ROOT/.asset-cache/port/native-audio-core-v1/project/bin/liboriginal_audio.linux.template_release.arm64.so}
	BRIDGE=${ROUTE_BRIDGE:-$ROOT/.asset-cache/port/native-core-bridge-v1/candidate_bgm_bridge.gd}
	REL=${SCRIPT#"$CODE"/}
	ROUTE_OBSERVER=long ROUTE_SCRIPT=$REL ROUTE_FRAMES=${ROUTE_FRAMES:-} TOKIMEMO_ROUTE_DEADLINE=${TOKIMEMO_ROUTE_DEADLINE:-36000} \
		CANDIDATE_SHA=${ROUTE_SO_SHA:-ab2d3cbcb3837edb4c69e3b493f231646cb6abada355e4c9a65d93e84ed89b2e} \
		sh "$CODE/tools/port/run_installed_route.sh" "$OUT/$NAME-port" "$BUNDLE" "$SO" "$BRIDGE" || STATUS=$?
fi
NATIVE=${ROUTE_NATIVE:-}
case $NATIVE in ""|/*) ;; *) NATIVE=$ROOT/$NATIVE ;; esac
if [ "$MODE" != --port ] && [ -z "$NATIVE" ]; then
	echo "== emulator (original game)"
	NATIVE_OUT=$ROOT/.asset-cache/emu/route-run-$NAME
	TOKIMEMO_ROOT=$ROOT "$CODE/tools/emu/route_driver/run.sh" "$NATIVE_OUT" "$SCRIPT" "${ROUTE_VSYNCS:-2000000}" || STATUS=$?
	ln -s "$NATIVE_OUT" "$OUT/native"
	NATIVE=$NATIVE_OUT
fi
if [ -d "$OUT/$NAME-port" ] && [ -n "$NATIVE" ] && [ -f "$NATIVE/route.json" ]; then
	echo "== compare"
	py compare tools/port/compare_route_checkpoints.py "/out/$NAME-port" "$(inside "$NATIVE")" --json /out/compare.json || { s=$?; [ $STATUS -ne 0 ] || STATUS=$s; }
fi
exit $STATUS
