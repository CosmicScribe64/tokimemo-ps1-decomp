#!/usr/bin/env python3
"""usage (in docker, repo root): python3 drafts/T-23004/try.py UNIT:func [--hunks N] [--ctx K] BODY.c ...
Each BODY.c replaces the guarded body of func in the unit file (in memory), compiled in the exact full-unit way."""
import sys, os, re, difflib, hashlib, concurrent.futures as cf
from pathlib import Path
sys.path.append("tools")
import funcdiff, funcloc, order_search

args = sys.argv[1:]
hunks = 6; ctx = 1; jobs = 2
while args and args[0].startswith("-"):
    o = args.pop(0)
    if o == "--hunks": hunks = int(args.pop(0))
    elif o == "--ctx": ctx = int(args.pop(0))
    elif o == "-j": jobs = int(args.pop(0))
spec = args.pop(0)
name, scope = funcloc.split_scope(spec, None)
c = funcloc.locate(name, scope)
src = Path(c.src).read_text()
inc = re.compile(r'(/\*[^\n]*\*/\n#ifdef NON_MATCHING\n)((?:(?!#endif).)*?)(\n#else\nINCLUDE_ASM\([^\n]*' + re.escape(name) + r'\);\n#endif\n)', re.S)
assert inc.search(src), "guard for %s not found" % name
expected, problem = funcdiff.expected_object(c)
assert not problem, problem
want = funcdiff.resolved_functions(expected, [name], c)[name]
def _norm(r):
    d = r[1] if isinstance(r, tuple) else r
    t = d.split(";", 1)[1].strip() if ";" in d else d
    m = re.match(r"(b|beq|bne|beqz|bnez|bgez|bgtz|blez|bltz|bgezal|bltzal|j)\s+(.*)", t)
    if m:
        ops = m.group(2)
        ops = re.sub(r"\s*[0-9a-f]+ <[^>]*>\s*$", "", ops)
        ops = re.sub(r"\s*<[^>]*>\s*$", "", ops)
        return m.group(1) + " " + ops
    return funcdiff.keys([r])[0]
def keys(rows):
    return [_norm(r) for r in rows]

def run(path):
    body = Path(path).read_text().rstrip("\n")
    text = inc.sub(lambda m: m.group(1) + body + m.group(3), src, count=1)
    work = Path("build/p04/work") / hashlib.sha1(path.encode()).hexdigest()[:10]
    try:
        obj = order_search.compile_variant(c, text, work, True)
        got = funcdiff.resolved_functions(obj, [name], c)[name]
    except BaseException as e:
        return path, None, "compile failed: %s" % (str(e).splitlines() or [""])[:3]
    from Levenshtein import distance
    return path, got, distance(keys(want), keys(got), weights=(1, 1, 2))

def frame(lines):
    for r in lines[:6]:
        d = r[1] if isinstance(r, tuple) else r
        m = re.search(r"addiu sp,sp,(-?\d+)", d)
        if m: return int(m.group(1))

print("want: %d insns frame %s" % (len(want), frame(want)))
_disp = lambda r: r[1] if isinstance(r, tuple) else r
open("build/p04/want.asm", "w").write("\n".join("%d %s" % (i, _disp(r).split(";",1)[-1].strip()) for i, r in enumerate(want)) + "\n")
with cf.ThreadPoolExecutor(jobs) as ex:
    res = list(ex.map(run, args))
for path, got, score in res:
    if got is not None:
        open(path.replace(".c", ".asm"), "w").write("\n".join("%d %s" % (i, _disp(r).split(";",1)[-1].strip()) for i, r in enumerate(got)) + "\n")
    if got is None:
        print("%s: %s" % (path, score)); continue
    print("%s: score %s insns %d frame %s" % (path, score, len(got), frame(got)))
if res and res[-1][1] is not None or True:
    best = min([r for r in res if r[1] is not None], key=lambda r: r[2], default=None)
    if best:
        path, got, score = best
        print("== hunks of %s (score %s)" % (path, score))
        sm = difflib.SequenceMatcher(None, keys(want), keys(got), autojunk=False)
        ops = sm.get_opcodes()
        disp = lambda r: r[1] if isinstance(r, tuple) else r
        shown = 0
        for idx, (op, i1, i2, j1, j2) in enumerate(ops):
            if op == "equal": continue
            if shown >= hunks: break
            shown += 1
            print("-- hunk %d: want[%d:%d] got[%d:%d]" % (shown, i1, i2, j1, j2))
            for k in range(max(0, i1 - ctx), i1): print("   ", disp(want[k]))
            for k in range(i1, i2): print("  -", disp(want[k]))
            for k in range(j1, j2): print("  +", disp(got[k]))
            for k in range(i2, min(len(want), i2 + ctx)): print("   ", disp(want[k]))
        print("total non-equal hunks: %d" % sum(1 for o in ops if o[0] != "equal"))
