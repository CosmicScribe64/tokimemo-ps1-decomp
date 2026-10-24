#!/bin/sh
# T-25608: every case file through 4.1's ugen, raw 5.3 ugen, the four pass rules and the 4.1-like rule
# (ugen_pass_41rule.py), then the ucode trace. Columns: 41 raw old A Aujp B r41.
# Usage (tokimemo-decomp-ido52 image, cwd /work, /wt = the tokimemo-wt worktrees):
#   sh drafts/T-25608/ido41/cases.sh tools/ido_eval_cases/ugen_copy_state/*.c
# The rule columns read the candidate passes from the T-25608 round-1 worktrees (mount them at /wt).
H=drafts/T-25608/ido41
OLD=tools/ugen_pass.py
A=/wt/f-b0c8-r1-as1-line-order-plus-/tools/ugen_pass.py
AU=/wt/f-b0c8-r1-copy-state-ring/tools/ugen_pass.py
B=/wt/f-b0c8-r1-label-and-base-hoist/tools/ugen_pass.py
R41=$H/ugen_pass_41rule.py
for c in "$@"; do
  k=build/t25608/$(basename "$c" .c)
  echo "== $c"
  python3 $H/run41.py --keep "$k" "$c" 41 raw $OLD $A $AU $B $R41 | awk '{s = $1; for (i = 3; i <= NF; i++) s = s " " $i; print "   " s}' | paste -sd' ' -
  python3 $H/utrace.py "$k/ucode/0.O" old=$OLD A=$A Aujp=$AU B=$B r41=$R41 | sed 's/^/   /'
done
