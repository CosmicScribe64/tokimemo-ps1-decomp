"""Region checks for func_8006B0C8 drafts (scratch helper, T-25608; run from the repo root in Docker): compiles the guarded C of the
worktree source like sbs.py and reports length, inner SR order, month/undo preheader order."""
import sys, re, tempfile
from pathlib import Path
sys.path.insert(0, "/work/tools")
import funcdiff, funcdiff_verify as proof
name="func_8006B0C8"
c = funcdiff.locate(name, "main")
job = Path(tempfile.mkdtemp(prefix="rg-"))
b = proof.compile_guarded(c, job)
if "orig" in sys.argv:
    b, _ = funcdiff.expected_object(c, False, stale=[])
got = funcdiff.resolved_functions(b, [name], c)[name]
G=[r[1].split(";",1)[1].strip() if ";" in r[1] else r[1] for r in got]
out=[f"len={len(G)}"]
ring=["t6","t7","t8","t9"]
inner=[]
for i,x in enumerate(G):
    m=re.match(r"addiu (t\d),sp,(\d+)$",x)
    if m and not any(y.startswith("addu") and m.group(1) in y.split(",")[1:] for y in G[i+1:i+3]): m=None
    if m:
        t=m.group(1); lu=[y for y in G[max(0,i-3):i] if y.startswith("lui t")]
        if lu and t in ring:
            tb=lu[-1].split()[1].split(",")[0]
            inner.append("keys" if tb in ring and ring[(ring.index(t)+1)%4]==tb else "entry")
        else: inner.append("?")
out.append("inner="+"/".join(inner))
inc=[]
for i,x in enumerate(G):
    if x in ("addiu v1,v1,4","addiu a1,a1,4"):
        inc.append("k" if x.startswith("addiu v1") else "e")
out.append("inc="+"".join(inc[:4]))
pre=[]
for i in range(len(G)-6):
    ma=re.match(r"sll (\w+),(\w+),0x2$",G[i]); mb=re.match(r"sll (\w+),(\w+),0x2$",G[i+1])
    if not(ma and mb): continue
    ads=[re.match(r"addu (\w+),(\w+),(\w+)$",x) for x in G[i+2:i+6]]
    ads=[x for x in ads if x]
    da=[x for x in ads if ma.group(1) in x.groups()[1:]]; db=[x for x in ads if mb.group(1) in x.groups()[1:]]
    if not da or not db: continue
    ra,rb=da[0].group(1),db[0].group(1)
    rest=G[i+6:i+60]
    if not any(re.match(r"bne (\w+),(\w+),",x) and set(re.match(r"bne (\w+),(\w+),",x).groups())=={ra,rb} for x in rest): continue
    walk=None
    for x in rest:
        m=re.match(r"addiu (\w+),\1,",x)
        if m and m.group(1) in (ra,rb): walk=m.group(1); break
    if walk: pre.append("end" if walk==rb else "start")
out.append("monthundo="+"/".join(pre))
out.append("frame="+G[0])
out.append("rlda="+str(sum(1 for x in G if x.startswith("lui ra") or x.startswith("lui t1"))))
print(" ".join(out))
