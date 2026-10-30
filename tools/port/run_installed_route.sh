#!/bin/sh
# Actual installed Rei route: physical X11 entry, unskipped opening, observer
# port/godot/tests/opening/run_installed_rei_banner.gd with REI_DIALOGUE=1.
#
# usage: sh tools/port/run_installed_route.sh <new-out-dir> [bundle] [candidate.so] [candidate_bridge.gd]
#   new-out-dir  must not exist; created below the repository (for example
#                .asset-cache/port/rei-route-v5)
#   bundle       installed bundle copied into the out dir as prepared-original
#                (default .asset-cache/port/installed-runtime-v12)
#   candidate.so / candidate_bridge.gd
#                isolated native audio candidate and its BGM bridge, bound
#                read-only over the tracked files (defaults: the retained
#                d937156a candidate, whose SHA is enforced, and the
#                rei-dialogue-v2 bridge copy). Library and bridge SHAs are
#                always written to <out>/candidate-hashes.txt, with HEAD, a
#                working-tree diff hash and the executed opening source files.
# Environment: CANDIDATE_SHA (when set, the candidate library must have this
#              SHA-256; use it for any non-default candidate, e.g. the Shiori
#              race build .asset-cache/port/native-audio-shiori-v2, 67211ee6...),
#              REI_TARGET_STAGE (observer stops on this stage, default 10),
#              TOKIMEMO_ROUTE_DEADLINE (seconds, default 900),
#              GODOT_GUI_TEST_IMAGE (default tokimemo-port-gui-test),
#              ROUTE_OBSERVER=command (T-39002: run
#              tests/opening/run_installed_command_route.gd instead, with the
#              scripted golden set A entry, TIMED_ENTRY_PAD default 2 and
#              REI_TARGET_STAGE=22, to the first command-select frame; no
#              X11 key driver, the godot process is bounded by the deadline),
#              ROUTE_OBSERVER=playtest (the same route continued past command
#              select by tests/opening/run_installed_command_playtest.gd;
#              PLAYTEST_FRAMES/PLAYTEST_DAYS/PLAYTEST_PRESSES/
#              PLAYTEST_PRESS_EVERY/PLAYTEST_COMMANDS tune its scripted presses;
#              an app-failure stop exits non-zero unless PLAYTEST_EXPECT_STOP
#              is exactly "<gate>: <failure>", and then any other stop fails),
#              ROUTE_OBSERVER=modern (T-39060: the command route by
#              tests/qa/run_modern_command_route.gd with ROUTE_PREFS, a JSON
#              object of "section.key" preferences applied before the entry,
#              ROUTE_WINDOW=<W>x<H> for the window size, e.g. 1280x720 so
#              a wide aspect has wing columns, and VISUAL_QA=1 for window
#              captures under /out/qa),
#              ROUTE_OBSERVER=playthrough (the playtest above with ROUTE_PREFS
#              and ROUTE_WINDOW applied, by tests/qa/run_english_playthrough.gd;
#              PLAYTHROUGH_CAPTURES=1 writes window captures under /out/window
#              at each revealed opening line, each mode after command select
#              and every PLAYTHROUGH_EVERY frames),
#              ROUTE_SCREEN (Xvfb screen, default 1280x1024x24; visual QA
#              needs at least 1920x1080),
#              ROUTE_SOURCE_OVERRIDES (space-separated <file>=<repo path>
#              pairs, no spaces in either: each file is bound read-only over
#              that tracked path and recorded as a source-override line,
#              which replaces the source line of that path),
#              ROUTE_ASSET_CACHE (a checkout's .asset-cache, bound read-only
#              at .asset-cache for a worktree that has none).
# The tracked native library and bridge are never modified.
set -eu
cd "$(dirname "$0")/../.."
ROOT=$PWD
[ $# -ge 1 ] || { sed -n '2,20p' "$0"; exit 2; }
OUT=$1
BUNDLE=${2:-.asset-cache/port/installed-runtime-v12}
SO=${3:-.asset-cache/recovery/rei-fade-review-v3/candidate.so}
BRIDGE=${4:-.asset-cache/port/rei-dialogue-v2/candidate_bgm_bridge.gd}
SO_SHA=d937156a7aad5ebeb11c60676a1301c470002860b6cde09fcf14696286795480
[ ! -e "$OUT" ] || { echo "output exists: $OUT" >&2; exit 2; }
[ -f "$BUNDLE/manifest.json" ] || { echo "bundle has no manifest: $BUNDLE" >&2; exit 2; }
[ -f "$SO" ] && [ -f "$BRIDGE" ] || { echo "missing candidate library or bridge" >&2; exit 2; }
sha() { set -- $(shasum -a 256 "$1" 2>/dev/null || sha256sum "$1"); echo "$1"; }
SO_ACTUAL=$(sha "$SO")
# The retained default candidate is pinned; an explicit candidate is only recorded.
if [ "$SO" = ".asset-cache/recovery/rei-fade-review-v3/candidate.so" ] && [ "$SO_ACTUAL" != "$SO_SHA" ]; then
	echo "default candidate library SHA $SO_ACTUAL differs from $SO_SHA" >&2; exit 2
fi
if [ -n "${CANDIDATE_SHA:-}" ] && [ "$SO_ACTUAL" != "$CANDIDATE_SHA" ]; then
	echo "candidate library SHA $SO_ACTUAL differs from CANDIDATE_SHA $CANDIDATE_SHA" >&2; exit 2
fi
mkdir -p "$OUT/content-install/.staging"
cp -R "$BUNDLE" "$OUT/content-install/.staging/prepared-original"
cp "$BRIDGE" "$OUT/candidate_bgm_bridge.gd"
# The bundle line records the SHA of the bundle's manifest.json.
printf 'library %s %s\nbridge %s %s\nbundle %s %s\n' "$SO_ACTUAL" "$SO" "$(sha "$OUT/candidate_bgm_bridge.gd")" "$BRIDGE" "$(sha "$BUNDLE/manifest.json")" "$BUNDLE" >"$OUT/candidate-hashes.txt"
# The route mounts the working tree, so also pin the uncommitted runtime
# sources it executes: HEAD, a hash of the whole tracked+untracked port/godot
# diff, and per-file hashes of the opening audio/boot files.
{
	printf 'head %s\n' "$(git rev-parse HEAD)"
	DIFF=$(mktemp); git diff HEAD -- port/godot tools/port >"$DIFF"
	git ls-files --others --exclude-standard -- port/godot | LC_ALL=C sort | while read -r f; do printf 'untracked %s %s\n' "$(sha "$f")" "$f"; done >>"$DIFF"
	printf 'worktree-diff %s git-diff-HEAD+untracked port/godot tools/port\n' "$(sha "$DIFF")"; rm -f "$DIFF"
	for f in port/godot/services/first_arrival_bgm_bridge.gd port/godot/services/opening_audio_rules.gd port/godot/services/opening_audio_bridge.gd \
		port/godot/services/prologue_shiori_bank_resources.gd port/godot/app/first_arrival_boot.gd port/godot/flow/prologue_arrival.gd \
		port/godot/services/first_arrival_media_bridge.gd port/godot/bgm_backend.gd port/godot/bgm_sink.gd; do
		case " ${ROUTE_SOURCE_OVERRIDES:-} " in *"=$f "*) continue ;; esac
		printf 'source %s %s\n' "$(sha "$f")" "$f"
	done
} >>"$OUT/candidate-hashes.txt"
EXTRA_MOUNTS=
for pair in ${ROUTE_SOURCE_OVERRIDES:-}; do
	file=${pair%%=*}; target=${pair#*=}
	[ -f "$file" ] && [ -f "$target" ] || { echo "source override or its tracked target is missing: $pair" >&2; exit 2; }
	case $file in /*) abs=$file ;; *) abs=$ROOT/$file ;; esac
	printf 'source-override %s %s %s\n' "$(sha "$file")" "$file" "$target" >>"$OUT/candidate-hashes.txt"
	EXTRA_MOUNTS="$EXTRA_MOUNTS -v $abs:/work/$target:ro"
done
if [ -n "${ROUTE_ASSET_CACHE:-}" ]; then
	[ -d "$ROUTE_ASSET_CACHE" ] || { echo "ROUTE_ASSET_CACHE is not a directory" >&2; exit 2; }
	EXTRA_MOUNTS="$EXTRA_MOUNTS -v $ROUTE_ASSET_CACHE:/work/.asset-cache:ro"
fi
case $OUT in /*) ABS_OUT=$OUT ;; *) ABS_OUT=$ROOT/$OUT ;; esac
case $SO in /*) ABS_SO=$SO ;; *) ABS_SO=$ROOT/$SO ;; esac
case ${ROUTE_OBSERVER:-} in
	command) OBSERVER=res://tests/opening/run_installed_command_route.gd ;;
	playtest) OBSERVER=res://tests/opening/run_installed_command_playtest.gd ;;
	modern) OBSERVER=res://tests/qa/run_modern_command_route.gd ;;
	playthrough) OBSERVER=res://tests/qa/run_english_playthrough.gd ;;
	*) OBSERVER= ;;
esac
if [ -n "$OBSERVER" ]; then
	exec docker run --rm --name "tokimemo-route-$(basename "$OUT")" --cpus=4 -e REI_DIALOGUE=1 -e REI_TARGET_STAGE=22 -e TIMED_ENTRY_PAD="${TIMED_ENTRY_PAD:-2}" \
		-e OBSERVER="$OBSERVER" -e PLAYTEST_FRAMES="${PLAYTEST_FRAMES:-}" -e PLAYTEST_DAYS="${PLAYTEST_DAYS:-}" \
		-e PLAYTEST_PRESSES="${PLAYTEST_PRESSES:-}" -e PLAYTEST_PRESS_EVERY="${PLAYTEST_PRESS_EVERY:-}" \
		-e PLAYTEST_EXPECT_STOP="${PLAYTEST_EXPECT_STOP:-}" -e PLAYTEST_COMMANDS="${PLAYTEST_COMMANDS:-}" \
		-e ROUTE_PREFS="${ROUTE_PREFS:-}" -e ROUTE_WINDOW="${ROUTE_WINDOW:-}" -e VISUAL_QA="${VISUAL_QA:-}" \
		-e PLAYTHROUGH_CAPTURES="${PLAYTHROUGH_CAPTURES:-}" -e PLAYTHROUGH_EVERY="${PLAYTHROUGH_EVERY:-}" -e ROUTE_SCREEN="${ROUTE_SCREEN:-1280x1024x24}" \
		-e GODOT_SILENCE_ROOT_WARNING=1 -e TOKIMEMO_ENTRY_DEBUG=1 -e DEADLINE="${TOKIMEMO_ROUTE_DEADLINE:-2400}" \
		-v "$ROOT:/work" -v "$ABS_OUT:/out" \
		-v "$ABS_SO:/work/port/godot/bin/liboriginal_audio.linux.template_release.arm64.so:ro" \
		-v "$ABS_OUT/candidate_bgm_bridge.gd:/work/port/godot/services/first_arrival_bgm_bridge.gd:ro" $EXTRA_MOUNTS \
		"${GODOT_GUI_TEST_IMAGE:-tokimemo-port-gui-test}" sh -ec '
		Xvfb :99 -screen 0 "$ROUTE_SCREEN" -nolisten tcp -ac >/out/xvfb.log 2>&1 &
		export DISPLAY=:99; sleep 1
		pulseaudio -D --exit-idle-time=-1 --system=false 2>/dev/null || true
		pactl load-module module-null-sink sink_name=virtual >/dev/null 2>&1 || true
		status=0
		timeout "$DEADLINE" godot --path /work/port/godot --audio-driver PulseAudio --rendering-method gl_compatibility --script "$OBSERVER" > /out/godot.log 2>&1 || status=$?
		grep -E "opening frame timing|^PASS|^FAIL|^PLAYTEST|SCRIPT ERROR|stopped at|New Game stopped" /out/godot.log | head -20
		exit $status'
fi
exec docker run --rm --name "tokimemo-route-$(basename "$OUT")" --cpus=4 -e REI_DIALOGUE=1 -e REI_TARGET_STAGE="${REI_TARGET_STAGE:-10}" -e TOKIMEMO_ROUTE_DEADLINE="${TOKIMEMO_ROUTE_DEADLINE:-900}" \
	-v "$ROOT:/work" -v "$ABS_OUT:/out" \
	-v "$ABS_SO:/work/port/godot/bin/liboriginal_audio.linux.template_release.arm64.so:ro" \
	-v "$ABS_OUT/candidate_bgm_bridge.gd:/work/port/godot/services/first_arrival_bgm_bridge.gd:ro" $EXTRA_MOUNTS \
	"${GODOT_GUI_TEST_IMAGE:-tokimemo-port-gui-test}" sh -ec '
	Xvfb :99 -screen 0 1280x1024x24 -nolisten tcp -ac >/out/xvfb.log 2>&1 &
	export DISPLAY=:99; sleep 1
	pulseaudio -D --exit-idle-time=-1 --system=false 2>/dev/null || true
	pactl load-module module-null-sink sink_name=virtual >/dev/null 2>&1 || true
	python3 /work/tools/port/drive_installed_first_scene.py --rei'
