"""T-24023 combine measured structural improvements, not blind permutations."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('build/t24023/condition_ternary/body.c').read_text();decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
old='((var_at = sp2B < 2U, (g->unk_036 == 0xC)) || (var_at = sp2B < 2U, (g->unk_036 == 1)) || (var_at = sp2B < 2U, (g->unk_036 == 2))) && (var_at != 0)'
new='((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (sp2B < 2U)'
combined=body.replace(old,new)
assert combined!=body
variants={'combined':combined,'ternary_uncast_args':body.replace('(s32)g->unk_EE6','g->unk_EE6'),'combined_uncast_args':combined.replace('(s32)g->unk_EE6','g->unk_EE6')}
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023')
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
