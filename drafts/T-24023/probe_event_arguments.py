"""T-24023 argument conversions suppress precolouring of the girl global."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('build/t24023/bool_assignment/body.c').read_text();decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
variants={}
for typ in ['s32','u32','s16','u16','u8']:
    new=body
    for call in ['func_8002328C','func_8002336C']:new=new.replace(call+'(g->unk_EE6)',call+'(('+typ+')g->unk_EE6)')
    variants['arg_'+typ]=new
new=body
for call in ['func_8002328C','func_8002336C']:new=new.replace(call+'(g->unk_EE6)',call+'(*(u8 *)&g->unk_EE6)')
variants['arg_byte_view']=new
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023')
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
