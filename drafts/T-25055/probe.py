"""T-25055 whole-TU TEL causal probes; run from repository root in Docker."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import sys

sys.path.insert(0, 'tools')
import cc
import funcdiff
import funcdiff_verify
import order_search
import permute

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('body', type=Path)
parser.add_argument('--capture', action='store_true')
args = parser.parse_args()
name = 'func_80136724'
unit = funcdiff.locate(name, 'TEL')
source = Path(unit.src).read_text()
body = args.body.read_text()
text = permute.replace_function(source, name, body)
if text is None:
    raise SystemExit('target function missing from current production TU')
work = Path('build/T-25055') / args.body.stem
work.mkdir(parents=True, exist_ok=True)
if args.capture:
    os.environ['TEL_CAPTURE'] = str((work / 'capture').resolve())
    for stage in ('uopt', 'ugen', 'as1'):
        cc.SHIMS[stage] = str(Path('drafts/T-25055/capture_stage.py').resolve())
    command = funcdiff_verify.command
    def capture_command(argv, *a, **kw):
        if argv[:2] == ['python3', 'tools/cc.py']:
            cc.main(argv[1:])
            return ''
        return command(argv, *a, **kw)
    funcdiff_verify.command = capture_command
obj = order_search.compile_variant(unit, text, work, False)
expected, error = funcdiff.expected_object(unit)
if error:
    raise SystemExit(error)
want = funcdiff.resolved_functions(expected, [name], unit)[name]
got = funcdiff.resolved_functions(obj, [name], unit)[name]
a, b = funcdiff.keys(want), funcdiff.keys(got)
result = dict(body=str(args.body), source_sha256=hashlib.sha256(body.encode()).hexdigest(),
              tu_sha256=hashlib.sha256(text.encode()).hexdigest(), target_words=len(a),
              candidate_words=len(b), changed_positions=sum(x != y for x,y in zip(a,b))+abs(len(a)-len(b)),
              aligned_cost=funcdiff.diff_score(want,got))
(work/'result.json').write_text(json.dumps(result,indent=2)+'\n')
(work/'diff.txt').write_text('\n'.join(funcdiff.diff_lines(want,got))+'\n')
(work/'target-private.json').write_text(json.dumps(want))
(work/'candidate-private.json').write_text(json.dumps(got))
print(json.dumps(result))
