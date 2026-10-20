"""T-24516 full-TU source diagnostics, run inside the project Docker image.

Compile one actual draft with default flags. Production is never edited.
"""
import argparse
from pathlib import Path
import subprocess
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
    original = Path(c.src).read_bytes()
    body = Path(args.body).read_text()
    out = Path('build/T-24516/replay') / Path(args.body).stem
    out.mkdir(parents=True, exist_ok=True)
    if any(g.func == args.func for g in autotweak.guards(original.decode())):
        full = autotweak.rewrite_guard(original.decode(), args.func, body)
    else:
        full = draft_fixes.fresh_context(dict(
            func=args.func, src=str(c.src), scope=args.unit + ':' + args.func), body, out)
    if full is None:
        raise SystemExit('Expected production guard or INCLUDE_ASM fallback')
    if args.proto:
        pre = out / 'pre.c'
        pre.write_text(full)
        full = command([
            'gcc', '-E', '-P', '-Iinclude', '-D__sgi', '-D__attribute__(x)=',
            '-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)',
            '-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)', str(pre)])
        full, _ = protoverride.apply(full, protoverride.parse(args.proto), allow_defined=True)
    expected, problem = funcdiff.expected_object(c)
    if problem:
        raise SystemExit(problem)
    want = funcdiff.resolved_functions(expected, [args.func], c)[args.func]
    obj = order_search.compile_variant(c, full, out, False)
    got = funcdiff.resolved_functions(obj, [args.func])[args.func]
    result = subprocess.run([
        'python3', 'tools/funcdiff.py', '--built', str(obj), '--no-build',
        '--resolve', '--version', 'default', '--unit', args.unit, args.func],
        text=True, capture_output=True)
    verdict = result.stdout + result.stderr
    if result.returncode not in (0, 1) or args.func not in verdict:
        raise SystemExit(verdict)
    lines = [line for line in verdict.splitlines()
             if args.func in line and ('DIFF' in line or 'MATCH' in line)]
    if not lines:
        raise SystemExit('Missing explicit-object function verdict:\n' + verdict)
    print('%s:%s: target=%d candidate=%d words, rows=%d aligned_cost=%d' % (
        args.unit, args.func, len(want), len(got), len(funcdiff.diff_lines(want, got)),
        funcdiff.diff_score(want, got)))
    print(lines[0])
    assert Path(c.src).read_bytes() == original, 'Production source changed'


if __name__ == '__main__':
    main()
