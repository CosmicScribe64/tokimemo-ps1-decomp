#!/bin/sh
cd /Users/lila/Documents/Projects/tokimemo-decomp
tools/docker.sh sh -c 'PREFIX='"${PREFIX:-0}"' python3 build/scratch/t25012/score.py "$@"' -- "$@"
