"""T-24023 simplify synthetic nested conditionals without changing evaluation order."""
from pathlib import Path
import sys,re,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801103A0','EVENT');base=Path(c.src).read_text().replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('build/t24023/arg_s32/body.c').read_text();decl=Path('drafts/T-15200/func_801103A0/declarations.h').read_text()
def close(s,pos,a,b):
    depth=1;i=pos+1
    while depth:
        if s[i]==a:depth+=1
        elif s[i]==b:depth-=1
        i+=1
    return i-1
def flatten(s):
    count=0
    while True:
        found=False
        for m in re.finditer(r'\bif\s*\(',s):
            p=s.index('(',m.start());q=close(s,p,'(',')');brace=q+1
            while s[brace].isspace():brace+=1
            if s[brace]!='{':continue
            end=close(s,brace,'{','}')
            inner=s[brace+1:end].strip()
            if not re.match(r'^if\s*\(',inner):continue
            ip=inner.index('(');iq=close(inner,ip,'(',')');ib=iq+1
            while inner[ib].isspace():ib+=1
            if inner[ib]!='{':continue
            ie=close(inner,ib,'{','}')
            if inner[ie+1:].strip():continue
            if re.match(r'\s*else\b',s[end+1:]):continue
            merged='if (('+s[p+1:q]+') && ('+inner[ip+1:iq]+')) {'+inner[ib+1:ie]+'}'
            s=s[:m.start()]+merged+s[end+1:];count+=1;found=True;break
        if not found:return s,count
flat,n=flatten(body);print('flattened',n,flush=True)
variants={'flattened':flat}
old='((var_at = sp2B < 2U, (g->unk_036 == 0xC)) || (var_at = sp2B < 2U, (g->unk_036 == 1)) || (var_at = sp2B < 2U, (g->unk_036 == 2))) && (var_at != 0)'
new='((g->unk_036 == 0xC) || (g->unk_036 == 1) || (g->unk_036 == 2)) && (sp2B < 2U)'
assert old in body
variants['plain_condition']=body.replace(old,new)
variants['flattened_plain']=flat.replace(old,new)
# Equivalent nested assignment conditions can alter boolean-result range splitting.
a=body.index('            if (!((g->unk_035 =');b=body.index('            if ((sp2B < 3U)',a)
for mode in ['nested','equal_one','ternary']:
    if mode=='nested':block='''            if (!(g->unk_035 = (g->unk_036 == 12 && g->unk_037 == 10))) {
                if (!(g->unk_035 = (g->unk_036 == 12 && g->unk_037 == 8))) {
                    if (!(g->unk_035 = (g->unk_036 == 12 && g->unk_037 == 7))) {
                        goto block_448;
                    }
                }
            }
'''
    else:
        block=body[a:b]
        for day in (10,8,7):
            if mode=='equal_one':block=block.replace('g->unk_037 == %d))'%day,'g->unk_037 == %d)) == 1'%day)
            else:block=block.replace('g->unk_036 == 12 && g->unk_037 == %d'%day,'g->unk_036 == 12 ? g->unk_037 == %d : 0'%day)
    variants['condition_'+mode]=body[:a]+block+body[b:]
score=order_search.make_text_scorer(c,'func_801103A0',resolve=True,non_matching=False)
root=Path('build/t24023')
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,decl+'\n'+variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
