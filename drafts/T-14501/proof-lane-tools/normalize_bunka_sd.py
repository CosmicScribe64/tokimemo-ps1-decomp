"""Normalize the BUNKA_SD candidate declarations and pointer-array loads."""
from pathlib import Path
import re
p=Path('src/ovl/BUNKA_SD/80136270.c');s=p.read_text()
block='/* bulk_m2c declarations (T-11190) */\nextern s32 D_8013C74C;\nextern u8 *D_8013C740;\nextern u8 *D_8013C744;\nextern u8 *D_8013C748;\n/* end bulk_m2c declarations */\n'
assert block in s
s=s.replace(block,'')
s=s.replace('(*(s32 *)D_8013C740)', '((s32 *)D_8013C740)[0]')
s=re.sub(r'\*\(s32 \*\)\(\(unsigned char \*\)D_8013C740 \+ \((\d+)\)\)',lambda m:'((s32 *)D_8013C740)['+str(int(m[1])//4)+']',s)
p.write_text(s)
p=Path('include/ovl/BUNKA_SD.h');s=p.read_text();s=s.replace('#endif /* OVL_BUNKA_SD_H */','/* T-14501: loaded scene data used by func_80136578. */\nextern s32 D_8013C74C;\nextern u8 *D_8013C740;\nextern u8 *D_8013C744;\nextern u8 *D_8013C748;\n\n#endif /* OVL_BUNKA_SD_H */');p.write_text(s)
