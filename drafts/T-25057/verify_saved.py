"""Recompile the two annotated full-TU source checkpoints inside Docker."""
from pathlib import Path
import json
import sys
sys.path.insert(0, 'drafts/T-24515/replay')
import probe
probe.ROOT = Path('build/T-25057')
name = 'func_801398EC'
results = []
for filename, label in [('func_801398EC.c', 'saved-scalar'), ('func_801398EC-array.c', 'saved-array')]:
    rows = probe.probe(name, label, (Path('drafts/T-25057')/filename).read_text())
    c = probe.funcloc.locate(name, 'NAME_ENT')
    expected, problem = probe.funcdiff.expected_object(c)
    assert problem is None, problem
    want = probe.funcdiff.resolved_functions(expected, [name], c)[name]
    got = probe.funcdiff.resolved_functions(str(probe.ROOT/name/label/'guarded.o'), [name])[name]
    same = sum(a[0] == b[0] for a,b in zip(want, got))
    results.append(dict(source=filename, target_words=len(want), compiled_words=len(got), identical_positions=same, differing_positions=max(len(want),len(got))-same, printed_rows=rows))
print(json.dumps(results, indent=2))
(Path('drafts/T-25057')/'saved-results.json').write_text(json.dumps(results, indent=2)+'\n')
