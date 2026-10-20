"""Compile the narrow ENDING slot checkpoint in Docker after the ENDING target build.

Scores normalize section-owned tables/strings. No literal linked C match is claimed.
"""
from pathlib import Path
import json
import sys
sys.path.insert(0, 'tools')
import funcdiff
import order_search

name = 'func_801330E4'
context = funcdiff.locate(name, 'ENDING')
source = Path(context.src).read_text()
anchor = 'INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133030", ' + name + ');'
assert source.count(anchor) == 1
body = Path('drafts/T-24560/' + name + '.c').read_text()
assert sys.argv[1:] in ([], ['--raw-mask']), 'only --raw-mask is supported'
if sys.argv[1:]:
    body = body.replace('s32 pad[4];', 's32 pad[3];')
    body = body.replace('    s16 best;', '    u32 raw_bonus;\n    s16 best;', 1)
    field = 'D_800E6280.unk_1BC[0xB].unk_0C.f.f9'
    assert body.count(field) == 2
    body = body.replace('        level.value = ' + field,
                        '        raw_bonus = D_800E6280.unk_1BC[0xB].unk_0C.w;\n'
                        '        level.value = ((raw_bonus << 20) >> 29)', 1)
    body = body.replace(field + ' != 0', '(raw_bonus & 0xE00) != 0', 1)
work = Path('build/t24560/replay-raw-mask' if sys.argv[1:] else 'build/t24560/replay')
work.mkdir(parents=True, exist_ok=True)
score = order_search.make_text_scorer(context, name, resolve=True)(source.replace(anchor, body), work)
assert score < 1000000, 'candidate did not compile'
expected, problem = funcdiff.expected_object(context)
assert not problem, problem
want = funcdiff.resolved_functions(expected, [name], context)[name]
got = funcdiff.resolved_functions(str(work / 'guarded.o'), [name], context)[name]
prefix = 0
for left, right in zip(want, got):
    if left[0] != right[0]:
        break
    prefix += 1
result = dict(aligned_score=score, target_positions=len(want), candidate_positions=len(got),
              diagnostic_prefix=prefix, linked_c_match=False)
(work / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result))
