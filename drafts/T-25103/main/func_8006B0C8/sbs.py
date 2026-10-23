"""Side-by-side of guarded C vs original for one function (scratch helper, T-25103)."""
import sys, re, difflib, tempfile
from pathlib import Path
sys.path.insert(0, "/work/tools")
import funcdiff, funcdiff_verify as proof
name = sys.argv[1]
unit = sys.argv[2] if len(sys.argv) > 2 else "main"
mode = sys.argv[3] if len(sys.argv) > 3 else "sbs"
c = funcdiff.locate(name, unit)
job = Path(tempfile.mkdtemp(prefix="sbs-"))
b = proof.compile_guarded(c, job)
e, prob = funcdiff.expected_object(c, False, stale=[])
want = funcdiff.resolved_functions(e, [name], c)[name]
got = funcdiff.resolved_functions(b, [name], c)[name]
def txt(rows):
    out = []
    for r in rows:
        s = r[1]
        s = s.split(";", 1)[1].strip() if ";" in s else s
        out.append((r[0], s))
    return out
W, G = txt(want), txt(got)
if mode == "raw":
    print(want[:3]); sys.exit()
def norm(s):
    return re.sub(r"\b(t[0-9]|v[01]|a[0-3]|s[0-8]|ra|at)\b", "R", s)
sm = difflib.SequenceMatcher(None, [norm(x[1]) for x in W], [norm(x[1]) for x in G], autojunk=False)
nd = sum(1 for x in difflib.ndiff([x[0] for x in W],[x[0] for x in G]) if x[:1] in "+-")
print("len want %d got %d ratio %.3f rows %d" % (len(W), len(G), sm.ratio(), nd))
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    n = max(i2 - i1, j2 - j1)
    for k in range(n):
        a = W[i1 + k] if i1 + k < i2 else ("", "")
        g = G[j1 + k] if j1 + k < j2 else ("", "")
        mark = " " if a[0] == g[0] else ("~" if tag == "equal" else "!")
        a, g = a[1], g[1]
        print("%4s %-34s %s %-34s" % (i1 + k if i1 + k < i2 else "", a, mark, g))
