"""Repair stale scene-record indexing using matched sibling types. Docker only."""
from pathlib import Path
import sys
sys.path.insert(0,'tools')
import funcdiff,order_search
name='func_801387B0'; c=funcdiff.locate(name,'ENDING');src=Path(c.src).read_text()
b=Path('drafts/T-20930/ENDING/'+name+'/hand.c').read_text()
for off,field in [('0x852','unk_16'),('0x80E','unk_16'),('0x810','unk_18'),('0x800','unk_08')]:
    rec=31 if off=='0x852' else 30
    b=b.replace('*(s16 *)&D_8011ECD0['+off+']','((DisplayActor44 *)D_8011ECD0)['+str(rec)+'].'+field)
anchor='INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", '+name+');'
score=order_search.make_text_scorer(c,name,resolve=True)
for label,body in [('scene-s32',b),('scene-u32',b.replace('s32 flag;','u32 flag;'))]:
    text=src.replace(anchor,body); w=Path('build/t24512')/label;w.mkdir(parents=True,exist_ok=True);(w/'full.c').write_text(text)
    print(label,score(text,w,label),flush=True)
