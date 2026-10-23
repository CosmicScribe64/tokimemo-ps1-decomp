"""Scratch T-25110: compile a guarded TU and save the ucode uopt hands to ugen (argv: guarded.c, unused, out.O)."""
import sys, os, tempfile
sys.path.insert(0, "tools")
import cc, cvt_pass as cp
src, fn, dump = sys.argv[1], sys.argv[2], sys.argv[3]
os.environ["UDUMP"] = dump
# shim: the ugen entry gets our wrapper; it receives --ugen REAL args...
w = os.path.join(tempfile.mkdtemp(), "ugen_wrap.py")
open(w, "w").write('''import sys, shutil, os, subprocess
args = sys.argv[1:]
open(os.environ["UDUMP"] + ".args", "w").write(repr(args))
real = args[1]; rest = args[2:]
skip = False
for k, a in enumerate(rest):
    if a == "-o" or a.startswith("-") and k + 1 < len(rest) and False: continue
for k, a in enumerate(rest):
    if not a.startswith("-") and (k == 0 or rest[k-1] not in ("-o", "-G", "-Olimit", "-t", "-temp")) and os.path.isfile(a):
        shutil.copy(a, os.environ["UDUMP"]); break
sys.exit(subprocess.call(["python3", "/work/tools/ugen_pass.py"] + args))
''')
cc.SHIMS["ugen"] = w
out = dump + ".o"
cc.compile_ido(src, out, "5.3", [])
insns = cp.parse(open(dump, "rb").read())
on = False
for i, r in enumerate(insns):
    name = cp.OPS[r.opc] if r.opc < len(cp.OPS) else "op%d" % r.opc
    if name in ("ent",):
        on = False
    if name in ("rlod", "rstr", "lod", "str") :
        mt, blk, off, ln = r.location()
        if on and mt != 0:
            print(i, name, "mt=%d blk=%d off=%d(0x%x) len=%d" % (mt, blk, off, off, ln))
    if name == "ent":
        on = True
