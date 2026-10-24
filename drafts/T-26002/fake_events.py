import sys,json,glob
sys.path.insert(0,"/work/tools")
import ugen_pass as up
OP=up.OP
r=json.load(open('/work/build/census.json'))
for cwd in sorted(glob.glob("/work/build/ugen_trace/*.cwd")):
    src=open(cwd).read().strip()
    if not src: continue
    lines=open("/work/"+src,encoding="utf-8",errors="replace").read().split("\n")
    insns=up.parse_ucode(open(cwd[:-4]+".O","rb").read())
    for s,e in up._procedures(insns):
        if not any(insns[k].opc==OP["mov"] for k in range(s,e)): continue
        name=None
        for k in range(s,min(s+4,e)):
            if insns[k].opc==OP["comm"]: name=insns[k].extra.split(b"\0")[0].decode();break
        x=[y for y in r if y['name']==name and y['src']==src][0]
        if not x['fakes'] : continue
        # events with lines
        ev=[]; cur=0
        for k in range(s+1,e):
            opc=insns[k].opc
            if opc==OP["loc"]: cur=insns[k].words[1]
            if opc in (OP["istr"],OP["istv"]): ev.append((k,'i',cur-1))
            elif opc in (OP["fjp"],OP["tjp"]) and up._is_register_compare_jump(insns,s,k): ev.append((k,'c',cur-1))
        def states(skip):
            st="B"; out=[]; evk={k:(t,l) for k,t,l in ev}
            for k in range(s+1,e):
                opc=insns[k].opc
                if k in evk and not skip(evk[k][1]): st="F" if evk[k][0]=='i' else "B"
                elif opc==OP["mov"]: out.append(st)
            return ''.join(out)
        base=states(lambda l:False)
        for f in x['fakes']:
            z=lambda l,f=f: f<=l<=f+7
            inz=[(t,l) for k,t,l in ev if z(l)]
            if not inz: continue
            alt=states(z)
            txt=lines[f-1].strip()[:110]
            print(("SAME " if alt==base else "DIFF ")+src.replace('src/','').replace('.c','')+" "+name+" "+f"{f}:"+txt+" ev="+''.join(t for t,l in inz)[:12]+" "+base[:12]+"->"+alt[:12])
