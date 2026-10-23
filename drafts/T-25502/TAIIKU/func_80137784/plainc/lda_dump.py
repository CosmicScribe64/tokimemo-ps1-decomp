import sys
sys.path.insert(0, "tools")
import cvt_pass as cp
ins = cp.parse(open(sys.argv[1], "rb").read())
names = {v: k for k, v in cp.OP.items()}
f = 0
show = set(sys.argv[2].split(",")) if len(sys.argv) > 2 else {"lda", "ixa", "equ", "lod", "inc", "dec", "add", "sub", "cvt", "ldc"}
for i, x in enumerate(ins):
    n = names.get(x.opc)
    if n == "ent": f += 1; print("f%d:" % f, end=" ")
    if n in show:
        if n == "lda": print("lda(b=%d len=%d off=%d a=%d,%d mt=%d)" % (x.words[1], x.words[2], x.words[3], x.words[4], x.words[5], x.mtype), end=" ")
        elif n in ("ixa", "inc", "dec", "ldc"): print("%s(%s)" % (n, ",".join(str(w) for w in x.words[1:])), end=" ")
        else: print(n, end=" ")
    if n == "ret": print()
