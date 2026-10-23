#!/bin/sh
# usage (host, repo root, TOKIMEMO_SCRATCH set; src/main/80047550.c with the plain INCLUDE_ASM):
#   sh tr.sh BODY.c OUT.log   -- traced-uopt compile of the unit with BODY.c (T-25606 r1-structure, from T-25317's tr.sh)
python3 - "$1" <<'PY'
import sys
from pathlib import Path
t = Path('src/main/80047550.c').read_text()
inc = 'INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);\n'
a = t.index(inc)
body = Path(sys.argv[1]).read_text().rstrip('\n')
Path('build/trsrc.c').write_text(t[:a] + "u8 func_8004901C(void);\n" + body + "\n" + t[a + len(inc):])
PY
tools/docker.sh sh -c "python3 tools/uopt_eval.py compile build/trsrc.c build/trsrc.o --uopt /scratch/r9-uopt/variants/trace --trace > $2 2>&1"
