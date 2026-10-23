"""Model target's named remainder range rather than an expression temporary."""
from pathlib import Path
from probe import probe
name='func_801398EC';base=Path('build/T-24515/func_801398EC/global-index-loop/body.c').read_text()
cap=base.replace('    s32 i;', '    s32 i;\n    u32 tile;')
cap=cap.replace('                D_801217D0[i].unk_00 = 0;', '                tile = (D_800E6280.unk_10F8 >> 2) % 3;\n                D_801217D0[i].unk_00 = 0;')
cap=cap.replace('((D_800E6280.unk_10F8 >> 2) % 3) * 0x10', 'tile * 0x10')
probe(name,'named-remainder',cap)
