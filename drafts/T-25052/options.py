"""Inspect actual pre-uopt options, including the accepted sibling, in Docker."""
from pathlib import Path
import json, sys
sys.path.insert(0,'tools')
import cvt_pass as cv
for label in sys.argv[1:]:
    directory=Path('build/t25052')/label/'trace'
    records=cv.parse((directory/'draft.B').read_bytes())
    results={}
    for a,b in cv._procedures(records):
        for name in ['func_8006D138','func_8007132C']:
            if any(name.encode() in r.extra for r in records[a:b+1]):
                opts=[]; k=a-1
                while k>=0 and records[k].opc == cv.OP['optn']:
                    opts.append(records[k].words[1:3]); k-=1
                results[name]=opts
    (directory/'options.json').write_text(json.dumps(results,indent=2)+'\n')
    print(label,results)
