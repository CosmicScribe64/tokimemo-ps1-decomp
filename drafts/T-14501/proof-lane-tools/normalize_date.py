"""Move recovered DATE forward declarations to its overlay header."""
from pathlib import Path
p=Path('src/ovl/DATE/80132000.c');s=p.read_text()
block='/* bulk_m2c declarations (T-11190) */\nvoid date_fill_sections_shiori(void);\nvoid date_fill_sections_mio(void);\nvoid date_fill_sections_yuina(void);\n/* end bulk_m2c declarations */\n'
assert block in s;s=s.replace(block,'');p.write_text(s)
p=Path('include/ovl/DATE.h');s=p.read_text();i=s.rfind('#endif');assert i!=-1;s=s[:i]+'/* T-14501: section fillers called before their definitions. */\nvoid date_fill_sections_shiori(void);\nvoid date_fill_sections_mio(void);\nvoid date_fill_sections_yuina(void);\n\n'+s[i:];p.write_text(s)
p=Path('src/ovl/DATE/80145AA0.c');s=p.read_text().replace('/* bulk_m2c declarations (T-11190) */\n/* end bulk_m2c declarations */\n','');p.write_text(s)
