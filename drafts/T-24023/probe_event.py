"""T-24023 full-TU outer-dispatch probes; Docker only."""
from pathlib import Path
import sys,re,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text()
base=base.replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('drafts/T-15200/func_801103A0/body.c').read_text()
decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
root=Path('build/t24023');root.mkdir(parents=True,exist_ok=True)
variants={'baseline':body}
# Each arm ends with goto common tail. Build equivalent switch/default and else-if.
start=body.index('    if (g->unk_EE6 == 0)')
end=body.index('block_448:\n')
region=body[start:end]
matches=list(re.finditer(r'^    if \(g->unk_EE6 == (0xA|[0-9])\) \{',region,re.M))
assert len(matches)==11
arms=[]
for m in matches:
    j=m.end();depth=1
    while depth:
        if region[j]=='{':depth+=1
        elif region[j]=='}':depth-=1
        j+=1
    arms.append((m[1],region[m.end():j-1],m.start(),j))
default=region[arms[-1][3]:]
switch='    switch (g->unk_EE6) {\n'
for val,arm,_,_ in arms:switch+='    case '+val+':'+arm+'\n'
switch+='    default:'+default+'    }\n'
variants['switch']=body[:start]+switch+body[end:]
chain=''
for i,(val,arm,_,_) in enumerate(arms):
    trimmed,n = re.subn(r'\n        goto block_448;\s*$', '', arm)
    assert n == 1, (val, 'expected exactly one terminal common-tail jump')
    assert trimmed.count('goto block_448;') == arm.count('goto block_448;') - 1
    chain+=('    if' if i==0 else ' else if')+' (g->unk_EE6 == '+val+') {'+trimmed+'\n    }'
chain+=' else {\n'+default+'    }\n'
variants['else_if']=body[:start]+chain+body[end:]
assert variants['else_if'].count('goto block_448;') == body.count('goto block_448;') - 11
# Explicit signed selector range: preserve frame by reusing dead temp_v0 after its masks.
variants['local_selector']=body.replace('    if (g->unk_EE6 == 0)', '    temp_v0 = g->unk_EE6;\n    if (temp_v0 == 0)',1)
variants['local_selector']=re.sub(r'^    if \(g->unk_EE6 ==', '    if (temp_v0 ==',variants['local_selector'],flags=re.M)
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
