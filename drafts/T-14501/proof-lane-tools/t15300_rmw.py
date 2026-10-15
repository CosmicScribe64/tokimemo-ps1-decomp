"""Test explicit byte read-modify-write assignment in the fresh BUNKASAI seed."""
from pathlib import Path
p=Path('src/ovl/BUNKASAI/80143770.c');s=p.read_text();start=s.index('void func_801438E4(void) {');end=s.index('\n#else',start);b=s[start:end];b=b.replace('D_800E6280.unk_1BC[7].unk_0C.b[1] |= 0x10;', 'D_800E6280.unk_1BC[7].unk_0C.b[1] = D_800E6280.unk_1BC[7].unk_0C.b[1] | 0x10;').replace('D_800E6280.unk_1BC[7].unk_0C.b[1] |= 0x20;', 'D_800E6280.unk_1BC[7].unk_0C.b[1] = D_800E6280.unk_1BC[7].unk_0C.b[1] | 0x20;');assert b!=s[start:end];p.write_text(s[:start]+b+s[end:])
