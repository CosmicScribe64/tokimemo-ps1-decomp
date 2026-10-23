"""Install the verified NAME_ENT function; run once in the isolated Docker tree."""
from pathlib import Path
import sys
sys.path.insert(0, 'tools')
import autotweak
path = Path('src/ovl/NAME_ENT/80139740.c')
source = Path('build/T-25057/func_801398EC/remove-tail-use/body.c').read_text()
Path('drafts/T-25057/func_801398EC-matched.c').write_text(source)
new = autotweak.rewrite_guard(path.read_text(), 'func_801398EC', source)
assert new is not None, 'target must still be guarded before promotion'
path.write_text(new)
