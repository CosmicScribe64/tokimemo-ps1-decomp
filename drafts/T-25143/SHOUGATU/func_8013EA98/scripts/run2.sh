#!/bin/sh
# run variants two at a time: run2.sh name1 name2 ...
cd "$(git rev-parse --show-toplevel)"
while [ $# -gt 0 ]; do
  a=$1; shift; b=$1; [ $# -gt 0 ] && shift
  tools/docker.sh python3 build/ea98/m.py $a build/ea98/$a.c 2>&1 | tail -1 &
  [ -n "$b" ] && tools/docker.sh python3 build/ea98/m.py $b build/ea98/$b.c 2>&1 | tail -1 &
  wait
done
