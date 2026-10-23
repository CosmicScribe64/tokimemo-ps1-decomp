#!/bin/sh
# usage (host, repo root, TOKIMEMO_SCRATCH set): sh drafts/T-25317/main/func_800476C0/tr.sh BODY.c [BODY2.c ...]
# T-25317: traced-uopt compile of src/main/80047550.c with BODY.c in place of func_800476C0's INCLUDE_ASM;
# prints the s-register ranges of func_800476C0 (bit, priority, D, units with frequency).
set -e
for b in "$@"; do
python3 - "$b" <<'PY'
import sys
from pathlib import Path
t = Path('src/main/80047550.c').read_text()
inc = 'INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);\n'
a = t.index(inc)
body = Path(sys.argv[1]).read_text().rstrip('\n')
Path('build/trsrc.c').write_text(t[:a] + "u8 func_8004901C(void);\n" + body + "\n" + t[a + len(inc):])
PY
tools/docker.sh sh -c 'python3 tools/uopt_eval.py compile build/trsrc.c build/trsrc.o --uopt /scratch/r9-uopt/variants/trace --trace > build/trsrc.log 2>&1' || { echo "$b: compile failed"; tail -3 build/trsrc.log; continue; }
python3 - "$b" <<'PY'
import re, sys
t = open("build/trsrc.log").read().split("\n")
cur = None; rng = None; R = {}; C = {}
for l in t:
    if l.startswith("R9PROC"): cur = l.split()[1]
    if cur != "func_800476C0": continue
    if l.startswith("R9SAVE"):
        m = re.search(r"bit=(\d+) lr=(\d+) blocks=(\d+) priority=(\S+)", l); rng = (m.group(1), m.group(2)); R[rng] = [m.group(3), float(m.group(4)), []]
    elif l.startswith("R9UNIT") and rng:
        m = re.search(r"bb=(\d+) freq=(\d+) loads=(\d+) stores=(\d+)", l); R[rng][2].append("%s:%s/%s%s" % (m.group(1), m.group(2), m.group(3), "s" if m.group(4) != "0" else ""))
    elif l.startswith("R9COST"):
        m = re.search(r"bit=(\d+) lr=(\d+) cost=\S+ reg=(\d+)", l); C[(m.group(1), m.group(2))] = int(m.group(3))
names = {14+i: "s%d" % i for i in range(8)}; names[22] = "fp"
print("==", sys.argv[1])
for k, v in sorted(R.items(), key=lambda kv: -kv[1][1]):
    r = C.get(k)
    if r in names:
        print("  %-3s bit %-4s prio %7.3f D %-3s %s" % (names[r], k[0], v[1], v[0], " ".join(v[2][:9])))
PY
done
