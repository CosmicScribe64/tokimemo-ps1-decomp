"""Test raw-word bonus extraction rather than a promoted bitfield. Docker only."""
from pathlib import Path
import sys
sys.path.insert(0,'tools')
import funcdiff,order_search
c=funcdiff.locate('func_801330E4','ENDING');score=order_search.make_text_scorer(c,'func_801330E4',resolve=True)
s=Path('build/t24512/no-n-pad/full.c').read_text()
f='D_800E6280.unk_1BC[0xB].unk_0C.f.f9'
raw='D_800E6280.unk_1BC[0xB].unk_0C.w'
variants=[('raw-both',s.replace(f,'((u32)('+raw+' << 20) >> 29)')),('raw-level',s.replace('level = '+f,'level = ((u32)('+raw+' << 20) >> 29)'))]
for label,text in variants:
 w=Path('build/t24512')/label;w.mkdir(parents=True,exist_ok=True);(w/'full.c').write_text(text);print(label,score(text,w,label),flush=True)
