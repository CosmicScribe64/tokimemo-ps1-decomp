"""T-25608 scratch: list the procedures where the current ugen_pass.py lock changes a mov state, with the tokens
between the last istr before the lock and the locking jump.  census_locks.py LOG..."""
import sys
sys.path.insert(0, __import__("os").path.dirname(__file__))
from census_eval import states
for path in sys.argv[1:]:
    for n, line in enumerate(open(path)):
        p = line.split()
        if len(p) < 2:
            continue
        unit, toks = (p[0] if p[0] != "?" else "%s:%d" % (path, n + 1)), p[2:]
        if states(toks, "R0") == states(toks, "NONE"):
            continue
        seen = False
        for i, t in enumerate(toks):
            if t == "U":
                seen, lu = True, i
            if t[0] == "R" and seen:
                break
        nm = sum(1 for t in toks[:i] if t == "M")
        print("%s @%s movs_before=%d R0=%s none=%s | %s || after: %s" % (unit.replace("build/", "").replace(".o.sjis.c", ""), p[1], nm,
              states(toks, "R0"), states(toks, "NONE"), " ".join(toks[lu:i + 1]), " ".join(toks[i + 1:i + 9])))
