#!/bin/sh
# g.sh body.c from to : dump ugen binasm of func_80134D94
cd /Users/lila/Documents/Projects/tokimemo-wt/w10-BUNKA_SD-80134540
n=$(basename "$1" .c)
s=$(tools/docker.sh python3 build/T-25306/mk.py "$1")
tools/docker.sh sh -c "python3 build/T-25306/gc.py $s build/T-25306/g_$n.o build/T-25306/g_$n.G >/dev/null 2>&1; python3 build/T-25306/gdump.py build/T-25306/g_$n.G $2 $3"
