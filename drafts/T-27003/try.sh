#!/bin/bash
# usage: try.sh SRC_FILE...   compile the objects of the C files and compare them with the baseline
objs=""
for f in "$@"; do
  u=$(echo $f | sed -E 's#^src/ovl/([A-Za-z_]+)/.*#\1#')
  if [[ $f == src/main/* ]]; then o=build/$f; o=${o%.c}.o; else o=build/ovl/$u/${f%.c}.o; fi
  objs="$objs $o"
done
ninja -j2 $objs 2>&1 | grep -v "^\[" | grep -B1 -A3 "FAILED\|error\|Error" | head -30
python3 drafts/T-27003/objdiff.py $(for f in "$@"; do basename $f .c; done)
