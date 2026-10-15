"""Normalize new RPG_BAT globals and an already-named SDK callee."""
from pathlib import Path
p=Path('src/ovl/RPG_BAT/8013F4F0.c');s=p.read_text()
s=s.replace('extern u8 D_801215BA;\n','').replace('extern s32 D_800E6354;\n','').replace('s32 func_800AE100();\n','')
s=s.replace('&D_800E6354','D_800E6280.unk_0D4').replace('func_800AE100(D_8015E828,','strcat(D_8015E828,')
s=s.replace('/* bulk_m2c declarations (T-11190) */\n/* end bulk_m2c declarations */\n','')
s=s.replace('s32 func_80140A58(void) {','/* T-14501: retain the earlier unprototyped callers\' implicit s32 view. */\ns32 func_80140A58(void) {')
p.write_text(s)
