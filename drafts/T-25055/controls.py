"""Generate three bounded range-sharing controls from the preserved pointer donor."""
from pathlib import Path
base = Path('drafts/T-25024/pointer-baseline.c').read_text()
out = Path('build/T-25055/controls')
out.mkdir(parents=True, exist_ok=True)
a = base.index('    case 0x40:')
b = base.index('    case 0x41:',a)
case = base[a:b]
# A promoted global may own the selector range, including the first constant store.
new = case.replace('        ent = D_801405E4;\n','').replace('        D_801405E4 = ent;\n','').replace('ent = 0xC;', 'D_801405E4 = 0xC;').replace('D_80141D80[ent]', 'D_80141D80[D_801405E4]')
variants = {'global-selector':base[:a]+new+base[b:]}
# Preserve the existing unconditional store while using the exact byte static view.
new = case.replace('ent = D_801405E4;', 'ent = *(u8 *)&D_801405E4;')
variants['selector-view'] = base[:a]+new+base[b:]
# Target carries the selected maximum and final total in the same v1 range.
a = base.index('    case 0x6:'); b = base.index('    case 0x20:',a)
new = base[a:b].replace('hi = lim;', 'total = lim;').replace('hi = (u32) hp;', 'total = (u32) hp;').replace('total = ent + hi;', 'total = ent + total;')
variants['shared-total'] = base[:a]+new+base[b:]
for name, body in variants.items():
    (out/(name+'.c')).write_text(body)
base = variants['global-selector']
a = base.index('    case 0x6:'); b = base.index('    case 0x20:', a)
new = base[a:b].replace('hi = lim;', 'total = lim;').replace('hi = (u32) hp;', 'total = (u32) hp;').replace('total = ent + hi;', 'total = ent + total;')
(out/'global-shared-total.c').write_text(base[:a]+new+base[b:])
