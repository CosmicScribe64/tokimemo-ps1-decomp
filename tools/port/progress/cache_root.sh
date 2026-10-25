#!/bin/sh
# Validate the local cache path before a Docker bind mount can resolve symlinks.
set -eu
if [ "$#" -lt 1 ] || [ "$#" -gt 2 ] || [ ! -d "$1" ]; then
  echo "repository root must be an existing directory" >&2
  exit 2
fi
ROOT=$(CDPATH= cd -P -- "$1" && pwd -P)
CACHE_DIR="$ROOT/.asset-cache"
MEDIA_DIR="$CACHE_DIR/port-progress"
WATCH_INPUTS="port-smoke port-preview-output port-captures"

check_component() {
  path=$1
  expected=$2
  if [ -L "$path" ]; then
    echo "refusing symlinked media path component: $path" >&2
    return 1
  fi
  if [ -e "$path" ]; then
    if [ ! -d "$path" ]; then
      echo "media path component is not a directory: $path" >&2
      return 1
    fi
    if ! physical=$(CDPATH= cd -P -- "$path" && pwd -P); then
      echo "cannot resolve media path component: $path" >&2
      return 1
    fi
    if [ "$physical" != "$expected" ]; then
      echo "media path escapes repository cache root: $path" >&2
      return 1
    fi
  fi
}

check_component "$CACHE_DIR" "$ROOT/.asset-cache" || exit 1
if [ -d "$CACHE_DIR" ]; then
  check_component "$MEDIA_DIR" "$ROOT/.asset-cache/port-progress" || exit 1
  if [ "${2:-media}" = "watch" ]; then
    for name in $WATCH_INPUTS; do
      check_component "$CACHE_DIR/$name" "$ROOT/.asset-cache/$name" || exit 1
    done
  fi
fi
printf '%s\n' "$MEDIA_DIR"
