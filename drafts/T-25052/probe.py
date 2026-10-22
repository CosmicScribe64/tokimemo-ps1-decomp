"""Replay causal MAIN dispatcher candidates using the actual whole TU."""
from pathlib import Path
import json, subprocess, sys
sys.path.insert(0, 'tools')
import funcdiff as fd
import funcdiff_verify as fv
name = 'func_8007132C'
c = fd.locate(name, 'main')
original = Path(c.src).read_text()
needle = next(l for l in original.splitlines() if 'INCLUDE_ASM' in l and name in l)
body = Path('drafts/T-23003/main/func_8007132C/hand.c').read_text()
variants = {'baseline': body}
for label, qual in [('field', ''), ('volatile-calendar', 'volatile ')]:
    s = body
    for field in ['03E', '03F', '040']:
        old = 'D_800E6280.unk_' + field
        s = s.replace(old, '(*(' + qual + 'u8 *)&' + old + ')')
    variants[label] = s
record_base = variants['field'].replace('GsRec69C *p;', 'GameState *p;')
record_base = record_base.replace('&D_800E6280.unk_69C[D_800E6280.unk_71C]', '(GameState *)((u8 *)&D_800E6280 + D_800E6280.unk_71C * 4)')
record_base = record_base.replace('p->unk_', 'p->unk_69C[0].unk_').replace('p[-1].unk_', 'p->unk_69C[-1].unk_')
variants['record-base'] = record_base
variants['record-base-index-first'] = record_base.replace('(u8 *)&D_800E6280 + D_800E6280.unk_71C * 4', 'D_800E6280.unk_71C * 4 + (u8 *)&D_800E6280')
variants['record-base-shared-count'] = record_base.replace('    s32 n;\n', '').replace('n = 0;', 't = 0;').replace('n++;', 't++;').replace('if (n ', 'if (t ')
for parent in ['field', 'record-base', 'record-base-shared-count']:
    variants[parent + '-entry-switch'] = variants[parent].replace('    func_80071280();', '    switch ((u32)D_800E6280.unk_03E) { default: break; }\n    func_80071280();', 1)
shared = variants['record-base-shared-count']
variants['shared-ne-loops'] = shared.replace('i < 11', 'i != 11')
variants['shared-late-bound'] = shared.replace('i < 11', 'i != 11 + ((u32)D_800E6280.unk_03E < 0)')
variants['shared-late-record-base'] = shared.replace('(u8 *)&D_800E6280 + D_800E6280.unk_71C * 4', '(u8 *)&D_800E6280 + D_800E6280.unk_71C * 4 + ((u32)D_800E6280.unk_03E < 0)')
gs_arrays = shared.replace('    GameState *p;', '    GameState *p;\n    GameState *gs = &D_800E6280;')
for field in ['unk_1BC', 'unk_69C', 'unk_044']:
    gs_arrays = gs_arrays.replace('D_800E6280.' + field, 'gs->' + field)
variants['shared-gs-arrays'] = gs_arrays
variants['shared-pointer-zero-record'] = shared.replace('(u8 *)&D_800E6280 + D_800E6280.unk_71C * 4', '(u8 *)&D_800E6280 + D_800E6280.unk_71C * 4 + ((u32)&D_800E6280 < 0)')
variants['shared-index-view'] = shared.replace('D_800E6280.unk_71C', '(*(u8 *)&D_800E6280.unk_71C)')
views = variants['shared-index-view']
for field in ['71D', '71E', '71F']:
    old = 'D_800E6280.unk_' + field
    views = views.replace(old, '(*(u8 *)&' + old + ')')
