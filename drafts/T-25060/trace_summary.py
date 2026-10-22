"""Summarize TEL allocator identities from private native traces; Docker only."""
import json
from pathlib import Path
import sys
sys.path.insert(0, 'tools')
import permute
import regdiff

result = {}
for label in ('baseline', 'arg0'):
    root = Path('build/T-25060/regdiff-' + label) / 'func_8013D88C'
    proc = regdiff.parse_listing((root / 'draft.o.trace').read_text(),
                                regdiff.parse_stderr((root / 'draft.log').read_text()))['func_8013D88C']
    text = (root / 'draft.c').read_text()
    start = text.count('\n', 0, permute.def_regex('func_8013D88C').search(text).start()) + 1
    refs = regdiff.ucode_refs((root / 'draft.B').read_bytes(), *regdiff.load_ucode_parser())
    names = regdiff.name_variables(proc, refs, text.split('\n'), start)
    rows = []
    for r in regdiff.final_ranges(proc):
        desc, _ = regdiff.describe_range(r, proc, names)
        if r.order and r.order <= 36 or desc in ('local xs', 'local ys', 'local steps'):
            rows.append(dict(description=desc, identity=proc.table.get(r.bit),
                             bit=r.bit, lr=r.lr, priority=r.priority, uses=r.loads,
                             defs=r.stores, blocks=r.blocks, order=r.order,
                             register=regdiff.rname(r.reg) if r.reg else None))
    result[label] = rows
Path('drafts/T-25060/allocator-summary.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps({k: len(v) for k, v in result.items()}))
