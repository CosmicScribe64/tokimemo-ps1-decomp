"""Use the existing character flag bit-fields for the two flag sets."""
from pathlib import Path
p=Path('src/ovl/BUNKASAI/80143770.c');s=p.read_text();start=s.index('void func_801438E4(void) {');end=s.index('\n#else',start);b=s[start:end]
for mask,bit in [('0x10','b12'),('0x20','b13')]:
 old='D_800E6280.unk_1BC[7].unk_0C.b[1] = D_800E6280.unk_1BC[7].unk_0C.b[1] | '+mask+';';assert old in b;b=b.replace(old,'D_800E6280.unk_1BC[7].unk_0C.f.'+bit+' = 1;')
p.write_text(s[:start]+b+s[end:])
