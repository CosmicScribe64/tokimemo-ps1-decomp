"""Replay EVENT closure and negative controls in full translation units, in Docker.

Usage: python3 drafts/T-24023/closure/replay.py [OUTPUT_DIRECTORY]
The zero-score candidate receives an independent literal linked-byte proof.
"""
from pathlib import Path
import json
import sys

sys.path.insert(0, 'tools')
import funcdiff as fd
import funcdiff_verify as proof
import order_search

name = 'func_801103A0'
c = fd.locate(name, 'EVENT')
root = Path(sys.argv[1] if len(sys.argv) > 1 else 'build/t24023-replay')
source = Path(c.src).read_text()
matched = Path('drafts/T-24023/closure/body.c').read_text().strip()
assert source.count(matched) == 1, 'saved closure body differs from production'
shared = matched
for op in ('* 1', '^ 0', '| 0'):
    shared = shared.replace('(g->unk_036 ' + op + ')', 'g->unk_036')
variants = {
    'matched': matched,
    'direct_selector_symbol': matched.replace(
        '(((u8 *)&D_801243A4)[0xC])', '(*(u8 *)&D_801243B0)'),
    'shared_month_equality': shared,
    'literal_comparison_constants': matched.replace(
        '(date_ok = 1)', '1').replace('(date_ok = 2)', '2'),
    'old_checkpoint': Path('drafts/T-24023/body.c').read_text(),
}
expected, problem = fd.expected_object(c)
if problem:
    raise RuntimeError(problem)
want = fd.resolved_functions(expected, [name], c)[name]
for label, body in variants.items():
    out = root / label
    obj = order_search.compile_variant(c, source.replace(matched, body), out, False)
    got = fd.resolved_functions(obj, [name])[name]
    record = {'variant': label, 'aligned_cost': fd.diff_score(want, got),
              'printed_rows': len(fd.diff_lines(want, got)),
              'expected_instructions': len(want), 'built_instructions': len(got)}
    if record['aligned_cost'] == 0:
        equal, detail = proof.exact_match(obj, c, name, out, build=False, is_c=True)
        record.update(literal_equal=equal, detail=detail)
        assert equal, detail
    if label == 'matched':
        assert record['aligned_cost'] == 0
    print(json.dumps(record), flush=True)
    (out / 'measurement.json').write_text(json.dumps(record, indent=2) + '\n')
