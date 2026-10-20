"""Replay bounded T-24513 ETC source controls in project Docker.

No production writes or matching claims. Use --case to select one causal control.
"""
import argparse
from pathlib import Path
import sys
import subprocess

sys.path.insert(0, 'tools')
import draft_fixes
import funcdiff
import funcloc
import order_search
import protoverride
from funcdiff_verify import command


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--case', choices=[
        'scene', 'ranking', 'ranking-walk', 'ranking-return', 'ranking-debug',
        'grade', 'grade-repeat', 'grade-view'], default='scene')
    args = parser.parse_args()
    proto = None
    if args.case.startswith('ranking'):
        func = 'func_801480D0'
        path = ('drafts/T-24513/walk-generation.c' if args.case == 'ranking-walk'
                else 'drafts/T-24049/exam-ranking.c')
        body = Path(path).read_text()
        body = body[body.index('void func_'):]
        if args.case == 'ranking-return':
            body = body.replace('void func_801480D0(void)', 's32 func_801480D0()')
        if args.case == 'ranking-debug':
            proto = 's32 func_800AE0B0();'
    elif args.case.startswith('grade'):
        func = 'func_80146D4C'
        body = Path('drafts/T-24303/ETC/func_80146D4C/body.c').read_text()
        if args.case != 'grade':
            body = body.replace('    s32 score;\n', '')
            body = body.replace('    score = w->unk_28[D_800E6280.unk_041];\n', '')
            body = body.replace('score <', 'w->unk_28[D_800E6280.unk_041] <')
        if args.case == 'grade-view':
            body = body.replace('D_800E6280.unk_041', '*(u8 *)&D_800E6280.unk_041')
    else:
        func = 'func_8013FFAC'
        body = Path('drafts/T-24513/scene.c').read_text()
    c = funcloc.locate(func, 'ETC')
    original = Path(c.src).read_bytes()
    out = Path('build/T-24513/replay') / args.case
    out.mkdir(parents=True, exist_ok=True)
    row = dict(func=func, src=str(c.src), scope='ETC:' + func)
    full = draft_fixes.fresh_context(row, body, out)
    if full is None:
        raise SystemExit('Expected production INCLUDE_ASM fallback')
    if proto:
        pre = out / 'pre.c'
        pre.write_text(full)
        full = command([
            'gcc', '-E', '-P', '-Iinclude', '-D__sgi', '-D__attribute__(x)=',
            '-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)',
            '-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)', str(pre)])
        full, _ = protoverride.apply(full, protoverride.parse([proto]), allow_defined=True)
    expected, problem = funcdiff.expected_object(c)
    if problem:
        raise SystemExit(problem)
    want = funcdiff.resolved_functions(expected, [func], c)[func]
    obj = order_search.compile_variant(c, full, out, False)
    got = funcdiff.resolved_functions(obj, [func])[func]
    result = subprocess.run([
        'python3', 'tools/funcdiff.py', '--built', str(obj), '--no-build',
        '--resolve', '--version', 'default', '--unit', 'ETC', func], text=True, capture_output=True)
    verdict = result.stdout + result.stderr
    if result.returncode not in (0, 1):
        raise SystemExit(verdict)
    if func not in verdict or not any(v in verdict for v in ['DIFF', 'MATCH']):
        raise SystemExit('Missing explicit-object diff verdict')
    print('%s: target=%d candidate=%d words; rows=%d aligned_cost=%d' % (
        args.case, len(want), len(got), len(funcdiff.diff_lines(want, got)),
        funcdiff.diff_score(want, got)))
    print(next(line for line in verdict.splitlines() if 'DIFF' in line or 'MATCH' in line))
    assert Path(c.src).read_bytes() == original, 'Production source changed'


if __name__ == '__main__':
    main()
