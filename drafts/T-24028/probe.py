"""T-24028: serial full-TU source-only draft reproduction, run in project Docker.

Usage: python3 drafts/T-24028/probe.py UNIT FUNC BODY.c [--proto 's32 callee();']
Prints diagnostic counts only; never claims a match or edits production source.
"""
import argparse
from pathlib import Path
import sys

sys.path.insert(0, 'tools')
import autotweak
import draft_fixes
import funcdiff
import funcloc
import order_search
import protoverride
from funcdiff_verify import command


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('unit')
    parser.add_argument('func')
    parser.add_argument('body')
    parser.add_argument('--proto', action='append', default=[])
    args = parser.parse_args()
    c = funcloc.locate(args.func, args.unit)
    out = Path('build/T-24028/reproduce') / args.func
    out.mkdir(parents=True, exist_ok=True)
    row = dict(func=args.func, src=str(c.src), scope=args.unit + ':' + args.func)
    body = Path(args.body).read_text()
    original = Path(c.src).read_text()
    if any(g.func == args.func for g in autotweak.guards(original)):
        full = autotweak.rewrite_guard(original, args.func, body)
    else:
        full = draft_fixes.fresh_context(row, body, out)
    if full is None:
        raise SystemExit('target must still have its production INCLUDE_ASM fallback')
    if args.proto:
        scratch = out / 'pre.c'
        scratch.write_text(full)
        full = command(['gcc', '-E', '-P', '-Iinclude', '-I' + str(Path(c.src).parent),
                        '-D__sgi', '-D__attribute__(x)=',
                        '-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)',
                        '-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)', str(scratch)])
        full, _ = protoverride.apply(full, protoverride.parse(args.proto), allow_defined=True)
    expected, problem = funcdiff.expected_object(c)
    if problem:
        raise SystemExit(problem)
    want = funcdiff.resolved_functions(expected, [args.func], c)[args.func]
    obj = order_search.compile_variant(c, full, out, False)
    got = funcdiff.resolved_functions(obj, [args.func])[args.func]
    print('%s:%s: target=%d candidate=%d instructions; printable_diff_rows=%d' %
          (args.unit, args.func, len(want), len(got), len(funcdiff.diff_lines(want, got))))


if __name__ == '__main__':
    main()
