"""T-25024 complete-TU TEL qualification replay; run from repository root in Docker."""
from pathlib import Path
import argparse
import hashlib
import json
import sys

sys.path.insert(0, 'tools')
import cc
import funcdiff
import order_search

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('body', type=Path)
parser.add_argument('--void-rank', action='store_true', help='rejected ABI diagnostic')
args = parser.parse_args()
name = 'func_80136724'
unit = funcdiff.locate(name, 'TEL')
source = Path(unit.src).read_text()
needle = next(line for line in source.splitlines() if 'INCLUDE_ASM' in line and name in line)
body = args.body.read_text()
text = source.replace(needle, body)
if args.void_rank:
    text = '#define MAIN_API_OVERRIDE_func_80053418 /* T-25024 diagnostic: matched caller void ABI donor. */\n' + text
    text = text.replace('void set_c_girl();', 'void set_c_girl();\nvoid func_80053418();')
expected, problem = funcdiff.expected_object(unit)
if problem:
    raise SystemExit(problem)
want = funcdiff.resolved_functions(expected, [name], unit)[name]
work = Path('build/T-25024/replay') / (args.body.stem + ('-void' if args.void_rank else ''))
obj = order_search.compile_variant(unit, text, work, False)
got = funcdiff.resolved_functions(obj, [name], unit)[name]
want_keys, got_keys = funcdiff.keys(want), funcdiff.keys(got)
first = next((i for i, (a, b) in enumerate(zip(want_keys, got_keys)) if a != b), None)
result = {
    'body': str(args.body),
    'source_sha256': hashlib.sha256(body.encode()).hexdigest(),
    'tu_sha256': hashlib.sha256(text.encode()).hexdigest(),
    'target_words': len(want),
    'candidate_words': len(got),
    'printed_lines': len(funcdiff.diff_lines(want, got)),
    'changed_positions': sum(a != b for a, b in zip(want_keys, got_keys)) + abs(len(want_keys) - len(got_keys)),
    'aligned_cost': funcdiff.diff_score(want, got),
    'first_divergence': first,
    'first_target': want[first][1] if first is not None else None,
    'first_candidate': got[first][1] if first is not None else None,
    'toolchain': cc.toolchain_of(unit.name),
}
(work / 'result.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result, indent=2))
