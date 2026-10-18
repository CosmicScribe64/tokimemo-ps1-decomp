"""T-24023 selector lifetime across the common tail."""
from pathlib import Path
import sys,json,re
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('build/t24023/combined/body.c').read_text();decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
variants={'combined_selector_view':body.replace('D_801243B0','(*(u8 *)&D_801243B0)')}
for var,typ in [('temp_v0','s32'),('temp_v0','u32'),('temp_v0','u8')]:
    new=body.replace('D_801243B0',var)
    new=re.sub(r'(if \(g->unk_EE6 == (?:0xA|[0-9])\) \{)',r'\1\n        '+var+' = D_801243B0;',new)
    needle_default='    if (('+var+' == 0xB) && (g->unk_1A4[1]'
    assert needle_default in new
    new=new.replace(needle_default,'    '+var+' = D_801243B0;\n'+needle_default,1)
    new=new.replace('    s32 temp_v0;', '    '+typ+' temp_v0;')
    variants['local_selector_'+typ]=new
# Read once before dispatch; the compiler may sink it to each use region.
new=body.replace('D_801243B0','temp_v0')
new=new.replace('    if (g->unk_EE6 == 0)', '    temp_v0 = D_801243B0;\n    if (g->unk_EE6 == 0)',1)
variants['entry_selector']=new
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023')
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
