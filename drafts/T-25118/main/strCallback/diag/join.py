import re,sys,collections
OUT='build/T-25118/diag/census'
rows=[]
for line in open(OUT+'/small.txt'):
    m=re.match(r'(\S+) changed=(\S*) order=(\S*)',line.strip())
    if not m: continue
    src,ch,order=m.groups(); ch=set(filter(None,ch.split(','))); order=order.split(',')
    tag=src.replace('/','_')[:-2]
    try: log=open('%s/%s.N.log'%(OUT,tag)).read().splitlines()
    except FileNotFoundError: continue
    for l in log:
        mm=re.match(r'P(\d+) (\d+) (\d+) size=(\d+) w=(\S+) want_f=(\d) last=(\S+) ctx=(.*)',l)
        p,st,k,size,w,wf,last,ctx=mm.groups()
        if int(size)>32: continue
        fn=order[int(p)] if int(p)<len(order) else '?'
        rows.append((src,fn,fn in ch,int(size),w,int(wf),last,ctx))
c=collections.Counter((r[2],r[5],r[6]) for r in rows)
for k,v in sorted(c.items()): print(k,v)
if len(sys.argv)>1:
    for r in rows:
        if eval(sys.argv[1]): print(r[1],r[2],r[3],r[4],'F' if r[5] else 'B',r[6],'|',r[7])
