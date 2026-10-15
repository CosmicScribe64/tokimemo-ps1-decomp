"""Enable a verified guarded near-match definition, leaving other code intact."""
from pathlib import Path
import sys
sys.path.insert(0,'tools');import autotweak
p=Path(sys.argv[1]);func=sys.argv[2];s=p.read_text();start=s.index('void '+func+'(void) {');end=s.index('\n#else',start);body=s[start:end];s=autotweak.rewrite_guard(s,func,body);s=s.replace('/* bulk_m2c declarations (T-11190) */\n/* end bulk_m2c declarations */\n','');p.write_text(s)
