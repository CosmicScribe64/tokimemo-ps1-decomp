"""T-25142 scratch scorer: compile body files as the guard of func_80134D94, print cost; --sbs writes side-by-side."""
import argparse, sys, difflib, re
from pathlib import Path
sys.path.insert(0, 'tools')
import autotweak, funcdiff, funcloc, order_search, protoverride
from funcdiff_verify import command

FUNC = 'func_80134D94'; UNIT = 'BUNKA_SD'

def norm(d):
    return d

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('bodies', nargs='+')
    ap.add_argument('--proto', action='append', default=[])
    ap.add_argument('--sbs', action='store_true')
    ap.add_argument('--hdr', action='append', default=[], help='old=>new replace in full text')
    a = ap.parse_args()
    c = funcloc.locate(FUNC, UNIT)
    original = Path(c.src).read_text()
    expected, problem = funcdiff.expected_object(c)
    if problem: raise SystemExit(problem)
    want = funcdiff.resolved_functions(expected, [FUNC], c)[FUNC]
    for b in a.bodies:
        body = Path(b).read_text()
        full = autotweak.rewrite_guard(original, FUNC, body)
        for h in a.hdr:
            old, new = h.split('=>')
            full = full.replace(old, new)
        out = Path('build/T-25142/runs') / Path(b).stem
        out.mkdir(parents=True, exist_ok=True)
        if a.proto:
            pre = out / 'pre.c'; pre.write_text(full)
            full = command(['gcc', '-E', '-P', '-Iinclude', '-D__sgi', '-D__attribute__(x)=',
                '-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)', '-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)', str(pre)])
            full, _ = protoverride.apply(full, protoverride.parse(a.proto), allow_defined=True)
        try:
            obj = order_search.compile_variant(c, full, out, False)
        except Exception as e:
            print('%s: COMPILE FAIL %s' % (b, str(e)[-400:])); continue
        got = funcdiff.resolved_functions(obj, [FUNC])[FUNC]
        if got is None:
            print('%s: no function' % b); continue
        print('%s: target=%d cand=%d rows=%d cost=%d' % (b, len(want), len(got),
              len(funcdiff.diff_lines(want, got)), funcdiff.diff_score(want, got)))
        if a.sbs:
            W = [r[1] if isinstance(r, tuple) else r for r in want]
            G = [r[1] if isinstance(r, tuple) else r for r in got]
            KW = funcdiff.keys(want); KG = funcdiff.keys(got)
            sm = difflib.SequenceMatcher(None, KW, KG, autojunk=False)
            lines = []
            for op, i1, i2, j1, j2 in sm.get_opcodes():
                n = max(i2 - i1, j2 - j1)
                for k in range(n):
                    l = W[i1 + k] if i1 + k < i2 else ''
                    r = G[j1 + k] if j1 + k < j2 else ''
                    l = re.sub(r'^\S+\s+', '', l); r = re.sub(r'^\S+\s+', '', r)
                    mark = ' ' if op == 'equal' else '|'
                    lines.append('%3d %-36s %s %s' % (i1 + k if i1 + k < i2 else -1, l[:36], mark, r))
            (out / 'sbs.txt').write_text('\n'.join(lines) + '\n')
            print('  sbs ->', out / 'sbs.txt')
    assert Path(c.src).read_text() == original

main()
