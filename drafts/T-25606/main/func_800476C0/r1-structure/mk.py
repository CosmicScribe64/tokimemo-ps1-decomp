"""usage: python3 mk.py BASE.c OUTDIR SPEC.py  -- SPEC.py defines V = {name: [(old, new, count), ...]}; count 0 = all.
Writes OUTDIR/name.c (T-25606 r1-structure helper)."""
import sys
base = open(sys.argv[1]).read()
ns = {}
exec(open(sys.argv[3]).read(), ns)
for name, reps in ns["V"].items():
    t = base
    for rep in reps:
        old, new = rep[0], rep[1]
        cnt = rep[2] if len(rep) > 2 else 1
        if old not in t:
            sys.exit("%s: not found: %r" % (name, old))
        t = t.replace(old, new) if cnt == 0 else t.replace(old, new, cnt)
    open("%s/%s.c" % (sys.argv[2], name), "w").write(t)
    print("%s/%s.c" % (sys.argv[2], name))
