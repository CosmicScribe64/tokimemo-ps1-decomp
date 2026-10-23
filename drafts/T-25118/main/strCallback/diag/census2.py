"""Private T-25118 diagnostic: guarded C of unmatched copy functions under each copy state."""
import os, re, subprocess, sys
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, "tools")
import cc
OUT = "build/T-25118/diag/census2"; os.makedirs(OUT, exist_ok=True)
MODES = os.environ.get("MODES", ",INVSMALL").split(",")

def target(asm):
    t = []
    for l in open(asm):
        m = re.match(r'\s*/\* \w+ \w+ (\w{8}) \*/', l)
        if m:
            w = m.group(1); t.append(int(w[6:8] + w[4:6] + w[2:4] + w[0:2], 16))
    return t

def words(obj, fn):
    d = subprocess.run(["mips-linux-gnu-objdump", "-d", "-z", obj], stdout=subprocess.PIPE, text=True).stdout
    out = []; on = False
    for l in d.splitlines():
        if re.match(r"[0-9a-f]+ <%s>:" % fn, l): on = True; continue
        if on:
            if not l.strip() or re.match(r"[0-9a-f]+ <", l): break
            m = re.match(r"\s*[0-9a-f]+:\s+([0-9a-f]{8})", l)
            if m: out.append(int(m.group(1), 16))
    return out

def rows(t, b):
    n = abs(len(t) - len(b))
    for x, y in zip(t, b):
        if x == y: continue
        if (x >> 26) == 3 and (y >> 26) == 3: continue
        if (y & 0xFFFF) == 0 and (x & 0xFFFF0000) == (y & 0xFFFF0000): continue
        n += 1
    return n

def one(item):
    fn, src, asm = item
    text = open(src).read()
    if not re.search(r"#ifdef NON_MATCHING[^#]*?\b%s\s*\(" % fn, text, re.S):
        return "%s noguard" % fn
    tag = fn
    pre = "%s/%s.c" % (OUT, tag)
    subprocess.run(["gcc", "-E", "-P", "-Iinclude", "-D__sgi", "-DNON_MATCHING", "-D__attribute__(x)=",
                    "-DINCLUDE_ASM(F,N)=INCLUDE_ASM(F,N)", "-DINCLUDE_RODATA(F,N)=INCLUDE_RODATA(F,N)", src],
                   stdout=open(pre, "w"), check=True)
    t = target(asm); res = []
    for mode in MODES:
        obj = "%s/%s.%s.o" % (OUT, tag, mode or "N")
        env = dict(os.environ, DIAG_STATE=mode, DIAG_LOG="%s/%s.%s.log" % (OUT, tag, mode or "N"))
        if os.path.exists(env["DIAG_LOG"]): os.remove(env["DIAG_LOG"])
        p = subprocess.run(["python3", os.environ.get("CC_PATH", "build/T-25118/diag/tools/cc.py"), pre, obj] + cc.toolchain_of_source(src), env=env,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        if p.returncode: res.append("%s=FAIL" % (mode or "N")); continue
        b = words(obj, fn)
        res.append("%s=%d/%d" % (mode or "N", rows(t, b), len(b)))
    return "%s %s tgt=%d" % (fn, " ".join(res), len(t))

items = [l.split() for l in open(sys.argv[1])]
with ThreadPoolExecutor(2) as ex:
    for line in ex.map(one, items):
        print(line, flush=True)
