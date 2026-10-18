"""T-24022 store/read range separation, diagnostic candidates only."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_80136724','TEL');base=Path(c.src).read_text()
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_80136724' in l)
body=Path('drafts/T-23001/TEL/func_80136724/hand.c').read_text()
root=Path('build/t24022')
variants={}
for typ in ['u32','s32','GsWord']:
    lhs='((%s *)&D_80140558)'%typ
    if typ=='GsWord':expr=lhs+'->w'
    else:expr='*'+lhs
    variants['store_'+typ]=body.replace('D_80140558 = month;',expr+' = month;')
variants['u32_month']=body.replace('s32 month;','u32 month;')
variants['unsigned_all_reads']=body.replace('month = D_80140558;', 'month = *(u32 *)&D_80140558;')
variants['unsigned_other_reads']=body.replace('D_80140558','(*(u32 *)&D_80140558)').replace('(*(u32 *)&D_80140558) = month;','D_80140558 = month;')
for name,expression in [('pointer_store','((s32 *)&D_80140558)[0]'),('word_store','((GsWord *)&D_80140558)->w')]:
    variants[name]=body.replace('D_80140558 = month;',expression+' = month;')
# Separate a local month from its global on later paths, preserving every reload.
lines=body.splitlines(); result=[]
for l in lines:
    if 'D_80140558' in l and 'month = D_' not in l and 'D_80140558 = month' not in l:
        indent=l[:len(l)-len(l.lstrip())]
        if l.lstrip().startswith('} else if'):break
        result.append(indent+'month = D_80140558;')
        l=l.replace('D_80140558','month')
    result.append(l)
else:variants['explicit_month_reloads']='\n'.join(result)+'\n'
score=order_search.make_text_scorer(c,'func_80136724',resolve=True,non_matching=False)
for label,variant in variants.items():
    out=root/label
    val=score(base.replace(needle,variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val,'fast':False})+'\n')
