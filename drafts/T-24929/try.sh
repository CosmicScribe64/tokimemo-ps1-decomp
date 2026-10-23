#!/bin/sh
# Scratch (T-24929): compile func_8013CCE4 with one text replacement, print its first 16 instructions.
# usage (host): drafts/T-24929/try.sh TAG FIND REPLACE [ENV=VAL]
T=$1; F=$2; R=$3; E=${4:-X=1}
tools/docker.sh sh -c "$E UCD_TAG=$T UCD_OUT=drafts/T-24929/ucdump/$T UCD_FIND='$F' UCD_REPLACE='$R' python3 drafts/T-24929/ucdump.py >drafts/T-24929/ucdump/$T.log 2>&1; mips-linux-gnu-objdump -d --no-show-raw-insn drafts/T-24929/ucdump/$T/guarded_$T.o" | awk '/<func_8013CCE4>:/,0' | sed -n 2,17p | cut -f2- | tr '\n' ';'; echo
