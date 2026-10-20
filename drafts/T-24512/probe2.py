"""Causal bonus-level source reconstruction probes; Docker only."""
from pathlib import Path
import sys
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801330E4','ENDING'); score=order_search.make_text_scorer(c,'func_801330E4',resolve=True)
s=Path('build/t24512/no-n-pad/full.c').read_text()
old='D_800E6280.unk_1BC[0xB].unk_0C.f.f9 + (D_800E6280.unk_56C[0x4C] == 0 ^ 1) * 2'
new='(D_800E6280.unk_56C[0x4C] == 0 ^ 1) * 2 + D_800E6280.unk_1BC[0xB].unk_0C.f.f9'
a=s.replace(old,new)
variants=[('flag-first',a),('level-word',a.replace('s16 level;','s32 level;').replace('level = (D_','level = (s16)((D_').replace('== 0 ^ 1) * 2;','== 0 ^ 1) * 2);'))]
for typ in ['u8','u16','s16']:
    b=s.replace('    s16 *p;','    '+typ+' n;').replace('            p = &points[i];\n','').replace('*p','points[i]')
    b=b.replace('        level =','        n = D_800E6280.unk_1BC[0xB].unk_0C.f.f9;\n        level =',1).replace('level = D_800E6280.unk_1BC[0xB].unk_0C.f.f9 +','level = n +').replace('if (D_800E6280.unk_1BC[0xB].unk_0C.f.f9 != 0','if (n != 0')
    variants.append(('n-'+typ,b))
for label,text in variants:
    w=Path('build/t24512')/label;w.mkdir(parents=True,exist_ok=True);(w/'full.c').write_text(text)
    print(label,score(text,w,label),flush=True)
