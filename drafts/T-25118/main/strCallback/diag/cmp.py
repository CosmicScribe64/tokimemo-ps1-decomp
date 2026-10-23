import re,sys
root=''  # run from the repository root
tgt=[]
for l in open(root+'asm/nonmatchings/main/800563F0/strCallback.s'):
    m=re.match(r'\s*/\* \w+ \w+ (\w{8}) \*/',l)
    if m:
        w=m.group(1); tgt.append(int(w[6:8]+w[4:6]+w[2:4]+w[0:2],16))
b=[int(l.strip(),16) for l in open(sys.argv[1]) if l.strip()]
n=0
for i,(t,x) in enumerate(zip(tgt,b)):
    if t==x: continue
    op=x>>26
    if op==3 and (t>>26)==3: continue
    if (x&0xFFFF)==0 and (t&0xFFFF0000)==(x&0xFFFF0000): continue
    n+=1
    if n<=int(sys.argv[2] if len(sys.argv)>2 else 10): print(i,'%08x %08x'%(t,x))
print('rows',n,'len',len(tgt),len(b))
