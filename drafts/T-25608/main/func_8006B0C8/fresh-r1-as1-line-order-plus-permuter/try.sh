#!/bin/bash
# T-25608 scratch: try.sh NAME... (worktree root): expand $D/NAME.c and run ugforce.py, two at a time.
D=drafts/T-25608/main/func_8006B0C8/fresh-r1-as1-line-order-plus-permuter
one() { $D/mk.sh $D/$1.c $D/x$1.c; r=$(tools/docker.sh python3 $D/ugforce.py $1 $D/x$1.c 2>&1 | tail -1); s=$(grep '^26 ' build/t25608/ug-$1/uglog.txt 2>/dev/null | cut -c1-70); echo "$r | $s"; }
export -f one; export D
printf '%s\n' "$@" | xargs -P 2 -n 1 bash -c 'one "$0"'
