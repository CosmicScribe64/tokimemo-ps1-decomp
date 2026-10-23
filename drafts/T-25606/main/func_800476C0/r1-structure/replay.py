#!/usr/bin/env python3
"""usage (docker, repo root): python3 replay.py PREFIX EDITS OUT.txt
T-25606 r1-structure experiment: runs frame_pass + the real IDO 5.3 as1 on a saved binasm PREFIX.G (+ .T)
from gdump.py after record edits, and writes func_800476C0's disassembly. EDITS is ';'-separated:
'del:N' deletes OCODE record N (index among the dumped lines), 'mv:N:M' moves line N before line M,
'set:N:field=value' sets reg1/reg2/reg3/imm of line N. Line numbers are those of PREFIX.txt."""
import sys, os, subprocess, tempfile, struct
sys.path.insert(0, "tools")
import frame_pass as fp, funcdiff as fd
prefix, edits, out = sys.argv[1:4]
recs = fp.parse(open(prefix + ".G", "rb").read())
start = next(k for k, (r, _) in enumerate(recs) if r.instr == fp.I_IENT)
idx = list(range(start, len(recs)))          # dumped line n -> record index start + n
order = list(range(len(recs)))
dels = set()
for e in [x for x in edits.split(";") if x]:
    p = e.split(":")
    if p[0] == "del":
        dels.add(start + int(p[1]))
    elif p[0] == "mv":
        a, b = start + int(p[1]), start + int(p[2])
        order.remove(a); order.insert(order.index(b), a)
    elif p[0] == "set":
        r = recs[start + int(p[1])][0]
        f, v = p[2].split("=")
        v = int(v)
        if f == "imm": r.w[3] = v & 0xFFFFFFFF
        elif f == "op": r.w[1] = (r.w[1] & ~(0x1FF << 1)) | (v << 1)
        elif f == "reg1": r.w[2] = (r.w[2] & ~(0x7F << 25)) | (v << 25)
        elif f == "reg2": r.w[2] = (r.w[2] & ~(0x7F << 18)) | (v << 18)
        elif f == "reg3": r.w[2] = (r.w[2] & ~(0x7F << 7)) | (v << 7)
new = [recs[k] for k in order if k not in dels]
data = fp.transform(fp.serialize(new))
d = tempfile.mkdtemp()
g = os.path.join(d, "in.G"); open(g, "wb").write(data)
o = os.path.join(d, "out.o")
r = subprocess.run(["/opt/ido/5.3/as1", "-elf", "-G", "0", "-p0", "-EL", "-g0", "-O2", g, "-o", o, "-t", prefix + ".T"],
                   capture_output=True, text=True)
if r.returncode:
    sys.exit(r.stdout + r.stderr)
lines = fd.functions(o, ["func_800476C0"])["func_800476C0"]
open(out, "w").write("\n".join(lines) + "\n")
print(len(lines))
