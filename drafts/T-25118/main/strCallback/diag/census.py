"""Private T-25118 diagnostic: which Umov copies are copy-state sensitive (normal vs inverted gadget)."""
import os, re, subprocess, sys, glob
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, "tools")
import cc
OUT = "build/T-25118/diag/census"
files = sorted(set(sys.argv[1:]))

def funcs(obj):
    d = subprocess.run(["mips-linux-gnu-objdump", "-d", "-z", "-M", "no-aliases", "-r", obj], stdout=subprocess.PIPE, text=True).stdout
    res = {}; cur = None
    for l in d.splitlines():
        m = re.match(r"[0-9a-f]+ <(\w+)>:", l)
        if m: cur = m.group(1); res[cur] = []; continue
        if cur and l.strip(): res[cur].append(re.sub(r"^\s*[0-9a-f]+:\s*", "", l))
    return res

def one(src):
    tag = src.replace("/", "_")[:-2]
    tc = cc.toolchain_of_source(src)
    r = {}
    for mode in ("", os.environ.get("CENSUS_MODE", "INV")):
        env = dict(os.environ, DIAG_STATE=mode, DIAG_LOG="%s/%s.%s.log" % (OUT, tag, mode or "N"))
        if os.path.exists(env["DIAG_LOG"]): os.remove(env["DIAG_LOG"])
        obj = "%s/%s.%s.o" % (OUT, tag, mode or "N")
        p = subprocess.run(["python3", os.environ.get("CC_PATH", "build/T-25118/diag/tools/cc.py"), src, obj] + tc, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        if p.returncode: return "%s FAIL %s" % (src, p.stdout[-200:])
        r[mode] = funcs(obj)
    names = list(r[""])
    ch = [f for f in r[""] if r[""][f] != r[list(r)[1]].get(f)]
    return "%s changed=%s order=%s" % (src, ",".join(ch), ",".join(names))

with ThreadPoolExecutor(2) as ex:
    for line in ex.map(one, files):
        print(line, flush=True)
