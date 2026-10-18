"""Compile and measure the saved EVENT draft; never modifies production source. Docker only."""
from pathlib import Path
import sys,json,hashlib
sys.path.insert(0,'tools')
import funcdiff as fd,order_search
c=fd.locate('func_801103A0','EVENT');base=Path(c.src).read_text()
base=base.replace('#include "ovl/EVENT.h"','#define func_801103A0 T24023_old_declaration\n#include "ovl/EVENT.h"\n#undef func_801103A0')
needle=next(l for l in base.splitlines() if 'INCLUDE_ASM' in l and 'func_801103A0' in l)
body=Path('drafts/T-24023/body.c').read_text()
e,problem=fd.expected_object(c)
if problem:raise RuntimeError(problem)
want=fd.resolved_functions(e,['func_801103A0'],c)['func_801103A0']
variants={'saved':body}
if '--layout' in sys.argv:variants['remove_unused_scalar']=body.replace('    s32 var_at;\n','')
for label,variant in variants.items():
    out=Path('build/t24023/checkpoint')/label
    obj=order_search.compile_variant(c,base.replace(needle,variant),out,False)
    got=fd.resolved_functions(obj,['func_801103A0'])['func_801103A0']
    record={'variant':label,'aligned_cost':fd.diff_score(want,got),'printed_rows':len(fd.diff_lines(want,got)),'expected_instructions':len(want),'draft_instructions':len(got),'body_sha256':hashlib.sha256(variant.encode()).hexdigest(),'matching_claim':False,'coverage_bytes_added':0}
    print(json.dumps(record),flush=True)
    (out/'measurement.json').write_text(json.dumps(record,indent=2)+'\n')
