# graph.py TRACE LO HI: print uopt flow-graph nodes LO..HI of func_80142C9C (pred/succ) from a regdiff draft.o.trace (T-25403 diagnostic)
import re, sys
lines=open(sys.argv[1]).read().splitlines()
lo,hi=int(sys.argv[2]),int(sys.argv[3])
start=[i for i,l in enumerate(lines) if 'flow graph for func_80142C9C' in l][0]
cur=None; g={}
for l in lines[start+1:]:
    if 'flow graph for' in l: break
    m=re.match(r'\s+(\d+)\s+(\d+)\s*$',l)
    if m: cur=int(m.group(1)); g[cur]={'lab':int(m.group(2)),'pre':[],'suc':[]}; continue
    m=re.match(r'(pre|suc)::::\s+(\d+)\s+(\d+)',l)
    if m and cur is not None: g[cur][m.group(1)].append(int(m.group(2)))
for b in range(lo,hi):
    if b in g: print(b, 'lab',g[b]['lab'],'pre',g[b]['pre'][:6],'suc',g[b]['suc'])
