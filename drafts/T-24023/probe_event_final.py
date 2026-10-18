"""T-24023 final faithful source/representation alternatives."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('build/t24023/combined_selector_view/body.c').read_text();decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
new=body
for old,field in [('D_800B0F28','unk_6C8'),('D_800B0F29','unk_6C9'),('D_800B0F2A','unk_6CA')]:new=new.replace(old,'g->'+field)
variants={'aggregate_tail':new}
# Retain recovered frame while using logical comparisons with opposite operand order.
a=body.index('            if (!((g->unk_035 =');b=body.index('            if ((sp2B < 3U)',a)
block=body[a:b]
for day in (10,8,7):block=block.replace('g->unk_036 == 12 ? g->unk_037 == %d : 0'%day,'12 == g->unk_036 && %d == g->unk_037'%day)
variants['commute_logical']=body[:a]+block+body[b:]
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023')
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
