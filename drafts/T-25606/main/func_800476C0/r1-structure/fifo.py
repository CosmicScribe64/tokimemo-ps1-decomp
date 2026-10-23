"""usage: python3 fifo.py DUMP.txt [--order rev] [-v]  (DUMP from gdump.py)
T-25606 r1-structure: replays ugen's temporary choices in a binasm dump against a first-in first-out
free-list model and prints every def where the register is not the model's head."""
import sys, re
T = {8: "t0", 9: "t1", 10: "t2", 11: "t3", 12: "t4", 13: "t5", 14: "t6", 15: "t7", 24: "t8", 25: "t9"}
NODEF = {"sw", "sh", "sb", "beq", "bne", "blt", "bge", "bgt", "ble", "bltu", "bgeu", "b", "j", "jal", "beqz", "bnez"}
lines = open(sys.argv[1]).read().split("\n")
ins = []
for k, l in enumerate(lines):
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
rev = "--order" in sys.argv and sys.argv[sys.argv.index("--order") + 1] == "rev"
import os
EXCL = [int(x) for x in os.environ.get("FIFO_EXCL", "8,9").split(",") if x]
init = [r for r in [14, 15, 24, 25, 8, 9, 10, 11, 12, 13] if r not in EXCL]
free = list(init)
live = set()
bad = 0
for n, (op, d, srcs, k) in enumerate(ins):
    if op == "LABEL":
        continue
    # free temps read here whose value is not read again before redefinition (scan to the next label)
    fr = []
    for s in (reversed(srcs) if rev else srcs):
        if s not in T or s in fr: continue
        used = False
        for op2, d2, s2, _ in ins[n + 1:]:
            if op2 == "LABEL": break
            if s in s2: used = True; break
            if d2 == s: break
        if not used and s in live:
            fr.append(s)
    for s in fr:
        live.discard(s); free.append(s)
    if d in T:
        if d in EXCL:
            continue
        if d in live:   # redefinition of a value still live (uopt range or move target)
            continue
        head = free[0] if free else None
        if "-v" in sys.argv:
            print("line %d: %s %s  free=%s" % (k + 1, op, T[d], [T[x] for x in free]))
        if head != d:
            bad += 1
            print("line %d: %s %s got %s model %s  free=%s" % (k + 1, op, T[d], T[d], T.get(head), [T[x] for x in free[:6]]))
        if d in free: free.remove(d)
        live.add(d)
print("mismatches", bad)
