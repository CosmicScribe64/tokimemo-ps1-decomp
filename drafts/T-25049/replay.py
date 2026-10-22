"""Recover the event selector's byte-table views; run only in Docker."""
from pathlib import Path
import re
import sys
sys.path.insert(0, 'tools')
import funcdiff as f
import funcdiff_verify as v
name='func_80073CB0'
c=f.locate(name,'main')
body=Path('drafts/T-25049/body.c').read_text()
out=Path('build/T-25049');out.mkdir(parents=True,exist_ok=True)
(out/'body.c').write_text(body)
marker='INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_80073CB0);'
source=out/'full.c'
base=Path(c.src).read_text()
if marker in base:
    source.write_text(base.replace(marker,body))
else:
    start=base.index('void func_80073CB0(void) {')
    end=base.index('\nu8 func_800741B8',start)
    source.write_text(base[:start]+body+'\n'+base[end:])
expected,problem=f.expected_object(c);assert problem is None,problem
obj,_=v.compile_unit(c,out,source=source)
a=f.resolved_functions(expected,[name],c)[name]
b=f.resolved_functions(obj,[name],c)[name]
print('target',len(a),'candidate',len(b),'cost',f.diff_score(a,b))
(out/'diff.txt').write_text('\n'.join(f.diff_lines(a,b)))
