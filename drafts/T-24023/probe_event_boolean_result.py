"""T-24023 int Boolean computation with narrowing at the global assignment."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('build/t24023/combined_selector_view/body.c').read_text();decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
a=body.index('            if (!((g->unk_035 =');b=body.index('            if ((sp2B < 3U)',a)
variants={}
for var in ['var_at','temp_v0','date_ok']:
    block=''
    for i,day in enumerate((10,8,7)):
        indent='            '+'    '*i
        block+=indent+var+' = g->unk_036 == 12;\n'
        block+=indent+'if ('+var+' != 0) {\n'+indent+'    '+var+' = g->unk_037 == '+str(day)+';\n'+indent+'}\n'
        block+=indent+'if (!(g->unk_035 = '+var+')) {\n'
    block+='                        goto block_448;\n                    }\n                }\n            }\n'
    variants['int_result_'+var]=body[:a]+block+body[b:]
# Test a Boolean false arm expressed by the original condition's value.
block=body[a:b].replace(': 0)',': (g->unk_036 == 12))')
variants['ternary_false_condition']=body[:a]+block+body[b:]
# Explicit narrowing at each short-circuit result.
block=body[a:b]
for day in (10,8,7):block=block.replace('g->unk_036 == 12 ? g->unk_037 == %d : 0'%day,'(u8)(g->unk_036 == 12 && g->unk_037 == %d)'%day)
variants['cast_logical']=body[:a]+block+body[b:]
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023')
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
