"""Docker-only edit-based scoring of OPTION func_80133010 forms, T-24140.

Usage: python3 drafts/T-24140/probe_33010.py forms.json
forms.json: [[label, old, new], ...] applied (in order) to the current
func_80133010 body in src/; empty old means the current body unchanged
(control). Scores via funcdiff_verify.compile_unit (no ninja).
"""
import sys, tempfile, json
from pathlib import Path
sys.path.insert(0, 'tools')
import funcdiff as fd
import funcdiff_verify as proof

NAME = 'func_80133010'
UNIT = 'OPTION'

c = fd.locate(NAME, UNIT)
e, problem = fd.expected_object(c)
assert not problem, problem
want = fd.resolved_functions(e, [NAME], c)[NAME]
text = Path(c.src).read_text()

anchor = 'void %s(void) {' % NAME
b0 = text.index(anchor) + len(anchor)
depth = 1
i = b0
while depth:
    if text[i] == '{':
        depth += 1
    elif text[i] == '}':
        depth -= 1
    i += 1
head, cur, tail = text[:b0], text[b0:i - 1], text[i - 1:]
print('control body chars:', len(cur), flush=True)

failed = False
import re
for item in json.loads(Path(sys.argv[1]).read_text()):
    label = item[0]
    pairs = item[1] if isinstance(item[1], list) else [[item[1], item[2]]]
    body = cur
    for old, new in pairs:
        if old:
            assert old in body, 'anchor missing for %s' % label
            body = body.replace(old, new)
    d = Path(tempfile.mkdtemp(dir='/tmp'))
    src = d / 'candidate.c'
    src.write_text(head + body + tail)
    try:
        b = proof.compile_unit(c, d, source=str(src))[0]
        got = fd.resolved_functions(b, [NAME])[NAME]
        diff = fd.diff_lines(want, got)
        print(label, 'score', len(diff), 'aligned', fd.diff_score(want, got), flush=True)
        Path('drafts/T-24140/%s.diff' % label).write_text('\n'.join(diff))
    except Exception as ex:
        failed = True
        print(label, 'ERROR', str(ex)[:300], flush=True)
if failed:
    sys.exit(1)
