"""usage: python3 ledger.py OUTPREFIX [-v REGION]  (OUTPREFIX.want/.got from score.py)
Region ledger for main:func_800476C0 (T-25606 r1-structure): splits the original at every jal and at the
loop/arm heads below, aligns on register-masked shapes and counts per region: rows, exact, register-only,
structural. -v N prints region N side by side."""
import sys, re, difflib
def load(p):
    return [re.sub(r"^[0-9a-f]{8} ", "", l) for l in open(p).read().split("\n") if l.strip() and not l.startswith("R_MIPS")]
def norm(l):
    l = re.sub(r"\b(zero|at|v0|v1|a0|a1|a2|a3|t[0-9]|s[0-8]|fp|sp|ra)\b", "R", l)
    l = re.sub(r"<[^>]*>", "<>", l)
    return re.sub(r"-?\b0x[0-9a-f]+\b|-?\b\d+\b", "N", l)
def strip_tgt(l):
    return re.sub(r"<[^>]*>", "<>", l)
w = load(sys.argv[1] + ".want"); g = load(sys.argv[1] + ".got")
# region starts (want row numbers) by hand from the original's listing
cuts = [0, 20, 28, 81, 112, 121]
for k, l in enumerate(w):
    if l.startswith("jal") and k + 2 < len(w):
        cuts.append(k + 2)
cuts = sorted(set(c for c in cuts if c < len(w)))
sm = difflib.SequenceMatcher(None, [norm(x) for x in w], [norm(x) for x in g], autojunk=False)
pair = {}
struct = set()
for op, i1, i2, j1, j2 in sm.get_opcodes():
    if op == "equal":
        for a, b in zip(range(i1, i2), range(j1, j2)):
            pair[a] = b
    else:
        struct.update(range(i1, i2))
        if i1 == i2:
            struct.add(i1)
verbose = int(sys.argv[3]) if len(sys.argv) > 3 and sys.argv[2] == "-v" else None
tot = [0, 0, 0, 0]
for r, a in enumerate(cuts):
    b = cuts[r + 1] if r + 1 < len(cuts) else len(w)
    ex = sum(1 for k in range(a, b) if k in pair and strip_tgt(w[k]) == strip_tgt(g[pair[k]]))
    st = sum(1 for k in range(a, b) if k in struct)
    rg = (b - a) - ex - st
    tot = [x + y for x, y in zip(tot, [b - a, ex, rg, st])]
    state = "exact" if ex == b - a else ("regs" if st == 0 else "STRUCT")
    if verbose is None:
        print("R%-3d want[%3d:%3d] rows %3d exact %3d regs %3d struct %2d  %-6s %s" % (r, a, b, b - a, ex, rg, st, state, w[a][:40]))
    elif verbose == r:
        for k in range(a, b):
            gg = g[pair[k]] if k in pair else "(struct)"
            mark = " " if k in pair and strip_tgt(w[k]) == strip_tgt(gg) else "*"
            print("%s %3d %-34s %s" % (mark, k, w[k][:34], gg[:40]))
if verbose is None:
    print("total rows %d exact %d regs %d struct %d" % tuple(tot))
