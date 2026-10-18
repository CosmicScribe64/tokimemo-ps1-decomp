"""T-24022: bounded equivalent month-normalization forms, Docker only."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_80136724','TEL')
body=Path('drafts/T-23001/TEL/func_80136724/hand.c').read_text()
base=Path(c.src).read_text()
needle='INCLUDE_ASM("asm/nonmatchings/ovl/TEL/80132000", func_80136724);'
if needle not in base:
    needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_80136724' in l)
old='''month = D_80140558;
        if (month == 0xD) {
            month = 1;
        }
        D_80140558 = month;'''
assert body.count(old)==2
forms={
'baseline':old,
'conditional_global':'''if (D_80140558 == 0xD) {
            D_80140558 = 1;
        }
        month = D_80140558;''',
'conditional_chain':'''month = D_80140558;
        if (month == 0xD) {
            month = D_80140558 = 1;
        }''',
'conditional_local_store':'''month = D_80140558;
        if (month == 0xD) {
            month = 1;
            D_80140558 = month;
        }''',
'global_ternary':'''D_80140558 = D_80140558 == 0xD ? 1 : D_80140558;
        month = D_80140558;''',
'conditional_subtract':'''if (D_80140558 == 0xD) {
            D_80140558 -= 12;
        }
        month = D_80140558;''',
'branch_store':'''if (D_80140558 == 0xD) {
            D_80140558 = 1;
            month = 1;
        } else {
            month = D_80140558;
        }''',
}
root=Path('build/t24022');root.mkdir(parents=True,exist_ok=True)
# Full TU first, then fast path for bounded equivalent variants.
score=order_search.make_text_scorer(c,'func_80136724',resolve=True,fast='--full' not in sys.argv,non_matching=False)
for label,form in forms.items():
    variant=body.replace(old,form)
    text=base.replace(needle,variant)
    out=root/label
    val=score(text,out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val,'fast':'--full' not in sys.argv})+'\n')
