#!/bin/sh
# Scratch (T-24929): diff one function between base and VARIANT objects. usage: fdiff.sh VARIANT OBJ FUNC
tools/docker.sh sh -c "for d in base $1; do mips-linux-gnu-objdump -d --no-show-raw-insn build/t24929/\$d/$2 | awk '/<$3>:/,/^\$/' | cut -f2- | sed 's/[0-9a-f]* <//' > /tmp/\$d.txt; done; diff /tmp/base.txt /tmp/$1.txt"
