"""TEL helper value and address range probes; Docker only."""
from pathlib import Path
import sys,json
sys.path.insert(0,'tools')
import funcdiff as fd,order_search
name='func_80134FE4';c=fd.locate(name,'TEL');base=Path(c.src).read_text()
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and name in l)
body=Path('drafts/T-23001/TEL/func_80134FE4/hand.c').read_text()
decl='extern u8 *D_801406E0[];\nextern u32 D_80140710[];\nextern u32 D_80140770[];\n'
variants={'baseline':body}
variants['game_pointer']=body.replace('    u8 zy;','    u8 zy;\n    GameState *g = &D_800E6280;').replace('D_800E6280.','g->')
variants['selector_view']=body.replace('D_801405E4','(*(u8 *)&D_801405E4)')
variants['signed_arguments']=body.replace('set_c_girl(D_801405E4)','set_c_girl((s32) D_801405E4)').replace('get_g_zyotai_s(D_801405E4)','get_g_zyotai_s((s32) D_801405E4)')
# A pointer range can replace a promoted global without adding any machine instruction.
variants['girl_pointer']=body.replace('    u8 zy;','    u8 zy;\n    u8 *girl = &D_801405E4;').replace('D_801405E4','(*girl)').replace('u8 *girl = &(*girl);','u8 *girl = &D_801405E4;')
variants['reuse_zy_first']=body.replace('        D_800E6280.unk_1BC[D_801405E4].unk_0C.b[3] += 1;','        zy = D_801405E4;\n        D_800E6280.unk_1BC[zy].unk_0C.b[3] += 1;')
root=Path('build/t24042');root.mkdir(parents=True,exist_ok=True)
score=order_search.make_text_scorer(c,name,resolve=True,non_matching=False)
for label,s in variants.items():
 out=root/label;val=score(base.replace(needle,decl+s),out,label);(out/'body.c').write_text(s)
 print(label,val,flush=True)
 with (root/'results.jsonl').open('a') as f:f.write(json.dumps({'variant':label,'aligned_cost':val})+'\n')
