"""bbmap.py BIT [LR]: units of a range with source lines (needs ucode.txt from udump.py of the same trace)."""
import sys, re
D='build/scratch/regdiff/func_8007132C/'
tr=open(D+'draft.o.trace').read().split('\n')
i=next(k for k,l in enumerate(tr) if l.strip()=='flow graph for func_8007132C:')
blk={}; order=[]
for l in tr[i+1:]:
    m=re.match(r'^\s+(\d+)\s+(\d+)\s*$', l)
    if m:
        b=int(m.group(1)); blk[b]=int(m.group(2)); order.append(b); continue
    if l.startswith('pre::') or l.startswith('suc::'): continue
    break
# label -> first loc line after it
uc=[l.split() for l in open('build/t25125/ucode.txt')]
lab2line={}; pend=[]
for r in uc:
    if r[1]=='lab': pend.append(int(r[3],16))
    elif r[1]=='loc':
        for p in pend: lab2line[p]=int(r[3],16)
        pend=[]
src=open(D+'draft.c').read().split('\n')
start=next(k for k,l in enumerate(src) if l.startswith('void func_8007132C(void) {'))+1
# assign each block a line: labeled -> its label line; unlabeled -> previous block line (marked ~)
line={}; last=None
for b in order:
    if blk[b] and blk[b] in lab2line: last=lab2line[blk[b]]; line[b]=str(last-start)
    else: line[b]='~'+(str(last-start) if last else '?')
want=sys.argv[1]; lr=sys.argv[2] if len(sys.argv)>2 else None
log=open(D+'draft.log').read().split('\n')
on=False; cur=None
for l in log:
    if l.startswith('R9PROC'): on = l.split()[1]=='func_8007132C'; continue
    if not on: continue
    m=re.match(r'R9SAVE bit=(\d+) lr=(\d+)', l)
    if m: cur=(m.group(1),m.group(2)); 
    if cur and cur[0]==want and (lr is None or cur[1]==lr):
        if l.startswith('R9SAVE'): print(l)
        elif l.startswith('R9UNIT'):
            b=int(re.search(r'bb=(\d+)',l).group(1))
            print('   ', l[7:], ' line', line.get(b,'?'), (src[start+int(line[b].strip('~'))-1].strip()[:70] if line.get(b,'?').strip('~').lstrip('-').isdigit() else ''))
