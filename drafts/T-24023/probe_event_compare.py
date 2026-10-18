"""T-24023 commute selector comparisons and inspect conversion ownership."""
from pathlib import Path
import sys,json,re
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('build/t24023/combined_selector_view/body.c').read_text();decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
selector='(*(u8 *)&D_801243B0)'
variants={
'commute_selector':re.sub(re.escape(selector)+r' == (0x[0-9A-F]+|[0-9]+)',r'\1 == '+selector,body),
'signed_selector_value':body.replace(selector,'((s32)*(u8 *)&D_801243B0)'),
'unsigned_selector_value':body.replace(selector,'((u32)*(u8 *)&D_801243B0)'),
'selector_pointer_index':body.replace(selector,'((u8 *)&D_801243B0)[0]'),
}
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023')
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
