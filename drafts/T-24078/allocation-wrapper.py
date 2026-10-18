#!/usr/bin/env python3
from pathlib import Path
import os,sys,json,subprocess,hashlib
sys.path.insert(0,'/work/tools')
import cvt_pass as cv
args=sys.argv[1:];files=[];i=0
while i<len(args):
    if args[i] in cv.VALUED:i+=2
    elif args[i].startswith('-'):i+=1
    else:files.append(args[i]);i+=1
if len(files)<2:raise SystemExit('missing actual uopt files')
a=cv.parse(Path(files[0]).read_bytes())
start=next(i for i,x in enumerate(a) if x.opc==cv.OP['ent'] and x.words[1]==56)
end=next(i for i in range(start+1,len(a)) if a[i].opc==cv.OP['end'])
hits=[]
for j in range(start,min(start+100,end)-1):
    x,y=a[j:j+2]
    if x.opc==cv.OP['ldc'] and len(x.words)>4 and x.words[4]==320 and y.opc==cv.OP['str']:
        hits.append(y.location())
if len(hits)!=1:raise SystemExit('actual page320 initial IR store ambiguous: '+repr(hits))
mt,block,off,length=hits[0]
if off>=2**31:off-=2**32
env=dict(os.environ,T24078_PAGE_OFFSET=str(off),T24078_PAGE_BLOCK=str(block),T24078_PAGE_MTYPE=str(mt))
root=Path('/work/build/etc24078/alloc618')
(root/'input-identity.json').write_text(json.dumps({'input_sha256':hashlib.sha256(Path(files[0]).read_bytes()).hexdigest(),'actual_page_store':{'mtype':mt,'block':block,'offset':off,'length':length}},indent=2)+'\n')
raise SystemExit(subprocess.call([str(root/'uopt-alloc')]+args,env=env))
