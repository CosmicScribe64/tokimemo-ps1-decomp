"""Preserve the prior header layout while adding its validated new global."""
from pathlib import Path
p=Path('include/main_api.h');s=Path('build/t14500/main_api.before.h').read_text();needle='extern s16 D_8012158C;\n';assert needle in s;s=s.replace(needle,needle+'extern u8 D_801215BA;\n',1);p.write_text(s)
