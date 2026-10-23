"""Scratch T-25110: list memory refs (mt, off) by op for the last procedure in a ucode dump."""
import sys, collections
sys.path.insert(0, "tools")
import cvt_pass as cp
ins = cp.parse(open(sys.argv[1], "rb").read())
ents = [i for i, r in enumerate(ins) if cp.OPS[r.opc] == "ent"]
start = ents[int(sys.argv[2]) if len(sys.argv) > 2 else -1]
cnt = collections.Counter()
seq = []
for i in range(start, len(ins)):
    r = ins[i]; n = cp.OPS[r.opc]
    if n == "end": break
    if n in ("lod", "str", "rlod", "rstr", "lda"):
        try:
            mt, blk, off, ln = r.location()
        except Exception:
            continue
        if mt in (1, 2, 4) or n in ("rlod", "rstr"):
            cnt[(n, mt, off, ln)] += 1
            seq.append((i, n, mt, off))
FRAME = int(sys.argv[3], 0) if len(sys.argv) > 3 else 0x110
def s(off):
    o = off - (1 << 32) if off >= 1 << 31 else off
    return o + FRAME
rows = collections.defaultdict(collections.Counter)
for k, v in cnt.items():
    n, mt, off, ln = k
    if mt == 1:
        rows[(s(off), ln)][n] += v
for (sp, ln) in sorted(rows, reverse=True):
    print("sp %3d (0x%02X) len %d: %s" % (sp, sp, ln, dict(rows[(sp, ln)])))
