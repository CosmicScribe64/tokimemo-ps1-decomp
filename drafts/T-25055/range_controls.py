"""Test lifetime sharing and exact-width girl uses after the two established repairs."""
from pathlib import Path
base = Path('drafts/T-25055/combined-41.c').read_text()
out = Path('build/T-25055/controls')
a = base.index('    case 0x6:'); b = base.index('    case 0x20:',a)
case = base[a:b]
new = case.replace('ent = D_80141134', 'rnd = D_80141134').replace('if (ent != 0)', 'if (rnd != 0)').replace('total = ent + total;', 'total = rnd + total;')
(out/'share-random.c').write_text(base[:a]+new+base[b:])
new = case.replace('        if (rnd < total)', '        lim = total - rnd;\n        if (rnd < total)').replace('(total - rnd)', '(s32) lim')
(out/'share-delta.c').write_text((base[:a]+new+base[b:]).replace('u32 lim;', 's32 lim;'))
# One additional real-width static read should change the late-split girl range's profit.
new = base.replace('        lim = sp68[g->unk_F5F] >> 2;', '        if ((u32)g->unk_F5F < 0) {}\n        lim = sp68[g->unk_F5F] >> 2;')
(out/'girl-profit.c').write_text(new)
# The original materializes the girl global address even when passing a widened argument.
old = 'sndisp(D_801413C8[D_801405E4], 0, (s32) g->unk_F5F);'
new = 'sndisp(D_801413C8[D_801405E4], 0, *(u8 *)&g->unk_F5F);'
assert base.count(old) == 1
(out/'girl-argument.c').write_text(base.replace(old,new))
base = Path('drafts/T-25055/girl-profit-1.c').read_text()
old = 'else if ((hp == 3) && (D_80140558 != g->unk_03F)'
assert base.count(old) == 1
(out/'compare-multiply.c').write_text(base.replace(old, 'else if (((hp * 1) == 3) && (D_80140558 != g->unk_03F)'))
