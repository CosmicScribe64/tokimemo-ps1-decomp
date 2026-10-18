"""usage: python3 drafts/T-23006/shape.py OUTPREFIX [LO HI]  (OUTPREFIX.got/.want written by try.py): opcode-shape hunks of want[LO:HI], registers and numbers masked."""
import sys, re, difflib
def load(p):
    out=[]
    for l in open(p).read().split("\n"):
        if not l.strip(): continue
        if re.match(r"^R_MIPS", l): continue
        out.append(l)
    return out
w = load(sys.argv[1]+".want"); g = load(sys.argv[1]+".got")
def norm(l):
    l = re.sub(r"^[0-9a-f]{8} ", "", l)
    l = re.sub(r"\b(zero|at|v0|v1|a0|a1|a2|a3|t[0-9]|s[0-8]|fp|sp|ra)\b", "R", l)
    l = re.sub(r"<[^>]*>", "<>", l)
    l = re.sub(r"-?\b0x[0-9a-f]+\b|-?\b\d+\b", "N", l)
    return l
nw=[norm(x) for x in w]; ng=[norm(x) for x in g]
sm=difflib.SequenceMatcher(None,nw,ng,autojunk=False)
blocks=[b for b in sm.get_opcodes() if b[0]!="equal"]
lo=int(sys.argv[2]) if len(sys.argv)>2 else 0
hi=int(sys.argv[3]) if len(sys.argv)>3 else 10**9
k=0
for op,i1,i2,j1,j2 in blocks:
    k+=1
    if not (lo<=i1<hi): continue
    print("@@ %s want[%d:%d] got[%d:%d]"%(op,i1,i2,j1,j2))
    for r in w[i1:i2]: print("-",re.sub(r"^[0-9a-f]{8} ","",r))
    for r in g[j1:j2]: print("+",re.sub(r"^[0-9a-f]{8} ","",r))
print("blocks",k, "want rows",len(w),"got rows",len(g))
