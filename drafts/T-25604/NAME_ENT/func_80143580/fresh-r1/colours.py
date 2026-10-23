"""T-25604 scratch helper: key colours of a compiled func_80143580 (objdump text).

    python3 colours.py X.dis

Prints the registers of the selector loads (lbu of D_800E6280+0x110D), the fade loads
(lbu of D_801220D0+0x12 / +0x36), the 0x80000000 constant (lui 0x8000), the 0x01000000 constant
(lui 0x100) and the high part of D_801220D0 (lui 0x8012 used as a load base). Target (Best):
selector t0, fades a1 (spr0) and t2 (spr1), 0x80000000 t1, 0x01000000 ra, high part a0."""
import re, sys, collections
c = collections.defaultdict(collections.Counter)
lines = open(sys.argv[1]).read().splitlines()
for i, l in enumerate(lines):
    m = re.match(r"\s+[0-9a-f]+:\s+[0-9a-f]{8}\s+(\S+)\s*([^<]*)", l)
    if not m: continue
    op, a = m.group(1), [x.strip() for x in m.group(2).split(",")]
    if op == "lbu" and a[1].startswith("4365("): c["selector"][a[0]] += 1
    elif op == "lbu" and re.match(r"(18|54)\(", a[1]): c["fade spr0" if a[1].startswith("18") else "fade spr1"][a[0]] += 1
    elif op == "lui" and a[1] == "0x8000": c["0x80000000"][a[0]] += 1
    elif op == "lui" and a[1] == "0x100": c["0x01000000"][a[0]] += 1
for k in ("selector", "fade spr0", "fade spr1", "0x80000000", "0x01000000"):
    print(f"{k}: " + " ".join(f"{r}x{n}" for r, n in c[k].most_common()), end=" | ")
print()
