#!/bin/sh
# private diagnostic (T-25118): compile src/main/800563F0.c NON_MATCHING with the diag tool copy
# usage: run.sh <src.c> <tag>
set -e
SRC=$1; TAG=$2
D=build/T-25118/diag/out; mkdir -p $D
gcc -E -P -Iinclude -D__sgi -DNON_MATCHING "-D__attribute__(x)=" "-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)" "-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)" $SRC > $D/$TAG.c
python3 build/T-25118/diag/tools/cc.py $D/$TAG.c $D/$TAG.o ido 5.3
mips-linux-gnu-objdump -d -M no-aliases --no-show-raw-insn $D/$TAG.o | awk '/<strCallback>:/{f=1} f&&/^$/{exit} f' > $D/$TAG.dis
grep -E "lwl|lui" $D/$TAG.dis | head -6