variants['shared-schedule-byte-views'] = views
variants['field-shared-index-view'] = variants['field'].replace('    s32 n;\n', '').replace('n = 0;', 't = 0;').replace('n++;', 't++;').replace('if (n ', 'if (t ').replace('D_800E6280.unk_71C', '(*(u8 *)&D_800E6280.unk_71C)')
field_shared = variants['field'].replace('    s32 n;\n', '').replace('n = 0;', 't = 0;').replace('n++;', 't++;').replace('if (n ', 'if (t ')
variants['field-shared'] = field_shared
split_at = field_shared.index('    p = &D_800E6280.unk_69C')
variants['field-prefix-index-view'] = field_shared[:split_at].replace('D_800E6280.unk_71C', '(*(u8 *)&D_800E6280.unk_71C)') + field_shared[split_at:]
# A late-folded base identity keeps the conventional record pointer type.
variants['field-pointer-zero'] = field_shared.replace('&D_800E6280.unk_69C[D_800E6280.unk_71C]', '(GsRec69C *)((u8 *)&D_800E6280.unk_69C[D_800E6280.unk_71C] + ((u32)&D_800E6280 < 0))')
first_end = field_shared.index('    func_8006D138();')
variants['field-first-count-op'] = field_shared[:first_end].replace('t = 0;', 'op = 0;').replace('t++;', 'op++;').replace('if (t >= 6)', 'if (op >= 6)') + field_shared[first_end:]
variants['field-count-loop-use'] = field_shared.replace('                    t++;', '                    t++;\n                    if (t) {}', 1)
# Narrow index stores versus reads are distinct alias controls.
import re
variants['field-index-store-view'] = re.sub(r'D_800E6280.unk_71C(?= [+-]=)', '(*(u8 *)&D_800E6280.unk_71C)', field_shared)
first_exact = variants['field-first-count-op']
variants['first-exact-dead-index-view'] = first_exact.replace('    func_80071280();', '    if ((u32)*(u8 *)&D_800E6280.unk_71C < 0) {}\n    func_80071280();', 1)
variants['first-exact-dead-base-view'] = first_exact.replace('    func_80071280();', '    if ((u32)*(u8 *)&D_800E6280 < 0) {}\n    func_80071280();', 1)
variants['first-exact-unsigned-i'] = first_exact.replace('    s32 i;', '    u32 i;')
variants['first-exact-unsigned-bound'] = first_exact.replace('i < 11', 'i < 11U')
variants['first-exact-inclusive-bound'] = first_exact.replace('i < 11', 'i <= 10')
index_static = first_exact.replace('D_800E6280.unk_71C', '(*(u8 *)&D_800E6280.unk_71C)')
variants['static-index-late-subscript'] = index_static.replace('unk_69C[(*(u8 *)&D_800E6280.unk_71C)]', 'unk_69C[(*(u8 *)&D_800E6280.unk_71C) + ((u32)&D_800E6280 < 0)]')
variants['static-index-late-pointer'] = index_static.replace('&D_800E6280.unk_69C[(*(u8 *)&D_800E6280.unk_71C)]', '(GsRec69C *)((u8 *)&D_800E6280.unk_69C[(*(u8 *)&D_800E6280.unk_71C)] + ((u32)&D_800E6280 < 0))')
variants['static-index-self-zero'] = variants['static-index-late-subscript'].replace('((u32)&D_800E6280 < 0)', '((u32)*(u8 *)&D_800E6280.unk_71C < 0)')
variants['first-exact-integer-record'] = first_exact.replace('&D_800E6280.unk_69C[D_800E6280.unk_71C]', '(GsRec69C *)((u32)D_800E6280.unk_69C + D_800E6280.unk_71C * sizeof(GsRec69C))')
expected, problem = fd.expected_object(c)
if problem: raise RuntimeError(problem)
want = fd.resolved_functions(expected, [name], c)[name]
for label in sys.argv[1:] or variants:
    directory = Path('build/t25052') / label
    directory.mkdir(parents=True, exist_ok=True)
    candidate = variants[label]
    (directory / 'body.c').write_text(candidate)
    source = directory / 'unit.c'
    source.write_text(original.replace(needle, candidate))
    obj, _ = fv.compile_unit(c, directory, source=source, non_matching=False)
    got = fd.resolved_functions(obj, [name], c)[name]
    rows = fd.diff_lines(want, got)
    (directory / 'diff.txt').write_text('\n'.join(rows) + '\n')
    dump = subprocess.check_output(['mips-linux-gnu-objdump', '-drz', '--disassemble='+name, obj], text=True)
    (directory / 'disassembly.txt').write_text(dump)
    result = dict(target_words=len(want), candidate_words=len(got), printed_lines=len(rows), aligned_cost=fd.diff_score(want, got))
    (directory / 'measurement.json').write_text(json.dumps(result, indent=2)+'\n')
    print(label, result, flush=True)
assert Path(c.src).read_text() == original
