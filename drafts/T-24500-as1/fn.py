"""fn.py UNIT.c XTR.log FUNC [all] : as1 global-scheduler trace lines (weights, visits, moves, XBB) of FUNC."""
import re, sys
unit, log, func = sys.argv[1:4]
full = len(sys.argv) > 4
src = open(unit, encoding="latin-1").read().split("\n")
# function line range in unit.c (1-based): from its definition line to the next line starting with '}'
start = next(i for i, l in enumerate(src, 1) if re.match(r"^[\w ]*\b%s\(.*\)\s*\{?\s*$" % func, l))
end = next(i for i in range(start, len(src) + 1) if src[i - 1].startswith("}"))
cur = None; out = []; weights = []
for l in open(log, encoding="latin-1"):
    m = re.search(r"(?:lineno |Line )(\d+)", l)
    if m: cur = int(m.group(1))
    inside = cur is not None and start <= cur <= end
    if "\tbb: " in l and "weight" in l:
        if inside: weights.append(l.strip().replace("bb: ", "b").replace("\tweight: ", "w"))
        continue
    if not inside: continue
    if re.search(r"current bb|MOVETO|MOVEFROM|XBB|dominated by", l):
        if not full and "XBBDETAIL" in l and ("movable=1" in l or "graph_reject=0" in l): continue
        out.append(l.rstrip().split("XBB", 1)[-1] if "XBB" in l else l.rstrip())
print("lines %d-%d" % (start, end))
print(" ".join(weights))
print("\n".join(("XBB" + o) if o.startswith(("SCAN", "DETAIL", "RESULT", "PATH", "RELOC")) else o for o in out))
