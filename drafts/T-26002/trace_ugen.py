#!/usr/bin/env python3
"""Scratch ugen shim: dump the input ucode, then run the repo's ugen_pass."""
import os, sys, hashlib, shutil
sys.path.insert(0, "/work/tools")
import ugen_pass as up
argv = sys.argv[3:]
inp = None
i = 0
while i < len(argv):
    if argv[i] == "-o": i += 2
    elif argv[i] in up.VALUED: i += 2
    elif argv[i].startswith("-"): i += 1
    else:
        if inp is None: inp = i
        i += 1
data = open(argv[inp], "rb").read()
d = "/work/build/ugen_trace"
os.makedirs(d, exist_ok=True)
h = hashlib.sha1(data).hexdigest()[:12]
open(os.path.join(d, h + ".O"), "wb").write(data)
src = ""
pid = os.getpid()
for _ in range(12):
    try:
        st = open("/proc/%d/stat" % pid).read()
        ppid = int(st.rsplit(")", 1)[1].split()[1])
        cl = open("/proc/%d/cmdline" % pid, "rb").read().split(b"\0")
    except Exception:
        break
    cl = [c.decode("latin1") for c in cl]
    if any(c.endswith("tools/cc.py") for c in cl) and "ido" in cl:
        src = " ".join(c for c in cl if c.endswith(".c")); break
    pid = ppid
open(os.path.join(d, h + ".cwd"), "w").write(src)
up.main(sys.argv)
