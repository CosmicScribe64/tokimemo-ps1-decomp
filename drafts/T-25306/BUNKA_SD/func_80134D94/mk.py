"""Write build/T-25306/src_<stem>.c: the unit with the guard body replaced (for regdiff --src)."""
import sys
from pathlib import Path
sys.path.insert(0, 'tools')
import autotweak
src = Path('src/ovl/BUNKA_SD/80134540.c').read_text()
for b in sys.argv[1:]:
    body = Path(b).read_text()
    out = Path('build/T-25306') / ('src_' + Path(b).stem + '.c')
    out.write_text(autotweak.rewrite_guard(src, 'func_80134D94', body))
    print(out)
