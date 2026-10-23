"""usage: python3 trall.py TRACE.log [BB...]  (traced uopt log of uopt_eval --trace)
T-25606 r1-structure: every range of func_800476C0 with colour, priority, blocks and units; with BB
arguments only the ranges that have a unit in one of those blocks."""
import re, sys
want = set(int(x) for x in sys.argv[2:])
cur = None; rng = None; R = {}; C = {}; order = []
for l in open(sys.argv[1]):
    if l.startswith("R9PROC"): cur = l.split()[1]
    if cur != "func_800476C0": continue
    if l.startswith("R9SAVE"):
        m = re.search(r"bit=(\d+) lr=(\d+) blocks=(\d+) priority=(\S+)", l)
        rng = (m.group(1), m.group(2)); R[rng] = [m.group(3), float(m.group(4)), []]; order.append(rng)
    elif l.startswith("R9UNIT") and rng:
        m = re.search(r"bb=(\d+) freq=(\d+) loads=(\d+) stores=(\d+)", l)
        R[rng][2].append((int(m.group(1)), m.group(2), m.group(3), m.group(4)))
    elif l.startswith("R9COST"):
        m = re.search(r"bit=(\d+) lr=(\d+) cost=\S+ reg=(\d+)", l); C[(m.group(1), m.group(2))] = int(m.group(3))
names = {0: "-", 1: "v0", 2: "v1", 3: "a0", 4: "a1", 5: "a2", 6: "a3", 7: "t0", 8: "t1", 9: "t2", 10: "t3",
         11: "t4", 12: "t5", 13: "t6"}
names.update({14 + i: "s%d" % i for i in range(8)}); names[22] = "fp"
for k in order:
    v = R[k]
    if want and not any(u[0] in want for u in v[2]): continue
    print("bit %-4s lr %-4s %-3s prio %8.3f D %-3s %s" % (k[0], k[1], names.get(C.get(k), "?"), v[1], v[0],
          " ".join("%d:%s/%s%s" % (u[0], u[1], u[2], "s" if u[3] != "0" else "") for u in v[2][:12])))
