#!/bin/sh
# xtr.sh SRC CAPDIR : capture and run the instrumented as1 on it
set -e
python3 /work/build/as1tr/cap.py "$1" "$2" $3 >/dev/null
cd "$2" && cp in.T x.T && stdbuf -o0 -e0 /work/build/as1tr/bin/as1-x2 -elf -G 0 -p0 -EL -g0 -O2 -R -xbbdbg 8 in.G -o x.o -t x.T > xtr.log 2>&1
