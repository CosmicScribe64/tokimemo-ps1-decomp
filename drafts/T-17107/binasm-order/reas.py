# re-run as1 on a permuted binasm: reas.py IN.G OUT.o idxA idxB [...pairs]  (swap records by proc-relative index list)
import sys, os, subprocess
sys.path.insert(0, "/work/tools")
import frame_pass as fp
src, out = sys.argv[1], sys.argv[2]
recs = fp.parse(open(src, "rb").read())
ops = sys.argv[3:]
# ops like "mv:A:B" move record A to before B (absolute indices)
for op in ops:
    if op.startswith("sym:"):
        _, i, sym, imm = op.split(":"); r = recs[int(i)][0]; r.w[0] = int(sym); r.set_imm(int(imm)); continue
    if op.startswith("del:"):
        recs.pop(int(op.split(":")[1])); continue
    if op.startswith("loc:"):
        _, src_i, pos, line = op.split(":")
        r0 = recs[int(src_i)][0]; assert r0.instr == 28, r0.instr
        r = fp.Rec(list(r0.w)); r.w[3] = int(line)
        recs.insert(int(pos), (r, b"")); continue
    if op.startswith("perm:"):
        _, a, b, order = op.split(":"); a = int(a); b = int(b)
        sl = [recs[int(x)] for x in order.split(",")]
        assert len(sl) == b - a
        recs[a:b] = sl; continue
    kind, a, b = op.split(":"); a = int(a); b = int(b)
    if kind == "sw": recs[a], recs[b] = recs[b], recs[a]
    elif kind == "mv":
        r = recs.pop(a); recs.insert(b if b < a else b - 1, r)
open(out + ".G", "wb").write(fp.serialize(recs))
rc = subprocess.call(["python3", "/work/tools/frame_pass.py", "--as1", "/opt/ido/5.3/as1", "-elf", "-G", "0", "-p0", "-EL", "-g0", "-O2", out + ".G", "-o", out, "-t", src + ".T"])
print("rc", rc)
