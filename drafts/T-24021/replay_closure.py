"""Reproduce the dialogue closure from its frozen pre-match C, inside Docker.

Usage: python3 drafts/T-24021/replay_closure.py BASELINE_C [OUTPUT_DIRECTORY]
The reference is committed source, never game data or patched instructions.
"""
from pathlib import Path
import json
import sys
sys.path.insert(0, 'tools')
import funcdiff as fd
import funcdiff_verify as proof
import order_search

name = 'dialogue_speak_part'
c = fd.locate(name, 'main')
if len(sys.argv) < 2:
    raise SystemExit('Pass the baseline C exported from b312d1e24; see README.md')
root = Path(sys.argv[2] if len(sys.argv) > 2 else 'build/t24021-replay')
base = Path(sys.argv[1]).read_text()
lines = base.replace('    s32 pad[1];', '    s32 render_x;').splitlines(True)
sites = []
for i, line in enumerate(lines):
    if line.strip() == 'temp_s1 += 0x10;' and 'dialogue_render(-0x28,' in lines[i + 1]:
        assert 'strcmp(sp7C,' in lines[i - 1]
        sites.append(i)
assert len(sites) == 2
for i in reversed(sites):
    lines[i - 1] = lines[i - 1].replace('if (strcmp(', 'if ((render_x = strcmp(').replace(') == 0)', ')) == 0)')
    lines[i + 1] = lines[i + 1].replace('dialogue_render(-0x28,', 'dialogue_render(render_x,')
    lines.insert(i, '            render_x -= 40;\n')
early = ''.join(lines)
variants = {'baseline': base, 'early_dependency': early,
            'masked_constant': early.replace('render_x -= 40;', 'render_x = (render_x & 0) - 40;')}
expected, problem = fd.expected_object(c)
if problem:
    raise RuntimeError(problem)
want = fd.resolved_functions(expected, [name], c)[name]
for label, source in variants.items():
    out = root / label
    obj = order_search.compile_variant(c, source, out, True)
    got = fd.resolved_functions(obj, [name])[name]
    record = {'variant': label, 'aligned_cost': fd.diff_score(want, got),
              'printed_rows': len(fd.diff_lines(want, got)),
              'expected_instructions': len(want), 'built_instructions': len(got)}
    if record['aligned_cost'] == 0:
        equal, detail = proof.exact_match(obj, c, name, out, build=False, is_c=True)
        record.update(literal_equal=equal, detail=detail)
        assert equal, detail
    print(json.dumps(record), flush=True)
    (out / 'measurement.json').write_text(json.dumps(record, indent=2) + '\n')
