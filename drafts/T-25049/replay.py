"""Recover the event selector's byte-table views; run only in Docker."""
from pathlib import Path
import re
import sys
sys.path.insert(0, 'tools')
import funcdiff as f
import funcdiff_verify as v
name='func_80073CB0'
c=f.locate(name,'main')
body=Path('drafts/T-17000/main/func_80073CB0/body.c').read_text()
for base in (0x800B68C0,0x800B68D0):
    for offset in (0,2,4,6,9,11):
        body=body.replace('*(&D_%08X + var_v1)'%(base+offset),'D_%08X[%d + var_v1]'%(base,offset))
    body=body.replace('D_%08X;'%(base+13),'D_%08X[13];'%base)
body=body.replace("D_800E6280.unk_000 >> 0xC", "D_800E6280.unk_0F4.h >> 0xC")
body='extern u8 D_800B68C0[16];\nextern u8 D_800B68D0[16];\n'+body
out=Path('build/T-25049');out.mkdir(parents=True,exist_ok=True)
(out/'body.c').write_text(body)
marker='INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_80073CB0);'
source=out/'full.c'
source.write_text(Path(c.src).read_text().replace(marker,body))
expected,problem=f.expected_object(c);assert problem is None,problem
obj,_=v.compile_unit(c,out,source=source)
a=f.resolved_functions(expected,[name],c)[name]
b=f.resolved_functions(obj,[name],c)[name]
print('target',len(a),'candidate',len(b),'cost',f.diff_score(a,b))
(out/'diff.txt').write_text('\n'.join(f.diff_lines(a,b)))
