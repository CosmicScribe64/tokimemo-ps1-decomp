"""Replay the MASTER35FB8 structural draft in Docker after the MASTER target build.

funcdiff normalizes section-owned strings/tables: this is a diagnostic comparison,
not linked-byte proof. Only build/t24517-later/checkpoint is written.
"""
import json
from pathlib import Path
import sys
sys.path.insert(0, 'tools')
import funcdiff
import order_search

name = 'func_80135FB8'
context = funcdiff.locate(name, 'MASTER')
source = Path(context.src).read_text()
body = Path('drafts/T-24517/later/' + name + '.c').read_text()
anchor = 'INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", ' + name + ');'
assert source.count(anchor) == 1
text = source.replace(anchor, body)
work = Path('build/t24517-later/checkpoint35')
work.mkdir(parents=True, exist_ok=True)
cost = order_search.make_text_scorer(context, name, resolve=True)(text, work)
assert cost < 1000000, 'candidate did not compile'
expected, problem = funcdiff.expected_object(context)
assert not problem, problem
want = funcdiff.resolved_functions(expected, [name], context)[name]
got = funcdiff.resolved_functions(str(work / 'guarded.o'), [name], context)[name]
registers, other = [], []
prefix = 0
for index, (left, right) in enumerate(zip(want, got)):
    if prefix == index and left[0] == right[0]:
        prefix += 1
    lparts, rparts = left[0].split(maxsplit=1), right[0].split(maxsplit=1)
    lw, rw = int(lparts[0], 16), int(rparts[0], 16)
    opcode = lw >> 26
    assert opcode not in (16, 17, 18, 19) and rw >> 26 not in (16, 17, 18, 19)
    if opcode == 0:
        mask = 0xFFFFFFFF if (lw & 63) in (12, 13) else 0xFC0007FF
    elif opcode == 1:
        mask = 0xFC1FFFFF
    else:
        mask = 0xFFFFFFFF if opcode in (2, 3) else 0xFC00FFFF
    if (lw & mask) != (rw & mask) or lparts[1:] != rparts[1:]:
        other.append(index)
    elif lw != rw:
        registers.append(index)
result = dict(aligned_score=cost, expected_rows=len(want), candidate_rows=len(got),
              exact_diagnostic_prefix=prefix, register_only_rows=registers,
              other_rows=other, linked_c_match=False)
(work / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result))
assert len(want) == len(got) == 271
