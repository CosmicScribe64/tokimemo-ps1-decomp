from pathlib import Path
import sys
sys.path.insert(0,'tools')
import funcloc,funcdiff,order_search
func='func_80148B08';c=funcloc.locate(func,'RPG_BAT');src=Path(c.src).read_text().replace('s32 '+func,'void '+func).replace('#include "ovl/RPG_BAT.h"','#include "ovl/RPG_BAT.h"\n\nvoid func_80148B08(void);')
folder=Path('build/t14900/void-prototype');obj=order_search.compile_variant(c,src,folder,False);expected,err=funcdiff.expected_object(c);assert not err
want=funcdiff.resolved_functions(expected,[func],c)[func];got=funcdiff.resolved_functions(obj,[func])[func];diff=funcdiff.diff_lines(want,got);(folder/'comparison.txt').write_text('\n'.join(diff));print(0 if funcdiff.keys(want)==funcdiff.keys(got) else len(diff))
