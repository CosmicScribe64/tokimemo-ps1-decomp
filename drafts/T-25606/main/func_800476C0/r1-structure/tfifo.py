"""usage: python3 tfifo.py DUMP.txt OUTPREFIX  (DUMP from gdump.py; OUTPREFIX.got/.want from score.py, same body)
T-25606 r1-structure: maps every ugen temporary def of our binasm to the original's register (binasm def ->
our as1 output by dest register and order -> the original by the shape alignment), then replays the
first-in first-out model with the original's registers and prints where the original leaves the model."""
import sys, re, difflib, os
sys.argv += []
dump, pre = sys.argv[1:3]
T = {8: "t0", 9: "t1", 10: "t2", 11: "t3", 12: "t4", 13: "t5", 14: "t6", 15: "t7", 24: "t8", 25: "t9"}
NAME2 = {v: k for k, v in T.items()}
EXCL = {8, 9}
NODEF = {"sw", "sh", "sb", "b", "j", "jal"}
ins = []
for k, l in enumerate(open(dump).read().split("\n")):
    if l.startswith("L"):
        ins.append(("LABEL", None, [], k)); continue
    m = re.match(r"^(\w+)\s+(\S+) (\S+) (\S+) (-?\d+)$", l)
    if not m: continue
    op, r1, r2, r3, imm = m.groups()
    rr = lambda r: None if r == "-" else int(r[1:])
    r1, r2, r3 = rr(r1), rr(r2), rr(r3)
    if op in NODEF or op.startswith("b"):
        ins.append((op, None, [r for r in (r1, r2, r3) if r is not None], k))
    else:
        ins.append((op, r1, [r for r in (r2, r3) if r is not None], k))
def load(p):
    return [re.sub(r"^[0-9a-f]{8} ", "", l) for l in open(p).read().split("\n") if l.strip() and not l.startswith("R_MIPS")]
def norm(l):
    l = re.sub(r"\b(zero|at|v0|v1|a0|a1|a2|a3|t[0-9]|s[0-8]|fp|sp|ra)\b", "R", l)
    l = re.sub(r"<[^>]*>", "<>", l)
    return re.sub(r"-?\b0x[0-9a-f]+\b|-?\b\d+\b", "N", l)
w = load(pre + ".want"); g = load(pre + ".got")
pair = {}
for op, i1, i2, j1, j2 in difflib.SequenceMatcher(None, [norm(x) for x in w], [norm(x) for x in g], autojunk=False).get_opcodes():
    if op == "equal":
        for a, b in zip(range(i1, i2), range(j1, j2)): pair[b] = a
STORE = ("sw", "sh", "sb", "b", "j", "jal", "jr", "beq", "bne", "bnez", "beqz", "bltz", "bgez", "blez", "bgtz", "nop", "break")
def gdest(l):
    p = l.replace(",", " ").split()
    if not p or p[0] in STORE: return None
    return p[1] if len(p) > 1 else None
used = set(); gpos = 0
dmap = {}   # ins index -> got index
for n, (op, d, srcs, k) in enumerate(ins):
    if d not in T or d in EXCL: continue
    lo, hi = max(0, gpos - 12), min(len(g), gpos + 30)
    cand = [j for j in range(lo, hi) if j not in used and gdest(g[j]) == T[d]]
    if not cand: continue
    RN = {0: "zero", 1: "at", 2: "v0", 3: "v1", 4: "a0", 5: "a1", 6: "a2", 7: "a3", 16: "s0", 17: "s1", 18: "s2",
          19: "s3", 20: "s4", 21: "s5", 22: "s6", 23: "s7", 29: "sp", 30: "s8", 31: "ra"}
    RN.update(T)
    rawl = open(dump).read().split("\n")[k]
    mm = re.match(r"^(\w+)\s+(\S+) (\S+) (\S+) (-?\d+)$", rawl)
    imm = int(mm.group(5))
    if op in ("lw", "lh", "lhu", "lbu", "lb") and srcs and srcs[0] != 29:
        key = "%s %s,%d(%s)" % (op, T[d], imm, RN.get(srcs[0], "?"))
        exact = [j for j in cand if g[j].startswith(key)]
        if exact: cand = exact
    elif op in ("lw", "lh", "lhu", "lbu", "lb") and srcs and srcs[0] == 29:
        exact = [j for j in cand if g[j].startswith(op + " ") and g[j].endswith("(sp)")]
        exact2 = [j for j in exact if j > gpos - 3]
        if exact2: cand = exact2
    elif op == "op3":
        exact = [j for j in cand if g[j].startswith("and")]
        if exact: cand = exact
    elif op == "op56":
        exact = [j for j in cand if g[j].startswith("sll")]
        if exact: cand = exact
    # prefer the last instruction of an expansion: take the first candidate, then extend over same-dest neighbours
    j = cand[0]
    used.add(j); dmap[n] = j; gpos = j
# replay with the original's registers
free = [r for r in [14, 15, 24, 25, 8, 9, 10, 11, 12, 13] if r not in EXCL]
cur = {}     # our reg -> original reg for the live value
live = set()
bad = 0
for n, (op, d, srcs, k) in enumerate(ins):
    if op == "LABEL": continue
    fr = []
    for s in srcs:
        if s not in T or s in EXCL or s in fr: continue
        usedlater = False
        for op2, d2, s2, _ in ins[n + 1:]:
            if op2 == "LABEL": break
            if s in s2: usedlater = True; break
            if d2 == s: break
        if not usedlater and s in cur:
            fr.append(s)
    for s in fr:
        o = cur.pop(s)
        if o in live: live.discard(o); free.append(o)
    if d in T and d not in EXCL:
        if d in cur and cur[d] in live:
            continue
        if n not in dmap or dmap[n] not in pair:
            print("line %d %s %s: unmapped" % (k + 1, op, T[d])); cur[d] = None; continue
        wl = w[pair[dmap[n]]]
        o = NAME2.get(gdest(wl))
        head = free[0] if free else None
        mark = "" if o == head else "  <-- original %s, model head %s, free %s" % (T.get(o), T.get(head), [T[x] for x in free[:5]])
        if mark: bad += 1
        if mark or "-v" in sys.argv:
            print("line %4d %-5s ours %s orig %s | %s%s" % (k + 1, op, T[d], T.get(o), wl[:32], mark))
        if o in free: free.remove(o)
        if o is not None: live.add(o)
        cur[d] = o
print("departures", bad)
