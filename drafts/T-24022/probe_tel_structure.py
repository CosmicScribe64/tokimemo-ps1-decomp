"""T-24022 structural hypotheses: field stores, alias ranges, explicit loads."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_80136724','TEL');base=Path(c.src).read_text()
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_80136724' in l)
body=Path('drafts/T-23001/TEL/func_80136724/hand.c').read_text()
root=Path('build/t24022')
variants={}
# Preserve local count/frame while trying a real state pointer for the record-update block.
a=body.index('        D_800E6280.unk_1BC[');b=body.index('        break;\n    case 0x20:',a)
for name,segment in [('gs_tail',body[a:b].replace('D_800E6280.','gs->')),('gs_records',body[a:b].replace('D_800E6280.unk_1BC','gs->unk_1BC'))]:
    variants[name]=body[:a]+'        gs = &D_800E6280;\n'+segment+body[b:]
    variants[name]=variants[name].replace('s32 x1;','GameState *gs;')
# Store boundaries determine whether the bit-field intermediate remains live.
for name,old,new in [
('bits_day_group','sp44.bits.day = D_80140554;\n        sp44.bits.unk_21 = D_80140560;','sp44.bits.day = D_80140554; sp44.bits.unk_21 = D_80140560;'),
('bits_month_day_group','sp44.bits.month = D_80140558;\n        sp44.bits.day = D_80140554;','sp44.bits.month = D_80140558; sp44.bits.day = D_80140554;'),
('bits_all_group','\n        sp44.bits.',' sp44.bits.'),
('t1_s32','u8 t1;','s32 t1;'),
('month_volatile_local','s32 month;','volatile s32 month;')]:
    variants[name]=body.replace(old,new)
# Read globals through an explicit local at each use, including independently reloaded comparisons.
text=body
for old,new in [
('func_8013D88C(0, D_8014055C, D_80140558, D_80140554);','month = D_80140558;\n        func_8013D88C(0, D_8014055C, month, D_80140554);'),
('func_8013D88C(1, D_8014055C, D_80140558, D_80140554);','month = D_80140558;\n        func_8013D88C(1, D_8014055C, month, D_80140554);'),
('sp44.bits.month = D_80140558;','month = D_80140558;\n        sp44.bits.month = month;'),
('func_800AE100(sp78, D_800B3288[D_80140558]);','month = D_80140558;\n        func_800AE100(sp78, D_800B3288[month]);'),
('D_80140558 == D_800E6280.unk_03F','(month = D_80140558) == D_800E6280.unk_03F'),
('D_80140558 != D_800E6280.unk_03F','(month = D_80140558) != D_800E6280.unk_03F')]:text=text.replace(old,new)
variants['explicit_month_all_uses']=text
score=order_search.make_text_scorer(c,'func_80136724',resolve=True,non_matching=False)
for label,variant in variants.items():
    out=root/label;val=score(base.replace(needle,variant),out,label)
    (out/'body.c').write_text(variant)
    print(label,val,flush=True)
    with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val,'fast':False})+'\n')
