"""T-24023 prevent cross-arm Boolean CSE with comparison promotion types."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('build/t24023/combined_selector_view/body.c').read_text();decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
a=body.index('            if (!((g->unk_035 =');b=body.index('            if ((sp2B < 3U)',a)
variants={}
for label,exprs in [
('mixed_sus',['(s32)g->unk_036 == 12','(u32)g->unk_036 == 12','(s32)g->unk_036 == 12']),
('mixed_usu',['(u32)g->unk_036 == 12','(s32)g->unk_036 == 12','(u32)g->unk_036 == 12']),
('mixed_widths',['(s32)g->unk_036 == 12','(s16)g->unk_036 == 12','(u32)g->unk_036 == 12']),
('eq_neq',['g->unk_036 == 12','!(g->unk_036 != 12)','g->unk_036 == 12']),
('signed_all',['(s32)g->unk_036 == 12']*3),
]:
    block=body[a:b]
    for day,expr in zip((10,8,7),exprs):
        block=block.replace('g->unk_036 == 12 ? g->unk_037 == %d : 0'%day,'('+expr+') && g->unk_037 == %d'%day)
    variants[label]=body[:a]+block+body[b:]
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023')
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
