"""T-24023 restore an explicit byte Boolean and the observed stack-byte gap."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('build/t24023/else_if/body.c').read_text();decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
a=body.index('            if (!((g->unk_035 =');b=body.index('            if ((sp2B < 3U)',a)
block='''            date_ok = g->unk_036 == 12;
            if (date_ok != 0) {
                date_ok = g->unk_037 == 10;
            }
            g->unk_035 = date_ok;
            if (date_ok == 0) {
                date_ok = g->unk_036 == 12;
                if (date_ok != 0) {
                    date_ok = g->unk_037 == 8;
                }
                g->unk_035 = date_ok;
                if (date_ok == 0) {
                    date_ok = g->unk_036 == 12;
                    if (date_ok != 0) {
                        date_ok = g->unk_037 == 7;
                    }
                    g->unk_035 = date_ok;
                    if (date_ok == 0) {
                        goto block_448;
                    }
                }
            }
'''
variants={}
for typ in ['u8','s32','u32']:
    for loc in ['between','after']:
        new=body[:a]+block+body[b:]
        marker='    u8 sp29;' if loc=='between' else '    s32 temp_v0;'
        new=new.replace(marker,'    '+typ+' date_ok;\n'+marker)
        variants['bool_'+typ+'_'+loc]=new
# Preserve short-circuit spelling, force the observable narrowed result through a real local.
newblock=body[a:b].replace('(g->unk_035 = (','(g->unk_035 = date_ok = (')
variants['bool_assignment']= (body[:a]+newblock+body[b:]).replace('    u8 sp29;','    u8 date_ok;\n    u8 sp29;')
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023')
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
