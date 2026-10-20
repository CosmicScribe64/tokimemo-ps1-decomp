"""Bounded ENDING frame and expression probes. Docker only."""
from pathlib import Path
import sys
sys.path.insert(0, 'tools')
import funcdiff, order_search
name='func_801330E4'
c=funcdiff.locate(name,'ENDING')
source=Path(c.src).read_text()
body=Path('drafts/T-20930/ENDING/'+name+'/hand.c').read_text()
source=source.replace('INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133030", '+name+');',body)
start=source.index('void '+name+'(void)')
end=source.index('\nvoid func_80133400',start)
score=order_search.make_text_scorer(c,name,resolve=True)
scalar='    s16 best;\n    s16 i;\n    u32 n;\n    s16 *p;\n'
b=body.replace(scalar,'').replace('    u8 rank[32];',scalar+'    u8 rank[32];')
variants=[('baseline',body),('scalars-first',b),('word-level',b.replace('s16 level;','s32 level;').replace('level = n +','level = (s16)(n +').replace('== 0 ^ 1) * 2;','== 0 ^ 1) * 2);'))]
for label,b in variants:
    text=source[:start]+b+source[end:]
    work=Path('build/t24512')/label; work.mkdir(parents=True,exist_ok=True)
    (work/'full.c').write_text(text)
    print(label,score(text,work,label),flush=True)
b=variants[1][1]
b=b.replace('    u32 n;\n','').replace('        n = D_800E6280.unk_1BC[0xB].unk_0C.f.f9;\n','').replace('level = n +','level = D_800E6280.unk_1BC[0xB].unk_0C.f.f9 +').replace('if (n != 0','if (D_800E6280.unk_1BC[0xB].unk_0C.f.f9 != 0')
for label,b in [('no-n',b),('no-n-pad',b.replace('    s16 best;','    s32 pad[4]; /* FAKE: reproduce B0 frame; original locals unknown. T-24512 */\n    s16 best;'))]:
    text=source[:start]+b+source[end:]
    work=Path('build/t24512')/label; work.mkdir(parents=True,exist_ok=True)
    (work/'full.c').write_text(text)
    print(label,score(text,work,label),flush=True)
