"""cap.py SRC CAPDIR [--asm]  : preprocess SRC (NON_MATCHING unless --asm) and compile with the capture shim."""
import os, sys, subprocess
sys.path.insert(0, "/work/tools")
import funcdiff_verify as fv
src, cap = sys.argv[1], sys.argv[2]
nm = "--asm" not in sys.argv
os.makedirs(cap, exist_ok=True)
pre = fv.preprocess_draft(src, nm)
u = os.path.join(cap, "unit.c")
open(u, "w").write(pre)
env = dict(os.environ, AS1CAP=cap)
subprocess.check_call(["python3", "/work/build/as1tr/cc_cap.py", u, os.path.join(cap, "unit.o"), "ido", "5.3"], env=env)
