#!/bin/sh
# usage (host, repo root, TOKIMEMO_SCRATCH set to a directory holding r9-uopt/variants/trace):
#   sh drafts/T-25116/main/func_800476C0/rd.sh BODY.c
# regdiff (traced uopt) for main:func_800476C0 with BODY.c in place of the INCLUDE_ASM line, via a scratch copy
# of src/main/80047550.c (drafts/T-25317/main/func_800476C0/regsrc.c, not committed). T-25317 (from T-25116).
set -e
python3 - "$1" <<'PY'
import sys
from pathlib import Path
t = Path('src/main/80047550.c').read_text()
inc = 'INCLUDE_ASM("asm/nonmatchings/main/80047550", func_800476C0);\n'
a = t.index(inc)
body = Path(sys.argv[1]).read_text().rstrip('\n')
Path('drafts/T-25317/main/func_800476C0/regsrc.c').write_text(
    t[:a] + "u8 func_8004901C(void);\n#ifdef NON_MATCHING\n" + body + "\n#else\n" + inc + "#endif\n" + t[a + len(inc):])
PY
tools/docker.sh python3 tools/regdiff.py main:func_800476C0 --src drafts/T-25317/main/func_800476C0/regsrc.c -v
