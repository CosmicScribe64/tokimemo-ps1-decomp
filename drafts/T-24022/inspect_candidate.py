"""Inspect register-masked structural alignment of an experiment object."""
import sys,difflib
from pathlib import Path
sys.path.insert(0,'tools')
import funcdiff as fd
unit,name,obj=sys.argv[1:4]
c=fd.locate(name,unit)
e,problem=fd.expected_object(c)
want=fd.resolved_functions(e,[name],c)[name]
got=fd.resolved_functions(obj,[name],c)[name]
def key(r): return r[0] if isinstance(r,tuple) else r
def disp(r): return r[1] if isinstance(r,tuple) else r
def mask(r):
    k=key(r);w=int(k[:8],16);op=w>>26
    if op==0:w &= ~(31<<21|31<<16|31<<11)
    elif op==1:w &= ~(31<<21|65535)
    elif op in (4,5,6,7):w &= ~(31<<21|31<<16|65535)
    elif op not in (2,3):w &= ~(31<<21|31<<16)
    return '%08x%s'%(w,k[8:])
W=list(map(mask,want));G=list(map(mask,got))
print('instructions',len(want),len(got),'aligned cost',fd.diff_score(want,got),'printed rows',len(fd.diff_lines(want,got)))
if len(sys.argv)>4:
    lo,hi=map(int,sys.argv[4:6])
    for i in range(lo,hi):print(i,disp(want[i]),'|',disp(got[i]))
else:
    for tag,a,b,x,y in difflib.SequenceMatcher(None,W,G,autojunk=False).get_opcodes():
        if tag=='equal':continue
        print('hunk',a,b,x,y)
        for i in range(a,b):print('-',i,disp(want[i]))
        for i in range(x,y):print('+',i,disp(got[i]))
