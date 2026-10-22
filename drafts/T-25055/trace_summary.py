"""Summarize selected allocator ranges from private regdiff captures, Docker only."""
import json
from pathlib import Path
import sys
sys.path.insert(0,'tools')
import regdiff
import permute

rows = {}
for label,folder in [('combined','regdiff-combined'),('final','regdiff-final')]:
    root=Path('build/T-25055')/folder/'func_80136724'
    proc=regdiff.parse_listing((root/'draft.o.trace').read_text(),regdiff.parse_stderr((root/'draft.log').read_text()))['func_80136724']
    text=(root/'draft.c').read_text()
    start=text.count('\n',0,permute.def_regex('func_80136724').search(text).start())+1
    parse,ops=regdiff.load_ucode_parser()
    refs=regdiff.ucode_refs((root/'draft.B').read_bytes(),parse,ops)
    names=regdiff.name_variables(proc,refs,text.split('\n'),start)
    result=[]
    for r in regdiff.final_ranges(proc):
        desc,_=regdiff.describe_range(r,proc,names)
        identity=proc.table.get(r.bit)
        if identity and (identity.get('cls'),identity.get('l'),identity.get('r')) == ('S',6,0xF5F):
            desc='global GameState.unk_F5F (girl byte)'
        if r.reg in (8,9) or any(desc == 'local '+n for n in ('lim','hp','total','ent')):
            result.append(dict(description=desc,bit=r.bit,lr=r.lr,priority=r.priority,
                               uses=r.loads,defs=r.stores,blocks=r.blocks,
                               allocation_order=r.order,register=regdiff.rname(r.reg) if r.reg else None,
                               identity=identity))
    rows[label]=result
p=Path('drafts/T-25055/allocator-summary.json')
p.write_text(json.dumps(rows,indent=2)+'\n')
print(json.dumps(rows,indent=2))
