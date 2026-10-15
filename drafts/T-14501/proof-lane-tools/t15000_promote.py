"""Enable the verified ordinary KANGEI branch structure."""
from pathlib import Path
import sys
sys.path.insert(0,'tools');import autotweak
p=Path('src/ovl/KANGEI/801355F0.c');s=p.read_text();start=s.index('void func_80135CE0(void) {');end=s.index('\n#else',start);body=s[start:end];s=autotweak.rewrite_guard(s,'func_80135CE0',body);s=s.replace('/* bulk_m2c declarations (T-11190) */\n/* end bulk_m2c declarations */\n','');p.write_text(s)
