"""T-24511 bounded causal TEL source probes; Docker only, no production edits."""
from pathlib import Path
import sys, json, re
sys.path.insert(0, 'tools')
import draft_fixes, funcloc, funcdiff, order_search
c=funcloc.locate('func_8013D88C','TEL')
root=Path('build/T-24511')
want=funcdiff.resolved_functions(funcdiff.expected_object(c)[0], ["func_8013D88C"], c)["func_8013D88C"]
base=Path('drafts/T-24039/menu-reconstruction.c').read_text()
variants=[('control',base)]
fix=base.replace('case 12:\n                    case 2:', 'case 14:\n                    case 12:\n                    case 2:')
variants.append(('month14',fix))
split=fix.replace('x = (- -((s32)i / 5) * 0x88) - 0x88;', 'x = -((s32)i / 5);\n            x = -x * 0x88 - 0x88;')
variants.append(('split-x',split))
reuse=split.replace('    s32 byteOffset;\n','').replace('byteOffset','arg1')
variants.append(('reuse-arg1',reuse))
byte=split.replace('GameState *flags;', 'u8 *flags;').replace('flags = &D_800E6280;', 'flags = (u8 *)&D_800E6280;').replace('flags->unk_120[0]','flags[0x120]').replace('flags->unk_120[10]','flags[0x12A]').replace('flags = (GameState *)((u8 *)flags + 1);','flags++;')
variants.append(('byte-flags',byte))
variants.append(('byte-reuse',byte.replace('    s32 byteOffset;\n','').replace('byteOffset','arg1')))
chain=split.replace('                }\n                if (i == 3)', '                } else if (i == 3)')
variants.append(('exclusive-chain',chain))
variants.append(('chain-reuse',chain.replace('    s32 byteOffset;\n','').replace('byteOffset','arg1')))
for name,body in variants:
    out=root/name
    out.mkdir(exist_ok=True)
    (out/'body.c').write_text(body)
    full=draft_fixes.fresh_context(dict(func="func_8013D88C",src=str(c.src),scope='TEL:'+"func_8013D88C"),body,out)
    obj=order_search.compile_variant(c,full,out,False)
    got=funcdiff.resolved_functions(obj,["func_8013D88C"])["func_8013D88C"]
    lines=funcdiff.diff_lines(want,got)
    (out/'diff.txt').write_text('\n'.join(lines))
    print(json.dumps(dict(name=name,words=len(got),rows=len(lines))),flush=True)
