"""T-24518 actual full-TU ETC source probe; run only inside project Docker."""
import argparse
from pathlib import Path
import sys
sys.path.insert(0, 'tools')
import draft_fixes
import funcdiff
import funcloc
import order_search

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('body')
parser.add_argument('--scope', default='ETC:func_8013FFAC')
parser.add_argument('--callee-int')
args = parser.parse_args()
unit,func = args.scope.split(':')
c = funcloc.locate(func, unit)
original = Path(c.src).read_bytes()
body = Path(args.body).read_text()
out = Path('build/T-24518') / Path(args.body).stem
out.mkdir(parents=True, exist_ok=True)
full = draft_fixes.fresh_context(dict(func=func, src=str(c.src), scope=args.scope), body, out)
assert full is not None
if args.callee_int:
    old = 'void ' + args.callee_int + '('
    assert old in full
    full = full.replace(old, 's32 ' + args.callee_int + '(')
if 's32 func_8013FFAC(void)' in body:
    header = out / 'ETC.h'
    header.write_text(Path('include/ovl/ETC.h').read_text().replace(
        'void func_8013FFAC(void);', 's32 func_8013FFAC(void);'))
    full = full.replace('#include "ovl/ETC.h"', '#include "' + str(header.resolve()) + '"')
expected, problem = funcdiff.expected_object(c)
assert not problem, problem
want = funcdiff.resolved_functions(expected, [func], c)[func]
obj = order_search.compile_variant(c, full, out, False)
got = funcdiff.resolved_functions(obj, [func])[func]
print('%s: target=%d candidate=%d words; rows=%d cost=%d' % (
    args.body, len(want), len(got), len(funcdiff.diff_lines(want, got)), funcdiff.diff_score(want, got)))
(out / 'diff.txt').write_text('\n'.join(funcdiff.diff_lines(want, got)))
for name, object_path in [('want', expected), ('got', obj)]:
    insns = funcdiff.object_instructions(object_path, func)
    rows = []
    active = False
    for insn in insns:
        relocs = [r[1] for r in insn[2]]
        if 'func_80048514' in relocs or 'load_tpage_buf_lock' in relocs:
            active = True
        if active:
            rows.append('%04x %s' % (insn.offset, insn[1]))
        if active and 'func_8004A1A0' in relocs:
            break
    (out / (name + '-first-loop.txt')).write_text('\n'.join(rows))
assert Path(c.src).read_bytes() == original
print('object=' + obj)
