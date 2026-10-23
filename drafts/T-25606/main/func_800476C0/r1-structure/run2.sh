#!/bin/bash
# usage (host, repo root): bash run2.sh NAME... ; scores build/s/v/NAME.c into build/s/NAME in two Docker jobs
# (T-25606 r1-structure helper; prints one line per variant)
D=drafts/T-25606/main/func_800476C0/r1-structure
names=("$@"); half=$(( (${#names[@]} + 1) / 2 ))
argsA=(); argsB=()
for k in "${!names[@]}"; do
  n=${names[$k]}
  if [ $k -lt $half ]; then argsA+=(build/s/v/$n.c build/s/$n); else argsB+=(build/s/v/$n.c build/s/$n); fi
done
tools/docker.sh python3 $D/score.py "${argsA[@]}" > build/s/runA.log 2>&1 &
if [ ${#argsB[@]} -gt 0 ]; then tools/docker.sh python3 $D/score.py "${argsB[@]}" > build/s/runB.log 2>&1; else : > build/s/runB.log; fi
wait
cat build/s/runA.log build/s/runB.log | grep "want=\|ERROR" | sed 's|build/s/v/||; s|want=783 ||'
