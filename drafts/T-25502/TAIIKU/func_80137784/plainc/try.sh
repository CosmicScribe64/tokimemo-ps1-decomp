#!/bin/bash
# usage: try.sh VARIANT.c [PROTO]  -> swaps the variant in, prints funcdiff row count (T-25502 experiment helper)
W=/Users/lila/Documents/Projects/tokimemo-wt/w12-taiiku-plainc
D=$W/drafts/T-25502/TAIIKU/func_80137784/plainc
V=$(cd "$(dirname "$1")" && pwd)/$(basename "$1")
cd $W
if [ -n "$2" ]; then python3 $D/swap.py "$V" --proto "$2"; else python3 $D/swap.py "$V"; fi || exit 1
out=$(tools/docker.sh python3 tools/funcdiff.py func_80137784 --unit TAIIKU --resolve --version c 2>&1)
echo "$out" > "$D/last.diff"
rows=$(echo "$out" | grep -c '^  [-+][0-9a-f@]')
echo "$(basename $1): rows=$rows $(echo "$out" | grep -m1 -E 'MATCH|error|Error' )"
