#!/bin/bash
# offset-independent disassembly diff; usage: odiff2.sh OBJ_RELATIVE_PATH [LINES]
f=$1
mips-linux-gnu-objdump -dr .base_build/$f > drafts/T-27003/tmp/a.txt
mips-linux-gnu-objdump -dr build/$f > drafts/T-27003/tmp/b.txt
python3 - <<PY
ren={}
for t in ("obin","sdk"):
    for l in open("drafts/T-27003/%s.tsv"%t):
        if l.startswith("#"): continue
        x=l.split(); ren[x[2]]=x[3]
a=open("drafts/T-27003/tmp/a.txt").read()
for k,v in ren.items(): a=a.replace(k,v)
open("drafts/T-27003/tmp/a2.txt","w").write(a)
PY
diff <(sed 1,3d drafts/T-27003/tmp/a2.txt | sed -E "s/^ +[0-9a-f]+:\t//; s/^\t+[0-9a-f]+: /\t/; s/<func_[0-9A-F]+\+0x[0-9a-f]+>//; s/ [0-9a-f]+ </ X </") <(sed 1,3d drafts/T-27003/tmp/b.txt | sed -E "s/^ +[0-9a-f]+:\t//; s/^\t+[0-9a-f]+: /\t/; s/<func_[0-9A-F]+\+0x[0-9a-f]+>//; s/ [0-9a-f]+ </ X </") | head -${2:-40}
