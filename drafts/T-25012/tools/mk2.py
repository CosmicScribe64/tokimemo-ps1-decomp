# mk2.py real func body out [extra_include_line]  -> unit with guarded body; extra line inserted after the last #include at top
import sys,re
real,name,body,out=sys.argv[1:5]
extra=sys.argv[5] if len(sys.argv)>5 else ""
t=open(real).read()
m=re.search(r'INCLUDE_ASM\("[^"]*", %s\);\n'%name,t)
t=t[:m.start()]+"#ifdef NON_MATCHING\n"+open(body).read()+"\n#else\n"+m.group(0)+"#endif\n"+t[m.end():]
if extra:
    ls=t.split("\n"); last=max(i for i,l in enumerate(ls[:30]) if l.startswith("#include"))
    ls.insert(last+1,extra); t="\n".join(ls)
open(out,'w').write(t)
