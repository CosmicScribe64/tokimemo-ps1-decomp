#!/bin/sh
# toys.sh FILE.c... : compact result per toy: weights, moves, head instructions up to jr
for f in "$@"; do
d=/work/build/as1tr/toys/out/$(basename $f .c); mkdir -p $d
AS1CAP=$d python3 /work/build/as1tr/cc_cap.py $f $d/unit.o ido 5.3 >/dev/null 2>$d/err || { echo "$f: COMPILE FAIL"; head -3 $d/err; continue; }
(cd $d && cp in.T x.T && stdbuf -o0 /work/build/as1tr/bin/as1-x2 -elf -G 0 -p0 -EL -g0 -O2 -R -xbbdbg 8 in.G -o x.o -t x.T > xtr.log 2>&1)
echo "== $f"
grep -h "	bb: \|MOVETO\|MOVEFROM" $d/xtr.log | tr '\n' ' ' | sed 's/	bb: /b/g; s/	weight: /w/g; s/  time: old [0-9]*, new [0-9]*//g; s/inst /i/g'; echo
mips-linux-gnu-objdump -d --no-show-raw-insn $d/unit.o | awk '/<f>:/{p=1;next} p&&/jr\t/{print; exit} p' | sed -n '4,30p' | cut -f3- | tr '\n' ';' ; echo
done
