"""T-24023 localize the selector view to retain natural compare operands."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('build/t24023/combined/body.c').read_text();decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
view='(*(u8 *)&D_801243B0)'
variants={'selector_first_only':body.replace('D_801243B0',view,1),'selector_last_only':view.join(body.rsplit('D_801243B0',1))}
cut=body.index('block_448:\n')
variants['selector_tail_only']=body[:cut]+body[cut:].replace('D_801243B0',view)
variants['selector_before_tail']=body[:cut].replace('D_801243B0',view)+body[cut:]
for typ in ['s16','u16']:
    variants['selector_'+typ]=body.replace('D_801243B0','(('+typ+')*(u8 *)&D_801243B0)')
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023')
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
