#!/usr/bin/env python3
"""as1 shim: frame_pass transform, capture .G/.T to $AS1CAP, run real as1 (+ trace binary)."""
import os, sys, shutil, subprocess
sys.path.insert(0, "/work/tools")
import frame_pass
real, argv = sys.argv[2], sys.argv[3:]
valued = ("-G", "-o", "-t", "-temp")
i, inp = 0, None
while i < len(argv):
    if argv[i] in valued: i += 2
    elif argv[i].startswith("-"): i += 1
    else: inp = i; i += 1
cap = os.environ["AS1CAP"]
os.makedirs(cap, exist_ok=True)
data = frame_pass.transform(open(argv[inp], "rb").read())
g = os.path.join(cap, "in.G")
open(g, "wb").write(data)
ti = argv.index("-t") + 1 if "-t" in argv else None
if ti: shutil.copy(argv[ti], os.path.join(cap, "in.T"))
args = list(argv); args[inp] = g
open(os.path.join(cap, "args.txt"), "w").write(" ".join(args))
rc = subprocess.call([real] + args)
# trace runs: stock -R -xbbdbg 8 and the instrumented binary
tr = os.environ.get("AS1TRACE")
for name, exe in (("bbdbg", real), ("xtr", tr)):
    if not exe: continue
    a = list(args)
    a[a.index("-o") + 1] = os.path.join(cap, name + ".o")
    if ti: 
        shutil.copy(os.path.join(cap, "in.T"), os.path.join(cap, name + ".T"))
        a[a.index("-t") + 1] = os.path.join(cap, name + ".T")
    with open(os.path.join(cap, name + ".log"), "w") as log:
        subprocess.call([exe] + a[:-1] + ["-R", "-xbbdbg", "8", a[-1]] if False else [exe, "-R", "-xbbdbg", "8"] + a, stdout=log, stderr=subprocess.STDOUT)
sys.exit(rc)
