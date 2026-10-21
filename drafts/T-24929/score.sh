#!/bin/sh
# Scratch (T-24929): compile func_8013CCE4 under ENV (uopt_flip shim knobs) and print the diff row count + frame.
# usage (host): drafts/T-24929/score.sh TAG "ENV=VAL ..." [FIND REPLACE]
T=$1; E=$2; F=${3:-}; R=${4:-}
tools/docker.sh sh -c "$E UCD_TAG=$T UCD_OUT=drafts/T-24929/ucdump/$T UCD_FIND='$F' UCD_REPLACE='$R' python3 drafts/T-24929/ucdump.py >drafts/T-24929/ucdump/$T.log 2>&1; python3 tools/funcdiff.py --resolve --unit ETC --built drafts/T-24929/ucdump/$T/guarded_$T.o func_8013CCE4 > drafts/T-24929/ucdump/$T.diff 2>&1; printf '%s rows=%s frame=%s\n' '$E' \$(grep -c '^  [-+]' drafts/T-24929/ucdump/$T.diff) \$(mips-linux-gnu-objdump -d drafts/T-24929/ucdump/$T/guarded_$T.o | awk '/<func_8013CCE4>:/,0' | grep -m1 'addiu.*sp,sp,-' | awk '{print \$NF}')"
