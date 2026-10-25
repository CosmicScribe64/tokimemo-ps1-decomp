#!/bin/sh
# Launch a code-only development export with external New Game inputs.
# The shipping app uses first-launch setup; this wrapper is for native verification.
set -eu
if [ "$#" -lt 5 ]; then
  echo "usage: $0 APP_BUNDLE RULES_JSON ENTRY_JSON FONT_DIRECTORY SEED [ENGINE_ARGUMENTS...]" >&2
  exit 2
fi
bundle=$1
rules=$2
entry=$3
font=$4
seed=$5
shift 5
for path in "$bundle" "$rules" "$entry" "$font"; do
  case "$path" in /*) ;; *) echo "Use absolute input paths." >&2; exit 2 ;; esac
done
case "$seed" in ''|*[!0-9]*) echo "Seed must be an unsigned decimal integer." >&2; exit 2 ;; esac
if [ "${#seed}" -gt 10 ] || [ "$seed" -gt 4294967295 ]; then
  echo "Seed must fit an unsigned 32-bit word." >&2
  exit 2
fi
[ -d "$bundle" ] && [ -f "$rules" ] && [ -f "$entry" ] && [ -d "$font" ] || {
  echo "A required application or content path is missing." >&2
  exit 2
}
executable="$bundle/Contents/MacOS/Tokimeki Memorial Port Foundation"
[ -x "$executable" ] || { echo "The development executable is unavailable." >&2; exit 2; }
# Launch the executable directly: Launch Services may reuse an existing process
# without the caller's environment. Strict content validation remains in the game.
export TOKIMEMO_NEW_GAME_RULES="$rules"
export TOKIMEMO_NEW_GAME_ENTRY="$entry"
export TOKIMEMO_NATIVE_FONT="$font"
export TOKIMEMO_NEW_GAME_SEED="$seed"
exec "$executable" "$@"
