#!/bin/sh
# T-25604 scratch: regdiff -v on a body file (put in a copy of the unit). usage: rd.sh BODY.c OUT.txt
set -e
cd "$(dirname "$0")/../../../../.."
B=$1; O=$2
python3 - "$B" <<'PY'
import sys; sys.path.insert(0,'drafts/T-25604/NAME_ENT/func_80143580/fresh-r1-search-on-new-best'); sys.path.insert(0,'tools')
import score2, funcloc
from pathlib import Path
c = funcloc.locate(score2.NAME, "NAME_ENT")
Path('build/T-25604/rd-src.c').write_text(score2.unit_text(c, Path(sys.argv[1]).read_text()))
PY
TOKIMEMO_SCRATCH=/work/build/scratch python3 tools/regdiff.py NAME_ENT:func_80143580 --src build/T-25604/rd-src.c -v > "$O" 2>&1 || true
cp build/scratch/regdiff/func_80143580/draft.log "$O.log"
