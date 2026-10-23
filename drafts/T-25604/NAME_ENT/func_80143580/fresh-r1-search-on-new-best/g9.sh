#!/bin/sh
# T-25604 scratch: compile BODY.c (optionally with the rev1 delta) with the binasm-saving as1 shim and print
# ugen's case 9 else block (from the 0x80000000 load before the 0x01000000 load to the next branch).
# usage (host): g9.sh BODY.c [--rev1]
cd "$(dirname "$0")/../../../../.."
S=drafts/T-25604/NAME_ENT/func_80143580/fresh-r1-search-on-new-best
n=$(basename "$1" .c)$2
tools/docker.sh sh -c "python3 $S/gc.py $1 build/T-25604/g/$n $2 >/dev/null 2>&1; python3 $S/gdump.py build/T-25604/g/$n.G > build/T-25604/g/$n.dump 2>&1"
python3 - build/T-25604/g/$n.dump <<'PY'
import sys
L = open(sys.argv[1]).read().splitlines()
k = max(i for i, l in enumerate(L) if 'imm=72 sym=115' in l)
s = max(i for i in range(k) if ' jal ' in L[i]); e = min(i for i in range(k, len(L)) if ' b  ' in L[i])
print('; '.join(' '.join(L[i].split()[1:2] + L[i].split()[3:5]) for i in range(s + 1, e)))
PY
