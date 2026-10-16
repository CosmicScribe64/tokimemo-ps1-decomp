"""Centralize the three authoritative main-executable forward declarations."""
from pathlib import Path
p=Path('src/main/8006CB30.c');s=p.read_text();names=['func_8006CD14','schdeule_girls_param','func_80070ECC']
for name in names:
 decl='void '+name+'(void);\n';assert decl in s;s=s.replace(decl,'')
s=s.replace('/* bulk_m2c declarations (T-11190) */\n/* end bulk_m2c declarations */\n','');p.write_text(s)
p=Path('include/main_api.h');s=p.read_text();needle='void _schedule_init(void);\n';assert needle in s
for name in names: assert 'void '+name+'(' not in s
s=s.replace(needle,needle+'/* T-14501: _schedule_init callees defined later in 8006CB30.c. */\n'+''.join('void '+name+'(void);\n' for name in names),1);p.write_text(s)
