"""T-24023 register-qualified Boolean lifetimes; Docker only."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('drafts/T-24023/body.c').read_text();a=body.index('            if (!((g->unk_035 =');b=body.index('            if ((sp2B < 3U)',a)
variants={'register_byte_ternary':body.replace('    u8 date_ok;','    register u8 date_ok;')}
for typ in ['u8','s32','u32']:
    block=''
    for i,day in enumerate((10,8,7)):
        indent='            '+'    '*i
        block+=indent+'date_ok = g->unk_036 == 12;\n'+indent+'if (date_ok) {\n'+indent+'    date_ok = g->unk_037 == '+str(day)+';\n'+indent+'}\n'+indent+'if (!(g->unk_035 = date_ok)) {\n'
    block+='                        goto block_448;\n                    }\n                }\n            }\n'
    new=(body[:a]+block+body[b:]).replace('    u8 date_ok;', '    register '+typ+' date_ok;')
    variants['register_'+typ+'_explicit']=new
# Narrow result is a byproduct used by the global store; retain nested short circuit.
for mode in ['logical','ternary_reuse']:
    block=body[a:b]
    for day in (10,8,7):
        old='g->unk_036 == 12 ? g->unk_037 == %d : 0'%day
        new='g->unk_036 == 12 && g->unk_037 == %d'%day if mode=='logical' else '(date_ok = g->unk_036 == 12) ? g->unk_037 == %d : date_ok'%day
        block=block.replace(old,new)
    variants['register_byte_'+mode]=(body[:a]+block+body[b:]).replace('    u8 date_ok;','    register u8 date_ok;')
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023-resume')
for label,variant in variants.items():
    assert variant.count('goto block_448;')==2
    out=root/label;val=score(base.replace(needle,variant),out,label);(out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
